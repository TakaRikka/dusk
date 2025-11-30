#ifndef D_A_COW_H
#define D_A_COW_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_particle_copoly.h"
#include "d/d_path.h"

#define N_COW_COLLIDERS 3

/**
 * @ingroup actors-unsorted
 * @class c
 * @brief Ordon Goat
 *
 * @details
 *
 */
class daCow_c : public fopAc_ac_c {
public:
    enum Crazy {
        Crazy_Wait = 0,
        Crazy_Dash = 1,
        Crazy_BeforeCatch = 2,
        Crazy_Catch = 3,
        Crazy_Throw = 4,
        Crazy_Attack = 5,
        Crazy_Away = 6,
        Crazy_End = 7,
        Crazy_Back = 8,
    };

    enum Mode {
        Mode_0 = 0,
        Mode_1 = 1,
        Mode_2 = 2,
        Mode_3 = 3,
    };

    enum Action {
        Action_Wait = 0,
        Action_NadeNade = 1,
        Action_Cry = 2,
        Action_3 = 3,
        Action_4 = 4,
        Action_5 = 5,
        Action_6 = 6,
        Action_Running = 7,
    };

    enum Animation {
        Animation_Catch = 5,
        Animation_ConnectA = 6,
        Animation_DownL = 7,
        Animation_DownR = 8,
        Animation_EatA = 9,
        Animation_FallL = 10,
        Animation_FallR = 11,
        Animation_GetupL = 12,
        Animation_GetupR = 13,
        Animation_Jump = 14,
        Animation_MooA = 15,
        Animation_Press = 16,
        Animation_PressWA = 17,
        Animation_PressWB = 18,
        Animation_Run = 19,
        Animation_Rush = 20,
        Animation_ShakeA = 21,
        Animation_Shakeoff = 22,
        Animation_Shock = 23,
        Animation_Toss = 24,
        Animation_TrotA = 25,
        Animation_WaitA = 26,
        Animation_WalkA = 27,
        Animation_WalkDejected = 28,
    };

    enum RunType {
        RunType_None = 0,
        RunType_Trot = 1,
        RunType_Run = 2,
    };

    enum Execute {
        Execute_0 = 0,
        Execute_1 = 1,
        Execute_2 = 2,
        Execute_3 = 3,
        Execute_4 = 4,
        Execute_5 = 5,
        Execute_6 = 6,
    };

    enum Flag {
        Flag_CrazyBeforeCatch = 1 << 0,
        Flag_CrazyCatch = 1 << 1,
        Flag_CrazyDash = 1 << 2,
        Flag_CrazyThrowLeft = 1 << 3,
        Flag_CrazyThrowRight = 1 << 4,
        Flag_CrazyReady = 1 << 5,
        Flag_CrazyGo = 1 << 6,
        Flag_Naderu = 1 << 7,
        Flag_NaderuFinish = 1 << 8,
        Flag_CrazyReadyDrawOn = 1 << 9,
    };

    int calcRunAnime(int);
    void setBck(int, u8, f32, f32);
    u8 checkBck(int);
    void setEffect();
    bool isChaseCowGame();
    void setCarryStatus();
    void setActetcStatus();
    bool checkNadeNadeFinish();
    bool checkNadeNade();
    void setSeSnort();
    void setRushVibration(int);
    bool checkThrow();
    void setBodyAngle(s16);
    void setBodyAngle2(s16);
    BOOL checkProcess(void (daCow_c::*)());
    bool setProcess(void (daCow_c::*)(), int);
    void damage_check();
    void setEnterCow20();
    void setEnterCow10();
    void setGroundAngle();
    bool checkRun();
    bool checkNearCowRun();
    void action_wait();
    void action_eat();
    void action_moo();
    void action_shake();
    bool checkNearWolf();
    bool checkPlayerWait();
    bool checkPlayerSurprise();
    bool checkPlayerPos();
    void checkBeforeBg();
    int checkOutOfGate(cXyz);
    s16 getCowshedAngle();
    double getCowshedDist();
    int checkCowIn(f32, f32);
    bool checkCowInOwn(int);
    void action_run();
    bool checkCurringPen();
    void setCowInCage();
    void setEnterCount();
    void action_enter();
    bool isAngry();
    bool isGuardFad();
    void setAngryHit();
    bool checkBeforeBgAngry(s16);
    void setRedTev();
    void setAngryTurn();
    void action_angry();
    void calcCatchPos(f32, int);
    void executeCrazyWait();
    void executeCrazyDash();
    void initCrazyBeforeCatch(int);
    void executeCrazyBeforeCatch();
    void initCrazyCatch(int);
    void executeCrazyCatch();
    void initCrazyThrow(int);
    void executeCrazyThrow();
    void initCrazyAttack(int);
    void executeCrazyAttack();
    void initCrazyAway(int);
    void executeCrazyAway();
    void executeCrazyEnd();
    void initCrazyBack(int);
    void executeCrazyBack();
    void action_crazy();
    void executeCrazyBack2();
    void action_thrown();
    bool checkWolfBusters();
    void action_wolf();
    void action_damage();
    void action();
    void setMtx();
    void setAttnPos();
    void setCollisions();
    int Execute();
    int CreateHeap();
    static int createHeapCallBack(fopAc_ac_c*);
    u8 initialize();
    int create();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int ctrlJointCallBack(J3DJoint*, int);
    int Draw();
    int Delete();
    // csXyz getShapeAngle();
    csXyz getShapeAngle() { return mSavedAngle; }

