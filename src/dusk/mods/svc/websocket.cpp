#include "registry.hpp"

#include "internal.hpp"
#include "net.hpp"

#include "dusk/mods/loader/loader.hpp"
#include "mods/svc/websocket.h"

#include <borealis/url.hpp>
#include <borealis/ws.hpp>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstring>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace dusk::mods::svc {
namespace {

constexpr size_t MaxConnectionsPerMod = 4;
constexpr size_t MaxUrlBytes = 8 * 1024;
constexpr size_t MaxHeaders = 64;
constexpr size_t MaxHeaderBytes = 16 * 1024;
constexpr size_t MaxProtocols = 8;
constexpr size_t DefaultMessageBytes = 1024 * 1024;
constexpr size_t MaxMessageBytes = 16 * 1024 * 1024;
constexpr std::chrono::milliseconds DefaultConnectTimeout{10000};
constexpr std::chrono::milliseconds DefaultCloseTimeout{5000};
constexpr std::string_view ReservedHeaders[]{
    "User-Agent",
    "Host",
    "Connection",
    "Upgrade",
    "Sec-WebSocket-Key",
    "Sec-WebSocket-Version",
    "Sec-WebSocket-Extensions",
    "Sec-WebSocket-Protocol",
    "Content-Length",
};

struct ConnectionState {
    borealis::ws::Connection connection;
    void* userData = nullptr;
    bool openPublished = false;
};

struct ModPollState {
    borealis::ws::Event currentEvent;
    std::vector<HttpHeader> headers;
    size_t next = 0;
};

SlotMap<ConnectionState> s_connections;
PerMod<ModPollState> s_pollStates;

bool valid_url(std::string_view url, bool& allowPlaintext) {
    allowPlaintext = false;
    if (url.size() > MaxUrlBytes) {
        return false;
    }
    const auto parsed = borealis::url::parse(url);
    if (!parsed || (parsed->scheme != "wss" && parsed->scheme != "ws")) {
        return false;
    }
    allowPlaintext = parsed->scheme == "ws";
    if (!allowPlaintext) {
        return true;
    }
    return parsed->host == "localhost" || parsed->host == "127.0.0.1" || parsed->host == "::1";
}

WebSocketError map_error(borealis::ws::Error error) {
    switch (error) {
    case borealis::ws::Error::None:
        return WEBSOCKET_ERROR_NONE;
    case borealis::ws::Error::InvalidUrl:
        return WEBSOCKET_ERROR_INVALID_URL;
    case borealis::ws::Error::UnsupportedScheme:
        return WEBSOCKET_ERROR_UNSUPPORTED_SCHEME;
    case borealis::ws::Error::Timeout:
        return WEBSOCKET_ERROR_TIMEOUT;
    case borealis::ws::Error::TooLarge:
        return WEBSOCKET_ERROR_TOO_LARGE;
    case borealis::ws::Error::Canceled:
        return WEBSOCKET_ERROR_CANCELED;
    case borealis::ws::Error::Protocol:
        return WEBSOCKET_ERROR_PROTOCOL;
    case borealis::ws::Error::Handshake:
        return WEBSOCKET_ERROR_HANDSHAKE;
    case borealis::ws::Error::NoBackend:
    case borealis::ws::Error::Network:
    default:
        return WEBSOCKET_ERROR_NETWORK;
    }
}

ModResult map_send_result(borealis::ws::SendResult result) {
    switch (result) {
    case borealis::ws::SendResult::Ok:
        return MOD_OK;
    case borealis::ws::SendResult::NotOpen:
        return MOD_UNAVAILABLE;
    case borealis::ws::SendResult::QueueFull:
        return MOD_CONFLICT;
    case borealis::ws::SendResult::TooLarge:
    case borealis::ws::SendResult::InvalidText:
        return MOD_INVALID_ARGUMENT;
    }
    return MOD_ERROR;
}

ModResult map_close_result(borealis::ws::CloseResult result) {
    switch (result) {
    case borealis::ws::CloseResult::Ok:
        return MOD_OK;
    case borealis::ws::CloseResult::NotOpen:
        return MOD_UNAVAILABLE;
    case borealis::ws::CloseResult::InvalidCode:
    case borealis::ws::CloseResult::ReasonTooLarge:
    case borealis::ws::CloseResult::InvalidText:
        return MOD_INVALID_ARGUMENT;
    }
    return MOD_ERROR;
}

size_t connection_count(const LoadedMod& mod) {
    size_t count = 0;
    s_connections.for_each([&](WebSocketHandle, const auto& entry) {
        if (entry.owner == &mod) {
            ++count;
        }
    });
    return count;
}

ModResult websocket_connect(
    ModContext* context, const WebSocketConnectDesc* desc, WebSocketHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(WebSocketConnectDesc) ||
        desc->url == nullptr || outHandle == nullptr ||
        (desc->header_count != 0 && desc->headers == nullptr) ||
        (desc->protocol_count != 0 && desc->protocols == nullptr))
    {
        return MOD_INVALID_ARGUMENT;
    }
    if (!declares_import(*mod, WEBSOCKET_SERVICE_ID)) {
        return MOD_UNSUPPORTED;
    }
    bool allowPlaintext = false;
    if (!valid_url(desc->url, allowPlaintext) || desc->header_count > MaxHeaders ||
        desc->protocol_count > MaxProtocols || desc->max_message_bytes > MaxMessageBytes)
    {
        return MOD_INVALID_ARGUMENT;
    }

