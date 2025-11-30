#ifndef D_A_E_TK_H
#define D_A_E_TK_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_path.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-enemies
 * @class e_tk_class
 * @brief Water Toadpoli
 *
 * @details Water Toadpoli in a Water Grotto. (And Goron Mines?)
 * Frog-like creatures that emerge from the water and shoot a stone ball
 * towards Link. If Link gets too close to the toadpoli, it will dive
 * underwater, where it is invulnerable. It likes to swim around.
 */

class e_tk_class : public fopEn_enemy_c {
public:
    /* 0x5AC */ request_of_phase_process_class mPhaseReq;
    /* 0x5B4 */ u8 mArg0;
    /* 0x5B5 */ u8 mArg1;
    /* 0x5B6 */ u8 mArg2;
    /* 0x5B7 */ u8 mParamPathIdx;
    /* 0x5B8 */ s8 mPathLoaded;
    /* 0x5BC */ dPath* mpPath;
    /* 0x5C0 */ s8 mPathID;
    /* 0x5C1 */ s8 mPathDirection;
    /* 0x5C4 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5C8 */ int mAnim;
    /* 0x5CC */ Z2CreatureEnemy mSound;
    /* 0x670 */ f32 mAnimSpeed;
    /* 0x674 */ s16 mLifetime;
    /* 0x676 */ s16 mAction;
    /* 0x678 */ s16 mMode;
    /* 0x67C */ cXyz mPos;
    /* 0x688 */ s16 mSomeAngle;
    /* 0x68A */ u8 field_0x68a[2];
    /* 0x68C */ s16 mPlayerAngleY;
    /* 0x690 */ f32 mPlayerDistanceLimit;
    /* 0x694 */ u8 field_0x694[2];
    /* 0x696 */ s16 mActionTimer[3];
    /* 0x69C */ s16 mExecuteState;
    /* 0x69E */ s16 mInvincibilityTimer;
    /* 0x6A0 */ s8 mAttentionOFF;
    /* 0x6A1 */ s8 mTKBallSpawned;
    /* 0x6A4 */ u32 mHamonSet;
    /* 0x6A8 */ u8 field_0x6a8[4];
    /* 0x6AC */ fpc_ProcID mBallID;
    /* 0x6B0 */ dBgS_AcchCir mAcchCir;
    /* 0x6F0 */ dBgS_ObjAcch mAcch;
    /* 0x8C8 */ dCcD_Stts mStts;
    /* 0x904 */ dCcD_Sph mSph;
    /* 0xA3C */ dCcU_AtInfo mAtInfo;
    /* 0xA60 */ bool mInitHIO;
};

STATIC_ASSERT(sizeof(e_tk_class) == 0xA64);

class daE_TK_HIO_c {
public:
    daE_TK_HIO_c();
    virtual ~daE_TK_HIO_c() {}

    /* 0x04 */ s8 field_0x04;
    /* 0x08 */ f32 mRadiusScale;
    /* 0x0C */ f32 mPlayerRange1;
    /* 0x10 */ f32 mPlayerRange2;
    /* 0x14 */ f32 mSpeedModifier1;
    /* 0x18 */ f32 mSpeedModifier2;
};

STATIC_ASSERT(sizeof(daE_TK_HIO_c) == 0x1C);

#endif /* D_A_E_TK_H */
