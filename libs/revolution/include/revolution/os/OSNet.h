#ifndef _REVOLUTION_OSNET_H_
#define _REVOLUTION_OSNET_H_

#include <revolution/ipc.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum NWC24Err {
    NWC24_OK = 0,
} NWC24Err;

NWC24Err NWC24iPrepareShutdown(void);
NWC24Err NWC24iSynchronizeRtcCounter(BOOL);
NWC24Err NWC24iOpenResourceManager_(const char* callerName, const char* path, IOSFd* fd, u32 flags);
NWC24Err NWC24SuspendScheduler(void);
NWC24Err NWC24iSetRtcCounter_(int rtc, BOOL);
NWC24Err NWC24iIoctlResourceManager_(const char* callerName, IOSFd fd, s32 cmd, void* input, u32 inputLen, void* output, u32 outputLen);
NWC24Err NWC24iCloseResourceManager_(const char* callerName, IOSFd);
NWC24Err NWC24iRequestShutdown(u32 event, s32* result);
NWC24Err NWC24iIoctlResourceManagerAsync_(const char* callerName, IOSFd fd, s32 cmd, void* input, u32 inputLen, void* output, u32 outputLen, void* cbArg);
BOOL NWC24iIsAsyncRequestPending_(void);

#ifdef __cplusplus
}
#endif

#endif
