#ifndef _REVOLUTION_IPCCLT_H_
#define _REVOLUTION_IPCCLT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/private/iosrestypes.h>
#include <revolution/private/iostypes.h>

typedef IOSError (*IOSIpcCb)(IOSError, void*);

IOSError IPCCltInit(void);
#if SDK_AUG2010
IOSError IPCCltReInit(void);
#endif

IOSError IOS_OpenAsync(const char* pPath, u32 flags, IOSIpcCb cb, void* callback_arg);
IOSError IOS_Open(const char* path, u32 flags);
IOSError IOS_CloseAsync(IOSFd fd, IOSIpcCb cb, void* cbArg);
IOSError IOS_Close(IOSFd fd);
IOSError IOS_ReadAsync(IOSFd fd, void* buf, u32 len, IOSIpcCb cb, void* cbArg);
IOSError IOS_Read(IOSFd fd, void* buf, u32 len);
IOSError IOS_WriteAsync(IOSFd fd, void* buf, u32 len, IOSIpcCb cb, void* cbArg);
IOSError IOS_Write(IOSFd fd, void* buf, u32 len);
IOSError IOS_SeekAsync(IOSFd fd, s32 offset, u32 whence, IOSIpcCb cb, void* cbArg);
IOSError IOS_Seek(IOSFd fd, s32 offset, u32 whence);
IOSError IOS_IoctlAsync(IOSFd fd, s32 cmd, void* input, u32 inputLen, void* output, u32 outputLen, IOSIpcCb cb, void* cbArg);
IOSError IOS_Ioctl(IOSFd fd, s32 cmd, void* input, u32 inputLen, void* output, u32 outputLen);
IOSError IOS_IoctlvAsync(IOSFd fd, s32 cmd, u32 readCount, u32 writeCount, IOSIoVector* vect, IOSIpcCb cb, void* cbArg);
IOSError IOS_Ioctlv(IOSFd fd, s32 cmd, u32 readCount, u32 writeCount, IOSIoVector* vect);
IOSError IOS_IoctlvReboot(IOSFd fd, s32 cmd, u32 readCount, u32 writeCount, IOSIoVector* vect);

#ifdef __cplusplus
}
#endif

#endif // _REVOLUTION_IPCCLT_H_
