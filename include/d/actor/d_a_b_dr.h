#ifndef D_A_B_DR_H
#define D_A_B_DR_H

#include "f_op/f_op_actor_mng.h"
#include "Z2AudioLib/Z2Creature.h"
#include "d/d_cc_d.h"
#include "d/d_msg_flow.h"
#include "d/d_bg_s_acch.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @ingroup actors-enemies
 * @class daB_DR_c
 * @brief Argorok
 *
 * @details City in the Sky dungeon boss.
 *
 */
class daB_DR_c : public fopEn_enemy_c {
public:
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int JointCallBack(J3DJoint*, int);
    int draw();
    void setBck(int, u8, f32, f32);
    void setActionMode(int, int);
    void mGlider_WindPosSet();
    void mBoot_c_tranceSet(bool);
    void mStatusONOFF(int);
    void setSe();
    void setDeathLightEffect();
    void chkPartCreate(bool);
    void mHabatakiAnmSet(int);
    void mGlider_AniSet(bool);
    void mGliderAtAniSet();
    void tail_hit_check();
    void week_hit_check();
    void mTimerClr();
    void mCountClr();
    void mAllClr();
    void mHeadAngleSet();
    bool flapMove(bool);
    bool revolutionMove();
    bool mPlayerHighCheck();
    bool mBgFallGroundCheck();
    void normalHoverMove();
    void executeWait();
    void executeFlyWait();
    void executeTailHit();
    void executeWeekHit();
    void mBreathSet();
    bool mBreathHighSet(bool);
    bool mFeintBreath();
    void executeBreathAttack();
    void executeWindAttack();
    bool mGliderMoveSub(f32);
    void executeGliderAttack();
    void executePillarSearch();
    void executePillarWait();
    bool startDemoCheck();
    void executeDamageDemo();
    void executeMiddleDemo();
    void executeDead();
    void executeBullet();
    void executeParts();
    void action();
    void action_fire();
    void action_parts();
    void mtx_set();
    void parts_mtx_set();
    void cc_set();
    void down_cc_set();
    void demo_skip(int);
    static int DemoSkipCallBack(void*, int);
    int execute();
    int _delete();
    int CreateHeap();
    int create();
    daB_DR_c();

    MtxP getMtx() { return mpModelMorf->getModel()->getAnmMtx(15); }

