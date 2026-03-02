#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAISoundParams.h"

void JAISoundParamsMove::moveVolume(f32 newValue, u32 maxSteps) {
    JUT_ASSERT(12, (newValue>=0.f));
    if (maxSteps == 0) {
        params_.mVolume = newValue;
        transition_.volume_.remainingSteps_ = 0;
    } else {
        transition_.volume_.set(newValue, params_.mVolume, maxSteps);
    }
}

void JAISoundParamsMove::movePitch(f32 newValue, u32 maxSteps) {
    JUT_ASSERT(27, (newValue>=0.f));
    if (maxSteps == 0) {
        params_.mPitch = newValue;
        transition_.pitch_.remainingSteps_ = 0;
    } else {
        transition_.pitch_.set(newValue, params_.mPitch, maxSteps);
    }
}

void JAISoundParamsMove::moveFxMix(f32 newValue, u32 maxSteps) {
    if (maxSteps == 0) {
        params_.mFxMix = newValue;
        transition_.fxMix_.remainingSteps_ = 0;
    } else {
        transition_.fxMix_.set(newValue, params_.mFxMix, maxSteps);
    }
}

void JAISoundParamsMove::movePan(f32 newValue, u32 maxSteps) {
    if (maxSteps == 0) {
        params_.mPan = newValue;
        transition_.pan_.remainingSteps_ = 0;
    } else {
        transition_.pan_.set(newValue, params_.mPan, maxSteps);
    }
}

void JAISoundParamsMove::moveDolby(f32 newValue, u32 maxSteps) {
    if (maxSteps == 0) {
        params_.mDolby = newValue;
        transition_.dolby_.remainingSteps_ = 0;
    } else {
        transition_.dolby_.set(newValue, params_.mDolby, maxSteps);
    }
}
