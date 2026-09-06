#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#define NET_SERVICE_ID "dev.twilitrealm.dusklight.net"
#define NET_SERVICE_MAJOR 1u
#define NET_SERVICE_MINOR 0u

/** 0 is never a valid handle. */
typedef uint64_t NetHandle;

typedef enum NetError {
    NET_ERROR_NONE = 0,
    NET_ERROR_INVALID_ENDPOINT = 1,
    NET_ERROR_RESOLVE = 2,
    NET_ERROR_TIMEOUT = 3,
    NET_ERROR_REFUSED = 4,
    NET_ERROR_UNREACHABLE = 5,
    NET_ERROR_RESET = 6,
    NET_ERROR_ADDRESS_IN_USE = 7,
    NET_ERROR_PERMISSION = 8,
    NET_ERROR_TOO_LARGE = 9,
    NET_ERROR_CANCELED = 10,
    NET_ERROR_NETWORK = 11,
} NetError;

#define NET_ENDPOINT_MAX 80
/** NUL-terminated tcp://host:port or udp://host:port endpoint. */
typedef struct NetEndpoint {
    char text[NET_ENDPOINT_MAX];
} NetEndpoint;

typedef struct NetConnectDesc {
    uint32_t struct_size;
    /** TCP endpoint. Hostnames and IP literals are accepted. */
    const char* endpoint;
    /** 0 defaults to 10 seconds. Resolution is included. */
    uint32_t connect_timeout_ms;
    /** 0 defaults to 5 seconds for flush and peer EOF. */
    uint32_t close_timeout_ms;
    /** 0 defaults to 1 MiB. Maximum of 8 MiB. */
    size_t max_send_queue_bytes;
    bool no_delay;
    /** Sampled when each event is polled. */
    void* user_data;
} NetConnectDesc;

#define NET_CONNECT_DESC_INIT {sizeof(NetConnectDesc), NULL, 0u, 0u, 0u, true, NULL}

typedef struct NetListenDesc {
    uint32_t struct_size;
    /** TCP endpoint with an IP literal. Port 0 requests an ephemeral port. */
    const char* bind;
    uint32_t close_timeout_ms;
    size_t max_send_queue_bytes;
    bool no_delay;
    void* user_data;
} NetListenDesc;

#define NET_LISTEN_DESC_INIT {sizeof(NetListenDesc), NULL, 0u, 0u, true, NULL}

typedef struct NetDatagramDesc {
    uint32_t struct_size;
    /** UDP endpoint with an IP literal. Port 0 requests an ephemeral port. */
    const char* bind;
    size_t max_send_queue_bytes;
    void* user_data;
} NetDatagramDesc;

#define NET_DATAGRAM_DESC_INIT {sizeof(NetDatagramDesc), NULL, 0u, NULL}

typedef enum NetEventType {
    NET_EVENT_NONE = 0,
    NET_EVENT_CONNECTED = 1,
    NET_EVENT_ACCEPTED = 2,
    NET_EVENT_STREAM_DATA = 3,
    NET_EVENT_DATAGRAM = 4,
    NET_EVENT_DROPPED = 5,
    NET_EVENT_RESOLVED = 6,
    NET_EVENT_CLOSED = 7,
} NetEventType;

typedef struct NetEvent {
    uint32_t struct_size;
    NetEventType type;
    /** Source handle. A CLOSED or RESOLVED handle is invalid after poll_event returns it. */
    NetHandle handle;
    void* user_data;
    NetHandle accepted;
    /** Peer, datagram source, or resolved endpoint as applicable. */
    NetEndpoint endpoint;
    /** Valid until this mod's next poll_event call or deactivation. */
    const void* data;
    size_t size;
    uint32_t dropped;
    NetError error;
    /** Never NULL. Valid until this mod's next poll_event call or deactivation. */
    const char* error_message;
} NetEvent;

#define NET_EVENT_INIT                                                                             \
    {sizeof(NetEvent), NET_EVENT_NONE, 0u, NULL, 0u, {{0}}, NULL, 0u, 0u, NET_ERROR_NONE, ""}

typedef struct NetStats {
    uint32_t struct_size;
    size_t queued_send_bytes;
    uint64_t inbound_dropped;
    uint64_t send_failures;
    uint64_t bytes_sent;
    uint64_t bytes_received;
} NetStats;

#define NET_STATS_INIT {sizeof(NetStats), 0u, 0u, 0u, 0u, 0u}

typedef struct NetService {
    ServiceHeader header;

    /** Starts an asynchronous TCP connection. */
    ModResult (*connect)(ModContext* ctx, const NetConnectDesc* desc, NetHandle* out_handle);
    /** Opens a TCP listener and returns its local endpoint. */
    ModResult (*listen)(ModContext* ctx, const NetListenDesc* desc, NetHandle* out_handle,
        NetEndpoint* out_local, NetError* out_error);
    /** Opens a UDP socket and returns its local endpoint. */
    ModResult (*open_datagram)(ModContext* ctx, const NetDatagramDesc* desc, NetHandle* out_handle,
        NetEndpoint* out_local, NetError* out_error);
    /** Resolves a TCP or UDP endpoint asynchronously. */
    ModResult (*resolve)(
        ModContext* ctx, const char* endpoint, void* user_data, NetHandle* out_handle);
    /** Returns MOD_OK and NET_EVENT_NONE when the queue is empty. */
    ModResult (*poll_event)(ModContext* ctx, NetEvent* out_event);
    /** Copies bytes to a connected stream's outbound queue. */
    ModResult (*send)(ModContext* ctx, NetHandle stream, const void* data, size_t size);
    /** Copies one datagram for a literal UDP destination. The maximum size is 65,507 bytes. */
    ModResult (*send_to)(
        ModContext* ctx, NetHandle socket, const char* endpoint, const void* data, size_t size);
    ModResult (*set_user_data)(ModContext* ctx, NetHandle handle, void* user_data);
    ModResult (*stats)(ModContext* ctx, NetHandle handle, NetStats* out_stats);
    ModResult (*close)(ModContext* ctx, NetHandle handle);
} NetService;

MOD_DECLARE_SERVICE(NetService, svc_net, NET_SERVICE_ID, NET_SERVICE_MAJOR, NET_SERVICE_MINOR);
