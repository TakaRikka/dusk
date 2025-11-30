#ifndef D_A_OBJ_BMSHUTTER_H
#define D_A_OBJ_BMSHUTTER_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjBmSh_c
 * @brief Boomerang Shutter
 *
 * @details
 *
 */
class daObjBmSh_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int Execute(Mtx**);
    int checkSw();
    int checkAnySw();
    int checkMySw();
    void event_proc_call();
    void actionWait();
    void actionOrderEvent();
    void actionEvent();
    void actionDead();
    void mode_proc_call();
    void modeWait();
    void modeMove();
    void modeEnd();
    int Draw();
    int Delete();

    u32 getEvNo() { return fopAcM_GetParamBit(this, 16, 8); }
    u32 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    u32 getSwbit2() { return fopAcM_GetParamBit(this, 8, 8); }
    int getSwbit3() { return mHomeX & 0xff; }

    u32 getSwNum() { return fopAcM_GetParamBit(this, 24, 4); }
    u32 getShtrNum() { return fopAcM_GetParamBit(this, 28, 2); }

    void setAction(u8 action) { mAction = action; }
    void setMoveMode(u8 mode) { mMoveMode = mode; }

private:
    /* 0x5a0 */ request_of_phase_process_class mPhase;
    /* 0x5a8 */ J3DModel* mModel;
    /* 0x5ac */ s16 mEventIdx;
    /* 0x5ae */ u8 mEvent;
    /* 0x5af */ u8 mAction;
    /* 0x5b0 */ u8 mMoveMode;
    /* 0x5b1 */ u8 mCurrentlyAnySw;
    /* 0x5b4 */ f32 mHeight;
    /* 0x5b8 */ f32 mShakeIntensity;
    /* 0x5bc */ csXyz mRotation;
    /* 0x5c4 */ int mTickCounter;
    /* 0x5c8 */ u16 mHomeX;
    /* 0x5ca */ u16 mHomeZ;
    /* 0x5cc */ bool mIsInitialized;
    /* 0x5cd */ u8 mCurrentlyMySw;
    /* 0x5d0 */ cXyz mSoundPosition;
};

STATIC_ASSERT(sizeof(daObjBmSh_c) == 0x5dc);

enum daObjBmSh_Action {
    daObjBmSh_Action_WAIT_e = 0,
    daObjBmSh_Action_ORDER_EVENT_e = 1,
    daObjBmSh_Action_EVENT_e = 2,
    daObjBmSh_Action_DEAD_e = 3,
};

enum daObjBmSh_MoveMode {
    daObjBmSh_MoveMode_WAIT_e = 0,
    daObjBmSh_MoveMode_MOVE_e = 1,
    daObjBmSh_MoveMode_END_e = 2,
};

#endif /* D_A_OBJ_BMSHUTTER_H */
