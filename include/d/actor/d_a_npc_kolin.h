#ifndef D_A_NPC_KOLIN_H
#define D_A_NPC_KOLIN_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Kolin_c
 * @brief Colin
 *
 * @details
 *
*/

struct daNpc_Kolin_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 start_distance;      // 走りはじめ距離   - Start Distance
    /* 0x90 */ f32 follow_distance;     // 追従距離         - Follow Distance
    /* 0x94 */ f32 run_speed;           // 走る速度         - Run Speed
    /* 0x98 */ s16 shy_walk_time;       // いじけ歩き時間   - Shy Walk Time
    /* 0x9A */ s16 sulk_time;           // いじけ時間       - Sulk Time
    /* 0x9C */ f32 yeah_yeah;           // ハイハイ         - Yeah Yeah (Likely a sarcastic connotation)
};

class daNpc_Kolin_Param_c {
public:
    virtual ~daNpc_Kolin_Param_c() {}

    static daNpc_Kolin_HIOParam const m;
};

class daNpc_Kolin_c : public daNpcT_c {
public:
    typedef int (daNpc_Kolin_c::*cutFunc)(int);
    typedef int (daNpc_Kolin_c::*actionFunc)(void*);

    ~daNpc_Kolin_c();
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
    void srchActors();
    int selectAction();
    BOOL chkAction(actionFunc);
    int setAction(actionFunc);
    void calcFollowSpeedAndAngle(fopAc_ac_c*, int, int);
    void followPlayer(int);
    void lookup(u8);
    int cutNoRide(int);
    int cutHail(int);
    int cutGiveMeWoodSwd(int);
    int cutGetWoodSwd(int);
    int cutConversationAboutLoopHole(int);
    int cutCacaricoConversation(int);
    int cutConversationAboutDeathMt(int);
    int cutConversationAboutGoron(int);
    int cutClothTry(int);
    int cutThankYou(int);
    int wait(void*);
    int timidWalk(void*);
    int follow(void*);
    int clothWait(void*);
    int talk(void*);
    int test(void*);
    daNpc_Kolin_c(
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
    virtual int checkChangeJoint(int i_joint) { return i_joint == 4; }
    virtual int checkRemoveJoint(int i_joint) { return i_joint == 8; }
    virtual s32 getBackboneJointNo() { return 1; }
    virtual s32 getNeckJointNo() { return 3; }
    virtual s32 getHeadJointNo() { return 4; }
    virtual s32 getFootLJointNo() { return 22; }
    virtual s32 getFootRJointNo() { return 25; }
    virtual u16 getEyeballMaterialNo() { return 2; }
    virtual void afterJntAnm(int);
    virtual void setParam();
    virtual BOOL checkChangeEvt();
    virtual BOOL evtTalk();
    virtual BOOL evtCutProc();
    virtual void setAfterTalkMotion();
    virtual void action();
    virtual void beforeMove();
    virtual void setAttnPos();
    virtual void setCollision();
    virtual int drawDbgInfo();
    virtual void drawOtherMdl();
    virtual void changeAnm(int*, int*);
    virtual void changeBck(int*, int*);

    static char* mCutNameList[11];
    static cutFunc mCutList[11];

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

    u32 getPathID() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }
    u32 getBitSW() { return (fopAcM_GetParam(this) & 0xFF0000) >> 16; }

    BOOL orderNoRideEvt() {
        if (mType == 11) {
            s16 var_r28 = (s16)(fopAcM_searchPlayerAngleY(this) - home.angle.y);
            int var_r29 = 3;

            if (var_r28 < 0) {
                var_r29 = 2;
            }

            if (strlen(mpEvtData[var_r29].eventName) != 0) {
                u32 len = strlen(mpArcNames[mpEvtData[var_r29].num]);
                if (len != 0) {
                    eventInfo.setArchiveName(mpArcNames[mpEvtData[var_r29].num]);
                    dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
                }

                mEvtId = dComIfGp_getEventManager().getEventIdx(this, mpEvtData[var_r29].eventName, 0xFF);
                fopAcM_orderOtherEventId(this, mEvtId, 0xFF, 0xFFFF, 4, 1);
                return 1;
            }
        }

        return 0;
    }

private:
    /* 0x0E40 */ u8 field_0xe40[0xe44 - 0xe40];
    /* 0x0E44 */ J3DModel* mpClothModel;
    /* 0x0E48 */ dCcD_Cyl field_0xe48;
    /* 0x0F84 */ u8 mType;
    /* 0x0F88 */ daNpcT_ActorMngr_c mActorMngr[5];
    /* 0x0FB0 */ daNpcT_Path_c mPath;
    /* 0x0FD8 */ actionFunc mNextAction;
    /* 0x0FE4 */ actionFunc mAction;
    /* 0x0FF0 */ u8 field_0xff0[0xffc - 0xff0];
    /* 0x0FFC */ int field_0xffc;
    /* 0x1000 */ int field_0x1000;
    /* 0x1004 */ int field_0x1004;
    /* 0x1008 */ int field_0x1008;
    /* 0x100C */ u8 field_0x100c[0x1010 - 0x100c];
    /* 0x1010 */ f32 field_0x1010;
    /* 0x1014 */ u8 field_0x1014;
    /* 0x1015 */ u8 field_0x1015;
    /* 0x1016 */ u8 field_0x1016;
    /* 0x1017 */ u8 field_0x1017;
    /* 0x1018 */ u8 field_0x1018;
    /* 0x1019 */ u8 field_0x1019[0x101c - 0x1019];
    /* 0x101C */ u8 field_0x101c;
};

STATIC_ASSERT(sizeof(daNpc_Kolin_c) == 0x1020);

#endif /* D_A_NPC_KOLIN_H */
