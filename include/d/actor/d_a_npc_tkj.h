#ifndef D_A_NPC_TKJ_H
#define D_A_NPC_TKJ_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpcTkj_c
 * @brief Oocca
 *
 * @details
 *
 */
class daNpcTkj_c : public daNpcT_c {
public:
    typedef int (daNpcTkj_c::*ActionFunc)(void*);

    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    int getType();
    int getFlowNodeNo();
    int isDelete();
    void reset();
    void srchActors();
    int selectAction();
    BOOL chkAction(ActionFunc);
    int setAction(ActionFunc);
    int wait(void*);
    int talk(void*);
    int cutLv7Start(int);
    
    daNpcTkj_c(daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
                              daNpcT_motionAnmData_c const* i_motionAnmData,
                              daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
                              int i_faceMotionStepNum,
                              daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
                              daNpcT_evtData_c const* i_evtData, char** i_arcNames)
                    : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                               i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                               i_arcNames)
                    {
                        OS_REPORT("|%06d:%x|daNpcTkj_c -> コンストラクト\n", g_Counter.mCounter0, this);
                    }

    virtual ~daNpcTkj_c();
    virtual BOOL checkChangeJoint(int i_jointNo) { return i_jointNo == 17; }
    virtual BOOL checkRemoveJoint(int i_jointNo) { return i_jointNo == 19; }
    virtual s32 getBackboneJointNo() { return 15; }
    virtual s32 getNeckJointNo() { return 16; }
    virtual s32 getHeadJointNo() { return 17; }
    virtual u16 getEyeballMaterialNo() { return 2; }
    virtual inline void setParam();
    virtual BOOL evtTalk();
    virtual BOOL evtCutProc();
    virtual void setAfterTalkMotion();
    virtual void action();
    virtual inline void beforeMove();
    virtual void setAttnPos();
    virtual void setCollision();
    virtual int drawDbgInfo();

    int getPath() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }

    static char* mCutNameList[2];
    static int (daNpcTkj_c::*mCutList[])(int);

private:
    /* 0xE40 */ u8 field_0xE40[0xE44 - 0xE40];
    /* 0xE44 */ dCcD_Cyl mCcCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF84 */ ActionFunc field_0xf84;
    /* 0xF90 */ ActionFunc mAction;
    /* 0xF9C */ u8 field_0xf9c;
    /* 0xFA0 */ int mItemNo;
};

STATIC_ASSERT(sizeof(daNpcTkj_c) == 0xfa4);

struct daNpc_Tkj_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
};
class daNpc_Tkj_Param_c {
public:
    virtual ~daNpc_Tkj_Param_c() {}

    static const daNpc_Tkj_HIOParam m;
};

#endif /* D_A_NPC_TKJ_H */
