/**
 * d_bg_s_poly_pass_chk.cpp
 * BG Collision Poly Passthrough Check
*/

#include "d/d_bg_s_poly_pass_chk.h"

dBgS_PolyPassChk::dBgS_PolyPassChk() {
    mObject = false;
    mCamera = false;
    mLink = false;
    mArrow = false;
    mBomb = false;
    mBoomerang = false;
    mRope = false;
    mUnderwaterRoof = false;
    mHorse = false;
    mStatue = false;
    mIronBall = false;
}

dBgS_PolyPassChk::~dBgS_PolyPassChk() {}

void dBgS_PolyPassChk::SetPassChkInfo(dBgS_PolyPassChk& chk) {
    *this = chk;
}

void dBgS_PolyPassChk::SetObj() {
    mObject = true;
}

void dBgS_PolyPassChk::ClrObj() {
    mObject = false;
}

void dBgS_PolyPassChk::SetCam() {
    mCamera = true;
}

void dBgS_PolyPassChk::ClrCam() {
    mCamera = false;
}

void dBgS_PolyPassChk::SetLink() {
    mLink = true;
}

void dBgS_PolyPassChk::ClrLink() {
    mLink = false;
}

void dBgS_PolyPassChk::SetArrow() {
    mArrow = true;
}

void dBgS_PolyPassChk::SetBomb() {
    mBomb = true;
}

void dBgS_PolyPassChk::ClrBomb() {
    mBomb = false;
}

void dBgS_PolyPassChk::SetBoomerang() {
    mBoomerang = true;
}

void dBgS_PolyPassChk::SetRope() {
    mRope = true;
}

void dBgS_PolyPassChk::SetUnderwaterRoof() {
    mUnderwaterRoof = true;
}

void dBgS_PolyPassChk::SetHorse() {
    mHorse = true;
}

bool dBgS_PolyPassChk::ChkNoHorse() {
    if (ChkObj() || ChkCam() || ChkLink() || ChkArrow() || ChkBomb() || ChkBoomerang() ||
        ChkRope() || ChkUnderwaterRoof())
    {
        return true;
    }

    if (!ChkHorse())
        return true;

    return false;
}

void dBgS_PolyPassChk::SetStatue() {
    mStatue = true;
}

void dBgS_PolyPassChk::ClrStatue() {
    mStatue = false;
}

void dBgS_PolyPassChk::SetIronBall() {
    mIronBall = true;
}

void dBgS_PolyPassChk::ClrIronBall() {
    mIronBall = false;
}
