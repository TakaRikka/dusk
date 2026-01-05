#include <revolution/os.h>
#include <revolution/wpad.h>
#include <revolution/wpad/wud.h>

#include <cstring>

#include "__wpad.h"

wpad_cb_st* __rvl_p_wpadcb[WPAD_MAX_CONTROLLERS];

static u8 _wpadSpeakerVol;

static s8 __wpadGetQueueSize(struct WPADCmdQueue* cmdQueue);
static u16 __wpadGetBTEBufferStatus(s32 chan);
static u16 __wpadGetBTMBufferStatus(s32 chan);
static BOOL __wpadPushCommand(struct WPADCmdQueue* cmdQueue, struct WPADCmd cmdBlk);

void dummyString(void* p_buf) {
    ASSERTLINE(0, p_buf != NULL);
}

static u32 __wpadFmt2Size(u32 fmt) {
    u32 fmtSize;

    switch (fmt) {
    case WPAD_FMT_FS_BTN:
    case WPAD_FMT_FS_BTN_ACC:
    case WPAD_FMT_FS_BTN_ACC_DPD:
        fmtSize = sizeof(WPADFSStatus);
        break;
    case WPAD_FMT_CLASSIC_BTN:
    case WPAD_FMT_CLASSIC_BTN_ACC:
    case WPAD_FMT_CLASSIC_BTN_ACC_DPD:
    case WPAD_FMT_GUITAR:
    case WPAD_FMT_DRUM:
    case WPAD_FMT_TAIKO:
    case WPAD_FMT_TURNTABLE:
        fmtSize = sizeof(WPADCLStatus);
        break;
    case WPAD_FMT_BULK:
        fmtSize = sizeof(WPADBKStatus);
        break;
    case WPAD_FMT_TRAIN:
        fmtSize = sizeof(WPADTRStatus);
        break;
    case 20:
        fmtSize = 0x32;
        break;
    case WPAD_FMT_BALANCE_CHECKER:
        fmtSize = sizeof(WPADBLStatus);
        break;
    case WPAD_FMT_VSM:
        fmtSize = sizeof(WPADVSStatus);
        break;
    case WPAD_FMT_BTN_ACC_DPD_EXTENDED:
        fmtSize = sizeof(WPADStatusEx);
        break;
    case WPAD_FMT_MOTION_PLUS:
        fmtSize = sizeof(WPADMPStatus);
        break;
    default:
        fmtSize = sizeof(WPADStatus);
        break;
    }

    return fmtSize;
}

u8 WPADGetRadioSensitivity(s32 chan) {
    wpad_cb_st* p_wpd = __rvl_p_wpadcb[chan];
    BOOL intrStatus = OSDisableInterrupts();
    u8 radioSensitivity = p_wpd->radioSensitivity;

    OSRestoreInterrupts(intrStatus);
    return radioSensitivity;
}

void WPADRead(s32 chan, WPADStatus* status) {
    wpad_cb_st* p_wpd = __rvl_p_wpadcb[chan];
    BOOL intrStatus;
    u8 rxBufIndex;
    WPADStatus* rxStatus;
    u32 fmtSize;

    ASSERTLINE(4013, (0 <= chan) && (chan < WPAD_MAX_CONTROLLERS));
    ASSERTLINE(4014, status != NULL);

    intrStatus = OSDisableInterrupts();

    rxBufIndex = p_wpd->rxBufIndex != 0 ? 0 : 1;
    rxStatus = (WPADStatus*)p_wpd->rxBufs[rxBufIndex];
    fmtSize = __wpadFmt2Size(p_wpd->dataFormat);

    if (rxStatus->err != WPAD_ESUCCESS)
        fmtSize = sizeof(WPADStatus);

    memcpy(status, rxStatus, fmtSize);

    OSRestoreInterrupts(intrStatus);
}

BOOL WPADIsSpeakerEnabled(s32 chan) {
    BOOL enabled;
    wpad_cb_st* p_wpd = __rvl_p_wpadcb[chan];
    BOOL intrStatus = OSDisableInterrupts();

    enabled = p_wpd->info.speaker;

    OSRestoreInterrupts(intrStatus);
    return enabled;
}

