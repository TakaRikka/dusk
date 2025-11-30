#ifndef D_A_MIDNA_H
#define D_A_MIDNA_H

#include "JSystem/J3DGraphBase/J3DMatBlock.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "Z2AudioLib/Z2Creature.h"
#include "f_op/f_op_actor.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_com_inf_game.h"
#include "d/d_msg_flow.h"
#include "d/actor/d_a_player.h"

class daKago_c;

class daMidna_hio_c1 {
public:
    /* 0x00 */ u8 forced_display;
    /* 0x01 */ u8 hio_priority;
    /* 0x02 */ s16 hair_color_1r;
    /* 0x04 */ s16 hair_color_1g;
    /* 0x06 */ s16 hair_color_1b;
    /* 0x08 */ s16 hair_k_color_1r;
    /* 0x0A */ s16 hair_k_color_1g;
    /* 0x0C */ s16 hair_k_color_1b;
    /* 0x0E */ s16 hair_k_color_2r;
    /* 0x10 */ s16 hair_k_color_2g;
    /* 0x12 */ s16 hair_k_color_2b;
    /* 0x14 */ f32 scale;
    /* 0x18 */ f32 y_pos;
    /* 0x1C */ f32 z_pos;
};

class daMidna_hio_c0 {
public:
    static daMidna_hio_c1 const m;
};

STATIC_ASSERT(sizeof(daMidna_hio_c0::m) == 0x20);

#if DEBUG
class daMidna_hio_c : public JORReflexible {
public:
    daMidna_hio_c();
    void genMessage(JORMContext*);
    virtual ~daMidna_hio_c() {}

    /* 0x4 */ s8 id;
    /* 0x8 */ daMidna_hio_c1 m;
};
#define NPC_MIDNA_HIO_CLASS daMidna_hio_c
#else
#define NPC_MIDNA_HIO_CLASS daMidna_hio_c0
#endif

class daMidna_McaMorfCB1_c : public mDoExt_McaMorfCallBack1_c {
public:
    int execute(u16, J3DTransformInfo*);
    virtual ~daMidna_McaMorfCB1_c() {}

    void setScale(cXyz* i_scale) { mpScale = i_scale; }

    /* 0x4 */ cXyz* mpScale;
};

class daMidna_matAnm_c : public J3DMaterialAnm {
public:
    void init();
    void calc(J3DMaterial*) const;
    inline ~daMidna_matAnm_c() {}

    daMidna_matAnm_c() { init(); }

    f32* getNowOffsetXP() { return &mNowOffsetX; }
    f32* getNowOffsetYP() { return &mNowOffsetY; }
    void setNowOffsetX(f32 offsetX) { mNowOffsetX = offsetX; }
    void setNowOffsetY(f32 offsetY) { mNowOffsetY = offsetY; }

    static bool getEyeMoveFlg() { return sEyeMoveFlg; }
    static void offEyeMoveFlg() { sEyeMoveFlg = false; }
    static void onEyeMoveFlg() { sEyeMoveFlg = true; }
    static void setMorfFrame(u8 frame) { sMorfFrame = frame; }
    static void decMorfFrame() { cLib_calcTimer(&sMorfFrame); }
    static u8 getMorfFrame() { return sMorfFrame; }

    /* 0x0F4 */ f32 mOldTransX;
    /* 0x0F8 */ f32 mOldTransY;
    /* 0x0FC */ f32 mNowOffsetX;
    /* 0x100 */ f32 mNowOffsetY;

    static bool sEyeMoveFlg;
    static u8 sMorfFrame;
};

STATIC_ASSERT(sizeof(daMidna_matAnm_c) == 0x104);

struct daMidna_anmData_s {
    /* 0x0 */ u16 mResID;
    /* 0x2 */ u16 mTexID;
    /* 0x4 */ f32 mSoundFrame;
    /* 0x8 */ u32 mSoundID;
};

