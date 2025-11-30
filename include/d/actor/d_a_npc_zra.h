#ifndef D_A_NPC_ZRA_H
#define D_A_NPC_ZRA_H

#include "d/actor/d_a_npc4.h"
#include "d/d_particle_copoly.h"

class daNpc_zrA_HIO_c;
/**
 * @ingroup actors-npcs
 * @class daNpc_zrA_Path_c
 * @brief Zora (Adult)
 *
 * @details
 *
*/

class daNpc_zrA_Path_c : public daNpcF_Path_c {
public:
    BOOL getDstPosDst2(cXyz, cXyz&);
    void setNextIdxDst(cXyz);
    BOOL chkPassedDstXZ(cXyz);
    BOOL getDstPosDstXZ(cXyz, cXyz&);
    BOOL chkPassedChase(u16, cXyz);
    int getDstPosChase(u16, cXyz, cXyz&);
    f32 chkPassDst(u16, cXyz);
    int getEndIdx() { return mPathInfo->m_num - 1; }
};

STATIC_ASSERT(sizeof(daNpc_zrA_Path_c) == 0x630);

class daNpc_zrA_c : public daNpcF_c {
public:
    typedef BOOL (daNpc_zrA_c::*ActionFn)(void*);
    typedef BOOL (daNpc_zrA_c::*EventFn)(int);

    enum Type {
        /* 0 */ TYPE_WAIT,
        /* 1 */ TYPE_SWIM,
        /* 2 */ TYPE_WATERFALL,
        /* 3 */ TYPE_RIVER,
        /* 4 */ TYPE_TOBIKOMI,
        /* 5 */ TYPE_SEARCH,
        /* 6 */ TYPE_SPA,
    };

    enum SoldierType {
        /* 0 */ SOLDIER_SPEAR,
        /* 1 */ SOLDIER_NOSPEAR,
        /* 2 */ SOLDIER_NONE,
    };

    enum ActionType {
        /* 0 */ ACT_TYPE_0,
        /* 1 */ ACT_TYPE_1,
        /* 2 */ ACT_TYPE_2,
        /* 3 */ ACT_TYPE_3,
        /* 4 */ ACT_TYPE_4,
        /* 5 */ ACT_TYPE_5,
        /* 6 */ ACT_TYPE_6,
    };

    enum GameMode {
        /* 0 */ GAME_MODE_0,
        /* 1 */ GAME_MODE_1,
        /* 2 */ GAME_MODE_2,
        /* 3 */ GAME_MODE_3,
    };

    enum SwimMode {
        /* 0 */ SWIM_WAIT,
        /* 1 */ SWIM_RAIL,
        /* 2 */ SWIM_TURN,
        /* 3 */ SWIM_3,
    };

