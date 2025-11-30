#ifndef D_A_E_PH_H
#define D_A_E_PH_H

#include "SSystem/SComponent/c_phase.h"
#include "Z2AudioLib/Z2Creature.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_path.h"

/**
 * @ingroup actors-enemies
 * @class daE_PH_c
 * @brief Peahat
 * 
 * @details 
 * 
 */
class daE_PH_c : public fopEn_enemy_c {
public:
    void initCcSph();
    void SetStopingCam();
    void SetStopCam(cXyz, f32, f32, s16);
    void SetCMoveCam(cXyz, f32, f32);
    void SetReleaseCam();
    int ctrlJoint(J3DJoint*, J3DModel*);
    void setCcSph();
    void SetAnm(int, int, f32, f32);
    void SearchNearP();
    void FlyAnm();
    void SetShapeAngle();
    void GoTarget();
    void SetCheckAngle();
    bool C_CheckPoint();
    void DownBoots();
    void UpBoots();
    void C_Action();
    void SearchNextPos();
    void SearchBackPos();
    void S_SetPlaySpeed();
    void S_GoTarget();
    bool S_CheckPoint();
    void S_SetAngle();
    void FlyAnm2();
    void S_Action();
    void StopAction();
    void Action();
    void SetHeadAngle(s16);
    void CamAction();
    void DemoAction();
    void ToumeiAction();
    int Execute();
    void AttentionSet();
    void ObjHit();
    void BaseSet();
    void De_Timer();
    void EyeSet();
    int Delete();
    void setBaseMtx();
    int create();

    inline int CreateHeap();
    inline int Draw();

    /* 0x5AC */ s16 mStopTimer;
    /* 0x5AE */ s16 field_0x5ae;
    /* 0x5B0 */ u8 mCAction;
    /* 0x5B1 */ u8 mCamAction;
    /* 0x5B2 */ u8 field_0x5b2;
    /* 0x5B4 */ s16 field_0x5b4;
    /* 0x5B8 */ f32 field_0x5b8;
    /* 0x5BC */ cXyz mCamEye;
    /* 0x5C8 */ cXyz mCamEyeTarget;
    /* 0x5D4 */ cXyz mCamCenter;
    /* 0x5E0 */ cXyz mCamCenterTarget;
    /* 0x5EC */ s16 mTimers[3];
    /* 0x5F4 */ f32 mCamFovY;
    /* 0x5F8 */ u32 mFlyRockEMKey;
    /* 0x5FC */ u32 mFlyTsubuEMKey;
    /* 0x600 */ u32 mDemoRockEMKey;
    /* 0x604 */ u32 mDemoTsubuEMKey;
    /* 0x608 */ u32 mDemoRock2EMKey;
    /* 0x60C */ u32 mDemoRock3EMKey;
    /* 0x610 */ s16 field_0x610;
    /* 0x612 */ s16 field_0x612;
    /* 0x614 */ s16 field_0x614;
    /* 0x616 */ s16 field_0x616;
    /* 0x618 */ s16 field_0x618;
    /* 0x61A */ s16 field_0x61a;
    /* 0x61C */ s16 field_0x61c;
    /* 0x61E */ s16 field_0x61e;
    /* 0x620 */ f32 field_0x620;
    /* 0x624 */ f32 mAnmSpeed;
    /* 0x628 */ int mAnmID;
    /* 0x62C */ int mCurrentPntNo;
    /* 0x630 */ f32 field_0x630;
    /* 0x634 */ f32 mTargetSpeedY;
    /* 0x638 */ f32 mTargetSpeedF;
    /* 0x63C */ f32 field_0x63c;
    /* 0x640 */ f32 field_0x640;
    /* 0x644 */ dPath* mpPath;
    /* 0x648 */ cXyz mCurrentPntPos;
    /* 0x654 */ cXyz field_0x654;
    /* 0x660 */ u8 mAction;
    /* 0x661 */ u8 mPathNo;
    /* 0x662 */ u8 field_0x662;
    /* 0x663 */ u8 mMoveSpeed;
    /* 0x664 */ u8 mStopTimePrm;
    /* 0x666 */ s16 mHeadRotX;
    /* 0x668 */ s16 mHeadRotY;
    /* 0x66C */ request_of_phase_process_class mPhase;
    /* 0x674 */ Z2CreatureEnemy mSound;
    /* 0x718 */ u32 mShadowKey;
    /* 0x71C */ mDoExt_McaMorfSO* mpMorf;
    /* 0x720 */ dBgS_AcchCir mAcchCir;
    /* 0x760 */ dBgS_ObjAcch mAcch;
    /* 0x938 */ dCcD_Stts mCcStts;
    /* 0x974 */ dCcD_Sph mCcSph;
    /* 0xAAC */ dCcU_AtInfo mAtInfo;
    /* 0xAD0 */ u8 mInitializedHIO;
};

class daE_PH_HIO_c {
public:
    daE_PH_HIO_c();
    virtual ~daE_PH_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mHangPos;
    /* 0x0C */ f32 mStopTime;
    /* 0x10 */ f32 mSpeed;
    /* 0x14 */ f32 mRotationInterpolation;
    /* 0x18 */ f32 mRotationYInterpolation;
};


#endif /* D_A_E_PH_H */
