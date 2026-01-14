//
// JKRExpHeap
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JGadget/binary.h"
#include "JSystem/JSupport/JSupport.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JUtility/JUTException.h"
#include <cstdlib>

JKRExpHeap* JKRExpHeap::createRoot(int maxHeaps, bool errorFlag) {
    JKRExpHeap* heap = NULL;
    if (!sRootHeap) {
#if !PLATFORM_GCN
        u8* mem2;
        u32 mem2Size;
        initArena2((char**)&mem2, &mem2Size, 1);
        u32 local_1c = ALIGN_NEXT(sizeof(JKRExpHeap), 0x10);
        u8* local_20 = mem2 + local_1c;
        u32 local_24 = mem2Size - local_1c;
#if DEBUG
        if (local_20) {
            u8* local_28 = local_20;
            u32 local_2c = local_24;
            if (isDefaultDebugFill()) {
                JKRFillMemory(local_28, local_2c, JKRValue_DEBUGFILL_NOTUSE);
            }
        }
#endif
        JKRExpHeap* heap2 = new (mem2) JKRExpHeap(local_20, local_24, NULL, errorFlag);
        sRootHeap2 = heap2;
        heap2->field_0x6e = true;
#endif
        void* mem1;
        u32 mem1Size;
        initArena((char**)&mem1, &mem1Size, maxHeaps);
        u32 local_3c = ALIGN_NEXT(sizeof(JKRExpHeap), 0x10);
        u8* start1 = (u8*)mem1 + local_3c;
        u32 alignedSize = mem1Size - local_3c;
#if DEBUG
        if (start1) {
            u8* local_4c = start1;
            u32 local_48 = alignedSize;
            if (isDefaultDebugFill()) {
                JKRFillMemory(local_4c, local_48, JKRValue_DEBUGFILL_NOTUSE);
            }
        }
#endif
        heap = new (mem1) JKRExpHeap(start1, alignedSize, NULL, errorFlag);
        sRootHeap = heap;
    }
    heap->field_0x6e = true;
    return heap;
}

JKRExpHeap* JKRExpHeap::create(u32 size, JKRHeap* parent, bool errorFlag) {
    JKRExpHeap* newHeap = NULL;
    if (!parent) {
        parent = sRootHeap;
    }
    u32 expHeapSize = ALIGN_NEXT(sizeof(JKRExpHeap), 0x10);
    u32 blockSize = sizeof(CMemBlock);

    if (size == 0xffffffff) {
        size = parent->getMaxAllocatableSize(0x10);
    }

    u32 alignedSize = ALIGN_PREV(size, 0x10);

    if (alignedSize < expHeapSize + blockSize)
        return NULL;

    u8* memory = (u8*)JKRAllocFromHeap(parent, alignedSize, 0x10);
    u8* dataPtr = (memory + expHeapSize);
    if (!memory) {
        return NULL;
    }

    newHeap = new (memory) JKRExpHeap(dataPtr, alignedSize - expHeapSize, parent, errorFlag);

    if (newHeap == NULL) {
        JKRFree(memory);
        return NULL;
    }
#if DEBUG
    if (newHeap) {
        u8* local_30 = dataPtr + sizeof(CMemBlock);
        u32 local_34 = newHeap->mHeadFreeList->size;
        if (isDefaultDebugFill()) {
            JKRFillMemory(local_30, local_34, JKRValue_DEBUGFILL_NOTUSE);
        }
    }
#endif
    newHeap->field_0x6e = false;
    return newHeap;
}

JKRExpHeap* JKRExpHeap::create(void* ptr, u32 size, JKRHeap* parent, bool errorFlag) {
    JKRHeap* parent2;
    if (parent == NULL) {
        parent2 = getRootHeap()->find(ptr);
        if (!parent2)
            return NULL;
    } else {
        parent2 = parent;
    }

    JKRExpHeap* newHeap = NULL;
    u32 expHeapSize = ALIGN_NEXT(sizeof(JKRExpHeap), 0x10);
    if (size < expHeapSize)
        return NULL;

    u8* r28 = (u8*)ptr;
    u8* dataPtr = r28 + expHeapSize;
    u32 alignedSize = ALIGN_PREV((uintptr_t)ptr + size - (uintptr_t)dataPtr, 0x10);
    if (r28) {
        newHeap = new (r28) JKRExpHeap(dataPtr, alignedSize, parent2, errorFlag);
    }
#if DEBUG
    if (newHeap) {
        u8* local_30 = dataPtr + sizeof(CMemBlock);
        u32 local_34 = newHeap->mHeadFreeList->size;
        if (isDefaultDebugFill()) {
            JKRFillMemory(local_30, local_34, JKRValue_DEBUGFILL_NOTUSE);
        }
    }
#endif
    newHeap->field_0x6e = true;
    newHeap->field_0x70 = ptr;
    newHeap->field_0x74 = size;
    return newHeap;
}

