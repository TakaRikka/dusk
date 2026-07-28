#include "mods/service.hpp"
#include "mods/svc/log.h"

#include "session.hpp"

DEFINE_MOD();
IMPORT_SERVICE(HostService, svc_host);
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(HookService, svc_hook);
IMPORT_SERVICE(UiService, svc_ui);
IMPORT_SERVICE(ResourceService, svc_res);
IMPORT_SERVICE(ConfigService, svc_config);

extern "C" {

MOD_EXPORT ModResult mod_initialize(ModError* error) {
    ModResult result = randomizer::session::initialize({
        mod_ctx,
        svc_host,
        svc_log,
        svc_hook,
        svc_ui,
        svc_res,
        svc_config
    });
    if (result != MOD_OK) {
        return mods::set_error(error, result, "failed to initialize session");
    }

    svc_log->info(mod_ctx, "randomizer initialized");
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    svc_log->info(mod_ctx, "randomizer unloaded");
    return MOD_OK;
}
}
