#include "mods/svc/http.h"

static void completion(ModContext* context, HttpRequestHandle handle,
    const HttpResponseView* response, void* user_data) {
    (void)context;
    (void)handle;
    (void)response;
    (void)user_data;
}

void http_c_abi_compile_check(void) {
    HttpHeader header = HTTP_HEADER_INIT;
    HttpRequestDesc request = HTTP_REQUEST_DESC_INIT;
    HttpRequestHandle handle = {0};
    HttpService service = {
        SERVICE_HEADER(HttpService, HTTP_SERVICE_MAJOR, HTTP_SERVICE_MINOR),
        0,
        0,
    };
    request.headers = &header;
    request.header_count = 1;
    service.begin = 0;
    service.cancel = 0;
    completion(0, handle, 0, 0);
}
