#ifndef D_A_DO_H
#define D_A_DO_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_msg_flow.h"
#include "f_op/f_op_actor.h"

enum daDo_Anm {
    ANM_BULBUL = 6,
    ANM_CARRY_B,
    ANM_CLIFF,
    ANM_CLIFF_WT,
    ANM_DEFEND,
    ANM_DOWN,
    ANM_DOWN_WT,
    ANM_JOYFUL,
    ANM_RUN,
    ANM_SIT,
    ANM_SIT_WAIT,
    ANM_STEP,
    ANM_STEP_2,
    ANM_SWIM,
    ANM_TO_CARRY_B,
    ANM_WAIT,
    ANM_WALK
};

enum daDo_Action {
    ACT_STAY,
    ACT_WALK,
    ACT_WALK_RUN,
    ACT_RUN,
    ACT_RUN_WALK,
    ACT_FOOD,
    ACT_WAIT_1,
    ACT_WAIT_2,
    ACT_SIT,
    ACT_A_SWIM = 10,
    ACT_SWIM = 20,
    ACT_HELP,
    ACT_BOAT,
    ACT_HANG = 30,
    ACT_CARRY = 35,
    ACT_MESSAGE = 39
};

// Remove this once the TU matches
struct daDo_HIO_tmp {
    /* 0x00 */ u8 field_0x00[4];
    /* 0x04 */ s8 field_0x04;
    /* 0x08 */ f32 mBaseSize;
    /* 0x0C */ f32 mWalkSpeed;
    /* 0x10 */ f32 mRunSpeed;
    /* 0x14 */ f32 mSwimSpeed;
    /* 0x18 */ f32 mPlayerRecognitionDist;
    /* 0x1C */ u8 mWaitType;
    /* 0x1D */ u8 mSwimming;
    /* 0x1E */ s16 mWaterHuntAnimType;
};

class daDo_HIO_c {
public:
    daDo_HIO_c();
    virtual ~daDo_HIO_c() {}

    /* 0x04 */ s8 field_0x04;
    /* 0x08 */ f32 mBaseSize;
    /* 0x0C */ f32 mWalkSpeed;
    /* 0x10 */ f32 mRunSpeed;
    /* 0x14 */ f32 mSwimSpeed;
    /* 0x18 */ f32 mPlayerRecognitionDist;
    /* 0x1C */ u8 mWaitType;
    /* 0x1D */ u8 mSwimming;
    /* 0x1E */ s16 mWaterHuntAnimType;
};

/**
 * @ingroup actors-unsorted
 * @class do_class
 * @brief Dog
 *
 * @details
 *
 */
