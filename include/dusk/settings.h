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

enum class GameLanguage : u8 {
    English = OS_LANGUAGE_ENGLISH,
    German = OS_LANGUAGE_GERMAN,
    French = OS_LANGUAGE_FRENCH,
    Spanish = OS_LANGUAGE_SPANISH,
    Italian = OS_LANGUAGE_ITALIAN,
};

enum class DiscVerificationState : u8 {
    Unknown = 0,
    Success,
    HashMismatch,
};

enum class GyroMode : u8 {
    Sensor = 0,
    Mouse = 1,
};

namespace config {
template <>
struct ConfigEnumRange<BloomMode> {
    static constexpr auto min = BloomMode::Off;
    static constexpr auto max = BloomMode::Dusk;
};

template <>
struct ConfigEnumRange<GameLanguage> {
    static constexpr auto min = GameLanguage::English;
    static constexpr auto max = GameLanguage::Italian;
};

template <>
struct ConfigEnumRange<DiscVerificationState> {
    static constexpr auto min = DiscVerificationState::Unknown;
    static constexpr auto max = DiscVerificationState::HashMismatch;
};

template <>
struct ConfigEnumRange<GyroMode> {
    static constexpr auto min = GyroMode::Sensor;
    static constexpr auto max = GyroMode::Mouse;
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
        ConfigVar<bool> enableFpsOverlay;
        ConfigVar<int> fpsOverlayCorner;
    } video;

    struct {
        // Audio
        ConfigVar<int> masterVolume;
        ConfigVar<int> mainMusicVolume;
        ConfigVar<int> subMusicVolume;
        ConfigVar<int> soundEffectsVolume;
        ConfigVar<int> fanfareVolume;
        ConfigVar<bool> enableReverb;
        ConfigVar<bool> enableHrtf;
        ConfigVar<bool> menuSounds;
    } audio;

    // Game settings

    struct {
        ConfigVar<GameLanguage> language;

        // QoL
        ConfigVar<bool> enableQuickTransform;
        ConfigVar<bool> hideTvSettingsScreen;
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
        ConfigVar<bool> no2ndFishForCat;
        ConfigVar<bool> instantSaves;
        ConfigVar<bool> instantText;
        ConfigVar<bool> sunsSong;
        ConfigVar<bool> autoSave;

        // Preferences
        ConfigVar<bool> enableMirrorMode;
        ConfigVar<bool> minimalHUD;
        ConfigVar<bool> pauseOnFocusLost;
        ConfigVar<bool> enableLinkDollRotation;
        ConfigVar<bool> enableAchievementToasts;
        ConfigVar<bool> enableControllerToasts;
        ConfigVar<bool> enableDiscordPresence;

        // Graphics
        ConfigVar<BloomMode> bloomMode;
        ConfigVar<float> bloomMultiplier;
        ConfigVar<bool> disableWaterRefraction;
        ConfigVar<bool> enableFrameInterpolation;
        ConfigVar<int> internalResolutionScale;
        ConfigVar<int> shadowResolutionMultiplier;
        ConfigVar<bool> enableDepthOfField;
        ConfigVar<bool> enableMapBackground;
        ConfigVar<bool> disableCutscenePillarboxing;

        // Audio
        ConfigVar<bool> noLowHpSound;
        ConfigVar<bool> midnasLamentNonStop;

        // Input
        ConfigVar<GyroMode> gyroMode;
        ConfigVar<bool> enableGyroAim;
        ConfigVar<bool> enableGyroRollgoal;
        ConfigVar<float> gyroSensitivityX;
        ConfigVar<float> gyroSensitivityY;
        ConfigVar<float> gyroSensitivityRollgoal;
        ConfigVar<float> gyroSmoothing;
        ConfigVar<float> gyroDeadband;
        ConfigVar<bool> gyroInvertPitch;
        ConfigVar<bool> gyroInvertYaw;
        ConfigVar<bool> freeCamera;
        ConfigVar<bool> invertCameraXAxis;
        ConfigVar<bool> invertCameraYAxis;
        ConfigVar<bool> invertFirstPersonXAxis;
        ConfigVar<bool> invertFirstPersonYAxis;
        ConfigVar<float> freeCameraSensitivity;
        ConfigVar<bool> debugFlyCam;
        ConfigVar<bool> debugFlyCamLockEvents;
        ConfigVar<bool> allowBackgroundInput;

        // Cheats
        ConfigVar<bool> infiniteHearts;
        ConfigVar<bool> infiniteArrows;
        ConfigVar<bool> infiniteBombs;
        ConfigVar<bool> infiniteOil;
        ConfigVar<bool> infiniteOxygen;
        ConfigVar<bool> infiniteRupees;
        ConfigVar<bool> enableIndefiniteItemDrops;
        ConfigVar<bool> moonJump;
        ConfigVar<bool> superClawshot;
        ConfigVar<bool> alwaysGreatspin;
        ConfigVar<bool> enableFastIronBoots;
        ConfigVar<bool> canTransformAnywhere;
        ConfigVar<bool> fastRoll;
        ConfigVar<bool> fastSpinner;
        ConfigVar<bool> freeMagicArmor;

        // Technical
        ConfigVar<bool> restoreWiiGlitches;

        // Controls
        ConfigVar<bool> enableTurboKeybind;
        ConfigVar<bool> enableResetKeybind;

        // Tools
        ConfigVar<bool> speedrunMode;
        ConfigVar<bool> liveSplitEnabled;
        ConfigVar<bool> showSpeedrunRTATimer;
        ConfigVar<bool> recordingMode;
        ConfigVar<bool> showInputViewer;
        ConfigVar<bool> showInputViewerGyro;
    } game;

    struct {
        ConfigVar<std::string> isoPath;
        ConfigVar<DiscVerificationState> isoVerification;
        ConfigVar<std::string> graphicsBackend;
        ConfigVar<bool> skipPreLaunchUI;
        ConfigVar<bool> showPipelineCompilation;
        ConfigVar<bool> wasPresetChosen;
        ConfigVar<bool> checkForUpdates;
        ConfigVar<int> cardFileType;
        ConfigVar<bool> enableAdvancedSettings;
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
    bool stateShareLoadActive;
};

TransientSettings& getTransientSettings();

}

#endif // DUSK_CONFIG_H
