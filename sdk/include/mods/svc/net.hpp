#pragma once

#include <mods/svc/net.h>

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

namespace mods::net {

class Socket {
public:
    Socket() = default;
    Socket(NetHandle handle, ModResult result) : mHandle{handle}, mResult{result} {}
    ~Socket() { reset(); }

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket&& other) noexcept { *this = std::move(other); }
    Socket& operator=(Socket&& other) noexcept {
        if (this != &other) {
            reset();
            mHandle = std::exchange(other.mHandle, 0);
            mResult = other.mResult;
        }
        return *this;
    }

    explicit operator bool() const { return mResult == MOD_OK && mHandle != 0; }

    ModResult result() const { return mResult; }
    NetHandle handle() const { return mHandle; }

    ModResult send(std::span<const std::byte> bytes) const {
        return svc_net != nullptr && mHandle != 0 ?
                   svc_net->send(mod_ctx, mHandle, bytes.data(), bytes.size()) :
                   MOD_UNAVAILABLE;
    }

    ModResult send_to(std::string_view endpoint, std::span<const std::byte> bytes) const {
        if (svc_net == nullptr || mHandle == 0) {
            return MOD_UNAVAILABLE;
        }
        const std::string endpointText{endpoint};
        return svc_net->send_to(mod_ctx, mHandle, endpointText.c_str(), bytes.data(), bytes.size());
    }

    std::optional<NetStats> stats() const {
        if (svc_net == nullptr || mHandle == 0) {
            return std::nullopt;
        }
        NetStats value = NET_STATS_INIT;
        if (svc_net->stats(mod_ctx, mHandle, &value) != MOD_OK) {
            return std::nullopt;
        }
        return value;
    }

    ModResult set_user_data(void* userData) const {
        return svc_net != nullptr && mHandle != 0 ?
                   svc_net->set_user_data(mod_ctx, mHandle, userData) :
                   MOD_UNAVAILABLE;
    }

    ModResult close() {
        if (mHandle == 0) {
            return mResult == MOD_OK ? MOD_OK : MOD_UNAVAILABLE;
        }
        mResult = svc_net != nullptr ? svc_net->close(mod_ctx, mHandle) : MOD_UNAVAILABLE;
        mHandle = 0;
        return mResult;
    }

    void detach() { mHandle = 0; }

private:
    void reset() { (void)close(); }

    NetHandle mHandle = 0;
    ModResult mResult = MOD_UNAVAILABLE;
};

struct BindOutcome {
    std::string local;
    NetError error = NET_ERROR_NONE;
};

inline Socket connect(std::string_view endpoint, NetConnectDesc options = NET_CONNECT_DESC_INIT) {
    if (svc_net == nullptr) {
        return {0, MOD_UNAVAILABLE};
    }
    const std::string endpointText{endpoint};
    options.struct_size = sizeof(options);
    options.endpoint = endpointText.c_str();
    NetHandle handle = 0;
    const ModResult result = svc_net->connect(mod_ctx, &options, &handle);
    return {handle, result};
}

inline Socket listen(std::string_view bind, BindOutcome* out = nullptr,
    NetListenDesc options = NET_LISTEN_DESC_INIT) {
    if (svc_net == nullptr) {
        return {0, MOD_UNAVAILABLE};
    }
    const std::string bindText{bind};
    options.struct_size = sizeof(options);
    options.bind = bindText.c_str();
    NetHandle handle = 0;
    NetEndpoint local{};
    NetError error = NET_ERROR_NONE;
    const ModResult result = svc_net->listen(mod_ctx, &options, &handle, &local, &error);
    if (out != nullptr) {
        *out = {.local = local.text, .error = error};
    }
    return {handle, result};
}

inline Socket open_datagram(std::string_view bind, BindOutcome* out = nullptr,
    NetDatagramDesc options = NET_DATAGRAM_DESC_INIT) {
    if (svc_net == nullptr) {
        return {0, MOD_UNAVAILABLE};
    }
    const std::string bindText{bind};
    options.struct_size = sizeof(options);
    options.bind = bindText.c_str();
    NetHandle handle = 0;
    NetEndpoint local{};
    NetError error = NET_ERROR_NONE;
    const ModResult result = svc_net->open_datagram(mod_ctx, &options, &handle, &local, &error);
    if (out != nullptr) {
        *out = {.local = local.text, .error = error};
    }
    return {handle, result};
}

inline Socket resolve(std::string_view endpoint, void* userData = nullptr) {
    if (svc_net == nullptr) {
        return {0, MOD_UNAVAILABLE};
    }
    const std::string endpointText{endpoint};
    NetHandle handle = 0;
    const ModResult result = svc_net->resolve(mod_ctx, endpointText.c_str(), userData, &handle);
    return {handle, result};
}

inline Socket adopt(NetHandle accepted) {
    return {accepted, accepted != 0 ? MOD_OK : MOD_INVALID_ARGUMENT};
}

struct Event {
    NetEventType type = NET_EVENT_NONE;
    NetHandle handle = 0;
    void* userData = nullptr;
    NetHandle accepted = 0;
    std::string_view endpoint;
    std::span<const std::byte> data;
    uint32_t dropped = 0;
    NetError error = NET_ERROR_NONE;
    std::string_view message;
};

inline bool poll(Event& out) {
    out = {};
    if (svc_net == nullptr) {
        return false;
    }
    NetEvent raw = NET_EVENT_INIT;
    if (svc_net->poll_event(mod_ctx, &raw) != MOD_OK || raw.type == NET_EVENT_NONE) {
        return false;
    }
    out.type = raw.type;
    out.handle = raw.handle;
    out.userData = raw.user_data;
    out.accepted = raw.accepted;
    out.endpoint = raw.endpoint.text;
    if (raw.data != nullptr && raw.size != 0) {
        out.data = {static_cast<const std::byte*>(raw.data), raw.size};
    }
    out.dropped = raw.dropped;
    out.error = raw.error;
    out.message = raw.error_message != nullptr ? raw.error_message : "";
    return true;
}

}  // namespace mods::net
