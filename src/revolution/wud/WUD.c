#include <revolution/wud.h>
#include <revolution/wud/__wud.h>

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <mem.h>
#include <revolution/os.h>
#include <revolution/sc.h>
#include <revolution/wpad.h>
#include <revolution/wpad/bte.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CLAMP(val, min, max) ((val) > (max) ? (max) : (val) < (min) ? (min) : (val))

#define DEBUGPrint WUD_DEBUGPrint

#define BT_VSC_NINTENDO_INSTALL_PATCH 0xFC4F
#define BT_VSC_NINTENDO_WRITE_PATCH 0xFC4C

void WUDSecurityCallback(tBTA_DM_SEC_EVT event, tBTA_DM_SEC* pData);
void WUDSearchCallback(tBTA_DM_SEARCH_EVT event, tBTA_DM_SEARCH* pData);
void WUDVendorSpecificCallback(UINT8 len, UINT8* pData);
void WUDDeviceStatusCallback(tBTM_DEV_STATUS status);
void WUDStoredLinkKeyCallback(void* p1);
void WUDPowerManagerCallback(BD_ADDR addr, tBTM_PM_STATUS status, UINT16 value, UINT8 hciStatus);

//static void InitCore(void);

static void WUDiRemovePatch(void);
static void WUDiWritePatch(void);
static void WUDiInstallPatch(void);

#if SDK_AUG2010
WUDControlBlock __rvl_wudcb;
#else
WUDControlBlock _wcb;
#endif
WUDDevInfo _work;
static WUDDiscResp _discResp;
SCBtDeviceInfoArray _scArray;
u8 __WUDHandlerStack[0x1000] ATTRIBUTE_ALIGN(32);

extern u8 _scFlush;

BD_ADDR_PTR _dev_handle_to_bda[WUD_MAX_DEV_ENTRY];
u16 _dev_handle_queue_size[WUD_MAX_DEV_ENTRY];
u16 _dev_handle_notack_num[WUD_MAX_DEV_ENTRY];

static BOOL _initialized = FALSE;
static u8 __bte_trace_level = 0;

static OSAlarm _arm;

static u8 _discNumResps;
static s8 _discRssi;

// clang-format off
static u8 descriptor[] = {
    0x05, 0x01, // Usage Page (Generic Desktop Ctrls)
    0x09, 0x05, // Usage (Game Pad)

    0xA1, 0x01, // Collection (Application)

    0x85, 0x10,       // Report ID (16)
    0x15, 0x00,       // Logical Minimum (0)
    0x26, 0xFF, 0x00, // Logical Maximum (255)
    0x75, 0x08,       // Report Size (8)
    0x95, 0x01,       // Report Count (1)
    0x06, 0x00, 0xFF, // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x01,       // Usage (0x01)
    0x91, 0x00,       // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x11, // Report ID (17)
    0x95, 0x01, // Report Count (1)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x12, // Report ID (18)
    0x95, 0x02, // Report Count (2)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x13, // Report ID (19)
    0x95, 0x01, // Report Count (1)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x14, // Report ID (20)
    0x95, 0x01, // Report Count (1)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x15, // Report ID (21)
    0x95, 0x01, // Report Count (1)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x16, // Report ID (22)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x17, // Report ID (23)
    0x95, 0x06, // Report Count (6)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x18, // Report ID (24)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x19, // Report ID (25)
    0x95, 0x01, // Report Count (1)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x1A, // Report ID (26)
    0x95, 0x01, // Report Count (1)
    0x09, 0x01, // Usage (0x01)
    0x91, 0x00, // Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0x85, 0x20, // Report ID (32)
    0x95, 0x06, // Report Count (6)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x21, // Report ID (33)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x22, // Report ID (34)
    0x95, 0x04, // Report Count (4)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x30, // Report ID (48)
    0x95, 0x02, // Report Count (2)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x31, // Report ID (49)
    0x95, 0x05, // Report Count (5)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x32, // Report ID (50)
    0x95, 0x0A, // Report Count (10)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x33, // Report ID (51)
    0x95, 0x11, // Report Count (17)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x34, // Report ID (52)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x35, // Report ID (53)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x36, // Report ID (54)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x37, // Report ID (55)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x3D, // Report ID (61)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x3E, // Report ID (62)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0x85, 0x3F, // Report ID (63)
    0x95, 0x15, // Report Count (21)
    0x09, 0x01, // Usage (0x01)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0xC0,       // End Collection
};
// clang-format on

void* App_MEMalloc(u32 size) {
    DEBUGPrint("App_MEMalloc\n");
    return _wcb.allocFunc(size);
}

BOOL App_MEMfree(void* pBlock) {
    DEBUGPrint("App_MEMfree\n");
    return _wcb.freeFunc(pBlock);
}

static void ReverseAddr(BD_ADDR_PTR pDst, BD_ADDR src) {
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < BD_ADDR_LEN; i++) {
        pDst[i] = src[BD_ADDR_LEN - 1 - i];
    }

    OSRestoreInterrupts(enabled);
}

static void SyncFlushCallback(u32 status) {
    WUDControlBlock* p = &_wcb;

    DEBUGPrint("SyncFlushCallback() : %d, Sync: %d\n", status, _wcb.syncState);

    if (p->syncState == WUD_STATE_SYNC_START) {
        return;
    }

    if (status == SC_STATUS_OK) {
        p->syncState = WUD_STATE_SYNC_COMPLETE;
    } else {
        p->syncState = WUD_STATE_SYNC_ERROR;
    }
}

static void DeleteFlushCallback(u32 status) {
    WUDControlBlock* p = &_wcb;

    DEBUGPrint("DeleteFlushCallback() : %d, Delete: %d\n", status, _wcb.deleteState);

    if (p->deleteState == WUD_STATE_DELETE_START) {
        return;
    }

    if (status == SC_STATUS_OK) {
        p->deleteState = WUD_STATE_DELETE_DONE;
    } else {
        p->deleteState = WUD_STATE_DELETE_DONE;
    }
}

static void ShutFlushCallback(u32 status) {
    WUDControlBlock* p = &_wcb;

    DEBUGPrint("ShutFlushCallback() : %d, Shutdown: %d\n", status, _wcb.shutdownState);

    OSCancelAlarm(&_wcb.alarm);
    p->shutdownState = WUD_STATE_SHUTDOWN_DONE;
    BTA_DisableBluetooth();
}

static void ClearDiscoverResult(void) {
    _discNumResps = 0;
    memset(&_discResp, 0, sizeof(WUDDiscResp));
}

static void _resumeSmpSync(OSAlarm* pAlarm, OSContext* pContext) {
#pragma unused(pAlarm)
#pragma unused(pContext)

    _wcb.syncState = WUD_STATE_SYNC_PREPARE_SEARCH;
}

static void WUDiWaitForIncomingConnection(void) {
    u32 waitSec = WUDiGetLinkNum() == WUD_MAX_CHANNELS - 1 ? 4 : 2;

    if (WUDiGetLinkNum() == WUD_MAX_CHANNELS && WUDiGetConnNumber() == WUD_MAX_CHANNELS) {
        _wcb.syncState = WUD_STATE_SYNC_DONE;
    }

    OSCreateAlarm(&_arm);
    OSSetAlarm(&_arm, OSSecondsToTicks(waitSec), _resumeSmpSync);
}

static u8 WUDiIsSyncDisabled(void) {
    WUDControlBlock* p = &_wcb;
    int i;
    WUDDevInfo* pInfo;
    u8 waitSecs;

    if (p->syncLoopNum == 0) {
        return WUD_STATE_SYNC_DONE;
    }

    if (WUDiGetLinkNum() == WUD_MAX_CHANNELS && WUDiGetConnNumber() == WUD_MAX_CHANNELS) {
        return WUD_STATE_SYNC_DONE;
    }

    if (p->syncSkipChecks) {
        for (i = 0; i < WUD_MAX_DEV_ENTRY; i++) {
            pInfo = WUDiGetDevInfoIndex(i);

            if (pInfo->status == 9) {
                WUDSetSniffMode(pInfo->devAddr, 0);
                return WUD_STATE_SYNC_PREPARE_SEARCH;
            }
        }
    }

    if (p->syncLoopNum > 0) {
        p->syncLoopNum--;
    }

    if (p->syncLoopNum == -1) {
        p->syncLoopNum = -2;
        waitSecs = WUDiGetLinkNum() == 3 ? 4 : 2;

        if (WUDiGetLinkNum() == 4 && WUDiGetConnNumber() == 4) {
            _wcb.syncState = WUD_STATE_SYNC_DONE;
        }

        OSCreateAlarm(&_arm);
        OSSetAlarm(&_arm, OSSecondsToTicks(waitSecs), _resumeSmpSync);
        return 0x18U;
    }

    return WUD_STATE_SYNC_START_SEARCH;
}

static u8 WUDiStartSearch(void) {
    WUDControlBlock* p = &_wcb;
    tBTA_DM_INQ dm_inq;

    dm_inq.mode = BTM_LIMITED_INQUIRY;
    dm_inq.max_resps = 1;
    dm_inq.report_dup = FALSE;

    if (p->syncSkipChecks) {
        dm_inq.duration = 5 - 2;
    } else if (p->syncType == WUD_SYNC_TYPE_STANDARD) {
        dm_inq.duration = WUDiGetLinkNum() == WUD_MAX_CHANNELS - 1 ? 10 : 5;
        dm_inq.duration -= p->syncLoopNum;
    } else {
        dm_inq.duration = WUDiGetLinkNum() == WUD_MAX_CHANNELS - 1 ? 10 - 2 : 5 - 2;
    }

    ClearDiscoverResult();
    BTA_DmSearch(&dm_inq, 0, &WUDSearchCallback);

    return WUD_STATE_SYNC_WAIT_FOR_SEARCH_RESULT;
}

static u8 WUDiWaitForSearchResult(void) {
    if (WUDiGetLinkNum() == WUD_MAX_CHANNELS && WUDiGetConnNumber() == WUD_MAX_CHANNELS) {
        DEBUGPrint("Cancel searching because 4 connections exist.\n");
        BTA_DmSearchCancel();

        return WUD_STATE_SYNC_CANCEL_SEARCH;
    }

    return WUD_STATE_SYNC_WAIT_FOR_SEARCH_RESULT;
}

static u8 WUDiNextStepBySearchResult(void) {
    WUDControlBlock* p = &_wcb;
    WUDSyncState nextState = WUD_STATE_SYNC_PREPARE_SEARCH;

    if (_discNumResps > 0) {
        if (WUD_DEV_NAME_IS_CNT(_discResp.devName)) {
            if (_discRssi > p->syncRssi) {
                nextState = WUD_STATE_SYNC_IS_EXISTED_DEVICE;
            }
        }
    } else {
        WUDiWaitForIncomingConnection();
        nextState = WUD_STATE_SYNC_WAIT_FOR_INCOMING;
    }

    return nextState;
}

static u8 WUDiCheckDeviceDataBase(void) {
    WUDDevInfo* pInfo;
    WUDSyncState nextState = WUD_STATE_SYNC_PREPARE_FOR_UNKNOWN_DEVICE;

    pInfo = WUDiGetDevInfo(_discResp.devAddr);

    if (pInfo != NULL) {
        memcpy(&_work, pInfo, sizeof(WUDDevInfo));
        nextState = WUD_STATE_SYNC_PREPARE_FOR_EXISTED_DEVICE;
    }

    return nextState;
}

