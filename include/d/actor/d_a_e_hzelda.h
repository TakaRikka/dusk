#ifndef D_A_E_HZELDA_H
#define D_A_E_HZELDA_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_bg_s_acch.h"

/**
 * @ingroup actors-enemies
 * @class e_hzelda_class
 * @brief Puppet Zelda
 * 
 * @details 
 * 
 */
class e_hzelda_class : public fopEn_enemy_c {
public:
    /* 0x05AC */ request_of_phase_process_class mPhase;
    /* 0x05B4 */ u8 mPrm0;
    /* 0x05B8 */ cXyz mTargetMovePos;
    /* 0x05C4 */ cXyz field_0x5c4;
    /* 0x05D0 */ s16 field_0x5d0;
    /* 0x05D4 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x05D8 */ Z2CreatureEnemy mSound;
    /* 0x067C */ u8 field_0x67C[0x0684 - 0x067C];
    /* 0x0684 */ int mAnm;
    /* 0x0688 */ J3DModel* mpSwordModel;
    /* 0x068C */ int field_0x68c;
    /* 0x0690 */ f32 mSwordColorIntensity;
    /* 0x0694 */ s16 mCounter;
    /* 0x0696 */ s16 mAction;
    /* 0x0698 */ s16 mMode;
    /* 0x069C */ f32 mMoveStep;
    /* 0x06A0 */ f32 mDistToPlayer;
    /* 0x06A4 */ s16 mAngleToPlayer;
    /* 0x06A8 */ u32 mShadowKey;
    /* 0x06AC */ s16 mTimers[4];
    /* 0x06B4 */ s16 field_0x6b4;
    /* 0x06B6 */ u8 field_0x6B6[0x06B8 - 0x06B6];
    /* 0x06B8 */ s8 field_0x6b8;
    /* 0x06B9 */ u8 field_0x6b9;
    /* 0x06BA */ u8 field_0x6ba;
    /* 0x06BB */ s8 mIsLookatPlayer;
    /* 0x06BC */ s16 mHeadRotX;
    /* 0x06BE */ s16 mHeadRotZ;
    /* 0x06C0 */ f32 mArmRRotY;
    /* 0x06C4 */ f32 field_0x6c4;
    /* 0x06C8 */ f32 mArmLRotY;
    /* 0x06CC */ f32 field_0x6cc;
    /* 0x06D0 */ f32 mBodyRotY;
    /* 0x06D4 */ f32 mBodyRotZ;
    /* 0x06D8 */ f32 mDodgeMove;
    /* 0x06DC */ s16 field_0x6dc;
    /* 0x06DE */ u8 field_0x6DE[0x06E4 - 0x06DE];
    /* 0x06E4 */ s8 field_0x6e4;
    /* 0x06E8 */ f32 field_0x6e8;
    /* 0x06EC */ cXyz mOldBallPos;
    /* 0x06F8 */ cXyz mBallPos;
    /* 0x0704 */ cXyz mBallMove;
    /* 0x0710 */ f32 mBallMoveStep;
    /* 0x0714 */ J3DModel* mpTriangleAtModel;
    /* 0x0718 */ s16 field_0x718;
    /* 0x071A */ s16 mDrawTriangleAt;
    /* 0x071C */ s16 mTriangleRotY;
    /* 0x0720 */ int mTriangleAnmFrame;
    /* 0x0724 */ f32 mTriangleSize;
    /* 0x0728 */ cXyz mTrianglePos;
    /* 0x0734 */ mDoExt_brkAnm* mpTriangleAtBrk;
    /* 0x0738 */ mDoExt_btkAnm* mpTriangleAtBtk;
    /* 0x073C */ u8 field_0x73C[0x0740 - 0x073C];
    /* 0x0740 */ dBgS_AcchCir mAcchCir;
    /* 0x0780 */ dBgS_ObjAcch mAcch;
    /* 0x0958 */ dCcD_Stts field_0x958;
    /* 0x0994 */ dCcD_Sph mSphCc[3];
    /* 0x0D3C */ dCcD_Stts field_0xd3c;
    /* 0x0D78 */ dCcD_Sph mSphAt;
    /* 0x0EB0 */ dCcU_AtInfo mAtInfo;
    /* 0x0ED4 */ dCcD_Sph mBallTgSph;
    /* 0x100C */ dCcD_Sph mBallSphAt;
    /* 0x1144 */ dCcD_Sph mTriAtSph;
    /* 0x127C */ dCcD_Stts field_0x127c;
    /* 0x12B8 */ dCcD_Sph mPlAtSph;
    /* 0x13F0 */ Z2SoundObjSimple mBallSound;
    /* 0x1410 */ u8 mMissedBallCount;
    /* 0x1411 */ u8 mBottleSwingAtTimer;
    /* 0x1412 */ u8 mSwordAtTimer;
    /* 0x1413 */ u8 mNoBallCount;
    /* 0x1414 */ u8 mMsgSetTimer;
    /* 0x1415 */ u8 mInitHIO;
    /* 0x1418 */ u32 field_0x1418;
    /* 0x141C */ u32 field_0x141c[3];
    /* 0x1428 */ u32 field_0x1428[6];
    /* 0x1440 */ s16 mDemoMode;
    /* 0x1442 */ s16 mDemoTimer;
    /* 0x1444 */ cXyz mDemoCamEye;
    /* 0x1450 */ cXyz mDemoCamCenter;
    /* 0x145C */ f32 mDemoCamFovy;
};

STATIC_ASSERT(sizeof(e_hzelda_class) == 0x1460);

class daE_HZELDA_HIO_c {
public:
    daE_HZELDA_HIO_c();
    virtual ~daE_HZELDA_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 mModelSize;
};


#endif /* D_A_E_HZELDA_H */
