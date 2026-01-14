#ifndef D_A_NPC_SEIC_H
#define D_A_NPC_SEIC_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_seiC_c
 * @brief Eldin Spirit
 *
 * @details
 *
 */

 struct daNpc_seiC_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 field_0x8c;  // "強制会話距離" "Forced conversation distance" | Slider
    /* 0x90 */ f32 field_0x90;  // "会話距離" "Conversation distance" | Slider
};

class daNpc_seiC_Param_c {
public:
    virtual ~daNpc_seiC_Param_c() {}

    static const daNpc_seiC_HIOParam m;
};

#if DEBUG
class daNpc_seiC_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_seiC_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);

    void genMessage(JORMContext*);

    /* 0x08 */ daNpc_seiC_HIOParam m;
};

#define NPC_SEIC_HIO_CLASS daNpc_seiC_HIO_c
#else
#define NPC_SEIC_HIO_CLASS daNpc_seiC_Param_c
#endif

class daNpc_seiC_c : public daNpcT_c {
public:
    typedef int (daNpc_seiC_c::*cutFunc)(int);
    typedef int (daNpc_seiC_c::*actionFunc)(void*);
    

    enum Type {
        TYPE_0,
        TYPE_1,
    };

    ~daNpc_seiC_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    u8 getType();
    u32 getFlowNodeNo();
    int isDelete();
    void reset();
    void setParam();
    void srchActors();
    BOOL evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void setAttnPos();
    int drawDbgInfo();
    bool afterSetMotionAnm(int, int, f32, int);
    int selectAction();
    int chkAction(int (daNpc_seiC_c::*)(void*));
    int setAction(int (daNpc_seiC_c::*)(void*));
    void ctrlWaitAnm();
    int wait(void*);
    int talk(void*);
    daNpc_seiC_c(daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData, daNpcT_motionAnmData_c const* i_motionAnmData,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData, int i_faceMotionStepNum,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
                                daNpcT_evtData_c const* i_evtData, char** i_arcNames)         
                                : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                                           i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                                           i_arcNames) {};

    static char* mCutNameList;
    static cutFunc mCutList[1];

private:
    /* 0xE40 */ NPC_SEIC_HIO_CLASS* mpHIO;
    /* 0xE44 */ u8 mType;
    /* 0xE45 */ u8 field_0xe45[0xe48 - 0xe45];
    /* 0xE48 */ actionFunc mAction;
    /* 0xE54 */ actionFunc mAction2;
    /* 0xE60 */ int field_0xe60;
};

STATIC_ASSERT(sizeof(daNpc_seiC_c) == 0xe64);

#endif /* D_A_NPC_SEIC_H */
