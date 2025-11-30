/**
 * flush_cache.c
 * Description:
 */

#include "dolphin/types.h"

asm void TRK_flush_cache(u32, int) {
    // clang-format off
    nofralloc

    lis r5, 0xFFFF
    ori r5, r5, 0xFFF1
    and r5, r5, r3
    subf r3, r5, r3
    add r4, r4, r3

loop:
    dcbst 0, r5
    dcbf 0, r5
    sync
    icbi 0, r5
    addic r5, r5, 8
    addic. r4, r4, -8
    bge loop

    isync 
    blr
    // clang-format on
}
