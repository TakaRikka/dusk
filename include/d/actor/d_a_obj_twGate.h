#ifndef D_A_OBJ_TWGATE_H
#define D_A_OBJ_TWGATE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daTwGate_c
 * @brief Twilight Gate (Wall?)
 *
 * @details
 *
 */
class daTwGate_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int CreateHeap();
    int create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    int getGateType() { return fopAcM_GetParamBit(this, 4, 8); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mModel;
    /* 0x5AC */ mDoExt_btkAnm mBtk;
    /* 0x5C4 */ mDoExt_brkAnm mBrk;
    /* 0x5DC */ u8 mGateType;
    /* 0x5E0 */ cM3dGLin mLine;
};

STATIC_ASSERT(sizeof(daTwGate_c) == 0x5fc);

class daTwGate_HIO_c : public fOpAcm_HIO_entry_c {
public:
    daTwGate_HIO_c();
    ~daTwGate_HIO_c() {}

    void genMessage(JORMContext* ctx);

    /* 0x4 */ f32 mRange;
};

#endif /* D_A_OBJ_TWGATE_H */
