#ifndef D_A_OBJ_LV4BRIDGE_H
#define D_A_OBJ_LV4BRIDGE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_event_lib.h"

/**
 * @ingroup actors-objects
 * @class daObjLv4Brg_c
 * @brief Arbiter's Grounds Bridge
 *
 * @details
 *
 */
class daObjLv4Brg_c : public dBgS_MoveBgActor, public dEvLib_callback_c {
public:
    daObjLv4Brg_c() : dEvLib_callback_c(this) {}

    void initBaseMtx();
    void setBaseMtx();
    int create1st();
    void action();
    void mode_init_wait();
    void mode_wait();
    void mode_init_move();
    void mode_move();
    void mode_init_dead();
    void mode_dead();

    virtual ~daObjLv4Brg_c() {}
    virtual bool eventStart();
    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getEvID() { return fopAcM_GetParamBit(this, 8, 8); }

    /* 0x5B0 */ request_of_phase_process_class mPhase;
    /* 0x5B8 */ J3DModel* mpModel;
    /* 0x5BC */ f32 field_0x5bc;
    /* 0x5C0 */ u8 field_0x5c0;
    /* 0x5C1 */ u8 mMode;
    /* 0x5C4 */ cXyz field_0x5c4;
};

STATIC_ASSERT(sizeof(daObjLv4Brg_c) == 0x5d0);


#endif /* D_A_OBJ_LV4BRIDGE_H */