    void onTarget() { mTarget = true; }
    bool isBack() { return arg0; }

private:
    /* 0x05AC */ request_of_phase_process_class mPhase;
    /* 0x05B4 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x05B8 */ J3DModel* mpPartModel;
    /* 0x05BC */ mDoExt_brkAnm* mpCoreBrk;
    /* 0x05C0 */ Z2CreatureEnemy mSound;
    /* 0x0664 */ Z2CreatureEnemy mSound2;
    /* 0x0708 */ int mActionMode;
    /* 0x070C */ int mMoveMode;
    /* 0x0710 */ int mAnm;
    /* 0x0714 */ u8 field_0x714[0x0718 - 0x0714];
    /* 0x0718 */ int field_0x718;
    /* 0x071C */ u32 mShadowKey;
    /* 0x0720 */ f32 mTargetHeight;
    /* 0x0724 */ f32 field_0x724;
    /* 0x0728 */ f32 field_0x728;
    /* 0x072C */ f32 field_0x72c;
    /* 0x0730 */ f32 field_0x730;
    /* 0x0734 */ f32 field_0x734;
    /* 0x0738 */ f32 field_0x738;
    /* 0x073C */ f32 field_0x73c;
    /* 0x0740 */ f32 field_0x740;
    /* 0x0744 */ f32 field_0x744;
    /* 0x0748 */ f32 field_0x748;
    /* 0x074C */ s16 field_0x74c;
    /* 0x074E */ s16 field_0x74e;
    /* 0x0750 */ s16 field_0x750;
    /* 0x0752 */ s16 field_0x752;
    /* 0x0754 */ s16 field_0x754;
    /* 0x0756 */ s16 field_0x756;
    /* 0x0758 */ csXyz mHeadAngle;
    /* 0x0760 */ cXyz field_0x760;
    /* 0x076C */ cXyz mBoot_c_trance;
    /* 0x0778 */ cXyz field_0x778;
    /* 0x0784 */ cXyz field_0x784;
    /* 0x0790 */ cXyz field_0x790;
    /* 0x079C */ cXyz mGlider_WindPos;
    /* 0x07A8 */ cXyz field_0x7a8;
    /* 0x07B4 */ cXyz field_0x7b4;
    /* 0x07C0 */ int mTimer[4];
    /* 0x07D0 */ u8 field_0x7d0;
    /* 0x07D1 */ u8 field_0x7d1;
    /* 0x07D2 */ u8 mBreakPartsNo;
    /* 0x07D3 */ u8 field_0x7d3;
    /* 0x07D4 */ u8 field_0x7d4;
    /* 0x07D5 */ u8 field_0x7d5;
    /* 0x07D6 */ u8 field_0x7d6;
    /* 0x07D7 */ u8 field_0x7d7;
    /* 0x07D8 */ u8 field_0x7d8;
    /* 0x07D9 */ u8 mTarget;
    /* 0x07DA */ u8 field_0x7da;
    /* 0x07DB */ u8 field_0x7db;
    /* 0x07DC */ u8 field_0x7dc;
    /* 0x07E0 */ int field_0x7e0;
    /* 0x07E4 */ u8 mCount[4];
    /* 0x07E8 */ u8 field_0x7e8;
    /* 0x07E9 */ u8 field_0x7e9;
    /* 0x07EA */ u8 arg0;
    /* 0x07EB */ u8 mPartNo;
    /* 0x07EC */ u8 bitSw;
    /* 0x07ED */ u8 bitSw2;
    /* 0x07EE */ u8 bitSw3;
    /* 0x07F0 */ dBgS_AcchCir mAcchCir;
    /* 0x0830 */ dBgS_ObjAcch mAcch;
    /* 0x0A08 */ dCcD_Stts mColliderStts;
    /* 0x0A44 */ dCcD_Cyl mWeekCc;
    /* 0x0B80 */ dCcD_Sph mFootAtCc[2];
    /* 0x0DF0 */ dCcD_Sph field_0xdf0;
    /* 0x0F28 */ dCcD_Sph mBreathAtCc;
    /* 0x1060 */ dCcD_Sph mWindAtCc;
    /* 0x1198 */ dCcD_Sph mWindAtCc2;
    /* 0x12D0 */ dCcD_Sph mWindAtCc3;
    /* 0x1408 */ dCcD_Sph mTailCc;
    /* 0x1540 */ dCcD_Cyl mPillarCc;
    /* 0x167C */ dCcD_Sph mDownSphCc[9];
    /* 0x2174 */ dCcD_Cyl mDownCc[5];
    /* 0x27A0 */ u8 field_0x27A0[0x27A8 - 0x27A0];
    /* 0x27A8 */ Z2CreatureEnemy* field_0x27a8;
    /* 0x27AC */ u8 field_0x27AC[0x27BE - 0x27AC];
    /* 0x27BE */ u8 field_0x27be;
    /* 0x27BF */ u8 field_0x27bf[0x27C4 - 0x27BF];
    /* 0x27C4 */ cXyz mDemoCamEye;
    /* 0x27D0 */ cXyz mDemoCamCenter;
    /* 0x27DC */ u32 field_0x27dc[4];
    /* 0x27EC */ u32 field_0x27ec[4];
    /* 0x27FC */ u32 field_0x27fc[4];
    /* 0x280C */ u32 field_0x280c;
    /* 0x2810 */ u8 mInitHIO;
    /* 0x2814 */ dMsgFlow_c mMsgFlow;
};

STATIC_ASSERT(sizeof(daB_DR_c) == 0x2860);

#endif /* D_A_B_DR_H */
