#ifndef D_A_NPC_BESU_H
#define D_A_NPC_BESU_H

#if DEBUG
#include "SSystem/SComponent/c_counter.h"
#endif
#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Besu_c
 * @brief Beth
 *
 * @details
 *
 */
class daNpc_Besu_c : public daNpcT_c {
public:
    typedef int (daNpc_Besu_c::*cutFunc)(int);
    typedef int (daNpc_Besu_c::*actionFunc)(void*);

    ~daNpc_Besu_c();
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
    BOOL chkNurse();
    BOOL chkBesu3();
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
    int setCupAnm(int, int, f32);
    bool afterSetMotionAnm(int, int, f32, int);
    daNpcT_faceMotionAnmData_c getFaceMotionAnm(daNpcT_faceMotionAnmData_c);
    void changeAnm(int*, int*);
    void changeBck(int*, int*);
    void changeBtp(int*, int*);
    void changeBtk(int*, int*);
    int selectAction();
    int chkAction(actionFunc);
    int setAction(actionFunc);
    void mop(int, int);
    int cutConversationAboutSaru(int);
    int cutConversationAboutPachinko(int);
    int cutConversationAboutWoodSwd(int);
    int cutSwdTutorial(int);
    int cutFindMonkey(int);
    int cutGetWoodSwd(int);
    int cutCacaricoConversation(int);
    int cutSurprise(int);
    int cutConversationInHotel(int);
    int cutConversationAboutDeathMt(int);
    int cutConversationAboutGoron(int);
    int cutNurse(int);
    int cutClothTry(int);
    int cutThankYou(int);
    int wait(void*);
    int swdTutorial(void*);
    int nurse(void*);
    int giveHotWater(void*);
    int talk(void*);
    daNpc_Besu_c(
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
        i_arcNames) {
        // "construct"
        OS_REPORT("|%06d:%x|daNpc_Besu_c -> コンストラクト\n", g_Counter.mCounter0, this);
    }
    u16 getEyeballMaterialNo() { return 2; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return true; }
    BOOL checkChangeJoint(int arg0) { return arg0 == 4; }
    BOOL checkRemoveJoint(int arg0) { return arg0 == 6; }

    u32 getFlowNodeNo() { 
        u16 nodeNo = home.angle.x;
        u32 rv;
        if (nodeNo == 0xffff) {
            rv = -1;
        } else {
            rv = nodeNo;
        }
        return rv;
    }

    u8 getPathID() { return (fopAcM_GetParam(this) & 0xff00) >> 8; }
    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xff0000) >> 16; }

    static char* mCutNameList[15];
    static cutFunc mCutList[15];

private:
    /* 0x0E40 */ mDoExt_McaMorfSO* mpCupModelMorf;
    /* 0x0E44 */ u8 field_0xe44[0x0E48 - 0xE44];
    /* 0x0E48 */ J3DModel* mpClothModel[1];
    /* 0x0E4C */ dCcD_Cyl mCyl1;
    /* 0x0F88 */ dCcD_Cyl mCyl2;
    /* 0x10C4 */ u8 mType;
    /* 0x10C8 */ daNpcT_ActorMngr_c mActorMngr[7];
    /* 0x1100 */ actionFunc mNextAction;
    /* 0x110C */ actionFunc mAction;
    /* 0x1118 */ int field_0x1118;
    /* 0x111C */ int field_0x111c;
    /* 0x1120 */ int field_0x1120;
    /* 0x1124 */ int field_0x1124;
    /* 0x1128 */ int field_0x1128;
    /* 0x112C */ u8 field_0x112c;
    /* 0x112D */ u8 field_0x112d;
    /* 0x112E */ u8 field_0x112e;
    /* 0x112F */ u8 field_0x112f;
    /* 0x1130 */ u8 field_0x1130;
    /* 0x1131 */ u8 field_0x1131[0x1134 - 0x1131];
    /* 0x1134 */ u8 field_0x1134;
};

STATIC_ASSERT(sizeof(daNpc_Besu_c) == 0x1138);

struct daNpc_Besu_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 field_0x8c;
};

class daNpc_Besu_Param_c {
public:
    virtual ~daNpc_Besu_Param_c() {}

    static const daNpc_Besu_HIOParam m;
};

#endif /* D_A_NPC_BESU_H */