static u8 WUDiExistedDevice(void) {
    WUDDevInfo* pWork = &_work;

    pWork->status = 1;

    switch (pWork->sync_type) {
    case 0:
    case 2:
    case 4:
    case 5: {
        if (_wcb.syncType == WUD_SYNC_TYPE_STANDARD) {
            WUDiRemoveDevice(pWork->devAddr);
            return WUD_STATE_SYNC_PREPARE_FOR_UNKNOWN_DEVICE;
        } else {
            pWork->sync_type = 4;
        }

        break;
    }

    case 1:
    case 3: {
        WUDiMoveBottomSmpDevInfoPtr(pWork);
        WUDiRemoveDevice(_wcb.smpListTail->devInfo->devAddr);
        return WUD_STATE_SYNC_PREPARE_FOR_UNKNOWN_DEVICE;
    }
    }

    return WUD_STATE_SYNC_TRY_CONNECT;
}

static u8 WUDiUnknownDevice(void) {
    WUDDevInfo* pWork = &_work;

    pWork->status = 1;
    pWork->sync_type = _wcb.syncType == WUD_SYNC_TYPE_STANDARD ? 0 : 1;

    WUD_BDCPY(pWork->devAddr, _discResp.devAddr);
    memcpy(pWork->conf.devName, _discResp.devName, sizeof(_discResp.devName));
    memset(pWork->linkKey, 0, LINK_KEY_LEN);

    return WUD_STATE_SYNC_TRY_CONNECT;
}

static u8 WUDiTryConnecting(void) {
    WUDSyncState nextState = WUD_STATE_SYNC_ERROR;

    if (WUD_DEV_NAME_IS_CNT(_discResp.devName)) {
        _work.status = 2;
        BTA_HhOpen(_work.devAddr, BTA_HH_PROTO_RPT_MODE, BTA_SEC_AUTHENTICATE);
        nextState = WUD_STATE_SYNC_6;
    }

    return nextState;
}

static int WUDiSetPinCode(BD_ADDR addr) {
    BD_ADDR pin;
    BD_ADDR_PTR pAddr;

    pAddr = _wcb.syncType == WUD_SYNC_TYPE_STANDARD ? _wcb.hostAddr : addr;

    _work.UNK_0x5A = 1;

    ReverseAddr(pin, pAddr);
    BTA_DmPinReply(addr, TRUE, BD_ADDR_LEN, pin);

    return 0;
}

static BOOL WUDiIsFinishLinkKeyCmd(void) {
    return _wcb.linkKeyState == WUD_STATE_LINK_KEY_START ? TRUE : FALSE;
}

static BOOL WUDiIsFinishScCmd(void) {
    return SCCheckStatus() != SC_STATUS_BUSY ? TRUE : FALSE;
}

static WUDDevInfo* WUDiGetDevInfoFromIndex(s32 index) {
    WUDDevInfo* result;
    BOOL irq_enabled;

    irq_enabled = OSDisableInterrupts();

    if (index >= 0 && index < WUD_MAX_DEV_ENTRY_FOR_STD) {
        result = &_wcb.stdDevs[index];
    } else {
        result = &_wcb.smpDevs[index - WUD_MAX_DEV_ENTRY_FOR_STD];
    }

    OSRestoreInterrupts(irq_enabled);

    return result;
}

static u8 WUDiSaveDeviceToNand(void) {
    WUDSyncState nextState = WUD_STATE_SYNC_COMPLETE;
    WUDDevInfoList* pIt;
    WUDDevInfo* devInfo;
    u8 i;
    u8 storedCount;

    if (!WUDiIsFinishScCmd()) {
        return WUD_STATE_SYNC_STORED_DEV_INFO_TO_NAND;
    }

    memset(&_scArray.regist, 0, sizeof(SCBtDeviceInfo) * WUD_MAX_DEV_ENTRY_FOR_STD);

    _scArray.num = WUDiGetDevNumber();

    for (i = 0, storedCount = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        devInfo = WUDiGetDevInfoFromIndex(i);
        if (devInfo->status != 0) {
            if (devInfo->sync_type == 0 || devInfo->sync_type == 2 || devInfo->sync_type == 4) {
                memcpy(&_scArray.regist[storedCount].addr, devInfo->devAddr, sizeof(BD_ADDR));
                memcpy(&_scArray.regist[storedCount].info, devInfo->conf.devName, sizeof(SCDevInfo));
                storedCount++;
            }
        }
    }

    DEBUGPrint("%d devices is stored into SC.\n", storedCount);

    if (!SCSetBtDeviceInfoArray(&_scArray)) {
        nextState = WUD_STATE_SYNC_STORED_DEV_INFO_TO_NAND;
    } else {
        nextState = WUD_STATE_SYNC_SC_FLUSH;
    }

    return nextState;
}

static void WUDiSaveFlush(void) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    p->syncState = WUD_STATE_SYNC_13;
    OSRestoreInterrupts(enabled);

    SCFlushAsync(SyncFlushCallback);
}

static u8 WUDiVirginSimpleSync(void) {
    WUDDevInfo* pInfo;

    if (WUDiGetDevSmpNumber() == WUD_MAX_DEV_ENTRY_FOR_SMP) {
        pInfo = WUDiGetRemoveSmpDevice();
        WUDiRemoveDevice(pInfo->devAddr);
    }

    pInfo = WUDiGetNewDevInfo();
    memcpy(pInfo, &_work, sizeof(WUDDevInfo));

    WUDiRegisterDevice(pInfo->devAddr);
    WUDiMoveTopSmpDevInfoPtr(pInfo);

    _dev_handle_to_bda[pInfo->devHandle] = pInfo->devAddr;

    return WUD_STATE_SYNC_COMPLETE;
}

static u8 WUDiVirginStandardSync(void) {
    WUDControlBlock* p = &_wcb;
    WUDDevInfo* pInfo;

    if (WUDiGetDevNumber() == WUD_MAX_DEV_ENTRY_FOR_STD) {
        p->linkKeyState = 1;
        BTM_ReadStoredLinkKey(0, WUDStoredLinkKeyCallback);

        return WUD_STATE_SYNC_WAIT_FOR_READING;
    } else {
        pInfo = WUDiGetNewDevInfo();
        memcpy(pInfo, &_work, sizeof(WUDDevInfo));

        WUDiRegisterDevice(pInfo->devAddr);

        _dev_handle_to_bda[pInfo->devHandle] = pInfo->devAddr;

        return WUD_STATE_SYNC_STORED_LINK_KEY_TO_EEPROM;
    }

}

static u8 WUDiChangeSimpleToStandard(void) {
    WUDiMoveBottomSmpDevInfoPtr(&_work);
    WUDiRemoveDevice(_wcb.smpListTail->devInfo->devAddr);
    return WUDiVirginStandardSync();
}

static u8 WUDiRegisterSyncDevice(void) {
    WUDSyncState nextState;

    switch (_work.sync_type) {
    case 1: {
        nextState = WUD_STATE_SYNC_VIRGIN_SIMPLE;
        break;
    }

    case 0: {
        nextState = WUD_STATE_SYNC_VIRGIN_STANDARD;
        break;
    }

    case 5: {
        nextState = WUD_STATE_SYNC_CHANGE_SIMPLE_TO_STANDARD;
        break;
    }

    case 3: {
        nextState = WUD_STATE_SYNC_COMPLETE;
        break;
    }

    case 4: {
        nextState = WUD_STATE_SYNC_STORED_LINK_KEY_TO_EEPROM;
        break;
    }

    case 2: {
        nextState = WUD_STATE_SYNC_STORED_LINK_KEY_TO_EEPROM;
        break;
    }

    default: {
        // UB
        break;
    }
    }

    return nextState;
}

static u8 WUDiStoredLinkKey(void) {
    WUDControlBlock* p = &_wcb;
    WUDDevInfo* pInfo;

    if (!WUDiIsFinishLinkKeyCmd()) {
        return WUD_STATE_SYNC_STORED_LINK_KEY_TO_EEPROM;
    }

    pInfo = WUDiGetDevInfo(_work.devAddr);

    // clang-format off
    DEBUGPrint("write stored link key\n");

    DEBUGPrint("addr : %02x:%02x:%02x:%02x:%02x:%02x\n",
               pInfo->devAddr[0], pInfo->devAddr[1], pInfo->devAddr[2],
               pInfo->devAddr[3], pInfo->devAddr[4], pInfo->devAddr[5]);

    DEBUGPrint("key  : %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
               pInfo->linkKey[0],  pInfo->linkKey[1],  pInfo->linkKey[2],  pInfo->linkKey[3],
               pInfo->linkKey[4],  pInfo->linkKey[5],  pInfo->linkKey[6],  pInfo->linkKey[7],
               pInfo->linkKey[8],  pInfo->linkKey[9],  pInfo->linkKey[10], pInfo->linkKey[11],
               pInfo->linkKey[12], pInfo->linkKey[13], pInfo->linkKey[14], pInfo->linkKey[15]);
    // clang-format on

    p->linkKeyState = WUD_STATE_LINK_KEY_WRITING;

    BTM_WriteStoredLinkKey(1, &pInfo->devAddr, &pInfo->linkKey, WUDStoredLinkKeyCallback);

    return WUD_STATE_SYNC_WAIT_FOR_STORING;
}

static u8 WUDiWaitFinishStoring(void) {
    return WUDiIsFinishLinkKeyCmd() ? WUD_STATE_SYNC_STORED_DEV_INFO_TO_NAND : WUD_STATE_SYNC_WAIT_FOR_STORING;
}

static u8 WUDiWaitFinishReading(void) {
    WUDSyncState nextState = WUD_STATE_SYNC_WAIT_FOR_READING;

    if (WUDiIsFinishLinkKeyCmd()) {
        nextState = WUD_STATE_SYNC_VIRGIN_STANDARD;
        WUDiRemoveDevice(_wcb.pairAddr);
    }

    return nextState;
}

static u8 WUDiSyncComplete(void) {
    _wcb.syncedNum++;

    memset(&_work, 0, sizeof(WUDDevInfo));

    return _wcb.syncType == WUD_SYNC_TYPE_STANDARD ? WUD_STATE_SYNC_DONE : WUD_STATE_SYNC_PREPARE_SEARCH;
}

static u8 WUDiSyncError(void) {
    memset(&_work, 0, sizeof(WUDDevInfo));

    return _wcb.syncType == WUD_SYNC_TYPE_STANDARD ? WUD_STATE_SYNC_DONE : WUD_STATE_SYNC_PREPARE_SEARCH;
}

static u8 WUDiSyncDone(void) {
    WUDControlBlock* p = &_wcb;
    WUDDevInfo* pInfo;
    int i;
    WUDSyncDeviceCallback pSyncCallback;

    if (p->syncSkipChecks) {
        for (i = 0; i < WUD_MAX_DEV_ENTRY; i++) {
            pInfo = WUDiGetDevInfoIndex(i);

            if (pInfo->status == 8) {
                WUDSetSniffMode(pInfo->devAddr, 8);
                return WUD_STATE_SYNC_DONE;
            }
        }
    }

    OSCancelAlarm(&p->alarm);

    pSyncCallback = p->syncType == WUD_SYNC_TYPE_STANDARD ? p->syncStdCB : p->syncSmpCB;

    if (pSyncCallback != NULL) {
        pSyncCallback(WUD_RESULT_SYNC_DONE, p->syncedNum);
    }

    DEBUGPrint("Pairing Done\n");
    return WUD_STATE_SYNC_START;
}

