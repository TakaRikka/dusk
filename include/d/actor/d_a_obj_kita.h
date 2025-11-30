#ifndef D_A_OBJ_KITA_H
#define D_A_OBJ_KITA_H

#include "f_op/f_op_actor_mng.h"

struct kita_s {
public:
    /* 0x00 */ J3DModel* mModel;
    /* 0x04 */ cXyz mPos;
    /* 0x10 */ cXyz field_0x10;
    /* 0x1C */ csXyz mAngle;
    /* 0x24 */ f32 mScaleX;
    /* 0x28 */ s16 field_0x28;
    /* 0x2C */ Mtx mMtx;
    /* 0x5C */ dBgW* mBgW;
};

STATIC_ASSERT(sizeof(kita_s) == 0x60);


/**
 * @ingroup actors-objects
 * @class obj_kita_class
 * @brief Wind Plank
 *
 * @details
 *
 */
class obj_kita_class {
public:
    /* 0x000 */ fopAc_ac_c mActor;
    /* 0x569 */ request_of_phase_process_class mPhase;
    /* 0x570 */ u8 field_0x570;
    /* 0x572 */ s16 field_0x572;
    /* 0x574 */ s16 field_0x574;
    /* 0x576 */ s16 field_0x576;
    /* 0x578 */ s16 field_0x578[2];
    /* 0x57C */ int mNumKitas;
    /* 0x580 */ f32 field_0x580;
    /* 0x584 */ s16 field_0x584;
    /* 0x586 */ s16 field_0x586;
    /* 0x588 */ s16 field_0x588;
    /* 0x58A */ s16 field_0x58a;
    /* 0x58C */ s16 field_0x58c;
    /* 0x58E */ s16 field_0x58e;
    /* 0x590 */ s8 field_0x590;
    /* 0x594 */ kita_s mKitas[21];
    /* 0xD74 */ bool mIsHIOOwner;
};

STATIC_ASSERT(sizeof(obj_kita_class) == 0xd78);

class daObj_Kita_HIO_c : public JORReflexible {
public:
    daObj_Kita_HIO_c();
    virtual ~daObj_Kita_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 mId;
    /* 0x08 */ f32 mModelSizeX;
    /* 0x0C */ f32 mModelSizeZ;
    /* 0x10 */ f32 mGap;
    /* 0x14 */ f32 mSwingRange;
};


#endif /* D_A_OBJ_KITA_H */
