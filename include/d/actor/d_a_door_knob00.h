#ifndef D_A_DOOR_KNOB00_H
#define D_A_DOOR_KNOB00_H

#include "f_op/f_op_actor.h"
#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_w.h"
#include "d/d_msg_flow.h"

/**
 * @ingroup actors-doors
 * @class daKnob20_c
 * @brief Knob Door
 *
 * @details
 *
 */
class daKnob20_c : public fopAc_ac_c {
public:
    enum {
        ACTION_INIT,
        ACTION_WAIT,
        ACTION_DEMO,
        ACTION_TALK,
        ACTION_TALK_WAIT,
        ACTION_TALK_OPEN,
        ACTION_DEAD,
        ACTION_SPECIAL_DEMO,
    };

    typedef int (daKnob20_c::*actionFunc)();

    char* getAlwaysArcName();
    char* getEvArcName();
    char* getDzb();
    char* getDummyBmd();
    char* getBmd();
    J3DModelData* getDoorModelData();
    int CreateHeap();
    void calcMtx();
    int CreateInit();
    int create();
    int checkOpenDoor(int*);
    void setActionFromFlow();
    void setEventId();
    int checkArea(f32, f32, f32);
    void setEventPrm();
    int releaseBG();
    int frontCheck();
    int getDemoAction();
    int demoProc();
    void setStart(f32, f32);
    void setAngle();
    int adjustmentProc();
    int openInit(int);
    int openProc(int);
    int openEnd(int);
    void initOpenDemo();
    void startDemoProc();
    int actionWait();
    int actionSpecialDemo();
    int actionDemo();
    int actionTalk();
    int actionTalkWait();
    int actionTalkOpen();
    int actionInit();
    int actionDead();
    int execute();
    int draw();
    int Delete();

    void setAction(u8 action) { mAction = action; }
    void offFlag(u16 flag) { field_0x60c &= ~flag; }
    void onFlag(u16 flag) { field_0x60c |= flag; }
    int checkFlag(u16 flag) { return field_0x60c & flag; }

private:
    /* 0x568 */ request_of_phase_process_class mPhase1;
    /* 0x570 */ request_of_phase_process_class mPhase2;
    /* 0x578 */ J3DModel* mModel1;
    /* 0x57C */ mDoExt_bckAnm field_0x57c;
    /* 0x598 */ J3DModel* mModel2;
    /* 0x59C */ s8 mJoint;
    /* 0x5A0 */ dBgW* field_0x5a0;
    /* 0x5A4 */ s16 field_0x5a4[7];
    /* 0x5B2 */ u8 field_0x5b2[7];
    /* 0x5B9 */ u8 field_0x5b9;
    /* 0x5BA */ u16 field_0x5ba;
    /* 0x5BC */ int field_0x5bc;
    /* 0x5C0 */ dMsgFlow_c field_0x5c0;
    /* 0x60C */ u16 field_0x60c;
    /* 0x60E */ u8 mAction;
    /* 0x60F */ u8 field_0x60f;
    /* 0x610 */ s16 field_0x610;
    /* 0x612 */ s16 field_0x612;
    /* 0x614 */ s16 field_0x614;
};

STATIC_ASSERT(sizeof(daKnob20_c) == 0x618);

class knob_param_c {
public:
    static u32 getDoorModel(fopAc_ac_c*);
    static u32 getDoorLightInf(fopAc_ac_c*);
    static u16 getMsgNo(fopAc_ac_c*);
    static u8 getExitNo(fopAc_ac_c*);
};

#endif /* D_A_DOOR_KNOB00_H */
