#include "mods/service.hpp"
#include "mods/svc/actor.h"
#include "mods/svc/log.hpp"
#include "mods/svc/stage.h"

#include "d/d_com_inf_game.h"

#include "m_a_mine.hpp"
#include "m_a_obj_wrock.hpp"

#include <array>

DEFINE_MOD();
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(ActorService, svc_actor);
IMPORT_SERVICE(StageService, svc_stage);

extern "C" {
MOD_EXPORT ModResult mod_initialize(ModError*) {
    if (svc_actor->register_actor(mod_ctx, &maObj_Wrock_c::sProfile, &maObj_Wrock_c::sProcName,
            &maObj_Wrock_c::sActorHandle) != MOD_OK)
    {
        mods::log::error("Failed to register actor wrock!");
        return MOD_ERROR;
    }

    const stage_actor_data_class wrockParams{
        .name = MAOBJ_WROCK_NAME,
        .base =
            {
                .parameters = 0,
                .position = {-14324.0f, 0.0f, 341.0f},
                .angle = {0, -16595, 0},
                .setID = 0xFFFF,
            },
    };
    if (svc_stage->add_actor(
            mod_ctx, "F_SP108", 0, -1, &wrockParams, sizeof(wrockParams), nullptr) != MOD_OK)
    {
        mods::log::error("Adding wrock to F_SP108 Failed!");
        return MOD_ERROR;
    }

    if (svc_actor->register_actor(mod_ctx, &ma_Mine_c::sProfile, &ma_Mine_c::sProcName,
            &ma_Mine_c::sActorHandle) != MOD_OK)
    {
        mods::log::error("Failed to register actor " MA_MINE_NAME);
        return MOD_ERROR;
    }

    static const std::array<cXyz, 6> minePositions = {
        {{-14445.0f, 11.0f, 1304.0f}, {-14557.0f, 7.0f, 990.0f}, {-14790.0f, 7.0f, 634.0f},
            {-14829.0f, 0.0f, 144.0f}, {-14615.0f, 0.0f, -170.0f}, {-14283.0f, 10.0f, -420.0f}}};
    for (const auto& pos : minePositions) {
        const stage_actor_data_class mineParams{
            .name = MA_MINE_NAME,
            .base =
                {
                    .parameters = 0,
                    .position = {pos.x, pos.y, pos.z},
                    .angle = {0x2000, (s16)(pos.x*100000), 0}, // Adjusted and seemingly random angle
                    .setID = 0xFFFF,
                },
        };
        if (svc_stage->add_actor(
                mod_ctx, "F_SP108", 0, -1, &mineParams, sizeof(mineParams), nullptr) != MOD_OK)
        {
            mods::log::error("Adding mine to F_SP108 Failed!");
            return MOD_ERROR;
        }
    }

    mods::log::info("custom_actor_demo initialized");
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    mods::log::info("custom_actor_demo shutdown");
    return MOD_OK;
}
}
