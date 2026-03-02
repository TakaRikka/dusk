#include <revolution/mem/expHeap.h>
#include <revolution/mem/heapCommon.h>

#define FillFreeMemory(pHeapHd, address, size) ((void)0)
#define FillNoUseMemory(pHeapHd, address, size) ((void)0)

#define RoundUp(value, alignment) (((value) + ((alignment) - 1)) & ~((alignment) - 1))
#define RoundUpPtr(ptr, alignment) ((void*)RoundUp(GetUIntPtr(ptr), (alignment)))
#define RoundDown(value, alignment) ((value) & ~((alignment) - 1))
#define RoundDownPtr(ptr, alignment) ((void*)RoundDown(GetUIntPtr(ptr), (alignment)))

typedef struct MemRegion MemRegion;

struct MemRegion {
    void* start;
    void* end;
};

static inline void* GetMemPtrForMBlock_(MEMiExpHeapMBlockHead* pMBlkHd) {
    return AddU32ToPtr(pMBlkHd, sizeof(MEMiExpHeapMBlockHead));
}

static inline void* GetMBlockEndAddr_(MEMiExpHeapMBlockHead* pMBHead) {
    return AddU32ToPtr(GetMemPtrForMBlock_(pMBHead), pMBHead->blockSize);
}

static inline u16 GetAllocMode_(MEMiExpHeapHead* pEHHead) {
    return pEHHead->feature.fields.allocMode;
}

static MEMiExpHeapMBlockHead* InitMBlock_(MemRegion* pRegion, u16 signature) {
    MEMiExpHeapMBlockHead* block = (MEMiExpHeapMBlockHead*)pRegion->start;

    block->signature = signature;
    block->attribute.val = 0;
    block->blockSize = GetOffsetFromPtr(GetMemPtrForMBlock_(block), pRegion->end);

    block->prev = NULL;
    block->next = NULL;

    return block;
}

static inline void SetAllocDirForMBlock_(MEMiExpHeapMBlockHead* pMBHead, u16 mode) {
    pMBHead->attribute.fields.allocDir = mode;
}

static inline u16 GetAlignmentForMBlock_(const MEMiExpHeapMBlockHead* pMBlkHd) {
    return pMBlkHd->attribute.fields.alignment;
}

static inline void SetAlignmentForMBlock_(MEMiExpHeapMBlockHead* pMBlkHd, u16 alignment) {
    pMBlkHd->attribute.fields.alignment = alignment;
}

static inline void SetGroupIDForMBlock_(MEMiExpHeapMBlockHead* pMBHead, u16 id) {
    pMBHead->attribute.fields.groupID = (u8)id;
}

static void GetRegionOfMBlock_(MemRegion* region, MEMiExpHeapMBlockHead* block) {
    region->start = SubU32ToPtr(block, GetAlignmentForMBlock_(block));
    region->end = GetMBlockEndAddr_(block);
}

static inline MEMiHeapHead* GetHeapHeadPtrFromExpHeapHead_(MEMiExpHeapHead* pEHHead) {
    return (MEMiHeapHead*)SubU32ToPtr(pEHHead, sizeof(MEMiHeapHead));
}

static MEMiExpHeapMBlockHead* InsertMBlock_(MEMiExpMBlockList* list, MEMiExpHeapMBlockHead* target, MEMiExpHeapMBlockHead* prev) {
    MEMiExpHeapMBlockHead* next;

    target->prev = prev;
    if (prev) {
        next = prev->next;
        prev->next = target;
    } else {
        next = list->head;
        list->head = target;
    }

    target->next = next;
    if (next) {
        next->prev = target;
    } else {
        list->tail = target;
    }

    return target;
}

static inline void AppendMBlock_(MEMiExpMBlockList* list, MEMiExpHeapMBlockHead* block) {
    (void)InsertMBlock_(list, block, list->tail);
}

static inline MEMiExpHeapHead* GetExpHeapHeadPtrFromHeapHead_(MEMiHeapHead* pHHead) {
    return (MEMiExpHeapHead*)AddU32ToPtr(pHHead, sizeof(MEMiHeapHead));
}

