#ifndef _DOLPHIN_OSALLOC_H_
#define _DOLPHIN_OSALLOC_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int OSHeapHandle;

extern volatile OSHeapHandle __OSCurrHeap;

void* OSAllocFromHeap(int heap, u32 size);
void* OSAllocFixed(void* rstart, void* rend);
void OSFreeToHeap(int heap, void* ptr);
int OSSetCurrentHeap(int heap);
void* OSInitAlloc(void* arenaStart, void* arenaEnd, int maxHeaps);
int OSCreateHeap(void* start, void* end);
void OSDestroyHeap(int heap);
void OSAddToHeap(int heap, void* start, void* end);
s32 OSCheckHeap(int heap);
u32 OSReferentSize(void* ptr);
void OSDumpHeap(int heap);
void OSVisitAllocated(void (*visitor)(void*, u32));

#define OSAlloc(size) OSAllocFromHeap(__OSCurrHeap, (size))
#define OSFree(ptr)   OSFreeToHeap(__OSCurrHeap, (ptr))

#ifdef __cplusplus
}
#endif

#endif
