#ifndef D_A_NPC_SOLDIERB_H
#define D_A_NPC_SOLDIERB_H

#include "d/actor/d_a_npc4.h"

struct daNpc_SoldierB_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
};

class daNpc_SoldierB_Param_c {
public:
    virtual ~daNpc_SoldierB_Param_c() {}

    static daNpc_SoldierB_HIOParam const m;
};

#if DEBUG
class daNpc_SoldierB_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_SoldierB_HIO_c();
    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_SoldierB_HIOParam m;
};
#define NPC_SOLDIERB_HIO_CLASS daNpc_SoldierB_HIO_c
#else
#define NPC_SOLDIERB_HIO_CLASS daNpc_SoldierB_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_SoldierB_c
 * @brief Soldier B (Castle Town?)
 *
 * @details
 *
 */
class daNpc_SoldierB_c : public daNpcF_c {
public:
    typedef int (daNpc_SoldierB_c::*actionFunc)(void*);
    typedef int (daNpc_SoldierB_c::*cutFunc)(int);

    daNpc_SoldierB_c();
    ~daNpc_SoldierB_c();
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
    void setMotionAnm(int, f32);
    void setMotion(int, f32, int);
    int drawDbgInfo();
    void drawOtherMdls();
    u8 getTypeFromParam();
    BOOL isDelete();
    void reset();
    void playMotion();
    BOOL chkAction(int (daNpc_SoldierB_c::*)(void*));
    BOOL setAction(int (daNpc_SoldierB_c::*)(void*));
    BOOL selectAction();
    void doNormalAction(int);
    BOOL doEvent();
    void setLookMode(int);
    void lookat();
    int wait(void*);
    int talk(void*);
    int ECut_listenLake(int);
    int test(void*);

    static char* mEvtCutNameList[2];
    static cutFunc mEvtCutList[2];

private:
    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ J3DModel* mSpearModel;
    /* 0xBDC */ daNpcF_Lookat_c mLookat;
    /* 0xC78 */ daNpcF_ActorMngr_c mActorMngrs[3];
    /* 0xC90 */ NPC_SOLDIERB_HIO_CLASS* mHIO;
    /* 0xC94 */ dCcD_Cyl mCyl;
    /* 0xDD0 */ actionFunc mNextAction;
    /* 0xDDC */ actionFunc mAction;
    /* 0xDE8 */ request_of_phase_process_class mPhases[3];
    /* 0xE00 */ int field_0xe00;
    /* 0xE04 */ int field_0xe04;
    /* 0xE08 */ int field_0xe08;
    /* 0xE0C */ int field_0xe0c;
    /* 0xE10 */ int mMsgNo;
    /* 0xE14 */ s16 mLookMode;
    /* 0xE16 */ u16 mMode;
    /* 0xE18 */ u8 mType;
    /* 0xE1C */ int mAnm;
};

STATIC_ASSERT(sizeof(daNpc_SoldierB_c) == 0xe20);

#endif /* D_A_NPC_SOLDIERB_H */
