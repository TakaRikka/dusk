#include <dolphin.h>
#include <dolphin/mcc.h>

#if DEBUG
const char* __MCCVersion = "<< Dolphin SDK - MCC\tdebug build: Apr  5 2004 03:57:07 (0x2301) >>";
#else
const char* __MCCVersion = "<< Dolphin SDK - MCC\trelease build: Apr  5 2004 04:15:49 (0x2301) >>";
#endif

static MCC_ChannelInfo gChannelInfo[16] ATTRIBUTE_ALIGN(32);
static char gStreamWork[32] ATTRIBUTE_ALIGN(32);
static char m_szAdapterMode[32] ATTRIBUTE_ALIGN(32);
static char m_szInitCode[32] ATTRIBUTE_ALIGN(32);
static MCC_Info channelInfo[16] ATTRIBUTE_ALIGN(32);

volatile static BOOL gIsChannelinfoDirty = TRUE;

static void (*volatile gCallbackSysEvent)(MCC_SYSEVENT);
static BOOL gOtherSideInitDone;
volatile static u8 gLastError;
static BOOL gMccInitialized;
static int gMccSession;
volatile static int gPingFlag;
volatile static u16 gAsyncResourceStatus;

// prototypes
static void mccDebugPrint(char* str);
static void callbackEventStream(MCC_CHANNEL chID, u32 event, u32 value);
static int SetUsbAdapterMode(u8 mode);
static u8 GetUsbAdapterMode(void);
static int InitializeCodeClear(void);
static int InitializeCodeSet(void);
static int InitializeCodeCheck(void);
static void AsyncResourceClearState(void);
static void AsyncResourceSetState(u16 state);
static void AsyncResourceStateBusy(u8 channel, u16 mode);
static void AsyncResourceStateDone(void);
static u16 AsyncResourceGetStat(void);
static u16 AsyncResourceGetMode(void);
static u8 AsyncResourceGetChannel(void);
static int AsyncResourceIsBusy(void);
static int LoadChannelInfo(MCC_ChannelInfo* info);
static int FlushChannelInfo(MCC_ChannelInfo* info);
static void SetChannelInfoDirty(int dirty);
static void ClearChannelInfo(int i);
static void MakeMemoryMap(u8* map);
static BOOL IsChannelOpened(MCC_CHANNEL chID);
static u8 SearchFreeBlocks(MCC_MODE mode, u8* index);
static int NotifyCompulsorily(MCC_CHANNEL chID, u32 notify, u32 timeout);
static int NotifyInit(void);
static int NotifyInitDone(void);
static int NotifyChannelEvent(MCC_CHANNEL chID, u32 notify);
static int WaitAMinute(int timeout, volatile int* flag, int value);
static void MailboxCheck(void);
static void MCCExiCallback(void);
static void MCCTxCallback(void);
static void MCCRxCallback(void);
static int mccInitializeCheck(u8 timeout);

static void mccDebugPrint(char* str) {}

static void callbackEventStream(MCC_CHANNEL chID, u32 event, u32 value) {
    value; // needed to bump registers. what?

    if (event == MCC_EVENT_READ) {
        gChannelInfo[chID].isStreamDone = TRUE;
    }
    if (event == MCC_EVENT_WRITE) {
        gChannelInfo[chID].isStreamDone = TRUE;
    }
    if (event == MCC_EVENT_CONNECT) {
        gChannelInfo[chID].isStreamConnection = TRUE;
    }
    if (event == MCC_EVENT_DISCONNECT) {
        gChannelInfo[chID].isStreamConnection = TRUE;
    }
    if (event == MCC_EVENT_UNK_0x100) {
        gChannelInfo[chID].isStreamConnection = TRUE;
    }
}

BOOL MCCStreamOpen(MCC_CHANNEL chID, u8 blockSize) {
    BOOL bResult;

    bResult = FALSE;
    if (MCCOpen(chID, blockSize, callbackEventStream)) {
        gChannelInfo[chID].isStreamOpened = 0;
        gChannelInfo[chID].isStreamConnection = 0;
        gChannelInfo[chID].isStreamDone = 0;
        bResult = TRUE;
    }

    return bResult;
}

int MCCStreamClose(MCC_CHANNEL chID) {
    gChannelInfo[chID].isStreamOpened = FALSE;
    return MCCClose(chID);
}

int MCCStreamWrite(MCC_CHANNEL chID, void* data, u32 dataBlockSize) {
    MCC_Info chanInfo;
    char* dataAddress;
    u32 lastBlocks;

    if (!gMccInitialized) {
        gLastError = 1;
    } else if (chID < 1 || chID >= 16) {
        gLastError = 14;
    } else if (LoadChannelInfo(gChannelInfo) == 0) {
        gLastError = 11;
    } else {
        if (gChannelInfo[chID].isStreamOpened != TRUE) {
            gChannelInfo[chID].isStreamOpened = TRUE;
        } else {
            MCCNotify(chID, 0);
        }

        if (WaitAMinute(5, &gChannelInfo[chID].isStreamConnection, 1) == 0) {
            gLastError = 2;
        } else {
            gChannelInfo[chID].isStreamConnection = 0;
            if (MCCGetChannelInfo(chID, &chanInfo) != 0) {
                *(u32*)&gStreamWork = dataBlockSize;
                if (MCCWrite(chID, 0, gStreamWork, 0x20, 0) != 0) {
                    if (WaitAMinute(5, &gChannelInfo[chID].isStreamDone, 1) == 0) {
                        gLastError = 2;
                    } else {
                        dataAddress = data;
                        lastBlocks = dataBlockSize;
                        gChannelInfo[chID].isStreamDone = FALSE;
                        while (lastBlocks) {
                            if (!MCCWrite(chID, 0, dataAddress, chanInfo.blockLength << 0xD, 0)) {
                                break;
                            }

                            if (WaitAMinute(5, &gChannelInfo[chID].isStreamDone, 1) == 0) {
                                gLastError = 2;
                                break;
                            } else {
                                gChannelInfo[chID].isStreamDone = 0;
                                dataAddress += chanInfo.blockLength << 0xD;
                                if (lastBlocks > chanInfo.blockLength) {
                                    lastBlocks -= chanInfo.blockLength;
                                } else {
                                    lastBlocks = 0;
                                    break;
                                }
                            }
                        }

                        return lastBlocks == 0;
                    }
                }
            }
        }
    } 

    return 0;
}

