#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msghndlr.h"
#include "TRK_MINNOW_DOLPHIN/Os/dolphin/target_options.h"
#include "TRK_MINNOW_DOLPHIN/ppc/Export/targsupp.h"

DSIOResult __read_file(u32 handle, u8* buffer, size_t* count, void* ref_con);
DSIOResult __write_file(u32 handle, u8* buffer, size_t* count, void* ref_con);
DSIOResult __close_file(u32 handle, u8* buffer, size_t* count, void* ref_con);
DSIOResult __access_file(u32 handle, u8* buffer, size_t* count, void* ref_con,
                         MessageCommandID cmd);

DSIOResult __read_console(u32 handle, u8* buffer, size_t* count, void* ref_con) {
    if (GetUseSerialIO() == 0) {
        return DS_IOError;
    }
    return __read_file(DS_Stdin, buffer, count, ref_con);
}

DSIOResult __TRK_write_console(u32 handle, u8* buffer, size_t* count, void* ref_con) {
    if (GetUseSerialIO() == 0) {
        return DS_IOError;
    }
    return __write_file(DS_Stdout, buffer, count, ref_con);
}

static DSIOResult __read_file(u32 handle, u8* buffer, size_t* count, void* ref_con) {
    return __access_file(handle, buffer, count, ref_con, DSMSG_ReadFile);
}

static DSIOResult __write_file(u32 handle, u8* buffer, size_t* count, void* ref_con) {
    return __access_file(handle, buffer, count, ref_con, DSMSG_WriteFile);
}

static DSIOResult __access_file(u32 handle, u8* buffer, size_t* count, void* ref_con,
                                       MessageCommandID cmd) {
    size_t countTemp;
    u32 r0;

    if (GetTRKConnected() == DS_NoError) {
        return DS_IOError;
    }

    countTemp = *count;
    r0 = TRKAccessFile(cmd, handle, &countTemp, buffer);
    *count = countTemp;

    switch ((u8)r0) {
    case DS_IONoError:
        return DS_IONoError;
    case DS_IOEOF:
        return DS_IOEOF;
    }

    return DS_IOError;
}
