#include "dusk/settings.h"

namespace dusk {

UserSettings g_userSettings = {
    // Program settings

    // Video
    .video = {
        .enableFullscreen = false,
        .lockAspectRatio = false,
    },

    // Audio
    .audio = {
        .masterVolume = 80,
        .mainMusicVolume = 100,
        .subMusicVolume = 100,
        .soundEffectsVolume = 100,
        .fanfareVolume = 100,
        .enableReverb = true
    },

    // Game settings
    .game = {
        // Quality of Life
        .enableQuickTransform = false,
        .hideTvSettingsScreen = false,
        .biggerWallets = false,
        .noReturnRupees = false,
        .disableRupeeCutscenes = false,
        .noSwordRecoil = false,
        .damageMultiplier = 1,
        .instantDeath = false,
        .fastClimbing = false,
        .noMissClimbing = false,
        .fastTears = false,

        // Preferences
        .enableMirrorMode = false,
        .invertCameraXAxis = false,

        // Graphics
        .enableBloom = true,
        .useWaterProjectionOffset = false,

        // Audio
        .noLowHpSound = false,
        .midnasLamentNonStop = false,

        // Cheats
        .enableFastIronBoots = false,
        .canTransformAnywhere = false,

        // Technical
        .restoreWiiGlitches = false,

        // Controls
        .enableTurboKeybind = true,
    }
};

UserSettings& getSettings() {
    return g_userSettings;
}

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
