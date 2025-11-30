#ifndef D_A_OBJ_LV4SLIDEWALL_H
#define D_A_OBJ_LV4SLIDEWALL_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daSldWall_c
 * @brief Arbiter's Grounds Sliding Wall
 *
 * @details
 *
 */
class daSldWall_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int create();
    void moveWall();
    void init_modeWait();
    void modeWait();
    void init_modeMoveOpen();
    void modeMoveOpen();
    void init_modeMoveClose();
    void modeMoveClose();
    void setSe();

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

STATIC_ASSERT(sizeof(daSldWall_c) == 0x5b4);

class daSldWall_HIO_c : public mDoHIO_entry_c {
public:
    daSldWall_HIO_c();
    virtual ~daSldWall_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x08 */ f32 move_amount;
    /* 0x0C */ f32 speed;
    /* 0x10 */ u8 vibration;
};


#endif /* D_A_OBJ_LV4SLIDEWALL_H */
