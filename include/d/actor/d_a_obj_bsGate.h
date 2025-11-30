#ifndef D_A_OBJ_BSGATE_H
#define D_A_OBJ_BSGATE_H

#include "SSystem/SComponent/c_phase.h"
#include "m_Do/m_Do_hostIO.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daBsGate_c
 * @brief Boss Gate
 *
 * @details
 *
 */
class daBsGate_c : public dBgS_MoveBgActor {
public:
    enum Mode {
        /* 0 */ MODE_WAIT,
        /* 1 */ MODE_OPEN,
        /* 2 */ MODE_CLOSE,
    };

    void setBaseMtx();
    int CreateHeap();
    cPhs__Step create();
    int Execute(Mtx**);
    void moveGate();
    void init_modeWait();
    void modeWait();
    void init_modeOpen();
    void modeOpen();
    void init_modeClose();
    void modeClose();
    int Draw();
    int Delete();

private:
    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mMode;
    /* 0x5AD */ u8 field_0x5ad;
    /* 0x5AE */ u8 mOpen;
    /* 0x5AF */ u8 mSide;
    /* 0x5B0 */ f32 mTargetDist;
    /* 0x5B4 */ f32 mOpenDist;
};

STATIC_ASSERT(sizeof(daBsGate_c) == 0x5B8);

class daBsGate_HIO_c : public mDoHIO_entry_c {
public:
    daBsGate_HIO_c();
    ~daBsGate_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ f32 mOpenSpeed;
    /* 0x8 */ f32 mCloseSpeed;
    /* 0xC */ u8 mShockStrength;
};

STATIC_ASSERT(sizeof(daBsGate_HIO_c) == 0x10);

#endif /* D_A_OBJ_BSGATE_H */
