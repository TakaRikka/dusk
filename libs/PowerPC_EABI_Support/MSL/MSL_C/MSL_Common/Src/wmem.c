#include <wmem.h>

#include <cstddef>
#include <cstring>

void wmemcpy(wchar_t* dst, const wchar_t* src, size_t n) {
    memcpy(dst, src, n * 2);
}

const wchar_t* wmemchr(const wchar_t* str, wchar_t needle, int max_len) {
    int i;
    for (i = 0; i != max_len; i++) {
        if (*str == needle) {
            return str;
        }
        str++;
    }
    return NULL;
}
