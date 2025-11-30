#include "string.h"

void* memmove(void* dst, const void* src, size_t n) {
    unsigned char* csrc;
    unsigned char* cdst;

    int reverse = (unsigned int)src < (unsigned int)dst;

    if (n >= 32) {
        if (((unsigned int)dst ^ (unsigned int)src) & 3) {
            if (!reverse) {
                __copy_longs_unaligned(dst, src, n);
            } else {
                __copy_longs_rev_unaligned(dst, src, n);
            }
        } else {
            if (!reverse) {
                __copy_longs_aligned(dst, src, n);
            } else {
                __copy_longs_rev_aligned(dst, src, n);
            }
        }

        return dst;
    } else {
        if (!reverse) {
            csrc = ((unsigned char*)src) - 1;
            cdst = ((unsigned char*)dst) - 1;
            n++;

            while (--n > 0) {
                *++cdst = *++csrc;
            }
        } else {
            csrc = (unsigned char*)src + n;
            cdst = (unsigned char*)dst + n;
            n++;

            while (--n > 0) {
                *--cdst = *--csrc;
            }
        }
    }

    return dst;
}

void* memchr(const void* ptr, int ch, size_t count) {
    const unsigned char* p;

    unsigned long v = (ch & 0xff);

    for (p = (unsigned char*)ptr - 1, count++; --count;)
        if ((*++p & 0xff) == v)
            return (void*)p;

    return NULL;
}

void* __memrchr(const void* ptr, int ch, size_t count) {
    const unsigned char* p;

    unsigned long v = (ch & 0xff);

    for (p = (unsigned char*)ptr + count, count++; --count;)
        if (*--p == v)
            return (void*)p;

    return NULL;
}

int memcmp(const void* lhs, const void* rhs, size_t count) {
    const unsigned char* p1;
    const unsigned char* p2;

    for (p1 = (const unsigned char*)lhs - 1, p2 = (const unsigned char*)rhs - 1, count++; --count;)
        if (*++p1 != *++p2)
            return ((*p1 < *p2) ? -1 : +1);

    return 0;
}
