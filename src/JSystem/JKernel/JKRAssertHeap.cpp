#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRAssertHeap.h"

JKRAssertHeap::JKRAssertHeap(void* data, u32 size, JKRHeap* parent, bool errorFlag)
    : JKRHeap(data, size, parent, errorFlag) {}

JKRAssertHeap::~JKRAssertHeap() {
    this->dispose();
}

JKRAssertHeap* JKRAssertHeap::create(JKRHeap* parent) {
    if (!parent) {
        parent = sRootHeap;
    }

    u32 size = ALIGN_NEXT(sizeof(JKRAssertHeap), 16);

    void* ptr = JKRAllocFromHeap(parent, size, 16);
    if (!ptr)
        return NULL;

    JKRAssertHeap* heap = new (ptr) JKRAssertHeap(NULL, 0, parent, false);
    return heap;
}

void JKRAssertHeap::do_destroy() {
    this->~JKRAssertHeap();
}

u32 JKRAssertHeap::getHeapType() {
    return 'ASTH';
}

bool JKRAssertHeap::check() {
    return true;
}

bool JKRAssertHeap::dump() {
    return true;
}

bool JKRAssertHeap::dump_sort() {
    return true;
}
