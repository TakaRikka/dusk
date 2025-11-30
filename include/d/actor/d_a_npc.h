#ifndef D_A_D_A_NPC_H
#define D_A_D_A_NPC_H

#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "SSystem/SComponent/c_counter.h"
#include "SSystem/SComponent/c_math.h"
#include "Z2AudioLib/Z2Creature.h"
#include "d/actor/d_a_player.h"
#include "d/d_path.h"
#include "d/d_msg_flow.h"
#include "d/d_particle_copoly.h"
#include "global.h"

struct daNpc_GetParam1 {  // name unknown
    /* 0x0 */ int fileIdx;
    /* 0x4 */ int arcIdx;
};

struct daNpc_GetParam2 {  // name unknown
    /* 0x0 */ int fileIdx;
    /* 0x4 */ int attr;
    /* 0x8 */ int arcIdx;
};

struct daNpc_GetParam3 {  // name unknown
    /* 0x0 */ int fileIdx;
    /* 0x4 */ int attr;
};

class daNpcT_ActorMngr_c {
private:
    /* 0x0 */ fpc_ProcID mActorID;

public:
    void initialize();
    void entry(fopAc_ac_c*);
    void remove();
    fopAc_ac_c* getActorP();

    daNpcT_ActorMngr_c() { initialize(); };

    virtual ~daNpcT_ActorMngr_c() {}
};

class daNpcT_MatAnm_c : public J3DMaterialAnm {
private:
    /* 0x0F4 */ mutable f32 field_0xF4;
    /* 0x0F8 */ mutable f32 field_0xF8;
    /* 0x0FC */ f32 mNowOffsetX;
    /* 0x100 */ f32 mNowOffsetY;
    /* 0x104 */ u8 mEyeMoveFlg;
    /* 0x105 */ u8 mMorfFrm;

public:
    daNpcT_MatAnm_c() { initialize(); }

    void initialize();
    virtual ~daNpcT_MatAnm_c() {}
    virtual void calc(J3DMaterial*) const;

    void setNowOffsetX(f32 i_value) { mNowOffsetX = i_value; }
    void setNowOffsetY(f32 i_value) { mNowOffsetY = i_value; }
    void onEyeMoveFlag() { mEyeMoveFlg = 1; }
    void offEyeMoveFlag() { mEyeMoveFlg = 0; }
    void setMorfFrm(u8 i_value) { mMorfFrm = i_value; }
};

class daNpcT_MotionSeqMngr_c {
public:
    class sequenceStepData_c {
    public:
        /* 0x0 */ s16 mAnmIdx;
        /* 0x2 */ s8 mMorf;
        /* 0x3 */ s8 mLoopCount;
    };  // Size: 0x4?

    void initialize();
    int play(u16, int*, f32*);
    s32 checkEndSequence();

    daNpcT_MotionSeqMngr_c(sequenceStepData_c const* param_1,int param_2) :
    mpSeqData(param_1), mStepNum(param_2) {
        initialize();
    }

    bool checkEntryNewMotion() { return mStepNo == 0 && mPrevStepNo == -1; }
    int getNo() { return mNo; }
    int getOffset() { return mOffset; }
    int getStepNo() { return mStepNo; }
    void setNo(int i_index, f32 i_morf, BOOL i_reset, int i_offset) {
        if (i_index >= 0) {
            int newNo = i_index + i_offset;
            if (i_reset || newNo != mNo) {
                int offset = mOffset;
                initialize();
                mOffset = offset;
                mNo = newNo;
                mInitialMorf = i_morf;
            }
        }
    }
    void setOffset(int i_offset) { mOffset = i_offset; }

private:
    /* 0x00 */ sequenceStepData_c const* mpSeqData;
    /* 0x04 */ int mStepNum;
    /* 0x08 */ int mNo;
    /* 0x0C */ int mOffset;
    /* 0x10 */ int mStepNo;
    /* 0x14 */ int mPrevStepNo;
    /* 0x18 */ f32 mInitialMorf;
    /* 0x1C */ bool mEndSequence;

public:
    virtual ~daNpcT_MotionSeqMngr_c() {}
};

class daNpcT_Hermite_c {
public:
    /* 0x00 */ f32 field_0x00;
    /* 0x04 */ f32 H00;
    /* 0x08 */ f32 H01;
    /* 0x0C */ f32 H10;
    /* 0x10 */ f32 H11;

    virtual ~daNpcT_Hermite_c() {}

    f32 GetH00() { return H00; }
    f32 GetH01() { return H01; }
    f32 GetH10() { return H10; }
    f32 GetH11() { return H11; }

