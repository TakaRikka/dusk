#ifndef D_A_NPC_URI_H
#define D_A_NPC_URI_H

#include "d/actor/d_a_npc.h"

struct daNpc_Uri_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ s16 field_0x8c;
    /* 0x8E */ s16 field_0x8e;
    /* 0x90 */ s16 field_0x90;
    /* 0x92 */ s16 field_0x92;
    /* 0x94 */ s16 field_0x94;
    /* 0x96 */ s16 field_0x96;
    /* 0x98 */ f32 field_0x98;
    /* 0x9C */ f32 field_0x9c;
    /* 0xA0 */ f32 field_0xa0;
    /* 0xA4 */ f32 field_0xa4;
    /* 0xA8 */ f32 field_0xa8;
};

class daNpc_Uri_Param_c {
public:
    virtual ~daNpc_Uri_Param_c() {}

    static const daNpc_Uri_HIOParam m;
};

#if DEBUG
class daNpc_Uri_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Uri_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_Uri_HIOParam m;
};

#define NPC_URI_HIO_CLASS daNpc_Uri_HIO_c
#else
#define NPC_URI_HIO_CLASS daNpc_Uri_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Uri_c
 * @brief Uli
 *
 * @details
 *
 */
class daNpc_Uri_c : public daNpcT_c {
public:
    typedef int (daNpc_Uri_c::*cutFunc)(int);
    typedef int (daNpc_Uri_c::*actionFunc)(void*);

    enum Type {
        TYPE_0,
        TYPE_1,
        TYPE_2,
        TYPE_3,
        TYPE_4,
        TYPE_5,
        TYPE_6,
    };

    ~daNpc_Uri_c();
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
    bool afterSetMotionAnm(int, int, f32, int);
    void changeBck(int*, int*);
    void changeBtp(int*, int*);
    int selectAction();
    int chkAction(int (daNpc_Uri_c::*)(void*));
    int setAction(int (daNpc_Uri_c::*)(void*));
    BOOL chkPlayerCarryBasket();
    BOOL chkPlayerGetWoodShield();
    int getTutorialCond(cXyz);
    int cutConversation(int);
    int cutStartCarryTutorial(int);
    int cutEndCarryTutorial(int);
    int cutConversationWithMoi(int);
    int cutFindWolf(int);
    int cutMeetingAgain(int);
    int krun(void*);
    int wait(void*);
    int walk(void*);
    int sitWait(void*);
    int talk(void*);
    daNpc_Uri_c(daNpcT_faceMotionAnmData_c const* param_1,
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
    BOOL checkChangeJoint(int param_0) { return param_0 == 4; }
    BOOL checkRemoveJoint(int param_0) { return param_0 == 8; }
    s32 getFootLJointNo() { return 25; }
    s32 getFootRJointNo() { return 28; }

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xffff) {
            return -1;
        }
        return nodeNo;
    }

    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xff0000) >> 16; }

    u8 getPathID() { return (fopAcM_GetParam(this) & 0xff00) >> 8; }

    static const char* mCutNameList[7];
    static cutFunc mCutList[7];

private:
    /* 0x0E40 */ NPC_URI_HIO_CLASS* mpHIO;
    /* 0x0E44 */ J3DModel* mpModel[1];
    /* 0x0E48 */ dCcD_Cyl mCyl;
    /* 0x0F84 */ u8 mType;
    /* 0x0F88 */ daNpcT_ActorMngr_c mActorMngr[2];
    /* 0x0F98 */ daNpcT_Path_c mPath;
    /* 0x0FC0 */ actionFunc field_0xfc0[2];
    /* 0x0FD8 */ cXyz field_0xfd8;
    /* 0x0FE4 */ csXyz field_0xfe4;
    /* 0x0FEC */ int field_0xfec;
    /* 0x0FF0 */ int field_0xff0;
    /* 0x0FF4 */ int field_0xff4;
    /* 0x0FF8 */ int field_0xff8;
    /* 0x0FFC */ int field_0xffc;
    /* 0x1000 */ f32 field_0x1000;
    /* 0x1004 */ f32 field_0x1004;
    /* 0x1008 */ u8 field_0x1008;
    /* 0x1009 */ u8 field_0x1009;
    /* 0x100A */ u8 field_0x100a;
    /* 0x100B */ u8 field_0x100b;
    /* 0x100C */ u8 field_0x100c;
    /* 0x100D */ u8 field_0x100d;
    /* 0x100E */ u8 field_0x100e;
    /* 0x100F */ u8 field_0x100f;
    /* 0x1010 */ u8 field_0x1010;
    /* 0x1011 */ u8 field_0x1011;
    /* 0x1012 */ u8 field_0x1012;
    /* 0x1014 */ int field_0x1014;
};

STATIC_ASSERT(sizeof(daNpc_Uri_c) == 0x1018);

#endif /* D_A_NPC_URI_H */
