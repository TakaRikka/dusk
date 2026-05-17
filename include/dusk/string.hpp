#ifndef DUSK_STRING_HPP
#define DUSK_STRING_HPP

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
#define TEXT_SPAN dusk::TextSpan
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

inline void SafeStringCopy(TextSpan dst, const char* src) {
    SafeStringCopy(dst.buffer, dst.size, src);
}

inline void SafeStringCat(TextSpan dst, const char* src) {
    SafeStringCat(dst.buffer, dst.size, src);
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

#if TARGET_PC
#define SAFE_STRCPY(dst, src) dusk::SafeStringCopy(dst, src)
#define SAFE_STRCAT(dst, src) dusk::SafeStringCat(dst, src)
#define SAFE_STRCPY_BOUNDED(dst, size, src) dusk::SafeStringCopy(dst, size, src)
#define SAFE_STRCAT_BOUNDED(dst, size, src) dusk::SafeStringCat(dst, size, src)
#else
#define SAFE_STRCPY(dst, src) strcpy(dst, src)
#define SAFE_STRCAT(dst, src) strcat(dst, src)
#define SAFE_STRCPY_BOUNDED(dst, size, src) strcpy(dst, src)
#define SAFE_STRCPY_BOUNDED(dst, size, src) strcat(dst, src)
#endif
}

#endif  // DUSK_STRING_HPP
