#ifndef DUSK_STRING_HPP
#define DUSK_STRING_HPP

#include "global.h"
#include <cstring>
#include <dolphin/os.h>

namespace dusk {

/**
 * Copy a string to a fixed-size array.
 * Truncates if the destination is not large enough, always inserts a null terminator (padding the remainder of the buffer with zeroes.)
 */
template <size_t BufSize>
void SafeStringCopyTruncate(char (&buffer)[BufSize], const char* src) {
    static_assert(BufSize > 0, "Target buffer cannot be size zero");

    // Can't just use strncpy as I can't figure out how to selectively mute the warnings on MSVC.
    // And can't use strncpy_s on MSVC  as it doesn't fill the entire buffer.
    constexpr size_t copyBufSize = BufSize - 1;
    const auto srcLen = strlen(src);
    const auto copyLen = srcLen > copyBufSize ? copyBufSize : srcLen;
    memcpy(buffer, src, copyLen);
    memset(buffer + copyLen, 0, BufSize - copyLen);
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

    // Can't just use strncpy as I can't figure out how to selectively mute the warnings on MSVC.
    // And can't use strncpy_s on MSVC as it doesn't fill the entire buffer.
    constexpr size_t copyBufSize = BufSize - 1;
    const auto srcLen = strlen(src);
    if (srcLen > copyBufSize) {
        CRASH("Destination buffer too small!");
    }

    memcpy(buffer, src, srcLen);
    memset(buffer + srcLen, 0, BufSize - srcLen);
}

}

#endif  // DUSK_STRING_HPP
