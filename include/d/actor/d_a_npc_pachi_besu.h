#ifndef D_A_NPC_PACHI_BESU_H
#define D_A_NPC_PACHI_BESU_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Pachi_Besu_c
 * @brief Beth (Slingshot Tutorial)
 *
 * @details
 *
*/

struct daNpc_Pachi_Besu_HIOParam {
    /* 0x0 */ daNpcT_HIOParam common;
};

class daNpc_Pachi_Besu_HIO_c : public mDoHIO_entry_c {
    /* 0x8 */ daNpc_Pachi_Besu_HIOParam param;
};

class daNpc_Pachi_Besu_Param_c {
public:
    virtual ~daNpc_Pachi_Besu_Param_c() {}

    static daNpc_Pachi_Besu_HIOParam const m;
};

class daNpc_Pachi_Besu_c : public daNpcT_c {
public:
    typedef BOOL (daNpc_Pachi_Besu_c::*actionFunc)(void*);
    typedef BOOL (daNpc_Pachi_Besu_c::*cutFunc)(int);

    ~daNpc_Pachi_Besu_c();
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
    void afterMoved();
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
    BOOL selectAction();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    BOOL wait(void*);
    BOOL talk(void*);
    BOOL test(void*);
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
    daNpc_Pachi_Besu_c(
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
        i_arcNames) {}
    u16 getEyeballMaterialNo() { return 2; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == 4; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == 6; }

    u32 getFlowNodeNo() { 
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xffff) {
            return -1;
        }
        return nodeNo;
    }
    int getPathID() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }
    int getBitSW() { return (fopAcM_GetParam(this) & 0xFF0000) >> 16; }
    void setFMotion_None() { mFMotion = 0; }
    void setFMotion_Hit() { mFMotion = 1; }
    void setFMotion_Mad() { mFMotion = 2; }
    void setFMotion_LookBros() { mFMotion = 3; }
    void setFMotion_LookNone() { mFMotion = 4; }
    void talkOff() { mTalk = 0; }
    void setTagPos(cXyz const& i_pos) { mTagPos = i_pos; }
    void setLookPos(cXyz const& i_pos) { mLookPos = i_pos; }

    static char* mCutNameList[11];
    static cutFunc mCutList[11];

private:
    /* 0xE40 */ daNpc_Pachi_Besu_HIO_c* mHIO;
    /* 0xE44 */ dCcD_Cyl mCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF81 */ u8 mTalk;
    /* 0xF84 */ cXyz mTagPos;
    /* 0xF90 */ cXyz mLookPos;
    /* 0xF9C */ u16 field_0xf9c;
    /* 0xF9E */ u8 mFMotion;
    /* 0xFA0 */ daNpcT_ActorMngr_c mActorMngrs[2];
    /* 0xFB0 */ actionFunc mNextAction;
    /* 0xFBC */ actionFunc mAction;
    /* 0xFC8 */ u8 field_0xfc8;
};

STATIC_ASSERT(sizeof(daNpc_Pachi_Besu_c) == 0xfcc);

#endif /* D_A_NPC_PACHI_BESU_H */
