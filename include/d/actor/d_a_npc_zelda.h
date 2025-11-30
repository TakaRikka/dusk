#ifndef D_A_NPC_ZELDA_H
#define D_A_NPC_ZELDA_H

#include "d/actor/d_a_npc.h"

class daNpc_Zelda_HIO_c;
class daNpc_Zelda_c;

/**
 * @ingroup actors-npcs
 * @class daNpc_Zelda_c
 * @brief Zelda
 *
 * @details
 *
 */
class daNpc_Zelda_c : public daNpcT_c {
public:
    typedef int (daNpc_Zelda_c::*cutFunc)(int);
    typedef int (daNpc_Zelda_c::*actionFunc)(void*);

    virtual ~daNpc_Zelda_c();
    u8 getPathID() {
        return (fopAcM_GetParam(this) & 0xff00) >> 8;
    }
    int getFlowNodeNo() {
        u16 pitch = home.angle.x;
        return pitch == 0xffff ? -1 : pitch;
    }
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    int isDelete();
    void reset();
    void afterJntAnm(int);
    int ctrlBtk();
    int checkChangeEvt();
    void setParam();
    void setAfterTalkMotion();
    void srchActors();
    int evtTalk();
    int evtCutProc();
    void action();
    void beforeMove();
    void setAttnPos();
    void setCollision();
    int drawDbgInfo();
    int selectAction();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    int wait(void*);
    int talk(void*);
    daNpc_Zelda_c(daNpcT_faceMotionAnmData_c const* param_0,
                                 daNpcT_motionAnmData_c const* param_1,
                                 daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_2,
                                 int param_3,
                                 daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_4,
                                 int param_5, daNpcT_evtData_c const* param_6, char** param_7) :
    daNpcT_c(param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7) {
        OS_REPORT("|%06d:%x|daNpc_Zelda_c -> コンストラクト\n", g_Counter.mCounter0, this);
    }
    u16 getEyeballRMaterialNo() { return 5; }
    u16 getEyeballLMaterialNo() { return 4; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    int checkChangeJoint(int param_0) { return param_0 == 4; }
    int checkRemoveJoint(int param_0) { return param_0 == 17; }

    static const char* mCutNameList;
    static cutFunc mCutList[1];

private:
    /* 0xE40 */ daNpc_Zelda_HIO_c* mHIO;
    /* 0xE44 */ dCcD_Cyl mCyl;
    /* 0xF80 */ u8 field_0xf80;
    /* 0xF84 */ actionFunc mAction1;
    /* 0xF90 */ actionFunc mAction2;
    /* 0xF9C */ u8 field_0xf9c[0xfa0 - 0xf9c];
};

STATIC_ASSERT(sizeof(daNpc_Zelda_c) == 0xfa0);

class daNpc_Zelda_HIOParam {
public:
    /* 0x00 */ daNpcT_HIOParam common;
};

STATIC_ASSERT(sizeof(daNpc_Zelda_HIOParam) == 0x8c);

class daNpc_Zelda_Param_c {
public:
    virtual ~daNpc_Zelda_Param_c() {}

    static const daNpc_Zelda_HIOParam m;
};

class daNpc_Zelda_HIO_c
#if DEBUG
    : public mDoHIO_entry_c
#endif
{
public:
    daNpc_Zelda_HIO_c();

    void genMessage(JORMContext* ctx);
    void listenPropertyEvent(const JORPropertyEvent*);

#if DEBUG
    daNpc_Zelda_HIOParam param;
#endif
};

#endif /* D_A_NPC_ZELDA_H */
