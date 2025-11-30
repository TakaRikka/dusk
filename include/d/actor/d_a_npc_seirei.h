#ifndef D_A_NPC_SEIREI_H
#define D_A_NPC_SEIREI_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Seirei_c
 * @brief Light Spirit Ordona
 *
 * @details
 *
*/

struct daNpc_Seirei_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 force_talk_dist;     // 強制会話距離 - Force Talk Distance
    /* 0x90 */ f32 talk_dist;           // 会話距離 - Talk Distance
};

class daNpc_Seirei_HIO_c : public mDoHIO_entry_c {
    /* 0x8 */ daNpc_Seirei_HIOParam param;
};

class daNpc_Seirei_Param_c {
public:
    virtual ~daNpc_Seirei_Param_c() {}

    static daNpc_Seirei_HIOParam const m;
};

class daNpc_Seirei_c : public daNpcT_c {
public:
    typedef int (daNpc_Seirei_c::*actionFunc)(void*);
    typedef BOOL (daNpc_Seirei_c::*cutFunc)(int);

    ~daNpc_Seirei_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    u8 getType();
    BOOL isDelete();
    void reset();
    void setParam();
    BOOL checkChangeEvt();
    void srchActors();
    BOOL evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void setAttnPos();
    int drawDbgInfo();
    bool afterSetMotionAnm(int, int, f32, int);
    BOOL selectAction();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    void ctrlWaitAnm();
    void setPrtcls();
    BOOL chkFirstMeeting();
    BOOL cutConversation(int);
    int wait(void*);
    int talk(void*);
    daNpc_Seirei_c(
            daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
            daNpcT_motionAnmData_c const* i_motionAnmData,
            daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
            int i_faceMotionStepNum,
            daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData,
            int i_motionStepNum,
            daNpcT_evtData_c const* i_evtData,
            char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
        i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
        i_arcNames) {}
    
    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xffff) {
            return -1;
        }

        return nodeNo;
    }
    u32 getBitSW() { return (fopAcM_GetParam(this) & 0xFF000) >> 12; }
    bool getDoBtnChkFlag() { return (fopAcM_GetParam(this) & 0x100) == 0; }

    static char* mCutNameList[2];
    static cutFunc mCutList[2];

private:
    /* 0xE40 */ daNpc_Seirei_HIO_c* mHIO;
    /* 0xE44 */ u8 mType;
    /* 0xE45 */ u8 arg0;
    /* 0xE48 */ actionFunc mNextAction;
    /* 0xE54 */ actionFunc mAction;
    /* 0xE60 */ int field_0xe60;
    /* 0xE64 */ u32 mPrtcls[7];
    /* 0xE80 */ bool field_0xe80;
    /* 0xE81 */ u8 field_0xe81[0xe84 - 0xe81];
    /* 0xE84 */ u8 field_0xe84;
};

STATIC_ASSERT(sizeof(daNpc_Seirei_c) == 0xe88);

#endif /* D_A_NPC_SEIREI_H */
