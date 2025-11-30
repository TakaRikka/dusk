#ifndef D_A_E_DT_H
#define D_A_E_DT_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor.h"

class daE_OT_c;

/**
 * @ingroup actors-enemies
 * @class daE_DT_c
 * @brief Deku Toad
 * 
 * @details 
 * 
 */
class daE_DT_c : public fopEn_enemy_c {
public:
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int JointCallBack(J3DJoint*, int);
    int ctrlJointTuba(J3DJoint*, J3DModel*);
    static int JointCallBackTuba(J3DJoint*, int);
    int draw();
    void demo_skip(int);
    static int DemoSkipCallBack(void*, int);
    void setBck(int, u8, f32, f32);
    bool checkBck(int);
    void setActionMode(int, int);
    void damage_check();
    void setStabPos();
    bool checkBeforeBg(f32);
    void checkTongueBg();
    void checkWaterSurface();
    void setSmallWaterEffect();
    void setLargeWaterEffect();
    void setHamonEffect(int);
    void setJumpEffect();
    void setJumpSizukuEffect();
    void setDamageShibukiEffect(int);
    void setDamageYodareEffect(int);
    void setSwingEffect();
    void setStepEffect();
    void setBombEffect();
    void setBombSmokeEffect();
    void setDeadShibukiEffect();
    void setDeadYodareEffect();
    void setBoxTubaEffect();
    void setBoxTubaLandEffect();
    int getNumberOfOtama();
    void setStayOtamaFall();
    void setStayOtamaDelete();
    bool checkBombEat();
    bool isShutterOpen();
    void executeWait();
    void executeCry();
    void executeShake();
    void executeWalk();
    void executeJump();
    void executeDown();
    void executeSearch();
    void executeDamage();
    void executeBombDamage();
    void executeDeath();
    bool pointInSight(cXyz*);
    void executeOpening();
    void calcCamera();
    void calcBoxAndTuba();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    cPhs__Step create();