static void SyncHandler(void) {
    WUDControlBlock* p = &_wcb;

    switch (p->syncState) {
    case WUD_STATE_SYNC_PREPARE_SEARCH: {
        p->syncState = WUDiIsSyncDisabled();
        break;
    }

    case WUD_STATE_SYNC_START_SEARCH: {
        p->syncState = WUDiStartSearch();
        break;
    }

    case WUD_STATE_SYNC_CHECK_SEARCH_RESULT: {
        p->syncState = WUDiNextStepBySearchResult();
        break;
    }

    case WUD_STATE_SYNC_IS_EXISTED_DEVICE: {
        p->syncState = WUDiCheckDeviceDataBase();
        break;
    }

    case WUD_STATE_SYNC_PREPARE_FOR_EXISTED_DEVICE: {
        p->syncState = WUDiExistedDevice();
        break;
    }

    case WUD_STATE_SYNC_PREPARE_FOR_UNKNOWN_DEVICE: {
        p->syncState = WUDiUnknownDevice();
        break;
    }

    case WUD_STATE_SYNC_TRY_CONNECT: {
        p->syncState = WUDiTryConnecting();
        break;
    }

    case WUD_STATE_SYNC_REGISTER_DEVICE: {
        p->syncState = WUDiRegisterSyncDevice();
        break;
    }

    case WUD_STATE_SYNC_VIRGIN_SIMPLE: {
        p->syncState = WUDiVirginSimpleSync();
        break;
    }

    case WUD_STATE_SYNC_VIRGIN_STANDARD: {
        p->syncState = WUDiVirginStandardSync();
        break;
    }

    case WUD_STATE_SYNC_CHANGE_SIMPLE_TO_STANDARD: {
        p->syncState = WUDiChangeSimpleToStandard();
        break;
    }

    case WUD_STATE_SYNC_STORED_LINK_KEY_TO_EEPROM: {
        p->syncState = WUDiStoredLinkKey();
        break;
    }

    case WUD_STATE_SYNC_WAIT_FOR_STORING: {
        p->syncState = WUDiWaitFinishStoring();
        break;
    }

    case WUD_STATE_SYNC_STORED_DEV_INFO_TO_NAND: {
        p->syncState = WUDiSaveDeviceToNand();
        break;
    }

    case WUD_STATE_SYNC_SC_FLUSH: {
        WUDiSaveFlush();
        break;
    }

    case WUD_STATE_SYNC_WAIT_FOR_READING: {
        p->syncState = WUDiWaitFinishReading();
        break;
    }

    case WUD_STATE_SYNC_COMPLETE: {
        p->syncState = WUDiSyncComplete();
        break;
    }

    case WUD_STATE_SYNC_ERROR: {
        p->syncState = WUDiSyncError();
        break;
    }

    case WUD_STATE_SYNC_DONE: {
        p->syncState = WUDiSyncDone();
        break;
    }

    case WUD_STATE_SYNC_WAIT_FOR_SEARCH_RESULT: {
        p->syncState = WUDiWaitForSearchResult();
        break;
    }

    case WUD_STATE_SYNC_6:
    case WUD_STATE_SYNC_13:
    case WUD_STATE_SYNC_WAIT_FOR_INCOMING:
    case WUD_STATE_SYNC_CANCEL_SEARCH: {
        break;
    }

    case WUD_STATE_SYNC_11:
    case WUD_STATE_SYNC_12:
    default: {
        DEBUGPrint("WARNING: Illigal status\n");
        break;
    }
    }
}

static void SyncHandler0(OSAlarm* pAlarm, OSContext* pContext) {
    OSSwitchFiberEx((u32)pAlarm, (u32)pContext, 0, 0, (u32)SyncHandler, (u32)__WUDHandlerStack + sizeof(__WUDHandlerStack));
}

static WUDDeleteState WUDiDisallowIncoming(void) {
    BTA_DmSetVisibility(FALSE, FALSE);
    return WUD_STATE_DELETE_DISCONNECT_ALL;
}

static WUDDeleteState WUDiTerminateDevice(void) {
    int i;
    WUDControlBlock* p = &_wcb;
    WUDDevInfo* pInfo;

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        pInfo = &p->stdDevs[i];

        if (pInfo->status > 1) {
            btm_remove_acl(pInfo->devAddr);
        }
    }

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
        pInfo = &p->smpDevs[i];

        if (pInfo->status > 1) {
            btm_remove_acl(pInfo->devAddr);
        }
    }

    return WUD_STATE_DELETE_CLEANUP_DATABASE;
}

static WUDDeleteState WUDiDeleteDevice(void) {
    int i;
    WUDControlBlock* p = &_wcb;
    WUDDevInfo* pInfo;

    if (WUDiGetLinkNum() > 0) {
        return WUD_STATE_DELETE_CLEANUP_DATABASE;
    }

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        pInfo = &p->stdDevs[i];

        if (pInfo->status == 1) {
            WUDiRemoveDevice(pInfo->devAddr);
        }
    }

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
        pInfo = &p->smpDevs[i];

        if (pInfo->status == 1) {
            WUDiRemoveDevice(pInfo->devAddr);
        }
    }

    return WUD_STATE_DELETE_CLEANUP_SETTING;
}

static void WUDiCleanUp(void) {
    WUDControlBlock* p = &_wcb;
    BOOL success = FALSE;

    if (SCCheckStatus() != SC_STATUS_OK) {
        return;
    }

    memset(&_scArray, 0, sizeof(SCBtDeviceInfoArray));
    success |= SCSetBtDeviceInfoArray(&_scArray);

    if (success) {
        p->deleteState = WUD_STATE_DELETE_6;
        SCFlushAsync(DeleteFlushCallback);
    }
}

static WUDDeleteState WUDiDeleteAllComplete(void) {
    WUDControlBlock* p = &_wcb;

    WUDSetVisibility(FALSE, WUDGetConnectable());

    OSCancelAlarm(&p->alarm);
    p->deleteState = WUD_STATE_DELETE_START;

    if (p->clearDevCB != NULL) {
        p->clearDevCB(WUD_RESULT_DELETE_COMPLETE);
    }

    return WUD_STATE_DELETE_START;
}

static void DeleteAllHandler(void) {
    WUDControlBlock* p = &_wcb;

    p->syncState = WUD_STATE_SYNC_START;

    switch (p->deleteState) {
    case WUD_STATE_DELETE_DISALLOW_INCOMING: {
        p->deleteState = WUDiDisallowIncoming();
        break;
    }

    case WUD_STATE_DELETE_DISCONNECT_ALL: {
        p->deleteState = WUDiTerminateDevice();
        break;
    }

    case WUD_STATE_DELETE_CLEANUP_DATABASE: {
        p->deleteState = WUDiDeleteDevice();
        break;
    }

    case WUD_STATE_DELETE_CLEANUP_SETTING: {
        WUDiCleanUp();
        break;
    }

    case WUD_STATE_DELETE_DONE: {
        p->deleteState = WUDiDeleteAllComplete();
        break;
    }

    case WUD_STATE_DELETE_7: {
        break;
    }
    }
}

static void DeleteAllHandler0(OSAlarm* pAlarm, OSContext* pContext) {
    OSSwitchFiberEx((u32)pAlarm, (u32)pContext, 0, 0, (u32)DeleteAllHandler, (u32)__WUDHandlerStack + sizeof(__WUDHandlerStack));
}

static WUDStackState WUDiCheckRegisteredDeviceInfo(void) {
    WUDControlBlock* p = &_wcb;

    // Set by BTA_HH_ENABLE_EVT
    if (p->hhFlags == 1) {
        p->linkKeyState = WUD_STATE_LINK_KEY_READING;
        BTM_ReadStoredLinkKey(NULL, WUDStoredLinkKeyCallback);
    }

    return WUD_STATE_STACK_CHECK_DEVICE_INFO;
}

static WUDStackState WUDiClearUnregisteredDevice(void) {
    WUDControlBlock* p = &_wcb;
    WUDDevInfo* pInfo;
    int i;

    if (p->linkKeyState == WUD_STATE_LINK_KEY_START) {
        for (i = 0; i < WUD_MAX_DEV_ENTRY; i++) {
            pInfo = WUDiGetDevInfoIndex(i);
            if (pInfo->status == 0) {
                continue;
            }

            if (pInfo->UNK_0x5C == 1) {
                p->linkKeyState = WUD_STATE_LINK_KEY_DELETING;
                BTM_DeleteStoredLinkKey(pInfo->devAddr, WUDStoredLinkKeyCallback);

                pInfo->UNK_0x5C = 0;
                return WUD_STATE_STACK_CHECK_DEVICE_INFO;
            }

            if (pInfo->UNK_0x5C != 3) {
                WUDiRemoveDevInfo(pInfo->devAddr);
            }
        }

        return WUD_STATE_STACK_DONE;
    }

    return WUD_STATE_STACK_CHECK_DEVICE_INFO;
}

static WUDStackState WUDiStackSetupComplete(void) {
    WUDControlBlock* p = &_wcb;

    if (p->linkKeyState == WUD_STATE_LINK_KEY_START) {
        OSCancelAlarm(&p->alarm);
        WUDiGetFirmwareVersion();
        return WUD_STATE_STACK_INITIALIZED;
    }

    return WUD_STATE_STACK_CHECK_DEVICE_INFO;
}

static void EnableStackHandler(void) {
    WUDControlBlock* p = &_wcb;

    switch (p->stackState) {
    case WUD_STATE_STACK_GET_STORED_LINK_KEY: {
        p->stackState = WUDiCheckRegisteredDeviceInfo();
        break;
    }

    case WUD_STATE_STACK_CHECK_DEVICE_INFO: {
        p->stackState = WUDiClearUnregisteredDevice();
        break;
    }

    case WUD_STATE_STACK_DONE: {
        p->stackState = WUDiStackSetupComplete();
        break;
    }
    }
}

static void EnableStackHandler0(OSAlarm* pAlarm, OSContext* pContext) {
    OSSwitchFiberEx((u32)pAlarm, (u32)pContext, 0, 0, (u32)EnableStackHandler, (u32)__WUDHandlerStack + sizeof(__WUDHandlerStack));
}

static WUDInitState WUDiWaitSCSetup(void) {
    //TODO: probably a fakematch
    return (SCCheckStatus() == SC_STATUS_OK) + 1;
}

static WUDInitState WUDiGetRegisteredDevice(void) {
    int i;
    WUDDevInfo* pInfo;

    memset(&_scArray, 0, sizeof _scArray);
    SCGetBtDeviceInfoArray(&_scArray);

    for (i = 0; i < _scArray.num; i++) {
        pInfo = WUDiGetNewDevInfo();

        WUD_BDCPY(pInfo->devAddr, _scArray.regist[i].addr);

        memcpy(&pInfo->conf, &_scArray.regist[i].info, sizeof(SCDevInfo));

        pInfo->status = 1;
        pInfo->sync_type = 0;
        pInfo->UNK_0x5C = 2;

        if (WUD_DEV_NAME_IS_CNT_01(pInfo->conf.devName)) {
            pInfo->subclass = 2;  // subclass 2 is marked as reserved

            pInfo->hhAttrMask = BTA_HH_SEC_REQUIRED | BTA_HH_BATTERY_POWER | BTA_HH_REMOTE_WAKE | BTA_HH_SUP_TOUT_AVLBL | BTA_HH_RECONN_INIT;

            pInfo->appID = 3;
        }

        DEBUGPrint("addr : %02x:%02x:%02x:%02x:%02x:%02x\n", pInfo->devAddr[0], pInfo->devAddr[1], pInfo->devAddr[2], pInfo->devAddr[3],
                   pInfo->devAddr[4], pInfo->devAddr[5]);

        DEBUGPrint("name : %s\n", pInfo->conf.devName);
    }

    return WUD_STATE_INIT_DONE;
}

static WUDInitState WUDiInitComplete(void) {
    WUDControlBlock* p = &_wcb;

    OSCancelAlarm(&p->alarm);
    p->libStatus = WUD_LIB_STATUS_1;

    BTA_EnableBluetooth(WUDSecurityCallback);
    return WUD_STATE_INIT_INITIALIZED;
}

