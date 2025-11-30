#ifndef D_A_NPC_KN_H
#define D_A_NPC_KN_H

#include "d/actor/d_a_npc.h"

struct daNpc_Kn_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ s16 alpha;
    /* 0x8E */ s16 attack_wait_time;
    /* 0x90 */ s16 followup_wait_time; // ?
    /* 0x94 */ f32 small_slide_distance;
    /* 0x98 */ f32 big_slide_distance;
    /* 0x9C */ f32 warp_distance;
    /* 0xA0 */ f32 attack_disappear_speed_h;  // ?
    /* 0xA4 */ f32 attack_disappear_speed_v;  // ?
    /* 0xA8 */ s16 rotation_num;
    /* 0xAA */ s16 rotation_speed;
    /* 0xAC */ s16 attack_start_min_time;
    /* 0xAE */ s16 attack_start_time_range;
    /* 0xB0 */ f32 move_speed;
};

class daNpc_Kn_Param_c {
public:
    virtual ~daNpc_Kn_Param_c() {}

    static const daNpc_Kn_HIOParam m;
};

#if DEBUG
class daNpc_Kn_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Kn_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daNpc_Kn_HIOParam m;
};

#define NPC_KN_HIO_CLASS daNpc_Kn_HIO_c
#else
#define NPC_KN_HIO_CLASS daNpc_Kn_Param_c
#endif

/**
 * @ingroup actors-npcs
 * @class daNpc_Kn_c
 * @brief Hero's Shade
 *
 * @details
 *
 */
class daNpc_Kn_c : public fopEn_enemy_c {
public: 
    typedef int (daNpc_Kn_c::*cutFunc)(int);
    typedef int (daNpc_Kn_c::*actionFunc)(void*);

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