    enum Animation {
        /* 0x00 */ ANM_NONE,
        /* 0x01 */ ANM_F_TALK_A,
        /* 0x02 */ ANM_F_TALK_A_SP,
        /* 0x03 */ ANM_F_WAIT_SWIM,
        /* 0x04 */ ANM_F_TALK_NOMAL,
        /* 0x05 */ ANM_FH_TALK_NOMAL,
        /* 0x06 */ ANM_FH_SADSIT_A,
        /* 0x07 */ ANM_FH_SADSIT_B,
        /* 0x08 */ ANM_FH_SADSIT_C,
        /* 0x09 */ ANM_FH_SADSIT_D,
        /* 0x0A */ ANM_FH_SADSIT_E,
        /* 0x0B */ ANM_F_LOOKING_SP,
        /* 0x0C */ ANM_FH_LOOKING_SP,
        /* 0x0D */ ANM_F_LOOKUP,
        /* 0x0E */ ANM_FH_LOOKUP,
        /* 0x0F */ ANM_F_TALK_SWIM_SP,
        /* 0x10 */ ANM_F_TALK_B_SP,
        /* 0x11 */ ANM_F_SPA_TALK_A,
        /* 0x12 */ ANM_FH_SPA_WAIT_A,
        /* 0x13 */ ANM_F_SPA_TALK_B,
        /* 0x14 */ ANM_FH_SPA_WAIT_B,
        /* 0x15 */ ANM_SWIM_TURN,
        /* 0x16 */ ANM_STILL,
        /* 0x17 */ ANM_FALLSWIM,
        /* 0x18 */ ANM_SWIM_TALK,
        /* 0x19 */ ANM_SADSIT_A,
        /* 0x1A */ ANM_SADSIT_B,
        /* 0x1B */ ANM_SADSIT_C,
        /* 0x1C */ ANM_SADSIT_D,
        /* 0x1D */ ANM_SADSIT_E,
        /* 0x1E */ ANM_TOBIKOMI_S,
        /* 0x1F */ ANM_TOBIKOMI_T,
        /* 0x20 */ ANM_TOBIKOMI_E,
        /* 0x21 */ ANM_LOOKING_SP,
        /* 0x22 */ ANM_TALK_SWIM_SP,
        /* 0x23 */ ANM_TALK_B_SP,
        /* 0x24 */ ANM_LOOKUP,
        /* 0x25 */ ANM_SPA_WAIT_B,
        /* 0x26 */ ANM_SPA_TALK_B,
        /* 0x27 */ ANM_SPA_WAIT_A,
        /* 0x28 */ ANM_SPA_TALK_A,
        /* 0x29 */ ANM_WAIT_A,
        /* 0x2A */ ANM_WALK_A,
        /* 0x2B */ ANM_TALK_A,
        /* 0x2C */ ANM_SWIM_A,
        /* 0x2D */ ANM_SWIM_B,
        /* 0x2E */ ANM_WAIT_SWIM,
        /* 0x2F */ ANM_DIVE,
        /* 0x30 */ ANM_DIVE_B,
        /* 0x31 */ ANM_FLOAT,
        /* 0x32 */ ANM_FLOAT_B,
        /* 0x33 */ ANM_STEP,
        /* 0x34 */ ANM_WAIT_SP,
        /* 0x35 */ ANM_WALK_A_SP,
        /* 0x36 */ ANM_TALK_A_SP,
        /* 0x37 */ ANM_SWIM_A_SP,
        /* 0x38 */ ANM_SWIM_B_SP,
        /* 0x39 */ ANM_WAIT_SWIM_SP,
        /* 0x3A */ ANM_DIVE_SP,
        /* 0x3B */ ANM_DIVE_B_SP,
        /* 0x3C */ ANM_FLOAT_SP,
        /* 0x3D */ ANM_FLOAT_B_SP,
        /* 0x3E */ ANM_STEP_SP,
    };

    enum Expression {
        /* 0x00 */ EXPR_TALK_NOMAL,
        /* 0x01 */ EXPR_SADSIT_A,
        /* 0x02 */ EXPR_SADSIT_B,
        /* 0x03 */ EXPR_SADSIT_C,
        /* 0x04 */ EXPR_SADSIT_D,
        /* 0x05 */ EXPR_SADSIT_E,
        /* 0x06 */ EXPR_LOOKING_SP,
        /* 0x07 */ EXPR_LOOKUP,
        /* 0x08 */ EXPR_SPA_TALK_A,
        /* 0x09 */ EXPR_SPA_TALK_B,
        /* 0x0A */ EXPR_TALK_SWIM_SP,
        /* 0x0B */ EXPR_TALK_B_SP,
        /* 0x0C */ EXPR_TALK_A,
        /* 0x0D */ EXPR_WAIT_SWIM,
        /* 0x0E */ EXPR_TALK_A_SP,
        /* 0x0F */ EXPR_TALK_NOMAL_2,
        /* 0x10 */ EXPR_LOOKING_SP_2,
        /* 0x11 */ EXPR_LOOKUP_2,
        /* 0x12 */ EXPR_SPA_WAIT_A,
        /* 0x13 */ EXPR_SPA_WAIT_B,
        /* 0x14 */ EXPR_NONE,
    };