s32 WPADControlSpeaker(s32 chan, u32 command, WPADCallback cb) {
    u8 data[7] = {0x00, 0x00, 0xd0, 0x07, 0x40, 0x0c, 0x0e};
    wpad_cb_st* p_wpd = __rvl_p_wpadcb[chan];
    BOOL intrStatus = OSDisableInterrupts();
    BOOL speakerEnabled = p_wpd->info.speaker;
    BOOL handshakeFinished;
    s32 status = p_wpd->status;

    handshakeFinished = p_wpd->handshakeFinished;

    OSRestoreInterrupts(intrStatus);

    if (status == WPAD_ENODEV)
        goto end;

    if (!handshakeFinished) {
        status = WPAD_EBUSY;
        goto end;
    }

    if (command == WPAD_SPEAKER_DISABLE) {
        if (!speakerEnabled) {
            status = WPAD_ESUCCESS;
            goto end;
        }

        intrStatus = OSDisableInterrupts();

        if (WPADiIsAvailableCmdQueue(&p_wpd->stdCmdQueue, 5)) {
            WPADiSendMuteSpeaker(&p_wpd->stdCmdQueue, TRUE, NULL);
            WPADiSendWriteDataCmd(&p_wpd->stdCmdQueue, 0x01, WM_REG_SPEAKER_01, NULL);
            WPADiSendWriteDataCmd(&p_wpd->stdCmdQueue, 0x00, WM_REG_SPEAKER_09, NULL);
            WPADiSendEnableSpeaker(&p_wpd->stdCmdQueue, FALSE, NULL);
            WPADiSendGetContStat(&p_wpd->stdCmdQueue, NULL, cb);

            OSRestoreInterrupts(intrStatus);

            return WPAD_ESUCCESS;
        }

        status = WPAD_EBUSY;
        OSRestoreInterrupts(intrStatus);
    } else {
        switch (command) {
        case WPAD_SPEAKER_ENABLE:
        case WPAD_SPEAKER_CMD_05:
            intrStatus = OSDisableInterrupts();

            if (WPADiIsAvailableCmdQueue(&p_wpd->stdCmdQueue, 7)) {
                WPADiSendEnableSpeaker(&p_wpd->stdCmdQueue, TRUE, NULL);
                WPADiSendMuteSpeaker(&p_wpd->stdCmdQueue, TRUE, NULL);
                WPADiSendWriteDataCmd(&p_wpd->stdCmdQueue, 0x01, WM_REG_SPEAKER_09, NULL);

                // sends 0x80 instead of 0x08?
                WPADiSendWriteDataCmd(&p_wpd->stdCmdQueue, 0x80, WM_REG_SPEAKER_01, NULL);

                data[4] = _wpadSpeakerVol;
                WPADiSendWriteData(&p_wpd->stdCmdQueue, &data, sizeof data, WM_REG_SPEAKER_01,
                                   NULL);
                WPADiSendMuteSpeaker(&p_wpd->stdCmdQueue, FALSE, NULL);
                WPADiSendGetContStat(&p_wpd->stdCmdQueue, NULL, cb);

                OSRestoreInterrupts(intrStatus);

                return WPAD_ESUCCESS;
            }

            status = WPAD_EBUSY;

            OSRestoreInterrupts(intrStatus);
            break;
        case WPAD_SPEAKER_MUTE:
            if (!WPADiSendMuteSpeaker(&p_wpd->stdCmdQueue, TRUE, cb)) {
                status = WPAD_EBUSY;
                goto end;
            }

            return WPAD_ESUCCESS;
        case WPAD_SPEAKER_UNMUTE:
            if (!WPADiSendMuteSpeaker(&p_wpd->stdCmdQueue, FALSE, cb)) {
                status = WPAD_EBUSY;
                goto end;
            }

            return WPAD_ESUCCESS;
        case WPAD_SPEAKER_PLAY:
            if (!WPADiSendWriteDataCmd(&p_wpd->stdCmdQueue, 0x01, WM_REG_SPEAKER_08, cb)) {
                status = WPAD_EBUSY;
                goto end;
            }

            return WPAD_ESUCCESS;
        }
    }

end:
    if (cb)
        (*cb)(chan, status);

    return status;
}

