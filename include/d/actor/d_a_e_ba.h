#ifndef D_A_E_BA_H
#define D_A_E_BA_H

#include "Z2AudioLib/Z2Creature.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_uty.h"
#include "d/d_path.h"

/**
 * @ingroup actors-enemies
 * @class e_ba_class
 * @brief Keese
 * 
 * @details 
 * 
 */
class e_ba_class {
public:
    enum Type {
        /* 0 */ TYPE_NORMAL,
        /* 1 */ TYPE_FIRE,
        /* 2 */ TYPE_ICE,
    };

    enum HomeType {
        /* 0 */ HOME_ROOF,
        /* 1 */ HOME_FLY,
        /* 2 */ HOME_APPEAR,
    };

    enum Action {
        /* 0x0 */ ACT_ROOF,
        /* 0x1 */ ACT_FIGHT_FLY,
        /* 0x2 */ ACT_FIGHT,
        /* 0x3 */ ACT_ATTACK,
        /* 0x4 */ ACT_RETURN,
        /* 0x5 */ ACT_FLY,
        /* 0x6 */ ACT_PATH_FLY,
        /* 0xA */ ACT_CHANCE = 10,
        /* 0xD */ ACT_WOLFBITE = 13,
        /* 0xE */ ACT_WIND,
        /* 0xF */ ACT_APPEAR,
    };

    enum Animation {
        /* 0x4 */ ANM_APPEAR = 4,
        /* 0x5 */ ANM_DEAD,
        /* 0x6 */ ANM_FLY,
        /* 0x7 */ ANM_FURA2,
        /* 0x8 */ ANM_HOLDWAIT,
        /* 0x9 */ ANM_HOVERING,
        /* 0xA */ ANM_WAIT,
    };

    /* 0x000 */ fopEn_enemy_c mEnemy;
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ u8 mHomeType;
    /* 0x5B5 */ u8 mDistanceParam;
    /* 0x5B6 */ u8 mType;
    /* 0x5B7 */ u8 mPathIndex;
    /* 0x5B8 */ DUSK_CONST char* mArcName;
    /* 0x5BC */ u8 field_0x5bc;
    /* 0x5BD */ s8 mPathPoint;
    /* 0x5BE */ s8 mPathStep;
    /* 0x5C0 */ dPath* mpPath;
    /* 0x5C4 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5C8 */ Z2CreatureEnemy mCreatureSound;
    /* 0x66C */ int mAnm;
    /* 0x670 */ s16 mCounter;
    /* 0x672 */ s16 mAction;
    /* 0x674 */ s16 mMode;
    /* 0x678 */ cXyz mTargetPos;
    /* 0x684 */ s16 mPlayerAngleY;
    /* 0x688 */ f32 mPlayerDistanceXZ;
    /* 0x68C */ f32 mFightFlyDistance;
    /* 0x690 */ f32 mSpeedRatio;
    /* 0x694 */ f32 mBaseAngleSpeed;
    /* 0x698 */ f32 mKnockbackSpeed;
    /* 0x69C */ s16 mKnockbackAngle;
    /* 0x69E */ csXyz mChanceAngle;
    /* 0x6A4 */ s8 mIsDying;
    /* 0x6A6 */ s16 mTimer[4];
    /* 0x6AE */ s16 mIFrames;
    /* 0x6B0 */ cXyz field_0x6b0;
    /* 0x6BC */ cXyz mWindOffset;
    /* 0x6C8 */ s16 mWindSpinSpeed;
    /* 0x6CC */ dBgS_AcchCir mAcchCir;
    /* 0x70C */ dBgS_ObjAcch mAcch;
    /* 0x8E4 */ dCcD_Stts mStts;
    /* 0x920 */ dCcD_Sph mSph;
    /* 0xA58 */ dCcU_AtInfo mAtInfo;
    /* 0xA7C */ u32 mSmokeKey1;
    /* 0xA80 */ u32 mSmokeKey2;
    /* 0xA84 */ u32 mParticleKey[4];
    /* 0xA94 */ u8 field_0xa94[0xc];
    /* 0xAA0 */ bool mHIOInit;
};

STATIC_ASSERT(sizeof(e_ba_class) == 0xAA4);

#endif /* D_A_E_BA_H */
