#ifndef D_A_NPC_SEIB_H
#define D_A_NPC_SEIB_H

#include "d/actor/d_a_npc.h"

struct daNpc_seiB_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 field_0x8c;
    /* 0x90 */ f32 mDist;
};

class daNpc_seiB_Param_c {
public:
    virtual ~daNpc_seiB_Param_c() {};

    static const daNpc_seiB_HIOParam m;
};

#if DEBUG
class daNpc_seiB_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_seiB_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);

    void genMessage(JORMContext*);

    daNpc_seiB_HIOParam m;
};

#define NPC_SEIB_HIO_CLASS daNpc_seiB_HIO_c
#else
#define NPC_SEIB_HIO_CLASS daNpc_seiB_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_seiB_c
 * @brief Faron Spirit
 *
 * @details
 *
 */
class daNpc_seiB_c : public daNpcT_c {
public:
    typedef int (daNpc_seiB_c::*cutFunc)(int);
    typedef int (daNpc_seiB_c::*actionFunc)(void*);

    enum Type {
        TYPE_0,
        TYPE_1,
    };

    ~daNpc_seiB_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    u8 getType();
    int getFlowNodeNo();
    int isDelete();
    void reset();
    void setParam();
    void srchActors();
    BOOL evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void setAttnPos();
    int drawDbgInfo();
    bool afterSetMotionAnm(int, int, f32, int);
    int selectAction();
    int chkAction(int (daNpc_seiB_c::*)(void*));
    int setAction(int (daNpc_seiB_c::*)(void*));
    void ctrlWaitAnm();
    int wait(void*);
    int talk(void*);
    daNpc_seiB_c(daNpcT_faceMotionAnmData_c const* param_1, daNpcT_motionAnmData_c const* param_2,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_3, int param_4,
                                daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_5, int param_6,
                                daNpcT_evtData_c const* param_7, char** param_8) :
                                daNpcT_c(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8)
                                {}

    static char* mCutNameList;
    static cutFunc mCutList[1];

private:
    /* 0xE40 */ NPC_SEIB_HIO_CLASS* mpHIO;
    /* 0xE44 */ u8 mType;
    /* 0xE48 */ actionFunc mActionFunc1;
    /* 0xE54 */ actionFunc mActionFunc2;
    /* 0xE60 */ int mAnim;
    /* 0xE64 */ void* field_0xe64;
};

STATIC_ASSERT(sizeof(daNpc_seiB_c) == 0xe68);

#endif /* D_A_NPC_SEIB_H */