static void InitHandler(void) {
    WUDControlBlock* p = &_wcb;

    switch (p->initState) {
    case WUD_STATE_INIT_WAIT_FOR_INITIALIZATION: {
        p->initState = WUDiWaitSCSetup();
        break;
    }

    case WUD_STATE_INIT_GET_DEV_INFO: {
        p->initState = WUDiGetRegisteredDevice();
        break;
    }

    case WUD_STATE_INIT_DONE: {
        p->initState = WUDiInitComplete();
        break;
    }
    }
}

static void InitHandler0(OSAlarm* pAlarm, OSContext* pContext) {
    OSSwitchFiberEx((u32)pAlarm, (u32)pContext, 0, 0, (u32)InitHandler, (u32)__WUDHandlerStack + sizeof(__WUDHandlerStack));
}

static void WUDiContMapTableFlush(void) {
    WUDControlBlock* p = &_wcb;
    u8 nextState;

    if (SCCheckStatus() == SC_STATUS_OK && SCSetBtDeviceInfoArray(&_scArray)) {
        SCFlushAsync(ShutFlushCallback);
        nextState = WUD_STATE_SHUTDOWN_FLUSH_SETTINGS;
    } else {
        nextState = WUD_STATE_SHUTDOWN_STORE_SETTINGS;
    }

    p->shutdownState = nextState;
}

static void ShutdownHandler(void) {
    WUDControlBlock* p = &_wcb;

    switch (p->shutdownState) {
    case WUD_STATE_SHUTDOWN_STORE_SETTINGS: {
        WUDiContMapTableFlush();
        break;
    }

    case WUD_STATE_SHUTDOWN_FLUSH_SETTINGS: {
        break;
    }
    }
}

static void ShutdownHandler0(OSAlarm* pAlarm, OSContext* pContext) {
    OSSwitchFiberEx((u32)pAlarm, (u32)pContext, 0, 0, (u32)ShutdownHandler, (u32)__WUDHandlerStack + sizeof(__WUDHandlerStack));
}

BOOL WUDInit(void) {
    WUDControlBlock* p = &_wcb;
    BOOL irqEnabled;
    int i;

    if (_initialized) {
        return FALSE;
    }

    irqEnabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY; i++) {
        _dev_handle_to_bda[i] = NULL;
        _dev_handle_queue_size[i] = 0;
        _dev_handle_notack_num[i] = 0;
    }

    p->smpListTail = &p->smpList[WUD_MAX_DEV_ENTRY_FOR_SMP - 1];
    p->smpListHead = &p->smpList[0];

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
        p->smpList[i].devInfo = &p->smpDevs[WUD_MAX_DEV_ENTRY_FOR_SMP - 1 - i];
        p->smpList[i].prev = i == 0 ? NULL : &p->smpList[i - 1];
        p->smpList[i].next = i == WUD_MAX_DEV_ENTRY_FOR_SMP - 1 ? NULL : &p->smpList[i + 1];
    }

    p->syncStdCB = NULL;
    p->syncSmpCB = NULL;
    p->clearDevCB = NULL;
    p->hidConnCB = NULL;
    p->hidRecvCB = NULL;

    p->syncState = WUD_STATE_SYNC_START;
    p->linkKeyState = WUD_STATE_LINK_KEY_START;
    p->deleteState = WUD_STATE_DELETE_START;
    p->stackState = WUD_STATE_STACK_START;
    p->initState = WUD_STATE_INIT_START;
    p->shutdownState = WUD_STATE_SHUTDOWN_START;

    p->syncSkipChecks = FALSE;
    p->syncType = WUD_SYNC_TYPE_STANDARD;
    p->syncLoopNum = 1;

    p->connectable = FALSE;
    p->discoverable = FALSE;

    p->libStatus = WUD_LIB_STATUS_0;
    p->pmID = 0;
    p->syncRssi = -65;

    memset(p->hostAddr, 0, BD_ADDR_LEN);
    memset(p->pairAddr, 0, BD_ADDR_LEN);

    p->bufferStatus0 = 0;
    p->bufferStatus1 = 10;

    WUDiClearDevice();

    p->initState = WUD_STATE_INIT_WAIT_FOR_INITIALIZATION;

    OSRestoreInterrupts(irqEnabled);

    DEBUGPrint("BTA_Init() is started\n");

    BTA_Init();
    bta_sys_set_trace_level(__bte_trace_level);
    L2CA_SetTraceLevel(__bte_trace_level);
    SDP_SetTraceLevel(__bte_trace_level);

    DEBUGPrint("BTA_Init() is done\n");

    SCInit();

    OSCreateAlarm(&p->alarm);
    OSSetPeriodicAlarm(&p->alarm, __OSGetSystemTime(), OSMillisecondsToTicks(10), InitHandler0);

    _initialized = TRUE;
    return TRUE;
}

void WUDRegisterAllocator(WUDAllocFunc pAllocFunc, WUDFreeFunc pFreeFunc) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    p->allocFunc = pAllocFunc;
    p->freeFunc = pFreeFunc;

    OSRestoreInterrupts(enabled);
}

extern u32 __ntd_get_allocated_mem_size(void);

u32 WUDGetAllocatedMemSize(void) {
    return __ntd_get_allocated_mem_size();
}

void WUDShutdown(void) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;
    int i;
    WUDDevInfoList* pIt;

    DEBUGPrint("WUDShutdown()\n");

    WUDSetVisibility(FALSE, FALSE);

    enabled = OSDisableInterrupts();

    if (WUDIsBusy()) {
        OSCancelAlarm(&p->alarm);
    }

    p->shutdownState = WUD_STATE_SHUTDOWN_STORE_SETTINGS;

    OSCreateAlarm(&p->alarm);
    OSSetPeriodicAlarm(&p->alarm, __OSGetSystemTime(), OSMillisecondsToTicks(10), ShutdownHandler0);

    p->libStatus = WUD_LIB_STATUS_4;

    OSRestoreInterrupts(enabled);
}

WUDLibStatus WUDGetStatus(void) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled = OSDisableInterrupts();

    s8 libStatus = p->libStatus;

    OSRestoreInterrupts(enabled);
    return libStatus;
}

u8 WUDGetBufferStatus(void) {
    WUDControlBlock* p = &_wcb;
    u8 status;
    BOOL enabled = OSDisableInterrupts();

    status = p->bufferStatus1 - p->bufferStatus0;

    OSRestoreInterrupts(enabled);
    return status;
}

void WUDSetSniffMode(BD_ADDR addr, s32 interval) {
    WUDControlBlock* p = &_wcb;
    tBTM_PM_PWR_MD block;

    if (interval > 0) {
        block.mode = BTM_PM_MD_SNIFF;
    } else {
        block.mode = BTM_PM_MD_ACTIVE;
    }

    block.max = interval;
    block.min = interval;
    block.attempt = 1;
    block.timeout = 0;

    BTM_SetPowerMode(p->pmID, addr, &block);
}

WUDSyncDeviceCallback WUDSetSyncSimpleCallback(WUDSyncDeviceCallback pCallback) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;
    WUDSyncDeviceCallback pOldCallback;

    DEBUGPrint("WUDSetSyncDeviceCallback\n");

    enabled = OSDisableInterrupts();

    pOldCallback = p->syncSmpCB;
    p->syncSmpCB = pCallback;

    OSRestoreInterrupts(enabled);
    return pOldCallback;
}

WUDClearDeviceCallback WUDSetClearDeviceCallback(WUDClearDeviceCallback pCallback) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;
    WUDClearDeviceCallback pOldCallback;

    DEBUGPrint("WUDSetClearDeviceCallback\n");

    enabled = OSDisableInterrupts();

    pOldCallback = p->clearDevCB;
    p->clearDevCB = pCallback;

    OSRestoreInterrupts(enabled);
    return pOldCallback;
}

static BOOL StartSyncDevice(u8 syncType, s8 syncLoopNum, BOOL syncSkipChecks) {
    WUDControlBlock* p = &_wcb;
    BOOL success = FALSE;
    BOOL enabled;
    u32 libStatus;

    enabled = OSDisableInterrupts();
    libStatus = p->libStatus;
    OSRestoreInterrupts(enabled);

    if (libStatus == WUD_LIB_STATUS_3 && !WUDIsBusy()) {
        enabled = OSDisableInterrupts();

        p->syncState = WUD_STATE_SYNC_PREPARE_SEARCH;
        p->syncLoopNum = syncLoopNum;
        p->syncType = syncType;
        p->syncSkipChecks = syncSkipChecks ? TRUE : FALSE;
        p->syncedNum = 0;

        OSCreateAlarm(&p->alarm);
        OSSetPeriodicAlarm(&p->alarm, __OSGetSystemTime(), OSMillisecondsToTicks(20), SyncHandler0);

        OSRestoreInterrupts(enabled);

        success = TRUE;
    }

    return success;
}

static BOOL StartSyncStandard(BOOL syncSkipChecks) {
    return StartSyncDevice(WUD_SYNC_TYPE_STANDARD, 3, syncSkipChecks);
}

BOOL WUDStartSyncDevice(void) {
    WUDControlBlock* p = &_wcb;
    BOOL success;
    BOOL enabled;
    WUDSyncDeviceCallback pSyncCallback;

    DEBUGPrint("WUDStartSyncDevice()\n");
    success = StartSyncStandard(FALSE);

    enabled = OSDisableInterrupts();
    pSyncCallback = p->syncStdCB;
    OSRestoreInterrupts(enabled);

    if (!success && pSyncCallback != NULL) {
        pSyncCallback(WUD_RESULT_SYNC_BUSY, 0);
    }

    return success;
}

static BOOL StartSyncSimple(BOOL syncSkipChecks) {
    return StartSyncDevice(WUD_SYNC_TYPE_SIMPLE, -1, syncSkipChecks);
}

BOOL WUDStartFastSyncSimple(void) {
    WUDControlBlock* p = &_wcb;
    BOOL success;
    BOOL enabled;
    WUDSyncDeviceCallback pSyncCallback;

    DEBUGPrint("WUDStartSyncSimple()\n");
    success = StartSyncSimple(TRUE);

    enabled = OSDisableInterrupts();
    pSyncCallback = p->syncSmpCB;
    OSRestoreInterrupts(enabled);

    if (!success && pSyncCallback != NULL) {
        pSyncCallback(WUD_RESULT_SYNC_BUSY, 0);
    }

    return success;
}

BOOL WUDStartSyncSimple(void) {
    WUDControlBlock* p = &_wcb;
    BOOL success;
    BOOL enabled;
    WUDSyncDeviceCallback pSyncCallback;

    DEBUGPrint("WUDStartSyncSimple()\n");
    success = StartSyncSimple(FALSE);

    enabled = OSDisableInterrupts();
    pSyncCallback = p->syncSmpCB;
    OSRestoreInterrupts(enabled);

    if (!success && pSyncCallback != NULL) {
        pSyncCallback(WUD_RESULT_SYNC_BUSY, 0);
    }

    return success;
}

BOOL WUDCancelSyncDevice(void) {
    DEBUGPrint("WUDCancelSyncDevice()\n");
}

static BOOL StopSync(void) {
    WUDControlBlock* p;
    BOOL success;
    BOOL enabled;

    DEBUGPrint("WUDStopSyncSimple()\n");

    p = &_wcb;
    success = FALSE;
    enabled = OSDisableInterrupts();

    if (p->libStatus == WUD_LIB_STATUS_3) {
        if (p->syncState == WUD_STATE_SYNC_WAIT_FOR_SEARCH_RESULT) {
            BTA_DmSearchCancel();
        }

        p->syncLoopNum = 0;

        success = TRUE;
    }

    OSRestoreInterrupts(enabled);
    return success;
}

