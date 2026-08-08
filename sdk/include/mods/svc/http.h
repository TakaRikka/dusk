#pragma once

#include "mods/api.h"

/*
 * Asynchronous HTTPS request service with bounded copied inputs and a single host worker.
 */

#define HTTP_SERVICE_ID "dev.twilitrealm.dusklight.http"
#define HTTP_SERVICE_MAJOR 1u
#define HTTP_SERVICE_MINOR 1u

#define HTTP_URL_MAX_SIZE 4096u
#define HTTP_HEADER_MAX_COUNT 32u
#define HTTP_HEADER_NAME_MAX_SIZE 128u
#define HTTP_HEADER_VALUE_MAX_SIZE 4096u
#define HTTP_HEADER_AGGREGATE_MAX_SIZE (16u * 1024u)
#define HTTP_REQUEST_BODY_MAX_SIZE (64u * 1024u)
#define HTTP_RESPONSE_MAX_SIZE (2u * 1024u * 1024u)

typedef enum HttpMethod {
    HTTP_METHOD_GET = 1,
    HTTP_METHOD_POST = 2,
} HttpMethod;

typedef enum HttpResult {
    HTTP_RESULT_OK = 0,
    HTTP_RESULT_UNAVAILABLE = 1,
    HTTP_RESULT_CANCELED = 2,
    HTTP_RESULT_RESPONSE_TOO_LARGE = 3,
    HTTP_RESULT_FAILED = 4,
    HTTP_RESULT_TIMEOUT = 5,
} HttpResult;

typedef struct HttpHeader {
    uint32_t struct_size;
    const char* name;
    size_t name_size;
    const char* value;
    size_t value_size;
} HttpHeader;

#define HTTP_HEADER_INIT {sizeof(HttpHeader), NULL, 0, NULL, 0}

typedef struct HttpRequestDesc {
    uint32_t struct_size;
    HttpMethod method;
    const char* url;
    size_t url_size;
    const HttpHeader* headers;
    size_t header_count;
    const uint8_t* body;
    size_t body_size;
    uint32_t timeout_ms;
    uint32_t max_response_size;
} HttpRequestDesc;

#define HTTP_REQUEST_DESC_INIT \
    {sizeof(HttpRequestDesc), HTTP_METHOD_GET, NULL, 0, NULL, 0, NULL, 0, 0, 0}

typedef struct HttpRequestHandle {
    uint64_t value;
} HttpRequestHandle;

typedef struct HttpResponseView {
    uint32_t struct_size;
    HttpResult result;
    uint32_t status_code;
    const HttpHeader* headers;
    size_t header_count;
    const uint8_t* body;
    size_t body_size;
} HttpResponseView;

/*
 * The response and every pointer reached from it are borrowed and valid only for this callback.
 * Requests are completed only by the host game-thread frame pump, never by the worker.
 */
typedef void (*HttpCompletionFn)(ModContext* ctx, HttpRequestHandle handle,
    const HttpResponseView* response, void* user_data);

typedef struct HttpService {
    ServiceHeader header;

    /*
     * Copies every input byte before returning. Only exact lowercase "https://" URLs are accepted:
     * every URL byte must be visible ASCII (0x21-0x7e). Header names are HTTP tokens; header-value
     * bytes must be HTAB (0x09) or printable ASCII (0x20-0x7e). GET and POST are the only methods.
     * On success out_handle receives a nonzero caller-owned handle; its completion is delivered once
     * unless the caller's mod detaches first.
     */
    ModResult (*begin)(ModContext* ctx, const HttpRequestDesc* desc, HttpCompletionFn callback,
        void* user_data, HttpRequestHandle* out_handle);

    /*
     * Cancels an accepted queued or running request owned by ctx. A canceled request receives one
     * HTTP_RESULT_CANCELED completion on a later frame pump. Native transports may need to wait for
     * their bounded in-flight call to return before that completion is queued. Other handles are rejected.
     */
    ModResult (*cancel)(ModContext* ctx, HttpRequestHandle handle);
} HttpService;

#ifdef __cplusplus
#include "mods/service.hpp"

template <>
struct dusk::mods::ServiceTraits<HttpService> {
    static constexpr const char* id = HTTP_SERVICE_ID;
    static constexpr uint16_t major_version = HTTP_SERVICE_MAJOR;
};
#endif
