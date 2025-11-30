#ifndef D_A_NPC_SOLA_H
#define D_A_NPC_SOLA_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_solA_c
 * @brief  Soldier A (Castle Town?)
 *
 * @details
 *
 */

class daNpc_solA_c : public daNpcT_c {
public:
    typedef int (daNpc_solA_c::*cutFunc)(int);
    typedef int (daNpc_solA_c::*actionFunc)(void*);

    enum Joint {
        /* 0x00 */ JNT_CENTER,
        /* 0x01 */ JNT_BACKBONE1,
        /* 0x02 */ JNT_BACKBONE2,
        /* 0x03 */ JNT_NECK,
        /* 0x04 */ JNT_HEAD,
        /* 0x05 */ JNT_SHOULDERL,
        /* 0x06 */ JNT_ARM1L,
        /* 0x07 */ JNT_ARM2L,
        /* 0x08 */ JNT_HANDL,
        /* 0x09 */ JNT_FINGERL,
        /* 0x0A */ JNT_THUMBL,
        /* 0x0B */ JNT_SHOULDERR,
        /* 0x0C */ JNT_ARM1R,
        /* 0x0D */ JNT_ARM2R,
        /* 0x0E */ JNT_HANDR,
        /* 0x0F */ JNT_FINGERR,
        /* 0x10 */ JNT_THUMBR,
        /* 0x11 */ JNT_WAIST,
        /* 0x12 */ JNT_LEG1L,
        /* 0x13 */ JNT_LEG2L,
        /* 0x14 */ JNT_FOOTL,
        /* 0x15 */ JNT_LEG1R,
        /* 0x16 */ JNT_LEG2R,
        /* 0x17 */ JNT_FOOTR,
        /* 0x18 */ JNT_SAYA,
    };

    ~daNpc_solA_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    void Draw();
    static BOOL createHeapCallBack(fopAc_ac_c*);
    static BOOL ctrlJointCallBack(J3DJoint*, int);
    bool getType();
    int getFlowNodeNo();
    int isDelete();
    void reset();
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
    int selectAction();
    int chkAction(int (daNpc_solA_c::*)(void*));
    int setAction(int (daNpc_solA_c::*)(void*));
    int wait(void*);
    int talk(void*);
    daNpc_solA_c(daNpcT_faceMotionAnmData_c const* param_1,
                                daNpcT_motionAnmData_c const* param_2,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_3,
                                int param_4,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_5,
                                int param_6, daNpcT_evtData_c const* param_7, char** param_8)
        : daNpcT_c(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8) {}
    s32 getHeadJointNo() { return JNT_HEAD; }
    s32 getNeckJointNo() { return JNT_NECK; }
    s32 getBackboneJointNo() { return JNT_BACKBONE1; }

    static char* mCutNameList[1];
    static cutFunc mCutList[1];

private:
    /* 0xE40 */ u8 field_0xe40[0xe44 - 0xe40];
    /* 0xE44 */ dCcD_Cyl mCyl;
    /* 0xF80 */ u8 field_0xf80;
    /* 0xF84 */ actionFunc mNextAction;
    /* 0xF90 */ actionFunc mAction;
    /* 0xF9c */ u8 field_0xf9c;
};

STATIC_ASSERT(sizeof(daNpc_solA_c) == 0xfa0);

struct daNpc_solA_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
};

class daNpc_solA_Param_c {
public:
    virtual ~daNpc_solA_Param_c() {}

    static daNpc_solA_HIOParam const m;
};

#endif /* D_A_NPC_SOLA_H */
