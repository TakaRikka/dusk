#ifndef D_A_E_BEE_H
#define D_A_E_BEE_H

#include "f_op/f_op_actor.h"
#include "SSystem/SComponent/c_phase.h"
#include "Z2AudioLib/Z2Creature.h"
#include "d/d_cc_d.h"

struct bee_s {
    enum Action {
        /* 0x0 */ ACT_DEAD,
        /* 0x1 */ ACT_FLY,
        /* 0x2 */ ACT_FLY_HOME_A,
        /* 0x3 */ ACT_FLY_HOME_B,
        /* 0x4 */ ACT_FAIL,
        /* 0x5 */ ACT_START,
        /* 0xA */ ACT_HOME = 10,
    };

    bee_s() {}
    ~bee_s() {}

    /* 0x00 */ J3DModel* mpModel1;
    /* 0x04 */ J3DModel* mpModel2;
    /* 0x08 */ J3DModel* mpModel3;
    /* 0x0C */ J3DModel* mpModel4;
    /* 0x10 */ cXyz mPos;
    /* 0x1C */ cXyz mTarget;
    /* 0x28 */ cXyz mSpeed;
    /* 0x34 */ csXyz mAngle;
    /* 0x3C */ f32 mSpeedF;
    /* 0x40 */ f32 mScale;
    /* 0x44 */ f32 mStartDistance;
    /* 0x48 */ s16 mCounter;
    /* 0x4A */ s16 mHomeTimer;
    /* 0x4C */ s8 mAction;
    /* 0x4D */ s8 mMode;
    /* 0x4E */ s8 mIsFlying;
    /* 0x4F */ u8 mTimer;
    /* 0x50 */ bool mNoDraw;
    /* 0x54 */ Z2SoundObjSimple mSound;
};

STATIC_ASSERT(sizeof(bee_s) == 0x74);

/**
 * @ingroup actors-enemies
 * @class e_bee_class
 * @brief Bee
 * 
 * @details 
 * 
 */
class e_bee_class : public fopAc_ac_c {
public:
    /* 0x0568 */ request_of_phase_process_class mPhase;
    /* 0x0570 */ u8 mParam0;
    /* 0x0571 */ u8 mParam1;
    /* 0x0572 */ u8 mParam2;
    /* 0x0574 */ Z2SoundObjBeeGroup mSound;
    /* 0x0594 */ s16 mCounter;
    /* 0x0596 */ u8 unused_0x596[4];
    /* 0x059A */ s16 mTimers[2];
    /* 0x059E */ u8 mCcSetTimer;
    /* 0x059F */ u8 mHomeTimer;
    /* 0x05A0 */ bee_s mBees[0x40];
    /* 0x22A0 */ int mNumBees;
    /* 0x22A4 */ u8 unused_0x22a4[0x30];
    /* 0x22D4 */ dCcD_Stts mCcStts;
    /* 0x2310 */ dCcD_Cyl mCcCyl;
    /* 0x244C */ dCcD_Sph mCcSph;
    /* 0x2584 */ s16 mBoomerangAngle;
    /* 0x2586 */ bool mHIOInit;
    /* 0x2587 */ bool mResLoaded;
};

STATIC_ASSERT(sizeof(e_bee_class) == 0x2588);

class daE_Bee_HIO_c {
public:
    daE_Bee_HIO_c();
    virtual ~daE_Bee_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 mScale;
    /* 0xC */ bool mDelete;
    /* 0xD */ bool mNoKill;
};

STATIC_ASSERT(sizeof(daE_Bee_HIO_c) == 0x10);

#endif /* D_A_E_BEE_H */