u32 MCCStreamRead(MCC_CHANNEL chID, void* data) {
    MCC_Info chanInfo;
    char* dataAddress;
    u32 allBlocks;
    u32 lastBlocks;

    if (!gMccInitialized) {
        gLastError = 1;
    } else if (chID < 1 || chID >= 0x10) {
        gLastError = 0xE;
    } else if (LoadChannelInfo(gChannelInfo) == 0) {
        gLastError = 0xB;
    } else {
        if (gChannelInfo[chID].isStreamOpened != TRUE) {
            gChannelInfo[chID].isStreamOpened = TRUE;
        } else {
            MCCNotify(chID, 0);
        }

        if (WaitAMinute(5, &gChannelInfo[chID].isStreamConnection, 1) == 0) {
            gLastError = 2;
        } else {
            gChannelInfo[chID].isStreamConnection = FALSE;
            if (MCCGetChannelInfo(chID, &chanInfo) != 0) {
                if (WaitAMinute(5, &gChannelInfo[chID].isStreamDone, 1) == 0) {
                    gLastError = 2;
                } else {
                    gChannelInfo[chID].isStreamDone = FALSE;
                    if (MCCRead(chID, 0, gStreamWork, 0x20, 0) != 0) {
                        dataAddress = data;
                        allBlocks = lastBlocks = *(u32*)&gStreamWork[0];
                        while (lastBlocks) {
                            if (WaitAMinute(5, &gChannelInfo[chID].isStreamDone, 1) == 0) {
                                gLastError = 2;
                                break;
                            }

                            gChannelInfo[chID].isStreamDone = FALSE;
                            if (MCCRead(chID, 0, dataAddress, (lastBlocks > chanInfo.blockLength) ? chanInfo.blockLength << 0xD : lastBlocks << 0xD, 0) != 0) {
                                dataAddress += chanInfo.blockLength << 0xD;
                                if (lastBlocks > chanInfo.blockLength) {
                                    lastBlocks -= chanInfo.blockLength;
                                } else {
                                    lastBlocks = 0;
                                    break;
                                }
                            } else {
                                break;
                            }
                        }
                        return allBlocks - lastBlocks;
                    }
                }
            }
        }
    }

    return 0;
}

static int SetUsbAdapterMode(u8 mode) {
    int result = 0;

    if (HIORead(0x680, m_szAdapterMode, 0x20) != 0) {
        DCInvalidateRange(m_szAdapterMode, 0x20);
        m_szAdapterMode[0] = mode;
        DCFlushRange(m_szAdapterMode, 0x20);
        if (HIOWrite(0x680, m_szAdapterMode, 0x20) != 0) {
            result = 1;
        }
    }
    return result;
}

static u8 GetUsbAdapterMode(void) {
    if (HIORead(0x680, m_szAdapterMode, 0x20) != 0) {
        DCInvalidateRange(m_szAdapterMode, 0x20);
        return m_szAdapterMode[0];
    }
    return 0;
}

static int InitializeCodeClear(void) {
    memset(m_szInitCode, 0, 0x20);
    DCFlushRange(m_szInitCode, 0x20);
    HIOWrite(0x600, m_szInitCode, 0x20);
}

static int InitializeCodeSet(void) {
    strcpy(m_szInitCode, "HUDSON/USB2EXI/INITCODE/TARGET");
    DCFlushRange(m_szInitCode, 0x20);
    HIOWrite(0x600, m_szInitCode, 0x20);    
}

static int InitializeCodeCheck(void) {
    int result;

    if ((result = HIORead(0x600, m_szInitCode, 0x20)) != 0) {
        DCInvalidateRange(m_szInitCode, 0x20);
        result = strcmp(m_szInitCode, "HUDSON/USB2EXI/INITCODE/HOST");
        return result == 0;
    }
    return result;
}

static void AsyncResourceClearState(void) {
    gAsyncResourceStatus = 0;
}

static void AsyncResourceSetState(u16 state) {
    gAsyncResourceStatus &= 0xFFFF0FFF;
    gAsyncResourceStatus |= state;    
}

static void AsyncResourceStateBusy(u8 channel, u16 mode) {
    AsyncResourceClearState();
    AsyncResourceSetState(0x1000);
    gAsyncResourceStatus |= channel;
    gAsyncResourceStatus |= mode;
}

static void AsyncResourceStateDone(void) {
    AsyncResourceSetState(0x2000);
}

static u16 AsyncResourceGetStat(void) {
    return gAsyncResourceStatus & 0xF000;
}

