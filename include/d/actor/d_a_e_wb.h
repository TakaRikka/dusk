#ifndef D_A_E_WB_H
#define D_A_E_WB_H

#include "f_op/f_op_actor_mng.h"
#include "d/actor/d_a_horse.h"
#include "SSystem/SComponent/c_phase.h"
#include "d/d_cc_uty.h"

// copypasta, fix when actual names known
enum daE_WB_ANM {
    ANM_APPEAR = 7,
    ANM_ATTACK,
    ANM_ATTACK_A,
    ANM_ATTACK_B,
    ANM_ATTACK_C,
    ANM_BOMB_DAMAGE,
    ANM_CORE_DAMAGE,
    ANM_DAMAGE_WAIT,
    ANM_DEAD,
    ANM_LOOK_M,
    ANM_NO_DAMAGE,
    ANM_RETURN_01,
    ANM_RETURN_02,
    ANM_RUNAWAY,
    ANM_TEST_MOTION,
    ANM_TODOME,
    ANM_WAIT_01,
};

enum daE_WB_ACT {
    /* 0x00 */ ACT_WAIT,
    /* 0x01 */ ACT_WAIT2,
    /* 0x04 */ ACT_RIDE = 0x04,
    /* 0x05 */ ACT_F_WAIT,
    /* 0x06 */ ACT_C_F_RUN,
    /* 0x07 */ ACT_A_RUN,
    /* 0x08 */ ACT_B_WAIT,
    /* 0x09 */ ACT_B_WAIT2,
    /* 0x0A */ ACT_B_RUN,
    /* 0x0B */ ACT_B_RUN2,
    /* 0x0F */ ACT_B_IKKI = 0x0F,
    /* 0x10 */ ACT_B_IKKI_END,
    /* 0x11 */ ACT_B_IKKI2,
    /* 0x12 */ ACT_B_IKKI2_END,
    /* 0x13 */ ACT_B_LV9_END,
    /* 0x15 */ ACT_S_DAMAGE = 0x15,
    /* 0x16 */ ACT_DAMAGE,
    /* 0x17 */ ACT_BG_DAMAGE,
    /* 0x18 */ ACT_LR_DAMAGE,
    /* 0x1E */ ACT_KIBA_START = 0x1E,
    /* 0x1F */ ACT_KIBA_END,
    /* 0x32 */ ACT_EVENT = 0x32,
    /* 0x65 */ ACT_PL_RIDE = 0x65,
    /* 0x66 */ ACT_PL_RIDE2,
    /* 0x67 */ ACT_PL_RIDE_NOW,
    /* 0x6E */ ACT_CRV_WAIT = 0x6E,
};

struct himo_s {
    /* 0x0 */ cXyz field_0x0[16];
};


/**
 * @ingroup actors-enemies
 * @class e_wb_class
 * @brief Bullbo (Wild Boar)
 *
 * @details
 *
 */
class e_wb_class {
public:
    BOOL checkWait();
    void setPlayerRideNow();
    void setPlayerRide();
    void getOff();
    BOOL checkDownDamage();
    BOOL checkNormalRideMode() const;
    void setRunRideMode();

    MtxP getRideMtx() { return mpModelMorf->getModel()->getAnmMtx(15); }
    f32 nowAnimeFrame() const { return mpModelMorf->getFrame(); }
    s16 getWaitRollAngle() const { return mWaitRollAngle; }
    BOOL checkGetOff() const { return mEnemy.speedF < 3.0f; }
    f32 rideSpeedRate() const { return mEnemy.speedF / mSpeedRate; }
    f32 getAnimeFrameRate() { return mpModelMorf->getFrame() / mpModelMorf->getEndFrame(); }
    BOOL checkAnmLoopFrame() { return mpModelMorf->checkFrame(0.0f); }

