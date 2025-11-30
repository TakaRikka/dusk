#ifndef D_A_E_OC_H
#define D_A_E_OC_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"


/**
 * @ingroup actors-enemies
 * @class daE_OC_c
 * @brief Bokoblin
 * 
 * @details 
 * 
 */
class daRotBridge_c;

class daE_OC_c : public fopEn_enemy_c {
public:
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int JointCallBack(J3DJoint*, int);
    int draw();
    s16 getVisionAngle(fopAc_ac_c*);
    void searchOtherOc();
    bool setWatchMode();
    bool searchPlayer();
    bool searchPlayer2();
    bool searchPlayerShakeHead();
    bool searchSound();
    int checkBeforeBg();
    bool checkBeforeBgFind();
    bool checkBeforeFloorBg(f32);
    void checkDamageBg();
    void setGroundAngle();
    void setActionMode(int, int);
    int getCutType();
    void offTgSph();
    void damage_check();
    void setBck(int, u8, f32, f32);
    bool checkBck(int);
    void setSpitEffect();
    s16 getHeadAngle();
    void setStabPos();
    void setWaitSound();
    void setWalkSound();
    void setWalkStSound();
    void setDashSound();
    void setWaitStSound();
    void setFootNoteSound();
    void executeWait();
    void executeWalk();
    void executeTalk();
    void executeFind();
    void setWeaponGroundAngle();
    void executeAttack();
    void executeDamage();
    void executeBigDamage();
    void executeWatch();
    void executeSoundWatch();
    bool checkBeforeDeath();
    void executeDeath();
    void setWaterEffect();
    void executeWaterDeath();
    void executeDemoMaster();
    void executeDemoChild();
    void checkFall();
    void executeFallDead();
    void executeFall();
    void executeFindStay();
    void executeMoveOut();
    bool checkWaterSurface();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    cPhs__Step create();

    bool isHomeWait() { return mIsHomeWait; }
    bool isBattleOn() { return mBattleOn; }
    f32 getMoveRange() { return mMoveRange; }
    f32 getPlayerRange() { return mPlayerRange; }
    int getActionMode() { return mActionMode; }
    daE_OC_c* getTalkOc() { return mpTalk; }
    J3DModel* getOcModel() { return mpMorf->getModel(); }

private:
    /* 0x5a0 */ request_of_phase_process_class mPhaseReqs[2];
    /* 0x5bc */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5c0 */ J3DModel* mpModel;
    /* 0x5c4 */ Z2CreatureEnemy mSound;
    /* 0x668 */ daE_OC_c* mpBattle;
    /* 0x66c */ daE_OC_c* mpDamage;
    /* 0x670 */ daE_OC_c* mpTalk;
    /* 0x674 */ daE_OC_c* mpParent;
    /* 0x678 */ daRotBridge_c* mpBridge;
    /* 0x67c */ cXyz mWatchPos;
    /* 0x688 */ csXyz field_0x688;
    /* 0x690 */ f32 mMoveRange;
    /* 0x690 */ f32 mPlayerRange;
    /* 0x698 */ f32 mWaterLvl;
    /* 0x69c */ f32 field_0x69c;
    /* 0x6a0 */ f32 field_0x6a0;
    /* 0x6a4 */ const char* mName;
    /* 0x6a8 */ int mActionMode;
    /* 0x6ac */ int mOldActionMode;
    /* 0x6b0 */ int mOcState;
    /* 0x6b4 */ s32 field_0x6b4;
    /* 0x6b8 */ u32 mShadowKey;
    /* 0x6bc */ s16 field_0x6bc;
    /* 0x6be */ s16 field_0x6be;
    /* 0x6c0 */ s16 field_0x6c0;
    /* 0x6c2 */ s16 field_0x6c2;
    /* 0x6c4 */ s16 field_0x6c4;
    /* 0x6c6 */ s16 field_0x6c6;
    /* 0x6c8 */ s16 field_0x6c8;
    /* 0x6ca */ s16 field_0x6ca;
    /* 0x6cc */ s16 field_0x6cc;
    /* 0x6ce */ s16 field_0x6ce;
    /* 0x6d0 */ s16 field_0x6d0;
    /* 0x6d2 */ s16 field_0x6d2;
    /* 0x6d4 */ s16 mPrevShapeAngle;
    /* 0x6d6 */ s16 field_0x6d6;
    /* 0x6d8 */ s16 field_0x6d8;
    /* 0x6da */ s16 field_0x6da;
    /* 0x6dc */ u8 field_0x6dc;
    /* 0x6dd */ u8 field_0x6dd;
    /* 0x6de */ u8 field_0x6de;
    /* 0x6df */ u8 field_0x6df;
    /* 0x6e0 */ u8 mDefeatedSwitch;
    /* 0x6e1 */ u8 field_0x6e1;
    /* 0x6e2 */ u8 field_0x6e2;
    /* 0x6e3 */ u8 field_0x6e3;
    /* 0x6e4 */ f32 field_0x6e4;
    /* 0x6e8 */ u8 field_0x6e8;
    /* 0x6e9 */ u8 mIsHomeWait;
    /* 0x6ea */ u8 mBattleOn;
    /* 0x6eb */ bool mHide;
    /* 0x6ec */ cXyz mPlayerPos;
    /* 0x6f8 */ cXyz field_0x6f8;
    /* 0x704 */ f32 field_0x704;
    /* 0x708 */ dBgS_AcchCir mAcchCir;
    /* 0x748 */ dBgS_ObjAcch mAcch;
    /* 0x920 */ dCcD_Stts mStts;
    /* 0x95c */ dCcD_Sph mSphs_cc[2];
    /* 0xbcc */ dCcD_Sph mSphs_at[2];
    /* 0xe3c */ dCcU_AtInfo mAtInfo;
    /* 0xe60 */ u32 field_0xe60;
    /* 0xe64 */ u32 field_0xe64;
    /* 0xe68 */ u32 mParticleKeys[2];
    /* 0xe70 */ u32 mWaterEffects[4];
    /* 0xe80 */ u32 mRippleKey;
    /* 0xe84 */ u8 field_0xe84;
};

STATIC_ASSERT(sizeof(daE_OC_c) == 0xe88);

struct E_OC_n {
    static f32 const oc_attackb_trans[10];
    static f32 const oc_attackc_trans[10];
    static dCcD_SrcSph cc_sph_src;
    static dCcD_SrcSph at_sph_src;
    static daE_OC_c* m_battle_oc;
    static daE_OC_c* m_damage_oc;
    static daE_OC_c* m_death_oc;
    static daE_OC_c* m_talk_oc;
};

#endif /* D_A_E_OC_H */
