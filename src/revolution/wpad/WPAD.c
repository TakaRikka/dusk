#include <revolution/wpad.h>
#include <revolution/wpad/__wpad.h>

#include <revolution/wud.h>
#include <revolution/dvd.h>
#include <revolution/sc.h>
#include <revolution/vi.h>
#include <revolution/wpad/bte.h>
#include <revolution/wud/__wud.h>
#include <cstdio>
#include <cstring>

extern volatile BOOL __OSIsReturnToIdle;

//TODO: this apparently should be aligned to 32 bytes, but
//      adding ATTRIBUTE_ALIGN breaks codegen in WPADInit
WPADControlBlock _wpd[WPAD_MAX_CONTROLLERS];
#if SDK_AUG2010
WPADControlBlock* __rvl_p_wpadcb[WPAD_MAX_CONTROLLERS];
#else
WPADControlBlock* _wpdcb[WPAD_MAX_CONTROLLERS];
#endif

u8 _sleepTime;
u8 _dpdSensitivity;
u8 _sensorBarPos;
BOOL _rumble;
u8 _speakerVolume;
u8 _scFlush;
u8 _gametype;
u16 _gameTitle[17];
const char* _gamecode;

s8 _dev_handle_index[16];
u8 _chan_active_state[WPAD_MAX_CONTROLLERS];
OSAlarm _managerAlarm;

DVDDiskID _diskId;

static u8 _scSetting;
static vu8 _shutdown;
static s8 _afhChannel;
static u8 _regShutdown = 0;

static u16 _senseCnt = 0;
static u8 _checkCnt = 0;
static u8 _extCnt = 0;
static u16 _afhCnt = 0;

static u8 _rumbleCnt[WPAD_MAX_CONTROLLERS] = {0, 0, 0, 0};

static s8 _infRes[4];

static BOOL _initialized;
static BOOL _startup = FALSE;
static int _recFlag = -1;
static int _recCnt = 0;

static const char* __WPADVersion = "<< RVL_SDK - WPAD \trelease build: Oct  3 2006 03:58:38 (0x4200_60422) >>";

#define WPAD_DEFAULT_ACC_DIFF_COUNT_THRESHOLD (u16)(6)
#define WPAD_DEFAULT_ACC_HYST_COUNT_THRESHOLD (u16)(30)

#define WPAD_DEFAULT_DPD_DIFF_COUNT_THRESHOLD (u16)(4)
#define WPAD_DEFAULT_DPD_HYST_COUNT_THRESHOLD (u16)(30)

static u16 __WPAD_acc_diff_count_threshold = WPAD_DEFAULT_ACC_DIFF_COUNT_THRESHOLD;
static u16 __WPAD_dpd_diff_count_threshold = WPAD_DEFAULT_DPD_DIFF_COUNT_THRESHOLD;

static u16 __WPAD_acc_hyst_count_threshold = WPAD_DEFAULT_ACC_HYST_COUNT_THRESHOLD;
static u16 __WPAD_dpd_hyst_count_threshold = WPAD_DEFAULT_DPD_HYST_COUNT_THRESHOLD;

static OSShutdownFunctionInfo ShutdownFunctionInfo = {OnShutdown, 127};

extern void DEBUGPrint(const char*, ...);

static void __ClearControlBlock(s32 chan);

BOOL OnShutdown(BOOL final, u32 event) {
    s32 i;
    BOOL ret;
    s32 status;

    ret = FALSE;
    status = WUDGetStatus();

    if (!final) {
        if (status == 2 || status == 3) {
            switch (event) {
            case 0:
            case 2:
            case 3:
                if (!_shutdown) {
                    _shutdown = TRUE;
                    WUDSetVisibility(0, 0);
                    for (i = 10; i < 14; i++) {
                        memset(&_scArray.regist[i], 0, sizeof(_scArray.regist[i]));
                    }
                    OSCancelAlarm(&_managerAlarm);
                    WUDSetHidRecvCallback(NULL);
                    WUDShutdown();
                }
                ret = FALSE;
                break;
            case 1:
            case 4:
            case 5:
            case 6:
                if (!_shutdown) {
                    _shutdown = TRUE;
                    BTA_DmSendHciReset();
                    OSCancelAlarm(&_managerAlarm);
                    WUDSetHidRecvCallback(NULL);
                    WUDShutdown();
                }
                ret = FALSE;
                break;
            }
        } else if (status == 4 || status == 1) {
            ret = FALSE;
        } else if (status == 0) {
            ret = TRUE;
        }
    } else {
        ret = TRUE;
    }

    return ret;
}

s32 WPADiGetStatus(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    s32 status;

    enable = OSDisableInterrupts();
    status = p_wpd->status;
    OSRestoreInterrupts(enable);

    return status;
}

BOOL __SetSensorBarPower(BOOL flag) {
    u32 reg;
    u32 regBak;
    BOOL enable;
    BOOL result;

    enable = OSDisableInterrupts();

    regBak = ACRReadReg(0xC0);

    if (flag) {
        reg = regBak | 0x100;
    } else {
        reg = regBak & ~0x100;
    }

    ACRWriteReg(0xC0, reg);
    result = (regBak & 0x100) ? TRUE : FALSE;
    OSRestoreInterrupts(enable);
    return result;
}

static u8 __GetDpdSensitivity() {
    u8 level;

    level = (u8)SCGetBtDpdSensibility();
    if (level < 1) {
        level = 1;
    }
    if (level > 5) {
        level = 5;
    }

    return level;
}

static u8 __GetSensorBarPosition() {
    u8 pos;

    if (1 == SCGetWpadSensorBarPosition()) {
        pos = WPAD_SENSOR_BAR_POS_TOP;
    } else {
        pos = WPAD_SENSOR_BAR_POS_BOTTOM;
    }
    return pos;
}

static BOOL __GetMotorMode() {
    BOOL rumble;

    if (1 == SCGetWpadMotorMode()) {
        rumble = TRUE;
    } else {
        rumble = FALSE;
    }
    return rumble;
}

static u8 __ClampSpeakerVolume(u8 vol) {
    u8 v = vol;

    if (vol < 0) {
        v = 0;
    }
    if (vol > 127) {
        v = 127;
    }

    return v;
}

static u8 __GetSpeakerVolume() {
    u8 vol;
    vol = SCGetWpadSpeakerVolume();
    vol = __ClampSpeakerVolume(vol);
    return vol;
}

void WPADiManageHandler(OSAlarm*, OSContext*) {
    s32 chan;
    s32 status;
    BOOL sendCmd;

    status = WPADGetStatus();

    if (status != WPAD_STATE_SETUP) {
        return;
    }

    for (chan = 0; chan < WPAD_MAX_CONTROLLERS; chan++) {
        sendCmd = FALSE;
        if (_extCnt == 5) {
            sendCmd |= WPADiProcessExtCommand(chan);
        }
        sendCmd |= WPADiProcessCommand(chan);

        WPADiRumbleMotor(chan, sendCmd);

        if (_checkCnt == 5) {
            WPADiCheckContInputs(chan);
        }

        if (_senseCnt == 10) {
            WPADiRadioSensitivity(chan);
        }
    }

    if (_afhCnt == 60000) {
        WPADiAfh();
    }

    _senseCnt = (u16)((_senseCnt == 10) ? 0 : _senseCnt + 1);
    _checkCnt = (u8)((_checkCnt == 5) ? 0 : _checkCnt + 1);
    _extCnt = (u16)((_extCnt == 5) ? 0 : _extCnt + 1);
    _afhCnt = (u16)((_afhCnt == 60000) ? 0 : _afhCnt + 1);

    WPADiContMapTableUpdate();
    WPADiGetScSettings();
    BTA_HhGetAclQueueInfo();
}

u8 __WPADiManageHandlerStack[4096] ATTRIBUTE_ALIGN(32);

void WPADiManageHandler0(OSAlarm* alarm, OSContext* context) {
    OSSwitchFiberEx((u32)alarm, (u32)context, 0, 0, (u32)WPADiManageHandler, (u32)(__WPADiManageHandlerStack + sizeof(__WPADiManageHandlerStack)));
}

inline void WPADiInitSub() {
    s32 i;

    __SetSensorBarPower(TRUE);

    for (i = 0; i < 16; i++) {
        _dev_handle_index[i] = -1;
    }

    //TODO: this gets placed in .data incorrectly with the inline keyword,
    //      might be fixed when we figure out -ipa program
    DEBUGPrint("WPADInit()\n");

    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        _wpdcb[i] = &_wpd[i];
        _chan_active_state[i] = 0;
        _wpdcb[i]->connectCallback = NULL;

        __ClearControlBlock(i);
        OSInitThreadQueue(&_wpd[i].threadQueue);
    }

    _sleepTime = 5;
    _gamecode = OSGetAppGamename();
    _gametype = OSGetAppType();
    _dpdSensitivity = __GetDpdSensitivity();
    _sensorBarPos = __GetSensorBarPosition();
    _rumble = __GetMotorMode();
    _speakerVolume = __GetSpeakerVolume();

    WUDSetHidConnCallback(WPADiConnCallback);
    WUDSetHidRecvCallback(WPADiRecvCallback);

    _senseCnt = 0;
    _checkCnt = 0;
    _extCnt = 0;
    _afhCnt = 0;
    _rumbleCnt[0] = _rumbleCnt[1] = _rumbleCnt[2] = _rumbleCnt[3] = 0;

    OSCreateAlarm(&_managerAlarm);
    OSSetPeriodicAlarm(&_managerAlarm, __OSGetSystemTime(), OSMillisecondsToTicks(1),
                       WPADiManageHandler0);

    _shutdown = 0;
    _scFlush = 0;
    _scSetting = 1;
    _afhChannel = -1;

    OSRegisterVersion(__WPADVersion);
}

void WPADInit(void) {
    BOOL result;

    if (_regShutdown == 0) {
        OSRegisterShutdownFunction(&ShutdownFunctionInfo);
        _regShutdown = 1;
    }

    result = WUDInit();

    if (result) {
        WPADiInitSub();
    }
}

void WPADiRadioSensitivity(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    u32 packet = 2;
    u16 rs;

    rs = (u16)(p_wpd->radioSensitivity * (10 - 1));
    rs += (u16)((p_wpd->packetCnt * 100) / packet);
    rs /= 10;
    rs = (u16)((rs > 100) ? 100 : rs);

    p_wpd->radioSensitivity = (u8)rs;
    p_wpd->packetCnt = 0;

    if (p_wpd->radioQuality) {
        if (rs > 85) {
            p_wpd->radioQuality = 0;
            p_wpd->radioQualityOkMs = 0;
        } else if (rs > 80) {
            p_wpd->radioQualityOkMs++;
            if (p_wpd->radioQualityOkMs >= 20) {
                p_wpd->radioQuality = 0;
                p_wpd->radioQualityOkMs = 0;
            }
        }
    } else {
        if (rs < 75) {
            p_wpd->radioQuality = 1;
            p_wpd->radioQualityOkMs = 0;
        } else if (rs < 80) {
            p_wpd->radioQualityOkMs++;
            if (p_wpd->radioQualityOkMs >= 1) {
                p_wpd->radioQuality = 1;
                p_wpd->radioQualityOkMs = 0;
            }
        }
    }
}

