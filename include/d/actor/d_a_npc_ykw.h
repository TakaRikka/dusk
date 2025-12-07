#ifndef D_A_NPC_YKW_H
#define D_A_NPC_YKW_H

#include "d/actor/d_a_npc.h"

class daNpc_ykW_HIO_c;
class daNpc_ykW_Param_c;

#if DEBUG
#define NPC_YKW_HIO_CLASS daNpc_ykW_HIO_c
#else
#define NPC_YKW_HIO_CLASS daNpc_ykW_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_ykW_c
 * @brief Yeta
 *
 * @details
 *
 */
class daNpc_ykW_c : public daNpcT_c {
public:
    typedef int (daNpc_ykW_c::*cutFunc)(int);
    typedef int (daNpc_ykW_c::*actionFunc)(void*);

    ~daNpc_ykW_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    static void* srchItemHeart(void*, void*);
    static void* srchYkw(void*, void*);
    u32 getType(daNpc_ykW_c* da_npc_yk_w_c);
    fopAc_ac_c* getOtherYkwP(int);
    static void* srchYkm(void*, void*);
    fopAc_ac_c* getOtherYkmP(int);
    u8 getType();
    int isDelete();
    void reset();
    void afterJntAnm(int);
    void setParam();
    int checkChangeEvt();
    void setAfterTalkMotion();
    void srchActors();
    int evtTalk();
    int evtCutProc();
    void action();
    void beforeMove();
    void afterMoved();
    void setAttnPos();
    void setCollision();
    int drawDbgInfo();
    int selectAction();
    BOOL chkAction(actionFunc param_0);
    BOOL setAction(int (daNpc_ykW_c::*)(void*));
    cXyz getGoalPos();
    int orderGoIntoBossRoomEvt();
    fopAc_ac_c* putUtuwaHeart(cXyz*, f32, f32, s16, cXyz*);
    int cutShowDoor(int);
    int cutGoIntoBossRoom(int);
    int cutLv5DungeonClear(int);
    int cutFindWolf(int);
    int cutStartSnowboardRace(int);
    int cutEndSnowboardRace(int);
    int cutHug(int);
    void setDialogueMotion();
    int dialogue();
    int wait(void*);
    int sitWait(void*);
    int walk(void*);
    int race(void*);
    int talk(void*);
    int test(void*);
    daNpc_ykW_c(const daNpcT_faceMotionAnmData_c * param_0,
                               const daNpcT_motionAnmData_c* param_1,
                               const daNpcT_MotionSeqMngr_c::sequenceStepData_c* param_2,
                               int param_3,
                               const daNpcT_MotionSeqMngr_c::sequenceStepData_c* param_4,
                               int param_5,
                               const daNpcT_evtData_c* param_6, char** param_7):
        daNpcT_c(param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7) {
        // "daNpc_ykW_c -> construct"
        OS_REPORT("|%06d:%x|daNpc_ykW_c -> コンストラクト\n", g_Counter.mCounter0, this);
    }
    u16 getEyeballMaterialNo() { return 2; }
    s32 getHeadJointNo() { return 5; }
    s32 getNeckJointNo() { return 4; }
    s32 getBackboneJointNo() { return 1; }
    int checkChangeJoint(int i_index) { return i_index == 5; }
    int checkRemoveJoint(int i_index) { return i_index == 9; }
    s32 getFootLJointNo() { return 13; }
    s32 getFootRJointNo() { return 16; }

    s32 getFlowNodeNo() {
        u32 angleX = (u16)home.angle.x;
        s32 temp_r30;
        if (angleX == 0xffff) {
            temp_r30 = -1;
        } else {
            temp_r30 = (u16)angleX;
        }
        return temp_r30;
    }

    u8 getBitTRB() {
        return home.angle.z & 0x3f;
    }

    u8 getBitSW() {
        return (fopAcM_GetParam(this) & 0xff0000) >> 16;
    }

    u8 getBitSW2() {
        return (fopAcM_GetParam(this) & 0xff000000) >> 24;
    }

    u8 getBitSW3() {
        return (home.angle.z & 0xff00) >> 8;
    }

    u8 getPathID() {
        return (fopAcM_GetParam(this) & 0xff00) >> 8;
    }

