#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "global.h"
#include <stdint.h>

JKRSolidHeap* JKRSolidHeap::create(u32 size, JKRHeap* heap, bool useErrorHandler) {
    if (!heap) {
        heap = getRootHeap();
    }

    if (size == -1) {
        size = heap->getMaxAllocatableSize(0x10);
    }

    u32 alignedSize = ALIGN_PREV(size, 0x10);
    u32 solidHeapSize = ALIGN_NEXT(sizeof(JKRSolidHeap), 0x10);
    if (alignedSize < solidHeapSize)
        return NULL;

    JKRSolidHeap* solidHeap = (JKRSolidHeap*)JKRAllocFromHeap(heap, alignedSize, 0x10);
    void* dataPtr = (u8*)solidHeap + solidHeapSize;
    if (!solidHeap)
        return NULL;

    solidHeap =
        new (solidHeap) JKRSolidHeap(dataPtr, alignedSize - solidHeapSize, heap, useErrorHandler);
    return solidHeap;
}

void JKRSolidHeap::do_destroy(void) {
    JKRHeap* parent = getParent();
    if (parent) {
        this->~JKRSolidHeap();
        JKRFreeToHeap(parent, this);
    }
}

JKRSolidHeap::JKRSolidHeap(void* start, u32 size, JKRHeap* parent, bool useErrorHandler)
    : JKRHeap(start, size, parent, useErrorHandler) {
    mFreeSize = mSize;
    mSolidHead = (u8*)mStart;
    mSolidTail = (u8*)mEnd;
    field_0x78 = NULL;
}

JKRSolidHeap::~JKRSolidHeap(void) {
    dispose();
}

s32 JKRSolidHeap::adjustSize(void) {
    JKRHeap* parent = getParent();
    if (parent) {
        lock();
        u32 thisSize = (uintptr_t)mStart - (uintptr_t)this;
        u32 newSize = ALIGN_NEXT(mSolidHead - mStart, 0x20);
        if (parent->resize(this, thisSize + newSize) != -1) {
            mFreeSize = 0;
            mSize = newSize;
            mEnd = mStart + mSize;
            mSolidHead = mEnd;
            mSolidTail = mEnd;
        }

        unlock();

        return thisSize + newSize;
    }

    return -1;
}

void* JKRSolidHeap::do_alloc(u32 size, int alignment) {
#if DEBUG
    // TODO(Julgodis): JUTAssertion::setConfirmMessage
    /* if (alignment != 0) {
        int u = abs(alignment);
        JUT_ASSERT(219, u < 0x80);
        JUT_ASSERT(220, JGadget::binary::isPower2(u));
    } */
#endif

    lock();

    if (size < 4) {
        size = 4;
    }

    void* ptr;
    if (alignment >= 0) {
        ptr = allocFromHead(size, alignment < 4 ? 4 : alignment);
    } else {
        if (-alignment < 4) {
            alignment = 4;
        } else {
            alignment = -alignment;
        }

        ptr = allocFromTail(size, alignment);
    }

    unlock();
    return ptr;
}

void* JKRSolidHeap::allocFromHead(u32 size, int alignment) {
    size = ALIGN_NEXT(size, 0x4);
    void* ptr = NULL;
    u32 alignedStart = (alignment - 1 + (uintptr_t)mSolidHead) & ~(alignment - 1);
    u32 offset = alignedStart - (uintptr_t)mSolidHead;
    u32 totalSize = size + offset;
    if (totalSize <= mFreeSize) {
        ptr = (void*)alignedStart;
        mSolidHead += totalSize;
        mFreeSize -= totalSize;
    } else {
        JUTWarningConsole_f("allocFromHead: cannot alloc memory (0x%x byte).\n", totalSize);
        if (getErrorFlag() == true) {
            callErrorHandler(this, size, alignment);
        }
    }

    return ptr;
}

void* JKRSolidHeap::allocFromTail(u32 size, int alignment) {
    size = ALIGN_NEXT(size, 4);
    void* ptr = NULL;
    u32 alignedStart = ALIGN_PREV((uintptr_t)mSolidTail - size, alignment);
    u32 totalSize = (uintptr_t)mSolidTail - (uintptr_t)alignedStart;
    if (totalSize <= mFreeSize) {
        ptr = (void*)alignedStart;
        mSolidTail -= totalSize;
        mFreeSize -= totalSize;
    } else {
        JUTWarningConsole_f("allocFromTail: cannot alloc memory (0x%x byte).\n", totalSize);
        if (getErrorFlag() == true) {
            callErrorHandler(this, size, alignment);
        }
    }
    return ptr;
}