    void Set(f32 param_1) {
        field_0x00 = param_1;
        f32 cubed = param_1 * param_1 * param_1;
        f32 sqr = param_1 * param_1;

        H00 = 1.0f + (2.0f * cubed - 3.0f * sqr);
        H01 = -2.0f * cubed + 3.0f * sqr;
        H10 = param_1 + (cubed - 2.0f * sqr);
        H11 = cubed - sqr;
    }
};

class daNpcT_Path_c {
public:
    /* 0x00 */ dPath* mPathInfo;
    /* 0x04 */ cXyz mPosition;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ u16 mIdx;
    /* 0x1E */ u16 field_0x1E;
    /* 0x20 */ u8 mDirection;
    /* 0x21 */ u8 mIsClosed;

    void hermite(cXyz&, cXyz&, cXyz&, cXyz&, daNpcT_Hermite_c&, cXyz&);
    void initialize();
    int setPathInfo(u8, s8, u8);
    int setNextPathInfo(s8, u8);
    void reverse();
    void setNextIdx() { setNextIdx(getNumPnts()); }
    int setNextIdx(int);
    int getDstPos(cXyz i_pnt, cXyz* o_pos_p) {
        return getDstPos(i_pnt, o_pos_p, getNumPnts());
    }
    int getDstPos(cXyz, cXyz*, int);
    int getDstPosH(cXyz, cXyz*, int, int);
    int chkPassed1(cXyz, int);
    int chkPassed2(cXyz, cXyz*, int, int);
#if DEBUG
    int drawDbgInfo(f32, int);
#endif

    daNpcT_Path_c() {
        initialize();
    }

    virtual ~daNpcT_Path_c() {}

    Vec getPntPos(int i_idx) { return mPathInfo->m_points[i_idx].m_position; }

    int getArg0() { return mPathInfo->m_points[mIdx].mArg0; }
    int getArg1() { return mPathInfo->m_points[mIdx].mArg1; }
    int getArg2() { return mPathInfo->m_points[mIdx].mArg2; }

    int chkClose() {
        return dPath_ChkClose(mPathInfo);
    }

    bool chkReverse() { return mDirection == 1; }

    int getNumPnts() { return mPathInfo->m_num; }

    void setIdx(int i_idx) { mIdx = i_idx; }

    const int getIdx() { return mIdx; }

    dPath* getPathInfo() { return mPathInfo; }

    void onReverse() {
        mDirection = 1;
        field_0x1E = 1;
    }

    void offReverse() {
        mDirection = 0;
        field_0x1E = 1;
    }

    bool checkReverse() {
        return mDirection == 1;
    }

    BOOL chkNextId() { return mPathInfo->m_nextID != 0xFFFF; }
};

class daNpcT_JntAnm_c {
private:
    /* 0x000 */ daNpcT_ActorMngr_c mActrMngr;
    /* 0x008 */ cXyz mPos;
    /* 0x014 */ cXyz mAttnPos;
    /* 0x020 */ cXyz* mAttnPosP;
    /* 0x024 */ cXyz field_0x24;
    /* 0x030 */ cXyz mJntPos[3];
    /* 0x054 */ cXyz field_0x54[3];
    /* 0x078 */ cXyz mMinRad[3];
    /* 0x09C */ cXyz mMaxRad[3];
    /* 0x0C0 */ cXyz mRad[3];
    /* 0x0E4 */ cXyz field_0xe4[3];
    /* 0x108 */ cXyz mEyePos;
    /* 0x114 */ cXyz mHeadPos;
    /* 0x120 */ cXyz field_0x120;
    /* 0x12C */ csXyz mEyeAngle;
    /* 0x132 */ csXyz field_0x132;
    /* 0x138 */ int mMode;
    /* 0x13C */ int field_0x13c;
    /* 0x140 */ int mBaseTimer;
    /* 0x144 */ int mTimer;
    /* 0x148 */ f32 field_0x148;
    /* 0x14C */ f32 field_0x14c;
    /* 0x150 */ csXyz field_0x150;
    /* 0x156 */ u8 field_0x156;
    /* 0x157 */ u8 mDirectFlag;
    /* 0x158 */ u8 field_0x158;

public:
    void initialize();
    void setParam(fopAc_ac_c*, J3DModel*, cXyz*, int, int, int, f32, f32, f32, f32,
                                 f32, f32, f32, f32, f32, f32, cXyz*);
    void calc(f32);
    void calcJntRad(f32, f32, f32);
#if DEBUG
    void drawDbgInfo();
#endif

    void setEyeAngleX(cXyz param_1, f32 param_2, s16 param_3) {
        cXyz unusedXyz;
        f32 param_2_recip = 0.0f;
        s16 sVar3 = 0;
        if (mAttnPosP != NULL) {
            cXyz cStack_50 = *mAttnPosP - param_1;
            sVar3 = -cM_atan2s(cStack_50.y,  cStack_50.absXZ());
            sVar3 += param_3;
        }
        sVar3 += field_0x150.x;
        param_2_recip = 1.0f / param_2;
        mEyeAngle.x = field_0x132.x * (1.0f - param_2_recip) +
                      sVar3 * param_2_recip;
    }

