#ifndef D_A_E_MB_H
#define D_A_E_MB_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

/**
 * @ingroup actors-enemies
 * @class e_mb_class
 * @brief Ook - Diababa Fight (Monkey Boomerang)
 * 
 * @details 
 * 
 */
class e_mb_class : public fopEn_enemy_c {
public:
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ s16 mMode;
    /* 0x5B6 */ u8 field_0x5b6;
    /* 0x5B8 */ cXyz field_0x5b8;
    /* 0x5C4 */ u8 field_0x5C4[0x5C8 - 0x5C4];
    /* 0x5C8 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x5CC */ int mAnm;
    /* 0x5D0 */ mDoExt_btpAnm* mpBtp;
    /* 0x5D4 */ s16 field_0x5d4;
    /* 0x5D8 */ f32 mBtpFrame;
    /* 0x5DC */ cXyz field_0x5dc;
    /* 0x5E8 */ Z2CreatureEnemy mSound;
    /* 0x68C */ s8 field_0x68c;
    /* 0x690 */ mDoExt_McaMorf* mpBombModelMorf;
    /* 0x694 */ mDoExt_McaMorf* mpBombModel2Morf;
    /* 0x698 */ s16 mCounter;
    /* 0x69A */ s16 mAction;
    /* 0x69C */ s16 mTimers[3];
    /* 0x6A2 */ s16 field_0x6a2;
    /* 0x6A4 */ cXyz field_0x6a4;
    /* 0x6B0 */ s8 field_0x6b0;
    /* 0x6B4 */ mDoExt_3DlineMat1_c mRopeMat;
    /* 0x6F0 */ f32 field_0x6f0;
    /* 0x6F4 */ dCcD_Stts mColliderStts;
    /* 0x730 */ dCcD_Sph mCcSph;
    /* 0x868 */ dCcU_AtInfo mAtInfo;
    /* 0x88C */ u8 field_0x88C[0x8C8 - 0x88C];
    /* 0x8C8 */ s8 field_0x8c8;
    /* 0x8C9 */ u8 mInitHIO;
};

STATIC_ASSERT(sizeof(e_mb_class) == 0x8cc);

class daE_MB_HIO_c : public JORReflexible {
public:
    daE_MB_HIO_c();
    virtual ~daE_MB_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 no;
    /* 0x08 */ f32 base_size;
    /* 0x0C */ f32 swing_speed;
    /* 0x10 */ f32 swing_descend;
    /* 0x14 */ f32 swing_correction;
    /* 0x18 */ f32 reappear_time;
};


#endif /* D_A_E_MB_H */
