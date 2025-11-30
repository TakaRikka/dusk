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
    param_2->field_0x1a = keyMap->field_0x4;
    return true;
}

void JASBasicInst::setKeyRegionCount(u32 count, JKRHeap* param_1) {
    delete [] mKeymap;
    mKeymap      = new (param_1, 0) TKeymap[count];
	mKeymapCount = count;
}

void JASBasicInst::setOsc(int param_0, JASOscillator::Data const* param_1) {
    field_0xc[param_0] = param_1;
}

JASBasicInst::TKeymap* JASBasicInst::getKeyRegion(int param_0) {
    if (param_0 >= mKeymapCount) {
        return NULL;
    }

    return mKeymap + param_0;
}

JASBasicInst::TKeymap::~TKeymap() {
}
