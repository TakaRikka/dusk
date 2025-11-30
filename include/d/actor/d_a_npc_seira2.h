#ifndef D_A_NPC_SEIRA2_H
#define D_A_NPC_SEIRA2_H

#include "d/actor/d_a_npc.h"
#include "d/d_shop_system.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Seira2_c
 * @brief Sera (Shopkeeper)
 *
 * @details
 *
 */
class daNpc_Seira2_c : public dShopSystem_c {
public:
    typedef int (daNpc_Seira2_c::*cutFunc)(int);
    typedef int (daNpc_Seira2_c::*actionFunc)(void*);

    ~daNpc_Seira2_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    int getFlowNodeNo();
    u8 getMaxNumItem();
    u8 checkBottle();
    int isDelete();
    void reset();
    void afterJntAnm(int);
    void setParam();
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
    int selectAction();
    int chkAction(actionFunc);
    int setAction(actionFunc);
    BOOL checkStageIsSeira2sShop();
    int wait(void*);
    int sit(void*);
    int happy(void*);
    int worry(void*);
    int sad(void*);
    int lookaround(void*);
    int smile(void*);
    int talk(void*);
    int shop(void*);
    daNpc_Seira2_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : dShopSystem_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {
        // "|%06d:%x|daNpc_Seira2_c -> Construct\n"
        OS_REPORT("|%06d:%x|daNpc_Seira2_c -> コンストラクト\n", g_Counter.mCounter0, this);
    }
    u16 getEyeballMaterialNo() { return 2; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return true; }
    BOOL checkChangeJoint(int val) { return val == 4; }
    BOOL checkRemoveJoint(int val) { return val == 8; }

    static char* mCutNameList[1];
    static cutFunc mCutList[1];

private:
    /* 0x0F7C */ mDoExt_McaMorfSO* mpSeiraMorf;
    /* 0x0F80 */ int field_0x0F80;
    /* 0x0F80 */ dCcD_Cyl mCyl1;
    /* 0x10C0 */ u8 mChkBottle;
    /* 0x10C1 */ u8 mType;
    /* 0x10C4 */ daNpcT_ActorMngr_c mActorMngr[1];
    /* 0x10CC */ actionFunc mNextAction;
    /* 0x10D8 */ actionFunc mActFunc_0;
    /* 0x10D4 */ int mEventTimer;
    /* 0x10E8 */ int mShopProcess;
    /* 0x10EC */ u8 field_0x10EC;
    /* 0x10ED */ u8 field_0x10ED;
    /* 0x10EE */ u8 mEvtBit;
    /* 0x10EF */ u8 field_0x10EF[0x10F0 - 0x10EF];
    /* 0x10F0 */ u8 field_0x10F0;
};

STATIC_ASSERT(sizeof(daNpc_Seira2_c) == 0x10F4);

struct daNpc_Seira2_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
};

class daNpc_Seira2_Param_c {
public:
    virtual ~daNpc_Seira2_Param_c() {}

    static const daNpc_Seira2_HIOParam m;
};


#endif /* D_A_NPC_SEIRA2_H */
