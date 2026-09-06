#ifndef D_A_E_S1_H
#define D_A_E_S1_H

#include "f_op/f_op_actor_mng.h"
#include "Z2AudioLib/Z2Creature.h"
#include "d/d_jnt_col.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

struct dPath;

struct s1_ke_s {
    /* 0x000 */ cXyz field_0x0[16];
    /* 0x0C0 */ cXyz field_0xc0[16];
    /* 0x180 */ cXyz field_0x180;
};

/**
 * @ingroup actors-enemies
 * @class e_s1_class
 * @brief Shadow Beast
 * 
 * @details 
 * 
 */
class e_s1_class : public fopEn_enemy_c {
public:
    /* 0x05AC */ request_of_phase_process_class mPhase;
    /* 0x05B4 */ u8 mRoofSpawn;
    /* 0x05B5 */ u8 mPrm1;
    /* 0x05B6 */ u8 mGroupID;
    /* 0x05B7 */ u8 mSwBit;
    /* 0x05B8 */ int mEntryNo;
    /* 0x05BC */ dPath* mpPath;
    /* 0x05C0 */ s8 mCurrentPathPointNo;
    /* 0x05C1 */ s8 mPathDirection;
    /* 0x05C4 */ cXyz mPathTargetPos;
    /* 0x05D0 */ s16 mTargetAngle;
    /* 0x05D4 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x05D8 */ int mAnm;
    /* 0x05DC */ u8 field_0x5DC[0x05E0 - 0x05DC];
    /* 0x05E0 */ Z2CreatureEnemy mSound;
    /* 0x0684 */ dJntCol_c mJntCol;
    /* 0x0694 */ s16 mCounter;
    /* 0x0696 */ s16 mAction;
    /* 0x0698 */ s16 mMode;
    /* 0x069A */ s8 mDrawShadow;
    /* 0x069C */ u32 mShadowKey;
    /* 0x06A0 */ s16 mTimers[4];
    /* 0x06A8 */ s16 mHitInvincibilityTimer;
    /* 0x06AA */ s16 field_0x6aa;
    /* 0x06AC */ f32 field_0x6ac;
    /* 0x06B0 */ s16 field_0x6b0;
    /* 0x06B2 */ s16 field_0x6b2;
    /* 0x06B4 */ u8 field_0x6B4[0x06B6 - 0x06B4];
    /* 0x06B6 */ s16 field_0x6b6;
    /* 0x06B8 */ s8 mIsSlowAttack;
    /* 0x06B9 */ s8 field_0x6b9;
    /* 0x06BA */ s8 field_0x6ba;
    /* 0x06BB */ s8 field_0x6bb;
    /* 0x06BC */ dBgS_AcchCir mAcchCir;
    /* 0x06FC */ dBgS_ObjAcch mAcch;
    /* 0x08D4 */ dCcD_Stts mCcStts;
    /* 0x0910 */ dCcD_Sph mCcSph[3];
    /* 0x0CB8 */ dCcD_Sph mCcAtSph;
    /* 0x0DF0 */ dCcU_AtInfo mAtInfo;
    /* 0x0E14 */ s1_ke_s mKe[22];
    /* 0x301C */ mDoExt_3DlineMat0_c mLineMat;
    /* 0x3038 */ s16 mDemoMode;
    /* 0x303A */ s16 mDemoTimer;
    /* 0x303C */ cXyz mDemoCamEye;
    /* 0x3048 */ cXyz mDemoCamCenter;
    /* 0x3054 */ f32 field_0x3054;
    /* 0x3058 */ f32 field_0x3058;
    /* 0x305C */ f32 mDemoCamBank;
    /* 0x3060 */ f32 field_0x3060;
    /* 0x3064 */ f32 mSearchRange;
    /* 0x3068 */ f32 field_0x3068;
    /* 0x306C */ s8 field_0x306c;
    /* 0x306D */ u8 field_0x306D[0x307C - 0x306D];
    /* 0x307C */ u32 mBodyEffEmtrID;
    /* 0x3080 */ u8 mInitHIO;
};

STATIC_ASSERT(sizeof(e_s1_class) == 0x3084);

#endif /* D_A_E_S1_H */
