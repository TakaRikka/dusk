#include "dusk/string.hpp"
#include "fmt/format.h"

namespace {
void strncpyProxy(char* dst, const char* src, size_t count) {
#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
    strncpy(dst, src, count);
#if _MSC_VER
#pragma warning(pop)
#endif
}
}  // namespace

namespace dusk {

void TextSpan::CrashSpawnEmpty() {
    CRASH("Span is empty!");
}

void SafeStringCopyTruncate(char* buffer, size_t bufSize, const char* src) {
    if (buffer == src) [[unlikely]] {
        CRASH("Cannot copy string to same buffer");
    }

    if (bufSize == 0) [[unlikely]] {
        CRASH("Target buffer cannot be size zero");
    }

    strncpyProxy(buffer, src, bufSize);
    buffer[bufSize - 1] = 0;
}

void SafeStringCopy(char* buffer, size_t bufSize, const char* src) {
    if (bufSize == 0) [[unlikely]] {
        CRASH("Target buffer cannot be size zero");
    }

    if (buffer == src) [[unlikely]] {
        CRASH("Cannot copy string to same buffer");
    }

    const auto srcSize = strlen(src);
    if (srcSize > bufSize - 1) [[unlikely]] {
        const auto msg = fmt::format(
            "Destination buffer too small! Need %d, have %d",
            srcSize + 1,
            bufSize);
        CRASH("%s", msg.c_str());
    }

    strncpyProxy(buffer, src, bufSize);
    buffer[bufSize - 1] = 0;
}

}  // namespace dusk