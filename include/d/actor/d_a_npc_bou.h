#ifndef D_A_NPC_BOU_H
#define D_A_NPC_BOU_H

#include "d/actor/d_a_npc.h"

struct daNpc_Bou_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 field_0x8c; // 16.0f
    /* 0x90 */ f32 field_0x90; // 1000.0f
    /* 0x94 */ f32 field_0x94; // 500.0f
    /* 0x98 */ f32 field_0x98; // -500.0f
};

 class daNpc_Bou_Param_c {
public:
    virtual ~daNpc_Bou_Param_c() {}

    static const daNpc_Bou_HIOParam m;
 };

#if DEBUG
class daNpc_Bou_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Bou_HIO_c();
    virtual ~daNpc_Bou_HIO_c() {}

    void listenPropertyEvent(const JORPropertyEvent*);

    void genMessage(JORMContext*);

    daNpc_Bou_HIOParam m;
};
#define NPC_BOU_HIO_CLASS daNpc_Bou_HIO_c
#else
#define NPC_BOU_HIO_CLASS daNpc_Bou_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Bou_c
 * @brief Mayor Bo
 *
 * @details
 *
 */
class daNpc_Bou_c : public daNpcT_c {
public:
    typedef int (daNpc_Bou_c::*cutFunc)(int);
    typedef int (daNpc_Bou_c::*actionFunc)(void*);

    enum Type {
    TYPE_0,
    TYPE_1,
    TYPE_2,
    TYPE_3,
    TYPE_4,
    TYPE_5,
    };

    ~daNpc_Bou_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    static void* srchCow(void*, void*);
    fopAc_ac_c* getCowP();
    u8 getType();
    int isDelete();
    void reset();
    void srchActors();
    int selectAction();
    int chkAction(int (daNpc_Bou_c::*)(void*));
    int setAction(int (daNpc_Bou_c::*)(void*));
    int cutWildGoat(int);
    int cutWildGoatSuccess(int);
    int cutWildGoatFailure(int);
    int cutSpeakTo(int);
    int cutConversationWithBou(int);
    int cutConfidentialConversation(int);
    int cutFindWolf(int);
    int cutMeetingAgain(int);
    int wait(void*);
    int talkwithJagar(void*);
    int talk(void*);
    daNpc_Bou_c(daNpcT_faceMotionAnmData_c const* param_1, daNpcT_motionAnmData_c const* param_2,
                               daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_3, int param_4,
                               daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_5, int param_6,
                               daNpcT_evtData_c const* param_7, char** param_8) :
                               daNpcT_c(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8)
                                {}

    virtual int checkChangeJoint(int param_0) { return param_0 == 4; }
    virtual int checkRemoveJoint(int param_0) { return param_0 == 8; }
    virtual s32 getBackboneJointNo() { return 1; }
    virtual s32 getNeckJointNo() { return 3; }
    virtual s32 getHeadJointNo() { return 4; }
    virtual u16 getEyeballMaterialNo() { return 1; }
    virtual void afterJntAnm(int);
    virtual void setParam();
    virtual BOOL checkChangeEvt();
    virtual BOOL evtTalk();
    virtual BOOL evtEndProc();
    virtual BOOL evtCutProc();
    virtual void setAfterTalkMotion();
    virtual void action();
    virtual void beforeMove();
    virtual void setAttnPos();
    virtual void setCollision();
    virtual int drawDbgInfo();
    virtual void changeAnm(int*, int*);

    static char* mCutNameList[9];
    static cutFunc mCutList[9];

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xffff) {
            return -1;
        }
        return nodeNo;
    }

    BOOL chkFindWolf() {
        int iVar1 = daNpcT_getDistTableIdx(field_0xfe0, field_0xfe4);
        return daNpcT_c::chkFindWolf(mCurAngle.y, iVar1, field_0xfdc,
            mpHIO->m.common.search_distance, mpHIO->m.common.fov,
            mpHIO->m.common.search_height, mpHIO->m.common.search_depth, 1);
    }

    int chkCondition(int i_val) {
        if (mStagger.checkStagger()) {
            return false;
        } else if (i_val != 0) {
            return true;
        } else {
            if (mMotionSeqMngr.getNo() == 3 ||
                mMotionSeqMngr.getNo() == 2 ||
                mMotionSeqMngr.getNo() == 4) {
                if (mMotionSeqMngr.getStepNo() > 0) {
                    return true;
                }
            } else if (mMotionSeqMngr.getNo() == 1) {
                return true;
            }
        }
        return false;
    }

    BOOL speakTo() {
        if (mType == 4) {
            if (current.pos.absXZ(daPy_getPlayerActorClass()->current.pos) < 1100.0f && strlen(mpEvtData[5].eventName) != 0) {
                u32 len = strlen(mpArcNames[mpEvtData[5].num]);
                if (len != 0) {
                    eventInfo.setArchiveName(mpArcNames[mpEvtData[5].num]);
                    dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
                }

                mEvtId = dComIfGp_getEventManager().getEventIdx(this, mpEvtData[5].eventName, 0xFF);
                fopAcM_orderOtherEventId(this, mEvtId, 0xFF, 0xFFFF, 4, 1);
                return 1;
            }
        }

        return 0;
    }

private:
    /* 0xE40 */ NPC_BOU_HIO_CLASS* mpHIO;
    /* 0xE44 */ dCcD_Cyl mCyl1;
    /* 0xF80 */ u8 mType;
    /* 0xF84 */ daNpcT_ActorMngr_c mActorMngr[3];
    /* 0xF9C */ daNpcT_Path_c field_0xf9c;
    /* 0xFC4 */ actionFunc field_0xfc4;
    /* 0xFD0 */ actionFunc field_0xfd0;
    /* 0xFDC */ int field_0xfdc;
    /* 0xFE0 */ int field_0xfe0;
    /* 0xFE4 */ int field_0xfe4;
    /* 0xFE8 */ u8 field_0xfe8[0xFF4 - 0xFE8];
    /* 0xFF4 */ u8 field_0xff4;
    /* 0xFF5 */ u8 field_0xff5[0xFF8 - 0xFF5];
    /* 0xFF8 */ u8 field_0xff8;
};

STATIC_ASSERT(sizeof(daNpc_Bou_c) == 0xffc);


#endif /* D_A_NPC_BOU_H */
