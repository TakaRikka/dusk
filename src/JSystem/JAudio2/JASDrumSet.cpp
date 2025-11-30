#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASDrumSet.h"
#include "JSystem/JAudio2/JASCalc.h"
#include "JSystem/JKernel/JKRHeap.h"


JASDrumSet::JASDrumSet() {
    field_0x4 = NULL;
    field_0x8 = 0;
}

JASDrumSet::~JASDrumSet() {
}

void JASDrumSet::newPercArray(u8 param_0, JKRHeap* param_1) {
    if (param_0) {
        field_0x8 = param_0;
        field_0x4 = new (param_1, 0) TPerc*[field_0x8];
        JASCalc::bzero(field_0x4, field_0x8 * sizeof(TPerc*));
    }
}

static JASOscillator::Data osc;

static JASOscillator::Data* oscp;

static s8 data_80451274[4];

bool JASDrumSet::getParam(int param_0, int param_1, JASInstParam* param_2) const {
    if (field_0x4 == NULL) {
        return false;
    }

    if (param_0 >= field_0x8) {
        return false;
    }

    TPerc* perc = field_0x4[param_0];
    if (perc == NULL) {
        return false;
    }

    param_2->field_0x1c = 0;
    param_2->field_0x1e = 1;
    param_2->mVolume = perc->mVolume;
    param_2->mPitch = perc->mPitch;
    param_2->mPan = perc->mPan;
    param_2->field_0x18 = perc->field_0xc;
    osc.mTarget = 0;
    osc._04 = 1.0f;
    osc.mTable = NULL;
    osc._0C = NULL;
    osc.mScale = 1.0f;
    osc._14 = 0.0f;

    if (data_80451274[0] == 0) {
        oscp = &osc;
        data_80451274[0] = 1;
    }

    param_2->field_0x14 = &oscp;
    param_2->field_0x1d = 1;
    param_2->mVolume *= perc->field_0x10;
    param_2->mPitch *= perc->field_0x14;
    param_2->field_0x1a = perc->field_0xe;
    return true;
}

void JASDrumSet::setPerc(int param_0, JASDrumSet::TPerc* param_1) {
    field_0x4[param_0] = param_1;
}


JASDrumSet::TPerc::TPerc() {
    mVolume = 1.0f;
    mPitch = 1.0f;
    mPan = 0.5f;
    field_0xc = 1000;
}

void JASDrumSet::TPerc::setRelease(u32 param_0) {
    field_0xc = param_0;
}

u32 JASDrumSet::getType() const { return 'PERC'; }
