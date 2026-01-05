#ifndef D_A_KAGO_H
#define D_A_KAGO_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_msg_flow.h"
#include "d/d_path.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-unsorted
 * @class daKago_c
 * @brief Player-controlled Kargarok
 *
 * @details
 *
 */
class daKago_c : public fopAc_ac_c {
public:
    int getBckName(int);
    void setBck(int, u8, f32, f32);
    bool checkBck(int);
    int draw();
    int executeBalloonMenu();
    f32 checkGroundHeight(cXyz, f32*);
    f32 checkRoofHeight(cXyz);
    void checkMoveHeight();
    void checkSizeBg();
    s16 getBeforeGroundHeight(u8);
    void demo_skip(int);
    static int DemoSkipCallBack(void*, int);
    void setActionMode(int, int);
    void setMidnaTagPos();
    void setMidnaRideOn();
    void setPlayerRideOn();
    void setRideOff();
    s8 searchNearPassPoint();
    int setSceneChange(int);
    void createBalloonScore();
    void startBalloonScore();
    void endBalloonScore();
    f32 checkNextPath(cXyz);
    void checkHeight();
    bool checkYaguraPos(cXyz);
    bool checkWallHitFall(int);
    bool checkAttackStart();
    s16 getValueY(f32);
    s16 getValueX(f32);
    s16 getValueAbsX(f32);
    void flySpeedCalcLockOn();
    void flySpeedCalc(s16, int);
    bool checkFlySceneChange();
    void setFlyAway();
    void setFlyAnime();
    void executeFly();
    void executeStagger();
    void executeWait();
    bool calcAttackMove(int);
    void executeAttack();
    void calcCircleCamera(int);
    void executeEvent();
    void initPerchDemo();
    bool executePerchDemo();
    bool PerchDemoAwayForward();
    void executePerch();
    void executeEvent2();
    void initFirstDemo();
    bool executeFirstDemo();
    void executePerch2();
    void executeLandingLakeHairia();
    void executeLandingBoartHouse();
    void executeLanding();
    void moveDemoFly();
    void setFlyEffect();
    void setDashEffect();
    void setDamageEffect();
    void setDashBlurEffect(int);
    void setWallHitEffect(cXyz, int);
    void setSibukiEffect();
    void setDashSibukiEffect();
    void setWaterFallEffect();
    void setFlySound();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int JointCallBack(J3DJoint*, int);
    int CreateHeap();
    int create();

    void setEvent() {
        mIsFlying = false;
        mCurrentAction = 2;
        field_0x744 = 0;
    }

    void setEvent2() {
        mIsFlying = false;
        mCurrentAction = 6;
        field_0x744 = 0;
    }

    void setKagoPath(u8 i_pathNo) {
        if (i_pathNo != 0xFF) {
            mpPath2 = dPath_GetRoomPath(i_pathNo, fopAcM_GetRoomNo(this));
        }
    }

    void onWaterFall() {
        field_0x6ea = 1;
    }

    bool isAttack() {
        return field_0x6dd != 0;
    }

