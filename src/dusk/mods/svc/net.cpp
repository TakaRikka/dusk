#include "registry.hpp"

#include "internal.hpp"
#include "net.hpp"

#include "dusk/logging.h"
#include "dusk/mods/loader/loader.hpp"
#include "mods/svc/net.h"

#include <borealis/net.hpp>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstring>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>

namespace dusk::mods::svc {
namespace {

constexpr size_t MaxListenersPerMod = 4;
constexpr size_t MaxStreamsPerMod = 32;
constexpr size_t MaxDatagramsPerMod = 4;
constexpr size_t MaxResolversPerMod = 8;
constexpr size_t DefaultSendQueueBytes = 1024 * 1024;
constexpr size_t MaxSendQueueBytes = 8 * 1024 * 1024;
constexpr std::chrono::milliseconds DefaultConnectTimeout{10000};
constexpr std::chrono::milliseconds DefaultCloseTimeout{5000};
constexpr std::chrono::milliseconds InboundStallTimeout{30000};

enum class HandleKind {
    Listener,
    Stream,
    Datagram,
    Resolver,
};

struct HandleState {
    borealis::net::SocketId socket = 0;
    HandleKind kind = HandleKind::Stream;
    // Stream sends remain unavailable until the mod observes CONNECTED.
    bool openPublished = false;
};

struct ModState {
    ModState()
        : context{borealis::net::ContextOptions{
              .maxQueuedBytes = 16 * 1024 * 1024,
              .maxQueuedEvents = 4096,
              .maxStreams = MaxStreamsPerMod,
              .maxListeners = MaxListenersPerMod,
              .maxDatagramSockets = MaxDatagramsPerMod,
              .maxResolvers = MaxResolversPerMod,
          }} {}