BOOL WUDStopSyncSimple(void) {
    return StopSync();
}

BOOL WUDStartClearDevice(void) {
    WUDControlBlock* p = &_wcb;
    BOOL success;
    BOOL enabled;
    u32 libStatus;

    success = FALSE;
    DEBUGPrint("WUDStartClearDevice()\n");

    enabled = OSDisableInterrupts();
    libStatus = p->libStatus;

    OSRestoreInterrupts(enabled);

    if (libStatus == WUD_LIB_STATUS_3 && !WUDIsBusy()) {
        enabled = OSDisableInterrupts();

        p->deleteState = WUD_STATE_DELETE_DISALLOW_INCOMING;

        OSCreateAlarm(&p->alarm);
        OSSetPeriodicAlarm(&p->alarm, __OSGetSystemTime(), OSMillisecondsToTicks(20), DeleteAllHandler0);

        OSRestoreInterrupts(enabled);

        success = TRUE;
    }

    return success;
}

BOOL WUDSetDisableChannel(s8 afhChannel) {
    int first = 0;
    int last = 0;
    BOOL success = FALSE;
    WUDControlBlock* p = &_wcb;
    BOOL enabled;
    u32 libStatus;
    tBTM_STATUS status;

    DEBUGPrint("WUDSetDisableChannel()\n");

    if (afhChannel < 0 || 13 < afhChannel) {
        return FALSE;
    }

    enabled = OSDisableInterrupts();
    libStatus = p->libStatus;
    OSRestoreInterrupts(enabled);

    if (libStatus == WUD_LIB_STATUS_3) {
        if (!afhChannel) {
            first = 0xFF;
            last = 0xFF;
        } else {
            first = (afhChannel + 1) * 5 - 14;
            last = (afhChannel + 1) * 5 + 14;

            if (first < 0) {
                first = 0;
            }

            if (last > 78) {
                last = 78;
            }
        }

        status = BTM_SetAfhChannels(first, last);
        DEBUGPrint("BTM_SetAfhChannels() : %d\n", status);

        success = TRUE;
    }

    return success;
}

WUDHidRecvCallback WUDSetHidRecvCallback(WUDHidRecvCallback pCallback) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;
    WUDHidRecvCallback pOldCallback;

    DEBUGPrint("WUDSetHidRecvCallback()\n");

    enabled = OSDisableInterrupts();

    pOldCallback = p->hidRecvCB;
    p->hidRecvCB = pCallback;

    OSRestoreInterrupts(enabled);
    return pOldCallback;
}

WUDHidConnCallback WUDSetHidConnCallback(WUDHidConnCallback pCallback) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;
    WUDHidConnCallback pOldCallback;

    DEBUGPrint("WUDSetHidConnCallback()\n");

    enabled = OSDisableInterrupts();

    pOldCallback = p->hidConnCB;
    p->hidConnCB = pCallback;

    OSRestoreInterrupts(enabled);
    return pOldCallback;
}

void WUDSetVisibility(u8 disc, u8 conn) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled = OSDisableInterrupts();

    p->discoverable = disc;
    p->connectable = conn;

    OSRestoreInterrupts(enabled);
    BTA_DmSetVisibility(disc, conn);
}

u8 WUDGetConnectable(void) {
    WUDControlBlock* p = &_wcb;
    u8 connectable;
    BOOL enabled = OSDisableInterrupts();

    connectable = p->connectable;

    OSRestoreInterrupts(enabled);
    return connectable;
}

void reset_again_cb(void* p1) {
#pragma unused(p1)

    WUDiInitSub();
}

void reset_again(void) {
    BTM_DeviceReset(reset_again_cb);
}

// clang-format off
static u8 patch_binary[] = {
    0x70, 0x99, 0x08, 0x00, // address (little-endian)
    0x38, 0x00, 0x00, 0x00, // size (little-endian)

    // patch data
    0x88, 0x43, 0xD1, 0x07,
    0x09, 0x0C, 0x08, 0x43,
    0xA0, 0x62, 0x19, 0x23,
    0xDB, 0x01, 0x33, 0x80,
    0x7C, 0xF7, 0x88, 0xF8,
    0x28, 0x76, 0x80, 0xF7,
    0x17, 0xFF, 0x43, 0x78,
    0xEB, 0x70, 0x19, 0x23,
    0xDB, 0x01, 0x33, 0x87,
    0x7C, 0xF7, 0xBC, 0xFB,
    0x0B, 0x60, 0xA3, 0x7B,
    0x01, 0x49, 0x0B, 0x60,
    0x90, 0xF7, 0x96, 0xFB,
    0xD8, 0x1D, 0x08, 0x00,
};
// clang-format on

// clang-format off
static WUDPatchList patch_install = {
    5,
    {
        {0x20, 0xBC, 0x65, 0x01, 0x00, 0x84, 0x42, 0x09, 0xD2, 0x84, 0x42, 0x09, 0xD1},
        {0x21, 0x84, 0x5A, 0x00, 0x00, 0x83, 0xF0, 0x74, 0xFF, 0x09, 0x0C, 0x08, 0x43},
        {0x22, 0x00, 0x61, 0x00, 0x00, 0x83, 0xF0, 0x40, 0xFC, 0x00, 0x00, 0x00, 0x00},
        {0x23, 0xCC, 0x9F, 0x01, 0x00, 0x6F, 0xF0, 0xE4, 0xFC, 0x03, 0x28, 0x7D, 0xD1},
        {0x24, 0x3C, 0x62, 0x01, 0x00, 0x28, 0x20, 0x00, 0xE0, 0x60, 0x8D, 0x23, 0x68},
    }
};
// clang-format on

static u8 remove_patch[] = {0x00};

static u8 install_num = 0;
static u8 patch_num = 0;

static u32 addr = 0;
static s32 offset = 0;
static u32 size = 0;

static void remove_patch_cb(tBTM_VSC_CMPL* p1) {
    if (p1 == NULL) {
        WUDiInitSub();
    } else {
        offset = 0;
        WUDiWritePatch();
    }
}

static void WUDiRemovePatch(void) {
    BTM_VendorSpecificCommand(BT_VSC_NINTENDO_INSTALL_PATCH, sizeof(remove_patch), remove_patch, remove_patch_cb);
}

static void write_patch_cb(tBTM_VSC_CMPL* p1) {
    if (p1 != NULL) {
        if (size == (u32)offset) {
            install_num = 0;
            patch_num = patch_install.num;
            WUDiInstallPatch();
        } else {
            WUDiWritePatch();
        }
    } else {
        WUDiInitSub();
    }
}

static void WUDiWritePatch(void) {
    u8 buf[WUD_PATCH_BUFFER_SIZE];
    u8 length;
    int i;

    length = MIN(size - offset, WUD_PATCH_BUFFER_SIZE - sizeof(u32));

    for (i = 0; i < (s32)sizeof(u32); i++) {
        buf[i] = (addr + offset) >> i * 8;
    }

    for (i = 0; i < (s32)length; i++) {
        buf[(s32)sizeof(u32) + i] = patch_binary[sizeof(size) + sizeof(offset) + offset + i];
    }

    offset += length;
    BTM_VendorSpecificCommand(BT_VSC_NINTENDO_WRITE_PATCH, (s32)sizeof(u32) + length, buf, write_patch_cb);
}

static void install_patch_cb(tBTM_VSC_CMPL* p1) {
    if (p1 != NULL) {
        if (patch_num == install_num) {
            WUDiInitSub();
        } else {
            WUDiInstallPatch();
        }
    } else {
        WUDiInitSub();
    }
}

static void WUDiInstallPatch(void) {
    u8 buf[WUD_PATCH_BUFFER_SIZE];
    u8 num;

    num = MIN(patch_num - install_num, WUD_MAX_PATCHES);
    buf[offsetof(WUDPatchList, num)] = num;

    memcpy(&buf[offsetof(WUDPatchList, cmds)], patch_install.cmds[install_num].data, num * sizeof(WUDPatchCmd));

    install_num += num;
    BTM_VendorSpecificCommand(BT_VSC_NINTENDO_INSTALL_PATCH, sizeof(num) + num * sizeof(WUDPatchCmd), buf, install_patch_cb);
}

static void WUDiAppendRuntimePatch(void) {
    int i;

    DEBUGPrint("WUDiAppendRuntimePatch()\n");

    size = 0;
    addr = 0;

    for (i = 0; i < (int)sizeof(u32); i++) {
        addr <<= 8;
        addr += patch_binary[3 - i];

        size <<= 8;
        size += patch_binary[7 - i];
    }

    WUDiRemovePatch();
}

void WUDiGetFirmwareVersion(void) {
    tBTM_VERSION_INFO version;
    BTM_ReadLocalVersion(&version);

    DEBUGPrint("hci_version   : %02x\n", version.hci_version);
    DEBUGPrint("hci_revision  : %04x\n", version.hci_revision);

    DEBUGPrint("lmp_version   : %02x\n", version.lmp_version);
    DEBUGPrint("lmp_subversion: %04x\n", version.lmp_subversion);

    DEBUGPrint("manufacturer  : %04x\n", version.manufacturer);

    DEBUGPrint(" ==> 2045 firmware ver.002.003.014.%d\n", version.hci_revision & 0xFFF);

    if ((version.hci_revision & 0xFFF) == 167) {
        WUDiAppendRuntimePatch();
    } else {
        WUDiInitSub();
    }
}

void WUDiInitSub(void) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;
    int i;

    char devName[] = "Wii";
    DEV_CLASS devClass = {
        0x00,  // No designated Major Service Classes
        0x04,  // Major Device Class 4 (Audio/Video)
        0x48   // Minor Device Class 18 (Audio/Video -> Gaming/Toy)
    };

    DEBUGPrint("start WUDiInitSub()\n");

    BTA_DmSetDeviceName(devName);
    BTM_SetDeviceClass(devClass);

    BTM_RegisterForVSEvents(&WUDVendorSpecificCallback);
    BTM_RegisterForDeviceStatusNotif(&WUDDeviceStatusCallback);
    BTM_PmRegister(BTM_PM_REG_SET | BTM_PM_REG_NOTIF, &p->pmID, &WUDPowerManagerCallback);

    BTM_WritePageTimeout(32768);
    BTM_SetDefaultLinkPolicy(HCI_ENABLE_MASTER_SLAVE_SWITCH | HCI_ENABLE_SNIFF_MODE);
    BTM_SetDefaultLinkSuperTout(3200);

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        if (p->stdDevs[i].status == 1) {
            WUDiRegisterDevice(p->stdDevs[i].devAddr);
        }
    }

    enabled = OSDisableInterrupts();
    p->libStatus = WUD_LIB_STATUS_3;
    OSRestoreInterrupts(enabled);

    WUDSetVisibility(FALSE, TRUE);
}

void WUDiEnableStack(void) {
    WUDControlBlock* p = &_wcb;

    BTA_HhEnable(BTA_SEC_AUTHENTICATE, WUDHidHostCallback);

    p->stackState = WUD_STATE_STACK_GET_STORED_LINK_KEY;

    OSCreateAlarm(&p->alarm);
    OSSetPeriodicAlarm(&p->alarm, __OSGetSystemTime(), OSMillisecondsToTicks(10), EnableStackHandler0);
}

void WUDiAutoSync(void) {
    WUDControlBlock* p = &_wcb;
    WUDSyncDeviceCallback pSyncCallback;
    s32 result;
    BOOL enabled;

    DEBUGPrint("WUDiAutoSync()\n");
    enabled = OSDisableInterrupts();

    pSyncCallback = p->syncStdCB;
    result = WUDIsBusy() ? -1 /* WUD_RESULT_DELETE_BUSY */
                           :
                           WUD_RESULT_DELETE_WAITING;

    OSRestoreInterrupts(enabled);

    if (pSyncCallback != NULL) {
        pSyncCallback(result, 0);
    } else {
        WUDStartSyncDevice();
    }
}

