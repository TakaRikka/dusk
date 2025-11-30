#ifndef D_A_OBJ_FOOD_H
#define D_A_OBJ_FOOD_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-objects
 * @class obj_food_class
 * @brief Dog Bone
 *
 * @details
 *
 */
class obj_food_class : public fopAc_ac_c {
public:
    enum Type {
        /* 0 */ TYPE_BONE,
        /* 1 */ TYPE_BALL,
    };

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ u8 mType;
    /* 0x571 */ u8 mParam1;
    /* 0x572 */ u8 mParam2;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ u32 mShadowKey;
    /* 0x57C */ s16 mCounter;
    /* 0x57E */ s16 mBounces;
    /* 0x580 */ s16 mTimer[4];
    /* 0x588 */ csXyz mRotSpeed;
    /* 0x58E */ s16 mGroundAngleX;
    /* 0x590 */ s16 mGroundAngleZ;
    /* 0x594 */ f32 mOldSpeedY;
    /* 0x598 */ Z2SoundObjSimple mSound;
    /* 0x5B8 */ u32 mParticleKey[4];
    /* 0x5C8 */ dBgS_AcchCir mAcchCir;
    /* 0x608 */ dBgS_ObjAcch mAcch;
    /* 0x7E0 */ dCcD_Stts mCcStts;
    /* 0x81C */ dCcD_Sph mCcSph;
    /* 0x954 */ dCcD_Sph mAtSph;
    /* 0xA8C */ bool mHIOInit;
};

STATIC_ASSERT(sizeof(obj_food_class) == 0xA90);

struct daObj_Food_HIO_c {
    daObj_Food_HIO_c();
    virtual ~daObj_Food_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 mBoneScale;
};

STATIC_ASSERT(sizeof(daObj_Food_HIO_c) == 0xC);

#endif /* D_A_OBJ_FOOD_H */
