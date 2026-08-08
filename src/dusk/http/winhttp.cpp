#include "http.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <shlwapi.h>
#include <winhttp.h>

#include <algorithm>
#include <chrono>
#include <cctype>
#include <limits>
#include <string_view>
#include <utility>
#include <vector>

namespace dusk::http {
namespace {

struct WinHttpHandle {
    HINTERNET handle = nullptr;
    WinHttpHandle() = default;
    explicit WinHttpHandle(HINTERNET value) : handle(value) {}
    WinHttpHandle(const WinHttpHandle&) = delete;
    WinHttpHandle& operator=(const WinHttpHandle&) = delete;
    ~WinHttpHandle() {
        if (handle != nullptr) {
            WinHttpCloseHandle(handle);
        }
    }
    operator HINTERNET() const { return handle; }
};

std::wstring utf8_to_wide(const std::string_view value) {
    if (value.empty() || value.size() > static_cast<size_t>(std::numeric_limits<int>::max())) {
        return {};
    }
    const int required = MultiByteToWideChar(
        CP_UTF8, MB_ERR_INVALID_CHARS, value.data(), static_cast<int>(value.size()), nullptr, 0);
    if (required <= 0) {
        return {};
    }
    std::wstring result(static_cast<size_t>(required), L'\0');
    return MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value.data(),
               static_cast<int>(value.size()), result.data(), required) == required
        ? result
        : std::wstring();
}

std::string wide_to_utf8(const std::wstring_view value) {
    if (value.empty() || value.size() > static_cast<size_t>(std::numeric_limits<int>::max())) {
        return {};
    }
    const int required = WideCharToMultiByte(
        CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0, nullptr, nullptr);
    if (required <= 0) {
        return {};
    }
    std::string result(static_cast<size_t>(required), '\0');
    return WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()),
               result.data(), required, nullptr, nullptr) == required
        ? result
        : std::string();
}

DWORD remaining_timeout(const std::chrono::steady_clock::time_point deadline) {
    const auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(
        deadline - std::chrono::steady_clock::now());
    if (remaining.count() <= 0) {
        return 0;
    }
    return static_cast<DWORD>(std::min<std::chrono::milliseconds::rep>(
        std::max<std::chrono::milliseconds::rep>(1, remaining.count()),
        std::numeric_limits<int>::max()));
}

bool set_remaining_timeouts(const HINTERNET session,
    const std::chrono::steady_clock::time_point deadline) {
    const DWORD timeout = remaining_timeout(deadline);
    return timeout != 0 && WinHttpSetTimeouts(session, timeout, timeout, timeout, timeout);
}

Error map_winhttp_error(const DWORD error) {
    switch (error) {
    case ERROR_WINHTTP_TIMEOUT:
        return Error::Timeout;
    case ERROR_WINHTTP_INVALID_URL:
    case ERROR_WINHTTP_UNRECOGNIZED_SCHEME:
        return Error::InvalidUrl;
    default:
        return Error::Network;
    }
}

Result fail(const Error error, const char* message) {
    return {.error = error, .message = message};
}

Result fail_from_last_error(const char* message) {
    return fail(map_winhttp_error(GetLastError()), message);
}

bool is_redirect(const DWORD status) {
    return status == 301 || status == 302 || status == 303 || status == 307 || status == 308;
}

bool header_name_equals(const std::string_view name, const std::string_view expected) {
    return name.size() == expected.size() && std::equal(name.begin(), name.end(), expected.begin(),
        [](const unsigned char left, const unsigned char right) {
            return std::tolower(left) == std::tolower(right);
        });
}

void apply_redirect_policy(Request& request, const DWORD status, const bool crossOrigin) {
    const bool convertsPost = request.method == Method::Post &&
        (status == 301 || status == 302 || status == 303);
    if (convertsPost) {
        request.method = Method::Get;
        request.body.clear();
    }
    std::erase_if(request.headers, [&](const Header& header) {
        const bool content = header.name.size() >= 8 &&
            std::equal(header.name.begin(), header.name.begin() + 8, "content-",
                [](const unsigned char left, const unsigned char right) {
                    return std::tolower(left) == std::tolower(right);
                });
        return (crossOrigin &&
                   (header_name_equals(header.name, "authorization") ||
                       header_name_equals(header.name, "cookie"))) ||
            (convertsPost && content);
    });
}