    MtxP getLegR3Mtx() { return mLegR3Mtx; }
    MtxP getMidnaLocaterMtx() { return mpMorf->getModel()->getAnmMtx(4); }
    s8 getPathDir() { return mPathDir; }
    u8 isFlying() { return mIsFlying; }
    fopAc_ac_c* getLockActor() { return mpLockActor; }
    void setLockActor(fopAc_ac_c* actor) { mpLockActor = actor; }
    void setEatYm() { field_0x6d8 |= (u8) 4; }
    MtxP getMouthMtx() { return mpMorf->getModel()->getAnmMtx(7); }

private:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x574 */ Z2CreatureEnemy mSound;
    /* 0x618 */ fopAc_ac_c* mpLockActor;
    /* 0x61C */ fpc_ProcID mBalloon2DId;
    /* 0x620 */ Mtx mLegR3Mtx;
    /* 0x650 */ cXyz field_0x650[3];
    /* 0x674 */ cXyz field_0x674;
    /* 0x680 */ cXyz field_0x680;
    /* 0x68C */ cXyz field_0x68c;
    /* 0x698 */ cXyz field_0x698;
    /* 0x6A4 */ cXyz field_0x6a4;
    /* 0x6B0 */ cXyz field_0x6b0;
    /* 0x6BC */ s16 field_0x6bc;
    /* 0x6BE */ csXyz field_0x6be;
    /* 0x6C4 */ f32 field_0x6c4;
    /* 0x6C8 */ s16 field_0x6c8;
    /* 0x6CA */ s16 field_0x6ca;
    /* 0x6CC */ f32 field_0x6cc;
    /* 0x6D0 */ f32 field_0x6d0;
    /* 0x6D4 */ f32 field_0x6d4;
    /* 0x6D8 */ u8 field_0x6d8;
    /* 0x6D9 */ u8 field_0x6d9;
    /* 0x6DA */ s16 field_0x6da;
    /* 0x6DC */ u8 field_0x6dc;
    /* 0x6DD */ u8 field_0x6dd;
    /* 0x6DE */ u8 field_0x6de;
    /* 0x6DF */ u8 field_0x6df;
    /* 0x6E0 */ u8 field_0x6e0;
    /* 0x6E1 */ u8 mIsFlying;
    /* 0x6E2 */ u8 field_0x6e2[0x6e3 - 0x6e2];
    /* 0x6E3 */ u8 field_0x6e3;
    /* 0x6E4 */ u8 field_0x6e4;
    /* 0x6E5 */ u8 field_0x6e5;
    /* 0x6E6 */ u8 field_0x6e6;
    /* 0x6E7 */ u8 field_0x6e7;
    /* 0x6E8 */ u8 field_0x6e8;
    /* 0x6E8 */ u8 field_0x6e9;
    /* 0x6EA */ u8 field_0x6ea;
    /* 0x6EB */ u8 field_0x6eb;
    /* 0x6EC */ u8 field_0x6ec[0x6ed - 0x6ec];
    /* 0x6ED */ u8 field_0x6ed;
    /* 0x6F0 */ f32 field_0x6f0;
    /* 0x6F4 */ f32 field_0x6f4;
    /* 0x6F8 */ f32 field_0x6f8;
    /* 0x6FC */ f32 field_0x6fc;
    /* 0x700 */ f32 field_0x700;
    /* 0x704 */ f32 mGroundHeight;
    /* 0x708 */ f32 mRoofHeight;
    /* 0x70C */ f32 field_0x70c;
    /* 0x710 */ s16 field_0x710;
    /* 0x712 */ s16 field_0x712;
    /* 0x714 */ s16 field_0x714;
    /* 0x716 */ s16 field_0x716;
    /* 0x718 */ s16 field_0x718;
    /* 0x71A */ s16 field_0x71a;
    /* 0x71C */ s32 field_0x71c;
    /* 0x720 */ s32 field_0x720;
    /* 0x724 */ u8 field_0x724[0x728 - 0x724];
    /* 0x728 */ s32 field_0x728;
    /* 0x72C */ s32 field_0x72c;
    /* 0x730 */ s32 mDashCooldownTime;
    /* 0x734 */ s32 mDashTime;
    /* 0x738 */ s32 field_0x738;
    /* 0x73C */ s32 field_0x73c;
    /* 0x740 */ s32 mCurrentAction;
    /* 0x744 */ s32 field_0x744;
    /* 0x748 */ s32 field_0x748;
    /* 0x74C */ s32 field_0x74c;
    /* 0x750 */ s32 field_0x750;
    /* 0x754 */ s32 field_0x754;
    /* 0x758 */ s32 field_0x758;
    /* 0x75C */ char* field_0x75c;
    /* 0x760 */ char* field_0x760;
    /* 0x764 */ s32 field_0x764;
    /* 0x768 */ dPath* mpPath1;
    /* 0x76C */ dPath* mpPath2;
    /* 0x770 */ s8 mPathIdx;
    /* 0x771 */ s8 field_0x771;
    /* 0x772 */ s8 mPathIdxOffset;
    /* 0x773 */ s8 mPathDir;
    /* 0x774 */ u8 field_0x774[0x778 - 0x774];
    /* 0x778 */ dBgS_AcchCir mAcchCir;
    /* 0x7B8 */ dBgS_ObjAcch mObjAcch;
    /* 0x990 */ dCcD_Stts mStts;
    /* 0x9CC */ dCcD_Sph mSph;
    /* 0xB04 */ dCcU_AtInfo mAtInfo;
    /* 0xB28 */ u8 field_0xb28[0xb30 - 0xb28];
    /* 0xB30 */ u32 field_0xb30;
    /* 0xB34 */ u32 field_0xb34;
    /* 0xB38 */ u32 field_0xb38;
    /* 0xB3C */ u32 field_0xb3c;
    /* 0xB40 */ u32 field_0xb40;
    /* 0xB44 */ u32 field_0xb44[3];
    /* 0xB50 */ u32 field_0xb50;
    /* 0xB54 */ u8 field_0xb54;
    /* 0xB58 */ dMsgFlow_c mMsgFlow;
};

STATIC_ASSERT(sizeof(daKago_c) == 0xBA4);

#endif /* D_A_KAGO_H */