void JKRExpHeap::do_destroy() {
    if (!field_0x6e) {
        JKRHeap* heap = getParent();
        if (heap) {
            this->~JKRExpHeap();
            JKRFreeToHeap(heap, this);
        }
    } else {
        u8* r28 = (u8*)field_0x70;
        u32 r27 = field_0x74;
        this->~JKRExpHeap();
#if DEBUG
        if (mDebugFill) {
            JKRFillMemory(r28, r27, JKRValue_DEBUGFILL_NOTUSE);
        }
#endif
    }
}

JKRExpHeap::JKRExpHeap(void* data, u32 size, JKRHeap* parent, bool errorFlag)
    : JKRHeap(data, size, parent, errorFlag) {
    mAllocMode = 0;
    mCurrentGroupId = 0xff;
    mHeadFreeList = (CMemBlock*)data;
    mTailFreeList = mHeadFreeList;
    mHeadFreeList->initiate(NULL, NULL, size - sizeof(CMemBlock), 0, 0);
    mHeadUsedList = NULL;
    mTailUsedList = NULL;
}

JKRExpHeap::~JKRExpHeap() {
    dispose();
}

void* JKRExpHeap::do_alloc(u32 size, int alignment) {
#if DEBUG
    if (alignment) {
        u32 u = abs(alignment);
        JUT_CONFIRM(356, u < 0x80);
        JUT_CONFIRM(357, JGadget::binary::isPower2( u ));
    }
#endif

    lock();
    if (size < 4) {
        size = 4;
    }

    void* ptr = NULL;

    if (alignment >= 0) {
        if (alignment <= 4) {
            ptr = allocFromHead(size);
        } else {
            ptr = allocFromHead(size, alignment);
        }
    } else {
        if (-alignment <= 4) {
            ptr = allocFromTail(size);
        } else {
            ptr = allocFromTail(size, -alignment);
        }
    }
#if DEBUG
    if (ptr) {
        CMemBlock* block = (CMemBlock*)((u8*)ptr - sizeof(CMemBlock));
        if (mCheckMemoryFilled) {
            JKRHeap::checkMemoryFilled((u8*)ptr, block->size, JKRValue_DEBUGFILL_DELETE);
        }
        if (mDebugFill) {
            JKRFillMemory((u8*)ptr, block->size, JKRValue_DEBUGFILL_NEW);
        }
        if (block->getAlignment() && mDebugFill) {
            JKRFillMemory((u8*)ptr - sizeof(CMemBlock) - block->getAlignment(), block->getAlignment(), JKRValue_DEBUGFILL_NEW);
        }
    }
#endif

    if (ptr == NULL) {
        JUTWarningConsole_f(":::cannot alloc memory (0x%x byte).\n", size);
        if (getErrorFlag() == true) {
            callErrorHandler(this, size, alignment);
        }
    }
    unlock();

    return ptr;
}

static u32 DBfoundSize;

static u32 DBfoundOffset;

static JKRExpHeap::CMemBlock* DBfoundBlock;

static JKRExpHeap::CMemBlock* DBnewFreeBlock;

static JKRExpHeap::CMemBlock* DBnewUsedBlock;