static MEMiExpHeapMBlockHead* RemoveMBlock_(MEMiExpMBlockList* list, MEMiExpHeapMBlockHead* block) {
    MEMiExpHeapMBlockHead* const prev = block->prev;
    MEMiExpHeapMBlockHead* const next = block->next;

    if (prev) {
        prev->next = next;
    } else {
        list->head = next;
    }

    if (next) {
        next->prev = prev;
    } else {
        list->tail = prev;
    }

    return prev;
}

static inline MEMiExpHeapMBlockHead* InitFreeMBlock_(MemRegion* pRegion) {
    return InitMBlock_(pRegion, 'FR');
}

static inline void SetAllocMode_(MEMiExpHeapHead* pEHHead, u16 mode) {
    pEHHead->feature.fields.allocMode = mode;
}

static MEMiHeapHead* InitExpHeap_(void* startAddress, void* endAddress, u16 optFlag) {
    MEMiHeapHead* pHeapHd = (MEMiHeapHead*)startAddress;
    MEMiExpHeapHead* pExpHeapHd = GetExpHeapHeadPtrFromHeapHead_(pHeapHd);

    MEMiInitHeapHead(pHeapHd, 'EXPH', AddU32ToPtr(pExpHeapHd, sizeof(MEMiExpHeapHead)), endAddress, optFlag);

    pExpHeapHd->groupID = 0;
    pExpHeapHd->feature.val = 0;
    SetAllocMode_(pExpHeapHd, 0);

    {
        MEMiExpHeapMBlockHead* pMBHead;
        MemRegion region;

        region.start = pHeapHd->heapStart;
        region.end = pHeapHd->heapEnd;
        pMBHead = InitFreeMBlock_(&region);
        pExpHeapHd->mbFreeList.head = pMBHead;
        pExpHeapHd->mbFreeList.tail = pMBHead;
        pExpHeapHd->mbUsedList.head = NULL;
        pExpHeapHd->mbUsedList.tail = NULL;

        return pHeapHd;
    }
}

static void* AllocUsedBlockFromFreeBlock_(MEMiExpHeapHead* pEHHead, MEMiExpHeapMBlockHead* pMBHeadFree, void* mblock, u32 size, u16 direction) {
    MemRegion freeRgnT;
    MemRegion freeRgnB;
    MEMiExpHeapMBlockHead* pMBHeadFreePrev;

    GetRegionOfMBlock_(&freeRgnT, pMBHeadFree);
    freeRgnB.end = freeRgnT.end;
    freeRgnB.start = AddU32ToPtr(mblock, size);
    freeRgnT.end = SubU32ToPtr(mblock, sizeof(MEMiExpHeapMBlockHead));

    pMBHeadFreePrev = RemoveMBlock_(&pEHHead->mbFreeList, pMBHeadFree);

    if ((GetOffsetFromPtr(freeRgnT.start, freeRgnT.end) < sizeof(MEMiExpHeapMBlockHead) + 4)
#if SDK_AUG2010
        || (direction == 0 && !pEHHead->feature.fields.useMarginOfAlign)
#endif
    ) {
        freeRgnT.end = freeRgnT.start;
    } else {
        pMBHeadFreePrev = InsertMBlock_(&pEHHead->mbFreeList, InitFreeMBlock_(&freeRgnT), pMBHeadFreePrev);
    }

    if ((GetOffsetFromPtr(freeRgnB.start, freeRgnB.end) < sizeof(MEMiExpHeapMBlockHead) + 4)
#if SDK_AUG2010
        || (direction == 1 && !pEHHead->feature.fields.useMarginOfAlign)
#endif
    ) {
        freeRgnB.start = freeRgnB.end;
    } else {
        (void)InsertMBlock_(&pEHHead->mbFreeList, InitFreeMBlock_(&freeRgnB), pMBHeadFreePrev);
    }

    FillAllocMemory(GetHeapHeadPtrFromExpHeapHead_(pEHHead), freeRgnT.end, GetOffsetFromPtr(freeRgnT.end, freeRgnB.start));

    {
        MEMiExpHeapMBlockHead* pMBHeadNewUsed;
        MemRegion region;

        region.start = SubU32ToPtr(mblock, sizeof(MEMiExpHeapMBlockHead));
        region.end = freeRgnB.start;

        pMBHeadNewUsed = InitMBlock_(&region, 'UD');
        SetAllocDirForMBlock_(pMBHeadNewUsed, direction);
        SetAlignmentForMBlock_(pMBHeadNewUsed, (u16)GetOffsetFromPtr(freeRgnT.end, pMBHeadNewUsed));
        SetGroupIDForMBlock_(pMBHeadNewUsed, pEHHead->groupID);
        AppendMBlock_(&pEHHead->mbUsedList, pMBHeadNewUsed);
    }

    return mblock;
}