BOOL WPADiIsCoreFormat(u32 fmt) {
    if (fmt == WPAD_FMT_CORE_BTN || fmt == WPAD_FMT_CORE_BTN_ACC || fmt == WPAD_FMT_CORE_BTN_ACC_DPD) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL WPADiIsFsFormat(u32 fmt) {
    if (fmt == WPAD_FMT_FS_BTN || fmt == WPAD_FMT_FS_BTN_ACC || fmt == WPAD_FMT_FS_BTN_ACC_DPD) {
        return TRUE;
    } else {
        return FALSE;
    }
}
BOOL WPADiIsClFormat(u32 fmt) {
    if (fmt == WPAD_FMT_CLASSIC_BTN || fmt == WPAD_FMT_CLASSIC_BTN_ACC || fmt == WPAD_FMT_CLASSIC_BTN_ACC_DPD) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static u8 IsButtonChanged(u16 curr, u16 prev) {
    return (u8)((curr != prev) ? 1 : 0);
}

static u8 IsAnalogChanged(s32 curr, s32 prev, s32 threshold) {
    s32 diff = ((curr - prev) < 0) ? prev - curr : curr - prev;
    return (u8)((diff > threshold) ? 1 : 0);
}

BOOL IsControllerDataChanged(WPADControlBlock* p_wpd, void* p_curr, void* p_prev) {
    u32 fmt;
    u8 mode, result = 0;
    int i;

    mode = p_wpd->devMode;
    fmt = p_wpd->dataFormat;

    if (WPADiIsCoreFormat(fmt)) {
        WPADStatus* curr = (WPADStatus*)p_curr;
        WPADStatus* prev = (WPADStatus*)p_prev;
        result |= IsButtonChanged(curr->button, prev->button);
        if (curr->err == WPAD_ERR_OK && prev->err == WPAD_ERR_OK) {
            if (IsAnalogChanged((s32)(curr->accX), (s32)(prev->accX), 12) | IsAnalogChanged((s32)(curr->accY), (s32)(prev->accY), 12) |
                IsAnalogChanged((s32)(curr->accZ), (s32)(prev->accZ), 12)) {
                DIFF_COUNT_ACC(p_wpd)++;
                if (DIFF_COUNT_ACC(p_wpd) > __WPAD_acc_diff_count_threshold) {
                    DIFF_COUNT_ACC(p_wpd) = 0;
                    HYST_COUNT_ACC(p_wpd) = 0;
                    result |= 1;
                }

            } else {
                HYST_COUNT_ACC(p_wpd) = (u16)((HYST_COUNT_ACC(p_wpd) + 1) % __WPAD_acc_hyst_count_threshold);
                if (HYST_COUNT_ACC(p_wpd) == __WPAD_acc_hyst_count_threshold - 1) {
                    if (DIFF_COUNT_ACC(p_wpd) > 0) {
                        DIFF_COUNT_ACC(p_wpd)--;
                    }
                }
            }

            for (i = 0; i < WPAD_DPD_MAX_OBJECTS; i++) {
                if (IsAnalogChanged((s32)(curr->obj[i].x), (s32)(prev->obj[i].x), 2) |
                    IsAnalogChanged((s32)(curr->obj[i].y), (s32)(prev->obj[i].y), 2)) {
                    DIFF_COUNT_DPD(p_wpd)++;
                    if (DIFF_COUNT_DPD(p_wpd) > __WPAD_dpd_diff_count_threshold) {
                        DIFF_COUNT_DPD(p_wpd) = 0;
                        result |= 1;
                    }
                } else {
                    HYST_COUNT_DPD(p_wpd) = (u16)((HYST_COUNT_DPD(p_wpd) + 1) % __WPAD_dpd_hyst_count_threshold);
                    if (HYST_COUNT_DPD(p_wpd) == __WPAD_dpd_hyst_count_threshold - 1) {
                        if (DIFF_COUNT_DPD(p_wpd) > 0) {
                            DIFF_COUNT_DPD(p_wpd)--;
                        }
                    }
                }
            }
        }

    } else if (WPADiIsFsFormat(fmt)) {
        WPADFSStatus* curr = (WPADFSStatus*)p_curr;
        WPADFSStatus* prev = (WPADFSStatus*)p_prev;

        result |= IsButtonChanged(curr->base.button, prev->base.button);
        if (curr->base.err == WPAD_ERR_OK && prev->base.err == WPAD_ERR_OK) {
            if (IsAnalogChanged((s32)(curr->base.accX), (s32)(prev->base.accX), 12) | IsAnalogChanged((s32)(curr->base.accY), (s32)(prev->base.accY), 12) |
                IsAnalogChanged((s32)(curr->base.accZ), (s32)(prev->base.accZ), 12)) {
                DIFF_COUNT_ACC(p_wpd)++;
                if (DIFF_COUNT_ACC(p_wpd) > __WPAD_acc_diff_count_threshold) {
                    DIFF_COUNT_ACC(p_wpd) = 0;
                    HYST_COUNT_ACC(p_wpd) = 0;
                    result |= 1;
                }

            } else {
                HYST_COUNT_ACC(p_wpd) = (u16)((HYST_COUNT_ACC(p_wpd) + 1) % __WPAD_acc_hyst_count_threshold);
                if (HYST_COUNT_ACC(p_wpd) == __WPAD_acc_hyst_count_threshold - 1) {
                    if (DIFF_COUNT_ACC(p_wpd) > 0) {
                        DIFF_COUNT_ACC(p_wpd)--;
                    }
                }
            }

            for (i = 0; i < WPAD_DPD_MAX_OBJECTS; i++) {
                if (IsAnalogChanged((s32)(curr->base.obj[i].x), (s32)(prev->base.obj[i].x), 2) |
                    IsAnalogChanged((s32)(curr->base.obj[i].y), (s32)(prev->base.obj[i].y), 2)) {
                    DIFF_COUNT_DPD(p_wpd)++;
                    if (DIFF_COUNT_DPD(p_wpd) > __WPAD_dpd_diff_count_threshold) {
                        DIFF_COUNT_DPD(p_wpd) = 0;
                        result |= 1;
                    }
                } else {
                    HYST_COUNT_DPD(p_wpd) = (u16)((HYST_COUNT_DPD(p_wpd) + 1) % __WPAD_dpd_hyst_count_threshold);
                    if (HYST_COUNT_DPD(p_wpd) == __WPAD_dpd_hyst_count_threshold - 1) {
                        if (DIFF_COUNT_DPD(p_wpd) > 0) {
                            DIFF_COUNT_DPD(p_wpd)--;
                        }
                    }
                }
            }

            if (IsAnalogChanged((s32)(curr->fsAccX), (s32)(prev->fsAccX), 12) | IsAnalogChanged((s32)(curr->fsAccY), (s32)(prev->fsAccY), 12) |
                IsAnalogChanged((s32)(curr->fsAccZ), (s32)(prev->fsAccZ), 12)) {
                DIFF_COUNT_FS_ACC(p_wpd)++;
                if (DIFF_COUNT_FS_ACC(p_wpd) > __WPAD_acc_diff_count_threshold) {
                    DIFF_COUNT_FS_ACC(p_wpd) = 0;
                    HYST_COUNT_FS_ACC(p_wpd) = 0;
                    result |= 1;
                }

            } else {
                HYST_COUNT_FS_ACC(p_wpd) = (u16)((HYST_COUNT_FS_ACC(p_wpd) + 1) % __WPAD_acc_hyst_count_threshold);
                if (HYST_COUNT_FS_ACC(p_wpd) == __WPAD_acc_hyst_count_threshold - 1) {
                    if (DIFF_COUNT_FS_ACC(p_wpd) > 0) {
                        DIFF_COUNT_FS_ACC(p_wpd)--;
                    }
                }
            }

            result |= IsAnalogChanged((s32)(curr->fsStickX), (s32)(prev->fsStickX), 1);
            result |= IsAnalogChanged((s32)(curr->fsStickY), (s32)(prev->fsStickY), 1);
        }
    } else if (WPADiIsClFormat(fmt)) {
        WPADCLStatus* curr = (WPADCLStatus*)p_curr;
        WPADCLStatus* prev = (WPADCLStatus*)p_prev;
        s32 lstk;
        s32 rstk;
        s32 trig;

        switch (mode) {
        case 1:
            lstk = 16;
            rstk = 32;
            trig = 8;
            break;
        case 3:
            lstk = 4;
            rstk = 4;
            trig = 1;
            break;
        default:
            lstk = 1;
            rstk = 1;
            trig = 1;
            break;
        }

        result |= IsButtonChanged(curr->base.button, prev->base.button);
        if (curr->base.err == WPAD_ERR_OK && prev->base.err == WPAD_ERR_OK) {
            if (IsAnalogChanged((s32)(curr->base.accX), (s32)(prev->base.accX), 12) | IsAnalogChanged((s32)(curr->base.accY), (s32)(prev->base.accY), 12) |
                IsAnalogChanged((s32)(curr->base.accZ), (s32)(prev->base.accZ), 12)) {
                DIFF_COUNT_ACC(p_wpd)++;
                if (DIFF_COUNT_ACC(p_wpd) > __WPAD_acc_diff_count_threshold) {
                    DIFF_COUNT_ACC(p_wpd) = 0;
                    HYST_COUNT_ACC(p_wpd) = 0;
                    result |= 1;
                }

            } else {
                HYST_COUNT_ACC(p_wpd) = (u16)((HYST_COUNT_ACC(p_wpd) + 1) % __WPAD_acc_hyst_count_threshold);
                if (HYST_COUNT_ACC(p_wpd) == __WPAD_acc_hyst_count_threshold - 1) {
                    if (DIFF_COUNT_ACC(p_wpd) > 0) {
                        DIFF_COUNT_ACC(p_wpd)--;
                    }
                }
            }

            for (i = 0; i < WPAD_DPD_MAX_OBJECTS; i++) {
                if (IsAnalogChanged((s32)(curr->base.obj[i].x), (s32)(prev->base.obj[i].x), 2) |
                    IsAnalogChanged((s32)(curr->base.obj[i].y), (s32)(prev->base.obj[i].y), 2)) {
                    DIFF_COUNT_DPD(p_wpd)++;
                    if (DIFF_COUNT_DPD(p_wpd) > __WPAD_dpd_diff_count_threshold) {
                        DIFF_COUNT_DPD(p_wpd) = 0;
                        result |= 1;
                    }
                } else {
                    HYST_COUNT_DPD(p_wpd) = (u16)((HYST_COUNT_DPD(p_wpd) + 1) % __WPAD_dpd_hyst_count_threshold);
                    if (HYST_COUNT_DPD(p_wpd) == __WPAD_dpd_hyst_count_threshold - 1) {
                        if (DIFF_COUNT_DPD(p_wpd) > 0) {
                            DIFF_COUNT_DPD(p_wpd)--;
                        }
                    }
                }
            }
            result |= IsButtonChanged(curr->clButton, prev->clButton);
            result |= IsAnalogChanged((s32)(curr->clLStickX / lstk), (s32)(prev->clLStickX / lstk), 1);
            result |= IsAnalogChanged((s32)(curr->clLStickY / lstk), (s32)(prev->clLStickY / lstk), 1);
            result |= IsAnalogChanged((s32)(curr->clRStickX / rstk), (s32)(prev->clRStickX / rstk), 1);
            result |= IsAnalogChanged((s32)(curr->clRStickY / rstk), (s32)(prev->clRStickY / rstk), 1);
            result |= IsAnalogChanged((s32)(curr->clTriggerL / trig), (s32)(prev->clTriggerL / trig), 1);
            result |= IsAnalogChanged((s32)(curr->clTriggerR / trig), (s32)(prev->clTriggerR / trig), 1);
        }
    } else {
        WPADStatusEx* curr = (WPADStatusEx*)p_curr;
        WPADStatusEx* prev = (WPADStatusEx*)p_prev;

        result |= IsButtonChanged(curr->base.button, prev->base.button);
        if (curr->base.err == WPAD_ERR_OK && prev->base.err == WPAD_ERR_OK) {
            if (IsAnalogChanged((s32)(curr->base.accX), (s32)(prev->base.accX), 12) |
                IsAnalogChanged((s32)(curr->base.accY), (s32)(prev->base.accY), 12) |
                IsAnalogChanged((s32)(curr->base.accZ), (s32)(prev->base.accZ), 12)) {
                DIFF_COUNT_ACC(p_wpd)++;
                if (DIFF_COUNT_ACC(p_wpd) > __WPAD_acc_diff_count_threshold) {
                    DIFF_COUNT_ACC(p_wpd) = 0;
                    HYST_COUNT_ACC(p_wpd) = 0;
                    result |= 1;
                }

            } else {
                HYST_COUNT_ACC(p_wpd) = (u16)((HYST_COUNT_ACC(p_wpd) + 1) % __WPAD_acc_hyst_count_threshold);
                if (HYST_COUNT_ACC(p_wpd) == __WPAD_acc_hyst_count_threshold - 1) {
                    if (DIFF_COUNT_ACC(p_wpd) > 0) {
                        DIFF_COUNT_ACC(p_wpd)--;
                    }
                }
            }

            for (i = 0; i < WPAD_DPD_MAX_OBJECTS; i++) {
                if (IsAnalogChanged((s32)(curr->base.obj[i].x), (s32)(prev->base.obj[i].x), 2) |
                    IsAnalogChanged((s32)(curr->base.obj[i].y), (s32)(prev->base.obj[i].y), 2)) {
                    DIFF_COUNT_DPD(p_wpd)++;
                    if (DIFF_COUNT_DPD(p_wpd) > __WPAD_dpd_diff_count_threshold) {
                        DIFF_COUNT_DPD(p_wpd) = 0;
                        result |= 1;
                    }
                } else {
                    HYST_COUNT_DPD(p_wpd) = (u16)((HYST_COUNT_DPD(p_wpd) + 1) % __WPAD_dpd_hyst_count_threshold);
                    if (HYST_COUNT_DPD(p_wpd) == __WPAD_dpd_hyst_count_threshold - 1) {
                        if (DIFF_COUNT_DPD(p_wpd) > 0) {
                            DIFF_COUNT_DPD(p_wpd)--;
                        }
                    }
                }
            }
        }
    }

    return result;
}

void WPADRecalibrate(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable = OSDisableInterrupts();
    p_wpd->calibrated = 0;
    OSRestoreInterrupts(enable);
}

inline void CheckButtonCombination(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    WPADCLStatus* status;
    BOOL enable;
    u8 index;
    s8 cnt = 0;

    enable = OSDisableInterrupts();
    index = p_wpd->rxBufIndex == 0;
    status = (WPADCLStatus*)(p_wpd->rxBufs + index);

    if (status->base.button == 0x1C10) {
        cnt = 1;
    }

    if (WPADiIsClFormat(p_wpd->dataFormat)) {
        if (status->clButton == 0x1450) {
            cnt = 1;
        }
    }

    p_wpd->recalibrateCount = cnt != 0 ? p_wpd->recalibrateCount + 1 : 0;

    OSRestoreInterrupts(enable);

    if (p_wpd->recalibrateCount > 600) {
        WPADRecalibrate(chan);
    }
}

static void __SetScreenSaverFlag(BOOL flag) {
    if (flag) {
        VIResetRFIdle();
    }
}

void WPADiCheckContInputs(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL diff = FALSE;
    BOOL enable;
    BOOL screen = FALSE;
    void* p_data;
    u8 index;

    if (p_wpd->status == WPAD_ERR_NO_CONTROLLER) {
        return;
    }

    enable = OSDisableInterrupts();
    index = (u8)((p_wpd->rxBufIndex) ? 0 : 1);
    p_data = (void*)(p_wpd->rxBufs + index);
    diff = IsControllerDataChanged(p_wpd, p_data, p_wpd->prBuf);

    if (diff) {
        memcpy(p_wpd->prBuf, p_data, WPAD_RX_DATASIZE);
    }
    if (((WPADStatus*)p_wpd->prBuf)->err != WPAD_ERR_OK) {
        memset(p_wpd->prBuf, 0, WPAD_RX_DATASIZE);
    }

    OSRestoreInterrupts(enable);
    CheckButtonCombination(chan);

    if (diff) {
        screen = TRUE;
        p_wpd->lastUpdateTime = __OSGetSystemTime();
    } else {
        if (_sleepTime > 0) {
            int sec = (int)OSTicksToSeconds(__OSGetSystemTime() - p_wpd->lastUpdateTime);

            if (sec > 60 * _sleepTime) {
                WPADiDisconnect(chan, TRUE);
            }
        }
    }

    __SetScreenSaverFlag(screen);
}

s8 __wpadGetQueueSize(WPADCmdQueue* queue) {
    BOOL enable;
    s8 num;

    enable = OSDisableInterrupts();

    num = (s8)(queue->tail - queue->head);
    if (num < 0) {
        num += queue->cmdlen;
    }

    OSRestoreInterrupts(enable);
    return num;
}

BOOL WPADiIsAvailableCmdQueue(WPADCmdQueue* queue, s8 cmd_num) {
    s8 curr = __wpadGetQueueSize(queue);

    if (curr + cmd_num <= queue->cmdlen - 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void WPADiClearQueue(WPADCmdQueue* queue) {
    BOOL enable;

    enable = OSDisableInterrupts();

    queue->head = 0;
    queue->tail = 0;
    memset(queue->cmd, 0, queue->cmdlen * sizeof(WPADCommand));

    OSRestoreInterrupts(enable);
}

BOOL __wpadPushCommand(WPADCmdQueue* queue, WPADCommand cmd) {
    BOOL enable;

    enable = OSDisableInterrupts();

    if (__wpadGetQueueSize(queue) == queue->cmdlen - 1) {
        OSRestoreInterrupts(enable);
        return FALSE;
    }

    memset(queue->cmd + queue->tail, 0, sizeof(WPADCommand));
    memcpy(queue->cmd + queue->tail, &cmd, sizeof(WPADCommand));
    queue->tail = (s8)((queue->tail == queue->cmdlen - 1) ? 0 : queue->tail + 1);
    OSRestoreInterrupts(enable);
    return TRUE;
}

BOOL WPADiSendDPDCSB(WPADCmdQueue* queue, BOOL enable, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;

    cmd.command = WPAD_HIDREP_DPDCSB;
    cmd.len = 1;
    cmd.data[0] = enable ? 4 : 0;
    cmd.callback = callback;

    result = __wpadPushCommand(queue, cmd);
    return result;
}

BOOL WPADiSendReadData(WPADCmdQueue* queue, void* p_buf, u16 len, u32 addr, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;

    ASSERT(p_buf != NULL);

    cmd.command = WPAD_HIDREP_RDDATA;
    cmd.len = 6;
    cmd.callback = callback;

    memcpy(cmd.data, &addr, sizeof(addr));
    memcpy(cmd.data + 4, &len, sizeof(len));

    cmd.readBuf = p_buf;
    cmd.readLen = len;
    cmd.readAddr = addr;

    result = __wpadPushCommand(queue, cmd);
    return result;
}

u16 __wpadGetBTEBufferStatus(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    s32 status;
    s8 handle;

    enable = OSDisableInterrupts();
    status = p_wpd->status;
    handle = p_wpd->devHandle;
    OSRestoreInterrupts(enable);

    if (status == WPAD_ERR_NO_CONTROLLER) {
        return 0;
    } else {
        return _WUDGetQueuedSize(handle);
    }
}

u16 __wpadGetBTMBufferStatus(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    s32 status;
    s8 handle;

    enable = OSDisableInterrupts();
    status = p_wpd->status;
    handle = p_wpd->devHandle;
    OSRestoreInterrupts(enable);

    if (status == WPAD_ERR_NO_CONTROLLER) {
        return 0;
    } else {
        return _WUDGetNotAckedSize(handle);
    }
}

void WPADiGetScSettings() {
    if (_scSetting) {
        if (SCCheckStatus() == 0) {
            _dpdSensitivity = __GetDpdSensitivity();
            _sensorBarPos = __GetSensorBarPosition();
            _rumble = __GetMotorMode();
            _speakerVolume = __GetSpeakerVolume();
            _scSetting = 0;
        }
    }
}

void WPADiAfh() {
    BOOL enable;
    u8* channel = (u8*)OSPhysicalToCached(0x31A2);

    DCInvalidateRange(channel, 1);
    DEBUGPrint("WiFi uses channel = %d\n", *channel);

    if (_afhChannel != *channel) {
        enable = OSDisableInterrupts();
        _afhChannel = (s8)*channel;
        OSRestoreInterrupts(enable);
        WUDSetDisableChannel(_afhChannel);
    }
}

void WPADiContMapTableUpdate() {
    if (_scFlush) {
        if (SCCheckStatus() == 0) {
            if (TRUE == SCSetBtDeviceInfoArray(&_scArray)) {
                SCFlushAsync(NULL);
                _scFlush = 0;
            }
        }
    }
}

BOOL WPADiProcessExtCommand(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    WPADCommand cmd;
    s32 err;

    if (p_wpd->used && p_wpd->info.attach) {
        if (WPADiGetCommand(&p_wpd->extCmdQueue, &cmd)) {
            err = WPADiSendData(chan, cmd);
            if (err == WPAD_ERR_OK) {
                WPADiPopCommand(&p_wpd->extCmdQueue);
                return TRUE;
            }
        }
    }
    return FALSE;
}

#if SDK_AUG2010
BOOL WPADiSendGetContStat(WPADCmdQueue* queue, WPADInfo* info, WPADCallback callback) {
#else
BOOL WPADiSendGetContStat(WPADCmdQueue* queue, WPADInfo* info, u32 addr, WPADCallback callback) {
#endif
    WPADCommand cmd;
    BOOL result;

    cmd.command = WPAD_HIDREP_GETSTAT;
    cmd.len = 1;
    cmd.data[0] = 0;
    cmd.callback = callback;
    cmd.info = info;

    result = __wpadPushCommand(queue, cmd);
    return result;
}

BOOL WPADiSendWriteDataCmd(WPADCmdQueue* queue, u8 cmd, u32 addr, WPADCallback callback) {
    return WPADiSendWriteData(queue, &cmd, 1, addr, callback);
}

BOOL WPADiSendWriteData(WPADCmdQueue* queue, void* p_buf, u16 len, u32 addr, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;
    u8 length = (u8)(len & WPAD_WRITE_LEN_MASK);
    ASSERTLINE(5830, len > 0 && len <= 16);
    ASSERTLINE(5831, p_buf != NULL);
    cmd.command = WPAD_HIDREP_WRDATA;
    cmd.len = 21;
    cmd.callback = callback;
    memcpy(cmd.data, &addr, sizeof(addr));
    memcpy(cmd.data + 4, &length, sizeof(length));
    memcpy(cmd.data + 5, p_buf, len);

    result = __wpadPushCommand(queue, cmd);
    return result;
}

BOOL WPADiSendSetPort(WPADCmdQueue* queue, u8 pattern, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;

    cmd.command = WPAD_HIDREP_PORT;
    cmd.len = 1;
    cmd.data[0] = (u8)(pattern << 4);
    cmd.callback = callback;

    result = __wpadPushCommand(queue, cmd);
    return result;
}

static void setupCallback(s32 chan, s32 result) {
    u16 size;
    u32 addr;
    WPADControlBlock* p_wpd = _wpdcb[chan];

    if (result == WPAD_ERR_NO_CONTROLLER) {
        return;
    }

    if (result == WPAD_ESUCCESS) {
        p_wpd->handshakeFinished = TRUE;

        if (p_wpd->connectCallback) {
            p_wpd->connectCallback(chan, result);
        }
    } else {
        size = p_wpd->oldFw != 0 ? 20 : 42;
        addr = p_wpd->oldFw != 0 ? WM_ADDR_MEM_176C : 0;
        WPADiSendReadData(&p_wpd->stdCmdQueue, p_wpd->wmReadDataBuf, size, addr, setupCallback);
    }
}

static void gameInfoCallback(s32 chan, s32 result) {
    WPADControlBlock* p_wpd = _wpdcb[chan];

    if (result != WPAD_ETRANSFER) {
        return;
    }

    WPADiSendReadData(&p_wpd->stdCmdQueue, p_wpd->wmReadDataBuf, sizeof(WPADMEMGameInfo), 0x2A, gameInfoCallback);
}

static void gameInfoCallback2(s32 chan, s32 result) {
    WPADControlBlock* p_wpd = _wpdcb[chan];

    if (result != WPAD_ETRANSFER) {
        return;
    }

    WPADiSendReadData(&p_wpd->stdCmdQueue, p_wpd->wmReadDataBuf, sizeof(WPADMEMGameInfo), 0x62, gameInfoCallback2);
}

static void firmwareCheckCallback(s32 chan, s32 result) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    u16 size;
    u32 addr;
    u8 port;
    BOOL enable;

    if (result == WPAD_ERR_NO_CONTROLLER) {
        return;
    }

    enable = OSDisableInterrupts();
    p_wpd->oldFw = (result == WPAD_ERR_OK) ? TRUE : FALSE;
    p_wpd->status = WPAD_ERR_OK;
    OSRestoreInterrupts(enable);
    size = (u16)((result == WPAD_ERR_OK) ? 20 : 42);
    addr = (u32)((result == WPAD_ERR_OK) ? WM_ADDR_MEM_176C : 0);

    DEBUGPrint(" ==>this error means that the firmware is for NDEV %s\n",
               p_wpd->oldFw != 0 ? "2.0" : "2.1 or later");
    WPADiSendSetReportType(&p_wpd->stdCmdQueue, WPAD_FMT_CORE_BTN, NULL);
    WPADiSendDPDCSB(&p_wpd->stdCmdQueue, FALSE, NULL);
    WPADiSendSetPort(&p_wpd->stdCmdQueue, 0x01 << chan, NULL);
    WPADiSendReadData(&p_wpd->stdCmdQueue, p_wpd->wmReadDataBuf, sizeof(WPADMEMGameInfo),
                      WM_ADDR_MEM_GAME_INFO_0, gameInfoCallback);
    WPADiSendReadData(&p_wpd->stdCmdQueue, p_wpd->wmReadDataBuf, sizeof(WPADMEMGameInfo),
                      WM_ADDR_MEM_GAME_INFO_0 + sizeof(WPADMEMGameInfo), gameInfoCallback2);
    WPADiSendReadData(&p_wpd->stdCmdQueue, p_wpd->wmReadDataBuf, size, addr, setupCallback);
#if SDK_AUG2010
    WPADiSendGetContStat(&p_wpd->stdCmdQueue, NULL, NULL);
#else
    WPADiSendGetContStat(&p_wpd->stdCmdQueue, NULL, 0, NULL);
#endif
}

s32 WPADiRetrieveChannel(u8 dev_handle) {
    u8* devAddr;
    s32 i;

    devAddr = _WUDGetDevAddr(dev_handle);

    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        if (!memcmp(_scArray.regist[i + 10].addr, devAddr, 6)) {
            if (_chan_active_state[i] == 0) {
                _chan_active_state[i] = 1;
                return i;
            }
        }
    }
    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        if (_chan_active_state[i] == 0) {
            _chan_active_state[i] = 1;
            memcpy(_scArray.regist[i + 10].addr, devAddr, 6);
            _scFlush = 1;
            return i;
        }
    }

    return -1;
}

void WPADiConnCallback(u8 dev_handle, u8 open) {
    s32 chan = -1;
    WPADControlBlock* p_wpd;
    BOOL isCmdExist;
    WPADCommand cmd;

    if (open) {
        DEBUGPrint("connection is opened\n");
        chan = WPADiRetrieveChannel(dev_handle);
        p_wpd = _wpdcb[chan];
        _dev_handle_index[dev_handle] = (s8)(chan & 0xff);
        __ClearControlBlock(chan);

        p_wpd->devHandle = (s8)dev_handle;
        p_wpd->devType = WPAD_DEV_CORE;
        p_wpd->dataFormat = WPAD_FMT_CORE_BTN;
        p_wpd->used = TRUE;
        p_wpd->status = WPAD_ERR_OK;
        p_wpd->radioSensitivity = 100;
        p_wpd->disconnect = 0;
        WPADiSendReadData(&p_wpd->stdCmdQueue, p_wpd->wmReadDataBuf, 1, WM_ADDR_MEM_1770, firmwareCheckCallback);
        __SetScreenSaverFlag(TRUE);
    } else {
        DEBUGPrint("connection is closed\n");
        chan = _dev_handle_index[dev_handle];
        _dev_handle_index[dev_handle] = -1;

        if (chan != -1) {
            p_wpd = _wpdcb[chan];
            p_wpd->status = WPAD_ERR_NO_CONTROLLER;

            if (p_wpd->cmdBlkCallback) {
                p_wpd->cmdBlkCallback(chan, WPAD_ERR_NO_CONTROLLER);
            } else if (_wmb[chan].callback) {
                _wmb[chan].callback(chan, WPAD_ERR_NO_CONTROLLER);
            }
            do {
                isCmdExist = WPADiGetCommand(&p_wpd->stdCmdQueue, &cmd);
                if (isCmdExist) {
                    if (cmd.callback) {
                        cmd.callback(chan, WPAD_ERR_NO_CONTROLLER);
                    }
                    WPADiPopCommand(&p_wpd->stdCmdQueue);
                }
            } while (isCmdExist);
            DEBUGPrint("clean up command queue\n");

            if (p_wpd->samplingBufs_ptr) {
                WPADSetAutoSamplingBuf(chan, p_wpd->samplingBufs_ptr, p_wpd->samplingBufLength);
            }

            __ClearControlBlock(chan);
            _chan_active_state[chan] = 0;

            if (p_wpd->connectCallback) {
                p_wpd->connectCallback(chan, WPAD_ERR_NO_CONTROLLER);
            }
        } else {
            DEBUGPrint("WARNING: disconnection for device handle not assigned to channel.\n");
        }
    }
}

void WPADiRecvCallback(u8 dev_handle, u8* p_rpt, u16) {
    u8 chan;
    s32 err;

    chan = (u8)_dev_handle_index[dev_handle];

    if ((chan >= 0) && (chan <= WPAD_MAX_CONTROLLERS)) {
        err = WPADiHIDParser(chan, p_rpt);
        if (err) {
            DEBUGPrint("HID Parser reports: %d\n", err);
        }
    } else {
        DEBUGPrint("WPADiRecvCallback(): Unknown channel %d\n", chan);
    }
}

void WPADGetAccGravityUnit(s32 chan, u32 type, WPADAcc* acc) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;

    enable = OSDisableInterrupts();
    if (acc) {
        switch (type) {
        case WPAD_DEV_CORE:
            acc->x = (s16)(p_wpd->devConf.acc_1g.x - p_wpd->devConf.acc_0g.x);
            acc->y = (s16)(p_wpd->devConf.acc_1g.y - p_wpd->devConf.acc_0g.y);
            acc->z = (s16)(p_wpd->devConf.acc_1g.z - p_wpd->devConf.acc_0g.z);
            break;

        case WPAD_DEV_FS:
            acc->x = (s16)(p_wpd->extConf.fs.acc_1g.x - p_wpd->extConf.fs.acc_0g.x);
            acc->y = (s16)(p_wpd->extConf.fs.acc_1g.y - p_wpd->extConf.fs.acc_0g.y);
            acc->z = (s16)(p_wpd->extConf.fs.acc_1g.z - p_wpd->extConf.fs.acc_0g.z);
            break;
        }
    }
    OSRestoreInterrupts(enable);
}

BOOL WPADiGetCommand(WPADCmdQueue* queue, WPADCommand* cmd) {
    BOOL enable = OSDisableInterrupts();

    if (__wpadGetQueueSize(queue) == 0) {
        OSRestoreInterrupts(enable);
        return FALSE;
    }

    memcpy(cmd, queue->cmd + queue->head, sizeof(WPADCommand));

    OSRestoreInterrupts(enable);
    return TRUE;
}

BOOL WPADiPopCommand(WPADCmdQueue* queue) {
    BOOL enable = OSDisableInterrupts();

    if (__wpadGetQueueSize(queue) == 0) {
        OSRestoreInterrupts(enable);
        return FALSE;
    }

    memset(queue->cmd + queue->head, 0, sizeof(WPADCommand));
    queue->head = (s8)((queue->head == queue->cmdlen - 1) ? 0 : queue->head + 1);

    OSRestoreInterrupts(enable);
    return TRUE;
}

BOOL WPADiProcessCommand(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    WPADCommand cmd;
    s32 err;

    if (p_wpd->used) {
        if (WPADiGetCommand(&p_wpd->stdCmdQueue, &cmd)) {
            err = WPADiSendData(chan, cmd);
            if (err == WPAD_ERR_OK) {
                WPADiPopCommand(&p_wpd->stdCmdQueue);
                return TRUE;
            }
        }
    }
    return FALSE;
}

static void __ClearControlBlock(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];

    p_wpd->rxBufIndex = 0;

    p_wpd->infoOut = NULL;
    p_wpd->motorRunning = FALSE;

    p_wpd->cmdBlkCallback = NULL;
    p_wpd->extensionCallback = NULL;
    p_wpd->samplingCallback = NULL;
    p_wpd->samplingBufs_ptr = NULL;
    p_wpd->samplingBufIndex = 0;
    p_wpd->samplingBufLength = 0;
    p_wpd->dataFormat = WPAD_FMT_CORE_BTN;
    p_wpd->status = WPAD_ERR_NO_CONTROLLER;
    p_wpd->devType = WPAD_DEV_NONE;
    p_wpd->devMode = 0;
    p_wpd->calibrated = 0;
    p_wpd->recalibrateCount = 0;
    p_wpd->statusReqBusy = 0;
    p_wpd->dpdDummyObjSize = 12;

    DIFF_COUNT_DPD(p_wpd) = 0;
    HYST_COUNT_DPD(p_wpd) = 0;
    DIFF_COUNT_ACC(p_wpd) = 0;
    HYST_COUNT_ACC(p_wpd) = 0;
    DIFF_COUNT_FS_ACC(p_wpd) = 0;
    HYST_COUNT_FS_ACC(p_wpd) = 0;

    p_wpd->lastUpdateTime = __OSGetSystemTime();
    p_wpd->lastReportSendTime = __OSGetSystemTime();
    p_wpd->cmdTimeoutAction = 0;
#if SDK_SEP2006
    p_wpd->reqVolume = 0;
    p_wpd->reqVolCb = NULL;
#endif

    p_wpd->wmReadDataPtr = NULL;
    p_wpd->wmReadAddr = 0;
    p_wpd->wmReadLength = 0;
    p_wpd->wmReadErr = 0;

    p_wpd->devHandle = -1;
    p_wpd->used = FALSE;
    p_wpd->handshakeFinished = FALSE;
    p_wpd->oldFw = FALSE;
    p_wpd->radioQuality = 1;
    p_wpd->radioQualityOkMs = 0;
    p_wpd->audioFrames = 0;

    p_wpd->keyIdx = 0;
    p_wpd->radioSensitivity = 0;
    p_wpd->packetCnt = 0;
    p_wpd->disconnect = 1;

    memset(&p_wpd->info, 0, sizeof(WPADInfo));
    memset(p_wpd->wmReadDataBuf, 0, sizeof(p_wpd->wmReadDataBuf));
    memset(p_wpd->rxBufs, 0, WPAD_RX_DATASIZE * 2);
    memset(p_wpd->prBuf, 0, WPAD_RX_DATASIZE);
    memset(&p_wpd->devConf, 0, sizeof(WPADDevConfig));
    memset(&p_wpd->extConf, 0, sizeof(WPADExtConfig));
    memset(&p_wpd->key, 0, sizeof(p_wpd->key));
    memset(&p_wpd->ft, 0, sizeof(p_wpd->ft));
    memset(&p_wpd->sb, 0, sizeof(p_wpd->sb));
    memset(&p_wpd->gameInfo, 0, sizeof(WPADMEMGameInfo));

    p_wpd->gameInfoErr[0] = WPAD_ERR_NO_CONTROLLER;
    p_wpd->gameInfoErr[1] = WPAD_ERR_NO_CONTROLLER;
    p_wpd->stdCmdQueue.cmd = p_wpd->stdCmdQueueList;
    p_wpd->stdCmdQueue.cmdlen = ARRAY_SIZE(p_wpd->stdCmdQueueList);
    p_wpd->extCmdQueue.cmd = p_wpd->extCmdQueueList;
    p_wpd->extCmdQueue.cmdlen = ARRAY_SIZE(p_wpd->extCmdQueueList);
    WPADiClearQueue(&p_wpd->stdCmdQueue);
    WPADiClearQueue(&p_wpd->extCmdQueue);

    _wmb[chan].used = FALSE;
    _wmb[chan].p_buf = NULL;
    _wmb[chan].len = 0;
    _wmb[chan].addr = 0;
    _wmb[chan].callback = NULL;
}

