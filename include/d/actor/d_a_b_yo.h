#ifndef D_A_B_YO_H
#define D_A_B_YO_H

#include "Z2AudioLib/Z2Creature.h"
#include "f_op/f_op_actor.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_bg_s_acch.h"

class dBgW;
class msg_class;

/**
 * @ingroup actors-enemies
 * @class daB_YO_c
 * @brief Blizzeta
 * 
 * @details Snowpeak Ruins dungeon boss.
 * 
 */
class daB_YO_c : public fopEn_enemy_c {
public:
    int draw();
    void onIceBreak(u16);
    void setBck(int, u8, f32, f32);
    void setActionMode(int, int);
    void damage_check();
    void setBreakFrizad();
    void setIcicleOperate(int);
    void setIcicleSubNumber();
    void setWindowBreakEffect(int);
    void setHensinEffect();
    void setLandingEffect(f32);
    void setBreakIceEffect();
    void setWallHitEffect();
    void setChillEffect();
    void setApperEffect();
    void setApperEffect2();
    void demo_skip(int);
    static int DemoSkipCallBack(void*, int);
    void setYoMessage(int);
    int doYoMessage();
    void executeOpening();
    void reflectFreeMove();
    void calcFreeMove(f32);
    void setReflectAngle();
    void executeChase();
    void executeSeriousDemo();
    u8 checkIcecleRevival();
    void createIcecleRevival(u8);
    void executeJump();
    void executeAttackIce();
    void executeAttackYose();
    void checkDamageWithIceBreak();
    void executeAttackBody();
    void executeDemoRevival();
    void executeDamage();
    void executeDeath();
    void reviseIceCenterPos(cXyz*);
    void calcPlayerBufPos();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    int CreateHeap2();
    cPhs__Step create();

