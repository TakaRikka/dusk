#ifndef D_A_OBJ_FW_H
#define D_A_OBJ_FW_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class obj_fw_class
 * @brief Firewood
 *
 * @details
 *
*/

class obj_fw_class {
public:
    /* 0x000 */ fopAc_ac_c actor;
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ u8 field_0x570;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ s16 field_0x578;
    /* 0x57A */ s16 field_0x57a;
    /* 0x57C */ u8 field_0x57c[0x57e - 0x57c];
    /* 0x57E */ s16 field_0x57e[2];
    /* 0x584 */ f32 field_0x584;
    /* 0x588 */ f32 field_0x588;
    /* 0x58C */ f32 field_0x58c;
    /* 0x590 */ s16 field_0x590;
    /* 0x592 */ s16 field_0x592;
    /* 0x594 */ f32 field_0x594;
    /* 0x598 */ f32 field_0x598;
    /* 0x59C */ u8 field_0x59c[0x5a0 - 0x59c];
    /* 0x5A0 */ s16 field_0x5a0;
    /* 0x5A2 */ s16 field_0x5a2;
    /* 0x5A4 */ s16 field_0x5a4;
    /* 0x5A8 */ cXyz field_0x5a8;
    /* 0x5B4 */ u8 field_0x5b4[0x5b8 - 0x5b4];
    /* 0x5B8 */ Mtx mMtx;
    /* 0x5E8 */ dBgW* mpBgW;
    /* 0x5EC */ Z2SoundObjSimple mSound;
    /* 0x60C */ u8 field_0x60c[0x61c - 0x60c];
    /* 0x61C */ u32 field_0x61c;
    /* 0x620 */ u8 field_0x620[0x624 - 0x620];
    /* 0x624 */ u8 field_0x624;
};

STATIC_ASSERT(sizeof(obj_fw_class) == 0x628);

class daObj_Fw_HIO_c : public JORReflexible {
public:
    daObj_Fw_HIO_c();
    virtual ~daObj_Fw_HIO_c() {}

#if DEBUG
    void genMessage(JORMContext*);
#endif

    /* 流木 - Driftwood */
    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 size;     // 大きさ - Size
};


#endif /* D_A_OBJ_FW_H */