static u16 AsyncResourceGetMode(void) {
    return gAsyncResourceStatus & 0xF00;
}

static u8 AsyncResourceGetChannel(void) {
    return gAsyncResourceStatus;
}

static int AsyncResourceIsBusy(void) {
    return AsyncResourceGetStat() & 0x1000;
}

static int LoadChannelInfo(MCC_ChannelInfo* info) {
    volatile int result = 0;
    u8 count;
#if !DEBUG
    int unused; // this is fake, but i cant seem to find whats messing with the stack.
#endif

    if (!gIsChannelinfoDirty) {
        result = 1;
    } else {
        count = 0;
        mccDebugPrint("+++ Load channel info.");
        while ((result = HIORead(0x700, channelInfo, 0x40)) != 1) {
            count -= 1;
            if (count == 0) {
                break;
            }
        }

        if (result) {
            DCInvalidateRange(channelInfo, 0x40);
            for(count = 0; count < 16; count++) {
                info[count].info = channelInfo[count];
            }
            SetChannelInfoDirty(0);
        }
    }

    return result;
}

static int FlushChannelInfo(MCC_ChannelInfo* info) {
    volatile int result;
    u8 count;

    result = 0;
    for(count = 0; count < 16; count++) {
        channelInfo[count] = info[count].info;
    }

    DCFlushRange(channelInfo, 0x40);
    while ((result = HIOWrite(0x700, channelInfo, 0x40)) != 1) {
        count -= 1;
        if (count == 0) {
            break;
        }
    }

    if (result != 0) {
        SetChannelInfoDirty(1);
        result = NotifyCompulsorily(0, 5, 10);
    }
    return result;
}

static void SetChannelInfoDirty(int dirty) {
    gIsChannelinfoDirty = dirty;
}

static void ClearChannelInfo(int i) {
    gChannelInfo[i].info.firstBlock = 0;
    gChannelInfo[i].info.blockLength = 0;
    gChannelInfo[i].info.connect = 0;
    gChannelInfo[i].info.isLocked = FALSE;
    gChannelInfo[i].eventMask = 0;
    gChannelInfo[i].callbackEvent = NULL;
    gChannelInfo[i].isStreamDone = FALSE;
    //! isStreamConnection isnt cleared. Intentional?
}

static void MakeMemoryMap(u8* map) {
    u8 iMap;
    u8 jMap;

    memset(map, 0, 0x10);
    for (iMap = 0; iMap < 16; iMap++) {
        if (gChannelInfo[iMap].info.connect) {
            for (jMap = 0; jMap < gChannelInfo[iMap].info.blockLength; jMap++) {
                if (jMap + gChannelInfo[iMap].info.firstBlock < 0x10) {
                    map[gChannelInfo[iMap].info.firstBlock + jMap] = iMap + 1;
                } else {
                    gLastError = 0xD;
                }
            }
        }
    }

    *map = 0xFF;
}

static int IsChannelOpened(MCC_CHANNEL chID) {
    u8 connectSide;

    if (chID <= 0 || chID >= 0x10) {
        gLastError = 0xE;
        goto exit;
    }
    connectSide = 2;

    return (connectSide & gChannelInfo[chID].info.connect) ? TRUE : FALSE;

exit:;
    return 0;
}

static u8 SearchFreeBlocks(MCC_MODE mode, u8* index) {
    u8 map[16];
    u8 iMap;
    u8 fIndex;
    u8 fSize;
    u8 fCount;

    MakeMemoryMap(map);
    fCount = 0;
    fIndex = 0;
    fSize = (mode == 0) ? 0x10 : 0;

    //! @bug I think this will read OOB of the map array by 1.
    for (iMap = 0; iMap <= 16; iMap++) {
        if (map[iMap] || iMap == 16) {
            if (fCount != 0) {
                if (mode == 0) {
                    if (fSize > fCount) {
                        fSize = fCount;
                        fIndex = iMap - fCount;
                    }
                } else if (mode == 1) {
                    if (fSize < fCount) {
                        fSize = fCount;
                        fIndex = iMap - fCount;
                    }
                } else if (mode == 2) {
                    fSize += fCount;
                }
                fCount = 0;
            }
        } else {
            fCount += 1;
        }
    }

    if (index) {
        *index = fIndex;
    }
    return fSize;
}

static int NotifyCompulsorily(MCC_CHANNEL chID, u32 notify, u32 timeout) {
    u32 status;
    u32 notifyData;
    volatile u32 tickStart;
    volatile u32 tickCur;
    volatile u32 tickSec;
#if !DEBUG
    int unused; // fake but blah
#endif

    status = 0;
    tickStart = OSGetTick();

    notifyData = (chID << 0x18);
    notifyData |= (notify & 0x10000000);
    notifyData |= (notify & 0xFFFFFF);

    while (1) {
        if (!HIOReadStatus(&status)) {
            mccDebugPrint("ERROR:HIOReadStatus\n");
        }

        if ((status & 2) == 0) {
            break;
        }

        tickCur = OSGetTick();
        tickSec = (tickStart < tickCur) ? tickCur - tickStart : (-1 - tickStart) + tickCur;
        tickSec = OSTicksToSeconds(tickSec);
        if (timeout == 0 || tickSec > timeout) {
            break;
        }
    }

    if (!HIOWriteMailbox(notifyData)) {
        gLastError = 6;
        goto exit;
    }
    return 1;

exit:;
    return 0;
}

static int NotifyInit(void) {
    return NotifyCompulsorily(0, 1, 0);
}

