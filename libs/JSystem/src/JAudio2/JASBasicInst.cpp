//
// JASBasicInst
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASBasicInst.h"
#include "JSystem/JAudio2/JASCalc.h"
#include "JSystem/JKernel/JKRHeap.h"

JASBasicInst::JASBasicInst() {
    mVolume = 1.0;
    mPitch = 1.0;
    mKeymapCount = 0;
    mKeymap = NULL;
#if TARGET_ANDROID
    JASCalc::_bzero(mOscillators, sizeof(mOscillators));
#else
    JASCalc::bzero(mOscillators, sizeof(mOscillators));
#endif
}

JASBasicInst::~JASBasicInst() {
    JKR_DELETE_ARRAY(mKeymap);
}

bool JASBasicInst::getParam(int key, int velocity, JASInstParam* o_param) const {
    UNUSED(velocity);
    o_param->mChannelType = 0;
    o_param->mDontSetKey = 0;
    o_param->mOscillators = (JASOscillator::Data**)&mOscillators;
    o_param->mOscillatorCount = 2;
    o_param->mVolume = mVolume;
    o_param->mPitch = mPitch;

    TKeymap* keyMap = NULL;
    for (int i = 0; i < mKeymapCount; i++) {
        if (key <= mKeymap[i].mHighKey) {
            keyMap = &mKeymap[i];
            break;
        }
    }

    if (keyMap == NULL) {
        return false;
    }

    o_param->mVolume *= keyMap->mVolumeMult;
    o_param->mPitch *= keyMap->mPitchMult;
    o_param->mWaveId = u16(keyMap->mWaveId);
    return true;
}

void JASBasicInst::setKeyRegionCount(u32 count, JKRHeap* param_1) {
    JKR_DELETE_ARRAY(mKeymap);
    mKeymap = JKR_NEW_ARRAY_ARGS(TKeymap, count, param_1, 0);
    JUT_ASSERT(114, mKeymap != NULL);
	mKeymapCount = count;
}

void JASBasicInst::setOsc(int index, JASOscillator::Data const* param_1) {
    JUT_ASSERT(128, index < OSC_MAX);
    JUT_ASSERT(129, index >= 0);
    mOscillators[index] = param_1;
}

JASBasicInst::TKeymap* JASBasicInst::getKeyRegion(int index) {
    JUT_ASSERT(146, index >= 0);
    if (index >= mKeymapCount) {
        return NULL;
    }

    return mKeymap + index;
}

JASBasicInst::TKeymap* JASBasicInst::getKeyRegion(int index) const {
    JUT_ASSERT(155, index >= 0);
    if (index >= mKeymapCount) {
        return NULL;
    }

    return mKeymap + index;
}

JASBasicInst::TKeymap::~TKeymap() {
}
