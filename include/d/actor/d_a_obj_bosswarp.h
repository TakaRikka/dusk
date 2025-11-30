#ifndef D_A_OBJ_BOSSWARP_H
#define D_A_OBJ_BOSSWARP_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_msg_flow.h"

/**
 * @ingroup actors-objects
 * @class daObjBossWarp_c
 * @brief Boss Warp
 *
 * @details
 *
 */
class daObjBossWarp_c : public fopAc_ac_c {
public:
    enum Action {
        /* 0x0 */ ACT_WAIT,
        /* 0x1 */ ACT_ORDER_EVENT,
        /* 0x2 */ ACT_EVENT,
        /* 0x3 */ ACT_WAIT_WARP,
        /* 0x4 */ ACT_ORDER_WARP_EVENT,
        /* 0x5 */ ACT_WARP_EVENT,
        /* 0x6 */ ACT_DEAD,
        /* 0x7 */ ACT_ORDER_CHK_EVENT,
        /* 0x8 */ ACT_CHK_EVENT,
        /* 0x9 */ ACT_ORDER_CANCEL_EVENT,
        /* 0xA */ ACT_CANCEL_EVENT,
    };

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    cPhs__Step create();
    void appear(int);
    void set_appear();
    void disappear(int);
    BOOL checkDistance();
    int execute();
    void event_proc_call();
    void actionWait();
    void actionOrderEvent();
    void actionEvent();
    void actionWaitWarp();
    void actionOrderWarpEvent();
    void actionWarpEvent();
    void actionDead();
    void actionOrderChkEvent();
    void actionChkEvent();
    void actionOrderCancelEvent();
    void actionCancelEvent();
    int demoProc();
    void setGoal();
    int draw();
    int _delete();

    u8 getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSceneListNo() { return fopAcM_GetParamBit(this, 8, 8); }
    bool isFirst() { return fopAcM_GetParamBit(this, 0x1b, 1); }
    void setAction(u8 action) { mAction = action; }

private:
    /* 0x568 */ request_of_phase_process_class mPhaseReq;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ mDoExt_btkAnm* mpBtkAnm[2];
    /* 0x57C */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x580 */ JPABaseEmitter* mpParticle[4];
    /* 0x590 */ u8 mAction;
    /* 0x591 */ bool field_0x591;
    /* 0x592 */ s16 mBossClearEventId;
    /* 0x594 */ u8 mBossClearMapToolId;
    /* 0x595 */ bool field_0x595;
    /* 0x596 */ s16 mBossWarpInEventId;
    /* 0x598 */ u8 mBossWarpInMapToolId;
    /* 0x599 */ u8 field_0x599;
    /* 0x59C */ int mStaffId;
    /* 0x5A0 */ int mTimer;
    /* 0x5A4 */ cXyz mYstonePos;
    /* 0x5B0 */ cXyz mYstoneTargetPos;
    /* 0x5BC */ cXyz mParticlePos;
    /* 0x5C8 */ s16 mWarpCheckEventId;
    /* 0x5CA */ u16 mCounter;
    /* 0x5CC */ f32 mScaleF;
    /* 0x5D0 */ dMsgFlow_c mMsgFlow;
    /* 0x61C */ u16 mWarpChoice;
    /* 0x61E */ s16 mWarpCancelEventId;
    /* 0x620 */ cXyz field_0x620;
    /* 0x62C */ bool mScalingUp;
};

STATIC_ASSERT(sizeof(daObjBossWarp_c) == 0x630);

#endif /* D_A_OBJ_BOSSWARP_H */
