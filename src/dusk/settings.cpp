#include "dusk/settings.h"
#include "dusk/config.hpp"

namespace dusk::settings {
namespace video {
ConfigVar<bool> enableFullscreen("video.enableFullscreen", false);
}

namespace audio {
ConfigVar<int> masterVolume("audio.masterVolume", 80);
ConfigVar<int> mainMusicVolume("audio.mainMusicVolume", 100);
ConfigVar<int> subMusicVolume("audio.subMusicVolume", 100);
ConfigVar<int> soundEffectsVolume("audio.soundEffectsVolume", 100);
ConfigVar<int> fanfareVolume("audio.fanfareVolume", 100);
ConfigVar<bool> enableReverb("audio.enableReverb", true);
}

namespace game {
// Quality of Life
ConfigVar<bool> enableQuickTransform("game.enableQuickTransform", false);
ConfigVar<bool> hideTvSettingsScreen("game.hideTvSettingsScreen", false);
ConfigVar<bool> biggerWallets("game.biggerWallets", false);
ConfigVar<bool> noReturnRupees("game.noReturnRupees", false);
ConfigVar<bool> disableRupeeCutscenes("game.disableRupeeCutscenes", false);
ConfigVar<bool> noSwordRecoil("game.noSwordRecoil", false);
ConfigVar<int> damageMultiplier("game.damageMultiplier", 1);
ConfigVar<bool> instantDeath("game.instantDeath", false);
ConfigVar<bool> fastClimbing("game.fastClimbing", false);
ConfigVar<bool> fastTears("game.fastTears", false);
ConfigVar<bool> noMissClimbing("game.noMissClimbing", false);

// Preferences
ConfigVar<bool> enableMirrorMode("game.enableMirrorMode", false);
ConfigVar<bool> invertCameraXAxis("game.invertCameraXAxis", false);

// Graphics
ConfigVar<bool> enableBloom("game.enableBloom", true);
ConfigVar<bool> useWaterProjectionOffset("game.useWaterProjectionOffset", false);

// Audio
ConfigVar<bool> noLowHpSound("game.noLowHpSound", false);
ConfigVar<bool> midnasLamentNonStop("game.midnasLamentNonStop", false);

// Cheats
ConfigVar<bool> enableFastIronBoots("game.enableFastIronBoots", false);
ConfigVar<bool> canTransformAnywhere("game.canTransformAnywhere", false);

// Technical
ConfigVar<bool> restoreWiiGlitches("game.restoreWiiGlitches", false);

// Controls
ConfigVar<bool> enableTurboKeybind("game.enableTurboKeybind", true);
}

void Register() {
    // Video
    Register(video::enableFullscreen);

    // Audio
    Register(audio::masterVolume);
    Register(audio::mainMusicVolume);
    Register(audio::subMusicVolume);
    Register(audio::soundEffectsVolume);
    Register(audio::fanfareVolume);
    Register(audio::enableReverb);

    // Game
    Register(game::enableQuickTransform);
    Register(game::hideTvSettingsScreen);
    Register(game::biggerWallets);
    Register(game::noReturnRupees);
    Register(game::disableRupeeCutscenes);
    Register(game::noSwordRecoil);
    Register(game::damageMultiplier);
    Register(game::instantDeath);
    Register(game::fastClimbing);
    Register(game::fastTears);
    Register(game::enableMirrorMode);
    Register(game::invertCameraXAxis);
    Register(game::enableBloom);
    Register(game::useWaterProjectionOffset);
    Register(game::enableFastIronBoots);
    Register(game::canTransformAnywhere);
    Register(game::restoreWiiGlitches);
    Register(game::noMissClimbing);
    Register(game::noLowHpSound);
    Register(game::midnasLamentNonStop);
    Register(game::enableTurboKeybind);
}
}

namespace dusk {

// Transient settings

static TransientSettings g_transientSettings = {
    .collisionView = {
        .enableTerrainView = false,
        .enableWireframe = false,
        .enableAtView = false,
        .enableTgView = false,
        .enableCoView = false,
        .terrainViewOpacity = 50.0f,
        .colliderViewOpacity = 50.0f,
        .drawRange = 100.0f,
    },
    .skipFrameRateLimit = false,
};

TransientSettings& getTransientSettings() {
    return g_transientSettings;
}

}