struct daMidna_texData_s {
    /* 0x0 */ u16 mBtpID;
    /* 0x2 */ u16 mBtkID;
};

class daMidna_c;
typedef int (daMidna_c::*daMidna_func)();

/**
 * @ingroup actors-unsorted
 * @class daMidna_c
 * @brief Midna
 *
 * @details
 *
*/

class daMidna_c : public fopAc_ac_c {
public:
    enum daMidna_ERFLG0 {
        ERFLG0_UNK_100 = 0x100,
        ERFLG0_NO_SERVICE_WAIT = 0x80,
        ERFLG0_UNK_40 = 0x40,
        ERFLG0_UNK_20 = 0x20,
        ERFLG0_FORCE_MORF_CANCEL = 0x10,
        ERFLG0_FORCE_PANIC = 8,
        ERFLG0_UNK_2 = 2,
        ERFLG0_UNK_1 = 1,
    };

    enum daMidna_FLG0 {
        FLG0_NO_HAIR_LEAD = 0x80000000,
        FLG0_UNK_40000000 = 0x40000000,
        FLG0_NO_HAIR_SCALE = 0x20000000,
        FLG0_UNK_10000000 = 0x10000000,
        FLG0_PORTAL_OBJ_CALL = 0x8000000,
        FLG0_UNK_4000000 = 0x4000000,
        FLG0_UNK_2000000 = 0x2000000,
        FLG0_UNK_1000000 = 0x1000000,
        FLG0_UNK_800000 = 0x800000,
        FLG0_UNK_400000 = 0x400000,
        FLG0_UNK_200000 = 0x200000,
        FLG0_NPC_NEAR = 0x100000,
        FLG0_UNK_80000 = 0x80000,
        FLG0_NPC_FAR = 0x40000,
        FLG0_FORCE_NECK_AIM = 0x20000,
        FLG0_UNK_10000 = 0x10000,
        FLG0_UNK_8000 = 0x8000,
        FLG0_NO_INPUT = 0x4000,
        FLG0_NO_DRAW = 0x2000,
        FLG0_UNK_1000 = 0x1000,
        FLG0_WOLF_NO_POS = 0x800,
        FLG0_TAG_WAIT = 0x400,
        FLG0_UNK_200 = 0x200,
        FLG0_UNK_100 = 0x100,
        FLG0_UNK_80 = 0x80,
        FLG0_UNK_40 = 0x40,
        FLG0_UNK_20 = 0x20,
        FLG0_UNK_10 = 0x10,
        FLG0_UNK_8 = 8,
        FLG0_UNK_4 = 4,
        FLG0_UNK_2 = 2,
        FLG0_UNK_1 = 1,
        FLG0_TAG_PORTAL = FLG0_TAG_WAIT | FLG0_PORTAL_OBJ_CALL,
    };

    enum daMidna_FLG1 {
        FLG1_SHADOW_NO_DRAW = 0x1000,
        FLG1_UNK_800 = 0x800,
        FLG1_UNK_400 = 0x400,
        FLG1_UNK_200 = 0x200,
        FLG1_SIDE_WARP = 0x100,
        FLG1_NO_MASK_DRAW = 0x80,
        FLG1_UNK_40 = 0x40,
        FLG1_UNK_20 = 0x20,
        FLG1_UNK_10 = 0x10,
        FLG1_FORCE_NORMAL_COL = 8,
        FLG1_FORCE_TIRED_COL = 4,
        FLG1_SHADOW_MODEL_DRAW_DEMO_FORCE = 2,
        FLG1_UNK_1 = 1,
    };