    borealis::net::Context context;
    std::unordered_map<borealis::net::SocketId, NetHandle> handles;
    borealis::net::Event currentEvent;
};

SlotMap<HandleState> s_handles;
PerMod<ModState> s_modStates;

NetError map_error(borealis::net::Error error) {
    switch (error) {
    case borealis::net::Error::None:
        return NET_ERROR_NONE;
    case borealis::net::Error::InvalidEndpoint:
        return NET_ERROR_INVALID_ENDPOINT;
    case borealis::net::Error::Resolve:
        return NET_ERROR_RESOLVE;
    case borealis::net::Error::Timeout:
        return NET_ERROR_TIMEOUT;
    case borealis::net::Error::Refused:
        return NET_ERROR_REFUSED;
    case borealis::net::Error::Unreachable:
        return NET_ERROR_UNREACHABLE;
    case borealis::net::Error::Reset:
        return NET_ERROR_RESET;
    case borealis::net::Error::AddressInUse:
        return NET_ERROR_ADDRESS_IN_USE;
    case borealis::net::Error::Permission:
        return NET_ERROR_PERMISSION;
    case borealis::net::Error::TooLarge:
        return NET_ERROR_TOO_LARGE;
    case borealis::net::Error::Canceled:
        return NET_ERROR_CANCELED;
    case borealis::net::Error::Network:
    default:
        return NET_ERROR_NETWORK;
    }
}

ModResult map_send_result(borealis::net::SendResult result) {
    switch (result) {
    case borealis::net::SendResult::Ok:
        return MOD_OK;
    case borealis::net::SendResult::NotOpen:
        return MOD_UNAVAILABLE;
    case borealis::net::SendResult::QueueFull:
        return MOD_CONFLICT;
    case borealis::net::SendResult::TooLarge:
    case borealis::net::SendResult::InvalidEndpoint:
        return MOD_INVALID_ARGUMENT;
    }
    return MOD_ERROR;
}

NetEventType map_event(borealis::net::Event::Kind kind) {
    switch (kind) {
    case borealis::net::Event::Kind::Connected:
        return NET_EVENT_CONNECTED;
    case borealis::net::Event::Kind::Accepted:
        return NET_EVENT_ACCEPTED;
    case borealis::net::Event::Kind::StreamData:
        return NET_EVENT_STREAM_DATA;
    case borealis::net::Event::Kind::Datagram:
        return NET_EVENT_DATAGRAM;
    case borealis::net::Event::Kind::Dropped:
        return NET_EVENT_DROPPED;
    case borealis::net::Event::Kind::Resolved:
        return NET_EVENT_RESOLVED;
    case borealis::net::Event::Kind::Closed:
        return NET_EVENT_CLOSED;
    default:
        return NET_EVENT_NONE;
    }
}

bool valid_endpoint(std::string_view text, std::string_view scheme, bool requireLiteral) {
    if (text.empty() || text.size() >= NET_ENDPOINT_MAX) {
        return false;
    }
    const auto endpoint = borealis::net::parse_endpoint(text);
    return endpoint && endpoint->scheme == scheme && (!requireLiteral || endpoint->literal);
}

bool copy_endpoint(NetEndpoint& output, std::string_view endpoint) {
    if (endpoint.size() >= sizeof(output.text)) {
        output.text[0] = '\0';
        return false;
    }
    std::memcpy(output.text, endpoint.data(), endpoint.size());
    output.text[endpoint.size()] = '\0';
    return true;
}

size_t bounded_send_queue(size_t requested) {
    return requested != 0 ? requested : DefaultSendQueueBytes;
}

std::chrono::milliseconds timeout_or_default(uint32_t value, std::chrono::milliseconds fallback) {
    return value != 0 ? std::chrono::milliseconds{value} : fallback;
}

size_t count_handles(const LoadedMod& mod, HandleKind kind) {
    size_t count = 0;
    s_handles.for_each([&](NetHandle, const auto& entry) {
        if (entry.owner == &mod && entry.value.kind == kind) {
            ++count;
        }
    });
    return count;
}

ModState& state_for(LoadedMod& mod) {
    return s_modStates.get_or_create(mod);
}

ModState* find_state(LoadedMod& mod) {
    return s_modStates.find(mod);
}

NetHandle add_handle(LoadedMod& mod, ModState& state, borealis::net::SocketId socket,
    HandleKind kind, bool openPublished) {
    const NetHandle handle = s_handles.emplace(mod, HandleState{
                                                        .socket = socket,
                                                        .kind = kind,
                                                        .openPublished = openPublished,
                                                    });
    state.handles.emplace(socket, handle);
    return handle;
}

ModResult net_connect(ModContext* context, const NetConnectDesc* desc, NetHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(NetConnectDesc) ||
        desc->endpoint == nullptr || outHandle == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    if (!declares_import(*mod, NET_SERVICE_ID)) {
        return MOD_UNSUPPORTED;
    }
    const std::string_view endpoint{desc->endpoint};
    if (!valid_endpoint(endpoint, "tcp", false) || desc->max_send_queue_bytes > MaxSendQueueBytes) {
        return MOD_INVALID_ARGUMENT;
    }
    if (!borealis::net::available()) {
        return MOD_UNAVAILABLE;
    }
    if (count_handles(*mod, HandleKind::Stream) >= MaxStreamsPerMod) {
        return MOD_CONFLICT;
    }
    auto& state = state_for(*mod);
    const size_t maxSendQueueBytes = bounded_send_queue(desc->max_send_queue_bytes);
    const borealis::net::SocketId socket = state.context.connect(endpoint,
        borealis::net::StreamOptions{
            .connectTimeout = timeout_or_default(desc->connect_timeout_ms, DefaultConnectTimeout),
            .closeTimeout = timeout_or_default(desc->close_timeout_ms, DefaultCloseTimeout),
            .inboundStallTimeout = InboundStallTimeout,
            .maxSendQueueBytes = maxSendQueueBytes,
            .readChunkBytes = 64 * 1024,
            .noDelay = desc->no_delay,
        },
        desc->user_data);
    if (socket == 0) {
        return MOD_CONFLICT;
    }
    *outHandle = add_handle(*mod, state, socket, HandleKind::Stream, false);
    return MOD_OK;
}

ModResult net_listen(ModContext* context, const NetListenDesc* desc, NetHandle* outHandle,
    NetEndpoint* outLocal, NetError* outError) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    if (outLocal != nullptr) {
        *outLocal = {};
    }
    if (outError != nullptr) {
        *outError = NET_ERROR_NONE;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(NetListenDesc) ||
        desc->bind == nullptr || outHandle == nullptr || outLocal == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    if (!declares_import(*mod, NET_SERVICE_ID)) {
        return MOD_UNSUPPORTED;
    }
    const std::string_view bind{desc->bind};
    if (!valid_endpoint(bind, "tcp", true) || desc->max_send_queue_bytes > MaxSendQueueBytes) {
        return MOD_INVALID_ARGUMENT;
    }
    if (!borealis::net::available()) {
        return MOD_UNAVAILABLE;
    }
    if (count_handles(*mod, HandleKind::Listener) >= MaxListenersPerMod) {
        return MOD_CONFLICT;
    }
    auto& state = state_for(*mod);
    const size_t maxSendQueueBytes = bounded_send_queue(desc->max_send_queue_bytes);
    const auto result = state.context.listen(bind,
        borealis::net::ListenOptions{
            .accepted =
                borealis::net::StreamOptions{
                    .connectTimeout = DefaultConnectTimeout,
                    .closeTimeout = timeout_or_default(desc->close_timeout_ms, DefaultCloseTimeout),
                    .inboundStallTimeout = InboundStallTimeout,
                    .maxSendQueueBytes = maxSendQueueBytes,
                    .readChunkBytes = 64 * 1024,
                    .noDelay = desc->no_delay,
                },
            .backlog = 64,
            .reuseAddress = true,
        },
        desc->user_data);
    if (result.id == 0) {
        if (outError != nullptr) {
            *outError = map_error(result.error);
        }
        DuskLog.error("[{}] could not listen on '{}': {}", mod->metadata.id, bind, result.message);
        return MOD_ERROR;
    }
    if (!copy_endpoint(*outLocal, result.localEndpoint)) {
        state.context.close(result.id);
        return MOD_ERROR;
    }
    *outHandle = add_handle(*mod, state, result.id, HandleKind::Listener, true);
    return MOD_OK;
}

ModResult net_open_datagram(ModContext* context, const NetDatagramDesc* desc, NetHandle* outHandle,
    NetEndpoint* outLocal, NetError* outError) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    if (outLocal != nullptr) {
        *outLocal = {};
    }
    if (outError != nullptr) {
        *outError = NET_ERROR_NONE;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(NetDatagramDesc) ||
        desc->bind == nullptr || outHandle == nullptr || outLocal == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    if (!declares_import(*mod, NET_SERVICE_ID)) {
        return MOD_UNSUPPORTED;
    }
    const std::string_view bind{desc->bind};
    if (!valid_endpoint(bind, "udp", true) || desc->max_send_queue_bytes > MaxSendQueueBytes) {
        return MOD_INVALID_ARGUMENT;
    }
    if (!borealis::net::available()) {
        return MOD_UNAVAILABLE;
    }
    if (count_handles(*mod, HandleKind::Datagram) >= MaxDatagramsPerMod) {
        return MOD_CONFLICT;
    }
    auto& state = state_for(*mod);
    const size_t maxSendQueueBytes = bounded_send_queue(desc->max_send_queue_bytes);
    const auto result = state.context.open_datagram(bind,
        borealis::net::DatagramOptions{
            .maxSendQueueBytes = maxSendQueueBytes,
            .recvBufferBytes = 1024 * 1024,
            .sendBufferBytes = 1024 * 1024,
        },
        desc->user_data);
    if (result.id == 0) {
        if (outError != nullptr) {
            *outError = map_error(result.error);
        }
        DuskLog.error("[{}] could not bind datagram socket on '{}': {}", mod->metadata.id, bind,
            result.message);
        return MOD_ERROR;
    }
    if (!copy_endpoint(*outLocal, result.localEndpoint)) {
        state.context.close(result.id);
        return MOD_ERROR;
    }
    *outHandle = add_handle(*mod, state, result.id, HandleKind::Datagram, true);
    return MOD_OK;
}

ModResult net_resolve(
    ModContext* context, const char* endpoint, void* userData, NetHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || endpoint == nullptr || outHandle == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    if (!declares_import(*mod, NET_SERVICE_ID)) {
        return MOD_UNSUPPORTED;
    }
    const std::string_view endpointText{endpoint};
    if (endpointText.empty() || endpointText.size() >= NET_ENDPOINT_MAX ||
        !borealis::net::parse_endpoint(endpointText))
    {
        return MOD_INVALID_ARGUMENT;
    }
    if (!borealis::net::available()) {
        return MOD_UNAVAILABLE;
    }
    if (count_handles(*mod, HandleKind::Resolver) >= MaxResolversPerMod) {
        return MOD_CONFLICT;
    }
    auto& state = state_for(*mod);
    const auto socket = state.context.resolve(endpointText, userData);
    if (socket == 0) {
        return MOD_CONFLICT;
    }
    *outHandle = add_handle(*mod, state, socket, HandleKind::Resolver, false);
    return MOD_OK;
}

ModResult net_poll_event(ModContext* context, NetEvent* outEvent) {
    const uint32_t structSize = outEvent != nullptr ? outEvent->struct_size : 0;
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outEvent == nullptr || structSize < sizeof(NetEvent)) {
        return MOD_INVALID_ARGUMENT;
    }
    NetEvent output{.struct_size = structSize, .error_message = ""};
    auto* state = find_state(*mod);
    if (state == nullptr) {
        std::memcpy(outEvent, &output, sizeof(output));
        return MOD_OK;
    }

