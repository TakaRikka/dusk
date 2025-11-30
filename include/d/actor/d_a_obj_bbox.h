#ifndef D_A_OBJ_BBOX_H
#define D_A_OBJ_BBOX_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjBBox_c
 * @brief B Box
 *
 * @details
 *
 */
class daObjBBox_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    u32 getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ dCcD_Stts mStts;
    /* 0x5E8 */ dCcD_Cyl mCyl;
};

STATIC_ASSERT(sizeof(daObjBBox_c) == 0x724);

#endif /* D_A_OBJ_BBOX_H */
