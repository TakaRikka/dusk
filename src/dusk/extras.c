#include "dusk/extras.h"
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>

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


void *_memcpy(void* dest, void const* src, int n) {
    return memcpy(dest, src, n);
}

void DCZeroRange(void* addr, uint32_t nBytes) {
    bzero(addr, nBytes);
}
