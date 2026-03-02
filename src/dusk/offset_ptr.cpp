#include "dusk/offset_ptr.h"

#include "JSystem/JUtility/JUTAssert.h"

#if TARGET_PC
bool OffsetPtr::isRelocated() {
    return value & 0x8000'0000;
}

bool OffsetPtr::setBase(void* base) {
    JUT_ASSERT(__LINE__, value != 0);

    if (isRelocated()) {
        // Already relocated, don't touch it again!
        return false;
    }

    ptrdiff_t diff = (u8*)this - (u8*)base;
    ptrdiff_t newDiff = value - diff;
    // Check that it's in range given that we use the 31st bit as a flag.
    if (newDiff < -0x4000'0000 || newDiff > 0x7FFF'FFFF) {
        OSPanic(__FILE__, __LINE__, "Not enough space in StageOffsetPtr!");
    }

    value = newDiff | 0x8000'0000;
    return true;
}
#endif
