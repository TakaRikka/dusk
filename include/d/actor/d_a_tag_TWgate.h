#ifndef D_A_TAG_TWGATE_H
#define D_A_TAG_TWGATE_H

#include "d/d_msg_flow.h"
#include "f_op/f_op_actor_mng.h"
#include "SSystem/SComponent/c_phase.h"

class daTagTWGate_c;
typedef void (daTagTWGate_c::*actionFunc)();

class daTagTWGate_c : public fopAc_ac_c {
public:
    enum EType {
        /* 0x0 */ TYPE_FILONE,
        /* 0x1 */ TYPE_ORDIN,
        /* 0x2 */ TYPE_RANAIL,
        /* 0x3 */ TYPE_HYRAL,
    };

    enum EAction {
        /* 0x00 */ ACT_WAIT,
        /* 0x01 */ ACT_DEMO_FILONE_1,
        /* 0x02 */ ACT_DEMO_FILONE_2,
        /* 0x03 */ ACT_DEMO_FILONE_3,
        /* 0x04 */ ACT_DEMO_ORDIN_1,
        /* 0x05 */ ACT_DEMO_ORDIN_2,
        /* 0x06 */ ACT_DEMO_ORDIN_3,
        /* 0x07 */ ACT_DEMO_RANAIL_1,
        /* 0x08 */ ACT_DEMO_RANAIL_2,
        /* 0x09 */ ACT_DEMO_RANAIL_3,
        /* 0x0A */ ACT_DEMO_HYRAL_1,
        /* 0x0B */ ACT_DEMO_HYRAL_2,
        /* 0x0C */ ACT_DEMO_HYRAL_3,
    };

    inline ~daTagTWGate_c();

    void initWait();
    void executeWait();
    void initDemoFilone1();
    void executeDemoFilone1();
    void initDemoFilone2();
    void executeDemoFilone2();
    void initDemoFilone3();
    void executeDemoFilone3();
    void initDemoOrdin1();
    void executeDemoOrdin1();
    void initDemoOrdin2();
    void executeDemoOrdin2();
    void initDemoOrdin3();
    void executeDemoOrdin3();
    void initDemoRanail1();
    void executeDemoRanail1();
    void initDemoRanail2();
    void executeDemoRanail2();
    void initDemoRanail3();
    void executeDemoRanail3();
    void initDemoHyral1();
    void executeDemoHyral1();
    void initDemoHyral2();
    void executeDemoHyral2();
    void initDemoHyral3();
    void executeDemoHyral3();
    void initBaseMtx();
    int downloadModels();
    void initTalk(int, fopAc_ac_c**);
    bool talkProc(int*, int, fopAc_ac_c**);
    static int createHeapCallBack(fopAc_ac_c*);
    int CreateHeap();

    u8 getSwitch() { return fopAcM_GetParam(this) >> 8; }
    u8 getType() { return fopAcM_GetParam(this); }

    inline int create();

    void create_init() {
        field_0x5e0 = 0;
        mActionID = 0;
        mAction = &ActionTable[mActionID][0];
        (this->**mAction)();
    }

    void callExecute() {
        (this->*mAction[1])();
        mDoAud_seStartLevel(Z2SE_OBJ_DARK_GATE, &current.pos, 0, 0);
    }

    int execute() {
        callExecute();

        if (field_0x5e0 != 0) {
            mpMorf->play(0, 0);
            mpMorf->modelCalc();
        }
        return 1;
    }

    int draw() {
        if (field_0x5e0 != 0) {
            mpMorf->entryDL();
        }
        return 1;
    }

    void callInit() {
        (this->*(*mAction))();
    }

    void setAction(EAction action) {
        mActionID = action;
        mAction = ActionTable[mActionID];
        callInit();
    }

    static u8 const mAttr[1];
    static actionFunc ActionTable[13][2];

private:
    /* 0x568 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x56C */ request_of_phase_process_class mPhaseZevArc;
    /* 0x574 */ request_of_phase_process_class mPhaseMdRes;
    /* 0x57C */ request_of_phase_process_class mPhasePyRes;
    /* 0x584 */ actionFunc* mAction;
    /* 0x588 */ int mActionID;
    /* 0x58C */ dMsgFlow_c mMsgFlow;
    /* 0x5D8 */ u8 field_0x5d8[4];
    /* 0x5DC */ s16 mEventID;
    /* 0x5DE */ u8 field_0x5de;
    /* 0x5DF */ bool mIsWolf;
    /* 0x5E0 */ u8 field_0x5e0;
    /* 0x5E1 */ u8 field_0x5e1;
    /* 0x5E2 */ u8 mType;
};

#endif /* D_A_TAG_TWGATE_H */
