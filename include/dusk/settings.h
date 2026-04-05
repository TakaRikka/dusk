#ifndef DUSK_CONFIG_H
#define DUSK_CONFIG_H

namespace dusk {

// Persistent user settings

struct UserSettings {
    // Program settings

    struct {
        // Video
        bool enableFullscreen;
        bool lockAspectRatio;
    } video;

    struct {
        // Audio
        int masterVolume;
        int mainMusicVolume;
        int subMusicVolume;
        int soundEffectsVolume;
        int fanfareVolume;
        bool enableReverb;
    } audio;

    // Game settings

    struct {
        // QoL
        bool enableQuickTransform;
        bool hideTvSettingsScreen;
        bool biggerWallets;
        bool noReturnRupees;
        bool disableRupeeCutscenes;
        bool noSwordRecoil;
        int damageMultiplier;
        bool instantDeath;
        bool fastClimbing;
        bool noMissClimbing;
        bool fastTears;

        // Preferences
        bool enableMirrorMode;
        bool invertCameraXAxis;

        // Graphics
        bool enableBloom;
        bool useWaterProjectionOffset;

        // Audio
        bool noLowHpSound;   
        bool midnasLamentNonStop;

        // Cheats
        bool enableFastIronBoots;
        bool canTransformAnywhere;

        // Technical
        bool restoreWiiGlitches;

        // Controls
        bool enableTurboKeybind;
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
    bool skipFrameRateLimit;
};

TransientSettings& getTransientSettings();

}

#endif // DUSK_CONFIG_H
