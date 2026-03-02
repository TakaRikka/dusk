#ifndef _DOLPHIN_HIO_H_
#define _DOLPHIN_HIO_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*HIOCallback)(void);
typedef BOOL (*HIOEnumCallback)(s32 chan);

BOOL HIOEnumDevices(HIOEnumCallback callback);
BOOL HIOInit(s32 chan, HIOCallback callback);
BOOL HIOInitEx(s32 chan, u32 dev, HIOCallback callback);
BOOL HIOReadMailbox(u32* word);
BOOL HIOWriteMailbox(u32 word);
BOOL HIORead(u32 addr, void* buffer, s32 size);
BOOL HIOWrite(u32 addr, void* buffer, s32 size);
BOOL HIOReadAsync(u32 addr, void* buffer, s32 size, HIOCallback callback);
BOOL HIOWriteAsync(u32 addr, void* buffer, s32 size, HIOCallback callback);
BOOL HIOReadStatus(u32* status);

#ifdef __cplusplus
}
#endif

#endif