static int NotifyInitDone(void) {
    return NotifyCompulsorily(0, 2, 0);
}

static int NotifyChannelEvent(MCC_CHANNEL chID, u32 notify) {
#if !DEBUG
    int unused[2]; // fake but blah
#endif

    if (LoadChannelInfo(gChannelInfo) == 0) {
        mccDebugPrint("Error:Could not update channelInfo.\n");
        gLastError = 0xB;
    } else if (!IsChannelOpened(chID)) {
        mccDebugPrint("Error:Channel is not opened.\n");
        gLastError = 0x12;
    } else if (NotifyCompulsorily(chID, notify, 10) != 0) {
        return 1;
    }

    return 0;
}

static int WaitAMinute(int timeout, volatile int*  flag, int value) {
    u32 tickStart;
    u32 tickDist;

    tickStart = OSGetTick();
    while (*flag != value) {
        tickDist = OSGetTick() - tickStart;
        tickDist = (tickDist & 0x80000000) ? (0x80000000 - tickStart) + OSGetTick() : tickDist;
        if (OSTicksToSeconds(tickDist) >= timeout) {
            mccDebugPrint("Error:Time is over.\n");
            return 0;
        }
    }
    return 1;
}

static void MailboxCheck(void) {
    u32 mailbox;
    BOOL isNotify;
    u8 chID;
    u32 value;
    BOOL bDoCall;
#if !DEBUG
    int unused[3]; // fake but blah
#endif

    mailbox = 0;
    if (HIOReadMailbox(&mailbox) == 0) {
        mccDebugPrint("Error:Could not read mailbox.\n");
        gLastError = 5;
        return;
    }

    isNotify = (mailbox & 0x10000000) != 0;
    chID = (mailbox >> 0x18U) & 0xF;
    value = (mailbox & 0xFFFFFF);

    if (chID == 0) {
        bDoCall = TRUE;
        switch(value) {
        case 2:                                     
            gMccInitialized = TRUE;
            gMccSession = 1;
            gOtherSideInitDone = TRUE;
            break;
        case 3:                                     
            NotifyCompulsorily(0, 4, 10);
            break;
        case 1:                                     
            gMccSession = 0;
            break;
        case 4:                                     
            if (gPingFlag == 0) {
                bDoCall = FALSE;
            }
            gPingFlag = 0;
            break;
        case 5:                                     
            SetChannelInfoDirty(1);
            break;
        default:                                    
            if (value == 8) {
                bDoCall = FALSE;
            } else {
                value = 0;
            }
            break;
        }

        if (bDoCall && gCallbackSysEvent) {
            gCallbackSysEvent(value);
        }
    } else {
        if (LoadChannelInfo(gChannelInfo) == 0) {
            mccDebugPrint("Error:Could not update channelInfo.\n");
            gLastError = 0xB;
            return;
        }

        if (IsChannelOpened(chID)) {
            if (!!isNotify) {
                if (gChannelInfo[chID].callbackEvent) {
                    gChannelInfo[chID].callbackEvent(chID, MCC_EVENT_UNK_0x100, value);
                }
            } else {
                switch(value) {
                case MCC_EVENT_READ_DONE_INSIDE:
                case MCC_EVENT_WRITE_DONE_INSIDE:
                    mccDebugPrint("ERROR: MCC_EVENT_READ_DONE_INSIDE / MCC_EVENT_WRITE_DONE_INSIDE received.");
                    break;
                case MCC_EVENT_CONNECT:
                case MCC_EVENT_DISCONNECT:
                case MCC_EVENT_LOCK:
                case MCC_EVENT_UNLOCK:
                case MCC_EVENT_READ:
                case MCC_EVENT_WRITE:
                    if (gChannelInfo[chID].callbackEvent) {
                        gChannelInfo[chID].callbackEvent(chID, value, 0);
                    }
                    break;
                default:
                    if (gChannelInfo[chID].callbackEvent) {
                        gChannelInfo[chID].callbackEvent(chID, 0, 0);
                    }
                    break;
                }
            }
        }
    }
}

static void MCCExiCallback(void) {
    MailboxCheck();
}

static void MCCTxCallback(void) {
    AsyncResourceStateDone();
}

static void MCCRxCallback(void) {
    AsyncResourceStateDone();
}

static int mccInitializeCheck(u8 timeout) {
    int dmyFlag;
    int i;
#if !DEBUG
    int unused[3]; // fake but blah
#endif

    dmyFlag = 0;
    if (InitializeCodeCheck() == 0) {
        if (gMccInitialized) {
            if (gMccSession == 0) {
                SetChannelInfoDirty(1);
                for (i = 0; i < 16; i++) {
                    ClearChannelInfo(i);
                }

                if (FlushChannelInfo(gChannelInfo) == 0) {
                    mccDebugPrint("Error:Could not flush channelInfo.\n");
                    gLastError = 0xA;
                    return 0;
                }
            }
            return 1;
        }

        InitializeCodeSet();
        if (FlushChannelInfo(gChannelInfo) == 0) {
            mccDebugPrint("Error:Could not flush channelInfo.\n");
            gLastError = 0xA;
        } else if (timeout != 0 && WaitAMinute(timeout, &gOtherSideInitDone, 1) == 0) {
            InitializeCodeClear();
            mccDebugPrint("Error:Time is over.\n");
            gLastError = 2;
            return 0;
        } else {
            return 1;
        }
    } else {
        InitializeCodeClear();
        if (NotifyInitDone() == 0) {
            gLastError = 4;
        } else {
            if (gCallbackSysEvent) {
                gCallbackSysEvent(2);
            }
            gMccInitialized = TRUE;
            gMccSession = 1;
            return 1;
        }
    }
    return 0;
}

