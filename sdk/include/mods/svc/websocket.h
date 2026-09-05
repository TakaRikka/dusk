#pragma once

#include <mods/api.h>
#include <mods/svc/http.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#define WEBSOCKET_SERVICE_ID "dev.twilitrealm.dusklight.websocket"
#define WEBSOCKET_SERVICE_MAJOR 1u
#define WEBSOCKET_SERVICE_MINOR 0u

/** Generational connection handle. Zero is never valid. */
typedef uint64_t WebSocketHandle;

/** Connection outcome. Callers must tolerate values added by later service minors. */
typedef enum WebSocketError {
    WEBSOCKET_ERROR_NONE = 0,
    WEBSOCKET_ERROR_INVALID_URL = 1,
    WEBSOCKET_ERROR_UNSUPPORTED_SCHEME = 2,
    WEBSOCKET_ERROR_TIMEOUT = 3,
    WEBSOCKET_ERROR_TOO_LARGE = 4,
    WEBSOCKET_ERROR_CANCELED = 5,
    WEBSOCKET_ERROR_NETWORK = 6,
    WEBSOCKET_ERROR_PROTOCOL = 7,
    WEBSOCKET_ERROR_HANDSHAKE = 8,
} WebSocketError;

typedef enum WebSocketMessageKind {
    WEBSOCKET_MESSAGE_TEXT = 0,
    WEBSOCKET_MESSAGE_BINARY = 1,
} WebSocketMessageKind;

typedef struct WebSocketConnectDesc {
    uint32_t struct_size;
    /** wss:// URL, or ws:// for localhost, 127.0.0.1, or [::1]. */
    const char* url;
    /** Request headers. WebSocket handshake headers and User-Agent are reserved. */
    const HttpHeader* headers;
    uint32_t header_count;
    const char* const* protocols;
    uint32_t protocol_count;
    uint32_t connect_timeout_ms;
    uint32_t close_timeout_ms;
    uint32_t keepalive_interval_ms;
    /** 0 defaults to 1 MiB. Maximum of 16 MiB. */
    size_t max_message_bytes;
    /** Passed in every event. */
    void* user_data;
} WebSocketConnectDesc;

#define WEBSOCKET_CONNECT_DESC_INIT                                                                \
    {sizeof(WebSocketConnectDesc), NULL, NULL, 0u, NULL, 0u, 0u, 0u, 0u, 0u, NULL}

typedef enum WebSocketEventType {
    WEBSOCKET_EVENT_NONE = 0,
    WEBSOCKET_EVENT_OPEN = 1,
    WEBSOCKET_EVENT_MESSAGE = 2,
    WEBSOCKET_EVENT_CLOSED = 3,
} WebSocketEventType;

typedef struct WebSocketEvent {
    uint32_t struct_size;
    WebSocketEventType type;
    WebSocketHandle ws;
    void* user_data;

    const char* protocol;
    /** Handshake response headers for OPEN or a handshake-rejected CLOSED event. */
    const HttpHeader* headers;
    uint32_t header_count;

    WebSocketMessageKind message_kind;
    /** Valid until this mod's next poll_event call or deactivation. */
    const void* data;
    size_t size;

    WebSocketError error;
    /** Never NULL. Valid until this mod's next poll_event call or deactivation. */
    const char* error_message;
    int32_t handshake_status;
    uint16_t close_code;
    const char* close_reason;
} WebSocketEvent;

#define WEBSOCKET_EVENT_INIT                                                                       \
    {sizeof(WebSocketEvent), WEBSOCKET_EVENT_NONE, 0u, NULL, "", NULL, 0u, WEBSOCKET_MESSAGE_TEXT, \
        NULL, 0u, WEBSOCKET_ERROR_NONE, "", 0, 0u, ""}

typedef struct WebSocketService {
    ServiceHeader header;

    /** Starts a connection. */
    ModResult (*connect)(
        ModContext* ctx, const WebSocketConnectDesc* desc, WebSocketHandle* out_handle);
    /** Returns MOD_OK and WEBSOCKET_EVENT_NONE when the queue is empty. */
    ModResult (*poll_event)(ModContext* ctx, WebSocketEvent* out_event);
    /** Copies a message into the outbound queue. */
    ModResult (*send)(ModContext* ctx, WebSocketHandle ws, WebSocketMessageKind kind,
        const void* data, size_t size);
    /** Code 0 defaults to 1000; accepted: 1000, 1001, and 3000-4999. */
    ModResult (*close)(ModContext* ctx, WebSocketHandle ws, uint16_t code, const char* reason);
} WebSocketService;

MOD_DECLARE_SERVICE(WebSocketService, svc_websocket, WEBSOCKET_SERVICE_ID, WEBSOCKET_SERVICE_MAJOR,
    WEBSOCKET_SERVICE_MINOR);