BOOL __wpadIsBusyStream(s32 chan) {
    BOOL intrStatus;
    wpad_cb_st* p_wpd = __rvl_p_wpadcb[chan];
    u8 radioQuality;
    u32 devType;
    u8 bufferStatus;
    u16 bteBufferStatus;
    u16 btmBufferStatus;
    u8 audioFrames;
    s8 queueSize;
    u8 linkNumber;

    intrStatus = OSDisableInterrupts();

    radioQuality = p_wpd->radioQuality;
    devType = p_wpd->devType;
    bufferStatus = WUDGetBufferStatus();

    queueSize = __wpadGetQueueSize(&p_wpd->stdCmdQueue);

    bteBufferStatus = __wpadGetBTEBufferStatus(chan);
    btmBufferStatus = __wpadGetBTMBufferStatus(chan);
    audioFrames = p_wpd->audioFrames;

    linkNumber = _WUDGetLinkNumber();

    OSRestoreInterrupts(intrStatus);

    if (radioQuality != WPAD_RADIO_QUALITY_GOOD || btmBufferStatus > 3 || bufferStatus == 10 ||
        bufferStatus >= linkNumber * 2 + 2 || devType == WPAD_DEV_INITIALIZING || queueSize >= 21 ||
        audioFrames >= 1)
    {
        return TRUE;
    } else {
        return FALSE;
    }
}

s32 WPADSendStreamData(s32 chan, void* p_buf, u16 len) {
    wpad_cb_st* p_wpd = __rvl_p_wpadcb[chan];
    BOOL intrStatus;
    BOOL handshakeFinished;
    s32 status;

    ASSERTLINE(4528, p_buf != NULL);
    ASSERTLINE(4529, len >= 0 && len <= 20);

    intrStatus = OSDisableInterrupts();

    status = p_wpd->status;
    handshakeFinished = p_wpd->handshakeFinished;

    OSRestoreInterrupts(intrStatus);

    if (status == WPAD_ENODEV)
        return WPAD_ENODEV;

    if (!handshakeFinished)
        return WPAD_EBUSY;

    if (__wpadIsBusyStream(chan))
        return WPAD_EBUSY;

    if (!WPADiSendStreamData(&p_wpd->stdCmdQueue, p_buf, len))
        return WPAD_EBUSY;

    intrStatus = OSDisableInterrupts();

    ++p_wpd->audioFrames;

    OSRestoreInterrupts(intrStatus);

    return WPAD_ESUCCESS;
}

BOOL WPADiSendEnableSpeaker(struct WPADCmdQueue* cmdQueue, BOOL enabled, WPADCallback cb) {
    BOOL success;
    struct WPADCmd cmdBlk;
    cmdBlk.reportID = RPTID_ENABLE_SPEAKER;
    cmdBlk.dataLength = RPT14_SIZE;
    cmdBlk.dataBuf[RPT14_SPEAKER_ENABLE] = enabled ? 4 : 0;
    cmdBlk.cmdCB = cb;

    success = __wpadPushCommand(cmdQueue, cmdBlk);
    return success;
}

BOOL WPADiSendGetContStat(struct WPADCmdQueue* cmdQueue, WPADInfo* infoOut, WPADCallback cb) {
    BOOL success;
    struct WPADCmd cmdBlk;
    cmdBlk.reportID = RPTID_REQUEST_STATUS;
    cmdBlk.dataLength = RPT15_SIZE;
    cmdBlk.dataBuf[0] = 0;
    cmdBlk.cmdCB = cb;
    cmdBlk.statusReportOut = infoOut;

    success = __wpadPushCommand(cmdQueue, cmdBlk);
    return success;
}

BOOL WPADiSendWriteDataCmd(struct WPADCmdQueue* cmdQueue, u8 cmd, u32 address, WPADCallback cb) {
    return WPADiSendWriteData(cmdQueue, &cmd, sizeof(cmd), address, cb);
}

BOOL WPADiSendWriteData(struct WPADCmdQueue* cmdQueue, void const* p_buf, u16 len, u32 address,
                        WPADCallback cb) {
    BOOL success;
    u8 packedLen = len & 0x1f;
    struct WPADCmd cmdBlk;

    ASSERTLINE(5830, len > 0 && len <= 16);
    ASSERTLINE(5831, p_buf != NULL);

    cmdBlk.reportID = RPTID_WRITE_DATA;
    cmdBlk.dataLength = RPT16_SIZE;
    cmdBlk.cmdCB = cb;
    memcpy(&cmdBlk.dataBuf[RPT16_DATA_DST_ADDRESS], &address, sizeof address);
    memcpy(&cmdBlk.dataBuf[RPT16_DATA_LENGTH], &packedLen, sizeof packedLen);
    memcpy(&cmdBlk.dataBuf[RPT16_DATA], p_buf, len);

    success = __wpadPushCommand(cmdQueue, cmdBlk);
    return success;
}

