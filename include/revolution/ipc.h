#ifndef _REVOLUTION_IPC_H_
#define _REVOLUTION_IPC_H_

#include <revolution/ipc/ipcclt.h>
#include <revolution/ipc/ipcProfile.h>
#include <revolution/ipc/memory.h>

#ifdef __cplusplus
extern "C" {
#endif

void IPCInit(void);
#if SDK_AUG2010
void IPCReInit(void);
#endif
u32 IPCReadReg(u32 regIdx);
void IPCWriteReg(u32 regIdx, u32 data);
void* IPCGetBufferHi(void);
void* IPCGetBufferLo(void);
void IPCSetBufferLo(void* newLo);

#ifdef __cplusplus
}
#endif

#endif // _REVOLUTION_IPC_H_
