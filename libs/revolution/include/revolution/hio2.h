#ifndef _REVOLUTION_HIO2_H_
#define _REVOLUTION_HIO2_H_

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HIO2_DEVICE_INVALID = -1,
    HIO2_DEVICE_UNK_0 = 0,
    HIO2_DEVICE_UNK_1 = 1,
    HIO2_DEVICE_UNK_2 = 2,
} HIO2DeviceType;

typedef BOOL (*HIO2EnumCallback)(HIO2DeviceType);
typedef void (*HIO2UnkCallback)(s32);
typedef void (*HIO2DisconnectCallback)(s32);

typedef struct HIO2Control {
    /* 0x00 */ HIO2DeviceType type;
    /* 0x04 */ s32 chan;
    /* 0x08 */ u32 _0x8;
    /* 0x0C */ HIO2UnkCallback exiCallback;
    /* 0x10 */ HIO2UnkCallback _0x10;
    /* 0x14 */ HIO2UnkCallback _0x14;
    /* 0x18 */ HIO2DisconnectCallback disconnectCb;
} HIO2Control; // size 0x1C


BOOL HIO2Init(void);
BOOL HIO2EnumDevices(HIO2EnumCallback callback);
s32 HIO2Open(HIO2DeviceType type, HIO2UnkCallback exiCb, HIO2DisconnectCallback disconnectCb);
BOOL HIO2Close(s32 handle);
BOOL HIO2Read(s32 handle, u32 addr, void* buffer, s32 size);
BOOL HIO2Write(s32 handle, u32 addr, void* buffer, s32 size);
void HIO2Exit(void);

#ifdef __cplusplus
}
#endif

#endif
