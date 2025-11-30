#ifndef D_A_E_IS_H
#define D_A_E_IS_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

/**
 * @ingroup actors-enemies
 * @class e_is_class
 * @brief Armos Titan (Idelia Statue)
 * 
 * @details Armos Titans are enemies from early, unreleased versions of the game. 
 * The name Idelia Statue seems like it might be a reference to the anime Powered Armor Dorvack.
 * 
 */
class e_is_class {
public:
    /* 0x000 */ fopEn_enemy_c enemy;
    /* 0x5AC */ request_of_phase_process_class phase;
    /* 0x5B4 */ u8 field_0x5b4;
    /* 0x5B8 */ u32 swbit;
    /* 0x5BC */ u32 swbit2;
    /* 0x5C0 */ u32 swbit3;
    /* 0x5C4 */ u32 field_0x5c4;
    /* 0x5C8 */ mDoExt_McaMorfSO* model_morf;
    /* 0x5CC */ Z2CreatureEnemy sound;
    /* 0x670 */ int anm;
    /* 0x674 */ f32 field_0x674;
    /* 0x678 */ f32 poweron_color_strength_target;
    /* 0x67C */ f32 poweron_color_strength;
    /* 0x680 */ f32 core_color_strength_target;
    /* 0x684 */ f32 core_color_strength;
    /* 0x688 */ s8 field_0x688;
    /* 0x68C */ u32 shadowId;
    /* 0x690 */ s16 lifetime;
    /* 0x692 */ s16 action;
    /* 0x694 */ s16 mode;
    /* 0x696 */ u8 field_0x696[0x6A6 - 0x696];
    /* 0x6A6 */ s16 angle_to_player;
    /* 0x6A8 */ f32 player_distance;
    /* 0x6AC */ f32 field_0x6ac;
    /* 0x6B0 */ s16 timers[4];
    /* 0x6B8 */ s16 body_hit_invulnerability_timer;
    /* 0x6BA */ s16 core_hit_invulnerability_timer;
    /* 0x6BC */ s8 field_0x6bc;
    /* 0x6BD */ s8 field_0x6bd;
    /* 0x6C0 */ int swbit4;
    /* 0x6C4 */ dBgS_AcchCir acchcir;
    /* 0x704 */ dBgS_ObjAcch acch;
    /* 0x8DC */ dCcD_Stts ccStts;
    /* 0x918 */ dCcD_Sph at_sph;
    /* 0xA50 */ dCcD_Sph core_sph;
    /* 0xB88 */ dCcD_Cyl cc_cyl;
    /* 0xCC4 */ dCcU_AtInfo atInfo;
    /* 0xCE8 */ s8 field_0xce8;
    /* 0xCE9 */ u8 field_0xCE9[0xCF4 - 0xCE9];
    /* 0xCF4 */ u8 hioInit;
};

STATIC_ASSERT(sizeof(e_is_class) == 0xcf8);

class daE_IS_HIO_c : public JORReflexible {
public:
    daE_IS_HIO_c();
    virtual ~daE_IS_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 base_size;
    /* 0x0C */ f32 normal_speed;
    /* 0x10 */ f32 attack_init_range;
    /* 0x14 */ f32 search_dist;
    /* 0x18 */ f32 hit_width;
};


#endif /* D_A_E_IS_H */
