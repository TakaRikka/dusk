#ifndef D_A_B_ZANT_SIMA_H
#define D_A_B_ZANT_SIMA_H

#include "d/d_bg_w.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class daB_ZANTS_c
 * @brief Zant (Goron Mines Phase)
 *
 * @details Palace of Twilight dungeon boss. This is the actor used during the Goron Mines part of the fight.
 *
 */
class daB_ZANTS_c : public fopAc_ac_c {
public:
    int draw();
    void action();
    int execute();
    int _delete();
    int CreateHeap();
    int create();

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x578 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x57C */ u32 mPlayerID;
    /* 0x580 */ u32 mBossID;
    /* 0x584 */ u8 mSinkTimers[2];
    /* 0x586 */ bool mSinkPlayer;
    /* 0x587 */ bool mSinkZant;
    /* 0x588 */ f32 mHeight;
    /* 0x58C */ csXyz mRotation;
    /* 0x594 */ Mtx mMtx;
    /* 0x5C4 */ dBgW* mpBgW;
};

STATIC_ASSERT(sizeof(daB_ZANTS_c) == 0x5C8);

#endif /* D_A_B_ZANT_SIMA_H */
