#ifndef D_A_NPC4
#define D_A_NPC4

#include "d/actor/d_a_player.h"
#include "d/d_msg_flow.h"
#include "d/d_bg_s_gnd_chk.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_path.h"
#include "f_op/f_op_actor.h"
#include "f_pc/f_pc_base.h"
#include "m_Do/m_Do_ext.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"

class daNpcF_ActorMngr_c {
private:
    /* 0x0 */ fpc_ProcID mActorID;

public:
    daNpcF_ActorMngr_c() { initialize(); }

    void initialize();
    void entry(unsigned int);
    void entry(fopAc_ac_c*);
    void remove();
    fopAc_ac_c* getActorP();

    fpc_ProcID getPId() { return mActorID; }

    virtual ~daNpcF_ActorMngr_c() {}
};

BOOL daNpcF_chkEvtBit(u32 i_idx);
void daNpcF_onEvtBit(u32 i_idx);
BOOL daNpcF_chkTmpBit(u32 i_idx);
void daNpcF_onTmpBit(u32 i_idx);
void daNpcF_offTmpBit(u32 i_idx);
u8 daNpcF_getDistTableIdx(int param_0, int param_1);
#if DEBUG
class daNpcF_HIOParam;
void daNpcF_commonListenPropertyEvent(char*, int*, daNpcF_HIOParam*);
void daNpcF_commonGenMessage(JORMContext*, daNpcF_HIOParam*);
#endif
void daNpcF_clearMessageTmpBit();
BOOL daNpcF_chkPointInArea(cXyz, cXyz, cXyz, s16);
int daNpcF_getPlayerInfoFromPlayerList(int param_0, int i_roomNo, cXyz& param_2, csXyz& param_3);
s16 daNpcF_getGroundAngle(cBgS_PolyInfo*, s16);
BOOL daNpcF_pntVsLineSegmentLengthSquare2D(f32 i_pntX, f32 i_pntZ, f32 i_startX, f32 i_startZ,
                                           f32 i_endX, f32 i_endZ, f32* o_projX, f32* o_projZ,
                                           f32* o_dist2);
BOOL daNpcF_chkPassed(cXyz i_pos, dPnt* i_points, u16 i_idx, u16 i_num, BOOL i_isClosed,
                      BOOL i_isReversed);
BOOL daNpcF_chkDoBtnEqSpeak(fopAc_ac_c* i_actor_p);

