#ifndef D_A_NPC_TOBY_H
#define D_A_NPC_TOBY_H

#include "dolphin/types.h"
#include "d/actor/d_a_npc.h"

struct daNpc_Toby_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 performance_speed;
    /* 0x90 */ f32 speedup_performance;
    /* 0x94 */ f32 init_play_speed;
    /* 0x98 */ f32 switching_speed;
};

class daNpc_Toby_Param_c {
public:
    virtual ~daNpc_Toby_Param_c() {}

    static const daNpc_Toby_HIOParam m;
};

#if DEBUG
class daNpc_Toby_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Toby_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_Toby_HIOParam m;
};

#define NPC_TOBY_HIO_CLASS daNpc_Toby_HIO_c
#else
#define NPC_TOBY_HIO_CLASS daNpc_Toby_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Toby_c
 * @brief Fyer
 *
 * @details
 *
 */
class daNpc_Toby_c : public daNpcT_c {
public:
    typedef int (daNpc_Toby_c::*cutFunc)(int);
    typedef int (daNpc_Toby_c::*actionFunc)(void*);

    ~daNpc_Toby_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    static void* srchToby(void*, void*);
    fopAc_ac_c* getOtherTobyP(int);
    u8 getType();
    int isDelete();
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
    void drawOtherMdl();
    bool afterSetFaceMotionAnm(int, int, f32, int);
    bool afterSetMotionAnm(int, int, f32, int);
    int selectAction();
    int chkAction(actionFunc);
    int setAction(actionFunc);
    int cutTobyHouseFire(int);
    int cutTalkToOneself(int);
    int cutRepairSCannon(int);
    int cutSCannon(int);
    int cutConversationAboutSCannon(int);
    int cutConversationAboutZra(int);
    int chkAttnZra();
    int wait(void*);
    int walk(void*);
    int play(void*);
    int talk(void*);
    int test(void*);
    daNpc_Toby_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {
        // "|%06d:%x|daNpc_Toby_c -> Construct\n"
        OS_REPORT("|%06d:%x|daNpc_Toby_c -> コンストラクト\n", g_Counter.mCounter0, this);
    }
    u16 getEyeballMaterialNo() { return 2; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int arg0) { return arg0 == 4; }
    BOOL checkRemoveJoint(int arg0) { return arg0 == 8; }
    s32 getFootLJointNo() { return 24; }
    s32 getFootRJointNo() { return 27; }
    BOOL chkXYItems() { return TRUE; }

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        u32 rv;
        if (nodeNo == 0xffff) {
            rv = -1;
        } else {
            rv = nodeNo;
        }

        return rv;
    }

    u8 getPathID() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }
    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xFF0000) >> 16; }

    static char* mCutNameList[7];
    static cutFunc mCutList[7];

private:
    /* 0x0E40 */ NPC_TOBY_HIO_CLASS* mpHIO;
    /* 0x0E44 */ J3DModel* mpTobyModels[1];
    /* 0x0E48 */ dCcD_Cyl mCyl1;
    /* 0x0F84 */ u8 mType;
    /* 0x0F88 */ daNpcT_ActorMngr_c mActorMngr[4];
    /* 0x0FA8 */ actionFunc mActFunc_0;
    /* 0x0FB4 */ actionFunc mActFunc_1;
    /* 0x0FC0 */ daNpcT_Path_c mPath;
    /* 0x0FE8 */ cXyz field_0x0FE8;
    /* 0x0FF4 */ int field_0x0FF4;
    /* 0x0FF8 */ f32 mPlaySpeed;
    /* 0x0FFC */ u16 field_0x0FFC;
    /* 0x0FFE */ u8 field_0x0FFE;
    /* 0x0FFF */ u8 field_0x0FFF;
    /* 0x1000 */ u8 field_0x1000;
    /* 0x1001 */ bool field_0x1001;
    /* 0x1002 */ bool field_0x1002;
    /* 0x1003 */ u8 field_0x1003;
    /* 0x1004 */ u8 field_0x1004;
};

STATIC_ASSERT(sizeof(daNpc_Toby_c) == 0x1008);

#endif /* D_A_NPC_TOBY_H */
