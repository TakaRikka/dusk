#ifndef D_A_NPC_PACHI_TARO_H
#define D_A_NPC_PACHI_TARO_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Pachi_Taro_c
 * @brief Talo (Slingshot Tutorial)
 *
 * @details
 *
*/

struct daNpc_Pachi_Taro_HIOParam {
    /* 0x0 */ daNpcT_HIOParam common;
};

class daNpc_Pachi_Taro_Param_c {
public:
    virtual ~daNpc_Pachi_Taro_Param_c() {}

    static daNpc_Pachi_Taro_HIOParam const m;
};

#if DEBUG
class daNpc_Pachi_Taro_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Pachi_Taro_HIO_c();
    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    /* 0x8 */ daNpc_Pachi_Taro_HIOParam m;
};
#define NPC_PACHI_TARO_HIO_CLASS daNpc_Pachi_Taro_HIO_c
#else
#define NPC_PACHI_TARO_HIO_CLASS daNpc_Pachi_Taro_Param_c
#endif

class daNpc_Pachi_Taro_c : public daNpcT_c {
public:
    enum Material {
        /* 0x0 */ MAT_SC_EYE,
        /* 0x1 */ MAT_TARO_BODY_M,
        /* 0x2 */ MAT_TARO_EYBALL_M,
        /* 0x3 */ MAT_TARO_FACE_M,
    };

    enum Joint {
        /* 0x00 */ JNT_CENTER,
        /* 0x01 */ JNT_BACKBONE1,
        /* 0x02 */ JNT_BACKBONE2,
        /* 0x03 */ JNT_NECK,
        /* 0x04 */ JNT_HEAD,
        /* 0x05 */ JNT_CHINE,
        /* 0x06 */ JNT_HEADBAND,
        /* 0x07 */ JNT_MOUTH,
        /* 0x08 */ JNT_SHOULDERL,
        /* 0x09 */ JNT_ARML1,
        /* 0x0A */ JNT_ARML2,
        /* 0x0B */ JNT_HANDL,
        /* 0x0C */ JNT_SHOULDERR,
        /* 0x0D */ JNT_ARMR1,
        /* 0x0E */ JNT_ARMR2,
        /* 0x0F */ JNT_HANDR,
        /* 0x10 */ JNT_WAIST,
        /* 0x11 */ JNT_LEGL1,
        /* 0x12 */ JNT_LEGL2,
        /* 0x13 */ JNT_FOOTL,
        /* 0x14 */ JNT_LEGR1,
        /* 0x15 */ JNT_LEGR2,
        /* 0x16 */ JNT_FOOTR,
        /* 0x17 */ JNT_SKART1,
        /* 0x18 */ JNT_SKART2,
    };

    typedef int (daNpc_Pachi_Taro_c::*actionFunc)(void*);
    typedef int (daNpc_Pachi_Taro_c::*cutFunc)(int);

