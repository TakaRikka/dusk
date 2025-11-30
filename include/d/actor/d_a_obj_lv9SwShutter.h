#ifndef D_A_OBJ_LV9SWSHUTTER_H
#define D_A_OBJ_LV9SWSHUTTER_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daLv9SwShutter_c
 * @brief Hyrule Castle Switch Shutter
 *
 * @details
 *
 */
class daLv9SwShutter_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int create();
    void moveShutter();
    void init_modeWait();
    void modeWait();
    void init_modeOpen();
    void modeOpen();
    void init_modeClose();
    void modeClose();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    int getSw() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mMode;
    /* 0x5AD */ u8 mSwbit;
    /* 0x5AE */ u8 mIsSwitch;
    /* 0x5B0 */ f32 field_0x5b0;
};

STATIC_ASSERT(sizeof(daLv9SwShutter_c) == 0x5b4);

#endif /* D_A_OBJ_LV9SWSHUTTER_H */
