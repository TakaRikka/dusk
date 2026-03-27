#ifndef DUSK_STRING_HPP
#define DUSK_STRING_HPP

#include "global.h"
#include <cstring>
#include <dolphin/os.h>

namespace dusk {

inline void strncpyProxy(char* dst, const char* src, size_t count) {
#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
    strncpy(dst, src, count);
#if _MSC_VER
#pragma warning(pop)
#endif
}

/**
 * Copy a string to a fixed-size array.
 * Truncates if the destination is not large enough, always inserts a null terminator (padding the remainder of the buffer with zeroes.)
 */
template <size_t BufSize>
void SafeStringCopyTruncate(char (&buffer)[BufSize], const char* src) {
    static_assert(BufSize > 0, "Target buffer cannot be size zero");

    if (buffer == src) {
        CRASH("Cannot copy string to same buffer");
    }

    strncpyProxy(buffer, src, BufSize);
    buffer[BufSize - 1] = 0;
}

/**
 * Copy a string to a fixed-size array.
 * Aborts if the destination is not large enough, always inserts a null terminator (padding the remainder of the buffer with zeroes.)
 */
template <size_t BufSize>
void SafeStringCopy(char (&buffer)[BufSize], const char* src) {
    static_assert(BufSize > 0, "Target buffer cannot be size zero");
    if (buffer == src) {
        CRASH("Cannot copy string to same buffer");
    }

    if (strlen(src) > BufSize - 1) {
        CRASH("Destination buffer too small!");
    }

    strncpyProxy(buffer, src, BufSize);
    buffer[BufSize - 1] = 0;
}

}

#endif  // DUSK_STRING_HPP
