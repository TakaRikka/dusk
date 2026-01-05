#ifndef D_A_E_YM_H
#define D_A_E_YM_H

#include "SSystem/SComponent/c_phase.h"
#include "Z2AudioLib/Z2Creature.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_path.h"
#include "m_Do/m_Do_ext.h"

class daKago_c;

/**
 * @ingroup actors-enemies
 * @class daE_YM_c
 * @brief Twilight Insect
 * 
 * @details 
 * 
 */
class daE_YM_c : public fopEn_enemy_c {
public:
    enum daE_YM_Action {
        /*  0 */ ACT_WAIT,
        /*  1 */ ACT_MOVE,
        /*  2 */ ACT_ESCAPE,
        /*  3 */ ACT_DOWN,
        /*  4 */ ACT_WIND,
        /*  5 */ ACT_SURPRISE,
        /*  6 */ ACT_BACK,
        /*  7 */ ACT_FALL,
        /*  8 */ ACT_ATTACK,
        /*  9 */ ACT_ATTACK_WALL,
        /* 10 */ ACT_DEFENSE,
        /* 11 */ ACT_FLY,
        /* 12 */ ACT_FLY_ATTACK,
        /* 13 */ ACT_RAIL,
        /* 14 */ ACT_BACK_RAIL,
        /* 15 */ ACT_ELECTRIC,
        /* 16 */ ACT_SWITCH,
        /* 17 */ ACT_FIRE,
        /* 18 */ ACT_RIVER,
    };

    u8 getTagNo() { return mTagNo; }

    void setTagPos(cXyz& i_position) { mTagPos = i_position; }

    void setTagPosP() { mTagPosP = &mTagPos; }

    bool checkBck(char const*, int);
    void bckSet(int, u8, f32, f32);
    void bckSetFly(int, u8, f32, f32);
    int draw();
    void setWaterEffect(); // unused
    void setDigEffect();
    void setElecEffect1();
    void setElecEffect2();
    void setFireEffect();
    bool checkWallCrash();
    bool checkWolfBark();
    bool checkSurpriseLock();
    bool checkRailSurprise();
    bool checkSurpriseNear();
    void setNormalCc();
    void setAppear();
    void setMoveSound(int);
    void setTurnSound();
    void setActionMode(int);
    void executeWait();
    void executeMove();
    void executeEscape();
    void executeDown();
    void damage_check();
    void executeWind();
    int getSurpriseType();
    void setSurpriseAway();
    void setGoHomeType();
    void executeSurprise();
    void executeBack();
    void executeFall();
    void executeAttack();
    bool checkAttackEnd();
    bool setAttackMotion();
    void executeAttackWall();
    void executeDefense();
    bool checkFlyTerritory();
    void initFly();
    void executeFly();
    void setInclination();
    void executeFlyAttack();
    void setNextPathPoint();
    bool checkRailDig();
    void executeRail();
    void executeBackRail();
    void checkElectricStart();
    void executeElectric();
    void executeSwitch();
    void initFireFly(int);
    void executeFire();
    void setRiverAttention();
    void setLockByCargo();
    void executeRiver();
    void checkFrinedSamePos();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    bool checkBeforeBg(s16);
    bool checkBeforeGround();
    void checkInitialWall();
    u8 checkWall();
    void setHideType();
    int create();

