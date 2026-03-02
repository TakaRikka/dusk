#ifndef _REVOLUTION_WUD_H_
#define _REVOLUTION_WUD_H_

#include <revolution/types.h>
#include <revolution/wpad/bte.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WUD_CHAN0         0
#define WUD_CHAN1         1
#define WUD_CHAN2         2
#define WUD_CHAN3         3
#define WUD_MAX_CHANNELS  4

#define WUD_CHAN_INVALID  -1

typedef void* WUDAllocFunc(u32 size);
typedef int WUDFreeFunc(void* ptr);  // signature from petari. not sure what the int is, though

typedef void WUDSyncDeviceCallback(s32 result, s32 num);  // what is result?
typedef void WUDClearDeviceCallback(signed);

typedef struct WUDDevInfo WUDDevInfo;
typedef void WUDHidConnectCallback(WUDDevInfo* devInfo, u8 isOpenEvent);
typedef void WUDHidReceiveCallback(u8 dev_handle, u8* p_rpt, u16 len);

struct WUDDevInfo {
    /* 0x00 */ struct small_dev_info small;
    /* 0x40 */ BD_ADDR devAddr;
    /* 0x46 */ LINK_KEY linkKey;
    /* 0x56 */ u8 devHandle;
    /* 0x57 */ u8 subclass;
    /* 0x58 */ u8 appID;
    /* 0x59 */ u8 at_0x59;
    /* 0x5a */ u8 at_0x5a; /* unknown */
    /* 0x5b */ u8 at_0x5b;
    /* 0x5c */ u8 at_0x5c;
    u8 at_0x5d[1]; // padding?
    /* 0x5e */ tBTA_HH_ATTR_MASK hhAttrMask;
}; // size 0x60

u8 WUDGetBufferStatus(void);
u8 _WUDGetLinkNumber(void);
u16 _WUDGetQueuedSize(s8 dev_handle);
u16 _WUDGetNotAckedSize(s8 dev_handle);

#ifdef __cplusplus
}
#endif

#endif  // _REVOLUTION_WUD_H_
