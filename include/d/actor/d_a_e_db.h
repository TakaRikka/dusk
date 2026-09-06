#ifndef D_A_E_DB_H
#define D_A_E_DB_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

/**
 * @ingroup actors-enemies
 * @class e_db_class
 * @brief Deku Baba
 * 
 * @details 
 * 
 */
class e_db_class {
public:
    /* 0x0000 */ fopEn_enemy_c enemy;
    /* 0x05AC */ request_of_phase_process_class phase;
    /* 0x05B4 */ u8 arg0;
    /* 0x05B5 */ u8 arg1;
    /* 0x05B6 */ u8 arg2;
    /* 0x05B7 */ u8 arg3;
    /* 0x05B8 */ mDoExt_McaMorfSO* modelMorf;
    /* 0x05BC */ Z2CreatureEnemy sound;
    /* 0x0660 */ int anm;
    /* 0x0664 */ u32 shadowId;
    /* 0x0668 */ s16 counter;
    /* 0x066A */ s16 action;
    /* 0x066C */ s16 mode;
    /* 0x0670 */ f32 pl_search_range;
    /* 0x0674 */ cXyz field_0x674;
    /* 0x0680 */ s16 field_0x680;
    /* 0x0682 */ u8 field_0x682[0x0684 - 0x0682];
    /* 0x0684 */ s16 angle_to_player;
    /* 0x0688 */ f32 dist_to_player;
    /* 0x068C */ f32 field_0x68c;
    /* 0x0690 */ f32 size;
    /* 0x0694 */ s16 timers[3];
    /* 0x069A */ s16 invulnerabilityTimer;
    /* 0x069C */ cXyz field_0x69c[12];
    /* 0x072C */ csXyz field_0x72c[12];
    /* 0x0774 */ J3DModel* thornModel[12];
    /* 0x07A4 */ f32 field_0x7a4[12];
    /* 0x07D4 */ f32 thorn_size[12];
    /* 0x0804 */ mDoExt_3DlineMat1_c stalkLine;
    /* 0x0840 */ f32 field_0x840;
    /* 0x0844 */ f32 field_0x844;
    /* 0x0848 */ f32 field_0x848;
    /* 0x084C */ s16 field_0x84c;
    /* 0x084E */ s16 field_0x84e;
    /* 0x0850 */ s8 field_0x850;
    /* 0x0851 */ s8 field_0x851;
    /* 0x0852 */ s8 field_0x852;
    /* 0x0854 */ f32 field_0x854;
    /* 0x0858 */ f32 field_0x858;
    /* 0x085C */ s16 field_0x85c;
    /* 0x085E */ s16 field_0x85e;
    /* 0x0860 */ s16 field_0x860;
    /* 0x0864 */ f32 field_0x864;
    /* 0x0868 */ dCcD_Stts ccStts;
    /* 0x08A4 */ dCcD_Sph ccSph;
    /* 0x09DC */ dCcD_Sph atSph;
    /* 0x0B14 */ s8 field_0xb14;
    /* 0x0B15 */ u8 field_0xb15;
    /* 0x0B18 */ dCcD_Sph kukiSph[4];
    /* 0x0FF8 */ dCcU_AtInfo atInfo;
    /* 0x101C */ dBgS_AcchCir acchcir;
    /* 0x105C */ dBgS_ObjAcch acch;
    /* 0x1234 */ fpc_ProcID leaf_actor_id;
    /* 0x1238 */ s8 field_0x1238;
    /* 0x1239 */ s8 field_0x1239;
    /* 0x123C */ u32 field_0x123c[3];
    /* 0x1248 */ u32 field_0x1248;
    /* 0x124C */ u32 field_0x124c;
    /* 0x1250 */ u32 field_0x1250;
    /* 0x1254 */ f32 field_0x1254;
    /* 0x1258 */ u32 field_0x1258;
    /* 0x125C */ u32 field_0x125c;
    /* 0x1260 */ u8 field_0x1260[0x126C - 0x1260];
    /* 0x126C */ u8 HIOInit;
};

STATIC_ASSERT(sizeof(e_db_class) == 0x1270);

#endif /* D_A_E_DB_H */
