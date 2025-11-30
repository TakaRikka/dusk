#ifndef D_A_B_ZANT_H
#define D_A_B_ZANT_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "Z2AudioLib/Z2Creature.h"
#include "d/d_bg_s_acch.h"
#include "d/d_msg_flow.h"

/**
 * @ingroup actors-enemies
 * @class daB_ZANT_c
 * @brief Zant
 *
 * @details Palace of Twilight dungeon boss.
 *
 */
class daB_ZANT_c : public fopEn_enemy_c {
public:
    enum daB_ZANT_ACTION {
        ACT_SMALL_ATTACK,
        ACT_WARP,
        ACT_DAMAGE,
        ACT_CONFUSE,
        ACT_OPENING,
        ACT_FLY,
        ACT_FLY_GROUND,
        ACT_WATER,
        ACT_HOOK,
        ACT_SWIM,
        ACT_SIMA_JUMP,
        ACT_ICE_DEMO,
        ACT_ICE_JUMP,
        ACT_ICE_STEP,
        ACT_ICE_DAMAGE,
        ACT_MONKEY,
        ACT_MONKEY_FALL,
        ACT_MONKEY_DAMAGE,
        ACT_LAST_START_DEMO,
        ACT_LAST_ATTACK,
        ACT_LAST_TIRED,
        ACT_LAST_DAMAGE,
        ACT_LAST_END_DEMO,
        ACT_ROOM_CHANGE,
    };

    enum daB_ZANT_PHASE {
        PHASE_OP,    // Opening
        PHASE_BB,    // Diababa
        PHASE_MG,    // Dangoro
        PHASE_OI,    // Morpheel
        PHASE_MK,    // Ook
        PHASE_YO,    // Blizzeta
        PHASE_LAST,  // Castle

        PHASE_MAX,
    };

    enum daB_ZANT_DMGTYPE {
        DMGTYPE_SWORD,
        DMGTYPE_MISC,
        DMGTYPE_BOOMERANG,
        DMGTYPE_OBJ,
        DMGTYPE_HOOK_OI,
    };

    int ctrlJoint(J3DJoint*, J3DModel*);
    static int JointCallBack(J3DJoint*, int);
    int draw();
    void setBck(int, u8, f32, f32);
    bool checkBck(int);
    void setActionMode(int, int);
    bool checkBigDamage();
    int checkDamageType();
    void setDamageSe(dCcD_Sph*, int);
    void damage_check();
    void ice_damage_check();
    bool setNextDamageMode(int);
    bool checkAvoidWeapon(int);
    void setTgHitBit(int);
    void setCoHitBit(int);
    void setTgShield(int);
    void setTgType(u32);
    void setZantMessage(int);
    int doZantMessage();
    void setIceLandingEffect(int);
    void setWaterBubble();
    void setMonkeyFallEffect();
    void setLastRollEffect();
    f32 getMagicSpeed();
    f32 getMagicWaterSpeed();
    void executeSmallAttack();
    bool calcScale(int);
    void executeWarp();
    void executeDamage();
    void executeConfuse();
    void executeOpening();
    void executeFly();
    void executeFlyGround();
    bool checkSwimLinkNearMouth();
    bool checkSwimLinkNear();
    void executeHook();
    void executeWater();
    void executeSwim();
    void executeSimaJump();
    void executeIceDemo();
    void executeIceJump();
    void executeIceStep();
    void executeIceDamage();
    void setFarPillarPos();
    void setNearPillarPos();
    void setNextPillarInfo(int);
    void setNextPillarPos();
    void checkPillarSwing();
    void executeMonkey();
    void executeMonkeyFall();
    void executeMonkeyDamage();
    void setLastWarp(int, int);
    void executeLastStartDemo();
    void executeLastAttack();
    void executeLastTired();
    void executeLastDamage();
    void executeLastEndDemo();
    void calcMahojinAnime();
    void calcRoomChangeCamera(int);
    void initNextRoom();
    void executeRoomChange();
    void setBaseActionMode(int);
    void action();
    void mtx_set();
    void cc_set();
    void cc_ice_set();
    int execute();
    int _delete();
    int CreateHeap();
    int create();