bool parse_url(const std::wstring& url, URL_COMPONENTS& components) {
    components = {};
    components.dwStructSize = sizeof(components);
    components.dwSchemeLength = static_cast<DWORD>(-1);
    components.dwHostNameLength = static_cast<DWORD>(-1);
    components.dwUserNameLength = static_cast<DWORD>(-1);
    components.dwPasswordLength = static_cast<DWORD>(-1);
    components.dwUrlPathLength = static_cast<DWORD>(-1);
    components.dwExtraInfoLength = static_cast<DWORD>(-1);
    return WinHttpCrackUrl(url.c_str(), static_cast<DWORD>(url.size()), 0, &components) &&
        components.nScheme == INTERNET_SCHEME_HTTPS && components.dwHostNameLength != 0 &&
        components.dwUserNameLength == 0 && components.dwPasswordLength == 0;
}

bool same_origin(const std::wstring& first, const std::wstring& second) {
    URL_COMPONENTS left{};
    URL_COMPONENTS right{};
    if (!parse_url(first, left) || !parse_url(second, right) || left.nPort != right.nPort ||
        left.dwHostNameLength != right.dwHostNameLength)
    {
        return false;
    }
    return CompareStringOrdinal(left.lpszHostName, static_cast<int>(left.dwHostNameLength),
               right.lpszHostName, static_cast<int>(right.dwHostNameLength), TRUE) == CSTR_EQUAL;
}

bool resolve_redirect(const std::wstring& base, const std::wstring& location, std::wstring& result) {
    DWORD size = static_cast<DWORD>(detail::kMaxUrlBytes + 1);
    result.resize(size);
    if (UrlCombineW(base.c_str(), location.c_str(), result.data(), &size, 0) != S_OK ||
        size == 0 || size > detail::kMaxUrlBytes)
    {
        return false;
    }
    result.resize(size);
    if (!result.empty() && result.back() == L'\0') {
        result.pop_back();
    }
    return true;
}

bool read_status(const HINTERNET request, DWORD& status) {
    DWORD bytes = sizeof(status);
    return WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
        WINHTTP_HEADER_NAME_BY_INDEX, &status, &bytes, WINHTTP_NO_HEADER_INDEX) != FALSE;
}

bool read_location(const HINTERNET request, std::wstring& location) {
    DWORD bytes = 0;
    WinHttpQueryHeaders(request, WINHTTP_QUERY_LOCATION, WINHTTP_HEADER_NAME_BY_INDEX,
        WINHTTP_NO_OUTPUT_BUFFER, &bytes, WINHTTP_NO_HEADER_INDEX);
    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER || bytes == 0 ||
        bytes > (detail::kMaxUrlBytes + 1) * sizeof(wchar_t))
    {
        return false;
    }
    location.assign(bytes / sizeof(wchar_t), L'\0');
    if (!WinHttpQueryHeaders(request, WINHTTP_QUERY_LOCATION, WINHTTP_HEADER_NAME_BY_INDEX,
            location.data(), &bytes, WINHTTP_NO_HEADER_INDEX))
    {
        return false;
    }
    if (!location.empty() && location.back() == L'\0') {
        location.pop_back();
    }
    return !location.empty();
}

bool read_headers(const HINTERNET request, Response& response) {
    constexpr DWORD maxRaw = static_cast<DWORD>(
        detail::kMaxHeaderBytes + detail::kMaxHeaders * 4 + 128);
    DWORD bytes = 0;
    WinHttpQueryHeaders(request, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX,
        WINHTTP_NO_OUTPUT_BUFFER, &bytes, WINHTTP_NO_HEADER_INDEX);
    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER || bytes == 0 || bytes > maxRaw) {
        return false;
    }
    std::wstring raw(bytes / sizeof(wchar_t), L'\0');
    if (!WinHttpQueryHeaders(request, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX,
            raw.data(), &bytes, WINHTTP_NO_HEADER_INDEX))
    {
        return false;
    }
    size_t aggregate = 0;
    size_t start = raw.find(L"\r\n");
    if (start == std::wstring::npos) {
        return false;
    }
    start += 2;
    while (start < raw.size()) {
        const size_t end = raw.find(L"\r\n", start);
        if (end == start || end == std::wstring::npos) {
            break;
        }
        const std::wstring_view line(raw.data() + start, end - start);
        const size_t colon = line.find(L':');
        if (colon == std::wstring_view::npos) {
            return false;
        }
        Header header{
            .name = wide_to_utf8(line.substr(0, colon)),
            .value = wide_to_utf8(line.substr(colon + 1)),
        };
        while (!header.value.empty() &&
               (header.value.front() == ' ' || header.value.front() == '\t')) {
            header.value.erase(header.value.begin());
        }
        if (!detail::valid_response_header(header, response.headers.size(), aggregate)) {
            return false;
        }
        aggregate += header.name.size() + header.value.size();
        response.headers.push_back(std::move(header));
        start = end + 2;
    }
    return true;
}

}  // namespace

