#ifndef DUSK_STRING_HPP
#define DUSK_STRING_HPP

#include "global.h"
#include <cstring>
#include <dolphin/os.h>

namespace dusk {

struct TextSpan {
    char* buffer;
    size_t size;

    constexpr operator char*() const {
        return buffer;
    }

    constexpr TextSpan(char* buffer, size_t size) : buffer(buffer), size(size) { }

    template<size_t BufSize>
    constexpr TextSpan(char (&buffer)[BufSize]) : buffer(buffer), size(BufSize) {
    }

    constexpr TextSpan() : buffer(nullptr), size(0) { }

    constexpr TextSpan operator++(int) {
        const auto prev = *this;

        if (size > 0) {
            size--;
        }
        buffer++;

        return prev;
    }

    constexpr char& operator*() const {
        if (size == 0) {
            CRASH("Span is empty!");
        }

        return *buffer;
    }
};

#if TARGET_PC
#define TEXT_SPAN dusk::TextSpan
#else
#define TEXT_SPAN char*
#endif

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

inline void SafeStringCopy(char* buffer, size_t bufSize, const char* src) {
    if (bufSize == 0) {
        CRASH("Target buffer cannot be size zero");
    }

    if (buffer == src) {
        CRASH("Cannot copy string to same buffer");
    }

    if (strlen(src) > bufSize - 1) {
        CRASH("Destination buffer too small!");
    }

    strncpyProxy(buffer, src, bufSize);
    buffer[bufSize - 1] = 0;
}

inline void SafeStringCopy(TextSpan dst, const char* src) {
    SafeStringCopy(dst.buffer, dst.size, src);
}

/**
 * Copy a string to a fixed-size array.
 * Aborts if the destination is not large enough, always inserts a null terminator (padding the remainder of the buffer with zeroes.)
 */
template <size_t BufSize>
void SafeStringCopy(char (&buffer)[BufSize], const char* src) {
    static_assert(BufSize > 0, "Target buffer cannot be size zero");
    SafeStringCopy(buffer, BufSize, src);
}

#if TARGET_PC
#define SAFE_STRCPY(dst, src) dusk::SafeStringCopy(dst, src)
#define SAFE_STRCPY_BOUNDED(dst, size, src) dusk::SafeStringCopy(dst, size, src)
#else
#define SAFE_STRCPY(dst, src) strcpy(dst, src)
#define SAFE_STRCPY_BOUNDED(dst, size, src) strcpy(dst, src)
#endif
}

#endif  // DUSK_STRING_HPP
