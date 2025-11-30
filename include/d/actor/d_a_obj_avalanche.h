#ifndef D_A_OBJ_AVALANCHE_H
#define D_A_OBJ_AVALANCHE_H

#include "dolphin/types.h"
#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

class dBgW;

/**
 * @ingroup actors-objects
 * @class daObjAvalanche_c
 * @brief Avalanche
 *
 * @details
 *
 */
class daObjAvalanche_c : public dBgS_MoveBgActor {
public:
    enum Action {
        ACTION_WAIT,
        ACTION_ORDER_EVENT,
        ACTION_EVENT,
        ACTION_DEAD,
    };

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    int checkCollapse();
    int move();
    void event_proc_call();
    void actionWait();
    void actionOrderEvent();
    void actionEvent();
    void actionDead();
    int demoProc();
    void setEffect();
    int Draw();
    int Delete();

    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getEvId() { return fopAcM_GetParamBit(this, 8, 8); }
    void setAction(u8 action) { mAction = action; }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mModel;
    /* 0x5AC */ mDoExt_bckAnm* mBckAnm;
    /* 0x5B0 */ dBgW* mpBgW2;
    /* 0x5B4 */ Mtx mMtx2;
    /* 0x5E4 */ f32 field_0x5e4;
    /* 0x5E8 */ u8 mAction;
    /* 0x5E9 */ u8 mMapToolId;
    /* 0x5EA */ u8 mTimer;
    /* 0x5EC */ int mStaffId;
    /* 0x5F0 */ s16 mEventIdx;
};


#endif /* D_A_OBJ_AVALANCHE_H */