    enum daMidna_ANM {
        /* 0x00 */ ANM_WAITA,
        /* 0x01 */ ANM_CLING,
        /* 0x02 */ ANM_CLINGB,
        /* 0x03 */ ANM_WAITB,
        /* 0x04 */ ANM_PANIC,
        /* 0x05 */ ANM_TALKA,
        /* 0x06 */ ANM_LEADST,
        /* 0x07 */ ANM_LEADED,
        /* 0x08 */ ANM_LEADWAIT,
        /* 0x09 */ ANM_LEADTOWAITA,
        /* 0x0A */ ANM_LEADSWAIT,
        /* 0x0B */ ANM_CLINGST,
        /* 0x0C */ ANM_SWAITB,
        /* 0x0D */ ANM_SWAITC,
        /* 0x0E */ ANM_WLDIEA,
        /* 0x0F */ ANM_WLSWIMDIE,
        /* 0x10 */ ANM_S_APPEAR,
        /* 0x11 */ ANM_S_APPEARBL,
        /* 0x12 */ ANM_S_RETURN,
        /* 0x13 */ ANM_S_HOLE,
        /* 0x14 */ ANM_WARPIN,
        /* 0x15 */ ANM_WAITTP,
        /* 0x16 */ ANM_ENTRANCE,
        /* 0x17 */ ANM_SWAITD,
        /* 0x18 */ ANM_HIT,
        /* 0x19 */ ANM_LOOKABOUT,
        /* 0x1A */ ANM_SWAITA,
        /* 0x1B */ ANM_S_MOVE,
        /* 0x1C */ ANM_RETURN,
        /* 0x1D */ ANM_WAITD,
        /* 0x1E */ ANM_CLINGD,
        /* 0x1F */ ANM_ROPEWAIT,
        /* 0x20 */ ANM_ROPEFALTER,
        /* 0x21 */ ANM_DIGIN,
        /* 0x22 */ ANM_DOOROPC,
        /* 0x23 */ ANM_DOOROPD,
        /* 0x24 */ ANM_GRAB,
        /* 0x25 */ ANM_GRABED,
        /* 0x26 */ ANM_MGNCATCHST,
        /* 0x27 */ ANM_MGNCATCHED,
        /* 0x28 */ ANM_MGNTHROWLST,
        /* 0x29 */ ANM_MGNTHROWLED,
        /* 0x2A */ ANM_MGNTHROWRST,
        /* 0x2B */ ANM_MGNTHROWRED,
        /* 0x2C */ ANM_FTALKA,
        /* 0x2D */ ANM_FTALKB,
        /* 0x2E */ ANM_FTALKC,
        /* 0x2F */ ANM_FTALKC2,
        /* 0x30 */ ANM_HAIR,
        /* 0x31 */ ANM_S_TAKES,
        /* 0x32 */ ANM_S_WAITS,
        /* 0x33 */ ANM_S_PACKAWAY,
        /* 0x34 */ ANM_GRABST,
        /* 0x35 */ ANM_MAX,
        /* 0x00 */ ANM_NONE = 0,
    };

    void onForcePanic() { onEndResetStateFlg0(ERFLG0_FORCE_PANIC); }
    void onRatBody(int) { onForcePanic(); }
    u32 checkForceNormalColor() const { return checkStateFlg1(FLG1_FORCE_NORMAL_COL); }
    u32 checkForceTiredColor() const { return checkStateFlg1(FLG1_FORCE_TIRED_COL); }
    void onNoServiceWait() { onEndResetStateFlg0(ERFLG0_NO_SERVICE_WAIT); }

