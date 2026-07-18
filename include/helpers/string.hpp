#pragma once

#include <cstdarg>
#include <cstddef>

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

        if (size > 0) [[likely]] {
            size--;
        }
        buffer++;

        return prev;
    }

    constexpr char& operator*() const {
        if (size == 0) [[unlikely]] {
            CrashSpawnEmpty();
        }

        return *buffer;
    }

private:
    static void CrashSpawnEmpty();
};

#if TARGET_PC
#define TEXT_SPAN TextSpan
#else
#define TEXT_SPAN char*
#endif

void SafeStringCopyTruncate(char* buffer, size_t bufSize, const char* src);

/**
 * Copy a string to a fixed-size array.
 * Truncates if the destination is not large enough, always inserts a null terminator (padding the remainder of the buffer with zeroes.)
 */
template <size_t BufSize>
void SafeStringCopyTruncate(char (&buffer)[BufSize], const char* src) {
    static_assert(BufSize > 0, "Target buffer cannot be size zero");
    SafeStringCopyTruncate(buffer, BufSize, src);
}

void SafeStringCopy(char* buffer, size_t bufSize, const char* src);
void SafeStringCat(char* buffer, size_t bufSize, const char* src);
int SafeStringVPrintf(char* buffer, size_t bufSize, const char* src, std::va_list args);

inline void SafeStringCopy(TextSpan dst, const char* src) {
    SafeStringCopy(dst.buffer, dst.size, src);
}

inline void SafeStringCat(TextSpan dst, const char* src) {
    SafeStringCat(dst.buffer, dst.size, src);
}

inline int SafeStringPrintf(TextSpan dst, const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    const auto ret = SafeStringVPrintf(dst.buffer, dst.size, format, args);
    va_end(args);

    return ret;
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

template <size_t BufSize>
void SafeStringCat(char (&buffer)[BufSize], const char* src) {
    static_assert(BufSize > 0, "Target buffer cannot be size zero");
    SafeStringCat(buffer, BufSize, src);
}

template <size_t BufSize>
int SafeStringPrintf(char (&buffer)[BufSize], const char* format, ...) {
    static_assert(BufSize > 0, "Target buffer cannot be size zero");

    std::va_list args;
    va_start(args, format);
    const auto ret = SafeStringVPrintf(buffer, BufSize, format, args);
    va_end(args);

    return ret;
}

#if TARGET_PC
#define SAFE_STRCPY SafeStringCopy
#define SAFE_STRCAT SafeStringCat
#define SAFE_SPRINTF SafeStringPrintf
#define SAFE_STRCPY_BOUNDED SafeStringCopy
#define SAFE_STRCAT_BOUNDED SafeStringCat
#else
#define SAFE_STRCPY strcpy
#define SAFE_STRCAT strcat
#define SAFE_SPRINTF sprintf
#define SAFE_STRCPY_BOUNDED strcpy
#define SAFE_STRCPY_BOUNDED strcat
#endif