    u8 getModelNo() { return mModelNo; }
    s16 getFrizadRollAngle() { return mIceAngle; }
    f32 getModeRarius() { return mFreezardRadius; }
    u8 getFrizadAttack() { return mFreezardAttack; }
    cXyz getIceCenterPos() { return mIceCenterPos; }
    f32 getIceRange() { return mIceRange; }
    s16 getIceAngle() { return mIceAngle; }
    f32 getPlayerXBuf(int param_0) { return mPlayerXBuf[param_0]; }
    f32 getPlayerZBuf(int param_0) { return mPlayerZBuf[param_0]; }
    void onIceStatus(u16 i_no) { mIceStatus |= (1 << i_no); }
    void setDrawOff() { mIsInactive = 1; }
    bool isEnemyDemoEnd() { return (u8)mIsEnemyDemoEnd != 0; }

private:
    /* 0x05AC */ request_of_phase_process_class mPhase1;
    /* 0x05B4 */ request_of_phase_process_class mPhase2;
    /* 0x05BC */ request_of_phase_process_class mPhase3;
    /* 0x05C4 */ request_of_phase_process_class mPhase4;
    /* 0x05CC */ request_of_phase_process_class mPhase5;
    /* 0x05D4 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x05D8 */ mDoExt_btkAnm* mpYetaBtkAnm;
    /* 0x05DC */ mDoExt_btpAnm* mpYetaBtpAnm;
    /* 0x05E0 */ mDoExt_brkAnm* mpYetaBrkAnm;
    /* 0x05E4 */ mDoExt_brkAnm* mpYetaWhiteBrkAnm;
    /* 0x05E8 */ J3DModel* mpModel[3];
    /* 0x05F4 */ mDoExt_McaMorfSO* mpYetaMorf;
    /* 0x05F8 */ mDoExt_McaMorfSO* mpYetaRevertedMorf;
    /* 0x05FC */ J3DModel* mpRoomModel[2];
    /* 0x0604 */ J3DModel* mpFurnitureModel;
    /* 0x0608 */ u32 mIcicleID[10];
    /* 0x0630 */ u32 mYstoneID;
    /* 0x0634 */ Z2CreatureEnemy mCreatureSound;
    /* 0x06D8 */ cXyz unk_6D8[5];
    /* 0x0714 */ f32 mHensinScale;
    /* 0x0718 */ cXyz mCamEye;
    /* 0x0724 */ cXyz mCamCenter;
    /* 0x0730 */ f32 mCamFovY;
    /* 0x0734 */ f32 mCamEyeRange;
    /* 0x0738 */ f32 mCamEyeSpeed;
    /* 0x073C */ f32 mCamCenterSpeed;
    /* 0x0740 */ s16 mCamEyeAngle;
    /* 0x0744 */ f32 mScale;
    /* 0x0748 */ f32 mPlayerXBuf[200];
    /* 0x0A68 */ f32 mPlayerZBuf[200];
    /* 0x0D88 */ s16 unk_D88[200];
    /* 0x0F18 */ s16 mPlayerAngle;
    /* 0x0F1C */ cXyz mOldPlayerPos;
    /* 0x0F28 */ cXyz mYstonePos;
    /* 0x0F34 */ cXyz mIceCenterPos;
    /* 0x0F40 */ u8 unk_F40[0xF4C - 0xF40];
    /* 0x0F4C */ f32 mIceRange;
    /* 0x0F50 */ f32 mIceCenterSpeed;
    /* 0x0F54 */ f32 mFreezardRadius;
    /* 0x0F58 */ f32 unk_F58;
    /* 0x0F5C */ s16 mIceAngle;
    /* 0x0F5E */ s16 mIceAngleSpeed;
    /* 0x0F60 */ f32 mYstoneScale;
    /* 0x0F64 */ f32 field_0xf64;
    /* 0x0F68 */ s16 field_0xf68;
    /* 0x0F6A */ s16 field_0xf6a;
    /* 0x0F6C */ u16 mIceStatus;
    /* 0x0F6E */ u16 mIceBreak;
    /* 0x0F70 */ int mAction;
    /* 0x0F74 */ int mMode;
    /* 0x0F78 */ u32 mShadowKey;
    /* 0x0F7C */ u8 unk_F7C[0xF80 - 0xF7C];
    /* 0x0F80 */ s16 field_0xf80;
    /* 0x0F82 */ s16 mAngleSpeed;
    /* 0x0F84 */ s16 mWallAngle;
    /* 0x0F86 */ u16 mIFrameTimer;
    /* 0x0F88 */ u16 mIFrameIronTimer;
    /* 0x0F8C */ int mActionTimer;
    /* 0x0F90 */ int mActionTimer2;
    /* 0x0F94 */ int mDamageTimer;
    /* 0x0F98 */ int mFreezardTimer;
    /* 0x0F9C */ int mQuakeTimer;
    /* 0x0FA0 */ int mAttentionTimer;
    /* 0x0FA4 */ int mCamLockOnTimer;
    /* 0x0FA8 */ u8 mModelNo;
    /* 0x0FA9 */ u8 mSwNo;
    /* 0x0FAA */ u8 mSwNo2;
    /* 0x0FAB */ u8 mLastPhaseDamage;
    /* 0x0FAC */ bool field_0xfac;
    /* 0x0FAD */ u8 mFreezardAttack;
    /* 0x0FAE */ u8 field_0xfae;
    /* 0x0FAF */ u8 mFreeMoveMode;
    /* 0x0FB0 */ u8 field_0xfb0;
    /* 0x0FB1 */ u8 field_0xfb1;
    /* 0x0FB2 */ u8 field_0xfb2;
    /* 0x0FB3 */ u8 field_0xfb3;
    /* 0x0FB4 */ u8 field_0xfb4;
    /* 0x0FB5 */ u8 field_0xfb5;
    /* 0x0FB6 */ u8 mFreezardNo;
    /* 0x0FB7 */ u8 mRoomType;
    /* 0x0FB8 */ f32 mRoomAlpha[2];
    /* 0x0FC0 */ u8 mColorMode;
    /* 0x0FC4 */ f32 mColBlend;
    /* 0x0FC8 */ f32 mBlureRate;
    /* 0x0FCC */ bool field_0xfcc;
    /* 0x0FCD */ bool field_0xfcd;
    /* 0x0FCE */ u8 mIsInactive;
    /* 0x0FCF */ u8 mIsInactive2;
    /* 0x0FD0 */ bool mReverted;
    /* 0x0FD1 */ bool field_0xfd1;
    /* 0x0FD2 */ bool mIsEnemyDemoEnd;
    /* 0x0FD4 */ s32 mWarpHoleID;
    /* 0x0FD8 */ int mMsgIdx;
    /* 0x0FDC */ u32 mMsgPcID;
    /* 0x0FE0 */ msg_class* mpMsg;
    /* 0x0FE4 */ Mtx mBgMtx;
    /* 0x1014 */ dBgW* mpRoomNormalBgW;
    /* 0x1018 */ dBgW* mpRoomArenaBgW;
    /* 0x101C */ dBgS_AcchCir mAcchCir;
    /* 0x105C */ dBgS_ObjAcch mAcch;
    /* 0x1234 */ dCcD_Stts mCcStts;
    /* 0x1270 */ dCcD_Stts mCcIronStts;
    /* 0x12AC */ dCcD_Cyl mAtCyl;
    /* 0x13E8 */ dCcD_Sph mTgSph[3];
    /* 0x1790 */ dCcD_Sph mTgIronSph[3];
    /* 0x1B38 */ dCcU_AtInfo mAtInfo;
    /* 0x1B5C */ u8 unk_1B5C[0x1B64 - 0x1B5C];
    /* 0x1B64 */ u32 mChillKey;
    /* 0x1B68 */ u32 mHensinKey[4];
    /* 0x1B78 */ u8 unk_1B78[0x1B7C - 0x1B78];
    /* 0x1B7C */ u32 mParticleKey;
    /* 0x1B80 */ u32 mAppearKey;
    /* 0x1B84 */ bool mHIOInit;

