#include "dusk/settings.h"
#include "dusk/config.hpp"

namespace dusk {

UserSettings g_userSettings = {
    .video = {
        .enableFullscreen {"video.enableFullscreen", false},
        .enableVsync {"video.enableVsync", true},
        .lockAspectRatio {"video.lockAspectRatio", false},
    },

    .audio = {
        .masterVolume {"audio.masterVolume", 80},
        .mainMusicVolume {"audio.mainMusicVolume", 100},
        .subMusicVolume {"audio.subMusicVolume", 100},
        .soundEffectsVolume {"audio.soundEffectsVolume", 100},
        .fanfareVolume {"audio.fanfareVolume", 100},
        .enableReverb {"audio.enableReverb", true},
    },

    .game = {
        .language { "game.language", GameLanguage::English },

        // Quality of Life
        .enableQuickTransform {"game.enableQuickTransform", false},
        .hideTvSettingsScreen {"game.hideTvSettingsScreen", false},
        .skipWarningScreen {"game.skipWarningScreen", false},
        .biggerWallets {"game.biggerWallets", false},
        .noReturnRupees {"game.noReturnRupees", false},
        .disableRupeeCutscenes {"game.disableRupeeCutscenes", false},
        .noSwordRecoil {"game.noSwordRecoil", false},
        .damageMultiplier {"game.damageMultiplier", 1},
        .noHeartDrops{"game.noHeartDrops", false},
        .instantDeath {"game.instantDeath", false},
        .fastClimbing {"game.fastClimbing", false},
        .noMissClimbing {"game.noMissClimbing", false},
        .fastTears {"game.fastTears", false},
        .instantSaves {"game.instantSaves", false},
        .instantText {"game.instantText", false},
        .sunsSong {"game.sunsSong", false},

        // Preferences
        .enableMirrorMode {"game.enableMirrorMode", false},
        .disableMainHUD {"game.disableMainHUD", false},
        .pauseOnFocusLost {"game.pauseOnFocusLost", false},
        .enableLinkDollRotation = {"game.enableLinkDollRotation", false },
        .enableAchievementNotifications {"game.enableAchievementNotifications", false},

        // Graphics
        .bloomMode {"game.bloomMode", BloomMode::Classic},
        .bloomMultiplier {"game.bloomMultiplier", 1.0f},
        .disableWaterRefraction {"game.disableWaterRefraction", false},
        .enableFrameInterpolation = {"game.enableFrameInterpolation", false},
        .internalResolutionScale {"game.internalResolutionScale", 0},
        .shadowResolutionMultiplier {"game.shadowResolutionMultiplier", 1},
        .enableDepthOfField {"game.enableDepthOfField", true},
        .enableMapBackground {"game.enableMapBackground", true},

        // Audio
        .noLowHpSound {"game.noLowHpSound", false},
        .midnasLamentNonStop {"game.midnasLamentNonStop", false},

        // Input
        .enableGyroAim {"game.enableGyroAim", false},
        .enableGyroRollgoal {"game.enableGyroRollgoal", false},
        .gyroSensitivityX {"game.gyroSensitivityX", 1.0f},
        .gyroSensitivityY {"game.gyroSensitivityY", 1.0f},
        .gyroSensitivityRollgoal {"game.gyroSensitivityRollgoal", 1.0f},
        .gyroSmoothing {"game.gyroSmoothing", 0.65f},
        .gyroDeadband {"game.gyroDeadband", 0.04f},
        .gyroInvertPitch {"game.gyroInvertPitch", false},
        .gyroInvertYaw {"game.gyroInvertYaw", false},
        .freeCamera {"game.freeCamera", false},
        .invertCameraXAxis {"game.invertCameraXAxis", false},
        .invertCameraYAxis {"game.invertCameraYAxis", false},
        .freeCameraSensitivity {"game.freeCameraSensitivity", 1.0f},

        // Cheats
        .infiniteHearts {"game.infiniteHearts", false},
        .infiniteArrows{"game.infiniteArrows", false},
        .infiniteBombs{"game.infiniteBombs", false},
        .infiniteOil{"game.infiniteOil", false},
        .infiniteOxygen{"game.infiniteOxygen", false},
        .infiniteRupees{"game.infiniteRupees", false},
        .enableIndefiniteItemDrops {"game.enableIndefiniteItemDrops", false},
        .moonJump{"game.moonJump", false},
        .superClawshot{"game.superClawshot", false},
        .alwaysGreatspin{"game.alwaysGreatspin", false},
        .enableFastIronBoots {"game.enableFastIronBoots", false},
        .canTransformAnywhere {"game.canTransformAnywhere", false},
        .fastSpinner {"game.fastSpinner", false},
        .freeMagicArmor {"game.freeMagicArmor", false},

        // Technical
        .restoreWiiGlitches {"game.restoreWiiGlitches", false},

        // Controls
        .enableTurboKeybind {"game.enableTurboKeybind", false}
    },

    .backend = {
        .isoPath {"backend.isoPath", ""},
        .graphicsBackend {"backend.graphicsBackend", "auto"},
        .skipPreLaunchUI {"backend.skipPreLaunchUI", false},
        .showPipelineCompilation {"backend.showPipelineCompilation", false},
        .wasPresetChosen {"backend.wasPresetChosen", false},
        .enableCrashReporting {"backend.enableCrashReporting", true},
        .duskMenuOpen {"backend.duskMenuOpen", false},
        .cardFileType {"backend.cardFileType", static_cast<int>(CARD_GCIFOLDER)}
    }
};

UserSettings& getSettings() {
    return g_userSettings;
}

void registerSettings() {
    // Video
    Register(g_userSettings.video.enableFullscreen);
    Register(g_userSettings.video.enableVsync);
    Register(g_userSettings.video.lockAspectRatio);

    // Audio
    Register(g_userSettings.audio.masterVolume);
    Register(g_userSettings.audio.mainMusicVolume);
    Register(g_userSettings.audio.subMusicVolume);
    Register(g_userSettings.audio.soundEffectsVolume);
    Register(g_userSettings.audio.fanfareVolume);
    Register(g_userSettings.audio.enableReverb);

    // Game
    Register(g_userSettings.game.language);
    Register(g_userSettings.game.enableQuickTransform);
    Register(g_userSettings.game.hideTvSettingsScreen);
    Register(g_userSettings.game.skipWarningScreen);
    Register(g_userSettings.game.biggerWallets);
    Register(g_userSettings.game.noReturnRupees);
    Register(g_userSettings.game.disableRupeeCutscenes);
    Register(g_userSettings.game.noSwordRecoil);
    Register(g_userSettings.game.damageMultiplier);
    Register(g_userSettings.game.noHeartDrops);
    Register(g_userSettings.game.instantDeath);
    Register(g_userSettings.game.fastClimbing);
    Register(g_userSettings.game.fastTears);
    Register(g_userSettings.game.instantSaves);
    Register(g_userSettings.game.instantText);
    Register(g_userSettings.game.sunsSong);
    Register(g_userSettings.game.enableMirrorMode);
    Register(g_userSettings.game.invertCameraXAxis);
    Register(g_userSettings.game.invertCameraYAxis);
    Register(g_userSettings.game.freeCameraSensitivity);
    Register(g_userSettings.game.disableMainHUD);
    Register(g_userSettings.game.pauseOnFocusLost);
    Register(g_userSettings.game.bloomMode);
    Register(g_userSettings.game.bloomMultiplier);
    Register(g_userSettings.game.disableWaterRefraction);
    Register(g_userSettings.game.internalResolutionScale);
    Register(g_userSettings.game.shadowResolutionMultiplier);
    Register(g_userSettings.game.enableDepthOfField);
    Register(g_userSettings.game.enableMapBackground);
    Register(g_userSettings.game.enableFastIronBoots);
    Register(g_userSettings.game.canTransformAnywhere);
    Register(g_userSettings.game.freeMagicArmor);
    Register(g_userSettings.game.restoreWiiGlitches);
    Register(g_userSettings.game.enableLinkDollRotation);
    Register(g_userSettings.game.enableAchievementNotifications);
    Register(g_userSettings.game.noMissClimbing);
    Register(g_userSettings.game.noLowHpSound);
    Register(g_userSettings.game.midnasLamentNonStop);
    Register(g_userSettings.game.enableTurboKeybind);
    Register(g_userSettings.game.fastSpinner);
    Register(g_userSettings.game.infiniteHearts);
    Register(g_userSettings.game.infiniteArrows);
    Register(g_userSettings.game.infiniteBombs);
    Register(g_userSettings.game.infiniteOil);
    Register(g_userSettings.game.infiniteOxygen);
    Register(g_userSettings.game.infiniteRupees);
    Register(g_userSettings.game.enableIndefiniteItemDrops);
    Register(g_userSettings.game.moonJump);
    Register(g_userSettings.game.superClawshot);
    Register(g_userSettings.game.alwaysGreatspin);
    Register(g_userSettings.game.enableFrameInterpolation);
    Register(g_userSettings.game.enableGyroAim);
    Register(g_userSettings.game.enableGyroRollgoal);
    Register(g_userSettings.game.gyroSensitivityX);
    Register(g_userSettings.game.gyroSensitivityY);
    Register(g_userSettings.game.gyroSensitivityRollgoal);
    Register(g_userSettings.game.gyroDeadband);
    Register(g_userSettings.game.gyroSmoothing);
    Register(g_userSettings.game.gyroInvertPitch);
    Register(g_userSettings.game.gyroInvertYaw);
    Register(g_userSettings.game.freeCamera);

    Register(g_userSettings.backend.isoPath);
    Register(g_userSettings.backend.graphicsBackend);
    Register(g_userSettings.backend.skipPreLaunchUI);
    Register(g_userSettings.backend.showPipelineCompilation);
    Register(g_userSettings.backend.wasPresetChosen);
    Register(g_userSettings.backend.enableCrashReporting);
    Register(g_userSettings.backend.duskMenuOpen);
    Register(g_userSettings.backend.cardFileType);
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
