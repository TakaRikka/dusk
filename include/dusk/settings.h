#ifndef DUSK_CONFIG_H
#define DUSK_CONFIG_H

#include "dusk/config_var.hpp"

namespace dusk {

using namespace config;

enum class BloomMode : int {
    Off = 0,
    Classic = 1,
    Dusk = 2,
};

namespace config {
template <>
struct ConfigEnumRange<BloomMode> {
    static constexpr auto min = BloomMode::Off;
    static constexpr auto max = BloomMode::Dusk;
};
}

// Persistent user settings

struct UserSettings {
    // Program settings

    struct {
        // Video
        ConfigVar<bool> enableFullscreen;
        ConfigVar<bool> enableVsync;
        ConfigVar<bool> lockAspectRatio;
    } video;

    struct {
        // Audio
        ConfigVar<int> masterVolume;
        ConfigVar<int> mainMusicVolume;
        ConfigVar<int> subMusicVolume;
        ConfigVar<int> soundEffectsVolume;
        ConfigVar<int> fanfareVolume;
        ConfigVar<bool> enableReverb;
    } audio;

    // Game settings

    struct {
        // QoL
        ConfigVar<bool> enableQuickTransform;
        ConfigVar<bool> hideTvSettingsScreen;
        ConfigVar<bool> skipWarningScreen;
        ConfigVar<bool> biggerWallets;
        ConfigVar<bool> noReturnRupees;
        ConfigVar<bool> disableRupeeCutscenes;
        ConfigVar<bool> noSwordRecoil;
        ConfigVar<int> damageMultiplier;
        ConfigVar<bool> noHeartDrops;
        ConfigVar<bool> instantDeath;
        ConfigVar<bool> fastClimbing;
        ConfigVar<bool> noMissClimbing;
        ConfigVar<bool> fastTears;
        ConfigVar<bool> instantSaves;
        ConfigVar<bool> sunsSong;

        // Preferences
        ConfigVar<bool> enableMirrorMode;
        ConfigVar<bool> invertCameraXAxis;
        ConfigVar<bool> disableMainHUD;

        // Graphics
        ConfigVar<BloomMode> bloomMode;
        ConfigVar<float> bloomMultiplier;
        ConfigVar<bool> enableWaterRefraction;
        ConfigVar<bool> enableFrameInterpolation;
        ConfigVar<int> shadowResolutionMultiplier;

        // Audio
        ConfigVar<bool> noLowHpSound;
        ConfigVar<bool> midnasLamentNonStop;

        // Input
        ConfigVar<bool> enableGyroAim;
        ConfigVar<float> gyroAimSensitivityX;
        ConfigVar<float> gyroAimSensitivityY;
        ConfigVar<bool> gyroAimInvertPitch;
        ConfigVar<bool> gyroAimInvertYaw;

        // Cheats
        ConfigVar<bool> enableFastIronBoots;
        ConfigVar<bool> canTransformAnywhere;
        ConfigVar<bool> fastSpinner;
        ConfigVar<bool> freeMagicArmor;

        // Technical
        ConfigVar<bool> restoreWiiGlitches;

        // Controls
        ConfigVar<bool> enableTurboKeybind;
    } game;

    struct {
        ConfigVar<std::string> isoPath;
        ConfigVar<std::string> graphicsBackend;
        ConfigVar<bool> skipPreLaunchUI;
        ConfigVar<bool> showPipelineCompilation;
        ConfigVar<bool> wasPresetChosen;
        ConfigVar<bool> enableCrashReporting;
    } backend;
};

UserSettings& getSettings();

void registerSettings();

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
    bool moveLinkActive;
};

TransientSettings& getTransientSettings();

}

#endif // DUSK_CONFIG_H