int MCCInit(MCC_EXI exiChannel, u8 timeout, MCC_CBSysEvent callbackSysEvent) {
    int dmyFlag;
    u8 adapterMode;
    u32 mailbox;
    u32 status;
    int i;
#if !DEBUG
    int unused[3]; // fake but blah
#endif

    OSRegisterVersion(__MCCVersion);

    mccDebugPrint("MCCInit\n");
    if (gMccInitialized) {
        SetChannelInfoDirty(1);
        return mccInitializeCheck(timeout);
    }

    if (!(exiChannel == 0 || exiChannel == 1 || exiChannel == 2)) {
        mccDebugPrint("[MCC] Error: Exi channel is out of range.\n");
        gLastError = 4;
        return 0;
    }

    if (HIOInit(exiChannel, MCCExiCallback) == 0) {
        mccDebugPrint("Error:Initialized Host I/O\n");
        gLastError = 4;
    } else {
        dmyFlag = 0;
        adapterMode = GetUsbAdapterMode();
        adapterMode = SetUsbAdapterMode(1);
        mailbox = 0;
        status = 0;

        if (HIOReadStatus(&status) != 0 && status & 1) {
            HIOReadMailbox(&mailbox);
        }

        WaitAMinute(1, &dmyFlag, 1);
        if (NotifyInit() == 0) {
            gLastError = 4;
        } else {
            gCallbackSysEvent = callbackSysEvent;
            gLastError = 0;
            SetChannelInfoDirty(1);
            for (i = 0; i < 16; i++) {
                ClearChannelInfo(i);
            }
            AsyncResourceClearState();
            return mccInitializeCheck(timeout);
        }
    }

    return 0;
}

void MCCExit(void) {
    u8 chID;

    if (!gMccInitialized) {
        gLastError = 1;
    } else {
        mccDebugPrint("MCCExit\n");
        for (chID = 1; chID < 16; chID++) {
            if (IsChannelOpened(chID)) {
                MCCClose(chID);
            }
        }
        gLastError = 0;
    }

    gMccInitialized = FALSE;
    gMccSession = 0;
}

int MCCPing(void) {
    mccDebugPrint("MCCPing\n");
    if (!gMccInitialized) {
        gLastError = 1;
    } else {
        gPingFlag = 1;
        gLastError = 0;
        return NotifyCompulsorily(0, 3, 10);
    }
    return 0;
}

int MCCEnumDevices(MCC_CBEnumDevices callbackEnumDevices) {
    if (callbackEnumDevices == NULL) {
        gLastError = 0xD;
    }

    if (HIOEnumDevices(callbackEnumDevices) == 0) {
        gLastError = 0xD;
    } else {
        gLastError = 0;
        return 1;
    }

    return 0;
}

u8 MCCGetFreeBlocks(MCC_MODE mode) {
#if !DEBUG
    int unused[3]; // fake but blah
#endif

    mccDebugPrint("MCCGetFreeBlocks\n");
    if (!gMccInitialized) {
        gLastError = 1;
    } else if (!(mode == 0 || mode == 1 || mode == 2)) {
        gLastError = 0xD;
    } else {
        if (LoadChannelInfo(gChannelInfo) == 0) {
            mccDebugPrint("Error:Could not update channelInfo.\n");
            gLastError = 0xB;
        } else {
            gLastError = 0;
            return SearchFreeBlocks(mode, NULL);
        }
    }

    return 0;
}

u8 MCCGetLastError(void) {
    mccDebugPrint("MCCGetFreeBlocks\n"); //! wrong print?
    return gLastError;
}

int MCCGetChannelInfo(MCC_CHANNEL chID, MCC_Info* info) {
#if !DEBUG
    int unused[3]; // fake but blah
#endif

    mccDebugPrint("MCCGetChannelInfo\n");
    if (!gMccInitialized) {
        gLastError = 1;
    } else if (chID <= 0 || chID >= 0x10) {
        gLastError = 0xE;
    } else if (!info) {
        mccDebugPrint("Error:Bad parameter channelInfo.\n");
        gLastError = 0xD;
    } else {
        if (LoadChannelInfo(gChannelInfo) == 0) {
            mccDebugPrint("Error:Could not update channelInfo.\n");
            gLastError = 0xB;
        } else {
            memcpy(info, &gChannelInfo[chID].info, sizeof(MCC_Info));
            gLastError = 0;
            return 1;
        }
    }

    return 0;
}

int MCCGetConnectionStatus(MCC_CHANNEL chID, MCC_CONNECT* connect) {
    MCC_Info info;
#if !DEBUG
    int unused[2]; // fake but blah
#endif

    mccDebugPrint("MCCGetConnectionStatus\n");
    if (!gMccInitialized) {
        gLastError = 1;
        return 0;
    }
    if (chID < 1 || chID >= 0x10) {
        gLastError = 0xE;
        return 0;
    }
    if (AsyncResourceIsBusy()) {
        gLastError = 0x15;
        return 0;
    }

    if (!connect) {
        mccDebugPrint("Error:Parameter error.\n");
        gLastError = 0xD;
    } else {
        if (MCCGetChannelInfo(chID, &info) != 0) {
            *connect = info.connect;
            gLastError = 0;
            return 1;
        }
    }

    return 0;
}