class do_class : public fopEn_enemy_c {
public:
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ u8 mParam0;
    /* 0x5B5 */ u8 mParam1;
    /* 0x5B6 */ u8 mParam2;
    /* 0x5B8 */ cXyz mTargetPos;
    /* 0x5C4 */ u8 field_0x5c4[4];
    /* 0x5C8 */ f32 mDistFromPlayer;
    /* 0x5CC */ s16 mTargetAngleY;
    /* 0x5D8 */ mDoExt_McaMorf* mpMorf;
    /* 0x5D4 */ mDoExt_btkAnm* mpBtk;
    /* 0x5D8 */ mDoExt_btpAnm* mpBtp;
    /* 0x5DC */ s8 mBtkFrame;
    /* 0x5DE */ s16 mBtpFrame;
    /* 0x5E0 */ s16 mBtpTimer;
    /* 0x5E2 */ s8 mBtpReset;
    /* 0x5E4 */ int mAnmID;
    /* 0x5E8 */ f32 mAnmSpeed;
    /* 0x5EC */ f32 mTargetAnmSpeed;
    /* 0x5F0 */ s16 mCounter;
    /* 0x5F2 */ s16 mAction;
    /* 0x5F4 */ s16 field_0x5f4;
    /* 0x5F6 */ s16 mMode;
    /* 0x5F8 */ u32 mShadowKey;
    /* 0x5FC */ s16 mTimer[4];
    /* 0x604 */ s16 mCcDisableTimer;
    /* 0x606 */ s16 mSwimEffectTimer;
    /* 0x608 */ f32 field_0x608;
    /* 0x60C */ s16 field_0x60c;
    /* 0x60E */ csXyz mHeadAngle;
    /* 0x614 */ s16 mHeadTilt;
    /* 0x616 */ s8 field_0x616;
    /* 0x618 */ cXyz mLookPos;
    /* 0x624 */ s16 mLookTimer;
    /* 0x626 */ csXyz mHeadBob;
    /* 0x62C */ csXyz mHeadBobTarget;
    /* 0x632 */ s16 mHeadBobTimer;
    /* 0x634 */ f32 mShakeSpeed;
    /* 0x638 */ f32 mShakeTargetSpeed;
    /* 0x63C */ s16 mASwimTimer;
    /* 0x63E */ csXyz mTailAngle;
    /* 0x644 */ f32 mTailWag;
    /* 0x648 */ f32 mTailWagTarget;
    /* 0x64C */ f32 mTailPhaseY;
    /* 0x650 */ f32 mTailPhaseX;
    /* 0x654 */ s16 mRandomAngleY;
    /* 0x656 */ s16 mRandomTargetAngleY;
    /* 0x658 */ f32 field_0x658;
    /* 0x65C */ f32 mWaterY;
    /* 0x660 */ s16 mHangAngleY;
    /* 0x664 */ cXyz mHangPos;
    /* 0x670 */ f32 mHangSpeed;
    /* 0x674 */ cXyz mScale;
    /* 0x680 */ u32 mItemActorID;
    /* 0x684 */ u32 mFoodActorID;
    /* 0x688 */ s8 mFoodInMouth;
    /* 0x689 */ s8 mNoDraw;
    /* 0x68C */ cXyz field_0x68c;
    /* 0x698 */ cXyz field_0x698;
    /* 0x6A4 */ u8 field_0x6a4[10];
    /* 0x6AE */ s8 mRunningSoundTimer;
    /* 0x6B0 */ s16 field_0x6b0;
    /* 0x6B4 */ f32 field_0x6b4;
    /* 0x6B8 */ u8 mItemControlTimer;
    /* 0x6BC */ dBgS_AcchCir mBgS_AcchCir;
    /* 0x6FC */ dBgS_ObjAcch mBgS_Acch;
    /* 0x8D4 */ dCcD_Stts mStts;
    /* 0x910 */ dCcD_Sph mCcSph;
    /* 0xA48 */ dCcD_Sph field_0xa48;
    /* 0xB80 */ cXyz field_0xb80;
    /* 0xB8C */ cXyz field_0xb8c;
    /* 0xB98 */ cXyz field_0xb98;
    /* 0xBA4 */ cXyz field_0xba4;
    /* 0xBB0 */ cXyz field_0xbb0;
    /* 0xBBC */ cXyz field_0xbbc;
    /* 0xBC8 */ u8 field_0xb4c[4];
    /* 0xBCC */ u32 mRippleKey;
    /* 0xBD0 */ u8 field_0xbd0[4];
    /* 0xBD4 */ u32 mParticleKey[4];
    /* 0xBE4 */ Z2SoundObjSimple mSound;
    /* 0xC04 */ u8 mFoodEndTimer;
    /* 0xC05 */ s8 mMessageState;
    /* 0xC06 */ s16 mIsTalking;
    /* 0xC08 */ s16 mFlowID;
    /* 0xC0C */ dMsgFlow_c mMsg;
    /* 0xC58 */ u8 field_0xc58[40];
    /* 0xC80 */ bool field_0xc80;
    /* 0xC84 */ cXyz field_0xc84;
    /* 0xC90 */ cXyz field_0xc90;
    /* 0xC9C */ cXyz field_0xc9c;
    /* 0xCA8 */ cXyz field_0xca8;
    /* 0xCB4 */ cXyz field_0xcb4;
    /* 0xCC0 */ bool mHIOInit;
};

#endif /* D_A_DO_H */
