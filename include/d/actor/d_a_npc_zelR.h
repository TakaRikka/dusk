#ifndef D_A_NPC_ZELR_H
#define D_A_NPC_ZELR_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_ZelR_c
 * @brief Zelda (Cloaked & Hooded)
 *
 * @details
 *
*/

struct daNpc_ZelR_HIOParam {
    /* 0x0 */ daNpcT_HIOParam common;
};

class daNpc_ZelR_HIO_c : public mDoHIO_entry_c {
    /* 0x8 */ daNpc_ZelR_HIOParam param;
};

class daNpc_ZelR_Param_c {
public:
    virtual ~daNpc_ZelR_Param_c() {};

    static const daNpc_ZelR_HIOParam m;
};

class daNpc_ZelR_c : public daNpcT_c {
public:
    typedef BOOL (daNpc_ZelR_c::*ActionFn)(void*);
    typedef BOOL (daNpc_ZelR_c::*EventFn)(int);

    enum Type {
        TYPE_0,
        TYPE_1,
    };

    ~daNpc_ZelR_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    u32 getFlowNodeNo();
    u8 getPath();
    int isDelete();
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
    int selectAction();
    int chkAction(int (daNpc_ZelR_c::*)(void*));
    int setAction(int (daNpc_ZelR_c::*)(void*));
    int wait(void*);
    BOOL talk(void*);
    daNpc_ZelR_c(daNpcT_faceMotionAnmData_c const* param_1, daNpcT_motionAnmData_c const* param_2,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_3, int param_4,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_5, int param_6,
                                daNpcT_evtData_c const* param_7, char** param_8) :
                                daNpcT_c(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8)
                                 {}
    u16 getEyeballRMaterialNo() { return 3; }
    u16 getEyeballLMaterialNo() { return 2; }
    s32 getHeadJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int param_1) { return param_1 == 3; };
    BOOL checkRemoveJoint(int param_1) { return param_1 == 13; };

    static char* mCutNameList;
    static EventFn mCutList[1];

private:
    /* 0xE40 */ u8 field_0xe40[0xe44 - 0xe40];
    /* 0xE44 */ dCcD_Cyl mCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF84 */ ActionFn field_0xf84;
    /* 0xF90 */ ActionFn field_0xf90;
    /* 0xF9C */ daNpcT_Path_c mPath;
    /* 0xFBE */ int field_0xfc4;
};

STATIC_ASSERT(sizeof(daNpc_ZelR_c) == 0xfc8);


#endif /* D_A_NPC_ZELR_H */
