#include "mods/service.hpp"
#include "mods/svc/audio_res.h"
#include "mods/svc/log.h"

DEFINE_MOD();
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(AudioResService, svc_audio_res);

extern "C" {

MOD_EXPORT ModResult mod_initialize(ModError*) {
    svc_log->info(mod_ctx, "template_mod initialized");
    AudioWaveHandle handle;
    svc_audio_res->replace_wave(
        mod_ctx,
        AUDIO_WAVE_BANK_SOUND_EFFECTS,
        4238,
        "res/go.opus",
        nullptr,
        &handle);
    svc_audio_res->replace_wave(
        mod_ctx,
        AUDIO_WAVE_BANK_SOUND_EFFECTS,
        4237,
        "res/go.opus",
        nullptr,
        &handle);

    constexpr AudioSoundTableEffectInfo effect(128, 1, 1.5);

    svc_audio_res->replace_sound_table_effect(
        mod_ctx,
        SE_CATEGORY_CHARA_SE,
        0, // MIDNA_APPEAR
        &effect,
        nullptr);

    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    svc_log->info(mod_ctx, "template_mod unloaded");
    return MOD_OK;
}
}
