#include "dusk/settings.h"
#include "dusk/config.hpp"

namespace dusk {

UserSettings g_userSettings = {
    .video = {
        .enableFullscreen {"video.enableFullscreen", false},
        .enableVsync {"video.enableVsync", true},
        .lockAspectRatio {"video.lockAspectRatio", false},
        .enableFpsOverlay {"game.enableFpsOverlay", false},
        .fpsOverlayCorner {"game.fpsOverlayCorner", 0},
    },

    .audio = {
        .masterVolume {"audio.masterVolume", 60},
        .mainMusicVolume {"audio.mainMusicVolume", 100},
        .subMusicVolume {"audio.subMusicVolume", 100},
        .soundEffectsVolume {"audio.soundEffectsVolume", 100},
        .fanfareVolume {"audio.fanfareVolume", 100},
        .enableReverb {"audio.enableReverb", true},
        .enableHrtf {"audio.enableHrtf", false},
        .menuSounds {"audio.menuSounds", true},
    },

    .game = {
        .language { "game.language", GameLanguage::English },

        // Quality of Life
        .enableQuickTransform {"game.enableQuickTransform", false},
        .hideTvSettingsScreen {"game.hideTvSettingsScreen", true},
        .biggerWallets {"game.biggerWallets", false},
        .noReturnRupees {"game.noReturnRupees", false},
        .disableRupeeCutscenes {"game.disableRupeeCutscenes", false},
        .noSwordRecoil {"game.noSwordRecoil", false},
        .damageMultiplier {"game.damageMultiplier", 1},
        .noHeartDrops {"game.noHeartDrops", false},
        .instantDeath {"game.instantDeath", false},
        .fastClimbing {"game.fastClimbing", false},
        .noMissClimbing {"game.noMissClimbing", false},
        .fastTears {"game.fastTears", false},
        .no2ndFishForCat {"game.no2ndFishForCat", false},
        .instantSaves {"game.instantSaves", false},
        .instantText {"game.instantText", false},
        .sunsSong {"game.sunsSong", false},
        .autoSave {"game.autoSave", false},

        // Preferences
        .enableMirrorMode {"game.enableMirrorMode", false},
        .minimalHUD {"game.minimalHUD", false},
        .pauseOnFocusLost {"game.pauseOnFocusLost", false},
        .enableLinkDollRotation {"game.enableLinkDollRotation", false},
        .enableAchievementToasts {"game.enableAchievementToasts", true},
        .enableControllerToasts {"game.enableControllerToasts", true},
        .enableDiscordPresence {"game.enableDiscordPresence", true},

        // Graphics
        .bloomMode {"game.bloomMode", BloomMode::Dusk},
        .bloomMultiplier {"game.bloomMultiplier", 1.0f},
        .disableWaterRefraction {"game.disableWaterRefraction", false},
        .enableFrameInterpolation {"game.enableFrameInterpolation", false},
        .internalResolutionScale {"game.internalResolutionScale", 0},
        .shadowResolutionMultiplier {"game.shadowResolutionMultiplier", 1},
        .enableDepthOfField {"game.enableDepthOfField", true},
        .enableMapBackground {"game.enableMapBackground", true},
        .disableCutscenePillarboxing {"game.disableCutscenePillarboxing", false},

        // Audio
        .noLowHpSound {"game.noLowHpSound", false},
        .midnasLamentNonStop {"game.midnasLamentNonStop", false},

        // Input
        .gyroMode {"game.gyroMode", GyroMode::Sensor},
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
        .invertFirstPersonXAxis {"game.invertFirstPersonXAxis", false},
        .invertFirstPersonYAxis {"game.invertFirstPersonYAxis", false},
        .freeCameraSensitivity {"game.freeCameraSensitivity", 1.0f},
        .debugFlyCam {"game.debugFlyCam", false},
        .debugFlyCamLockEvents {"game.debugFlyCamLockEvents", true},
        .allowBackgroundInput {"game.allowBackgroundInput", true},

        // Cheats
        .infiniteHearts {"game.infiniteHearts", false},
        .infiniteArrows {"game.infiniteArrows", false},
        .infiniteSeeds {"game.infiniteSeeds", false},
        .infiniteBombs {"game.infiniteBombs", false},
        .infiniteOil {"game.infiniteOil", false},
        .infiniteOxygen {"game.infiniteOxygen", false},
        .infiniteRupees {"game.infiniteRupees", false},
        .enableIndefiniteItemDrops {"game.enableIndefiniteItemDrops", false},
        .moonJump {"game.moonJump", false},
        .superClawshot {"game.superClawshot", false},
        .alwaysGreatspin {"game.alwaysGreatspin", false},
        .enableFastIronBoots {"game.enableFastIronBoots", false},
        .canTransformAnywhere {"game.canTransformAnywhere", false},
        .fastRoll {"game.fastRoll", false},
        .fastSpinner {"game.fastSpinner", false},
        .freeMagicArmor {"game.freeMagicArmor", false},
        .invincibleEnemies {"game.invincibleEnemies", false},

        // Technical
        .restoreWiiGlitches {"game.restoreWiiGlitches", false},

        // Controls
        .enableTurboKeybind {"game.enableTurboKeybind", false},
        .enableResetKeybind {"game.enableResetKeybind", false},

        // Tools
        .speedrunMode {"game.speedrunMode", false},
        .liveSplitEnabled {"game.liveSplitEnabled", false},
        .showSpeedrunRTATimer {"game.showSpeedrunRTATimer", true},
        .recordingMode {"game.recordingMode", false},
        .showInputViewer {"game.showInputViewer", false},
        .showInputViewerGyro {"game.showInputViewerGyro", false}
    },

    .backend = {
        .isoPath {"backend.isoPath", ""},
        .isoVerification {"backend.isoVerification", DiscVerificationState::Unknown},
        .graphicsBackend {"backend.graphicsBackend", "auto"},
        .skipPreLaunchUI {"backend.skipPreLaunchUI", false},
        .showPipelineCompilation {"backend.showPipelineCompilation", false},
        .wasPresetChosen {"backend.wasPresetChosen", false},
        .checkForUpdates {"backend.checkForUpdates", true},
        .cardFileType {"backend.cardFileType", static_cast<int>(CARD_GCIFOLDER)},
        .enableAdvancedSettings {"backend.enableAdvancedSettings", false},
    },

    // Not sure if there's a better way to declare this
    .actionBindings = {
        .firstPersonCamera {
            ActionBindConfigVar{"actionBindings.firstPersonCamera_port0", PAD_NATIVE_BUTTON_INVALID},
            ActionBindConfigVar{"actionBindings.firstPersonCamera_port1", PAD_NATIVE_BUTTON_INVALID},
            ActionBindConfigVar{"actionBindings.firstPersonCamera_port2", PAD_NATIVE_BUTTON_INVALID},
            ActionBindConfigVar{"actionBindings.firstPersonCamera_port3", PAD_NATIVE_BUTTON_INVALID},
        },
        .callMidna {
            ActionBindConfigVar{"actionBindings.callMidna_port0", PAD_NATIVE_BUTTON_INVALID},
            ActionBindConfigVar{"actionBindings.callMidna_port1", PAD_NATIVE_BUTTON_INVALID},
            ActionBindConfigVar{"actionBindings.callMidna_port2", PAD_NATIVE_BUTTON_INVALID},
            ActionBindConfigVar{"actionBindings.callMidna_port3", PAD_NATIVE_BUTTON_INVALID},
        },
        .openDusklightMenu {
            ActionBindConfigVar{"actionBindings.openDusklightMenu_port0", PAD_NATIVE_BUTTON_INVALID},
            ActionBindConfigVar{"actionBindings.openDusklightMenu_port1", PAD_NATIVE_BUTTON_INVALID},
            ActionBindConfigVar{"actionBindings.openDusklightMenu_port2", PAD_NATIVE_BUTTON_INVALID},
            ActionBindConfigVar{"actionBindings.openDusklightMenu_port3", PAD_NATIVE_BUTTON_INVALID},
        },
        .turboSpeedButton {
            ActionBindConfigVar{"actionBindings.turboButton_port0", PAD_NATIVE_BUTTON_INVALID},
            ActionBindConfigVar{"actionBindings.turboButton_port1", PAD_NATIVE_BUTTON_INVALID},
            ActionBindConfigVar{"actionBindings.turboButton_port2", PAD_NATIVE_BUTTON_INVALID},
            ActionBindConfigVar{"actionBindings.turboButton_port3", PAD_NATIVE_BUTTON_INVALID},
        },
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
    Register(g_userSettings.video.enableFpsOverlay);
    Register(g_userSettings.video.fpsOverlayCorner);

    // Audio
    Register(g_userSettings.audio.masterVolume);
    Register(g_userSettings.audio.mainMusicVolume);
    Register(g_userSettings.audio.subMusicVolume);
    Register(g_userSettings.audio.soundEffectsVolume);
    Register(g_userSettings.audio.fanfareVolume);
    Register(g_userSettings.audio.enableReverb);
    Register(g_userSettings.audio.enableHrtf);
    Register(g_userSettings.audio.menuSounds);

    // Game
    Register(g_userSettings.game.language);
    Register(g_userSettings.game.enableQuickTransform);
    Register(g_userSettings.game.hideTvSettingsScreen);
    Register(g_userSettings.game.biggerWallets);
    Register(g_userSettings.game.noReturnRupees);
    Register(g_userSettings.game.disableRupeeCutscenes);
    Register(g_userSettings.game.noSwordRecoil);
    Register(g_userSettings.game.damageMultiplier);
    Register(g_userSettings.game.noHeartDrops);
    Register(g_userSettings.game.instantDeath);
    Register(g_userSettings.game.fastClimbing);
    Register(g_userSettings.game.fastTears);
    Register(g_userSettings.game.no2ndFishForCat);
    Register(g_userSettings.game.instantSaves);
    Register(g_userSettings.game.instantText);
    Register(g_userSettings.game.sunsSong);
    Register(g_userSettings.game.autoSave);
    Register(g_userSettings.game.enableMirrorMode);
    Register(g_userSettings.game.invertCameraXAxis);
    Register(g_userSettings.game.invertCameraYAxis);
    Register(g_userSettings.game.invertFirstPersonXAxis);
    Register(g_userSettings.game.invertFirstPersonYAxis);
    Register(g_userSettings.game.freeCameraSensitivity);
    Register(g_userSettings.game.minimalHUD);
    Register(g_userSettings.game.pauseOnFocusLost);
    Register(g_userSettings.game.enableDiscordPresence);
    Register(g_userSettings.game.bloomMode);
    Register(g_userSettings.game.bloomMultiplier);
    Register(g_userSettings.game.disableWaterRefraction);
    Register(g_userSettings.game.internalResolutionScale);
    Register(g_userSettings.game.shadowResolutionMultiplier);
    Register(g_userSettings.game.enableDepthOfField);
    Register(g_userSettings.game.enableMapBackground);
    Register(g_userSettings.game.disableCutscenePillarboxing);
    Register(g_userSettings.game.enableFastIronBoots);
    Register(g_userSettings.game.canTransformAnywhere);
    Register(g_userSettings.game.fastRoll);
    Register(g_userSettings.game.freeMagicArmor);
    Register(g_userSettings.game.restoreWiiGlitches);
    Register(g_userSettings.game.enableLinkDollRotation);
    Register(g_userSettings.game.enableAchievementToasts);
    Register(g_userSettings.game.enableControllerToasts);
    Register(g_userSettings.game.noMissClimbing);
    Register(g_userSettings.game.noLowHpSound);
    Register(g_userSettings.game.midnasLamentNonStop);
    Register(g_userSettings.game.enableTurboKeybind);
    Register(g_userSettings.game.enableResetKeybind);
    Register(g_userSettings.game.speedrunMode);
    Register(g_userSettings.game.liveSplitEnabled);
    Register(g_userSettings.game.showSpeedrunRTATimer);
    Register(g_userSettings.game.recordingMode);
    Register(g_userSettings.game.showInputViewer);
    Register(g_userSettings.game.showInputViewerGyro);
    Register(g_userSettings.game.fastSpinner);
    Register(g_userSettings.game.infiniteHearts);
    Register(g_userSettings.game.infiniteArrows);
    Register(g_userSettings.game.infiniteSeeds);
    Register(g_userSettings.game.infiniteBombs);
    Register(g_userSettings.game.infiniteOil);
    Register(g_userSettings.game.infiniteOxygen);
    Register(g_userSettings.game.infiniteRupees);
    Register(g_userSettings.game.enableIndefiniteItemDrops);
    Register(g_userSettings.game.moonJump);
    Register(g_userSettings.game.superClawshot);
    Register(g_userSettings.game.alwaysGreatspin);
    Register(g_userSettings.game.invincibleEnemies);
    Register(g_userSettings.game.enableFrameInterpolation);
    Register(g_userSettings.game.gyroMode);
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
    Register(g_userSettings.game.debugFlyCam);
    Register(g_userSettings.game.debugFlyCamLockEvents);
    Register(g_userSettings.game.allowBackgroundInput);

    Register(g_userSettings.backend.isoPath);
    Register(g_userSettings.backend.isoVerification);
    Register(g_userSettings.backend.graphicsBackend);
    Register(g_userSettings.backend.skipPreLaunchUI);
    Register(g_userSettings.backend.showPipelineCompilation);
    Register(g_userSettings.backend.wasPresetChosen);
    Register(g_userSettings.backend.checkForUpdates);
    Register(g_userSettings.backend.cardFileType);
    Register(g_userSettings.backend.enableAdvancedSettings);

    Register(g_userSettings.actionBindings.firstPersonCamera[0]);
    Register(g_userSettings.actionBindings.firstPersonCamera[1]);
    Register(g_userSettings.actionBindings.firstPersonCamera[2]);
    Register(g_userSettings.actionBindings.firstPersonCamera[3]);
    Register(g_userSettings.actionBindings.callMidna[0]);
    Register(g_userSettings.actionBindings.callMidna[1]);
    Register(g_userSettings.actionBindings.callMidna[2]);
    Register(g_userSettings.actionBindings.callMidna[3]);
    Register(g_userSettings.actionBindings.openDusklightMenu[0]);
    Register(g_userSettings.actionBindings.openDusklightMenu[1]);
    Register(g_userSettings.actionBindings.openDusklightMenu[2]);
    Register(g_userSettings.actionBindings.openDusklightMenu[3]);
    Register(g_userSettings.actionBindings.turboSpeedButton[0]);
    Register(g_userSettings.actionBindings.turboSpeedButton[1]);
    Register(g_userSettings.actionBindings.turboSpeedButton[2]);
    Register(g_userSettings.actionBindings.turboSpeedButton[3]);
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
