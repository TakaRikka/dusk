#ifndef _REVOLUTION_KPAD_H_
#define _REVOLUTION_KPAD_H_

#include <revolution/mtx.h>
#include <revolution/wpad.h>
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define KPAD_BUTTON_MASK 0x0000ffff
#define KPAD_BUTTON_RPT 0x80000000

#define KPAD_BTN_RPT_TIME_MAX 20000
#define KPAD_BTN_NO_RPT_DELAY 40000

#define KPAD_CMOS_RESO_WX 128
#define KPAD_CMOS_RESO_WY 96
#define KPAD_CMOS_RESO_CX (KPAD_CMOS_RESO_WX / 2)
#define KPAD_CMOS_RESO_CY (KPAD_CMOS_RESO_WY / 2)

#define KPAD_DPD_RESO_WX WPAD_DPD_IMG_RESO_WX
#define KPAD_DPD_RESO_WY WPAD_DPD_IMG_RESO_WY
#define KPAD_DPD_RESO_CX (KPAD_DPD_RESO_WX / 2)
#define KPAD_DPD_RESO_CY (KPAD_DPD_RESO_WY / 2)

#define KPAD_USE_OBJECTS 2

typedef struct Vec2 {
	f32 x, y;
} Vec2;

typedef enum KPADPlayMode { KPAD_PLAY_MODE_LOOSE = 0, KPAD_PLAY_MODE_TIGHT } KPADPlayMode;

typedef s32 KPADResult;

typedef union KPADEXStatus {
    struct {
        Vec2 stick;
        Vec acc;
        f32 acc_value;
        f32 acc_speed;
    } fs;  // size 0x1c

    struct {
        u32 hold;
        u32 trig;
        u32 release;
        Vec2 lstick;
        Vec2 rstick;
        f32 ltrigger;
        f32 rtrigger;
    } cl;  // size 0x24
} KPADEXStatus;

typedef struct KPADStatus {
    /* 0x00 */ u32 hold;
    /* 0x04 */ u32 trig;
    /* 0x08 */ u32 release;
    /* 0x0C */ Vec acc;
    /* 0x18 */ f32 acc_value;
    /* 0x1C */ f32 acc_speed;
    /* 0x20 */ Vec2 pos;
    /* 0x28 */ Vec2 vec;
    /* 0x30 */ f32 speed;
    /* 0x34 */ Vec2 horizon;
    /* 0x3C */ Vec2 hori_vec;
    /* 0x44 */ f32 hori_speed;
    /* 0x48 */ f32 dist;
    /* 0x4C */ f32 dist_vec;
    /* 0x50 */ f32 dist_speed;
    /* 0x54 */ Vec2 acc_vertical;
#if PLATFORM_SHIELD
    /* 0x5C */ u8 dev_type;
    /* 0x5D */ s8 wpad_err;
    /* 0x5E */ s8 dpd_valid_fg;
    /* 0x5F */ u8 data_format;
    /* 0x60 */ KPADEXStatus ex_status;
    /* 0x84 */ u8 unk_shield_0x86[0xf0 - 0x84];
#else
    /* 0x5C */ u32 dev_type;
    /* 0x60 */ KPADEXStatus ex_status;
    /* 0x84 */ s8 dpd_valid_fg;
    /* 0x85 */ s8 wpad_err;
#endif
#if PLATFORM_SHIELD
#endif
} KPADStatus; // size 0x88

typedef struct KPADUnifiedWpadStatus {
    union {
        WPADStatus core;
        WPADFSStatus fs;
        WPADCLStatus cl;
    } u;
} KPADUnifiedWpadStatus;

typedef struct {
    Vec2 center;
    s8 error_fg;
    s8 state_fg;
    s8 _fg_1;
    s8 _fg_2;
} KPADObject;

typedef union {
    KPADStatus k;
    KPADUnifiedWpadStatus w;
} KPADTmpStatus;

