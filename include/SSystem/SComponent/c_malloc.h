#ifndef C_MALLOC_H
#define C_MALLOC_H

#include <types.h>

class JKRHeap;

struct cMl {
    static DUSK_GAME_DATA JKRHeap* Heap;
    static void init(JKRHeap*);
    static void* memalignB(int, u32);
    static void free(void*);
};

#endif /* C_MALLOC_H */
