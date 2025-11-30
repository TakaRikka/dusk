#ifndef D_A_NPC_KNJ_H
#define D_A_NPC_KNJ_H

#include "d/actor/d_a_npc.h"

struct daNpc_Knj_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
};

class daNpc_Knj_Param_c {
public:
    virtual ~daNpc_Knj_Param_c() {}

    static const daNpc_Knj_HIOParam m;
};

#if DEBUG
class daNpc_Knj_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Knj_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_Knj_HIOParam m;
};

#define NPC_KNJ_HIO_CLASS daNpc_Knj_HIO_c
#else
#define NPC_KNJ_HIO_CLASS daNpc_Knj_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Knj_c
 * @brief Sage
 *
 * @details
 *
 */
class daNpc_Knj_c : public daNpcT_c {
public:
    typedef int (daNpc_Knj_c::*actionFunc)(void*);
    typedef int (daNpc_Knj_c::*cutFunc)(int);

    ~daNpc_Knj_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    u8 getType();
    int getFlowNodeNo();
    BOOL isDelete();
    void reset();
    void setParam();
    void srchActors();
    BOOL evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void setAttnPos();
    BOOL drawDbgInfo();
    bool afterSetMotionAnm(int, int, f32, int);
    void drawGhost();
    int selectAction();
    BOOL chkAction(actionFunc);
    int setAction(actionFunc);
    int wait(void*);
    int talk(void*);
    daNpc_Knj_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {}

    static char* mCutNameList[1];
    static cutFunc mCutList[1];

private:
    /* 0xE40 */ NPC_KNJ_HIO_CLASS* mpHIO;
    /* 0xE44 */ u8 mType;
    /* 0xE48 */ actionFunc mNextAction;
    /* 0xE54 */ actionFunc mAction;
    /* 0xE60 */ u8 field_0xe60[0xe64 - 0xe60];
    /* 0xE64 */ int field_0xe64;
};

STATIC_ASSERT(sizeof(daNpc_Knj_c) == 0xe68);

#endif /* D_A_NPC_KNJ_H */