    size_t headerBytes = 0;
    for (uint32_t index = 0; index < desc->header_count; ++index) {
        const HttpHeader& header = desc->headers[index];
        if (header.name == nullptr || header.value == nullptr) {
            return MOD_INVALID_ARGUMENT;
        }
        const std::string_view name{header.name};
        const std::string_view value{header.value};
        if (!valid_header(name, value, ReservedHeaders) ||
            name.size() > MaxHeaderBytes - std::min(headerBytes, MaxHeaderBytes))
        {
            return MOD_INVALID_ARGUMENT;
        }
        headerBytes += name.size();
        if (value.size() > MaxHeaderBytes - std::min(headerBytes, MaxHeaderBytes)) {
            return MOD_INVALID_ARGUMENT;
        }
        headerBytes += value.size();
    }
    for (uint32_t index = 0; index < desc->protocol_count; ++index) {
        if (desc->protocols[index] == nullptr || !valid_header_name(desc->protocols[index])) {
            return MOD_INVALID_ARGUMENT;
        }
    }
    if (!borealis::ws::available()) {
        return MOD_UNAVAILABLE;
    }
    if (connection_count(*mod) >= MaxConnectionsPerMod) {
        return MOD_CONFLICT;
    }

    const size_t maxMessageBytes =
        desc->max_message_bytes != 0 ? desc->max_message_bytes : DefaultMessageBytes;
    borealis::ws::Options options{
        .url = desc->url,
        .connectTimeout = desc->connect_timeout_ms != 0 ?
                              std::chrono::milliseconds{desc->connect_timeout_ms} :
                              DefaultConnectTimeout,
        .closeTimeout = desc->close_timeout_ms != 0 ?
                            std::chrono::milliseconds{desc->close_timeout_ms} :
                            DefaultCloseTimeout,
        .keepaliveInterval = std::chrono::milliseconds{desc->keepalive_interval_ms},
        .maxMessageBytes = maxMessageBytes,
        .maxQueuedBytes = 16 * 1024 * 1024,
        .maxSendQueueBytes = 4 * 1024 * 1024,
        .allowPlaintext = allowPlaintext,
    };
    options.headers.reserve(desc->header_count + 1);
    for (uint32_t index = 0; index < desc->header_count; ++index) {
        options.headers.push_back({desc->headers[index].name, desc->headers[index].value});
    }
    options.headers.push_back({
        .name = "User-Agent",
        .value = user_agent(*mod),
    });
    options.protocols.reserve(desc->protocol_count);
    for (uint32_t index = 0; index < desc->protocol_count; ++index) {
        options.protocols.emplace_back(desc->protocols[index]);
    }
    auto connection = borealis::ws::connect(std::move(options));
    if (!connection) {
        return MOD_UNAVAILABLE;
    }
    *outHandle = s_connections.emplace(*mod, ConnectionState{
                                                 .connection = std::move(connection),
                                                 .userData = desc->user_data,
                                             });
    return MOD_OK;
}

