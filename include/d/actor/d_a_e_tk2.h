#ifndef D_A_E_TK2_H
#define D_A_E_TK2_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-enemies
 * @class e_tk2_class
 * @brief Fire Toadpoli
 *
 * @details Fire Toadpoli in the Goron Mines Entrance.
 * Armored, two-legged frogs that spit Fireballs at Link.
 * They cannot swim, but will dive under the lava if Link
 * gets too close. They share most of the code with the
 * Water Toadpoli e_tk.
 */
 
class e_tk2_class : public fopEn_enemy_c {
public:
    /* 0x5AC */ request_of_phase_process_class mPhaseReq;
    /* 0x5B4 */ u8 mArg0;
    /* 0x5B5 */ u8 mArg1;
    /* 0x5B6 */ u8 mArg2;
    /* 0x5B7 */ u8 mArg3; /* Unused */
    /* 0x5B8 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5BC */ int mAnim;
    /* 0x5C0 */ Z2CreatureEnemy mSound;
    /* 0x664 */ f32 mAnimSpeed;
    /* 0x668 */ s16 mLifetime;
    /* 0x66A */ s16 mAction;
    /* 0x66C */ s16 mMode;
    /* 0x670 */ cXyz mPos;
    /* 0x67C */ s16 mSomeAngle;
    /* 0x67E */ u8 field18_0x67e[2];
    /* 0x680 */ s16 mPlayerAngleY;
    /* 0x684 */ f32 mPlayerDistanceLimit;
    /* 0x688 */ u8 field24_0x688[2];
    /* 0x68A */ s16 mActionTimer[3];
    /* 0x690 */ s16 mExecuteState;
    /* 0x692 */ s16 mInvincibilityTimer;
    /* 0x694 */ s8 mAttentionOFF;
    /* 0x695 */ s8 mTKBallSpawned;
    /* 0x698 */ fpc_ProcID mBallID;
    /* 0x69C */ dBgS_AcchCir mAcchCir;
    /* 0x6DC */ dBgS_ObjAcch mAcch;
    /* 0x8B4 */ dCcD_Stts mStts;
    /* 0x8F0 */ dCcD_Sph mSph;
    /* 0xA28 */ dCcU_AtInfo mAtInfo;
    /* 0xA4C */ bool mInitHIO;
};

STATIC_ASSERT(sizeof(e_tk2_class) == 0xA50);

class daE_TK2_HIO_c {
public:
    daE_TK2_HIO_c();
    virtual ~daE_TK2_HIO_c() {};

    /* 0x04 */ s8 field_0x04;
    /* 0x08 */ f32 mRadiusScale;
    /* 0x0C */ f32 mPlayerRange1;
    /* 0x10 */ f32 mPlayerRange2;
    /* 0x14 */ f32 mSpeedModifier1; /* Unused */
    /* 0x18 */ f32 mSpeedModifier2; /* Unused */
};

STATIC_ASSERT(sizeof(daE_TK2_HIO_c) == 0x1C);

#endif /* D_A_E_TK2_H */
