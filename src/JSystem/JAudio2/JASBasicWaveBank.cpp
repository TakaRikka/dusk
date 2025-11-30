#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASBasicWaveBank.h"
#include "JSystem/JAudio2/JASMutex.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"

JASBasicWaveBank::JASBasicWaveBank() {
    mWaveTable = NULL;
    mWaveGroupArray = NULL;
    mHandleCount = 0;
    mGroupCount = 0;
    OSInitMutex(&field_0x4);
}

JASBasicWaveBank::~JASBasicWaveBank() {
    delete[] mWaveTable;
    delete[] mWaveGroupArray;
}

JASBasicWaveBank::TWaveGroup* JASBasicWaveBank::getWaveGroup(u32 param_0) {
    if (param_0 >= mGroupCount) {
        return NULL;
    }
    return mWaveGroupArray + param_0;
}

void JASBasicWaveBank::setGroupCount(u32 param_0, JKRHeap* param_1) {
    delete[] mWaveGroupArray;
    mGroupCount = param_0;
    mWaveGroupArray = new(param_1, 0) TWaveGroup[param_0];
    JUT_ASSERT(62, mWaveGroupArray != NULL);
    for (int i = 0; i < mGroupCount; i++) {
        mWaveGroupArray[i].mBank = this;
    }
}

void JASBasicWaveBank::setWaveTableSize(u32 param_0, JKRHeap* param_1) {
    delete[] mWaveTable;
    mWaveTable = new(param_1, 0) TWaveHandle[param_0];
    JUT_ASSERT(92, mWaveTable != NULL);
    mHandleCount = param_0;
}

void JASBasicWaveBank::incWaveTable(JASBasicWaveBank::TWaveGroup const* param_0) {
    JASMutexLock lock(&field_0x4);
    for (u32 i = 0; i < param_0->getWaveCount(); i++) {
        TWaveHandle* handle = mWaveTable + param_0->getWaveID(i);
        if (!handle->mHeap) {
            handle->mHeap = &param_0->mHeap;
            handle->field_0x4.field_0x20 = &param_0->_48;
            handle->field_0x4.field_0x08 = param_0->mCtrlWaveArray[i].field_0x4;
        }
    }
}

u32 JASBasicWaveBank::mNoLoad;

void JASBasicWaveBank::decWaveTable(JASBasicWaveBank::TWaveGroup const* param_0) {
    JASMutexLock lock(&field_0x4);
    for (u32 i = 0; i < param_0->getWaveCount(); i++) {
        TWaveHandle* handle = mWaveTable + param_0->getWaveID(i);
        if (handle->mHeap == &param_0->mHeap) {
            handle->mHeap = NULL;
            handle->field_0x4.field_0x20 = &mNoLoad;
            handle->field_0x4.field_0x08 = -1;
        }
    }
}

JASWaveHandle* JASBasicWaveBank::getWaveHandle(u32 param_0) const {
    if (param_0 >= mHandleCount) {
        return NULL;
    }
    if (!mWaveTable[param_0].mHeap) {
        return NULL;
    }
    return mWaveTable + param_0;
}

void JASBasicWaveBank::setWaveInfo(JASBasicWaveBank::TWaveGroup* wgrp, int index,
                                   u16 param_2, JASWaveInfo const& param_3) {
    JUT_ASSERT(204, wgrp);
    JUT_ASSERT(205, index < wgrp->mWaveCount);
    JUT_ASSERT(206, index >= 0);
    mWaveTable[param_2].field_0x4 = param_3;
    mWaveTable[param_2].field_0x4.field_0x20 = &mNoLoad;
    mWaveTable[param_2].field_0x4.field_0x08 = -1;
    wgrp->mCtrlWaveArray[index].field_0x0 = param_2;
    wgrp->mCtrlWaveArray[index].field_0x4 = param_3.field_0x08;
}

JASBasicWaveBank::TWaveGroup::TWaveGroup() {
    mBank = NULL;
    mCtrlWaveArray = NULL;
    mWaveCount = 0;
}

JASBasicWaveBank::TWaveGroup::~TWaveGroup() {
    delete[] mCtrlWaveArray;
}

void JASBasicWaveBank::TWaveGroup::setWaveCount(u32 param_0, JKRHeap* param_1) {
    delete[] mCtrlWaveArray;
    mWaveCount = param_0;
    mCtrlWaveArray = new(param_1, 0) TGroupWaveInfo[param_0];
    JUT_ASSERT(255, mCtrlWaveArray != NULL);
}

void JASBasicWaveBank::TWaveGroup::onLoadDone() {
    JUT_ASSERT(286, mBank);
    mBank->incWaveTable(this);
}

void JASBasicWaveBank::TWaveGroup::onEraseDone() {
    JUT_ASSERT(292, mBank);
    mBank->decWaveTable(this);
}


u32 JASBasicWaveBank::TWaveGroup::getWaveID(int index) const {
    JUT_ASSERT(298, index < mWaveCount);
    JUT_ASSERT(299, index >= 0);
    return mCtrlWaveArray[index].field_0x0;
}

int JASBasicWaveBank::TWaveHandle::getWavePtr() const {
    JUT_ASSERT(313, mHeap);
    void* base = mHeap->getBase();
    if (base == 0) {
        return 0;
    }
    return (int)base + field_0x4.field_0x08;
}
