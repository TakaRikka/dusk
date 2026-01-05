#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRAramHeap.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "global.h"
#include <climits>

JSUList<JKRAramBlock> JKRAramHeap::sAramList;

JKRAramHeap::JKRAramHeap(u32 startAddress, u32 size) {
    OSInitMutex(&mMutex);

    mHeap = JKRHeap::findFromRoot(this);
    mSize = ALIGN_PREV(size, 0x20);
    mHeadAddress = ALIGN_NEXT(startAddress, 0x20);
    mTailAddress = mHeadAddress + mSize;
    mGroupId = -1;

    JKRAramBlock* block = new (mHeap, 0) JKRAramBlock(mHeadAddress, 0, mSize, -1, false);
    sAramList.append(&block->mBlockLink);
}

JKRAramHeap::~JKRAramHeap() {
    for (JSUListIterator<JKRAramBlock> iterator = sAramList.getFirst(); iterator != sAramList.getEnd(); delete (iterator++).getObject()) {}
}

JKRAramBlock* JKRAramHeap::alloc(u32 size, JKRAramHeap::EAllocMode allocationMode) {
    lock();

    JKRAramBlock* block;
    if (allocationMode == JKRAramHeap::HEAD) {
        block = allocFromHead(size);
    } else {
        block = allocFromTail(size);
    }

    unlock();
    return block;
}

JKRAramBlock* JKRAramHeap::allocFromHead(u32 size) {
    u32 alignedSize = ALIGN_NEXT(size, 0x20);
    u32 bestFreeSize = UINT_MAX;
    JKRAramBlock* bestBlock = NULL;

    for (JSUListIterator<JKRAramBlock> iterator = sAramList.getFirst(); iterator != sAramList.getEnd(); ++iterator) {
        JKRAramBlock* block = iterator.getObject();
        if (block->mFreeSize < alignedSize) {
            continue;
        }
        if (bestFreeSize <= block->mFreeSize) {
            continue;
        }

        bestFreeSize = block->mFreeSize;
        bestBlock = block;

        if (bestFreeSize == alignedSize) {
            break;
        }
    }

    if (bestBlock) {
        return bestBlock->allocHead(alignedSize, mGroupId, this);
    }

    return NULL;
}

JKRAramBlock* JKRAramHeap::allocFromTail(u32 size) {
    u32 alignedSize = ALIGN_NEXT(size, 0x20);
    JKRAramBlock* tailBlock = NULL;

    for (JSUListIterator<JKRAramBlock> iterator = sAramList.getLast(); iterator != sAramList.getEnd(); --iterator) {
        JKRAramBlock* block = iterator.getObject();
        if (block->mFreeSize >= alignedSize) {
            tailBlock = block;
            break;
        }
    }

    if (tailBlock) {
        return tailBlock->allocTail(alignedSize, mGroupId, this);
    }

    return NULL;
}

u32 JKRAramHeap::getFreeSize(void) {
    u32 maxFreeSize = 0;

    lock();

    for (JSUListIterator<JKRAramBlock> iterator = sAramList.getFirst(); iterator != sAramList.getEnd(); ++iterator) {
        if (iterator->mFreeSize > maxFreeSize) {
            maxFreeSize = iterator->mFreeSize;
        }
    }

    unlock();
    return maxFreeSize;
}

u32 JKRAramHeap::getTotalFreeSize(void) {
    u32 totalFreeSize = 0;

    lock();

    for (JSUListIterator<JKRAramBlock> iterator = sAramList.getFirst(); iterator != sAramList.getEnd(); ++iterator) {
        totalFreeSize += iterator->mFreeSize;
    }

    unlock();
    return totalFreeSize;
}

void JKRAramHeap::dump(void) {
    lock();

    OS_REPORT("\nJKRAramHeap dump\n");
    OS_REPORT(" attr  address:   size    gid\n");

    u32 usedBytes = 0;
    for (JSUListIterator<JKRAramBlock> it = sAramList.getFirst(); it != sAramList.getEnd(); ++it) {
        if (it->mSize) {
            OS_REPORT("%s %08x: %08x  %3d\n", it->isTempMemory() ? " temp" : "alloc", it->mAddress, it->mSize, it->mGroupId);
        }
        if (it->mFreeSize) {
            OS_REPORT(" free %08x: %08x    0\n", it->mAddress + it->mSize, it->mFreeSize);
        }
        usedBytes += it->mSize;
    }

    OS_REPORT("%d / %d bytes (%6.2f%%) used\n", usedBytes, mSize, f32(usedBytes) / f32(mSize) * 100.0f);

    unlock();
}
