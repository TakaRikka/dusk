#ifndef D_A_OBJ_ROCK_H
#define D_A_OBJ_ROCK_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor.h"

struct rock_ss {
    ~rock_ss();
    rock_ss();

    /* 0x00 */ J3DModel* mpModel;
    /* 0x04 */ cXyz mPos;
    /* 0x10 */ cXyz mScale;
    /* 0x1C */ f32 mOffsetY;
    /* 0x20 */ s16 mRot;
    /* 0x24 */ dCcD_Sph mCollider;
    /* 0x15C */ s8 mActive;
};  // Size: 0x160

STATIC_ASSERT(sizeof(rock_ss) == 0x160);

/**
 * @ingroup actors-objects
 * @class obj_rock_class
 * @brief Rock
 *
 * @details
 *
 */
class obj_rock_class : public fopAc_ac_c {
public:
    /* 0x568  */ request_of_phase_process_class mPhaseReq;
    /* 0x570  */ u8 mPatchRockNum;
    /* 0x571  */ u8 mRockSpacing;
    /* 0x572  */ u8 field_0x572; // Unused?
    /* 0x574  */ s16 mPatchInitialized;
    /* 0x578  */ rock_ss mRocks[0x10];
    /* 0x1B78 */ s32 mRockNum;
    /* 0x1B7C */ dCcD_Stts mStts;
    /* 0x1BB8 */ u8 field_0x1bb8; // Unused?
    /* 0x1BB9 */ bool mInitialized;
};

STATIC_ASSERT(sizeof(obj_rock_class) == 0x1BBC);

#endif /* D_A_OBJ_ROCK_H */