    class daNpc_Kn_prtclMngr_c {
    public:
        /* 0x00 */ bool mpModel;
        /* 0x04 */ cXyz mPos;
        /* 0x10 */ csXyz mAngle;
        /* 0x18 */ cXyz mScale;
        /* 0x24 */ dPaPo_c mManager;
    };

public:
    /* 0x05AC */ const daNpcT_faceMotionAnmData_c* mpFaceMotionAnmData;
    /* 0x05B0 */ const daNpcT_motionAnmData_c* mpMotionAnmData;
    /* 0x05B4 */ const daNpcT_evtData_c* mpEventData;
    /* 0x05B8 */ char** mpArcNames;
    /* 0x05BC */ mDoExt_McaMorfSO* mpModelMorf[2];
    /* 0x05C4 */ Z2Creature mSound;
    /* 0x0654 */ mDoExt_bckAnm mBckAnm;
    /* 0x0670 */ mDoExt_btpAnm mBtpAnm;
    /* 0x0688 */ mDoExt_btkAnm mBtkAnm;
    /* 0x06A0 */ mDoExt_brkAnm mBrkAnm;
    /* 0x06B8 */ mDoExt_bpkAnm mBpkAnm;
    /* 0x06D0 */ dBgS_ObjAcch mAcch;
    /* 0x08A8 */ dCcD_Stts mCcStts;
    /* 0x08E4 */ dBgS_AcchCir mAcchCir;
    /* 0x0924 */ request_of_phase_process_class mPhase[8];
    /* 0x0964 */ cBgS_GndChk mGndChk;
    /* 0x09A0 */ daNpcT_MatAnm_c* mpMatAnm;
    /* 0x09A4 */ u8 field_0x9a4[4];
    /* 0x09A8 */ dMsgFlow_c mFlow;
    /* 0x09F4 */ dPaPoT_c field_0x9F4;
    /* 0x0A74 */ dCcD_Stts mCcStts2;
    /* 0x0AB0 */ u32 mFlowNodeNo;
    /* 0x0AB4 */ f32 mExpressionMorfFrame;
    /* 0x0AB8 */ f32 mMorfFrames;
    /* 0x0ABC */ u8 mCreating;
    /* 0x0ABD */ u8 mTwilight;
    /* 0x0AC0 */ dBgS_GndChk mGndChk2;
    /* 0x0B14 */ dBgS_LinChk mLinChk;
    /* 0x0B84 */ daNpcT_MotionSeqMngr_c mFaceMotionSeqMngr;
    /* 0x0BA8 */ daNpcT_MotionSeqMngr_c mMotionSeqMngr;
    /* 0x0BCC */ daNpcT_ActorMngr_c mActorMngr1;
    /* 0x0BD4 */ daNpcT_ActorMngr_c mActorMngr2;
    /* 0x0BDC */ daNpcT_JntAnm_c mJntAnm;
    /* 0x0D3C */ daNpcT_DmgStagger_c mDmgStagger;
    /* 0x0D58 */ cXyz mFootLPos;
    /* 0x0D64 */ cXyz mFootRPos;
    /* 0x0D70 */ cXyz mFootLOffset;
    /* 0x0D7C */ cXyz mFootROffset;
    /* 0x0D88 */ cXyz mOldFootLOffset;
    /* 0x0D94 */ cXyz mOldFootROffset;
    /* 0x0DA0 */ cXyz field_0xDA0;
    /* 0x0DAC */ csXyz mCurAngle;
    /* 0x0DB2 */ csXyz field_0xDB2;
    /* 0x0DB8 */ csXyz field_0xDB8;
    /* 0x0DBE */ csXyz field_0xDBE;
    /* 0x0DC4 */ fpc_ProcID mItemPartnerId;
    /* 0x0DC8 */ u32 mShadowKey;
    /* 0x0DCC */ u32 mAnmFlags;
    /* 0x0DD0 */ fpc_ProcID mMsgId;
    /* 0x0DD4 */ int mStaffId;
    /* 0x0DD8 */ int mCutType;
    /* 0x0DDC */ int field_0xddc;
    /* 0x0DE0 */ int mTimerStart;
    /* 0x0DE4 */ int mTimer;
    /* 0x0DE8 */ int mBtpPauseTimer;
    /* 0x0DEC */ int field_0xdec;
    /* 0x0DF0 */ s16 mGroundAngle;
    /* 0x0DF2 */ s16 field_0xdf2;
    /* 0x0DF4 */ s16 mFootLPolyAngle;
    /* 0x0DF5 */ u8 field_0xdf5[0x0DF8 - 0x0DF6];
    /* 0x0DF8 */ s16 mFootRPolyAngle;
    /* 0x0DFA */ s16 mStartAngle;
    /* 0x0DFC */ s16 mTargetAngle;
    /* 0x0DFE */ s16 mStepMode;
    /* 0x0E00 */ s16 mTurnAmount;
    /* 0x0E04 */ f32 mTurnCount;
    /* 0x0E08 */ u8 field_0xe08[0x0E0C - 0x0E08];
    /* 0x0E0C */ f32 field_0xe0c;
    /* 0x0E10 */ f32 mRadius;
    /* 0x0E14 */ f32 mGroundHeight;
    /* 0x0E18 */ u8 field_0xe18[0xe1c - 0xe18];
    /* 0x0E1C */ u32 mHitodamaPrtclKey[2];
    /* 0x0E24 */ s16 mEvtId;
    /* 0x0E26 */ u16 mMorfLoops;
    /* 0x0E28 */ u16 mBckLoops;
    /* 0x0E2A */ u16 mMode;
    /* 0x0E2C */ u8 field_0xe2c;
    /* 0x0E2D */ u8 field_0xe2d;
    /* 0x0E2E */ u8 mFootLOnGround;
    /* 0x0E2F */ u8 mFootROnGround;
    /* 0x0E30 */ u8 field_0xe30;
    /* 0x0E31 */ u8 field_0xe31;
    /* 0x0E32 */ u8 mNoDraw;
    /* 0x0E33 */ u8 field_0xe33;
    /* 0x0E34 */ u8 field_0xe34;
    /* 0x0E36 */ u16 mEvtNo;
    /* 0x0E38 */ u8 mSpeakEvent;
    /* 0x0E39 */ u8 field_0xe39;
    /* 0x0E3A */ u8 field_0xe3a;
    /* 0x0E3B */ u8 field_0xe3b;
    /* 0x0E3C */ int field_0xe3c;
    // /* 0x0E40 */ vtable

public:
    void initialize() {
        memset(&mFootLPos, 0, (int)&field_0xe3c - (int)&mFootLPos);

        mFaceMotionSeqMngr.initialize();
        mMotionSeqMngr.initialize();
        mActorMngr1.initialize();
        mActorMngr2.initialize();
        mJntAnm.initialize();
        mDmgStagger.initialize();
        mItemPartnerId = -1;
        field_0xe2d = 1;
        mPodAnmFlags = 0;
    }

