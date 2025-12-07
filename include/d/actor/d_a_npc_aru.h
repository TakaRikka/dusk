#ifndef D_A_NPC_ARU_H
#define D_A_NPC_ARU_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Aru_c
 * @brief Fado
 *
 * @details
 *
*/

struct daNpc_Aru_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 warning_range;       // 警戒範囲 - Warning Range
    /* 0x90 */ f32 run_speed;           // 走る速度 - Run Speed        
    /* 0x94 */ f32 walk_speed;          // 歩く速度 - Walk Speed        
    /* 0x98 */ s16 no_turn_time;        // 旋回禁止時間 - No Turn Time    
    /* 0x9A */ s16 avoid_time;          // 避け時間 - Avoid Time        
    /* 0x9C */ f32 forward_visibility;  // 前方視界 - Forward Visibility        
};

class daNpc_Aru_HIO_c : public mDoHIO_entry_c {
public:
    /* 0x8 */ daNpc_Aru_HIOParam param;
};

class daNpc_Aru_Param_c {
public:
    virtual ~daNpc_Aru_Param_c() {}

    static daNpc_Aru_HIOParam const m;
};

class daNpc_Aru_c : public daNpcT_c {
public:
    enum Joint {
        /* 0x00  */ JNT_CENTER,
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
        /* 0x0D */ JNT_FINGERL1,
        /* 0x0E */ JNT_FINGERL2,
        /* 0x0F */ JNT_THAMBL,
        /* 0x10 */ JNT_SHOULDERR,
        /* 0x11 */ JNT_ARMR1,
        /* 0x12 */ JNT_ARMR2,
        /* 0x13 */ JNT_HANDR,
        /* 0x14 */ JNT_FINGERR1,
        /* 0x15 */ JNT_FINGERR2,
        /* 0x16 */ JNT_THAMBR,
        /* 0x17 */ JNT_WAIST,
        /* 0x18 */ JNT_LEGL1,
        /* 0x19 */ JNT_LEGL2,
        /* 0x1A */ JNT_FOOTL,
        /* 0x1B */ JNT_SKIRTL,
        /* 0x1C */ JNT_LEGR1,
        /* 0x1D */ JNT_LEGR2,
        /* 0x1E */ JNT_FOOTR,
        /* 0x1F */ JNT_SKIRTR,
        /* 0x20 */ JNT_SKIRT1,
        /* 0x21 */ JNT_SKIRT2,
    };

    enum Material {
        /* 0x0 */ SC_EYE,
        /* 0x1 */ ARU_BODY_M,
        /* 0x2 */ ARU_EYEBALL_M,
        /* 0x3 */ ARU_FACE_M,
    };

    typedef int (daNpc_Aru_c::*actionFunc)(void*);
    typedef int (daNpc_Aru_c::*cutFunc)(int);

    ~daNpc_Aru_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    static void* srchCow(void*, void*);
    static void* srchUDoor(void*, void*);
    BOOL chkThrust(fopAc_ac_c*);
    fopAc_ac_c* getCowP(int);
    fopAc_ac_c* getUDoor_l_P();
    fopAc_ac_c* getUDoor_r_P();
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
    BOOL selectAction();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    BOOL chkBullRunningStage();
    BOOL chkSkipFenceStage();
    s16 srchActorDirection(fopAc_ac_c*);
    void adjustMoveDir();
    int duck(int);
    int lookround(s16);
    int cutRideOnHorse(int);
    int cutGotoBullRunningStage(int);
    int cutEndBullRunning(int);
    int cutGotoSkipFenceStage(int);
    int cutSpeakTo(int);
    int cutNoEntrance(int);
    int wait(void*);
    int bullRunning(void*);
    int skipFence(void*);
    int talk(void*);
    int test(void*);
    daNpc_Aru_c(
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
    u16 getEyeballMaterialNo() { return ARU_EYEBALL_M; }
    s32 getHeadJointNo() { return JNT_HEAD; }
    s32 getNeckJointNo() { return JNT_NECK; }
    s32 getBackboneJointNo() { return JNT_BACKBONE1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == JNT_HEAD; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == JNT_MOUTH; }
    s32 getFootLJointNo() { return JNT_FOOTL; }
    s32 getFootRJointNo() { return JNT_FOOTR; }

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        if (nodeNo == 0xffff) {
            return -1;
        }

        return nodeNo;
    }
    int getPathID() { return (fopAcM_GetParam(this) & 0xFF00) >> 8; }
    void setLastIn() { mLastGoatIn = true; }

    static char* mCutNameList[7];
    static cutFunc mCutList[7];

private:
    /* 0xE40 */ daNpc_Aru_HIO_c* mHIO;
    /* 0xE44 */ dCcD_Cyl mCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF84 */ daNpcT_ActorMngr_c mActorMngrs[4];
    /* 0xFA4 */ actionFunc mNextAction;
    /* 0xFB0 */ actionFunc mAction;
    /* 0xFBC */ fpc_ProcID mTimerID;
    /* 0xFC0 */ int mTimer;
    /* 0xFC4 */ int field_0xfc4;
    /* 0xFC8 */ s16 mActorDirection;
    /* 0xFCA */ s16 field_0xfca;
    /* 0xFCC */ u8 field_0xfcc;
    /* 0xFCD */ bool field_0xfcd;
    /* 0xFCE */ bool mLastGoatIn;
    /* 0xFCF */ bool field_0xfcf;
    /* 0xFD0 */ bool field_0xfd0;
    /* 0xFD1 */ bool field_0xfd1;
    /* 0xFD2 */ bool field_0xfd2;
    /* 0xFD3 */ bool field_0xfd3;
    /* 0xFD4 */ u8 field_0xfd4[0xfd8 - 0xfd4];
};

STATIC_ASSERT(sizeof(daNpc_Aru_c) == 0xfd8);

#endif /* D_A_NPC_ARU_H */