int MCCNotify(MCC_CHANNEL chID, u32 notify) {
    MCC_CONNECT connect;
#if !DEBUG
    int unused[3]; // fake but blah
#endif


    mccDebugPrint("MCCNotify\n");
    if (!gMccInitialized) {
        gLastError = 1;
    } else if(chID <= 0 || chID >= 0x10) {
        gLastError = 0xE;;
    } else if (LoadChannelInfo(gChannelInfo) == 0) {
        mccDebugPrint("Error:Could not update channelInfo.\n");
        gLastError = 0xB;
    } else if(MCCGetConnectionStatus(chID, &connect) == 0) {
        gLastError = 9;
    } else {
        if (connect != 3) {
            mccDebugPrint("Error:Channel is not opened.\n");
            gLastError = 0x12;
        } else {
            notify |= 0x10000000;
            return NotifyCompulsorily(chID, notify, 0xAU);
        }
    }

    return 0;
}

u32 MCCSetChannelEventMask(MCC_CHANNEL chID, u32 event) {
    u32 oldMask;
#if !DEBUG
    int unused[2]; // fake but blah
#endif

    oldMask = 0xFFFFFFFF;
    if (!gMccInitialized) {
        gLastError = 1;
    } else if (chID <= 0 || chID >= 0x10) {
        gLastError = 0xE;
    } else if (LoadChannelInfo(gChannelInfo) == 0) {
        mccDebugPrint("Error:Could not update channelInfo.\n");
        gLastError = 0xB;
    } else if (!IsChannelOpened(chID)) {
        mccDebugPrint("Error:This channel is closed.");
        gLastError = 0x12;
    } else {
        oldMask = gChannelInfo[chID].eventMask;
        gChannelInfo[chID].eventMask = (u16)event;
    }

    return oldMask;
}

int MCCOpen(MCC_CHANNEL chID, u8 blockSize, MCC_CBEvent callbackEvent) {
    u8 connectSide;
    u8 blockIndex;
#if !DEBUG
    int unused2[2];
#endif
    u8 freeBlocks;
#if !DEBUG
    int unused[6]; // fake but blah
#endif

    mccDebugPrint("MCCOpen\n");
    if (!gMccInitialized) {
        gLastError = 1;
        return 0;
    }
    if (chID < 1 || chID >= 0x10) {
        gLastError = 0xE;
        return 0;
    }
    if (AsyncResourceIsBusy()) {
        gLastError = 0x15;
        return 0;
    }
    if (blockSize == 0) {
        gLastError = 0xF;
        return 0;
    }

    if (chID <= 0 || chID >= 0x10) {
        mccDebugPrint("Error:Invalid channel.\n");
        gLastError = 0xE;
        goto exit;
    } else {
        connectSide = 2;
        if (LoadChannelInfo(gChannelInfo) == 0) {
            mccDebugPrint("Error:Could not update channelInfo.\n");
            gLastError = 0xB;
            goto exit;
        } else if (!gChannelInfo[chID].info.connect) {
            freeBlocks = SearchFreeBlocks(1, &blockIndex);
            if (blockSize > freeBlocks) {
                mccDebugPrint("Error:Not enough free blocks.\n");
                gLastError = 0xC;
                goto exit;
            } else {
                gChannelInfo[chID].info.firstBlock = blockIndex;
                gChannelInfo[chID].info.blockLength = blockSize;
                gChannelInfo[chID].info.connect = connectSide;
                gChannelInfo[chID].info.isLocked = FALSE;
                gChannelInfo[chID].eventMask = 0;
                gChannelInfo[chID].callbackEvent = callbackEvent;
                gChannelInfo[chID].isStreamDone = FALSE;

                if (FlushChannelInfo(gChannelInfo) == 0) {
                    mccDebugPrint("Error:Could not flush channelInfo.\n");
                    gLastError = 0xA;
                    goto exit;
                }
                gLastError = 0;
                return 1;
            }
        }
    }

    if (gChannelInfo[chID].info.connect & connectSide) {
        mccDebugPrint("Error:Already opened.\n");
        gLastError = 0x11;
        goto exit;
    } else if (blockSize != gChannelInfo[chID].info.blockLength) {
        mccDebugPrint("Error:Block size error.\n");
        gLastError = 0xD;
        goto exit;
    }

    gChannelInfo[chID].info.connect = 3;
    gChannelInfo[chID].callbackEvent = callbackEvent;

    if (FlushChannelInfo(gChannelInfo) == 0) {
        mccDebugPrint("Error:Could not flush channelInfo.\n");
        gLastError = 0xA;
        goto exit;
    }

    if (~(gChannelInfo[chID].eventMask) & 1) {
        NotifyCompulsorily(chID, 1, 10);
        if (gChannelInfo[chID].callbackEvent) {
            gChannelInfo[chID].callbackEvent(chID, 1, 0);
        }
    }

    gLastError = 0;
    return 1;
exit:;
    return 0;
}

