#ifndef D_A_OBJ_SWPUSH5_H
#define D_A_OBJ_SWPUSH5_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daObjSw5_c
 * @brief Push Switch 5
 *
 * @details Mutli-purpose heavy switch actor. (Iron Boots switch)
 *
 */
class daObjSw5_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int create();
    void setting_ride_flag();
    BOOL checkPushable();
    void event_proc_call();
    void actionPauseNoneInit();
    void actionPauseNone();
    void actionPauseOrderInit();
    void actionPauseOrder();
    void actionPauseInit();
    void actionPause();
    void actionWait();
    void actionOrder();
    void actionEvent();
    void calc_top_pos();
    void mode_proc_call();
    void modeWaitLowerInit();
    void modeWaitLower();
    void modeLowerInit();
    void modeLower();
    void modeWaitUpperInit();
    void modeWaitUpper();
    void modeUpperInit();
    void modeUpper();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    u8 getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwNo2() { return 0xFF; }

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mUnkRideTimer;
    /* 0x5AD */ u8 field_0x5ad;
    /* 0x5AE */ u8 field_0x5ae;
    /* 0x5AF */ u8 field_0x5af;
    /* 0x5B0 */ u8 field_0x5b0;
    /* 0x5B1 */ u8 field_0x5b1;
    /* 0x5B2 */ u8 mIsPlayerRideHvy;
    /* 0x5B3 */ u8 mIsPlayerRideHvyPrev;
    /* 0x5B4 */ int mCounter;
    /* 0x5B8 */ dBgW* mpBgW2;
    /* 0x5BC */ Mtx mMtx;
    /* 0x5EC */ u8 field_0x5EC[0x5EE - 0x5EC];
    /* 0x5EE */ u8 mMode;
    /* 0x5EF */ u8 mAction;
    /* 0x5F0 */ u8 mPauseTimer;
    /* 0x5F4 */ f32 mTopPos;
    /* 0x5F8 */ f32 field_0x5f8;
    /* 0x5FC */ f32 field_0x5fc;
};

STATIC_ASSERT(sizeof(daObjSw5_c) == 0x600);


#endif /* D_A_OBJ_SWPUSH5_H */
