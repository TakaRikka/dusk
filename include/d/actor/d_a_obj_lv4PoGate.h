#ifndef D_A_OBJ_LV4POGATE_H
#define D_A_OBJ_LV4POGATE_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @ingroup actors-objects
 * @class daLv4PoGate_c
 * @brief Arbiter's Grounds Poe Gate
 *
 * @details
 *
 */
class daLv4PoGate_c : public dBgS_MoveBgActor {
public:
    enum Mode_e {
        MODE_WAIT_e,
        MODE_MOVE_OPEN_e,
        MODE_MOVE_CLOSE_e,
        MODE_MOVE_CLOSE_WAIT_e,
        MODE_MOVE_CLOSE2_e,
        MODE_MOVE_CLOSE2_WAIT_e,
        MODE_MOVE_CLOSE3_e,
    };

    void setBaseMtx();
    int create();
    void moveGate();
    void init_modeWait();
    void modeWait();
    void init_modeMoveOpen();
    void modeMoveOpen();
    void init_modeMoveClose();
    void modeMoveClose();
    void init_modeMoveCloseWait();
    void modeMoveCloseWait();
    void init_modeMoveClose2();
    void modeMoveClose2();
    void init_modeMoveClose2Wait();
    void modeMoveClose2Wait();
    void init_modeMoveClose3();
    void modeMoveClose3();
    void setSe();
    void setEffect(int);

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    int getSw() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mMode;
    /* 0x5AD */ u8 mSw;
    /* 0x5AE */ u8 mInitMove;
    /* 0x5B0 */ f32 mMoveTarget;
    /* 0x5B4 */ f32 mMoveValue;
    /* 0x5B8 */ u8 mCloseWaitTime;
};

STATIC_ASSERT(sizeof(daLv4PoGate_c) == 0x5BC);

class daLv4PoGate_HIO_c : public mDoHIO_entry_c {
public:
    daLv4PoGate_HIO_c();
    virtual ~daLv4PoGate_HIO_c() {};

    void genMessage(JORMContext*);

    /* 0x04 */ f32 mOpenSpeed;
    /* 0x08 */ f32 mCloseStep1Speed;
    /* 0x0C */ f32 mCloseStep2Speed;
    /* 0x10 */ f32 mCloseStep1Amount;
    /* 0x14 */ f32 mCloseStep2Amount;
    /* 0x18 */ u8 mCloseStep1Wait;
    /* 0x19 */ u8 mCloseStep2Wait;
    /* 0x1C */ f32 mCloseStep3Speed;
    /* 0x20 */ f32 mCloseStep3Max;
    /* 0x24 */ u8 mShockStrength;
};

#endif /* D_A_OBJ_LV4POGATE_H */