int MCCClose(MCC_CHANNEL chID) {
    u8 connectSide;
#if !DEBUG
    int unused[4]; // fake but blah
#endif

    connectSide = 2;
    mccDebugPrint("MCCClose\n");
    if (!gMccInitialized) {
        gLastError = 1;
        return 0;
    }
    if (chID < 1 || chID >= 0x10) {
        gLastError = 0xE;
        return 0;
    }
    if (AsyncResourceIsBusy()) {
        gLastError = 0x15;
        return 0;
    }
    if (LoadChannelInfo(gChannelInfo) == 0) {
        mccDebugPrint("Error:Could not update channelInfo.\n");
        gLastError = 0xB;
        goto exit;
    }
    if (!IsChannelOpened(chID)) {
        mccDebugPrint("Error:This channel is closed.");
        gLastError = 0x12;
        goto exit;
    }

    gChannelInfo[chID].info.connect &= ~connectSide;
    if (gChannelInfo[chID].info.connect == 0) {
        ClearChannelInfo(chID);
    }
    if (FlushChannelInfo(gChannelInfo) == 0) {
        mccDebugPrint("Error:Could not flush channelInfo.\n");
        gLastError = 0xA;
        goto exit;
    }

    if (gChannelInfo[chID].info.connect != 0) {
        if (~(gChannelInfo[chID].eventMask) & 2) {
            NotifyCompulsorily(chID, 2, 10);
            if (gChannelInfo[chID].callbackEvent) {
                gChannelInfo[chID].callbackEvent(chID, 2, 0);
            }
        }
    }

    gLastError = 0;
    return 1;
exit:;
    return 0;
}

int MCCLock(MCC_CHANNEL chID) {
#if !DEBUG
    int unused[7]; // fake but blah
#endif

    mccDebugPrint("MCCLock\n");

    if (!gMccInitialized) {
        gLastError = 1;
        return 0;
    }
    if (chID < 1 || chID >= 0x10) {
        gLastError = 0xE;
        return 0;
    }
    if (AsyncResourceIsBusy()) {
        gLastError = 0x15;
        return 0;
    }
    if (LoadChannelInfo(gChannelInfo) == 0) {
        mccDebugPrint("Error:Could not update channelInfo.\n");
        gLastError = 0xB;
        goto exit;
    }
    if (!IsChannelOpened(chID)) {
        mccDebugPrint("Error:This channel is closed.");
        gLastError = 0x12;
        goto exit;
    }
    if (gChannelInfo[chID].info.isLocked == TRUE) {
        mccDebugPrint("Error:This channel is already locked.");
        gLastError = 0x13;
        goto exit;
    }

    gChannelInfo[chID].info.isLocked = TRUE;

    if (FlushChannelInfo(gChannelInfo) == 0) {
        mccDebugPrint("Error:Could not flush channelInfo.\n");
        gLastError = 0xA;
        goto exit;
    }

    if (~(gChannelInfo[chID].eventMask) & 4) {
        NotifyChannelEvent(chID, 4);
    }
    gLastError = 0;
    return 1;

exit:;
    return 0;
}

int MCCUnlock(MCC_CHANNEL chID) {
#if !DEBUG
    int unused[7]; // fake but blah
#endif

    mccDebugPrint("MCCUnlock\n");

    if (!gMccInitialized) {
        gLastError = 1;
        return 0;
    }
    if (chID < 1 || chID >= 0x10) {
        gLastError = 0xE;
        return 0;
    }
    if (AsyncResourceIsBusy()) {
        gLastError = 0x15;
        return 0;
    }
    if (LoadChannelInfo(gChannelInfo) == 0) {
        mccDebugPrint("Error:Could not update channelInfo.\n");
        gLastError = 0xB;
        goto exit;
    }
    if (!IsChannelOpened(chID)) {
        mccDebugPrint("Error:This channel is closed.");
        gLastError = 0x12;
        goto exit;
    }
    if (gChannelInfo[chID].info.isLocked == FALSE) {
        mccDebugPrint("Error:This channel is already unlocked.");
        gLastError = 0x14;
        goto exit;
    }

    gChannelInfo[chID].info.isLocked = FALSE;

    if (FlushChannelInfo(gChannelInfo) == 0) {
        mccDebugPrint("Error:Could not flush channelInfo.\n");
        gLastError = 0xA;
        goto exit;
    }

    if (~(gChannelInfo[chID].eventMask) & 8) {
        NotifyChannelEvent(chID, 8);
    }
    gLastError = 0;
    return 1;

exit:
    return 0;
}

int MCCRead(MCC_CHANNEL chID, u32 offset, void* data, s32 size, MCC_SYNC_STATE async) {
#if !DEBUG
    int unused[11]; // fake but blah
#endif

    mccDebugPrint("MCCRead\n");
    if (!gMccInitialized) {
        gLastError = 1;
        return 0;
    }
    if (chID < 1 || chID >= 0x10) {
        gLastError = 0xE;
        return 0;
    }
    if (!(async == 1 || async == 0)) {
        gLastError = 0xD;
        return 0;
    }
    if ((offset & 3) || ((u32)data & 0x1F) || (size % 32) != 0) {
        gLastError = 0xD;
        return 0;
    }
    if (AsyncResourceIsBusy()) {
        gLastError = 0x15;
        return 0;
    }
    if (LoadChannelInfo(gChannelInfo) == 0) {
        mccDebugPrint("Error:Could not update channelInfo.\n");
        gLastError = 0xB;
        goto exit;
    }
    if (!IsChannelOpened(chID)) {
        mccDebugPrint("Error:This channel is closed.");
        gLastError = 0x12;
        goto exit;
    }
    if (offset > gChannelInfo[chID].info.blockLength << 0xD) {
        mccDebugPrint("Error:Invarid offset");
        gLastError = 0x10;
        goto exit;
    }
    if ((offset + size) > gChannelInfo[chID].info.blockLength << 0xD) {
        mccDebugPrint("Error:Invarid data size.");
        gLastError = 0xF;
        goto exit;
    }

    if (async == 1) {
        if (MCCCheckAsyncDone() == 0) {
            mccDebugPrint("Error:Channel busy.");
            gLastError = 0x15;
            goto exit;
        }
        AsyncResourceStateBusy(chID, 0U);
        if (HIOReadAsync(offset + (gChannelInfo[chID].info.firstBlock << 0xD), data, size, MCCRxCallback) == 0) {
            mccDebugPrint("Error:Read data error.");
            gLastError = 7;
            goto exit;
        }
        DCInvalidateRange(data, size);
        gLastError = 0;
        return 1;
    }

    if (HIORead(offset + (gChannelInfo[chID].info.firstBlock << 0xD), data, size) == 0) {
        mccDebugPrint("Error:Read data error.");
        gLastError = 7;
        goto exit;
    }

    DCInvalidateRange(data, size);

    if (~(gChannelInfo[chID].eventMask) & 0x10) {
        NotifyChannelEvent(chID, 0x10);
    }

    if (~(gChannelInfo[chID].eventMask) & 0x40 && gChannelInfo[chID].callbackEvent) {
        gChannelInfo[chID].callbackEvent(chID, 0x40, 0);
    }
    gLastError = 0;
    return 1;

exit:;
    return 0;
}

