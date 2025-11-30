#ifndef D_A_E_SM_H
#define D_A_E_SM_H
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_enemy.h"

/**
 * @ingroup actors-enemies
 * @class daE_SM_c
 * @brief Chu Worm
 * 
 * @details 
 * 
*/

class daE_SM_c : public fopEn_enemy_c {
public:
    void SetAnm(int, int, f32, f32);
    int Draw();
    void Particle_Set(u16, cXyz, cXyz, csXyz);
    void ArrowCheck();
    void E_SM_Damage();
    void SmDamageCheck();
    void E_SM_Normal();
    void E_SM_Move();
    void E_SM_Attack();
    void Action();
    void E_SM_C_Normal();
    void E_SM_C_Fight();
    void E_SM_C_Near_Escape();
    void E_SM_C_Far_Escape();
    void E_SM_C_Home_Escape();
    void E_SM_C_Escape();
    void E_SM_C_Free();
    void E_SM_C_Death();
    void C_DamageCheck();
    void E_SM_C_Hook();
    bool CheckViewArea();
    bool CameraSet();
    void SetStopCam(cXyz, f32, f32, s16);
    void SetCMoveCam(cXyz, f32, f32);
    void SetStopingCam();
    void SetReleaseCam();
    void DemoStart();
    void DemoMid();
    void E_SM_C_Demo();
    void CoreAction();
    void ArrowOn();
    int Execute();
    void setSmBaseMtx();
    void setBaseMtx();
    void CoCoreSet();
    void CoSmSet();
    int Delete();
    int CreateHeap();
    void initCoSph();
    cPhs__Step Create();
    daE_SM_c();

    /* 0x05AC */ request_of_phase_process_class mPhase;
    /* 0x05B4 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x05B8 */ J3DModel* mpBubbleModel;
    /* 0x05BC */ Z2CreatureEnemy mSound;
    /* 0x0660 */ Z2SoundObjSimple mSoundSimple;
    /* 0x0680 */ u8 mSwBit;
    /* 0x0681 */ u8 mSwBit2;
    /* 0x0682 */ bool field_0x682;
    /* 0x0683 */ bool field_0x683;
    /* 0x0684 */ bool field_0x684;
    /* 0x0685 */ bool field_0x685;
    /* 0x0688 */ int mAnm;
    /* 0x068C */ bool field_0x68c;
    /* 0x0690 */ f32 field_0x690;
    /* 0x0694 */ f32 field_0x694;
    /* 0x0698 */ bool field_0x698;
    /* 0x069C */ f32 field_0x69c;
    /* 0x06A0 */ u8 field_0x6a0[0x6ac - 0x6a0];
    /* 0x06AC */ bool field_0x6ac;
    /* 0x06AD */ u8 field_0x6ad[0x6b0 - 0x6ad];
    /* 0x06B0 */ s16 mCoreAction;
    /* 0x06B2 */ s16 field_0x6b2;
    /* 0x06B4 */ s16 field_0x6b4;
    /* 0x06B6 */ s16 field_0x6b6;
    /* 0x06B8 */ f32 field_0x6b8;
    /* 0x06BC */ s16 field_0x6bc;
    /* 0x06BE */ s16 field_0x6be;
    /* 0x06C0 */ s16 field_0x6c0[5];
    /* 0x06CA */ s16 field_0x6ca[5];
    /* 0x06D4 */ s16 field_0x6d4;
    /* 0x06D6 */ s16 field_0x6d6;
    /* 0x06D8 */ s16 field_0x6d8;
    /* 0x06DC */ f32 field_0x6dc;
    /* 0x06E0 */ f32 field_0x6e0;
    /* 0x06E4 */ f32 field_0x6e4;
    /* 0x06E8 */ f32 field_0x6e8;
    /* 0x06EC */ u8 field_0x6ec[0x6f0 - 0x6ec];
    /* 0x06F0 */ f32 field_0x6f0;
    /* 0x06F4 */ u8 field_0x6f4[0x6f8 - 0x6f4];
    /* 0x06F8 */ s8 field_0x6f8;
    /* 0x06F9 */ s8 field_0x6f9[30]; 
    /* 0x0718 */ fpc_ProcID mArrowProcIDs[30];
    /* 0x0790 */ f32 field_0x790[30];
    /* 0x0808 */ csXyz field_0x808[30];
    /* 0x08BC */ csXyz field_0x8bc[30];
    /* 0x0970 */ f32 field_0x970;
    /* 0x0974 */ f32 field_0x974;
    /* 0x0978 */ f32 field_0x978;
    /* 0x097C */ f32 field_0x97c;
    /* 0x0980 */ f32 field_0x980;
    /* 0x0984 */ int field_0x984;
    /* 0x0988 */ s16 mAction;
    /* 0x098A */ s16 mMode;
    /* 0x098C */ s16 field_0x98c;
    /* 0x0990 */ cXyz field_0x990;
    /* 0x099C */ cXyz field_0x99c;
    /* 0x09A8 */ f32 field_0x9a8;
    /* 0x09AC */ u8 field_0x9ac[0x9b0 - 0x9ac];
    /* 0x09B0 */ csXyz field_0x9b0;
    /* 0x09B8 */ cXyz field_0x9b8;
    /* 0x09C4 */ s8 field_0x9c4;
    /* 0x09C8 */ cXyz field_0x9c8;
    /* 0x09D4 */ cXyz mDemoCamCenter;
    /* 0x09E0 */ cXyz field_0x9e0;
    /* 0x09EC */ cXyz mDemoCamEye;
    /* 0x09F8 */ cXyz field_0x9f8;
    /* 0x0A04 */ f32 mFovy;
    /* 0x0A08 */ bool field_0xa08;
    /* 0x0A0A */ s16 mCDemoMode;
    /* 0x0A0C */ s16 mDemoMode;
    /* 0x0A0E */ u8 field_0xa0e[0xa10 - 0xa0e];
    /* 0x0A10 */ bool field_0xa10;
    /* 0x0A14 */ u32 mShadowKey;
    /* 0x0A18 */ u32 field_0xa18;
    /* 0x0A1C */ u32 field_0xa1c;
    /* 0x0A20 */ dBgS_AcchCir field_0xa20;
    /* 0x0A60 */ dBgS_ObjAcch field_0xa60;
    /* 0x0C38 */ dBgS_AcchCir field_0xc38;
    /* 0x0C78 */ dBgS_ObjAcch field_0xc78;
    /* 0x0E50 */ dCcD_Stts field_0xe50;
    /* 0x0E8C */ dCcD_Stts field_0xe8c;
    /* 0x0EC8 */ dCcD_Sph mCoCore;
    /* 0x1000 */ dCcD_Sph mCoSm;
    /* 0x1138 */ dCcU_AtInfo mAtInfo;
    /* 0x115C */ u8 field_0x115c;
};

STATIC_ASSERT(sizeof(daE_SM_c) == 0x1160);

#endif /* D_A_E_SM_H */
