#include "mods/service.hpp"
#include "mods/svc/log.h"

#include "hooks.hpp"
#include "item.hpp"
#include "session.hpp"
#include "ui/ui.hpp"

DEFINE_MOD();
IMPORT_SERVICE(HostService, svc_host);
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(HookService, svc_hook);
IMPORT_SERVICE(UiService, svc_ui);
IMPORT_SERVICE(ResourceService, svc_res);
IMPORT_SERVICE(ConfigService, svc_config);
IMPORT_SERVICE(SaveService, svc_save);
IMPORT_SERVICE(StageService, svc_stage);
IMPORT_SERVICE(ItemService, svc_item);

extern "C" {

MOD_EXPORT ModResult mod_initialize(ModError* error) {
    ModResult result = randomizer::session::initialize({
        mod_ctx,
        svc_host,
        svc_log,
        svc_hook,
        svc_ui,
        svc_res,
        svc_config,
        svc_save,
        svc_stage,
        svc_item,
    });
    if (result != MOD_OK) {
        return mods::set_error(error, result, "failed to initialize session");
    }

    result = randomizer::hooks::initialize();
    if (result != MOD_OK) {
        return mods::set_error(error, result, "failed to initialize hooks");
    }

    result = randomizer::ui::initialize();
    if (result != MOD_OK) {
        return mods::set_error(error, result, "failed to initialize ui");
    }

    svc_log->info(mod_ctx, "randomizer initialized");
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    randomizer::ui::update();
    randomizer::session::update();
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    randomizer::session::deactivateSeed();
    randomizer::hooks::uninstall();
    svc_log->info(mod_ctx, "randomizer unloaded");
    return MOD_OK;
}
}
