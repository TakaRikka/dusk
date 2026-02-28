#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTGraphFifo.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <stdint.h>

static bool data_804514B8;

JUTGraphFifo* JUTGraphFifo::sCurrentFifo;

JUTGraphFifo::JUTGraphFifo(u32 size) {
    mSize = ROUND(size, 0x20);
    if (data_804514B8) {
        u32 fifoSize = sizeof(GXFifoObj);
        mFifo = (GXFifoObj*)JKRAllocFromSysHeap(fifoSize + mSize, 32);
        mBase = (u8*)mFifo + fifoSize;
        GXInitFifoBase(mFifo, mBase, mSize);
        GXInitFifoPtrs(mFifo, mBase, mBase);
    } else {
        mBase = JKRAllocFromSysHeap(mSize + 0xA0, 32);
        mBase = (void*)ROUND((intptr_t)mBase, 0x20);
        mFifo = GXInit(mBase, mSize);
        GXSetColorUpdate(GX_ENABLE);
        data_804514B8 = true;
        sCurrentFifo = this;
    }
}

bool JUTGraphFifo::mGpStatus[5];

JUTGraphFifo::~JUTGraphFifo() {
    sCurrentFifo->save();

    while (isGPActive()) {
        // nop
    }

    if (sCurrentFifo == this) {
        sCurrentFifo = NULL;
    }
    JKRFreeToSysHeap(mBase);
}
