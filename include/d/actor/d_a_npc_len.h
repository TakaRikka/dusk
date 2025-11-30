#ifndef D_A_NPC_LEN_H
#define D_A_NPC_LEN_H

#include "d/actor/d_a_npc.h"

struct daNpc_Len_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ s16 field_0x8c;
    /* 0x8E */ s16 field_0x8e;
    /* 0x90 */ f32 field_0x90;
};

class daNpc_Len_Param_c {
public:
    virtual ~daNpc_Len_Param_c() {}

    static const daNpc_Len_HIOParam m;
};

#if DEBUG
class daNpc_Len_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Len_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_Len_HIOParam m;
};

#define NPC_LEN_HIO_CLASS daNpc_Len_HIO_c
#else
#define NPC_LEN_HIO_CLASS daNpc_Len_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Len_c
 * @brief Renado
 *
 * @details
 *
 */

class daNpc_Len_c : public daNpcT_c {
public:
    typedef int (daNpc_Len_c::*actionFunc)(void*);
    typedef int (daNpc_Len_c::*cutFunc)(int);

    ~daNpc_Len_c();
    int create();
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
    int selectAction();
    BOOL chkAction(actionFunc);
    int setAction(actionFunc);
    BOOL checkStartDemo13StbEvt(fopAc_ac_c*, f32, f32, f32, f32, f32, f32, f32);
    int cutConversationInHotel(int);
    int cutHurry(int);
    int cutTakeWoodStatue(int);
    int wait(void*);
    int patrol(void*);
    int talk(void*);
    daNpc_Len_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {}
    u16 getEyeballMaterialNo() { return 1; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == 4; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == 9; }
    s32 getFootLJointNo() { return 29; }
    s32 getFootRJointNo() { return 32; }
    BOOL chkXYItems() { return TRUE; }

    static char* mCutNameList[4];
    static cutFunc mCutList[4];

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xffff) {
            return -1;
        }
        return nodeNo;
    }
    u8 getPathID() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }
    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xff0000) >> 16; }

private:
    /* 0xE40 */ NPC_LEN_HIO_CLASS* mpHIO;
    /* 0xE44 */ dCcD_Cyl mCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF84 */ daNpcT_ActorMngr_c mActorMngr[4];
    /* 0xFA4 */ daNpcT_Path_c mPath;
    /* 0xFCC */ actionFunc field_0xfcc;
    /* 0xFD8 */ actionFunc mAction;
    /* 0xFE4 */ int field_0xfe4;
    /* 0xFE8 */ int field_0xfe8;
    /* 0xFEC */ u8 field_0xfec;
    /* 0xFED */ u8 field_0xfed;
    /* 0xFEE */ u8 field_0xfee;
    /* 0xFF0 */ int field_0xff0;
};

STATIC_ASSERT(sizeof(daNpc_Len_c) == 0xff4);

#endif /* D_A_NPC_LEN_H */
