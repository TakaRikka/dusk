#ifndef D_A_SWC00_H
#define D_A_SWC00_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-unsorted
 * @class daSwc00_c
 * @brief Switch Area C
 *
 * @details
 *
 */
class daSwc00_c : public fopAc_ac_c {
public:
    typedef void (daSwc00_c::*actionFunc)();

    enum Action {
        ACTION_WAIT,
        ACTION_ORDER_EVENT,
        ACTION_EVENT,
        ACTION_DEAD,
    };

    int execute();
    void event_proc_call();
    void actionWait();
    void actionOrderEvent();
    void actionEvent();
    void actionDead();

    void setAction(u8 action) { mAction = action; }

    /* 0x568 */ cXyz field_0x568;
    /* 0x574 */ cXyz field_0x574;
    /* 0x580 */ s16 mEventID;
    /* 0x582 */ u8 mAction;
    /* 0x583 */ u8 field_0x583;
    /* 0x584 */ u8 field_0x584;
    /* 0x585 */ u8 field_0x585[3];
};

STATIC_ASSERT(sizeof(daSwc00_c) == 0x588);

inline u8 daSwc00_getCondition(daSwc00_c* i_this) {
    return i_this->shape_angle.x;
}

inline int daSwc00_getShape(daSwc00_c* i_this) {
    return (fopAcM_GetParam(i_this) >> 18) & 3;
}


#endif /* D_A_SWC00_H */
