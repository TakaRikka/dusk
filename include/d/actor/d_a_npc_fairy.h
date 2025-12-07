#ifndef D_A_NPC_FAIRY_H
#define D_A_NPC_FAIRY_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpc_Fairy_c
 * @brief Great Fairy
 *
 * @details
 *
*/

class daNpc_Fairy_c;

class _Fairy_Feather_c {
public:
    enum Joint {
        /* 0x00 */ JNT_ROOT,
        /* 0x01 */ JNT_TUKENE,
        /* 0x02 */ JNT_FEATHER1AL1,
        /* 0x03 */ JNT_FEATHER1AL2,
        /* 0x04 */ JNT_FEATHER1AR1,
        /* 0x05 */ JNT_FEATHER1AR2,
        /* 0x06 */ JNT_FEATHER1BL1,
        /* 0x07 */ JNT_FEATHER1BL2,
        /* 0x08 */ JNT_FEATHER1BR1,
        /* 0x09 */ JNT_FEATHER1BR2,
        /* 0x0A */ JNT_FEATHER2AL1,
        /* 0x0B */ JNT_FEATHER2AL2,
        /* 0x0C */ JNT_FEATHER2AR1,
        /* 0x0D */ JNT_FEATHER2AR2,
        /* 0x0E */ JNT_FEATHER2BL1,
        /* 0x0F */ JNT_FEATHER2BL2,
        /* 0x10 */ JNT_FEATHER2BR1,
        /* 0x11 */ JNT_FEATHER2BR2,
        /* 0x12 */ JNT_FEATHER3AL1,
        /* 0x13 */ JNT_FEATHER3AL2,
        /* 0x14 */ JNT_FEATHER3AR1,
        /* 0x15 */ JNT_FEATHER3AR2,
        /* 0x16 */ JNT_FEATHER3BL1,
        /* 0x17 */ JNT_FEATHER3BL2,
        /* 0x18 */ JNT_FEATHER3BR1,
        /* 0x19 */ JNT_FEATHER3BR2,
    };

    _Fairy_Feather_c() {
        mpMorf = NULL;
        mPrevAnm = -1;
    }
    ~_Fairy_Feather_c() {
        if (mpMorf != NULL) {
            delete mpMorf;
            mpMorf = NULL;
        }
    }
    BOOL create();
    BOOL loadModel();
    BOOL setAnm(int);
    BOOL setBrk(int);
    BOOL setBtk(int);
    static int ctrlJointCallBack(J3DJoint*, int);
    void connect(daNpc_Fairy_c*);
    void draw(daNpc_Fairy_c*);

    void output_anmframe() {}

    void playAnm() {
        mBtkAnm.play();
        mBrkAnm.play();
        mpMorf->play(0, 0);
    }

    f32 getAnmFrame() { return mpMorf->getFrame(); }
    void setAnmFrame(f32 i_frame) { mpMorf->setFrame(i_frame); }
    void setAnmPlaySpeed(f32 i_playSpeed) { mpMorf->setPlaySpeed(i_playSpeed); }

    /* 0x00 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x04 */ mDoExt_invisibleModel mModel;
    /* 0x0C */ mDoExt_btkAnm mBtkAnm;
    /* 0x24 */ mDoExt_brkAnm mBrkAnm;
    /* 0x3C */ int mPrevAnm;
    /* 0x40 */ Z2Creature mSound;
};

struct daNpc_Fairy_HIOParam {
    /* 0x00 */ daNpcT_HIOParam common;
    /* 0x8C */ f32 spirit_width;
    /* 0x90 */ f32 spirit_height;
};

class daNpc_Fairy_Param_c {
public:
    virtual ~daNpc_Fairy_Param_c() {}

    static daNpc_Fairy_HIOParam const m;
};

#if DEBUG
class daNpc_Fairy_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Fairy_HIO_c();
    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    /* 0x4 */ daNpc_Fairy_HIOParam m;
};

#define NPC_FAIRY_HIO_CLASS daNpc_Fairy_HIO_c
#else
#define NPC_FAIRY_HIO_CLASS daNpc_Fairy_Param_c
#endif

