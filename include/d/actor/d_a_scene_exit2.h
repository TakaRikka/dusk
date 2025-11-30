#ifndef D_A_SCENE_EXIT2_H
#define D_A_SCENE_EXIT2_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-unsorted
 * @class daScExit_c
 * @brief Scene Exit 2
 *
 * @details
 *
 */
class daScExit_c : public fopAc_ac_c {
public:
    enum act_e {
        WAIT_e,
        START_e,
        SCENE_CHG_e,
    };

    enum action_e {
        ACTION_WAIT_e,
        ACTION_ORDER_EVENT_e,
        ACTION_EVENT_e,
        ACTION_DEAD_e,
    };

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int create();
    int execute();
    void event_proc_call();
    void actionWait();
    void actionOrderEvent();
    void actionEvent();
    void actionDead();
    int demoProc();
    BOOL checkArea();
    int draw();
    int _delete();

    int getSceneID() { return fopAcM_GetParamBit(this, 0, 8); }
    void setAction(u8 i_action) { mAction = i_action; }

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ int mStaffID;
    /* 0x574 */ f32 mRadius;
    /* 0x578 */ s16 mEventID;
    /* 0x57A */ u8 mToolID;
    /* 0x57B */ u8 mAction;
    /* 0x57C */ u8 mTimer;
};

#endif /* D_A_SCENE_EXIT2_H */
