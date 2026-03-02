#include "JSystem/JSystem.h" // IWYU pragma: keep

#include <algorithm>

static void dummy() {
    u32 a = 0;
    u32 b = 1;
    std::min<u32>(a, b);
}
