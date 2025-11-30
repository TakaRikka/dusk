#ifndef D_A_B_MGN_H
#define D_A_B_MGN_H

#include "f_op/f_op_actor.h"
#include "JSystem/JHostIO/JORReflexible.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

/**
 * @ingroup actors-enemies
 * @class daB_MGN_c
 * @brief Beast Ganon
 *
 * @details Hyrule Castle dungeon boss.
 *
*/

class daB_MGN_c : public fopEn_enemy_c {
public:
    enum daB_MGN_ACTION {
        ACTION_OPENING_e,
        ACTION_CIRCLE_e,
        ACTION_DASH_e,
        ACTION_THROWN_e,
        ACTION_DOWN_e,
        ACTION_DOWN_DAMAGE_e,
        ACTION_DOWN_BITE_DAMAGE_e,
        ACTION_WARP_e,
        ACTION_FALL_e,
        ACTION_DEATH_e,
        ACTION_JUMP_e,
    };

    int ctrlJoint(J3DJoint*, J3DModel*);
    static int JointCallBack(J3DJoint*, int);
    int draw();
    void setDashSmokeEffect();
    void setBackJumpEffect();
    void setSlideSmokeEffect();
    void setBodySlideEffect();
    void setOpeningEffect();
    void setStepEffect();
    void setFallAttackEffect();
    void setHeadLightEffect();
    void setHeadHitEffect();
    int setHideSmokeEffect(cXyz*, u8);
    void calcHideSmokeEffect();
    void drawHideSmokeEffect();
    void setBloodEffect();
    void calcBloodMove();
    void checkDownBeforeBG();
    void setBck(int, u8, f32, f32);
    bool checkBck(int);
    void setActionMode(int, int);
    void setExitMode(int);
    void onBodyShield();
    void offBodyShield();
    void onBodySlideAt();    
    void offBodySlideAt();
    bool checkHitSlideAt();
    void onBodyFallAt();
    void offBodyFallAt();
    void onBodyCo();
    void offBodyCo();
    cXyz getNearHitPos(cXyz*);
    void damage_check();
    void calcJointAngle(s16);
    void executeCircle();
    void executeDash();
    void executeThrown();
    void setDownEnd();
    void executeDown();
    void executeDownDamage();
    void executeDownBiteDamage();
    void executeWarp();
    void executeFall();
    void demo_skip(int);
    static int DemoSkipCallBack(void*, int);
    void executeOpening();
    void executeDeath();
    void executeJump();
    void checkStartJump();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    int create();