BOOL WPADStartSimpleSync() {
    WUDStartSyncSimple();
}

BOOL WPADStartFastSimpleSync() {
    return WUDStartFastSyncSimple();
}

BOOL WPADStopSimpleSync() {
    return WUDStopSyncSimple();
}

WPADSyncDeviceCallback WPADSetSimpleSyncCallback(WPADSyncDeviceCallback callback) {
    return WUDSetSyncSimpleCallback(callback);
}

void WPADRegisterAllocator(WPADAlloc alloc, WPADFree free) {
    WUDRegisterAllocator(alloc, free);
}

u32 WPADGetWorkMemorySize(void) {
    return WUDGetAllocatedMemSize();
}

s32 WPADGetStatus() {
    return WUDGetStatus();
}

u8 WPADGetSensorBarPosition() {
    BOOL enable;
    u8 pos;

    enable = OSDisableInterrupts();
    pos = _sensorBarPos;
    OSRestoreInterrupts(enable);
    return pos;
}

void WPADDisconnect(s32 chan) {
    memset(&_scArray.regist[chan + 10], 0, sizeof(SCBtDeviceInfo));
    _scFlush = 1;
    WPADiDisconnect(chan, TRUE);
}

void WPADSetAutoSleepTime(u8 minute) {
    BOOL enable = OSDisableInterrupts();
    _sleepTime = minute;
    OSRestoreInterrupts(enable);
}

