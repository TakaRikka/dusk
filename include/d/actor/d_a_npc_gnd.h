#ifndef D_A_NPC_GND_H
#define D_A_NPC_GND_H

#include "d/actor/d_a_npc.h"

struct daNpc_Gnd_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
};

class daNpc_Gnd_Param_c {
public:
    virtual ~daNpc_Gnd_Param_c() {}

    static const daNpc_Gnd_HIOParam m;
};

#if DEBUG
class daNpc_Gnd_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Gnd_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_Gnd_HIOParam m;
};

#define NPC_GND_HIO_CLASS daNpc_Gnd_HIO_c
#else
#define NPC_GND_HIO_CLASS daNpc_Gnd_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Gnd_c
 * @brief Ganondorf
 *
 * @details
 *
 */
class daNpc_Gnd_c : public daNpcT_c {
public:
    typedef int (daNpc_Gnd_c::*cutFunc)(int);

    enum Type {
        TYPE_0,
        TYPE_1,
    };

    ~daNpc_Gnd_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    int getFlowNodeNo();
    int isDelete();
    void reset();
    void afterJntAnm(int);
    BOOL ctrlBtk();
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
    bool afterSetMotionAnm(int, int, f32, int);
    int selectAction();
    int chkAction(int (daNpc_Gnd_c::*)(void*));
    int setAction(int (daNpc_Gnd_c::*)(void*));
    int wait(void*);
    int talk(void*);
    daNpc_Gnd_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {
        // "|%06d:%x|daNpc_Gnd_c -> Construct\n"
        OS_REPORT("|%06d:%x|daNpc_Gnd_c -> コンストラクト\n", g_Counter.mCounter0, this);
    }
    u16 getEyeballRMaterialNo() { return 3; }
    u16 getEyeballLMaterialNo() { return 2; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }

    static char* mCutNameList[1];
    static cutFunc mCutList[1];

private:
    /* 0xE40 */ NPC_GND_HIO_CLASS* mpHIO;
    /* 0xE44 */ dCcD_Cyl mCyl1;
    /* 0xF80 */ u8 mType;
    /* 0xF84 */ int (daNpc_Gnd_c::*mNextAction)(void*);
    /* 0xF90 */ int (daNpc_Gnd_c::*mAction)(void*);
    /* 0xF9C */ u8 field_0xF9C;
};

STATIC_ASSERT(sizeof(daNpc_Gnd_c) == 0xFA0);

#endif /* D_A_NPC_GND_H */
