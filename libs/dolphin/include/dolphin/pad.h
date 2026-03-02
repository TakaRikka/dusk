#ifndef _DOLPHIN_PAD_H_
#define _DOLPHIN_PAD_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PAD_SPEC_0 0
#define PAD_SPEC_1 1
#define PAD_SPEC_2 2
#define PAD_SPEC_3 3
#define PAD_SPEC_4 4
#define PAD_SPEC_5 5

#define PAD_MOTOR_STOP      0
#define PAD_MOTOR_RUMBLE    1
#define PAD_MOTOR_STOP_HARD 2

#define PAD_CHAN0_BIT 0x80000000
#define PAD_CHAN1_BIT 0x40000000
#define PAD_CHAN2_BIT 0x20000000
#define PAD_CHAN3_BIT 0x10000000

#define PAD_MAX_CONTROLLERS     4

#define PAD_BUTTON_LEFT  (1 << 0)   // 0x0001
#define PAD_BUTTON_RIGHT (1 << 1)   // 0x0002
#define PAD_BUTTON_DOWN  (1 << 2)   // 0x0004
#define PAD_BUTTON_UP    (1 << 3)   // 0x0008
#define PAD_TRIGGER_Z    (1 << 4)   // 0x0010
#define PAD_TRIGGER_R    (1 << 5)   // 0x0020
#define PAD_TRIGGER_L    (1 << 6)   // 0x0040
#define PAD_BUTTON_A     (1 << 8)   // 0x0100
#define PAD_BUTTON_B     (1 << 9)   // 0x0200
#define PAD_BUTTON_X     (1 << 10)  // 0x0400
#define PAD_BUTTON_Y     (1 << 11)  // 0x0800
#define PAD_BUTTON_MENU  (1 << 12)  // 0x1000
#define PAD_BUTTON_START (1 << 12)  // 0x1000

#define PAD_ERR_NONE           0
#define PAD_ERR_NO_CONTROLLER -1
#define PAD_ERR_NOT_READY     -2
#define PAD_ERR_TRANSFER      -3

#define RES_WIRELESS_LITE 0x40000

typedef struct PADStatus {
    /* 0x00 */ u16 button;
    /* 0x02 */ s8 stickX;
    /* 0x03 */ s8 stickY;
    /* 0x04 */ s8 substickX;
    /* 0x05 */ s8 substickY;
    /* 0x06 */ u8 triggerLeft;
    /* 0x07 */ u8 triggerRight;
    /* 0x08 */ u8 analogA;
    /* 0x09 */ u8 analogB;
    /* 0x0A */ s8 err;
} PADStatus;

typedef struct PADClampRegion {
    u8 minTrigger;
    u8 maxTrigger;
    s8 minStick;
    s8 maxStick;
    s8 xyStick;
    s8 minSubstick;
    s8 maxSubstick;
    s8 xySubstick;
    s8 radStick;
    s8 radSubstick;
} PADClampRegion;

typedef void (*PADSamplingCallback)(void);

// Pad
int PADReset(u32 mask);
BOOL PADRecalibrate(u32 mask);
BOOL PADInit();
u32 PADRead(PADStatus* status);
void PADSetSamplingRate(u32 msec);
void __PADTestSamplingRate(u32 tvmode);
void PADControlAllMotors(const u32* commandArray);
void PADControlMotor(s32 chan, u32 command);
void PADSetSpec(u32 spec);
u32 PADGetSpec();
int PADGetType(s32 chan, u32* type);
BOOL PADSync(void);
void PADSetAnalogMode(u32 mode);
BOOL __PADDisableRecalibration(BOOL disable);
BOOL PADIsBarrel(s32 chan);

PADSamplingCallback PADSetSamplingCallback(PADSamplingCallback callback);

// Padclamp
void PADClamp(PADStatus* status);
void PADClampCircle(PADStatus* status);

#ifdef __cplusplus
}
#endif

#endif