class daNpcF_c : public fopAc_ac_c {
protected:
    /* 0x568 */ mDoExt_McaMorfSO* mAnm_p;
    /* 0x56C */ mDoExt_bckAnm mBckAnm;
    /* 0x588 */ mDoExt_btpAnm mBtpAnm;
    /* 0x5A0 */ mDoExt_btkAnm mBtkAnm;
    /* 0x5B8 */ mDoExt_brkAnm mBrkAnm;
    /* 0x5D0 */ dBgS_ObjAcch mAcch;
    /* 0x7A8 */ dCcD_Stts mCcStts;
    /* 0x7E4 */ dBgS_AcchCir mAcchCir;
    /* 0x824 */ daNpcF_ActorMngr_c field_0x824;
    /* 0x82C */ daNpcF_ActorMngr_c mAttnActor[5];
    /* 0x854 */ cXyz mLookatPos[3];
    /* 0x878 */ cXyz mLookPos;
    /* 0x884 */ cXyz field_0x884;
    /* 0x890 */ cXyz field_0x890;
    /* 0x89C */ cXyz field_0x89c;
    /* 0x8A8 */ cXyz field_0x8a8;
    /* 0x8B4 */ cXyz field_0x8b4;
    /* 0x8C0 */ cXyz field_0x8c0;
    /* 0x8CC */ cXyz field_0x8cc;
    /* 0x8D8 */ cXyz field_0x8d8;
    /* 0x8E4 */ cXyz mHeadPos;
    /* 0x8F0 */ csXyz mCurAngle;
    /* 0x8F6 */ csXyz mOldAngle;
    /* 0x8FC */ csXyz mEyeAngle;
    /* 0x902 */ csXyz mHeadAngle;
    /* 0x908 */ csXyz field_0x908[3];
    /* 0x91A */ csXyz mLookatAngle[3];
    /* 0x92C */ int mStaffID;
    /* 0x930 */ int mCutType;
    /* 0x934 */ int mAttnIdx;
    /* 0x938 */ int mAttnChangeTimer;
    /* 0x93C */ int mAttnActorTimer[5];
    /* 0x950 */ int mMsgTimer;
    /* 0x954 */ int mDamageTimer;
    /* 0x958 */ int mTotalDamageTimer;
    /* 0x95C */ int field_0x95c;
    /* 0x960 */ int mEventTimer;
    /* 0x964 */ int mBtpTimer;
    /* 0x968 */ int mTurnAmount;
    /* 0x96C */ int mTurnMode;
    /* 0x970 */ f32 mTurnStepNum;
    /* 0x974 */ f32 mExpressionMorfOverride;
    /* 0x978 */ f32 mExpressionMorf;
    /* 0x97C */ f32 mMotionMorfOverride;
    /* 0x980 */ f32 mGroundH;
    /* 0x984 */ f32 field_0x984[3];
    /* 0x990 */ s16 field_0x990;
    /* 0x992 */ s16 field_0x992;
    /* 0x994 */ s16 mTurnStartAngle;
    /* 0x996 */ s16 mTurnTargetAngle;
    /* 0x998 */ s16 field_0x998;
    /* 0x99C */ u32 mAnmFlags;
    /* 0x9A0 */ u32 mShadowKey;
    /* 0x9A4 */ u32 field_0x9a4;
    /* 0x9A8 */ u32 mHitodamaParticleKey[2];
    /* 0x9B0 */ u32 mPolySound;
    /* 0x9B4 */ int mFlowNodeNo;
    /* 0x9B8 */ cXyz field_0x9b8;
    /* 0x9C4 */ cXyz field_0x9c4;
    /* 0x9D0 */ s16 field_0x9d0;
    /* 0x9D2 */ s16 field_0x9d2;
    /* 0x9D4 */ s16 mEventIdx;
    /* 0x9D6 */ s16 mExpressionPhase;
    /* 0x9D8 */ s16 mExpressionPrevPhase;
    /* 0x9DA */ s16 mMotionPhase;
    /* 0x9DC */ s16 mMotionPrevPhase;
    /* 0x9DE */ s16 mExpression;
    /* 0x9E0 */ s16 mMotion;
    /* 0x9E2 */ u16 mMotionLoops;
    /* 0x9E4 */ u16 mExpressionLoops;
    /* 0x9E6 */ u16 mOrderEvtNo;
    /* 0x9E8 */ s8 mReverb;
    /* 0x9E9 */ bool mOrderSpeakEvt;
    /* 0x9EA */ bool field_0x9ea;
    /* 0x9EB */ bool field_0x9eb;
    /* 0x9EC */ bool field_0x9ec;
    /* 0x9ED */ bool mOrderNewEvt;
    /* 0x9EE */ bool field_0x9ee;
    /* 0x9EF */ u8 field_0x9ef;
    /* 0x9F0 */ bool mIsDamaged;
    /* 0x9F1 */ u8 field_0x9f1;
    /* 0x9F2 */ bool mHide;
    /* 0x9F3 */ u8 field_0x9f3;
    /* 0x9F4 */ u8 mTwilight;
    /* 0x9F5 */ u8 field_0x9f5;
    /* 0x9F6 */ u8 field_0x9f6;
    /* 0x9F8 */ dMsgFlow_c mFlow;
    /* 0xA44 */ cBgS_GndChk mGndChk;
    /* 0xA80 */ dBgS_GndChk field_0xa80;
    /* 0xAD4 */ dBgS_LinChk field_0xad4;

public:
    struct daNpcF_anmPlayData {
        u16 idx;
        f32 morf;
        int numLoops;
    };

