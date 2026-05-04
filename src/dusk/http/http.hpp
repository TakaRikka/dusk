#ifndef DUSK_HTTP_HTTP_HPP
#define DUSK_HTTP_HTTP_HPP

#include <chrono>
#include <cstddef>
#include <string>
#include <vector>

namespace dusk::http {

enum class Backend {
    None,
    WinHttp,
    UrlSession,
    LibCurl,
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

struct Header {
    std::string name;
    std::string value;
};

struct Request {
    std::string url;
    std::vector<Header> headers;
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

bool available() noexcept;
Backend backend() noexcept;
const char* backend_name() noexcept;
Result get(const Request& request);

}  // namespace dusk::http

#endif  // DUSK_HTTP_HTTP_HPP
