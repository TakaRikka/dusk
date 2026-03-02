#include <revolution/mem/allocator.h>
#include <revolution/mem/expHeap.h>

static void* AllocatorAllocForExpHeap_(MEMAllocator *pAllocator, u32 size) {
    return MEMAllocFromExpHeapEx(pAllocator->pHeap, size, pAllocator->heapParam1);
}

static void AllocatorFreeForExpHeap_(MEMAllocator *pAllocator, void *pBlock) {
    MEMFreeToExpHeap(pAllocator->pHeap, pBlock);
}

void* MEMAllocFromAllocator(MEMAllocator *pAllocator, u32 size) {
    return (*pAllocator->pFunc->pfAlloc)(pAllocator, size);
}

void MEMFreeToAllocator(MEMAllocator *pAllocator, void *pBlock) {
    (*pAllocator->pFunc->pfFree)(pAllocator, pBlock);
}

void MEMInitAllocatorForExpHeap(MEMAllocator *pAllocator, MEMHeapHandle handle, int align) {
    static const MEMAllocatorFunc sAllocatorFunc =
    {
        AllocatorAllocForExpHeap_,
        AllocatorFreeForExpHeap_,
    };

    pAllocator->pFunc = &sAllocatorFunc;
    pAllocator->pHeap = handle;
    pAllocator->heapParam1 = align;
    pAllocator->heapParam2 = 0;
}