    bool checkHide() {
        return field_0xe2c != 0 || (!dComIfGs_wolfeye_effect_check() && mTwilight != 0);
    }

    int getHeadJointNo() {
        return 4;
    }

    int getNeckJointNo() {
        return 3;
    }

    int getBackboneJointNo() {
        return 1;
    }

    bool checkChangeJoint(int i_jointNo) { return i_jointNo == 4; }
    bool checkRemoveJoint(int i_jointNo) { return i_jointNo == 5; }

    void setCutType(int i_type) { mCutType = i_type;}

    void setActionMode(s8 i_actionMode) { mActionMode = i_actionMode;}
    s8 getActionMode() { return mActionMode; }

    void setTalkFlag(u8 i_flag) { field_0x15bd = i_flag; }
    
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    u8 getType();
    u8 getDivideNo();
    u32 getFlowNodeNo();
    u8 getPath();
    BOOL isDelete();
    void resetCol();
    void reset();
    void resetType();
    void setParam();
    u32 setParamTeach03();
    u32 setParamTeach04();
    u32 setParamTeach05();
    u32 setParamTeach06();
    u32 setParamTeach07();
    void setAfterTalkMotion();
    void srchActors();
    bool evtTalk();
    BOOL evtCutProc();
    void action();
    void beforeMove();
    void afterMoved();
    void setAttnPos();
    void setCollision();
    void setCollisionSword();
    BOOL checkCollisionSword();
    void setCollisionTeach03();
    void setCollisionTeach04();
    void setCollisionTeach05();
    void setCollisionTeach06();
    void setCollisionTeach07();
    int drawDbgInfo();
    void drawOtherMdl();
    void drawGhost();
    int selectAction();
    void teach03_selectAction();
    void teach04_selectAction();
    void teach05_selectAction();
    void teach06_selectAction();
    void teach07_selectAction();
    BOOL chkAction(actionFunc);
    BOOL setAction(actionFunc);
    int wait(void*);
    int talk(void*);
    int test(void*);
    void setSceneChange(int);
    int ECut_noneEquipChangeTalk(int);
    int ECut_noneEquipChangeTalkStand(int);
    int ECut_largeDamageTalk(int);
    int teach02_start(void*);
    int teach03_attackWait(void*);
    int teach04_attackWait(void*);
    int teach04_headBreakWait(void*);
    int teach04_finishWait(void*);
    int teach05_surpriseAttackWait(void*);
    int teach06_superJumpWait(void*);
    int teach06_divideMove(void*);
    int teach06_waitDivide(void*);
    int teach06_superJumpWaitDivide(void*);
    int teach06_superJumpedDivide(void*);
    int teach06_warpDelete(void*);
    int teach07_superTurnAttackWait(void*);
    int teach07_divideMove(void*);
    int teach07_waitDivide(void*);
    int teach07_superTurnAttackWaitDivide(void*);
    int teach07_superTurnAttackedDivide(void*);
    int teach07_warpDelete(void*);
    int ECut_secondEncount(int);
    int ECut_thirdSkillExplain(int);
    int ECut_thirdSkillGet(int);
    int ECut_fourthSkillExplain(int);
    int ECut_fourthSkillGet(int);
    int ECut_fifthSkillExplain(int);
    int ECut_fifthSkillGet(int);
    int ECut_sixthSkillExplain(int);
    int ECut_sixthSkillGet(int);
    int ECut_seventhSkillExplain(int);
    int ECut_seventhSkillGet(int);
    int setPrtcl();
    int setSlipPrtcl();
    int setLandingPrtcl();
    void calcMagicBallPos();
    void setSwordChargePtcl();
    void calcSlip();
    void calcSwordAttackMove(int);
    void setSe();
    static void tgHitCallBack(fopAc_ac_c*, dCcD_GObjInf*, fopAc_ac_c*, dCcD_GObjInf*);
    J3DAnmTransform* getTrnsfrmAnmP(char const*, int);
    J3DAnmTransformKey* getTrnsfrmKeyAnmP(char const*, int);
    J3DAnmTexPattern* getTexPtrnAnmP(char const*, int);
    J3DAnmTextureSRTKey* getTexSRTKeyAnmP(char const*, int);
    BOOL setBckAnm(J3DAnmTransform*, f32, int, int, int, bool);
    int setMcaMorfAnm(J3DAnmTransformKey*, f32, f32, int, int, int);
    BOOL setBtpAnm(J3DAnmTexPattern*, J3DModelData*, f32, int);
    BOOL setBtkAnm(J3DAnmTextureSRTKey*, J3DModelData*, f32, int);
    int loadRes(s8 const*, char const**);
    void deleteRes(s8 const*, char const**);
    int execute();
    int draw(int, int, f32, GXColorS10*, f32, int, int, int);
    void setEnvTevColor();
    void setRoomNo();
    BOOL checkEndAnm(f32);
    BOOL checkEndAnm(J3DFrameCtrl*, f32);
    void playAllAnm();
    void setMtx();
    void ctrlFaceMotion();
    void ctrlMotion();
    int ctrlMsgAnm(int*, int*, fopAc_ac_c*, int);
    void evtChange();
    bool setFaceMotionAnm(int, bool);
    bool setMotionAnm(int, f32, int);
    void setPos(cXyz);
    void setAngle(s16);
    BOOL chkActorInSight(fopAc_ac_c*, f32, s16);
    BOOL chkPointInArea(cXyz, cXyz, f32, f32, f32, s16);
    BOOL chkPointInArea(cXyz, cXyz, cXyz, s16);
    int chkFindPlayer(int, s16);
    BOOL srchPlayerActor();
    cXyz getAttnPos(fopAc_ac_c*);
    BOOL turn(s16, int, int);
    int step(s16, int, int, int, int);
    int initTalk(int, fopAc_ac_c**);
    BOOL talkProc(int*, int, fopAc_ac_c**, int);
    void setHitodamaPrtcl();
    daNpc_Kn_c(daNpcT_faceMotionAnmData_c const* param_0,
                              daNpcT_motionAnmData_c const* param_1,
                              daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_2, int param_3,
                              daNpcT_MotionSeqMngr_c::sequenceStepData_c const* param_4, int param_5,
                              daNpcT_evtData_c const* param_6, char** param_7)
                        : mpFaceMotionAnmData(param_0), mpMotionAnmData(param_1), mFaceMotionSeqMngr(param_2, param_3),
                          mMotionSeqMngr(param_4, param_5), mpEventData(param_6), mpArcNames(param_7)
                    {
                        OS_REPORT("|%06d:%x|daNpc_Kn_c -> コンストラクト\n", g_Counter.mCounter0, this);
                        initialize();
                    }
    u32 setParamTeach01();
    void setCollisionTeach01();
    void teach01_selectAction();
    int teach01_start(void*);
    int teach01_attackWait(void*);
    int teach01_swordAttackWait(void*);
    int teach01_swordFinishWait(void*);
    int ctrlWarp();
    int ECut_firstEncount(int);
    int ECut_attackFailed(int);
    int ECut_firstSkillGet(int);
    u32 setParamTeach02();
    void setCollisionTeach02();
    void teach02_selectAction();
    int teach02_shieldBashWait(void*);
    int teach02_finishWait(void*);
    int teach02_shieldReflectWait(void*);
    int ECut_secondSkillExplain(int);
    int ECut_reflectExplain(int);
    int ECut_secondSkillGet(int);

