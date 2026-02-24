// C++ Mangled version of extras.c
#include <cstring>
#include <cstdint>
#ifdef _MSC_VER
#include <intrin.h>
#endif

void *__memcpy(void* dest, void const* src, int n) {
    return memcpy(dest, src, n);
}

void __dcbz(void* addr, int offset) {
    // Gekko cache lines are 32 bytes.
    // dcbz usually requires addr to be 32-byte aligned.
    memset((char*)addr + offset, 0, 32); 
}

int __cntlzw(unsigned int val) {
    if (val == 0) return 32; // PowerPC returns 32 if the input is 0
#ifdef _MSC_VER
    unsigned long idx;
    _BitScanReverse(&idx, val);
    return 31 - (int)idx;
#else
    return __builtin_clz(val);
#endif
}
