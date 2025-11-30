#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRAssertHeap.h"

JKRAssertHeap::JKRAssertHeap(void* data, unsigned long size, JKRHeap* parent, bool errorFlag)
    : JKRHeap(data, size, parent, errorFlag) {}

JKRAssertHeap::~JKRAssertHeap() {
    this->dispose();
}

JKRAssertHeap* JKRAssertHeap::create(JKRHeap* parent) {
    if (!parent) {
        parent = JKRHeap::getRootHeap();
    }

    // 0x70 is sizeof(JKRAssertHeap) aligned to 16 bytes
    u32 size = 0x70;
    int alignment = 16;

    void* ptr = JKRHeap::alloc(size, alignment, parent);
    if (!ptr)
        return NULL;

    return new (ptr) JKRAssertHeap(NULL, 0, parent, false);
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


s32 JKRAssertHeap::do_changeGroupID(u8) {
    return 0;
}

u8 JKRAssertHeap::do_getCurrentGroupId() {
    return 0;
}

void* JKRAssertHeap::do_alloc(u32, int) {
    return NULL;
}

void JKRAssertHeap::do_free(void* param_0) {}

void JKRAssertHeap::do_freeAll() {}

void JKRAssertHeap::do_freeTail() {}

void JKRAssertHeap::do_fillFreeArea() {}

s32 JKRAssertHeap::do_resize(void*, u32) {
    return 0;
}

s32 JKRAssertHeap::do_getSize(void*) {
    return 0;
}

s32 JKRAssertHeap::do_getFreeSize() {
    return 0;
}

void* JKRAssertHeap::do_getMaxFreeBlock() {
    return NULL;
}

s32 JKRAssertHeap::do_getTotalFreeSize() {
    return 0;
}
