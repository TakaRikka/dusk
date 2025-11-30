#ifndef D_A_OBJ_LP_H
#define D_A_OBJ_LP_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class obj_lp_class
 * @brief Lily Pad
 *
 * @details
 *
 */
 
struct wd_ss {
public:
    ~wd_ss() {}
    wd_ss() {}
    
    /* 0x00 */ J3DModel* mpModel;
    /* 0x04 */ cXyz field_0x4;
    /* 0x10 */ cXyz field_0x10;
    /* 0x1C */ cXyz field_0x1c;
    /* 0x28 */ cXyz field_0x28;
    /* 0x34 */ s16 field_0x34;
    /* 0x36 */ s16 field_0x36;
    /* 0x38 */ s16 field_0x38;
    /* 0x3A */ u8 field_0x3a[0x3c - 0x3a];
    /* 0x3C */ f32 field_0x3c;
    /* 0x40 */ s16 field_0x40;
    /* 0x42 */ s16 field_0x42;
    /* 0x44 */ s16 field_0x44;
    /* 0x46 */ u8 field_0x46[0x4e - 0x46];
    /* 0x4E */ s8 field_0x4e;
    /* 0x4F */ u8 field_0x4f;
    /* 0x50 */ int field_0x50;
};

STATIC_ASSERT(sizeof(wd_ss) == 0x54);

class obj_lp_class {
public:
    /* 0x0000 */ fopAc_ac_c mActor;
    /* 0x0568 */ request_of_phase_process_class mPhase;
    /* 0x0570 */ u8 field_0x570;
    /* 0x0571 */ u8 field_0x571;
    /* 0x0572 */ u8 field_0x572;
    /* 0x0573 */ u8 field_0x573;
    /* 0x0574 */ s16 field_0x574;
    /* 0x0576 */ u8 field_0x576[0x57c - 0x576];
    /* 0x057C */ wd_ss mWdSs[512];
    /* 0xAD7C */ mDoExt_3DlineMat0_c field_0xad7c;
    /* 0xAD98 */ int field_0xad98;
    /* 0xAD9C */ cXyz field_0xad9c;
    /* 0xADA8 */ f32 field_0xada8;
    /* 0xADAC */ f32 field_0xadac;
    /* 0xADB0 */ u8 field_0xadb0;
    /* 0xADB1 */ u8 field_0xadb1;
    /* 0xADB2 */ u8 field_0xadb2;
    /* 0xADB3 */ u8 field_0xadb3;
    /* 0xADB4 */ u8 field_0xadb4;
    /* 0xADB5 */ u8 field_0xadb5[0xadb8 - 0xadb5];
};

STATIC_ASSERT(sizeof(obj_lp_class) == 0xadb8);


#endif /* D_A_OBJ_LP_H */
