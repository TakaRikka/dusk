#ifndef D_A_E_NEST_H
#define D_A_E_NEST_H

#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_bg_s_acch.h"
#include "Z2AudioLib/Z2Creature.h"

struct nest_hahen_s {
    nest_hahen_s() {}
    ~nest_hahen_s() {}

    /* 0x00 */ J3DModel* mpModel;
    /* 0x04 */ cXyz mPos;
    /* 0x10 */ cXyz mLastPos;
    /* 0x1C */ cXyz mSpeed;
    /* 0x28 */ cXyz mScale;
    /* 0x34 */ s16 mAngleZ;
    /* 0x36 */ s16 mAngleY;
    /* 0x38 */ u8 unused_0x38[2];
    /* 0x3A */ s16 mGroundAngleX;
    /* 0x3C */ s16 mGroundAngleZ;
    /* 0x3E */ u8 unused_0x3e[6];
    /* 0x44 */ s16 mCounter;
    /* 0x46 */ u8 unused_0x46[0xa];
    /* 0x50 */ s16 mMaxRotationZ;
    /* 0x52 */ s8 mGroundHitStage;
    /* 0x53 */ bool mActive;
    /* 0x54 */ u8 mTimer;
};

STATIC_ASSERT(sizeof(nest_hahen_s) == 0x58);

/**
 * @ingroup actors-enemies
 * @class e_nest_class
 * @brief Beehive
 * 
 * @details 
 * 
 */
class e_nest_class : public fopAc_ac_c {
public:
    enum Type {
        TYPE_BEE = 1,
        TYPE_BUG = 2,
    };

    enum Action {
        ACT_NORMAL = 0,
        ACT_DROP = 1,
        ACT_HOOK = 2,
        ACT_CARRY = 3,
        ACT_FLOAT = 4,
        ACT_DEBRIS = 5,
    };

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ u8 mParamType;
    /* 0x571 */ u8 mParam1;
    /* 0x572 */ u8 mParamScale;
    /* 0x573 */ u8 mBrokenSwitch;
    /* 0x574 */ u8 unused_0x574[4];
    /* 0x578 */ J3DModel* mpModel;
    /* 0x57C */ s16 mFrame;
    /* 0x57E */ s16 mAction;
    /* 0x580 */ s16 mMode;
    /* 0x582 */ s16 mTimers[3];
    /* 0x588 */ s16 mHitTimer;
    /* 0x58A */ s8 mKnockDown;
    /* 0x58C */ f32 mCarryOffset;
    /* 0x590 */ f32 mDropOffset;
    /* 0x594 */ f32 mWaterHeight;
    /* 0x598 */ s16 mCarryAngle;
    /* 0x59A */ s16 mCarryAngleSpeed;
    /* 0x59C */ u8 unused_0x59c[4];
    /* 0x5A0 */ u32 mShadowKey;
    /* 0x5A4 */ cXyz mCenterPos;
    /* 0x5B0 */ s8 mIframes;
    /* 0x5B4 */ dBgS_AcchCir mAcchCir;
    /* 0x5F4 */ dBgS_ObjAcch mAcch;
    /* 0x7CC */ dCcD_Stts mCcStts;
    /* 0x808 */ dCcD_Sph mCcSph;
    /* 0x940 */ dCcD_Cyl mCcCyl;
    /* 0xA7C */ dCcU_AtInfo mAtInfo;
    /* 0xAA0 */ bool mHawkHit;
    /* 0xAA4 */ int mHitActorID;
    /* 0xAA8 */ f32 mBob;
    /* 0xAAC */ csXyz mRotation;
    /* 0xAB2 */ s16 mSpin;
    /* 0xAB4 */ f32 mWobble;
    /* 0xAB8 */ u32 mSmokeKey1;
    /* 0xABC */ u32 mSmokeKey2;
    /* 0xAC0 */ u32 mParticleKey[4];
    /* 0xAD0 */ u32 mRippleKey;
    /* 0xAD4 */ u8 unused_0xad4[4];
    /* 0xAD8 */ Z2SoundObjBeeGroup mBeeSound;
    /* 0xAF8 */ Z2SoundObjSimple mSound;
    /* 0xB18 */ s8 mBreakStage;
    /* 0xB1C */ nest_hahen_s mDebris[5];
    /* 0xCD4 */ s16 mDemoStage;
    /* 0xCD6 */ s16 mDemoTimer;
    /* 0xCD8 */ cXyz mDemoCameraEye;
    /* 0xCE4 */ cXyz mDemoCameraCenter;
    /* 0xCF0 */ f32 mDemoCameraFovY;
    /* 0xCF4 */ bool mHIOInit;
};

STATIC_ASSERT(sizeof(e_nest_class) == 0xCF8);

class daE_Nest_HIO_c {
public:
    daE_Nest_HIO_c();
    virtual ~daE_Nest_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 mScale;
};

STATIC_ASSERT(sizeof(daE_Nest_HIO_c) == 0xC);

#endif /* D_A_E_NEST_H */
