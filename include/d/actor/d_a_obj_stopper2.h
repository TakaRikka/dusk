#ifndef D_A_OBJ_STOPPER2_H
#define D_A_OBJ_STOPPER2_H

#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjStopper2_c
 * @brief Door Stop
 *
 * @details
 *
 */
class daObjStopper2_c : public fopAc_ac_c {
public:
    J3DModelData* getStopModelData();
    void initBaseMtx();
    void setBaseMtx();
    f32 getMaxOffsetY();
    int Create();
    int CreateHeap();
    int create();
    int execute();
    void event_proc_call();
    void actionWait();
    void actionOrderEvent();
    void actionEvent();
    void actionDead();
    int demoProc();
    int draw();
    int _delete();

    u32 getEvId() { return fopAcM_GetParamBit(this, 8, 8); }
    u32 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    void setAction(u8 i_action) { mAction = i_action; }

private:
    /* 0x568 */ request_of_phase_process_class mPhaseReq;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ f32 mOffsetY;
    /* 0x578 */ int mStaffId;
    /* 0x57C */ s16 mEventIdx[4];
    /* 0x584 */ u8 field_0x584;
    /* 0x585 */ u8 mTool;
    /* 0x586 */ u8 mAction;
    /* 0x587 */ u8 mTimer;
    /* 0x588 */ u8 field_0x588;
};

STATIC_ASSERT(sizeof(daObjStopper2_c) == 0x58C);


#endif /* D_A_OBJ_STOPPER2_H */