int MCCWrite(MCC_CHANNEL chID, u32 offset, void* data, s32 size, MCC_SYNC_STATE async) {
#if !DEBUG
    int unused[11]; // fake but blah
#endif
    mccDebugPrint("MCCWrite\n");

    if (!gMccInitialized) {
        gLastError = 1;
        return 0;
    }
    if (chID < 1 || chID >= 0x10) {
        gLastError = 0xE;
        return 0;
    }
    if (!(async == 1 || async == 0)) {
        gLastError = 0xD;
        return 0;
    }
    if ((offset & 3) || ((u32)data & 0x1F) || (size % 32) != 0) {
        gLastError = 0xD;
        return 0;
    }
    if (AsyncResourceIsBusy()) {
        gLastError = 0x15;
        return 0;
    }
    if (LoadChannelInfo(gChannelInfo) == 0) {
        mccDebugPrint("Error:Could not update channelInfo.\n");
        gLastError = 0xB;
        goto exit;
    }
    if (!IsChannelOpened(chID)) {
        mccDebugPrint("Error:This channel is closed.");
        gLastError = 0x12;
        goto exit;
    }
    if (gChannelInfo[chID].info.isLocked == 1) {
        mccDebugPrint("Error:This channel was locked.");
        gLastError = 0x13;
        goto exit;
    }
    if (offset > (gChannelInfo[chID].info.blockLength << 0xD)) {
        mccDebugPrint("Error:Invarid offset");
        gLastError = 0x10;
        goto exit;
    }
    if (offset + size > (gChannelInfo[chID].info.blockLength << 0xD)) {
        mccDebugPrint("Error:Invarid data size.");
        gLastError = 0xF;
        goto exit;
    }

    if (async == 1) {
        if (MCCCheckAsyncDone() == 0) {
            mccDebugPrint("Error:Channel busy.");
            gLastError = 0x15;
            goto exit;
        }
        AsyncResourceStateBusy(chID, 0x100);
        DCFlushRange(data, size);
        if (HIOWriteAsync(offset + (gChannelInfo[chID].info.firstBlock << 0xD), data, size, MCCTxCallback) == 0) {
            mccDebugPrint("Error:Write data error.");
            gLastError = 8;
            goto exit;
        }
        gLastError = 0;
        return 1;
    }

    DCFlushRange(data, size);

    if (HIOWrite(offset + (gChannelInfo[chID].info.firstBlock << 0xD), data, size) == 0) {
        mccDebugPrint("Error:Write data error.");
        gLastError = 8;
        goto exit;
    }

    if (~(gChannelInfo[chID].eventMask) & 0x20) {
        NotifyChannelEvent(chID, 0x20);
    }

    if (~(gChannelInfo[chID].eventMask) & 0x80 && gChannelInfo[chID].callbackEvent) {
        gChannelInfo[chID].callbackEvent(chID, 0x80, 0);
    }
    gLastError = 0;
    return 1;

exit:
    return 0;
}

int MCCCheckAsyncDone() {
    u16 stat;
    u16 mode;
    u8 chID;
#if !DEBUG
    int unused[5]; // fake but blah
#endif

    if (!gMccInitialized) {
        gLastError = 1;
        return 0;
    }

    stat = AsyncResourceGetStat();

    if (stat == 0x1000) {
        return 0;
    } else if (stat && stat == 0x2000) {
        mode = AsyncResourceGetMode();
        chID = AsyncResourceGetChannel();
        AsyncResourceClearState();
        if (mode == 0) {
            if (~(gChannelInfo[chID].eventMask) & 0x10) {
                NotifyChannelEvent(chID, 0x10);
            }
            if (~(gChannelInfo[chID].eventMask) & 0x40 && gChannelInfo[chID].callbackEvent) {
                gChannelInfo[chID].callbackEvent(chID, 0x40, 0);
            }
        } else {
            if (~(gChannelInfo[chID].eventMask) & 0x20) {
                NotifyChannelEvent(chID, 0x20);
            }
            if (~(gChannelInfo[chID].eventMask) & 0x80 && gChannelInfo[chID].callbackEvent) {
                gChannelInfo[chID].callbackEvent(chID, 0x80, 0);
            }
        }
    }

    return 1;
}
