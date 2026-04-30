#include "dusk/extras.h"
#include <ctype.h>
#include <string.h>
#include <stdint.h>

#ifdef _MSC_VER
#include <intrin.h>
#endif

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

#ifndef _MSC_VER
int stricmp(const char* str1, const char* str2) {
	char a_var;
	char b_var;

	do {
		b_var = tolower(*str1++);
		a_var = tolower(*str2++);

		if (b_var < a_var) {
			return -1;
		}
		if (b_var > a_var) {
			return 1;
		}
	} while (b_var != 0);

	return 0;
}

int strnicmp(const char* str1, const char* str2, int n) {
    int i;
    char c1, c2;

    for (i = 0; i < n; i++) {
        c1 = tolower(*str1++);
        c2 = tolower(*str2++);

        if (c1 < c2) {
            return -1;
        }

        if (c1 > c2) {
            return 1;
        }

        if (c1 == '\0') {
            return 0;
        }
    }

    return 0;
}
#endif

void DCZeroRange(void* addr, uint32_t nBytes) {
#if defined(_MSC_VER) || TARGET_ANDROID
    memset(addr, 0, nBytes);
#else
    bzero(addr, nBytes);
#endif
}
