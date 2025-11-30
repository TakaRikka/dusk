#ifndef D_A_OBJ_WARP_KBRG_H
#define D_A_OBJ_WARP_KBRG_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_msg_flow.h"

/**
 * @ingroup actors-objects
 * @class daObjWarpKBrg_c
 * @brief Kakariko Gorge Warp Bridge
 *
 * @details
 *
 */
class daObjWarpKBrg_c : public dBgS_MoveBgActor {
public:
    enum Action_e {
        ACTION_WAIT_e,
        ACTION_ORDER_EVENT_e,
        ACTION_TALK_EVENT_e,
        ACTION_WAIT_WARP_EVENT_e,
        ACTION_WARP_EVENT_e,
        ACTION_ORDER_A_TALK_EVENT_e,
        ACTION_DEAD_e,
        ACTION_WAIT2_e,
        ACTION_ORDER_EVENT2_e,
        ACTION_WARP_EVENT2_e,
        ACTION_DEAD2_e,
    };

    void initBaseMtx();
    void setBaseMtx();
    int create1st();
    void event_proc_call();
    void orderZHintEvent();
    void actionWait();
    void actionOrderEvent();
    void actionWaitWarpEvent();
    void actionOrderATalkEvent();
    void actionTalkEvent();
    void actionWarpEvent();
    void actionDead();
    void actionWait2();
    void actionOrderEvent2();
    void actionWarpEvent2();
    void actionDead2();
    void demoProc();
    void calcObjPos();
    bool checkTalkDistance();
    void calcMidnaWaitPos();
    void setBindEffect();
    void followBindEffect();
    void setSrcEffect();
    void setDstEffect();
    void followSrcEffect();
    void followDstEffect();
    void endSrcEffect();
    void endDstEffect();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    u8 getNameArg() { return fopAcM_GetParamBit(this, 31, 2); }
    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwbit2() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getArg0() { return fopAcM_GetParamBit(this, 16, 8); }

    void setAction(u8 i_action) { mAction = i_action; }
    u16 getMsgID() { return mMsgID; }

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ request_of_phase_process_class mEvPhase;
    /* 0x5B0 */ J3DModel* mpBridgeModel;
    /* 0x5B4 */ mDoExt_btkAnm* mpBridgeBtk;
    /* 0x5B8 */ mDoExt_bckAnm* mpBridgeBck;
    /* 0x5BC */ J3DModel* mpPortalModel;
    /* 0x5C0 */ mDoExt_btkAnm* mpPortalBtk;
    /* 0x5C4 */ mDoExt_brkAnm* mpPortalBrk;
    /* 0x5C8 */ u8 field_0x5c8;
    /* 0x5C9 */ u8 mTimer;
    /* 0x5CA */ u8 mAction;
    /* 0x5CB */ u8 mPrmInit;
    /* 0x5CC */ u16 mMsgID;
    /* 0x5CE */ s16 mEventId[2];
    /* 0x5D4 */ int mStaffId;
    /* 0x5D8 */ u8 mEventType;
    /* 0x5D9 */ u8 field_0x5d9;
    /* 0x5DC */ dMsgFlow_c mMsgFlow;
    /* 0x628 */ cXyz mMidnaWaitPos;
    /* 0x634 */ cXyz mObjPos;
    /* 0x640 */ JPABaseEmitter* mStartEfEmitterID[3];
    /* 0x64C */ JPABaseEmitter* mDisappEfEmitterID[5];
    /* 0x660 */ JPABaseEmitter* mAppEfEmitterID[6];
    /* 0x678 */ JPABaseEmitter* mEndEfEmitterID[2];
    /* 0x680 */ u8 field_0x680[0x688 - 0x680];
    /* 0x688 */ JPABaseEmitter* mBindEfEmitterID[4];
    /* 0x698 */ u16 mEffJointNo;
    /* 0x69A */ u8 field_0x69a;
    /* 0x69B */ u8 field_0x69b;
};

STATIC_ASSERT(sizeof(daObjWarpKBrg_c) == 0x69c);


#endif /* D_A_OBJ_WARP_KBRG_H */