void* JKRExpHeap::allocFromHead(u32 size, int align) {
    u32 foundOffset;
    int foundSize;

    size = ALIGN_NEXT(size, 4);
    foundSize = -1;
    foundOffset = 0;
    CMemBlock* foundBlock = NULL;
    CMemBlock* newFreeBlock = NULL;
    CMemBlock* newUsedBlock = NULL;

    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        u32 offset =
            ALIGN_PREV(align - 1 + (uintptr_t)block->getContent(), align) - (uintptr_t)block->getContent();
        if (block->size < size + offset) {
            continue;
        }

        if (foundSize <= (u32)block->size) {
            continue;
        }

        foundSize = block->size;
        foundBlock = block;
        foundOffset = offset;
        if (mAllocMode != 0) {
            break;
        }

        if (foundSize == size) {
            break;
        }
    }

    DBfoundSize = foundSize;
    DBfoundOffset = foundOffset;
    DBfoundBlock = foundBlock;

    if (foundBlock) {
        if (foundOffset >= sizeof(CMemBlock)) {
            CMemBlock* prev = foundBlock->mPrev;
            CMemBlock* next = foundBlock->mNext;
            newUsedBlock = foundBlock->allocFore(foundOffset - sizeof(CMemBlock), 0, 0, 0, 0);

            if (newUsedBlock) {
                newFreeBlock = newUsedBlock->allocFore(size, mCurrentGroupId, 0, 0, 0);
            } else {
                newFreeBlock = NULL;
            }

            if (newFreeBlock) {
                setFreeBlock(foundBlock, prev, newFreeBlock);
            } else {
                setFreeBlock(foundBlock, prev, next);
            }

            if (newFreeBlock) {
                setFreeBlock(newFreeBlock, foundBlock, next);
            }

            appendUsedList(newUsedBlock);
            DBnewFreeBlock = newFreeBlock;
            DBnewUsedBlock = newUsedBlock;
            return newUsedBlock->getContent();
        } else {
            if (foundOffset != 0) {
                CMemBlock* prev = foundBlock->mPrev;
                CMemBlock* next = foundBlock->mNext;
                removeFreeBlock(foundBlock);
                newUsedBlock = (CMemBlock*)((uintptr_t)foundBlock + foundOffset);
                newUsedBlock->size = foundBlock->size - foundOffset;
                newFreeBlock =
                    newUsedBlock->allocFore(size, mCurrentGroupId, (u8)foundOffset, 0, 0);
                if (newFreeBlock) {
                    setFreeBlock(newFreeBlock, prev, next);
                }
                appendUsedList(newUsedBlock);
                return newUsedBlock->getContent();
            } else {
                CMemBlock* prev = foundBlock->mPrev;
                CMemBlock* next = foundBlock->mNext;

                newFreeBlock = foundBlock->allocFore(size, mCurrentGroupId, 0, 0, 0);
                removeFreeBlock(foundBlock);
                if (newFreeBlock) {
                    setFreeBlock(newFreeBlock, prev, next);
                }
                appendUsedList(foundBlock);
                return foundBlock->getContent();
            }
        }
    }

    return NULL;
}

void* JKRExpHeap::allocFromHead(u32 size) {
    size = ALIGN_NEXT(size, 4);
    s32 foundSize = -1;
    CMemBlock* foundBlock = NULL;
    CMemBlock* newblock = NULL;
    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        if (block->size < size) {
            continue;
        }

        if (foundSize <= block->size) {
            continue;
        }

        foundSize = block->size;
        foundBlock = block;
        if (mAllocMode != 0) {
            break;
        }

        if (foundSize == size) {
            break;
        }
    }

    if (foundBlock) {
        newblock = foundBlock->allocFore(size, mCurrentGroupId, 0, 0, 0);
        if (newblock) {
            setFreeBlock(newblock, foundBlock->mPrev, foundBlock->mNext);
        } else {
            removeFreeBlock(foundBlock);
        }
        appendUsedList(foundBlock);
        return foundBlock->getContent();
    }
    return NULL;
}

void* JKRExpHeap::allocFromTail(u32 size, int align) {
    u32 local_2c = 0;
    u32 offset = 0;
    CMemBlock* foundBlock = NULL;
    CMemBlock* newBlock = NULL;
    u32 usedSize;
    u32 start;

    for (CMemBlock* block = mTailFreeList; block; block = block->mPrev) {
        start = ALIGN_PREV((uintptr_t)block->getContent() + block->size - size, align);
        usedSize = (uintptr_t)block->getContent() + block->size - start;
        if (block->size >= usedSize) {
            local_2c = usedSize;
            foundBlock = block;
            offset = block->size - usedSize;
            newBlock = (CMemBlock*)start - 1;
            break;
        }
    }

    if (foundBlock != NULL) {
        if (offset >= sizeof(CMemBlock)) {
            newBlock->initiate(NULL, NULL, usedSize, mCurrentGroupId, -0x80);
            foundBlock->size = foundBlock->size - usedSize - sizeof(CMemBlock);
            appendUsedList(newBlock);
            return newBlock->getContent();
        } else {
            if (offset != 0) {
                removeFreeBlock(foundBlock);
                newBlock->initiate(NULL, NULL, usedSize, mCurrentGroupId, offset | 0x80);
                appendUsedList(newBlock);
                return newBlock->getContent();
            } else {
                removeFreeBlock(foundBlock);
                newBlock->initiate(NULL, NULL, usedSize, mCurrentGroupId, -0x80);
                appendUsedList(newBlock);
                return newBlock->getContent();
            }
        }
    }

    return NULL;
}

