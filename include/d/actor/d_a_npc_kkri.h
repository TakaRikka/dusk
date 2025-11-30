#ifndef D_A_NPC_KKRI_H
#define D_A_NPC_KKRI_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Kkri_c
 * @brief Coro
 *
 * @details
 *
 */
class daNpc_Kkri_c : public daNpcT_c {
public:
    typedef int (daNpc_Kkri_c::*ActionFunc)(void*);
    
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    static void* srchYm(void*, void*);
    int getType();
    int isDelete();
    void reset();
    void srchActors();
    int selectAction();
    BOOL chkAction(ActionFunc);
    BOOL setAction(ActionFunc);
    int cutConversationAboutSoup(int);
    int cutYmLook(int);
    int sleep();
    int wait(void*);
    int sitWait1(void*);
    int fearWait(void*);
    int talk(void*);

    daNpc_Kkri_c(daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
                                   daNpcT_motionAnmData_c const* i_motionAnmData,
                                   daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
                                   int i_faceMotionStepNum,
                                   daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
                                   daNpcT_evtData_c const* i_evtData, char** i_arcNames)
                    : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                               i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                               i_arcNames)
                    {
                        OS_REPORT("|%06d:%x|daNpc_Kkri_c -> コンストラクト\n", g_Counter.mCounter0, this);
                    }

    virtual ~daNpc_Kkri_c();
    virtual BOOL checkChangeJoint(int i_jointNo) { return i_jointNo == 6; }
    virtual BOOL checkRemoveJoint(int i_jointNo) { return i_jointNo == 10; }
    virtual s32 getBackboneJointNo() { return 1; }
    virtual s32 getNeckJointNo() { return 3; }
    virtual s32 getHeadJointNo() { return 4; }
    virtual u16 getEyeballMaterialNo() { return 2; }
    virtual void setParam();
    virtual BOOL checkChangeEvt();
    virtual BOOL evtTalk();
    virtual BOOL evtCutProc();
    virtual void setAfterTalkMotion();
    virtual void action();
    virtual void beforeMove();
    virtual void setAttnPos();
    virtual void setCollision();
    virtual int drawDbgInfo();

    int getFlowNodeNo() {
        u16 prm = home.angle.x;
        return prm == 0xFFFF ? -1 : prm;
    }

    u8 getPathID() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }
    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xFF0000) >> 16; }
    u8 getBitSW2() { return (fopAcM_GetParam(this) & 0xFF000000) >> 24; }

    MtxP getBd3Mtx() {
        return mpMorf[0]->getModel()->getAnmMtx(0x11);
    }

    MtxP getBd2Mtx() {
        return mpMorf[0]->getModel()->getAnmMtx(6);
    }

    MtxP getBd1Mtx() {
        return mpMorf[0]->getModel()->getAnmMtx(5);
    }

    static char* mCutNameList[3];
    static int (daNpc_Kkri_c::*mCutList[])(int);

private:
    /* 0xE40 */ u8 field_0xE40[0xE44 - 0xE40];
    /* 0xE44 */ dCcD_Cyl mCcCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF84 */ daNpcT_ActorMngr_c mActorMng[1];
    /* 0xF8C */ ActionFunc mSelectAction;
    /* 0xF98 */ ActionFunc mAction;
    /* 0xFA4 */ daNpcT_Path_c mPath;
    /* 0xFCC */ int field_0xfcc;
    /* 0xFD0 */ int field_0xfd0;
    /* 0xFD4 */ u8 field_0xfd4;
    /* 0xFD5 */ u8 field_0xfd5;
    /* 0xFD6 */ u8 field_0xfd6;
    /* 0xFD7 */ u8 field_0xfd7;
    /* 0xFD8 */ u8 field_0xfd8;
};

STATIC_ASSERT(sizeof(daNpc_Kkri_c) == 0xfdc);

struct daNpc_Kkri_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
};

class daNpc_Kkri_Param_c {
public:
    virtual ~daNpc_Kkri_Param_c() {}

    static const daNpc_Kkri_HIOParam m;
};


#endif /* D_A_NPC_KKRI_H */
