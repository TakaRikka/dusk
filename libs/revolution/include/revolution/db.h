#ifndef _REVOLUTION_DB_H_
#define _REVOLUTION_DB_H_

#include <revolution/types.h>
#include <revolution/db/DBInterface.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_DBINTERFACE_ADDR 0x00000040

BOOL DBIsDebuggerPresent(void);
void DBPrintf(char* str, ...);
s32 DBQueryData(void);
u32 DBRead(u8*, u32);

#ifdef __cplusplus
}
#endif

#endif // _REVOLUTION_DB_H_
