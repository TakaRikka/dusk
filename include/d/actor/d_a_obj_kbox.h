#ifndef D_A_OBJ_KBOX_H
#define D_A_OBJ_KBOX_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class obj_kbox_class
 * @brief (Kakariko?) Crate
 *
 * @details
 *
 */
class obj_kbox_class {
public:
    /* 0x000 */ fopAc_ac_c mActor;
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ u8 field_0x570;
    /* 0x574 */ J3DModel* mModel;
    /* 0x578 */ s16 field_0x578;
    /* 0x57A */ s16 field_0x57a;
    /* 0x57C */ s16 field_0x57c;
    /* 0x57E */ s16 field_0x57e[2];
    /* 0x584 */ u32 mShadowId;
    /* 0x588 */ f32 field_0x588;
    /* 0x58C */ f32 field_0x58c;
    /* 0x590 */ f32 field_0x590;
    /* 0x594 */ f32 field_0x594;
    /* 0x598 */ f32 field_0x598;
    /* 0x59C */ f32 field_0x59c;
    /* 0x5A0 */ s16 field_0x5a0;
    /* 0x5A4 */ f32 field_0x5a4;
    /* 0x5A8 */ f32 field_0x5a8;
    /* 0x5AC */ f32 field_0x5ac;
    /* 0x5B0 */ s16 field_0x5b0;
    /* 0x5B2 */ s16 field_0x5b2;
    /* 0x5B4 */ cXyz field_0x5b4;
    /* 0x5C0 */ dBgS_AcchCir mAcchCir;
    /* 0x600 */ dBgS_ObjAcch mAcch;
    /* 0x7D8 */ dCcD_Stts mStts;
    /* 0x814 */ dCcD_Sph mSph;
    /* 0x94C */ dCcU_AtInfo mAtInfo;
    /* 0x970 */ int field_0x970;
    /* 0x974 */ Mtx mBgMtx;
    /* 0x9A4 */ dBgW* mBgw;
    /* 0x9A8 */ Z2SoundObjSimple mSound;
    /* 0x9C8 */ u8 field_0x9a8[0x9d0 - 0x9c8];
    /* 0x9D0 */ u32 field_0x9d0[4];
    /* 0x9E0 */ u32 field_0x9e0;
    /* 0x9E4 */ u32 field_0x9e4;
    /* 0x9E8 */ u32 field_0x9e8;
    /* 0x9EC */ bool mIsHIOOwner;
};

STATIC_ASSERT(sizeof(obj_kbox_class) == 0x9f0);

class daObj_Kbox_HIO_c : public JORReflexible {
public:
    daObj_Kbox_HIO_c();
    virtual ~daObj_Kbox_HIO_c() {}
    void genMessage(JORMContext*);

    /* 0x04 */ s8 mId;
    /* 0x08 */ f32 mWidth;

};


#endif /* D_A_OBJ_KBOX_H */
