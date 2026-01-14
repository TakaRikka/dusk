#ifndef D_A_NPC_ZANT_H
#define D_A_NPC_ZANT_H

#include "d/actor/d_a_npc.h"

struct daNpc_Zant_HIOParam {
    daNpcT_HIOParam common;
};

class daNpc_Zant_Param_c {
public:
    virtual ~daNpc_Zant_Param_c() {}

    static const daNpc_Zant_HIOParam m;
};

#if DEBUG
class daNpc_Zant_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Zant_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);

    void genMessage(JORMContext*);

    daNpc_Zant_HIOParam m;
};
#define NPC_ZANT_HIO_CLASS daNpc_Zant_HIO_c
#else
#define NPC_ZANT_HIO_CLASS daNpc_Zant_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Zant_c
 * @brief Zant
 *
 * @details
 *
 */
class daNpc_Zant_c : public daNpcT_c {
public:
    typedef int (daNpc_Zant_c::*cutFunc)(int);
    typedef int (daNpc_Zant_c::*actionFunc)(void*);

    enum Type {
        TYPE_0,
        TYPE_1,
    };

    ~daNpc_Zant_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    u32 getFlowNodeNo();
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
    int drawDbgInfo();
    void drawGhost();
    int selectAction();
    int chkAction(int (daNpc_Zant_c::*)(void*));
    int setAction(int (daNpc_Zant_c::*)(void*));
    int wait(void*);
    int talk(void*);
    daNpc_Zant_c(daNpcT_faceMotionAnmData_c const* param_1, daNpcT_motionAnmData_c const* param_2,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_3, int param_4,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_5, int param_6,
                                daNpcT_evtData_c const* param_7, char** param_8) :
                                daNpcT_c(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8) {}
    
    static char* mCutNameList;
    static cutFunc mCutList[1];

private:
    /* 0xE40 */ NPC_ZANT_HIO_CLASS* mpHIO;
    /* 0xE44 */ dCcD_Cyl mCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF81 */ u8 field_0xf81[0xf84 - 0xf81];
    /* 0xF84 */ actionFunc field_0xf84;
    /* 0xF90 */ actionFunc field_0xf90;
    /* 0xF9C */ int field_0xf9c;
};

STATIC_ASSERT(sizeof(daNpc_Zant_c) == 0xfa0);

#endif /* D_A_NPC_ZANT_H */
