#ifndef D_A_OBJ_GANONWALL2_H
#define D_A_OBJ_GANONWALL2_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjGWall2_c
 * @brief Ganon Wall 2
 *
 * @details
 *
 */
class daObjGWall2_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    u32 getEventBit1() { return fopAcM_GetParamBit(this, 0, 10); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x5B0 */ u16 mMatIdx;
    /* 0x5B2 */ cXyz mSePos;
};

STATIC_ASSERT(sizeof(daObjGWall2_c) == 0x5c0);


#endif /* D_A_OBJ_GANONWALL2_H */
