#ifndef _REVOLUTION_MEM_HEAPCOMMON_H_
#define _REVOLUTION_MEM_HEAPCOMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/mem/list.h>
#include <revolution/os.h>
#include <cstring>

typedef struct MEMiHeapHead MEMiHeapHead;

struct MEMiHeapHead {
    u32 signature;
    MEMLink link;
    MEMList childList;
    void* heapStart;
    void* heapEnd;
    OSMutex mutex;

    union {
        u32 val;

        struct {
            u32 reserved : 24;
            u32 optFlag : 8;
        } fields;
    } attribute;
};

typedef MEMiHeapHead* MEMHeapHandle;

typedef u32 UIntPtr;

static inline UIntPtr GetUIntPtr(const void* ptr) {
    return (UIntPtr)(ptr);
}

static inline u32 GetOffsetFromPtr(const void* start, const void* end) {
    return GetUIntPtr(end) - GetUIntPtr(start);
}

static inline void* SubU32ToPtr(void* ptr, u32 val) {
    return (void*)(GetUIntPtr(ptr) - val);
}

static inline void* AddU32ToPtr(void* ptr, u32 val) {
    return (void*)(GetUIntPtr(ptr) + val);
}

static inline void SetOptForHeap(MEMiHeapHead* pHeapHd, u16 optFlag) {
    pHeapHd->attribute.fields.optFlag = (u8)optFlag;
}

static inline u16 GetOptForHeap(const MEMiHeapHead* pHeapHd) {
    return (u16)pHeapHd->attribute.fields.optFlag;
}

static inline void FillAllocMemory(MEMiHeapHead* pHeapHd, void* address, u32 size) {
    if (GetOptForHeap(pHeapHd) & 1) {
        (void)memset(address, 0, size);
    }
}

static inline int ComparePtr(const void* a, const void* b) {
    const u8* wa = (const u8*)a;
    const u8* wb = (const u8*)b;

    return wa - wb;
}


static inline void LockHeap(MEMiHeapHead* pHeapHd) {
    if (GetOptForHeap(pHeapHd) & 4) {
        OSLockMutex(&pHeapHd->mutex);
    }
}

static inline void UnlockHeap(MEMiHeapHead* pHeapHd) {
    if (GetOptForHeap(pHeapHd) & 4) {
        OSUnlockMutex(&pHeapHd->mutex);
    }
}

void MEMiInitHeapHead(MEMiHeapHead* pHeapHd, u32 signature, void* heapStart, void* heapEnd, u16 optFlag);

void MEMiFinalizeHeap(MEMiHeapHead* pHeapHd);

void MEMiDumpHeapHead(MEMiHeapHead* pHeapHd);

#ifdef __cplusplus
}
#endif

#endif  // _REVOLUTION_MEM_HEAPCOMMON_H_
