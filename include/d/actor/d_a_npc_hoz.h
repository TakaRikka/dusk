#ifndef D_A_NPC_HOZ_H
#define D_A_NPC_HOZ_H

#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_startAndGoal.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Hoz_c
 * @brief Iza
 *
 * @details
 *
 */
class daNpc_Hoz_c : public daNpcT_c {
public:
    typedef int (daNpc_Hoz_c::*actionFunc)(void*);
    typedef int (daNpc_Hoz_c::*cutFunc)(int);

    enum Type {
        /* 0 */ TYPE_0,
        /* 1 */ TYPE_1,
        /* 2 */ TYPE_2,
        /* 3 */ TYPE_3,
        /* 4 */ TYPE_4,
        /* 5 */ TYPE_5,
        /* 6 */ TYPE_6,
    };

    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    int getFlowNodeNo();
    int isDelete();
    void reset();
    void srchActors();
    int selectAction();
    int chkAction(int (daNpc_Hoz_c::*)(void*));
    int setAction(int (daNpc_Hoz_c::*)(void*));
    int wait(void*);
    int waitBattle(void*);
    int waitTwilightBattle(void*);
    int waitBoat(void*);
    int waitBoat2(void*);
    int waitBoat1_5(void*);
    int waitTw(void*);
    int talk(void*);
    int ECut_boatRace(int);
    int ECut_beforeBattle(int);
    int ECut_afterBattle(int);
    int ECut_beforeBlast(int);
    int ECut_afterBlast(int);
    int ECut_yMushi(int);
    int ECut_talkBreak(int);
    daNpc_Hoz_c(daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
                                   daNpcT_motionAnmData_c const* i_motionAnmData,
                                   daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
                                   int i_faceMotionStepNum,
                                   daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
                                   daNpcT_evtData_c const* i_evtData, char** i_arcNames)
                    : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                               i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                               i_arcNames)
                    {
                        OS_REPORT("|%06d:%x|daNpc_Hoz_c -> コンストラクト\n", g_Counter.mCounter0, this);
                    }

    virtual ~daNpc_Hoz_c();
    virtual int checkChangeJoint(int i_jointNo) { return i_jointNo == 4; }
    virtual int checkRemoveJoint(int i_jointNo) { return i_jointNo == 9; }
    virtual s32 getBackboneJointNo() { return 1; }
    virtual s32 getNeckJointNo() { return 3; }
    virtual s32 getHeadJointNo() { return 4; }
    virtual u16 getEyeballMaterialNo() { return 1; }
    virtual void setParam();
    virtual BOOL checkChangeEvt();
    virtual BOOL evtTalk();
    virtual BOOL evtEndProc();
    virtual BOOL evtCutProc();
    virtual void setAfterTalkMotion();
    virtual int evtProc();
    virtual void action();
    virtual void beforeMove();
    virtual void setAttnPos();
    virtual void setCollision();
    virtual void evtOrder();
    virtual int drawDbgInfo();

    int test(void*);

    bool getGameStartFlag() { return mGameStartFlag; }
    void setPotBreakFlag() { mPotBreakFlag = true; }

    static char* mCutNameList[8];
    static cutFunc mCutList[];

private:
    /* 0xE40 */ u8 field_0xE40[0xE44 - 0xE40];
    /* 0xE44 */ dCcD_Cyl mCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF84 */ daStartAndGoal_c* field_0xf84;
    /* 0xF88 */ u8 field_0xf88;
    /* 0xF89 */ u8 field_0xf89;
    /* 0xF8A */ u8 field_0xf8a;
    /* 0xF8B */ u8 field_0xf8b;
    /* 0xF8C */ u8 field_0xf8c;
    /* 0xF8D */ bool mGameStartFlag;
    /* 0xF8E */ bool mPotBreakFlag;
    /* 0xF90 */ cXyz field_0xf90;
    /* 0xF9C */ s16 field_0xf9c;
    /* 0xFA0 */ daNpcT_ActorMngr_c field_0xfa0;
    /* 0xFA8 */ int field_0xfa8;
    /* 0xFAC */ int (daNpc_Hoz_c::*field_0xfac)(void*);
    /* 0xFB8 */ int (daNpc_Hoz_c::*mAction)(void*);
    /* 0xFC4 */ u8 field_0xfc4;
};

STATIC_ASSERT(sizeof(daNpc_Hoz_c) == 0xFC8);

struct daNpc_Hoz_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 field_0x8c;
};

class daNpc_Hoz_Param_c {
public:
    virtual ~daNpc_Hoz_Param_c() {}

    static const daNpc_Hoz_HIOParam m;
};

#endif /* D_A_NPC_HOZ_H */
