#ifndef JAISOUNDPARAMS_H
#define JAISOUNDPARAMS_H

#include "JSystem/JAudio2/JASSoundParams.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISoundParamsProperty {
    void init() {
        field_0x0 = 1.0f;
        field_0x4 = 0.0f;
        field_0x8 = 1.0f;
    }

    /* 0x00 */ f32 field_0x0;
    /* 0x04 */ f32 field_0x4;
    /* 0x08 */ f32 field_0x8;
};  // Size: 0xC

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISoundParamsTransition {
    struct TTransition {
        void zero() {
            step_ = 0.0f;
            remainingSteps_ = 0;
            targetValue_ = 0.0f;
        }

        void set(f32 newValue, f32 currentValue, u32 maxSteps) {
            remainingSteps_ = maxSteps;
            step_ = (newValue - currentValue) / remainingSteps_;
            targetValue_ = newValue;
        }

        f32 apply(f32 value) {
            if (remainingSteps_ > 1) {
                remainingSteps_--;
                value += step_;
            } else {
                if (remainingSteps_ == 1) {
                    remainingSteps_ = 0;
                    value = targetValue_;
                }
            }
            return value;
        }

        /* 0x0 */ f32 step_;
        /* 0x4 */ f32 targetValue_;
        /* 0x8 */ u32 remainingSteps_;
    };  // Size: 0xC

    void init() {
        volume_.zero();
        pitch_.zero();
        fxMix_.zero();
        pan_.zero();
        dolby_.zero();
    }

    void apply(JASSoundParams* params) {
        params->mVolume = volume_.apply(params->mVolume);
        params->mPitch = pitch_.apply(params->mPitch);
        params->mFxMix = fxMix_.apply(params->mFxMix);
        params->mDolby = dolby_.apply(params->mDolby);
        params->mPan = pan_.apply(params->mPan);
    }

    /* 0x00 */ TTransition volume_;
    /* 0x0C */ TTransition pitch_;
    /* 0x18 */ TTransition fxMix_;
    /* 0x24 */ TTransition pan_;
    /* 0x30 */ TTransition dolby_;
};  // Size: 0x3C

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISoundParamsMove {
    JAISoundParamsMove() : params_() {}

    void init() {
        params_.init();
        transition_.init();
    }

    void calc() { transition_.apply(&params_); }

    void moveVolume(f32 newValue, u32 maxSteps);
    void movePitch(f32 newValue, u32 maxSteps);
    void moveFxMix(f32 newValue, u32 maxSteps);
    void movePan(f32 newValue, u32 maxSteps);
    void moveDolby(f32 newValue, u32 maxSteps);

    /* 0x00 */ JASSoundParams params_;
    /* 0x14 */ JAISoundParamsTransition transition_;
};  // Size: 0x50

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISoundParams {
    JAISoundParams() : move_() {}
    void mixOutAll(const JASSoundParams& inParams, JASSoundParams* outParams, f32);

    void init() {
        move_.init();
        property_.init();
    }

    /* 0x0 */ JAISoundParamsProperty property_;
    /* 0xC */ JAISoundParamsMove move_;
};  // Size: 0x5C

#endif /* JAISOUNDPARAMS_H */
