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
    JASCalc::bzero(field_0xc, sizeof(field_0xc));
}

JASBasicInst::~JASBasicInst() {
    delete[] mKeymap;
}

bool JASBasicInst::getParam(int param_0, int param_1, JASInstParam* param_2) const {
    UNUSED(param_1);
    param_2->field_0x1c = 0;
    param_2->field_0x1e = 0;
    param_2->field_0x14 = (JASOscillator::Data**)&field_0xc;
    param_2->field_0x1d = 2;
    param_2->mVolume = mVolume;
    param_2->mPitch = mPitch;

    TKeymap* keyMap = NULL;
    for (int i = 0; i < mKeymapCount; i++) {
        if (param_0 <= mKeymap[i].mHighKey) {
            keyMap = &mKeymap[i];
            break;
        }
    }

    if (keyMap == NULL) {
        return false;
    }

    param_2->mVolume *= keyMap->field_0x8;
    param_2->mPitch *= keyMap->field_0xc;
    param_2->field_0x1a = u16(keyMap->field_0x4);
    return true;
}

void JASBasicInst::setKeyRegionCount(u32 count, JKRHeap* param_1) {
    delete [] mKeymap;
    mKeymap = new (param_1, 0) TKeymap[count];
    JUT_ASSERT(114, mKeymap != NULL);
	mKeymapCount = count;
}

void JASBasicInst::setOsc(int index, JASOscillator::Data const* param_1) {
    JUT_ASSERT(128, index < OSC_MAX);
    JUT_ASSERT(129, index >= 0);
    field_0xc[index] = param_1;
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
