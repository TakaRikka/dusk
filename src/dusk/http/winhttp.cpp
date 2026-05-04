#include "http.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <winhttp.h>

#include <algorithm>
#include <limits>
#include <string_view>
#include <utility>
#include <vector>

namespace dusk::http {
namespace {

struct WinHttpHandle {
    HINTERNET handle = nullptr;

    WinHttpHandle() = default;
    explicit WinHttpHandle(HINTERNET handle) : handle(handle) {}
    WinHttpHandle(const WinHttpHandle&) = delete;
    WinHttpHandle& operator=(const WinHttpHandle&) = delete;

    ~WinHttpHandle() {
        if (handle != nullptr) {
            WinHttpCloseHandle(handle);
        }
    }

    operator HINTERNET() const { return handle; }
};

std::wstring utf8_to_wide(std::string_view value) {
    if (value.empty()) {
        return {};
    }

    const int required = MultiByteToWideChar(
        CP_UTF8, MB_ERR_INVALID_CHARS, value.data(), static_cast<int>(value.size()), nullptr, 0);
    if (required <= 0) {
        return {};
    }

    std::wstring result(static_cast<size_t>(required), L'\0');
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value.data(), static_cast<int>(value.size()),
        result.data(), required);
    return result;
}

std::string wide_to_utf8(std::wstring_view value) {
    if (value.empty()) {
        return {};
    }

    const int required = WideCharToMultiByte(
        CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0, nullptr, nullptr);
    if (required <= 0) {
        return {};
    }

    std::string result(static_cast<size_t>(required), '\0');
    WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), result.data(),
        required, nullptr, nullptr);
    return result;
}

DWORD timeout_ms(std::chrono::milliseconds timeout) {
    const auto count = std::max<std::chrono::milliseconds::rep>(1, timeout.count());
    return static_cast<DWORD>(
        std::min<std::chrono::milliseconds::rep>(count, std::numeric_limits<int>::max()));
}

Error map_winhttp_error(DWORD error) {
    switch (error) {
    case ERROR_WINHTTP_TIMEOUT:
        return Error::Timeout;
    case ERROR_WINHTTP_INVALID_URL:
    case ERROR_WINHTTP_UNRECOGNIZED_SCHEME:
        return Error::InvalidUrl;
    case ERROR_WINHTTP_SECURE_FAILURE:
    case ERROR_WINHTTP_CANNOT_CONNECT:
    case ERROR_WINHTTP_CONNECTION_ERROR:
    default:
        return Error::Network;
    }
}

Result fail_from_last_error(const char* message) {
    const DWORD error = GetLastError();
    return {
        .error = map_winhttp_error(error),
        .message = std::string(message) + " (" + std::to_string(error) + ")",
    };
}

std::string trim_header_value(std::string_view value) {
    while (!value.empty() && (value.front() == ' ' || value.front() == '\t')) {
        value.remove_prefix(1);
    }
    while (!value.empty() && (value.back() == '\r' || value.back() == '\n' || value.back() == ' ' ||
                                 value.back() == '\t'))
    {
        value.remove_suffix(1);
    }
    return std::string(value);
}

void parse_headers(std::wstring_view rawHeaders, Response& response) {
    size_t start = 0;
    bool firstLine = true;
    while (start < rawHeaders.size()) {
        size_t end = rawHeaders.find(L"\r\n", start);
        if (end == std::wstring_view::npos) {
            end = rawHeaders.size();
        }

        const std::wstring_view line = rawHeaders.substr(start, end - start);
        if (!line.empty() && !firstLine) {
            const size_t colon = line.find(L':');
            if (colon != std::wstring_view::npos) {
                response.headers.push_back({
                    .name = wide_to_utf8(line.substr(0, colon)),
                    .value = trim_header_value(wide_to_utf8(line.substr(colon + 1))),
                });
            }
        }
        firstLine = false;

        if (end == rawHeaders.size()) {
            break;
        }
        start = end + 2;
    }
}

bool read_status(HINTERNET request, Response& response) {
    DWORD statusCode = 0;
    DWORD statusCodeSize = sizeof(statusCode);
    if (!WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &statusCodeSize, WINHTTP_NO_HEADER_INDEX))
    {
        return false;
    }
    response.statusCode = static_cast<int>(statusCode);
    return true;
}

bool read_headers(HINTERNET request, Response& response) {
    DWORD headerBytes = 0;
    WinHttpQueryHeaders(request, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX,
        WINHTTP_NO_OUTPUT_BUFFER, &headerBytes, WINHTTP_NO_HEADER_INDEX);
    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
        return false;
    }

    std::wstring rawHeaders(headerBytes / sizeof(wchar_t), L'\0');
    if (!WinHttpQueryHeaders(request, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX,
            rawHeaders.data(), &headerBytes, WINHTTP_NO_HEADER_INDEX))
    {
        return false;
    }
    if (!rawHeaders.empty() && rawHeaders.back() == L'\0') {
        rawHeaders.pop_back();
    }
    parse_headers(rawHeaders, response);
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

