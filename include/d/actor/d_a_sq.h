#ifndef D_A_SQ_H
#define D_A_SQ_H

#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_msg_flow.h"

/**
 * @ingroup actors-unsorted
 * @class sq_class
 * @brief Squirrel
 *
 * @details Is this different from the talking npc actor?
 *
 */
class sq_class : public fopEn_enemy_c {
public:
    enum Action {
        /* 0 */ ACT_NORMAL,
        /* 1 */ ACT_AWAY,
        /* 2 */ ACT_CARRY,
        /* 3 */ ACT_FLY,
        /* 7 */ ACT_MESSAGE = 7,
    };

    /* 0x5AC */ request_of_phase_process_class mPhaseReq;
    /* 0x5B4 */ u8 mParam0;
    /* 0x5B5 */ u8 mParam1;
    /* 0x5B6 */ u8 mParam2;
    /* 0x5B7 */ u8 mParam3;
    /* 0x5B8 */ u8 field_0x5b8;
    /* 0x5B9 */ u8 field_0x5b9;
    /* 0x5BA */ u8 field_0x5ba[0xa];
    /* 0x5C4 */ cXyz field_0x5c4;
    /* 0x5D0 */ s16 mTargetAngleY;
    /* 0x5D4 */ f32 mPlayerDist;
    /* 0x5D8 */ u8 field_0x5d8[0xc];
    /* 0x5E4 */ s8 field_0x5e4;
    /* 0x5E5 */ u8 field_0x5e5[7];
    /* 0x5EC */ mDoExt_McaMorf* mpMorf;
    /* 0x5F0 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x5F4 */ s8 mBtkFrame;
    /* 0x5F6 */ s16 mCounter;
    /* 0x5F8 */ s16 mAction;
    /* 0x5FA */ s16 mMode;
    /* 0x5FC */ u32 mShadowKey;
    /* 0x600 */ s16 mTimer[4];
    /* 0x608 */ s16 mMessageTimer;
    /* 0x60A */ s16 mRunCheckTimer;
    /* 0x60C */ f32 field_0x60c;
    /* 0x610 */ s16 field_0x610;
    /* 0x612 */ csXyz field_0x612;
    /* 0x618 */ csXyz field_0x618;
    /* 0x61E */ s8 mMessageState;
    /* 0x620 */ s16 mIsTalking;
    /* 0x622 */ s16 mFlowID;
    /* 0x624 */ dMsgFlow_c mMsgFlow;
    /* 0x670 */ dBgS_AcchCir mAcchCir;
    /* 0x6B0 */ dBgS_ObjAcch mAcch;
    /* 0x888 */ dCcD_Stts mCcStts;
    /* 0x8C4 */ dCcD_Sph mCcSph;
    /* 0x9FC */ dCcD_Sph mAtSph;
    /* 0xB34 */ dCcU_AtInfo mAtInfo;
    /* 0xB58 */ bool mHIOInit;
};

STATIC_ASSERT(sizeof(sq_class) == 0xB5C);

class daSq_HIO_c {
public:
    daSq_HIO_c();
    virtual ~daSq_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mScale;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 mRunAnmSpeed;
    /* 0x14 */ bool mRunAway;
    /* 0x18 */ f32 mRunDistance;
    /* 0x1C */ f32 mRunSpeed;
    /* 0x20 */ f32 mFallSpeedXZ;
    /* 0x24 */ f32 mFallSpeedY;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
};

STATIC_ASSERT(sizeof(daSq_HIO_c) == 0x30);

#endif /* D_A_SQ_H */
