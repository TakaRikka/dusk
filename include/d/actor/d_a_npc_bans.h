#ifndef D_A_NPC_BANS_H
#define D_A_NPC_BANS_H

#include "d/actor/d_a_npc.h"
#include "d/d_shop_system.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Bans_c
 * @brief Barnes
 *
 * @details
 *
*/

struct daNpc_Bans_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 run_spd;
};

class daNpc_Bans_Param_c {
public:
    virtual ~daNpc_Bans_Param_c() {}

    static daNpc_Bans_HIOParam const m;
};

#if DEBUG
class daNpc_Bans_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Bans_HIO_c();
    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    /* 0x8 */ daNpc_Bans_HIOParam m;
};
#define NPC_BANS_HIO_CLASS daNpc_Bans_HIO_c
#else
#define NPC_BANS_HIO_CLASS daNpc_Bans_Param_c
#endif

class daNpc_Bans_c : public dShopSystem_c {
public:
    typedef int (daNpc_Bans_c::*actionFunc)(void*);
    typedef int (daNpc_Bans_c::*cutFunc)(int);

    ~daNpc_Bans_c();
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
    void drawOtherMdl();
    bool setScoopAnm(int, int, f32);
    bool afterSetMotionAnm(int, int, f32, int);
    BOOL selectAction();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    BOOL checkPlayerIn2ndFloorOfBombShop();
    void orderAngerEvt();
    fopAc_ac_c* getKMsgTagP();
    void setPrtcls();
    int cutAnger(int);
    int cutPurchase(int);
    int wait(void*);
    int tend(void*);
    int talk(void*);
    int shop(void*);
    int test(void*);
    daNpc_Bans_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : dShopSystem_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                i_arcNames) {
            OS_REPORT("|%06d:%x|daNpc_Bans_c -> コンストラクト\n", g_Counter.mCounter0, this);
        }
    u16 getEyeballMaterialNo() { return 2; }
    s32 getHeadJointNo() { return 6; }
    s32 getNeckJointNo() { return 5; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == 6; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == 0xB; }

    u8 getGroupId() { return (fopAcM_GetParam(this) & ~0xFFFFFFF) >> 28; }
    u8 getMaxNumItem() { return (fopAcM_GetParam(this) & 0xF000000) >> 24; }
    u8 getPathID() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }
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

    static char* mCutNameList[4];
    static cutFunc mCutList[4];

    /* 0x0F7C */ mDoExt_McaMorfSO* mpScoopMorf;
    /* 0x0F80 */ NPC_BANS_HIO_CLASS* mHIO;
    /* 0x0F84 */ J3DModel* mTubModel[1];
    /* 0x0F88 */ dCcD_Cyl mCyl1;
    /* 0x10C4 */ dCcD_Cyl mCyl2;
    /* 0x1200 */ u8 mType;
    /* 0x1204 */ daNpcT_ActorMngr_c mActorMngrs[8];
    /* 0x1244 */ actionFunc mNextAction;
    /* 0x1250 */ actionFunc mAction;
    /* 0x125C */ int mShopProcess;
    /* 0x1260 */ int mTimer;
    /* 0x1264 */ u8 mSetParamFlag;
    /* 0x1265 */ u8 mShopFlag;
    /* 0x1266 */ u8 field_0x1266;
    /* 0x1267 */ u8 mDemo13Flag;
    /* 0x1268 */ u8 mDropWaterTimer;
    /* 0x1269 */ u8 field_0x1269;
    /* 0x126A */ u8 field_0x126a;
    /* 0x126B */ u8 field_0x126b;
    /* 0x126C */ u8 field_0x126c;
};

STATIC_ASSERT(sizeof(daNpc_Bans_c) == 0x1270);

#endif /* D_A_NPC_BANS_H */
