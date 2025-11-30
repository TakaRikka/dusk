#ifndef D_A_NPC_YELIA_H
#define D_A_NPC_YELIA_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Yelia_c
 * @brief Ilia
 *
 * @details
 *
 */
class daNpc_Yelia_c : public daNpcT_c {
public:
    ~daNpc_Yelia_c();
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
    void setAttnPos();
    void setCollision();
    BOOL drawDbgInfo();
    BOOL selectAction();
    BOOL chkAction(BOOL (daNpc_Yelia_c::*)(void*));
    BOOL setAction(BOOL (daNpc_Yelia_c::*)(void*));
    BOOL cutConversationAboutLoopHole(int);
    BOOL cutTWResistance(int);
    BOOL cutTakeWoodStatue(int);
    BOOL cutClothTry(int);
    BOOL cutThankYou(int);
    BOOL wait(void*);
    BOOL horseWait(void*);
    BOOL reminiscence(void*);
    BOOL talk(void*);
    daNpc_Yelia_c(daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
                        daNpcT_motionAnmData_c const* i_motionAnmData,
                        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
                        int i_faceMotionStepNum,
                        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData,
                        int i_motionStepNum,
                        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
                        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                        i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                        i_arcNames) {}
    u16 getEyeballMaterialNo() { return 1; }
    s32 getHeadJointNo() { return 4; }
    s32 getNeckJointNo() { return 3; }
    s32 getBackboneJointNo() { return 1; }
    BOOL checkChangeJoint(int i_jointNo) { return i_jointNo == 4; }
    BOOL checkRemoveJoint(int i_jointNo) {return i_jointNo == 9; }
    BOOL chkXYItems() { return true; }

    u32 getFlowNodeNo() {
        u16 no = home.angle.x;
        if (no == 0xffff) {
            return -1;
        }
        return no;
    }

    static char* mCutNameList[6];
    static int (daNpc_Yelia_c::*mCutList[6])(int);
private:
    /* 0xE40 */ u8 field_0xe40[0xe44 - 0xe40];
    /* 0xE44 */ dCcD_Cyl mCcCyl;
    /* 0xF80 */ u8 mType;
    /* 0xF84 */ daNpcT_ActorMngr_c mActorMngr[4];
    /* 0xFA4 */ BOOL (daNpc_Yelia_c::*mpNextAction)(void*);
    /* 0xFB0 */ BOOL (daNpc_Yelia_c::*mpAction)(void*);
    /* 0xFBC */ daNpcT_Path_c mPath;
    /* 0xFE4 */ fpc_ProcID mItemId;
    /* 0xFE8 */ int mPendantTimer;
    /* 0xFEC */ u8 field_0xfec;
};

STATIC_ASSERT(sizeof(daNpc_Yelia_c) == 0xff0);

class daNpc_Yelia_Param_c {
public:
    struct param {
        /* 0x00 */ f32 mAttnOffsetY;
        /* 0x04 */ f32 mGravity;
        /* 0x08 */ f32 mScale;
        /* 0x0C */ f32 mShadowDepth;
        /* 0x10 */ f32 mCcWeight;
        /* 0x14 */ f32 mCylH;
        /* 0x18 */ f32 mWallH;
        /* 0x1C */ f32 mWallR;
        /* 0x20 */ f32 mBodyUpAngle;
        /* 0x24 */ f32 mBodyDownAngle;
        /* 0x28 */ f32 mBodyLeftAngle;
        /* 0x2C */ f32 mBodyRightAngle;
        /* 0x30 */ f32 mHeadUpAngle;
        /* 0x34 */ f32 mHeadDownAngle;
        /* 0x38 */ f32 mHeadLeftAngle;
        /* 0x3C */ f32 mHeadRightAngle;
        /* 0x40 */ f32 mNeckAngleScl;
        /* 0x44 */ f32 mMorfFrames;
        /* 0x48 */ s16 mSpeakDistIdx;
        /* 0x4A */ s16 mSpeakAngleIdx;
        /* 0x4C */ s16 mTalkDistIdx;
        /* 0x4E */ s16 mTalkAngleIdx;
        /* 0x50 */ f32 mAttnFovY;
        /* 0x54 */ f32 mAttnRadius;
        /* 0x58 */ f32 mAttnUpperY;
        /* 0x5C */ f32 mAttnLowerY;
        /* 0x60 */ s16 field_0x60;
        /* 0x62 */ s16 mDamageTimer;
        /* 0x64 */ s16 mTestExpression;
        /* 0x66 */ s16 mTestMotion;
        /* 0x68 */ s16 mTestLookMode;
        /* 0x6A */ bool mTest;
        /* 0x6C */ f32 field_0x6c;
        /* 0x70 */ f32 field_0x70;
        /* 0x74 */ f32 field_0x74;
        /* 0x78 */ f32 field_0x78;
        /* 0x7C */ f32 field_0x7c;
        /* 0x80 */ f32 field_0x80;
        /* 0x84 */ f32 field_0x84;
        /* 0x88 */ f32 field_0x88;
    };

    virtual ~daNpc_Yelia_Param_c() {}

    static param const m;
};

#endif /* D_A_NPC_YELIA_H */
