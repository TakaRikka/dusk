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

        // Preferences
        .enableMirrorMode = false,
        .invertCameraXAxis = false,

        // Graphics
        .enableBloom = true,
        .useWaterProjectionOffset = false,

        // Cheats
        .enableFastIronBoots = false,

        // Technical
        .restoreWiiGlitches = false,
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
};

TransientSettings& getTransientSettings() {
    return g_transientSettings;
}

}
