#ifndef C_BG_S_LIN_CHK_H
#define C_BG_S_LIN_CHK_H

#include "SSystem/SComponent/c_bg_s_chk.h"
#include "SSystem/SComponent/c_bg_s_poly_info.h"
#include "SSystem/SComponent/c_m3d_g_lin.h"

class cBgS_LinChk : public cBgS_Chk, public cBgS_PolyInfo {
public:
    /* 0x024 */ cM3dGLin mLin;
    /* 0x040 */ cXyz field_0x40;
    /* 0x04C */ u32 mStts;
    /* 0x050 */ bool mPreWallChk;
    /* 0x051 */ bool mPreGroundChk;
    /* 0x052 */ bool mPreRoofChk;
    /* 0x053 */ bool mFrontFlag;
    /* 0x054 */ bool mBackFlag;

    enum Status {
        STTS_HIT = 0x10,
        STTS_ROOF_OFF = 0x20000000,
        STTS_WALL_OFF = 0x40000000,
        STTS_GROUND_OFF = 0x80000000,
    };

public:
    cBgS_LinChk();
    void ct();
    void Set2(const cXyz*, const cXyz*, unsigned int);
    void PreCalc();
    cXyz& GetCross() { return mLin.GetEnd(); }

    virtual ~cBgS_LinChk();

    void ClrHit() { mStts &= ~STTS_HIT; }
    void SetHit() { mStts |= STTS_HIT; }
    u32 ChkHit() const { return mStts & STTS_HIT; }
    void SetCross(const cXyz& pos) { mLin.SetEnd(pos); }
    cXyz* GetCrossP() { return mLin.GetEndP(); }
    cM3dGLin* GetLinP() { return &mLin; }
    bool ChkBackFlag() const { return mBackFlag; }
    void OnBackFlag() { mBackFlag = true; }
    bool ChkFrontFlag() const { return mFrontFlag; }
    void OnFrontFlag() { mFrontFlag = true; }
    bool GetPreWallChk() const { return mPreWallChk; }
    bool GetPreGroundChk() const { return mPreGroundChk; }
    bool GetPreRoofChk() const { return mPreRoofChk; }
    cXyz* GetStartP() { return mLin.GetStartP(); }
    void ClrSttsRoofOff() { mStts &= ~STTS_ROOF_OFF; }
    void SetSttsRoofOff() { mStts |= STTS_ROOF_OFF; }
    void ClrSttsWallOff() { mStts &= ~STTS_WALL_OFF; }
    void SetSttsWallOff() { mStts |= STTS_WALL_OFF; }
    void ClrSttsGroundOff() { mStts &= ~STTS_GROUND_OFF; }
    void SetSttsGroundOff() { mStts |= STTS_GROUND_OFF; }
    void ClearCheckIndex() { ClearPi(); }
    bool ChkSttsWallOff() const { return (mStts & STTS_WALL_OFF); }
    bool ChkSttsGroundOff() const { return (mStts & STTS_GROUND_OFF); }
    bool ChkSttsRoofOff() const { return (mStts & STTS_ROOF_OFF); }
};

#endif /* C_BG_S_LIN_CHK_H */