void WUDiCancelSync() {
    DEBUGPrint("WUDiCancelSync()\n");
}

void WUDiDeleteAllLinkKeys(void) {
    WUDControlBlock* p = &_wcb;
    WUDClearDeviceCallback pClearCallback;
    s32 result;
    BOOL enabled;

    DEBUGPrint("WUDiDeleteAllLinkKeys()\n");
    enabled = OSDisableInterrupts();

    pClearCallback = p->clearDevCB;
    result = WUDIsBusy() ? -1 /* WUD_RESULT_DELETE_BUSY */
                           :
                           WUD_RESULT_DELETE_WAITING;

    OSRestoreInterrupts(enabled);

    if (pClearCallback != NULL) {
        pClearCallback(result);
    } else {
        WUDStartClearDevice();
    }
}

void WUDiRegisterDevice(BD_ADDR addr) {
    WUDControlBlock* p = &_wcb;
    WUDDevInfo* pInfo;
    tBTA_STATUS status;
    BOOL enabled;

    pInfo = WUDiGetDevInfo(addr);

    status = BTA_DmAddDevice(pInfo->devAddr, pInfo->linkKey, 0, FALSE);
    DEBUGPrint("BTA_DmAddDevice(): %d\n", status);

    if (WUD_DEV_NAME_IS_CNT(pInfo->conf.devName)) {
        tBTA_HH_DEV_DESCR desc;
        desc.dl_len = sizeof(descriptor);
        desc.dsc_list = descriptor;

        DEBUGPrint("BTA_HhAddDev()\n");
        BTA_HhAddDev(pInfo->devAddr, pInfo->hhAttrMask, pInfo->subclass, pInfo->appID, desc);
    }

    enabled = OSDisableInterrupts();

    if (pInfo->sync_type == 0 || pInfo->sync_type == 4 || pInfo->sync_type == 2 || pInfo->sync_type == 5) {
        p->devNums++;
    } else {
        p->devSmpNums++;
    }

    OSRestoreInterrupts(enabled);
}

void WUDiRemoveDevice(BD_ADDR addr) {
    WUDControlBlock* p = &_wcb;
    WUDDevInfo* pInfo;
    tBTM_STATUS status;
    BOOL enabled = OSDisableInterrupts();

    DEBUGPrint("WUDiRemoveDevice : \n");

    pInfo = WUDiGetDevInfo(addr);

    if (pInfo != NULL) {
        DEBUGPrint(" handle : %d,  addr : %02x:%02x:%02x:%02x:%02x:%02x\n", pInfo->devHandle, pInfo->devAddr[0], pInfo->devAddr[1], pInfo->devAddr[2],
                   pInfo->devAddr[3], pInfo->devAddr[4], pInfo->devAddr[5]);

        DEBUGPrint("remove device info from database.\n");

        if (WUD_DEV_NAME_IS_CNT(pInfo->conf.devName)) {
            DEBUGPrint("BTA_HhRemoveDev()\n");
            DEBUGPrint(" handle : %d\n", pInfo->devHandle);
            BTA_HhRemoveDev(pInfo->devHandle);
        }

        status = BTA_DmRemoveDevice(pInfo->devAddr);
        DEBUGPrint("BTA_DmRemoveDevice(): %d\n", status);

        if (pInfo->sync_type == 0) {
            p->devNums--;
        } else {
            p->devSmpNums--;
        }

        memset(pInfo, 0, sizeof *pInfo);
    }

    OSRestoreInterrupts(enabled);
}

WUDDevInfo* WUDiGetDevInfo(BD_ADDR addr) {
    WUDControlBlock* p = &_wcb;
    WUDDevInfo* pInfo;
    BOOL enabled;
    int i;

    pInfo = NULL;
    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        if (WUD_BDCMP(p->stdDevs[i].devAddr, addr) == 0) {
            pInfo = &p->stdDevs[i];
            break;
        }
    }

    if (pInfo == NULL) {
        for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
            if (WUD_BDCMP(p->smpDevs[i].devAddr, addr) == 0) {
                pInfo = &p->smpDevs[i];
                break;
            }
        }
    }

    OSRestoreInterrupts(enabled);
    return pInfo;
}

WUDDevInfo* WUDiGetDevInfoIndex(int idx) {
    WUDControlBlock* p = &_wcb;
    WUDDevInfo* pInfo;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if (0 <= idx && idx < WUD_MAX_DEV_ENTRY_FOR_STD) {
        pInfo = &p->stdDevs[idx];
    } else {
        pInfo = &p->smpDevs[idx - WUD_MAX_DEV_ENTRY_FOR_STD];
    }

    OSRestoreInterrupts(enabled);
    return pInfo;
}

WUDDevInfo* WUDiGetNewDevInfo(void) {
    WUDControlBlock* p = &_wcb;
    WUDDevInfo* pInfo;
    BOOL enabled;
    int i;

    pInfo = NULL;
    enabled = OSDisableInterrupts();

    if (p->syncType == WUD_SYNC_TYPE_STANDARD) {
        for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
            if (p->stdDevs[i].status == 0) {
                DEBUGPrint("new entry index is %d\n", i);
                pInfo = &p->stdDevs[i];
                break;
            }
        }
    } else {
        pInfo = p->smpListTail->devInfo;
    }

    OSRestoreInterrupts(enabled);
    return pInfo;
}

void WUDiRemoveDevInfo(BD_ADDR addr) {
    WUDDevInfo* pInfo;
    BOOL enabled;

    pInfo = WUDiGetDevInfo(addr);
    if (pInfo == NULL) {
        return;
    }

    enabled = OSDisableInterrupts();
    memset(pInfo, 0, sizeof(WUDDevInfo));
    OSRestoreInterrupts(enabled);
}

void WUDiClearDevice(void) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    memset(p->stdDevs, 0, sizeof(WUDDevInfo) * WUD_MAX_DEV_ENTRY_FOR_STD);
    memset(p->smpDevs, 0, sizeof(WUDDevInfo) * WUD_MAX_DEV_ENTRY_FOR_SMP);

    p->devNums = 0;
    p->devSmpNums = 0;

    p->connectedNum = 0;
    p->linkedNum = 0;
    p->syncedNum = 0;

    OSRestoreInterrupts(enabled);
}

u8 WUDiGetDevNumber(void) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled = OSDisableInterrupts();

    u8 num = p->devNums;

    OSRestoreInterrupts(enabled);
    return num;
}

u8 WUDiGetDevSmpNumber(void) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled = OSDisableInterrupts();

    u8 num = p->devSmpNums;

    OSRestoreInterrupts(enabled);
    return num;
}

u8 WUDiGetConnNumber(void) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled = OSDisableInterrupts();

    u8 num = p->connectedNum;

    OSRestoreInterrupts(enabled);
    return num;
}

u8 WUDiGetLinkNum(void) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled = OSDisableInterrupts();

    u8 num = p->linkedNum;

    OSRestoreInterrupts(enabled);
    return num;
}

void WUDiMoveTopSmpDevInfoPtr(WUDDevInfo* pInfo) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
        if (WUD_BDCMP(p->smpList[i].devInfo->devAddr, pInfo->devAddr) != 0) {
            continue;
        }

        if (WUD_BDCMP(p->smpListHead->devInfo->devAddr, p->smpList[i].devInfo->devAddr) == 0) {
            break;
        }

        p->smpList[i].prev->next = p->smpList[i].next;

        if (WUD_BDCMP(p->smpListTail->devInfo->devAddr, p->smpList[i].devInfo->devAddr) == 0) {
            p->smpListTail = p->smpList[i].prev;
        } else {
            p->smpList[i].next->prev = p->smpList[i].prev;
        }

        p->smpList[i].next = p->smpListHead;
        p->smpListHead->prev = &p->smpList[i];
        p->smpListHead = &p->smpList[i];

        p->smpList[i].prev = NULL;

        break;
    }

    OSRestoreInterrupts(enabled);
}

void WUDiMoveBottomSmpDevInfoPtr(WUDDevInfo* pInfo) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
        if (WUD_BDCMP(p->smpList[i].devInfo->devAddr, pInfo->devAddr) != 0) {
            continue;
        }

        if (WUD_BDCMP(p->smpListTail->devInfo->devAddr, p->smpList[i].devInfo->devAddr) == 0) {
            break;
        }

        p->smpList[i].next->prev = p->smpList[i].prev;

        if (WUD_BDCMP(p->smpListHead->devInfo->devAddr, p->smpList[i].devInfo->devAddr) == 0) {
            p->smpListHead = p->smpList[i].next;
        } else {
            p->smpList[i].prev->next = p->smpList[i].next;
        }

        p->smpList[i].prev = p->smpListTail;
        p->smpListTail->next = &p->smpList[i];
        p->smpListTail = &p->smpList[i];

        p->smpList[i].next = NULL;

        break;
    }

    OSRestoreInterrupts(enabled);
}

WUDDevInfo* WUDiGetRemoveSmpDevice(void) {
    return _wcb.smpListTail->devInfo;
}

void WUDiMoveTopOfDisconnectedSmpDevice(WUDDevInfo* pInfo) {
    WUDControlBlock* p = &_wcb;
    int i;
    BOOL enabled;
    WUDDevInfoList* pIt;

    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
        if (WUD_BDCMP(p->smpList[i].devInfo->devAddr, pInfo->devAddr) != 0) {
            continue;
        }

        for (pIt = p->smpListHead; pIt; pIt = pIt->next) {
            if (WUD_BDCMP(pIt->devInfo->devAddr, pInfo->devAddr) == 0) {
                continue;
            }

            if (pIt->devInfo->status > 1) {
                continue;
            }

            if (WUD_BDCMP(p->smpListHead->devInfo->devAddr, p->smpList[i].devInfo->devAddr) == 0) {
                if (pIt == p->smpListHead->next) {
                    break;
                }

                p->smpListHead = p->smpList[i].next;
            } else {
                p->smpList[i].prev->next = p->smpList[i].next;
            }

            p->smpList[i].next->prev = p->smpList[i].prev;

            if (pIt != p->smpListHead) {
                p->smpList[i].prev = pIt->prev;
                p->smpList[i].next = pIt;

                pIt->prev->next = &p->smpList[i];
                pIt->prev = &p->smpList[i];
            } else {
                p->smpList[i].prev = pIt;
                p->smpList[i].next = pIt->next;

                pIt->next->prev = &p->smpList[i];
                pIt->next = &p->smpList[i];
            }

            break;
        }
    }

    OSRestoreInterrupts(enabled);
}

#if SDK_AUG2010
void WUDiMoveTopStdDevInfoPtr(WUDDevInfo* pInfo) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        if (WUD_BDCMP(p->stdList[i].devInfo->devAddr, pInfo->devAddr) != 0) {
            continue;
        }

        if (WUD_BDCMP(p->stdListHead->devInfo->devAddr, p->stdList[i].devInfo->devAddr) == 0) {
            break;
        }

        p->stdList[i].prev->next = p->stdList[i].next;

        if (WUD_BDCMP(p->stdListTail->devInfo->devAddr, p->stdList[i].devInfo->devAddr) == 0) {
            p->stdListTail = p->stdList[i].prev;
        } else {
            p->stdList[i].next->prev = p->stdList[i].prev;
        }

        p->stdList[i].next = p->stdListHead;
        p->stdListHead->prev = &p->stdList[i];
        p->stdListHead = &p->stdList[i];

        p->stdList[i].prev = NULL;

        break;
    }

    OSRestoreInterrupts(enabled);
}

