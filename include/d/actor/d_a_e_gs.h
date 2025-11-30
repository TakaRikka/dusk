#ifndef D_A_E_GS_H
#define D_A_E_GS_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_uty.h"

/**
 * @ingroup actors-enemies
 * @class e_gs_class
 * @brief Ghost Soldier
 * 
 * @details 
 * 
 */
class e_gs_class {
public:
    /* 0x000 */ fopEn_enemy_c enemy;
    /* 0x5AC */ request_of_phase_process_class phase;
    /* 0x5B4 */ s16 mode;
    /* 0x5B6 */ u8 arg0;
    /* 0x5B7 */ u8 arg1;
    /* 0x5B8 */ u8 field_0x5B8[0x5C8 - 0x5B8];
    /* 0x5C8 */ mDoExt_McaMorfSO* model_morf;
    /* 0x5CC */ int anm;
    /* 0x5D0 */ mDoExt_invisibleModel invisible_model;
    /* 0x5D8 */ f32 opacity;
    /* 0x5DC */ Z2CreatureEnemy sound;
    /* 0x680 */ s16 lifetime;
    /* 0x682 */ s16 action;
    /* 0x684 */ f32 player_dist;
    /* 0x688 */ s16 angle_to_player;
    /* 0x68A */ s16 timers[4];
    /* 0x692 */ s16 field_0x692;
    /* 0x694 */ dCcU_AtInfo atInfo;
    /* 0x6B8 */ u8 hio_init;
};

STATIC_ASSERT(sizeof(e_gs_class) == 0x6bc);

class daE_GS_HIO_c : public JORReflexible {
public:
    daE_GS_HIO_c();
    virtual ~daE_GS_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 base_size;
    /* 0x0C */ s16 appear_time_lag;
    /* 0x10 */ f32 appear_alpha_speed;
    /* 0x14 */ f32 disappear_alpha_speed;
    /* 0x18 */ f32 react_distance;
};


#endif /* D_A_E_GS_H */
