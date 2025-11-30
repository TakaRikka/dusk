#ifndef _REVOLUTION_OSSTATEFLAGS_H_
#define _REVOLUTION_OSSTATEFLAGS_H_

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u32 checkSum;
    u8 lastBootApp;
    u8 lastShutdown;
    u8 lastDiscState;
    u8 menuMode;
    u8 reserved[24];
} OSStateFlags;

BOOL __OSWriteStateFlags(OSStateFlags*);
BOOL __OSReadStateFlags(OSStateFlags*);

#ifdef __cplusplus
}
#endif

#endif // OSSTATEFLAGS_H
