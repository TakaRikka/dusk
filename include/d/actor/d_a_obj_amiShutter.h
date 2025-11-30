#ifndef D_A_OBJ_AMISHUTTER_H
#define D_A_OBJ_AMISHUTTER_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_event_lib.h"
#include "f_op/f_op_actor_mng.h"


/**
 * @ingroup actors-objects
 * @class daAmiShutter_c
 * @brief Drain Gate
 *
 * @details
 *
 */

class daAmiShutter_c : public dBgS_MoveBgActor, public request_of_phase_process_class, public dEvLib_callback_c {
public:
    typedef void (daAmiShutter_c::*modeProc)();

    enum Mode {
        MODE_WAIT = 0,
        MODE_WAIT_EVENT = 1,
        MODE_CLOSE = 2,
        MODE_CLOSE_EVENT = 3,
        MODE_CLOSE_END = 4,
        MODE_OPEN = 5
    };

    daAmiShutter_c(): dEvLib_callback_c(this) {}
    ~daAmiShutter_c() {}

    void setBaseMtx();
    int CreateHeap();
    cPhs__Step create();
    int Execute(Mtx**);
    void moveShutter();
    BOOL playerAreaCheck();
    void init_modeWait();
    void modeWait();
    void init_modeWaitEvent();
    void modeWaitEvent();
    bool eventStart();
    void init_modeClose();
    void modeClose();
    void init_modeCloseEvent();
    void modeCloseEvent();
    void init_modeCloseEnd();
    void modeCloseEnd();
    void init_modeOpen();
    void modeOpen();
    int Draw();
    int Delete();

    
    u8 getType() { return fopAcM_GetParamBit(this, 16, 4); }

    u8 getSwBit() { return fopAcM_GetParamBit(this, 8, 8); }

    int getEvent() { return fopAcM_GetParamBit(this, 0, 8); }

    bool isShutterOpen() { return mOpen; }

private:
    /* 0x5b8 */ request_of_phase_process_class mPhaseReq;
    /* 0x5c0 */ J3DModel* mpModel;
    /* 0x5c4 */ u8 mMode;
    /* 0x5c5 */ u8 mType;
    /* 0x5c6 */ u8 mSwBit;
    /* 0x5c7 */ u8 mSwitch;
    /* 0x5c8 */ f32 mPosZ;
    /* 0x5cc */ cXyz mPos;
    /* 0x5d8 */ u8 mWaitTime;
    /* 0x5d9 */ bool mOpen;
};

STATIC_ASSERT(sizeof(daAmiShutter_c) == 0x5dc);

#endif /* D_A_OBJ_AMISHUTTER_H */
