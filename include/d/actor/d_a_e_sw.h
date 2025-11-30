#ifndef D_A_E_SW_H
#define D_A_E_SW_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class daE_SW_c
 * @brief Moldorm
 * 
 * @details 
 * 
 */
class daE_SW_c : public fopEn_enemy_c {
public:
    typedef void (daE_SW_c::*actionFunc)();

    void bckSet(int, f32, u8, f32);
    int draw();
    void setBeforeJumpEffect();
    void setJumpEffect();
    void setSmokeEffect();
    void setDiveEffect();
    void setHideEffect();
    void setActionMode(s16, s16);
    fopAc_ac_c* search_bomb();
    BOOL bomb_check();
    bool setModeBack();
    void checkFall();
    bool checkBeforeWall();
    void executeWait();
    int checkRunChase();
    void executeChaseSlow();
    void executeChaseFast();
    void executeBomb();
    void executeAttack();
    void executeHook();
    void executeMoveOut();
    void executeChance();
    void executeDive();
    void executeDamage();
    void executeDie();
    void executeMaster();
    bool checkSuddenAttack(int);
    void executeSuddenAttack();
    void executeFall();
    void action();
    void mtx_set();
    void cc_set();
    void damage_check();
    void checkGroundSand();
    int execute();
    int _delete();
    int CreateHeap();
    cPhs__Step create();
    void d_setAction(void (daE_SW_c::*)());
    void d_checkFall();
    void d_damage_check();
    void d_action();
    bool d_bomb_check();
    void d_bm_chase();
    void d_wait();
    bool d_chaseCheck();
    void d_chase();
    void d_attk();
    void d_chance();
    void d_chance2();
    void d_dive();
    void d_damage();
    void d_die();
    void d_hook();
    void d_fall();
    int d_execute();

    bool d_checkAction(void (daE_SW_c::*action)()) {
        return field_0xafc == action;
    }

    bool d_checkOldAction(void (daE_SW_c::*action)()) {
        return field_0xb0c == action;
    }
private:
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x5B8 */ Z2CreatureEnemy mSound;
    /* 0x65C */ cXyz field_0x65c;
    /* 0x668 */ cXyz field_0x668;
    /* 0x674 */ cXyz field_0x674;
    /* 0x680 */ s16 mMoveMode;
    /* 0x682 */ s16 mActionMode;
    /* 0x684 */ u8 field_0x684;
    /* 0x688 */ fpc_ProcID field_0x688;
    /* 0x68C */ u8 field_0x68c;
    /* 0x690 */ f32 field_0x690;
    /* 0x694 */ u8 field_0x694;
    /* 0x695 */ u8 field_0x695;
    /* 0x696 */ u8 field_0x696;
    /* 0x697 */ u8 field_0x697;
    /* 0x698 */ int field_0x698;
    /* 0x69C */ s16 field_0x69c;
    /* 0x69E */ s16 field_0x69e;
    /* 0x6A0 */ s16 field_0x6a0;
    /* 0x6A2 */ s16 field_0x6a2;
    /* 0x6A4 */ s16 field_0x6a4;
    /* 0x6A8 */ cXyz field_0x6a8;
    /* 0x6B4 */ f32 field_0x6b4;
    /* 0x6B8 */ f32 field_0x6b8;
    /* 0x6BC */ s16 field_0x6bc;
    /* 0x6BE */ u8 field_0x6be[0x6c8 - 0x6be];
    /* 0x6C8 */ f32 field_0x6c8;
    /* 0x6CC */ s16 field_0x6cc;
    /* 0x6D0 */ f32 field_0x6d0;
    /* 0x6D4 */ f32 field_0x6d4;
    /* 0x6D8 */ f32 field_0x6d8;
    /* 0x6DC */ f32 field_0x6dc;
    /* 0x6E0 */ u32 mShadowKey;
    /* 0x6E4 */ u8 field_0x6e4;
    /* 0x6E5 */ u8 field_0x6e5;
    /* 0x6E6 */ u8 field_0x6e6;
    /* 0x6E7 */ u8 field_0x6e7;
    /* 0x6E8 */ u8 field_0x6e8;
    /* 0x6E9 */ u8 field_0x6e9;
    /* 0x6EA */ s16 field_0x6ea;
    /* 0x6EC */ s16 field_0x6ec;
    /* 0x6EE */ s16 field_0x6ee;
    /* 0x6F0 */ s16 field_0x6f0;
    /* 0x6F2 */ s16 field_0x6f2;
    /* 0x6F4 */ u8 field_0x6f4[0x6f6 - 0x6f4];
    /* 0x6F6 */ s16 field_0x6f6;
    /* 0x6F8 */ dBgS_AcchCir field_0x6f8[2];
    /* 0x778 */ dBgS_ObjAcch mBgc;
    /* 0x950 */ dCcD_Stts mStts;
    /* 0x98C */ dCcD_Sph field_0x98c;
    /* 0xAC4 */ dCcU_AtInfo mAtInfo;
    /* 0xAE8 */ u32 field_0xae8;
    /* 0xAEC */ u32 field_0xaec;
    /* 0xAF0 */ u32 field_0xaf0;
    /* 0xAF4 */ u32 field_0xaf4;
    /* 0xAF8 */ u8 field_0xaf8;
    /* 0xAF9 */ u8 field_0xaf9;
    /* 0xAFC */ actionFunc field_0xafc;
    /* 0xB0C */ actionFunc field_0xb0c;
    /* 0xB14 */ bool field_0xb14;
};

STATIC_ASSERT(sizeof(daE_SW_c) == 0xb18);

#endif /* D_A_E_SW_H */
