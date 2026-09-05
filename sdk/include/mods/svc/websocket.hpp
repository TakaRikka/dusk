#pragma once

#include <mods/svc/http.hpp>
#include <mods/svc/websocket.h>

#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace mods::ws {

struct Options {
    std::string url;
    std::vector<http::Header> headers;
    std::vector<std::string> protocols;
    uint32_t connectTimeoutMs = 0;
    uint32_t closeTimeoutMs = 0;
    uint32_t keepaliveIntervalMs = 0;
    size_t maxMessageBytes = 0;
    void* userData = nullptr;
};

class Connection {
public:
    Connection() = default;
    Connection(WebSocketHandle handle, ModResult result) : mHandle{handle}, mResult{result} {}
    ~Connection() { reset(); }

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
    Connection(Connection&& other) noexcept { *this = std::move(other); }
    Connection& operator=(Connection&& other) noexcept {
        if (this != &other) {
            reset();
            mHandle = std::exchange(other.mHandle, 0);
            mResult = other.mResult;
        }
        return *this;
    }

    explicit operator bool() const { return mResult == MOD_OK && mHandle != 0; }

    ModResult result() const { return mResult; }
    WebSocketHandle handle() const { return mHandle; }

    ModResult send(WebSocketMessageKind kind, std::span<const std::byte> bytes) const {
        return svc_websocket != nullptr && mHandle != 0 ?
                   svc_websocket->send(mod_ctx, mHandle, kind, bytes.data(), bytes.size()) :
                   MOD_UNAVAILABLE;
    }

    ModResult send_text(std::string_view text) const {
        return svc_websocket != nullptr && mHandle != 0 ?
                   svc_websocket->send(
                       mod_ctx, mHandle, WEBSOCKET_MESSAGE_TEXT, text.data(), text.size()) :
                   MOD_UNAVAILABLE;
    }

    ModResult send_binary(std::span<const std::byte> bytes) const {
        return send(WEBSOCKET_MESSAGE_BINARY, bytes);
    }

    ModResult close(uint16_t code = 1000, std::string_view reason = {}) {
        if (svc_websocket == nullptr || mHandle == 0) {
            return MOD_UNAVAILABLE;
        }
        const std::string reasonText{reason};
        mResult = svc_websocket->close(mod_ctx, mHandle, code, reasonText.c_str());
        if (mResult == MOD_OK) {
            mHandle = 0;
        }
        return mResult;
    }

    void detach() { mHandle = 0; }

private:
    void reset() {
        if (mHandle != 0) {
            (void)close(1001, "Connection owner released");
            mHandle = 0;
        }
    }

    WebSocketHandle mHandle = 0;
    ModResult mResult = MOD_UNAVAILABLE;
};

inline Connection connect(const Options& options) {
    if (svc_websocket == nullptr || options.headers.size() > std::numeric_limits<uint32_t>::max() ||
        options.protocols.size() > std::numeric_limits<uint32_t>::max())
    {
        return {0, svc_websocket == nullptr ? MOD_UNAVAILABLE : MOD_INVALID_ARGUMENT};
    }

    std::vector<HttpHeader> headers;
    headers.reserve(options.headers.size());
    for (const auto& header : options.headers) {
        headers.push_back({.name = header.name.c_str(), .value = header.value.c_str()});
    }
    std::vector<const char*> protocols;
    protocols.reserve(options.protocols.size());
    for (const auto& protocol : options.protocols) {
        protocols.push_back(protocol.c_str());
    }

    WebSocketConnectDesc desc = WEBSOCKET_CONNECT_DESC_INIT;
    desc.url = options.url.c_str();
    desc.headers = headers.empty() ? nullptr : headers.data();
    desc.header_count = static_cast<uint32_t>(headers.size());
    desc.protocols = protocols.empty() ? nullptr : protocols.data();
    desc.protocol_count = static_cast<uint32_t>(protocols.size());
    desc.connect_timeout_ms = options.connectTimeoutMs;
    desc.close_timeout_ms = options.closeTimeoutMs;
    desc.keepalive_interval_ms = options.keepaliveIntervalMs;
    desc.max_message_bytes = options.maxMessageBytes;
    desc.user_data = options.userData;

    WebSocketHandle handle = 0;
    const ModResult result = svc_websocket->connect(mod_ctx, &desc, &handle);
    return {handle, result};
}

struct Event {
    WebSocketEventType type = WEBSOCKET_EVENT_NONE;
    WebSocketHandle handle = 0;
    void* userData = nullptr;
    std::string_view protocol;
    std::span<const HttpHeader> headers;
    WebSocketMessageKind messageKind = WEBSOCKET_MESSAGE_TEXT;
    std::span<const std::byte> data;
    WebSocketError error = WEBSOCKET_ERROR_NONE;
    std::string_view message;
    int handshakeStatus = 0;
    uint16_t closeCode = 0;
    std::string_view closeReason;
};

inline bool poll(Event& out) {
    out = {};
    if (svc_websocket == nullptr) {
        return false;
    }
    WebSocketEvent raw = WEBSOCKET_EVENT_INIT;
    if (svc_websocket->poll_event(mod_ctx, &raw) != MOD_OK || raw.type == WEBSOCKET_EVENT_NONE) {
        return false;
    }
    out.type = raw.type;
    out.handle = raw.ws;
    out.userData = raw.user_data;
    out.protocol = raw.protocol != nullptr ? raw.protocol : "";
    if (raw.headers != nullptr && raw.header_count != 0) {
        out.headers = {raw.headers, raw.header_count};
    }
    out.messageKind = raw.message_kind;
    if (raw.data != nullptr && raw.size != 0) {
        out.data = {static_cast<const std::byte*>(raw.data), raw.size};
    }
    out.error = raw.error;
    out.message = raw.error_message != nullptr ? raw.error_message : "";
    out.handshakeStatus = raw.handshake_status;
    out.closeCode = raw.close_code;
    out.closeReason = raw.close_reason != nullptr ? raw.close_reason : "";
    return true;
}

}  // namespace mods::ws
