#ifndef D_A_E_WS_H
#define D_A_E_WS_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

/**
 * @ingroup actors-enemies
 * @class daE_WS_c
 * @brief Wall Skulltula
 * 
 * @details 
 * 
 */
class daE_WS_c : public fopEn_enemy_c {
public:
    enum daE_WS_Action_e {
        ACTION_WAIT_e,
        ACTION_ATTACK_e,
        ACTION_DOWN_e,
        ACTION_WIND_DOWN_e,
    };

    int draw();
    void setBck(int, u8, f32, f32);
    void setFootSound();
    void setActionMode(int);
    f32 calcTargetDist(cXyz, cXyz);
    s16 calcTargetAngle(cXyz, cXyz);
    bool checkInSearchRange(cXyz, cXyz);
    int checkPlayerPos();
    bool checkAttackEnd();
    void executeWait();
    void executeAttack();
    void executeDown();
    void executeWindDown();
    void damage_check();
    void action();
    void mtx_set();
    void cc_set();
    int execute();
    void checkInitialWall();
    bool checkBeforeBg(s16);
    bool checkWall();
    int _delete();
    int CreateHeap();
    int create();

    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x5B8 */ Z2CreatureEnemy mSound;
    /* 0x65C */ cXyz field_0x65c;
    /* 0x668 */ csXyz field_0x668;
    /* 0x66E */ csXyz field_0x66e;
    /* 0x674 */ f32 mDownColor;
    /* 0x678 */ f32 mBodyScale;
    /* 0x67C */ int mAction;
    /* 0x680 */ int mMode;
    /* 0x684 */ u32 mShadowId;
    /* 0x688 */ s16 mTargetAngle;
    /* 0x68A */ s16 mTargetStep;
    /* 0x68C */ u8 mMoveWaitTimer;
    /* 0x68E */ s16 mInvulnerabilityTimer;
    /* 0x690 */ u8 field_0x690;
    /* 0x691 */ u8 field_0x691;
    /* 0x692 */ u8 mSwbit;
    /* 0x694 */ dBgS_AcchCir mAcchCir;
    /* 0x6D4 */ dBgS_ObjAcch mAcch;
    /* 0x8AC */ dCcD_Stts mCcStts;
    /* 0x8E8 */ dCcD_Sph mCcSph;
    /* 0xA20 */ dCcD_Sph mCcBokkuriSph;
    /* 0xB58 */ dCcU_AtInfo mAtInfo;
    /* 0xB7C */ u8 mHIOInit;
};

STATIC_ASSERT(sizeof(daE_WS_c) == 0xb80);


#endif /* D_A_E_WS_H */
