#ifndef D_A_OBJ_DIGSNOW_H
#define D_A_OBJ_DIGSNOW_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"

class J3DModel;

/**
 * @ingroup actors-objects
 * @class daObjDigSnow_c
 * @brief Wolf Dig Place (Snow)
 *
 * @details
 *
 */
class daObjDigSnow_c : public dBgS_MoveBgActor {
public:
    enum Mode_e {
        /* 0 */ MODE_WAIT_e,
        /* 1 */ MODE_DIG_e,
        /* 2 */ MODE_END_e,
    };

    enum Action_e {
        /* 0 */ ACTION_WAIT_e,
        /* 1 */ ACTION_DIG_e,
        /* 2 */ ACTION_END_e,
    };

    void initBaseMtx();
    void setBaseMtx();
    virtual int Create();
    virtual int CreateHeap();
    int create1st();
    virtual int Execute(Mtx**);
    void action();
    void mode_init_wait();
    void mode_wait();
    void mode_init_dig();
    void mode_dig();
    void mode_init_end();
    void mode_end();
    virtual int Draw();
    virtual int Delete();

    u8 getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }
    void startDig() { mMode = ACTION_DIG_e; }
    void endDig() { mMode = ACTION_END_e; }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ dBgS_ObjAcch mAcch;
    /* 0x784 */ dBgS_AcchCir mAcchCir;
    /* 0x7C4 */ dCcD_Stts mStts;
    /* 0x800 */ dCcD_Cyl mCyl;
    /* 0x93C */ u8 mMode;
    /* 0x93D */ u8 mAction;
    /* 0x940 */ f32 mGroundHeight;
};  // Size: 0x944

STATIC_ASSERT(sizeof(daObjDigSnow_c) == 0x944);

#endif /* D_A_OBJ_DIGSNOW_H */