    enum AnmFlags {
        ANM_PAUSE_MORF = 0x0001,
        ANM_PAUSE_BTK = 0x0002,
        ANM_PAUSE_BRK = 0x0004,
        ANM_PLAY_MORF = 0x0008,
        ANM_PLAY_BTK = 0x0010,
        ANM_PLAY_BRK = 0x0020,
        ANM_PAUSE_BCK = 0x0040,
        ANM_PAUSE_BTP = 0x0080,
        ANM_PLAY_BCK = 0x0100,
        ANM_PLAY_BTP = 0x0200,
        ANM_FLAG_400 = 0x0400,
        ANM_FLAG_800 = 0x0800,
        ANM_PAUSE_EXPRESSION = 0x1000,
        ANM_PAUSE_BPK = 0x2000,
        ANM_PLAY_BPK = 0x4000,
        ANM_MOTION_FLAGS = ANM_PAUSE_MORF | ANM_PAUSE_BTK | ANM_PAUSE_BRK | ANM_PLAY_MORF |
                           ANM_PLAY_BTK | ANM_PLAY_BRK,
        ANM_EXPRESSION_FLAGS = ANM_PAUSE_BCK | ANM_PAUSE_BTP | ANM_PLAY_BCK | ANM_PLAY_BTP |
                               ANM_FLAG_400 | ANM_FLAG_800 | ANM_PAUSE_EXPRESSION,
        ANM_PAUSE_ALL = ANM_PAUSE_MORF | ANM_PAUSE_BTK | ANM_PAUSE_BRK | ANM_PAUSE_BCK |
                        ANM_PAUSE_BTP | ANM_PAUSE_EXPRESSION,
    };

    enum LookMode {
        /* 0 */ LOOK_NONE,
        /* 1 */ LOOK_RESET,
        /* 2 */ LOOK_PLAYER,
        /* 3 */ LOOK_PLAYER_TALK,
        /* 4 */ LOOK_ACTOR,
        /* 5 */ LOOK_ATTN,
    };

    daNpcF_c() {
        FORCE_DONT_INLINE;
        initialize();
    }
    BOOL execute();
    int draw(BOOL, BOOL, f32, _GXColorS10*, BOOL);
    static void tgHitCallBack(fopAc_ac_c*, dCcD_GObjInf*, fopAc_ac_c*,
                                             dCcD_GObjInf*);
    static void* srchAttnActor1(void*, void*);
    static void* srchActor(void*, void*);

    void initialize();
    J3DAnmTransformKey* getTrnsfrmKeyAnmP(char*, int);
    J3DAnmTexPattern* getTexPtrnAnmP(char*, int);
    J3DAnmTextureSRTKey* getTexSRTKeyAnmP(char*, int);
    J3DAnmTevRegKey* getTevRegKeyAnmP(char*, int);
    BOOL setMcaMorfAnm(J3DAnmTransformKey* i_anm, f32 i_rate, f32 i_morf, int i_attr,
                                      int i_start, int i_end);
    BOOL setBckAnm(J3DAnmTransform* i_bck, f32 i_rate, int i_attr, int i_start,
                                  int i_end, bool i_modify);
    BOOL setBtpAnm(J3DAnmTexPattern* i_btp, J3DModelData* i_modelData, f32 i_rate,
                                  int i_attr);
    BOOL setBtkAnm(J3DAnmTextureSRTKey* i_btk, J3DModelData* i_modelData, f32 i_rate,
                                  int i_attr);
    BOOL setBrkAnm(J3DAnmTevRegKey* i_brk, J3DModelData* i_modelData, f32 i_rate,
                                  int i_attr);
    void setEnvTevColor();
    void setRoomNo();
    BOOL chkEndAnm(f32);
    BOOL chkEndAnm(J3DFrameCtrl*, f32);
    BOOL playAllAnm();
    void playExpressionAnm(daNpcF_c::daNpcF_anmPlayData***);
    void playMotionAnm(daNpcF_c::daNpcF_anmPlayData***);
    void setLookatMtx(int, int*, f32);
    BOOL hitChk2(dCcD_Cyl*, BOOL, BOOL);
    void setDamage(int, int, int);
    int ctrlMsgAnm(int&, int&, fopAc_ac_c*, BOOL);
    void orderEvent(int, char*, u16, u16, u8, u16);
    void changeEvent(char*, char*, u16, u16);
    BOOL chkActorInSight(fopAc_ac_c*, f32);
    BOOL chkActorInArea(fopAc_ac_c*, cXyz, cXyz, s16);
    BOOL chkActorInAttnArea(fopAc_ac_c*, fopAc_ac_c*, int);
    BOOL initTalk(int, fopAc_ac_c**);
    BOOL talkProc(int*, BOOL, fopAc_ac_c**);
    BOOL turn(s16, f32, int);
    BOOL step(s16, int, int, int);
    void setAngle(s16);
    u8 getDistTableIdx(int, int);
    fopAc_ac_c* getEvtAreaTagP(int, int);
    fopAc_ac_c* getAttnActorP(BOOL, fpcLyIt_JudgeFunc, f32, f32, f32, f32, s16,
                                             int, BOOL);
    BOOL chkActorInSight2(fopAc_ac_c*, f32, s16);
    BOOL chkPointInArea(cXyz, cXyz, f32, f32, f32, s16);
    BOOL chkPointInArea(cXyz, cXyz, cXyz, s16);
    cXyz getAttentionPos(fopAc_ac_c*);
    BOOL chkFindPlayer2(BOOL, s16);
    void setHitodamaPrtcl();

