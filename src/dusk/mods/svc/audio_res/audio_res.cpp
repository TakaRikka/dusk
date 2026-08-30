#include "audio_res.hpp"

#include "helpers/cast.hpp"
#include "mods/svc/audio_res.h"

#include "../registry.hpp"

namespace dusk::mods::svc {

namespace audio_res {
namespace {

using namespace dusk::helpers::cast;

constexpr AudioResService s_audioResService{
    .header = SERVICE_HEADER(AudioResService, AUDIO_RES_SERVICE_MAJOR, AUDIO_RES_SERVICE_MINOR),
    .default_wave_info = &wsys::default_wave_info,
    .replace_wave = &wsys::insert_replace_wave,
    .add_wave = &wsys::insert_add_wave,
    .remove_wave = &wsys::remove_wave,
    .default_effect_info = &bst::default_effect_info,
    .replace_sound_table_effect = &bst::replace_sound_table_effect,
    .add_sound_table_effect = &bst::add_sound_table_effect,
    .default_stream_info = &bst::default_stream_info,
    .replace_sound_table_stream = &bst::replace_sound_table_stream,
    .add_sound_table_stream = &bst::add_sound_table_stream,
    .remove_sound_table = &bst::remove_sound_table,
};

void frame_end() {
    wsys::frame_end();
    bst::frame_end();
}

void mod_detached(LoadedMod& mod) {
    wsys::remove_mod(mod);
    bst::remove_mod(mod);
}

void sync_audio_replacements() {
    wsys::sync_audio_replacements();
    bst::sync_audio_replacements();
}

}

}  // namespace audio_res

constinit const ServiceModule g_audioResModule{
    .id = AUDIO_RES_SERVICE_ID,
    .majorVersion = AUDIO_RES_SERVICE_MAJOR,
    .minorVersion = AUDIO_RES_SERVICE_MINOR,
    .service = &audio_res::s_audioResService,
    .modDetached = audio_res::mod_detached,
    .lifecycleApplied = audio_res::sync_audio_replacements,
    .frameEnd = audio_res::frame_end
    };
};

