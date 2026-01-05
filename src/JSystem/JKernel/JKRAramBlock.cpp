#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRAramBlock.h"
#include "JSystem/JKernel/JKRAramHeap.h"
#include "JSystem/JKernel/JKRHeap.h"

JKRAramBlock::JKRAramBlock(u32 address, u32 size, u32 freeSize, u8 groupId, bool isTempMemory)
    : mBlockLink(this) {
    mAddress = address;
    mSize = size;
    mFreeSize = freeSize;
    mGroupId = groupId;
    mIsTempMemory = isTempMemory;
}

JKRAramBlock::~JKRAramBlock() {
    JSUList<JKRAramBlock>* list = mBlockLink.getSupervisor();
    JSULink<JKRAramBlock>* prev = mBlockLink.getPrev();

    if (prev) {
        prev->getObject()->mFreeSize += mSize + mFreeSize;
        list->remove(&mBlockLink);
    } else {
        mFreeSize = mFreeSize + mSize;
        mSize = 0;
    }
}

JKRAramBlock* JKRAramBlock::allocHead(u32 size, u8 groupId, JKRAramHeap* aramHeap) {
    u32 nextAddress = mAddress + mSize;
    u32 nextFreeSize = mFreeSize - size;

    JKRAramBlock* block = new (aramHeap->getMgrHeap(), 0)
        JKRAramBlock(nextAddress, size, nextFreeSize, groupId, false);

    mFreeSize = 0;
    mBlockLink.getSupervisor()->insert(mBlockLink.getNext(), &block->mBlockLink);
    return block;
}

JKRAramBlock* JKRAramBlock::allocTail(u32 size, u8 groupId, JKRAramHeap* aramHeap) {
    u32 tailAddress = mAddress + mSize + mFreeSize - size;

    JKRAramBlock* block =
        new (aramHeap->getMgrHeap(), 0) JKRAramBlock(tailAddress, size, 0, groupId, true);

    mFreeSize -= size;
    mBlockLink.getSupervisor()->insert(mBlockLink.getNext(), &block->mBlockLink);
    return block;
}
