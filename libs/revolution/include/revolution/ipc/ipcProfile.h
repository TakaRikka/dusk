#ifndef _REVOLUTION_IPCPROFILE_H_
#define _REVOLUTION_IPCPROFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

void IPCiProfInit(void);
void IPCiProfQueueReq(void* req, s32 handle);
void IPCiProfReply(void* req, s32 handle);
void IPCiProfAck(void);

#ifdef __cplusplus
}
#endif

#endif // _REVOLUTION_IPCPROFILE_H_
