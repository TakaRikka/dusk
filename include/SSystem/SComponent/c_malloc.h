#ifndef C_MALLOC_H
#define C_MALLOC_H

#include "dolphin/types.h"

class JKRHeap;

struct cMl {
    static JKRHeap* Heap;
    static void init(JKRHeap*);
    static void* memalignB(int, u32);
    static void free(void*);
};

#endif /* C_MALLOC_H */