    void setEyeAngleY(cXyz param_1, s16 param_2, BOOL param_3, f32 param_4, s16 param_5) {
        f32 param_4_recip = 0.0f;
        s16 sVar3 = 0;
        if (mAttnPosP != NULL) {
            cXyz cStack_50 = *mAttnPosP - param_1;
            sVar3 += cM_atan2s(cStack_50.x,  cStack_50.z);
            sVar3 -= param_2;
            sVar3 -= (s16)(field_0x150.y - param_2);
            sVar3 += param_5;
        }
        if (param_3) {
            sVar3 = sVar3 * -1;
        }
        param_4_recip = 1.0f / param_4;
        mEyeAngle.y = field_0x132.y * (1.0f - param_4_recip) +
                      sVar3 * param_4_recip;
    }

    void setEyeAngleY(s16 param_1, f32 param_2) {
        mEyeAngle.y = field_0x132.y * (1.0f - 1.0f / param_2) +
        param_1 * (1.0f / param_2);
    }

    s16 getEyeAngleX() { return mEyeAngle.x; }
    s16 getEyeAngleY() { return mEyeAngle.y; }

    f32 getJntRadX(int param_0) {
        f32 ret = 0.0f;
        ret = field_0xe4[2].x;
        if (param_0 == 0) {
            ret += field_0xe4[0].x * (1.0f - field_0x148);
        } else if (param_0 == 1) {
            ret += field_0xe4[0].x * field_0x148;
        }
        return ret;
    }

    f32 getJntRadY(int param_0) {
        f32 ret = 0.0f;
        ret = field_0xe4[2].y;
        if (param_0 == 0) {
            ret += field_0xe4[0].y * (1.0f - field_0x148);
        } else if (param_0 == 1) {
            ret += field_0xe4[0].y * field_0x148;
        }
        return ret;
    }

    s16 getJntRadX2S(int param_0) {
        return cM_rad2s(getJntRadX(param_0));
    }

    s16 getJntRadY2S(int param_0) {
        return cM_rad2s(getJntRadY(param_0));
    }

    void clrRad() {
        for (int i = 0; i < 3; i++) {
            mRad[i].setall(0.0f);
        }
    }

    int getTime() {
        return cLib_getRndValue(mBaseTimer / 2, mBaseTimer);
    }

    daNpcT_JntAnm_c() {
        initialize();
    }
    virtual ~daNpcT_JntAnm_c() {}

    void clrDirectFlag() { mDirectFlag = 0; }
    int getMode() { return mMode; }

    int setMode(int mode, BOOL param_2) {
        if (mode >= 0 && (param_2 || mode != mMode)) {
            mActrMngr.remove();
            mAttnPosP = NULL;
            field_0x14c = 0.0f;
            mDirectFlag = 0;
            mMode = mode;
            return 1;
        }
        return 0;
    }

    void setDirect(u8 isDirect) {
        if (isDirect != 0) {
            mEyeAngle.x = 0;
            mEyeAngle.y = 0;
        }
        mDirectFlag = isDirect;
    }

    enum LookMode {
        /* 0 */ LOOK_NONE,
        /* 1 */ LOOK_PLAYER,
        /* 2 */ LOOK_ACTOR,
        /* 3 */ LOOK_POS,
        /* 4 */ LOOK_CAMERA,
        /* 5 */ LOOK_MODE_5,
        /* 6 */ LOOK_MODE_6,
        /* 7 */ LOOK_MODE_7,
        /* 8 */ LOOK_MODE_8,
    };

    void lookNone(u8 isDirect) {
        setMode(LOOK_NONE, FALSE);
        setDirect(isDirect);
    }

    void lookPlayer(u8 isDirect) {
        setMode(LOOK_PLAYER, FALSE);
        setDirect(isDirect);
    }

    void lookCamera(u8 isDirect) {
        setMode(LOOK_CAMERA, FALSE);
        setDirect(isDirect);
    }

    void lookActor(fopAc_ac_c* param_1, f32 param_2, u8 isDirect) {
        bool isDifferentActor = mActrMngr.getActorP() != param_1;
        if (setMode(LOOK_ACTOR, isDifferentActor) && fopAcM_IsActor(param_1)) {
            mActrMngr.entry(param_1);
            field_0x14c = param_2;
        }
        setDirect(isDirect);
    }

    void lookPos(cXyz* i_pos, u8 i_direct) {
        BOOL tmp = mAttnPosP != i_pos;
        if (setMode(LOOK_POS, tmp)) {
            mAttnPosP = i_pos;
        }
        setDirect(i_direct);
    }

