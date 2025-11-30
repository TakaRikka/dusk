#ifndef D_A_OBJ_LBOX_H
#define D_A_OBJ_LBOX_H

#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor.h"
#include "d/d_bg_w.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class obj_lbox_class
 * @brief L - Box (Large Box?)
 *
 * @details
 *
 */
class obj_lbox_class : public fopAc_ac_c {
public:
    /* 0x568 */ request_of_phase_process_class mPhaseReq;
    /* 0x570 */ u8 mParam;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ s16 field_0x578;
    /* 0x57A */ s16 field_0x57a;
    /* 0x57C */ s16 field_0x57c;
    /* 0x57E */ s16 field_0x57e[2];
    /* 0x584 */ u32 field_0x584;
    /* 0x588 */ dBgS_AcchCir mAcchCir;
    /* 0x5C8 */ dBgS_ObjAcch mObjAcch;
    /* 0x7A0 */ dCcD_Stts mStts;
    /* 0x7DC */ dCcD_Sph mSph;
    /* 0x914 */ dCcD_Sph mSph2;
    /* 0xA4C */ u32 field_0xa4c;
    /* 0xA50 */ Mtx mMtx;
    /* 0xA80 */ dBgW* mpBgW;
    /* 0xA84 */ u32 field_0xa84;
    /* 0xA88 */ u32 field_0xa88;
    /* 0xA8C */ u8 field_0xa8c;
};

STATIC_ASSERT(sizeof(obj_lbox_class) == 0xA90);

class daObj_Lbox_HIO_c {
public:
    daObj_Lbox_HIO_c();
    virtual ~daObj_Lbox_HIO_c() {};

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 mSize;
    /* 0xC */ f32 mHeight;
};  // Size = 0x10

#endif /* D_A_OBJ_LBOX_H */