    /* 0x05AC */ request_of_phase_process_class mPhase;
    /* 0x05B4 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x05B8 */ J3DModel* mpSwordLModel;
    /* 0x05BC */ J3DModel* mpSwordRModel;
    /* 0x05C0 */ J3DModel* mpMahojinModel;
    /* 0x05C4 */ J3DModel* mpMahojinModel2;
    /* 0x05C8 */ mDoExt_brkAnm* mpMahojinEndBrk;
    /* 0x05CC */ mDoExt_btkAnm* mpMahojinBtk;
    /* 0x05D0 */ mDoExt_btkAnm* mpMahojinStartBtk;
    /* 0x05D4 */ mDoExt_brkAnm* mpMahojinBrk2;
    /* 0x05D8 */ mDoExt_btkAnm* mpMahojinStartBtk2;
    /* 0x05DC */ cXyz field_0x5dc;
    /* 0x05E8 */ u8 mMahojinAnmMode;
    /* 0x05E9 */ u8 mMahojin2AnmMode;
    /* 0x05EC */ f32 mMahojin2Size;
    /* 0x05F0 */ Z2CreatureEnemy mSound;
    /* 0x0694 */ u8 field_0x694[0x06A0 - 0x0694];
    /* 0x06A0 */ cXyz mFlyWarpPos;
    /* 0x06AC */ cXyz field_0x6ac;
    /* 0x06B8 */ s16 field_0x6b8;
    /* 0x06BA */ s16 field_0x6ba;
    /* 0x06BC */ f32 field_0x6bc;
    /* 0x06C0 */ f32 mModelScaleXZ;
    /* 0x06C4 */ f32 mModelScaleY;
    /* 0x06C8 */ f32 mKankyoBlend;
    /* 0x06CC */ f32 field_0x6cc;
    /* 0x06D0 */ f32 mSwordSize;
    /* 0x06D4 */ int mAction;
    /* 0x06D8 */ int mLastAction;
    /* 0x06DC */ int mMode;
    /* 0x06E0 */ u32 mShadowKey;
    /* 0x06E4 */ u16 field_0x6e4;
    /* 0x06E8 */ int mModeTimer;
    /* 0x06EC */ int field_0x6ec;
    /* 0x06F0 */ int field_0x6f0;
    /* 0x06F4 */ int field_0x6f4;
    /* 0x06F8 */ s16 field_0x6f8;
    /* 0x06FA */ u8 mSwbit;
    /* 0x06FB */ u8 mFightPhase;
    /* 0x06FC */ u8 field_0x6fc;
    /* 0x06FD */ u8 field_0x6fd;
    /* 0x06FE */ u8 mFlyWarpPosID;
    /* 0x06FF */ u8 field_0x6ff;
    /* 0x0700 */ u8 field_0x700[0x0701 - 0x0700];
    /* 0x0701 */ u8 field_0x701;
    /* 0x0702 */ u8 field_0x702;
    /* 0x0703 */ u8 field_0x703;
    /* 0x0704 */ u8 field_0x704;
    /* 0x0705 */ u8 field_0x705;
    /* 0x0706 */ u8 field_0x706;
    /* 0x0707 */ u8 field_0x707;
    /* 0x0708 */ u8 field_0x708;
    /* 0x0709 */ u8 field_0x709;
    /* 0x070A */ u8 field_0x70a;
    /* 0x070B */ u8 field_0x70b;
    /* 0x070C */ u8 field_0x70c;
    /* 0x070D */ u8 field_0x70d;
    /* 0x070E */ u8 field_0x70e;
    /* 0x070F */ u8 field_0x70f;
    /* 0x0710 */ u8 mDrawSwords;
    /* 0x0711 */ u8 field_0x711;
    /* 0x0712 */ u8 field_0x712;
    /* 0x0713 */ u8 field_0x713;
    /* 0x0714 */ u8 field_0x714;
    /* 0x0715 */ u8 field_0x715;
    /* 0x0716 */ u8 field_0x716;
    /* 0x0717 */ u8 field_0x717;
    /* 0x0718 */ u8 field_0x718;
    /* 0x0719 */ u8 mTakenBigDmg;
    /* 0x071A */ u8 mFightCycle;
    /* 0x071B */ u8 field_0x71b;
    /* 0x071C */ u8 field_0x71c;
    /* 0x071D */ u8 field_0x71d;
    /* 0x071E */ s16 mNeckRotZ;
    /* 0x0720 */ s16 mNeckRotX;
    /* 0x0722 */ s16 mBackboneRotZ;
    /* 0x0724 */ u32 field_0x724;
    /* 0x0728 */ u8 mCorrectMobileNo;
    /* 0x072C */ fpc_ProcID mMobileIDs[4];
    /* 0x073C */ fpc_ProcID mPillarIDs[9];
    /* 0x0760 */ cXyz mDemoCamEye;
    /* 0x076C */ cXyz mDemoCamCenter;
    /* 0x0778 */ f32 mDemoCamBank;
    /* 0x077C */ f32 field_0x77c;
    /* 0x0780 */ int mMsgNo;
    /* 0x0784 */ fpc_ProcID mMsgID;
    /* 0x0788 */ msg_class* mpMsg;
    /* 0x078C */ dBgS_AcchCir mAcchCir;
    /* 0x07CC */ dBgS_ObjAcch mAcch;
    /* 0x09A4 */ dCcD_Stts field_0x9a4;
    /* 0x09E0 */ dCcD_Sph mBodySphCc[2];
    /* 0x0C50 */ dCcU_AtInfo mAtInfo;
    /* 0x0C74 */ dCcD_Stts field_0xc74;
    /* 0x0CB0 */ dCcD_Sph mFootCc[11];
    /* 0x1A18 */ dCcD_Sph mFoot2Cc[11];
    /* 0x2780 */ dCcD_Sph mCameraCc[11];
    /* 0x34E8 */ dCcD_Cyl mRollCc;
    /* 0x3624 */ dCcD_Sph mSwordCc[2];
    /* 0x3894 */ u32 field_0x3894;
    /* 0x3898 */ u32 field_0x3898;
    /* 0x389C */ u32 field_0x389c[4];
    /* 0x38AC */ u32 field_0x38ac[2];
    /* 0x38B4 */ u32 field_0x38b4;
    /* 0x38B8 */ u8 mInitHIO;

