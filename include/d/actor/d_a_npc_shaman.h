#ifndef D_A_NPC_SHAMAN_H
#define D_A_NPC_SHAMAN_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Sha_c
 * @brief Fanadi
 *
 * @details
 *
*/

struct daNpc_Sha_HIOParam {
    /* 0x0 */ daNpcT_HIOParam common;
};

class daNpc_Sha_Param_c {
public:
    virtual ~daNpc_Sha_Param_c() {}

    static daNpc_Sha_HIOParam const m;
};

#if DEBUG
class daNpc_Sha_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Sha_HIO_c();
    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    /* 0x8 */ daNpc_Sha_HIOParam m;
};
#define NPC_SHA_HIO_CLASS daNpc_Sha_HIO_c
#else
#define NPC_SHA_HIO_CLASS daNpc_Sha_Param_c
#endif

class daNpc_Sha_c : public daNpcT_c {
public:
    typedef int (daNpc_Sha_c::*actionFunc)(void*);
    typedef int (daNpc_Sha_c::*cutFunc)(int);
    typedef BOOL (daNpc_Sha_c::*queryFunc)();
    
    ~daNpc_Sha_c();
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
    void setParam();
    BOOL checkChangeEvt();
    void setAfterTalkMotion();
    void srchActors();
    BOOL evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void setAttnPos();
    void setCollision();
    int drawDbgInfo();
    BOOL selectAction();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    int getSceneChangeNoTableIx();
    void setTempBit(int);
    int cutPerformAugury(int);
    int wait(void*);
    int talk(void*);
    int test(void*);
    BOOL query265();
    BOOL query268();
    BOOL query267();
    BOOL query078();
    BOOL query264();
    BOOL query266();
    BOOL query542();
    BOOL query461();
    BOOL query288();
    BOOL query000();
    BOOL query999();
    daNpc_Sha_c(
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
            i_arcNames) { OS_REPORT("|%06d:%x|daNpc_Sha_c -> コンストラクト\n", g_Counter.mCounter0, this); }
    u16 getEyeballMaterialNo() { return 1; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == 4; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == 8; }

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        return nodeNo == 0xFFFF ? -1 : nodeNo;
    }

    static char* mCutNameList[2];
    static cutFunc mCutList[2];
    static const u16 mEvtBitLabels[6];
    static const u16 mTmpBitLabels[6];
    static const int mSceneChangeNoTable[48];
    static queryFunc mQueries[48];

private:
    /* 0xE40 */ NPC_SHA_HIO_CLASS* mpHIO;
    /* 0xE44 */ dCcD_Cyl mCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF84 */ actionFunc mNextAction;
    /* 0xF90 */ actionFunc mAction;
    /* 0xF9C */ int mSceneChangeNoTableIx;
    /* 0xFA0 */ u8 field_0xfa0;
    /* 0xFA1 */ u8 field_0xfa1;
    /* 0xFA2 */ u8 field_0xfa2;
    /* 0xFA3 */ u8 field_0xfa3;
    /* 0xFA4 */ u8 field_0xfa4;
};

STATIC_ASSERT(sizeof(daNpc_Sha_c) == 0xfa8);

#endif /* D_A_NPC_SHAMAN_H */