    void lookAround(u8 i_direct, int i_baseTimer) {
        if (setMode(LOOK_MODE_7, FALSE)) {
            mTimer = 0;
        }

        mBaseTimer = i_baseTimer;
        setDirect(i_direct);
    }

    void lookAround2(u8 i_direct, int i_baseTimer) {
        if (setMode(LOOK_MODE_8, FALSE)) {
            mTimer = 0;
        }

        mBaseTimer = i_baseTimer;
        setDirect(i_direct);
    }

    void sorasu1(cXyz *arg0, u8 arg1) {
        BOOL diff_ptrs = mAttnPosP != arg0;
        if (setMode(5, diff_ptrs) != 0) {
            mAttnPosP = arg0;
        }

        setDirect(arg1);
    }

    void sorasu2(cXyz *arg0, u8 arg1) {
        BOOL diff_ptrs = mAttnPosP != arg0;
        if (setMode(6, diff_ptrs) != 0) {
            mAttnPosP = arg0;
        }

        setDirect(arg1);
    }
};

class daNpcT_DmgStagger_c {
public:
    void setParam(fopAc_ac_c*, fopAc_ac_c*, s16);
    void calc(BOOL);

    void initialize() {
        //TODO: Separate decl is fakematch, but using 0 directly in the setall call causes
        //      weird issues in some functions that call this.
        u8 zero = 0;
        for (int i = 0; i < 2; i++) {
            mAngle[i].setall(zero);
            mPower[i] = 0.0f;
        }
        mStagger = 0;
        field_0x16 = 0;
        mRebirth = 0;
    }

    int checkStagger() { return mStagger != 0; }

    s16 getAngleX(int idx) { return mAngle[idx].x; }
    s16 getAngleZ(int idx) { return mAngle[idx].z; }
    int checkRebirth() { return mRebirth; }
    void setPower(f32 power) {
        for (int i = 0; i < 2; i++) {
            mPower[i] = power;
        }
    }
    void setRebirth() {
        mRebirth = true;
    }

    /* 0x00 */ csXyz mAngle[2];
    /* 0x0C */ f32 mPower[2];
    /* 0x14 */ s16 mStagger;
    /* 0x16 */ s16 field_0x16;
    /* 0x18 */ bool mRebirth;
};

struct daNpcT_evtData_c {
    /* 0x0 */ const char* eventName;
    /* 0x4 */ int num;
};

struct daNpcT_faceMotionAnmData_c {
    /* 0x00 */ int mBckFileIdx;
    /* 0x04 */ int mBckAttr;
    /* 0x08 */ int mBckArcIdx;
    /* 0x0C */ int mBtpFileIdx;
    /* 0x10 */ int mBtpAttr;
    /* 0x14 */ int mBtpArcIdx;
    /* 0x18 */ BOOL field_0x18;
};

struct daNpcT_pntData_c {
    /* 0x0 */ int field_0x0;
    /* 0x4 */ int field_0x4;
};

struct daNpcT_motionAnmData_c {
    /* 0x00 */ int mBckFileIdx;
    /* 0x04 */ int mBckAttr;
    /* 0x08 */ int mBckArcIdx;
    /* 0x0C */ int mBtkFileIdx;
    /* 0x10 */ int mBtkAttr;
    /* 0x14 */ int mBtkArcIdx;
    /* 0x18 */ s16 field_0x18;
    /* 0x1A */ s16 field_0x1a;
};

class mDoExt_McaMorfSO;

