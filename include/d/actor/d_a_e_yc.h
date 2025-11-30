#ifndef D_A_E_YC_H
#define D_A_E_YC_H

#include "SSystem/SComponent/c_phase.h"
#include "Z2AudioLib/Z2Creature.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-enemies
 * @class e_yc_class
 * @brief Twilight Kargorok
 * 
 * @details 
 * 
 */
class e_yc_class : public fopEn_enemy_c {
public:
    MtxP getLegR3Mtx() { return mpMorf->getModel()->getAnmMtx(28); }

    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ u8 mParam;
    /* 0x5B8 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5BC */ Z2CreatureEnemy mCreatureSound;
    /* 0x660 */ int mAnm;
    /* 0x664 */ int mFlyAnmMode;
    /* 0x668 */ u32 mShadowKey;
    /* 0x66C */ s16 mCounter;
    /* 0x66E */ s16 mAction;
    /* 0x670 */ s16 mMode;
    /* 0x674 */ cXyz mTargetPos;
    /* 0x680 */ s16 mAngleToPlayer;
    /* 0x684 */ f32 mDistToPlayer;
    /* 0x688 */ f32 mAngleSpeedRatio;
    /* 0x68C */ f32 mAngleXSpeedRatio;
    /* 0x690 */ f32 mMaxAngleSpeed;
    /* 0x694 */ f32 field_0x694;
    /* 0x698 */ f32 field_0x698;
    /* 0x69C */ f32 field_0x69c;
    /* 0x6A0 */ s16 field_0x6a0;
    /* 0x6A2 */ s16 mTimer[4];
    /* 0x6AA */ s16 mCcDisableTimer;
    /* 0x6AC */ u32 mRiderID;
    /* 0x6B0 */ u8 field_0x6b0;
    /* 0x6B1 */ u8 field_0x6b1;
    /* 0x6B2 */ s8 mNoDrawFlag;
    /* 0x6B3 */ s8 mWolfBiteDamageCount;
    /* 0x6B4 */ dBgS_AcchCir mAcchCir;    
    /* 0x6F4 */ dBgS_ObjAcch mAcch;
    /* 0x8CC */ dCcD_Stts mCcStts;
    /* 0x908 */ dCcD_Sph mCcSph;
    /* 0xA40 */ dCcD_Sph mAtSph;
    /* 0xB78 */ dCcU_AtInfo mAtInfo;
    /* 0xB9C */ u32 mParticleKey[2];
    /* 0xBA4 */ bool mHIOInit;

    enum Action {
        /* 0x00 */ ACT_FLY,
        /* 0x0A */ ACT_F_FLY = 10,
        /* 0x0B */ ACT_HOVERING,
        /* 0x0C */ ACT_ATTACK,
        /* 0x14 */ ACT_WOLFBITE = 20,
    };

    enum Animation {
        /* 0x04 */ ANM_CATCH_END = 4,
        /* 0x05 */ ANM_CATCH_MIDDLE,
        /* 0x06 */ ANM_CATCH_START,
        /* 0x07 */ ANM_CRASH,
        /* 0x08 */ ANM_CRASH2,
        /* 0x09 */ ANM_FLY,
        /* 0x0A */ ANM_FLY_BRAKE,
        /* 0x0B */ ANM_FLY_DASH_WL,
        /* 0x0C */ ANM_FLY_GLIDE,
        /* 0x0D */ ANM_FLY_GLIDE_WL,
        /* 0x0E */ ANM_FLY_LIMIT_WL,
        /* 0x0F */ ANM_FLY_WL,
        /* 0x10 */ ANM_HANGED,
        /* 0x11 */ ANM_HANGED_BRUSH,
        /* 0x12 */ ANM_HANGED_BRUSH2,
        /* 0x13 */ ANM_HANGED_DAMAGE,
        /* 0x14 */ ANM_HANGED_WAIT,
        /* 0x15 */ ANM_HOVERING,
    };
};

STATIC_ASSERT(sizeof(e_yc_class) == 0xBA8);

struct daE_YC_HIO_c {
    daE_YC_HIO_c();
    virtual ~daE_YC_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 mScale;
};

STATIC_ASSERT(sizeof(daE_YC_HIO_c) == 0xC);

#endif /* D_A_E_YC_H */
