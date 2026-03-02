#include <revolution.h>
#include <revolution/private/iostypes.h>

typedef struct IOSChunk {
    u32 status;
    u32 size;
    struct IOSChunk* prevFree;
    struct IOSChunk* nextFree;
} IOSChunk;

typedef struct {
    void* base;
    u32 owner;
    u32 size;
    IOSChunk *freeList;
} IOSHeap;

static IOSHeap __heaps[8];

static void __iosCoalesceChunk(IOSChunk* p) {
    if (p && ((u32)p->nextFree == ((u32)p + p->size + sizeof(IOSChunk)))) {
        IOSChunk* next = p->nextFree;
        p->nextFree = next->nextFree;

        if (p->nextFree != 0) {
            p->nextFree->prevFree = p;
        }

        p->size += next->size + sizeof(IOSChunk);
    }
}

IOSHeapId iosCreateHeap(void* ptr, u32 size) {
    IOSError rv = -4;
    u32 mask;
    s32 i;
    IOSHeap* h;

    mask = OSDisableInterrupts();

    if ((u32)ptr & 31) {
        goto finish;
    }

    for (i = 0; i < sizeof(__heaps) / sizeof(__heaps[0]); ++i) {
        if (__heaps[i].base == 0) {
            break;
        }
    }

    if (i == 8) {
        rv = -5;
        goto finish;
    }

    h = __heaps + i;
    h->base = ptr;
    h->size = size;
    h->freeList = ptr;
    h->freeList->status = 0xBABE0000;
    h->freeList->size = size - sizeof(IOSChunk);
    h->freeList->prevFree = NULL;
    h->freeList->nextFree = NULL;
    rv = i;

finish:
    OSRestoreInterrupts(mask);
    return rv;
}

#define CHECK_HEAP_ID(id, err) \
    if (((id) < 0) || ((id) >= 8) || (!__heaps[id].base)) { \
        rv = err; \
        goto finish; \
    }

#define GET_HEAP(h, id, err) \
    do { \
        CHECK_HEAP_ID(id, err); \
        h = __heaps + id; \
    } while (0)

void* __iosAlloc(IOSHeapId id, u32 size, u32 alignment) {
    u32 mask;
    IOSChunk* p, *best;
    IOSHeap* h;
    void* rv = NULL;

    mask = OSDisableInterrupts();

    if (!size) {
        goto finish;
    }

    if (!alignment || (alignment & (alignment - 1))) {
        goto finish;
    }
    
    if (alignment < 32) {
        alignment = 32;
    }

    size = (size + 31) & ~31;

    GET_HEAP(h, id, NULL);

    p = h->freeList;
    best = NULL;

    while (p != 0) {
        u32 ptr = (u32)p + sizeof(IOSChunk);
        u32 extra = (alignment - (ptr & (alignment - 1))) & (alignment - 1);

        if ((p->size == size) && !extra) {
            best = p;
            break;
        } else if ((p->size >= (size + extra)) && (!best || (p->size < best->size))) {
            best = p;
        }

        p = p->nextFree;
    }

    p = best;

    if (p != 0) {
        u32 ptr = (u32)p + sizeof(IOSChunk);
        u32 extra = (alignment - (ptr & (alignment - 1))) & (alignment - 1);

        if (p->size > (size + extra + sizeof(IOSChunk))) {
            IOSChunk *n = (IOSChunk *)((u8 *)p + size + extra + sizeof(IOSChunk));
            n->status = 0xBABE0000;
            n->size = p->size - size - extra - sizeof(IOSChunk);
            n->nextFree = p->nextFree;

            if (n->nextFree) {
                n->nextFree->prevFree = n;
            }

            p->nextFree = n;
            p->size = size + extra;
        }

        p->status = 0xBABE0001;

        if (p->prevFree) {
            p->prevFree->nextFree = p->nextFree;
        } else {
            h->freeList = p->nextFree;
        }

        if (p->nextFree) {
            p->nextFree->prevFree = p->prevFree;
        }

        p->prevFree = p->nextFree = NULL;
        rv = (u8*)p + extra + sizeof(IOSChunk);

        if (extra) {
            IOSChunk* n = (IOSChunk*)rv - 1;
            n->status = 0xBABE0002;
            n->prevFree = p;
        }
    }

finish:
    OSRestoreInterrupts(mask);
    return rv;
}

void* iosAllocAligned(IOSHeapId id, u32 size, u32 alignment) {
    return __iosAlloc(id, size, alignment);
}

IOSError iosFree(IOSHeapId id, void* ptr) {
    u32 mask;
    IOSChunk* p;
    IOSHeap* h;
    IOSError rv = -4;
    IOSChunk* prev;

    mask = OSDisableInterrupts();

    if (!ptr) {
        goto finish;
    }

    GET_HEAP(h, id, -4);

    if (((u32)ptr) < (((u32)h->base) + sizeof(IOSChunk)) || ((u32)ptr) > ((u32)h->base + h->size)) {
        goto finish;
    }

    p = (IOSChunk*)ptr - 1;

    if (p->status == 0xBABE0002) {
        p = p->prevFree;
    }

    if (p->status != 0xBABE0001) {
        goto finish;
    }

    p->status = 0xBABE0000;
    prev = h->freeList;

    while (prev != 0) {
        if (!prev->nextFree || prev->nextFree > p) {
            break;
        }

        prev = prev->nextFree;
    }

    if (prev && p > prev) {
        p->prevFree = prev;
        p->nextFree = prev->nextFree;
        prev->nextFree = p;

        if (p->nextFree) {
            p->nextFree->prevFree = p;
        }
    } else {
        p->nextFree = h->freeList;
        h->freeList = p;
        p->prevFree = NULL;

        if (p->nextFree) {
            p->nextFree->prevFree = p;
        }
    }

    __iosCoalesceChunk(p);
    __iosCoalesceChunk(p->prevFree);
    rv = 0;

finish:
    OSRestoreInterrupts(mask);
    return rv;
}