    virtual ~daNpcF_c() {}
    virtual void setParam() {}
    virtual BOOL main() { return TRUE; }
    virtual BOOL ctrlBtk() { return FALSE; }
    virtual void adjustShapeAngle() {}
    virtual void setMtx();
    virtual void setMtx2();
    virtual void setAttnPos() {}
    virtual void setCollisions() {}
    virtual bool setExpressionAnm(int i_idx, bool i_modify) { return true; }
    virtual bool setExpressionBtp(int i_idx) { return true; }
    virtual void setExpression(int i_expression, f32 i_morf) {}
    virtual void setMotionAnm(int i_idx, f32 i_morf) {}
    virtual void setMotion(int i_motion, f32 i_morf, int i_restart) {}
    virtual BOOL drawDbgInfo() { return TRUE; }
    virtual void drawOtherMdls() {}

    BOOL chkActorInSpeakArea(fopAc_ac_c* i_actorCheck, fopAc_ac_c* i_actorArea) {
        return chkActorInAttnArea(i_actorCheck, i_actorArea, attention_info.distances[fopAc_attn_SPEAK_e]);
    }
    BOOL chkPlayerInSpeakArea(fopAc_ac_c* i_actor) {
        return chkActorInSpeakArea(daPy_getPlayerActorClass(), i_actor);
    }
    BOOL chkActorInTalkArea(fopAc_ac_c* i_actorCheck, fopAc_ac_c* i_actorArea) {
        return chkActorInAttnArea(i_actorCheck, i_actorArea, attention_info.distances[fopAc_attn_TALK_e]);
    }
    BOOL chkPlayerInTalkArea(fopAc_ac_c* i_actor) {
        return chkActorInTalkArea(daPy_getPlayerActorClass(), i_actor);
    }
    BOOL checkHide() { return mHide || (mTwilight && !dComIfGs_wolfeye_effect_check()); }
    void setIntDemander(fopAc_ac_c* i_actor) { field_0x824.entry(i_actor); }
    void setIntFlowNodeNo(int i_flowNodeNo) { mFlowNodeNo = i_flowNodeNo; }
    void setCutType(int i_cutType) { mCutType = i_cutType; }
    void onInterrupt(u8 param_0) { field_0x9ef = param_0; }
    void onHide() { mHide = true; }
    void offHide() { mHide = false; }

    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;
    static dCcD_SrcSph mCcDSph;
    static fopAc_ac_c* mFindActorPList[100];
    static s32 mFindCount;
    static s16 mSrchActorName;
    static char mFileNameBuf[0x15];
};

STATIC_ASSERT(sizeof(daNpcF_c) == 0xB48);

class daNpcF_MatAnm_c : public J3DMaterialAnm {
private:
    /* 0x0F4 */ mutable f32 field_0xF4;
    /* 0x0F8 */ mutable f32 field_0xF8;
    /* 0x0FC */ f32 mNowOffsetX;
    /* 0x100 */ f32 mNowOffsetY;
    /* 0x104 */ u8 mEyeMoveFlag;
    /* 0x105 */ u8 mMorfFrm;

public:
    daNpcF_MatAnm_c() { initialize(); }
    void initialize();
    void calc(J3DMaterial*) const;
    ~daNpcF_MatAnm_c() {}
    void setNowOffsetX(float i_nowOffsetX) { mNowOffsetX = i_nowOffsetX; }
    void setNowOffsetY(float i_nowOffsetY) { mNowOffsetY = i_nowOffsetY; }
    void onEyeMoveFlag() { mEyeMoveFlag = 1; }
    void offEyeMoveFlag() { mEyeMoveFlag = 0; }
};

