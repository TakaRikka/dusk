/**
 * d_cam_param.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_cam_param.h"

#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"

f32 dCamMath::rationalBezierRatio(f32 param_0, f32 param_1) {
    f64 var_f31;
    f32 var_f1 = param_0;

    if (var_f1 >= 0.0f) {
        var_f31 = 1.0;
    } else {
        var_f31 = -1.0;
        var_f1 = -var_f1;
    }

    f64 temp_f2 = 2.0 * var_f1;
    f64 temp_f2_2 = ((temp_f2 * param_1) - temp_f2) - (2.0 * param_1);
    f64 temp_f29 = -temp_f2_2;
    f64 temp_f28 = temp_f29 - 1.0;
    f64 var_f1_2 = (temp_f2_2 * temp_f2_2) - (4.0 * temp_f28 * var_f1);

    if (var_f1_2 > 0.0) {
        var_f1_2 = sqrt(var_f1_2);
    } else {
        var_f1_2 = 0.0;
    }

    f64 temp = temp_f29 - var_f1_2;
    f64 temp_f2_3 = temp_f28 * 2.0;
    if (temp_f2_3 > 1e-07 || temp_f2_3 < -1e-07) {
        f64 temp_f2_4 = temp / temp_f2_3;
        f64 temp_f3 = temp_f2_4 * temp_f2_4;
        f64 temp_f4 = 1.0 - temp_f2_4;
        f64 temp_f1 = temp_f3 + ((temp_f4 * temp_f4) + (param_1 * (2.0 * temp_f4 * temp_f2_4)));

        if (temp_f1 > 1.0000000116860974e-07) {
            return var_f31 * (temp_f3 / temp_f1);
        }
        return 0.0f;
    }

    return 0.0f;
}

static f32 dummy_literal1() {
    return 1.0f;
}

f32 dCamMath::zoomFovy(f32 param_0, f32 param_1) {
    cDegree deg(param_0);

    f32 radian = cM_atan2f(deg.Sin(), param_1 * deg.Cos());
    return cAngle::r2d(radian);
}

cXyz dCamMath::xyzRotateX(cXyz& i_xyz, cSAngle i_angle) {
    Mtx m;
    cXyz rot_xyz;

    s16 angle = i_angle.Val();
    mDoMtx_XrotS(m, angle);
    MTXMultVec(m, &i_xyz, &rot_xyz);
    return rot_xyz;
}

cXyz dCamMath::xyzRotateY(cXyz& i_xyz, cSAngle i_angle) {
    Mtx m;
    cXyz rot_xyz;

    s16 angle = i_angle.Val();
    mDoMtx_YrotS(m, angle);
    MTXMultVec(m, &i_xyz, &rot_xyz);
    return rot_xyz;
}

f32 dCamMath::xyzHorizontalDistance(cXyz& i_posA, cXyz& i_posB) {
    f64 x = i_posA.x - i_posB.x;
    f64 z = i_posA.z - i_posB.z;

    return sqrt((x * x) + (z * z));
}

dCstick_c::dCstick_c() {
    mThresholdLow = 0.2f;
    mThresholdHigh = 0.95f;
    mInputSpeed = 6;
}

bool dCstick_c::Shift(u32 param_0) {
    return false;
}

dCamBGChk_c::dCamBGChk_c() {
    mFloorMargin = 32.0f;

    mChkInfo[0].mDistance = 1.0f;
    mChkInfo[0].mChkAngle = 25.0f;
    mChkInfo[0].mWeightH = 0.4f;
    mChkInfo[0].mWeightL = 0.6f;

    mChkInfo[1].mDistance = 3.0f;
    mChkInfo[1].mChkAngle = 15.0f;
    mChkInfo[1].mWeightH = 0.5f;
    mChkInfo[1].mWeightL = 0.3f;

    mFwdBackMargin = 10.0f;
    mFwdCushion = 0.1f;
    field_0x2c = 0.2f;
    mGazeBackMargin = 10.0f;
    mCornerCushion = 0.75f;
    mWallCushion = 0.5f;
    mWallUpDistance = 80.0f;
    mWallBackCushion = 0.08f;
    mCornerAngleMax = 120.0f;
}

dCamParam_c::dCamParam_c(s32 i_styleID) {
    u8* cam_data = (u8*)dComIfG_getObjectRes(dComIfGp_getCameraParamFileName(0), "camstyle.dat");

    mCamStyleData = (dCamStyleData::StyleData*)(cam_data + 8);
    mStyleNum = ((dCamStyleData*)cam_data)->mStyleNum;
    Change(i_styleID);

    mMapToolFovy = 0xFF;
    mMapToolArg0 = 0xFF;
    mMapToolArg1 = 0xFF;
    mMapToolArg2 = -1;
}

dCamParam_c::~dCamParam_c() {}

int dCamParam_c::Change(s32 i_styleID) {
    if (i_styleID >= 0 && i_styleID < mStyleNum) {
        mStyleID = i_styleID;
        mCurrentStyle = &mCamStyleData[mStyleID];
        return 1;
    }

    mStyleID = 0;
    mCurrentStyle = mCamStyleData;
    return 0;
}

int dCamParam_c::SearchStyle(u32 param_0) {
    int styleID = -1;

    for (int i = 0; i < mStyleNum; i++) {
        if (param_0 == mCamStyleData[i].field_0x0) {
            styleID = i;
            break;
        }
    }

    return styleID;
}

static f32 dummy_literal2() {
    return 0.25f;
}

dCamSetup_c::dCamSetup_c() {
    mDrawNear = 1.0f;
    mDrawFar = 100000.0f;
    field_0xc = 1;
    mModeSwitchType = -1;
    mForceType = -1;
    mCusCus = 0.2f;
    field_0x24 = 0.05f;
    field_0x60 = 80.0f;
    field_0x5c = -60.0f;
    field_0x28 = 0.33f;
    field_0x2c = 0.02f;
    field_0x50 = 100.0f;
    field_0x30 = 0.75f;
    mBaseCushion = 0.28f;
    mJumpCushion = 1.0f;
    field_0x58 = 0.15f;
    field_0x44 = 0.005f;
    field_0x54 = 0.06f;
    mCurveWeight = 1.0f;
    field_0x34 = 25.0f;
    mSubjLinkCullDist = 70.0f;
    mParallelDist = 60.0f;
    mTrimVistaHeight = 35.0f;
    mTrimCineScopeHeight = 65.0f;
    field_0xb0 = 150;
    field_0xb4 = 60.0f;
    mManualStartCThreshold = 0.3f;
    mManualEndVal = 0.2f;
    mFalseValue = 0.1f;
    mFalseAngle = 30.0f;
    mDebugFlags = 1;

    mFlags2 = 0x118;
    mFlags2 |= 0x200;
    mFlags2 |= 0x400;

    mChargeLatitude = 27.0f;
    mChargeTimer = 90;
    mChargeBRatio = 0.5f;
    mLockonChangeTimer = 40;
    mLockonChangeCushion = 0.25f;
    field_0x6c = 45.0f;
    mForceLockOffDist = 3500.0f;
    mForceLockOffTimer = 90;
    mThrowTimer = 30;
    mThrowVAngle = 25.0f;
    mThrowCushion = 0.05f;
    mThrowCtrOffset = 10.0f;
    field_0x78 = 0.5f;
    field_0x7c = 0.4f;
    field_0x80 = 10.0f;
    field_0x84 = 4.0f;
    field_0xd4 = 60.0f;
    field_0xd8 = 100.0f;
    mWindShakeCtr = 15.0f;
    mWindShakeFvy = 0.3f;
    mMapToolCamShortTimer = 60;
    mMapToolCamLongTimer = 120;
    mDebugFlags |= 0x4800;
    field_0x15c = 0.0f;
    field_0x160 = 0.0f;
    mDebugFlags |= 0x800;
    mWaitRollTimer = 120;
    mWaitRollSpeed = 0.02f;
    field_0x16c = 1200.0f;
    field_0xe0 = 0.06f;
    field_0xec = 25;
    field_0xf4 = 0.85f;
    field_0xf8 = 1.0f;
    field_0xf0 = 12;
    field_0xe4 = 50.0f;
    field_0xe8 = 0.0f;
    mScreensaverFirstWaitTimer = 30;
    mScreensaverWaitTimer = 600;
    mScreensaverExecTimer = 120;

    dCam_getBody()->EventRecoverNotime();
}

dCamSetup_c::~dCamSetup_c() {}

bool dCamSetup_c::CheckLatitudeRange(s16* param_0) {
    s16 temp0 = cAngle::d2s(field_0x60);
    s16 temp1 = cAngle::d2s(field_0x5c);

    if (*param_0 > temp0) {
        *param_0 = temp0;
        return false;
    }

    if (*param_0 < temp1) {
        *param_0 = temp1;
        return false;
    }

    return true;
}

f32 dCamSetup_c::PlayerHideDist() {
    if (daPy_py_c::checkNowWolf()) {
        return 120.0f;
    }

    return mSubjLinkCullDist;
}
