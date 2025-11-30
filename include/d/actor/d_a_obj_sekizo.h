#ifndef D_A_OBJ_SEKIZO_H
#define D_A_OBJ_SEKIZO_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObj_Sekizo_c
 * @brief Stone Statue
 *
 * @details
 *
 */
class daObj_Sekizo_c : public dBgS_MoveBgActor {
public:
    cPhs__Step create();

    int CreateHeap();
    int Create();
    int Delete();
    int Execute(Mtx**);
    int Draw();
    void initBaseMtx();
    void setBaseMtx();

private:
    /* 0x5A0 */ u32 field_0x5a0;
    /* 0x5A4 */ request_of_phase_process_class mPhaseReq;
    /* 0x5AC */ J3DModel* mpModel;
    /* 0x5B0 */ u8 field_0x5b0;
    /* 0x5B1 */ bool field_0x5b1;
    /* 0x5B2 */ bool field_0x5b2;
};

STATIC_ASSERT(sizeof(daObj_Sekizo_c) == 0x5B4);

class daObj_Sekizo_Param_c {
public:
    virtual ~daObj_Sekizo_Param_c() {}
};

#endif /* D_A_OBJ_SEKIZO_H */