    enum B_ZAN_RES_FILE_ID {
        /* BCK */
        /* 0x07 */ BCK_ZAN_DAMAGEL_A = 7,
        /* 0x08 */ BCK_ZAN_DAMAGER_A,
        /* 0x09 */ BCK_ZAN_DIE_DEMO,
        /* 0x0A */ BCK_ZAN_FAINT,
        /* 0x0B */ BCK_ZAN_FALL,
        /* 0x0C */ BCK_ZAN_FLOAT_APPEAR,
        /* 0x0D */ BCK_ZAN_FLOAT_APPEARSHORT,
        /* 0x0E */ BCK_ZAN_FLOAT_APPEARWAIT,
        /* 0x0F */ BCK_ZAN_FLOAT_DAMAGEL,
        /* 0x10 */ BCK_ZAN_FLOAT_DAMAGER,
        /* 0x11 */ BCK_ZAN_FLOAT_REACTION,
        /* 0x12 */ BCK_ZAN_FLOAT_WAIT,
        /* 0x13 */ BCK_ZAN_FLOAT_WAITRETURN,
        /* 0x14 */ BCK_ZAN_GROUND_REACTION,
        /* 0x15 */ BCK_ZAN_HOOK_HIT,
        /* 0x16 */ BCK_ZAN_HOOK_RELEASE,
        /* 0x17 */ BCK_ZAN_HOOK_WAIT,
        /* 0x18 */ BCK_ZAN_HUGE,
        /* 0x19 */ BCK_ZAN_HUGE_LANDING,
        /* 0x1A */ BCK_ZAN_JUMP_A,
        /* 0x1B */ BCK_ZAN_JUMP_B,
        /* 0x1C */ BCK_ZAN_LANDING,
        /* 0x1D */ BCK_ZAN_LANDING_DAMAGE,
        /* 0x1E */ BCK_ZAN_LANDING_WAIT,
        /* 0x1F */ BCK_ZAN_LAST_DEMO,
        /* 0x20 */ BCK_ZAN_LV1_FATIGUE,
        /* 0x21 */ BCK_ZAN_LV1_JUMP_A,
        /* 0x22 */ BCK_ZAN_LV1_JUMP_B,
        /* 0x23 */ BCK_ZAN_MAGICSHOOTA_A,
        /* 0x24 */ BCK_ZAN_MAGICSHOOTA_B,
        /* 0x25 */ BCK_ZAN_MAGICSHOOTA_B_A,
        /* 0x26 */ BCK_ZAN_MAGICSHOOTA_B_B,
        /* 0x27 */ BCK_ZAN_MAGICSHOOTA_B_C,
        /* 0x28 */ BCK_ZAN_MAGICSHOOTA_C,
        /* 0x29 */ BCK_ZAN_MAGICSHOOTA_D,
        /* 0x2A */ BCK_ZAN_OP_1,
        /* 0x2B */ BCK_ZAN_OP_2,
        /* 0x2C */ BCK_ZAN_OP_3,
        /* 0x2D */ BCK_ZAN_OP_RISE,
        /* 0x2E */ BCK_ZAN_SHIND_L,
        /* 0x2F */ BCK_ZAN_SHIND_R,
        /* 0x30 */ BCK_ZAN_SPIN,
        /* 0x31 */ BCK_ZAN_SWAMP_FALL_A,
        /* 0x32 */ BCK_ZAN_SWAMP_FALL_B,
        /* 0x33 */ BCK_ZAN_SWAMP_FALL_LOOP,
        /* 0x34 */ BCK_ZAN_SWAMP_LANDING,
        /* 0x35 */ BCK_ZAN_SWIM,
        /* 0x36 */ BCK_ZAN_SW_ATTACK,
        /* 0x37 */ BCK_ZAN_SW_ATTACKB,
        /* 0x38 */ BCK_ZAN_SW_DAMAGEL,
        /* 0x39 */ BCK_ZAN_SW_DAMAGER,
        /* 0x3A */ BCK_ZAN_SW_FATIGUE,
        /* 0x3B */ BCK_ZAN_SW_WAIT,
        /* 0x3C */ BCK_ZAN_SW_WALK,
        /* 0x3D */ BCK_ZAN_TRAMPLEA,
        /* 0x3E */ BCK_ZAN_TRAMPLEB,
        /* 0x3F */ BCK_ZAN_TRAMPLEC,
        /* 0x40 */ BCK_ZAN_TRAMPLED,
        /* 0x41 */ BCK_ZAN_WAIT,
        /* 0x42 */ BCK_ZZ_APPEARANCE,
        /* 0x43 */ BCK_ZZ_CLOSE,
        /* 0x44 */ BCK_ZZ_OPEN,

