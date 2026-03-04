#ifndef _REVOLUTION_IPCMEMORY_H_
#define _REVOLUTION_IPCMEMORY_H_

#include <revolution/private/iostypes.h>

#ifdef __cplusplus
extern "C" {
#endif

IOSError iosFree(IOSHeapId id, void* ptr);
void* iosAllocAligned(IOSHeapId id, u32 size, u32 alignment);
IOSHeapId iosCreateHeap(void* ptr, u32 size);

#ifdef __cplusplus
}
#endif

#endif // _REVOLUTION_IPCMEMORY_H_