    bool isFlyingAttack() { return mFlyingAttack; }
    bool isFlyingAttackLand() { return mFlyingAttackLand; }
    u8 isDead() { return mDead; }
    bool isDemoJump() { return mDemoJump; }
    MtxP getBackMtx() { return mpMorf->getModel()->getAnmMtx(1); }
    fpc_ProcID getDemoId() { return mDemoID; }

private:
    /* 0x05AC */ request_of_phase_process_class mPhase[3];
    /* 0x05C4 */ J3DModel* mpChestModel;
    /* 0x05C8 */ J3DModel* mpSpitModel;
    /* 0x05CC */ J3DModel* mpSpitBModel;
    /* 0x05D0 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x05D4 */ Z2CreatureEnemy mSound;
    /* 0x0678 */ fpc_ProcID mDemoID;
    /* 0x067C */ daE_OT_c* mDemoOtama[3];
    /* 0x0688 */ cXyz mTargetPos;
    /* 0x0694 */ cXyz mCamCenter;
    /* 0x06A0 */ cXyz mCamEye;
    /* 0x06AC */ cXyz mChestPos;
    /* 0x06B8 */ cXyz mChestSpeed;
    /* 0x06C4 */ csXyz mChestAngle;
    /* 0x06CA */ csXyz mSpitAngle[3];
    /* 0x06DC */ s16 field_0x6dc;
    /* 0x06DE */ s16 field_0x6de;
    /* 0x06E0 */ f32 mCamFovy;
    /* 0x06E4 */ f32 field_0x6e4;
    /* 0x06E8 */ f32 field_0x6e8;
    /* 0x06EC */ f32 mScale;
    /* 0x06F0 */ f32 mSpitScale[3];
    /* 0x06FC */ f32 mSpitScaleX;
    /* 0x0700 */ f32 mSpitOffset;
    /* 0x0704 */ int field_0x704;
    /* 0x0708 */ int mAction;
    /* 0x070C */ int mMode;
    /* 0x0710 */ u32 mShadowKey;
    /* 0x0714 */ int field_0x714;
    /* 0x0718 */ f32 mSpitFade[4];
    /* 0x0728 */ f32 mWaterSurface;
    /* 0x072C */ f32 field_0x72c;
    /* 0x0730 */ int field_0x730;
    /* 0x0734 */ s16 field_0x734;
    /* 0x0736 */ s16 field_0x736;
    /* 0x0738 */ s16 mTargetAngleY;
    /* 0x073A */ s16 field_0x73a;
    /* 0x073C */ s16 mCamBank;
    /* 0x073E */ s16 field_0x73e;
    /* 0x0740 */ s16 field_0x740;
    /* 0x0742 */ s16 field_0x742;
    /* 0x0744 */ s16 field_0x744;
    /* 0x0748 */ f32 field_0x748;
    /* 0x074C */ s16 field_0x74c;
    /* 0x074E */ s16 field_0x74e;
    /* 0x0750 */ s16 field_0x750;
    /* 0x0752 */ s16 field_0x752;
    /* 0x0754 */ s16 field_0x754;
    /* 0x0758 */ int mTimer;
    /* 0x075C */ int mWalkTimer;
    /* 0x0760 */ int mDamageTimer;
    /* 0x0764 */ int mDemoTimer;
    /* 0x0768 */ int mSpitTimer;
    /* 0x076C */ u8 mBodyDamageTimer;
    /* 0x076D */ u8 mLegLDamageTimer;
    /* 0x076E */ u8 mLegRDamageTimer;
    /* 0x076F */ u8 mQuakeTimer;
    /* 0x0770 */ u8 mEyePosMode;
    /* 0x0771 */ bool mLinkPressed;
    /* 0x0772 */ u8 mDemoMode;
    /* 0x0773 */ u8 mHitCounter;
    /* 0x0774 */ u8 field_0x774;
    /* 0x0775 */ u8 field_0x775;
    /* 0x0776 */ bool mFlyingAttack;
    /* 0x0777 */ bool mFlyingAttackLand;
    /* 0x0778 */ u8 mChestMode;
    /* 0x0779 */ u8 mDead;
    /* 0x077A */ u8 mSwBit;
    /* 0x077C */ f32 mBlurRate;
    /* 0x0780 */ u8 mBlurMode;
    /* 0x0781 */ bool field_0x781;
    /* 0x0782 */ bool mDemoJump;
    /* 0x0783 */ bool field_0x783;
    /* 0x0784 */ dBgS_AcchCir mAcchCir;
    /* 0x07C4 */ dBgS_ObjAcch mAcch;
    /* 0x099C */ dCcD_Stts mCcStts;
    /* 0x09D8 */ dCcD_Sph mBodyCcSph[2];
    /* 0x0C48 */ dCcD_Sph mLegCcSph[2];
    /* 0x0EB8 */ dCcD_Sph mTongueCcSph[2];
    /* 0x1128 */ dCcU_AtInfo mAtInfo;
    /* 0x114C */ u32 mParticleKey[4];
    /* 0x115C */ u32 mSmokeKey;
    /* 0x1160 */ u32 mRippleKey[2];
    /* 0x1168 */ u8 field_0x1168[0x116c - 0x1168];
    /* 0x116C */ u32 mSpitKey;
    /* 0x1170 */ bool mHIOInit;
};

STATIC_ASSERT(sizeof(daE_DT_c) == 0x1174);

struct E_DT_n {
    static f32 eDt_ShakeFrame[10];
    static f32 eDt_ShakeFrameDemo[10];
    static dCcD_SrcSph cc_dt_body_src;
    static dCcD_SrcSph cc_dt_tongue_src;
    static int mDt_OtamaNum;
    static int mDt_OtamaNo[20];
    static int m_fall_no;
    static cXyz m_tongue_pos;
};


#endif /* D_A_E_DT_H */