    int modelCallBack(int);
    int changeUpperBck();
    int changeFaceBck();
    int baseModelCallBack(int);
    int initInvModel(u16, J3DModel**, mDoExt_invisibleModel*, u32);
    int initDemoModel(J3DModel**, char const*, u32);
    int createHeap();
    cPhs__Step create();
    void allAnimePlay();
    void setMatrix();
    void setBodyPartMatrix();
    void setRoomInfo();
    void setBodyPartPos();
    BOOL checkAppear();
    void checkMidnaPosState();
    BOOL setUpperAnime(u16, u16);
    BOOL setUpperAnimeAndSe(daMidna_ANM);
    BOOL setFaceAnime(u16, u16);
    void endHighModel();
    BOOL setDemoAnm();
    void setFaceBtp(u16, int);
    void setFaceBtk(u16, int);
    void setLeftHandShape(u16);
    void setRightHandShape(u16);
    BOOL checkHairOnlyAnime(int) const;
    void setBckAnime(J3DAnmTransform*, int, f32);
    void setAnm();
    s16 getNeckAimAngle(cXyz const*, s16*, s16*, s16*, s16*);
    void clearEyeMove();
    void setEyeMove(cXyz const*, s16, s16);
    void setNeckAngle();
    void initHairAngle();
    void setHairAngle();
    void setDemoData();
    J3DAnmTevRegKey* setSimpleBrk(J3DModelData*, u16);
    J3DAnmTextureSRTKey* setSimpleBtk(J3DModelData*, u16);
    void initMidnaModel();
    void setMidnaNoDrawFlg();
    int checkMetamorphoseEnableBase();
    BOOL checkNoDrawState();
    void setSound();
    int execute();
    int draw();
    ~daMidna_c();

    daMidna_c() : mBtpHeap(0x400), mBtkHeap(0x400) {}

    void onEndResetStateFlg0(daMidna_ERFLG0 pFlg) { mEndResetStateFlg0 |= pFlg; }
    void onStateFlg0(daMidna_FLG0 i_flag) { mStateFlg0 |= i_flag; }
    void offStateFlg0(daMidna_FLG0 i_flag) { mStateFlg0 &= ~i_flag; }
    void onStateFlg1(daMidna_FLG1 i_flag) { mStateFlg1 |= i_flag; }
    void offStateFlg1(daMidna_FLG1 i_flag) { mStateFlg1 &= ~i_flag; }
    u32 checkEndResetStateFlg0(daMidna_ERFLG0 flag) const { return mEndResetStateFlg0 & flag; }
    u32 checkStateFlg0(daMidna_FLG0 flag) const { return mStateFlg0 & flag; }
    u32 checkStateFlg1(daMidna_FLG1 flag) const { return mStateFlg1 & flag; }
    void setFaceNum(int num) { mFaceNum = num; }
    void setMotionNum(int num) { mMotionNum = num; }
    bool checkDemoTypeNone() const { return mDemoType == 0; }
    void changeOriginalDemo() { mDemoType = 3; }
    void changeDemoMode(u32 mode) { mDemoMode = mode; }
    BOOL checkPortalObjCall() { return checkStateFlg0(FLG0_PORTAL_OBJ_CALL); }
    BOOL checkNoHairScale() const { return checkStateFlg0(FLG0_NO_HAIR_SCALE); }
    BOOL checkNpcNear() const { return checkStateFlg0(FLG0_NPC_NEAR); }
    BOOL checkNpcFar() const { return checkStateFlg0(FLG0_NPC_FAR); }
    u32 checkNoDraw() const { return checkStateFlg0(FLG0_NO_DRAW); }
    BOOL checkNoInput() const { return checkStateFlg0(FLG0_NO_INPUT); }
    BOOL checkWolfNoPos() const { return checkStateFlg0(FLG0_WOLF_NO_POS); }
    int checkMetamorphoseEnable() { return (this->*mpFunc)(); }
    u32 checkShadowModelDrawDemoForce() const { return checkStateFlg1(FLG1_SHADOW_MODEL_DRAW_DEMO_FORCE); }
    
    void setForceNeckAimPos(const cXyz& i_pos) {
        onStateFlg0(FLG0_FORCE_NECK_AIM);
        mForceNeckAimPos = i_pos;
    }

    void onTagWaitPosWarp(const cXyz* param_0) {
        mTagWaitPos = *param_0;
        onStateFlg0(FLG0_TAG_WAIT);
        onEndResetStateFlg0(ERFLG0_UNK_2);
    }

    void onTagWaitPosPortalObj(const cXyz& i_pos) {
        mTagWaitPos = i_pos;
        onStateFlg0(FLG0_TAG_PORTAL);
    }

