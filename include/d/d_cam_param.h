#ifndef D_D_CAM_PARAM_H
#define D_D_CAM_PARAM_H

#include "SSystem/SComponent/c_angle.h"

struct dCamMath {
    static f32 rationalBezierRatio(f32, f32);
    static f32 zoomFovy(f32, f32);
    static cXyz xyzRotateX(cXyz&, cSAngle);
    static cXyz xyzRotateY(cXyz&, cSAngle);
    static f32 xyzHorizontalDistance(cXyz&, cXyz&);
};

class dCstick_c {
public:
    dCstick_c();
    bool Shift(u32);

    /* 0x0 */ f32 mThresholdLow;
    /* 0x4 */ f32 mThresholdHigh;
    /* 0x8 */ int mInputSpeed;
    /* 0xC */ u8 field_0xc[4];

    virtual ~dCstick_c() {}

    f32 SwTHH() { return mThresholdHigh; }
};

class dCamBGChk_c {
public:
    dCamBGChk_c();

    f32 WallUpDistance() { return mWallUpDistance; }
    f32 FwdDistance(s32 param_0) { return mChkInfo[param_0].mDistance; }
    s16 FwdChkAngle(s32 param_0) { return cAngle::d2s(mChkInfo[param_0].mChkAngle); }
    f32 FwdWeightH(s32 param_0) { return mChkInfo[param_0].mWeightH; }
    f32 FwdWeightL(s32 param_0) { return mChkInfo[param_0].mWeightL; }
    f32 FwdBackMargin() { return mFwdBackMargin; }
    f32 FwdCushion() { return mFwdCushion; }
    f32 GazeBackMargin() { return mGazeBackMargin; }
    f32 WallCushion() { return mWallCushion; }
    f32 WallBackCushion() { return mWallBackCushion; }
    f32 CornerCushion() { return mCornerCushion; }
    f32 CornerAngleMax() { return mCornerAngleMax; }
    f32 FloorMargin() { return mFloorMargin; }

    // name is a guess for now
    struct ChkInfo {
        /* 0x0 */ f32 mDistance;
        /* 0x4 */ f32 mChkAngle;
        /* 0x8 */ f32 mWeightH;
        /* 0xC */ f32 mWeightL;
    };  // Size: 0x10

    /* 0x00 */ f32 mFloorMargin;
    /* 0x04 */ ChkInfo mChkInfo[2];
    /* 0x24 */ f32 mFwdBackMargin;
    /* 0x28 */ f32 mFwdCushion;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 mGazeBackMargin;
    /* 0x34 */ f32 mCornerCushion;
    /* 0x38 */ f32 mWallCushion;
    /* 0x3C */ f32 mWallUpDistance;
    /* 0x40 */ f32 mWallBackCushion;
    /* 0x44 */ f32 mCornerAngleMax;
};

struct dCamStyleData {
    struct StyleData {
        /* 0x0 */ u32 field_0x0;
        /* 0x4 */ u16 field_0x4;
        /* 0x6 */ u16 field_0x6;
        /* 0x8 */ f32 field_0x8[28];
    };  // Size: 0x78

    /* 0x0 */ u8 field_0x0[4];
    /* 0x4 */ int mStyleNum;
    /* 0x8 */ StyleData* mStyleData;
};

class dCamParam_c {
public:
    dCamParam_c(s32);
    int Change(s32);
    int SearchStyle(u32);

    void Arg2(s16 val) { mMapToolArg2 = val; }
    s16 Arg2() { return mMapToolArg2; }
    u8 Arg1() { return mMapToolArg1; }
    void Arg1(u8 val) { mMapToolArg1 = val; }
    u8 Arg0() { return mMapToolArg0; }
    void Arg0(u8 val) { mMapToolArg0 = val; }
    void Fovy(u8 val) { mMapToolFovy = val; }
    u8 Fovy() { return mMapToolFovy; }
    bool CheckFlag(u16 flag) { return (flag & mCurrentStyle->field_0x6) != 0; }
    f32 Val(s32 param_0, int param_1) {
        return mCamStyleData[param_0].field_0x8[param_1];
    }
    void SetVal(s32 param_0, int param_1, f32 i_value) {
        mCamStyleData[param_0].field_0x8[param_1] = i_value;
    }