void* JKRExpHeap::allocFromTail(u32 size) {
    size = ALIGN_NEXT(size, 4);
    CMemBlock* foundBlock = NULL;
    CMemBlock* freeBlock = NULL;
    CMemBlock* usedBlock = NULL;
    for (CMemBlock* block = mTailFreeList; block; block = block->mPrev) {
        if (block->size >= size) {
            foundBlock = block;
            break;
        }
    }

    if (foundBlock != NULL) {
        usedBlock = foundBlock->allocBack(size, 0, 0, mCurrentGroupId, 0);
        if (usedBlock) {
            freeBlock = foundBlock;
        } else {
            removeFreeBlock(foundBlock);
            usedBlock = foundBlock;
            freeBlock = NULL;
        }

        if (freeBlock) {
            setFreeBlock(freeBlock, foundBlock->mPrev, foundBlock->mNext);
        }
        appendUsedList(usedBlock);
        return usedBlock->getContent();
    }
    return NULL;
}

void JKRExpHeap::do_free(void* ptr) {
    lock();
    if (mStart <= ptr && ptr <= mEnd) {
        CMemBlock* block = CMemBlock::getHeapBlock(ptr);
        if (block) {
            block->free(this);
        }
    } else {
        JUT_WARN(921, "free: memblock %x not in heap %x", ptr, this);
    }
    unlock();
}

static void dummy() {
    OS_REPORT("newSize > 0");
}

void JKRExpHeap::do_freeAll() {
    lock();
    JKRHeap::callAllDisposer();
    mHeadFreeList = (CMemBlock*)mStart;
    mTailFreeList = mHeadFreeList;
    mHeadFreeList->initiate(NULL, NULL, mSize - 0x10, 0, 0);
    mHeadUsedList = NULL;
    mTailUsedList = NULL;
#if DEBUG
    if (mDebugFill) {
        JKRFillMemory((u8*)(mHeadFreeList + 1), mHeadFreeList->size, JKRValue_DEBUGFILL_DELETE);
    }
#endif
    unlock();
}

void JKRExpHeap::do_freeTail() {
    lock();
    for (CMemBlock* block = mHeadUsedList; block != NULL;) {
        if (block->isTempMemBlock()) {
            dispose(block->getContent(), block->size);
            CMemBlock* temp = block->mNext;
            block->free(this);
            block = temp;
        } else {
            block = block->mNext;
        }
    }
    unlock();
}

void JKRExpHeap::do_fillFreeArea() {
#if DEBUG
    lock();
    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        JKRFillMemory((u8*)block->getContent(), block->size, JKRValue_DEBUGFILL_DELETE);
    }
    unlock();
#endif
}

s32 JKRExpHeap::do_changeGroupID(u8 param_0) {
    lock();
    u8 prev = mCurrentGroupId;
    mCurrentGroupId = param_0;
    unlock();
    return prev;
}

s32 JKRExpHeap::do_resize(void* ptr, u32 size) {
    lock();
    CMemBlock* block = CMemBlock::getHeapBlock(ptr);
    if (block == NULL || ptr < mStart || mEnd < ptr) {
        unlock();
        return -1;
    }

    size = ALIGN_NEXT(size, 4);
    if (size == block->size) {
        unlock();
        return size;
    }

    if (size > block->size) {
        CMemBlock* foundBlock = NULL;
        for (CMemBlock* freeBlock = mHeadFreeList; freeBlock; freeBlock = freeBlock->mNext) {
            if (freeBlock == (CMemBlock*)((uintptr_t)(block + 1) + block->size)) {
                foundBlock = freeBlock;
                break;
            }
        }

        if (foundBlock == NULL) {
            unlock();
            return -1;
        }

        if (size > block->size + sizeof(CMemBlock) + foundBlock->size) {
            unlock();
            return -1;
        }

        u32 local_24 = block->size;
        removeFreeBlock(foundBlock);
        block->size += foundBlock->size + sizeof(CMemBlock);
        if (block->size - size > sizeof(CMemBlock)) {
            CMemBlock* newBlock = block->allocFore(size, block->mGroupId, block->mFlags, 0, 0);
            if (newBlock) {
                recycleFreeBlock(newBlock);
            }
        }
#if DEBUG
        if (mDebugFill) {
            JKRFillMemory((u8*)ptr + local_24, block->size - local_24, JKRValue_DEBUGFILL_NEW);
        }
#endif
    } else {
        if (block->size - size > sizeof(CMemBlock)) {
            CMemBlock* freeBlock = block->allocFore(size, block->mGroupId, block->mFlags, 0, 0);
            if (freeBlock) {
                recycleFreeBlock(freeBlock);
            }
        }
    }

    unlock();
    return block->size;
}

