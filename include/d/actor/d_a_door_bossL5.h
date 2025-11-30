#ifndef D_A_DOOR_BOSSL5_H
#define D_A_DOOR_BOSSL5_H

#include "dolphin/types.h"
#include "f_op/f_op_actor.h"
#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_w.h"
#include "d/d_bg_s_acch.h"
#include "f_pc/f_pc_manager.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-doors
 * @class daBdoorL5_c
 * @brief Snowpeak Ruins Boss Door
 *
 * @details
 *
 */
class daBdoorL5_c : public fopAc_ac_c {
public:
    enum ActionType {
        ACTION_WAIT,
        ACTION_CLOSE_WAIT,
        ACTION_OPEN,
        ACTION_END,
    };

    typedef int (daBdoorL5_c::*actionFunc)();

    const char* getArcName();
    const char* getAlwaysArcName();
    const char* getBmd();
    const char* getDzb();
    const char* getAnmArcName();
    s32 getOpenAnm();
    s32 getCloseAnm();
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
    void calcGoal(cXyz*, int);
    void smokeInit();
    int createKey();
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
    int deleteKey() {
        if (mKeyHoleId != -1 && fpcM_IsCreating(mKeyHoleId) == 0) {
            fopAcM_delete(mKeyHoleId);
            mKeyHoleId = -1;
        }
        return 1;
    }

private:
    /* 0x568 */ request_of_phase_process_class mPhase1;
    /* 0x570 */ request_of_phase_process_class mPhase2;
    /* 0x578 */ request_of_phase_process_class mPhase3;
    /* 0x580 */ J3DModel* field_0x580;
    /* 0x584 */ mDoExt_bckAnm* field_0x584;
    /* 0x588 */ dBgW* field_0x588;
    /* 0x58C */ u32 mKeyHoleId;
    /* 0x590 */ u8 field_0x590;
    /* 0x591 */ u8 mAction;
    /* 0x592 */ u8 field_0x592;
    /* 0x593 */ u8 field_0x593;
    /* 0x594 */ u8 field_0x594;
    /* 0x596 */ s16 field_0x596;
    /* 0x598 */ s16 field_0x598;
    /* 0x59A */ s16 field_0x59a;
    /* 0x59C */ int field_0x59c;
    /* 0x5A0 */ int field_0x5a0;
    /* 0x5A4 */ cXyz field_0x5a4;
    /* 0x5B0 */ csXyz field_0x5b0;
    /* 0x5B6 */ u16 field_0x5b6;
    /* 0x5B8 */ dBgS_ObjAcch field_0x5b8;
    /* 0x790 */ dBgS_AcchCir field_0x790;
};

STATIC_ASSERT(sizeof(daBdoorL5_c) == 0x7D0);

#endif /* D_A_DOOR_BOSSL5_H */
