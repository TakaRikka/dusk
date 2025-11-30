#ifndef D_A_OBJ_ITO_H
#define D_A_OBJ_ITO_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class obj_ito_class
 * @brief ???
 *
 * @details
 *
*/

struct ito_s {
    /* 0x000 */ mDoExt_3DlineMat0_c mLineMat;
    /* 0x01C */ f32 field_0x1c[6][20];
    /* 0x1FC */ cXyz field_0x1fc[6];
    /* 0x244 */ s16 field_0x244[6];
    /* 0x250 */ f32 field_0x250;
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ f32 field_0x258;
};

class obj_ito_class {
public:
    /* 0x0000 */ fopAc_ac_c actor;
    /* 0x0568 */ request_of_phase_process_class mPhase;
    /* 0x0570 */ u8 field_0x570;
    /* 0x0571 */ u8 field_0x571;
    /* 0x0574 */ J3DModel* field_0x574[2];
    /* 0x057C */ Z2Creature mSound;
    /* 0x060C */ s16 field_0x60c;
    /* 0x060E */ u8 field_0x60e[0x610 - 0x60e];
    /* 0x0610 */ s16 field_0x610;
    /* 0x0612 */ s16 field_0x612[2];
    /* 0x0618 */ cXyz field_0x618;
    /* 0x0624 */ cXyz field_0x624;
    /* 0x0630 */ f32 field_0x630;
    /* 0x0634 */ f32 field_0x634;
    /* 0x0638 */ s8 field_0x638;
    /* 0x0639 */ s8 field_0x639;
    /* 0x063A */ s16 field_0x63a;
    /* 0x063C */ dCcD_Stts mStts;
    /* 0x0678 */ dCcD_Sph field_0x678[8];
    /* 0x1038 */ s8 field_0x1038;
    /* 0x103C */ ito_s field_0x103c;
    /* 0x1298 */ ito_s field_0x1298;
    /* 0x14F4 */ u32 field_0x14f4;
    /* 0x14F8 */ cXyz field_0x14f8;
    /* 0x1504 */ f32 field_0x1504;
    /* 0x1508 */ s8 field_0x1508;
    /* 0x1509 */ s8 field_0x1509;
    /* 0x150A */ s8 field_0x150a;
    /* 0x150B */ u8 field_0x150b;
};

STATIC_ASSERT(sizeof(obj_ito_class) == 0x150c);

class daObj_Ito_HIO_c {
public:
    daObj_Ito_HIO_c();
    virtual ~daObj_Ito_HIO_c() {}

    /* ぶら下りＯＢＪ糸 */
    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 field_0x8;   // 糸長さ基準値 - Thread Length Reference Value
    /* 0x0C */ f32 field_0xc;   // 糸位置補正値 - Thread Position Correction Value
    /* 0x10 */ s16 field_0x10;
};


#endif /* D_A_OBJ_ITO_H */
