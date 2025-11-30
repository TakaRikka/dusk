#ifndef D_A_E_GA_H
#define D_A_E_GA_H

#include "f_op/f_op_actor_mng.h"

struct ga_s {
    /* 0x00 */ J3DModel* model;
    /* 0x04 */ cXyz position;
    /* 0x10 */ cXyz pos_target;
    /* 0x1C */ cXyz field_0x1c;
    /* 0x28 */ csXyz rotation;
    /* 0x2E */ csXyz rot_target;
    /* 0x34 */ u8 field_0x34[0x38 - 0x34];
    /* 0x38 */ f32 size;
    /* 0x3C */ f32 field_0x3c;
    /* 0x40 */ s8 initialized;
    /* 0x41 */ u8 field_0x41[0x44 - 0x41];
    /* 0x44 */ s16 field_0x44;
    /* 0x46 */ u8 move_timer;
};

/**
 * @ingroup actors-enemies
 * @class e_ga_class
 * @brief Decorative Moth
 * 
 * @details 
 * 
 */
class e_ga_class {
public:
    /* 0x0000 */ fopAc_ac_c actor;
    /* 0x0568 */ request_of_phase_process_class phase;
    /* 0x0570 */ u8 prm0;
    /* 0x0571 */ u8 prm1;
    /* 0x0572 */ u8 prm2;
    /* 0x0574 */ ga_s ga_s[256];
    /* 0x4D74 */ int ga_num;
    /* 0x4D78 */ u8 HIOInit;
    /* 0x4D79 */ u8 initialized;
};

STATIC_ASSERT(sizeof(e_ga_class) == 0x4d7c);

class daE_Ga_HIO_c {
public:
    daE_Ga_HIO_c();
    virtual ~daE_Ga_HIO_c() {}

    /* 0x4 */ s8 id;
    /* 0x8 */ f32 base_size;
};


#endif /* D_A_E_GA_H */
