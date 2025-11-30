#ifndef D_A_OBJ_HSTARGET_H
#define D_A_OBJ_HSTARGET_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_hostIO.h"
/**
 * @ingroup actors-objects
 * @class daHsTarget_c
 * @brief Clawshot Target
 *
 * @details
 *
 */
class daHsTarget_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int CreateHeap();
    int create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    u8 getModelType() { return fopAcM_GetParamBit(this, 0, 4); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mIndex;
};

STATIC_ASSERT(sizeof(daHsTarget_c) == 0x5B0);

class daHsTarget_HIO_c : public mDoHIO_entry_c {
public:
    daHsTarget_HIO_c();
    virtual ~daHsTarget_HIO_c() {}

    void genMessage(JORMContext*);
};

#endif /* D_A_OBJ_HSTARGET_H */
