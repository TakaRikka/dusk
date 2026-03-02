#ifndef _REVOLUTION_HBM_H_
#define _REVOLUTION_HBM_H_

#include <revolution/kpad.h>
#include <revolution/mem.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NW4HBM_VERSION(major, minor) ((major & 0xFF) << 8 | minor & 0xFF)

typedef enum HBMSelectBtnNum {
    HBM_SELECT_NULL = -1,

    HBM_SELECT_HOMEBTN,
    HBM_SELECT_BTN1,
    HBM_SELECT_BTN2,
    HBM_SELECT_BTN3,
    HBM_SELECT_BTN4,

    HBM_SELECT_MAX
} HBMSelectBtnNum;

enum HBMSoundEvent_et {
    HBM_SOUND_EVENT_0,    // StartInitSound? and then num would not matter
    HBM_SOUND_EVENT_1,    // EndInitSound? and then num would not matter
    HBM_SOUND_EVENT_2,    // Fadeout, num = ms? see calc_fadeoutAnm
    HBM_SOUND_EVENT_3,    // Blackout, num = ms? from various
    HBM_SOUND_EVENT_4,    // ShutdownSound? and then num would not matter
    HBM_SOUND_EVENT_PLAY, /* num = HBMSound_et (see below) */
};

enum HBMSound_et {
    HBM_SOUND_HOME_BUTTON,
    HBM_SOUND_RETURN_APP,
    HBM_SOUND_GOTO_MENU,
    HBM_SOUND_RESET_APP,
    HBM_SOUND_FOCUS,
    HBM_SOUND_SELECT,
    HBM_SOUND_CANCEL,
    HBM_SOUND_OPEN_CONTROLLER,
    HBM_SOUND_CLOSE_CONTROLLER,
    HBM_SOUND_VOLUME_PLUS,
    HBM_SOUND_VOLUME_MINUS,
    HBM_SOUND_VOLUME_PLUS_LIMIT,
    HBM_SOUND_VOLUME_MINUS_LIMIT,
    HBM_SOUND_NOTHING_DONE,
    HBM_SOUND_VIBE_ON,
    HBM_SOUND_VIBE_OFF,
    HBM_SOUND_START_CONNECT_WINDOW,
    HBM_SOUND_CONNECTED1,
    HBM_SOUND_CONNECTED2,
    HBM_SOUND_CONNECTED3,
    HBM_SOUND_CONNECTED4,
    HBM_SOUND_END_CONNECT_WINDOW,
    HBM_SOUND_MANUAL_OPEN,
    HBM_SOUND_MANUAL_FOCUS,
    HBM_SOUND_MANUAL_SELECT,
    HBM_SOUND_MANUAL_SCROLL,
    HBM_SOUND_MANUAL_CANCEL,
    HBM_SOUND_MANUAL_RETURN_APP
};

typedef int HBMSoundCallback(int evt, int num);
typedef struct HBMDataInfo {
    /* 0x00 */ void* layoutBuf;
    /* 0x04 */ void* spkSeBuf;
    /* 0x08 */ void* msgBuf;
    /* 0x0C */ void* configBuf;
    /* 0x10 */ void* mem;
    /* 0x14 */ HBMSoundCallback* sound_callback;
    /* 0x18 */ int backFlag;
    /* 0x1C */ int region;
    /* 0x20 */ int cursor;
    /* 0x28 */ u32 memSize;
    /* 0x2C */ f32 frameDelta;
    /* 0x30 */ Vec2 adjust;
    /* 0x38 */ MEMAllocator* pAllocator;
} HBMDataInfo;  // size 0x3C

typedef struct HBMKPadData {
    KPADStatus* kpad;
    Vec2 pos;
    u32 use_devtype;
} HBMKPadData;  // size 0x10

typedef struct HBMControllerData {
    HBMKPadData wiiCon[WPAD_MAX_CONTROLLERS];
} HBMControllerData;  // size 0x40

void HBMCreate(const HBMDataInfo* pHBInfo);
void HBMDelete(void);
void HBMInit(void);
HBMSelectBtnNum HBMCalc(const HBMControllerData* pController);
void HBMDraw(void);
HBMSelectBtnNum HBMGetSelectBtnNum(void);
void HBMSetAdjustFlag(BOOL flag);
void HBMStartBlackOut(void);
BOOL HBMIsReassignedControllers(void);

void HBMCreateSound(void* soundData, void* memBuf, u32 memSize);
void HBMDeleteSound(void);
void HBMUpdateSound(void);

#ifdef __cplusplus
}
#endif

#endif  // _REVOLUTION_HBM_H_
