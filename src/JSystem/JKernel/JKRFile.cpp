#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRFile.h"
#include "dolphin/vi.h"

void JKRFile::read(void* data, s32 length, s32 offset) {
    JUT_ASSERT(32, ( length & 0x1f ) == 0);
    while (true) {
        if (length != readData(data, length, offset)) {
            VIWaitForRetrace();
        } else {
            return;
        }
    }
}
