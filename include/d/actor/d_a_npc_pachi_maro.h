#ifndef D_A_NPC_PACHI_MARO_H
#define D_A_NPC_PACHI_MARO_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Pachi_Maro_c
 * @brief Malo (Slingshot Tutorial)
 *
 * @details
 *
*/

struct daNpc_Pachi_Maro_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ s16 field_0x8c;
};

class daNpc_Pachi_Maro_Param_c {
public:
    virtual ~daNpc_Pachi_Maro_Param_c() {}

    static daNpc_Pachi_Maro_HIOParam const m;
};

#if DEBUG
class daNpc_Pachi_Maro_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Pachi_Maro_HIO_c();
    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    /* 0x8 */ daNpc_Pachi_Maro_HIOParam m;
};
#define NPC_PACHI_MARO_HIO_CLASS daNpc_Pachi_Maro_HIO_c
#else
#define NPC_PACHI_MARO_HIO_CLASS daNpc_Pachi_Maro_Param_c
#endif

class daNpc_Pachi_Maro_c : public daNpcT_c {
public:
    enum Material {
        /* 0x0 */ MAT_SC_EYE,
        /* 0x1 */ MAT_MARO_EYBALL_M,
        /* 0x2 */ MAT_MARO_FACE_M,
        /* 0x3 */ MAT_MARO_M,
    };

    enum Joint {
        /* 0x00 */ JNT_CENTER,
        /* 0x01 */ JNT_BACKBONE1,
        /* 0x02 */ JNT_BACKBONE2,
        /* 0x03 */ JNT_NECK,
        /* 0x04 */ JNT_HEAD,
        /* 0x05 */ JNT_CHIN,
        /* 0x06 */ JNT_MOUTH,
        /* 0x07 */ JNT_SHOULDERL,
        /* 0x08 */ JNT_ARML1,
        /* 0x09 */ JNT_ARML2,
        /* 0x0A */ JNT_HANDL,
        /* 0x0B */ JNT_FINGERL,
        /* 0x0C */ JNT_SHOULDERR,
        /* 0x0D */ JNT_ARMR1,
        /* 0x0E */ JNT_ARMR2,
        /* 0x0F */ JNT_HANDR,
        /* 0x10 */ JNT_FINGERR,
        /* 0x11 */ JNT_WAIST,
        /* 0x12 */ JNT_LEGL1,
        /* 0x13 */ JNT_LEGL2,
        /* 0x14 */ JNT_FOOTL,
        /* 0x15 */ JNT_LEGR1,
        /* 0x16 */ JNT_FOOTR,
    };

    typedef int (daNpc_Pachi_Maro_c::*actionFunc)(void*);
    typedef int (daNpc_Pachi_Maro_c::*cutFunc)(int);

    ~daNpc_Pachi_Maro_c();
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
    daNpcT_faceMotionAnmData_c getFaceMotionAnm(daNpcT_faceMotionAnmData_c);
    daNpcT_motionAnmData_c getMotionAnm(daNpcT_motionAnmData_c);
    BOOL selectAction();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    int wait(void*);
    int talk(void*);
    int test(void*);
    BOOL _turn_to_link(s16);
    BOOL _turn_pos(cXyz const&);
    BOOL _turn_pos(cXyz const&, s16);
    BOOL cutTutrialBegin(int);
    BOOL _cutTutrialBegin_Init(int const&);
    BOOL _cutTutrialBegin_Main(int const&);
    BOOL cutTutrialBegin_Skip(int);
    BOOL _cutTutrialBegin_Skip_Init(int const&);
    BOOL _cutTutrialBegin_Skip_Main(int const&);
    BOOL cutTutrialClear(int);
    BOOL _cutTutrialClear_Init(int const&);
    BOOL _cutTutrialClear_Main(int const&);
    BOOL cutTalk(int);
    BOOL _cutTalk_Init(int const&);
    BOOL _cutTalk_Main(int const&);
    BOOL cutTalk2(int);
    BOOL _cutTalk2_Init(int const&);
    BOOL _cutTalk2_Main(int const&);
    BOOL cut1stHit_KakasiBody(int);
    BOOL _cut1stHit_KakasiBody_Init(int const&);
    BOOL _cut1stHit_KakasiBody_Main(int const&);
    BOOL cutTutrialSelectGiveUp(int);
    BOOL _cutTutrialSelectGiveUp_Init(int const&);
    BOOL _cutTutrialSelectGiveUp_Main(int const&);
    BOOL cutTutrialGiveUp(int);
    BOOL _cutTutrialGiveUp_Init(int const&);
    BOOL _cutTutrialGiveUp_Main(int const&);
    BOOL cutTutrialContinue(int);
    BOOL _cutTutrialContinue_Init(int const&);
    BOOL _cutTutrialContinue_Main(int const&);
    BOOL cutTutrialCaution(int);
    BOOL _cutTutrialCaution_Init(int const&);
    BOOL _cutTutrialCaution_Main(int const&);
    daNpc_Pachi_Maro_c(
                        daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
                        daNpcT_motionAnmData_c const* i_motionAnmData,
                        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
                        int i_faceMotionStepNum,
                        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData,
                        int i_motionStepNum,
                        daNpcT_evtData_c const* i_evtData,
                        char** i_arcNames)
                    : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                    i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                    i_arcNames) {
                        OS_REPORT("|%06d:%x|daNpc_Pachi_Maro_c -> コンストラクト\n", g_Counter.mCounter0, this);
                    }
    u16 getEyeballMaterialNo() { return MAT_MARO_EYBALL_M; }
    s32 getHeadJointNo() { return JNT_HEAD; }
    s32 getNeckJointNo() { return JNT_NECK; }
    s32 getBackboneJointNo() { return JNT_BACKBONE1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == JNT_HEAD; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == JNT_MOUTH; }

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
    u8 getPathID() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }
    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xFF0000) >> 16; }
    u8 getMaxNumItem() { return (fopAcM_GetParam(this) & 0xF000000) >> 24; }
    u8 getGroupId() { return (fopAcM_GetParam(this) & 0xF0000000) >> 28; }

    void talkOff() { mTalk = 0; }
    void setTagPos(cXyz const& i_pos) { mTagPos = i_pos; }
    void setLookPos(cXyz const& i_pos) { mLookPos = i_pos; }
    void setFMotion_None() { mFMotion = 0; }
    void setFMotion_Niramu_to_Besu() { mFMotion = 1; }
    void setFMotion_LookNone() { mFMotion = 2; }

    static char* mCutNameList[11];
    static cutFunc mCutList[11];

private:
    /* 0xE40 */ NPC_PACHI_MARO_HIO_CLASS* mpHIO;
    /* 0xE44 */ dCcD_Cyl mCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF82 */ u16 mPrevEvtNo;
    /* 0xF84 */ u8 mTalk;
    /* 0xF85 */ u8 field_0xf85;
    /* 0xF86 */ u8 mFMotion;
    /* 0xF88 */ cXyz mTagPos;
    /* 0xF94 */ cXyz mLookPos;
    /* 0xFA0 */ daNpcT_ActorMngr_c mActorMngrs[2];
    /* 0xFB0 */ actionFunc mNextAction;
    /* 0xFBC */ actionFunc mAction;
    /* 0xFC8 */ u8 field_0xfc8;
};

STATIC_ASSERT(sizeof(daNpc_Pachi_Maro_c) == 0xfcc);

#endif /* D_A_NPC_PACHI_MARO_H */