    virtual int ctrlBtk();
    virtual int getFootLJointNo() { return -1; }
    virtual int getFootRJointNo() { return -1; }
    virtual u16 getEyeballLMaterialNo() { return 0; }
    virtual u16 getEyeballRMaterialNo() { return 0; }
    virtual u16 getEyeballMaterialNo() { return 0; }
    virtual int ctrlJoint(J3DJoint*, J3DModel*);
    virtual void afterJntAnm(int) {}
    virtual BOOL checkChangeEvt() { return FALSE; }
    virtual BOOL evtEndProc() { return TRUE; }
    virtual int evtProc();
    virtual void setFootPos();
    virtual void setFootPrtcl(cXyz*, f32);
    virtual bool checkCullDraw();
    virtual void twilight();
    virtual BOOL chkXYItems() { return FALSE; }
    virtual void evtOrder();
    virtual void decTmr();
    virtual void clrParam();
    virtual bool afterSetFaceMotionAnm(int, int, f32, int) { return true; }
    virtual daNpcT_faceMotionAnmData_c getFaceMotionAnm(daNpcT_faceMotionAnmData_c arg0) {
        return arg0;
    }
    virtual daNpcT_motionAnmData_c getMotionAnm(daNpcT_motionAnmData_c arg0) {
        return arg0;
    }
    virtual ~daNpc_Kn_c();
    virtual bool afterSetMotionAnm(int, int, f32, int);
    