    J3DModel* getModel() {
        if (mpModelMorf != NULL) {
            return mpModelMorf->getModel();
        }

        return NULL;
    }

public:
    /* 0x0000 */ fopEn_enemy_c mEnemy;                   ///< @brief Base enemy class instance.
    /* 0x05AC */ request_of_phase_process_class mPhase;  ///< @brief Phase process request handler.
    /* 0x05B4 */ s16 mActionMode;  ///< @brief Current sub-action mode within the run action.
    /* 0x05B8 */ char* mResName;   ///< @brief Resource name for loading models/animations.
    /* 0x05BC */ u8 mParam;        ///< @brief Creation parameter 1.
    /* 0x05BD */ u8 mParam2;       ///< @brief Creation parameter 2.
    /* 0x05BE */ u8 field_0x5be;
    /* 0x05BF */ bool field_0x5bf;
    /* 0x05C0 */ u8 field_0x5c0;
    /* 0x05C4 */ cXyz field_0x5c4;
    /* 0x05D0 */ cXyz field_0x5d0;
    /* 0x05DC */ s16 mTargetFacingAngle;  ///< @brief Target facing angle for turning towards path point.
    /* 0x05DE */ s16 field_0x5de;
    /* 0x05E0 */ mDoExt_McaMorfSO* mpModelMorf;  ///< @brief Pointer to model morph animator.
    /* 0x05E4 */ f32 mPlaySpeed;                 ///< @brief Animation playback speed.
    /* 0x05E8 */ int mAnmID;                     ///< @brief Current animation ID.
    /* 0x05EC */ Z2CreatureRide mZ2Ride;         ///< @brief Z2 sound creature for ride effects.
    /* 0x0688 */ int field_0x688;
    /* 0x068C */ s8 mPathInitialized;  ///< @brief Flag indicating if path has been initialized.
    /* 0x068E */ s16 field_0x68e;
    /* 0x0690 */ s16 mActionID;  ///< @brief High-level action ID.
    /* 0x0692 */ s16 field_0x692;
    /* 0x0694 */ int mShadowKey;     ///< @brief Shadow drawing key.
    /* 0x0698 */ s16 field_0x698[4];
    /* 0x06A0 */ s16 field_0x6a0;
    /* 0x06A2 */ u8 field_0x6a2[0x6ae - 0x6a2];
    /* 0x06AE */ csXyz field_0x6ae;
    /* 0x06B4 */ csXyz field_0x6b4;
    /* 0x06BA */ s16 field_0x6ba;
    /* 0x06BC */ u8 field_0x6bc;
    /* 0x06BD */ s8 mPursuitFlag;   ///< @brief Flag indicating pursuit mode.
    /* 0x06BE */ u16 mStatusFlags;  ///< @brief Bitfield for various status flags.
    /* 0x06C0 */ s8 field_0x6c0;
    /* 0x06C4 */ f32 field_0x6c4;
    /* 0x06C8 */ int mCurrentPathIndex;  ///< @brief Current index in the path points array.
    /* 0x06CC */ int mPathDirection;     ///< @brief Direction to iterate path points (+1 or -1).
    /* 0x06D0 */ s16 field_0x6d0;
    /* 0x06D2 */ s16 mTurnStep;  ///< @brief Step value for turning angle adjustment.
    /* 0x06D4 */ s16 field_0x6d4;
    /* 0x06D6 */ s16 field_0x6d6;
    /* 0x06D8 */ s16 field_0x6d8;
    /* 0x06DA */ s16 field_0x6da;
    /* 0x06DC */ s16 field_0x6dc;
    /* 0x06DE */ csXyz field_0x6de;
    /* 0x06E4 */ s8 field_0x6e4;
    /* 0x06E5 */ u8 field_0x6e5[0x6ec - 0x6e5];
    /* 0x06EC */ csXyz field_0x6ec[0x1d];
    /* 0x079A */ s16 mBodyTiltAngle;  ///< @brief Body tilt angle based on turning.
    /* 0x079C */ u8 field_0x79c;
    /* 0x079D */ s8 field_0x79d;
    /* 0x079E */ s8 mLapCount;  ///< @brief Lap or progress count affecting path generation.
    /* 0x079F */ s8 mPathAdjustCounter;  ///< @brief Counter for periodic path adjustment.
    /* 0x07A0 */ s16 field_0x7a0;
    /* 0x07A2 */ s8 field_0x7a2;
    /* 0x07A4 */ s16 field_0x7a4;
    /* 0x07A6 */ s8 field_0x7a6;
    /* 0x07A7 */ s8 field_0x7a7;
    /* 0x07A8 */ f32 field_0x7a8;
    /* 0x07AC */ dBgS_AcchCir field_0x7ac;
    /* 0x07EC */ dBgS_ObjAcch mAcch;
    /* 0x09C4 */ dCcD_Stts mStts;
    /* 0x0A00 */ dCcD_Sph field_0xa00[7];
    /* 0x1288 */ dCcD_Sph field_0x1288;
    /* 0x13C0 */ dCcU_AtInfo field_0x13c0;
    /* 0x13E4 */ u8 field_0x13e4;
    /* 0x13E8 */ u32 field_0x13e8[2];
    /* 0x13F0 */ u32 field_0x13f0;
    /* 0x13F4 */ u32 field_0x13f4;
    /* 0x13F8 */ u32 field_0x13f8;
    /* 0x13FC */ u32 field_0x13fc;
    /* 0x1400 */ u32 field_0x1400;
    /* 0x1404 */ u32 field_0x1404;
    /* 0x1408 */ u32 field_0x1408[2];
    /* 0x1410 */ u32 field_0x1410;
    /* 0x1414 */ u32 field_0x1414;
    /* 0x1418 */ u32 field_0x1418;
    /* 0x141C */ u32 field_0x141c;
    /* 0x1420 */ u32 field_0x1420[3];
    /* 0x142C */ u8 mMovementType;    ///< @brief Type of movement (1 for normal, 2 for faster).
    /* 0x142D */ u8 mCollisionFlags;  ///< @brief Flags for collision states.
    /* 0x142E */ u8 mLandingFlag;     ///< @brief Flag for landing after jump.
    /* 0x142F */ s8 mSpeedCapTimer;   ///< @brief Timer to cap speed at 30.
    /* 0x1430 */ s8 field_0x1430;
    /* 0x1432 */ s16 field_0x1432;
    /* 0x1434 */ fpc_ProcID field_0x1434;  // Rider actor ID? ///< @brief Process ID of rider actor.
    /* 0x1438 */ cXyz field_0x1438[2];
    /* 0x1450 */ himo_s field_0x1450[2];
    /* 0x15D0 */ mDoExt_3DlineMat1_c field_0x15d0[2];
    /* 0x1648 */ mDoExt_3DlineMat1_c field_0x1648;
    /* 0x1684 */ f32 mAnimDuration;  ///< @brief Duration or timer for animation.
    /* 0x1688 */ s16 field_0x1688;
    /* 0x168A */ s16 field_0x168a;
    /* 0x168C */ f32 field_0x168c;
    /* 0x1690 */ f32 field_0x1690;
    /* 0x1694 */ u8 field_0x1694[0x169e - 0x1694];
    /* 0x169E */ s16 field_0x169e;
    /* 0x16A0 */ s16 field_0x16a0;
    /* 0x16A4 */ cXyz field_0x16a4;
    /* 0x16B0 */ cXyz field_0x16b0;
    /* 0x16BC */ cXyz field_0x16bc;
    /* 0x16C8 */ cXyz field_0x16c8;
    /* 0x16D4 */ cXyz field_0x16d4;
    /* 0x16E0 */ cXyz field_0x16e0;
    /* 0x16EC */ cXyz field_0x16ec;
    /* 0x16F8 */ cXyz field_0x16f8;
    /* 0x1704 */ s16 field_0x1704;
    /* 0x1708 */ f32 field_0x1708;
    /* 0x170C */ f32 field_0x170c;
    /* 0x1710 */ f32 field_0x1710;
    /* 0x1714 */ f32 field_0x1714;
    /* 0x1718 */ f32 field_0x1718;
    /* 0x171C */ f32 field_0x171c;
    /* 0x1720 */ s8 field_0x1720;
    /* 0x1721 */ s8 field_0x1721;
    /* 0x1722 */ s8 field_0x1722;
    /* 0x1723 */ u8 field_0x1723[0x17c4 - 0x1723];
    /* 0x17C4 */ u32 field_0x17c4;
    /* 0x17C8 */ u8 field_0x17c8[0x17D0 - 0x17C8];
    /* 0x17D0 */ u32 field_0x17d0[4];
    /* 0x17E0 */ u8 field_0x17e0;
    /* 0x17E1 */ u8 field_0x17e1;
    /* 0x17E2 */ s16 mWaitRollAngle;  ///< @brief Roll angle during wait state.
    /* 0x17E4 */ u8 field_0x17e4[0x17e8 - 0x17e4];
    /* 0x17E8 */ f32 mSpeedRate;  ///< @brief Speed rate for riding calculations.
};

STATIC_ASSERT(sizeof(e_wb_class) == 0x17EC);


#endif /* D_A_E_WB_H */
