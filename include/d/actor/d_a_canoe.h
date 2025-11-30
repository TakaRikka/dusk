#ifndef D_A_CANOE_H
#define D_A_CANOE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"
#include "d/d_bg_s_acch.h"
#include "SSystem/SComponent/c_phase.h"

/**
 * @ingroup actors-unsorted
 * @class daCanoe_c
 * @brief Canoe
 *
 * @details
 *
 */
class daCanoe_c : public fopAc_ac_c {
public:
    int createHeap();
    int create();
    ~daCanoe_c();
    void setRoomInfo();
    void setMatrix();
    void setCollision();
    void posMove();
    void checkGomikabe(cBgS_PolyInfo&);
    void setFrontBackPos();
    void frontBackBgCheck();
    void setPaddleEffect();
    void setCanoeSliderEffect();
    int execute();
    int draw();

    J3DModel* getModel() { return mpModel; }
    MtxP getModelMtx() { return mpModel->getBaseTRMtx(); }
    bool checkTandem() { return fopAcM_GetParam(this) == 2; }
    bool checkJumpRideFlg() { return mJumpRideFlg; }
    void incShapeOffsetY(f32 offset) { mShapeOffsetY += offset; }
    void incShapeAngleZ(s16 incZ) { shape_angle.z += incZ; }
    void onRodID(fpc_ProcID i_rodID) { mRodID = i_rodID; }
    void setWaterFallDownSe() { mSound.startSound(Z2SE_AL_CANOE_FALL_DW, 0, -1); }
    bool checkPaddleChangeWater() { return mPaddleWaterType == 3; }
    void startInitPaddleWater() { mPaddleWaterType = 4; }
    fpc_ProcID getRodID() { return mRodID; }

    void setAimSpeed(f32 i_speed) { mAimSpeed = i_speed; }
    void setAimAngle(s16 i_angle) {
        mAimAngle = i_angle;
        mIsForceAimAngle = TRUE;
    }

    void setPosAndAngle(cXyz const* i_pos, s16 i_angle) {
        mForcePos = *i_pos;
        mForceAngle = i_angle;
        mIsForcePosAngle = TRUE;
    }

    void setPaddleOffsetRight() {
        field_0x1454 = 64;
        field_0x1456 = 15;
    }

    void setPaddleOffsetLeft() {
        field_0x1454 = -64;
        field_0x1456 = 15;
    }

private:
    /* 0x0568 */ request_of_phase_process_class mPhaseReq;
    /* 0x0570 */ J3DModel* mpModel;
    /* 0x0574 */ J3DModel* mpPaddleModel;
    /* 0x0578 */ J3DModel* mpMaskModel;
    /* 0x057C */ dCcD_Stts mCcStts;
    /* 0x05B8 */ dBgS_AcchCir mAcchCir[3][3];
    /* 0x07F8 */ dBgS_LinkAcch mAcch[3];
    /* 0x0D80 */ dCcD_Cyl mCcCyl[5];
    /* 0x13AC */ dBgS_LinkLinChk mLineChk;
    /* 0x141C */ Z2SoundObjSimple mSound;
    /* 0x143C */ const char* mArcName;
    /* 0x1440 */ u8 mPaddleWaterType;
    /* 0x1441 */ u8 field_0x1441;
    /* 0x1442 */ u8 mIsForceAimAngle;
    /* 0x1443 */ u8 mJumpRideFlg;
    /* 0x1444 */ u8 field_0x1444;
    /* 0x1445 */ s8 mReverb;
    /* 0x1446 */ u8 mIsForcePosAngle;
    /* 0x1447 */ u8 mIsSliderHit;
    /* 0x1448 */ s16 field_0x1448;
    /* 0x144A */ s16 mForceAngle;
    /* 0x144C */ s16 mAimAngle;
    /* 0x144E */ s16 field_0x144e;
    /* 0x1450 */ s16 field_0x1450;
    /* 0x1452 */ s16 field_0x1452;
    /* 0x1454 */ s16 field_0x1454;
    /* 0x1456 */ s16 field_0x1456;
    /* 0x1458 */ s16 field_0x1458;
    /* 0x145A */ s16 field_0x145a;
    /* 0x145C */ s16 mHitInvulnerabilityTimer;
    /* 0x145E */ u16 mHitCount;
    /* 0x1460 */ s16 field_0x1460;
    /* 0x1464 */ fpc_ProcID mRodID;
    /* 0x1468 */ f32 mAimSpeed;
    /* 0x146C */ f32 mShapeOffsetY;
    /* 0x1470 */ f32 field_0x1470;
    /* 0x1474 */ u32 mShadowId;
    /* 0x1478 */ u32 field_0x1478;
    /* 0x147C */ u8 field_0x147c[0x1480 - 0x147C];
    /* 0x1480 */ u32 field_0x1480;
    /* 0x1484 */ u8 field_0x1484[0x1488 - 0x1484];
    /* 0x1488 */ u32 field_0x1488[2];
    /* 0x1490 */ u32 field_0x1490;
    /* 0x1494 */ cXyz mFrontPos;
    /* 0x14A0 */ cXyz mBackPos;
    /* 0x14AC */ cXyz mOldFrontPos;
    /* 0x14B8 */ cXyz mOldBackPos;
    /* 0x14C4 */ cXyz mWaterSpeed;
    /* 0x14D0 */ cXyz mForcePos;
};

STATIC_ASSERT(sizeof(daCanoe_c) == 0x14dc);

#endif /* D_A_CANOE_H */
