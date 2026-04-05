#include "dusk/settings.h"

namespace dusk {

UserSettings g_userSettings = {
    // Program settings

    // Video
    .video = {
        .enableFullscreen = false,
    },

    // Audio
    .audio = {
        .masterVolume = 0.8f,
        .mainMusicVolume = 1.0f,
        .subMusicVolume = 1.0f,
        .soundEffectsVolume = 1.0f,
        .fanfareVolume = 1.0f,
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