class daNpc_Fairy_c : public daNpcT_c {
public:
    enum Joint {
        /* 0x00 */ JNT_CENTER,
        /* 0x01 */ JNT_BACKBONE1,
        /* 0x02 */ JNT_BACKBONE2,
        /* 0x03 */ JNT_BHAIR1,
        /* 0x04 */ JNT_BHAIR2,
        /* 0x05 */ JNT_BHAIR3,
        /* 0x06 */ JNT_FHAIRL1,
        /* 0x07 */ JNT_FHAIRL2,
        /* 0x08 */ JNT_FHAIRL3,
        /* 0x09 */ JNT_FHAIRR1,
        /* 0x0A */ JNT_FHAIRR2,
        /* 0x0B */ JNT_FHAIRR3,
        /* 0x0C */ JNT_NECK,
        /* 0x0D */ JNT_HEAD,
        /* 0x0E */ JNT_CHIN,
        /* 0x0F */ JNT_MAYUL,
        /* 0x10 */ JNT_MAYUR,
        /* 0x11 */ JNT_MOUTH,
        /* 0x12 */ JNT_SHOULDERL,
        /* 0x13 */ JNT_ARML1,
        /* 0x14 */ JNT_ARML2,
        /* 0x15 */ JNT_HANDL,
        /* 0x16 */ JNT_FINGERL,
        /* 0x17 */ JNT_THAMBL,
        /* 0x18 */ JNT_SHOULDERR,
        /* 0x19 */ JNT_ARMR1,
        /* 0x1A */ JNT_ARMR2,
        /* 0x1B */ JNT_HANDR,
        /* 0x1C */ JNT_FINGERR,
        /* 0x1D */ JNT_THAMBR,
        /* 0x1E */ JNT_WAIST,
        /* 0x1F */ JNT_CLOTH,
        /* 0x20 */ JNT_LEGL1,
        /* 0x21 */ JNT_LEGL2,
        /* 0x22 */ JNT_FOOTL,
        /* 0x23 */ JNT_TOEL,
        /* 0x24 */ JNT_LEGR1,
        /* 0x25 */ JNT_LEGR2,
        /* 0x26 */ JNT_FOOTR,
        /* 0x27 */ JNT_TOER,
    };

    typedef int (daNpc_Fairy_c::*actionFunc)(int);
    typedef int (daNpc_Fairy_c::*cutFunc)(int);

    ~daNpc_Fairy_c();
    cPhs__Step Create();
    int CreateHeap();
    void setAnmData();
    bool setMotionAnm(int, f32, int);
    bool afterSetMotionAnm(int, int, f32, int);
    int Delete();
    int Execute();
    int Draw();
    void drawOtherMdl();
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    BOOL isDelete();
    void reset();
    void initStatus();
    void afterJntAnm(int);
    void setParam();
    BOOL checkChangeEvt();
    void setAfterTalkMotion();
    void srchActors();
    BOOL evtTalk();
    BOOL evtCutProc();
    BOOL evtEndProc();
    void action();
    void ptcl_water();
    void ptcl_sprit();
    void _to_FairyField();
    int chk_tempbit();
    void beforeMove();
    void afterMoved();
    void setAttnPos();
    void setCollision();
    int drawDbgInfo();
    BOOL setAction(actionFunc, int);
    int wait(int);
    int talk(int);
    int test(int);
    void AppearDemoCall();
    void ReturnDemoCall();
    void PresentDemoCall();
    int cutAppear_10F_01(int);
    int _cutAppear_10F_01_Init(int const&);
    int _cutAppear_10F_01_Main(int const&);
    int cutAppear_10F_02(int);
    int _cutAppear_10F_02_Init(int const&);
    int _cutAppear_10F_02_Main(int const&);
    int cutAppear_20F_01(int);
    int _cutAppear_20F_01_Init(int const&);
    int _cutAppear_20F_01_Main(int const&);
    int cutAppear_20F_02(int);
    int _cutAppear_20F_02_Init(int const&);
    int _cutAppear_20F_02_Main(int const&);
    int cutAppear_30F_01(int);
    int _cutAppear_30F_01_Init(int const&);
    int _cutAppear_30F_01_Main(int const&);
    int cutAppear_30F_02(int);
    int _cutAppear_30F_02_Init(int const&);
    int _cutAppear_30F_02_Main(int const&);
    int cutAppear_40F_01(int);
    int _cutAppear_40F_01_Init(int const&);
    int _cutAppear_40F_01_Main(int const&);
    int cutAppear_40F_02(int);
    int _cutAppear_40F_02_Init(int const&);
    int _cutAppear_40F_02_Main(int const&);
    int cutAppear_50F_01(int);
    int _cutAppear_50F_01_Init(int const&, int const&);
    int _cutAppear_50F_01_Main(int const&, int const&);
    int cutAppear_50F_02(int);
    int _cutAppear_50F_02_Init(int const&, int const&);
    int _cutAppear_50F_02_Main(int const&, int const&);
    int cutAppear_50F_03(int);
    int _cutAppear_50F_03_Init(int const&, int const&);
    int _cutAppear_50F_03_Main(int const&, int const&);
    int cutAppear_50F_04(int);
    int _cutAppear_50F_04_Init(int const&, int const&);
    int _cutAppear_50F_04_Main(int const&, int const&);
    int cutAppear_50F_05(int);
    int _cutAppear_50F_05_Init(int const&, int const&);
    int _cutAppear_50F_05_Main(int const&, int const&);
    int cutSelect_Return1(int);
    int _cutSelect_Return1_Init(int const&);
    int _cutSelect_Return1_Main(int const&);
    int cutSelect_Return2(int);
    int _cutSelect_Return2_Init(int const&);
    int _cutSelect_Return2_Main(int const&);
    int cutSelect_Return3(int);
    int _cutSelect_Return3_Init(int const&);
    int _cutSelect_Return3_Main(int const&);
    int cutReturnCancel(int);
    int _cutReturnCancel_Init(int const&);
    int _cutReturnCancel_Main(int const&);
    daNpc_Fairy_c(daNpcT_faceMotionAnmData_c const* i_faceMotionAnmData,
        daNpcT_motionAnmData_c const* i_motionAnmData,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_faceMotionSequenceData,
        int i_faceMotionStepNum,
        daNpcT_MotionSeqMngr_c::sequenceStepData_c const* i_motionSequenceData, int i_motionStepNum,
        daNpcT_evtData_c const* i_evtData, char** i_arcNames)
        : daNpcT_c(i_faceMotionAnmData, i_motionAnmData, i_faceMotionSequenceData,
                   i_faceMotionStepNum, i_motionSequenceData, i_motionStepNum, i_evtData,
                   i_arcNames) {
                    OS_REPORT("|%06d:%x|daNpc_Fairy_c -> コンストラクト\n", g_Counter.mCounter0, this);
                   }
    u16 getEyeballMaterialNo() { return 1; }
    s32 getHeadJointNo() { return JNT_HEAD; }
    s32 getNeckJointNo() { return JNT_NECK; }
    s32 getBackboneJointNo() { return JNT_BACKBONE1; }
    BOOL checkChangeJoint(int i_joint) { return i_joint == JNT_HEAD; }
    BOOL checkRemoveJoint(int i_joint) { return i_joint == JNT_MOUTH; }

