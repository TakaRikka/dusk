#ifndef D_A_OBJ_LV4HSTARGET_H
#define D_A_OBJ_LV4HSTARGET_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @ingroup actors-objects
 * @class daLv4HsTarget_c
 * @brief Arbiter's Grounds Clawshot Target
 *
 * @details
 *
 */
class daLv4HsTarget_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int create();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
};

STATIC_ASSERT(sizeof(daLv4HsTarget_c) == 0x5AC);

class daLv4HsTarget_HIO_c : public mDoHIO_entry_c {
public:
    daLv4HsTarget_HIO_c();
    virtual ~daLv4HsTarget_HIO_c() {}

    void genMessage(JORMContext*);
};

#endif /* D_A_OBJ_LV4HSTARGET_H */
