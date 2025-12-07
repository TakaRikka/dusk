#ifndef D_A_OBJ_THDOOR_H
#define D_A_OBJ_THDOOR_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "d/d_msg_flow.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjThDoor_c
 * @brief Telma's Bar Door
 *
 * @details
 *
 */
class daObjThDoor_c : public dBgS_MoveBgActor {
public:
    typedef void (daObjThDoor_c::*actionFunc)();

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
    void set_cyl();
    void action();
    void event_proc_call();
    void actionWait();
    void actionOrderEvent();
    void actionEvent();
    void actionDead();
    int demoProc();
    BOOL checkArea();
    int Draw();
    int Delete();

    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    int getMsg() { return fopAcM_GetParamBit(this, 8, 16); }
    BOOL checkDemo() { return fopAcM_GetParamBit(this, 24, 1); }
    void setAction(u8 action) { mAction = action; }

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mModel;
    /* 0x5AC */ u8 mAction;
    /* 0x5AD */ u8 mMapToolId;
    /* 0x5AE */ s16 mEventIdx;
    /* 0x5B0 */ int mStaffId;
    /* 0x5B4 */ u8 field_0x5b4;
    /* 0x5B5 */ u8 field_0x5b5;
    /* 0x5B6 */ u8 field_0x5b6;
    /* 0x5B8 */ s16 field_0x5b8;
    /* 0x5BC */ dMsgFlow_c mMsgFlow;
    /* 0x608 */ s16 field_0x608;
    /* 0x60A */ s16 field_0x60a;
    /* 0x60C */ dCcD_Stts mStts;
    /* 0x648 */ dCcD_Cyl mCyls[2];
};

STATIC_ASSERT(sizeof(daObjThDoor_c) == 0x8c0);

#if DEBUG
class daObjThDoor_HIO_c : public mDoHIO_entry_c {
public:
    daObjThDoor_HIO_c();
    void genMessage(JORMContext* ctx);
};
#endif

#endif /* D_A_OBJ_THDOOR_H */