    u8 getExitNo() {
        return (fopAcM_GetParam(this) & 0xf0) >> 4;
    }

    static const char* mCutNameList[8];
    static cutFunc mCutList[8];

    void setDialogueTmr1(int param_1) {
        field_0x1024 = param_1;
        field_0x1018 = 10;
    }

    void setDialogueTmr2(int param_1) {
        field_0x1024 = param_1;
        field_0x101c = 10;
    }

    /* 0x0E40 */ NPC_YKW_HIO_CLASS* mpHIO;
    /* 0x0E44 */ dCcD_Cyl mCyl;
    /* 0x0F80 */ u8 field_0xf80;
    /* 0x0F84 */ daNpcT_ActorMngr_c mActorMngrs[5];
    /* 0x0FAC */ daNpcT_Path_c mPath;
    /* 0x0FD4 */ dPath* mpPath;
    /* 0x0FD8 */ actionFunc field_0xfd8;
    /* 0x0FE4 */ actionFunc field_0xfe4;
    /* 0x0FF0 */ cXyz field_0xff0;
    /* 0x0FFC */ cXyz field_0xffc;
    /* 0x1008 */ cXyz field_0x1008;
    /* 0x1014 */ u32 mTimer;
    /* 0x1018 */ int field_0x1018;
    /* 0x101C */ int field_0x101c;
    /* 0x1020 */ int field_0x1020;
    /* 0x1024 */ int field_0x1024;
    /* 0x1028 */ int field_0x1028;
    /* 0x102C */ int field_0x102c;
    /* 0x1030 */ int field_0x1030;
    /* 0x1034 */ int field_0x1034;
    /* 0x1038 */ int field_0x1038;
    /* 0x103C */ int field_0x103c;
    /* 0x1040 */ u8 field_0x1040[0x1044 - 0x1040];
    /* 0x1044 */ daNpcT_pntData_c field_0x1044;
    /* 0x104C */ int field_0x104c;
    /* 0x1050 */ u8 field_0x1050[0x1054 - 0x1050];
    /* 0x1054 */ f32 field_0x1054;
    /* 0x1058 */ s16 field_0x1058;
    /* 0x105C */ u32 field_0x105c;
    /* 0x1060 */ u8 field_0x1060;
    /* 0x1061 */ u8 field_0x1061;
    /* 0x1062 */ u8 field_0x1062;
    /* 0x1063 */ u8 field_0x1063;
    /* 0x1064 */ u8 field_0x1064;
    /* 0x1065 */ u8 field_0x1065;
    /* 0x1066 */ u8 field_0x1066;
    /* 0x1067 */ u8 field_0x1067;
    /* 0x1068 */ u8 field_0x1068;
    /* 0x1069 */ u8 field_0x1069;
    /* 0x106A */ u8 field_0x106a;
    /* 0x106B */ u8 field_0x106b;
    /* 0x106C */ u8 field_0x106c;
    /* 0x106D */ u8 field_0x106d[0x106e - 0x106d];
    /* 0x106E */ u8 field_0x106e;
    /* 0x106F */ u8 field_0x106f;
    /* 0x1070 */ u8 field_0x1070;
    /* 0x1071 */ u8 field_0x1071[0x1074 - 0x1071];
    /* 0x1070 */ u8 field_0x1074[0x1078 - 0x1074];
};

STATIC_ASSERT(sizeof(daNpc_ykW_c) == 0x1078);

struct daNpc_ykW_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 mSkipFrame;
    /* 0x90 */ f32 mFlySpeed;
    /* 0x94 */ f32 mFlyAngle;
    /* 0x98 */ f32 mJumpInitialSpeed;
    /* 0x9C */ f32 mSlidingAccel;
    /* 0xA0 */ f32 mSlidingSpeed;
    /* 0xA4 */ s16 mCompetParamA;
    /* 0xA6 */ s16 mCompetParamB;
    /* 0xA8 */ f32 mCompetParamC;
    /* 0xAC */ s16 mWalkingTime;
    /* 0xAE */ s16 mDownTime;
    /* 0xB0 */ s16 mStepSpeed;
};

class daNpc_ykW_Param_c {
public:
    virtual ~daNpc_ykW_Param_c() {}

    static const daNpc_ykW_HIOParam m;
};

#endif /* D_A_NPC_YKW_H */
