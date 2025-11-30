#ifndef D_A_E_MF_H
#define D_A_E_MF_H
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_jnt_col.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class e_mf_class
 * @brief Dynalfos
 * 
 * @details 
 * 
 */
class e_mf_class {
public:
    e_mf_class();

    /* 0x0000 */ fopEn_enemy_c actor;
    /* 0x05AC */ request_of_phase_process_class mPhase;
    /* 0x05B4 */ s16 field_0x5b4;
    /* 0x05B6 */ u8 field_0x5b6;
    /* 0x05B7 */ u8 field_0x5b7;
    /* 0x05B8 */ u8 field_0x5b8;
    /* 0x05B9 */ u8 field_0x5b9;
    /* 0x05BC */ cXyz field_0x5bc;
    /* 0x05C8 */ cXyz field_0x5c8;
    /* 0x05D4 */ s16 field_0x5d4;
    /* 0x05D6 */ s16 field_0x5d6;
    /* 0x05D8 */ s8 field_0x5d8;
    /* 0x05DC */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x05E0 */ int mAnmID;
    /* 0x05E4 */ J3DModel* mpAxeModel;
    /* 0x05E8 */ Z2CreatureEnemy mSound;
    /* 0x068C */ J3DModel* mpShieldModel;
    /* 0x0690 */ u8 field_0x690[4];
    /* 0x0694 */ dJntCol_c mJntCol;
    /* 0x06A4 */ s8 field_0x6a4;
    /* 0x06A8 */ f32 field_0x6a8;
    /* 0x06AC */ s16 field_0x6ac;
    /* 0x06AE */ s16 mAction;
    /* 0x06B0 */ f32 mPlayerDistance;
    /* 0x06B4 */ s16 mYAngleToPlayer;
    /* 0x06B6 */ s16 mXAngleToPlayer;
    /* 0x06B8 */ f32 field_0x6b8;
    /* 0x06BC */ u32 mShadowKey;
    /* 0x06C0 */ s16 field_0x6c0[4];
    /* 0x06C8 */ s16 field_0x6c8;
    /* 0x06CA */ s16 field_0x6ca;
    /* 0x06CC */ s16 field_0x6cc;
    /* 0x06CE */ s8 field_0x6ce;
    /* 0x06CF */ s8 field_0x6cf;
    /* 0x06D0 */ s8 field_0x6d0;
    /* 0x06D1 */ s8 field_0x6d1;
    /* 0x06D2 */ s8 field_0x6d2;
    /* 0x06D3 */ s8 field_0x6d3;
    /* 0x06D4 */ s8 field_0x6d4;
    /* 0x06D5 */ u8 field_0x6d5;
    /* 0x06D6 */ s16 field_0x6d6;
    /* 0x06D8 */ s16 field_0x6d8;
    /* 0x06DA */ s16 field_0x6da;
    /* 0x06DC */ s16 field_0x6dc;
    /* 0x06DE */ s16 field_0x6de;
    /* 0x06E0 */ s8 field_0x6e0;
    /* 0x06E1 */ s8 field_0x6e1;
    /* 0x06E4 */ f32 field_0x6e4;
    /* 0x06E8 */ f32 field_0x6e8;
    /* 0x06EC */ s16 field_0x6ec;
    /* 0x06EE */ s16 field_0x6ee;
    /* 0x06F0 */ s8 field_0x6f0;
    /* 0x06F1 */ u8 field_0x6f1[0x700 - 0x6f1];
    /* 0x0700 */ f32 field_0x700;
    /* 0x0704 */ csXyz field_0x704;
    /* 0x070A */ csXyz field_0x70a;
    /* 0x0710 */ csXyz field_0x710;
    /* 0x0716 */ s8 field_0x716;
    /* 0x0717 */ s8 field_0x717;
    /* 0x0718 */ cXyz field_0x718;
    /* 0x0724 */ u32 field_0x724;
    /* 0x0728 */ u8 field_0x728;
    /* 0x072C */ f32 field_0x72c;
    /* 0x0730 */ s16 field_0x730;
    /* 0x0734 */ f32 field_0x734;
    /* 0x0738 */ s16 field_0x738;
    /* 0x073C */ f32 field_0x73c;
    /* 0x0740 */ s16 field_0x740;
    /* 0x0742 */ csXyz field_0x742[11];
    /* 0x0784 */ csXyz field_0x784[11];
    /* 0x07C8 */ f32 field_0x7c8;
    /* 0x07CC */ s16 field_0x7cc[2];
    /* 0x07D0 */ s16 field_0x7d0;
    /* 0x07D2 */ s16 field_0x7d2;
    /* 0x07D4 */ s16 field_0x7d4;
    /* 0x07D6 */ s16 field_0x7d6;
    /* 0x07D8 */ s16 field_0x7d8;
    /* 0x07DA */ s16 field_0x7da;
    /* 0x07DC */ s16 field_0x7dc[3];
    /* 0x07E2 */ s16 field_0x7e2[4];
    /* 0x07EA */ s16 field_0x7ea;
    /* 0x07EC */ s16 field_0x7ec;
    /* 0x07EE */ csXyz field_0x7ee[4];
    /* 0x0806 */ s16 field_0x806;
    /* 0x0808 */ s16 field_0x808;
    /* 0x080A */ s16 field_0x80a;
    /* 0x080C */ s16 field_0x80c;
    /* 0x080E */ s8 field_0x80e;
    /* 0x0810 */ s16 field_0x810;
    /* 0x0812 */ s16 field_0x812;
    /* 0x0814 */ u8 field_0x814[2];
    /* 0x0816 */ csXyz field_0x816;
    /* 0x081C */ f32 field_0x81c;
    /* 0x0820 */ s16 field_0x820;
    /* 0x0822 */ s16 field_0x822;
    /* 0x0824 */ dBgS_AcchCir mAcchCir;
    /* 0x0864 */ dBgS_ObjAcch mObjAcch;
    /* 0x0A3C */ f32 field_0xa3c;
    /* 0x0A40 */ dCcD_Stts mStts;
    /* 0x0A7C */ dCcD_Sph field_0xa7c[3];
    /* 0x0E24 */ dCcD_Sph field_0xe24;
    /* 0x0F5C */ dCcD_Sph field_0xf5c;
    /* 0x1094 */ dCcU_AtInfo mAtInfo;
    /* 0x10B8 */ u8 field_0x10b8;
    /* 0x10B9 */ u8 field_0x10b9;
    /* 0x10BA */ s8 field_0x10ba;
    /* 0x10BB */ u8 field_0x10bb;
    /* 0x10BC */ u32 field_0x10bc;
    /* 0x10C0 */ u32 field_0x10c0;
    /* 0x10C4 */ s8 field_0x10c4;
    /* 0x10C8 */ u32 field_0x10c8[2];
    /* 0x10D0 */ u32 field_0x10d0[2];
    /* 0x10D8 */ u8 field_0x10d8[0x10e4 - 0x10d8];
    /* 0x10E4 */ u32 field_0x10e4;
    /* 0x10E8 */ u32 field_0x10e8;
};

STATIC_ASSERT(sizeof(e_mf_class) == 0x10ec);

#endif /* D_A_E_MF_H */
