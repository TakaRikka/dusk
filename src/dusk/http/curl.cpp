#include "http.hpp"

#include <curl/curl.h>

#include <algorithm>
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

    bool append(const std::string& header) {
        curl_slist* next = curl_slist_append(list, header.c_str());
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
    bool tooLarge = false;
};

void initialize_curl() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

std::string trim_header_value(std::string_view value) {
    while (!value.empty() && (value.front() == ' ' || value.front() == '\t')) {
        value.remove_prefix(1);
    }
    while (!value.empty() &&
           (value.back() == '\r' || value.back() == '\n' || value.back() == ' ' ||
               value.back() == '\t')) {
        value.remove_suffix(1);
    }
    return std::string(value);
}

size_t write_body(char* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* context = static_cast<CurlContext*>(userdata);
    const size_t bytes = size * nmemb;
    if (bytes > context->maxBodyBytes ||
        context->response.body.size() > context->maxBodyBytes - bytes) {
        context->tooLarge = true;
        return 0;
    }

    context->response.body.append(ptr, bytes);
    return bytes;
}

size_t write_header(char* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* context = static_cast<CurlContext*>(userdata);
    const std::string_view line(ptr, size * nmemb);
    if (line.starts_with("HTTP/")) {
        context->response.headers.clear();
        return size * nmemb;
    }

    const size_t colon = line.find(':');
    if (colon == std::string_view::npos) {
        return size * nmemb;
    }

    context->response.headers.push_back({
        .name = std::string(line.substr(0, colon)),
        .value = trim_header_value(line.substr(colon + 1)),
    });
    return size * nmemb;
}

Error map_curl_error(CURLcode code, bool tooLarge) {
    if (tooLarge) {
        return Error::TooLarge;
    }

    switch (code) {
    case CURLE_OK:
        return Error::None;
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

long timeout_ms(std::chrono::milliseconds timeout) {
    return std::max<std::chrono::milliseconds::rep>(1, timeout.count());
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

Result get(const Request& request) {
    if (request.url.empty()) {
        return {
            .error = Error::InvalidUrl,
            .message = "URL is empty",
        };
    }
    if (!request.url.starts_with("https://")) {
        return {
            .error = Error::UnsupportedScheme,
            .message = "Only https:// URLs are supported",
        };
    }

    static std::once_flag initFlag;
    std::call_once(initFlag, initialize_curl);

    CURL* curl = curl_easy_init();
    if (curl == nullptr) {
        return {
            .error = Error::Network,
            .message = "Failed to create libcurl request",
        };
    }

    CurlHeaders headers;
    for (const Header& header : request.headers) {
        if (!headers.append(header.name + ": " + header.value)) {
            curl_easy_cleanup(curl);
            return {
                .error = Error::Network,
                .message = "Failed to allocate libcurl headers",
            };
        }
    }

    CurlContext context{
        .maxBodyBytes = request.maxBodyBytes,
    };

    curl_easy_setopt(curl, CURLOPT_URL, request.url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers.list);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout_ms(request.timeout));
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, timeout_ms(request.timeout));
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &context);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &context);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
#if CURL_AT_LEAST_VERSION(7, 85, 0)
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS_STR, "https");
    curl_easy_setopt(curl, CURLOPT_REDIR_PROTOCOLS_STR, "https");
#else
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTPS);
    curl_easy_setopt(curl, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_HTTPS);
#endif

    const CURLcode code = curl_easy_perform(curl);
    long statusCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
    curl_easy_cleanup(curl);

    context.response.statusCode = static_cast<int>(statusCode);
    if (code == CURLE_OK) {
        return {
            .response = std::move(context.response),
        };
    }

    const Error error = map_curl_error(code, context.tooLarge);
    return {
        .error = error,
        .message = error == Error::TooLarge ? "Response body exceeded the configured limit"
                                            : curl_easy_strerror(code),
        .response = std::move(context.response),
    };
}

}  // namespace dusk::http