    enum Action {
        /* 0 */ ACT_OPENING,
        /* 1 */ ACT_CHASE,
        /* 2 */ ACT_SERIOUS_DEMO,
        /* 3 */ ACT_JUMP,
        /* 4 */ ACT_ATTACK_ICE,
        /* 5 */ ACT_ATTACK_YOSE,
        /* 6 */ ACT_ATTACK_BODY,
        /* 7 */ ACT_DEMO_REVIVAL,
        /* 8 */ ACT_DAMAGE,
        /* 9 */ ACT_DEATH,
    };

    enum Animation {
        /* 0x00 */ ANM_ANGRY,
        /* 0x01 */ ANM_ANGRY_WAIT,
        /* 0x02 */ ANM_FLOAT,
        /* 0x03 */ ANM_TALK_A,
        /* 0x04 */ ANM_TALK_B,
        /* 0x05 */ ANM_TO_TALK_B,
        /* 0x06 */ ANM_WAIT_A,
        /* 0x07 */ ANM_WAIT_B,
        /* 0x08 */ ANM_WALK,
        /* 0x09 */ ANM_SCREAM,
        /* 0x0A */ ANM_SCREAM_WAIT,
        /* 0x0B */ ANM_WAIT_C2,
        /* 0x0C */ ANM_WAIT_C,
        /* 0x0D */ ANM_FLOAT_B,
        /* 0x0E */ ANM_WAIT_D,
        /* 0x0F */ ANM_DOWN_A,
        /* 0x10 */ ANM_DOWN_B,
        /* 0x11 */ ANM_DOWN_C,
        /* 0x12 */ ANM_DOWN,
    };
};

STATIC_ASSERT(sizeof(daB_YO_c) == 0x1B88);

#endif /* D_A_B_YO_H */
