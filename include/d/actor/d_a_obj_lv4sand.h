#ifndef D_A_OBJ_LV4SAND_H
#define D_A_OBJ_LV4SAND_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_event_lib.h"

/**
 * @ingroup actors-objects
 * @class daObjLv4Sand_c
 * @brief Arbiter's Grounds Sand (Stallord Arena)
 *
 * @details
 *
 */
class daObjLv4Sand_c : public dBgS_MoveBgActor, public dEvLib_callback_c {
public:
    enum {
        MODE_WAIT,
        MODE_MOVE,
        MODE_DEAD,
    };

    daObjLv4Sand_c() : dEvLib_callback_c(this) {}

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

    virtual ~daObjLv4Sand_c() {}

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    virtual bool eventStart();

    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x5B0 */ request_of_phase_process_class mPhase;
    /* 0x5B8 */ J3DModel* mpModel;
    /* 0x5BC */ mDoExt_btkAnm* mpBtk;
    /* 0x5C0 */ f32 mHeight;
    /* 0x5C4 */ u8 mMode;
};

STATIC_ASSERT(sizeof(daObjLv4Sand_c) == 0x5c8);


#endif /* D_A_OBJ_LV4SAND_H */
