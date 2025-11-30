#ifndef D_A_NPC_GRD_H
#define D_A_NPC_GRD_H

#include "d/actor/d_a_npc4.h"

struct daNpc_Grd_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
};

class daNpc_Grd_Param_c {
public:
    virtual ~daNpc_Grd_Param_c() {}

    static daNpc_Grd_HIOParam const m;
};

#if DEBUG
class daNpc_Grd_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Grd_HIO_c();
    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    /* 0x8 */ daNpc_Grd_HIOParam m;
};
#define NPC_GRD_HIO_CLASS daNpc_Grd_HIO_c
#else
#define NPC_GRD_HIO_CLASS daNpc_Grd_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Grd_c
 * @brief Gor Coron
 *
 * @details
 *
 */
class daNpc_Grd_c : public daNpcF_c {
public:
    typedef int (daNpc_Grd_c::*actionFunc)(void*);
    typedef int (daNpc_Grd_c::*cutFunc)(int);

    daNpc_Grd_c();
    ~daNpc_Grd_c();
    int create();
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
    int isDelete();
    void reset();
    void playExpression();
    void playMotion();
    int chkAction(actionFunc);
    int setAction(actionFunc);
    int selectAction();
    void doNormalAction(int);
    BOOL doEvent();
    void setLookMode(int);
    void lookat();
    void setExpressionTalkAfter();
    int wait(void*);
    int talk(void*);
    int test(void*);
    BOOL ECut_nodToGrz(int);
    void adjustShapeAngle() {}

    static char* mEvtCutNameList[2];
    static cutFunc mEvtCutList[2];

private:
    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBDC */ daNpcF_Lookat_c mLookat;
    /* 0xC78 */ daNpcF_ActorMngr_c mActorMngr[3];
    /* 0xC90 */ NPC_GRD_HIO_CLASS* mpHIO;
    /* 0xC94 */ dCcD_Cyl mCyl;
    /* 0xDD0 */ actionFunc mNextAction;
    /* 0xDDC */ actionFunc mAction;
    /* 0xDE8 */ request_of_phase_process_class mPhase[3];
    /* 0xE00 */ int field_0xe00;
    /* 0xE04 */ int field_0xe04;
    /* 0xE08 */ int field_0xe08;
    /* 0xE0C */ int field_0xe0c;
    /* 0xE10 */ int mFlowID;
    /* 0xE14 */ s16 mLookMode;
    /* 0xE16 */ u16 mMode;
    /* 0xE18 */ u8 mType;
};

STATIC_ASSERT(sizeof(daNpc_Grd_c) == 0xe1c);

#endif /* D_A_NPC_GRD_H */
