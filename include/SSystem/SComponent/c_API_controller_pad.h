#ifndef C_API_CONTROLLER_PAD_
#define C_API_CONTROLLER_PAD_

#include <dolphin/types.h>

struct interface_of_controller_pad {
    /* 0x00 */ f32 mMainStickPosX;
    /* 0x04 */ f32 mMainStickPosY;
    /* 0x08 */ f32 mMainStickValue;
    /* 0x0C */ s16 mMainStickAngle;
    /* 0x0E */ u8 field_0xe;
    /* 0x0F */ u8 field_0xf;
    /* 0x10 */ f32 mCStickPosX;
    /* 0x14 */ f32 mCStickPosY;
    /* 0x18 */ f32 mCStickValue;
    /* 0x1C */ s16 mCStickAngle;
    /* 0x1E */ u8 field_0x1e;
    /* 0x1F */ u8 field_0x1f;
    /* 0x20 */ f32 mAnalogA;
    /* 0x24 */ f32 mAnalogB;
    /* 0x28 */ f32 mTriggerLeft;
    /* 0x2C */ f32 mTriggerRight;
    /* 0x30 */ u32 mButtonFlags;
    /* 0x34 */ u32 mPressedButtonFlags;
    /* 0x38 */ s8 mGamepadErrorFlags;
    /* 0x39 */ u8 mHoldLockL;
    /* 0x3A */ u8 mTrigLockL;
    /* 0x3B */ u8 mHoldLockR;
    /* 0x3C */ u8 mTrigLockR;
    /* 0x3D */ u8 field_0x3d;
    /* 0x3E */ u8 field_0x3e;
    /* 0x3F */ u8 field_0x3f;
};

void cAPICPad_recalibrate(void);
u32 cAPICPad_ANY_BUTTON(u32 param_0);

#endif
