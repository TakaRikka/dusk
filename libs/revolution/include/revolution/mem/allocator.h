#ifndef _REVOLUTION_MEM_ALLOCATOR_H_
#define _REVOLUTION_MEM_ALLOCATOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/mem/heapCommon.h>
#include <revolution/types.h>

typedef struct MEMAllocator MEMAllocator;
typedef void* (*MEMFuncAllocatorAlloc)(MEMAllocator* pAllocator, u32 size);
typedef void (*MEMFuncAllocatorFree)(MEMAllocator* pAllocator, void* memBlock);
typedef struct MEMAllocatorFunc MEMAllocatorFunc;

struct MEMAllocatorFunc {
    MEMFuncAllocatorAlloc pfAlloc;
    MEMFuncAllocatorFree pfFree;
};

struct MEMAllocator {
    const MEMAllocatorFunc* pFunc;
    void* pHeap;
    u32 heapParam1;
    u32 heapParam2;
};

void* MEMAllocFromAllocator(MEMAllocator*, u32);
void MEMFreeToAllocator(MEMAllocator*, void*);
void MEMInitAllocatorForExpHeap(MEMAllocator*, MEMHeapHandle, int);
void MEMInitAllocatorForFrmHeap(MEMAllocator* allocator, struct MEMiHeapHead* heap, s32 align);

#ifdef __cplusplus
}
#endif

#endif  // _REVOLUTION_MEM_ALLOCATOR_H_
