#ifndef D_A_NI_H
#define D_A_NI_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_bg_s_acch.h"

#define BCK_FALL 5
#define BCK_FLY 6
#define BCK_IKAKU 7
#define BCK_MIWATASI 8
#define BCK_MOGAKU 9
#define BCK_NAKU 10
#define BCK_WAIT1 11
#define BCK_WALK_A 12
#define BCK_WALK_B 13

enum daNi_color {
    COLOR_WHITE,
    COLOR_BLACK,
    COLOR_BROWN,
    COLOR_GOLD,
    COLOR_RANDOM = 0xFF,
};

enum daNi_action {
    ACTION_NORMAL_e,
    ACTION_AWAY_e,
    ACTION_SWIM_e,
    ACTION_CARRY_e,
    ACTION_FLY_e,
    ACTION_DROP_e,
    ACTION_RETURN_e,
    ACTION_MESSAGE_e,
    ACTION_DAMAGE_e,
    ACTION_WINDSPIN_e,
    ACTION_DEMO0_e,
    ACTION_PLAY_e = 15,
};

/**
 * @ingroup actors-unsorted
 * @class ni_class
 * @brief Cucco
 *
 * @details
 *
 */
class ni_class : public fopEn_enemy_c {
public:
    bool checkGold() { return mColor == COLOR_GOLD; }

    void setMtx(MtxP mtx) {
        cXyz newPos;
        field_0xb08 = 1;
        mDoMtx_stack_c::copy(mtx);
        mDoMtx_stack_c::multVecZero(&newPos);
        current.pos = newPos;
        old.pos = current.pos;
        speed.y = 0.0f;
        speedF = 0.0f;
        mpMorf->getModel()->setBaseTRMtx(mtx);
    }

