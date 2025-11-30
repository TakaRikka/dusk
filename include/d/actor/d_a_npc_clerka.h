#ifndef D_A_NPC_CLERKA_H
#define D_A_NPC_CLERKA_H

#include "d/actor/d_a_npc.h"
#include "d/d_shop_system.h"

struct daNpc_clerkA_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ s16 field_0x8c;
    /* 0x8F */ s16 field_0x8f;
};

class daNpc_clerkA_Param_c {
public:
    virtual ~daNpc_clerkA_Param_c() {}

    static const daNpc_clerkA_HIOParam m;
};

#if DEBUG
class daNpc_clerkA_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_clerkA_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_clerkA_HIOParam m;
};

#define NPC_CLERKA_HIO_CLASS daNpc_clerkA_HIO_c
#else
#define NPC_CLERKA_HIO_CLASS daNpc_clerkA_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_clerkA_c
 * @brief Chudley
 *
 * @details
 *
 */
class daNpc_clerkA_c : public dShopSystem_c {
public:
    typedef int (daNpc_clerkA_c::*cutFunc)(int);
    typedef int (daNpc_clerkA_c::*actionFunc)(void*);

    enum Type {
        TYPE_SHOP,
        TYPE_1,
    };

    enum FaceMotion {
        /* 0x01 */ FACE_MOT_UNK_1 = 1,
        /* 0x02 */ FACE_MOT_UNK_2 = 2,
        /* 0x03 */ FACE_MOT_UNK_3 = 3,
        /* 0x04 */ FACE_MOT_UNK_4 = 4,
    };

    enum Motion {
        /* 0x0 */ MOT_UNK_0 = 0,
        /* 0x2 */ MOT_UNK_2 = 2,
        /* 0x3 */ MOT_UNK_3 = 3,
        /* 0x6 */ MOT_UNK_6 = 6,
    };

    ~daNpc_clerkA_c();
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
    int chkAction(int (daNpc_clerkA_c::*)(void*));
    int setAction(int (daNpc_clerkA_c::*)(void*));
    int wait(void*);
    int tend(void*);
    int talk(void*);
    int shop(void*);
    daNpc_clerkA_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : dShopSystem_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                        i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                        i_arcNames) {}
    u16 getEyeballMaterialNo() { return 3; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == 4; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == 8; }

    u32 getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        u32 rv;
        if (nodeNo == 0xffff) {
            rv = -1;
        } else {
            rv = nodeNo;
        }
        return rv;
    }

    u8 getMaxNumItem() { return (fopAcM_GetParam(this) & 0xF000000) >> 24; }

    static char* mCutNameList[1];
    static cutFunc mCutList[1];

private:
    /* 0x0F7C */ NPC_CLERKA_HIO_CLASS* mpHIO;
    /* 0x0F80 */ dCcD_Cyl mCyl;
    /* 0x10BC */ u8 mType;
    /* 0x10C0 */ daNpcT_ActorMngr_c mActorMngr[1];
    /* 0x10C8 */ actionFunc mNextAction;
    /* 0x10D4 */ actionFunc mAction;
    /* 0x10E0 */ int mShopProcess;
    /* 0x10E4 */ int field_0x10e4;
    /* 0x10E8 */ int field_0x10e8;
    /* 0x10EC */ u8 field_0x10ec;
    /* 0x10ED */ u8 field_0x10ed;
    /* 0x10EE */ u8 field_0x10ee;
    /* 0x10EF */ u8 mShopFlag;
    /* 0x10F0 */ u8 field_0x10f0;
};

STATIC_ASSERT(sizeof(daNpc_clerkA_c) == 0x10f4);

#endif /* D_A_NPC_CLERKA_H */
