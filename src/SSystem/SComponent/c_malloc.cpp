/**
 * c_malloc.cpp
 *
 */

#include "SSystem/SComponent/c_malloc.h"
#include "JSystem/JKernel/JKRHeap.h"

JKRHeap* cMl::Heap;

void cMl::init(JKRHeap* heap) {
    Heap = heap;
}

void* cMl::memalignB(int alignment, u32 size) {
    void* ret;
    if (size == 0) {
        ret = NULL;
    } else {
        ret = Heap->alloc(size, alignment);
    }

    return ret;
}

void cMl::free(void* ptr) {
    if (ptr != NULL) {
        Heap->free(ptr);
    }
}
