#ifndef D_A_E_GI_H
#define D_A_E_GI_H

#include "Z2AudioLib/Z2Creature.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class daE_GI_c
 * @brief Gibdo
 * 
 * @details 
 * 
 */
class daE_GI_c : public fopEn_enemy_c {
public:
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int JointCallBack(J3DJoint*, int);
    int draw();
    void setBck(int, u8, f32, f32);
    void setActionMode(int, int);
    void damage_check();
    void setWeaponAtBit(u8);
    bool setCryStop();
    void setAttackEffect();
    void setDragSwordEffect();
    void setDeathSmokeEffect();
    void setDamageEffect();
    void executeSleep();
    void executeWait();
    void executeChase();
    void executeAttack();
    void executeDamage();
    void executeBiteDamage();
    void PushButtonCount();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    int create();

    bool isBattleOn() { return mIsBattleOn; }
    bool isAttackStart() { return mIsAttackStart; }
    
private:
    /* 0x05AC */ request_of_phase_process_class mPhase;
    /* 0x05B4 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x05B8 */ J3DModel* mpSwordModel;
    /* 0x05BC */ Z2CreatureEnemy mSound;
    /* 0x0660 */ f32 mBodyDamageColor;
    /* 0x0664 */ f32 mWallCheckRadius;
    /* 0x0668 */ f32 field_0x668;
    /* 0x066C */ s16 field_0x66c;
    /* 0x0670 */ int mActionMode;
    /* 0x0674 */ int mMoveMode;
    /* 0x0678 */ u32 mShadowKey;
    /* 0x067C */ u8 field_0x67C[0x067E - 0x067C];
    /* 0x067E */ s16 field_0x67e;
    /* 0x0680 */ int mInvulnerabilityTimer;
    /* 0x0684 */ int field_0x684;
    /* 0x0688 */ int field_0x688;
    /* 0x068C */ int mPlayerStunTimer;
    /* 0x0690 */ int mCryTimer;
    /* 0x0694 */ int mContinuousHitTimer;
    /* 0x0698 */ u8 field_0x698;
    /* 0x0699 */ u8 mIsBattleOn;
    /* 0x069A */ u8 mIsCreateAwake;
    /* 0x069B */ u8 mDamageDirection;
    /* 0x069C */ u8 mIsAttackStart;
    /* 0x069D */ u8 mSwbit;
    /* 0x069E */ u8 mSwbit2;
    /* 0x069F */ u8 field_0x69f;
    /* 0x06A0 */ u8 field_0x6a0;
    /* 0x06A2 */ s16 field_0x6a2;
    /* 0x06A4 */ u8 mWolfBiteCount;
    /* 0x06A5 */ u8 mIsOnHeadLock;
    /* 0x06A6 */ s16 mPrevStickAngle;
    /* 0x06A8 */ int mPushButtonCount;
    /* 0x06AC */ dBgS_AcchCir mAcchCir;
    /* 0x06EC */ dBgS_ObjAcch mAcch;
    /* 0x08C4 */ dCcD_Stts mCcStts;
    /* 0x0900 */ dCcD_Sph mCcSph[2];
    /* 0x0B70 */ dCcD_Sph mAtSph[4];
    /* 0x1050 */ dCcU_AtInfo mAtInfo;
    /* 0x1074 */ u32 mPolyColorKey;
    /* 0x1078 */ u8 mHIOInit;
};

STATIC_ASSERT(sizeof(daE_GI_c) == 0x107c);

#endif /* D_A_E_GI_H */
