//
// JKRExpHeap
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JSupport/JSupport.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JUtility/JUTException.h"

JKRExpHeap* JKRExpHeap::createRoot(int maxHeaps, bool errorFlag) {
    JKRExpHeap* heap = NULL;
    if (!sRootHeap) {
        void* memory;
        u32 memorySize;
        initArena((char**)&memory, &memorySize, maxHeaps);
        u8* start = (u8*)memory + ALIGN_NEXT(sizeof(JKRExpHeap), 0x10);
        u32 alignedSize = memorySize - ALIGN_NEXT(sizeof(JKRExpHeap), 0x10);
        heap = new (memory) JKRExpHeap(start, alignedSize, NULL, errorFlag);
        sRootHeap = heap;
    }
    heap->field_0x6e = true;
    return heap;
}

JKRExpHeap* JKRExpHeap::create(u32 size, JKRHeap* parent, bool errorFlag) {
    if (!parent) {
        parent = sRootHeap;
    }

    if (size == 0xffffffff) {
        size = parent->getMaxAllocatableSize(0x10);
    }

    u32 alignedSize = ALIGN_PREV(size, 0x10);
    u32 expHeapSize = ALIGN_NEXT(sizeof(JKRExpHeap), 0x10);
    if (alignedSize < 0xa0)
        return NULL;

    u8* memory = (u8*)JKRAllocFromHeap(parent, alignedSize, 0x10);
    u8* dataPtr = (memory + expHeapSize);
    if (!memory) {
        return NULL;
    }

    JKRExpHeap* newHeap =
        new (memory) JKRExpHeap(dataPtr, alignedSize - expHeapSize, parent, errorFlag);

    if (newHeap == NULL) {
        JKRFree(memory);
        return NULL;
    }

    newHeap->field_0x6e = false;
    return newHeap;
}

JKRExpHeap* JKRExpHeap::create(void* ptr, u32 size, JKRHeap* parent, bool errorFlag) {
    JKRHeap* parent2;
    if (parent == NULL) {
        parent2 = sRootHeap->find(ptr);
        if (!parent2)
            return NULL;
    } else {
        parent2 = parent;
    }

    JKRExpHeap* newHeap = NULL;
    u32 expHeapSize = ALIGN_NEXT(sizeof(JKRExpHeap), 0x10);
    if (size < expHeapSize)
        return NULL;

    void* dataPtr = (u8*)ptr + expHeapSize;
    u32 alignedSize = ALIGN_PREV((uintptr_t)ptr + size - (uintptr_t)dataPtr, 0x10);
    if (ptr) {
        newHeap = new (ptr) JKRExpHeap(dataPtr, alignedSize, parent2, errorFlag);
    }

    newHeap->field_0x6e = true;
    newHeap->field_0x70 = ptr;
    newHeap->field_0x74 = size;
    return newHeap;
}