void WUDiMoveBottomStdDevInfoPtr(WUDDevInfo* pInfo) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        if (WUD_BDCMP(p->stdList[i].devInfo->devAddr, pInfo->devAddr) != 0) {
            continue;
        }

        if (WUD_BDCMP(p->stdListTail->devInfo->devAddr, p->stdList[i].devInfo->devAddr) == 0) {
            break;
        }

        p->stdList[i].next->prev = p->stdList[i].prev;

        if (WUD_BDCMP(p->stdListHead->devInfo->devAddr, p->stdList[i].devInfo->devAddr) == 0) {
            p->stdListHead = p->stdList[i].next;
        } else {
            p->stdList[i].prev->next = p->stdList[i].next;
        }

        p->stdList[i].prev = p->stdListTail;
        p->stdListTail->next = &p->stdList[i];
        p->stdListTail = &p->stdList[i];

        p->stdList[i].next = NULL;

        break;
    }

    OSRestoreInterrupts(enabled);
}

WUDDevInfo* WUDiGetRemoveStdDevice(void) {
    return _wcb.stdListTail->devInfo;
}

void WUDiMoveTopOfDisconnectedStdDevice(WUDDevInfo* pInfo) {
    WUDControlBlock* p = &_wcb;
    int i;
    BOOL enabled;
    WUDDevInfoList* pIt;

    enabled = OSDisableInterrupts();

    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_STD; i++) {
        if (WUD_BDCMP(p->stdList[i].devInfo->devAddr, pInfo->devAddr) != 0) {
            continue;
        }

        for (pIt = p->stdListHead; pIt; pIt = pIt->next) {
            if (WUD_BDCMP(pIt->devInfo->devAddr, pInfo->devAddr) == 0) {
                continue;
            }

            if (pIt->devInfo->status > 1) {
                continue;
            }

            if (WUD_BDCMP(p->stdListHead->devInfo->devAddr, p->stdList[i].devInfo->devAddr) == 0) {
                if (pIt == p->stdListHead->next)
                    break;

                p->stdListHead = p->stdList[i].next;
            } else {
                p->stdList[i].prev->next = p->stdList[i].next;
            }

            p->stdList[i].next->prev = p->stdList[i].prev;

            if (pIt != p->stdListHead) {
                p->stdList[i].prev = pIt->prev;
                p->stdList[i].next = pIt;

                pIt->prev->next = &p->stdList[i];
                pIt->prev = &p->stdList[i];
            } else {
                p->stdList[i].prev = pIt;
                p->stdList[i].next = pIt->next;

                pIt->next->prev = &p->stdList[i];
                pIt->next = &p->stdList[i];
            }

            break;
        }
    }

    OSRestoreInterrupts(enabled);
}
#endif

BOOL WUDIsBusy(void) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if (p->syncState == WUD_STATE_SYNC_START &&
        p->deleteState == WUD_STATE_DELETE_START &&
        p->stackState == WUD_STATE_STACK_INITIALIZED &&
        p->initState == 4) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

static void CleanupCallback(tBTA_STATUS status) {
    WUDControlBlock* p = &_wcb;

    if (status == BTA_SUCCESS) {
        _initialized = FALSE;
        p->libStatus = WUD_LIB_STATUS_0;
        return;
    }

    DEBUGPrint("WARNING: USB_CLOSE_FAILURE!\n");
}

void WUDSecurityCallback(tBTA_DM_SEC_EVT event, tBTA_DM_SEC* pData) {
    WUDControlBlock* p = &_wcb;
    WUDDevInfo* pInfo;
    u8 i;

    tBTA_DM_ENABLE* pEnable;
    tBTA_DM_LINK_UP* pLinkUp;
    tBTA_DM_LINK_DOWN* pLinkDown;
    tBTA_DM_PIN_REQ* pPinReq;
    tBTA_DM_AUTH_CMPL* pAuthCmpl;

    DEBUGPrint("WUDSecurityCallback: ");

    switch (event) {
    case BTA_DM_ENABLE_EVT: {
        pEnable = &pData->enable;
        WUD_BDCPY(p->hostAddr, pEnable->bd_addr);

        DEBUGPrint("BTA_ENABLE_EVT\n");
        DEBUGPrint("host : %02x:%02x:%02x:%02x:%02x:%02x\n", p->hostAddr[0], p->hostAddr[1], p->hostAddr[2], p->hostAddr[3], p->hostAddr[4],
                   p->hostAddr[5]);

        WUDiEnableStack();
        p->libStatus = WUD_LIB_STATUS_2;
        break;
    }

    case BTA_DM_DISABLE_EVT: {
        DEBUGPrint("BTA_DISABLE_EVT\n");

        BTA_CleanUp(CleanupCallback);
        break;
    }

    case BTA_DM_PIN_REQ_EVT: {
        pPinReq = &pData->pin_req;

        DEBUGPrint("BTA_DM_PIN_REQ_EVT\n");

        WUDiSetPinCode(pPinReq->bd_addr);
        break;
    }

    case BTA_DM_AUTH_CMPL_EVT: {
        pAuthCmpl = &pData->auth_cmpl;

        DEBUGPrint("BTA_DM_AUTH_CMPL_EVT\n");

        DEBUGPrint("  addr : %02x:%02x:%02x:%02x:%02x:%02x\n", pAuthCmpl->bd_addr[0], pAuthCmpl->bd_addr[1], pAuthCmpl->bd_addr[2],
                   pAuthCmpl->bd_addr[3], pAuthCmpl->bd_addr[4], pAuthCmpl->bd_addr[5]);

        // clang-format off
        DEBUGPrint("  key  : %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
                   pAuthCmpl->key[0],  pAuthCmpl->key[1],  pAuthCmpl->key[2],  pAuthCmpl->key[3],
                   pAuthCmpl->key[4],  pAuthCmpl->key[5],  pAuthCmpl->key[6],  pAuthCmpl->key[7],
                   pAuthCmpl->key[8],  pAuthCmpl->key[9],  pAuthCmpl->key[10], pAuthCmpl->key[11],
                   pAuthCmpl->key[12], pAuthCmpl->key[13], pAuthCmpl->key[14], pAuthCmpl->key[15]);
        // clang-format on

        DEBUGPrint("  result = %d\n", pAuthCmpl->key_type);

        if (pAuthCmpl->key_type == HCI_LKEY_TYPE_COMBINATION) {
            break;
        }

        pInfo = WUDiGetDevInfo(pAuthCmpl->bd_addr);

        if (WUD_BDCMP(_work.devAddr, pAuthCmpl->bd_addr) == 0) {
            _work.status = 12;

            if (pInfo == NULL) {
                pInfo = &_work;
            }
        }

        memcpy(pInfo->linkKey, pAuthCmpl->key, LINK_KEY_LEN);
        break;
    }

    case BTA_DM_AUTHORIZE_EVT: {
        DEBUGPrint("BTA_DM_AUTHORIZE_EVT\n");
        break;
    }

    case BTA_DM_LINK_UP_EVT: {
        pLinkUp = &pData->link_up;

        DEBUGPrint("BTA_DM_LINK_UP_EVT\n");

        DEBUGPrint("   addr : %02x:%02x:%02x:%02x:%02x:%02x\n", pLinkUp->bd_addr[0], pLinkUp->bd_addr[1], pLinkUp->bd_addr[2], pLinkUp->bd_addr[3],
                   pLinkUp->bd_addr[4], pLinkUp->bd_addr[5]);

        pInfo = WUDiGetDevInfo(pLinkUp->bd_addr);
        if (pInfo == NULL && WUD_BDCMP(pLinkUp->bd_addr, _work.devAddr) == 0) {
            pInfo = &_work;
        }

        if (pInfo == NULL || WUDiGetLinkNum() == WUD_MAX_CHANNELS) {
            btm_remove_acl(pLinkUp->bd_addr);

            DEBUGPrint("%s --> %02x:%02x:%02x:%02x:%02x:%02x\n", pInfo == NULL ? "not paired" : "4 links exist", pLinkUp->bd_addr[0],
                       pLinkUp->bd_addr[1], pLinkUp->bd_addr[2], pLinkUp->bd_addr[3], pLinkUp->bd_addr[4], pLinkUp->bd_addr[5]);
            break;
        }

        pInfo->status = pInfo->status == 2 ? 12 : 3;
        p->linkedNum++;
        break;
    }

    case BTA_DM_LINK_DOWN_EVT: {
        pLinkDown = &pData->link_down;

        DEBUGPrint("BTA_DM_LINK_DOWN_EVT\n");

        DEBUGPrint("   addr : %02x:%02x:%02x:%02x:%02x:%02x\n", pLinkDown->bd_addr[0], pLinkDown->bd_addr[1], pLinkDown->bd_addr[2],
                   pLinkDown->bd_addr[3], pLinkDown->bd_addr[4], pLinkDown->bd_addr[5]);

        DEBUGPrint("result: %d\n", pLinkDown->status);

        pInfo = WUDiGetDevInfo(pLinkDown->bd_addr);
        if (pInfo != NULL) {
            pInfo->status = 1;
            p->linkedNum--;

            if (WUD_BDCMP(_work.devAddr, pLinkDown->bd_addr) == 0) {
                p->syncState = WUD_STATE_SYNC_ERROR;
            }

            if (pLinkDown->status == HCI_ERR_PEER_POWER_OFF) {
                for (i = 0; i < WUD_MAX_CHANNELS; i++) {
                    if (WUD_BDCMP(_scArray.regist[WUD_MAX_DEV_ENTRY_FOR_STD + i].addr, pLinkDown->bd_addr) == 0) {
                        memset(&_scArray.regist[WUD_MAX_DEV_ENTRY_FOR_STD + i], 0, sizeof(SCBtDeviceInfo));

                        _scFlush = TRUE;
                    }
                }
            }
        } else if (WUD_BDCMP(_work.devAddr, pLinkDown->bd_addr) == 0) {
            p->syncState = WUD_STATE_SYNC_ERROR;
            p->linkedNum--;
        } else {
            DEBUGPrint("this device in not paired\n");
        }

        if (p->linkedNum <= 255 && p->linkedNum >= 250) {
            OSReport("WARNING: link num count is reset.\n");
            p->linkedNum = 0;
        }
        break;
    }

    case BTA_DM_SIG_STRENGTH_EVT: {
        DEBUGPrint("BTA_DM_SIG_STRENGTH_EVT\n");
        break;
    }

    case BTA_DM_BUSY_LEVEL_EVT: {
        DEBUGPrint("BTA_DM_BUSY_LEVEL_EVT\n");
        break;
    }
    }
}

