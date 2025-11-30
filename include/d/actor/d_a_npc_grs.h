#ifndef D_A_NPC_GRS_H
#define D_A_NPC_GRS_H

#include "d/actor/d_a_npc4.h"

struct daNpc_grS_HIOParam {
    /* 0x000 */ daNpcF_HIOParam common;
};

class daNpc_grS_Param_c {
public:
    virtual ~daNpc_grS_Param_c() {}

    static const daNpc_grS_HIOParam m;
};

#if DEBUG
class daNpc_grS_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_grS_HIO_c();
    virtual ~daNpc_grS_HIO_c() {}

    void genMessage(JORMContext*);

    daNpc_grS_HIOParam m;
};

#define NPC_GRS_HIO_CLASS daNpc_grS_HIO_c
#else
#define NPC_GRS_HIO_CLASS daNpc_grS_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_grS_c
 * @brief Gor Amoto
 *
 * @details
 *
 */
class daNpc_grS_c : public daNpcF_c {
public:
    typedef int (daNpc_grS_c::*actionFunc)(void*);
    typedef int (daNpc_grS_c::*cutFunc)(int);

    daNpc_grS_c();
    virtual ~daNpc_grS_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    virtual void setParam();
    virtual int main();
    virtual int ctrlBtk();
    virtual void setAttnPos();
    virtual bool setExpressionAnm(int, bool);
    virtual bool setExpressionBtp(int);
    virtual void setExpression(int, f32);
    virtual void setMotionAnm(int, f32);
    virtual void setMotion(int, f32, int);
    virtual int drawDbgInfo();
    virtual void drawOtherMdls();
    u8 getTypeFromParam();
    int isDelete();
    void reset();
    void playExpression();
    void playMotion();
    int chkAction(actionFunc);
    int setAction(actionFunc);
    int selectAction();
    void doNormalAction(int);
    int doEvent();
    void setLookMode(int);
    void lookat();
    void setExpressionTalkAfter();
    int cutPushOut(int);
    int wait(void*);
    int talk(void*);
    int test(void*);
    void setPrtcl();
    void adjustShapeAngle() {}

    static char* mEvtCutNameList[2];
    static cutFunc mEvtCutList[2];

private:
    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBDC */ J3DModel* mpModel;
    /* 0xBE0 */ daNpcF_Lookat_c mLookat;
    /* 0xC7C */ daNpcF_ActorMngr_c mActorMngrs[2];
    /* 0xC8C */ NPC_GRS_HIO_CLASS* mpHIO;
    /* 0xC90 */ dCcD_Cyl mCyl;
    /* 0xDCC */ actionFunc mpNextActionFn;
    /* 0xDD8 */ actionFunc mpActionFn;
    /* 0xDE4 */ request_of_phase_process_class mPhases[2];
    /* 0xDF4 */ u8 field_0xdf4[0xdfc - 0xdf4];
    /* 0xDFC */ int field_0xdfc;
    /* 0xE00 */ int field_0xe00;
    /* 0xE04 */ int field_0xe04;
    /* 0xE08 */ int field_0xe08;
    /* 0xE0C */ int field_0xe0c;
    /* 0xE10 */ s16 mLookMode;
    /* 0xE12 */ u16 mActionState;
    /* 0xE14 */ u8 mType;
    /* 0xE18 */ fpc_ProcID mPresentItemId;
    /* 0xE1C */ int mParticleIds[6];
    /* 0xE34 */ bool mIsSpeaking;
};

STATIC_ASSERT(sizeof(daNpc_grS_c) == 0xe38);


#endif /* D_A_NPC_GRS_H */
