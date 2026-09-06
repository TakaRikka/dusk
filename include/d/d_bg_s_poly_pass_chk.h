#ifndef D_BG_D_BG_S_POLY_PASS_CHK_H
#define D_BG_D_BG_S_POLY_PASS_CHK_H

#include "SSystem/SComponent/c_bg_s_poly_pass_chk.h"


class dBgS_PolyPassChk : public cBgS_PolyPassChk {
public:
    dBgS_PolyPassChk();
    virtual ~dBgS_PolyPassChk();

    bool ChkArrow() { return mArrow; }
    bool ChkBomb() { return mBomb; }
    bool ChkBoomerang() { return mBoomerang; }
    bool ChkCam() { return mCamera; }
    bool ChkHorse() { return mHorse; }
    bool ChkIronBall() { return mIronBall; }
    bool ChkLink() { return mLink; }
    bool ChkNoHorse();
    bool ChkObj() { return mObject; }
    bool ChkRope() { return mRope; }
    bool ChkStatue() { return mStatue; }
    bool ChkUnderwaterRoof() { return mUnderwaterRoof; }

    void ClrBomb();
    void ClrCam();
    void ClrIronBall();
    void ClrLink();
    void ClrObj();
    void ClrStatue();

    void SetArrow();
    void SetBomb();
    void SetBoomerang();
    void SetCam();
    void SetHorse();
    void SetIronBall();
    void SetLink();
    void SetObj();
    void SetPassChkInfo(dBgS_PolyPassChk& chk);
    void SetRope();
    void SetStatue();
    void SetUnderwaterRoof();

    /* 0x4 */ bool mObject;
    /* 0x5 */ bool mCamera;
    /* 0x6 */ bool mLink;
    /* 0x7 */ bool mArrow;
    /* 0x8 */ bool mBomb;
    /* 0x9 */ bool mBoomerang;
    /* 0xA */ bool mRope;
    /* 0xB */ bool mUnderwaterRoof;
    /* 0xC */ bool mHorse;
    /* 0xD */ bool mStatue;
    /* 0xE */ bool mIronBall;
};

#endif /* D_BG_D_BG_S_POLY_PASS_CHK_H */
