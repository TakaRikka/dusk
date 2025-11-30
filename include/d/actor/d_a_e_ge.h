#ifndef D_A_E_GE_H
#define D_A_E_GE_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class daE_GE_c
 * @brief Guay
 *
 * @details
 *
 */
class daE_GE_c : public fopEn_enemy_c {
public:
    typedef void (daE_GE_c::*actionFunc)();

    void bckSet(int, f32, u8, f32);
    bool bckCheck(int);
    int draw();
    bool checkBeforeBg(int, f32);
    void setActionMode(int);
    void damage_check();
    bool checkOtherAttacker();
    bool searchNextAttacker();
    bool checkCircleSpeedAdd(cXyz*, cXyz*);
    void setAddCalcSpeed(cXyz&, cXyz const&, f32, f32, f32, f32);
    cXyz calcCircleFly(cXyz*, cXyz*, s16, f32, s16, f32);
    void executeWait();
    void executeFly();
    bool checkAttackPossible(s16, bool);
    void executeAttack();
    void setBackAnime(int);
    void executeBack();
    void executeDown();
    void setSurpriseTime(s16);
    void setSurprise();
    void executeSurprise();
    void setCaw();
    void executeCaw();
    void executeWind();
    void executeShield();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    int create();

    s16 getSurpriseTime() { return mSurpriseTime; }

    s32 getActionMode() { return mActionMode; }

    s32 getMoveType() {  // needs to be s32 for some reason
        return mMoveType;
    }

    u8 getGroupNo() { return mGroupNo; }

private:
    /* 0x5AC */ request_of_phase_process_class mPhaseReq;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpMorfSO;
    /* 0x5B8 */ Z2CreatureEnemy mSound;
    /* 0x65C */ dBgS_AcchCir mAcchCir;
    /* 0x69C */ dBgS_ObjAcch mObjAcch;
    /* 0x874 */ dCcD_Stts mStts;
    /* 0x8B0 */ dCcD_Sph mSphere;
    /* 0x9E8 */ dCcD_Sph mSphere2;
    /* 0xB20 */ dCcU_AtInfo mAtInfo;
    /* 0xB44 */ u32 mSmokeKey;
    /* 0xB48 */ u32 mSmokeKey2;
    /* 0xB4C */ cXyz field_0xb4c;
    /* 0xB58 */ f32 field_0xb58;
    /* 0xB5C */ f32 field_0xb5c;
    /* 0xB60 */ f32 field_0xb60;
    /* 0xB64 */ f32 field_0xb64;
    /* 0xB68 */ f32 field_0xb68;
    /* 0xB6C */ u8 field_0xB6C[0xB70 - 0xB6C];
    /* 0xB70 */ int mActionMode;
    /* 0xB74 */ int mPrevActionMode;
    /* 0xB78 */ int mMode;
    /* 0xB7C */ int mSubMode;
    /* 0xB80 */ int mBackAnimeTimer;
    /* 0xB84 */ u32 mShadowKey;
    /* 0xB88 */ s16 mHitDirectionY; // not really used
    /* 0xB8A */ s16 field_0xb8a;
    /* 0xB8C */ s16 field_0xb8c;
    /* 0xB8E */ s16 field_0xb8e[2];
    /* 0xB92 */ s16 mDamageCooldownTimer;
    /* 0xB94 */ s16 mSurpriseTime;
    /* 0xB96 */ s16 mAnmChangeTimer;
    /* 0xB98 */ s16 mCircleFlyScale;
    /* 0xB9A */ u8 mMoveType;
    /* 0xB9B */ u8 mGroupNo;
    /* 0xB9C */ u8 mSwitch;
    /* 0xB9D */ u8 field_0xb9d;
    /* 0xB9E */ u8 field_0xb9e;
    /* 0xB9F */ u8 mHIOInit;

    static actionFunc l_actionmenu[9];
};

STATIC_ASSERT(sizeof(daE_GE_c) == 0xBA0);

#endif /* D_A_E_GE_H */