    /* 0x00 */ u8 mMapToolFovy;
    /* 0x01 */ u8 mMapToolArg0;
    /* 0x02 */ u8 mMapToolArg1;
    /* 0x04 */ int mMapToolArg2;
    /* 0x08 */ dCamStyleData::StyleData* mCamStyleData;
    /* 0x0C */ s32 mStyleNum;
    /* 0x10 */ dCamStyleData::StyleData* mCurrentStyle;
    /* 0x14 */ int mStyleID;
    /* 0x18 */ u8 field_0x18[4];

    u32 Id(s32 i_style) { return mCamStyleData[i_style].field_0x0; }
    int Algorythmn(s32 i_style) { return mCamStyleData[i_style].field_0x4; }
    int Algorythmn() { return mCurrentStyle->field_0x4; }
    u16 Flag(s32 param_0, u16 param_1) { return mCamStyleData[param_0].field_0x6 & param_1; }
    void SetFlag(u16 i_flag) { mCurrentStyle->field_0x6 |= i_flag; }

    virtual ~dCamParam_c();
};

class dCamSetup_c {
public:
    dCamSetup_c();
    bool CheckLatitudeRange(s16*);
    f32 PlayerHideDist();

    bool CheckFlag2(u16 i_flag) { return (i_flag & mFlags2) != 0; }
    f32 WaitRollSpeed() { return mWaitRollSpeed; }
    int WaitRollTimer() { return mWaitRollTimer; }
    int ThrowTimer() { return mThrowTimer; }
    f32 ThrowCushion() { return mThrowCushion; }
    f32 ThrowVAngle() { return mThrowVAngle; }
    f32 ThrowCtrAdjust() { return mThrowCtrOffset; }
    f32 ChargeBRatio() { return mChargeBRatio; }
    int ChargeTimer() { return mChargeTimer; }
    f32 ChargeLatitude() { return mChargeLatitude; }

    bool CheckFlag(u16 i_flag) { return (i_flag & mDebugFlags) != 0; }
    f32 ManualEndVal() { return mManualEndVal; }
    f32 CinemaScopeTrimHeight() { return mTrimCineScopeHeight; }
    f32 VistaTrimHeight() { return mTrimVistaHeight; }
    f32 ForceLockOffTimer() { return mForceLockOffTimer; }
    f32 ForceLockOffDist() { return mForceLockOffDist; }
    f32 USOValue() { return mFalseValue; }
    f32 USOAngle() { return mFalseAngle; }
    f32 WindShakeGap4Ctr() { return mWindShakeCtr; }
    f32 WindShakeGap4Fvy() { return mWindShakeFvy; }
    f32 LockonChangeCushion() { return mLockonChangeCushion; }
    int LockonChangeTimer() { return mLockonChangeTimer; }
    f32 Cushion4Base() { return mBaseCushion; }
    f32 Cushion4Jump() { return mJumpCushion; }
    f32 CusCus() { return mCusCus; }
    f32 ParallelDist() { return mParallelDist; }
    f32 CurveWeight() { return mCurveWeight; }
    s16 MapToolCameraLongTimer() { return mMapToolCamLongTimer; }
    s16 MapToolCameraShortTimer() { return mMapToolCamShortTimer; }

