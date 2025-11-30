/**
 * target_options.c
 * Description:
 */

#include "TRK_MINNOW_DOLPHIN/Os/dolphin/target_options.h"

static u8 useSerialIO;

void SetUseSerialIO(u8 serialIO) {
    useSerialIO = serialIO;
}

u8 GetUseSerialIO(void) {
    return useSerialIO;
}
