#ifndef D_A_OBJ_SW_H
#define D_A_OBJ_SW_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class obj_sw_class
 * @brief Switch
 *
 * @details
 *
 */

struct obj_sc_s {
public:
    /* 0x00 */ s8 field_0x0;
    /* 0x01 */ u8 field_0x1[0x4 - 0x1];
    /* 0x04 */ cXyz field_0x4;
    /* 0x10 */ cXyz field_0x10;
    /* 0x1C */ cXyz field_0x1c;
    /* 0x28 */ csXyz field_0x28;
    /* 0x2E */ u8 field_0x2e[0x30 - 0x2e];
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ s8 field_0x34;
    /* 0x35 */ s8 field_0x35;
    /* 0x36 */ u8 field_0x36[0x38 - 0x36];
    /* 0x38 */ int field_0x38;
    /* 0x3C */ int field_0x3c;
    /* 0x40 */ mDoExt_McaMorf* mpModelMorf;
    /* 0x44 */ Z2SoundObjSimple mSound;
};

STATIC_ASSERT(sizeof(obj_sc_s) == 0x64);

class obj_sw_class {
public:
    /* 0x000 */ fopAc_ac_c actor;
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ u8 field_0x570;
    /* 0x571 */ u8 field_0x571;
    /* 0x572 */ u8 field_0x572[0x574 - 0x572];
    /* 0x574 */ s16 field_0x574;
    /* 0x576 */ s16 field_0x576;
    /* 0x578 */ u8 field_0x578[0x57a - 0x578];
    /* 0x57A */ s16 field_0x57a[3];
    /* 0x580 */ s16 field_0x580;
    /* 0x582 */ s16 field_0x582;
    /* 0x584 */ f32 field_0x584;
    /* 0x588 */ f32 field_0x588;
    /* 0x58C */ f32 field_0x58c;
    /* 0x590 */ f32 field_0x590;
    /* 0x594 */ s16 field_0x594;
    /* 0x596 */ s16 field_0x596;
    /* 0x598 */ cXyz field_0x598;
    /* 0x5A4 */ f32 field_0x5a4;
    /* 0x5A8 */ obj_sc_s field_0x5a8[8];
    /* 0x8C8 */ J3DModel* field_0x8c8;
    /* 0x8CC */ s16 mDemoMode;
    /* 0x8CE */ s16 field_0x8ce;
    /* 0x8D0 */ cXyz mDemoCamEye;
    /* 0x8DC */ cXyz mDemoCamCenter;
    /* 0x8E8 */ cXyz field_0x8e8;
    /* 0x8F4 */ f32 mDemoCamFovy;
    /* 0x8F8 */ u8 field_0x8f8[0x8fc - 0x8f8];
    /* 0x8FC */ f32 field_0x8fc;
    /* 0x900 */ f32 field_0x900;
    /* 0x904 */ cXyz field_0x904[2];
    /* 0x91C */ int field_0x91c;
    /* 0x920 */ cXyz field_0x920[63];
    /* 0xC14 */ f32 field_0xc14[63];
    /* 0xD10 */ s8 field_0xd10[64];
    /* 0xD50 */ mDoExt_3DlineMat1_c field_0xd50;
    /* 0xD8C */ int field_0xd8c;
};

STATIC_ASSERT(sizeof(obj_sw_class) == 0xd90);

#endif /* D_A_OBJ_SW_H */
