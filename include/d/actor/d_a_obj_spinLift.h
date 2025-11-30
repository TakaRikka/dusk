#ifndef D_A_OBJ_SPINLIFT_H
#define D_A_OBJ_SPINLIFT_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daSpinLift_c
 * @brief Spinner Lift
 *
 * @details
 *
 */
class daSpinLift_c : public dBgS_MoveBgActor {
public:
    typedef void (daSpinLift_c::*modeFunc)();

    void setBaseMtx();
    int create();
    void moveLift();
    void init_modeWait();
    void modeWait();
    void init_modeUpMove();
    void modeUpMove();
    void init_modeDownMove();
    void modeDownMove();
    void init_modeMoveEnd();
    void modeMoveEnd();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    u8 getModelType() { return current.angle.x; }
    int getMoveHeight() { return fopAcM_GetParamBit(this, 16, 8); }
    int getMoveSpeed() { return fopAcM_GetParamBit(this, 24, 8); }
    int getSw() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getTimer() { return (current.angle.x & 0xff00) >> 8; }
    int getEndSw() { return fopAcM_GetParamBit(this, 8, 8); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 field_0x5ac;
    /* 0x5B0 */ f32 mMoveHeight;
    /* 0x5B4 */ f32 mMoveSpeed;
    /* 0x5B8 */ u8 field_0x5b8;
    /* 0x5B9 */ u8 mTimer;
    /* 0x5BA */ u8 field_0x5ba;
    /* 0x5BB */ u8 mModelType;
    /* 0x5BC */ u8 field_0x5bc;
};

STATIC_ASSERT(sizeof(daSpinLift_c) == 0x5c0);

#endif /* D_A_OBJ_SPINLIFT_H */