void JKRSolidHeap::do_free(void* ptr) {
    JUTWarningConsole_f("free: cannot free memory block (%08x)\n", ptr);
}

void JKRSolidHeap::do_freeAll(void) {
    lock();

    this->JKRHeap::callAllDisposer();
    mFreeSize = mSize;
    mSolidHead = (u8*)mStart;
    mSolidTail = (u8*)mEnd;
    field_0x78 = NULL;

    unlock();
}

void JKRSolidHeap::do_freeTail(void) {
    lock();

    if (mSolidTail != mEnd) {
        dispose(mSolidTail, mEnd);
    }

    this->mFreeSize = ((uintptr_t)mEnd - (uintptr_t)mSolidTail + mFreeSize);
    this->mSolidTail = mEnd;

    JKRSolidHeap::Unknown* unknown = field_0x78;
    while (unknown) {
        unknown->field_0xc = mEnd;
        unknown = unknown->mNext;
    }

    unlock();
}

void JKRSolidHeap::do_fillFreeArea() {
#if DEBUG
    // fillMemory(mSolidHead, mEnd - mSolidHead, (uint)DAT_8074a8ba);
#endif
}

s32 JKRSolidHeap::do_resize(void* ptr, u32 newSize) {
    JUTWarningConsole_f("resize: cannot resize memory block (%08x: %d)\n", ptr, newSize);
    return -1;
}

s32 JKRSolidHeap::do_getSize(void* ptr) {
    JUTWarningConsole_f("getSize: cannot get memory block size (%08x)\n", ptr);
    return -1;
}

bool JKRSolidHeap::check(void) {
    lock();

    bool result = true;
    u32 calculatedSize =
        ((uintptr_t)mSolidHead - (uintptr_t)mStart) + mFreeSize + ((uintptr_t)mEnd - (uintptr_t)mSolidTail);
    u32 availableSize = mSize;
    if (calculatedSize != availableSize) {
        result = false;
        JUTWarningConsole_f("check: bad total memory block size (%08X, %08X)\n", availableSize,
                            calculatedSize);
    }

    unlock();
    return result;
}

bool JKRSolidHeap::dump(void) {
    bool result = check();

    lock();
    u32 headSize = ((uintptr_t)mSolidHead - (uintptr_t)mStart);
    u32 tailSize = ((uintptr_t)mEnd - (uintptr_t)mSolidTail);
    s32 htSize = headSize + tailSize;
    JUTReportConsole_f("head %08x: %08x\n", mStart, headSize);
    JUTReportConsole_f("tail %08x: %08x\n", mSolidTail, ((uintptr_t)mEnd - (uintptr_t)mSolidTail));

    u32 totalSize = mSize;
    float percentage = (float)htSize / (float)totalSize * 100.0f;
    JUTReportConsole_f("%d / %d bytes (%6.2f%%) used\n", htSize, totalSize, percentage);
    unlock();

    return result;
}
void JKRSolidHeap::state_register(JKRHeap::TState* p, u32 id) const {
    JUT_ASSERT(604, p != NULL);
    JUT_ASSERT(605, p->getHeap() == this);

    getState_(p);
    setState_u32ID_(p, id);
    setState_uUsedSize_(p, getUsedSize((JKRSolidHeap*)this));
    u32 r29 = (uintptr_t)mSolidHead;
    r29 += (uintptr_t)mSolidTail * 3;
    setState_u32CheckCode_(p, r29);
}

bool JKRSolidHeap::state_compare(JKRHeap::TState const& r1, JKRHeap::TState const& r2) const {
    JUT_ASSERT(632, r1.getHeap() == r2.getHeap());

    bool result = true;
    if (r1.getCheckCode() != r2.getCheckCode()) {
        result = false;
    }

    if (r1.getUsedSize() != r2.getUsedSize()) {
        result = false;
    }

    return result;
}

u32 JKRSolidHeap::getHeapType(void) {
    return 'SLID';
}

s32 JKRSolidHeap::do_getFreeSize(void) {
    return mFreeSize;
}

void* JKRSolidHeap::do_getMaxFreeBlock(void) {
    return mSolidHead;
}

s32 JKRSolidHeap::do_getTotalFreeSize(void) {
    return getFreeSize();
}
