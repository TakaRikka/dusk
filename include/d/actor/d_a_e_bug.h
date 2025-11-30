#ifndef D_A_E_BUG_H
#define D_A_E_BUG_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class e_bug_class
 * @brief Poison Mite
 * 
 * @details 
 * 
 */

struct bug_s {
    /* 0x00 */ J3DModel* field_0x0;
    /* 0x04 */ J3DModel* field_0x4;
    /* 0x08 */ int field_0x8;
    /* 0x0C */ cXyz field_0xc;
    /* 0x18 */ cXyz field_0x18;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ cXyz field_0x30;
    /* 0x3C */ csXyz field_0x3c;
    /* 0x42 */ s16 field_0x42;
    /* 0x44 */ s16 field_0x44;
    /* 0x46 */ s16 field_0x46;
    /* 0x48 */ s16 field_0x48;
    /* 0x4A */ s16 field_0x4a;
    /* 0x4C */ s16 field_0x4c;
    /* 0x4E */ s16 field_0x4e;
    /* 0x50 */ s8 field_0x50;
    /* 0x51 */ s8 field_0x51;
    /* 0x52 */ s8 field_0x52;
    /* 0x53 */ u8 field_0x53;
    /* 0x54 */ u8 field_0x54[3];
    /* 0x58 */ Z2SoundObjSimple mSound;
};

STATIC_ASSERT(sizeof(bug_s) == 0x78);
class e_bug_class {
public:
    /* 0x0000 */ fopAc_ac_c actor;
    /* 0x0568 */ request_of_phase_process_class mPhase;
    /* 0x0570 */ u8 field_0x570;
    /* 0x0574 */ int bitSw;
    /* 0x0578 */ u32 field_0x578;
    /* 0x057C */ f32 field_0x57c;
    /* 0x0580 */ s16 field_0x580;
    /* 0x0582 */ u8 field_0x582[0x588 - 0x582];
    /* 0x0588 */ bug_s Bug_s[256];
    /* 0x7D88 */ int bug_num;
    /* 0x7D8C */ Z2SoundObjBeeGroup mSound;
    /* 0x7DAC */ u8 field_0x7dac;
    /* 0x7DAD */ u8 field_0x7dad;
};

STATIC_ASSERT(sizeof(e_bug_class) == 0x7db0);

class daE_Bug_HIO_c: public JORReflexible {
public:
    daE_Bug_HIO_c();
    virtual ~daE_Bug_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 field_0x8;
    /* 0xC */ f32 field_0xc;
};

#endif /* D_A_E_BUG_H */