bool available() noexcept {
    return true;
}

Backend backend() noexcept {
    return Backend::WinHttp;
}

const char* backend_name() noexcept {
    return "WinHTTP";
}

Result request(const Request& source) {
    if (!detail::valid_https_url(source.url) ||
        source.body.size() > static_cast<size_t>(std::numeric_limits<DWORD>::max()))
    {
        return fail(Error::InvalidUrl, "Invalid HTTPS URL or request body");
    }
    std::wstring currentUrl = utf8_to_wide(source.url);
    if (currentUrl.empty()) {
        return fail(Error::InvalidUrl, "URL is not valid UTF-8");
    }
    Request request = source;
    const auto deadline = std::chrono::steady_clock::now() + request.timeout;
    WinHttpHandle session(WinHttpOpen(L"Dusk", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0));
    if (session.handle == nullptr) {
        return fail_from_last_error("Failed to create WinHTTP session");
    }
    for (size_t redirects = 0; redirects <= 5; ++redirects) {
        if (!set_remaining_timeouts(session, deadline)) {
            return remaining_timeout(deadline) == 0 ? fail(Error::Timeout, "Request timed out")
                                                    : fail_from_last_error("Failed to set WinHTTP timeouts");
        }
        URL_COMPONENTS components{};
        if (!parse_url(currentUrl, components)) {
            return fail(Error::InvalidUrl, "Redirect URL rejected");
        }
        const std::wstring host(components.lpszHostName, components.dwHostNameLength);
        std::wstring path;
        if (components.dwUrlPathLength != 0) {
            path.assign(components.lpszUrlPath, components.dwUrlPathLength);
        }
        if (components.dwExtraInfoLength != 0) {
            path.append(components.lpszExtraInfo, components.dwExtraInfoLength);
        }
        if (path.empty()) {
            path = L"/";
        }
        if (!set_remaining_timeouts(session, deadline)) {
            return remaining_timeout(deadline) == 0 ? fail(Error::Timeout, "Request timed out")
                                                    : fail_from_last_error("Failed to set WinHTTP timeouts");
        }
        WinHttpHandle connection(WinHttpConnect(session, host.c_str(), components.nPort, 0));
        if (connection.handle == nullptr || remaining_timeout(deadline) == 0) {
            return connection.handle == nullptr ? fail_from_last_error("Failed to connect")
                                                : fail(Error::Timeout, "Request timed out");
        }
        WinHttpHandle httpRequest(WinHttpOpenRequest(connection,
            request.method == Method::Post ? L"POST" : L"GET", path.c_str(), nullptr,
            WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE));
        if (httpRequest.handle == nullptr) {
            return fail_from_last_error("Failed to create request");
        }
        DWORD redirectPolicy = WINHTTP_OPTION_REDIRECT_POLICY_NEVER;
        DWORD maxHeaders = static_cast<DWORD>(detail::kMaxHeaderBytes + detail::kMaxHeaders * 4 + 128);
        DWORD disableCookies = WINHTTP_DISABLE_COOKIES;
        if (!WinHttpSetOption(httpRequest, WINHTTP_OPTION_REDIRECT_POLICY, &redirectPolicy,
                sizeof(redirectPolicy)) ||
            !WinHttpSetOption(httpRequest, WINHTTP_OPTION_MAX_RESPONSE_HEADER_SIZE, &maxHeaders,
                sizeof(maxHeaders)) ||
            !WinHttpSetOption(httpRequest, WINHTTP_OPTION_DISABLE_FEATURE, &disableCookies,
                sizeof(disableCookies)))
        {
            return fail_from_last_error("Failed to configure WinHTTP request");
        }
        for (const Header& header : request.headers) {
            const std::wstring wideHeader = utf8_to_wide(header.name + ": " + header.value);
            if (wideHeader.empty() || !WinHttpAddRequestHeaders(httpRequest, wideHeader.c_str(),
                    static_cast<DWORD>(wideHeader.size()), WINHTTP_ADDREQ_FLAG_ADD))
            {
                return wideHeader.empty() ? fail(Error::InvalidUrl, "Request header is not valid UTF-8")
                                          : fail_from_last_error("Failed to add request header");
            }
        }
        const DWORD bodySize = static_cast<DWORD>(request.body.size());
        if (!set_remaining_timeouts(httpRequest, deadline)) {
            return remaining_timeout(deadline) == 0 ? fail(Error::Timeout, "Request timed out")
                                                    : fail_from_last_error("Failed to set WinHTTP timeouts");
        }
        if (!WinHttpSendRequest(httpRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                request.body.empty() ? WINHTTP_NO_REQUEST_DATA : request.body.data(), bodySize, bodySize, 0) ||
            remaining_timeout(deadline) == 0)
        {
            return remaining_timeout(deadline) == 0 ? fail(Error::Timeout, "Request timed out")
                                                    : fail_from_last_error("Failed to send request");
        }
        if (!set_remaining_timeouts(httpRequest, deadline)) {
            return remaining_timeout(deadline) == 0 ? fail(Error::Timeout, "Request timed out")
                                                    : fail_from_last_error("Failed to set WinHTTP timeouts");
        }
        if (!WinHttpReceiveResponse(httpRequest, nullptr) || remaining_timeout(deadline) == 0) {
            return remaining_timeout(deadline) == 0 ? fail(Error::Timeout, "Request timed out")
                                                    : fail_from_last_error("Failed to receive response");
        }
        DWORD status = 0;
        Response redirectResponse;
        if (!read_status(httpRequest, status) || !read_headers(httpRequest, redirectResponse)) {
            return fail(Error::Network, "Failed to read response headers");
        }
        if (is_redirect(status)) {
            std::wstring location;
            std::wstring nextUrl;
            if (redirects == 5 || !read_location(httpRequest, location) ||
                !resolve_redirect(currentUrl, location, nextUrl))
            {
                return fail(Error::Network, "Redirect rejected");
            }
            const std::string nextUtf8 = wide_to_utf8(nextUrl);
            if (!detail::valid_https_url(nextUtf8)) {
                return fail(Error::Network, "Redirect rejected");
            }
            apply_redirect_policy(request, status, !same_origin(currentUrl, nextUrl));
            currentUrl = std::move(nextUrl);
            continue;
        }
        Response response;
        response.statusCode = static_cast<int>(status);
        if (!read_headers(httpRequest, response)) {
            return fail(Error::Network, "Response headers rejected");
        }
        for (;;) {
            DWORD available = 0;
            if (!set_remaining_timeouts(httpRequest, deadline)) {
                return remaining_timeout(deadline) == 0 ? fail(Error::Timeout, "Request timed out")
                                                        : fail_from_last_error("Failed to set WinHTTP timeouts");
            }
            if (!WinHttpQueryDataAvailable(httpRequest, &available) || remaining_timeout(deadline) == 0) {
                return remaining_timeout(deadline) == 0 ? fail(Error::Timeout, "Request timed out")
                                                        : fail_from_last_error("Failed to query response body");
            }
            if (available == 0) {
                break;
            }
            if (available > request.maxBodyBytes ||
                response.body.size() > request.maxBodyBytes - available)
            {
                return fail(Error::TooLarge, "Response body exceeded the configured limit");
            }
            std::vector<char> buffer(available);
            DWORD bytesRead = 0;
            if (!set_remaining_timeouts(httpRequest, deadline)) {
                return remaining_timeout(deadline) == 0 ? fail(Error::Timeout, "Request timed out")
                                                        : fail_from_last_error("Failed to set WinHTTP timeouts");
            }
            if (!WinHttpReadData(httpRequest, buffer.data(), available, &bytesRead) ||
                bytesRead > available || remaining_timeout(deadline) == 0)
            {
                return remaining_timeout(deadline) == 0 ? fail(Error::Timeout, "Request timed out")
                                                        : fail_from_last_error("Failed to read response body");
            }
            response.body.append(buffer.data(), bytesRead);
        }
        return {.response = std::move(response)};
    }
    return fail(Error::Network, "Redirect rejected");
}

Result get(const Request& source) {
    Request request = source;
    request.method = Method::Get;
    request.body.clear();
    return dusk::http::request(request);
}

}  // namespace dusk::http
