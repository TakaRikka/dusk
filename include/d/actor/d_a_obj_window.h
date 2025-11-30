#ifndef D_A_OBJ_WINDOW_H
#define D_A_OBJ_WINDOW_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjWindow_c
 * @brief Destructable Kakariko Village Window
 *
 * @details
 *
 */
class daObjWindow_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    cPhs__Step create1st();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    u8 getType() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getLightInf() { return fopAcM_GetParamBit(this, 8, 8); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ mDoExt_bckAnm* mpBckAnm;
    /* 0x5B0 */ u8 mType;
    /* 0x5B4 */ dCcD_Stts mStts;
    /* 0x5F0 */ dCcD_Cyl mCyl;
};

STATIC_ASSERT(sizeof(daObjWindow_c) == 0x72C);

#endif /* D_A_OBJ_WINDOW_H */
