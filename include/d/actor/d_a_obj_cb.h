#ifndef D_A_OBJ_CB_H
#define D_A_OBJ_CB_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class obj_cb_class
 * @brief Castle Block?
 *
 * @details
 *
 */
class obj_cb_class : public fopAc_ac_c {
public:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mModel;
    /* 0x574 */ Z2SoundObjSimple mSoundObj;
    /* 0x594 */ s16 field_0x594;
    /* 0x596 */ s16 field_0x596;
    /* 0x598 */ s16 field_0x598[2];
    /* 0x59C */ s16 field_0x59c;
    /* 0x59E */ s16 field_0x59e;
    /* 0x5A0 */ csXyz field_0x5a0;
    /* 0x5A8 */ f32 field_0x5a8;
    /* 0x5AC */ dBgS_AcchCir mAcchCir;
    /* 0x5EC */ dBgS_ObjAcch mAcch;
    /* 0x7C4 */ int field_0x7c4;
    /* 0x7C8 */ Mtx mBgMtx;
    /* 0x7F8 */ dBgW* mBgW;
    /* 0x7FC */ dCcD_Stts mStts;
    /* 0x838 */ dCcD_Sph mSph;
    /* 0x970 */ u32 field_0x970;
    /* 0x974 */ u32 field_0x974;
    /* 0x978 */ u8 field_0x978;
};

STATIC_ASSERT(sizeof(obj_cb_class) == 0x97c);

class daObj_Cb_HIO_c {
public:
    daObj_Cb_HIO_c();
    virtual ~daObj_Cb_HIO_c() {}

    s8 field_0x4;
};


#endif /* D_A_OBJ_CB_H */