static void* AllocFromHead_(MEMiHeapHead* pHeapHd, u32 size, int alignment) {
    MEMiExpHeapHead* pExpHeapHd = GetExpHeapHeadPtrFromHeapHead_(pHeapHd);

    const BOOL bAllocFirst = GetAllocMode_(pExpHeapHd) == 0;

    MEMiExpHeapMBlockHead* pMBlkHd = NULL;
    MEMiExpHeapMBlockHead* pMBlkHdFound = NULL;
    u32 foundSize = 0xffffffff;
    void* foundMBlock = NULL;

    for (pMBlkHd = pExpHeapHd->mbFreeList.head; pMBlkHd; pMBlkHd = pMBlkHd->next) {
        void* const mblock = GetMemPtrForMBlock_(pMBlkHd);
        void* const reqMBlock = RoundUpPtr(mblock, alignment);
        const u32 offset = GetOffsetFromPtr(mblock, reqMBlock);

        if (pMBlkHd->blockSize >= size + offset && foundSize > pMBlkHd->blockSize) {
            pMBlkHdFound = pMBlkHd;
            foundSize = pMBlkHd->blockSize;
            foundMBlock = reqMBlock;

            if (bAllocFirst || foundSize == size) {
                break;
            }
        }
    }

    if (!pMBlkHdFound) {
        return NULL;
    }

    return AllocUsedBlockFromFreeBlock_(pExpHeapHd, pMBlkHdFound, foundMBlock, size, 0);
}

static void* AllocFromTail_(MEMiHeapHead* pHeapHd, u32 size, int alignment) {
    MEMiExpHeapHead* pExpHeapHd = GetExpHeapHeadPtrFromHeapHead_(pHeapHd);
    const BOOL bAllocFirst = GetAllocMode_(pExpHeapHd) == 0;
    MEMiExpHeapMBlockHead* pMBlkHd = NULL;
    MEMiExpHeapMBlockHead* pMBlkHdFound = NULL;
    u32 foundSize = 0xffffffff;
    void* foundMBlock = NULL;

    for (pMBlkHd = pExpHeapHd->mbFreeList.tail; pMBlkHd; pMBlkHd = pMBlkHd->prev) {
        void* const mblock = GetMemPtrForMBlock_(pMBlkHd);
        void* const mblockEnd = AddU32ToPtr(mblock, pMBlkHd->blockSize);
        void* const reqMBlock = RoundDownPtr(SubU32ToPtr(mblockEnd, size), alignment);

        if (ComparePtr(reqMBlock, mblock) >= 0 && foundSize > pMBlkHd->blockSize) {
            pMBlkHdFound = pMBlkHd;
            foundSize = pMBlkHd->blockSize;
            foundMBlock = reqMBlock;

            if (bAllocFirst || foundSize == size) {
                break;
            }
        }
    }

    if (!pMBlkHdFound) {
        return NULL;
    }

    return AllocUsedBlockFromFreeBlock_(pExpHeapHd, pMBlkHdFound, foundMBlock, size, 1);
}