typedef void (*KPADControlDpdCallback)(s32 chan, s32 reason);
typedef WPADCallback KPADCallback;
typedef WPADChannel KPADChannel;

typedef struct {
    /* 0x000 */ KPADStatus status;
    /* 0x088 */ f32 pos_play_radius;
    /* 0x08C */ f32 pos_sensitivity;
    /* 0x090 */ f32 hori_play_radius;
    /* 0x094 */ f32 hori_sensitivity;
    /* 0x098 */ f32 dist_play_radius;
    /* 0x09C */ f32 dist_sensitivity;
    /* 0x0A0 */ f32 acc_play_radius;
    /* 0x0A4 */ f32 acc_sensitivity;
    /* 0x0A8 */ f32 dist_org;
    /* 0x0AC */ Vec2 accXY_nrm_hori;
    /* 0x0B4 */ Vec2 sec_nrm_hori;
    /* 0x0BC */ Vec2 center_org;
    /* 0x0C4 */ f32 dpd2pos_scale;
    /* 0x0C8 */ KPADObject kobj_sample[4];
    /* 0x0F8 */ KPADObject kobj_regular[2];
    /* 0x110 */ s16 valid_objs;
    /* 0x112 */ u16 repeat_count;
    /* 0x114 */ s16 active_chan;
    /* 0x116 */ s16 unk_0x116;
    /* 0x118 */ KPADUnifiedWpadStatus uniRingBuf[12];
    /* 0x3A0 */ f32 sec_length;
    /* 0x3A4 */ Vec2 sec_nrm;
    /* 0x3AC */ f32 sec_dist;
    /* 0x3B0 */ f32 trust_sec_length;
    /* 0x3B4 */ Vec hard_acc;
    /* 0x3C0 */ Vec2 obj_horizon;
    /* 0x3C8 */ Vec2 acc_horizon;
    /* 0x3D0 */ Vec2 ah_circle_pos;
    /* 0x3D8 */ u16 ah_circle_ct;
    /* 0x3DA */ u8 dpd_valid2_ct;
    /* 0x3DC */ u16 btn_repeat_time;
    /* 0x3DE */ u16 btn_repeat_next;
    /* 0x3E0 */ u16 btn_repeat_delay;
    /* 0x3E2 */ u16 btn_repeat_pulse;
    /* 0x3E4 */ u16 btn_cl_repeat_time;
    /* 0x3E6 */ u16 btn_cl_repeat_next;
    /* 0x3E8 */ u16 btn_cl_repeat_delay;
    /* 0x3EA */ u16 btn_cl_repeat_pulse;
    /* 0x3EC */ BOOL is_dpd_enabled;
    /* 0x3F0 */ BOOL unk_0x3f0;
    /* 0x3F4 */ BOOL unk_0x3f4;
    /* 0x3F8 */ KPADControlDpdCallback dpd_ctrl_callback;
    /* 0x3FC */ BOOL unk_0x3fc;
} KPADInsideStatus;

void KPADInit();

void KPADSetFSStickClamp(s8 min, s8 max);
void KPADSetBtnRepeat(s32 chan, f32 delay_sec, f32 pulse_sec);
void KPADSetObjInterval(f32 interval);
void KPADSetPosParam(s32 chan, f32 play_radius, f32 sensitivity);
void KPADSetHoriParam(s32 chan, f32 play_radius, f32 sensitivity);
void KPADSetDistParam(s32 chan, f32 play_radius, f32 sensitivity);
void KPADSetAccParam(s32 chan, f32 play_radius, f32 sensitivity);
void KPADSetSensorHeight(s32 chan, f32 level);


void KPADReset(void);
s32 KPADRead(s32 chan, KPADStatus* sampling_bufs, s32 length);

void KPADEnableDPD(s32 chan);
void KPADDisableDPD(s32 chan);

extern KPADInsideStatus inside_kpads[];

#ifdef __cplusplus
}
#endif

#endif  // _REVOLUTION_KPAD_H_