        /* BMDR */
        /* 0x47 */ BMDR_EF_TK2BALL = 0x47,
        /* 0x48 */ BMDR_ZAN,
        /* 0x49 */ BMDR_ZAN_MAHOJIN,
        /* 0x4A */ BMDR_ZAN_SWORD_L,
        /* 0x4B */ BMDR_ZAN_SWORD_R,
        /* 0x4C */ BMDR_ZZ,

        /* BRK */
        /* 0x4F */ BRK_ZAN_MAHOJIN_END = 0x4F,
        /* 0x50 */ BRK_ZZ,

        /* BTK */
        /* 0x53 */ BTK_ZAN_MAHOJIN = 0x53,
        /* 0x54 */ BTK_ZAN_MAHOJIN_START,

        /* DZB */
        /* 0x57 */ DZB_ZZ = 0x57,
    };
};

STATIC_ASSERT(sizeof(daB_ZANT_c) == 0x38BC);

class daB_ZANT_HIO_c {
public:
    daB_ZANT_HIO_c();
    virtual ~daB_ZANT_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mModelSize;
    /* 0x0C */ f32 mMahojinWaitTime;
    /* 0x10 */ f32 mDemoWarpTime;
    /* 0x14 */ f32 mPlayWarpTime;
    /* 0x18 */ f32 mSwordAttackSize;
    /* 0x1C */ f32 mAttackAnmSpeed;
    /* 0x20 */ f32 mBulletNum;
    /* 0x24 */ f32 mBulletSpeed;
    /* 0x28 */ f32 mBulletSpeedUnderwater;
    /* 0x2C */ f32 mMahojinSize;
    /* 0x30 */ f32 mMahojinOffsetX;
    /* 0x34 */ f32 mMahojinOffsetY;
    /* 0x38 */ f32 mMahojinOffsetZ;
    /* 0x3C */ f32 mAppearAnmSpeed;
    /* 0x40 */ f32 mDisappearAnmSpeed;
};

#endif /* D_A_B_ZANT_H */
