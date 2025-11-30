#ifndef D_A_E_DN_H
#define D_A_E_DN_H
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_jnt_col.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class e_dn_class
 * @brief Lizalfos
 * 
 * @details 
 * 
 */
class e_dn_class {
public:
    e_dn_class();
    
    /* 0x0000 */ fopEn_enemy_c actor;
    /* 0x05AC */ request_of_phase_process_class mPhase;
    /* 0x05B4 */ s16 mMode;
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
    /* 0x05E4 */ J3DModel* mpKnifeModel;
    /* 0x05E8 */ Z2CreatureEnemy mSound;
    /* 0x068C */ J3DModel* mpShieldModel;
    /* 0x0690 */ J3DModel* mpSkullModel;
    /* 0x0694 */ s8 field_0x694;
    /* 0x0698 */ cXyz field_0x698;
    /* 0x06A4 */ cXyz field_0x6a4;
    /* 0x06B0 */ s16 field_0x6b0;
    /* 0x06B2 */ s16 field_0x6b2;
    /* 0x06B4 */ dJntCol_c mJntCol;
    /* 0x06C4 */ s8 field_0x6c4;
    /* 0x06C8 */ f32 field_0x6c8;
    /* 0x06CC */ s16 field_0x6cc;
    /* 0x06CE */ s16 mAction;
    /* 0x06D0 */ f32 mPlayerDistance;
    /* 0x06D4 */ s16 mYAngleToPlayer;
    /* 0x06D6 */ s16 mXAngleToPlayer;
    /* 0x06D8 */ f32 field_0x6d8;
    /* 0x06DC */ u32 mShadowKey;
    /* 0x06E0 */ s16 field_0x6e0[4];
    /* 0x06E8 */ s16 field_0x6e8;
    /* 0x06EA */ s16 field_0x6ea;
    /* 0x06EC */ s16 field_0x6ec;
    /* 0x06EE */ s8 field_0x6ee;
    /* 0x06EF */ s8 field_0x6ef;
    /* 0x06F0 */ s8 field_0x6f0;
    /* 0x06F1 */ s8 field_0x6f1;
    /* 0x06F2 */ s8 field_0x6f2;
    /* 0x06F3 */ s8 field_0x6f3;
    /* 0x06F4 */ s8 field_0x6f4;
    /* 0x06F5 */ u8 field_0x6f5;
    /* 0x06F6 */ s16 field_0x6f6;
    /* 0x06F8 */ s16 field_0x6f8;
    /* 0x06FA */ s16 field_0x6fa;
    /* 0x06FC */ s16 field_0x6fc;
    /* 0x06FE */ s16 field_0x6fe;
    /* 0x0700 */ s8 field_0x700;
    /* 0x0701 */ s8 field_0x701;
    /* 0x0704 */ f32 field_0x704;
    /* 0x0708 */ f32 field_0x708;
    /* 0x070C */ s16 field_0x70c;
    /* 0x070E */ s16 field_0x70e;
    /* 0x0710 */ s8 field_0x710;
    /* 0x0711 */ u8 field_0x711[15];
    /* 0x0720 */ f32 field_0x720;
    /* 0x0724 */ csXyz field_0x724;
    /* 0x072A */ csXyz field_0x72a;
    /* 0x0730 */ csXyz field_0x730;
    /* 0x0736 */ s8 field_0x736;
    /* 0x0737 */ s8 field_0x737;
    /* 0x0738 */ cXyz field_0x738;
    /* 0x0744 */ int field_0x744;
    /* 0x0748 */ u8 field_0x748[4];
    /* 0x074C */ f32 field_0x74c;
    /* 0x0750 */ s16 field_0x750;
    /* 0x0754 */ f32 field_0x754;
    /* 0x0758 */ s16 field_0x758;
    /* 0x075C */ f32 field_0x75c;
    /* 0x0760 */ s16 field_0x760;
    /* 0x0762 */ csXyz field_0x762[11];
    /* 0x07A4 */ csXyz field_0x7a4[11];
    /* 0x07E8 */ f32 field_0x7e8;
    /* 0x07EC */ s16 field_0x7ec[2];
    /* 0x07F0 */ s16 field_0x7f0;
    /* 0x07F2 */ s16 field_0x7f2;
    /* 0x07F4 */ s16 field_0x7f4;
    /* 0x07F6 */ s16 field_0x7f6;
    /* 0x07FA */ s16 field_0x7f8;
    /* 0x07FA */ s16 field_0x7fa;
    /* 0x07FC */ s16 field_0x7fc[3];
    /* 0x0802 */ s16 field_0x802[4];
    /* 0x080A */ s16 field_0x80a;
    /* 0x080C */ s16 field_0x80c;
    /* 0x080E */ csXyz field_0x80e[4];
    /* 0x0826 */ s16 field_0x826;
    /* 0x0828 */ s16 field_0x828;
    /* 0x082A */ s16 field_0x82a;
    /* 0x082C */ s16 field_0x82c;
    /* 0x082E */ s8 field_0x82e;
    /* 0x082F */ u8 field_0x82f;
    /* 0x0830 */ s16 field_0x830;
    /* 0x0832 */ s16 field_0x832;
    /* 0x0834 */ u8 field_0x834[2];
    /* 0x0836 */ csXyz field_0x836;
    /* 0x083C */ f32 field_0x83c;
    /* 0x0840 */ s16 field_0x840;
    /* 0x0842 */ s16 field_0x842;
    /* 0x0844 */ dBgS_AcchCir mAcchCir;
    /* 0x0884 */ dBgS_ObjAcch mObjAcch;
    /* 0x0A5C */ f32 field_0xa5c;
    /* 0x0A60 */ dCcD_Stts mStts;
    /* 0x0A9C */ dCcD_Sph field_0xa9c[3];
    /* 0x0E44 */ dCcD_Sph field_0xe44;
    /* 0x0F7C */ dCcD_Sph field_0xf7c;
    /* 0x10B4 */ dCcU_AtInfo mAtInfo;
    /* 0x10D8 */ u8 field_0x10d8;
    /* 0x10D9 */ u8 field_0x10d9;
    /* 0x10DA */ s8 field_0x10da;
    /* 0x10DB */ u8 field_0x10db;
    /* 0x10DC */ u32 field_0x10dc;
    /* 0x10E0 */ u32 field_0x10e0;
    /* 0x10E4 */ s8 field_0x10e4;
    /* 0x10E8 */ u32 field_0x10e8[2];
    /* 0x10F0 */ u32 field_0x10f0[2];
    /* 0x10F8 */ u32 field_0x10f8;
    /* 0x10FC */ u32 field_0x10fc;
    /* 0x1100 */ u8 field_0x1100[12];
};

STATIC_ASSERT(sizeof(e_dn_class) == 0x110c);

#endif /* D_A_E_DN_H */