    int getFlowNodeNo() {
        u16 nodeNo = home.angle.x;
        return nodeNo == 0xFFFF ? -1 : nodeNo;
    }

    u8 getType() {
        u32 param = fopAcM_GetParam(this) & 0xFF;
        if (param == 0xFF) {
            param = 0;
        }

        return param;
    }

    BOOL chkAction(actionFunc action) { return mAction == action; }

    int getSwitchBitNo() { return fopAcM_GetParam(this) >> 24; }

    void sprit_se() { mSound.startCreatureSoundLevel(Z2SE_FAIRY_STAY, 0, -1); }
    void feather_se() { mSound.startCreatureSoundLevel(Z2SE_FAIRY_HANE_STAY, 0, -1); }
    void feather_appear_se() { mSound.startCreatureSound(Z2SE_FAIRY_HANE_OP, 0, -1); }

    u8 getSceneNo1() { return (fopAcM_GetParam(this) >> 8) & 0xFF; }
    u8 getSceneNo2() { return (fopAcM_GetParam(this) >> 16) & 0xFF; }

    static char* mCutNameList[18];
    static cutFunc mCutList[18];
private:
    /* 0x0E40 */ NPC_FAIRY_HIO_CLASS* mHIO;
    /* 0x0E44 */ dCcD_Cyl mCyl;
    /* 0x0F80 */ u8 mType;
    /* 0x0F84 */ actionFunc mAction;
    /* 0x0F90 */ s16 mTimer;
    /* 0x0F92 */ u8 field_0xf92[0xf94 - 0xf92];
    /* 0x0F94 */ s16 mPrtclTimer;
    /* 0x0F98 */ dPaPo_c mPaPo;
    /* 0x0FD0 */ u32 mPrtcls[3];
    /* 0x0FDC */ int mSwitchBit;
    /* 0x0FE0 */ u8 field_0xfe0[0xfe4 - 0xfe0];
    /* 0x0FE4 */ int mTempBit;
    /* 0x0FE8 */ cXyz mCurrentPosCopy;
    /* 0x0FF4 */ u16 field_0xff4;
    /* 0x0FF6 */ u8 mStatus;
    /* 0x0FF7 */ u8 mUnkFlag;
    /* 0x0FF8 */ _Fairy_Feather_c mFairyFeather;
};

STATIC_ASSERT(sizeof(daNpc_Fairy_c) == 0x10c8);

#endif /* D_A_NPC_FAIRY_H */