Result get(const Request& request) {
    if (request.url.empty()) {
        return {
            .error = Error::InvalidUrl,
            .message = "URL is empty",
        };
    }

    std::wstring wideUrl = utf8_to_wide(request.url);
    if (wideUrl.empty()) {
        return {
            .error = Error::InvalidUrl,
            .message = "URL is not valid UTF-8",
        };
    }

    URL_COMPONENTS components{};
    components.dwStructSize = sizeof(components);
    components.dwSchemeLength = static_cast<DWORD>(-1);
    components.dwHostNameLength = static_cast<DWORD>(-1);
    components.dwUrlPathLength = static_cast<DWORD>(-1);
    components.dwExtraInfoLength = static_cast<DWORD>(-1);
    if (!WinHttpCrackUrl(wideUrl.c_str(), static_cast<DWORD>(wideUrl.size()), 0, &components)) {
        return fail_from_last_error("Failed to parse URL");
    }
    if (components.nScheme != INTERNET_SCHEME_HTTPS) {
        return {
            .error = Error::UnsupportedScheme,
            .message = "Only https:// URLs are supported",
        };
    }

    const std::wstring host(components.lpszHostName, components.dwHostNameLength);
    std::wstring path;
    if (components.lpszUrlPath != nullptr && components.dwUrlPathLength > 0) {
        path.assign(components.lpszUrlPath, components.dwUrlPathLength);
    }
    if (components.lpszExtraInfo != nullptr && components.dwExtraInfoLength > 0) {
        path.append(components.lpszExtraInfo, components.dwExtraInfoLength);
    }
    if (path.empty()) {
        path = L"/";
    }

    WinHttpHandle session(WinHttpOpen(L"Dusk", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0));
    if (session.handle == nullptr) {
        return fail_from_last_error("Failed to create WinHTTP session");
    }

    const DWORD timeout = timeout_ms(request.timeout);
    WinHttpSetTimeouts(session, timeout, timeout, timeout, timeout);

    WinHttpHandle connection(WinHttpConnect(session, host.c_str(), components.nPort, 0));
    if (connection.handle == nullptr) {
        return fail_from_last_error("Failed to connect");
    }

    WinHttpHandle httpRequest(WinHttpOpenRequest(connection, L"GET", path.c_str(), nullptr,
        WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE));
    if (httpRequest.handle == nullptr) {
        return fail_from_last_error("Failed to create request");
    }

    DWORD redirectPolicy = WINHTTP_OPTION_REDIRECT_POLICY_DISALLOW_HTTPS_TO_HTTP;
    WinHttpSetOption(
        httpRequest, WINHTTP_OPTION_REDIRECT_POLICY, &redirectPolicy, sizeof(redirectPolicy));
    DWORD maxRedirects = 5;
    WinHttpSetOption(httpRequest, WINHTTP_OPTION_MAX_HTTP_AUTOMATIC_REDIRECTS, &maxRedirects,
        sizeof(maxRedirects));

    for (const Header& header : request.headers) {
        const std::wstring wideHeader = utf8_to_wide(header.name + ": " + header.value);
        if (wideHeader.empty()) {
            return {
                .error = Error::InvalidUrl,
                .message = "Request header is not valid UTF-8",
            };
        }
        if (!WinHttpAddRequestHeaders(httpRequest, wideHeader.c_str(),
                static_cast<DWORD>(wideHeader.size()), WINHTTP_ADDREQ_FLAG_ADD))
        {
            return fail_from_last_error("Failed to add request header");
        }
    }

    if (!WinHttpSendRequest(
            httpRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
    {
        return fail_from_last_error("Failed to send request");
    }
    if (!WinHttpReceiveResponse(httpRequest, nullptr)) {
        return fail_from_last_error("Failed to receive response");
    }

    Response response;
    if (!read_status(httpRequest, response)) {
        return fail_from_last_error("Failed to read response status");
    }
    read_headers(httpRequest, response);

    for (;;) {
        DWORD availableBytes = 0;
        if (!WinHttpQueryDataAvailable(httpRequest, &availableBytes)) {
            return fail_from_last_error("Failed to query response body");
        }
        if (availableBytes == 0) {
            break;
        }
        if (availableBytes > request.maxBodyBytes ||
            response.body.size() > request.maxBodyBytes - availableBytes)
        {
            return {
                .error = Error::TooLarge,
                .message = "Response body exceeded the configured limit",
                .response = std::move(response),
            };
        }

        std::vector<char> buffer(availableBytes);
        DWORD bytesRead = 0;
        if (!WinHttpReadData(httpRequest, buffer.data(), availableBytes, &bytesRead)) {
            return fail_from_last_error("Failed to read response body");
        }
        response.body.append(buffer.data(), bytesRead);
    }

    return {
        .response = std::move(response),
    };
}

}  // namespace dusk::http
