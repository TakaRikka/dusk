#ifndef D_A_E_SG_H
#define D_A_E_SG_H

#include "f_op/f_op_actor.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_jnt_col.h"

/**
 * @ingroup actors-enemies
 * @class e_sg_class
 * @brief Skullfish
 *
 * @details 
 * Skullfish are found in the Lakebed Temple and some caves. They attack by
 * chasing and biting Link, often appearing in groups.
 * Skullfish can also be caught with the Fishing Rod, leaving behind a Heart
 * when released. It will then flop on land.
 * */
class e_sg_class : public fopEn_enemy_c {
public:
    /* 0x5AC */ request_of_phase_process_class mPhaseReq;
    /* 0x5B4 */ u8 mArg0;
    /* 0x5B8 */ J3DModel* mpModel;
    /* 0x5BC */ Z2CreatureEnemy mSound;
    /* 0x660 */ s16 mTimers[3];
    /* 0x666 */ s16 mInvincibilityTimer;
    /* 0x668 */ u8 mInitTimer;
    /* 0x66A */ s16 mRandomSeed;
    /* 0x66C */ s16 mAction;
    /* 0x66E */ s16 mMode;
    /* 0x670 */ f32 mIdleMoveBound;
    /* 0x674 */ f32 mSearchBound;
    /* 0x678 */ cXyz mTargetPos;
    /* 0x684 */ u8 field_0x684[4];
    /* 0x688 */ s16 mPlayerAngle;
    /* 0x68C */ f32 mPlayerDist;
    /* 0x690 */ f32 mTargetDist;
    /* 0x694 */ f32 mTargetSpeed;
    /* 0x698 */ f32 mStepSpeed;
    /* 0x69C */ s16 mSwimAngle;
    /* 0x69E */ s16 mSwimAngleSpeed;
    /* 0x6A0 */ s16 mJoints[4];
    /* 0x6A8 */ f32 mJointYRot;
    /* 0x6AC */ f32 mJointSpeed;
    /* 0x6B0 */ s16 mJointAngle;
    /* 0x6B4 */ f32 field_0x6b4;
    /* 0x6B8 */ f32 mGroundY;
    /* 0x6BC */ f32 mKamuSpeed;
    /* 0x6C0 */ s16 mRotation;
    /* 0x6C2 */ s16 mRotationTarget;
    /* 0x6C4 */ s8 mStickIdx;
    /* 0x6C5 */ u8 mKamuTimer;
    /* 0x6C6 */ s16 field_0x6C6;
    /* 0x6C8 */ int mShadowKey;
    /* 0x6CC */ fpc_ProcID mTargetActorID;
    /* 0x6D0 */ dJntCol_c mJoint;
    /* 0x6E0 */ dBgS_AcchCir mAcchCir;
    /* 0x720 */ dBgS_ObjAcch mAcch;
    /* 0x8F8 */ dCcD_Stts mStts;
    /* 0x934 */ dCcD_Sph mSph;
    /* 0xA6C */ dCcU_AtInfo mAtInfo;
    /* 0xA90 */ s8 mAttackCollistion;
    /* 0xA91 */ s8 mCollisionResponse;
    /* 0xA92 */ u8 mInactive;
    /* 0xA93 */ s8 mWaterSplash;
    /* 0xA94 */ int mParticleKey[4];
    /* 0xAA4 */ u32 mHamon;
    /* 0xAA8 */ u8 field_0xaa8[4];
    /* 0xAAC */ bool mHioInit;
};

STATIC_ASSERT(sizeof(e_sg_class) == 0xAB0);

class daE_SG_HIO_c {
public:
    daE_SG_HIO_c();
    virtual ~daE_SG_HIO_c() {};

    s8 mUnk0;
    f32 mScaleFactor;
    f32 mMovementSpeed;
    f32 mSearchSpeed;
    f32 mPlayerHeightThreshold;
};

STATIC_ASSERT(sizeof(daE_SG_HIO_c) == 0x18);

#endif /* D_A_E_SG_H */
