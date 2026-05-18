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
        .maxFrameRate {"video.maxFrameRate", 240},
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
        .enableTextureReplacements {"game.enableTextureReplacements", true},
        .enableFrameInterpolation {"game.enableFrameInterpolation", FrameInterpMode::Off},
        .internalResolutionScale {"game.internalResolutionScale", 0},
        .shadowResolutionMultiplier {"game.shadowResolutionMultiplier", 1},
        .resampler {"game.resampler", Resampler::Bilinear},
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
        .enableClassicKeybinds {"game.enableClassicKeybinds", true},
        .dusklightMenuInputFallthrough {"game.dusklightMenuInputFallthrough", false},
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

#define ADD_FULL_ACTIONBINDCONFIGVAR(varname, bindingdefault)    .varname    {   \
            ActionBindConfigVar{"actionBindings." #varname "_port0", bindingdefault},   \
            ActionBindConfigVar{"actionBindings." #varname "_port1", bindingdefault},   \
            ActionBindConfigVar{"actionBindings." #varname "_port2", bindingdefault},   \
            ActionBindConfigVar{"actionBindings." #varname "_port3", bindingdefault},   \
        }

    // Not sure if there's a better way to declare this
    .actionBindings = {
        ADD_FULL_ACTIONBINDCONFIGVAR(openDusklightMenu, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(menuUp, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(menuLeft, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(menuDown, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(menuRight, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(menuConfirm, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(menuReject, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(menuX, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(menuY, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(turboSpeedButton, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(dusklightSpecial, PAD_NATIVE_BUTTON_INVALID),

        ADD_FULL_ACTIONBINDCONFIGVAR(swordAttack, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(quickSpin, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(itemAccessX, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(itemAccessY, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(shieldUse, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(interact, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(reject, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(roll, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(moveModifierToggle, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(target, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(dig, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(sense, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(grabHeavy, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(grabLight, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(callMidna, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(ingamePause, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(firstPersonCamera, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(mapCycleUp, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(mapCycleDown, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(peekMap, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(toggleMapScreen, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(hudVisibility, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(itemCycleNext, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(itemCyclePrev, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(grabAll, PAD_NATIVE_BUTTON_INVALID),

        // Will figure out analog input later
        ADD_FULL_ACTIONBINDCONFIGVAR(moveX, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(moveY, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(camX, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(camY, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(aimX, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(aimY, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(wheelX, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(wheelY, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(allcamX, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(allcamY, PAD_NATIVE_BUTTON_INVALID),

        ADD_FULL_ACTIONBINDCONFIGVAR(buttonA, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(buttonB, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(buttonX, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(buttonY, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(buttonZ, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(buttonStart, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(dpadUp, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(dpadLeft, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(dpadDown, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(dpadRight, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(digitalL, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(digitalR, PAD_NATIVE_BUTTON_INVALID),

        ADD_FULL_ACTIONBINDCONFIGVAR(mainStickX, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(mainStickY, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(cStickX, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(cStickY, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(triggerL, PAD_NATIVE_BUTTON_INVALID),
        ADD_FULL_ACTIONBINDCONFIGVAR(triggerR, PAD_NATIVE_BUTTON_INVALID),
    }

#undef ADD_FULL_ACTIONBINDCONFIGVAR

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
    Register(g_userSettings.video.maxFrameRate);

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
    Register(g_userSettings.game.enableTextureReplacements);
    Register(g_userSettings.game.internalResolutionScale);
    Register(g_userSettings.game.resampler);
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
    Register(g_userSettings.game.enableClassicKeybinds);
    Register(g_userSettings.game.dusklightMenuInputFallthrough);
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

#define REGISTER_ACTION_BINDINGS(bindingname) Register(g_userSettings.actionBindings.bindingname[0]); \
    Register(g_userSettings.actionBindings.bindingname[1]); \
    Register(g_userSettings.actionBindings.bindingname[2]); \
    Register(g_userSettings.actionBindings.bindingname[3]);

    REGISTER_ACTION_BINDINGS(openDusklightMenu)
    REGISTER_ACTION_BINDINGS(menuUp)
    REGISTER_ACTION_BINDINGS(menuLeft)
    REGISTER_ACTION_BINDINGS(menuDown)
    REGISTER_ACTION_BINDINGS(menuRight)
    REGISTER_ACTION_BINDINGS(menuConfirm)
    REGISTER_ACTION_BINDINGS(menuReject)
    REGISTER_ACTION_BINDINGS(menuX)
    REGISTER_ACTION_BINDINGS(menuY)
    REGISTER_ACTION_BINDINGS(turboSpeedButton)
    REGISTER_ACTION_BINDINGS(dusklightSpecial)

    REGISTER_ACTION_BINDINGS(swordAttack)
    REGISTER_ACTION_BINDINGS(quickSpin)
    REGISTER_ACTION_BINDINGS(itemAccessX)
    REGISTER_ACTION_BINDINGS(itemAccessY)
    REGISTER_ACTION_BINDINGS(shieldUse)
    REGISTER_ACTION_BINDINGS(interact)
    REGISTER_ACTION_BINDINGS(reject)
    REGISTER_ACTION_BINDINGS(roll)
    REGISTER_ACTION_BINDINGS(moveModifierToggle)
    REGISTER_ACTION_BINDINGS(target)
    REGISTER_ACTION_BINDINGS(dig)
    REGISTER_ACTION_BINDINGS(sense)
    REGISTER_ACTION_BINDINGS(grabHeavy)
    REGISTER_ACTION_BINDINGS(grabLight)
    REGISTER_ACTION_BINDINGS(callMidna)
    REGISTER_ACTION_BINDINGS(ingamePause)
    REGISTER_ACTION_BINDINGS(firstPersonCamera)
    REGISTER_ACTION_BINDINGS(mapCycleUp)
    REGISTER_ACTION_BINDINGS(mapCycleDown)
    REGISTER_ACTION_BINDINGS(peekMap)
    REGISTER_ACTION_BINDINGS(toggleMapScreen)
    REGISTER_ACTION_BINDINGS(hudVisibility)
    REGISTER_ACTION_BINDINGS(itemCycleNext)
    REGISTER_ACTION_BINDINGS(itemCyclePrev)
    REGISTER_ACTION_BINDINGS(grabAll)

    REGISTER_ACTION_BINDINGS(moveX)
    REGISTER_ACTION_BINDINGS(moveY)
    REGISTER_ACTION_BINDINGS(camX)
    REGISTER_ACTION_BINDINGS(camY)
    REGISTER_ACTION_BINDINGS(aimX)
    REGISTER_ACTION_BINDINGS(aimY)
    REGISTER_ACTION_BINDINGS(wheelX)
    REGISTER_ACTION_BINDINGS(wheelY)
    REGISTER_ACTION_BINDINGS(allcamX)
    REGISTER_ACTION_BINDINGS(allcamY)

    REGISTER_ACTION_BINDINGS(buttonA)
    REGISTER_ACTION_BINDINGS(buttonB)
    REGISTER_ACTION_BINDINGS(buttonX)
    REGISTER_ACTION_BINDINGS(buttonY)
    REGISTER_ACTION_BINDINGS(buttonZ)
    REGISTER_ACTION_BINDINGS(buttonStart)
    REGISTER_ACTION_BINDINGS(dpadUp)
    REGISTER_ACTION_BINDINGS(dpadLeft)
    REGISTER_ACTION_BINDINGS(dpadDown)
    REGISTER_ACTION_BINDINGS(dpadRight)
    REGISTER_ACTION_BINDINGS(digitalL)
    REGISTER_ACTION_BINDINGS(digitalR)

    REGISTER_ACTION_BINDINGS(mainStickX)
    REGISTER_ACTION_BINDINGS(mainStickY)
    REGISTER_ACTION_BINDINGS(cStickX)
    REGISTER_ACTION_BINDINGS(cStickY)
    REGISTER_ACTION_BINDINGS(triggerL)
    REGISTER_ACTION_BINDINGS(triggerR)

#undef REGISTER_ACTION_BINDINGS
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
