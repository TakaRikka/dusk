#ifndef D_A_OBJ_LV4FLOOR_H
#define D_A_OBJ_LV4FLOOR_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjLv4Floor_c
 * @brief Arbiter's Grounds Floor
 *
 * @details
 *
 */
class daObjLv4Floor_c : public dBgS_MoveBgActor {
public:
    enum Action_e {
        MODE_WAIT_e,
        MODE_MOVE_e,
        MODE_DEAD_e,
    };

    void initBaseMtx();
    void setBaseMtx();
    int create1st();
    void action();
    void mode_wait();
    void mode_init_move();
    void mode_move();
    void mode_init_dead();
    void mode_dead();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8) & 0xFF; }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ f32 mMoveYPos;
    /* 0x5B0 */ u8 mAction;
};

STATIC_ASSERT(sizeof(daObjLv4Floor_c) == 0x5B4);

#endif /* D_A_OBJ_LV4FLOOR_H */
