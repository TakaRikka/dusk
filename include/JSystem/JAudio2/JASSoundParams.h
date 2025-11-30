#ifndef JASSOUNDPARAMS_H
#define JASSOUNDPARAMS_H

#include "dolphin/types.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASSoundParams {
    void clamp();
    void combine(JASSoundParams const&, JASSoundParams const&);

    void initVolume() { mVolume = 1.0f; }
    void initFxMix() { mFxMix = 0.0f; }
    void initPitch() { mPitch = 1.0f; }
    void initPan() { mPan = 0.5f; }
    void initDolby() { mDolby = 0.0f; }
    void init() {
        initVolume();
        initPitch();
        initFxMix();
        initPan();
        initDolby();
    }

    JASSoundParams() { init(); }

    void clampVolume() {
        if (mVolume < 0.0f)
            mVolume = 0.0f;
        else if (mVolume > 1.0f)
            mVolume = 1.0f;
    }

    void clampFxMix(void) {
        if (mFxMix < 0.0f)
            mFxMix = 0.0f;
        else if (mFxMix > 1.0f)
            mFxMix = 1.0f;
    }

    void clampPitch() {
        if (mPitch < 0.0f)
            mPitch = 0.0f;
        else if (mPitch > 8.0f)
            mPitch = 8.0f;
    }

    void clampPan() {
        if (mPan < 0.0f)
            mPan = 0.0f;
        else if (mPan > 1.0f)
            mPan = 1.0f;
    }
    
    void clampDolby() {
        if (mDolby < 0.0f)
            mDolby = 0.0f;
        else if (mDolby > 1.0f)
            mDolby = 1.0f;
    }

    /* 0x00 */ f32 mVolume;
    /* 0x04 */ f32 mFxMix;
    /* 0x08 */ f32 mPitch;
    /* 0x0C */ f32 mPan;
    /* 0x10 */ f32 mDolby;
};  // Size: 0x14

#endif /* JASSOUNDPARAMS_H */
