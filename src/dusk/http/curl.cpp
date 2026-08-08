#include "http.hpp"

#include <curl/curl.h>

#include <algorithm>
#include <chrono>
#include <limits>
#include <mutex>
#include <string_view>
#include <utility>

namespace dusk::http {
namespace {

struct CurlHeaders {
    curl_slist* list = nullptr;

    ~CurlHeaders() {
        if (list != nullptr) {
            curl_slist_free_all(list);
        }
    }

    bool append(const Header& header) {
        curl_slist* next = curl_slist_append(list, (header.name + ": " + header.value).c_str());
        if (next == nullptr) {
            return false;
        }
        list = next;
        return true;
    }
};

struct CurlContext {
    Response response;
    size_t maxBodyBytes = 0;
    size_t headerBytes = 0;
    bool tooLarge = false;
    bool malformed = false;
};

void initialize_curl() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

std::string_view trim_header_value(std::string_view value) {
    while (!value.empty() && (value.front() == ' ' || value.front() == '\t')) {
        value.remove_prefix(1);
    }
    while (!value.empty() &&
           (value.back() == '\r' || value.back() == '\n' || value.back() == ' ' ||
               value.back() == '\t')) {
        value.remove_suffix(1);
    }
    return value;
}

bool byte_count(const size_t size, const size_t count, size_t& result) {
    if (count != 0 && size > std::numeric_limits<size_t>::max() / count) {
        return false;
    }
    result = size * count;
    return true;
}

size_t write_body(char* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* context = static_cast<CurlContext*>(userdata);
    size_t bytes = 0;
    if (!byte_count(size, nmemb, bytes) || bytes > context->maxBodyBytes ||
        context->response.body.size() > context->maxBodyBytes - bytes)
    {
        context->tooLarge = true;
        return 0;
    }
    context->response.body.append(ptr, bytes);
    return bytes;
}

size_t write_header(char* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* context = static_cast<CurlContext*>(userdata);
    size_t bytes = 0;
    if (!byte_count(size, nmemb, bytes)) {
        context->malformed = true;
        return 0;
    }
    const std::string_view line(ptr, bytes);
    if (line.starts_with("HTTP/")) {
        context->response.headers.clear();
        context->headerBytes = 0;
        return bytes;
    }
    const size_t colon = line.find(':');
    if (colon == std::string_view::npos) {
        return bytes;
    }
    const std::string_view name = line.substr(0, colon);
    const std::string_view value = trim_header_value(line.substr(colon + 1));
    if (context->response.headers.size() >= detail::kMaxHeaders ||
        name.size() > detail::kMaxHeaderNameBytes || value.size() > detail::kMaxHeaderValueBytes ||
        !detail::is_token(name) || value.find_first_of("\r\n") != std::string_view::npos ||
        value.find('\0') != std::string_view::npos ||
        context->headerBytes > detail::kMaxHeaderBytes - name.size() ||
        context->headerBytes + name.size() > detail::kMaxHeaderBytes - value.size())
    {
        context->malformed = true;
        return 0;
    }
    Header header{.name = std::string(name), .value = std::string(value)};
    context->headerBytes += header.name.size() + header.value.size();
    context->response.headers.push_back(std::move(header));
    return bytes;
}

Error map_curl_error(const CURLcode code, const CurlContext& context) {
    if (context.tooLarge) {
        return Error::TooLarge;
    }
    switch (code) {
    case CURLE_OK:
        return context.malformed ? Error::Network : Error::None;
    case CURLE_URL_MALFORMAT:
        return Error::InvalidUrl;
    case CURLE_UNSUPPORTED_PROTOCOL:
        return Error::UnsupportedScheme;
    case CURLE_OPERATION_TIMEDOUT:
        return Error::Timeout;
    default:
        return Error::Network;
    }
}

bool is_redirect(const long status) {
    return status == 301 || status == 302 || status == 303 || status == 307 || status == 308;
}

unsigned char ascii_lower(const unsigned char value) {
    return value >= 'A' && value <= 'Z' ? value + ('a' - 'A') : value;
}

bool ascii_equals(const std::string_view left, const std::string_view right) {
    if (left.size() != right.size()) {
        return false;
    }
    for (size_t index = 0; index < left.size(); ++index) {
        if (ascii_lower(static_cast<unsigned char>(left[index])) !=
            ascii_lower(static_cast<unsigned char>(right[index])))
        {
            return false;
        }
    }
    return true;
}

bool same_origin(const std::string_view first, const std::string_view second) {
    const auto origin = [](const std::string_view url) {
        const size_t authorityStart = std::string_view("https://").size();
        const size_t authorityEnd = url.find_first_of("/?", authorityStart);
        std::string authority(url.substr(authorityStart, authorityEnd - authorityStart));
        for (char& value : authority) {
            value = static_cast<char>(ascii_lower(static_cast<unsigned char>(value)));
        }
        if (authority.find(':') == std::string::npos) {
            authority += ":443";
        }
        return authority;
    };
    return origin(first) == origin(second);
}

bool header_name_equals(const std::string_view name, const std::string_view expected) {
    return ascii_equals(name, expected);
}

void apply_redirect_policy(Request& request, const long status, const bool crossOrigin) {
    const bool convertsPost = request.method == Method::Post &&
        (status == 301 || status == 302 || status == 303);
    if (convertsPost) {
        request.method = Method::Get;
        request.body.clear();
    }
    std::erase_if(request.headers, [&](const Header& header) {
        return (crossOrigin &&
                   (header_name_equals(header.name, "authorization") ||
                       header_name_equals(header.name, "cookie"))) ||
            (convertsPost &&
                header.name.size() >= 8 &&
                ascii_equals(std::string_view(header.name).substr(0, 8), "content-"));
    });
}

long remaining_timeout_ms(const std::chrono::steady_clock::time_point deadline) {
    const auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(
        deadline - std::chrono::steady_clock::now());
    return remaining.count() <= 0 ? 0 : std::max<long>(1, remaining.count());
}

Result failed(const Error error, const char* message) {
    return {.error = error, .message = message};
}

}  // namespace

bool available() noexcept {
    return true;
}

Backend backend() noexcept {
    return Backend::LibCurl;
}

const char* backend_name() noexcept {
    return "libcurl";
}

Result request(const Request& source) {
    if (!detail::valid_https_url(source.url)) {
        return failed(Error::InvalidUrl, "Invalid HTTPS URL");
    }

    static std::once_flag initFlag;
    std::call_once(initFlag, initialize_curl);

    Request request = source;
    const auto deadline = std::chrono::steady_clock::now() + request.timeout;
    for (size_t redirects = 0; redirects <= 5; ++redirects) {
        const long timeout = remaining_timeout_ms(deadline);
        if (timeout == 0) {
            return failed(Error::Timeout, "Request timed out");
        }

        CURL* curl = curl_easy_init();
        if (curl == nullptr) {
            return failed(Error::Network, "Failed to create libcurl request");
        }
        CurlHeaders headers;
        for (const Header& header : request.headers) {
            if (!headers.append(header)) {
                curl_easy_cleanup(curl);
                return failed(Error::Network, "Failed to allocate libcurl headers");
            }
        }
        CurlContext context{.maxBodyBytes = request.maxBodyBytes};
        curl_easy_setopt(curl, CURLOPT_URL, request.url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, request.method == Method::Get ? 1L : 0L);
        curl_easy_setopt(curl, CURLOPT_POST, request.method == Method::Post ? 1L : 0L);
        if (request.method == Method::Post) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS,
                request.body.empty() ? "" : request.body.data());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE,
                static_cast<curl_off_t>(request.body.size()));
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers.list);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, timeout);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &context);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &context);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
#if CURL_AT_LEAST_VERSION(7, 85, 0)
        curl_easy_setopt(curl, CURLOPT_PROTOCOLS_STR, "https");
        curl_easy_setopt(curl, CURLOPT_REDIR_PROTOCOLS_STR, "https");