s32 JKRExpHeap::do_getSize(void* ptr) {
    lock();
    CMemBlock* block = CMemBlock::getHeapBlock(ptr);
    if (!block || ptr < mStart || mEnd < ptr) {
        unlock();
        return -1;
    }
    unlock();
    return block->size;
}

s32 JKRExpHeap::do_getFreeSize() {
    lock();
    s32 size = 0;
    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        if (size < (s32)block->size) {
            size = block->size;
        }
    }
    unlock();
    return size;
}

void* JKRExpHeap::do_getMaxFreeBlock() {
    lock();
    s32 size = 0;
    CMemBlock* res = NULL;
    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        if (size < (s32)block->size) {
            size = block->size;
            res = block;
        }
    }
    unlock();
    return res;
}

s32 JKRExpHeap::do_getTotalFreeSize() {
    u32 size = 0;
    lock();
    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        size += block->size;
    }
    unlock();
    return size;
}

s32 JKRExpHeap::getUsedSize(u8 groupId) const {
    lock();
    u32 size = 0;

    for (CMemBlock* block = mHeadUsedList; block; block = block->mNext) {
        if (block->mGroupId == groupId) {
            size += block->size + sizeof(CMemBlock);
        }
    }

    unlock();
    return size;
}

s32 JKRExpHeap::getTotalUsedSize() const {
    lock();
    u32 size = 0;

    for (CMemBlock* block = mHeadUsedList; block; block = block->mNext) {
        size += block->size + sizeof(CMemBlock);
    }

    unlock();
    return size;
}

BOOL JKRExpHeap::isEmpty() {
    lock();
    BOOL result = !mHeadUsedList ? TRUE : FALSE;
    unlock();
    return result;
}

void JKRExpHeap::appendUsedList(JKRExpHeap::CMemBlock* newblock) {
    if (!newblock) {
        JUTException::panic(__FILE__, 1568, "bad appendUsedList\n");
    }

    CMemBlock* block = mTailUsedList;
    newblock->mMagic = 'HM';

    if (block) {
        block->mNext = newblock;
        newblock->mPrev = block;
    } else {
        newblock->mPrev = NULL;
    }

    mTailUsedList = newblock;
    if (!mHeadUsedList) {
        mHeadUsedList = newblock;
    }
    newblock->mNext = NULL;
}

void JKRExpHeap::setFreeBlock(CMemBlock* block, CMemBlock* prev, CMemBlock* next) {
    if (prev == NULL) {
        mHeadFreeList = block;
        block->mPrev = NULL;
    } else {
        prev->mNext = block;
        block->mPrev = prev;
    }

    if (next == NULL) {
        mTailFreeList = block;
        block->mNext = NULL;
    } else {
        next->mPrev = block;
        block->mNext = next;
    }

    block->mMagic = 0;
}

void JKRExpHeap::removeFreeBlock(CMemBlock* block) {
    CMemBlock* prev = block->mPrev;
    CMemBlock* next = block->mNext;

    if (prev == NULL) {
        mHeadFreeList = next;
    } else {
        prev->mNext = next;
    }

    if (next == NULL) {
        mTailFreeList = prev;
    } else {
        next->mPrev = prev;
    }
}

void JKRExpHeap::removeUsedBlock(JKRExpHeap::CMemBlock* block) {
    CMemBlock* prev = block->mPrev;
    CMemBlock* next = block->mNext;

    if (prev == NULL) {
        mHeadUsedList = next;
    } else {
        prev->mNext = next;
    }

    if (next == NULL) {
        mTailUsedList = prev;
    } else {
        next->mPrev = prev;
    }
}

