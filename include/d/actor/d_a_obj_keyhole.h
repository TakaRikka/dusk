#ifndef D_A_OBJ_KEYHOLE_H
#define D_A_OBJ_KEYHOLE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

struct kh_chain_s {
    /* 0x000 */ J3DModel* model[16];
    /* 0x040 */ cXyz field_0x40;
    /* 0x04C */ cXyz field_0x4c;
    /* 0x058 */ cXyz field_0x58[16];
    /* 0x118 */ cXyz field_0x118[16];
    /* 0x1D8 */ csXyz field_0x1d8[16];
    /* 0x238 */ f32 field_0x238;
    /* 0x23C */ dCcD_Stts ccStts;
    /* 0x278 */ dCcD_Sph ccSph[3];
};

/**
 * @ingroup actors-objects
 * @class obj_keyhole_class
 * @brief Small Key Door Chains?
 *
 * @details
 *
 */
class obj_keyhole_class{
public:
    void setOpen() { open_state = 1; }
    void setOpenEnd() { open_state = 3; }
    bool checkOpenEnd() { return open_state == 3; }
    bool checkOpen() { return open_state == 1; }

    /* 0x0000 */ fopAc_ac_c actor;
    /* 0x0568 */ request_of_phase_process_class phase;
    /* 0x0570 */ char* arcname;
    /* 0x0574 */ mDoExt_McaMorf* lock_modelMorf;
    /* 0x0578 */ J3DModel* lock_model;
    /* 0x057C */ int chain_num;
    /* 0x0580 */ int field_0x580;
    /* 0x0584 */ u8 arg0;
    /* 0x0588 */ Z2Creature sound;
    /* 0x0618 */ s16 hit_timer;
    /* 0x061A */ s16 field_0x61a;
    /* 0x061C */ s16 action;
    /* 0x061E */ s16 mode;
    /* 0x0620 */ s16 timers[2];
    /* 0x0624 */ kh_chain_s chain_s[6];
    /* 0x2AE4 */ f32 field_0x2ae4;
    /* 0x2AE8 */ f32 field_0x2ae8;
    /* 0x2AEC */ s16 field_0x2aec;
    /* 0x2AF0 */ f32 field_0x2af0;
    /* 0x2AF4 */ f32 field_0x2af4;
    /* 0x2AF8 */ f32 field_0x2af8;
    /* 0x2AFC */ f32 field_0x2afc;
    /* 0x2B00 */ s16 field_0x2b00;
    /* 0x2B04 */ dCcD_Stts ccStts;
    /* 0x2B40 */ dCcD_Sph ccSph;
    /* 0x2C78 */ dCcU_AtInfo atInfo;
    /* 0x2C9C */ u32 field_0x2c9c;
    /* 0x2CA0 */ u32 field_0x2ca0;
    /* 0x2CA4 */ u8 open_state;
    /* 0x2CA5 */ u8 HIOInit;
    /* 0x2CA6 */ s8 field_0x2ca6;
    /* 0x2CA7 */ s8 hide_lock;
    /* 0x2CA8 */ cXyz field_0x2ca8;
    /* 0x2CB4 */ u8 field_0x2cb4;
};

STATIC_ASSERT(sizeof(obj_keyhole_class) == 0x2CB8);

class daObj_Keyhole_HIO_c : public JORReflexible {
public:
    daObj_Keyhole_HIO_c();
    virtual ~daObj_Keyhole_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ s8 id;
    /* 0x8 */ f32 obj_size;
};


#endif /* D_A_OBJ_KEYHOLE_H */