static BOOL RecycleRegion_(MEMiExpHeapHead* pEHHead, const MemRegion* pRegion) {
    MEMiExpHeapMBlockHead* pBlkPrFree = NULL;
    MemRegion freeRgn = *pRegion;

    {
        MEMiExpHeapMBlockHead* pBlk;

        for (pBlk = pEHHead->mbFreeList.head; pBlk; pBlk = pBlk->next) {
            if (pBlk < pRegion->start) {
                pBlkPrFree = pBlk;
                continue;
            }

            if (pBlk == pRegion->end) {
                freeRgn.end = GetMBlockEndAddr_(pBlk);
                (void)RemoveMBlock_(&pEHHead->mbFreeList, pBlk);
                FillNoUseMemory(GetHeapHeadPtrFromExpHeapHead_(pEHHead), pBlk, sizeof(MEMiExpHeapMBlockHead));
            }
            break;
        }
    }

    if (pBlkPrFree && GetMBlockEndAddr_(pBlkPrFree) == pRegion->start) {
        freeRgn.start = pBlkPrFree;
        pBlkPrFree = RemoveMBlock_(&pEHHead->mbFreeList, pBlkPrFree);
    }

    if (GetOffsetFromPtr(freeRgn.start, freeRgn.end) < sizeof(MEMiExpHeapMBlockHead)) {
        return FALSE;
    }

    FillFreeMemory(GetHeapHeadPtrFromExpHeapHead_(pEHHead), pRegion->start, GetOffsetFromPtr(pRegion->start, pRegion->end));

    (void)InsertMBlock_(&pEHHead->mbFreeList, InitFreeMBlock_(&freeRgn), pBlkPrFree);

    return TRUE;
}

MEMHeapHandle MEMCreateExpHeapEx(void* startAddress, u32 size, u16 optFlag) {
    void* endAddress;

    endAddress = RoundDownPtr(AddU32ToPtr(startAddress, size), 4);
    startAddress = RoundUpPtr(startAddress, 4);

    if (GetUIntPtr(startAddress) > GetUIntPtr(endAddress) ||
        GetOffsetFromPtr(startAddress, endAddress) < sizeof(MEMiHeapHead) + sizeof(MEMiExpHeapHead) + sizeof(MEMiExpHeapMBlockHead) + 4) {
        return 0;
    }

    {
        MEMiHeapHead* pHeapHd = InitExpHeap_(startAddress, endAddress, optFlag);
        return pHeapHd;
    }
}

void* MEMAllocFromExpHeapEx(MEMHeapHandle heap, u32 size, int alignment) {
    void* memory = NULL;

    if (size == 0) {
        size = 1;
    }

    size = RoundUp(size, 4);

    LockHeap(heap);

    if (alignment >= 0) {
        memory = AllocFromHead_(heap, size, alignment);
    } else {
        memory = AllocFromTail_(heap, size, -alignment);
    }

    UnlockHeap(heap);

    return memory;
}

static inline MEMiExpHeapMBlockHead* GetMBlockHeadPtr_(void* memBlock) {
    return (MEMiExpHeapMBlockHead*)SubU32ToPtr(memBlock, sizeof(MEMiExpHeapMBlockHead));
}

static inline MEMiExpHeapHead* GetExpHeapHeadPtrFromHandle_(MEMHeapHandle heap) {
    return (MEMiExpHeapHead*)GetExpHeapHeadPtrFromHeapHead_(heap);
}

void MEMFreeToExpHeap(MEMHeapHandle heap, void* memBlock) {
    MEMiHeapHead* pHeapHd;
    MEMiExpHeapHead* pExpHeapHd;
    MEMiExpHeapMBlockHead* pMBHead;
    MemRegion region;

    if (memBlock == NULL) {
        return;
    }

#if SDK_AUG2010
    LockHeap(heap);
#endif

    pHeapHd = heap;
    pExpHeapHd = GetExpHeapHeadPtrFromHandle_(pHeapHd);
    pMBHead = GetMBlockHeadPtr_(memBlock);


#if !SDK_AUG2010
    LockHeap(heap);
#endif

    GetRegionOfMBlock_(&region, pMBHead);
    (void)RemoveMBlock_(&pExpHeapHd->mbUsedList, pMBHead);
    (void)RecycleRegion_(pExpHeapHd, &region);

    UnlockHeap(heap);
}
