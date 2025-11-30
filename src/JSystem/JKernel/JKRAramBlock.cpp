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
        JKRAramBlock* block = prev->getObject();
        block->mFreeSize = mSize + mFreeSize + block->mFreeSize;
        list->remove(&mBlockLink);
    } else {
        mFreeSize = mFreeSize + mSize;
        mSize = 0;
    }
}

JKRAramBlock* JKRAramBlock::allocHead(u32 size, u8 groupId, JKRAramHeap* aramHeap) {
    u32 address = mAddress;
    u32 usedSize = mSize;
    u32 nextAddress = address + usedSize;
    u32 freeSize = mFreeSize;
    u32 nextFreeSize = freeSize - size;

    JKRAramBlock* block = new (aramHeap->getMgrHeap(), 0)
        JKRAramBlock(nextAddress, size, nextFreeSize, groupId, false);

    mFreeSize = 0;
    JSULink<JKRAramBlock>* next = mBlockLink.getNext();
    JSUList<JKRAramBlock>* list = mBlockLink.getSupervisor();
    list->insert(next, &block->mBlockLink);
    return block;
}

JKRAramBlock* JKRAramBlock::allocTail(u32 size, u8 groupId, JKRAramHeap* aramHeap) {
    u32 freeSize = mFreeSize;
    u32 address = mAddress;
    u32 usedSize = mSize;
    u32 endAddress = address + usedSize + freeSize;
    u32 tailAddress = endAddress - size;

    JKRAramBlock* block =
        new (aramHeap->getMgrHeap(), 0) JKRAramBlock(tailAddress, size, 0, groupId, true);

    mFreeSize -= size;
    JSULink<JKRAramBlock>* next = mBlockLink.getNext();
    JSUList<JKRAramBlock>* list = mBlockLink.getSupervisor();
    list->insert(next, &block->mBlockLink);
    return block;
}
