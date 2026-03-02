#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASSoundParams.h"

void JASSoundParams::clamp() {
    clampVolume();
    clampPitch();
    clampFxMix();
    clampPan();
    clampDolby();
}

void JASSoundParams::combine(const JASSoundParams &other1, const JASSoundParams &other2) {
    mVolume = other1.mVolume * other2.mVolume;
    mPitch = other1.mPitch * other2.mPitch;
    mFxMix = other1.mFxMix + other2.mFxMix;
    mPan = other1.mPan + other2.mPan - 0.5f;
    mDolby = other1.mDolby + other2.mDolby;
}