    while (state->context.poll(state->currentEvent)) {
        const auto handleFound = state->handles.find(state->currentEvent.id);
        if (handleFound == state->handles.end()) {
            continue;
        }
        const NetHandle handle = handleFound->second;
        auto* handleState = s_handles.find_owned(handle, *mod);
        if (handleState == nullptr) {
            continue;
        }
        output.type = map_event(state->currentEvent.kind);
        output.handle = handle;
        output.user_data = state->currentEvent.userData;
        output.data = state->currentEvent.data.data();
        output.size = state->currentEvent.data.size();
        output.dropped = state->currentEvent.dropped;
        output.error = map_error(state->currentEvent.error);
        output.error_message = state->currentEvent.message.c_str();
        if (!copy_endpoint(output.endpoint, state->currentEvent.endpoint)) {
            output.endpoint = {};
        }

        if (state->currentEvent.kind == borealis::net::Event::Kind::Connected) {
            handleState->value.openPublished = true;
        } else if (state->currentEvent.kind == borealis::net::Event::Kind::Accepted) {
            const NetHandle accepted =
                add_handle(*mod, *state, state->currentEvent.accepted, HandleKind::Stream, true);
            output.accepted = accepted;
        } else if (state->currentEvent.kind == borealis::net::Event::Kind::Closed ||
                   state->currentEvent.kind == borealis::net::Event::Kind::Resolved)
        {
            state->handles.erase(state->currentEvent.id);
            s_handles.erase(handle);
        }

        std::memcpy(outEvent, &output, sizeof(output));
        return MOD_OK;
    }

