#ifndef D_A_NPC_POST_H
#define D_A_NPC_POST_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Post_c
 * @brief Postman
 *
 * @details
 *
*/

struct daNpc_Post_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 run_spd;
    /* 0x90 */ s16 nod_interval;
};

class daNpc_Post_Param_c {
public:
    virtual ~daNpc_Post_Param_c() {}

    static daNpc_Post_HIOParam const m;
};

#if DEBUG
class daNpc_Post_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Post_HIO_c();
    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    /* 0x8 */ daNpc_Post_HIOParam m;
};
#define NPC_POST_HIO_CLASS daNpc_Post_HIO_c
#else
#define NPC_POST_HIO_CLASS daNpc_Post_Param_c
#endif

class daNpc_Post_c : public daNpcT_c {
public:
    enum Material {
        /* 0x0 */ MAT_SC_EYE,
        /* 0x1 */ MAT_EYEBALL,
        /* 0x2 */ MAT_POST_BODY_M,
        /* 0x3 */ MAT_POST_FACE_M,
    };

    enum Joint {
        /* 0x00 */ JNT_CENTER,
        /* 0x01 */ JNT_BACKBONE1,
        /* 0x02 */ JNT_BACKBONE2,
        /* 0x03 */ JNT_NECK,
        /* 0x04 */ JNT_HEAD,
        /* 0x05 */ JNT_CHIN,
        /* 0x06 */ JNT_MAYU_L,
        /* 0x07 */ JNT_MAYU_R,
        /* 0x08 */ JNT_MOUTH,
        /* 0x09 */ JNT_SHOULDERL,
        /* 0x0A */ JNT_ARML1,
        /* 0x0B */ JNT_ARML2,
        /* 0x0C */ JNT_HANDL,
        /* 0x0D */ JNT_FINGERL,
        /* 0x0E */ JNT_THAMBL,
        /* 0x0F */ JNT_SHOULDERR,
        /* 0x10 */ JNT_ARMR1,
        /* 0x11 */ JNT_ARMR2,
        /* 0x12 */ JNT_HANDR,
        /* 0x13 */ JNT_FINGERR,
        /* 0x14 */ JNT_THAMBR,
        /* 0x15 */ JNT_WAIST,
        /* 0x16 */ JNT_LEGL1,
        /* 0x17 */ JNT_LEGL2,
        /* 0x18 */ JNT_FOOTL,
        /* 0x19 */ JNT_LEGR1,
        /* 0x1A */ JNT_LEGR2,
        /* 0x1B */ JNT_FOOTR,
    };

    typedef int (daNpc_Post_c::*actionFunc)(void*);
    typedef int (daNpc_Post_c::*cutFunc)(int);

    ~daNpc_Post_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    BOOL isDelete();
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
    void afterMoved();
    void setAttnPos();
    void setCollision();
    int drawDbgInfo();
    void drawOtherMdl();
    BOOL setFlagAnm(int, int, f32);
    bool afterSetMotionAnm(int, int, f32, int);
    BOOL selectAction();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    BOOL chkPullOutLetter1();
    BOOL chkPullOutLetter2();
    void pullOutLetter();
    int cutDeliver(int);
    int wait(void*);
    int talk(void*);
    int test(void*);
    daNpc_Post_c(
        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {
        OS_REPORT("|%06d:%x|daNpc_Post_c -> コンストラクト\n", g_Counter.mCounter0, this);
    }
    u16 getEyeballMaterialNo() { return MAT_EYEBALL; }
    s32 getHeadJointNo() { return JNT_HEAD; }
    s32 getNeckJointNo() { return JNT_NECK; }
    s32 getBackboneJointNo() { return JNT_BACKBONE1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == JNT_HEAD; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == JNT_MOUTH; }
    s32 getFootLJointNo() { return JNT_FOOTL; }
    s32 getFootRJointNo() { return JNT_FOOTR; }

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

    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }

    static char* mCutNameList[2];
    static cutFunc mCutList[2];

private:
    /* 0x0E40 */ mDoExt_McaMorfSO* mpFlagModelMorf;
    /* 0x0E44 */ NPC_POST_HIO_CLASS* mHIO;
    /* 0x0E48 */ J3DModel* mpLetterModels[2];
    /* 0x0E50 */ dCcD_Cyl mCyl;
    /* 0x0F8C */ u8 mType;
    /* 0x0F90 */ daNpcT_ActorMngr_c mActorMngrs[4];
    /* 0x0FB0 */ daNpcT_Path_c mPath;
    /* 0x0FD8 */ actionFunc mNextAction;
    /* 0x0FE4 */ actionFunc mAction;
    /* 0x0FF0 */ cXyz mActorPos;
    /* 0x0FFC */ cXyz field_0xffc;
    /* 0x1008 */ f32 mPrevPosY;
    /* 0x100C */ int mNodTimer;
    /* 0x1010 */ u8 mLetterADispFlag;
    /* 0x1011 */ u8 mLetterBDispFlag;
    /* 0x1012 */ u8 mFlagModelDispFlag;
    /* 0x1013 */ u8 mSitFlag;
    /* 0x1014 */ u8 field_0x1014;
};

STATIC_ASSERT(sizeof(daNpc_Post_c) == 0x1018);

#endif /* D_A_NPC_POST_H */
