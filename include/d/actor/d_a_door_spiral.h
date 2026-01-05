#ifndef D_A_DOOR_SPIRAL_H
#define D_A_DOOR_SPIRAL_H

#include "f_op/f_op_actor_mng.h"

class daSpiral_c;
class dSpiral_stop_c {
public:
    int create(daSpiral_c*);
    int remove(daSpiral_c*);
    void calcMtx(daSpiral_c*);
    void openInit(daSpiral_c*);
    BOOL openProc(daSpiral_c*);
    void closeInit(daSpiral_c*);
    BOOL closeProc(daSpiral_c*);
    void draw(daSpiral_c*);
    BOOL regist(daSpiral_c*);
    BOOL release(daSpiral_c*);

    /* 0x00 */ J3DModel* mpModel;
    /* 0x04 */ dBgW* mpBgw;
    /* 0x08 */ u8 field_0x08[0x38 - 0x08];
    /* 0x38 */ Mtx mBgMtx;
    /* 0x68 */ f32 mHeight;
    /* 0x6C */ f32 mHeightStep;
    /* 0x70 */ u8 field_0x70;
    /* 0x71 */ u8 field_0x71;
    /* 0x72 */ u8 field_0x72;
};

class daSpiral_c : public fopAc_ac_c {
public:
    enum daSpiral_Type_e {
        daSpiral_TYPE_DOWN_e,
        daSpiral_TYPE_UP_e,
    };

    enum daSpiral_DemoAction_e {
        daSpiral_DEMOACT_WAIT_e,
        daSpiral_DEMOACT_START_e,
        daSpiral_DEMOACT_SETGOAL_e,
        daSpiral_DEMOACT_SETSTART_e,
        daSpiral_DEMOACT_SETPOS_e,
        daSpiral_DEMOACT_L_SET_e,
        daSpiral_DEMOACT_SETROOM_e,
        daSpiral_DEMOACT_SETROOM2_e,
        daSpiral_DEMOACT_RUN_e,
        daSpiral_DEMOACT_STOP_OPEN_e,
        daSpiral_DEMOACT_STOP_CLOSE_e,
        daSpiral_DEMOACT_END_CHECK_e,
    };

    enum daSpiral_Action_e {
        daSpiral_ACT_INIT_e,
        daSpiral_ACT_WAIT_e,
        daSpiral_ACT_DEMO_e,
        daSpiral_ACT_STOP_CLOSE_e,
    };

    daSpiral_c() {}
    ~daSpiral_c() {}

    const char* getAlwaysArcName();
    const char* getSpiralDzbName(int);
    const char* getDzb();
    const char* getBmdArcName();
    const char* getEvArcName();
    const char* getBmd(int);
    const char* getBmd2(int);
    const char* getStopBmd();
    const char* getStopBmd2();
    const char* getStopDzb();

    J3DModelData* getModelData();
    J3DModelData* getStopModelData();
    bool debugCheckParam();
    int create();
    int CreateHeap();
    int CreateInit();
    void makeEventId(int);
    void setEventPrm();
    void settingStartRoom();
    void settingEndRoom();
    void EndCommon();
    BOOL checkArea();
    void calcMtx();
    void initProc(int);
    void initOpenDemo(int);
    int getDemoAction();
    int demoProc();
    int actionWait();
    int actionDemo();
    int actionStopClose();
    int actionInit();
    void setPartner();
    void clrPartner();
    void setGoal();
    void setNextSpiral();
    BOOL RunPlayerSpiral();
    u8 frontCheck();
    int checkExecute();
    int execute();
    BOOL adjoinPlayer();
    BOOL drawCheck_local();
    BOOL drawCheck(int);
    void debugDraw();
    int draw();
    int Delete();
    BOOL checkMakeStop();
    int checkStopF();
    void setStopDemo();
    BOOL checkStopClose();
    void setStop();
    BOOL checkStopOpen();

    void setAction(u8 i_action) { mAction = i_action; }

    /* 0x570 */ request_of_phase_process_class mAlwaysResPhase;
    /* 0x578 */ request_of_phase_process_class mBmdResPhase;
    /* 0x580 */ request_of_phase_process_class mEvResPhase;
    /* 0x588 */ J3DModel* mpModel;
    /* 0x58C */ dBgW* mpSpiralDzb;
    /* 0x590 */ Mtx mSpiralBgMtx;
    /* 0x5C0 */ dBgW* mDoorDarkDzb;
    /* 0x5C4 */ Mtx mDoorDarkBgMtx;
    /* 0x5F4 */ u8 mType;
    /* 0x5F5 */ s8 field_0x5f5;
    /* 0x5F6 */ u8 field_0x5f6;
    /* 0x5F7 */ u8 mAction;
    /* 0x5F8 */ int field_0x5f8;
    /* 0x5FC */ s16 mEventIds[8];
    /* 0x60C */ u8 mMapToolIds[8];
    /* 0x614 */ u8 mDemoMode;
    /* 0x618 */ int mStaffId;
    /* 0x61C */ s8 mRoomNo;
    /* 0x61D */ u8 mStartRoomNo;
    /* 0x61E */ u8 mEndRoomNo;
    /* 0x620 */ cXyz field_0x620;
    /* 0x62C */ u8 field_0x62c;
    /* 0x62D */ u8 mTimer;
    /* 0x630 */ cXyz field_0x630;
    /* 0x63C */ dSpiral_stop_c mStop;
    /* 0x6B0 */ u8 field_0x6B0[0x6B4 - 0x6B0];
    /* 0x6B4 */ s16 mPlRunPointNo;

    #if VERSION == VERSION_SHIELD_DEBUG
    /* 0x6B8 */ cXyz field_0x6b8[2];
    /* 0x6D0 */ u8 mIsCheckParam;
    #endif
};

#endif /* D_A_DOOR_SPIRAL_H */
