#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASBasicBank.h"
#include "JSystem/JAudio2/JASCalc.h"

JASBasicBank::JASBasicBank() {
    mInstTable = NULL;
    mInstNumMax = 0;
}

void JASBasicBank::newInstTable(u8 num, JKRHeap* heap) {
    if (num != 0) {
        mInstNumMax = num;
        mInstTable = new (heap, 0) JASInst*[mInstNumMax];
        JASCalc::bzero(mInstTable, mInstNumMax * 4);
    }
}

bool JASBasicBank::getInstParam(int prg_no, int param_1, int param_2,
                                JASInstParam* o_param) const {
    JASInst* inst = getInst(prg_no);
    if (inst == NULL) {
        return NULL;
    }
    return inst->getParam(param_1, param_2, o_param);
}

void JASBasicBank::setInst(int prg_no, JASInst* inst) {
    if (mInstTable != NULL) {
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
