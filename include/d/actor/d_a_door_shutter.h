#ifndef D_A_DOOR_SHUTTER_H
#define D_A_DOOR_SHUTTER_H

#include "dolphin/types.h"
#include "f_op/f_op_actor.h"
#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_w.h"
#include "d/d_msg_flow.h"
#include "d/d_bg_s_acch.h"

struct daDoor20_c;

class dDoor_stop_c {
public:
    void calcMtx(daDoor20_c*);
    void closeInit(daDoor20_c*);
    int closeProc(daDoor20_c*);
    void openInit(daDoor20_c*);
    int openProc(daDoor20_c*);
    int create(daDoor20_c*);
    void draw(daDoor20_c*);

    J3DModel* field_0x0;
    f32 field_0x4;
    u8 field_0x8;
    u8 field_0x9;
    u8 field_0xa;
    u8 field_0xb;
};

/**
 * @ingroup actors-doors
 * @class daDoor20_c
 * @brief Sliding Door
 *
 * @details Multi-purpose door actor. This actor is used for vertical sliding doors in the game.
 *
 */
class daDoor20_c : public fopAc_ac_c {
public:
    enum ActionType {
        ACTION_INIT,
        ACTION_WAIT,
        ACTION_STOP_CLOSE,
        ACTION_DEMO,
    };

    char* getStopBmdName();
    J3DModelData* getStopModelData();
    char* getAlwaysArcName();
    char* getArcName();
    char* getBmdName();
    char* getBtk();
    J3DModelData* getModelData();
    f32 getSize2X();
    int CreateHeap();
    void setEventPrm();
    int checkOpenMsgDoor(int*);
    int adjustmentAngle();
    int adjustmentProc();
    void setAngle();
    void openInit_0();
    void openInit_1();
    int openInit(int);
    int openProc(int);
    void openEnd_0();
    void openEnd_1();
    int openEnd(int);
    void closeInit_0();
    void closeInit_1();
    int closeInit(int);
    int closeProc(int);
    void closeEnd_0();
    void closeEnd_1();
    int closeEnd(int);
    void openInit2();
    void closeEnd2();
    void calcMtx_2();
    void calcMtx();
    int CreateInit();
    void setKey();
    int create();
    int demoProc();
    int orderStopEvent();
    int actionWait();
    int actionStopClose();
    int actionDemo();
    int actionInit();
    int execute();
    int draw();
    void setDoorAngleSpec();
    int _delete();
    int createKey();
    int deleteKey();
    int adjoinPlayer();
    int frontCheckOld();
    int frontCheck();
    int drawCheck(int);
    int checkExecute();
    void startDemoProc();
    void makeEventId();
    void initProc();
    void initOpenDemo(int);
    int checkArea(f32, f32, f32);
    void openInitCom(int);
    void closeEndCom();
    void getRestartPos(cXyz*);
    void setGoal();
    int chkMakeStop();
    int chkStopF();
    int chkStopB();
    void setStopDemo();
    int chkStopClose();
    void setStop();
    int chkStopOpen();
    int chkMakeKey();

    virtual char* getDzbName();
    virtual int getDemoAction();

    void onFlag(u16 flag) { field_0x68e |= flag; }
    void offFlag(u16 flag) { field_0x68e &= ~flag; }
    bool checkFlag(u16 flag) { return field_0x68e & flag; }
    void setAction(u8 action) { mAction = action; }

private:
    /* 0x56C */ request_of_phase_process_class mPhase1;
    /* 0x574 */ request_of_phase_process_class mPhase2;
    /* 0x57C */ J3DModel* mModel1;
    /* 0x580 */ J3DModel* mModel2;
    /* 0x584 */ mDoExt_bckAnm field_0x584;
    /* 0x5A0 */ mDoExt_bckAnm field_0x5a0;
    /* 0x5BC */ J3DModel* field_0x5bc;
    /* 0x5C0 */ mDoExt_btkAnm* field_0x5c0;
    /* 0x5C4 */ dBgW* field_0x5c4;
    /* 0x5C8 */ s8 field_0x5c8;
    /* 0x5C9 */ bool field_0x5c9;
    /* 0x5CA */ u8 field_0x5ca[0x5dd - 0x5ca];
    /* 0x5DD */ u8 field_0x5dd;
    /* 0x5E0 */ cXyz field_0x5e0;
    /* 0x5EC */ u32 field_0x5ec;
    /* 0x5F0 */ bool field_0x5f0;
    /* 0x5F1 */ u8 field_0x5f1;
    /* 0x5F4 */ Mtx field_0x5f4;
    /* 0x624 */ dMsgFlow_c field_0x624;
    /* 0x670 */ s16 field_0x670;
    /* 0x672 */ bool field_0x672;
    /* 0x673 */ u8 field_0x673;
    /* 0x674 */ u8 field_0x674;
    /* 0x678 */ int field_0x678;
    /* 0x67C */ u8 field_0x67c;
    /* 0x67D */ s8 field_0x67d;
    /* 0x67E */ u8 field_0x67e;
    /* 0x67F */ u8 field_0x67f;
    /* 0x680 */ cXyz field_0x680;
    /* 0x68C */ u8 field_0x68c;
    /* 0x68D */ u8 field_0x68d;
    /* 0x68E */ u16 field_0x68e;
    /* 0x690 */ u8 mAction;
    /* 0x691 */ u8 field_0x691;
    /* 0x692 */ s16 field_0x692[19];
    /* 0x6B8 */ u8 field_0x6b8[19];
    /* 0x6CB */ u8 field_0x6cb;
    /* 0x6CC */ u32 field_0x6cc;
    /* 0x6D0 */ dDoor_stop_c mDoorStop;
    /* 0x6DC */ s16 field_0x6dc;
    /* 0x6DE */ dBgS_ObjAcch field_0x6de;
    /* 0x8B8 */ dBgS_AcchCir field_0x8b8;
    /* 0x8F8 */ u32 field_0x8f8;
};

STATIC_ASSERT(sizeof(daDoor20_c) == 0x8FC);

#endif /* D_A_DOOR_SHUTTER_H */
