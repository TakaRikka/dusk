#ifndef D_A_DOOR_MBOSSL1_H
#define D_A_DOOR_MBOSSL1_H

#include "dolphin/types.h"
#include "f_op/f_op_actor.h"
#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_w.h"
#include "d/d_bg_s_acch.h"

class dDoor_stop2_c {
public:
    void calcMtx(fopAc_ac_c*);
    void closeInit(fopAc_ac_c*, u8);
    int closeProc(fopAc_ac_c*);
    void openInit(fopAc_ac_c*, u8);
    int openProc(fopAc_ac_c*);
    int create(J3DModelData*);
    void draw(fopAc_ac_c*);
    f32 getHeight();

    /* 0x0 */ J3DModel* mModel;
    /* 0x4 */ f32 field_0x4;
    /* 0x8 */ u8 field_0x8;
    /* 0x9 */ u8 field_0x9;
    /* 0xA */ u8 field_0xa;
    /* 0xB */ u8 field_0xb;
};

/**
 * @ingroup actors-doors
 * @class daMBdoorL1_c
 * @brief Mini Boss Door
 *
 * @details Multi-purpose door actor. This actor is used for mini boss doors in the game.
 *
 */
class daMBdoorL1_c : public fopAc_ac_c {
public:
    enum DoorType {
        DOOR_TYPE_0,
        DOOR_TYPE_1,
        DOOR_TYPE_2,
    };

    enum ActionType {
        ACTION_INIT,
        ACTION_START_DEMO,
        ACTION_WAIT,
        ACTION_CLOSE_WAIT,
        ACTION_OPEN,
        ACTION_DEMO,
        ACTION_STOP_CLOSE,
    };

    typedef int (daMBdoorL1_c::*actionFunc)();

    char* getAnmArcName();
    char* getArcName();
    char* getAlwaysArcName();
    int getDoorType();
    char* getOpenAnm();
    char* getCloseAnm();
    char* getBmd();
    char* getBtk();
    char* getDzb();
    J3DModelData* getDoorModelData();
    int CreateHeap();
    void calcMtx();
    int createKey();
    int CreateInit();
    int create();
    int getDemoAction();
    void demoProc();
    int openInit();
    int openProc();
    int openEnd();
    int closeInit();
    int closeProc();
    int closeEnd();
    int unlockInit();
    int unlock();
    int openInitKnob(int);
    int openProcKnob(int);
    int openEndKnob(int);
    void calcGoal(cXyz*, int);
    int setAngle();
    void setAngleQuickly();
    int adjustmentProc();
    void setPos();
    void setStart(f32, f32);
    void smokeInit2();
    int checkArea();
    int checkFront();
    int checkOpen();
    int checkFrontKey();
    int checkBackKey();
    int checkFrontStop();
    int checkBackStop();
    int checkFrontSw();
    int checkBackSw();
    int checkStopOpen();
    int startDemoEnd();
    int checkMBossRoom();
    int checkStopClose();
    int checkMakeStop();
    int checkMakeKey();
    int actionWait();
    int actionCloseWait();
    int actionStopClose();
    int actionDemo();
    int actionOpen();
    int actionInit();
    int actionStartDemo();
    int execute();
    int draw();
    void draw_sub0();
    void draw_sub1();
    int Delete();

    void setAction(u8 action) {
        mAction = action;
    }

private:
    /* 0x568 */ request_of_phase_process_class mPhase1;
    /* 0x570 */ request_of_phase_process_class mPhase2;
    /* 0x578 */ request_of_phase_process_class mPhase3;
    /* 0x580 */ J3DModel* mModels[2];
    /* 0x588 */ mDoExt_bckAnm* mBckAnms[2];
    /* 0x590 */ mDoExt_bckAnm* field_0x590;
    /* 0x594 */ mDoExt_btkAnm* field_0x594;
    /* 0x598 */ dBgW* field_0x598;
    /* 0x59C */ Mtx field_0x59c;
    /* 0x5CC */ u8 mAction;
    /* 0x5CD */ u8 field_0x5cd;
    /* 0x5CE */ s16 field_0x5ce;
    /* 0x5D0 */ s16 field_0x5d0;
    /* 0x5D2 */ s16 field_0x5d2;
    /* 0x5D4 */ s16 field_0x5d4[4];
    /* 0x5DC */ u8 field_0x5dc;
    /* 0x5DE */ u8 field_0x5dd;
    /* 0x5DE */ u8 field_0x5de;
    /* 0x5DF */ u8 field_0x5df;
    /* 0x5E0 */ u8 field_0x5e0;
    /* 0x5E1 */ u8 field_0x5e1;
    /* 0x5E4 */ u32 field_0x5e4;
    /* 0x5E8 */ u32 field_0x5e8;
    /* 0x5EC */ int field_0x5ec;
    /* 0x5F0 */ u32 mStopperId;
    /* 0x5F4 */ s32 mKeyHoleId;
    /* 0x5F8 */ dDoor_stop2_c mDoorStop;
    /* 0x604 */ dBgS_ObjAcch field_0x604;
    /* 0x81C */ dBgS_AcchCir field_0x7dc;
    /* 0x81C */ cXyz field_0x81c;
    /* 0x828 */ cXyz field_0x828;
};

STATIC_ASSERT(sizeof(daMBdoorL1_c) == 0x834);

#endif /* D_A_DOOR_MBOSSL1_H */
