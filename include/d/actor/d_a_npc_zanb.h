#ifndef D_A_NPC_ZANB_H
#define D_A_NPC_ZANB_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_zanB_c
 * @brief Zant Boss (Unused?)
 *
 * @details
 *
*/

struct daNpc_zanB_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
};

class daNpc_zanB_Param_c {
public:
    virtual ~daNpc_zanB_Param_c() {}

    static daNpc_zanB_HIOParam const m;
};

#if DEBUG
class daNpc_zanB_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_zanB_HIO_c();
    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    /* 0x8 */ daNpc_zanB_HIOParam m;
};
#define NPC_ZANB_HIO_CLASS daNpc_zanB_HIO_c
#else
#define NPC_ZANB_HIO_CLASS daNpc_zanB_Param_c
#endif

class daNpc_zanB_c : public daNpcT_c {
public:
    typedef int (daNpc_zanB_c::*actionFunc)(void*);
    typedef int (daNpc_zanB_c::*cutFunc)(int);
    
    ~daNpc_zanB_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    BOOL isDelete();
    void reset();
    void afterJntAnm(int);
    BOOL checkChangeEvt();
    void setParam();
    void setAfterTalkMotion();
    void srchActors();
    BOOL evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void setAttnPos();
    void setCollision();
    int drawDbgInfo();
    void drawGhost();
    BOOL selectAction();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    int wait(void*);
    int talk(void*);
    int test(void*);
    daNpc_zanB_c(
                daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                i_arcNames) {
            OS_REPORT("|%06d:%x|daNpc_zanB_c -> コンストラクト\n", g_Counter.mCounter0, this);
        }
    u16 getEyeballMaterialNo() { return 4; }
    s32 getHeadJointNo() { return 3; }
    s32 getNeckJointNo() { return 2; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == 3; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == 0x12; }

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xFFFF) {
            return -1;
        }
        return nodeNo;
    }

    static char* mCutNameList[1];
    static cutFunc mCutList[1];

private:
    /* 0xE40 */ NPC_ZANB_HIO_CLASS* mHIO;
    /* 0xE44 */ dCcD_Cyl mCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF84 */ actionFunc mNextAction;
    /* 0xF90 */ actionFunc mAction;
    /* 0xF9C */ u8 field_0xf9c;
};

STATIC_ASSERT(sizeof(daNpc_zanB_c) == 0xfa0);

#endif /* D_A_NPC_ZANB_H */
