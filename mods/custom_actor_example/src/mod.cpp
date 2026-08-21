
#include "mods/service.hpp"
#include "mods/svc/actor.h"
#include "mods/svc/log.hpp"
#include "mods/svc/stage.h"

#include "d/d_com_inf_game.h"

#include "m_a_obj_wrock.hpp"

DEFINE_MOD();
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(ActorService, svc_actor);
IMPORT_SERVICE(StageService, svc_stage);

extern "C" {
MOD_EXPORT ModResult mod_initialize(ModError* error) {
    if (svc_actor->register_actor(mod_ctx, &maObj_Wrock_c::sProfile, &maObj_Wrock_c::sProcName,
            &maObj_Wrock_c::sActorHandle) != MOD_OK)
    {
        mods::log::error("Failed to register actor wrock!");
        return MOD_ERROR;
    }

    StageActorHandle wrock_faron_woods_handle;
    stage_actor_data_class wrockParams = stage_actor_data_class{.name = MAOBJ_WROCK_NAME,
        .base = {.parameters = 0,
            .position = {-14324.0f, 0.0f, 341.0f},
            .angle = {0, -16595, 0},
            .setID = 0xFFFF}};
    
    
    if (svc_stage->add_actor(mod_ctx, "F_SP108", 0, -1, &wrockParams, sizeof(wrockParams), &wrock_faron_woods_handle) != MOD_OK) {
        mods::log::error("Adding wrock to F_SP108 Failed!");
        return MOD_ERROR;
    }

    mods::log::info("custom_actor_example initialized");
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    // All custom actors will automatically be unregistered and unloaded by the service when the mod
    // is shutdown
    mods::log::info("custom_actor_example shutdown");
    return MOD_OK;
}
}
