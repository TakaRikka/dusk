#ifndef D_A_NPC_GRR_H
#define D_A_NPC_GRR_H

#include "d/actor/d_a_npc4.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_grR_c
 * @brief Gor Liggs
 *
 * @details
 *
*/

struct daNpc_grR_HIOParam {
    /* 0x0 */ daNpcF_HIOParam common;
};

class daNpc_grR_HIO_c
#if DEBUG
: public mDoHIO_entry_c
#endif
{
    /* 0x8 */ daNpc_grR_HIOParam param;
};

class daNpc_grR_Param_c {
public:
    virtual ~daNpc_grR_Param_c() {}

    static daNpc_grR_HIOParam const m;
};

class daNpc_grR_c : public daNpcF_c {
public:
    typedef int (daNpc_grR_c::*cutFunc)(int);
    typedef int (daNpc_grR_c::*actionFunc)(void*);
    daNpc_grR_c();
    ~daNpc_grR_c();
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
    BOOL drawDbgInfo();
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
    int waitMaro(void*);
    int talk(void*);
    int test(void*);
    void adjustShapeAngle() {}

    static char* mEvtCutNameList;
    static cutFunc mEvtCutList[1];

private:
    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBDC */ daNpcF_Lookat_c mLookat;
    /* 0xC78 */ daNpcF_ActorMngr_c mActorMngr[2];
    /* 0xC88 */ daNpc_grR_HIO_c* mHIO;
    /* 0xC8C */ dCcD_Cyl mCyl;
    /* 0xDC8 */ actionFunc mNextAction;
    /* 0xDD4 */ actionFunc mAction;
    /* 0xDE0 */ request_of_phase_process_class mPhases[3];
    /* 0xDF8 */ int field_0xdf8;
    /* 0xDFC */ int field_0xdfc;
    /* 0xE00 */ int field_0xe00;
    /* 0xE04 */ int field_0xe04;
    /* 0xE08 */ int mFlowID;
    /* 0xE0C */ s16 mLookMode;
    /* 0xE0E */ u16 mMode;
    /* 0xE10 */ u8 mType;
    /* 0xE14 */ fpc_ProcID mItemID;
    /* 0xE18 */ bool field_0xe18;
    /* 0xE1C */ int mAnm;
};

STATIC_ASSERT(sizeof(daNpc_grR_c) == 0xe20);

#endif /* D_A_NPC_GRR_H */
