#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTGraphFifo.h"
#include "JSystem/JKernel/JKRHeap.h"

static bool data_804514B8;

JUTGraphFifo* JUTGraphFifo::sCurrentFifo;

JUTGraphFifo::JUTGraphFifo(u32 size) {
    mSize = size + 0x1F & ~0x1F;
    if (data_804514B8) {
        mFifo = (GXFifoObj*)JKRAllocFromSysHeap(mSize + 0x80, 32);
        mBase = mFifo + 1;
        GXInitFifoBase(mFifo, mBase, mSize);
        GXInitFifoPtrs(mFifo, mBase, mBase);
    } else {
        mBase = JKRAllocFromSysHeap(mSize + 0xA0, 32);
        mBase = (void*)((int)mBase + 0x1F & ~0x1F);
        mFifo = GXInit(mBase, mSize);
        data_804514B8 = true;
        sCurrentFifo = this;
    }
}

bool JUTGraphFifo::mGpStatus[5];

JUTGraphFifo::~JUTGraphFifo() {
    sCurrentFifo->save();

    do {
    } while (isGPActive());

    if (sCurrentFifo == this) {
        sCurrentFifo = NULL;
    }
    JKRFreeToSysHeap(mBase);
}
