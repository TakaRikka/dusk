#ifndef D_A_NPC_YELIA_H
#define D_A_NPC_YELIA_H

#include "d/actor/d_a_npc.h"

struct daNpc_Yelia_HIOParam {
    daNpcT_HIOParam common;
};

class daNpc_Yelia_Param_c {
public:
    virtual ~daNpc_Yelia_Param_c() {}

    static daNpc_Yelia_HIOParam const m;
};

#if DEBUG
class daNpc_Yelia_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Yelia_HIO_c();


    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_Yelia_HIOParam m;
};

#define NPC_YELIA_HIO_CLASS daNpc_Yelia_HIO_c
#else
#define NPC_YELIA_HIO_CLASS daNpc_Yelia_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Yelia_c
 * @brief Ilia
 *
 * @details
 *
 */
class daNpc_Yelia_c : public daNpcT_c {
public:
    ~daNpc_Yelia_c();
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
    BOOL drawDbgInfo();
    BOOL selectAction();
    BOOL chkAction(BOOL (daNpc_Yelia_c::*)(void*));
    BOOL setAction(BOOL (daNpc_Yelia_c::*)(void*));
    BOOL cutConversationAboutLoopHole(int);
    BOOL cutTWResistance(int);
    BOOL cutTakeWoodStatue(int);
    BOOL cutClothTry(int);
    BOOL cutThankYou(int);
    BOOL wait(void*);
    BOOL horseWait(void*);
    BOOL reminiscence(void*);
    BOOL talk(void*);
    daNpc_Yelia_c(daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
                        daNpcT_motionAnmData_c const* i_motionAnmData,
                        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
                        int i_faceMotionStepNum,
                        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData,
                        int i_motionStepNum,
                        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
                        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                        i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                        i_arcNames) {}
    u16 getEyeballMaterialNo() { return 1; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int i_jointNo) { return i_jointNo == 4; }
    BOOL checkRemoveJoint(int i_jointNo) {return i_jointNo == 9; }
    BOOL chkXYItems() { return true; }

    u32 getFlowNodeNo() {
        u16 no = home.angle.x;
        if (no == 0xffff) {
            return -1;
        }
        return no;
    }

    static char* mCutNameList[6];
    static int (daNpc_Yelia_c::*mCutList[6])(int);
private:
    /* 0xE40 */ NPC_YELIA_HIO_CLASS* mpHIO;
    /* 0xE44 */ dCcD_Cyl mCcCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF84 */ daNpcT_ActorMngr_c mActorMngr[4];
    /* 0xFA4 */ BOOL (daNpc_Yelia_c::*mpNextAction)(void*);
    /* 0xFB0 */ BOOL (daNpc_Yelia_c::*mpAction)(void*);
    /* 0xFBC */ daNpcT_Path_c mPath;
    /* 0xFE4 */ fpc_ProcID mItemId;
    /* 0xFE8 */ int mPendantTimer;
    /* 0xFEC */ u8 field_0xfec;
};

STATIC_ASSERT(sizeof(daNpc_Yelia_c) == 0xff0);

#endif /* D_A_NPC_YELIA_H */