    std::memcpy(outEvent, &output, sizeof(output));
    return MOD_OK;
}

ModResult net_send(ModContext* context, NetHandle handle, const void* data, size_t size) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || (size != 0 && data == nullptr)) {
        return MOD_INVALID_ARGUMENT;
    }
    auto* handleState = s_handles.find_owned(handle, *mod);
    auto* state = find_state(*mod);
    if (handleState == nullptr || state == nullptr ||
        handleState->value.kind != HandleKind::Stream || !handleState->value.openPublished)
    {
        return MOD_UNAVAILABLE;
    }
    const auto* bytes = static_cast<const std::byte*>(data);
    return map_send_result(state->context.send(handleState->value.socket, {bytes, size}));
}

ModResult net_send_to(
    ModContext* context, NetHandle handle, const char* endpoint, const void* data, size_t size) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || endpoint == nullptr || (size != 0 && data == nullptr)) {
        return MOD_INVALID_ARGUMENT;
    }
    auto* handleState = s_handles.find_owned(handle, *mod);
    auto* state = find_state(*mod);
    if (handleState == nullptr || state == nullptr ||
        handleState->value.kind != HandleKind::Datagram)
    {
        return MOD_UNAVAILABLE;
    }
    const auto* bytes = static_cast<const std::byte*>(data);
    return map_send_result(
        state->context.send_to(handleState->value.socket, endpoint, {bytes, size}));
}

