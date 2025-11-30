#ifndef D_A_OBJ_WARP_OBRG_H
#define D_A_OBJ_WARP_OBRG_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_msg_flow.h"

/**
 * @ingroup actors-objects
 * @class daObjWarpOBrg_c
 * @brief Eldin Warp Bridge
 *
 * @details
 *
 */
class daObjWarpOBrg_c : public dBgS_MoveBgActor {
public:
    enum Action_e {
        ACTION_WAIT_e,
        ACTION_ORDER_EVENT_e,
        ACTION_TALK_EVENT_e,
        ACTION_WARP_EVENT_e,
        ACTION_DEAD_e,
        ACTION_ORDER_EVENT_DST_e,
        ACTION_WARP_EVENT_DST_e,
    };

    void initBaseMtx();
    void setBaseMtx();
    int create1st();
    void event_proc_call();
    void actionWait();
    void actionOrderEvent();
    void actionTalkEvent();
    void actionWarpEvent();
    void actionOrderEventDst();
    void actionWarpEventDst();
    void actionDead();
    void demoProc();
    bool checkTalkDistance();
    void calcMidnaWaitPos();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    u8 getNameArg() { return argument; }
    u8 getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwNo2() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getArg0() { return fopAcM_GetParamBit(this, 16, 8); }
    u8 getMyPortalID() { return fopAcM_GetParamBit(this, 24, 7); }

    void setAction(u8 i_action) { mAction = i_action; }
    u16 getMsgID() { return mMsgID; }

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ mDoExt_bckAnm* mpBck;
    /* 0x5B0 */ mDoExt_btkAnm* mpBtk;
    /* 0x5B4 */ dMsgFlow_c mMsgFlow;
    /* 0x600 */ JPABaseEmitter* mEfEmitter[5];
    /* 0x614 */ cXyz field_0x614;
    /* 0x620 */ cXyz mMidnaWaitPos;
    /* 0x62C */ f32 mTalkRange;
    /* 0x630 */ int mStaffId;
    /* 0x634 */ s16 mEventId;
    /* 0x636 */ u16 mMsgID;
    /* 0x638 */ u8 mPrmInit;
    /* 0x639 */ u8 field_0x639;
    /* 0x63A */ u8 field_0x63a;
    /* 0x63B */ u8 mAction;
    /* 0x63C */ u8 mTimer;
};

STATIC_ASSERT(sizeof(daObjWarpOBrg_c) == 0x640);


#endif /* D_A_OBJ_WARP_OBRG_H */
