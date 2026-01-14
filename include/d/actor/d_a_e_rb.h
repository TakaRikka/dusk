#ifndef D_A_E_RB_H
#define D_A_E_RB_H

#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_bg_s_acch.h"

/**
 * @ingroup actors-enemies
 * @class e_rb_class
 * @brief Leever (Riiba)
 * 
 * @details 
 * 
 */
class e_rb_class {
public:
    /* 0x000 */ fopEn_enemy_c enemy;
    /* 0x5AC */ request_of_phase_process_class phase;
    /* 0x5B4 */ fopAc_ac_c* parent;
    /* 0x5B8 */ dBgS_AcchCir acchcir;
    /* 0x5F8 */ dBgS_ObjAcch acch;
    /* 0x7D0 */ dCcD_Stts ccStts;
    /* 0x80C */ dCcD_Sph ccSph;
    /* 0x944 */ dCcU_AtInfo atInfo;
    /* 0x968 */ mDoExt_McaMorfSO* modelMorf;
    /* 0x96C */ Z2CreatureEnemy sound;
    /* 0xA10 */ cXyz field_0xa10;
    /* 0xA1C */ f32 distToPlayer;
    /* 0xA20 */ f32 height;
    /* 0xA24 */ f32 field_0xa24;
    /* 0xA28 */ f32 field_0xa28;
    /* 0xA2C */ f32 field_0xa2c;
    /* 0xA30 */ f32 field_0xa30;
    /* 0xA34 */ int anm;
    /* 0xA38 */ s16 timers[4];
    /* 0xA40 */ s16 counter;
    /* 0xA42 */ s16 action;
    /* 0xA44 */ s16 mode;
    /* 0xA46 */ u8 field_0xA46[0xA48 - 0xA46];
    /* 0xA48 */ s16 angleToPlayer;
    /* 0xA4A */ s16 invincibilityTimer;
    /* 0xA4C */ s16 field_0xa4c;
    /* 0xA4E */ s16 field_0xa4e;
    /* 0xA50 */ s16 field_0xa50;
    /* 0xA52 */ s16 rot_y;
    /* 0xA54 */ s16 rot_x;
    /* 0xA56 */ s16 rot_step;
    /* 0xA58 */ s16 death_rot_y;
    /* 0xA5C */ u32 smokeEmtrID;
    /* 0xA60 */ u32 field_0xa60;
    /* 0xA64 */ s8 field_0xa64;
    /* 0xA65 */ u8 field_0xa65;
    /* 0xA66 */ u8 isChild;
    /* 0xA67 */ u8 childNum;
    /* 0xA68 */ u8 appearRange;
    /* 0xA69 */ u8 field_0xa69;
    /* 0xA6A */ u8 swbit;
    /* 0xA6B */ u8 HIOInit;
};

STATIC_ASSERT(sizeof(e_rb_class) == 0xa6c);

#endif /* D_A_E_RB_H */