class daNpcT_c : public fopAc_ac_c {
public:
    /* 0x568 */ daNpcT_faceMotionAnmData_c const* mpFaceMotionAnmData;
    /* 0x56C */ daNpcT_motionAnmData_c const* mpMotionAnmData;
    /* 0x570 */ daNpcT_evtData_c const* mpEvtData;
    /* 0x574 */ char** mpArcNames;
    /* 0x578 */ mDoExt_McaMorfSO* mpMorf[2];
    /* 0x580 */ Z2Creature mSound;
    /* 0x610 */ mDoExt_bckAnm mBckAnm;
    /* 0x62C */ mDoExt_btpAnm mBtpAnm;
    /* 0x644 */ mDoExt_btkAnm mBtkAnm;
    /* 0x65C */ mDoExt_brkAnm mBrkAnm;
    /* 0x674 */ mDoExt_bpkAnm mBpkAnm;
    /* 0x68C */ dBgS_ObjAcch mAcch;
    /* 0x864 */ dCcD_Stts mCcStts;
    /* 0x8A0 */ dBgS_AcchCir mAcchCir;
    /* 0x8E0 */ request_of_phase_process_class mPhase[10];
    /* 0x930 */ cBgS_GndChk mGndChk;
    /* 0x96C */ daNpcT_MatAnm_c* mpMatAnm[2];
    /* 0x974 */ dMsgFlow_c mFlow;
    /* 0x9C0 */ dPaPoT_c field_0x9c0;
    /* 0xA40 */ dCcD_Stts field_0xa40;
    /* 0xA7C */ s32 mFlowNodeNo;
    /* 0xA80 */ f32 mExpressionMorfFrame;
    /* 0xA84 */ f32 mMorfFrames;
    /* 0xA88 */ bool mCreating;
    /* 0xA89 */ u8 mTwilight;
    /* 0xA8C */ dBgS_GndChk field_0xa8c;
    /* 0xAE0 */ dBgS_LinChk mLinChk;
    /* 0xB50 */ daNpcT_MotionSeqMngr_c mFaceMotionSeqMngr;
    /* 0xB74 */ daNpcT_MotionSeqMngr_c mMotionSeqMngr;
    /* 0xB98 */ daNpcT_ActorMngr_c mPlayerActorMngr;
    /* 0xBA0 */ daNpcT_ActorMngr_c field_0xba0;
    /* 0xBA8 */ daNpcT_JntAnm_c mJntAnm;
    /* 0xD08 */ daNpcT_DmgStagger_c mStagger;
    /* 0xD24 */ cXyz mFootLPos;
    /* 0xD30 */ cXyz mFootRPos;
    /* 0xD3C */ cXyz mFootLOffset;
    /* 0xD48 */ cXyz mFootROffset;
    /* 0xD54 */ cXyz mOldFootLOffset;
    /* 0xD60 */ cXyz mOldFootROffset;
    /* 0xD6C */ cXyz field_0xd6c;
    /* 0xD78 */ csXyz mCurAngle;
    /* 0xD7E */ csXyz field_0xd7e;
    /* 0xD84 */ csXyz field_0xd84;
    /* 0xD8A */ csXyz field_0xd8a;
    /* 0xD90 */ fpc_ProcID mItemPartnerId;
    /* 0xD94 */ u32 mShadowKey;
    /* 0xD98 */ u32 mAnmFlags;
    /* 0xD9C */ fpc_ProcID mMsgId;
    /* 0xDA0 */ u32 mHitodamaPrtclKey[2];
    /* 0xDA8 */ u32 mPolSound;
    /* 0xDAC */ int mStaffId;
    /* 0xDB0 */ int mCutType;
    /* 0xDB4 */ int field_0xdb4;
    /* 0xDB8 */ int mDamageTimerStart;
    /* 0xDBC */ int mDamageTimer;
    /* 0xDC0 */ int mBtpPauseTimer;
    /* 0xDC4 */ int mEventTimer;
    /* 0xDC8 */ s16 mPlayerAngle;
    /* 0xDCA */ s16 mGroundAngle;
    /* 0xDCC */ s16 field_0xdcc;
    /* 0xDCE */ s16 mFootLPolyAngle;
    /* 0xDD0 */ s16 field_0xdd0;
    /* 0xDD2 */ s16 mFootRPolyAngle;
    /* 0xDD4 */ s16 mStartAngle;
    /* 0xDD6 */ s16 mTargetAngle;
    /* 0xDD8 */ s16 mStepMode;
    /* 0xDDA */ s16 mTurnAmount;
    /* 0xDDC */ f32 mTurnCount;
    /* 0xDE0 */ f32 field_0xde0;
    /* 0xDE4 */ f32 field_0xde4;
    /* 0xDE8 */ f32 mRealShadowSize;
    /* 0xDEC */ f32 mCylH;
    /* 0xDF0 */ f32 mWallR;
    /* 0xDF4 */ f32 mGroundH;
    /* 0xDF8 */ f32 mAttnFovY;
    /* 0xDFC */ f32 field_0xdfc;
    /* 0xE00 */ cXyz field_0xe00;
    /* 0xE0C */ cXyz field_0xe0c;
    /* 0xE18 */ s16 field_0xe18;
    /* 0xE1A */ s16 field_0xe1a;
    /* 0xE1C */ s16 mEvtId;
    /* 0xE1E */ u16 mMorfLoops;
    /* 0xE20 */ u16 mBckLoops;
    /* 0xE22 */ u16 mMode;
    /* 0xE24 */ s8 mReverb;
    /* 0xE25 */ u8 mHide;
    /* 0xE26 */ bool field_0xe26;
    /* 0xE27 */ u8 mFootLOnGround;
    /* 0xE28 */ u8 mFootROnGround;
    /* 0xE29 */ u8 field_0xe29;
    /* 0xE2A */ u8 field_0xe2a;
    /* 0xE2B */ bool mNoDraw;
    /* 0xE2C */ u8 field_0xe2c;
    /* 0xE2D */ bool field_0xe2d;
    /* 0xE2E */ u8 mPreItemNo;
    /* 0xE30 */ u16 mEvtNo;
    /* 0xE32 */ bool mSpeakEvent;
    /* 0xE33 */ bool field_0xe33;
    /* 0xE34 */ u8 field_0xe34;
    /* 0xE35 */ u8 field_0xe35;
    /* 0xE36 */ u8 field_0xe36;
    /* 0xE37 */ u8 field_0xe37;
    /* 0xE38 */ u8 field_0xe38;
    /* 0xE3C vtable */

public:
    daNpcT_c(daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
             daNpcT_motionAnmData_c const* i_motionAnmData,
             daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
             int i_faceMotionStepNum,
             daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData,
             int i_motionStepNum, daNpcT_evtData_c const* i_evtData, char** i_arcNames) :
        mpFaceMotionAnmData(i_faceMotionAnmData),
        mpMotionAnmData(i_motionAnmData),
        mpEvtData(i_evtData),
        mpArcNames(i_arcNames),
        mFaceMotionSeqMngr(i_faceMotionSequenceData, i_faceMotionStepNum),
        mMotionSeqMngr(i_motionSequenceData, i_motionStepNum) {
        // "daNpcT_c -> construct"
        OS_REPORT("|%06d:%x|daNpcT_c -> コンストラクト\n", g_Counter.mCounter0, this);
        initialize();
    }

