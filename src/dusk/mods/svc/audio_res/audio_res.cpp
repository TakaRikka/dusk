#include "audio_res.hpp"

#include "helpers/cast.hpp"
#include "mods/svc/audio_res.h"

#include "../registry.hpp"
#include "wsys.hpp"

namespace dusk::mods::svc {

namespace audio_res {
namespace {

using namespace dusk::helpers::cast;

constexpr AudioResService s_audioResService{
    .header = SERVICE_HEADER(AudioResService, AUDIO_RES_SERVICE_MAJOR, AUDIO_RES_SERVICE_MINOR),
    .replace_wave = &wsys::insert_replace_wave,
    .add_wave = &wsys::insert_add_wave,
    .remove_wave = &wsys::remove_wave,
};

void frame_end() {
    wsys::frame_end();
}

void mod_detached(LoadedMod& mod) {
    wsys::remove_mod(mod);
}

void sync_audio_replacements() {
    wsys::sync_audio_replacements();
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

