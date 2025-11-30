#ifndef D_A_OBJ_DMELEVATOR_H
#define D_A_OBJ_DMELEVATOR_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjDmElevator_c
 * @brief Death Mountain Elevator
 *
 * @details
 *
 */
class daObjDmElevator_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    virtual int Create();
    int init();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    void setting_ride_flag();
    void event_sw_proc_call();
    void actionSwPauseNoneInit();
    void actionSwPauseNone();
    void actionSwPauseOrderInit();
    void actionSwPauseOrder();
    void actionSwPauseInit();
    void actionSwPause();
    void calc_top_pos();
    void mode_sw_proc_call();
    void modeSwWaitLowerInit();
    void modeSwWaitLower();
    void modeSwLowerInit();
    void modeSwLower();
    void modeSwWaitUpperInit();
    void modeSwWaitUpper();
    void modeSwUpperInit();
    void modeSwUpper();
    void event_proc_call();
    void actionWait();
    void actionOrderEvent();
    void actionEvent();
    void actionStartEvent();
    void actionMoveStart();
    void actionDead();
    int demoProc();
    void moveInit();
    int moveProc();
    int Draw();
    int Delete();

    u32 getPathID() { return fopAcM_GetParamBit(this, 8, 8); }
    void setAction(char action){mAction = action;}

    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ request_of_phase_process_class mPhaseReq2;
    /* 0x5B0 */ J3DModel* mpElevatorModel;
    /* 0x5B4 */ J3DModel* mpSwitchModel;
    /* 0x5B8 */ Z2SoundObjSimple mSoundObj;
    /* 0x5D8 */ u8 field_0x5D8[0x5DC - 0x5D8];
    /* 0x5DC */ s32 field_0x5dc;
    /* 0x5E0 */ u8 field_0x5e0;
    /* 0x5E1 */ s8 field_0x5e1;
    /* 0x5E2 */ u8 mAction;
    /* 0x5E3 */ u8 field_0x5e3;
    /* 0x5E4 */ s16 field_0x5e4;
    /* 0x5E6 */ s16 mEventIndex;
    /* 0x5E8 */ dBgW* mpBgW;
    /* 0x5EC */ Mtx mMtx;
    /* 0x61C */ cXyz mXyz;
    /* 0x628 */ u8 field_0x628;
    /* 0x629 */ u8 field_0x629;
    /* 0x62A */ u8 field_0x62a;
    /* 0x62B */ u8 field_0x62b;
    /* 0x62C */ u8 field_0x62C[0x62D - 0x62C];
    /* 0x62D */ u8 field_0x62d;
    /* 0x62E */ u8 field_0x62e;
    /* 0x62F */ u8 field_0x62f;
    /* 0x630 */ u8 field_0x630;
    /* 0x631 */ u8 field_0x631;
    /* 0x632 */ u8 field_0x632;
    /* 0x633 */ u8 field_0x633;
    /* 0x634 */ u8 field_0x634;
    /* 0x635 */ u8 field_0x635;
    /* 0x636 */ u8 field_0x636;
    /* 0x637 */ u8 field_0x637;
    /* 0x638 */ f32 field_0x638;
    /* 0x63C */ f32 field_0x63c;
    /* 0x640 */ f32 field_0x640;
};

STATIC_ASSERT(sizeof(daObjDmElevator_c) == 0x644);


#endif /* D_A_OBJ_DMELEVATOR_H */
