#ifndef D_A_NPC_KYURY_H
#define D_A_NPC_KYURY_H

#include "d/actor/d_a_npc.h"

struct daNpc_Kyury_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
};

class daNpc_Kyury_Param_c {
public:
    virtual ~daNpc_Kyury_Param_c() {}

    static const daNpc_Kyury_HIOParam m;
};

#if DEBUG
class daNpc_Kyury_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Kyury_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_Kyury_HIOParam m;
};

#define NPC_KYURY_HIO_CLASS daNpc_Kyury_HIO_c
#else
#define NPC_KYURY_HIO_CLASS daNpc_Kyury_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Kyury_c
 * @brief Pergie
 *
 * @details
 *
 */
class daNpc_Kyury_c : public daNpcT_c {
public:
    typedef int (daNpc_Kyury_c::*cutFunc)(int);
    typedef int (daNpc_Kyury_c::*actionFunc)(void*);

    enum Type {
        TYPE_0,
        TYPE_1,
        TYPE_2,
        TYPE_3,
    };

    enum FaceMotion {
        /* 0x01 */ FACE_MOT_UNK_1 = 1,
        /* 0x05 */ FACE_MOT_UNK_5 = 5,
        /* 0x06 */ FACE_MOT_UNK_6 = 6,
        /* 0x07 */ FACE_MOT_UNK_7 = 7,
        /* 0x08 */ FACE_MOT_UNK_8 = 8,
        /* 0x09 */ FACE_MOT_UNK_9 = 9,
        /* 0x0A */ FACE_MOT_UNK_10 = 10,
        /* 0x0C */ FACE_MOT_UNK_12 = 12,
        /* 0x0D */ FACE_MOT_UNK_13 = 13,
        /* 0x0E */ FACE_MOT_UNK_14 = 14,
        /* 0x0F */ FACE_MOT_UNK_15 = 15,
    };

    enum Motion {
        /* 0x00 */ MOT_UNK_0 = 0,
        /* 0x05 */ MOT_UNK_5 = 5,
        /* 0x08 */ MOT_UNK_8 = 8,
        /* 0x0C */ MOT_UNK_12 = 12,
    };

    ~daNpc_Kyury_c();
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
    BOOL checkChangeEvt();
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
    int chkAction(int (daNpc_Kyury_c::*)(void*));
    int setAction(int (daNpc_Kyury_c::*)(void*));
    int cutConversation(int);
    int wait(void*);
    int talk(void*);
    daNpc_Kyury_c(daNpcT_faceMotionAnmData_c const* param_1,
                                 daNpcT_motionAnmData_c const* param_2,
                                 daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_3,
                                 int param_4,
                                 daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_5,
                                 int param_6, daNpcT_evtData_c const* param_7, char** param_8)
        : daNpcT_c(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8) {}
    u16 getEyeballMaterialNo() { return 2; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == 4; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == 10; }

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xFFFF) {
            return -1;
        }
        return nodeNo;
    }

    static char* mCutNameList[2];
    static cutFunc mCutList[2];

private:
    /* 0xE40 */ NPC_KYURY_HIO_CLASS* mpHIO;
    /* 0xE44 */ J3DModel* mpModel[2];
    /* 0xE4C */ dCcD_Cyl mCyl;
    /* 0xF88 */ u8 mType;
    /* 0xF8C */ daNpcT_ActorMngr_c mActorMngr[1];
    /* 0xF94 */ daNpcT_Path_c mPath;
    /* 0xFBC */ actionFunc field_0xfbc[2];
    /* 0xFD4 */ u8 field_0xfd4;
    /* 0xFD5 */ u8 field_0xfd5;
    /* 0xFD8 */ int field_0xfd8;
};

STATIC_ASSERT(sizeof(daNpc_Kyury_c) == 0xfdc);

#endif /* D_A_NPC_KYURY_H */
