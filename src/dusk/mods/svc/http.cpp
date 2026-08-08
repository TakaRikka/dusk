#include "http_core.hpp"
#include "registry.hpp"

#include "dusk/mods/loader/loader.hpp"

#include <memory>

namespace dusk::mods::svc {
namespace {

std::unique_ptr<HttpCore> s_core;

ModResult http_begin(ModContext* context, const HttpRequestDesc* desc,
    const HttpCompletionFn callback, void* userData, HttpRequestHandle* outHandle) {
    return s_core != nullptr ? s_core->begin(context, desc, callback, userData, outHandle)
                             : MOD_UNAVAILABLE;
}

ModResult http_cancel(ModContext* context, const HttpRequestHandle handle) {
    return s_core != nullptr ? s_core->cancel(context, handle) : MOD_UNAVAILABLE;
}

constexpr HttpService s_httpService{
    .header = SERVICE_HEADER(HttpService, HTTP_SERVICE_MAJOR, HTTP_SERVICE_MINOR),
    .begin = http_begin,
    .cancel = http_cancel,
};

}  // namespace

constinit const ServiceModule g_httpModule{
    .id = HTTP_SERVICE_ID,
    .majorVersion = HTTP_SERVICE_MAJOR,
    .minorVersion = HTTP_SERVICE_MINOR,
    .service = &s_httpService,
    .initialize = [] { s_core = std::make_unique<HttpCore>(); },
    .modDetached = [](LoadedMod& mod) {
        if (s_core != nullptr) {
            s_core->modDetached(mod.context.get());
        }
    },
    .frameBegin = [] {
        if (s_core != nullptr) {
            s_core->frameBegin();
        }
    },
    .shutdown = [] { s_core.reset(); },
};

}  // namespace dusk::mods::svc
