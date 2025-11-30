#ifndef D_A_E_ZS_H
#define D_A_E_ZS_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-enemies
 * @class daE_ZS_c
 * @brief Staltroop
 * 
 * @details 
 * 
 */
class daE_ZS_c : public fopEn_enemy_c {
public:
    enum daE_ZS_Action {
        /* 0 */ ACT_APPEAR,
        /* 1 */ ACT_WAIT,
        /* 2 */ ACT_DAMAGE,
        /* 3 */ ACT_DRIVE
    };

    int draw();
    void setBck(int, u8, f32, f32);
    void setActionMode(int, int);
    void damage_check();
    u8 mBossHandCheck();
    void executeAppear();
    void executeWait();
    void executeDamage();
    void executeDrive();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    int create();

private:
    /* 0x5ac */ request_of_phase_process_class mPhase;
    /* 0x5b4 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5b8 */ Z2CreatureEnemy mSound;
    /* 0x65c */ f32 field_0x65c;
    /* 0x660 */ int mAction;
    /* 0x664 */ int mMode;
    /* 0x668 */ int mResIndex;
    /* 0x66c */ u32 mShadowKey;
    /* 0x670 */ u8 field_0x670;
    /* 0x671 */ u8 field_0x671;
    /* 0x672 */ u8 field_0x672;
    /* 0x673 */ u8 field_0x673;
    /* 0x674 */ dBgS_AcchCir mAcchCir;
    /* 0x6b4 */ dBgS_ObjAcch mAcch;
    /* 0x88c */ dCcD_Stts mStts;
    /* 0x8c8 */ dCcD_Cyl mCyl;
    /* 0xa04 */ cCcD_Obj* mpCollider;
    /* 0xa08 */ u8 field_0xa08[0xa0c - 0xa08];
    /* 0xa0c */ Z2CreatureEnemy* mpSound;
    /* 0xa10 */ u8 field_0xa10[0xa22 - 0xa10];
    /* 0xa22 */ s8 field_0xa22;
    /* 0xa23 */ u8 field_0xa23[0xa28 - 0xa23];
    /* 0xa28 */ u8 field_0xa28;
    /* 0xa29 */ u8 field_0xa29[0xa3c - 0xa29];
};

STATIC_ASSERT(sizeof(daE_ZS_c) == 0xa3c);

#endif /* D_A_E_ZS_H */
