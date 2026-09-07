#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASBasicBank.h"
#include "JSystem/JAudio2/JASCalc.h"

JASBasicBank::JASBasicBank() {
    mInstTable = NULL;
    mInstNumMax = 0;
}

void JASBasicBank::newInstTable(u8 num, JKRHeap* heap) {
    if (num != 0) {
        JUT_ASSERT(31, num <= JASBank::PRG_OSC);
        mInstNumMax = num;
        mInstTable = JKR_NEW_ARRAY_ARGS(JASInst*, mInstNumMax, heap, 0);
#if TARGET_ANDROID
        JASCalc::_bzero(mInstTable, mInstNumMax * sizeof(mInstTable[0]));
#else
        JASCalc::bzero(mInstTable, mInstNumMax * sizeof(mInstTable[0]));
#endif
    }
}

bool JASBasicBank::getInstParam(int prg_no, int key, int velocity,
                                JASInstParam* o_param) const {
    JASInst* inst = getInst(prg_no);
    if (inst == NULL) {
        return false;
    }
    return inst->getParam(key, velocity, o_param);
}

void JASBasicBank::setInst(int prg_no, JASInst* inst) {
    if (mInstTable != NULL) {
        JUT_ASSERT(50, prg_no < mInstNumMax);
        JUT_ASSERT(54, prg_no >= 0);
        JUT_ASSERT(56, mInstTable[prg_no] == 0);
        mInstTable[prg_no] = inst;
    }
}

JASInst* JASBasicBank::getInst(int prg_no) const {
    if (prg_no < 0) {
        return NULL;
    }
    if (prg_no >= mInstNumMax) {
        return NULL;
    }
    if (mInstTable == NULL) {
        return NULL;
    }
    return mInstTable[prg_no];
}
