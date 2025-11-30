#ifndef D_A_ANDSW2_H
#define D_A_ANDSW2_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-unsorted
 * @class daAndsw2_c
 * @brief
 *
 * @details
 *
 */
class daAndsw2_c : public fopAc_ac_c {
public:
    enum Action_e {
        ACT_ON_ALL_e,
        ACT_TIMER_e,
        ACT_ORDER_e,
        ACT_EVENT_e,
        ACT_OFF_e,
        ACT_WAIT_e,
    };

    u8 getEventNo();
    u8 getSwbit();
    u8 getSwbit2();
    u8 getType();
    u8 getTimer();
    u8 getNum();
    u8 getTopSw();
    u8 getSwAns();
    BOOL chkAllSw2();
    bool chkSwStatus();
    bool chkSwStatus2();
    ~daAndsw2_c();
    int create();

    inline int execute();

    void setActio(u8 i_action) { mAction = i_action; }

    /* 0x568 */ s16 mTimer;
    /* 0x56A */ s16 mEventIdx;
    /* 0x56C */ u8 mAction;
};

#endif /* D_A_ANDSW2_H */
