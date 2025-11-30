#ifndef D_A_NPC_LUD_H
#define D_A_NPC_LUD_H

#include "d/actor/d_a_npc.h"

struct daNpc_Lud_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 play_speed;
};

class daNpc_Lud_Param_c {
public:
    virtual ~daNpc_Lud_Param_c() {}

    static const daNpc_Lud_HIOParam m;
};

#if DEBUG
class daNpc_Lud_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Lud_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_Lud_HIOParam m;
};

#define NPC_LUD_HIO_CLASS daNpc_Lud_HIO_c
#else
#define NPC_LUD_HIO_CLASS daNpc_Lud_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Lud_c
 * @brief Luda
 *
 * @details
 *
 */
class daNpc_Lud_c : public daNpcT_c {
public:
    typedef int (daNpc_Lud_c::*actionFunc)(void*);
    typedef int (daNpc_Lud_c::*cutFunc)(int);

    ~daNpc_Lud_c();
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
    bool setBowlAnm(int, int, f32);
    bool afterSetMotionAnm(int, int, f32, int);
    int selectAction();
    BOOL chkAction(actionFunc);
    int setAction(actionFunc);
    void mop(int, int);
    int cutHurry(int);
    int cutConversationInHotel(int);
    int cutConversationAboutDeathMt(int);
    int cutConversationAboutGoron(int);
    int cutNurse(int);
    int cutClothTry(int);
    int cutThankYou(int);
    int wait(void*);
    int nurse(void*);
    int giveSoup(void*);
    int talk(void*);
    daNpc_Lud_c(
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
    BOOL checkChangeJoint(int param_0) { return param_0 == 4; }
    BOOL checkRemoveJoint(int param_0) { return param_0 == 10; }

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xffff) {
            return -1;
        }
        return nodeNo;
    }
    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xff0000) >> 16; }

    static char* mCutNameList[8];
    static cutFunc mCutList[8];

private:
    /* 0xE40 */ mDoExt_McaMorfSO* mpBowlMorf;
    /* 0xE44 */ NPC_LUD_HIO_CLASS* mpHIO;
    /* 0xE48 */ J3DModel* mpModel[2];
    /* 0xE50 */ dCcD_Cyl mCyl;
    /* 0xF8C */ u8 mType;
    /* 0xF90 */ daNpcT_ActorMngr_c mActorMngr[3];
    /* 0xFA8 */ actionFunc field_0xfa8;
    /* 0xFB4 */ actionFunc mAction;
    /* 0xFC0 */ int field_0xfc0;
    /* 0xFC4 */ int field_0xfc4;
    /* 0xFC8 */ int field_0xfc8;
    /* 0xFCC */ int field_0xfcc;
    /* 0xFD0 */ s16 field_0xfd0;
    /* 0xFD2 */ u8 field_0xfd2;
    /* 0xFD3 */ u8 field_0xfd3;
    /* 0xFD4 */ u8 field_0xfd4;
    /* 0xFD5 */ u8 field_0xfd5;
    /* 0xFD8 */ int field_0xfd8;
};

STATIC_ASSERT(sizeof(daNpc_Lud_c) == 0xfdc);

#endif /* D_A_NPC_LUD_H */