void JKRExpHeap::do_destroy() {
    if (!field_0x6e) {
        JKRHeap* heap = mChildTree.getParent()->getObject();
        if (heap) {
            this->~JKRExpHeap();
            JKRHeap::free(this, heap);
        }
    } else {
        this->~JKRExpHeap();
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
    void* ptr;

    lock();
    if (size < 4) {
        size = 4;
    }

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

    if (ptr == NULL) {
        JUTWarningConsole_f(":::cannot alloc memory (0x%x byte).\n", size);
        if (mErrorFlag == true) {
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
    CMemBlock* newFreeBlock;
    CMemBlock* newUsedBlock;
    CMemBlock* foundBlock;

    size = ALIGN_NEXT(size, 4);
    foundSize = -1;
    foundOffset = 0;
    foundBlock = NULL;

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

        u32 blockSize = block->size;
        if (blockSize == size) {
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
                // Works but very fake match
                size = (uintptr_t)foundBlock->allocFore(size, mCurrentGroupId, 0, 0, 0);
                removeFreeBlock(foundBlock);
                if (size) {
                    setFreeBlock((CMemBlock*)size, prev, next);
                }
                // newFreeBlock = foundBlock->allocFore(size, mCurrentGroupId, 0, 0, 0);
                // removeFreeBlock(foundBlock);
                // if (newFreeBlock) {
                //     setFreeBlock(newFreeBlock, prev, next);
                // }
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
    for (CMemBlock* block = mHeadFreeList; block; block = block->getNextBlock()) {
        if (block->getSize() < size) {
            continue;
        }

        if (foundSize <= block->getSize()) {
            continue;
        }

        foundSize = block->getSize();
        foundBlock = block;
        if (mAllocMode != 0) {
            break;
        }

        if (foundSize == size) {
            break;
        }
    }

    if (foundBlock) {
        CMemBlock* newblock = foundBlock->allocFore(size, mCurrentGroupId, 0, 0, 0);
        if (newblock) {
            setFreeBlock(newblock, foundBlock->getPrevBlock(), foundBlock->getNextBlock());
        } else {
            removeFreeBlock(foundBlock);
        }
        appendUsedList(foundBlock);
        return foundBlock->getContent();
    }
    return NULL;
}

void* JKRExpHeap::allocFromTail(u32 size, int align) {
    u32 offset = 0;
    CMemBlock* foundBlock = NULL;
    CMemBlock* newBlock = NULL;
    u32 usedSize;
    u32 start;

    for (CMemBlock* block = mTailFreeList; block; block = block->mPrev) {
        start = ALIGN_PREV((uintptr_t)block->getContent() + block->size - size, align);
        usedSize = (uintptr_t)block->getContent() + block->size - start;
        if (block->size >= usedSize) {
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
    u32 size2 = ALIGN_NEXT(size, 4);
    CMemBlock* foundBlock = NULL;
    for (CMemBlock* block = mTailFreeList; block; block = block->getPrevBlock()) {
        if (block->getSize() >= size2) {
            foundBlock = block;
            break;
        }
    }

    if (foundBlock != NULL) {
        CMemBlock* usedBlock = foundBlock->allocBack(size2, 0, 0, mCurrentGroupId, 0);
        CMemBlock* freeBlock;
        if (usedBlock) {
            freeBlock = foundBlock;
        } else {
            removeFreeBlock(foundBlock);
            usedBlock = foundBlock;
            freeBlock = NULL;
        }

        if (freeBlock) {
            setFreeBlock(freeBlock, foundBlock->getPrevBlock(), foundBlock->getNextBlock());
        }
        appendUsedList(usedBlock);
        return usedBlock->getContent();
    }
    return NULL;
}

void JKRExpHeap::do_free(void* ptr) {
    lock();
    if (getStartAddr() <= ptr && ptr <= getEndAddr()) {
        CMemBlock* block = CMemBlock::getHeapBlock(ptr);
        if (block) {
            block->free(this);
        }
    }
    unlock();
}

void JKRExpHeap::do_freeAll() {
    lock();
    JKRHeap::callAllDisposer();
    mHeadFreeList = (CMemBlock*)getStartAddr();
    mTailFreeList = mHeadFreeList;
    mHeadFreeList->initiate(NULL, NULL, getSize() - 0x10, 0, 0);
    mHeadUsedList = NULL;
    mTailUsedList = NULL;
    unlock();
}

void JKRExpHeap::do_freeTail() {
    lock();
    for (CMemBlock* block = mHeadUsedList; block != NULL;) {
        if ((block->mFlags & 0x80) != 0) {
            dispose(block + 1, block->size);
            CMemBlock* temp = block->mNext;
            block->free(this);
            block = temp;
        } else {
            block = block->mNext;
        }
    }
    unlock();
}

void JKRExpHeap::do_fillFreeArea() {}

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

        removeFreeBlock(foundBlock);
        block->size += foundBlock->size + sizeof(CMemBlock);
        if (block->size - size > sizeof(CMemBlock)) {
            CMemBlock* newBlock = block->allocFore(size, block->mGroupId, block->mFlags, 0, 0);
            if (newBlock) {
                recycleFreeBlock(newBlock);
            }
        }
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
    if (!block || ptr < getStartAddr() || getEndAddr() < ptr) {
        unlock();
        return -1;
    }
    unlock();
    return block->getSize();
}

s32 JKRExpHeap::do_getFreeSize() {
    lock();
    s32 size = 0;
    for (CMemBlock* block = mHeadFreeList; block; block = block->getNextBlock()) {
        if (size < (s32)block->getSize()) {
            size = block->getSize();
        }
    }
    unlock();
    return size;
}

void* JKRExpHeap::do_getMaxFreeBlock() {
    lock();
    s32 size = 0;
    CMemBlock* res = NULL;
    for (CMemBlock* block = mHeadFreeList; block; block = block->getNextBlock()) {
        if (size < (s32)block->getSize()) {
            size = block->getSize();
            res = block;
        }
    }
    unlock();
    return res;
}

s32 JKRExpHeap::do_getTotalFreeSize() {
    u32 size = 0;
    lock();
    for (CMemBlock* block = mHeadFreeList; block; block = block->getNextBlock()) {
        size += block->getSize();
    }
    unlock();
    return size;
}

s32 JKRExpHeap::getUsedSize(u8 groupId) const {
    JKRExpHeap* this2 = const_cast<JKRExpHeap*>(this);
    this2->lock();
    u32 size = 0;

    for (CMemBlock* block = mHeadUsedList; block; block = block->getNextBlock()) {
        u8 blockGroupId = block->getGroupId();
        if (blockGroupId == groupId) {
            size += block->getSize() + sizeof(CMemBlock);
        }
    }

    this2->unlock();
    return size;
}

s32 JKRExpHeap::getTotalUsedSize() const {
    JKRExpHeap* this2 = const_cast<JKRExpHeap*>(this);
    this2->lock();
    u32 size = 0;

    for (CMemBlock* block = mHeadUsedList; block; block = block->getNextBlock()) {
        size += block->getSize() + sizeof(CMemBlock);
    }

    this2->unlock();
    return size;
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

/* 802CFE68-802CFEB4 2CA7A8 004C+00 5/5 0/0 0/0 .text
 * setFreeBlock__10JKRExpHeapFPQ210JKRExpHeap9CMemBlockPQ210JKRExpHeap9CMemBlockPQ210JKRExpHeap9CMemBlock
 */
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
    void* blockEnd = (u8*)block + size;
    block->mMagic = 0;

    if ((block->mFlags & 0x7f) != 0) {
        newBlock = (CMemBlock*)((u8*)block - (block->mFlags & 0x7f));
        size += (block->mFlags & 0x7f);
        blockEnd = (u8*)newBlock + size;
        newBlock->mGroupId = 0;
        newBlock->mFlags = 0;
        newBlock->size = size;
    }

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
        JKRGetCurrentHeap()->dump();
        JUTException::panic(__FILE__, 1820, "Bad Block\n");
    }

    if (endAddr == nextAddr) {
        block->size = next->size + sizeof(CMemBlock) + next->getAlignment() + block->size;
        setFreeBlock(block, block->mPrev, next->mNext);
    }
}

bool JKRExpHeap::check() {
    lock();
    int totalBytes = 0;
    bool ok = true;
    for (CMemBlock* block = mHeadUsedList; block; block = block->mNext) {
        if (!block->isValid()) {
            ok = false;
            JUTWarningConsole_f(":::addr %08x: bad heap signature. (%c%c)\n", block,
                                JSUHiByte(block->mMagic), JSULoByte(block->mMagic));
        }

        if (block->mNext) {
            if (!block->mNext->isValid()) {
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
        if (!block->isValid()) {
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

    float percent = ((float)usedBytes / (float)mSize) * 100.0f;
    JUTReportConsole_f("%d / %d bytes (%6.2f%%) used (U:%d F:%d)\n", usedBytes, mSize, percent,
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

            if (block == (CMemBlock*)0xffffffff) {
                break;
            }

            if (!block->isValid()) {
                JUTReportConsole_f("xxxxx %08x: --------  --- ---  (-------- --------)\nabort\n",
                                   var1);
                break;
            }

            int offset = block->getAlignment();
            void* content = block->getContent();
            const char* type = block->isTempMemBlock() ? " temp" : "alloc";
            JUTReportConsole_f("%s %08x: %08x  %3d %3d  (%08x %08x)\n", type, content, block->size,
                               block->getGroupId(), offset, block->mPrev, block->mNext);
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
                           block->size, block->getGroupId(), block->getAlignment(), block->mPrev,
                           block->mNext);
        freeCount++;
    }

    float percent = ((float)usedBytes / (float)mSize) * 100.0f;
    JUTReportConsole_f("%d / %d bytes (%6.2f%%) used (U:%d F:%d)\n", usedBytes, mSize, percent,
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

JKRExpHeap::CMemBlock* JKRExpHeap::CMemBlock::allocFore(u32 size, u8 groupId1, u8 alignment1,
                                                        u8 groupId2, u8 alignment2) {
    CMemBlock* block = NULL;
    mGroupId = groupId1;
    mFlags = alignment1;
    if (getSize() >= size + sizeof(CMemBlock)) {
        block = (CMemBlock*)(size + (uintptr_t)this);
        block[1].mGroupId = groupId2;
        block[1].mFlags = alignment2;
        block[1].size = this->size - (size + sizeof(CMemBlock));
        this->size = size;
        block = block + 1;
    }
    return block;
}

JKRExpHeap::CMemBlock* JKRExpHeap::CMemBlock::allocBack(u32 size, u8 groupId1, u8 alignment1,
                                                        u8 groupId2, u8 alignment2) {
    CMemBlock* newblock = NULL;
    if (getSize() >= size + sizeof(CMemBlock)) {
        newblock = (CMemBlock*)((uintptr_t)this + getSize() - size);
        newblock->mGroupId = groupId2;
        newblock->mFlags = alignment2 | 0x80;
        newblock->size = size;
        mGroupId = groupId1;
        mFlags = alignment1;
        this->size -= size + sizeof(CMemBlock);
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

void JKRExpHeap::state_register(JKRHeap::TState* p, u32 param_1) const {
    p->mId = param_1;
    if (param_1 <= 0xff) {
        p->mUsedSize = getUsedSize(param_1);
    } else {
        s32 freeSize = const_cast<JKRExpHeap*>(this)->getTotalFreeSize();
        p->mUsedSize = getSize() - freeSize;
    }

    u32 checkCode = 0;
    for (CMemBlock* block = mHeadUsedList; block; block = block->getNextBlock()) {
        if (param_1 <= 0xff) {
            u8 groupId = block->getGroupId();
            if (groupId == param_1) {
                checkCode += (uintptr_t)block * 3;
            }
        } else {
            checkCode += (uintptr_t)block * 3;
        }
    }
    p->mCheckCode = checkCode;
}

bool JKRExpHeap::state_compare(JKRHeap::TState const& r1, JKRHeap::TState const& r2) const {
    bool result = true;
    if (r1.mCheckCode != r2.mCheckCode) {
        result = false;
    }

    if (r1.mUsedSize != r2.mUsedSize) {
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

static char* dummyLiteral1() { return "+---------------JKRExpHeap\n"; }
static char* dummyLiteral2() { return "|         Align Group  size    ( prev , next )\n"; }
static char* dummyLiteral3() { return "| ---- FreeFirst\n"; }
static char* dummyLiteral4() { return "| %08x  "; }
static char* dummyLiteral5() { return "%2x  %3d  %6x  (%08x %08x)\n"; }
static char* dummyLiteral6() { return "| ---- FreeLast\n"; }
static char* dummyLiteral7() { return "| ---- UsedFirst\n"; }
static char* dummyLiteral8() { return "| ---- UsedLast\n"; }
static char* dummyLiteral9() { return "+---------------End\n"; }