void JKRExpHeap::recycleFreeBlock(JKRExpHeap::CMemBlock* block) {
    JKRExpHeap::CMemBlock* newBlock = block;
    int size = block->size;
    void* blockEnd = (u8*)newBlock + size;
    block->mMagic = 0;

    if ((block->mFlags & 0x7f) != 0) {
        newBlock = (CMemBlock*)((u8*)newBlock - (block->mFlags & 0x7f));
        size += (block->mFlags & 0x7f);
        blockEnd = (u8*)newBlock + size;
        newBlock->mGroupId = 0;
        newBlock->mFlags = 0;
        newBlock->size = size;
    }
#if DEBUG
    u8* local_24 = (u8*)(newBlock + 1);
    u32 local_28 = newBlock->size;
    if (mDebugFill) {
        JKRFillMemory(local_24, local_28, JKRValue_DEBUGFILL_DELETE);
    }
#endif
    if (!mHeadFreeList) {
        newBlock->initiate(NULL, NULL, size, 0, 0);
        mHeadFreeList = newBlock;
        mTailFreeList = newBlock;
        setFreeBlock(newBlock, NULL, NULL);
        return;
    }

    if (mHeadFreeList >= blockEnd) {
        newBlock->initiate(NULL, NULL, size, 0, 0);
        setFreeBlock(newBlock, NULL, mHeadFreeList);
        joinTwoBlocks(newBlock);
        return;
    }

    if (mTailFreeList <= newBlock) {
        newBlock->initiate(NULL, NULL, size, 0, 0);
        setFreeBlock(newBlock, mTailFreeList, NULL);
        joinTwoBlocks(newBlock->mPrev);
        return;
    }

    for (CMemBlock* freeBlock = mHeadFreeList; freeBlock; freeBlock = freeBlock->mNext) {
        if (freeBlock >= newBlock || newBlock >= freeBlock->mNext) {
            continue;
        }
        newBlock->mNext = freeBlock->mNext;
        newBlock->mPrev = freeBlock;
        freeBlock->mNext = newBlock;
        newBlock->mNext->mPrev = newBlock;
        newBlock->mGroupId = 0;
        joinTwoBlocks(newBlock);
        joinTwoBlocks(freeBlock);
        return;
    }
}

void JKRExpHeap::joinTwoBlocks(CMemBlock* block) {
    u32 endAddr = (uintptr_t)(block + 1) + block->size;
    CMemBlock* next = block->mNext;
    u32 nextAddr = (uintptr_t)next - (next->mFlags & 0x7f);
    if (endAddr > nextAddr) {
        JUTWarningConsole_f(":::Heap may be broken. (block = %x)", block);
        OS_REPORT(":::block = %x\n", block);
        OS_REPORT(":::joinTwoBlocks [%x %x %x][%x %x %x]\n", block, block->mFlags, block->size, block->mNext, block->mNext->mFlags, block->mNext->size);
        OS_REPORT(":::: endAddr = %x\n", endAddr);
        OS_REPORT(":::: nextAddr = %x\n", nextAddr);
        JKRHeap* heap = JKRGetCurrentHeap();
        heap->dump();
        JUTException::panic(__FILE__, 1820, "Bad Block\n");
    }

    if (endAddr == nextAddr) {
        block->size = next->size + sizeof(CMemBlock) + (next->mFlags & 0x7f) + block->size;
        CMemBlock* local_30 = next->mNext;
#if DEBUG
        u8* local_34 = (u8*)next;
        u32 local_38 = sizeof(CMemBlock);
        if (mDebugFill) {
            JKRFillMemory(local_34, local_38, JKRValue_DEBUGFILL_DELETE);
        }
#endif
        setFreeBlock(block, block->mPrev, local_30);
    }
}

bool JKRExpHeap::check() {
    lock();
    int totalBytes = 0;
    bool ok = true;
    for (CMemBlock* block = mHeadUsedList; block; block = block->mNext) {
        if (block->mMagic != 'HM') {
            ok = false;
            JUTWarningConsole_f(":::addr %08x: bad heap signature. (%c%c)\n", block,
                                JSUHiByte(block->mMagic), JSULoByte(block->mMagic));
        }

        if (block->mNext) {
            if (block->mNext->mMagic != 'HM') {
                ok = false;
                JUTWarningConsole_f(":::addr %08x: bad next pointer (%08x)\nabort\n", block,
                                    block->mNext);
                break;
            }
            if (block->mNext->mPrev != block) {
                ok = false;
                JUTWarningConsole_f(":::addr %08x: bad previous pointer (%08x)\n", block->mNext,
                                    block->mNext->mPrev);
            }
        } else {
            if (mTailUsedList != block) {
                ok = false;
                JUTWarningConsole_f(":::addr %08x: bad used list(REV) (%08x)\n", block,
                                    mTailUsedList);
            }
        }
        totalBytes += sizeof(CMemBlock) + block->size + block->getAlignment();
    }

    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        totalBytes += block->size + sizeof(CMemBlock);
        if (block->mNext) {
            if (block->mNext->mPrev != block) {
                ok = false;
                JUTWarningConsole_f(":::addr %08x: bad previous pointer (%08x)\n", block->mNext,
                                    block->mNext->mPrev);
            }

            if ((uintptr_t)block + block->size + sizeof(CMemBlock) > (uintptr_t)block->mNext) {
                ok = false;
                JUTWarningConsole_f(":::addr %08x: bad block size (%08x)\n", block, block->size);
            }
        } else {
            if (mTailFreeList != block) {
                ok = false;
                JUTWarningConsole_f(":::addr %08x: bad used list(REV) (%08x)\n", block,
                                    mTailFreeList);
            }
        }
#if DEBUG
        if (mCheckMemoryFilled) {
            u8* local_34 = (u8*)block->getContent();
            u32 local_38 = block->size;
            ok = JKRHeap::checkMemoryFilled(local_34, local_38, JKRValue_DEBUGFILL_DELETE);
        }
#endif
    }

    if (totalBytes != mSize) {
        ok = false;
        JUTWarningConsole_f(":::bad total memory block size (%08X, %08X)\n", mSize, totalBytes);
    }

    if (!ok) {
        JUTWarningConsole(":::there is some error in this heap!\n");
    }

    unlock();
    return ok;
}