ModResult websocket_poll_event(ModContext* context, WebSocketEvent* outEvent) {
    const uint32_t structSize = outEvent != nullptr ? outEvent->struct_size : 0;
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outEvent == nullptr || structSize < sizeof(WebSocketEvent)) {
        return MOD_INVALID_ARGUMENT;
    }
    WebSocketEvent output{
        .struct_size = structSize,
        .protocol = "",
        .error_message = "",
        .close_reason = "",
    };

    std::vector<WebSocketHandle> handles;
    s_connections.for_each([&](WebSocketHandle handle, const auto& entry) {
        if (entry.owner == mod) {
            handles.push_back(handle);
        }
    });
    auto& pollState = s_pollStates.get_or_create(*mod);
    if (handles.empty()) {
        pollState.next = 0;
        std::memcpy(outEvent, &output, sizeof(output));
        return MOD_OK;
    }

    const size_t start = pollState.next % handles.size();
    WebSocketHandle selected = 0;
    ConnectionState* selectedState = nullptr;
    for (size_t offset = 0; offset < handles.size(); ++offset) {
        const size_t index = (start + offset) % handles.size();
        auto* entry = s_connections.find_owned(handles[index], *mod);
        if (entry != nullptr && entry->value.connection.poll(pollState.currentEvent)) {
            selected = handles[index];
            selectedState = &entry->value;
            pollState.next = index + 1;
            break;
        }
    }
    if (selectedState == nullptr) {
        std::memcpy(outEvent, &output, sizeof(output));
        return MOD_OK;
    }

    output.ws = selected;
    output.user_data = selectedState->userData;
    const auto& event = pollState.currentEvent;
    pollState.headers.clear();
    pollState.headers.reserve(event.headers.size());
    for (const auto& header : event.headers) {
        pollState.headers.push_back({header.name.c_str(), header.value.c_str()});
    }
    output.headers = pollState.headers.data();
    output.header_count = static_cast<uint32_t>(pollState.headers.size());
    if (event.kind == borealis::ws::Event::Kind::Open) {
        output.type = WEBSOCKET_EVENT_OPEN;
        output.protocol = event.protocol.c_str();
        selectedState->openPublished = true;
    } else if (event.kind == borealis::ws::Event::Kind::Message) {
        output.type = WEBSOCKET_EVENT_MESSAGE;
        output.message_kind = event.messageKind == borealis::ws::MessageKind::Text ?
                                  WEBSOCKET_MESSAGE_TEXT :
                                  WEBSOCKET_MESSAGE_BINARY;
        output.data = event.data.data();
        output.size = event.data.size();
    } else {
        output.type = WEBSOCKET_EVENT_CLOSED;
        output.error = map_error(event.error);
        output.error_message = event.message.c_str();
        output.handshake_status = event.status;
        output.close_code = event.code;
        output.close_reason = event.reason.c_str();
    }

    std::memcpy(outEvent, &output, sizeof(output));
    if (event.kind == borealis::ws::Event::Kind::Closed) {
        s_connections.erase(selected);
    }
    return MOD_OK;
}

ModResult websocket_send(ModContext* context, WebSocketHandle handle, WebSocketMessageKind kind,
    const void* data, size_t size) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || (size != 0 && data == nullptr) ||
        (kind != WEBSOCKET_MESSAGE_TEXT && kind != WEBSOCKET_MESSAGE_BINARY))
    {
        return MOD_INVALID_ARGUMENT;
    }
    auto* entry = s_connections.find_owned(handle, *mod);
    if (entry == nullptr || !entry->value.openPublished) {
        return MOD_UNAVAILABLE;
    }
    const std::string_view bytes = data != nullptr ?
                                       std::string_view{static_cast<const char*>(data), size} :
                                       std::string_view{};
    return map_send_result(entry->value.connection.send(kind == WEBSOCKET_MESSAGE_TEXT ?
                                                            borealis::ws::MessageKind::Text :
                                                            borealis::ws::MessageKind::Binary,
        bytes));
}

ModResult websocket_close(
    ModContext* context, WebSocketHandle handle, uint16_t code, const char* reason) {
    auto* mod = mod_from_context(context);
    const std::string_view reasonText =
        reason != nullptr ? std::string_view{reason} : std::string_view{};
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    auto* entry = s_connections.find_owned(handle, *mod);
    if (entry == nullptr) {
        return MOD_UNAVAILABLE;
    }
    return map_close_result(entry->value.connection.close(code != 0 ? code : 1000, reasonText));
}

void websocket_mod_deactivating(LoadedMod& mod) {
    (void)s_connections.take_all(mod);
    s_pollStates.erase(mod);
}

void websocket_mod_detached(LoadedMod& mod) {
    bool found = false;
    s_connections.for_each(
        [&](WebSocketHandle, const auto& entry) { found = found || entry.owner == &mod; });
    assert(!found);
    assert(!s_pollStates.contains(mod));
}

void websocket_shutdown() {
    s_connections = {};
    s_pollStates.clear();
}

bool websocket_available() {
    return borealis::ws::available();
}

constexpr WebSocketService s_websocketService{
    .header = SERVICE_HEADER(WebSocketService, WEBSOCKET_SERVICE_MAJOR, WEBSOCKET_SERVICE_MINOR),
    .connect = SERVICE_FUNCTION(websocket_connect),
    .poll_event = SERVICE_FUNCTION(websocket_poll_event),
    .send = SERVICE_FUNCTION(websocket_send),
    .close = SERVICE_FUNCTION(websocket_close),
};

}  // namespace

constinit const ServiceModule g_websocketModule{
    .id = WEBSOCKET_SERVICE_ID,
    .majorVersion = WEBSOCKET_SERVICE_MAJOR,
    .minorVersion = WEBSOCKET_SERVICE_MINOR,
    .service = &s_websocketService,
    .available = websocket_available,
    .modDeactivating = websocket_mod_deactivating,
    .modDetached = websocket_mod_detached,
    .shutdown = websocket_shutdown,
};

}  // namespace dusk::mods::svc