s32 WPADProbe(s32 chan, u32* devType) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    s32 status;
    BOOL enable = OSDisableInterrupts();
    if (devType != NULL) {
        *devType = p_wpd->devType;
    }
    status = p_wpd->status;
    if (status != WPAD_ERR_NO_CONTROLLER) {
        if (p_wpd->devType == WPAD_DEV_NONE) {
            status = WPAD_ERR_NO_CONTROLLER;
        } else if (!p_wpd->handshakeFinished) {
            status = WPAD_ERR_BUSY;
        }
    }
    OSRestoreInterrupts(enable);
    return status;
}

WPADSamplingCallback WPADSetSamplingCallback(s32 chan, WPADSamplingCallback cb) {
    WPADControlBlock* p_wpd;
    WPADSamplingCallback prev;
    BOOL enable;

    DEBUGPrint("WPADSetSamplingCallback()\n");

    enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];
    prev = p_wpd->samplingCallback;
    p_wpd->samplingCallback = cb;
    OSRestoreInterrupts(enable);
    return prev;
}

WPADConnectCallback WPADSetConnectCallback(s32 chan, WPADConnectCallback cb) {
    WPADControlBlock* p_wpd;
    WPADConnectCallback prev;
    BOOL enable;

    DEBUGPrint("WPADSetConnectCallback()\n");

    enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];
    prev = p_wpd->connectCallback;
    p_wpd->connectCallback = cb;
    OSRestoreInterrupts(enable);
    return prev;
}