bool JKRExpHeap::dump() {
    lock();
    bool result = check();
    u32 usedBytes = 0;
    u32 usedCount = 0;
    u32 freeCount = 0;

    JUTReportConsole(" attr  address:   size    gid aln   prev_ptr next_ptr\n");
    JUTReportConsole("(Used Blocks)\n");
    if (!mHeadUsedList) {
        JUTReportConsole(" NONE\n");
    }

    for (CMemBlock* block = mHeadUsedList; block; block = block->mNext) {
        if (block->mMagic != 'HM') {
            JUTReportConsole_f("xxxxx %08x: --------  --- ---  (-------- --------)\nabort\n",
                               block);
            break;
        }

        JUTReportConsole_f("%s %08x: %08x  %3d %3d  (%08x %08x)\n",
                           block->isTempMemBlock() ? " temp" : "alloc", block->getContent(),
                           block->size, block->mGroupId, block->getAlignment(), block->mPrev,
                           block->mNext);
        usedBytes += sizeof(CMemBlock) + block->size + block->getAlignment();
        usedCount++;
    }

    JUTReportConsole("(Free Blocks)\n");
    if (!mHeadFreeList) {
        JUTReportConsole(" NONE\n");
    }

    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        JUTReportConsole_f("%s %08x: %08x  %3d %3d  (%08x %08x)\n", " free", block->getContent(),
                           block->size, block->mGroupId, block->getAlignment(), block->mPrev,
                           block->mNext);
        freeCount++;
    }

    JUTReportConsole_f("%d / %d bytes (%6.2f%%) used (U:%d F:%d)\n", usedBytes, mSize, (f32(usedBytes) / f32(mSize)) * 100.0f,
                       usedCount, freeCount);
    unlock();
    return result;
}

bool JKRExpHeap::dump_sort() {
    lock();
    bool result = check();
    u32 usedBytes = 0;
    u32 usedCount = 0;
    u32 freeCount = 0;
    JUTReportConsole(" attr  address:   size    gid aln   prev_ptr next_ptr\n");
    JUTReportConsole("(Used Blocks)\n");

    if (mHeadUsedList == NULL) {
        JUTReportConsole(" NONE\n");
    } else {
        CMemBlock* var1 = NULL;
        while (true) {
            CMemBlock* block = (CMemBlock*)0xffffffff;
            for (CMemBlock* iterBlock = mHeadUsedList; iterBlock; iterBlock = iterBlock->mNext) {
                if (var1 < iterBlock && iterBlock < block) {
                    block = iterBlock;
                }
            }

            if (uintptr_t(block) == 0xffffffff) {
                break;
            }

            if (block->mMagic != 'HM') {
                JUTReportConsole_f("xxxxx %08x: --------  --- ---  (-------- --------)\nabort\n",
                                   var1);
                break;
            }

            JUTReportConsole_f("%s %08x: %08x  %3d %3d  (%08x %08x)\n", block->isTempMemBlock() ? " temp" : "alloc", block->getContent(), block->size,
                               block->mGroupId, block->getAlignment(), block->mPrev, block->mNext);
            usedBytes += sizeof(CMemBlock) + block->size + block->getAlignment();
            usedCount++;
            var1 = block;
        }
    }

    JUTReportConsole("(Free Blocks)\n");
    if (mHeadFreeList == NULL) {
        JUTReportConsole(" NONE\n");
    }

    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        JUTReportConsole_f("%s %08x: %08x  %3d %3d  (%08x %08x)\n", " free", block->getContent(),
                           block->size, block->mGroupId, block->getAlignment(), block->mPrev,
                           block->mNext);
        freeCount++;
    }

    JUTReportConsole_f("%d / %d bytes (%6.2f%%) used (U:%d F:%d)\n", usedBytes, mSize, (f32(usedBytes) / f32(mSize)) * 100.0f,
                       usedCount, freeCount);
    unlock();
    return result;
}

