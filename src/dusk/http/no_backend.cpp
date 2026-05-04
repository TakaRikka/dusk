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

Result get(const Request&) {
    return {
        .error = Error::NoBackend,
        .message = "No HTTP backend is available",
    };
}

}  // namespace dusk::http
