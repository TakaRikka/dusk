#ifndef _REVOLUTION_WUD_PRIVATE_H_
#define _REVOLUTION_WUD_PRIVATE_H_

#include <revolution/os.h>
#include <revolution/sc.h>
#include <revolution/wpad/bte.h>
#include <revolution/wud.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WUD_BDCMP(LHS, RHS) memcmp(LHS, RHS, BD_ADDR_LEN)
#define WUD_BDCPY(DST, SRC) memcpy(DST, SRC, BD_ADDR_LEN)

// clang-format off
#define WUD_DEV_NAME_IS(NAME, VALUE)                                           \
    (memcmp(NAME, VALUE, sizeof(VALUE) - 1) == 0)

#define WUD_DEV_NAME_IS_CNT(NAME)                                              \
    WUD_DEV_NAME_IS(NAME, "Nintendo RVL-CNT")

#define WUD_DEV_NAME_IS_CNT_01(NAME)                                           \
    WUD_DEV_NAME_IS(NAME, "Nintendo RVL-CNT-01")
// clang-format on

typedef enum {
    WUD_STATE_SYNC_START = 0,
    WUD_STATE_SYNC_PREPARE_SEARCH = 1,
    WUD_STATE_SYNC_START_SEARCH = 2,
    WUD_STATE_SYNC_WAIT_FOR_SEARCH_RESULT = 3,
    WUD_STATE_SYNC_CHECK_SEARCH_RESULT = 4,
    WUD_STATE_SYNC_IS_EXISTED_DEVICE = 5,
    WUD_STATE_SYNC_6 = 6,
    WUD_STATE_SYNC_STORED_LINK_KEY_TO_EEPROM = 7,
    WUD_STATE_SYNC_WAIT_FOR_STORING = 8,
    WUD_STATE_SYNC_WAIT_FOR_READING = 10,
    WUD_STATE_SYNC_11 = 11,
    WUD_STATE_SYNC_12 = 12,
    WUD_STATE_SYNC_13 = 13,
    WUD_STATE_SYNC_DONE = 14,
    WUD_STATE_SYNC_TRY_CONNECT = 15,
    WUD_STATE_SYNC_PREPARE_FOR_EXISTED_DEVICE = 16,
    WUD_STATE_SYNC_PREPARE_FOR_UNKNOWN_DEVICE = 17,
    WUD_STATE_SYNC_REGISTER_DEVICE = 18,
    WUD_STATE_SYNC_VIRGIN_SIMPLE = 19,
    WUD_STATE_SYNC_VIRGIN_STANDARD = 20,
    WUD_STATE_SYNC_CHANGE_SIMPLE_TO_STANDARD = 21,
    WUD_STATE_SYNC_STORED_DEV_INFO_TO_NAND = 22,
    WUD_STATE_SYNC_COMPLETE = 23,
    WUD_STATE_SYNC_WAIT_FOR_INCOMING = 24,
    WUD_STATE_SYNC_SC_FLUSH = 25,
    WUD_STATE_SYNC_CANCEL_SEARCH = 26,
    WUD_STATE_SYNC_WAIT_FOR_START_SEARCH = 29,
    WUD_STATE_SYNC_ERROR = 255,
} WUDSyncState;

typedef enum {
    WUD_STATE_DELETE_START = 0,
    WUD_STATE_DELETE_DISALLOW_INCOMING = 1,
    WUD_STATE_DELETE_DISCONNECT_ALL = 2,
    WUD_STATE_DELETE_CLEANUP_DATABASE = 3,
    WUD_STATE_DELETE_CLEANUP_SETTING = 5,
    WUD_STATE_DELETE_6 = 6,
    WUD_STATE_DELETE_7 = 7,
    WUD_STATE_DELETE_DONE = 8
} WUDDeleteState;

typedef enum {
    WUD_RESULT_DELETE_BUSY = -1,
    WUD_RESULT_DELETE_WAITING,
    WUD_RESULT_DELETE_COMPLETE,
} WUDDeleteResult;

typedef enum {
    WUD_STATE_LINK_KEY_START = 0,
    WUD_STATE_LINK_KEY_READING = 1,
    WUD_STATE_LINK_KEY_WRITING = 2,
    WUD_STATE_LINK_KEY_DELETING = 3,
    WUD_STATE_LINK_KEY_ERROR = 255,
} WUDLinkKeyState;

typedef enum {
    WUD_STATE_STACK_START = 0,
    WUD_STATE_STACK_GET_STORED_LINK_KEY = 1,
    WUD_STATE_STACK_CHECK_DEVICE_INFO = 2,
    WUD_STATE_STACK_DONE = 3,
    WUD_STATE_STACK_INITIALIZED = 4,
    WUD_STATE_STACK_ERROR = 255,
} WUDStackState;

typedef enum {
    WUD_STATE_INIT_START = 0,
    WUD_STATE_INIT_WAIT_FOR_INITIALIZATION = 1,
    WUD_STATE_INIT_GET_DEV_INFO = 2,
    WUD_STATE_INIT_DONE = 3,
    WUD_STATE_INIT_INITIALIZED = 4,
    WUD_STATE_INIT_ERROR = 255,
} WUDInitState;

typedef enum {
    WUD_STATE_SHUTDOWN_START = 0,
    WUD_STATE_SHUTDOWN_STORE_SETTINGS = 1,
    WUD_STATE_SHUTDOWN_FLUSH_SETTINGS = 2,
    WUD_STATE_SHUTDOWN_DONE = 3,
    WUD_STATE_SHUTDOWN_ERROR = 255,
} WUDShutdownState;

