#ifndef M_DO_EXT2_H
#define M_DO_EXT2_H

#include "JSystem/JKernel/JKRHeap.h"

class FixedMemoryCheck {
public:
    FixedMemoryCheck(u32*, u32, JKRHeap*);

    BOOL alloc();
    BOOL save();
    BOOL check();
    BOOL diff();

    static FixedMemoryCheck* easyCreate(void* param_1, s32 param_2) { return easyCreate(param_1, u32(param_2)); }
    static FixedMemoryCheck* easyCreate(void*, u32);
    static BOOL checkAll();
    static BOOL diffAll();
    static BOOL saveAll();

    static FixedMemoryCheck* sFirst;

    /* 0x00 */ u32* mNowCode;
    /* 0x04 */ u32 mSize;
    /* 0x08 */ u32* mSaveCode;
    /* 0x0C */ JKRHeap* mpHeap;
    /* 0x10 */ int field_0x10;
    /* 0x14 */ bool field_0x14;
    /* 0x18 */ FixedMemoryCheck* mNext;
};

#endif