WPADExtensionCallback WPADSetExtensionCallback(s32 chan, WPADExtensionCallback cb) {
    WPADControlBlock* p_wpd;
    WPADExtensionCallback prev;
    BOOL enable;

    DEBUGPrint("WPADSetExtensionCallback()\n");

    enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];
    prev = p_wpd->extensionCallback;
    p_wpd->extensionCallback = cb;
    OSRestoreInterrupts(enable);

    return prev;
}

void WPADSetAutoSamplingBuf(s32 chan, void* buf, u32 cnt) {
    WPADControlBlock* p_wpd;
    BOOL enabled;
    s8 err;
    size_t step;
    s32 i;
    size_t off;
    WPADStatus* status;
    WPADStatus* base;

    DEBUGPrint("WPADSetAutoSamplingBuf()\n");
    enabled = OSDisableInterrupts();

    p_wpd = _wpdcb[chan];
    err = p_wpd->status == WPAD_ERR_NO_CONTROLLER ? WPAD_ERR_NO_CONTROLLER : WPAD_ERR_INVALID;

    if (WPADiIsCoreFormat(p_wpd->dataFormat)) {
        step = sizeof(WPADStatus);
    } else if (WPADiIsFsFormat(p_wpd->dataFormat)) {
        step = sizeof(WPADFSStatus);
    } else if (WPADiIsClFormat(p_wpd->dataFormat)) {
        step = sizeof(WPADCLStatus);
    } else {
        step = 0x5A;
    }

    if (buf != NULL) {
        memset(buf, 0, step * cnt);

        off = 0;
        for (i = 0; i < cnt; i++) {
            status = (WPADStatus*)((uintptr_t)buf + off);
            off += step;
            status->err = err;
        }

        p_wpd->samplingBufIndex = -1;
        p_wpd->samplingBufLength = cnt;
    }

    p_wpd->samplingBufs_ptr = buf;

    OSRestoreInterrupts(enabled);
}

u32 WPADGetLatestIndexInBuf(s32 chan) {
    BOOL enable;
    u32 idx;

    enable = OSDisableInterrupts();
    idx = _wpdcb[chan]->samplingBufIndex;
    OSRestoreInterrupts(enable);

    return idx == -1 ? 0 : idx;
}

void WPADiExcludeButton(s32 chan) {
    WPADControlBlock *cb = _wpdcb[chan];
    BOOL irq = OSDisableInterrupts();

    WPADStatus* status = (WPADStatus*)&cb->rxBufs[(u8)(cb->rxBufIndex == 0)];

    if ((status->button & 0x0003) == 0x0003) {
        status->button &= ~0x0002;
    }

    if ((status->button & 0x000C) == 0x000C) {
        status->button &= ~0x0004;
    }

    if (cb->dataFormat == WPAD_FMT_CLASSIC_BTN ||
        cb->dataFormat == WPAD_FMT_CLASSIC_BTN_ACC ||
        cb->dataFormat == WPAD_FMT_CLASSIC_BTN_ACC_DPD) {
        WPADCLStatus* cl = (WPADCLStatus*)status;
        if ((cl->clButton & 0x8002) == 0x8002) {
            cl->clButton &= ~0x8000;
        }

        if ((cl->clButton & 0x4001) == 0x4001) {
            cl->clButton &= ~0x4000;
        }
    }

    OSRestoreInterrupts(irq);
}