void WUDSearchCallback(tBTA_DM_SEARCH_EVT event, tBTA_DM_SEARCH* pData) {
    s32 timeout;

    switch (event) {
    case BTA_DM_INQ_RES_EVT: {
        tBTA_DM_INQ_RES* pResp = &pData->inq_res;

        // clang-format off
        DEBUGPrint("INQUIRY RESULT: %02x:%02x:%02x:%02x:%02x:%02x   %02x%02x%02x   %d\n",
                   pResp->bd_addr[0], pResp->bd_addr[1], pResp->bd_addr[2],
                   pResp->bd_addr[3], pResp->bd_addr[4], pResp->bd_addr[5],
                   pResp->dev_class[0], pResp->dev_class[1], pResp->dev_class[2],
                   pResp->rssi);
        // clang-format on

        _discRssi = pResp->rssi;

        if (_wcb.syncSkipChecks == TRUE || (_wcb.syncSkipChecks == FALSE && WUDiGetLinkNum() < WUD_MAX_CHANNELS - 1)) {
            timeout = 4800;
        } else {
            timeout = 32768;
        }

        BTM_WritePageTimeout(timeout);
        break;
    }

    case BTA_DM_INQ_CMPL_EVT: {
        DEBUGPrint("INQUIRY_COMPLETED\n");
        break;
    }

    case BTA_DM_DISC_RES_EVT: {
        tBTA_DM_DISC_RES* dmDiscRes = &pData->disc_res;

        WUD_BDCPY(_discResp.devAddr, dmDiscRes->bd_addr);
        memcpy(_discResp.devName, dmDiscRes->bd_name, sizeof(_discResp.devName));

        _discResp.services = dmDiscRes->services;
        _discNumResps++;

        // clang-format off
        DEBUGPrint("DISCOVER RESULT:  %02x:%02x:%02x:%02x:%02x:%02x   %s (%04x)\n",
                   _discResp.devAddr[0], _discResp.devAddr[1], _discResp.devAddr[2],
                   _discResp.devAddr[3], _discResp.devAddr[4], _discResp.devAddr[5],
                   _discResp.devName, _discResp.services);
        // clang-format on
        break;
    }

    case BTA_DM_DISC_CMPL_EVT: {
        DEBUGPrint("DISCOVER COMPLETED\n");

        _wcb.syncState = WUD_STATE_SYNC_CHECK_SEARCH_RESULT;
        break;
    }

    case BTA_DM_SEARCH_CANCEL_CMPL_EVT: {
        DEBUGPrint("SEARCH CANCEL\n\n");

        _discNumResps = 0;
        memset(&_discResp, 0, sizeof(WUDDiscResp));

        _wcb.syncState = WUD_STATE_SYNC_CHECK_SEARCH_RESULT;
        break;
    }

    default: {
        DEBUGPrint("Warning: Search Callback returns invalid event\n");
        break;
    }
    }
}

void WUDVendorSpecificCallback(UINT8 len, UINT8* pData) {
#pragma unused(len)

    u8 event = pData[0];
    WUDControlBlock* p = &_wcb;

    switch (event) {
    case WUD_VSE_INITIATE_PAIRING: {
        DEBUGPrint("VSE:- INITIATE_PAIRING\n");
        WUDiAutoSync();
        break;
    }

    case WUD_VSE_DELETE_ALL_KEYS: {
        DEBUGPrint("VSE:- DELETE_ALL_KEYS\n");
        WUDiDeleteAllLinkKeys();
        break;
    }

    case WUD_VSE_SI_PORT_STATUS: {
        DEBUGPrint("VSE:- SI_PORT_STATUS  status = %d\n", pData[1]);
        p->serialPortStatus = pData[1];
        break;
    }

    case WUD_VSE_WATCH_DOG_RESET_HW: {
        DEBUGPrint("VSE:- WATCH_DOG_RESET  HW error = %d\n", pData[1]);

        // clang-format off
        OSPanic(__FILE__, 3484, "MODULE FATAL ERROR\n");
        // clang-format on
        break;
    }
    }
}

void WUDDeviceStatusCallback(tBTM_DEV_STATUS status) {
    DEBUGPrint("WUDDeviceStatusCallback\n");

    if (status == BTM_DEV_STATUS_CMD_TOUT) {
        OSReport("---- WARNING: USB FATAL ERROR! ----\n");
    }
}

void WUDStoredLinkKeyCallback(void* p1) {
    tBTM_BD_ADDR_LINK_KEY_PAIR* pPair;
    WUDControlBlock* p = &_wcb;
    WUDDevInfo* pInfo;
    int i;

    tBTM_STORED_LINK_KEYS_EVT* pData = p1;
    tBTM_READ_STORED_LINK_KEY_COMPLETE* pRead;
    tBTM_WRITE_STORED_LINK_KEY_COMPLETE* pWrite;
    tBTM_DELETE_STORED_LINK_KEY_COMPLETE* pDelete;
    tBTM_RETURN_LINK_KEYS_EVT* pReturn;

    switch (pData->event) {
    case BTM_CB_EVT_RETURN_LINK_KEYS: {
        pReturn = p1;

        DEBUGPrint("BTM_CB_EVT_RETURN_LINK_KEYS\n");

        pPair = (tBTM_BD_ADDR_LINK_KEY_PAIR*)(pReturn + 1);
        i = 0;

        while (i < pReturn->num_keys) {
            pInfo = WUDiGetDevInfo(pPair->bd_addr);

            DEBUGPrint("BD_ADDR:  %02x:%02x:%02x:%02x:%02x:%02x\n", pPair->bd_addr[0], pPair->bd_addr[1], pPair->bd_addr[2], pPair->bd_addr[3],
                       pPair->bd_addr[4], pPair->bd_addr[5]);

            // clang-format off
            DEBUGPrint("LINKKEY: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
                       pPair->link_key[0],  pPair->link_key[1],  pPair->link_key[2],  pPair->link_key[3],
                       pPair->link_key[4],  pPair->link_key[5],  pPair->link_key[6],  pPair->link_key[7],
                       pPair->link_key[8],  pPair->link_key[9],  pPair->link_key[10], pPair->link_key[11],
                       pPair->link_key[12], pPair->link_key[13], pPair->link_key[14], pPair->link_key[15]);
            // clang-format on

            if (pInfo == NULL) {
                DEBUGPrint("this device is not stored in NAND\n");

                pInfo = WUDiGetNewDevInfo();

                if (pInfo == NULL) {
                    for (i = 0; i < WUD_MAX_DEV_ENTRY_FOR_SMP; i++) {
                        if (p->smpDevs[i].status == 0) {
                            pInfo = &p->smpDevs[i];
                            break;
                        }
                    }
                }

                if (pInfo) {
                    pInfo->UNK_0x5C = 1;
                    pInfo->status = 1;

                    WUD_BDCPY(pInfo->devAddr, pPair->bd_addr);
                    memcpy(pInfo->linkKey, pPair->link_key, LINK_KEY_LEN);
                }
            } else {
                WUD_BDCPY(pInfo->devAddr, pPair->bd_addr);
                memcpy(pInfo->linkKey, pPair->link_key, LINK_KEY_LEN);

                if (pInfo->UNK_0x5C == 2) {
                    pInfo->UNK_0x5C = 3;
                }

                WUD_BDCPY(p->pairAddr, pPair->bd_addr);
                DEBUGPrint("   LAST: %02x:%02x:%02x:%02x:%02x:%02x\n", p->pairAddr[0], p->pairAddr[1], p->pairAddr[2], p->pairAddr[3], p->pairAddr[4],
                           p->pairAddr[5]);
            }

            pPair++;
            i++;
        }

        break;
    }

    case BTM_CB_EVT_READ_STORED_LINK_KEYS: {
        pRead = p1;

        DEBUGPrint("BTM_CB_EVT_READ_STORED_LINK_KEYS\n");
        DEBUGPrint("  status: %d  max_keys: %d  num_keys: %d\n", pRead->status, pRead->max_keys, pRead->read_keys);

        p->linkKeyState = WUD_STATE_LINK_KEY_START;
        break;
    }

    case BTM_CB_EVT_WRITE_STORED_LINK_KEYS: {
        pWrite = p1;

        DEBUGPrint("BTM_CB_EVT_WRITE_STORED_LINK_KEYS\n");
        DEBUGPrint("  status: %d  num_keys: %d\n", pWrite->status, pWrite->num_keys);

        p->linkKeyState = WUD_STATE_LINK_KEY_START;
        break;
    }

    case BTM_CB_EVT_DELETE_STORED_LINK_KEYS: {
        pDelete = p1;

        DEBUGPrint("BTM_CB_EVT_DELETE_STORED_LINK_KEYS\n");
        DEBUGPrint("  status: %d  num_keys: %d\n", pDelete->status, pDelete->num_keys);

        if (pDelete->num_keys == 0) {
            DEBUGPrint("WARNING: no entry is deleted\n");
        }

        p->linkKeyState = WUD_STATE_LINK_KEY_START;
        break;
    }

    default: {
        // clang-format off
        OSPanic(__FILE__, 3636, "Unknown event\n");
        // clang-format on
    }
    }
}

void WUDPowerManagerCallback(BD_ADDR addr, tBTM_PM_STATUS status, UINT16 value, UINT8 hciStatus) {
#pragma unused(value)

    WUDDevInfo* pInfo;

    DEBUGPrint("WUDPowerManagerCallback\n");
    DEBUGPrint("hci_status = %d", hciStatus);

    pInfo = WUDiGetDevInfo(addr);
    if (pInfo == NULL) {
        if (WUD_BDCMP(_work.devAddr, addr) == 0) {
            pInfo = &_work;
        }
    }

    switch (status) {
    case BTM_PM_STS_ACTIVE: {
        pInfo->status = 8;
        break;
    }

    case BTM_PM_STS_SNIFF: {
        pInfo->status = 9;
        break;
    }
    }

    DEBUGPrint(" addr = %02x:%02x:%02x:%02x:%02x:%02x,  status = %d\n", pInfo->devAddr[0], pInfo->devAddr[1], pInfo->devAddr[2], pInfo->devAddr[3],
               pInfo->devAddr[4], pInfo->devAddr[5], pInfo->status);
}

BD_ADDR_PTR _WUDGetDevAddr(UINT8 handle) {
    BD_ADDR_PTR pAddr;
    BOOL enabled = OSDisableInterrupts();

    if (handle < WUD_MAX_DEV_ENTRY) {
        pAddr = _dev_handle_to_bda[handle];

    } else {
        pAddr = NULL;
    }

    OSRestoreInterrupts(enabled);
    return pAddr;
}

u16 _WUDGetQueuedSize(s8 dev_handle) {
    u16 queuedSize;
    BOOL intrStatus = OSDisableInterrupts();

    if (dev_handle >= 0 && dev_handle < WUD_MAX_DEV_ENTRY) {
        queuedSize = WUDiGetQueueSizeForHandle(dev_handle);
    } else {
        queuedSize = 0;
    }

    OSRestoreInterrupts(intrStatus);
    return queuedSize;
}

u16 _WUDGetNotAckedSize(s8 dev_handle) {
    u16 notAckedSize;
    BOOL intrStatus = OSDisableInterrupts();

    if (dev_handle >= 0 && dev_handle < WUD_MAX_DEV_ENTRY) {
        notAckedSize = WUDiGetNotAckNumForHandle(dev_handle);
    } else {
        notAckedSize = 0;
    }

    OSRestoreInterrupts(intrStatus);
    return notAckedSize;
}

u8 _WUDGetLinkNumber(void) {
    WUDControlBlock* p = &_wcb;
    BOOL enabled = OSDisableInterrupts();

    u8 num = p->linkedNum;

    OSRestoreInterrupts(enabled);
    return num;
}

// not sure if this is a fakematch or an actual version difference of some kind
#if SDK_AUG2010
inline u16 WUDiGetQueueSizeForHandle(u8 dev_handle) {
#else
inline u16 WUDiGetQueueSizeForHandle(u32 dev_handle) {
#endif
    return _dev_handle_queue_size[dev_handle];
}

// see comment for WUDiGetQueueSizeForHandle
#if SDK_AUG2010
inline u16 WUDiGetNotAckNumForHandle(u8 dev_handle) {
#else
inline u16 WUDiGetNotAckNumForHandle(u32 dev_handle) {
#endif
    return _dev_handle_notack_num[dev_handle];
}

void _WUDEnableTestMode() {
    DEBUGPrint("_WUDEnableTestMode\n");
}

void _WUDStartSyncDevice() {
    DEBUGPrint("_WUDStartSyncDevice()\n");
}

void _WUDDeleteStoreDevice() {
    DEBUGPrint("_WUDDeleteStoreDevice()\n");
    DEBUGPrint("dev number = %d\n", 0);
}
