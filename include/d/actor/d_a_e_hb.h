#ifndef D_A_E_HB_H
#define D_A_E_HB_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

/**
 * @ingroup actors-enemies
 * @class e_hb_class
 * @brief Hebi Baba
 * 
 * @details 
 * 
 */
class e_hb_class {
public:
    /* 0x0000 */ fopEn_enemy_c enemy;
    /* 0x05AC */ request_of_phase_process_class phase;
    /* 0x05B4 */ u8 arg0;
    /* 0x05B5 */ u8 arg3;
    /* 0x05B6 */ u8 arg1;
    /* 0x05B7 */ u8 arg2;
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
    /* 0x068C */ s16 field_0x68c;
    /* 0x0690 */ f32 field_0x690;
    /* 0x0694 */ f32 size;
    /* 0x0698 */ s16 timers[3];
    /* 0x069E */ s16 invulnerabilityTimer;
    /* 0x06A0 */ cXyz field_0x6a0[12];
    /* 0x0730 */ csXyz field_0x730[12];
    /* 0x0778 */ J3DModel* thornModel[11];
    /* 0x07A4 */ u8 field_0x7A4[0x7D8 - 0x7A4];
    /* 0x07D8 */ f32 thorn_size[12];
    /* 0x0808 */ mDoExt_3DlineMat1_c stalkLine;
    /* 0x0844 */ f32 field_0x844;
    /* 0x0848 */ f32 field_0x848;
    /* 0x084C */ f32 field_0x84c;
    /* 0x0850 */ s8 field_0x850;
    /* 0x0851 */ s8 field_0x851;
    /* 0x0852 */ s8 field_0x852;
    /* 0x0854 */ f32 field_0x854;
    /* 0x0858 */ u8 field_0x858[0x860 - 0x858];
    /* 0x0860 */ dCcD_Stts ccStts;
    /* 0x089C */ dCcD_Sph ccSph;
    /* 0x09D4 */ dCcD_Sph atSph;
    /* 0x0B0C */ s8 field_0xb0c;
    /* 0x0B0D */ s8 field_0xb0d;
    /* 0x0B10 */ dCcD_Sph kukiSph[4];
    /* 0x0FF0 */ dCcU_AtInfo atInfo;
    /* 0x1014 */ dBgS_AcchCir acchcir;
    /* 0x1054 */ dBgS_ObjAcch acch;
    /* 0x122C */ u32 leaf_actor_id;
    /* 0x1230 */ s8 field_0x1230;
    /* 0x1231 */ s8 field_0x1231;
    /* 0x1234 */ u32 field_0x1234[3];
    /* 0x1240 */ u32 field_0x1240;
    /* 0x1244 */ u32 field_0x1244;
    /* 0x1248 */ u32 field_0x1248;
    /* 0x124C */ f32 field_0x124c;
    /* 0x1250 */ u8 field_0x1250[0x1264 - 0x1250];
    /* 0x1264 */ u8 HIOInit;
};

STATIC_ASSERT(sizeof(e_hb_class) == 0x1268);

#endif /* D_A_E_HB_H */
