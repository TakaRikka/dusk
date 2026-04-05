#ifndef DUSK_CONFIG_H
#define DUSK_CONFIG_H

#include "config_var.hpp"

namespace dusk::settings {
using namespace config;

// Persistent user settings

namespace video {
extern ConfigVar<bool> enableFullscreen;
}

namespace audio {
extern ConfigVar<float> masterVolume;
extern ConfigVar<float> mainMusicVolume;
extern ConfigVar<float> subMusicVolume;
extern ConfigVar<float> soundEffectsVolume;
extern ConfigVar<float> fanfareVolume;
extern ConfigVar<bool> enableReverb;
}

namespace game {
// QoL
extern ConfigVar<bool> enableQuickTransform;
extern ConfigVar<bool> hideTvSettingsScreen;
extern ConfigVar<bool> biggerWallets;
extern ConfigVar<bool> noReturnRupees;
extern ConfigVar<bool> disableRupeeCutscenes;
extern ConfigVar<bool> noSwordRecoil;
extern ConfigVar<int> damageMultiplier;
extern ConfigVar<bool> instantDeath;
extern ConfigVar<bool> fastClimbing;
extern ConfigVar<bool> fastTears;
extern ConfigVar<bool> noMissClimbing;

// Preferences
extern ConfigVar<bool> enableMirrorMode;
extern ConfigVar<bool> invertCameraXAxis;

// Graphics
extern ConfigVar<bool> enableBloom;
extern ConfigVar<bool> useWaterProjectionOffset;

// Audio
extern ConfigVar<bool> noLowHpSound;
extern ConfigVar<bool> midnasLamentNonStop;

// Cheats
extern ConfigVar<bool> enableFastIronBoots;
extern ConfigVar<bool> canTransformAnywhere;

// Technical
extern ConfigVar<bool> restoreWiiGlitches;
}

void Register();

}

namespace dusk {
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
