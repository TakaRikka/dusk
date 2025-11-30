#ifndef D_A_NPC_SEIRA_H
#define D_A_NPC_SEIRA_H

#include "d/actor/d_a_npc.h"
#include "d/d_shop_system.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Seira_c
 * @brief Sera
 *
 * @details
 *
 */
class daNpc_Seira_c : public dShopSystem_c {
public:
    typedef int (daNpc_Seira_c::*cutFunc)(int);
    typedef int (daNpc_Seira_c::*actionFunc)(void*);

    ~daNpc_Seira_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    int getFlowNodeNo();
    u8 checkBottle();
    u8 getNpcType();
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
    BOOL drawDbgInfo();
    void drawOtherMdl();
    bool setBottleAnm(int, int, f32, bool);
    bool afterSetMotionAnm(int, int, f32, int);
    void changeAnm(int*, int*);
    void changeBck(int*, int*);
    void changeBtp(int*, int*);
    void changeBtk(int*, int*);
    int selectAction();
    int chkAction(actionFunc);
    int setAction(actionFunc);
    int cutConversationAboutSaru(int);
    BOOL checkStageIsSeirasShop();
    int getShopItemType();
    int wait(void*);
    int sit(void*);
    int happy(void*);
    int worry(void*);
    int sad(void*);
    int lookaround(void*);
    int lookaround_wait(void*);
    int smile(void*);
    int talk(void*);
    int shop(void*);
    daNpc_Seira_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : dShopSystem_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {
        // "|%06d:%x|daNpc_Seira_c -> Construct\n"
        OS_REPORT("|%06d:%x|daNpc_Seira_c -> コンストラクト\n", g_Counter.mCounter0, this);
    }
    u16 getEyeballMaterialNo() { return 2; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return true; }
    BOOL checkChangeJoint(int val) { return val == 4; }
    BOOL checkRemoveJoint(int val) { return val == 8; }

    static char* mCutNameList[2];
    static cutFunc mCutList[2];

private:
    /* 0x0F7C */ mDoExt_McaMorfSO* mpSeiraMorf;
    /* 0x0F80 */ int field_0x0F80;
    /* 0x0F80 */ dCcD_Cyl mCyl1;
    /* 0x10C0 */ u8 mChkBottle;
    /* 0x10C1 */ u8 mType;
    /* 0x10C4 */ daNpcT_ActorMngr_c mActorMngr[3];
    /* 0x10DC */ actionFunc mNextAction;
    /* 0x10E8 */ actionFunc mActFunc_0;
    /* 0x10F4 */ int mEventTimer;
    /* 0x10F8 */ int mShopProcess;
    /* 0x10FC */ u8 field_0x10fc;
    /* 0x10FD */ u8 field_0x10fd;
    /* 0x10FE */ u8 mNpcType;
    /* 0x10FF */ u8 field_0x10FF;
    /* 0x1100 */ u8 mEvtBit;
    /* 0x1101 */ u8 field_0x1101[0x1104 - 0x1101];
    /* 0x1104 */ u8 field_0x1104;
};

STATIC_ASSERT(sizeof(daNpc_Seira_c) == 0x1108);

struct daNpc_Seira_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
};

class daNpc_Seira_Param_c {
public:
    virtual ~daNpc_Seira_Param_c() {}

    static const daNpc_Seira_HIOParam m;
};


#endif /* D_A_NPC_SEIRA_H */
