#ifndef DUSK_CONFIG_H
#define DUSK_CONFIG_H

namespace dusk {

// Persistent user settings

struct UserSettings {
    // Program settings

    struct {
        // Video
        bool enableFullscreen;
    } video;

    struct {
        // Audio
        float masterVolume;
        float mainMusicVolume;
        float subMusicVolume;
        float soundEffectsVolume;
        float fanfareVolume;
    } audio;

    // Game settings

    struct {
        // QoL
        bool enableQuickTransform;
        bool hideTvSettingsScreen;
        bool biggerWallets;
        bool noReturnRupees;
        bool disableRupeeCutscenes;

        // Preferences
        bool enableMirrorMode;
        bool invertCameraXAxis;

        // Graphics
        bool enableBloom;
        bool useWaterProjectionOffset;

        // Cheats
        bool enableFastIronBoots;

        // Technical
        bool restoreWiiGlitches;
    } game;
};

UserSettings& getSettings();

// Transient settings

struct CollisionViewSettings {
    bool enableTerrainView;
    bool enableWireframe;
    bool enableAtView;
    bool enableTgView;
    bool enableCoView;
    float terrainViewOpacity;
    float colliderViewOpacity;
    float drawRange;
};

struct TransientSettings {
    CollisionViewSettings collisionView;
};

TransientSettings& getTransientSettings();

}

#endif // DUSK_CONFIG_H