    static void tgHitCallBack(fopAc_ac_c*, dCcD_GObjInf*, fopAc_ac_c*, dCcD_GObjInf*);
    static void* srchActor(void*, void*);
    J3DAnmTransform* getTrnsfrmAnmP(char const*, int);
    J3DAnmTransformKey* getTrnsfrmKeyAnmP(char const*, int);
    J3DAnmTexPattern* getTexPtrnAnmP(char const*, int);
    J3DAnmTextureSRTKey* getTexSRTKeyAnmP(char const*, int);
    J3DAnmTevRegKey* getTevRegKeyAnmP(char const*, int);
    J3DAnmColor* getColorAnmP(char const*, int);
    int setBckAnm(J3DAnmTransform*, f32, int, int, int, bool);
    int setMcaMorfAnm(J3DAnmTransformKey*, f32, f32, int, int, int);
    int setBtpAnm(J3DAnmTexPattern*, J3DModelData*, f32, int);
    int setBtkAnm(J3DAnmTextureSRTKey*, J3DModelData*, f32, int);
    int setBrkAnm(J3DAnmTevRegKey*, J3DModelData*, f32, int);
    int setBpkAnm(J3DAnmColor*, J3DModelData*, f32, int);
    int loadRes(s8 const*, char const**);
    void deleteRes(s8 const*, char const**);
    int execute();
    int draw(BOOL, BOOL, f32, _GXColorS10*, f32, BOOL, BOOL, BOOL);
    void setEnvTevColor();
    void setRoomNo();
    int checkEndAnm(f32);
    int checkEndAnm(J3DFrameCtrl*, f32);
    void playAllAnm();
    void setMtx();
    void ctrlFaceMotion();
    void ctrlMotion();
    int ctrlMsgAnm(int*, int*, fopAc_ac_c*, int);
    void evtChange();
    bool setFaceMotionAnm(int, bool);
    void setPos(cXyz);
    void setAngle(s16);
    void setAngle(csXyz);
    fopAc_ac_c* hitChk(dCcD_GObjInf*, u32);
    void setDamage(int, int, int);
    BOOL chkActorInSight(fopAc_ac_c*, f32, s16);
    BOOL chkPointInArea(cXyz, cXyz, f32, f32, f32, s16);
    BOOL chkPointInArea(cXyz, cXyz, cXyz, s16);
    BOOL chkFindActor(fopAc_ac_c*, BOOL, s16);
    BOOL chkWolfAction();
    BOOL chkFindWolf(s16, int, int, f32, f32, f32, f32, BOOL);
    BOOL srchPlayerActor();
    cXyz getAttnPos(fopAc_ac_c*);
    BOOL turn(s16, int, int);
    BOOL step(s16, int, int, int, int);
    void calcSpeedAndAngle(cXyz, BOOL, s16, s16);
    int getActorDistance(fopAc_ac_c*, int, int);
    BOOL initTalk(int, fopAc_ac_c**);
    BOOL talkProc(int*, BOOL, fopAc_ac_c**, BOOL);
    fopAc_ac_c* getNearestActorP(s16);
    fopAc_ac_c* getEvtAreaTagP(int, int);
    fopAc_ac_c* getShopItemTagP();
    void setHitodamaPrtcl();