    void onTagWaitPos(const cXyz* param_0) {
        mTagWaitPos = *param_0;
        onStateFlg0(FLG0_TAG_WAIT);
    }

    void offTagWaitPos() {
        offStateFlg0((daMidna_FLG0)(FLG0_NO_HAIR_LEAD | FLG0_PORTAL_OBJ_CALL | FLG0_TAG_WAIT));
    }

    void onTagNoHairLead() { onStateFlg0(FLG0_NO_HAIR_LEAD); }
    void onSideWarp() { onStateFlg1(FLG1_SIDE_WARP); }

    MtxP getMtxHairTop() { return mpShadowModel->getAnmMtx(10); }

    bool checkSetAnime(int param_0, daMidna_ANM i_anm) const {
        return mBckHeap[param_0].getIdx() == m_anmDataTable[i_anm].mResID;
    }

    bool checkReturnAnime() const {
        return mBckHeap[0].getIdx() == 0x1CA;
    }

    bool checkShadowModelDrawSmode() const {
        return field_0x84e == 3 || field_0x84e == 4 || field_0x84e == 2;
    }

    bool checkShadowModelDraw() const {
        return checkShadowModelDrawDemoForce() || checkShadowModelDrawSmode();
    }

    bool checkShadowReturnEnd() const {
        return mBckHeap[0].getIdx() == 0x21C && mpMorf->isStop();
    }

    bool checkShadowModeTalkWait() const {
        return (field_0x84e == 2 || field_0x84e == 1) ;
    }

    void setShadowReturn() { field_0x84e = 4; }

    bool checkPortalObjRide() const {
        return checkStateFlg0(FLG0_PORTAL_OBJ_CALL) && checkStateFlg0(FLG0_UNK_200);
    }

    BOOL checkShadowNoDraw() const {
        return checkStateFlg1(FLG1_SHADOW_NO_DRAW);
    }

    BOOL checkNoMaskDraw() const {
        return checkStateFlg1(FLG1_NO_MASK_DRAW);
    }

    BOOL checkDemoPortalWarpWait() const { return mDemoMode == 11; }

    static BOOL checkMidnaRealBody() {
        return dKy_darkworld_check() == true || dComIfGs_isTransformLV(3);
    }

    static bool checkMidnaTired() {
                                                   /* dSv_event_flag_c::F_0250 - Cutscene - [cutscene: 21] reunion with Zelda / Midna revived (Hyrule Castle barrier appears) */
        return dComIfGs_isTransformLV(3) && !dComIfGs_isEventBit(0x1E08);
    }

    void resetRatBody() {}

    bool checkFlyWaitAnime() const {
        // fakematch (doesn't match in debug)
        return (u16)mBckHeap[0].getIdx() == 0x1CB || ((u16)mBckHeap[0].getIdx() == 0x1C7
            || (u16)mBckHeap[0].getIdx() == 0x1C8 || (u16)mBckHeap[0].getIdx() == 0x1C9);
    }

    void onForceMorfCancel() { onEndResetStateFlg0(ERFLG0_FORCE_MORF_CANCEL); }

    void setCargoActor(fopAc_ac_c* i_actor) {
        mpKago = (daKago_c*)i_actor;
    }

    void offCargoActor() {
        mpKago = NULL;
    }

    void onTagWaitPosPortalObj(const cXyz* i_pos) {
        mTagWaitPos = *i_pos;
        onStateFlg0(daMidna_FLG0(FLG0_PORTAL_OBJ_CALL | FLG0_TAG_WAIT));
    }

    static u32 getOtherHeapSize() { return 0x1D0; }

