#ifndef D_A_OBJ_TOBY_H
#define D_A_OBJ_TOBY_H

#include "f_op/f_op_actor_mng.h"

struct yuka_s {
public:
    /* 0x00 */ J3DModel* mModel1;
    /* 0x04 */ J3DModel* mModel2;
    /* 0x08 */ cXyz mPos;
    /* 0x14 */ cXyz mSpeed;
    /* 0x20 */ csXyz mAngle;
    /* 0x26 */ csXyz mAngleSpeed;
    /* 0x2C */ int field_0x2c;
    /* 0x30 */ f32 mScale;
    /* 0x34 */ s8 field_0x34;
    /* 0x36 */ s16 field_0x36;
    /* 0x38 */ u8 field_0x38;
    /* 0x39 */ s8 field_0x39;
    /* 0x3A */ s8 field_0x3a;
    /* 0x3C */ int field_0x3c;
    /* 0x40 */ Mtx mMtx;
    /* 0x70 */ dBgW* mBgW;
    /* 0x74 */ int mParticle;
};

STATIC_ASSERT(sizeof(yuka_s) == 0x78);

/**
 * @ingroup actors-objects
 * @class obj_toby_class
 * @brief Fyer (Object)
 *
 * @details
 *
 */
class obj_toby_class {
public:
    /* 0x0000 */ fopAc_ac_c mBase;
    /* 0x0568 */ request_of_phase_process_class mPhase;
    /* 0x0570 */ char DUSK_CONST* mArcName;
    /* 0x0574 */ u8 field_0x574;
    /* 0x0575 */ u8 field_0x575;
    /* 0x0576 */ u8 field_0x576;
    /* 0x0577 */ u8 field_0x577;
    /* 0x0578 */ u8 mSwBit;
    /* 0x0579 */ u8 field_0x579[0x57c - 0x579];
    /* 0x057C */ f32 field_0x57c;
    /* 0x0580 */ s16 field_0x580;
    /* 0x0582 */ s16 field_0x582;
    /* 0x0584 */ s16 field_0x584;
    /* 0x0586 */ s16 field_0x586[2];
    /* 0x058C */ yuka_s mYukas[200];
    /* 0x634C */ int mYukaNum;
    /* 0x6350 */ s16 field_0x6350;
    /* 0x6352 */ s16 field_0x6352;
    /* 0x6354 */ cXyz field_0x6354;
    /* 0x6360 */ cXyz field_0x6360;
    /* 0x636C */ f32 field_0x636c;
    /* 0x6370 */ u8 field_0x6370;
    /* 0x6371 */ u8 field_0x6371;
};

STATIC_ASSERT(sizeof(obj_toby_class) == 0x6374);

#endif /* D_A_OBJ_TOBY_H */
