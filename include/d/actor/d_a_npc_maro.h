#ifndef D_A_NPC_MARO_H
#define D_A_NPC_MARO_H

#include "SSystem/SComponent/c_counter.h"
#include "d/actor/d_a_npc.h"
#include "d/d_shop_system.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Maro_c
 * @brief Malo
 *
 * @details
 *
 */
class daNpc_Maro_c : public dShopSystem_c {
public:
    typedef int (daNpc_Maro_c::*actionFunc)(void*);
    typedef int (daNpc_Maro_c::*cutFunc)(int);

    enum Type {
        TYPE_0,
        TYPE_1,
        TYPE_2,
        TYPE_3,
        TYPE_4,
        TYPE_5,
        TYPE_6,
        TYPE_7,
        TYPE_8,
        TYPE_9,
        TYPE_10,
        TYPE_11,
        TYPE_12,
        TYPE_13,
        TYPE_14,
        TYPE_15,
        TYPE_16,
    };

    ~daNpc_Maro_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    static void* srchArrow(void*, void*);
    fopAc_ac_c* getArrowP();
    static void* srchItaMato(void*, void*);
    fopAc_ac_c* getItaMatoP(int);
    u8 getType();
    int isDelete();
    void reset();
    void afterJntAnm(int);
    void setParam();
    BOOL checkChangeEvt();
    BOOL evtEndProc();
    void setAfterTalkMotion();
    void srchActors();
    BOOL evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void setAttnPos();
    void setCollision();
    BOOL drawDbgInfo();
    void drawOtherMdl();
    daNpcT_faceMotionAnmData_c getFaceMotionAnm(daNpcT_faceMotionAnmData_c);
    daNpcT_motionAnmData_c getMotionAnm(daNpcT_motionAnmData_c);
    int selectAction();
    int chkAction(int (daNpc_Maro_c::*)(void*));
    int setAction(int (daNpc_Maro_c::*)(void*));
    int cutConversationAboutPachinko(int);
    int cutConversationAboutWoodSwd(int);
    int cutSwdTutorial(int);
    int cutHail(int);
    int cutFindMonkey(int);
    int cutGiveMeWoodSwd(int);
    int cutGetWoodSwd(int);
    int cutConversationWithMaro(int);
    int cutCacaricoConversation(int);
    int cutSurprise(int);
    int cutArrowTutorial(int);
    int cutBokinTalk(int);
    int cutMarosWhisper(int);
    int cutTagPush1(int);
    int cutNotGonnaLet(int);
    int cutTalkToKakashi(int);
    int wait(void*);
    int swdTutorial(void*);
    int talk_withTaro(void*);
    int choccai(void*);
    int tend(void*);
    int arrowTutorial(void*);
    int talk(void*);
    int shop(void*);
    daNpc_Maro_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : dShopSystem_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {
        OS_REPORT("|%06d:%x|daNpc_Maro_c -> コンストラクト\n", g_Counter.mCounter0, this);
    }
    u16 getEyeballMaterialNo() { return 1; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int arg0) { return arg0 == 4; };
    BOOL checkRemoveJoint(int arg0) { return arg0 == 6; };

    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xFF0000) >> 16; }

    void lostPlayer() { mPlayerActorMngr.remove(); }
    BOOL checkNowMotionIsChoccai() {
        if (mMotionSeqMngr.getNo() == 5 || mMotionSeqMngr.getNo() == 6) {
            return TRUE;
        }
        return FALSE;
    }

    BOOL checkEndMotionIsChoccai() {
        if (checkNowMotionIsChoccai() == TRUE && mMotionSeqMngr.getStepNo() > 0) {
            return TRUE;
        }
        return FALSE;
    }

    void findPlayer() { mPlayerActorMngr.entry(daPy_getPlayerActorClass()); }

    void startChoccai() { field_0x1134 = 1; }
    void endChoccai() { field_0x1134 = 0; }

    static char* mCutNameList[17];
    static cutFunc mCutList[17];

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xffff) {
            return -1;
        }
        return nodeNo;
    }

    u8 getMaxNumItem() {
        return (fopAcM_GetParam(this) & 0xf000000) >> 0x18;
    }

private:
    /* 0x0F7C */ int field_0xf7c;
    /* 0x0F80 */ dCcD_Cyl mCyl1;
    /* 0x10BC */ int field_0x10bc;
    /* 0x10C0 */ u8 mType;
    /* 0x10C4 */ daNpcT_ActorMngr_c mActorMngr[9];
    /* 0x110C */ actionFunc field_0x110c;
    /* 0x110C */ actionFunc field_0x1118;
    /* 0x1124 */ u8 field_0x1124[0x1128 - 0x1124];
    /* 0x1128 */ int field_0x1128;
    /* 0x112C */ int field_0x112c;
    /* 0x1130 */ u8 field_0x1130;
    /* 0x1131 */ u8 field_0x1131;
    /* 0x1132 */ u8 field_0x1132;
    /* 0x1134 */ u8 field_0x1133;
    /* 0x1134 */ u8 field_0x1134;
    /* 0x1135 */ u8 field_0x1135;
    /* 0x1136 */ u8 field_0x1136;
    /* 0x1137 */ u8 field_0x1137;
    /* 0x1138 */ u8 field_0x1138;
    /* 0x1139 */ u8 field_0x1139;
    /* 0x113A */ u8 field_0x113a[0x113C - 0x113A];
    /* 0x113C */ u8 field_0x113C[0x1140 - 0x113C];
};

STATIC_ASSERT(sizeof(daNpc_Maro_c) == 0x1140);

struct daNpc_Maro_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ u32 field_0x8c;
};

class daNpc_Maro_Param_c {
public:
    virtual ~daNpc_Maro_Param_c() {}

    static const daNpc_Maro_HIOParam m;
};


#endif /* D_A_NPC_MARO_H */