    static daMidna_texData_s const m_texDataTable[21];
    static daMidna_anmData_s const m_anmDataTable[53];

private:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ J3DModel* mpShadowModel;
    /* 0x578 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x57C */ daMidna_McaMorfCB1_c mMorfCB;
    /* 0x584 */ mDoExt_bckAnm mUpperBck;
    /* 0x5A0 */ mDoExt_bckAnm mFaceBck;
    /* 0x5BC */ J3DAnmTexPattern* mpFaceBtp;
    /* 0x5C0 */ J3DAnmTextureSRTKey* mpFaceBtk;
    /* 0x5C4 */ mDoExt_invisibleModel mInvModel;
    /* 0x5CC */ mDoExt_invisibleModel mMaskInvModel;
    /* 0x5D4 */ mDoExt_invisibleModel mHandsInvModel;
    /* 0x5DC */ mDoExt_invisibleModel mHairhandInvModel;
    /* 0x5E4 */ daPy_anmHeap_c mBckHeap[3];
    /* 0x620 */ daPy_anmHeap_c mBtpHeap;
    /* 0x634 */ daPy_anmHeap_c mBtkHeap;
    /* 0x648 */ J3DAnmTextureSRTKey* mpMoyaBtk;
    /* 0x64C */ J3DAnmTextureSRTKey* mpMaskMoyaBtk;
    /* 0x650 */ J3DAnmTextureSRTKey* mpHandsMoyaBtk;
    /* 0x654 */ J3DAnmTextureSRTKey* mpHairhandMoyaBtk;
    /* 0x658 */ J3DAnmTevRegKey* mpAppearBrk;
    /* 0x65C */ J3DAnmTevRegKey* mpMaskAppearBrk;
    /* 0x660 */ J3DAnmTevRegKey* mpHairhandAppearBrk;
    /* 0x664 */ J3DAnmTevRegKey* mpGokouAppearBrk;
    /* 0x668 */ J3DAnmTevRegKey* field_0x668;
    /* 0x66C */ J3DAnmTevRegKey* mpDemoFCBlendBrk;
    /* 0x670 */ J3DModel* mpMaskBmd;
    /* 0x674 */ J3DModel* mpShadowMaskBmd;
    /* 0x678 */ J3DModel* mpHandsBmd;
    /* 0x67C */ J3DModel* mpShadowHandsBmd;
    /* 0x680 */ J3DModel* mpHairhandBmd;
    /* 0x684 */ J3DModel* mpShadowHairhandBmd;
    /* 0x688 */ J3DModel* mpDemoBDTmpBmd;
    /* 0x68C */ J3DModel* mpDemoBDMaskBmd;
    /* 0x690 */ J3DModel* mpDemoBDHandsBmd;
    /* 0x694 */ J3DModel* mpDemoBDHairhandBmd;
    /* 0x698 */ J3DModel* mpGokouBmd;
    /* 0x69C */ J3DShape* mpLeftHandShape;
    /* 0x6A0 */ J3DShape* mpRightHandShape;
    /* 0x6A4 */ J3DShape* mpShadowLeftHandShape;
    /* 0x6A8 */ J3DShape* mpShadowRightHandShape;
    /* 0x6AC */ J3DModel* mpDemoFCBlendBmd;
    /* 0x6B0 */ J3DModel* mpDemoFCTongueBmd;
    /* 0x6B4 */ J3DAnmTextureSRTKey* mpDemoFCTongueBtk;
    /* 0x6B8 */ mDoExt_bckAnm* mpDemoFCTmpBck;
    /* 0x6BC */ J3DDeformData* mpDemoFCTmpBls;
    /* 0x6C0 */ J3DAnmCluster* mpDemoFCTmpCluster;
    /* 0x6C4 */ mDoExt_blkAnm* mpDemoFCTmpBlk;
    /* 0x6C8 */ J3DModel* mpDemoHLTmpBmd;
    /* 0x6CC */ mDoExt_bckAnm* mpDemoHLTmpBck;
    /* 0x6D0 */ J3DModel* mpDemoHRTmpBmd;
    /* 0x6D4 */ mDoExt_bckAnm* mpDemoHRTmpBck;
    /* 0x6D8 */ J3DModel* mpDemoHDTmpBmd;
    /* 0x6DC */ mDoExt_bckAnm* mpDemoHDTmpBck;
    /* 0x6E0 */ J3DGXColorS10 field_0x6e0;
    /* 0x6E8 */ J3DGXColor field_0x6e8;
    /* 0x6EC */ J3DGXColor field_0x6ec;
    /* 0x6F0 */ daMidna_matAnm_c* mpEyeMatAnm[2];
    /* 0x6F8 */ NPC_MIDNA_HIO_CLASS* mpHIO;
    /* 0x6FC */ Z2Creature mSound;
    /* 0x78C */ dMsgFlow_c mMsgFlow;
    /* 0x7D8 */ dBgS_LinkLinChk mLinChk;
    /* 0x848 */ u8 mMotionNum;
    /* 0x849 */ u8 mFaceNum;
    /* 0x84A */ u8 mFrameCounter;
    /* 0x84B */ u8 mFaceBckTimer;
    /* 0x84C */ u8 mEyeMoveTimer;
    /* 0x84D */ s8 mReverb;
    /* 0x84E */ u8 field_0x84e;
    /* 0x84F */ u8 mDemoType;
    /* 0x850 */ u8 field_0x850;
    /* 0x851 */ u8 field_0x851;
    /* 0x852 */ u8 field_0x852[0x85a - 0x852];
    /* 0x85A */ s16 mPrevShapeAngleY;
    /* 0x85C */ s16 mHairAngleZ[5];
    /* 0x866 */ s16 mHairAngleY[5];
    /* 0x870 */ u8 field_0x870[0x872 - 0x870];
    /* 0x872 */ s16 field_0x872;
    /* 0x874 */ s16 mBackboneAngleZ;
    /* 0x876 */ s16 mPlayerAngleY;
    /* 0x878 */ u16 mLeftHandShapeIdx;
    /* 0x87A */ u16 mRightHandShapeIdx;
    /* 0x87C */ s16 mDemoAngle;
    /* 0x87E */ s16 mJntNo;
    /* 0x880 */ csXyz mNeckAngle;
    /* 0x886 */ u8 field_0x886[0x88c - 0x886];
    /* 0x88C */ int mStaffID;
    /* 0x890 */ u32 mStateFlg0;
    /* 0x894 */ u32 mStateFlg1;
    /* 0x898 */ u32 mEndResetStateFlg0;
    /* 0x89C */ u32 mShadowKey;
    /* 0x8A0 */ u32 mParticleKey1;
    /* 0x8A4 */ u32 mParticleKey2;
    /* 0x8A8 */ u32 mThroughEffectKey[6];
    /* 0x8C0 */ u32 mDemoMode;
    /* 0x8C4 */ u32 mSoundID;
    /* 0x8C8 */ f32 mVoiceFrame;
    /* 0x8CC */ f32 mEyeMoveX;
    /* 0x8D0 */ f32 mEyeMoveY;
    /* 0x8D4 */ f32 field_0x8d4;
    /* 0x8D8 */ f32 mDemoSpeed;
    /* 0x8DC */ f32 field_0x8dc;
    /* 0x8E0 */ cXyz mHairPos[5];
    /* 0x91C */ cXyz mHairDir[5];
    /* 0x958 */ cXyz mHeadCenterPos;
    /* 0x964 */ cXyz mHairScale[5];
    /* 0x9A0 */ cXyz field_0x9a0;
    /* 0x9AC */ cXyz mTagWaitPos;
    /* 0x9B8 */ cXyz mDemoPos;
    /* 0x9C4 */ cXyz mForceNeckAimPos;
    /* 0x9D0 */ daKago_c* mpKago;
    /* 0x9D4 */ daMidna_func mpFunc;
};

STATIC_ASSERT(sizeof(daMidna_c) == 0x9E0);

#endif /* D_A_MIDNA_H */
