#ifndef D_A_E_YD_H
#define D_A_E_YD_H
#include "SSystem/SComponent/c_cc_d.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class e_yd_class
 * @brief Twilight Deku Baba
 *
 * @details
 *
 */
class e_yd_class {
public:
    /* 0x0   */ fopEn_enemy_c actor;
    /* 0x5AC */ request_of_phase_process_class field_0x5ac;
    /* 0x5B4 */ u8 field_0x5b4;
    /* 0x5B5 */ u8 field_0x5b5;
    /* 0x5B6 */ u8 field_0x5b6;
    /* 0x5B7 */ u8 field_0x5b7;
    /* 0x5B8 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5BC */ Z2CreatureEnemy mCreatureSound;
    /* 0x660 */ mDoExt_brkAnm* mBrkAnm;
    /* 0x664 */ s32 field_0x664;
    /* 0x668 */ u32 mShadowKey;
    /* 0x66C */ s16 field_0x66c;
    /* 0x66E */ s16 field_0x66e;
    /* 0x670 */ s16 field_0x670;
    /* 0x674 */ f32 field_0x674;
    /* 0x678 */ cXyz field_0x678;
    /* 0x684 */ s16 field_0x684;
    /* 0x686 */ u8 field_0x686[0x688 - 0x686];
    /* 0x688 */ s16 mAngleToPlayer;
    /* 0x68C */ f32 mHomeDistFromPlayer;
    /* 0x690 */ s16 field_0x690;
    /* 0x694 */ f32 field_0x694;
    /* 0x698 */ f32 field_0x698;
    /* 0x69C */ s16 field_0x69c[4];
    /* 0x6A4 */ cXyz mJointPositions[12];
    /* 0x734 */ csXyz mJointRotations[12];
    /* 0x77C */ J3DModel* field_0x77c[11];
    /* 0x7A8 */ u8 field_0x7a8[0x7dc - 0x7a8];
    /* 0x7DC */ f32 field_0x7dc[11];
    /* 0x802 */ u8 field_0x808[0x80c - 0x808];
    /* 0x80C */ mDoExt_3DlineMat1_c mLineMat;
    /* 0x848 */ f32 field_0x848;
    /* 0x84C */ f32 field_0x84c;
    /* 0x850 */ f32 field_0x850;
    /* 0x854 */ u8 field_0x854[0x855 - 0x854];
    /* 0x855 */ s8 mLeafRoomNo;
    /* 0x856 */ s8 field_0x856;
    /* 0x858 */ f32 field_0x858;
    /* 0x85C */ u8 field_0x85c[0x864 - 0x85C];
    /* 0x864 */ dCcD_Stts mStts;
    /* 0x8A0 */ dCcD_Sph field_0x8a0;
    /* 0x9D8 */ dCcD_Sph field_0x9d8;
    /* 0xB10 */ s8 field_0xb10;
    /* 0xB11 */ s8 field_0xb11;
    /* 0xB14 */ dCcD_Sph field_0xb14[4];
    /* 0xFF4 */ dCcU_AtInfo field_0xff4;
    /* 0x1018 */ dBgS_AcchCir field_0x1018;
    /* 0x1058 */ dBgS_ObjAcch field_0x1058;
    /* 0x1230 */ fpc_ProcID mLeafId;
    /* 0x1234 */ s8 field_0x1234;
    /* 0x1235 */ s8 field_0x1235;
    /* 0x1238 */ u32 field_0x1238[3];
    /* 0x1244 */ s32 field_0x1244;
    /* 0x1248 */ s32 field_0x1248;
    /* 0x124C */ s32 field_0x124c;
    /* 0x1250 */ f32 field_0x1250;
    /* 0x1254 */ u8 field_0x1254[0x1268 - 0x1254];
    /* 0x1268 */ u8 field_0x1268;
};

STATIC_ASSERT(sizeof(e_yd_class) == 0x126c);


#endif /* D_A_E_YD_H */
