#ifndef D_A_E_PM_H
#define D_A_E_PM_H

#include "SSystem/SComponent/c_phase.h"
#include "Z2AudioLib/Z2Creature.h"
#include "f_op/f_op_actor.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_msg_flow.h"

class dPath;

/**
 * @ingroup actors-enemies
 * @class daE_PM_c
 * @brief Skullkid
 * 
 * @details 
 * 
 */
class daE_PM_c : public fopEn_enemy_c {
public:
    void DemoSkip(int);
    static int DemoSkipCallBack(void*, int);
    int CreateHeap();
    void initCcCylinder();
    void setCcCylinder();
    int ctrlJoint(J3DJoint*, J3DModel*);
    void Particle_Set(u16, cXyz);
    void SetAnm(int, int, f32, f32);
    void Yazirushi();
    void SearchRndP();
    void SearchFarP();
    void SearchNearP();
    BOOL SearchNextPos();
    void SetGakkiEffect();
    bool CameraSet();
    void SetStopingCam();
    void SetStopCam(cXyz, f32, f32, s16);
    void SetMoveCam2(f32, f32);
    void SetMoveCam(f32, f32);
    void SetReleaseCam();
    void Ap_StartAction();
    void Ap_CreateAction();
    void Ap_EscapeAction();
    void AppearAction();
    void DemoBeforeEscape();
    void DemoAfterEscape();
    void DemoMoveAction();
    void WaitAction();
    void GroundCheck();
    void EscapeAction();
    void DeathAction();
    void Action();
    void DemoBossStart2();
    void DemoBossStart();
    void BossEscapeAction();
    void BossDamageAction();
    void BossWaitAction();
    void BossDeathAction();
    void BossAction();
    int Execute();
    void StartAction();
    void GakkiLoopAction(cXyz, f32);
    void DemoCreateAction();
    void CreateAction();
    void CreateChk();
    void LampAction();
    void DamageAction();
    void At_Check();
    void ObjHit();
    void EyeMove();
    int Draw();
    int Delete();
    void setBaseMtx();
    void setGakkiBaseMtx();
    void setLampBaseMtx();
    void SkipChk();
    cPhs__Step Create();

    void AppearSet() { mAppear = true; }
    u8 ActionChk() { return mPuppetDelete; }
    u8 SwitchChk() { return mStage; }
    u8 DoorChk() { return mDoorAction; }

private:
    /* 0x5AC */ fpc_ProcID mPuppetID[4];
    /* 0x5BC */ u8 field_0x5bc[0x5DC - 0x5BC];
    /* 0x5DC */ u8 mDoorAction;
    /* 0x5E0 */ dPath* mpPath;
    /* 0x5E4 */ Vec mPoint;
    /* 0x5F0 */ u8 mPathIndex;
    /* 0x5F4 */ int mPointIndex;
    /* 0x5F8 */ u8 mBossHealth;
    /* 0x5FC */ f32 field_0x5fc;
    /* 0x600 */ u8 mPuppetNum;
    /* 0x602 */ s16 mTargetAngleY;
    /* 0x604 */ s16 mHeadAngleX;
    /* 0x606 */ s16 mHeadAngleZ;
    /* 0x608 */ s16 mTargetHeadAngleX;
    /* 0x60C */ f32 mTargetSpeed;
    /* 0x610 */ u8 mAction;
    /* 0x611 */ u8 mMode;
    /* 0x612 */ u8 mDemoMode;
    /* 0x613 */ u8 field_0x613;
    /* 0x614 */ u8 mPuppetDelete;
    /* 0x616 */ s16 mIFrameTimer;
    /* 0x618 */ u8 field_0x618;
    /* 0x619 */ bool mAppear;
    /* 0x61A */ u8 mStage;
    /* 0x61B */ u8 mSwBit;
    /* 0x61C */ int mAnm;
    /* 0x620 */ s16 mTimer[4];
    /* 0x628 */ u32 mParticleKey;
    /* 0x62C */ u32 mGakkiParticleKey[3];
    /* 0x638 */ u32 mLampParticleKey;
    /* 0x63C */ dMsgFlow_c mMsgFlow;
    /* 0x688 */ u8 mEyeAnmTimer;
    /* 0x689 */ u8 mEyeAnmFrame;
    /* 0x68C */ cXyz mCamCenter;
    /* 0x698 */ cXyz mCamEye;
    /* 0x6A4 */ cXyz mCamCenterTarget;
    /* 0x6B0 */ cXyz mCamEyeTarget;
    /* 0x6BC */ cXyz mCamCenterTarget2;
    /* 0x6C8 */ cXyz mCamEyeTarget2;
    /* 0x6D4 */ f32 mCamFovY;
    /* 0x6D8 */ bool mSecondEncounter;
    /* 0x6DC */ cXyz mOldLampPosition;
    /* 0x6E8 */ cXyz mLampPosition;
    /* 0x6F4 */ csXyz mLampAngle;
    /* 0x6FA */ s16 field_0x6fa;
    /* 0x6FC */ s16 field_0x6fc;
    /* 0x6FE */ bool mBossLightOn;
    /* 0x6FF */ bool field_0x6ff;
    /* 0x700 */ mDoExt_btpAnm* mpEyeAnm;
    /* 0x704 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x708 */ mDoExt_McaMorf* mpTrumpetMorf;
    /* 0x70C */ J3DModel* mpLampModel;
    /* 0x710 */ mDoExt_McaMorf* mpGlowEffectMorf;
    /* 0x714 */ request_of_phase_process_class mPhase;
    /* 0x71C */ u32 mShadowKey;
    /* 0x720 */ Z2CreatureEnemy mCreatureSound;
    /* 0x7C4 */ dBgS_AcchCir mAcchCir;
    /* 0x804 */ dBgS_ObjAcch mAcch;
    /* 0x9DC */ dCcD_Stts mCcStts;
    /* 0xA18 */ dCcD_Cyl mCcCyl;
    /* 0xB54 */ dCcU_AtInfo mAtInfo;
    /* 0xB78 */ bool mHIOInit;
};

STATIC_ASSERT(sizeof(daE_PM_c) == 0xB7C);

#endif /* D_A_E_PM_H */
