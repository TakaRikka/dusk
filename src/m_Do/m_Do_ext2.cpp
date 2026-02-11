
#include "d/dolzel.h" // IWYU pragma: keep

#include "m_Do/m_Do_ext2.h"

FixedMemoryCheck* FixedMemoryCheck::sFirst;

FixedMemoryCheck::FixedMemoryCheck(u32* param_1, u32 size, JKRHeap* heap) {
    mNowCode = param_1;
    mSize = size;
    mSaveCode = NULL;
    mpHeap = heap;
    field_0x10 = 0;
    field_0x14 = false;
    mNext = NULL;
    if (sFirst) {
        mNext = sFirst;
    }
    sFirst = this;
}

FixedMemoryCheck* FixedMemoryCheck::easyCreate(void* param_1, u32 size) {
    JKRHeap* heap = JKRGetRootHeap2();
    FixedMemoryCheck* memCheck = new (heap, 0) FixedMemoryCheck((u32*)param_1, size, heap);
    if (!memCheck) {
        memCheck = new (JKRGetSystemHeap(), 0) FixedMemoryCheck((u32*)param_1, size, NULL);
    }
    if (memCheck) {
        memCheck->alloc();
        memCheck->save();
        memCheck->check();
    }
    return memCheck;
}

BOOL FixedMemoryCheck::checkAll() {
    BOOL result = true;
    for (FixedMemoryCheck* memCheck = sFirst; memCheck; memCheck = memCheck->mNext) {
        if (!memCheck->check()) {
            result = false;
        }
    }
    return result;
}

BOOL FixedMemoryCheck::diffAll() {
    BOOL result = true;
    for (FixedMemoryCheck* memCheck = sFirst; memCheck; memCheck = memCheck->mNext) {
        if (!memCheck->diff()) {
            result = false;
        }
    }
    return result;
}

BOOL FixedMemoryCheck::saveAll() {
    BOOL result = true;
    for (FixedMemoryCheck* memCheck = sFirst; memCheck; memCheck = memCheck->mNext) {
        if (!memCheck->save()) {
            result = false;
        }
    }
    return result;
}

BOOL FixedMemoryCheck::alloc() {
    if (!mpHeap) {
        OSReport_Error("FixedMemoryCheck::alloc ヒープ未設定\n");
        return false;
    }

    if (mSaveCode) {
        OSReport_Error("FixedMemoryCheck::alloc セーブ確保済み\n");
        return false;
    }

    mSaveCode = (u32*)JKRAllocFromHeap(mpHeap, mSize, 4);
    OSReport("FixedMemoryCheck::alloc: %dKB確保\n", mSize >> 10);
    return mSaveCode != NULL;
}

BOOL FixedMemoryCheck::save() {
    if (!mNowCode) {
        OSReport_Error("FixedMemoryCheck::save 設定ミス\n");
        return false;
    }
    if (mSaveCode) {
        memcpy(mSaveCode, mNowCode, mSize);
    }
    u32 r28 = 0;
    for (int i = 0; i < mSize / 4; i++) {
        r28 += mNowCode[i];
    }
    field_0x10 = r28;
    OSReport("FixedMemoryCheck::save: チェックサム=%08x\n", field_0x10);
    field_0x14 = false;
    return true;
}

BOOL FixedMemoryCheck::check() {
    BOOL result = true;
    if (!mSaveCode || !mNowCode) {
        OSReport_Error("FixedMemoryCheck::check 設定ミス\n");
        return false;
    }
    u32 r29 = 0;
    for (int i = 0; i < mSize / 4; i++) {
        r29 += mNowCode[i];
    }
    if (r29 != field_0x10) {
        OSReport_Error("FixedMemoryCheck::check: チェックサム不一致 %08x : %08x\n", field_0x10, r29);
        result = false;
    }
    if (mSaveCode) {
        int cmp = memcmp(mSaveCode, mNowCode, mSize);
        if(cmp) {
            OSReport_Error("FixedMemoryCheck::check メモリ破壊発見 %08x %08x\n", mNowCode, mSize);
            result = false;
            if (!field_0x14) {
                diff();
                field_0x14 = true;
            }
        }
    }
    return result;
}

BOOL FixedMemoryCheck::diff() {
    BOOL result = true;
    OSReport("Address  SaveCode NowCode\n");
    for (int i = 0; i < mSize / 4; i++) {
        if (mSaveCode[i] != mNowCode[i]) {
            result = false;
            OSReport("%08x %08x %08x\n", mNowCode + i, mSaveCode[i], mNowCode[i]);
        }
    }
    return result;
}
