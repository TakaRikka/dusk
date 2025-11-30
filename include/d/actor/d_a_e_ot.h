#ifndef D_A_E_OT_H
#define D_A_E_OT_H

#include "SSystem/SComponent/c_phase.h"
#include "d/actor/d_a_e_dt.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-enemies
 * @class daE_OT_c
 * @brief Toado
 * 
 * @details 
 * 
 */
class daE_OT_c : public fopEn_enemy_c {
public:
    int draw();
    void setBck(int, u8, f32, f32);
    void setActionMode(int, int);
    void damage_check();
    void checkWaterSurface();
    void setWaterEffect();
    void setEggBreakEffect();
    void setDeathLightEffect();
    void setEggStay();
    void executeEgg();
    void executeBorn();
    void executeAttack();
    void executePanic();
    void executeDamage();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    cPhs__Step create();

    int getChildNo() { return mChildNo; }
    int getActionMode() { return mAction; }
    void setShakeEgg() { mMode = 4; }
    void setDeleteEgg() { mMode = 6; }
    void setFallEgg() { mMode = 8; }

    enum Action {
        /* 0x0 */ ACT_EGG,
        /* 0x1 */ ACT_BORN,
        /* 0x2 */ ACT_ATTACK,
        /* 0x3 */ ACT_PANIC,
        /* 0x4 */ ACT_DAMAGE,
    };

private:
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5B8 */ J3DModel* mpEggModel;
    /* 0x5BC */ mDoExt_btkAnm* mpEggAnm;
    /* 0x5C0 */ Z2CreatureEnemy mSound;
    /* 0x664 */ cXyz mEggPos;
    /* 0x670 */ daE_DT_c* mpToadActor;
    /* 0x674 */ int mAction;
    /* 0x678 */ int mMode;
    /* 0x67C */ u32 mShadowKey;
    /* 0x680 */ f32 mScale;
    /* 0x684 */ f32 mAnmSpeed;
    /* 0x688 */ f32 mWaterSurface;
    /* 0x68C */ int mChildNo;
    /* 0x690 */ s16 mTargetAngleY;
    /* 0x692 */ u8 mDemoMode;
    /* 0x693 */ bool mHatched;
    /* 0x694 */ u8 mTimer1;
    /* 0x695 */ u8 mTimer2;
    /* 0x698 */ dBgS_AcchCir mAcchCir;
    /* 0x6D8 */ dBgS_ObjAcch mAcch;
    /* 0x8B0 */ dCcD_Stts mCcStts;
    /* 0x8EC */ dCcD_Sph mCcSph;
    /* 0xA24 */ dCcD_Sph mAtSph;
    /* 0xB5C */ dCcU_AtInfo mAtInfo;
    /* 0xB80 */ u32 mWaterEffectKey[4];
    /* 0xB90 */ u32 mRippleKey;
    /* 0xB94 */ bool mHIOInit;
};

STATIC_ASSERT(sizeof(daE_OT_c) == 0xb98);

#endif /* D_A_E_OT_H */