#else
        curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTPS);
        curl_easy_setopt(curl, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_HTTPS);
#endif

        const CURLcode code = curl_easy_perform(curl);
        long status = 0;
        char* redirectUrl = nullptr;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
        curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &redirectUrl);
        const std::string target = redirectUrl == nullptr ? std::string() : std::string(redirectUrl);
        curl_easy_cleanup(curl);
        if (remaining_timeout_ms(deadline) == 0) {
            return failed(Error::Timeout, "Request timed out");
        }
        const Error error = map_curl_error(code, context);
        if (error != Error::None) {
            return failed(error, error == Error::TooLarge ? "Response body exceeded the configured limit"
                                                           : "libcurl request failed");
        }
        if (!is_redirect(status)) {
            context.response.statusCode = static_cast<int>(status);
            return {.response = std::move(context.response)};
        }
        if (redirects == 5 || !detail::valid_https_url(target)) {
            return failed(Error::Network, "Redirect rejected");
        }
        const bool crossOrigin = !same_origin(request.url, target);
        apply_redirect_policy(request, status, crossOrigin);
        request.url = target;
    }
    return failed(Error::Network, "Redirect rejected");
}

Result get(const Request& source) {
    Request request = source;
    request.method = Method::Get;
    request.body.clear();
    return dusk::http::request(request);
}

}  // namespace dusk::http
