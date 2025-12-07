#ifndef D_A_NPC_GRMC_H
#define D_A_NPC_GRMC_H

#include "d/actor/d_a_npc.h"
#include "d/d_shop_system.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_grMC_c
 * @brief Goron Child (Shopkeeper)
 *
 * @details
 *
*/

#if DEBUG
#define NPC_GRMC_HIO_CLASS daNpc_grMC_HIO_c
#else
#define NPC_GRMC_HIO_CLASS daNpc_grMC_Param_c
#endif

struct daNpc_grMC_HIOParam {
    /* 0x0 */ daNpcT_HIOParam common;
};

class daNpc_grMC_HIO_c : public mDoHIO_entry_c {
public:
    /* 0x8 */ daNpc_grMC_HIOParam m;
};

class daNpc_grMC_Param_c {
public:
    virtual ~daNpc_grMC_Param_c() {}

    static daNpc_grMC_HIOParam const m;
};

class daNpc_grMC_c : public dShopSystem_c {
public:

    enum Joint {
        /* 0x00 */ JNT_CENTER,
        /* 0x01 */ JNT_BACKBONE1,
        /* 0x02 */ JNT_BACKBONE2,
        /* 0x03 */ JNT_NECK,
        /* 0x04 */ JNT_HEAD,
        /* 0x05 */ JNT_CHIN,
        /* 0x06 */ JNT_MAYU,
        /* 0x07 */ JNT_MOUTH,
        /* 0x08 */ JNT_SHOULDERL,
        /* 0x09 */ JNT_ARML1,
        /* 0x0A */ JNT_ARML2,
        /* 0x0B */ JNT_HANDL,
        /* 0x0C */ JNT_SHOULDERR,
        /* 0x0D */ JNT_ARMR1,
        /* 0x0E */ JNT_ARMR2,
        /* 0x0F */ JNT_HANDR,
        /* 0x10 */ JNT_WAIST,
        /* 0x11 */ JNT_LEGL1,
        /* 0x12 */ JNT_LEGL2,
        /* 0x13 */ JNT_FOOTL,
        /* 0x14 */ JNT_LEGR1,
        /* 0x15 */ JNT_LEGR2,
        /* 0x16 */ JNT_FOOTR,
    };

    typedef int (daNpc_grMC_c::*cutFunc)(int);
    typedef int (daNpc_grMC_c::*actionFunc)(void*);

    ~daNpc_grMC_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    int getFlowNodeNo();
    u8 getMaxNumItem();
    BOOL isDelete();
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
    int drawDbgInfo();
    void drawOtherMdl();
    BOOL selectAction();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    int tend(void*);
    int talk(void*);
    int shop(void*);
    int test(void*);
    daNpc_grMC_c(
            daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
            daNpcT_motionAnmData_c const* i_motionAnmData,
            daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
            int i_faceMotionStepNum,
            daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData,
            int i_motionStepNum,
            daNpcT_evtData_c const* i_evtData,
            char** i_arcNames)
        : dShopSystem_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
        i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
        i_arcNames) {}
    s32 getHeadJointNo() { return JNT_HEAD; }
    s32 getNeckJointNo() { return JNT_NECK; }
    s32 getBackboneJointNo() { return JNT_BACKBONE1; }
    BOOL checkChangeJoint(int param_1) { return param_1 == JNT_HEAD; }
    BOOL checkRemoveJoint(int param_1) { return param_1 == JNT_MOUTH; }
    u16 getEyeballMaterialNo() { return 1; };

    static char* mCutNameList;
    static cutFunc mCutList[1];

private:
    /* 0x0F7C */ NPC_GRMC_HIO_CLASS* mHIO;
    /* 0x0F80 */ dCcD_Cyl mCyl;
    /* 0x10BC */ u8 mType;
    /* 0x10C0 */ actionFunc mNextAction;
    /* 0x10CC */ actionFunc mAction;
    /* 0x10D8 */ int field_0x10d8;
    /* 0x10DC */ u8 field_0x10dc;
    /* 0x10DD */ u8 field_0x10dd;
    /* 0x10DE */ u8 field_0x10de[0x10e0 - 0x10de];
    /* 0x10E0 */ u8 field_0x10e0;
};

STATIC_ASSERT(sizeof(daNpc_grMC_c) == 0x10e4);

#endif /* D_A_NPC_GRMC_H */
