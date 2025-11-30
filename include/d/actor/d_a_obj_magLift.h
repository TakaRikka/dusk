#ifndef D_A_OBJ_MAGLIFT_H
#define D_A_OBJ_MAGLIFT_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daMagLift_c
 * @brief Magnetic Lift
 *
 * @details
 *
 */
class daMagLift_c : public dBgS_MoveBgActor {
public:
    enum Mode_e {
        MODE_ACC_e,
        MODE_MOVE_e,
        MODE_BRK_e,
        MODE_WAIT_e,
        MODE_WAIT_INIT_e,
        MODE_DEAD_e,
        MODE_MOVE_WAIT_e
    };

    void setBaseMtx();
    int CreateHeap();
    int create();
    int Execute(Mtx**);
    void moveLift();
    void modeAcc();
    void init_modeMove();
    void modeMove();
    void init_modeBrk();
    void modeBrk();
    void init_modeWaitInit();
    void modeWaitInit();
    void init_modeWait();
    void modeWait();
    void init_modeMoveWait();
    void modeMoveWait();
    void modeDead();
    void setNextPoint();
    int Draw();
    int Delete();

    int getMoveSpeed() { return fopAcM_GetParamBit(this, 0x8, 4); }

    static f32 const mSpeed[16];

private:
    /* 0x5a0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5a8 */ J3DModel* mpModel;
    /* 0x5ac */ u8 field_0x5ac [0x5ae - 0x5ac];
    /* 0x5ae */ u8 field_0x5ae;
    /* 0x5af */ s8 field_0x5af;
    /* 0x5b0 */ s16 mPoint;
    /* 0x5b4 */ cXyz field_0x5b4;
    /* 0x5c0 */ cXyz field_0x5c0;
    /* 0x5cc */ f32 mMoveSpeed;
    /* 0x5d0 */ u8 mMode;
    /* 0x5d1 */ u8 is_switch;
};

STATIC_ASSERT(sizeof(daMagLift_c) == 0x5d4);

class daMagLift_HIO_c : public mDoHIO_entry_c {
public:
    daMagLift_HIO_c();
    virtual ~daMagLift_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 field_0x5;
};


#endif /* D_A_OBJ_MAGLIFT_H */
