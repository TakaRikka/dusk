#ifndef D_A_NPC_GRM_H
#define D_A_NPC_GRM_H

#include "d/actor/d_a_npc.h"
#include "d/d_shop_system.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_grM_c
 * @brief Goron Adult (Shopkeeper)
 *
 * @details
 *
 */
class daNpc_grM_c : public dShopSystem_c {
public:
    typedef int (daNpc_grM_c::*cutFunc)(int);
    typedef int (daNpc_grM_c::*actionFunc)(void*);

    enum JOINT {
        /* 0X00 */ JNT_CENTER,
        /* 0X01 */ JNT_BACKBONE1,
        /* 0X02 */ JNT_BACKBONE2,
        /* 0X03 */ JNT_NECK,
        /* 0X04 */ JNT_HEAD,
        /* 0X05 */ JNT_CHIN,
        /* 0X06 */ JNT_MAYU,
        /* 0X07 */ JNT_MOUTH,
        /* 0X08 */ JNT_SHOULDERL,
        /* 0X09 */ JNT_ARML1,
        /* 0X0A */ JNT_ARML2,
        /* 0X0B */ JNT_HANDL,
        /* 0X0C */ JNT_FINGERL1,
        /* 0X0D */ JNT_THUMBL,
        /* 0X0E */ JNT_SHOULDERR,
        /* 0X0F */ JNT_ARMR1,
        /* 0X10 */ JNT_ARMR2,
        /* 0X11 */ JNT_HANDR,
        /* 0X12 */ JNT_FINGERR1,
        /* 0X13 */ JNT_THUMBR,
        /* 0X14 */ JNT_WAIST,
        /* 0X15 */ JNT_LEGL1,
        /* 0X16 */ JNT_LEGL2,
        /* 0X17 */ JNT_FOOTL,
        /* 0X18 */ JNT_LEGR1,
        /* 0X19 */ JNT_LEGR2,
        /* 0X1A */ JNT_FOOTR
    };

    enum Material {
        /* 0x0 */ MAT_SC_EYE,
        /* 0x1 */ MAT_GRA_A_EYEBALL,
        /* 0x2 */ MAT_GRA_A_FACE_M,
        /* 0x3 */ MAT_GRA_M,
    };

    ~daNpc_grM_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    bool getType();
    int getFlowNodeNo();
    u8 getMaxNumItem();
    BOOL isDelete();
    void reset();
    void afterJntAnm(int);
    void setParam();
    void setAfterTalkMotion();
    void srchActors();
    BOOL evtProc();
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
    int cutTalkSpa(int);
    int tend(void*);
    int talk(void*);
    int shop(void*);
    daNpc_grM_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : dShopSystem_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                        i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                        i_arcNames) {}
    u16 getEyeballMaterialNo() { return MAT_GRA_A_EYEBALL; }
    s32 getHeadJointNo() { return JNT_HEAD; }
    s32 getNeckJointNo() { return JNT_NECK; }
    s32 getBackboneJointNo() { return JNT_BACKBONE1; }
    BOOL checkChangeJoint(int param_0) { return param_0 == JNT_HEAD; }
    BOOL checkRemoveJoint(int param_0) { return param_0 == JNT_MOUTH; }

    static char* mCutNameList[2];
    static cutFunc mCutList[2];

private:
    /* 0x0F7C */ u8 field_0xf7c[0xf80 - 0xf7c];
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

STATIC_ASSERT(sizeof(daNpc_grM_c) == 0x10e4);

struct daNpc_grM_HIOParam {
    /* 0x0 */ daNpcT_HIOParam common;
};

class daNpc_grM_HIO_c : public mDoHIO_entry_c {
public:
    /* 0x8 */ daNpc_grM_HIOParam param;
};

class daNpc_grM_Param_c {
public:
    virtual ~daNpc_grM_Param_c() {}

    static daNpc_grM_HIOParam const m;
};

#endif /* D_A_NPC_GRM_H */