void JKRExpHeap::CMemBlock::initiate(JKRExpHeap::CMemBlock* prev, JKRExpHeap::CMemBlock* next,
                                     u32 size, u8 groupId, u8 alignment) {
    mMagic = 'HM';
    mFlags = alignment;
    mGroupId = groupId;
    this->size = size;
    mPrev = prev;
    mNext = next;
}

JKRExpHeap::CMemBlock* JKRExpHeap::CMemBlock::allocFore(u32 size1, u8 groupId1, u8 alignment1,
                                                        u8 groupId2, u8 alignment2) {
    CMemBlock* block = NULL;
    mGroupId = groupId1;
    mFlags = alignment1;
    if (size >= size1 + sizeof(CMemBlock)) {
        block = (CMemBlock*)(size1 + (uintptr_t)this);
        block[1].mGroupId = groupId2;
        block[1].mFlags = alignment2;
        block[1].size = size - (size1 + sizeof(CMemBlock));
        size = size1;
        block = block + 1;
    }
    return block;
}

JKRExpHeap::CMemBlock* JKRExpHeap::CMemBlock::allocBack(u32 size1, u8 groupId1, u8 alignment1,
                                                        u8 groupId2, u8 alignment2) {
    CMemBlock* newblock = NULL;
    if (size >= size1 + sizeof(CMemBlock)) {
        newblock = (CMemBlock*)((uintptr_t)this + size - size1);
        newblock->mGroupId = groupId2;
        newblock->mFlags = alignment2 | 0x80;
        newblock->size = size1;
        mGroupId = groupId1;
        mFlags = alignment1;
        this->size -= size1 + sizeof(CMemBlock);
    } else {
        mGroupId = groupId2;
        mFlags = 0x80;
    }
    return newblock;
}

int JKRExpHeap::CMemBlock::free(JKRExpHeap* heap) {
    heap->removeUsedBlock(this);
    heap->recycleFreeBlock(this);
    return 0;
}

JKRExpHeap::CMemBlock* JKRExpHeap::CMemBlock::getHeapBlock(void* ptr) {
    if (ptr) {
        CMemBlock* block = (CMemBlock*)ptr - 1;
        if (block->isValid()) {
            return block;
        }
    }
    return NULL;
}

static void dummy2() {
    DEAD_STRING("+---------------JKRExpHeap\n");
    DEAD_STRING("|         Align Group  size    ( prev , next )\n");
    DEAD_STRING("| ---- FreeFirst\n");
    DEAD_STRING("| %08x  ");
    DEAD_STRING("%2x  %3d  %6x  (%08x %08x)\n");
    DEAD_STRING("| ---- FreeLast\n");
    DEAD_STRING("| ---- UsedFirst\n");
    DEAD_STRING("| ---- UsedLast\n");
    DEAD_STRING("+---------------End\n");
}

void JKRExpHeap::state_register(JKRHeap::TState* p, u32 param_1) const {
    JUT_ASSERT(2433, p != NULL);
    JUT_ASSERT(2434, p->getHeap() == this);
    void* r24 = getState_(p);
    u32 r25 = param_1;
    setState_u32ID_(p, param_1);
    if (param_1 <= 0xff) {
        setState_uUsedSize_(p, getUsedSize(r25));
    } else {
        setState_uUsedSize_(p, getUsedSize_(const_cast<JKRExpHeap*>(this)));
    }

    u32 checkCode = 0;
    for (CMemBlock* block = mHeadUsedList; block; block = block->mNext) {
        if (param_1 <= 0xff) {
            if (block->mGroupId == param_1) {
                checkCode += (uintptr_t)block * 3;
            }
        } else {
            checkCode += (uintptr_t)block * 3;
        }
    }
    setState_u32CheckCode_(p, checkCode);
}

bool JKRExpHeap::state_compare(JKRHeap::TState const& r1, JKRHeap::TState const& r2) const {
    JUT_ASSERT(2481, r1.getHeap() == r2.getHeap());
    bool result = true;
    if (r1.getCheckCode() != r2.getCheckCode()) {
        result = false;
    }

    if (r1.getUsedSize() != r2.getUsedSize()) {
        result = false;
    }

    return result;
}

u32 JKRExpHeap::getHeapType() {
    return 'EXPH';
}

u8 JKRExpHeap::do_getCurrentGroupId() {
    return mCurrentGroupId;
}
