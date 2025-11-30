#ifndef D_A_NPC_GRC_H
#define D_A_NPC_GRC_H

#include "d/actor/d_a_npc4.h"
#include "d/d_particle_copoly.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_grC_c
 * @brief Goron (Child)
 *
 * @details
 *
*/


class daNpc_grC_c : public daNpcF_c {
public:
    typedef BOOL (daNpc_grC_c::*ActionFn)(void*);
    typedef BOOL (daNpc_grC_c::*EventFn)(int);

    daNpc_grC_c();
    ~daNpc_grC_c();
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
    BOOL chkAction(ActionFn);
    BOOL setAction(ActionFn);
    BOOL selectAction();
    void doNormalAction(int);
    BOOL doEvent();
    void setLookMode(int);
    void lookat();
    BOOL chkFindPlayer();
    void setExpressionTalkAfter();
    BOOL wait(void*);
    BOOL waitTW(void*);
    BOOL waitSpa(void*);
    BOOL waitBuyer(void*);
    BOOL talk(void*);
    BOOL test(void*);
    void setPrtcl();
    void adjustShapeAngle() {}

    static char* mEvtCutNameList;
    static EventFn mEvtCutList[1];

private:
    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ int field_0xbd8;
    /* 0xBDC */ daNpcF_Lookat_c mLookat;
    /* 0xC78 */ dPaPo_c mPaPo;
    /* 0xCB0 */ daNpcF_ActorMngr_c mActorMngr[2];
    /* 0xCC0 */ u8 field_0xcc0[0xcc4 - 0xcc0];
    /* 0xCC4 */ dCcD_Cyl mCyl;
    /* 0xE00 */ ActionFn mNextAction;
    /* 0xE0C */ ActionFn mAction;
    /* 0xE18 */ request_of_phase_process_class mPhases[3];
    /* 0xE30 */ int field_0xe30;
    /* 0xE34 */ int field_0xe34;
    /* 0xE38 */ int field_0xe38;
    /* 0xE3C */ int field_0xe3c;
    /* 0xE40 */ int mFlowID;
    /* 0xE44 */ s16 mLookMode;
    /* 0xE46 */ u16 mMode;
    /* 0xE48 */ u8 mType;
    /* 0xE4C */ fpc_ProcID mItemID;
    /* 0xE50 */ bool mIsSpringWaterShopClosed;
    /* 0xE51 */ bool field_0xe51;
};

STATIC_ASSERT(sizeof(daNpc_grC_c) == 0xe54);

struct daNpc_grC_HIOParam {
    /* 0x0 */ daNpcF_HIOParam common;
};

class daNpc_grC_HIO_c : public mDoHIO_entry_c {
    /* 0x8 */ daNpc_grC_HIOParam field_0x8;
};

class daNpc_grC_Param_c {
public:
    virtual ~daNpc_grC_Param_c() {}

    static daNpc_grC_HIOParam const m;
};


#endif /* D_A_NPC_GRC_H */
