#ifndef D_A_SWLBALL_H
#define D_A_SWLBALL_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-unsorted
 * @class daSwLBall_c
 * @brief Switch L Ball
 *
 * @details
 *
 */
class daSwLBall_c : public fopAc_ac_c {
public:
    enum Action {
        ACTION_INIT,
        ACTION_RUN,
        ACTION_STOP,
    };

    typedef void (daSwLBall_c::*actionFunc)();

    int checkArea_sub(fopAc_ac_c*);
    int checkArea();
    void search_lb();
    int Create();
    int create();
    int execute();
    void actionInit();
    void actionRun();
    void actionStop();
    void PutCrrPos();
    int _delete();

    u8 getType() { return fopAcM_GetParamBit(this, 24, 4); }
    u8 getArg0() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getArg1() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getSwbit() { return fopAcM_GetParamBit(this, 16, 8); }
    void setAction(Action action) { mAction = action; }
    int checkPullLBall() {
        return fopAcM_GetParamBit(this,28,2) == 0;
    }

private:
    /* 0x568 */ fpc_ProcID mProcIds[2];
    /* 0x570 */ u8 mRunTimer;
    /* 0x571 */ u8 mAction;
    /* 0x572 */ u8 field_0x572;
    /* 0x574 */ u8 mIsActorCarrying[2];
    /* 0x575 */ u8 mIsPulled[2];
};

STATIC_ASSERT(sizeof(daSwLBall_c) == 0x578);


#endif /* D_A_SWLBALL_H */