    virtual ~daNpcT_c() {
        // "daNpcT_c -> destructed"
        OS_REPORT("|%06d:%x|daNpcT_c -> デストラクト\n", g_Counter.mCounter0, this);
    }
    virtual BOOL ctrlBtk();
    virtual void ctrlSubFaceMotion(int) {}
    virtual BOOL checkChangeJoint(int) { return FALSE; }
    virtual BOOL checkRemoveJoint(int) { return FALSE; }
    virtual s32 getBackboneJointNo() { return -1; }
    virtual s32 getNeckJointNo() { return -1; }
    virtual s32 getHeadJointNo() { return -1; }
    virtual s32 getFootLJointNo() { return -1; }
    virtual s32 getFootRJointNo() { return -1; }
    virtual u16 getEyeballLMaterialNo() { return 0; }
    virtual u16 getEyeballRMaterialNo() { return 0; }
    virtual u16 getEyeballMaterialNo() { return 0; }
    virtual int ctrlJoint(J3DJoint*, J3DModel*);
    virtual void afterJntAnm(int) {}
    virtual void setParam() {}
    virtual BOOL checkChangeEvt() { return FALSE; }
    virtual BOOL evtTalk() { return TRUE; }
    virtual BOOL evtEndProc() { return TRUE; }
    virtual BOOL evtCutProc() { return FALSE; }
    virtual void setAfterTalkMotion() {}
    virtual BOOL evtProc();
    virtual void action() {}
    virtual void beforeMove() {}
    virtual void afterMoved() {}
    virtual void setAttnPos() {}
    virtual void setFootPos();
    virtual void setCollision() {}
    virtual void setFootPrtcl(cXyz*, f32, f32);
    virtual bool checkCullDraw();
    virtual void twilight();
    virtual BOOL chkXYItems() { return FALSE; }
    virtual void evtOrder();
    virtual void decTmr() {
        if (mDamageTimer != 0) {
            mDamageTimer--;
        }
    }
    virtual void clrParam();
    virtual int drawDbgInfo() { return 0; }
    virtual void drawOtherMdl() {}
    virtual void drawGhost() {}
    virtual bool afterSetFaceMotionAnm(int, int, f32, int) { return true; }
    virtual bool afterSetMotionAnm(int, int, f32, int) { return true; }
    virtual daNpcT_faceMotionAnmData_c getFaceMotionAnm(daNpcT_faceMotionAnmData_c param_1) { return param_1; }
    virtual daNpcT_motionAnmData_c getMotionAnm(daNpcT_motionAnmData_c param_1) { return param_1; }
    virtual void changeAnm(int*, int*) {}
    virtual void changeBck(int*, int*) {}
    virtual void changeBtp(int*, int*) {}
    virtual void changeBtk(int*, int*) {}
    virtual bool setMotionAnm(int, f32, BOOL);

    bool checkHide() { return mHide || (!dComIfGs_wolfeye_effect_check() && mTwilight); }
    BOOL checkStep() { return mStepMode == 1; }
    void setCommander(fopAc_ac_c* param_0) { field_0xba0.entry(param_0); }
    void setCutType(int i_cutType) { mCutType = i_cutType; }
    void onHide() { mHide = true; }
    void offHide() { mHide = false; }
    void hide() { onHide(); }
    void show() { offHide(); }

    void initialize() {
        memset(&mFootLPos, 0, (u8*)&field_0xe38 - (u8*)&mFootLPos);
        mFaceMotionSeqMngr.initialize();
        mMotionSeqMngr.initialize();
        mPlayerActorMngr.initialize();
        field_0xba0.initialize();
        mJntAnm.initialize();
        mStagger.initialize();
        mItemPartnerId = fpcM_ERROR_PROCESS_ID_e;
        field_0xe26 = true;
        field_0xe1a = cM_rndF(65536.0f);
        field_0xde0 = 0.2f;
        field_0xde4 = 0.2f;
    }

    BOOL chkFindPlayer(BOOL param_0, s16 param_1) {
        return chkFindActor(daPy_getPlayerActorClass(), param_0, param_1);
    }

    static dCcD_SrcGObjInf const mCcDObjData;
    static dCcD_SrcCyl mCcDCyl;
    static dCcD_SrcSph mCcDSph;
    static fopAc_ac_c* mFindActorPtrs[50];
    static s16 mSrchName;
    static int mFindCount;

    enum Mode {
        /*  0 */ MODE_ENTER,
        /*  1 */ MODE_INIT,
        /*  2 */ MODE_RUN,
        /*  3 */ MODE_EXIT,
        /*  4 */ MODE_4,
        /* -1 */ MODE_INVALID = -1,
    };

