#include "wstring.h"

size_t wcslen(const wchar_t* str) {
    size_t len = -1;
    wchar_t* p = (wchar_t*)str - 1;

    do {
        len++;
    } while (*++p);

    return len;
}

wchar_t* wcsncpy(wchar_t* dest, const wchar_t* src, size_t num) {
    const wchar_t* p = (const wchar_t*)src - 1;
    wchar_t* q = (wchar_t*)dest - 1;
    wchar_t zero = 0;

    num++;

    while (--num) {
        if (!(*++q = *++p)) {
            while (--num) {
                *++q = 0;
            }

            break;
        }
    }

    return dest;
}

int wcscmp(const wchar_t* str1, const wchar_t* str2) {
    const wchar_t* p1 = (wchar_t*)str1 - 1;
    const wchar_t* p2 = (wchar_t*)str2 - 1;
    wchar_t c1, c2;

    while ((c1 = *++p1) == (c2 = *++p2))
        if (!c1)
            return 0;

    return c1 - c2;
}
