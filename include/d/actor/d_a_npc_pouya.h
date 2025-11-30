#ifndef D_A_NPC_POUYA_H
#define D_A_NPC_POUYA_H

#include "d/actor/d_a_npc.h"

struct daNpc_Pouya_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
};

class daNpc_Pouya_Param_c {
public:
    virtual ~daNpc_Pouya_Param_c() {}

    static const daNpc_Pouya_HIOParam m;
};

#if DEBUG
class daNpc_Pouya_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Pouya_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_Pouya_HIOParam m;
};

#define NPC_POUYA_HIO_CLASS daNpc_Pouya_HIO_c
#else
#define NPC_POUYA_HIO_CLASS daNpc_Pouya_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Pouya_c
 * @brief Poe Merchant (Jovani?)
 *
 * @details
 *
 */
class daNpc_Pouya_c : public daNpcT_c {
public:
    typedef int (daNpc_Pouya_c::*cutFunc)(int);
    typedef int (daNpc_Pouya_c::*actionFunc)(void*);

    enum Type {
        TYPE_0,
        TYPE_1,
        TYPE_2,
        TYPE_3,
        TYPE_4,
        TYPE_5,
    };

    enum FaceMotion {
        /* 0x00 */ FACE_MOT_UNK_0 = 0,
        /* 0x01 */ FACE_MOT_UNK_1 = 1,
        /* 0x02 */ FACE_MOT_UNK_2 = 2,
        /* 0x03 */ FACE_MOT_UNK_3 = 3,
        /* 0x04 */ FACE_MOT_UNK_4 = 4,
        /* 0x05 */ FACE_MOT_UNK_5 = 5,
        /* 0x06 */ FACE_MOT_UNK_6 = 6,
        /* 0x07 */ FACE_MOT_UNK_7 = 7,
        /* 0x08 */ FACE_MOT_UNK_8 = 8,
        /* 0x09 */ FACE_MOT_UNK_9 = 9,
        /* 0x0A */ FACE_MOT_UNK_10 = 10,
        /* 0x0B */ FACE_MOT_UNK_11 = 11,
        /* 0x0C */ FACE_MOT_UNK_12 = 12,
        /* 0x0D */ FACE_MOT_UNK_13 = 13,
        /* 0x0E */ FACE_MOT_UNK_14 = 14,
        /* 0x0F */ FACE_MOT_UNK_15 = 15,
        /* 0x10 */ FACE_MOT_UNK_16 = 16,
        /* 0x11 */ FACE_MOT_UNK_17 = 17,
    };

    enum Motion {
        /* 0x00 */ MOT_UNK_0 = 0,
        /* 0x01 */ MOT_UNK_1 = 1,
        /* 0x04 */ MOT_UNK_4 = 4,
        /* 0x08 */ MOT_UNK_8 = 8,
        /* 0x0A */ MOT_UNK_10 = 10,
        /* 0x0F */ MOT_UNK_15 = 15,
    };

    ~daNpc_Pouya_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    BOOL isDelete();
    void reset();
    void ctrlSubFaceMotion(int);
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
    bool afterSetMotionAnm(int, int, f32, int);
    int selectAction();
    BOOL chkAction(int (daNpc_Pouya_c::*)(void*));
    int setAction(int (daNpc_Pouya_c::*)(void*));
    void setPrtcls();
    int cutHaveFavorToAsk(int);
    int wait(void*);
    int talk(void*);
    daNpc_Pouya_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {}
    u16 getEyeballMaterialNo() { return 1; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == 4; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == 8; }

    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xffff) {
            return -1;
        }
        return nodeNo;
    }

    BOOL chkPouyaB() { return (mType == TYPE_2 || mType == TYPE_3 || mType == TYPE_4); }

    MtxP getHeadMtx() { return mpMorf[0]->getModel()->getAnmMtx(4); }

    static char* mCutNameList[3];
    static cutFunc mCutList[3];

private:
    /* 0xE40 */ NPC_POUYA_HIO_CLASS* mpHIO;
    /* 0xE44 */ J3DModel* mpModel[1];
    /* 0xE48 */ dCcD_Cyl mCyl;
    /* 0xF84 */ u8 mType;
    /* 0xF88 */ daNpcT_Path_c mPath;
    /* 0xFB0 */ actionFunc field_0xfb0;
    /* 0xFBC */ actionFunc mAction;
    /* 0xFC8 */ u32 field_0xfc8;
    /* 0xFCC */ s8 field_0xfcc;
    /* 0xFCD */ s8 field_0xfcd;
    /* 0xFCE */ u8 field_0xfce;
    /* 0xFCF */ u8 field_0xfcf;
    /* 0xFD0 */ u8 field_0xfd0;
};

STATIC_ASSERT(sizeof(daNpc_Pouya_c) == 0xfd4);

#endif /* D_A_NPC_POUYA_H */
