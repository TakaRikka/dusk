#include <dolphin/dolphin.h>
#include <dolphin/ax.h>

static void* __AXFXAllocFunction(u32 bytes) {
    return OSAlloc(bytes);
}

static void __AXFXFreeFunction(void* p) {
    OSFree(p);
}

void* (*__AXFXAlloc)(u32) = __AXFXAllocFunction;
void (*__AXFXFree)(void*) = __AXFXFreeFunction;

void AXFXSetHooks(void* (*alloc)(u32), void (*free)(void*)) {
    ASSERTLINE(46, alloc && free);

    __AXFXAlloc = alloc;
    __AXFXFree = free;
}
