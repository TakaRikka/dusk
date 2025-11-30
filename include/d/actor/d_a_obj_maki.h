#ifndef D_A_OBJ_MAKI_H
#define D_A_OBJ_MAKI_H

#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "SSystem/SComponent/c_phase.h"
#include "d/d_cc_d.h"
#include "d/d_kankyo.h"
#include "d/d_particle.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-objects
 * @class obj_maki_class
 * @brief Stick Bundle?
 *
 * @details
 *
 */
class obj_maki_class : public fopAc_ac_c {
public:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mModel1;
    /* 0x574 */ mDoExt_brkAnm* mBrk;
    /* 0x578 */ J3DModel* mModel2;
    /* 0x57c */ u8 field_0x57c;
    /* 0x57d */ u8 field_0x57d; // Undefined
    /* 0x57e */ s16 field_0x57e;
    /* 0x580 */ s16 field_0x580[2];
    /* 0x584*/  dCcD_Stts mStts;
    /* 0x5c0 */ dCcD_Sph mSph;
    /* 0x6f8 */ u32 field_0x6f8[4];
    /* 0x6fc */ u8 field_0x6fc[0x70c - 0x708]; // Undefined
    /* 0x70c */ LIGHT_INFLUENCE mLightObj;
    /* 0x72c */ u8 field_0x72c;
    /* 0x72d */ u8 field_0x72d[0x730 - 0x72d]; //Undefined
};

STATIC_ASSERT(sizeof(obj_maki_class) == 0x730);
class daObj_Maki_HIO_c {
public:
    daObj_Maki_HIO_c();
    virtual ~daObj_Maki_HIO_c() {};

    /* 0x04 */ s8 field_0x4;
};


#endif /* D_A_OBJ_MAKI_H */
