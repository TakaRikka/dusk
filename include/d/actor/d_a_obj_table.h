#ifndef D_A_OBJ_TABLE_H
#define D_A_OBJ_TABLE_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_msg_flow.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjTable_c
 * @brief Table
 *
 * @details
 *
 */
class daObjTable_c : public dBgS_MoveBgActor {
public:
    inline int CreateHeap();
    inline int Create();
    inline int Execute(Mtx**);
    inline int Draw();
    inline int Delete();

    inline void initBaseMtx();
    inline void setBaseMtx();
    inline int create();

    s16 getMessageNo() { return fopAcM_GetParam(this) & 0xFFFF; }

private:
    /* 0x5A0 */ J3DModel* mpModel;
    /* 0x5A4 */ request_of_phase_process_class mPhaseReq;
    /* 0x5AC */ dMsgFlow_c mMsgFlow;
    /* 0x5F8 */ u32 mShadowKey;
    /* 0x5FC */ s16 mEventID;
};

STATIC_ASSERT(sizeof(daObjTable_c) == 0x600);

#endif /* D_A_OBJ_TABLE_H */
