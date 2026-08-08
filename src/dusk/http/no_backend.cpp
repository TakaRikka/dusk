#include "http.hpp"

namespace dusk::http {

bool available() noexcept {
    return false;
}

Backend backend() noexcept {
    return Backend::None;
}

const char* backend_name() noexcept {
    return "none";
}

Result request(const Request&) {
    return {
        .error = Error::NoBackend,
        .message = "No HTTP backend is available",
    };
}

Result get(const Request& source) {
    Request request = source;
    request.method = Method::Get;
    request.body.clear();
    return dusk::http::request(request);
}

}  // namespace dusk::http
