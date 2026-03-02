#ifndef RVL_SDK_WUD_H
#define RVL_SDK_WUD_H


#include <revolution/sc.h>
#include <revolution/wpad/bte.h>

#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

#define WUD_MAX_DEV_ENTRY_FOR_STD SC_MAX_DEV_ENTRY_FOR_STD
#define WUD_MAX_DEV_ENTRY_FOR_SMP SC_MAX_DEV_ENTRY_FOR_SMP
#define WUD_MAX_DEV_ENTRY SC_MAX_DEV_ENTRY
#define WUD_DEV_HANDLE_INVALID (-1)
#define WUD_ADDR_LEN BD_ADDR_LEN

// Forward declarations
typedef struct WUDDevInfo WUDDevInfo;

typedef enum {
    WUD_LIB_STATUS_0,
    WUD_LIB_STATUS_1,
    WUD_LIB_STATUS_2,
    WUD_LIB_STATUS_3,
    WUD_LIB_STATUS_4,
    WUD_LIB_STATUS_5,
} WUDLibStatus;

typedef enum {
    WUD_CHAN0,
    WUD_CHAN1,
    WUD_CHAN2,
    WUD_CHAN3,

    WUD_MAX_CHANNELS,
    WUD_CHAN_INVALID = -1
} WUDChannel;

typedef enum {
    WUD_SYNC_TYPE_STANDARD,
    WUD_SYNC_TYPE_SIMPLE,
} WUDSyncType;

typedef enum {
    WUD_RESULT_SYNC_BUSY = -1,
    WUD_RESULT_SYNC_WAITING,
    WUD_RESULT_SYNC_DONE,
} WUDSyncResult;

typedef void* (*WUDAllocFunc)(u32 size);
typedef BOOL (*WUDFreeFunc)(void* pBlock);

typedef void (*WUDSyncDeviceCallback)(s32 result, s32 num);
typedef void (*WUDClearDeviceCallback)(s32 result);

typedef void (*WUDHidConnCallback)(u8, u8);
typedef void (*WUDHidRecvCallback)(UINT8 devHandle, UINT8* pReport, UINT16 len);

typedef struct WUDDevInfo {
    SCDevInfo conf;    // at 0x0
    BD_ADDR devAddr;   // at 0x40
    LINK_KEY linkKey;  // at 0x46
    UINT8 devHandle;   // at 0x56
    UINT8 subclass;    // at 0x57
    UINT8 appID;       // at 0x58
    u8 status;         // at 0x59
    s8 UNK_0x5A;       // at 0x5A
    u8 sync_type;      // at 0x5B
    u8 UNK_0x5C;       // at 0x5C
    u8 UNK_0x5D[1];
    tBTA_HH_ATTR_MASK hhAttrMask;  // at 0x5E
} WUDDevInfo;

BOOL WUDInit(void);
BOOL WUDIsBusy(void);
void WUDRegisterAllocator(WUDAllocFunc pAllocFunc, WUDFreeFunc pFreeFunc);
u32 WUDGetAllocatedMemSize(void);
void WUDShutdown(void);

WUDLibStatus WUDGetStatus(void);
u8 WUDGetBufferStatus(void);

void WUDSetSniffMode(BD_ADDR addr, s32 interval);

// clang-format off
WUDSyncDeviceCallback
WUDSetSyncSimpleCallback(WUDSyncDeviceCallback pCallback);

WUDClearDeviceCallback
WUDSetClearDeviceCallback(WUDClearDeviceCallback pCallback);
// clang-format on

BOOL WUDStartSyncDevice(void);
BOOL WUDStartFastSyncSimple(void);
BOOL WUDStartSyncSimple(void);
BOOL WUDStopSyncSimple(void);
BOOL WUDCancelSyncDevice(void);

BOOL WUDStartClearDevice(void);
BOOL WUDSetDisableChannel(s8 afhChannel);

WUDHidRecvCallback WUDSetHidRecvCallback(WUDHidRecvCallback pCallback);
WUDHidConnCallback WUDSetHidConnCallback(WUDHidConnCallback pCallback);

void WUDSetVisibility(u8 disc, u8 conn);
u8 WUDGetConnectable(void);

void WUDiGetFirmwareVersion(void);
void WUDiInitSub(void);

void WUDiEnableStack(void);
void WUDiAutoSync(void);
void WUDiDeleteAllLinkKeys(void);

void WUDiRegisterDevice(BD_ADDR addr);
void WUDiRemoveDevice(BD_ADDR addr);

WUDDevInfo* WUDiGetDevInfo(BD_ADDR addr);
WUDDevInfo* WUDiGetDevInfoIndex(int idx);
WUDDevInfo* WUDiGetNewDevInfo(void);
void WUDiRemoveDevInfo(BD_ADDR addr);
void WUDiClearDevice(void);

u8 WUDiGetDevNumber(void);
u8 WUDiGetDevSmpNumber(void);
u8 WUDiGetConnNumber(void);
u8 WUDiGetLinkNum(void);

void WUDiMoveTopSmpDevInfoPtr(WUDDevInfo* pInfo);
void WUDiMoveBottomSmpDevInfoPtr(WUDDevInfo* pInfo);
WUDDevInfo* WUDiGetRemoveSmpDevice(void);
void WUDiMoveTopOfDisconnectedSmpDevice(WUDDevInfo* pInfo);

void WUDiMoveTopStdDevInfoPtr(WUDDevInfo* pInfo);
void WUDiMoveBottomStdDevInfoPtr(WUDDevInfo* pInfo);
WUDDevInfo* WUDiGetRemoveStdDevice(void);
void WUDiMoveTopOfDisconnectedStdDevice(WUDDevInfo* pInfo);

BD_ADDR_PTR _WUDGetDevAddr(UINT8 handle);
u16 _WUDGetQueuedSize(s8 dev_handle);
u16 _WUDGetNotAckedSize(s8 dev_handle);
u8 _WUDGetLinkNumber(void);

#ifdef __cplusplus
}
#endif

#endif