    /* 0x000 */ f32 mDrawNear;
    /* 0x004 */ f32 mDrawFar;
    /* 0x008 */ u16 mDebugFlags;
    /* 0x00A */ u16 mFlags2;
    /* 0x00C */ int field_0xc;
    /* 0x010 */ int mModeSwitchType;
    /* 0x014 */ void* mTypeTable;
    /* 0x018 */ int mTypeNum;
    /* 0x01C */ int mForceType;
    /* 0x020 */ f32 mCusCus;
    /* 0x024 */ f32 field_0x24;
    /* 0x028 */ f32 field_0x28;
    /* 0x02C */ f32 field_0x2c;
    /* 0x030 */ f32 field_0x30;
    /* 0x034 */ f32 field_0x34;
    /* 0x038 */ f32 mBaseCushion;
    /* 0x03C */ f32 mJumpCushion;
    /* 0x040 */ f32 mParallelDist;
    /* 0x044 */ f32 field_0x44;
    /* 0x048 */ f32 mSubjLinkCullDist;
    /* 0x04C */ f32 mCurveWeight;
    /* 0x050 */ f32 field_0x50;
    /* 0x054 */ f32 field_0x54;
    /* 0x058 */ f32 field_0x58;
    /* 0x05C */ f32 field_0x5c;
    /* 0x060 */ f32 field_0x60;
    /* 0x064 */ f32 mTrimVistaHeight;
    /* 0x068 */ f32 mTrimCineScopeHeight;
    /* 0x06C */ f32 field_0x6c;
    /* 0x070 */ f32 mForceLockOffDist;
    /* 0x074 */ int mForceLockOffTimer;
    /* 0x078 */ f32 field_0x78;
    /* 0x07C */ f32 field_0x7c;
    /* 0x080 */ f32 field_0x80;
    /* 0x084 */ f32 field_0x84;
    /* 0x088 */ f32 mThrowVAngle;
    /* 0x08C */ f32 mThrowCtrOffset;
    /* 0x090 */ f32 mThrowCushion;
    /* 0x094 */ int mThrowTimer;
    /* 0x098 */ f32 mWindShakeCtr;
    /* 0x09C */ f32 field_0x9c;
    /* 0x0A0 */ f32 mWindShakeFvy;
    /* 0x0A4 */ f32 mFalseValue;
    /* 0x0A8 */ f32 mFalseAngle;
    /* 0x0AC */ f32 field_0xac;
    /* 0x0B0 */ int field_0xb0;
    /* 0x0B4 */ f32 field_0xb4;
    /* 0x0B8 */ f32 mManualStartCThreshold;
    /* 0x0BC */ f32 mManualEndVal;
    /* 0x0C0 */ f32 mChargeLatitude;
    /* 0x0C4 */ int mChargeTimer;
    /* 0x0C8 */ f32 mChargeBRatio;
    /* 0x0CC */ int mLockonChangeTimer;
    /* 0x0D0 */ f32 mLockonChangeCushion;
    /* 0x0D4 */ f32 field_0xd4;
    /* 0x0D8 */ f32 field_0xd8;
    /* 0x0DC */ s16 mMapToolCamShortTimer;
    /* 0x0DE */ s16 mMapToolCamLongTimer;
    /* 0x0E0 */ f32 field_0xe0;
    /* 0x0E4 */ f32 field_0xe4;
    /* 0x0E8 */ f32 field_0xe8;
    /* 0x0EC */ int field_0xec;
    /* 0x0F0 */ int field_0xf0;
    /* 0x0F4 */ f32 field_0xf4;
    /* 0x0F8 */ f32 field_0xf8;
    /* 0x0FC vtable */

    virtual ~dCamSetup_c();

    /* 0x100 */ dCstick_c mCStick;
    /* 0x114 */ dCamBGChk_c mBGChk;
    /* 0x15C */ f32 field_0x15c;
    /* 0x160 */ f32 field_0x160;
    /* 0x164 */ int mWaitRollTimer;
    /* 0x168 */ f32 mWaitRollSpeed;
    /* 0x16C */ f32 field_0x16c;
    /* 0x170 */ int mScreensaverFirstWaitTimer;
    /* 0x174 */ int mScreensaverWaitTimer;
    /* 0x178 */ int mScreensaverExecTimer;
};

#endif /* D_D_CAM_PARAM_H */
