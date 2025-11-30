#ifndef D_A_E_RDY_H
#define D_A_E_RDY_H

#include "JSystem/JHostIO/JORReflexible.h"
#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_jnt_col.h"
#include "d/d_msg_flow.h"
#include "d/d_path.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-enemies
 * @class e_rdy_class
 * @brief Shadow Bulblin
 * 
 * @details 
 * 
 */
class e_rdy_class {
public:
    e_rdy_class();

    /* 0x0000 */ fopEn_enemy_c actor;
    /* 0x05AC */ request_of_phase_process_class mPhase;
    /* 0x05B4 */ s16 mMode;
    /* 0x05B6 */ u8 field_0x5b6;
    /* 0x05B7 */ u8 field_0x5b7;
    /* 0x05B8 */ u8 field_0x5b8;
    /* 0x05B9 */ u8 mSwBit;
    /* 0x05BA */ u8 mWeaponType;
    /* 0x05BB */ s8 field_0x5bb;
    /* 0x05BC */ cXyz field_0x5bc;
    /* 0x05C8 */ s16 mTargetAngleY;
    /* 0x05CC */ mDoExt_McaMorfSO* mpMorf;
    /* 0x05D0 */ Z2CreatureEnemy mSound;
    /* 0x0674 */ char* mpArcName;
    /* 0x0678 */ int mAnm;
    /* 0x067C */ u8 field_0x67c[0x680 - 0x67c];
    /* 0x0680 */ dJntCol_c mJntCol;
    /* 0x0690 */ J3DModel* mpWeaponModel;
    /* 0x0694 */ mDoExt_McaMorfSO* mpBowMorf;
    /* 0x0698 */ J3DModel* mpHawkGrassModel;
    /* 0x069C */ s8 mIsHoldingGrass;
    /* 0x06A0 */ dKy_tevstr_c mHawkGrassTevStr;
    /* 0x0A28 */ J3DModel* mpEyeModel[2];
    /* 0x0A30 */ f32 mEyeScale;
    /* 0x0A34 */ f32 mTargetEyeScale;
    /* 0x0A38 */ s8 mIsDying;
    /* 0x0A3C */ f32 mDyingTevColor;
    /* 0x0A40 */ s16 mCounter;
    /* 0x0A42 */ s16 mAction;
    /* 0x0A44 */ s16 mPrevAction;
    /* 0x0A48 */ f32 mPlayerDist;
    /* 0x0A4C */ s16 mPlayerAngle;
    /* 0x0A50 */ f32 mPlayerCheckDist;
    /* 0x0A54 */ f32 mWaterHeight;
    /* 0x0A58 */ u32 mShadowKey;
    /* 0x0A5C */ fpc_ProcID mKargarokID;
    /* 0x0A60 */ s16 mTimer[4];
    /* 0x0A68 */ s16 mIFrameTimer;
    /* 0x0A6A */ u8 field_0xa6a[0xa6c - 0xa6a];
    /* 0x0A6C */ s16 field_0xa6c;
    /* 0x0A6E */ s16 field_0xa6e;
    /* 0x0A70 */ u8 field_0xa70;
    /* 0x0A71 */ s8 field_0xa71;
    /* 0x0A72 */ s8 mHasArrow;
    /* 0x0A73 */ s8 mArrowFired;
    /* 0x0A74 */ s8 field_0xa74;
    /* 0x0A76 */ s16 mBowRotationTimer;
    /* 0x0A78 */ s16 mBowRotation;
    /* 0x0A7A */ u8 field_0xa7a[0xa7b - 0xa7a];
    /* 0x0A7B */ s8 field_0xa7b;
    /* 0x0A7C */ s8 field_0xa7c;
    /* 0x0A7D */ s8 mCollisionEnabled;
    /* 0x0A7E */ u8 mFadeTimer;
    /* 0x0A80 */ cXyz mKargarokPos;
    /* 0x0A8C */ s8 mRideState;
    /* 0x0A8D */ u8 field_0xa8d[0xa8e - 0xa8d];
    /* 0x0A8E */ s8 field_0xa8e;
    /* 0x0A8F */ s8 field_0xa8f;
    /* 0x0A90 */ f32 mJumpHeight;
    /* 0x0A94 */ f32 mJumpSpeed;
    /* 0x0A98 */ s8 field_0xa98;
    /* 0x0A9A */ s16 field_0xa9a;
    /* 0x0A9C */ s16 field_0xa9c;
    /* 0x0A9E */ s16 field_0xa9e;
    /* 0x0AA0 */ s16 field_0xaa0;
    /* 0x0AA4 */ cXyz mArrowTarget;
    /* 0x0AB0 */ cXyz field_0xab0;
    /* 0x0ABC */ f32 field_0xabc;
    /* 0x0AC0 */ f32 mKnockback;
    /* 0x0AC4 */ s16 mHitDirection;
    /* 0x0AC6 */ s16 field_0xac6;
    /* 0x0AC8 */ s8 field_0xac8;
    /* 0x0ACC */ cXyz field_0xacc;
    /* 0x0AD8 */ u8 field_0xad8[0xadc - 0xad8];
    /* 0x0ADC */ csXyz field_0xadc;
    /* 0x0AE2 */ csXyz field_0xae2;
    /* 0x0AE8 */ csXyz field_0xae8;
    /* 0x0AEE */ s8 field_0xaee;
    /* 0x0AEF */ s8 mIsUpsideDown;
    /* 0x0AF0 */ u32 field_0xaf0;
    /* 0x0AF4 */ f32 field_0xaf4;
    /* 0x0AF8 */ s16 field_0xaf8;
    /* 0x0AFC */ f32 field_0xafc;
    /* 0x0B00 */ s16 field_0xb00;
    /* 0x0B02 */ csXyz field_0xb02[11];
    /* 0x0B44 */ csXyz field_0xb44[11];
    /* 0x0B88 */ f32 field_0xb88;
    /* 0x0B8C */ s16 field_0xb8c[11];
    /* 0x0BA2 */ s16 field_0xba2[4];
    /* 0x0BAA */ s16 field_0xbaa;
    /* 0x0BAC */ s16 field_0xbac;
    /* 0x0BAE */ csXyz field_0xbae;
    /* 0x0BB4 */ csXyz field_0xbb4;
    /* 0x0BBC */ f32 field_0xbbc;
    /* 0x0BC0 */ s16 field_0xbc0;
    /* 0x0BC2 */ u8 field_0xbc2[0xbc8 - 0xbc2];
    /* 0x0BC8 */ s16 field_0xbc8;
    /* 0x0BCA */ s8 field_0xbca;
    /* 0x0BCB */ s8 mKargarokDeleteTimer;
    /* 0x0BCC */ dPath* mpPath;
    /* 0x0BD0 */ s8 mPathIndex;
    /* 0x0BD1 */ s8 mPathDirection;
    /* 0x0BD4 */ dBgS_AcchCir mAcchCir;
    /* 0x0C14 */ dBgS_ObjAcch mAcch;
    /* 0x0DEC */ dCcD_Stts mCcStts;
    /* 0x0E28 */ dCcD_Sph mCcSph[3];
    /* 0x11D0 */ dCcD_Sph mAtSph;
    /* 0x1308 */ dCcU_AtInfo mAtInfo;
    /* 0x132C */ bool mHIOInit;
    /* 0x132D */ s8 mGroundHit;
    /* 0x1330 */ u32 mSmokeKey1;
    /* 0x1334 */ u32 mSmokeKey2;
    /* 0x1338 */ u32 mWaterParticleKey[3];
    /* 0x1344 */ u32 mFireParticleKey[2];
    /* 0x134C */ cXyz field_0x134c;
    /* 0x1358 */ cXyz field_0x1358;
    /* 0x1364 */ s8 mInWater;
    /* 0x1365 */ s8 field_0x1365;
    /* 0x1366 */ s8 field_0x1366;
    /* 0x1367 */ s8 mDrawEyeModel;
    /* 0x1368 */ u8 field_0x1368;
    /* 0x1369 */ s8 field_0x1369;
    /* 0x136A */ u8 field_0x136a[0x136b - 0x136a];
    /* 0x136B */ s8 mParticleTimer;
    /* 0x136C */ int mSwBit2;
    /* 0x1370 */ s16 mDemoMode;
    /* 0x1372 */ s16 mDemoTimer;
    /* 0x1374 */ s16 mCamBankPhase;
    /* 0x1378 */ cXyz mCamEye;
    /* 0x1384 */ cXyz mCamCenter;
    /* 0x1390 */ cXyz mCamEyeTarget;
    /* 0x139C */ cXyz mCamCenterTarget;
    /* 0x13A8 */ cXyz mCamEyeSpeed;
    /* 0x13B4 */ cXyz mCamCenterSpeed;
    /* 0x13C0 */ f32 mCamDist;
    /* 0x13C4 */ u8 field_0x13c4[0x13c8 - 0x13c4];
    /* 0x13C8 */ f32 mCamFovy;
    /* 0x13CC */ f32 mCamBank;
    /* 0x13D0 */ f32 mCamSpeed;
    /* 0x13D4 */ s16 field_0x13d4;
    /* 0x13D6 */ u8 field_0x13d6[0x13e0 - 0x13d6];
    /* 0x13E0 */ dMsgFlow_c mMsgFlow;
};

STATIC_ASSERT(sizeof(e_rdy_class) == 0x142C);

#endif /* D_A_E_RDY_H */
