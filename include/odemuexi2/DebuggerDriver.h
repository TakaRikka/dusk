#ifndef ODEMUEXI_DEBUGGERDRIVER_H
#define ODEMUEXI_DEBUGGERDRIVER_H

#include "types.h"

typedef void (*MTRCallbackType)(int);

void DBInitComm(u8** a, MTRCallbackType b);

void DBInitInterrupts(void);

u32 DBQueryData(void);

BOOL DBRead(u32* buffer, s32 count);

BOOL DBWrite(void* src, u32 size);

void DBOpen(void);

void DBClose(void);

#endif /* ODEMUEXI_DEBUGGERDRIVER_H */