typedef enum {
    WUD_VSE_INITIATE_PAIRING = 8,
    WUD_VSE_DELETE_ALL_KEYS,
    WUD_VSE_SI_PORT_STATUS,
    WUD_VSE_WATCH_DOG_RESET_HW = 16,
} WUDVendorSpecificEvent;

typedef struct WUDDevInfoList {
    /* 0x00 */ WUDDevInfo* devInfo;
    /* 0x04 */ struct WUDDevInfoList* prev;
    /* 0x08 */ struct WUDDevInfoList* next;
} WUDDevInfoList;

typedef struct WUDDiscResp {
    /* 0x000 */ BD_ADDR devAddr;
    /* 0x006 */ char devName[64];
    /* 0x046 */ u8 unk_0x46[0x100 - 0x46];
    /* 0x100 */ tBTA_SERVICE_MASK services;
    /* 0x104 */ u8 unk_0x104[0x104 - 0x100];
} WUDDiscResp;

typedef struct WUDPatchCmd {
    /* 0x00 */ u8 data[13];
} WUDPatchCmd;

typedef struct WUDPatchList {
    /* 0x00 */ u8 num;
    /* 0x01 */ WUDPatchCmd cmds[];
} WUDPatchList;

#define WUD_PATCH_BUFFER_SIZE 0xFF
#define WUD_MAX_PATCHES (WUD_PATCH_BUFFER_SIZE / (s32)sizeof(WUDPatchCmd))

typedef struct WUDCB {
    /* 0x000 */ WUDSyncDeviceCallback syncStdCB;
    /* 0x004 */ WUDSyncDeviceCallback syncSmpCB;
    /* 0x008 */ WUDClearDeviceCallback clearDevCB;

    /* 0x00C */ u8 syncState;
    /* 0x00D */ u8 deleteState;
    /* 0x00E */ u8 linkKeyState;
    /* 0x00F */ u8 stackState;
    /* 0x010 */ u8 initState;
    /* 0x011 */ u8 shutdownState;

    /* 0x012 */ u8 devNums;
    /* 0x013 */ u8 devSmpNums;

    /* 0x014 */ WUDDevInfoList* smpListHead;
    /* 0x018 */ WUDDevInfoList* smpListTail;
    /* 0x01C */ WUDDevInfoList smpList[WUD_MAX_DEV_ENTRY_FOR_SMP];

#if SDK_AUG2010
    /* 0x064 */ WUDDevInfoList* stdListHead;
    /* 0x068 */ WUDDevInfoList* stdListTail;
    /* 0x06C */ WUDDevInfoList stdList[WUD_MAX_DEV_ENTRY_FOR_STD];
#endif

    /* 0x064 */ WUDDevInfo stdDevs[WUD_MAX_DEV_ENTRY_FOR_STD];
    /* 0x424 */ WUDDevInfo smpDevs[WUD_MAX_DEV_ENTRY_FOR_SMP];

    /* 0x664 */ u8 connectedNum;
    /* 0x665 */ u8 linkedNum;
    /* 0x666 */ u8 syncedNum;

    /* 0x667 */ u8 syncSkipChecks;
    /* 0x668 */ s8 syncLoopNum;
    /* 0x669 */ u8 syncType;

    /* 0x66A */ u8 connectable;
    /* 0x66B */ u8 discoverable;

    /* 0x66C */ WUDHidRecvCallback hidRecvCB;
    /* 0x670 */ WUDHidConnCallback hidConnCB;

    /* 0x674 */ WUDAllocFunc allocFunc;
    /* 0x678 */ WUDFreeFunc freeFunc;

    /* 0x67C */ BD_ADDR pairAddr;
    /* 0x682 */ BD_ADDR hostAddr;

    /* 0x688 */ s8 libStatus;

    /* 0x689 */ u8 serialPortStatus;
    /* 0x68A */ UINT8 pmID;
    /* 0x68B */ s8 syncRssi;
    /* 0x670 */ OSAlarm alarm;
    /* 0x6C0 */ u32 hhFlags;

    /* 0x6C4 */ u16 bufferStatus0;
    /* 0x6C6 */ u16 bufferStatus1;
#if SDK_AUG2010
    /* 0x748 */ u8 unk_0x748[0x750 - 0x748];
#endif
} WUDControlBlock;

#if SDK_AUG2010
extern WUDControlBlock __rvl_wudcb;
#define _wcb __rvl_wudcb
#else
extern WUDControlBlock _wcb;
#endif
extern WUDDevInfo _work;

extern SCBtDeviceInfoArray _scArray;

extern BD_ADDR_PTR _dev_handle_to_bda[WUD_MAX_DEV_ENTRY];
extern u16 _dev_handle_queue_size[WUD_MAX_DEV_ENTRY];
extern u16 _dev_handle_notack_num[WUD_MAX_DEV_ENTRY];

#if SDK_AUG2010
u16 WUDiGetQueueSizeForHandle(u8 dev_handle);
u16 WUDiGetNotAckNumForHandle(u8 dev_handle);
#else
u16 WUDiGetQueueSizeForHandle(u32 dev_handle);
u16 WUDiGetNotAckNumForHandle(u32 dev_handle);
#endif

/* wud/WUDHidHost.c */
void WUDHidHostCallback(tBTA_HH_EVT event, tBTA_HH *p_data);

/* wud/debug_msg.c */

void WUD_DEBUGPrint(const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif  /* _REVOLUTION_WUD_PRIVATE_H_ */
