#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASDrumSet.h"
#include "JSystem/JAudio2/JASCalc.h"
#include "JSystem/JKernel/JKRHeap.h"


JASDrumSet::JASDrumSet() {
    mPercArray = NULL;
    mPercNumMax = 0;
}

JASDrumSet::~JASDrumSet() {
}

void JASDrumSet::newPercArray(u8 num, JKRHeap* heap) {
    if (num) {
        JUT_ASSERT(39, num <= 128);
        mPercNumMax = num;
        mPercArray = new (heap, 0) TPerc*[mPercNumMax];
        JASCalc::bzero(mPercArray, mPercNumMax * sizeof(TPerc*));
    }
}

bool JASDrumSet::getParam(int key, int param_1, JASInstParam* param_2) const {
    UNUSED(param_1);
    JUT_ASSERT(48, key >= 0);
    if (mPercArray == NULL) {
        return false;
    }

    if (key >= mPercNumMax) {
        JUT_WARN(54, "JASDrumSet: key %d >= PERC_MAX %d\n", key, mPercNumMax);
        return false;
    }

    TPerc* perc = mPercArray[key];
    if (perc == NULL) {
        return false;
    }

    param_2->field_0x1c = 0;
    param_2->field_0x1e = 1;
    param_2->mVolume = perc->mVolume;
    param_2->mPitch = perc->mPitch;
    param_2->mPan = perc->mPan;
    param_2->field_0x18 = u16(perc->field_0xc);

    static JASOscillator::Data osc;

    osc.mTarget = 0;
    osc._04 = 1.0f;
    osc.mTable = NULL;
    osc.rel_table = NULL;
    osc.mScale = 1.0f;
    osc._14 = 0.0f;

    static JASOscillator::Data* oscp = &osc;

    param_2->field_0x14 = &oscp;
    param_2->field_0x1d = 1;
    param_2->mVolume *= perc->field_0x10;
    param_2->mPitch *= perc->field_0x14;
    param_2->field_0x1a = u16(perc->field_0xe);
    return true;
}

void JASDrumSet::setPerc(int index, JASDrumSet::TPerc* param_1) {
    JUT_ASSERT(123, index >= 0);
    JUT_ASSERT(125, index < mPercNumMax);
    JUT_ASSERT(126, mPercArray);
    JUT_ASSERT(127, mPercArray[index] == 0);
    mPercArray[index] = param_1;
}

JASDrumSet::TPerc* JASDrumSet::getPerc(int index) {
    if (index < 0) {
        return NULL;
    }
    if (index >= mPercNumMax) {
        return NULL;
    }
    if (!mPercArray) {
        return NULL;
    }
    return mPercArray[index];
}

JASDrumSet::TPerc* JASDrumSet::getPerc(int index) const {
    if (index < 0) {
        return NULL;
    }
    if (index >= mPercNumMax) {
        return NULL;
    }
    if (!mPercArray) {
        return NULL;
    }
    return mPercArray[index];
}

JASDrumSet::TPerc::TPerc() {
    mVolume = 1.0f;
    mPitch = 1.0f;
    mPan = 0.5f;
    field_0xc = 1000;
}

void JASDrumSet::TPerc::setRelease(u32 release) {
    JUT_ASSERT(224, release < 0x10000);
    field_0xc = release;
}

u32 JASDrumSet::getType() const { return 'PERC'; }