BOOL WPADiSendStreamData(struct WPADCmdQueue* cmdQueue, void const* p_buf, u16 len) {
    BOOL success;
    u8 packedLen = len << 3;
    struct WPADCmd cmdBlk;

    ASSERTLINE(5873, len > 0 && len <= 20);

    cmdBlk.reportID = RPTID_SEND_SPEAKER_DATA;
    cmdBlk.dataLength = sizeof cmdBlk.dataBuf;
    cmdBlk.dataBuf[RPT18_DATA_LENGTH] = packedLen;
    cmdBlk.cmdCB = NULL;
    memcpy(&cmdBlk.dataBuf[RPT18_DATA], p_buf, len);

    success = __wpadPushCommand(cmdQueue, cmdBlk);
    return success;
}

BOOL WPADiSendMuteSpeaker(struct WPADCmdQueue* cmdQueue, BOOL muted, WPADCallback cb) {
    BOOL success;
    struct WPADCmd cmdBlk;

    cmdBlk.reportID = RPTID_MUTE_SPEAKER;
    cmdBlk.dataLength = RPT19_SIZE;
    cmdBlk.dataBuf[RPT19_SPEAKER_MUTE] = muted ? 4 : 0;
    cmdBlk.cmdCB = cb;

    success = __wpadPushCommand(cmdQueue, cmdBlk);
    return success;
}

BOOL WPADiIsAvailableCmdQueue(struct WPADCmdQueue* cmdQueue, s8 num) {
    s8 queueSize = __wpadGetQueueSize(cmdQueue);

    if ((u32)(queueSize + num) <= cmdQueue->length - 1)
        return TRUE;
    else
        return FALSE;
}

static s8 __wpadGetQueueSize(struct WPADCmdQueue* cmdQueue) {
    BOOL intrStatus = OSDisableInterrupts();

    s8 queueRemaining = cmdQueue->indexIn - cmdQueue->indexOut;

    if (queueRemaining < 0)
        queueRemaining += cmdQueue->length;

    OSRestoreInterrupts(intrStatus);
    return queueRemaining;
}

static BOOL __wpadPushCommand(struct WPADCmdQueue* cmdQueue, struct WPADCmd cmdBlk) {
    BOOL intrStatus = OSDisableInterrupts();

    if (cmdQueue->length - 1 == (u32)__wpadGetQueueSize(cmdQueue)) {
        OSRestoreInterrupts(intrStatus);

        return FALSE;
    }

    memset(&cmdQueue->queue[cmdQueue->indexIn], 0, sizeof cmdQueue->queue[cmdQueue->indexIn]);
    memcpy(&cmdQueue->queue[cmdQueue->indexIn], &cmdBlk, sizeof cmdQueue->queue[cmdQueue->indexIn]);

    cmdQueue->indexIn = (u32)cmdQueue->indexIn == ((cmdQueue->length) - 1) ? 0 : cmdQueue->indexIn + 1;

    OSRestoreInterrupts(intrStatus);
    return TRUE;
}

static u16 __wpadGetBTEBufferStatus(s32 chan) {
    wpad_cb_st* p_wpd = __rvl_p_wpadcb[chan];
    BOOL intrStatus = OSDisableInterrupts();
    s32 status = p_wpd->status;
    s8 dev_handle = p_wpd->devHandle;

    OSRestoreInterrupts(intrStatus);

    if (status == WPAD_ENODEV)
        return 0;

    return _WUDGetQueuedSize(dev_handle);
}

static u16 __wpadGetBTMBufferStatus(s32 chan) {
    wpad_cb_st* p_wpd = __rvl_p_wpadcb[chan];
    BOOL intrStatus = OSDisableInterrupts();
    s32 status = p_wpd->status;
    s8 dev_handle = p_wpd->devHandle;

    OSRestoreInterrupts(intrStatus);

    if (status == WPAD_ENODEV)
        return 0;

    return _WUDGetNotAckedSize(dev_handle);
}