    static const dCcD_SrcGObjInf mCcDObjData;
    static char* mCutNameList[21];
    static cutFunc mCutList[21];
    static dCcD_SrcCyl mCcDCyl;
    static dCcD_SrcSph mCcDSph;
    static s16 mSrchName;
    static fopAc_ac_c* mFindActorPtrs[50];
    static u8 mFindCount[4];

private:
    /* 0x0E44 */ J3DModel* mpPodModel;
    /* 0x0E48 */ mDoExt_invisibleModel mInvisModel;
    /* 0x0E50 */ dKy_tevstr_c mTevStr;
    /* 0x11D8 */ NPC_KN_HIO_CLASS* mpHIO;
    /* 0x11DC */ dCcD_Cyl mCylCc;
    /* 0x1318 */ dCcD_Sph mSphCc[2];
    /* 0x1588 */ mDoExt_bckAnm mPodBck;
    /* 0x15A4 */ u32 mPodAnmFlags;
    /* 0x15A8 */ u32 field_0x15A8;
    /* 0x15AC */ u8 mType;
    /* 0x15AD */ u8 mDivideNo;
    /* 0x15AE */ s8 mActionMode;
    /* 0x15AF */ u8 field_0x15af;
    /* 0x15B0 */ cXyz mTargetPos;
    /* 0x15BC */ u8 field_0x15bc;
    /* 0x15BD */ u8 field_0x15bd;
    /* 0x15BE */ u16 field_0x15be;
    /* 0x15C0 */ cXyz field_0x15c0;
    /* 0x15CC */ u8 field_0x15cc;
    /* 0x15CD */ u8 field_0x15cd;
    /* 0x15CE */ u8 field_0x15ce;
    /* 0x15CF */ u8 field_0x15cf;
    /* 0x15D0 */ s16 field_0x15d0;
    /* 0x15D4 */ int field_0x15d4[3];
    /* 0x15E0 */ daNpc_Kn_prtclMngr_c mParticleMngr[3];
    /* 0x16F4 */ cXyz field_0x16f4;
    /* 0x1700 */ cXyz field_0x1700;
    /* 0x170C */ u8 field_0x170c;
    /* 0x170D */ u8 field_0x170d;
    /* 0x170E */ u8 field_0x170e;
    /* 0x170F */ u8 field_0x170f;
    /* 0x1710 */ s16 field_0x1710;
    /* 0x1712 */ s16 field_0x1712;
    /* 0x1714 */ actionFunc mpTeachAction;
    /* 0x1720 */ actionFunc mpActionFunc;
    /* 0x172C */ daNpcT_Path_c mPath;
    /* 0x1754 */ u8 field_0x1754;
};

STATIC_ASSERT(sizeof(daNpc_Kn_c) == 0x1758);

#endif /* D_A_NPC_KN_H */