    enum AnmFlags {
        ANM_PAUSE_MORF = 0x1,
        ANM_PAUSE_BTK = 0x2,
        ANM_PAUSE_BRK = 0x4,
        ANM_PAUSE_BPK = 0x8,
        ANM_PAUSE_BCK = 0x10,
        ANM_PAUSE_BTP = 0x20,
        ANM_PLAY_MORF = 0x40,
        ANM_PLAY_BTK = 0x80,
        ANM_PLAY_BRK = 0x100,
        ANM_PLAY_BPK = 0x200,
        ANM_PLAY_BCK = 0x400,
        ANM_PLAY_BTP = 0x800,
        ANM_FLAG_1000 = 0x1000,
        ANM_PAUSE_TIMER_BTP = 0x2000,
        ANM_FLAG_4000 = 0x4000,
    };
};

STATIC_ASSERT(sizeof(daNpcT_c) == 0xE40);

#define daNpcT_ct(ptr, ClassName, faceMotionAnmData, motionAnmData, faceMotionSequenceData, faceMotionStepNum, motionSequenceData, motionStepNum, evtData, arcNames) \
    if (!fopAcM_CheckCondition(ptr, fopAcCnd_INIT_e)) { \
        new (ptr) ClassName(faceMotionAnmData, motionAnmData, faceMotionSequenceData, faceMotionStepNum, motionSequenceData, motionStepNum, evtData, arcNames); \
        fopAcM_OnCondition(ptr, fopAcCnd_INIT_e); \
    }

BOOL daNpcT_chkEvtBit(u32 i_idx);
BOOL daNpcT_chkPointInArea(cXyz param_0, cXyz param_1, cXyz param_2, s16 param_3, BOOL param_4);
u8 daNpcT_getDistTableIdx(int param_0, int param_1);
BOOL daNpcT_chkDoBtnIsSpeak(fopAc_ac_c* i_ActorP);
void daNpcT_offTmpBit(u32 i_idx);
void daNpcT_onTmpBit(u32 i_idx);
BOOL daNpcT_chkTmpBit(u32 i_idx);
void daNpcT_onEvtBit(u32 i_idx);
void daNpcT_offEvtBit(u32 i_idx);
BOOL daNpcT_chkTmpBit(u32 i_idx);
BOOL daNpcT_getPlayerInfoFromPlayerList(int param_0, int i_roomNo, cXyz* o_spawnPos,
                                        csXyz* o_angle);
int daNpcT_judgeRace(dPath* i_path, fopAc_ac_c** param_1, daNpcT_pntData_c* i_pntData, int param_3, int* param_4);
BOOL daNpcT_chkActorInScreen(fopAc_ac_c* i_ActorP, f32 param_1, f32 param_2, f32 param_3,
f32 param_4, f32 param_5, f32 param_6, f32 param_7, int param_8);

struct daNpcT_HIOParam {
    /* 0x00 */ f32 attention_offset;
    /* 0x04 */ f32 gravity;
    /* 0x08 */ f32 scale;
    /* 0x0C */ f32 real_shadow_size;
    /* 0x10 */ f32 weight;
    /* 0x14 */ f32 height;
    /* 0x18 */ f32 knee_length;
    /* 0x1C */ f32 width;
    /* 0x20 */ f32 body_angleX_max;
    /* 0x24 */ f32 body_angleX_min;
    /* 0x28 */ f32 body_angleY_max;
    /* 0x2C */ f32 body_angleY_min;
    /* 0x30 */ f32 head_angleX_max;
    /* 0x34 */ f32 head_angleX_min;
    /* 0x38 */ f32 head_angleY_max;
    /* 0x3C */ f32 head_angleY_min;
    /* 0x40 */ f32 neck_rotation_ratio;
    /* 0x44 */ f32 morf_frame;
    /* 0x48 */ s16 talk_distance;
    /* 0x4A */ s16 talk_angle;
    /* 0x4C */ s16 attention_distance;
    /* 0x4E */ s16 attention_angle;
    /* 0x50 */ f32 fov;
    /* 0x54 */ f32 search_distance;
    /* 0x58 */ f32 search_height;
    /* 0x5C */ f32 search_depth;
    /* 0x60 */ s16 attention_time;
    /* 0x62 */ s16 damage_time;
    /* 0x64 */ s16 face_expression;
    /* 0x66 */ s16 motion;
    /* 0x68 */ s16 look_mode;
    /* 0x6A */ u8 debug_mode_ON;
    /* 0x6B */ u8 debug_info_ON;
    /* 0x6C */ f32 expression_morf_frame;
    /* 0x70 */ f32 box_min_x;
    /* 0x74 */ f32 box_min_y;
    /* 0x78 */ f32 box_min_z;
    /* 0x7C */ f32 box_max_x;
    /* 0x80 */ f32 box_max_y;
    /* 0x84 */ f32 box_max_z;
    /* 0x88 */ f32 box_offset;
};

void daNpcT_cmnGenMessage(JORMContext*, daNpcT_HIOParam* param_1);

void daNpcT_cmnListenPropertyEvent(char*, int*, daNpcT_HIOParam*);

#endif /* D_A_D_A_NPC_H */
