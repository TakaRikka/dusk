#ifndef D_A_OBJ_ENEMY_CREATE_H
#define D_A_OBJ_ENEMY_CREATE_H

#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daObjE_CREATE_c
 * @brief Enemy Spawner
 *
 * @details
 *
 */
class daObjE_CREATE_c : public fopAc_ac_c {
public:
    void NormalAction();
    void SearchAction();
    void LoopAction();
    void Action();
    int Execute();
    int Delete();
    void NameChk();
    int create();

private:
    /* 0x568 */ u8 field_0x568;
    /* 0x569 */ u8 mEnemyNum;
    /* 0x56A */ u8 mActivateSw;
    /* 0x56B */ u8 mDeactivateSw;
    /* 0x56C */ u8 mEndLoopSw;
    /* 0x56E */ csXyz mEnemyAngle;
    /* 0x574 */ s16 mEnemyProcName;
    /* 0x578 */ u32 mEnemyParams;
    /* 0x57C */ u32* mActorList;
    /* 0x580 */ u8 mActionMode;
    /* 0x582 */ s16 mKillCheckDelayTimer;
    /* 0x584 */ u8 mAction;
    /* 0x588 */ u8 unused[12];
};

STATIC_ASSERT(sizeof(daObjE_CREATE_c) == 0x594);

enum daObjE_EnemyType {
    ENEMY_NONE,
    ENEMY_KEESE,
    ENEMY_RAT,
    ENEMY_CHU,
    ENEMY_CHU2,
    ENEMY_BABY_GOHMA,
    ENEMY_BUBBLE,
    ENEMY_BOKOBLIN,
    ENEMY_LIZALFOS,
    ENEMY_SHADOW_BEAST,
    ENEMY_SHADOW_VERMIN,
    ENEMY_SHADOW_KEESE,
    ENEMY_BOKOBLIN2,
};

enum daObjE_Action {
    ACT_NORMAL,
    ACT_SEARCH,
    ACT_LOOP,
    ACT_DEFAULT = 0xF,
};

enum daObjE_Mode {
    MODE_CREATE,
    MODE_WAIT,
    MODE_KILL_CHECK,
    MODE_END,
};

#endif /* D_A_OBJ_ENEMY_CREATE_H */
