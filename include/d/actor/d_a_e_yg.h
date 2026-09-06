#ifndef D_A_E_YG_H
#define D_A_E_YG_H
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class e_yg_class
 * @brief Twilight Vermin
 * 
 * @details 
 * 
*/

struct yg_ke_s {
    /* 0x00 */ cXyz field_0x0[10];
    /* 0x78 */ cXyz field_0x78[10];
    /* 0xF0 */ cXyz field_0xf0;
};

class e_yg_class {
public:
    /* 0x0000 */ fopEn_enemy_c actor;
    /* 0x05AC */ request_of_phase_process_class mPhase;
    /* 0x05B4 */ s16 mActionMode;
    /* 0x05B6 */ u8 arg0;
    /* 0x05B7 */ u8 arg1;
    /* 0x05B8 */ int mSwBit;
    /* 0x05BC */ cXyz field_0x5bc;
    /* 0x05C8 */ s16 mCurrentAngleYTarget;
    /* 0x05CC */ mDoExt_McaMorfSO* mpMorf;
    /* 0x05D0 */ int mAnm;
    /* 0x05D4 */ u32 mShadowKey;
    /* 0x05D8 */ Z2CreatureEnemy mSound;
    /* 0x067C */ s16 mFrameCounter;
    /* 0x067E */ s16 mAction;
    /* 0x0680 */ f32 mPlayerDist;
    /* 0x0684 */ s16 mPlayerAngleY;
    /* 0x0686 */ s16 mTimers[4];
    /* 0x068E */ s16 mInvulnerabilityTimer;
    /* 0x0690 */ f32 mGroundCross;
    /* 0x0694 */ f32 mCurrentPosYOffset;
    /* 0x0698 */ dBgS_AcchCir mAcchCir;
    /* 0x06D8 */ dBgS_ObjAcch mBgc;
    /* 0x08B0 */ s8 mWaterFlag;
    /* 0x08B4 */ dCcD_Stts mStts;
    /* 0x08F0 */ dCcD_Sph mSph1;
    /* 0x0A28 */ dCcD_Sph mSph2;
    /* 0x0B60 */ dCcU_AtInfo mAtInfo;
    /* 0x0B84 */ s8 mUnkFlag1;
    /* 0x0B88 */ u32 mUnkPrtcl;
    /* 0x0B8C */ u32 mDownWtrAPrtcls[4];
    /* 0x0B9C */ u32 mHamonPrtcl;
    /* 0x0BA0 */ u8 field_0xba0[0xba5 - 0xba0];
    /* 0x0BA5 */ s8 mDispFlag;
    /* 0x0BA6 */ s8 mUnkFlag2;
    /* 0x0BA7 */ s8 mSplashTimer;
    /* 0x0BA8 */ s8 mDeathFlag;
    /* 0x0BAC */ f32 mDistance;
    /* 0x0BB0 */ fpc_ProcID mSkullActorID;
    /* 0x0BB4 */ yg_ke_s mYgKes[13];
    /* 0x1880 */ mDoExt_3DlineMat0_c mLineMat;
    /* 0x189C */ u8 mIsFirstSpawn;
};

STATIC_ASSERT(sizeof(e_yg_class) == 0x18a0);

#endif /* D_A_E_YG_H */
