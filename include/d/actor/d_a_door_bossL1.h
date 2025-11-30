#ifndef D_A_DOOR_BOSSL1_H
#define D_A_DOOR_BOSSL1_H

#include "dolphin/types.h"
#include "f_op/f_op_actor.h"
#include "d/d_bg_w.h"
#include "d/d_bg_s_acch.h"
#include "SSystem/SComponent/c_phase.h"

class JPABaseEmitter;

/**
 * @ingroup actors-doors
 * @class daBdoorL1_c
 * @brief Boss Door
 *
 * @details Multi-purpose door actor. This actor is used for boss doors in the game.
 *
 */
class daBdoorL1_c : public fopAc_ac_c {
public:
    enum DoorType {
        DOOR_TYPE_0,
        DOOR_TYPE_1,
    };

    enum ActionType {
        ACTION_WAIT,
        ACTION_CLOSE_WAIT,
        ACTION_OPEN,
        ACTION_END,
    };

    typedef int (daBdoorL1_c::*actionFunc)();

    char* getDoorEventName();
    char* getDoorEventName2();
    const char* getArcName();
    const char* getAnmArcName();
    int getDoorType();
    int getOpenAnm();
    int getCloseAnm();
    const char* getAlwaysArcName();
    const char* getBmd();
    const char* getDzb();
    J3DModelData* getDoorModelData();
    int CreateHeap();
    void calcMtx();
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
    void setPos();
    void calcGoal(cXyz*, int);
    int setAngle();
    int createKey();
    void smokeInit();
    void deleteEmitter();
    int checkArea();
    int checkFront();
    int checkOpen();
    int actionWait();
    int actionCloseWait();
    int actionOpen();
    int actionEnd();
    int execute();
    int draw();
    int Delete();

    void setAction(u8 action) { mAction = action; }

private:
    /* 0x568 */ request_of_phase_process_class mPhase1;
    /* 0x570 */ request_of_phase_process_class mPhase2;
    /* 0x578 */ request_of_phase_process_class mPhase3;
    /* 0x580 */ J3DModel* field_0x580;
    /* 0x584 */ J3DModel* field_0x584;
    /* 0x588 */ mDoExt_bckAnm* field_0x588;
    /* 0x58C */ mDoExt_btkAnm* field_0x58c;
    /* 0x590 */ dBgW* field_0x590;
    /* 0x594 */ u32 mKeyHoleId;
    /* 0x598 */ u8 field_0x598;
    /* 0x599 */ u8 mAction;
    /* 0x59A */ u8 field_0x59a;
    /* 0x59B */ u8 field_0x59b;
    /* 0x59C */ s16 field_0x59c;
    /* 0x59E */ s16 field_0x59e;
    /* 0x5A0 */ int field_0x5a0;
    /* 0x5A4 */ int field_0x5a4;
    /* 0x5A8 */ f32 field_0x5a8;
    /* 0x5AC */ dBgS_ObjAcch field_0x5ac;
    /* 0x784 */ dBgS_AcchCir field_0x784;
    /* 0x7C4 */ cXyz field_0x7c4;
    /* 0x7D0 */ cXyz field_0x7d0;
    /* 0x7DC */ csXyz field_0x7dc;
    /* 0x7E4 */ JPABaseEmitter* field_0x7e4[2];
    /* 0x7EC */ Mtx field_0x7ec;
};

STATIC_ASSERT(sizeof(daBdoorL1_c) == 0x81C);

#endif /* D_A_DOOR_BOSSL1_H */
