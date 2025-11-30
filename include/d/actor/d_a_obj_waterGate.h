#ifndef D_A_OBJ_WATERGATE_H
#define D_A_OBJ_WATERGATE_H

#include "d/d_bg_s_movebg_actor.h"
#include "m_Do/m_Do_hostIO.h"
#include "SSystem/SComponent/c_phase.h"

/**
 * @ingroup actors-objects
 * @class daWtGate_c
 * @brief Water Gate
 *
 * @details Sluice gate that player opens in 3F and 4F of Lakebed Temple.
 * Object does not control the flow of water streams in the temple, it is purely aesthetic. 
 *
 */

class daWtGate_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int CreateHeap();
    cPhs__Step create();
    int Execute(Mtx**);
    void move();
    void init_modeWait();
    void modeWait();
    void init_modeOpen();
    void modeOpen();
    void init_modeClose();
    void modeClose();
    void init_modeEnd();
    void modeEnd();
    int Draw();
    int Delete();

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mMode;
    /* 0x5AD */ u8 field_0x5AD; // Neither modified nor read; unused 
    /* 0x5AE */ u8 mPrevSwitchState;
    /* 0x5B0 */ f32 mOpenYOffset;
    /* 0x5B4 */ f32 mClosedYPos;
    /* 0x5B8 */ f32 mMaxMoveSpeed;

    enum SwitchState_e {
        CLOSED, OPENED
    };

    enum Mode_e {
        WAIT, OPEN, CLOSE, END
    };
};

STATIC_ASSERT(sizeof(daWtGate_c) == 0x5bc);

struct daWtGate_HIO_c : public mDoHIO_entry_c {
    daWtGate_HIO_c();
    ~daWtGate_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ f32 mMaxSpeed;
    /* 0x8 */ u8 field_0x8;    // Modified, but never read; unused?
    /* 0x9 */ u8 field_0x9;    // Modified, but never read; unused?
};


#endif /* D_A_OBJ_WATERGATE_H */
