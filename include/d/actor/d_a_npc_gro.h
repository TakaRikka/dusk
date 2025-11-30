#ifndef D_A_NPC_GRO_H
#define D_A_NPC_GRO_H

#include "d/actor/d_a_npc4.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_grO_c
 * @brief Gor Ebizo
 *
 * @details
 *
*/

struct daNpc_grO_HIOParam {
    /* 0x0 */ daNpcF_HIOParam common;
};

class daNpc_grO_Param_c {
public:
    virtual ~daNpc_grO_Param_c() {}

    static daNpc_grO_HIOParam const m;
};

#if DEBUG
class daNpc_grO_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_grO_HIO_c();
    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    /* 0x8 */ daNpc_grO_HIOParam m;
};
#define NPC_GRO_HIO_CLASS daNpc_grO_HIO_c
#else
#define NPC_GRO_HIO_CLASS daNpc_grO_Param_c
#endif

class daNpc_grO_c : public daNpcF_c {
public:
    typedef int (daNpc_grO_c::*actionFunc)(void*);
    typedef int (daNpc_grO_c::*cutFunc)(int);
    
    daNpc_grO_c();
    ~daNpc_grO_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    void setParam();
    BOOL main();
    BOOL ctrlBtk();
    void setAttnPos();
    bool setExpressionAnm(int, bool);
    bool setExpressionBtp(int);
    void setExpression(int, f32);
    void setMotionAnm(int, f32);
    void setMotion(int, f32, int);
    int drawDbgInfo();
    void drawOtherMdls();
    u8 getTypeFromParam();
    BOOL isDelete();
    void reset();
    void playExpression();
    void playMotion();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    BOOL selectAction();
    void doNormalAction(int);
    BOOL doEvent();
    void setLookMode(int);
    void lookat();
    void setExpressionTalkAfter();
    int wait(void*);
    int bokinWait(void*);
    int waitMaro(void*);
    int talk(void*);
    int ECut_bokinFinish(int);
    int cutPushOut(int);
    int test(void*);
    void adjustShapeAngle() {}

    static char* mEvtCutNameList[3];
    static cutFunc mEvtCutList[3];

private:
    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ J3DModel* mpOtherModel[1];
    /* 0xBDC */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBE0 */ daNpcF_Lookat_c mLookat;
    /* 0xC7C */ daNpcF_ActorMngr_c mActorMngrs[3];
    /* 0xC94 */ NPC_GRO_HIO_CLASS* mpHIO;
    /* 0xC98 */ dCcD_Cyl mCyl;
    /* 0xDD4 */ actionFunc mNextAction;
    /* 0xDE0 */ actionFunc mAction;
    /* 0xDEC */ request_of_phase_process_class mPhases[4];
    /* 0xE0C */ int field_0xe0c;
    /* 0xE10 */ int field_0xe10;
    /* 0xE14 */ int field_0xe14;
    /* 0xE18 */ int field_0xe18;
    /* 0xE1C */ int mMsgNo;
    /* 0xE20 */ s16 mLookMode;
    /* 0xE22 */ u16 mMode;
    /* 0xE24 */ u8 mType;
    /* 0xE28 */ fpc_ProcID mItemID;
    /* 0xE2C */ u8 mUnkFlag;
};

STATIC_ASSERT(sizeof(daNpc_grO_c) == 0xe30);

#endif /* D_A_NPC_GRO_H */