static void __SendData(s32 chan, WPADCommand cmd) {
    BOOL enable;
    BOOL motor;
    BT_HDR* p_buf = NULL;
    u8* ptr;
    s8 handle;
    s32 status;
    WPADControlBlock* p_wpd;
    u8 rep_id = (u8)cmd.command;
    u8* p_data = cmd.data;
    u16 len = cmd.len;

    enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];
    status = p_wpd->status;
    handle = p_wpd->devHandle;
    if (handle < 0) {
        OSRestoreInterrupts(enable);
        return;
    }
    p_wpd->status = WPAD_ERR_BUSY;
    motor = p_wpd->motorRunning & _rumble;
    OSRestoreInterrupts(enable);

    if (rep_id == WPAD_HIDREP_VIBRATOR) {
        enable = OSDisableInterrupts();
        p_wpd->status = status;
        OSRestoreInterrupts(enable);
    } else if (rep_id == WPAD_HIDREP_STRM) {
        enable = OSDisableInterrupts();
        p_wpd->status = status;
        p_wpd->audioFrames--;
        OSRestoreInterrupts(enable);
    } else {
        enable = OSDisableInterrupts();
        switch (rep_id) {
        case WPAD_HIDREP_WRDATA:
            break;

        case WPAD_HIDREP_RDDATA:
            p_wpd->wmReadErr = 0;
            p_wpd->wmReadAddr = cmd.readAddr;
            p_wpd->wmReadLength = cmd.readLen;
            p_wpd->wmReadDataPtr = cmd.readBuf;
            break;

        case WPAD_HIDREP_GETSTAT:
            p_wpd->status = status;
            p_wpd->infoOut = cmd.info;
            p_wpd->statusReqBusy = 1;
            break;

        case WPAD_HIDREP_WAIT: {
            OSTick tick;
            memcpy(&tick, cmd.data, sizeof(OSTick));
            p_wpd->lastReportSendTime = tick + __OSGetSystemTime();
            p_wpd->cmdTimeoutAction = 1;
            return;
        }

        default:
            p_data[0] |= 0x2;
            break;
        }

        p_wpd->cmdBlkCallback = cmd.callback;
        p_wpd->lastReportId = rep_id;
        p_wpd->lastReportSendTime = OSSecondsToTicks(_sleepTime * 60) + __OSGetSystemTime();
        p_wpd->cmdTimeoutAction = 0;
        OSRestoreInterrupts(enable);
    }

    DEBUGPrint("handle = %d, repid = %02x\n", handle, rep_id);

    p_buf = GKI_getbuf((u8)(10 + len + sizeof(BT_HDR)));
    p_buf->len = (u8)(len + 1);
    p_buf->offset = 10;
    ptr = (u8*)(p_buf + 1) + p_buf->offset;

    ptr[0] = rep_id;

    memcpy(ptr + 1, p_data, len);

    if (motor) {
        ptr[1] |= 1;
    } else {
        ptr[1] &= ~1;
    }

    BTA_HhSendData((u8)handle, p_buf);
}

s32 WPADiSendData(s32 chan, WPADCommand cmd) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    s32 status;
    BOOL enable;
    BOOL getStat;
    int t;

    status = WPADiGetStatus(chan);

    if (status == WPAD_ERR_OK) {
        enable = OSDisableInterrupts();
        getStat = p_wpd->statusReqBusy;
        OSRestoreInterrupts(enable);

        if (getStat) {
            status = WPAD_ERR_BUSY;
        } else {
            __SendData(chan, cmd);
        }
    } else if (status != WPAD_ERR_BUSY) {
        if (status == WPAD_ERR_TRANSFER) {
            enable = OSDisableInterrupts();
            p_wpd->status = WPAD_ERR_OK;
            OSRestoreInterrupts(enable);
        }
    } else {
        t = (int)OSTicksToSeconds(__OSGetSystemTime() - p_wpd->lastReportSendTime);
        if (t > 1 && _sleepTime > 0) {
            if (p_wpd->cmdTimeoutAction == 0) {
                p_wpd->lastReportSendTime = __OSGetSystemTime();
                WPADiDisconnect(chan, TRUE);
            } else {
                enable = OSDisableInterrupts();
                p_wpd->status = WPAD_ERR_TRANSFER;
                OSRestoreInterrupts(enable);
            }
        }
    }
    return status;
}

void WPADiRumbleMotor(s32 chan, BOOL sendCmd) {
    WPADCommand cmd;

    if (_wpdcb[chan]->status == WPAD_ERR_NO_CONTROLLER) {
        return;
    }

    if (sendCmd == TRUE || __wpadGetQueueSize(&_wpdcb[chan]->stdCmdQueue) > 0) {
        _wpdcb[chan]->motorBusy = FALSE;
    } else if (_rumbleCnt[chan] == 5) {
        _wpdcb[chan]->motorBusy = FALSE;

        cmd.command = WPAD_HIDREP_VIBRATOR;
        cmd.len = 1;
        cmd.data[0] = 0;
        cmd.callback = NULL;

        __SendData(chan, cmd);
    }
    _rumbleCnt[chan] = (u8)((_wpdcb[chan]->motorBusy) ? _rumbleCnt[chan] + 1 : 0);
}

u32 WPADGetDataFormat(s32 chan) {
    WPADControlBlock* p_wpd;
    u32 fmt;
    BOOL enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];
    fmt = p_wpd->dataFormat;
    OSRestoreInterrupts(enable);
    return fmt;
}

s32 WPADSetDataFormat(s32 chan, u32 fmt) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    s32 ret;
    BOOL busy;
    BOOL enable;
    BOOL setup;
    u32 prevFmt;

    enable = OSDisableInterrupts();
    setup = p_wpd->handshakeFinished;
    ret = p_wpd->status;
    prevFmt = p_wpd->dataFormat;
    OSRestoreInterrupts(enable);

    if (ret != WPAD_ERR_NO_CONTROLLER) {
        if (!setup) {
            ret = WPAD_ERR_BUSY;
        } else if (prevFmt != fmt) {
            busy = WPADiSendSetReportType(&p_wpd->stdCmdQueue, fmt, NULL);
            if (!busy) {
                ret = WPAD_ERR_BUSY;
            } else {
                enable = OSDisableInterrupts();
                p_wpd->dataFormat = fmt;
                OSRestoreInterrupts(enable);
                ret = WPAD_ERR_OK;
            }
        } else {
            ret = WPAD_ERR_OK;
        }
    }
    return ret;
}

void WPADiDisconnect(s32 chan, BOOL polite) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    s32 status;

    status = WPADiGetStatus(chan);

    if (status != WPAD_ERR_NO_CONTROLLER) {
        if (polite) {
            enable = OSDisableInterrupts();
            if (p_wpd->disconnect) {
                OSRestoreInterrupts(enable);
                return;
            }
            p_wpd->disconnect = 1;
            OSRestoreInterrupts(enable);
            BTA_HhClose(p_wpd->devHandle);
        } else {
            u8 addr[6];
            WPADGetAddress(chan, addr);
            btm_remove_acl(addr);
        }
    }
}

s32 WPADGetInfoAsync(s32 chan, WPADInfo* info, WPADCallback callback) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    BOOL setup;
    s32 result;

    enable = OSDisableInterrupts();
    setup = p_wpd->handshakeFinished;
    result = p_wpd->status;
    OSRestoreInterrupts(enable);

    if (result != WPAD_ERR_NO_CONTROLLER) {
        if (!setup) {
            result = WPAD_ERR_BUSY;
        } else {
#if SDK_AUG2010
            if (WPADiSendGetContStat(&p_wpd->stdCmdQueue, info, callback)) {
#else
            if (WPADiSendGetContStat(&p_wpd->stdCmdQueue, info, 0, callback)) {
#endif
                result = WPAD_ERR_OK;
            } else {
                result = WPAD_ERR_BUSY;
            }
        }
    }

    return result;
}

void WPADControlMotor(s32 chan, u32 command) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    BOOL needed = FALSE;
    s32 status;

    enable = OSDisableInterrupts();
    status = p_wpd->status;

    if (status == WPAD_ERR_NO_CONTROLLER) {
        OSRestoreInterrupts(enable);
        return;
    }

    if (_rumble == FALSE) {
        if (command != WPAD_MOTOR_STOP || p_wpd->motorRunning != TRUE) {
            OSRestoreInterrupts(enable);
            return;
        }
    }

    if ((command == WPAD_MOTOR_STOP && p_wpd->motorRunning == FALSE) || (command == WPAD_MOTOR_RUMBLE && p_wpd->motorRunning == TRUE)) {
        OSRestoreInterrupts(enable);
        return;
    }

    p_wpd->motorRunning = (command == WPAD_MOTOR_STOP) ? FALSE : TRUE;
    p_wpd->motorBusy = TRUE;
    OSRestoreInterrupts(enable);
}

void WPADEnableMotor(BOOL enable) {
    BOOL intr = OSDisableInterrupts();
    _rumble = enable;
    OSRestoreInterrupts(intr);
}

BOOL WPADIsMotorEnabled() {
    BOOL result;
    BOOL enable = OSDisableInterrupts();
    result = _rumble;
    OSRestoreInterrupts(enable);
    return result;
}

s32 WPADControlLed(s32 chan, u8 pattern, WPADCallback callback) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL busy;
    BOOL enable;
    BOOL setup;
    s32 result = WPAD_ERR_OK;

    enable = OSDisableInterrupts();
    result = p_wpd->status;
    setup = p_wpd->handshakeFinished;
    OSRestoreInterrupts(enable);

    if (result != WPAD_ERR_NO_CONTROLLER) {
        if (!setup) {
            result = WPAD_ERR_BUSY;
        } else {
            busy = WPADiSendSetPort(&p_wpd->stdCmdQueue, pattern, callback);
            result = (busy) ? WPAD_ERR_OK : WPAD_ERR_BUSY;
        }
    }

    if (result != WPAD_ERR_OK) {
        if (callback) {
            callback(chan, result);
        }
    }
    return result;
}

BOOL WPADSaveConfig(WPADFlushCallback callback) {
    BOOL result = TRUE;
    BOOL enable;
    u8 volume;
    u8 mode;

    if (SCCheckStatus() != 0) {
        return FALSE;
    }

    enable = OSDisableInterrupts();
    volume = _speakerVolume;
    mode = (u8)((_rumble) ? 1 : 0);
    OSRestoreInterrupts(enable);

    result &= SCSetWpadSpeakerVolume(volume);
    result &= SCSetWpadMotorMode(mode);

    if (result) {
        SCFlushAsync(callback);
    } else if (callback) {
        callback(2);
    }
    return result;
}

