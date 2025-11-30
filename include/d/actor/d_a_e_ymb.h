#ifndef D_A_E_YMB_H
#define D_A_E_YMB_H

#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_w.h"

/**
 * @ingroup actors-enemies
 * @class daE_YMB_c
 * @brief Twilight Insect Boss
 * 
 * @details 
 * 
*/

class daE_YMB_c : public fopEn_enemy_c {
public:
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int JointCallBack(J3DJoint*, int);
    int draw();
    void setBck(int, u8, f32, f32);
    bool checkBck(int);
    void setActionMode(int, int);
    void setLastDamage();
    void damage_check();
    void setCameraSwitch();
    void checkWaterPos();
    void checkGroundPos();
    void setFlyWaitVoice();
    void setDownWaitVoice();
    void setHitBoardSe();
    void setElecEffect1();
    void setElecEffect2();
    void setWaterEffect1();
    void setWaterEffect2();
    void setBoilEffect(int);
    void setDamageEffect(int);
    void setDownHamonEffect();
    void setFlyBlurEffect();
    void setRockDamageEffect();
    void setDeathEffect();
    void executeWait();
    void setBitePos(int);
    void setInclination();
    void setBodyAngle();
    bool checkWolfLockIn();
    void executeFly();
    void executeFlyAttack();
    void executeRunAway();
    void executeGuard();
    void executeSwim();
    void executeWaterJump();
    void getNearDownPos();
    void executeDamage();
    void setMidnaBindInit(cXyz*);
    void setMidnaBindLevel(int);
    void setWolfLockEffect();
    void initDownToWater();
    void calcDownToWater();
    void executeDown();
    void setCreateDrop();
    void executeDeath();
    void demo_skip(int);
    static int DemoSkipCallBack(void*, int);
    void calcLakeDemoPlayerPos();
    void executeLakeDemo();
    void executeStartDemo();
    bool checkStartBattleDemo();
    void executeBattleDemo();
    void action();
    void mtx_set();
    void getBellyBitePos(cXyz*);
    int getDownLockPoint();
    void setAttentionPos();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    cPhs__Step create();

    int getMaxLockAttack() { return 6; }
    u8 getSwitchBit() { return mSwitchBit; }

    /* 0x05AC */ request_of_phase_process_class mPhase;
    /* 0x05B4 */ mDoExt_invisibleModel mInvisModel;
    /* 0x05BC */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x05C0 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x05C4 */ Z2CreatureEnemy mSound;
    /* 0x0668 */ f32 field_0x668[6];
    /* 0x0680 */ int field_0x680[6];
    /* 0x0698 */ int field_0x698;
    /* 0x069C */ cXyz field_0x69c;
    /* 0x06A8 */ cXyz field_0x6a8;
    /* 0x06B4 */ int mAction;
    /* 0x06B8 */ int mMode;
    /* 0x06BC */ u32 mShadowKey;
    /* 0x06C0 */ u8 mSwitchBit;
    /* 0x06C1 */ u8 field_0x6c1;
    /* 0x06C4 */ f32 field_0x6c4;
    /* 0x06C8 */ f32 field_0x6c8;
    /* 0x06CC */ f32 field_0x6cc;
    /* 0x06D0 */ f32 field_0x6d0;
    /* 0x06D4 */ f32 field_0x6d4;
    /* 0x06D8 */ f32 field_0x6d8;
    /* 0x06DC */ f32 field_0x6dc;
    /* 0x06E0 */ f32 field_0x6e0;
    /* 0x06E4 */ s16 field_0x6e4;
    /* 0x06E6 */ s16 field_0x6e6;
    /* 0x06E8 */ s16 field_0x6e8;
    /* 0x06EA */ s16 field_0x6ea;
    /* 0x06EC */ int field_0x6ec;
    /* 0x06F0 */ int field_0x6f0;
    /* 0x06F4 */ int field_0x6f4;
    /* 0x06F8 */ int field_0x6f8;
    /* 0x06FC */ int field_0x6fc;
    /* 0x0700 */ int field_0x700;
    /* 0x0704 */ int field_0x704;
    /* 0x0708 */ int field_0x708;
    /* 0x070C */ int field_0x70c;
    /* 0x0710 */ u8 field_0x710;
    /* 0x0711 */ u8 field_0x711;
    /* 0x0712 */ u8 field_0x712;
    /* 0x0713 */ u8 field_0x713;
    /* 0x0714 */ u8 field_0x714;
    /* 0x0715 */ u8 field_0x715;
    /* 0x0716 */ u8 field_0x716;
    /* 0x0717 */ u8 field_0x717;
    /* 0x0718 */ u8 field_0x718;
    /* 0x0719 */ u8 field_0x719[0x71f - 0x719];
    /* 0x071F */ u8 field_0x71f;
    /* 0x0720 */ u8 field_0x720;
    /* 0x0721 */ u8 field_0x721;
    /* 0x0722 */ u8 field_0x722;
    /* 0x0723 */ u8 field_0x723;
    /* 0x0724 */ u8 field_0x724;
    /* 0x0725 */ u8 field_0x725;
    /* 0x0728 */ cXyz mDemoCamEye;
    /* 0x0734 */ cXyz mDemoCamCenter;
    /* 0x0740 */ cXyz field_0x740;
    /* 0x074C */ cXyz field_0x74c;
    /* 0x0758 */ f32 mDemoCamFovy;
    /* 0x075C */ f32 field_0x75c;
    /* 0x0760 */ s16 field_0x760;
    /* 0x0762 */ s16 field_0x762;
    /* 0x0764 */ u8 field_0x764;
    /* 0x0765 */ u8 field_0x765;
    /* 0x0768 */ Mtx field_0x768;
    /* 0x0798 */ dBgW* mpBgW;
    /* 0x079C */ dBgS_GndChk mGndChk;
    /* 0x07F0 */ dCcD_Stts mStts;
    /* 0x082C */ dCcD_Sph field_0x82c[2];
    /* 0x0A9C */ dCcD_Sph field_0xa9c;
    /* 0x0BD4 */ dCcD_Sph field_0xbd4[6];
    /* 0x1324 */ dCcD_Sph field_0x1324;
    /* 0x145C */ dCcU_AtInfo mAtInfo;
    /* 0x1480 */ u32 mElecParticles[4];
    /* 0x1490 */ u32 mMidnaBindParticles[3];
    /* 0x149C */ u8 field_0x149c[0x14d8 - 0x149c];
    /* 0x14D8 */ u32 mWaterParticles1[6];
    /* 0x14F0 */ u32 mWaterParticles2[7];
    /* 0x150C */ u32 mDownHamonParticles[2];
    /* 0x1514 */ u32 mBlurParticles[2];
    /* 0x151C */ u32 mDamageParticle;
    /* 0x1520 */ u8 field_0x1520;
};

STATIC_ASSERT(sizeof(daE_YMB_c) == 0x1524);

#endif /* D_A_E_YMB_H */