    void setCowP(daCow_c* cow) { mCowP = cow; }
    daCow_c* getCowP() { return mCowP; }

    // cLib_onBit<u16>(mFlags, (mask))
    void setCrazyBeforeCatch() { mFlags |= Flag_CrazyBeforeCatch; }
    void setCrazyCatch() { mFlags |= Flag_CrazyCatch; }
    void setCrazyDash() { mFlags |= Flag_CrazyDash; }
    void setCrazyThrowLeft() { mFlags |= Flag_CrazyThrowLeft; }
    void setCrazyThrowRight() { mFlags |= Flag_CrazyThrowRight; }
    void setCrazyReady() { mFlags |= Flag_CrazyReady; }
    void setCrazyGo() { mFlags |= Flag_CrazyGo; }
    void setNaderu() { mFlags |= Flag_Naderu; }
    void setNaderuFinish() { mFlags |= Flag_NaderuFinish; }
    void setCrazyReadyDrawOn() { mFlags |= Flag_CrazyReadyDrawOn; }

    bool getCowIn() { return mCowIn; }
    int getNoNearCheckTimer() const { return mNoNearCheckTimer; }

private:
    /* 0x568 */ u16 mFlags;
    /* 0x56c */ request_of_phase_process_class mPhase;
    /* 0x574 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x578 */ mDoExt_btpAnm* mpBtp;
    /* 0x57c */ Z2Creature mSound;
    /* 0x60c */ dBgS_ObjAcch mAcch;
    /* 0x7e4 */ dCcD_Stts mCcStts;
    /* 0x820 */ dBgS_AcchCir mAcchCir;
    /* 0x860 */ dCcD_Sph mSph[N_COW_COLLIDERS];
    /* 0xc08 */ daCow_c* mCowP;
    /* 0xc0c */ dPath* mPath;
    /* 0xc10 */ s8 mPointIndex;
    /* 0xc14 */ cXyz mJointPos;
    /* 0xc20 */ cXyz mTarget;
    /* 0xc2c */ csXyz mGroundTransform;
    /* 0xc32 */ csXyz mSavedAngle;
    /* 0xc38 */ csXyz mJoint1Offset;
    /* 0xc3e */ csXyz mJoint8Offset;
    /* 0xc44 */ f32 mGroundHeight;
    /* 0xc48 */ void (daCow_c::*mProcess)();
    /* 0xc54 */ int mTimer6;
    /* 0xc58 */ int mIdleTimer;
    /* 0xc5c */ u16 mMode;
    /* 0xc5e */ s8 mRunType;
    /* 0xc5f */ u8 mPrm0;
    /* 0xc60 */ u8 mExecute;
    /* 0xc61 */ u8 mAction;
    /* 0xc62 */ u8 mJointIndex;
    /* 0xc63 */ bool mIsCrazy;
    /* 0xc64 */ u32 mShadowKey;
    /* 0xc68 */ u8 mCounter1;
    /* 0xc69 */ u8 mISpeed;
    /* 0xc6c */ f32 mBoostSpeed;
    /* 0xc70 */ s16 mBoostAngle;
    /* 0xc72 */ s16 mTargetAngle;
    /* 0xc74 */ s16 mTurningSpeed;
    /* 0xc76 */ s16 mThrowIntensity;
    /* 0xc78 */ f32 mAttentionDistance;
    /* 0xc7c */ f32 mSpeed;
    /* 0xc80 */ int mInvulnerabilityTimer;
    /* 0xc84 */ int mDealDamageTimer;
    /* 0xc88 */ int mWaitTimer;
    /* 0xc8c */ int mTakenDamageCounter;
    /* 0xc90 */ int mTimer1;
    /* 0xc94 */ int mForgetCowPTimer;
    /* 0xc98 */ int mTimer10;
    /* 0xc9c */ u8 mAnimationInterval;
    /* 0xc9d */ u8 mOutOfGate;
    /* 0xc9e */ u8 mWillGetAngry;
    /* 0xc9f */ u8 mCrazy;
    /* 0xca0 */ bool mReadyToDash;
    /* 0xca1 */ u8 mRunDuration;
    /* 0xca2 */ u8 mIntersectedPlanes;
    /* 0xca3 */ u8 mRunTimer;
    /* 0xca4 */ u8 mNoNearCheckTimer;
    /* 0xca5 */ u8 mCowIn;
    /* 0xca6 */ bool mDrawOff;
    /* 0xca7 */ u8 _unused;
    /* 0xca8 */ bool mNadeNade;
    /* 0xca9 */ bool mEnterTimerDone;
    /* 0xcaa */ u8 mDoConnectAnim;
    /* 0xcac */ f32 mRedTev;
    /* 0xcb0 */ f32 mTargetRedTev;
    /* 0xcb4 */ u8 mChangeRedTev;
    /* 0xcb5 */ u8 mShouldSetEffect;
    /* 0xcb8 */ dPaPoT_c mParticle;
    /* 0xd38 */ u32 mSmokeKey;
    /* 0xd3c */ u32 mSmokeKey2;
};

STATIC_ASSERT(sizeof(daCow_c) == 0xd40);

#endif /* D_A_COW_H */