    void setFriendFlag(u8 i_flag) { mFriendFlag |= i_flag; }
    u8 getFriendFlag() { return mFriendFlag; }
    u8 getSwitchBit() { return mSwitchBit; }
    u8 isHide() { return mIsHide; }
    
private:
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5B8 */ mDoExt_brkAnm* mpBrk;
    /* 0x5BC */ Z2CreatureEnemy mSound;
    /* 0x660 */ fopAc_ac_c* field_0x660;
    /* 0x664 */ daKago_c* mpKago;
    /* 0x668 */ csXyz field_0x668;
    /* 0x66E */ u8 field_0x66E[0x670 - 0x66E];
    /* 0x670 */ cXyz mPrevPos;
    /* 0x67C */ cXyz field_0x67c;
    /* 0x688 */ f32 field_0x688;
    /* 0x68C */ f32 field_0x68c;    // model size?
    /* 0x690 */ int mAction;
    /* 0x694 */ int mLastAction;
    /* 0x698 */ int mMode;
    /* 0x69C */ u32 field_0x69c;
    /* 0x6A0 */ u8 mType;
    /* 0x6A1 */ u8 field_0x6a1;
    /* 0x6A2 */ u8 mSwitchBit;
    /* 0x6A3 */ u8 field_0x6a3;
    /* 0x6A4 */ u8 mIsHide;
    /* 0x6A5 */ u8 field_0x6a5;
    /* 0x6A6 */ u8 field_0x6a6;
    /* 0x6A7 */ u8 field_0x6a7;
    /* 0x6A8 */ u8 field_0x6a8;
    /* 0x6A9 */ u8 field_0x6a9;
    /* 0x6AA */ u8 mFriendFlag;
    /* 0x6AB */ u8 field_0x6AB[0x6AC - 0x6AB];
    /* 0x6AC */ dPath* mpPath;
    /* 0x6B0 */ dPath* field_0x6b0;
    /* 0x6B4 */ s8 mCurrentPntNo;
    /* 0x6B5 */ u8 field_0x6B5[0x6B8 - 0x6B5];
    /* 0x6B8 */ cXyz mTagPos;
    /* 0x6C4 */ cXyz* mTagPosP;
    /* 0x6C8 */ u8 mTagNo;
    /* 0x6C9 */ u8 field_0x6c9;
    /* 0x6CA */ u8 mFlyType;
    /* 0x6CB */ u8 field_0x6cb;
    /* 0x6CC */ u8 field_0x6cc;
    /* 0x6CD */ u8 field_0x6cd;
    /* 0x6CE */ u8 field_0x6ce;
    /* 0x6CF */ u8 field_0x6cf;
    /* 0x6D0 */ cXyz* field_0x6d0;
    /* 0x6D4 */ f32 field_0x6d4;
    /* 0x6D8 */ f32 mDistToPlayer;
    /* 0x6DC */ f32 field_0x6dc;
    /* 0x6E0 */ f32 field_0x6e0;
    /* 0x6E4 */ s16 field_0x6e4;
    /* 0x6E6 */ s16 field_0x6e6;
    /* 0x6E8 */ s16 field_0x6e8;
    /* 0x6EA */ s16 mAngleToPlayer;
    /* 0x6EC */ f32 field_0x6ec;
    /* 0x6F0 */ s16 field_0x6f0;
    /* 0x6F2 */ s16 field_0x6f2;
    /* 0x6F4 */ s16 field_0x6f4;
    /* 0x6F6 */ s16 field_0x6f6;
    /* 0x6F8 */ s16 field_0x6f8;
    /* 0x6FA */ s16 field_0x6fa;
    /* 0x6FC */ s16 field_0x6fc;
    /* 0x6FE */ s16 field_0x6fe;
    /* 0x700 */ s16 field_0x700;
    /* 0x702 */ u8 field_0x702[0x704 - 0x702];
    /* 0x704 */ int field_0x704;
    /* 0x708 */ s16 field_0x708;
    /* 0x70A */ s16 field_0x70a;
    /* 0x70C */ s16 field_0x70c;
    /* 0x70E */ s16 field_0x70e;
    /* 0x710 */ s16 field_0x710;
    /* 0x712 */ u8 field_0x712[0x714 - 0x712];
    /* 0x714 */ int field_0x714;
    /* 0x718 */ u32 field_0x718;
    /* 0x71C */ u8 field_0x71c;
    /* 0x71D */ u8 field_0x71d;
    /* 0x71E */ u8 field_0x71E[0x720 - 0x71E];
    /* 0x720 */ dBgS_AcchCir mAcchCir;
    /* 0x760 */ dBgS_ObjAcch mAcch;
    /* 0x938 */ dCcD_Stts mStts;
    /* 0x974 */ dCcD_Sph mSphCc;
    /* 0xAAC */ dCcU_AtInfo mAtInfo;
    /* 0xAD0 */ u32 field_0xad0;
    /* 0xAD4 */ u32 field_0xad4;
    /* 0xAD8 */ u32 field_0xad8;
    /* 0xADC */ u32 field_0xadc;
    /* 0xAE0 */ u32 field_0xae0;
    /* 0xAE4 */ u32 field_0xae4;
    /* 0xAE8 */ u32 mParticleKeys[3];
    /* 0xAF4 */ u8 field_0xaf4;
};

STATIC_ASSERT(sizeof(daE_YM_c) == 0xAF8);

struct E_YM_n {
    static dCcD_SrcSph cc_sph_src;
};


#endif /* D_A_E_YM_H */