ModResult net_set_user_data(ModContext* context, NetHandle handle, void* userData) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    const auto* handleState = s_handles.find_owned(handle, *mod);
    auto* state = find_state(*mod);
    if (handleState == nullptr || state == nullptr) {
        return MOD_UNAVAILABLE;
    }
    state->context.set_user_data(handleState->value.socket, userData);
    return MOD_OK;
}

ModResult net_stats(ModContext* context, NetHandle handle, NetStats* outStats) {
    const uint32_t structSize = outStats != nullptr ? outStats->struct_size : 0;
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outStats == nullptr || structSize < sizeof(NetStats)) {
        return MOD_INVALID_ARGUMENT;
    }
    NetStats output{.struct_size = structSize};
    const auto* handleState = s_handles.find_owned(handle, *mod);
    auto* state = find_state(*mod);
    if (handleState == nullptr || state == nullptr) {
        std::memcpy(outStats, &output, sizeof(output));
        return MOD_UNAVAILABLE;
    }
    const auto stats = state->context.stats(handleState->value.socket);
    if (!stats) {
        std::memcpy(outStats, &output, sizeof(output));
        return MOD_UNAVAILABLE;
    }
    output.queued_send_bytes = stats->queuedSendBytes;
    output.inbound_dropped = stats->inboundDropped;
    output.send_failures = stats->sendFailures;
    output.bytes_sent = stats->bytesSent;
    output.bytes_received = stats->bytesReceived;
    std::memcpy(outStats, &output, sizeof(output));
    return MOD_OK;
}

ModResult net_close(ModContext* context, NetHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    auto* handleState = s_handles.find_owned(handle, *mod);
    auto* state = find_state(*mod);
    if (handleState == nullptr || state == nullptr) {
        return MOD_UNAVAILABLE;
    }
    state->context.close(handleState->value.socket);
    return MOD_OK;
}

void net_mod_deactivating(LoadedMod& mod) {
    (void)s_handles.take_all(mod);
    s_modStates.erase(mod);
}

void net_mod_detached(LoadedMod& mod) {
    assert(find_state(mod) == nullptr);
    bool found = false;
    s_handles.for_each([&](NetHandle, const auto& entry) { found = found || entry.owner == &mod; });
    assert(!found);
}

void net_shutdown() {
    s_modStates.clear();
    s_handles = {};
}

bool net_available() {
    return borealis::net::available();
}

constexpr NetService s_netService{
    .header = SERVICE_HEADER(NetService, NET_SERVICE_MAJOR, NET_SERVICE_MINOR),
    .connect = SERVICE_FUNCTION(net_connect),
    .listen = SERVICE_FUNCTION(net_listen),
    .open_datagram = SERVICE_FUNCTION(net_open_datagram),
    .resolve = SERVICE_FUNCTION(net_resolve),
    .poll_event = SERVICE_FUNCTION(net_poll_event),
    .send = SERVICE_FUNCTION(net_send),
    .send_to = SERVICE_FUNCTION(net_send_to),
    .set_user_data = SERVICE_FUNCTION(net_set_user_data),
    .stats = SERVICE_FUNCTION(net_stats),
    .close = SERVICE_FUNCTION(net_close),
};

}  // namespace

constinit const ServiceModule g_netModule{
    .id = NET_SERVICE_ID,
    .majorVersion = NET_SERVICE_MAJOR,
    .minorVersion = NET_SERVICE_MINOR,
    .service = &s_netService,
    .available = net_available,
    .modDeactivating = net_mod_deactivating,
    .modDetached = net_mod_detached,
    .shutdown = net_shutdown,
};

}  // namespace dusk::mods::svc
