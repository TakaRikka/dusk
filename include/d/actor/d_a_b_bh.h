#ifndef D_A_B_BH_H
#define D_A_B_BH_H

#include "f_op/f_op_actor_mng.h"
#include "JSystem/JHostIO/JORMContext.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_bg_s_acch.h"

/**
 * @ingroup actors-enemies
 * @class b_bh_class
 * @brief Diababa - Baba Hand
 *
 * @details Forest Temple enemy.
 *
 */
class b_bh_class : public fopEn_enemy_c {
public:
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ u8 mID;
    /* 0x5B5 */ u8 field_0x5b5;
    /* 0x5B6 */ u8 field_0x5b6;
    /* 0x5B7 */ u8 field_0x5b7;
    /* 0x5B8 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x5BC */ Z2CreatureEnemy mSound;
    /* 0x660 */ int mAnm;
    /* 0x664 */ u8 field_0x664[0x668 - 0x664];
    /* 0x668 */ s16 field_0x668;
    /* 0x66A */ s16 mAction;
    /* 0x66C */ s16 mMode;
    /* 0x66E */ u8 field_0x66E[0x674 - 0x66E];
    /* 0x674 */ cXyz field_0x674;
    /* 0x680 */ u8 field_0x680[0x684 - 0x680];
    /* 0x684 */ s16 field_0x684;
    /* 0x686 */ s16 field_0x686;
    /* 0x688 */ f32 field_0x688;
    /* 0x68C */ f32 field_0x68c;
    /* 0x690 */ f32 field_0x690;
    /* 0x694 */ s16 mTimers[5];
    /* 0x69E */ s16 field_0x69e;
    /* 0x6A0 */ s8 field_0x6a0;
    /* 0x6A4 */ cXyz mBasePos;
    /* 0x6B0 */ cXyz field_0x6b0;
    /* 0x6BC */ cXyz field_0x6bc[18];
    /* 0x794 */ csXyz field_0x794[18];
    /* 0x800 */ J3DModel* mpStemModels[17];
    /* 0x844 */ u8 field_0x844[0x88C - 0x844];
    /* 0x88C */ f32 field_0x88c[17];
    /* 0x8D0 */ u8 field_0x8D0[0x8D4 - 0x8D0];
    /* 0x8D4 */ s16 field_0x8d4[17];
    /* 0x8F6 */ u8 field_0x8f6[0x8F8 - 0x8F6];
    /* 0x8F8 */ s16 field_0x8f8;
    /* 0x8FC */ f32 field_0x8fc;
    /* 0x900 */ s16 field_0x900[3];
    /* 0x906 */ s8 field_0x906[3];
    /* 0x909 */ s8 field_0x909[3];
    /* 0x90C */ s8 field_0x90c;
    /* 0x910 */ f32 field_0x910;
    /* 0x914 */ f32 field_0x914;
    /* 0x918 */ f32 field_0x918;
    /* 0x91C */ J3DModel* mpLeafModels[17];
    /* 0x960 */ f32 field_0x960[17];
    /* 0x9A4 */ u8 field_0x9A4[0x9A8 - 0x9A4];
    /* 0x9A8 */ csXyz field_0x9a8[18];
    /* 0xA14 */ u8 field_0xA14[0xA20 - 0xA14];
    /* 0xA20 */ f32 field_0xa20;
    /* 0xA24 */ s8 field_0xa24;
    /* 0xA25 */ s8 field_0xa25;
    /* 0xA26 */ csXyz field_0xa26;
    /* 0xA2C */ s8 mAnmSeTimer;
    /* 0xA30 */ u32 mAnmSe;
    /* 0xA34 */ dCcD_Stts mColliderStts;
    /* 0xA70 */ dCcD_Sph mCcSph;
    /* 0xBA8 */ dCcD_Sph mTgSph;
    /* 0xCE0 */ s8 field_0xce0;
    /* 0xCE4 */ dCcU_AtInfo mAtInfo;
    /* 0xD08 */ dBgS_AcchCir mAcchCir;
    /* 0xD48 */ dBgS_ObjAcch mAcch;
    /* 0xF20 */ s8 field_0xf20;
    /* 0xF21 */ s8 field_0xf21;
    /* 0xF22 */ s8 field_0xf22;
    /* 0xF24 */ u32 field_0xf24;
    /* 0xF28 */ u8 field_0xF28[0xF30 - 0xF28];
    /* 0xF30 */ u32 mMouthMizuEmitterIDs[5];
    /* 0xF44 */ f32 mMouthMizuParticleSize;
    /* 0xF48 */ u8 mInitHIO;
};

STATIC_ASSERT(sizeof(b_bh_class) == 0xf4c);

class daB_BH_HIO_c : public JORReflexible {
public:
    daB_BH_HIO_c();
    virtual ~daB_BH_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 no;
    /* 0x08 */ f32 model_size;
    /* 0x0C */ u8 unk_0xc[0x10 - 0xC];
    /* 0x10 */ s16 attack_freq_a;
    /* 0x12 */ s16 attack_freq_b;
    /* 0x14 */ s16 down_revive_time;
};


#endif /* D_A_B_BH_H */
