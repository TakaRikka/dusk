#ifndef D_A_NPC_ZRC_H
#define D_A_NPC_ZRC_H

#include "d/actor/d_a_npc4.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_zrC_c
 * @brief Ralis
 *
 * @details
 *
 */
class daNpc_zrC_c : public daNpcF_c {
public:
    typedef BOOL (daNpc_zrC_c::*ActionFn)(void*);
    typedef BOOL (daNpc_zrC_c::*EventFn)(int);

    daNpc_zrC_c();
    ~daNpc_zrC_c();
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    void setParam();
    BOOL main();
    BOOL ctrlBtk();
    void setAttnPos();
    bool setExpressionAnm(int, bool);
    bool setExpressionBtp(int);
    void setExpression(int, f32);
    void setMotionAnm(int, f32);
    void setMotion(int, f32, int);
    BOOL drawDbgInfo();
    void drawOtherMdls();
    u8 getTypeFromParam();
    int isDelete();
    void reset();
    void playExpression();
    void playMotion();
    void playMotionAnm2(daNpcF_c::daNpcF_anmPlayData***);
    BOOL chkAction(ActionFn);
    BOOL setAction(ActionFn);
    BOOL selectAction();
    void doNormalAction(BOOL);
    BOOL doEvent();
    void setLookMode(int);
    void lookat();
    void setExpressionTalkAfter();
    BOOL wait(void*);
    BOOL waitSick(void*);
    BOOL waitPray(void*);
    BOOL waitThrone(void*);
    BOOL talk(void*);
    BOOL test(void*);
    BOOL ECut_earringGet(int);
    void adjustShapeAngle() {}

    static char* mEvtCutNameList[2];
    static EventFn mEvtCutList[2];

private:
    /* 0xB48 */ Z2Creature mCreatureSound;
    /* 0xBD8 */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBDC */ daNpcF_Lookat_c mLookat;
    /* 0xC78 */ daNpcF_ActorMngr_c mActorMngr[3];
    /* 0xC90 */ u8 field_0xc90[4];
    /* 0xC94 */ dCcD_Cyl mCcCyl;
    /* 0xDD0 */ ActionFn mpNextActionFn;
    /* 0xDDC */ ActionFn mpActionFn;
    /* 0xDE8 */ request_of_phase_process_class mPhase[5];
    /* 0xE10 */ int field_0xe10;
    /* 0xE14 */ int field_0xe14;
    /* 0xE18 */ int field_0xe18;
    /* 0xE1C */ int field_0xe1c;
    /* 0xE20 */ int mFlowID;
    /* 0xE24 */ s16 mLookMode;
    /* 0xE26 */ u16 mMode;
    /* 0xE28 */ u8 mType;
    /* 0xE2C */ int mMotionAnm;
    /* 0xE30 */ bool field_0xe30;
    /* 0xE34 */ u32 mItemID;
    /* 0xE38 */ bool mSpeakEvent;

    enum Animation {
        /* 0x00 */ ANM_F_WAIT_A,
        /* 0x01 */ ANM_F_SIT_TALK,
        /* 0x02 */ ANM_F_TALK_A,
        /* 0x03 */ ANM_F_TOPRAY,
        /* 0x04 */ ANM_F_SAD,
        /* 0x05 */ ANM_F_SMILE,
        /* 0x06 */ ANM_F_KIZUKU,
        /* 0x07 */ ANM_F_SAD_WAIT,
        /* 0x08 */ ANM_F_KIZUKU_WAIT,
        /* 0x09 */ ANM_F_TALK_NORMAL,
        /* 0x0A */ ANM_F_PRAY,
        /* 0x0B */ ANM_F_GETUP,
        /* 0x0C */ ANM_F_SIT,
        /* 0x0D */ ANM_F_STEP,
        /* 0x0E */ ANM_FH_SAD,
        /* 0x0F */ ANM_FH_SMILE,
        /* 0x10 */ ANM_FH_KIZUKU,
        /* 0x11 */ ANM_F_SICK_DEMO_WAIT,
        /* 0x12 */ ANM_WAIT_A,
        /* 0x13 */ ANM_TOPRAY,
        /* 0x14 */ ANM_PRAY,
        /* 0x15 */ ANM_GETUP,
        /* 0x16 */ ANM_STEP,
        /* 0x17 */ ANM_SIT,
        /* 0x18 */ ANM_SIT_TALK,
        /* 0x19 */ ANM_SAD_TALK,
        /* 0x1A */ ANM_SAD_WAIT,
        /* 0x1B */ ANM_TALK_A,
        /* 0x1C */ ANM_KIZUKU,
        /* 0x1D */ ANM_KIZUKU_WAIT,
        /* 0x1E */ ANM_SMILE_TALK,
        /* 0x1F */ ANM_SICK_DEMO_WAIT,
    };

    enum Expression {
        /* 0x00 */ EXPR_TALK_A,
        /* 0x01 */ EXPR_SIT_TALK,
        /* 0x02 */ EXPR_PRAY,
        /* 0x03 */ EXPR_SAD,
        /* 0x04 */ EXPR_SMILE,
        /* 0x05 */ EXPR_KIZUKU,
        /* 0x06 */ EXPR_SAD_WAIT,
        /* 0x07 */ EXPR_TALK_NORMAL,
        /* 0x08 */ EXPR_KIZUKU_WAIT,
        /* 0x09 */ EXPR_H_KIZUKU,
        /* 0x0A */ EXPR_H_SAD,
        /* 0x0B */ EXPR_H_SMILE,
        /* 0x0C */ EXPR_STEP,
        /* 0x0D */ EXPR_TOPRAY,
        /* 0x0E */ EXPR_GETUP,
        /* 0x0F */ EXPR_SIT,
        /* 0x10 */ EXPR_SICK_DEMO_WAIT,
        /* 0x11 */ EXPR_WAIT_A,
    };

    enum Motion {
        /* 0x0 */ MOT_WAIT_A,
        /* 0x1 */ MOT_SIT_TALK,
        /* 0x2 */ MOT_PRAY,
        /* 0x3 */ MOT_SIT,
        /* 0x4 */ MOT_SAD_TALK,
        /* 0x5 */ MOT_SAD_WAIT,
        /* 0x6 */ MOT_TALK_A,
        /* 0x7 */ MOT_KIZUKU,
        /* 0x8 */ MOT_KIZUKU_WAIT,
        /* 0x9 */ MOT_SMILE_TALK,
        /* 0xA */ MOT_TOPRAY,
        /* 0xB */ MOT_GETUP,
        /* 0xC */ MOT_STEP,
        /* 0xD */ MOT_SICK_DEMO_WAIT,
    };

    enum Event {
        /* 0 */ EVT_NONE,
        /* 1 */ EVT_EARRING_GET,
    };
};

STATIC_ASSERT(sizeof(daNpc_zrC_c) == 0xe3c);

class daNpc_zrC_Param_c {
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
    };

    virtual ~daNpc_zrC_Param_c() {}

    static param const m;
};

STATIC_ASSERT(sizeof(daNpc_zrC_Param_c::param) == 0x74);

#endif /* D_A_NPC_ZRC_H */