    void changeMode() { field_0x5fe = 1; }

    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ u8 field_0x5b4;
    /* 0x5B5 */ u8 mType;
    /* 0x5B6 */ u8 field_0x5b6[0x5C0 - 0x5B6];
    /* 0x5C0 */ cXyz field_0x5c0;
    /* 0x5CC */ cXyz mDemoInitPos;
    /* 0x5D8 */ f32 field_0x5d8;
    /* 0x5DC */ s16 mMoveAngle;
    /* 0x5E0 */ f32 mDistToPlayer;
    /* 0x5E4 */ s16 mAngleToPlayer;
    /* 0x5E8 */ mDoExt_McaMorf* mpMorf;
    /* 0x5EC */ mDoExt_btkAnm* mpBtk;
    /* 0x5F0 */ s8 mColor;
    /* 0x5F4 */ int mAnmID;
    /* 0x5F8 */ s16 mCounter;
    /* 0x5FA */ s16 mAction;
    /* 0x5FC */ s16 mMode;
    /* 0x5FE */ s16 field_0x5fe;
    /* 0x600 */ u32 mShadowKey;
    /* 0x604 */ s16 mTimers[4];
    /* 0x60C */ s16 field_0x60c;
    /* 0x60E */ s16 field_0x60e;
    /* 0x610 */ f32 field_0x610;
    /* 0x614 */ s16 field_0x614;
    /* 0x616 */ s16 field_0x616;
    /* 0x618 */ s16 field_0x618;
    /* 0x61A */ s16 field_0x61a;
    /* 0x61C */ s16 field_0x61c;
    /* 0x61E */ s16 field_0x61e;
    /* 0x620 */ s16 field_0x620;
    /* 0x622 */ s16 field_0x622;
    /* 0x624 */ s16 field_0x624;
    /* 0x626 */ s16 field_0x626;
    /* 0x628 */ s16 field_0x628;
    /* 0x62A */ s16 field_0x62a;
    /* 0x62C */ int mTargetActorID;
    /* 0x630 */ dBgS_AcchCir mAcchCir;
    /* 0x670 */ dBgS_ObjAcch mAcch;
    /* 0x848 */ s8 field_0x848;
    /* 0x84C */ dCcD_Stts mCcStts;
    /* 0x888 */ dCcD_Sph mCcSph;
    /* 0x9C0 */ dCcU_AtInfo mAtInfo;
    /* 0x9E4 */ s8 field_0x9e4;
    /* 0x9E6 */ s16 field_0x9e6;
    /* 0x9E8 */ f32 field_0x9e8;
    /* 0x9EC */ s16 field_0x9ec;
    /* 0x9EE */ s16 field_0x9ee[4];
    /* 0x9F8 */ f32 mSplGroundH;
    /* 0x9FC */ f32 mBoomerangMoveMaxStep;
    /* 0xA00 */ f32 field_0xa00;
    /* 0xA04 */ s16 field_0xa04;
    /* 0xA06 */ s8 field_0xa06;
    /* 0xA08 */ u32 field_0xa08[4];
    /* 0xA18 */ u32 mHamonEmtrID;
    /* 0xA1C */ u8 field_0xA1C[0xA20 - 0xA1C];
    /* 0xA20 */ u32 mEnoEmtrIDs[2];
    /* 0xA28 */ Z2SoundObjSimple mSound;
    /* 0xA48 */ s8 field_0xa48;
    /* 0xA4C */ u32 mHaneEmitterID;
    /* 0xA50 */ s8 field_0xa50;
    /* 0xA52 */ s16 field_0xa52;
    /* 0xA54 */ s16 mFlowNo;
    /* 0xA58 */ dMsgFlow_c mMsgFlow;
    /* 0xAA4 */ f32 mPadMainStickX;
    /* 0xAA8 */ f32 mPadMainStickY;
    /* 0xAAC */ f32 mPadSubStickX;
    /* 0xAB0 */ f32 mPadSubStickY;
    /* 0xAB4 */ f32 mPadAnalogL;
    /* 0xAB8 */ u32 mPadHoldL;
    /* 0xABC */ f32 field_0xabc;
    /* 0xAC0 */ f32 mPlayAnmSpeed;
    /* 0xAC4 */ s16 mPlayCamMode;
    /* 0xAC6 */ s16 mPlayCamModeTimer;
    /* 0xAC8 */ cXyz field_0xac8;
    /* 0xAD4 */ cXyz field_0xad4;
    /* 0xAE0 */ u8 field_0xAE0[0xAEC - 0xAE0];
    /* 0xAEC */ f32 field_0xaec;
    /* 0xAF0 */ f32 field_0xaf0;
    /* 0xAF4 */ s16 field_0xaf4;
    /* 0xAF8 */ f32 field_0xaf8;
    /* 0xAFC */ f32 field_0xafc;
    /* 0xB00 */ f32 field_0xb00;
    /* 0xB04 */ f32 field_0xb04;
    /* 0xB08 */ u8 field_0xb08;
    /* 0xB09 */ u8 mInitHIO;
};

STATIC_ASSERT(sizeof(ni_class) == 0xb0c);

class daNi_HIO_c {
public:
    daNi_HIO_c();
    virtual ~daNi_HIO_c() {}

    /* 0x04 */ s8 field_0x04;
    /* 0x08 */ f32 mBaseSize;
    /* 0x0C */ f32 mMoveSpeed;
    /* 0x10 */ f32 mMotionSpeed;
    /* 0x14 */ u8 mRecognizeLink;
    /* 0x18 */ f32 mRecognizeRange;
    /* 0x1C */ f32 mFleeWalkSpeed;
    /* 0x20 */ f32 mFleeFlySpeed;
    /* 0x24 */ f32 mFleeJumpPower;
    /* 0x28 */ f32 mFleeJumpGravity;
    /* 0x2C */ f32 mThrowSpeed;
    /* 0x30 */ f32 mThrowJumpPower;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ u8 mMogakuModelShake;
    /* 0x39 */ u8 field_0x39;
};


#endif /* D_A_NI_H */
