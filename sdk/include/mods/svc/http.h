#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#define HTTP_SERVICE_ID "dev.twilitrealm.dusklight.http"
#define HTTP_SERVICE_MAJOR 1u
#define HTTP_SERVICE_MINOR 0u

/* Handle for an in-flight request. 0 is never a valid handle. */
typedef uint64_t HttpRequestHandle;

typedef enum HttpMethod {
    HTTP_METHOD_GET = 0,
    HTTP_METHOD_POST = 1,
    HTTP_METHOD_HEAD = 2,
} HttpMethod;

/* Transport-level outcome. HTTP status errors are reported through status_code. */
typedef enum HttpError {
    HTTP_ERROR_NONE = 0,
    HTTP_ERROR_INVALID_URL = 1,
    HTTP_ERROR_UNSUPPORTED_SCHEME = 2,
    HTTP_ERROR_TIMEOUT = 3,
    HTTP_ERROR_TOO_LARGE = 4,
    HTTP_ERROR_CANCELED = 5,
    HTTP_ERROR_IO = 6,
    HTTP_ERROR_NETWORK = 7,
} HttpError;

typedef struct HttpHeader {
    const char* name;
    const char* value;
} HttpHeader;

typedef struct HttpRequestDesc {
    uint32_t struct_size;
    HttpMethod method;
    const char* url;
    const HttpHeader* headers;
    uint32_t header_count;
    /* Request body; POST only. */
    const void* body;
    size_t body_size;
    /* Absolute destination under this mod's data_dir or mod_dir, or NULL for an in-memory body.
     * GET and POST only. */
    const char* download_path;
    uint32_t connect_timeout_ms; /* 0 = 10 seconds */
    uint32_t idle_timeout_ms;    /* 0 = 10 seconds without network progress */
    uint32_t total_timeout_ms;   /* 0 = no total timeout */
    size_t max_body_bytes;       /* 0 = 1 MiB; ignored for downloads */
} HttpRequestDesc;

#define HTTP_REQUEST_DESC_INIT                                                                     \
    {sizeof(HttpRequestDesc), HTTP_METHOD_GET, NULL, NULL, 0u, NULL, 0u, NULL, 0u, 0u, 0u, 0u}

/* Snapshot valid only for the duration of the completion callback. */
typedef struct HttpResult {
    uint32_t struct_size;
    HttpError error;
    const char* error_message;
    int32_t status_code;
    const HttpHeader* headers;
    uint32_t header_count;
    const void* body;
    size_t body_size;
    /* Published absolute destination, or NULL unless a download succeeded. */
    const char* download_path;
} HttpResult;

/* Runs on the game thread exactly once, unless the calling mod begins deactivation first. */
typedef void (*HttpCompleteFn)(
    ModContext* ctx, HttpRequestHandle request, const HttpResult* result, void* user_data);

typedef struct HttpProgress {
    uint32_t struct_size;
    uint64_t completed_bytes;
    uint64_t total_bytes;
    bool total_known;
} HttpProgress;

#define HTTP_PROGRESS_INIT {sizeof(HttpProgress), 0u, 0u, false}

typedef struct HttpService {
    ServiceHeader header;

    /* Starts an asynchronous HTTPS request. */
    ModResult (*request)(ModContext* ctx, const HttpRequestDesc* desc, HttpCompleteFn fn,
        void* user_data, HttpRequestHandle* out_handle);
    ModResult (*progress)(ModContext* ctx, HttpRequestHandle request, HttpProgress* out_progress);
    /* Requests cancellation. The completion callback still runs if the mod remains active. */
    ModResult (*cancel)(ModContext* ctx, HttpRequestHandle request);
} HttpService;

MOD_DECLARE_SERVICE(HttpService, svc_http, HTTP_SERVICE_ID, HTTP_SERVICE_MAJOR, HTTP_SERVICE_MINOR);
