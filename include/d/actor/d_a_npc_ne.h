#ifndef D_A_NPC_NE_H
#define D_A_NPC_NE_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_path.h"
#include "d/d_msg_flow.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-npcs
 * @class npc_ne_class
 * @brief Cat (Neko)
 *
 * @details
 *
 */
class npc_ne_class : public fopEn_enemy_c {
public:
    enum Action {
        ACT_WAIT = 0,
        ACT_AWAY = 1,
        ACT_TAME = 2,
        ACT_ROOF = 3,
        ACT_HOME = 4,
        ACT_JUMP = 5,
        ACT_S_JUMP = 6,
        ACT_BIRD = 7,
        ACT_BALL = 8,
        ACT_SANBASI = 9,
        ACT_PATH_WALK = 10,
        ACT_SWIM = 12,
        ACT_OUT_SWIM = 13,
        ACT_CLIMB = 14,
        ACT_DROP = 15,
        ACT_S_DROP = 16,
        ACT_MESSAGE = 17,
        ACT_DISH = 18,
        ACT_CARRY = 20,
    };

    enum Animation {
        ANM_CARRY_A = 6,
        ANM_CARRY_B = 7,
        ANM_DRINK_DISH = 8,
        ANM_EMPTY_DISH = 9,
        ANM_FAWN_L = 10,
        ANM_FAWN_R = 11,
        ANM_JUMP_END = 12,
        ANM_JUMP_LOOP = 13,
        ANM_JUMP_START = 14,
        ANM_JUMP_UP = 15,
        ANM_MILK = 16,
        ANM_RUN = 17,
        ANM_SIT_A = 18,
        ANM_SIT_B = 19,
        ANM_SWIM = 20,
        ANM_TO_JUMP = 21,
        ANM_TO_WAIT = 22,
        ANM_TO_CARRY_B = 23,
        ANM_WAIT = 24,
        ANM_WALK = 25,
    };

    enum Behavior {
        BHV_NONE = 0,
        BHV_TAME = 1,
        BHV_PIER = 2,
        BHV_DISH = 3,
    };

    enum LookMode {
        LOOK_FORWARD = 0,
        LOOK_PLAYER = 1,
        LOOK_TARGET = 2,
        LOOK_NONE = 3,
    };

