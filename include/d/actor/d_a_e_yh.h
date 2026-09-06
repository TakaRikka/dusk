#ifndef D_A_E_YH_H
#define D_A_E_YH_H
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class e_yh_class
 * @brief Twilight Hebi Baba
 * 
 * @details 
 * 
 */
class e_yh_class {
public:
    /* 0x0000 */ fopEn_enemy_c enemy;
    /* 0x05AC */ request_of_phase_process_class mPhase;
    /* 0x05B4 */ u8 field_0x5b4;
    /* 0x05B5 */ u8 field_0x5b5;
    /* 0x05B6 */ u8 field_0x5b6;
    /* 0x05B7 */ u8 field_0x5b7;
    /* 0x05B8 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x05BC */ Z2CreatureEnemy mSound;
    /* 0x0660 */ mDoExt_brkAnm* mBrk;
    /* 0x0664 */ int field_0x664;
    /* 0x0668 */ int mShadowKey;
    /* 0x066C */ s16 field_0x66c;
    /* 0x066E */ s16 field_0x66e;
    /* 0x0670 */ s16 field_0x670;
    /* 0x0674 */ f32 field_0x674;
    /* 0x0678 */ cXyz field_0x678;
    /* 0x0684 */ s16 field_0x684;
    /* 0x0686 */ s16 field_0x686;
    /* 0x0688 */ s16 field_0x688;
    /* 0x068C */ f32 field_0x68c;
    /* 0x0690 */ f32 field_0x690;
    /* 0x0694 */ f32 field_0x694;
    /* 0x0698 */ s16 field_0x698[3];
    /* 0x069E */ s16 field_0x69e;
    /* 0x06A0 */ cXyz field_0x6a0[12];
    /* 0x0730 */ csXyz field_0x730[12];
    /* 0x0730 */ J3DModel* mModels[12];
    /* 0x07A8 */ f32 field_0x7a8[12];
    /* 0x07D8 */ f32 field_0x7d8[12];
    /* 0x0808 */ mDoExt_3DlineMat1_c mLine;
    /* 0x0844 */ f32 field_0x844;
    /* 0x0848 */ f32 field_0x848;
    /* 0x084C */ f32 field_0x84c;
    /* 0x0850 */ s16 field_0x850;
    /* 0x0852 */ s16 field_0x852;
    /* 0x0854 */ s8 field_0x854;
    /* 0x0855 */ s8 field_0x855;
    /* 0x0858 */ f32 field_0x858;
    /* 0x085C */ f32 field_0x85c;
    /* 0x085C */ s16 field_0x860;
    /* 0x0862 */ s16 field_0x862;
    /* 0x0864 */ s16 field_0x864;
    /* 0x0868 */ f32 field_0x868;
    /* 0x086C */ dCcD_Stts mStts;
    /* 0x08A8 */ dCcD_Sph mCcSph;
    /* 0x09E0 */ dCcD_Sph mAtSph;
    /* 0x0B18 */ s8 field_0xb18;
    /* 0x0B19 */ s8 field_0xb19;
    /* 0x0B1C */ dCcD_Sph mKukiSphs[4];
    /* 0x0FFC */ dCcU_AtInfo mAtInfo;
    /* 0x1020 */ dBgS_AcchCir mAcchCir;
    /* 0x1060 */ dBgS_ObjAcch mAcch;
    /* 0x1238 */ fpc_ProcID mChildID;
    /* 0x123C */ s8 field_0x123c;
    /* 0x123D */ s8 field_0x123d;
    /* 0x1240 */ u32 mParticleIds[3];
    /* 0x124C */ u32 mParticleIds2[3];
    /* 0x125C */ f32 field_0x1258;
    /* 0x125C */ u32 field_0x125c;
    /* 0x1260 */ u32 field_0x1260;
    /* 0x1260 */ u8 field_0x1264[0x1270 - 0x1264];
    /* 0x1270 */ bool mIsHIOOwner;
};

STATIC_ASSERT(sizeof(e_yh_class) == 0x1274);

#endif /* D_A_E_YH_H */