#if SDK_AUG2010
u32 __wpadFmt2Size(u32 fmt) {
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
#endif

u8 WPADGetRadioSensitivity(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL intrStatus = OSDisableInterrupts();
    u8 radioSensitivity = p_wpd->radioSensitivity;

    OSRestoreInterrupts(intrStatus);
    return radioSensitivity;
}

BOOL WPADSetAcceptConnection(u8 accept) {
    BOOL result = FALSE;
    BOOL visible = accept ? TRUE : FALSE;
    if (WUDGetStatus() == 3) {
        WUDSetVisibility(0, visible);
        result = TRUE;
    }

    return result;
}

BOOL WPADGetAcceptConnection(void) {
    if ((u8)WUDGetConnectable() == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

#if SDK_AUG2010
void WPADRead(s32 chan, WPADStatus* status) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL intrStatus;
    u8 rxBufIndex;
    WPADStatus* rxStatus;
    u32 fmtSize;

    ASSERTLINE(4013, (0 <= chan) && (chan < WPAD_MAX_CONTROLLERS));
    ASSERTLINE(4014, status != NULL);

    intrStatus = OSDisableInterrupts();

    rxBufIndex = p_wpd->rxBufIndex != 0 ? 0 : 1;
    rxStatus = (WPADStatus*)&p_wpd->rxBufs[rxBufIndex];
    fmtSize = __wpadFmt2Size(p_wpd->dataFormat);

    if (rxStatus->err != WPAD_ESUCCESS)
        fmtSize = sizeof(WPADStatus);

    memcpy(status, rxStatus, fmtSize);

    OSRestoreInterrupts(intrStatus);
}
#else
void WPADRead(s32 chan, WPADStatus* status) {
    WPADControlBlock* p_wpd;
    BOOL intrStatus;
    u8 step;
    WPADUnkStatus* src;

    intrStatus = OSDisableInterrupts();

    p_wpd = _wpdcb[chan];
    src = &p_wpd->rxBufs[(u8)(p_wpd->rxBufIndex == 0)];

    if (p_wpd->status == WPAD_ERR_OK) {
        if (WPADiIsCoreFormat(p_wpd->dataFormat))
        {
            memcpy(status, src, sizeof(WPADStatus));
        } else if (WPADiIsFsFormat(p_wpd->dataFormat))
        {
            memcpy(status, src, sizeof(WPADFSStatus));
        } else if (WPADiIsClFormat(p_wpd->dataFormat))
        {
            memcpy(status, src, sizeof(WPADCLStatus));
        } else {
            memcpy(status, src, 0x5A);
        }
    } else if (p_wpd->status == WPAD_ERR_BUSY) {
        memcpy(status, src, sizeof(WPADStatus));
    } else {
        if (WPADiIsCoreFormat(p_wpd->dataFormat))
        {
            memset(status, 0, sizeof(WPADStatus));
        } else if (WPADiIsFsFormat(p_wpd->dataFormat))
        {
            memset(status, 0, sizeof(WPADFSStatus));
        } else if (WPADiIsClFormat(p_wpd->dataFormat))
        {
            memset(status, 0, sizeof(WPADCLStatus));
        } else {
            memset(status, 0, 0x5A);
        }

        status->err = p_wpd->status;
    }

    OSRestoreInterrupts(intrStatus);
}
#endif

void WPADiCopyOut(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    WPADUnkStatus* p_data;
    u8 index;
    u32 size;

    enable = OSDisableInterrupts();
    p_data = &p_wpd->rxBufs[(u8)(p_wpd->rxBufIndex == 0)];

    if (p_wpd->samplingBufs_ptr) {
        p_wpd->samplingBufIndex++;
        if (p_wpd->samplingBufIndex >= p_wpd->samplingBufLength) {
            p_wpd->samplingBufIndex = 0;
        }

        if (WPADiIsCoreFormat(p_wpd->dataFormat)) {
            WPADStatus* p_stat = (WPADStatus*)p_wpd->samplingBufs_ptr + p_wpd->samplingBufIndex;
            size = sizeof(WPADStatus);
            memcpy(p_stat, p_data, size);
        } else if (WPADiIsFsFormat(p_wpd->dataFormat)) {
            WPADFSStatus* p_stat = (WPADFSStatus*)p_wpd->samplingBufs_ptr + p_wpd->samplingBufIndex;
            size = (p_data->base.err == WPAD_ERR_OK) ? sizeof(WPADFSStatus) : sizeof(WPADStatus);
            memcpy(p_stat, p_data, size);
        } else if (WPADiIsClFormat(p_wpd->dataFormat)) {
            WPADCLStatus* p_stat = (WPADCLStatus*)p_wpd->samplingBufs_ptr + p_wpd->samplingBufIndex;
            size = (p_data->base.err == WPAD_ERR_OK) ? sizeof(WPADCLStatus) : sizeof(WPADStatus);
            memcpy(p_stat, p_data, size);
        } else {
            WPADStatusEx* p_stat = (WPADStatusEx*)p_wpd->samplingBufs_ptr + p_wpd->samplingBufIndex;
            size = (p_data->base.err == WPAD_ERR_OK) ? sizeof(WPADStatusEx) : sizeof(WPADStatus);
            memcpy(p_stat, p_data, size);
        }
    }
    if (p_wpd->samplingCallback) {
        p_wpd->samplingCallback(chan);
    }

    p_wpd->packetCnt++;
    OSRestoreInterrupts(enable);
}

BOOL WPADIsSpeakerEnabled(s32 chan) {
    BOOL state;
    BOOL enable;
    WPADControlBlock* p_wpd = _wpdcb[chan];
    enable = OSDisableInterrupts();
    state = p_wpd->info.speaker;
    OSRestoreInterrupts(enable);
    return state;
}

BOOL WPADiSendMuteSpeaker(WPADCmdQueue* queue, BOOL enable, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;
    cmd.command = WPAD_HIDREP_MUTE;
    cmd.len = 1;
    cmd.data[0] = (u8)((enable) ? (u8)4 : (u8)0);
    cmd.callback = callback;
    result = __wpadPushCommand(queue, cmd);
    return result;
}

BOOL WPADiSendEnableSpeaker(WPADCmdQueue* queue, BOOL enable, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;
    cmd.command = WPAD_HIDREP_ENSPK;
    cmd.len = 1;
    cmd.data[0] = (u8)((enable) ? (u8)4 : (u8)0);
    cmd.callback = callback;
    result = __wpadPushCommand(queue, cmd);
    return result;
}

s32 WPADControlSpeaker(s32 chan, u32 command, WPADCallback callback) {
    BOOL enable;
    BOOL spk;
    BOOL setup;
    s32 result;
    u8 initCmd[] = {0x00, 0x00, 0xd0, 0x07, 0x40, 0x0c, 0x0e};
    WPADControlBlock* p_wpd = _wpdcb[chan];

    enable = OSDisableInterrupts();
    spk = p_wpd->info.speaker;
    result = p_wpd->status;
    setup = p_wpd->handshakeFinished;
    OSRestoreInterrupts(enable);

    if (result != WPAD_ERR_NO_CONTROLLER) {
        if (!setup) {
            result = WPAD_ERR_BUSY;
        } else if (command == 0) {
            if (!spk) {
                result = WPAD_ERR_OK;
            } else {
                enable = OSDisableInterrupts();
                if (WPADiIsAvailableCmdQueue(&p_wpd->stdCmdQueue, 5)) {
                    WPADiSendMuteSpeaker(&p_wpd->stdCmdQueue, TRUE, NULL);
                    WPADiSendWriteDataCmd(&p_wpd->stdCmdQueue, SPK_DERESET, SPK_RST_REG, NULL);
                    WPADiSendWriteDataCmd(&p_wpd->stdCmdQueue, SPK_CLK_OFF, SPK_CLK_REG, NULL);
#if SDK_AUG2010
                    WPADiSendEnableSpeaker(&p_wpd->stdCmdQueue, FALSE, NULL);
#else
                    WPADiSendEnableSpeaker(&p_wpd->stdCmdQueue, FALSE, callback);
#endif
#if SDK_AUG2010
                    WPADiSendGetContStat(&p_wpd->stdCmdQueue, NULL, callback);
#else
                    WPADiSendGetContStat(&p_wpd->stdCmdQueue, NULL, 0, callback);
#endif
                    OSRestoreInterrupts(enable);
                    return WPAD_ERR_OK;
                } else {
                    result = WPAD_ERR_BUSY;
                }
                OSRestoreInterrupts(enable);
            }
        } else {
            switch (command) {
            case 1:
#if SDK_AUG2010
            case 5:
#endif
                enable = OSDisableInterrupts();
                if (WPADiIsAvailableCmdQueue(&p_wpd->stdCmdQueue, 7)) {
                    WPADiSendEnableSpeaker(&p_wpd->stdCmdQueue, TRUE, NULL);
                    WPADiSendMuteSpeaker(&p_wpd->stdCmdQueue, TRUE, NULL);
                    WPADiSendWriteDataCmd(&p_wpd->stdCmdQueue, SPK_CLK_ON, SPK_CLK_REG, NULL);
                    WPADiSendWriteDataCmd(&p_wpd->stdCmdQueue, SPK_RESET, SPK_RST_REG, NULL);
                    initCmd[4] = _speakerVolume;
#if SDK_AUG2010
                    WPADiSendWriteData(&p_wpd->stdCmdQueue, initCmd, sizeof(initCmd), SPK_RST_REG, NULL);
#else
                    WPADiSendWriteData(&p_wpd->stdCmdQueue, initCmd, sizeof(initCmd), SPK_RST_REG, callback);
#endif
                    WPADiSendMuteSpeaker(&p_wpd->stdCmdQueue, FALSE, NULL);
#if SDK_AUG2010
                    WPADiSendGetContStat(&p_wpd->stdCmdQueue, NULL, callback);
#else
                    WPADiSendGetContStat(&p_wpd->stdCmdQueue, NULL, 0, NULL);
#endif
                    OSRestoreInterrupts(enable);
                    return WPAD_ERR_OK;
                } else {
                    result = WPAD_ERR_BUSY;
                }
                OSRestoreInterrupts(enable);
                break;

            case 2:
                if (!WPADiSendMuteSpeaker(&p_wpd->stdCmdQueue, TRUE, callback)) {
                    result = WPAD_ERR_BUSY;
                } else {
                    return WPAD_ERR_OK;
                }
                break;

            case 3:
                if (!WPADiSendMuteSpeaker(&p_wpd->stdCmdQueue, FALSE, callback)) {
                    result = WPAD_ERR_BUSY;
                } else {
                    return WPAD_ERR_OK;
                }
                break;

            case 4:
                if (!WPADiSendWriteDataCmd(&p_wpd->stdCmdQueue, SPK_CTRL_PLAY, SPK_CTRL_REG, callback)) {
                    result = WPAD_ERR_BUSY;
                } else {
                    return WPAD_ERR_OK;
                }
                break;
            }
        }
    }

    if (callback) {
        callback(chan, result);
    }

    return result;
}

u8 WPADGetSpeakerVolume() {
    BOOL enable;
    u8 vol;

    enable = OSDisableInterrupts();
    vol = _speakerVolume;
    OSRestoreInterrupts(enable);

    return vol;
}

void WPADSetSpeakerVolume(u8 volume) {
    BOOL enable;

    enable = OSDisableInterrupts();
    _speakerVolume = volume;
    OSRestoreInterrupts(enable);
}

BOOL IsBusyStream(s32 chan) {
    BOOL enable;
    WPADControlBlock* p_wpd = _wpdcb[chan];
    u8 stop;
    u32 devType;
    u8 bufNum;
    u16 qSize;
    u16 notAck;
    u8 frames;
    s8 cmdNum;
    u8 linkNum;

    enable = OSDisableInterrupts();
    stop = p_wpd->radioQuality;
    devType = p_wpd->devType;
    bufNum = WUDGetBufferStatus();
    cmdNum = __wpadGetQueueSize(&p_wpd->stdCmdQueue);
    qSize = __wpadGetBTEBufferStatus(chan);
    notAck = __wpadGetBTMBufferStatus(chan);
    frames = p_wpd->audioFrames;
    linkNum = _WUDGetLinkNumber();
    OSRestoreInterrupts(enable);

    if ((stop) || (notAck > 3) || (bufNum == 10) || (bufNum >= linkNum * 2 + 2) || (devType == WPAD_DEV_INITIALIZING) ||
        (cmdNum >= WPAD_COMMAND_CMD_MAX_LEN - 3) || (frames >= 1)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL WPADCanSendStreamData(s32 chan) {
    BOOL enable;
    BOOL setup;
    s32 status;
    WPADControlBlock* p_wpd = _wpdcb[chan];

    enable = OSDisableInterrupts();
    status = p_wpd->status;
    setup = p_wpd->handshakeFinished;
    OSRestoreInterrupts(enable);

    if ((status == WPAD_ERR_NO_CONTROLLER) || (setup == FALSE) || (IsBusyStream(chan))) {
        return FALSE;
    } else {
        return TRUE;
    }
}

BOOL __wpadIsBusyStream(s32 chan) {
    BOOL intrStatus;
    WPADControlBlock* p_wpd = _wpdcb[chan];
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

BOOL WPADiSendStreamData(WPADCmdQueue* queue, void* p_buf, u16 len) {
    WPADCommand cmd;
    BOOL result;
    u8 length = (u8)(len << 3);
    ASSERTLINE(5873, len > 0 && len <= 20);

    cmd.command = WPAD_HIDREP_STRM;
    cmd.len = 21;
    cmd.data[0] = length;
    cmd.callback = NULL;
    memcpy(cmd.data + 1, p_buf, len);
    result = __wpadPushCommand(queue, cmd);
    return result;
}

s32 WPADSendStreamData(s32 chan, void* p_buf, u16 len) {
    BOOL busy;
    BOOL enable;
    BOOL setup;
    s32 status;
    WPADControlBlock* p_wpd = _wpdcb[chan];

    ASSERTLINE(4528, p_buf != NULL);
    ASSERTLINE(4529, len >= 0 && len <= 20);

    enable = OSDisableInterrupts();
    status = p_wpd->status;
    setup = p_wpd->handshakeFinished;
    OSRestoreInterrupts(enable);

    if (status == WPAD_ERR_NO_CONTROLLER) {
        return WPAD_ERR_NO_CONTROLLER;
    }

    if (!setup) {
        return WPAD_ERR_BUSY;
    }

    if (IsBusyStream(chan)) {
        return WPAD_ERR_BUSY;
    }

    if (!WPADiSendStreamData(&p_wpd->stdCmdQueue, p_buf, len)) {
        return WPAD_ERR_BUSY;
    }
    enable = OSDisableInterrupts();
    p_wpd->audioFrames++;
    OSRestoreInterrupts(enable);
    return WPAD_ERR_OK;
}

u8 WPADGetDpdSensitivity() {
    return _dpdSensitivity;
}

BOOL WPADIsDpdEnabled(s32 chan) {
    BOOL state;
    BOOL enable;
    WPADControlBlock* p_wpd = _wpdcb[chan];

    enable = OSDisableInterrupts();
    state = p_wpd->info.dpd;
    OSRestoreInterrupts(enable);
    return state;
}

BOOL WPADiSendEnableDPD(WPADCmdQueue* queue, BOOL enable, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;

    cmd.command = WPAD_HIDREP_ENDPD;
    cmd.len = 1;
    cmd.data[0] = (u8)((enable) ? (u8)4 : (u8)0);
    cmd.callback = callback;

    return __wpadPushCommand(queue, cmd);
}

s32 WPADControlDpd(s32 chan, u32 command, WPADCallback callback) {
    BOOL enable;
    BOOL dpd;
    BOOL setup;
    s32 result;
    WPADControlBlock* p_wpd = _wpdcb[chan];

    static const u8 cfg1[5][9] = {
        0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0x64, 0x00,
        0xfe, 0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0x96,
        0x00, 0xb4, 0x02, 0x00, 0x00, 0x71, 0x01, 0x00,
        0xaa, 0x00, 0x64, 0x02, 0x00, 0x00, 0x71, 0x01,
        0x00, 0xc8, 0x00, 0x36, 0x07, 0x00, 0x00, 0x71,
        0x01, 0x00, 0x72, 0x00, 0x20,
    };
    static const u8 cfg2[5][2] = {
        0xfd, 0x05, 0xb3, 0x04, 0x63, 0x03, 0x35, 0x03,
        0x1f, 0x03,
    };

    enable = OSDisableInterrupts();
    dpd = p_wpd->info.dpd;
    result = p_wpd->status;
    setup = p_wpd->handshakeFinished;
    OSRestoreInterrupts(enable);

    if (result != WPAD_ERR_NO_CONTROLLER) {
        if (!setup) {
            result = WPAD_ERR_BUSY;
        } else if (command == 0) {
            if (!dpd) {
                result = WPAD_ERR_OK;
            } else {
                enable = OSDisableInterrupts();

                if (WPADiIsAvailableCmdQueue(&p_wpd->stdCmdQueue, 3)) {
                    WPADiSendEnableDPD(&p_wpd->stdCmdQueue, FALSE, NULL);
                    WPADiSendDPDCSB(&p_wpd->stdCmdQueue, FALSE, NULL);
#if SDK_AUG2010
                    WPADiSendGetContStat(&p_wpd->stdCmdQueue, NULL, callback);
#else
                    WPADiSendGetContStat(&p_wpd->stdCmdQueue, NULL, 0, callback);
#endif

                    OSRestoreInterrupts(enable);
                    return WPAD_ERR_OK;
                } else {
                    result = WPAD_ERR_BUSY;
                }

                OSRestoreInterrupts(enable);
            }
        } else {
            enable = OSDisableInterrupts();

            if (WPADiIsAvailableCmdQueue(&p_wpd->stdCmdQueue, 8)) {
                WPADiSendEnableDPD(&p_wpd->stdCmdQueue, TRUE, NULL);
                WPADiSendDPDCSB(&p_wpd->stdCmdQueue, TRUE, NULL);
#if SDK_AUG2010
                WPADiSendGetContStat(&p_wpd->stdCmdQueue, NULL, NULL);
#else
                WPADiSendGetContStat(&p_wpd->stdCmdQueue, NULL, WM_REG_DPD_30, NULL);
#endif
                WPADiSendWriteDataCmd(&p_wpd->stdCmdQueue, 1, WM_REG_DPD_30, NULL);
                WPADiSendWriteData(&p_wpd->stdCmdQueue, (void*)cfg1[_dpdSensitivity - 1], 9,
                                   WM_REG_DPD_CONFIG_BLOCK_1, NULL);
                WPADiSendWriteData(&p_wpd->stdCmdQueue, (void*)cfg2[_dpdSensitivity - 1], 2,
                                   WM_REG_DPD_CONFIG_BLOCK_2, NULL);
                WPADiSendWriteDataCmd(&p_wpd->stdCmdQueue, (u8)command,
                                   WM_REG_DPD_DATA_FORMAT, NULL);
                WPADiSendWriteDataCmd(&p_wpd->stdCmdQueue, 8, WM_REG_DPD_30, callback);

                OSRestoreInterrupts(enable);
                return WPAD_ERR_OK;
            } else {
                result = WPAD_ERR_BUSY;
            }
            OSRestoreInterrupts(enable);
        }
    }

    if (callback) {
        callback(chan, result);
    }

    return result;
}

BOOL WPADiSendSetReportType(WPADCmdQueue* queue, u32 fmt, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;

    cmd.command = WPAD_HIDREP_REPTYPE;
    cmd.len = 2;
    cmd.data[0] = 4;
    cmd.callback = callback;

    switch (fmt) {
    case WPAD_FMT_CORE_BTN:
        cmd.data[1] = RPTID_DATA_BTN;
        break;
    case WPAD_FMT_CORE_BTN_ACC:
        cmd.data[1] = RPTID_DATA_BTN_ACC;
        break;
    case WPAD_FMT_CORE_BTN_ACC_DPD:
        cmd.data[1] = RPTID_DATA_BTN_ACC_DPD12;
        break;
    case WPAD_FMT_FS_BTN:
        cmd.data[1] = RPTID_DATA_BTN_EXT8;
        break;
    case WPAD_FMT_FS_BTN_ACC:
        cmd.data[1] = RPTID_DATA_BTN_ACC_EXT16;
        break;
    case WPAD_FMT_FS_BTN_ACC_DPD:
        cmd.data[1] = RPTID_DATA_BTN_ACC_DPD10_EXT9;
        break;
    case WPAD_FMT_CLASSIC_BTN:
        cmd.data[1] = RPTID_DATA_BTN_EXT8;
        break;
    case WPAD_FMT_CLASSIC_BTN_ACC:
        cmd.data[1] = RPTID_DATA_BTN_ACC_EXT16;
        break;
    case WPAD_FMT_CLASSIC_BTN_ACC_DPD:
        cmd.data[1] = RPTID_DATA_BTN_ACC_DPD10_EXT9;
        break;
    case WPAD_FMT_BTN_ACC_DPD_EXTENDED:
        cmd.data[1] = RPTID_DATA_BTN_ACC_DPD18_1;
        break;
    }

    return __wpadPushCommand(queue, cmd);
}

void WPADGetAddress(s32 chan, u8* addr) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    s8 handle;
    u8* devAddr;

    enable = OSDisableInterrupts();
    handle = p_wpd->devHandle;
    OSRestoreInterrupts(enable);

    devAddr = _WUDGetDevAddr((u8)handle);

    if (devAddr) {
        memcpy(addr, devAddr, 6);
    } else {
        memset(addr, 0, 6);
    }
}

void WPADShutdown() {
    return;
}

void __WPADShutdown() {
    BOOL enable;
    int i;

    enable = OSDisableInterrupts();

    if (_shutdown) {
        OSRestoreInterrupts(enable);
        return;
    }

    _shutdown = 1;
    WUDSetVisibility(0, 0);

    for (i = 10; i < 14; i++) {
        memset(&_scArray.regist[i], 0, sizeof(SCBtDeviceInfo));
    }

    WPADiShutdown(FALSE);
    OSRestoreInterrupts(enable);
}

void WPADiShutdown() {
    OSCancelAlarm(&_managerAlarm);
    WUDSetHidRecvCallback(NULL);
    WUDShutdown();
}

BOOL WPADCancelSyncDevice() {
    return WUDCancelSyncDevice();
}
