#ifndef D_A_OBJ_LV4GATE_H
#define D_A_OBJ_LV4GATE_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @ingroup actors-objects
 * @class daLv4Gate_c
 * @brief Arbiter's Grounds Gate
 *
 * @details
 *
 */
class daLv4Gate_c : public dBgS_MoveBgActor {
public:
    enum Mode_e {
        MODE_WAIT_e,
        MODE_MOVE_e,
        MODE_MOVE_END_e,
    };

    void setBaseMtx();
    int create();
    void moveGate();
    void init_modeWait();
    void modeWait();
    void init_modeMove();
    void modeMove();
    void init_modeMoveEnd();
    void modeMoveEnd();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mMode;
    /* 0x5AD */ u8 mGateOpened;
    /* 0x5AE */ u8 mInitMove;
    /* 0x5AF */ u8 mMoveType;
    /* 0x5B0 */ f32 mMoveTarget;
    /* 0x5B4 */ f32 mMoveValue;
};

class daLv4Gate_HIO_c : public mDoHIO_entry_c {
public:
    daLv4Gate_HIO_c();
    virtual ~daLv4Gate_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ f32 mMoveSpeed;
    /* 0x8 */ u8 mShockStrength;
};

#endif /* D_A_OBJ_LV4GATE_H */
