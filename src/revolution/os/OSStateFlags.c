#include <revolution/os.h>
#include <revolution/nand.h>
#include <stdio.h>

static OSStateFlags StateFlags ATTRIBUTE_ALIGN(32);

static u32 CheckSum(OSStateFlags* flags) {
    u32* ptr, i, sum;

    ptr = (u32*)&flags->lastBootApp;
    sum = 0;

    for (i = 0; i < (sizeof(OSStateFlags) - 4) / 4; i++) {
        sum = sum + *ptr;
        ptr++;
    }

    return sum;
}

BOOL __OSWriteStateFlags(OSStateFlags* flags) {
    NANDFileInfo fileInfo;
    s32 result;

    memcpy(&StateFlags, flags, sizeof(StateFlags));
    StateFlags.checkSum = CheckSum(&StateFlags);

    result = NANDOpen("/title/00000001/00000002/data/state.dat", &fileInfo, 2);

    if (result == 0) {
        result = NANDWrite(&fileInfo, &StateFlags, sizeof(StateFlags));
        if (result != sizeof(StateFlags)) {
            NANDClose(&fileInfo);
            return FALSE;
        }

        result = NANDClose(&fileInfo);
        if (result != 0) {
            return FALSE;
        }
    } else {
        return FALSE;
    }

    return TRUE;
}

BOOL __OSReadStateFlags(OSStateFlags* flags) {
    NANDFileInfo fileInfo;
    s32 result;

    result = NANDOpen("/title/00000001/00000002/data/state.dat", &fileInfo, 1);
    if (result == 0) {
        result = NANDRead(&fileInfo, &StateFlags, sizeof(OSStateFlags));
        NANDClose(&fileInfo);

        if (result != sizeof(OSStateFlags)) {
            NANDDelete("/title/00000001/00000002/data/state.dat");
            memset(flags, 0, sizeof(StateFlags));
            return FALSE;
        }
    } else {
        memset(flags, 0, sizeof(OSStateFlags));
        return FALSE;
    }

    if (CheckSum(&StateFlags) != StateFlags.checkSum) {
        memset(flags, 0, sizeof(OSStateFlags));
        return FALSE;
    }

    memcpy(flags, &StateFlags, sizeof(OSStateFlags));
    return TRUE;
}
