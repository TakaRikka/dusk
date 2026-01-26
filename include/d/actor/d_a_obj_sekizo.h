#ifndef D_A_OBJ_SEKIZO_H
#define D_A_OBJ_SEKIZO_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

struct daObj_Sekizo_HIOParam {
};

class daObj_Sekizo_Param_c {
public:
    virtual ~daObj_Sekizo_Param_c() {}
    static daObj_Sekizo_HIOParam const m;
};

#if DEBUG
class daObj_Sekizo_HIO_c : public mDoHIO_entry_c {
public:
    daObj_Sekizo_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);

    void genMessage(JORMContext*);

    daObj_Sekizo_HIOParam m;
};

#define OBJ_SEKIZO_HIO_CLASS daObj_Sekizo_HIO_c
#else
#define OBJ_SEKIZO_HIO_CLASS daObj_Sekizo_Param_c
#endif

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
    cPhs_Step create();

    int CreateHeap();
    int Create();
    int Delete();
    int Execute(Mtx**);
    int Draw();
    void initBaseMtx();
    void setBaseMtx();

private:
    /* 0x5A0 */ OBJ_SEKIZO_HIO_CLASS* mpHIO;
    /* 0x5A4 */ request_of_phase_process_class mPhaseReq;
    /* 0x5AC */ J3DModel* mpModel;
    /* 0x5B0 */ u8 field_0x5b0;
    /* 0x5B1 */ bool field_0x5b1;
    /* 0x5B2 */ bool field_0x5b2;
};

STATIC_ASSERT(sizeof(daObj_Sekizo_c) == 0x5B4);

#endif /* D_A_OBJ_SEKIZO_H */
