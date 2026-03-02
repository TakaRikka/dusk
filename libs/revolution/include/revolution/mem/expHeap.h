#ifndef _REVOLUTION_MEM_EXPHEAP_H_
#define _REVOLUTION_MEM_EXPHEAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/mem/heapCommon.h>

typedef struct MEMiExpHeapMBlockHead MEMiExpHeapMBlockHead;

struct MEMiExpHeapMBlockHead {
    u16 signature;

    union {
        u16 val;

        struct {
            u16 allocDir : 1;
            u16 alignment : 7;
            u16 groupID : 8;
        } fields;
    } attribute;

    u32 blockSize;
    MEMiExpHeapMBlockHead* prev;
    MEMiExpHeapMBlockHead* next;
};

typedef struct MEMiExpMBlockList MEMiExpMBlockList;

struct MEMiExpMBlockList {
    MEMiExpHeapMBlockHead* head;
    MEMiExpHeapMBlockHead* tail;
};

typedef struct MEMiExpHeapHead MEMiExpHeapHead;

struct MEMiExpHeapHead {
    MEMiExpMBlockList mbFreeList;
    MEMiExpMBlockList mbUsedList;

    u16 groupID;

    union {
        u16 val;
        struct {
            u16 _reserved : 14;
            u16 useMarginOfAlign : 1;
            u16 allocMode : 1;
        } fields;
    } feature;
};

MEMHeapHandle MEMCreateExpHeapEx(void*, u32, u16);
void* MEMDestroyExpHeap(MEMHeapHandle);
void* MEMAllocFromExpHeapEx(MEMHeapHandle, u32, int);
void MEMFreeToExpHeap(MEMHeapHandle, void*);
u32 MEMGetAllocatableSizeForExpHeapEx(MEMHeapHandle, int);
MEMHeapHandle MEMCreateFrmHeapEx(void* start, u32 size, u16 opt);
MEMHeapHandle MEMDestroyFrmHeap(MEMHeapHandle heap);

static inline MEMHeapHandle MEMCreateExpHeap(void* start, u32 size) {
    return MEMCreateExpHeapEx(start, size, 0);
}

static inline void* MEMAllocFromExpHeap(MEMHeapHandle heap, u32 size) {
    return MEMAllocFromExpHeapEx(heap, size, 4);
}

#ifdef __cplusplus
}
#endif

#endif  // _REVOLUTION_MEM_EXPHEAP_H_
