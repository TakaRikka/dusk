#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRFile.h"
#include "dolphin/vi.h"

s32 JKRFile::read(void* data, s32 size, s32 offset) {
    while (true) {
        s32 result = readData(data, size, offset);
        if (size != result)
            VIWaitForRetrace();
        else
            return result;
    }
}