    enum Motion {
        /* 0x00 */ MOT_WAIT_A,
        /* 0x01 */ MOT_WAIT_SWIM,
        /* 0x02 */ MOT_SWIM_TALK,
        /* 0x03 */ MOT_WAIT_A_2,
        /* 0x04 */ MOT_WAIT_SWIM_2,
        /* 0x05 */ MOT_TALK_A,
        /* 0x06 */ MOT_SADSIT_A,
        /* 0x07 */ MOT_SADSIT_B,
        /* 0x08 */ MOT_SADSIT_C,
        /* 0x09 */ MOT_SADSIT_D,
        /* 0x0A */ MOT_SADSIT_E,
        /* 0x0B */ MOT_LOOKING_SP,
        /* 0x0C */ MOT_LOOKUP,
        /* 0x0D */ MOT_SPA_TALK_A,
        /* 0x0E */ MOT_SPA_TALK_B,
        /* 0x0F */ MOT_TALK_SWIM_SP,
        /* 0x10 */ MOT_TALK_B_SP,
        /* 0x11 */ MOT_TALK_A_2,
        /* 0x12 */ MOT_SPA_WAIT_A,
        /* 0x13 */ MOT_SPA_WAIT_B,
        /* 0x14 */ MOT_WALK_A,
        /* 0x16 */ MOT_SWIM_A = 0x16,
        /* 0x17 */ MOT_SWIM_B,
        /* 0x18 */ MOT_DIVE_SWIM_A,
        /* 0x19 */ MOT_DIVE_B_SWIM_A,
        /* 0x1A */ MOT_DIVE_SWIM_B,
        /* 0x1B */ MOT_DIVE_B_SWIM_B,
        /* 0x1C */ MOT_STEP,
        /* 0x1D */ MOT_FLOAT,
        /* 0x1E */ MOT_FLOAT_B,
        /* 0x1F */ MOT_SWIM_TURN,
        /* 0x20 */ MOT_TOBIKOMI_S,
        /* 0x21 */ MOT_TOBIKOMI_T,
        /* 0x22 */ MOT_TOBIKOMI_E,
        /* 0x23 */ MOT_FALLSWIM,
    };

    enum Event {
        /* 0x0 */ EVT_NONE,
        /* 0x1 */ EVT_TALK_SWIM,
        /* 0x2 */ EVT_BEFORE_BLAST_ZRR,
        /* 0x3 */ EVT_AFTER_BLAST_ZRR,
        /* 0x4 */ EVT_THANKS_BLAST,
        /* 0x5 */ EVT_RESULT_ANNOUNCE,
        /* 0x6 */ EVT_CARRY_WATERFALL,
        /* 0x7 */ EVT_CARRY_WATERFALL_NIGHT,
        /* 0x8 */ EVT_CARRY_WATERFALL_SKIP,
        /* 0x9 */ EVT_CARRY_WATERFALL_NIGHT_SKIP,
        /* 0xA */ EVT_SEARCH_PRINCE,
        /* 0xB */ EVT_TALK_MULTI,
        /* 0xC */ EVT_TALK_MULTI2,
    };