    ~daNpc_Pachi_Taro_c();
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
    fopAc_ac_c* srchDistTag1();
    static void* _srch_DistTag1_main(void*, void*);
    fopAc_ac_c* srchDistTag2();
    static void* _srch_DistTag2_main(void*, void*);
    fopAc_ac_c* srchEscapeTag1();
    static void* _srch_EscapeTag1_main(void*, void*);
    fopAc_ac_c* srchEscapeTag2();
    static void* _srch_EscapeTag2_main(void*, void*);
    BOOL evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void setAttnPos();
    void setCollision();
    int drawDbgInfo();
    void drawOtherMdl();
    BOOL selectAction();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    int wait(void*);
    void setMesPat();
    BOOL _turn_to_link(s16);
    BOOL _turn_pos(cXyz const&, s16);
    BOOL _turn_pos(cXyz const&);
    void talk_cancel();
    void watch_tutrial();
    u16 chkDistTag();
    u16 chkEscapeTag();
    u16 chkTarget();
    void clr_tagHit();
    void allTagPosSet(cXyz const&);
    void allLookPosSet(cXyz const&);
    int talk(void*);
    int test(void*);
    BOOL evtEndProc();
    int cutTutrialBegin(int);
    int _cutTutrialBegin_Init(int const&);
    int _cutTutrialBegin_Main(int const&);
    int cutTutrialBegin_Skip(int);
    int _cutTutrialBegin_Skip_Init(int const&);
    int _cutTutrialBegin_Skip_Main(int const&);
    int cutTutrialClear(int);
    int _cutTutrialClear_Init(int const&);
    int _cutTutrialClear_Main(int const&);
    int cutTalk(int);
    int _cutTalk_Init(int const&);
    int _cutTalk_Main(int const&, int const&);
    int cutTalk2(int);
    int _cutTalk2_Init(int const&);
    int _cutTalk2_Main(int const&, int const&);
    int cut1stHit_KakasiBody(int);
    int _cut1stHit_KakasiBody_Init(int const&);
    int _cut1stHit_KakasiBody_Main(int const&);
    int cutTutrialSelectGiveUp(int);
    int _cutTutrialSelectGiveUp_Init(int const&);
    int _cutTutrialSelectGiveUp_Main(int const&);
    int cutTutrialGiveUp(int);
    int _cutTutrialGiveUp_Init(int const&);
    int _cutTutrialGiveUp_Main(int const&);
    int cutTutrialCaution(int);
    int _cutTutrialCaution_Init(int const&);
    int _cutTutrialCaution_Main(int const&);
    int cutTutrialContinue(int);
    int _cutTutrialContinue_Init(int const&);
    int _cutTutrialContinue_Main(int const&);
    daNpc_Pachi_Taro_c(
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
                        OS_REPORT("|%06d:%x|daNpc_Pachi_Taro_c -> コンストラクト\n", g_Counter.mCounter0, this);
                    }
    u16 getEyeballMaterialNo() { return MAT_TARO_EYBALL_M; }
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
    u8 getBitSW2() { return (fopAcM_GetParam(this) & 0xFF000000) >> 24; }
    void clrMesPat() { mMesPat = -1; }
    void setTagPos(cXyz const& i_pos) { mTagPos = i_pos; }
    void setLookPos(cXyz const& i_pos) { mLookPos = i_pos; }

    static char* mCutNameList[11];
    static cutFunc mCutList[11];

private:
    /* 0x0E40 */ NPC_PACHI_TARO_HIO_CLASS* mpHIO;
    /* 0x0E44 */ J3DModel* mpModels[2];
    /* 0x0E4C */ dCcD_Cyl mCyl;
    /* 0x0F88 */ u8 mType;
    /* 0x0F8C */ daNpcT_ActorMngr_c mActorMngrs[6];
    /* 0x0FBC */ actionFunc mNextAction;
    /* 0x0FC8 */ actionFunc mAction;
    /* 0x0FD4 */ int field_0xfd4;
    /* 0x0FD8 */ u8 field_0xfd8[0xfda - 0xfd8];
    /* 0x0FDA */ s16 field_0xfda;
    /* 0x0FDC */ u16 mPrevEvtNo;
    /* 0x0FDE */ u8 field_0xfde;
    /* 0x0FDF */ u8 field_0xfdf;
    /* 0x0FE0 */ u8 field_0xfe0;
    /* 0x0FE1 */ u8 mHitMsgIdx;
    /* 0x0FE2 */ u8 field_0xfe2;
    /* 0x0FE3 */ u8 field_0xfe3;
    /* 0x0FE4 */ u8 field_0xfe4[0xfe8 - 0xfe4];
    /* 0x0FE8 */ s8 mMesPat;
    /* 0x0FEC */ cXyz mTagPos;
    /* 0x0FF8 */ cXyz mLookPos;
    /* 0x1004 */ u8 field_0x1004;
    /* 0x1005 */ u8 field_0x1005;
    /* 0x1006 */ u8 field_0x1006;
    /* 0x1007 */ u8 field_0x1007;
    /* 0x1008 */ u8 field_0x1008;

};

STATIC_ASSERT(sizeof(daNpc_Pachi_Taro_c) == 0x100c);

#endif /* D_A_NPC_PACHI_TARO_H */
