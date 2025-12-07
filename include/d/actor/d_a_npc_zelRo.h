#ifndef D_A_NPC_ZELRO_H
#define D_A_NPC_ZELRO_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_ZelRo_c
 * @brief Zelda (Cloaked)
 *
 * @details
 *
*/

struct daNpc_ZelRo_HIOParam {
    /* 0x0 */ daNpcT_HIOParam common;
};

class daNpc_ZelRo_HIO_c : public mDoHIO_entry_c {
    /* 0x8 */ daNpc_ZelRo_HIOParam param;
};

class daNpc_ZelRo_Param_c {
public:
    virtual ~daNpc_ZelRo_Param_c() {}

    static daNpc_ZelRo_HIOParam const m;
};

class daNpc_ZelRo_c : public daNpcT_c {
public:
    enum Material {
        /* 0x0 */ SC_EYE_L_V,
        /* 0x1 */ SC_EYE_R_V,
        /* 0x2 */ ZELR_ROBE01_M_V,
        /* 0x3 */ ZELR_ROBE02_M_V,
        /* 0x4 */ ZELDA_BODY_M_V,
        /* 0x5 */ ZELDA_DOUBLESIDE_M_V,
        /* 0x6 */ ZELDA_EYEBALL_L_V,
        /* 0x7 */ ZELDA_EYEBALL_R_V,
        /* 0x8 */ ZELDA_HAIR_M_V,
        /* 0x9 */ ZELDA_JEWEL_M_V,
    };

    enum Joint {
        /* 0x00 */ JNT_CENTER,
        /* 0x01 */ JNT_BACKBONE1,
        /* 0x02 */ JNT_BACKBONE2,
        /* 0x03 */ JNT_NECK,
        /* 0x04 */ JNT_HEAD,
        /* 0x05 */ JNT_CHIN,
        /* 0x06 */ JNT_MANT,
        /* 0x07 */ JNT_MAYUL,
        /* 0x08 */ JNT_MAYUR,
        /* 0x09 */ JNT_MOMIL1,
        /* 0x0A */ JNT_MOMIL2,
        /* 0x0B */ JNT_MOMIL3,
        /* 0x0C */ JNT_MOMIR1,
        /* 0x0D */ JNT_MOMIR2,
        /* 0x0E */ JNT_MOMIR3,
        /* 0x0F */ JNT_MOUTH,
        /* 0x10 */ JNT_SHOULDERL,
        /* 0x11 */ JNT_ARML1,
        /* 0x12 */ JNT_ARML2,
        /* 0x13 */ JNT_HANDL,
        /* 0x14 */ JNT_FINGERL1,
        /* 0x15 */ JNT_FINGERL2,
        /* 0x16 */ JNT_THAMBL,
        /* 0x17 */ JNT_SHOULDERR,
        /* 0x18 */ JNT_ARMR1,
        /* 0x19 */ JNT_ARMR2,
        /* 0x1A */ JNT_HANDR,
        /* 0x1B */ JNT_FINGERR1,
        /* 0x1C */ JNT_FINGERR2,
        /* 0x1D */ JNT_THAMBR,
        /* 0x1E */ JNT_WAIST,
        /* 0x1F */ JNT_SKARTL_01,
        /* 0x20 */ JNT_SKARTL_02,
        /* 0x21 */ JNT_SKARTL_03,
        /* 0x22 */ JNT_SKARTR_01,
        /* 0x23 */ JNT_SKARTR_02,
        /* 0x24 */ JNT_SKARTR_03,
    };

    typedef int (daNpc_ZelRo_c::*actionFunc)(void*);
    typedef int (daNpc_ZelRo_c::*cutFunc)(int);

    ~daNpc_ZelRo_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    int getFlowNodeNo();
    u8 getPath();
    BOOL isDelete();
    void reset();
    void afterJntAnm(int);
    BOOL ctrlBtk();
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
    BOOL selectAction();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    int wait(void*);
    int talk(void*);
    int test(void*);
    daNpc_ZelRo_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {}
    u16 getEyeballRMaterialNo() { return ZELDA_EYEBALL_R_V; }
    u16 getEyeballLMaterialNo() { return ZELDA_EYEBALL_L_V; }
    s32 getHeadJointNo() { return JNT_HEAD; }
    s32 getNeckJointNo() { return JNT_NECK; }
    s32 getBackboneJointNo() { return JNT_BACKBONE1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == JNT_HEAD; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == JNT_MOUTH; }

    static char* mCutNameList;
    static cutFunc mCutList[1];

private:
    /* 0xE40 */ daNpc_ZelRo_HIO_c* mHIO;
    /* 0xE44 */ dCcD_Cyl mCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF84 */ actionFunc mNextAction;
    /* 0xF90 */ actionFunc mAction;
    /* 0xF9C */ daNpcT_Path_c mPath;
    /* 0xFC4 */ u8 field_0xfc4;
};

STATIC_ASSERT(sizeof(daNpc_ZelRo_c) == 0xfc8);

#endif /* D_A_NPC_ZELRO_H */