    daNpc_zrA_c();
    ~daNpc_zrA_c();
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
    void setMtx();
    bool setExpressionAnm(int, bool);
    bool setExpressionBtp(int);
    void setExpression(int, f32);
    void setMotionAnm(int, f32);
    void setMotionWaterAnm(int);
    void setMotion(int, f32, BOOL);
    BOOL drawDbgInfo();
    void drawOtherMdls();
    Type getTypeFromArgument();
    SoldierType getSoldierTypeFromParam();
    GameMode getGameModeFromParam();
    ActionType getActionTypeFromParam();
    u8 getMultiModeFromParam();
    u8 getMultiNoFromParam();
    /*  inline  */ u8 getNoFromParam() { return (u8)fopAcM_GetParam(this); }
    /*  inline  */ u8 getPathNoFromParam() { return (u8)(fopAcM_GetParam(this) >> 8); }
    BOOL isDelete();
    void reset();
    void playExpression();
    void playMotion();
    BOOL chkAction(ActionFn);
    bool setAction(ActionFn);
    bool selectAction();
    BOOL doEvent();
    BOOL setSkipZev(int, int);
    void setLookMode(int);
    void lookat();
    BOOL chkFindPlayer();
    void setExpressionTalkAfter();
    void setPrtcl();
    BOOL test(void*);
    BOOL wait(void*);
    void walkCalc();
    void waitCalc();
    BOOL waitSpa(void*);
    BOOL waitLake(void*);
    BOOL talk(void*);
    BOOL ECut_talkMulti(int);
    BOOL swim(void*);
    BOOL railSwim();
    BOOL waitSwim();
    BOOL turnSwimInit();
    BOOL turnSwim();
    BOOL turnSwimCheck();
    BOOL evasionCalc(cXyz&, csXyz&);
    void calcBank(s16, s16, s16&, s16&);
    void calcWaistAngle();
    void calcWaistAngleInit();
    void calcWaistAngleStop();
    BOOL calcWaistAngleCheck();
    void calcModulation();
    void resetModulation();
    void calcSwimAngle(csXyz&, csXyz&, s16, s16);
    void calcSwimPos(cXyz&);
    void calcWaitSwim(BOOL);
    BOOL ECut_talkSwim(int);
    BOOL waitWaterfall(void*);
    BOOL swimWaterfall(void*);
    BOOL diveWaterfall(void*);
    BOOL talkSwim(void*);
    BOOL ECut_carryWaterfall(int);
    BOOL ECut_carryWaterfallSkip(int);
    BOOL diveCalc(s16, s16, BOOL);
    BOOL waitRiverDescend(void*);
    BOOL swimRiverDescend(void*);
    BOOL swimRiverDescend2(void*);
    BOOL diveRiverDescend(void*);
    BOOL swimGoalRiverDescend(void*);
    BOOL returnRiverDescend(void*);
    void moveRiverPosCalc(cXyz&);
    void createRuppi();
    BOOL ECut_beforeBlastzrR(int);
    BOOL ECut_afterBlastzrR(int);
    BOOL ECut_thanksBlast(int);
    BOOL ECut_resultAnnounce(int);
    void calcCanoeMove(BOOL);
    BOOL tobiSearch(void*);
    BOOL tobiWait(void*);
    BOOL tobikomi1(void*);
    BOOL tobikomi2(void*);
    BOOL tobikomi3(void*);
    BOOL tobiJump(void*);
    BOOL tobiEnd(void*);
    BOOL waitSearch(void*);
    BOOL ECut_searchPrince1(int);
    BOOL ECut_searchPrince2(int);
    void adjustShapeAngle() {}

    int getType() { return mType; }
    u32 getAngleNoFromParam() { return (u8)(fopAcM_GetParam(this) >> 8); }
    void setBlastFlag(u8 i_flag) { mBlastFlag = i_flag; }
    MtxP getHeadMtx() { return mAnm_p->getModel()->getAnmMtx(4); }