class daNpcF_SPCurve_c {
private:
    /* 0x00 */ u16 mNurbs;
    /* 0x02 */ u16 field_0x02;
    /* 0x04 */ u8 mIsReversed;
    /* 0x05 */ u8 mIsClosed;
    /* 0x08 */ dPnt mPoints[96];

public:
    void initialize(dPath*, int);

    daNpcF_SPCurve_c() {
        initialize(NULL, 0);
    }

    virtual ~daNpcF_SPCurve_c() {}
};  // Size: 0x60C

class daNpcF_Path_c {
protected:
    /* 0x00 */ u16 mIdx;
    /* 0x02 */ u8 mIsReversed;
    /* 0x03 */ u8 mIsClosed;
    /* 0x04 */ dPath* mPathInfo;
    /* 0x08 */ f32 mRange;
    /* 0x0C */ f32 mPosDst;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ cXyz mPosition;
    /* 0x20 */ daNpcF_SPCurve_c mSPCurve;

public:
    void initialize();
    int setPathInfo(u8, s8, u8);
    BOOL chkPassed(cXyz);
    BOOL chkPassedDst(cXyz);
    void reverse();
    BOOL setNextIdx();
    int getNextIdx();
    int getBeforeIdx();
    BOOL getBeforePos(cXyz&);
    BOOL getNextPos(cXyz&);
    BOOL getDstPos(cXyz, cXyz&);
    void setNextIdxDst(cXyz);

    daNpcF_Path_c() {
        initialize();
    }

    virtual ~daNpcF_Path_c() {}

    int getIdx() { return mIdx; };
    void setIdx(int i_idx) { mIdx = i_idx; }
    int getArg0() { return mPathInfo->m_points[mIdx].mArg0; }
    u8 getArg0(int i_idx) { return mPathInfo->m_points[i_idx].mArg0; }
    Vec getPntPos(int i_idx) { return mPathInfo->m_points[i_idx].m_position; }
    BOOL chkClose() { return dPath_ChkClose(mPathInfo); }
    BOOL chkReverse() { return mIsReversed == true; }
    void onReverse() { mIsReversed = true; }
    void offReverse() { mIsReversed = false; }
    dPath* getPathInfo() { return mPathInfo; }
    void setRange(f32 i_range) { mRange = i_range; }
    int getNumPnts() { return mPathInfo->m_num; }
#if DEBUG
    void drawDbgInfoXyz();
#endif
};  // Size: 0x630

class daNpcF_Lookat_c {
private:
    /* 0x00 */ cXyz mJointPos[4];
    /* 0x30 */ cXyz* mAttnPos_p;
    /* 0x34 */ csXyz mAngularMoveDis[4];
    /* 0x4C */ csXyz mMinAngle[4];
    /* 0x64 */ csXyz mMaxAngle[4];
    /* 0x7C */ csXyz mRotAngle[4];
    /* 0x94 */ bool field_0x94[4];
    /* 0x98 vtable */

public:
    void initialize();
    void setParam(f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, s16,
                                 cXyz*);
    void calc(fopAc_ac_c*, Mtx, csXyz**, BOOL, int, BOOL);
    void adjustMoveDisAngle(s16&, s16, s16, s16);
    void initCalc(fopAc_ac_c*, Mtx, cXyz*, csXyz*, f32*, cXyz&, BOOL);
    void update(cXyz*, csXyz*, f32*);
    void calcMoveDisAngle(int, cXyz*, csXyz*, cXyz, int, BOOL);
    void setRotAngle();
    void clrRotAngle();

    daNpcF_Lookat_c() { initialize(); }
    virtual ~daNpcF_Lookat_c() {}

    cXyz* getAttnPos() { return mAttnPos_p; }
    void setAttnPos(cXyz* i_attnPos) { mAttnPos_p = i_attnPos; }
};

class daNpcF_MoveBgActor_c : public daNpcF_c {
private:
public:
    ~daNpcF_MoveBgActor_c() {}
    virtual bool CreateHeap() { return true; }
    virtual bool Create() { return true; }
    virtual bool Execute(Mtx**) { return true; }
    virtual bool Draw() { return true; }
    virtual bool Delete() { return true; }
    virtual bool IsDelete() { return true; }
    virtual bool ToFore() { return true; }
    virtual bool ToBack() { return true; }
};

struct daNpcF_HIOParam {
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
};

#endif /* D_A_NPC4 */
