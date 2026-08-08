#ifndef DUSK_HTTP_HTTP_HPP
#define DUSK_HTTP_HTTP_HPP

#include <chrono>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace dusk::http {

enum class Backend {
    None,
    WinHttp,
    UrlSession,
    LibCurl,
    Android,
};

enum class Error {
    None,
    NoBackend,
    InvalidUrl,
    UnsupportedScheme,
    Timeout,
    TooLarge,
    Network,
};

enum class Method {
    Get,
    Post,
};

struct Header {
    std::string name;
    std::string value;
};

struct Request {
    Method method = Method::Get;
    std::string url;
    std::vector<Header> headers;
    std::string body;
    std::chrono::milliseconds timeout{10000};
    size_t maxBodyBytes = 1024 * 1024;
};

struct Response {
    int statusCode = 0;
    std::vector<Header> headers;
    std::string body;
};

struct Result {
    Error error = Error::None;
    std::string message;
    Response response;
};

namespace detail {

inline constexpr size_t kMaxUrlBytes = 4096;
inline constexpr size_t kMaxHeaders = 32;
inline constexpr size_t kMaxHeaderNameBytes = 128;
inline constexpr size_t kMaxHeaderValueBytes = 4096;
inline constexpr size_t kMaxHeaderBytes = 16 * 1024;

inline bool is_token(const std::string_view value) {
    if (value.empty()) {
        return false;
    }
    for (const unsigned char ch : value) {
        if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') ||
                (ch >= '0' && ch <= '9') ||
                std::string_view("!#$%&'*+-.^_`|~").find(static_cast<char>(ch)) !=
                    std::string_view::npos))
        {
            return false;
        }
    }
    return true;
}

inline bool valid_https_url(const std::string_view url) {
    constexpr std::string_view prefix = "https://";
    if (!url.starts_with(prefix) || url.size() > kMaxUrlBytes ||
        url.find_first_of("\r\n") != std::string_view::npos ||
        url.find('\0') != std::string_view::npos)
    {
        return false;
    }
    const size_t authorityStart = prefix.size();
    const size_t authorityEnd = url.find_first_of("/?#", authorityStart);
    const std::string_view authority = url.substr(authorityStart, authorityEnd - authorityStart);
    if (authority.empty() || authority.find('@') != std::string_view::npos ||
        url.find('#') != std::string_view::npos)
    {
        return false;
    }
    return authority.front() != ':' && authority != "[]";
}

inline bool valid_response_header(const Header& header, size_t count, size_t aggregate) {
    return count < kMaxHeaders && header.name.size() <= kMaxHeaderNameBytes &&
        header.value.size() <= kMaxHeaderValueBytes && is_token(header.name) &&
        header.value.find_first_of("\r\n") == std::string::npos &&
        header.value.find('\0') == std::string::npos &&
        aggregate <= kMaxHeaderBytes - header.name.size() &&
        aggregate + header.name.size() <= kMaxHeaderBytes - header.value.size();
}

}  // namespace detail

bool available() noexcept;
Backend backend() noexcept;
const char* backend_name() noexcept;
Result request(const Request& request);
Result get(const Request& request);

}  // namespace dusk::http

#endif  // DUSK_HTTP_HTTP_HPP