    /* 0x0B48 */ Z2Creature mCreatureSound;
    /* 0x0BD8 */ J3DModel* mpObjectModel[3];
    /* 0x0BE4 */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0x0BE8 */ mDoExt_btkAnm mWaterBtkAnm;
    /* 0x0C00 */ mDoExt_bpkAnm mWaterBpkAnm;
    /* 0x0C18 */ daNpc_zrA_Path_c mPath;
    /* 0x1248 */ daNpcF_Lookat_c mLookat;
    /* 0x12E4 */ dPaPo_c mPaPo;
    /* 0x131C */ daNpcF_ActorMngr_c mActorMngr[3];
    /* 0x1334 */ daNpc_zrA_HIO_c* mpHIO;
    /* 0x1338 */ dCcD_Cyl mCcCyl;
    /* 0x1474 */ ActionFn mpNextActionFn;
    /* 0x1480 */ ActionFn mpActionFn;
    /* 0x148C */ request_of_phase_process_class mPhase[8];
    /* 0x14CC */ u32 mItemID;
    /* 0x14D0 */ int field_0x14d0;
    /* 0x14D4 */ int field_0x14d4;
    /* 0x14D8 */ int mSwimTurnTimer;
    /* 0x14DC */ int field_0x14dc;
    /* 0x14E0 */ int mFlowID;
    /* 0x14E4 */ s16 mLookMode;
    /* 0x14E6 */ u16 mMode;
    /* 0x14E8 */ u8 mType;
    /* 0x14E9 */ u8 mSoldierType;
    /* 0x14EC */ int mBaseMotionAnm;
    /* 0x14F0 */ int mBtkID;
    /* 0x14F4 */ int mBpkID;
    /* 0x14F8 */ u32 mWaterAnmFlags;
    /* 0x14FC */ char* mStaffName;
    /* 0x1500 */ cXyz field_0x1500;
    /* 0x150C */ u8 mMeterCount;
    /* 0x150D */ u8 mGameMode;
    /* 0x150E */ u8 mSwitch1;
    /* 0x150F */ u8 mSwitch2;
    /* 0x1510 */ int field_0x1510;
    /* 0x1514 */ int mSpinAngle;
    /* 0x1518 */ s16 field_0x1518;
    /* 0x151C */ int mSpinTimer;
    /* 0x1520 */ u8 mSwimMode;
    /* 0x1521 */ bool field_0x1521;
    /* 0x1524 */ f32 mSwimSpeedF;
    /* 0x1528 */ f32 mSwimSpeedScale;
    /* 0x152C */ u8 field_0x152c[4];
    /* 0x1530 */ csXyz mWaistAngle;
    /* 0x1536 */ csXyz mSwimAngleCalc;
    /* 0x153C */ bool field_0x153c;
    /* 0x153D */ bool field_0x153d;
    /* 0x153E */ bool field_0x153e;
    /* 0x1540 */ int mSwimFastTurnTimer;
    /* 0x1544 */ cXyz mSwimSpeed;
    /* 0x1550 */ bool field_0x1550;
    /* 0x1554 */ int field_0x1554;
    /* 0x1558 */ int field_0x1558;
    /* 0x155C */ int mModulationParam;
    /* 0x1560 */ cXyz mModulationOffset;
    /* 0x156C */ f32 mScaleFactor;
    /* 0x1570 */ int mEvasionTimer;
    /* 0x1574 */ s16 mEvasionAngle;
    /* 0x1576 */ u8 mActionType;
    /* 0x1577 */ bool mIsAboveWater;
    /* 0x1578 */ cXyz field_0x1578;
    /* 0x1584 */ cXyz field_0x1584;
    /* 0x1590 */ bool mResetWaistAngle;
    /* 0x1591 */ bool mIsTurning;
    /* 0x1592 */ u8 mCheckSwimTurnTimer;
    /* 0x1593 */ bool mCheckSwimTurn;
    /* 0x1594 */ u8 mActionSelect;
    /* 0x1595 */ int mRiverPathIdx;
    /* 0x159C */ cXyz field_0x159c[3];
    /* 0x15C0 */ u8 field_0x15c0;
    /* 0x15C1 */ bool mBlastFlag;

    static char* mEvtCutNameList[11];
    static EventFn mEvtCutList[11];
};

STATIC_ASSERT(sizeof(daNpc_zrA_c) == 0x15C4);

class daNpc_zrA_Param_c {
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
        /* 0x6C */ f32 mSwimSpeed;
        /* 0x70 */ f32 mMinSwimSpeedScale;
        /* 0x74 */ s16 mSwimAngleSpeed;
        /* 0x78 */ f32 mSwimAnmRate;
        /* 0x7C */ f32 field_0x7c;
        /* 0x80 */ f32 field_0x80;
        /* 0x84 */ f32 mMaxScaleFactor;
        /* 0x88 */ f32 mMinDepth;
        /* 0x8C */ f32 field_0x8c;
        /* 0x90 */ f32 field_0x90;
        /* 0x94 */ f32 mWalkSpeed;
        /* 0x98 */ s16 mWalkAngleSpeed;
        /* 0x9A */ s16 mWalkAngleScale;
        /* 0x9C */ f32 mWalkAnmRate;
        /* 0xA0 */ f32 field_0xa0;
        /* 0xA4 */ f32 field_0xa4;
        /* 0xA8 */ f32 field_0xa8;
        /* 0xAC */ f32 field_0xac;
     };

    virtual ~daNpc_zrA_Param_c() {}

    static param const m;
};

STATIC_ASSERT(sizeof(daNpc_zrA_Param_c::param) == 0xB0);

#endif /* D_A_NPC_ZRA_H */