    bool isDown() { return mDownFlag != 0; }
    bool isAppear() { return mAppearFlag != 0; }

private:
    /* 0x05AC */ request_of_phase_process_class mMgnPhase;
    /* 0x05B4 */ request_of_phase_process_class mMgnePhase;
    /* 0x05BC */ mDoExt_McaMorfSO* mpMgnModelMorf;
    /* 0x05C0 */ mDoExt_brkAnm* mpMgnCoreBrk;
    /* 0x05C4 */ mDoExt_btkAnm* mpMgnBtk;
    /* 0x05C8 */ f32 mJewelColorStrength;
    /* 0x05CC */ u8 mJewelColorMode;
    /* 0x05D0 */ J3DModel* mpMgnTaiekiModel[20];
    /* 0x0620 */ cXyz mBloodEffPos[20];
    /* 0x0714 */ cXyz field_0x710[20];
    /* 0x0800 */ cXyz mBloodEffSize[20];
    /* 0x08F0 */ f32 field_0x8f0[20];
    /* 0x0940 */ f32 field_0x940[20];
    /* 0x0990 */ int mBloodEffMode[20];
    /* 0x09E0 */ Z2CreatureEnemy mSound;
    /* 0x0A84 */ cXyz field_0xa84;
    /* 0x0A90 */ s16 field_0xa90;
    /* 0x0A92 */ s16 field_0xa92;
    /* 0x0A94 */ u32 mShadowKey;
    /* 0x0A98 */ u16 mDamageInvulnerabilityTimer;
    /* 0x0A9C */ int field_0xa9c;
    /* 0x0AA0 */ int field_0xaa0;
    /* 0x0AA4 */ int mBloodEffTimer;
    /* 0x0AA8 */ int field_0xaa8;
    /* 0x0AAC */ int field_0xaac;
    /* 0x0AB0 */ cXyz mNextGdgatePos;
    /* 0x0ABC */ cXyz mDemoCamEye;
    /* 0x0AC8 */ cXyz mDemoCamCenter;
    /* 0x0AD4 */ f32 mDemoCamFovy;
    /* 0x0AD4 */ f32 mDemoCamEyeStep;
    /* 0x0ADC */ f32 field_0xadc;
    /* 0x0AE0 */ s16 field_0xae0;
    /* 0x0AE2 */ s16 field_0xae2;
    /* 0x0AE4 */ u8 field_0xae4[0xae8 - 0xae4];
    /* 0x0AE8 */ f32 field_0xae8;
    /* 0x0AEC */ u8 field_0xaec[0xaf0 - 0xaec];
    /* 0x0AF0 */ f32 mBlurRate;
    /* 0x0AF4 */ f32 mKankyoBlend;
    /* 0x0AF8 */ u8 mKankyoColMode;
    /* 0x0AF9 */ u8 mActionMode;
    /* 0x0AFA */ u8 mMoveMode;
    /* 0x0AFB */ u8 mSwBit;
    /* 0x0AFC */ u8 mLeftRightFlag;
    /* 0x0AFD */ u8 field_0xafd;
    /* 0x0AFE */ u8 field_0xafe;
    /* 0x0AFF */ u8 field_0xaff;
    /* 0x0B00 */ u8 field_0xb00;
    /* 0x0B01 */ u8 mDownFlag;
    /* 0x0B02 */ u8 mGdgateNum;
    /* 0x0B03 */ u8 field_0xb03;
    /* 0x0B04 */ u8 field_0xb04;
    /* 0x0B05 */ u8 mAppearFlag;
    /* 0x0B06 */ u8 field_0xb06;
    /* 0x0B07 */ u8 field_0xb07;
    /* 0x0B06 */ u8 field_0xb08;
    /* 0x0B09 */ u8 mHeadLightEffFlag;
    /* 0x0B0A */ u8 mHeadHitEffTimer;
    /* 0x0B0B */ u8 field_0xb0b;
    /* 0x0B0C */ u8 field_0xb0c;
    /* 0x0B10 */ int mGdgateStatusIdx;
    /* 0x0B14 */ s16 field_0xb14;
    /* 0x0B16 */ s16 field_0xb16;
    /* 0x0B18 */ s16 field_0xb18;
    /* 0x0B1C */ dBgS_AcchCir mAcchCir;
    /* 0x0B5C */ dBgS_ObjAcch mAcch;
    /* 0x0D34 */ dCcD_Stts mCcStts;
    /* 0x0D70 */ dCcD_Sph mBodyCcSph[15];
    /* 0x1FB8 */ u32 mTgType;
    /* 0x1FBC */ dCcD_Sph mAtSph;
    /* 0x20F4 */ dCcD_Sph field_0x20f4[2];
    /* 0x2364 */ dCcD_Sph field_0x2364[2];
    /* 0x25D4 */ dCcU_AtInfo mAtInfo;
    /* 0x25F8 */ u32 field_0x25f8[4];
    /* 0x2608 */ u32 field_0x2608[4];
    /* 0x2618 */ u32 mHeadLightEmitterID;
    /* 0x261C */ u32 mHeadHitEmitterID;
    /* 0x2620 */ u32 mHeadHitEmitter2ID;
    /* 0x2624 */ mDoExt_McaMorfSO* mpGdgateModelMorf[4];
    /* 0x2634 */ mDoExt_brkAnm* mpGdgateStartBrk[4];
    /* 0x2644 */ mDoExt_brkAnm* mpGdgateAppearBrk[4];
    /* 0x2654 */ mDoExt_btkAnm* mpGdgateBtk[4];
    /* 0x2664 */ cXyz mGdgatePos[4];
    /* 0x2670 */ u8 field_0x2694[0x26a4 - 0x2694];
    /* 0x26A4 */ u32 mGdgateStartEffID[4];
    /* 0x26B4 */ u8 field_0x26b4[4];
    /* 0x26B8 */ u8 mGdgateStatus[4];
    /* 0x26BC */ u8 mGdgateType[4];
    /* 0x26C0 */ s16 mGdgateAngle[4];
    /* 0x26C8 */ u8 mInitHIO;
};

STATIC_ASSERT(sizeof(daB_MGN_c) == 0x26cc);

#endif /* D_A_B_MGN_H */