    npc_ne_class();

    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ u8 mBehavior;
    /* 0x5B5 */ u8 mTexture;
    /* 0x5B6 */ u8 mSize;
    /* 0x5B7 */ u8 mIsGengle;
    /* 0x5B8 */ u8 mPathIndex;
    /* 0x5B9 */ u8 field_0x5b9;
    /* 0x5BC */ dPath* mpPath;
    /* 0x5C0 */ s8 mPathPointNo;
    /* 0x5C1 */ s8 mPathDir;
    /* 0x5C4 */ cXyz mTargetPos;
    /* 0x5D0 */ s16 mTargetAngleY;
    /* 0x5D4 */ f32 mAngleYStep;
    /* 0x5D8 */ f32 mDistToTarget;
    /* 0x5DC */ s16 mAngleToPlayer;
    /* 0x5E0 */ f32 mDistScale;
    /* 0x5E4 */ char* mResName;
    /* 0x5E8 */ mDoExt_McaMorf* mpMorf;
    /* 0x5EC */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x5F0 */ mDoExt_btpAnm* mpBtpAnm;
    /* 0x5F4 */ s8 mBtkFrame;
    /* 0x5F6 */ s16 mBtpFrame;
    /* 0x5F8 */ s8 mBtpReset;
    /* 0x5FC */ mDoExt_McaMorf* mpDishMorf;
    /* 0x600 */ cXyz mDishPos;
    /* 0x60C */ Z2SoundObjSimple mSound;
    /* 0x62C */ s16 mBtpTimer;
    /* 0x630 */ int mAnmID;
    /* 0x634 */ u8 field_0x634[4];
    /* 0x638 */ f32 mAnmSpeed;
    /* 0x63C */ u8 field_0x63c[4];
    /* 0x640 */ s16 mCounter;
    /* 0x642 */ s16 mAction;
    /* 0x644 */ u8 field_0x644[2];
    /* 0x646 */ s16 mMode;
    /* 0x648 */ u32 mShadowKey;
    /* 0x64C */ s16 mTimers[5];
    /* 0x656 */ s16 mCcDisableTimer;
    /* 0x658 */ s16 mSplashTimer;
    /* 0x65C */ f32 field_0x65c;
    /* 0x660 */ s16 field_0x660;
    /* 0x662 */ s16 mHeadAngleZ;
    /* 0x664 */ s16 mHeadAngleY;
    /* 0x666 */ s16 mHeadAngleX;
    /* 0x668 */ s8 mLookMode;
    /* 0x66C */ cXyz mLookTarget;
    /* 0x678 */ s16 mLookNoMoveTimer;
    /* 0x67A */ s16 mLookDownTimer;
    /* 0x67C */ s16 mLookUpTimer;
    /* 0x67E */ s16 mHeadTargetAngleY;
    /* 0x680 */ s16 mHeadTargetAngleZ;
    /* 0x682 */ s16 mNeckAngleY;
    /* 0x684 */ s16 mTailSwayAngle[2];
    /* 0x688 */ s16 mTailAngle;
    /* 0x68A */ s16 mTailTargetAngle;
    /* 0x68C */ f32 mTailSway;
    /* 0x690 */ f32 mTailSwayTarget;
    /* 0x694 */ s16 mBackboneAngleY;
    /* 0x696 */ s16 mBackboneTargetAngleY;
    /* 0x698 */ u8 field_0x698[2];
    /* 0x69A */ s16 mHeadBobAngleY;
    /* 0x69C */ s16 mHeadBobAngleX;
    /* 0x69E */ u8 field_0x69e[2];
    /* 0x6A0 */ s16 mHeadBobTargetAngleY;
    /* 0x6A2 */ s16 mHeadBobTargetAngleX;
    /* 0x6A4 */ s16 mHeadBobTimer;
    /* 0x6A6 */ u8 field_0x6a6[0x12];
    /* 0x6B8 */ f32 field_0x6b8;
    /* 0x6BC */ f32 mWaterY;
    /* 0x6C0 */ u8 field_0x6c0[0x14];
    /* 0x6D4 */ f32 field_0x6d4;
    /* 0x6D8 */ cXyz mBaseScale;
    /* 0x6E4 */ cXyz mClimbPos;
    /* 0x6F0 */ s8 mCarryTimer;
    /* 0x6F1 */ u8 field_0x6f1[7];
    /* 0x6F8 */ s8 mSearchBall;
    /* 0x6FC */ int mHomePathIdx;
    /* 0x700 */ s8 mNoFollow;
    /* 0x701 */ s8 mWantsFish;
    /* 0x704 */ dBgS_AcchCir mAcchCir;
    /* 0x744 */ dBgS_ObjAcch mAcch;
    /* 0x91C */ f32 mGroundY;
    /* 0x920 */ dCcD_Stts mStts;
    /* 0x95C */ dCcD_Sph mCcSph;
    /* 0xA94 */ dCcD_Sph mAtSph;
    /* 0xBCC */ s8 mAtEnable;
    /* 0xBCD */ u8 field_0xbcd[0x27];
    /* 0xBF4 */ f32 field_0xbf4;
    /* 0xBF8 */ u8 field_0xbf8[8];
    /* 0xC00 */ u32 mParticle[2];
    /* 0xC08 */ s8 mMessageState;
    /* 0xC09 */ s16 mIsTalking;
    /* 0xC0C */ s16 mFlowID;
    /* 0xC10 */ dMsgFlow_c mMsgFlow;
    /* 0xC5C */ s16 mDemoMode;
    /* 0xC5E */ s16 mDemoCounter;
    /* 0xC60 */ s16 mDemoTimer;
    /* 0xC64 */ cXyz mCameraEye1;
    /* 0xC70 */ cXyz mCameraCenter1;
    /* 0xC7C */ u8 field_0xc7c[0xc];
    /* 0xC88 */ f32 mCameraFovY;
    /* 0xC8C */ u8 field_0xc8c[4];
    /* 0xC90 */ cXyz mDemoFishPos;
    /* 0xC9C */ cXyz mCameraEye2;
    /* 0xCA8 */ cXyz mCameraCenter2;
    /* 0xCB4 */ bool field_0xcb4;
    /* 0xCB6 */ s16 field_0xcb6;
    /* 0xCB8 */ f32 field_0xcb8;
    /* 0xCBC */ u32 mFishID;
    /* 0xCC0 */ bool field_0xcc0;
    /* 0xCC1 */ bool mHIOInit;
};

STATIC_ASSERT(sizeof(npc_ne_class) == 0xCC4);

class daNpc_Ne_HIO_c {
public:
    daNpc_Ne_HIO_c();
    virtual ~daNpc_Ne_HIO_c() {}

    /* 0x04 */ s8 field_0x04;
    /* 0x08 */ f32 mScale;
    /* 0x0C */ f32 mWalkSpeed;
    /* 0x10 */ f32 mRunSpeed;
    /* 0x14 */ f32 mSwimSpeed;
    /* 0x18 */ u8 field_0x18;
};

STATIC_ASSERT(sizeof(daNpc_Ne_HIO_c) == 0x1C);

struct home_path_pnt {
public:
    /* 0x0 */ int mNo;
    /* 0x4 */ Vec mPosition;
};  // Size: 0x10


#endif /* D_A_NPC_NE_H */
