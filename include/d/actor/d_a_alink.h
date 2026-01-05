#ifndef D_A_D_A_ALINK_H
#define D_A_D_A_ALINK_H

#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "JSystem/J3DGraphBase/J3DMatBlock.h"
#include "Z2AudioLib/Z2WolfHowlMgr.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_tag_mmsg.h"
#include "d/d_eye_hl.h"
#include "d/d_jnt_col.h"
#include "d/d_msg_flow.h"
#include "d/d_particle_copoly.h"
#include "d/d_save.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_camera_mng.h"
#include "res/Object/AlAnm.h"

class J2DAnmColorKey;
class J2DAnmTransformKey;
class J2DScreen;
class J2DPane;
class J2DAnmBase;
class J3DAnmTevRegKey;
class dDemo_actor_c;
class daAlinkHIO_anm_c;
class daScex_c;
class fopEn_enemy_c;

class daAlink_lockCursor_c : public dDlst_base_c {
public:
    daAlink_lockCursor_c() {}
    BOOL create();
    void update();
    virtual void draw();
    virtual ~daAlink_lockCursor_c() {}

    void initFrame() {
        field_0x4 = 0;
        mLockCursorAngle = 4.0f;
        field_0x30 = 0.0f;
    }

    void setPos(f32 i_posX, f32 i_posY) {
        mPosX = i_posX;
        mPosY = i_posY;
    }

public:
    /* 0x04 */ u8 field_0x4;
    /* 0x05 */ u8 field_0x5[3];
    /* 0x08 */ J2DScreen* mScrn;
    /* 0x0C */ J2DPane* field_0xc;
    /* 0x10 */ J2DPane* mCursor0;
    /* 0x14 */ J2DPane* mCursor1;
    /* 0x18 */ J2DPane* mCursor2;
    /* 0x1C */ J2DAnmTevRegKey* field_0x1c;
    /* 0x20 */ J2DAnmTevRegKey* field_0x20;
    /* 0x24 */ J2DAnmColorKey* field_0x24;
    /* 0x28 */ J2DAnmTransformKey* field_0x28;
    /* 0x2C */ f32 mLockCursorAngle;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 mPosX;
    /* 0x38 */ f32 mPosY;
};

class daAlink_sight_c : public daPy_sightPacket_c {
public:
    daAlink_sight_c() {}
    BOOL create();
    void onLockFlg();

    virtual void draw();
    virtual ~daAlink_sight_c() {}

    bool getLockFlg() { return mLockFlag; }
    void offLockFlg() { mLockFlag = false; }

    /* 0x4C */ bool mLockFlag;
    /* 0x4D */ u8 field_0x4d[3];
    /* 0x50 */ daAlink_lockCursor_c mLockCursor;
};

class daAlink_blur_c : public J3DPacket {
public:
    daAlink_blur_c() {}
    void initBlur(f32, int, cXyz const*, cXyz const*, cXyz const*);
    void copyBlur(cXyz const*, cXyz const*, cXyz const*);
    void traceBlur(cXyz const*, cXyz const*, s16);

    virtual void draw();
    virtual ~daAlink_blur_c() {}

    // private:
    /* 0x010 */ ResTIMG* m_blurTex;
    /* 0x014 */ int field_0x14;
    /* 0x018 */ u8 field_0x18[4];
    /* 0x01C */ int field_0x1c;
    /* 0x020 */ int field_0x20;
    /* 0x024 */ f32 field_0x24;
    /* 0x028 */ u8 field_0x28[4];
    /* 0x02C */ cXyz field_0x2c;
    /* 0x038 */ cXyz field_0x38[60];
    /* 0x308 */ cXyz field_0x308[60];
};  // Size = 0x5D8

class dAlink_bottleWaterPcallBack_c : public JPAParticleCallBack {
public:
    dAlink_bottleWaterPcallBack_c() { initialize(0.0f); }
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
    virtual ~dAlink_bottleWaterPcallBack_c() {}

    void initialize(f32 minY) {
        mHitFlg = 0;
        mAppearFlg = 0;
        mKeepMinY = minY;
    }

    s16 getAppearFlg() const { return mAppearFlg; }
    s16 getHitFlg() const { return mHitFlg; }
    cXyz& getHitPos() { return mHitPos; }
    f32 getKeepMinY() { return mKeepMinY; }

    void onAppearFlg() { mAppearFlg = true; }

private:
    /* 0x04 */ s16 mHitFlg;
    /* 0x06 */ s16 mAppearFlg;
    /* 0x08 */ f32 mKeepMinY;
    /* 0x0C */ cXyz mHitPos;
};  // Size = 0x18

class daAlink_footData_c {
public:
    /* 0x00 */ u8 field_0x0;
    /* 0x01 */ u8 field_0x1;
    /* 0x02 */ s16 field_0x2;
    /* 0x04 */ s16 field_0x4;
    /* 0x06 */ s16 field_0x6;
    /* 0x08 */ cXyz field_0x8;
    /* 0x14 */ Mtx field_0x14[3];
};  // Size: 0xA4

STATIC_ASSERT(sizeof(daAlink_footData_c) == 0xA4);

class daAlink_matAnm_c : public J3DMaterialAnm {
public:
    daAlink_matAnm_c() { init(); }

    void init();
    void offSetFlg() { mSetFlag = FALSE; }

    virtual ~daAlink_matAnm_c() {}
    virtual void calc(J3DMaterial*) const;

    static void decMorfFrame() { cLib_calcTimer<u8>(&m_morf_frame); }
    static void setMorfFrame(u8 i_frame) { m_morf_frame = i_frame; }
    static u8 getMorfFrame() { return m_morf_frame; }

    static void onEyeMoveFlg() { m_eye_move_flg = true; }
    static void offEyeMoveFlg() { m_eye_move_flg = false; }
    static bool getEyeMoveFlg() { return m_eye_move_flg; }

    f32* getNowOffsetXP() { return &mNowOffsetX; }
    f32* getNowOffsetYP() { return &mNowOffsetY; }
    void setNowOffsetX(f32 i_offset) { mNowOffsetX = i_offset; }
    void setNowOffsetY(f32 i_offset) { mNowOffsetY = i_offset; }

    static bool m_eye_move_flg;
    static u8 m_morf_frame;

    /* 0x0F4 */ f32 field_0xf4;
    /* 0x0F8 */ f32 field_0xf8;
    /* 0x0FC */ f32 mNowOffsetX;
    /* 0x100 */ f32 mNowOffsetY;
    /* 0x104 */ int mSetFlag;
};

// this class is mostly a complete guess
class daAlink_hsChainLight_c : public dKy_tevstr_c {
public:
    GXLightObj* getLightObj() { return &mLightObj.mLightObj; }
};

struct daAlink_BckData {
    /* 0x0 */ u16 m_underID;
    /* 0x2 */ u16 m_upperID;
};

struct daAlink_FaceTexData {
    /* 0x0 */ u16 m_btpID;
    /* 0x2 */ u16 m_btkID;
};

struct daAlink_AnmData {
    /* 0x0 */ daAlink_BckData field_0x0;
    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 field_0x5;
    /* 0x6 */ u16 m_faceTexID;
    /* 0x8 */ u16 m_faceBckID;
    /* 0xA */ u16 field_0xa;
};  // Size: 0xC

struct daAlink_WlAnmData {
    /* 0x0 */ u16 field_0x0;
    /* 0x2 */ u8 field_0x2;
    /* 0x3 */ u8 field_0x3;
    /* 0x4 */ u16 field_0x4;
    /* 0x6 */ u8 field_0x6;
    /* 0x7 */ u8 field_0x7;
};  // Size: 0x8

class daAlinkHIO_data_c : public JORReflexible {
public:
#if DEBUG
    daAlinkHIO_data_c();
    virtual ~daAlinkHIO_data_c();

    void setStructData(char*);
    void checkDataSize();
    void baseCopy();
    void update();

#if DEBUG
    virtual void listenPropertyEvent(const JORPropertyEvent* event);
#endif
#endif

public:
#if DEBUG
    /* 0x04 */ int m_anm_num;
    /* 0x08 */ int m_u8_num;
    /* 0x0C */ int m_s16_num;
    /* 0x10 */ int m_f32_num;
    /* 0x14 */ int f14;
    /* 0x18 */ int f18;
    /* 0x1C */ int f1c;
    /* 0x20 */ u32 m_len;
    /* 0x24 */ void* mp_src_data;
    /* 0x28 */ void* mp_data;
    /* 0x2C */ const char* m_name_string;
    /* 0x30 */ daAlinkHIO_data_c* mpNext;
#endif
};

struct daAlinkHIO_basic_c1 {
    /* 0x00 */ bool mOneHitKill;
    /* 0x02 */ s16 mHeadMaxTurnHorizontal;
    /* 0x04 */ s16 mHeadMaxTurnUp;
    /* 0x06 */ s16 mHeadMaxTurnDown;
    /* 0x08 */ s16 field_0x08;
    /* 0x0A */ s16 mHotspringRecoverTime;
    /* 0x0C */ s16 mWiiCamSpeedV;
    /* 0x0E */ s16 mWiiCamSpeedH;
    /* 0x10 */ s16 mTransformBlockFarAngle;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 mBasicInterpolation;
    /* 0x1C */ f32 mRotationASpeed;
    /* 0x20 */ f32 mStandingGuardInterpolation;
    /* 0x24 */ f32 mWaterSurfaceEffectHeight;
    /* 0x28 */ f32 mWolfWaterSurfaceEffectHeight;
    /* 0x2C */ f32 mMaxWindInfluenceDist;
    /* 0x30 */ f32 mNoWindInfluenceDist;
    /* 0x34 */ f32 mMaxWindSpeed;
    /* 0x38 */ f32 mLavaDeathDepth;
    /* 0x3C */ f32 mLinkToWolfCancelFrame;
    /* 0x40 */ f32 mWolfToLinkCancelFrame;
    /* 0x44 */ f32 mIndoorSpeedFactor;
    /* 0x48 */ f32 mSandSinkSpeed;
    /* 0x4C */ f32 mSandSurfaceSpeed;
    /* 0x50 */ f32 mTransformBlockNearDis;
    /* 0x54 */ f32 mTransformBlockFarDis;
};

class daAlinkHIO_basic_c0 {
public:
    static daAlinkHIO_basic_c1 const m;
};

class daAlinkHIO_basic_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_basic_c();
    ~daAlinkHIO_basic_c() {}

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_basic_c1 m;
#endif
};

struct daAlinkHIO_anm_c {
    /* 0x00 */ s16 mEndFrame;
    /* 0x04 */ f32 mSpeed;
    /* 0x08 */ f32 mStartFrame;
    /* 0x0C */ f32 mInterpolation;
    /* 0x10 */ f32 mCancelFrame;
};  // size: 0x14

class daAlinkHIO_move_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mSlideAnm;
    /* 0x14 */ s16 mMaxTurnAngle;
    /* 0x16 */ s16 mMinTurnAngle;
    /* 0x18 */ s16 mTurnAngleRate;
    /* 0x1C */ f32 mWaitAnmSpeed;
    /* 0x20 */ f32 mWalkAnmSpeed;
    /* 0x24 */ f32 mRunAnmSpeed;
    /* 0x28 */ f32 mWalkChangeRate;
    /* 0x2C */ f32 mRunChangeRate;
    /* 0x30 */ f32 mMaxSpeed;
    /* 0x34 */ f32 mAcceleration;
    /* 0x38 */ f32 mDeceleration;
    /* 0x3C */ f32 mSlideThresholdSpeed;  // speed needed to trigger a slide
    /* 0x40 */ f32 mSlideSpeed;
    /* 0x44 */ f32 mSlideDeceleration;
    /* 0x48 */ f32 mFootPositionRatio;
    /* 0x4C */ f32 mWaitBInterpolation;
    /* 0x50 */ f32 mMinWalkRate;
    /* 0x54 */ f32 mMinTiredWalkRate;
};  // Size: 0x58

class daAlinkHIO_move_c0 {
public:
    static daAlinkHIO_move_c1 const m;
};

class daAlinkHIO_move_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_move_c();
    ~daAlinkHIO_move_c() {}

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_move_c1 m;
#endif
};

class daAlinkHIO_noActAtnMove_c1 {
public:
    /* 0x00 */ s16 mMaxTurnAngle;
    /* 0x02 */ s16 mMinTurnAngle;
    /* 0x04 */ s16 mTurnAngleRate;
    /* 0x08 */ f32 mWaitAnmSpeed;
    /* 0x0C */ f32 mWalkAnmSpeed;
    /* 0x10 */ f32 mRunAnmSpeed;
    /* 0x14 */ f32 mWalkChangeRate;
    /* 0x18 */ f32 mRunChangeRate;
    /* 0x1C */ f32 mMaxSpeed;
    /* 0x20 */ f32 mAcceleration;
    /* 0x24 */ f32 mDeceleration;
    /* 0x28 */ f32 mBackWalkAnmSpeed;
    /* 0x2C */ f32 mBackRunAnmSpeed;
    /* 0x30 */ f32 mBackWalkChangeRate;
    /* 0x34 */ f32 mBackRunChangeRate;
    /* 0x38 */ f32 mMaxBackwardsSpeed;
    /* 0x3C */ f32 mBackAcceleration;
    /* 0x40 */ f32 mBackDeceleration;
    /* 0x44 */ f32 mMinWalkFrame;
    /* 0x48 */ f32 mMinBackWalkFrame;
    /* 0x4C */ f32 mWalkForwardAnmSpeed;
    /* 0x50 */ f32 mRunForwardAnmSpeed;
};  // Size: 0x54

class daAlinkHIO_noActAtnMove_c0 {
public:
    static daAlinkHIO_noActAtnMove_c1 const m;
};

class daAlinkHIO_noActAtnMove_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_noActAtnMove_c();
    ~daAlinkHIO_noActAtnMove_c() {}

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_noActAtnMove_c1 m;
#endif
};

class daAlinkHIO_frontRoll_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mRollAnm;
    /* 0x14 */ daAlinkHIO_anm_c mCrashAnm;
    /* 0x28 */ daAlinkHIO_anm_c mCrashHitAnm;  // ?
    /* 0x3C */ s16 mCrashAngleThreshold;
    /* 0x3E */ s16 mTurnRate;
    /* 0x40 */ s16 mMaxTurnAngle;
    /* 0x42 */ s16 mTurnMinAngle;
    /* 0x44 */ f32 mInitSpeed;
    /* 0x48 */ f32 mSpeedRate;
    /* 0x4C */ f32 mMinSpeed;
    /* 0x50 */ f32 mCrashSpeedThreshold;
    /* 0x54 */ f32 mCrashInitF;
    /* 0x58 */ f32 mCrashEndF;
    /* 0x5C */ f32 mCrashSpeedH;
    /* 0x60 */ f32 mCrashSpeedV;
    /* 0x64 */ f32 mBootsAttackInitF;
    /* 0x68 */ f32 mBootsAttackEndF;
    /* 0x6C */ f32 mBootsAttackRadius;
};  // Size: 0x70

class daAlinkHIO_frontRoll_c0 {
public:
    static daAlinkHIO_frontRoll_c1 const m;
};

class daAlinkHIO_frontRoll_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_frontRoll_c();
    ~daAlinkHIO_frontRoll_c() {}

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_frontRoll_c1 m;
#endif
};

class daAlinkHIO_backJump_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mBackflipAnm;
    /* 0x14 */ daAlinkHIO_anm_c mLandAnm;
    /* 0x28 */ f32 mBackflipSpeedH;
    /* 0x2C */ f32 mBackflipSpeedV;
    /* 0x30 */ f32 mFallHeight;
    /* 0x34 */ f32 mFallInterpolation;
};  // Size: 0x38

class daAlinkHIO_backJump_c0 {
public:
    static daAlinkHIO_backJump_c1 const m;
};

class daAlinkHIO_backJump_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_backJump_c();
    ~daAlinkHIO_backJump_c() {}

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_backJump_c1 m;
#endif
};

class daAlinkHIO_sideStep_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mSideJumpAnm;
    /* 0x14 */ daAlinkHIO_anm_c mSideLandAnm;
    /* 0x28 */ daAlinkHIO_anm_c mBackJumpAnm;
    /* 0x3C */ daAlinkHIO_anm_c mBackLandAnm;
    /* 0x50 */ f32 mSideJumpSpeedH;
    /* 0x54 */ f32 mSideJumpSpeedV;
    /* 0x58 */ f32 mBackJumpSpeedH;
    /* 0x5C */ f32 mBackJumpSpeedV;
    /* 0x60 */ f32 mFallHeight;
    /* 0x64 */ f32 mFallInterpolation;
};  // Size: 0x68

class daAlinkHIO_sideStep_c0 {
public:
    static daAlinkHIO_sideStep_c1 const m;
};

class daAlinkHIO_sideStep_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_sideStep_c();
    ~daAlinkHIO_sideStep_c() {}

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_sideStep_c1 m;
#endif
};

class daAlinkHIO_slide_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mForwardSlideAnm;
    /* 0x14 */ daAlinkHIO_anm_c mBackwardSlideAnm;
    /* 0x28 */ daAlinkHIO_anm_c mForwardLandAnm;
    /* 0x3C */ daAlinkHIO_anm_c mBackwardLandAnm;
    /* 0x50 */ f32 mMaxSpeed;
    /* 0x54 */ f32 mAcceleration;
    /* 0x58 */ f32 mSlideAngle;
    /* 0x5C */ f32 mClimbAngle;
    /* 0x60 */ f32 mClimbAnmMinSpeed;
    /* 0x64 */ f32 mMaxClimbSpeed;
    /* 0x68 */ f32 mLV2MinibossFloorSlideAngle;
    /* 0x6C */ f32 mLV2MinibossFloorWeakSlideAngle;
    /* 0x70 */ f32 mLV2MinibossFloorWeakSlideSpeed;
    /* 0x74 */ f32 mMaxClimbAnmSpeed;
};  // Size: 0x78

class daAlinkHIO_slide_c0 {
public:
    static daAlinkHIO_slide_c1 const m;
};

class daAlinkHIO_slide_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_slide_c();
    ~daAlinkHIO_slide_c() {}

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_slide_c1 m;
#endif
};

class daAlinkHIO_atnMove_c1 {
public:
    /* 0x00 */ s16 mMaxTurnAngle;
    /* 0x02 */ s16 mMinTurnAngle;
    /* 0x04 */ s16 mTurnAngleRate;
    /* 0x08 */ f32 mWaitAnmSpeed;
    /* 0x0C */ f32 mWalkAnmSpeed;
    /* 0x10 */ f32 mRunAnmSpeed;
    /* 0x14 */ f32 mWalkChangeRate;
    /* 0x18 */ f32 mRunChangeRate;
    /* 0x1C */ f32 mMaxSpeed;
    /* 0x20 */ f32 mAcceleration;
    /* 0x24 */ f32 mDeceleration;
    /* 0x28 */ f32 mBackWalkAnmSpeed;
    /* 0x2C */ f32 mBackRunAnmSpeed;
    /* 0x30 */ f32 mBackWalkChangeRate;
    /* 0x34 */ f32 mBackRunChangeRate;
    /* 0x38 */ f32 mMaxBackwardsSpeed;
    /* 0x3C */ f32 mBackAcceleration;
    /* 0x40 */ f32 mBackDeceleration;
    /* 0x44 */ f32 mMinWalkFrame;
    /* 0x48 */ f32 mMinBackWalkFrame;
    /* 0x4C */ f32 mWalkForwardAnmSpeed;
    /* 0x50 */ f32 mRunForwardAnmSpeed;
};  // Size: 0x54

class daAlinkHIO_atnMove_c0 {
public:
    static daAlinkHIO_atnMove_c1 const m;
};

class daAlinkHIO_atnMove_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_atnMove_c();
    ~daAlinkHIO_atnMove_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_atnMove_c1 m;
#endif
};

class daAlinkHIO_cutNormal_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mCutAnm;
    /* 0x14 */ f32 mSpeed;
    /* 0x18 */ f32 mAttackStartFrame;
    /* 0x1C */ f32 mAttackEndFrame;
};  // Size: 0x20

class daAlinkHIO_cutNmV_c0 {
public:
    static daAlinkHIO_cutNormal_c1 const m;
};

class daAlinkHIO_cutNmL_c0 {
public:
    static daAlinkHIO_cutNormal_c1 const m;
};

class daAlinkHIO_cutNmR_c0 {
public:
    static daAlinkHIO_cutNormal_c1 const m;
};

class daAlinkHIO_cutNmSL_c0 {
public:
    static daAlinkHIO_cutNormal_c1 const m;
};

class daAlinkHIO_cutNmSR_c0 {
public:
    static daAlinkHIO_cutNormal_c1 const m;
};

class daAlinkHIO_cutNormal_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_cutNormal_c(int, float);
    ~daAlinkHIO_cutNormal_c();

    virtual void genMessage(JORMContext*);
#endif

public:
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ daAlinkHIO_cutNormal_c1 m;
};  // Size: 0x58

class daAlinkHIO_cutFinish_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mCutAnm;
    /* 0x14 */ s16 mStopTime;
    /* 0x16 */ s16 mComboStopTime;
    /* 0x18 */ f32 mSpeed;
    /* 0x1C */ f32 mAttackStartFrame;
    /* 0x20 */ f32 mAttackEndFrame;
    /* 0x24 */ f32 mComboCheckFrame;
    /* 0x28 */ f32 mComboStartFrame;
};  // Size: 0x2C

class daAlinkHIO_cutFnL_c0 {
public:
    static daAlinkHIO_cutFinish_c1 const m;
};

class daAlinkHIO_cutFnV_c0 {
public:
    static daAlinkHIO_cutFinish_c1 const m;
};

class daAlinkHIO_cutFnS_c0 {
public:
    static daAlinkHIO_cutFinish_c1 const m;
};

class daAlinkHIO_cutFnSl_c0 {
public:
    static daAlinkHIO_cutFinish_c1 const m;
};

class daAlinkHIO_cutFnSm_c0 {
public:
    static daAlinkHIO_cutFinish_c1 const m;
};

class daAlinkHIO_cutFnR_c0 {
public:
    static daAlinkHIO_cutFinish_c1 const m;
};

class daAlinkHIO_cutFinish_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_cutFinish_c(int, float);
    ~daAlinkHIO_cutFinish_c();

    virtual void genMessage(JORMContext*);
#endif

public:
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ daAlinkHIO_cutFinish_c1 m;
};

class daAlinkHIO_cutFnJU_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mCutAnm;
    /* 0x14 */ daAlinkHIO_anm_c mLandAnm;
    /* 0x28 */ s16 mStopTime;
    /* 0x2A */ s16 mComboStopTime;
    /* 0x2C */ f32 mSpeedH;
    /* 0x30 */ f32 mSpeedV;
    /* 0x34 */ f32 mAttackStartFrame;
    /* 0x38 */ f32 mAttackEndFrame;
    /* 0x3C */ f32 mComboCheckFrame;
    /* 0x40 */ f32 mComboStartFrame;
    /* 0x44 */ f32 mFallHeight;
    /* 0x48 */ f32 mFallInterpolation;
    /* 0x4C */ f32 mAttackRadius;
    /* 0x50 */ f32 mAttackOffset;
};  // Size: 0x54

class daAlinkHIO_cutFnJU_c0 {
public:
    static daAlinkHIO_cutFnJU_c1 const m;
};

class daAlinkHIO_cutFnJU_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_cutFnJU_c();
    ~daAlinkHIO_cutFnJU_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_cutFnJU_c1 m;
#endif
};  // Size: 0x88

class daAlinkHIO_cutDash_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mCutAnm;
    /* 0x14 */ f32 mAttackStartFrame;
    /* 0x18 */ f32 mAttackEndFrame;
};  // Size: 0x1C

class daAlinkHIO_cutDaL_c0 {
public:
    static daAlinkHIO_cutDash_c1 const m;
};

class daAlinkHIO_cutDaR_c0 {
public:
    static daAlinkHIO_cutDash_c1 const m;
};

class daAlinkHIO_cutDaCharge_c0 {
public:
    static daAlinkHIO_cutDash_c1 const m;
};

class daAlinkHIO_cutDash_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_cutDash_c(int, float);
    ~daAlinkHIO_cutDash_c();

    virtual void genMessage(JORMContext*);
#endif

public:
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ daAlinkHIO_cutDash_c1 m;
};

class daAlinkHIO_cutJump_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mCutAnm;
    /* 0x14 */ daAlinkHIO_anm_c mLandAnm;
    /* 0x28 */ f32 mBaseJumpSpeedH;
    /* 0x2C */ f32 mBaseJumpSpeedV;
    /* 0x30 */ f32 mAirJumpSpeedH;
    /* 0x34 */ f32 mAirJumpSpeedV;
    /* 0x38 */ f32 mStartAttackFrame;
    /* 0x3C */ f32 mEndAttackFrame;
    /* 0x40 */ f32 mJumpSpinDelay;
};  // Size: 0x44

class daAlinkHIO_cutJump_c0 {
public:
    static daAlinkHIO_cutJump_c1 const m;
};

class daAlinkHIO_cutJump_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_cutJump_c();
    ~daAlinkHIO_cutJump_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_cutJump_c1 m;
#endif
};  // Size: 0x78

class daAlinkHIO_cutTurn_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mRightTurnAnm;
    /* 0x14 */ daAlinkHIO_anm_c mChargeAnm;
    /* 0x28 */ daAlinkHIO_anm_c mLeftTurnAnm;
    /* 0x3C */ s16 mStopTime;
    /* 0x3E */ s16 field_0x3E;
    /* 0x40 */ f32 mRightTurnInputStartFrame;
    /* 0x44 */ f32 mRightAttackStartFrame;
    /* 0x48 */ f32 mRightAttackEndFrame;
    /* 0x4C */ f32 mMoveFBAnmSpeed;
    /* 0x50 */ f32 mMoveFBInterpolation;
    /* 0x54 */ f32 mMoveLRAnmSpeed;
    /* 0x58 */ f32 mMoveLRInterpolation;
    /* 0x5C */ f32 mMaxSpeed;
    /* 0x60 */ f32 mChargeMoveAccel;
    /* 0x64 */ f32 mChargeMoveDecel;
    /* 0x68 */ f32 mSpeed;
    /* 0x6C */ f32 mRightComboStartFrame;
    /* 0x70 */ f32 mAttackRadius;
    /* 0x74 */ f32 mLeftTurnInputStartFrame;
    /* 0x78 */ f32 mLeftAttackStartFrame;
    /* 0x7C */ f32 mLeftAttackEndFrame;
    /* 0x80 */ f32 mLeftComboStartFrame;
    /* 0x84 */ f32 mAttackRadiusAccel;
    /* 0x88 */ f32 mLightAttackRadius;
    /* 0x8C */ f32 mLightAttackRadiusAccel;
    /* 0x90 */ f32 mLargeAttackRadius;
    /* 0x94 */ f32 mLargeAttackAccel;
};  // Size: 0x98

class daAlinkHIO_cutTurn_c0 {
public:
    static daAlinkHIO_cutTurn_c1 const m;
};

class daAlinkHIO_cutTurn_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_cutTurn_c();
    ~daAlinkHIO_cutTurn_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_cutTurn_c1 m;
#endif
};  // Size: 0xCC

class daAlinkHIO_hoCut_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mCutAnm;
    /* 0x14 */ f32 mAttackStartFrame;
    /* 0x18 */ f32 mAttackEndFrame;
    /* 0x1C */ f32 mAfterCutMorf;
};  // Size: 0x20

class daAlinkHIO_hoCutLA_c0 {
public:
    static daAlinkHIO_hoCut_c1 const m;
};

class daAlinkHIO_hoCutLB_c0 {
public:
    static daAlinkHIO_hoCut_c1 const m;
};

class daAlinkHIO_hoCutRA_c0 {
public:
    static daAlinkHIO_hoCut_c1 const m;
};

class daAlinkHIO_hoCutRB_c0 {
public:
    static daAlinkHIO_hoCut_c1 const m;
};

class daAlinkHIO_hoCut_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_hoCut_c(int, float);
    ~daAlinkHIO_hoCut_c();

    virtual void genMessage(JORMContext*);
#endif

public:
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ daAlinkHIO_hoCut_c1 m;
};  // Size: 0x58

class daAlinkHIO_hoCutCharge_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mChargeAnm;
    /* 0x14 */ daAlinkHIO_anm_c mCutAnm;
    /* 0x28 */ daAlinkHIO_anm_c mSpinAnm;
    /* 0x3C */ s16 mCutStopTime;
    /* 0x3E */ s16 mSpinStopTime;
    /* 0x40 */ f32 mCutAttackStartFrame;
    /* 0x44 */ f32 mCutAttackEndFrame;
    /* 0x48 */ f32 mSpinAttackStartFrame;
    /* 0x4C */ f32 mSpinAttackEndFrame;
    /* 0x50 */ f32 mSpinAttackRadius;
};  // Size: 0x54

class daAlinkHIO_hoCutCharge_c0 {
public:
    static daAlinkHIO_hoCutCharge_c1 const m;
};

class daAlinkHIO_hoCutCharge_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_hoCutCharge_c();
    ~daAlinkHIO_hoCutCharge_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_hoCutCharge_c1 m;
#endif
};  // Size: 0x88

class daAlinkHIO_cutDown_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mJumpAnm;
    /* 0x14 */ daAlinkHIO_anm_c mFallAnm;
    /* 0x28 */ daAlinkHIO_anm_c mLandAnm;
    /* 0x3C */ daAlinkHIO_anm_c mMissAnm;
    /* 0x50 */ daAlinkHIO_anm_c mRecoverAnm;
    /* 0x64 */ s16 mSuccessStopTime;
    /* 0x66 */ s16 mFailStopTime;
    /* 0x68 */ f32 mRecoverSpeedH;
    /* 0x6C */ f32 mRecoverSpeedV;
    /* 0x70 */ f32 mSpeedV;
};  // Size: 0x74

class daAlinkHIO_cutDown_c0 {
public:
    static daAlinkHIO_cutDown_c1 const m;
};

class daAlinkHIO_cutDown_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_cutDown_c();
    ~daAlinkHIO_cutDown_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_cutDown_c1 m;
#endif
};  // Size: 0xA8

class daAlinkHIO_cutHead_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mJumpAnm;
    /* 0x14 */ daAlinkHIO_anm_c mCutAnm;
    /* 0x28 */ daAlinkHIO_anm_c mLandAnm;
    /* 0x3C */ f32 mGravity;
    /* 0x40 */ f32 mMaxHeight;
    /* 0x44 */ f32 mMaxDistance;
    /* 0x48 */ f32 mAddSpeedV;
    /* 0x4C */ f32 mAddSpeedH;
    /* 0x50 */ f32 mAttackStartFrame;
    /* 0x54 */ f32 mAttackEndFrame;
    /* 0x58 */ f32 mSwordRadius;
    /* 0x5C */ f32 mSwordLength;
};  // Size: 0x60

class daAlinkHIO_cutHead_c0 {
public:
    static daAlinkHIO_cutHead_c1 const m;
};

class daAlinkHIO_cutHead_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_cutHead_c();
    ~daAlinkHIO_cutHead_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_cutHead_c1 m;
#endif
};  // Size: 0x94

class daAlinkHIO_cutLargeJump_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mChargeAnm;
    /* 0x14 */ daAlinkHIO_anm_c mChargeMoveAnm;
    /* 0x28 */ daAlinkHIO_anm_c mCutAnm;
    /* 0x3C */ daAlinkHIO_anm_c mLandAnm;
    /* 0x50 */ f32 mCutSpeedH;
    /* 0x54 */ f32 mCutSpeedV;
    /* 0x58 */ f32 mJumpAttackStartFrame;
    /* 0x5C */ f32 mJumpAttackEndFrame;
    /* 0x60 */ f32 mCutInitFrame;
    /* 0x64 */ f32 mLandAttackInitFrame;
    /* 0x68 */ f32 mLandAttackEndFrame;
    /* 0x6C */ f32 mLandAttackRadius;
    /* 0x70 */ f32 mSpinSlashCheckFrame;
};  // Size: 0x74

class daAlinkHIO_cutLargeJump_c0 {
public:
    static daAlinkHIO_cutLargeJump_c1 const m;
};

class daAlinkHIO_cutLargeJump_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_cutLargeJump_c();
    ~daAlinkHIO_cutLargeJump_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_cutLargeJump_c1 m;
#endif
};  // Size: 0xA8

class daAlinkHIO_cut_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mEquipAnm;
    /* 0x14 */ daAlinkHIO_anm_c mUnequipAnm;
    /* 0x28 */ daAlinkHIO_anm_c mRecoilAnm;
    /* 0x3C */ daAlinkHIO_anm_c mStabAnm;
    /* 0x50 */ bool mForceHitCombo;
    /* 0x52 */ s16 mComboDuration;
    /* 0x54 */ s16 mBlurAlpha;
    /* 0x56 */ s16 mNormalSwingDuration;
    /* 0x58 */ s16 mDashBlurAlpha;
    /* 0x5A */ s16 mUnkTime;  // might be related to flourish
    /* 0x5C */ s16 mFlourishTime;
    /* 0x5E */ s16 mUnkBodyDownwards;  // ?
    /* 0x60 */ s16 mUnkBodyUpwards;    // ?
    /* 0x62 */ s16 mSpinSlashWait;     // ? maybe related to wii?
    /* 0x64 */ f32 mRecoilSpeed;
    /* 0x68 */ f32 mRecoilDeceleration;
    /* 0x6C */ f32 mFlourishAnmSpeed;
    /* 0x70 */ f32 mFlourishEndAnmSpeed;
    /* 0x74 */ f32 mSwordLength;
    /* 0x78 */ f32 mSwordRadius;
    /* 0x7C */ f32 mSwordLengthHorseback;
    /* 0x80 */ f32 mSwordRadiusHorseback;
    /* 0x84 */ f32 mRunCutLength;
    /* 0x88 */ f32 mRunCutRadius;
    /* 0x8C */ f32 mFastUnequipAnmSpeed;
    /* 0x90 */ f32 mSwordLengthHorsebackFight;
    /* 0x94 */ f32 mSwordRadiusHorsebackFight;
    /* 0x98 */ f32 mFlourishControlStartFrame;
};  // Size: 0x9C

class daAlinkHIO_cut_c0 {
public:
    static daAlinkHIO_cutNmV_c0 const mCutVertical;
    static daAlinkHIO_cutNmL_c0 const mCutLeft;
    static daAlinkHIO_cutNmR_c0 const mCutRight;
    static daAlinkHIO_cutNmSL_c0 const mCutLeftStab;
    static daAlinkHIO_cutNmSR_c0 const mCutRightStab;
    static daAlinkHIO_cutFnL_c0 const mCutFinishLeft;
    static daAlinkHIO_cutFnV_c0 const mCutFinishVertical;
    static daAlinkHIO_cutFnS_c0 const mCutFinishStab;
    static daAlinkHIO_cutFnSl_c0 const mCutFinishSweep;
    static daAlinkHIO_cutFnSm_c0 const mCutFinishSlash;
    static daAlinkHIO_cutFnR_c0 const mCutFinishRight;
    static daAlinkHIO_cutFnJU_c0 const mCutFinishJumpUppercut;
    static daAlinkHIO_cutJump_c0 const mCutJump;
    static daAlinkHIO_cutTurn_c0 const mCutTurn;
    static daAlinkHIO_hoCutLA_c0 const mHorseCutLeftA;
    static daAlinkHIO_hoCutLB_c0 const mHorseCutLeftB;
    static daAlinkHIO_hoCutRA_c0 const mHorseCutRightA;
    static daAlinkHIO_hoCutRB_c0 const mHorseCutRightB;
    static daAlinkHIO_hoCutCharge_c0 const mHorseCutCharge;
    static daAlinkHIO_cutDaL_c0 const mCutDashLeft;
    static daAlinkHIO_cutDaR_c0 const mCutDashRight;
    static daAlinkHIO_cutDaCharge_c0 const mCutDashCharge;
    static daAlinkHIO_cutDown_c0 const mCutDown;
    static daAlinkHIO_cutHead_c0 const mCutHead;
    static daAlinkHIO_cutLargeJump_c0 const mCutLargeJump;
    static daAlinkHIO_cut_c1 const m;
};

class daAlinkHIO_cut_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_cut_c();
    ~daAlinkHIO_cut_c();

    virtual void genMessage(JORMContext*);
#else
    ~daAlinkHIO_cut_c();
#endif

public:
#if DEBUG
    /* 0x034 */ daAlinkHIO_cutNormal_c mCutVertical;
    /* 0x08C */ daAlinkHIO_cutNormal_c mCutLeft;
    /* 0x0E4 */ daAlinkHIO_cutNormal_c mCutRight;
    /* 0x13C */ daAlinkHIO_cutNormal_c mCutLeftStab;
    /* 0x194 */ daAlinkHIO_cutNormal_c mCutRightStab;
    /* 0x1EC */ daAlinkHIO_cutFinish_c mCutFinishLeft;
    /* 0x250 */ daAlinkHIO_cutFinish_c mCutFinishVertical;
    /* 0x2B4 */ daAlinkHIO_cutFinish_c mCutFinishStab;
    /* 0x318 */ daAlinkHIO_cutFinish_c mCutFinishSweep;
    /* 0x37C */ daAlinkHIO_cutFinish_c mCutFinishSlash;
    /* 0x3E0 */ daAlinkHIO_cutFinish_c mCutFinishRight;
    /* 0x444 */ daAlinkHIO_cutFnJU_c mCutFinishJumpUppercut;
    /* 0x4CC */ daAlinkHIO_cutJump_c mCutJump;
    /* 0x544 */ daAlinkHIO_cutTurn_c mCutTurn;
    /* 0x610 */ daAlinkHIO_hoCut_c mHorseCutLeftA;
    /* 0x668 */ daAlinkHIO_hoCut_c mHorseCutLeftB;
    /* 0x6C0 */ daAlinkHIO_hoCut_c mHorseCutRightA;
    /* 0x718 */ daAlinkHIO_hoCut_c mHorseCutRightB;
    /* 0x770 */ daAlinkHIO_hoCutCharge_c mHorseCutCharge;
    /* 0x7F8 */ daAlinkHIO_cutDash_c mCutDashLeft;
    /* 0x84C */ daAlinkHIO_cutDash_c mCutDashRight;
    /* 0x8A0 */ daAlinkHIO_cutDash_c mCutDashCharge;
    /* 0x8F4 */ daAlinkHIO_cutDown_c mCutDown;
    /* 0x99C */ daAlinkHIO_cutHead_c mCutHead;
    /* 0xA30 */ daAlinkHIO_cutLargeJump_c mCutLargeJump;
    /* 0xAD8 */ daAlinkHIO_cut_c1 m;
#endif
};  // Size: 0xB74

class daAlinkHIO_guardAttack_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mAttackAnm;
    /* 0x14 */ f32 mSpeed;
    /* 0x18 */ f32 mAttackStartFrame;
    /* 0x1C */ f32 mAttackEndFrame;
    /* 0x20 */ f32 mSlashCheckFrame;
};  // Size: 0x24

class daAlinkHIO_gAtPush_c0 {
public:
    static daAlinkHIO_guardAttack_c1 const m;
};

class daAlinkHIO_gAtKick_c0 {
public:
    static daAlinkHIO_guardAttack_c1 const m;
};

class daAlinkHIO_guardAttack_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_guardAttack_c(int, float);
    ~daAlinkHIO_guardAttack_c();

    virtual void genMessage(JORMContext*);
#endif

public:
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ daAlinkHIO_guardAttack_c1 m;
};

class daAlinkHIO_turnMove_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mTurnAnm;
    /* 0x14 */ s16 mTurnSpeedRate;
    /* 0x16 */ s16 mMaxTurnSpeed;
    /* 0x18 */ s16 mMinTurnSpeed;
    /* 0x1A */ s16 mMaxHalfTurnSpeed;
    /* 0x1C */ f32 mHalfTurnAnmSpeed;
    /* 0x20 */ f32 mTwirlCutDelayF;
    /* 0x24 */ f32 mSideRollAnmSpeed;
    /* 0x28 */ f32 mSideRollSpeed;
};  // Size: 0x2C

class daAlinkHIO_turnMove_c0 {
public:
    static daAlinkHIO_turnMove_c1 const m;
};

class daAlinkHIO_turnMove_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_turnMove_c();
    ~daAlinkHIO_turnMove_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_turnMove_c1 m;
#endif
};

class daAlinkHIO_guard_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mGuardHitAnm;
    /* 0x14 */ daAlinkHIO_anm_c mCrouchGuardHitAnm;  // Unused
    /* 0x28 */ daAlinkHIO_anm_c mRecoilAnm;
    /* 0x3C */ daAlinkHIO_anm_c mGuardBreakAnm;
    /* 0x50 */ s16 mGuardLRAngleMax;
    /* 0x52 */ s16 mGuardFBAngleMax;
    /* 0x54 */ s16 mGuardBodyInterpolation;
    /* 0x56 */ s16 mSmallGuardLRAngleMax;
    /* 0x58 */ s16 mSmallGuardFBAngleMax;
    /* 0x5C */ f32 mCrouchGuardAnmSpeed;       // Unused
    /* 0x60 */ f32 mCrouchGuardInterpolation;  // Unused
    /* 0x64 */ f32 mGuardSpeedNormal;
    /* 0x68 */ f32 mGuardSpeedLarge;
    /* 0x6C */ f32 mGuardSpeedHuge;
    /* 0x70 */ f32 mAttackPosOffset;
    /* 0x74 */ f32 mAttackRadius;
    /* 0x78 */ f32 mMagneGuardSpeed;
    /* 0x7C */ f32 mMagneHvyGuardSpeed;
};  // Size: 0x80

class daAlinkHIO_guard_c0 {
public:
    static daAlinkHIO_gAtPush_c0 const mAtPush;
    static daAlinkHIO_gAtKick_c0 const mAtKick;
    static daAlinkHIO_turnMove_c0 const mTurnMove;
    static daAlinkHIO_guard_c1 const m;
};

class daAlinkHIO_guard_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_guard_c();
    ~daAlinkHIO_guard_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x034 */ daAlinkHIO_guardAttack_c mAtPush;
    /* 0x090 */ daAlinkHIO_guardAttack_c mAtKick;
    /* 0x0EC */ daAlinkHIO_turnMove_c mTurnMove;
    /* 0x14C */ daAlinkHIO_guard_c1 m;
#endif
};

class daAlinkHIO_crouch_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mCrawlStartAnm;
    /* 0x14 */ daAlinkHIO_anm_c mCrawlEndAnm;
    /* 0x28 */ s16 mCrawlTurnRate;
    /* 0x2A */ s16 mCrawlTurnMax;
    /* 0x2C */ s16 mCrawlTurnMin;
    /* 0x30 */ f32 mCrouchAnmSpeed;
    /* 0x34 */ f32 mCrouchInterpolation;
    /* 0x38 */ f32 mStandInterpolation;
    /* 0x3C */ f32 mCrawlMoveRate;
    /* 0x40 */ f32 mCrawlAnmSpeedMax;
    /* 0x44 */ f32 mCrawlAnmSpeedMin;
    /* 0x48 */ f32 mCrawlInterpolation;
};  // Size: 0x4C

class daAlinkHIO_crouch_c0 {
public:
    static daAlinkHIO_crouch_c1 const m;
};

class daAlinkHIO_crouch_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_crouch_c();
    ~daAlinkHIO_crouch_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_crouch_c1 m;
#endif
};

class daAlinkHIO_autoJump_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mJumpAnm;
    /* 0x14 */ daAlinkHIO_anm_c mLandAnm;
    /* 0x28 */ daAlinkHIO_anm_c mDiveAnm;
    /* 0x3C */ daAlinkHIO_anm_c mDiveConnectAnm;
    /* 0x50 */ bool mAlwaysMaxSpeedJump;
    /* 0x52 */ s16 mJumpAngle;
    /* 0x54 */ s16 mSpinJumpRotateSpeed;
    /* 0x56 */ s16 mSpinJumpLandStopTime;
    /* 0x58 */ s16 mCuccoJumpAngle;
    /* 0x5C */ f32 mJumpSpeedLimit;
    /* 0x60 */ f32 mMinJumpSpeed;
    /* 0x64 */ f32 mJumpSpeedRate;
    /* 0x68 */ f32 mAirborneInterpolation;
    /* 0x6C */ f32 mJumpFallInterpolation;
    /* 0x70 */ f32 mFallInterpolation;
    /* 0x74 */ f32 mGravity;
    /* 0x78 */ f32 mMaxFallSpeed;
    /* 0x7C */ f32 mMaxJumpSpeed;
    /* 0x80 */ f32 mSpinJumpInterpolation;
    /* 0x84 */ f32 mSpinJumpFallInterpolation;
    /* 0x88 */ f32 mSpinJumpAddSpeed;
    /* 0x8C */ f32 mSpinJumpAccel;
    /* 0x90 */ f32 mHangHeightLimit;
    /* 0x94 */ f32 mGrabHeightLimit;
    /* 0x98 */ f32 mOoccooJumpMaxSpeed;
    /* 0x9C */ f32 mDiveSpeedV;
    /* 0xA0 */ f32 mDiveSpeedH;
    /* 0xA4 */ f32 mDiveGravity;
    /* 0xA8 */ f32 mCuccoJumpMaxSpeed;
    /* 0xAC */ f32 mCuccoFallMaxSpeed;
    /* 0xB0 */ f32 mCuccoStartSpeed;
};  // size = 0xB4

class daAlinkHIO_autoJump_c0 {
public:
    static daAlinkHIO_autoJump_c1 const m;
};

class daAlinkHIO_autoJump_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_autoJump_c();
    ~daAlinkHIO_autoJump_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    daAlinkHIO_autoJump_c1 m;
#endif
};

class daAlinkHIO_smallJump_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mSmallJumpAnm;
    /* 0x14 */ daAlinkHIO_anm_c mStepClimbAnm;
    /* 0x28 */ daAlinkHIO_anm_c mStepLandAnm;
    /* 0x3C */ f32 mSpeedH;
    /* 0x40 */ f32 mTargetHeightOffset;
    /* 0x44 */ f32 mFallInterpolation;
};  // Size: 0x48

class daAlinkHIO_smallJump_c0 {
public:
    static daAlinkHIO_smallJump_c1 const m;
};

class daAlinkHIO_smallJump_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_smallJump_c();
    ~daAlinkHIO_smallJump_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    daAlinkHIO_smallJump_c1 m;
#endif
};

class daAlinkHIO_wallCatch_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mGrabAAnm;
    /* 0x14 */ daAlinkHIO_anm_c mGrabBAnm;
    /* 0x28 */ daAlinkHIO_anm_c mClimbAnm;
    /* 0x3C */ daAlinkHIO_anm_c mJumpAnm;
    /* 0x50 */ f32 mClimbStartFrame;
};  // Size: 0x54

class daAlinkHIO_wallCatch_c0 {
public:
    static daAlinkHIO_wallCatch_c1 const m;
};

class daAlinkHIO_wallCatch_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wallCatch_c();
    ~daAlinkHIO_wallCatch_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    daAlinkHIO_wallCatch_c1 m;
#endif
};

class daAlinkHIO_wallFall_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mOneHandGrabAnm;
    /* 0x14 */ daAlinkHIO_anm_c mTwoHandGrabAnm;
};  // Size: 0x28

class daAlinkHIO_wallFall_c0 {
public:
    static daAlinkHIO_wallFall_c1 const m;
};

class daAlinkHIO_wallFall_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wallFall_c();
    ~daAlinkHIO_wallFall_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    daAlinkHIO_wallFall_c1 m;
#endif
};

class daAlinkHIO_wallMove_c1 {
public:
    /* 0x00 */ f32 mMinAnmSpeed;
    /* 0x04 */ f32 mMaxAnmSpeed;
    /* 0x08 */ f32 mInterpolation;
    /* 0x0C */ f32 mMinSpeed;
    /* 0x10 */ f32 mMaxSpeed;
};  // Size: 0x14

class daAlinkHIO_wallMove_c0 {
public:
    static daAlinkHIO_wallMove_c1 const m;
};

class daAlinkHIO_wallMove_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wallMove_c();
    ~daAlinkHIO_wallMove_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    daAlinkHIO_wallMove_c1 m;
#endif
};

class daAlinkHIO_wallHang_c1 {
public:
    /* 0x00 */ s16 small_jump_input_time;
    /* 0x02 */ s16 grab_input_time;
    /* 0x04 */ f32 auto_walk_height;
    /* 0x08 */ f32 small_jump_height;
    /* 0x0C */ f32 climb_height;
    /* 0x10 */ f32 jump_climb_height;
    /* 0x14 */ f32 jump_hang_height;
    /* 0x18 */ f32 hang_foot_pos_height;
};  // Size: 0x1C

class daAlinkHIO_wallHang_c0 {
public:
    static daAlinkHIO_smallJump_c0 const mSmallJump;
    static daAlinkHIO_wallCatch_c0 const mWallCatch;
    static daAlinkHIO_wallFall_c0 const mWallFall;
    static daAlinkHIO_wallMove_c0 const mWallMove;
    static daAlinkHIO_wallHang_c1 const m;
};

class daAlinkHIO_wallHang_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wallHang_c();
    ~daAlinkHIO_wallHang_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_smallJump_c mSmallJump;
    /* 0x00 */ daAlinkHIO_wallCatch_c mWallCatch;
    /* 0x00 */ daAlinkHIO_wallFall_c mWallFall;
    /* 0x00 */ daAlinkHIO_wallMove_c mWallMove;
    /* 0x00 */ daAlinkHIO_wallHang_c1 m;
#endif
};

class daAlinkHIO_pushpull_c1 {
public:
    /* 0x00 */ u8 mEnableSumoWaitRotation;
    /* 0x02 */ s16 mSumoMaxGauge;
    /* 0x04 */ s16 mSumoIncreaseGauge;
    /* 0x06 */ s16 mSumoPenaltyTime;
    /* 0x08 */ s16 mSumoPower3To2Switch;
    /* 0x0A */ s16 mSumoPower2To1Switch;
    /* 0x0C */ s16 mSumoRotationAllowPower;
    /* 0x0E */ s16 mSumoRotationAngle;
    /* 0x10 */ f32 mStandbyASpeed;
    /* 0x14 */ f32 mStandbyInterpolation;
    /* 0x18 */ f32 mPushASpeed;
    /* 0x1C */ f32 mPushInterpolation;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 mPullInterpolation;
    /* 0x28 */ f32 mSumoPushASpeed;
    /* 0x2C */ f32 mSumoPushAddASpeed;
    /* 0x30 */ f32 mSumoPushedASpeed;
    /* 0x34 */ f32 mSumoPushedAddASpeed;
    /* 0x38 */ f32 mPushASpeedHeavy;
};  // Size: 0x3C

class daAlinkHIO_pushpull_c0 {
public:
    static daAlinkHIO_pushpull_c1 const m;
};

class daAlinkHIO_pushpull_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_pushpull_c();
    ~daAlinkHIO_pushpull_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    daAlinkHIO_pushpull_c1 m;
#endif
};

class daAlinkHIO_damNormal_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mFrontAnm;
    /* 0x14 */ daAlinkHIO_anm_c mRearAnm;
    /* 0x28 */ daAlinkHIO_anm_c mLeftAnm;
    /* 0x3C */ daAlinkHIO_anm_c mRightAnm;
    /* 0x50 */ s16 mFrontBackBodyMaxAngle;
    /* 0x52 */ s16 mLeftRightBodyMaxAngle;
    /* 0x54 */ f32 mInitialSpeed;
    /* 0x58 */ f32 mAttackSpeedRate;
    /* 0x5C */ f32 mDeceleration;
    /* 0x60 */ f32 mIceDamageASpeed;
};  // Size: 0x64

class daAlinkHIO_damNormal_c0 {
public:
    static daAlinkHIO_damNormal_c1 const m;
};

class daAlinkHIO_damNormal_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_damNormal_c();
    ~daAlinkHIO_damNormal_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    daAlinkHIO_damNormal_c1 m;
#endif
};

class daAlinkHIO_damLaHu_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mFrontGetUpAnm;
    /* 0x14 */ daAlinkHIO_anm_c mBackGetUpAnm;
    /* 0x28 */ daAlinkHIO_anm_c mLeftGetUpAnm;
    /* 0x3C */ daAlinkHIO_anm_c mRightGetUpAnm;
    /* 0x50 */ daAlinkHIO_anm_c mFrontWallHitAnm;
    /* 0x64 */ daAlinkHIO_anm_c mBackWallHitAnm;
    /* 0x78 */ daAlinkHIO_anm_c mLeftWallHitAnm;
    /* 0x8C */ daAlinkHIO_anm_c mRightWallHitAnm;
    /* 0xA0 */ s16 mBodyRotateRate;
    /* 0xA4 */ f32 mGravity;
    /* 0xA8 */ f32 mDamageBlend;
    /* 0xAC */ f32 mHorizontalSpeed;
    /* 0xB0 */ f32 mVerticalSpeed;
    /* 0xB4 */ f32 mBounceSpeed;
    /* 0xB8 */ f32 mDeceleration;
};  // Size: 0xBC

class daAlinkHIO_damLarge_c0 {
public:
    static daAlinkHIO_damLaHu_c1 const m;
};

class daAlinkHIO_damHuge_c0 {
public:
    static daAlinkHIO_damLaHu_c1 const m;
};

class daAlinkHIO_damLaHu_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_damLaHu_c(int);
    ~daAlinkHIO_damLaHu_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    daAlinkHIO_damLaHu_c1 m;
#endif
};

class daAlinkHIO_damHorse_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mNoDirectionAnm;
    /* 0x14 */ daAlinkHIO_anm_c mWithDirectionAnm;
    /* 0x28 */ s16 mFrontBackBodyMaxAngle;
    /* 0x2A */ s16 mLeftRightBodyMaxAngle;
};  // Size: 0x2C

class daAlinkHIO_damHorse_c0 {
public:
    static daAlinkHIO_damHorse_c1 const m;
};

class daAlinkHIO_damHorse_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_damHorse_c();
    ~daAlinkHIO_damHorse_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_damHorse_c1 m;
#endif
};

class daAlinkHIO_damFall_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mLandAnm;
    /* 0x14 */ daAlinkHIO_anm_c mStandAnm;
    /* 0x28 */ s16 mSmallStopTime;
    /* 0x2A */ s16 mBigStopTime;
    /* 0x2C */ f32 mMinRollHeight;
    /* 0x30 */ f32 mMaxRollHeight;
    /* 0x34 */ f32 mSmallDmgHeight;
    /* 0x38 */ f32 mBigDmgHeight;
    /* 0x3C */ f32 mSmallDmgLandStartFrame;
    /* 0x40 */ f32 mFallAnmTransitionHeight;
    /* 0x44 */ f32 mFallAnmMorf;
};  // Size: 0x48

class daAlinkHIO_damFall_c0 {
public:
    static daAlinkHIO_damFall_c1 const m;
};

class daAlinkHIO_damFall_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_damFall_c();
    ~daAlinkHIO_damFall_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_damFall_c1 m;
#endif
};

class daAlinkHIO_damCaught_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mEscapeAnm;
    /* 0x14 */ s16 mGrabDuration;
    /* 0x16 */ s16 mInputFadeTime;
    /* 0x18 */ f32 mStandbySpeed;
    /* 0x1C */ f32 mStandbyInterp;
    /* 0x20 */ f32 mStandbyAddASpeed;
};  // Size: 0x24

class daAlinkHIO_damCaught_c0 {
public:
    static daAlinkHIO_damCaught_c1 const m;
};

class daAlinkHIO_damCaught_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_damCaught_c();
    ~daAlinkHIO_damCaught_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_damCaught_c1 m;
#endif
};

class daAlinkHIO_damSwim_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mFrontAnm;
    /* 0x14 */ daAlinkHIO_anm_c mRearAnm;
    /* 0x28 */ daAlinkHIO_anm_c mLeftAnm;
    /* 0x3C */ daAlinkHIO_anm_c mRightAnm;
    /* 0x50 */ daAlinkHIO_anm_c mSinkAnm;
    /* 0x64 */ daAlinkHIO_anm_c mSurfaceAnm;
    /* 0x78 */ s16 mFrontBackBodyMaxAngle;
    /* 0x7A */ s16 mLeftRightBodyMaxAngle;
    /* 0x7C */ f32 mInitialSpeed;
    /* 0x80 */ f32 mAttackSpeedRate;
    /* 0x84 */ f32 mDeceleration;
    /* 0x88 */ f32 mMaxSpeed;
};  // Size: 0x8C

class daAlinkHIO_damSwim_c0 {
public:
    static daAlinkHIO_damSwim_c1 const m;
};

class daAlinkHIO_damSwim_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_damSwim_c();
    ~daAlinkHIO_damSwim_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_damSwim_c1 m;
#endif
};

class daAlinkHIO_damage_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mFloorDmgAnm;
    /* 0x14 */ daAlinkHIO_anm_c mDashDmgAnm;
    /* 0x28 */ u8 mInvincible;
    /* 0x2A */ s16 mInvincibleTime;
    /* 0x2C */ s16 mWolfFloorInvincibleTime;
    /* 0x2E */ s16 mFreezeTime;
    /* 0x30 */ s16 mFreezeInitR;
    /* 0x32 */ s16 mFreezeInitG;
    /* 0x34 */ s16 mFreezeInitB;
    /* 0x36 */ s16 mDamageR0;
    /* 0x38 */ s16 mDamageG0;
    /* 0x3A */ s16 mDamageB0;
    /* 0x3C */ s16 mDamageR1;
    /* 0x3E */ s16 mDamageG1;
    /* 0x40 */ s16 mDamageB1;
    /* 0x42 */ s16 mDamageR2;
    /* 0x44 */ s16 mDamageG2;
    /* 0x46 */ s16 mDamageB2;
    /* 0x48 */ s16 mFreezeR;
    /* 0x4A */ s16 mFreezeG;
    /* 0x4C */ s16 mFreezeB;
    /* 0x50 */ f32 mRecoverStandAnmSpeed;
    /* 0x54 */ f32 mInvertedFallInterpolation;
};  // Size: 0x58

class daAlinkHIO_damage_c0 {
public:
    static daAlinkHIO_damage_c1 const m;
    static daAlinkHIO_damNormal_c0 const mDamNormal;
    static daAlinkHIO_damLarge_c0 const mDamLarge;
    static daAlinkHIO_damHuge_c0 const mDamHuge;
    static daAlinkHIO_damHorse_c0 const mDamHorse;
    static daAlinkHIO_damFall_c0 const mDamFall;
    static daAlinkHIO_damCaught_c0 const mDamCaught;
    static daAlinkHIO_damSwim_c0 const mDamSwim;
};

class daAlinkHIO_damage_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_damage_c();
    ~daAlinkHIO_damage_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_damage_c1 m;
    /* 0x34 */ daAlinkHIO_damNormal_c mDamNormal;
    /* 0x00 */ daAlinkHIO_damLaHu_c mDamLarge;
    /* 0x00 */ daAlinkHIO_damLaHu_c mDamHuge;
    /* 0x00 */ daAlinkHIO_damHorse_c mDamHorse;
    /* 0x00 */ daAlinkHIO_damFall_c mDamFall;
    /* 0x00 */ daAlinkHIO_damCaught_c mDamCaught;
    /* 0x00 */ daAlinkHIO_damSwim_c mDamSwim;
#endif
};

class daAlinkHIO_horse_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mWalkOutAnm;
    /* 0x14 */ daAlinkHIO_anm_c mRunOutAnm;
    /* 0x28 */ daAlinkHIO_anm_c mWhipAnm;
    /* 0x3C */ daAlinkHIO_anm_c mWhipRunOutAnm;
    /* 0x50 */ s16 mSwordUpTime;
    /* 0x52 */ s16 mSearchRangeAngle;
    /* 0x54 */ s16 mWalkOutProhibitionTime;
    /* 0x56 */ s16 mWhipWaitTime;
    /* 0x58 */ s16 mSubjectiveDownwardMaxAngle;
    /* 0x5A */ s16 mSubjectiveUpwardMaxAngle;
    /* 0x5C */ f32 mSwordUpAnmSpeed;
    /* 0x60 */ f32 mSwordUpInterpolation;
    /* 0x64 */ f32 mHorseWalkStartFrame;
    /* 0x68 */ f32 mTiredWaitInterpolation;
};  // Size: 0x6C

class daAlinkHIO_horse_c0 {
public:
    static daAlinkHIO_horse_c1 const m;
};

class daAlinkHIO_horse_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_horse_c();
    ~daAlinkHIO_horse_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_horse_c1 m;
#endif
};

class daAlinkHIO_canoe_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mLeftRightChangeAnm;
    /* 0x14 */ s16 mMaxTurnAngle;
    /* 0x16 */ s16 mMaxTurnAngle_RiverRide;
    /* 0x18 */ f32 mStickRowAnmSpeed;
    /* 0x1C */ f32 mBtnRowAnmSpeed;
    /* 0x20 */ f32 mSpeedRate;
    /* 0x24 */ f32 mMaxSpeed;
    /* 0x28 */ f32 mDeceleration;
    /* 0x2C */ f32 mStickRowStartAnmSpeed;
    /* 0x30 */ f32 mBtnRowStartAnmSpeed;
    /* 0x34 */ f32 mBackSpeedRate;
    /* 0x38 */ f32 mBackMaxSpeed;
    /* 0x3C */ f32 mMaxSpeed_RiverRide;
    /* 0x40 */ f32 mSpeedRate_RiverRide;
    /* 0x44 */ f32 mDeceleration_RiverRide;
};  // Size: 0x48

class daAlinkHIO_canoe_c0 {
public:
    static daAlinkHIO_canoe_c1 const m;
};

class daAlinkHIO_canoe_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_canoe_c();
    ~daAlinkHIO_canoe_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_canoe_c1 m;
#endif
};

class daAlinkHIO_bow_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mShootAnm;
    /* 0x14 */ daAlinkHIO_anm_c mLoadAnm;
    /* 0x28 */ s16 mChargeArrowTime;
    /* 0x2A */ s16 mBombArrowHoldTime;
    /* 0x2C */ s16 mBombArrowFlyExplodeTime;
    /* 0x30 */ f32 mStartInterpolation;
    /* 0x34 */ f32 mArrowSpeed;
    /* 0x38 */ f32 mArrowDistance;
    /* 0x3C */ f32 mChargeArrowSpeed;
    /* 0x40 */ f32 mChargeArrowDistance;
    /* 0x44 */ f32 mArrowAttackRadius;
    /* 0x48 */ f32 mBombArrowSpeed;
    /* 0x4C */ f32 mBombArrowDistance;
    /* 0x50 */ f32 mChargeBombArrowSpeed;
    /* 0x54 */ f32 mChargeBombArrowDistance;
    /* 0x58 */ f32 mScopeArrowSpeed;
    /* 0x5C */ f32 mScopeArrowDistance;
    /* 0x60 */ f32 mArrowIncAttackMaxStart;
    /* 0x64 */ f32 mArrowIncAttackMax;
    /* 0x68 */ f32 mSlingshotSpeed;
    /* 0x6C */ f32 mSlingshotDistance;
};  // Size: 0x70

class daAlinkHIO_bow_c0 {
public:
    static daAlinkHIO_bow_c1 const m;
};

class daAlinkHIO_bow_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_bow_c();
    ~daAlinkHIO_bow_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_bow_c1 m;
#endif
};

class daAlinkHIO_boom_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mThrowAnm;
    /* 0x14 */ daAlinkHIO_anm_c mCatchAnm;
    /* 0x28 */ s16 mChargeTime;
    /* 0x2A */ s16 mBgThroughTime;
    /* 0x2C */ f32 mIdleAnmSpeed;
    /* 0x30 */ f32 mStartInterpolation;
    /* 0x34 */ f32 mFlySpeed;
    /* 0x38 */ f32 mChargeFlySpeed;
    /* 0x3C */ f32 mFlyDistMax;
    /* 0x40 */ f32 mHorsebackFlyDistMax;
    /* 0x44 */ f32 mLockDistMax;
    /* 0x48 */ f32 mCatchSpeed;
};  // Size: 0x4C

class daAlinkHIO_boom_c0 {
public:
    static daAlinkHIO_boom_c1 const m;
};

class daAlinkHIO_boom_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_boom_c();
    ~daAlinkHIO_boom_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_boom_c1 m;
#endif
};

class daAlinkHIO_bomb_c1 {
public:
    /* 0x00 */ s16 mExplodeTime;
    /* 0x02 */ s16 mEnemyBombColorR;
    /* 0x04 */ s16 mBombInsectLimitAngle;
    /* 0x08 */ f32 mGravity;
    /* 0x0C */ f32 mMaxFallSpeed;
    /* 0x10 */ f32 mBoundRate;
    /* 0x14 */ f32 mStopSpeedY;
    /* 0x18 */ f32 mMaxSpeedY;
    /* 0x1C */ f32 mEffScale;
    /* 0x20 */ f32 mAttackRadius;
    /* 0x24 */ f32 mPokeBombTrackRate;
    /* 0x28 */ f32 mWaterGravity;
    /* 0x2C */ f32 mWaterMaxFallSpeed;
    /* 0x30 */ f32 mThrowSpeedH;
    /* 0x34 */ f32 mThrowSpeedV;
    /* 0x38 */ f32 mWaterThrowSpeedH;
    /* 0x3C */ f32 mWaterThrowSpeedV;
    /* 0x40 */ f32 mWolfThrowSpeedH;
    /* 0x44 */ f32 mWolfThrowSpeedV;
    /* 0x48 */ f32 mExplodeWaterEffectLimit;
};  // Size: 0x4C

class daAlinkHIO_bomb_c0 {
public:
    static daAlinkHIO_bomb_c1 const m;
};

class daAlinkHIO_bomb_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_bomb_c();
    ~daAlinkHIO_bomb_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_bomb_c1 m;
#endif
};

class daAlinkHIO_huLight_c1 {
public:
    /* 0x00 */ u8 mAngleAttenuationType;
    /* 0x01 */ u8 mDistanceAttenuationType;
    /* 0x02 */ s16 mAlpha;
    /* 0x04 */ s16 mColorR;
    /* 0x06 */ s16 mColorG;
    /* 0x08 */ s16 mColorB;
    /* 0x0A */ s16 mXAngle;
    /* 0x0C */ f32 mPower;
    /* 0x10 */ f32 mWidth;
    /* 0x14 */ f32 mYOffset;
    /* 0x18 */ f32 mZOffset;
};  // Size: 0x1C

class daAlinkHIO_huLight_c0 {
public:
    static daAlinkHIO_huLight_c1 const m;
};

class daAlinkHIO_wlLight_c1 {
public:
    /* 0x00 */ s16 field_0x0;
    /* 0x02 */ s16 field_0x2;
    /* 0x04 */ s16 field_0x4;
    /* 0x06 */ s16 field_0x6;
    /* 0x08 */ s16 field_0x8;
    /* 0x0A */ s16 field_0xA;
    /* 0x0C */ f32 field_0xC;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
};  // Size: 0x1C

class daAlinkHIO_wlLight_c0 {
public:
    static daAlinkHIO_wlLight_c1 const m;
};

class daAlinkHIO_zwLight_c1 {  // may be wrong
public:
    /* 0x00 */ s16 field_0x0;
    /* 0x02 */ s16 field_0x2;
    /* 0x04 */ s16 field_0x4;
    /* 0x06 */ s16 field_0x6;
    /* 0x08 */ s16 field_0x8;
    /* 0x0A */ s16 field_0xA;
    /* 0x0C */ f32 field_0xC;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
};  // Size: 0x1C

class daAlinkHIO_zwLight_c0 {
public:
    static daAlinkHIO_zwLight_c1 const m;
};

class daAlinkHIO_light_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_light_c(int);
    ~daAlinkHIO_light_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_huLight_c1 m;
#endif
};

class daAlinkHIO_kandelaar_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mShakeAnm;
    /* 0x14 */ daAlinkHIO_anm_c mBeginUnkAnm;
    /* 0x28 */ daAlinkHIO_anm_c mEndUnkAnm;
    /* 0x3C */ s16 mColorReg1R;
    /* 0x3E */ s16 mColorReg1G;
    /* 0x40 */ s16 mColorReg1B;
    /* 0x42 */ s16 mColorReg2R;
    /* 0x44 */ s16 mColorReg2G;
    /* 0x46 */ s16 mColorReg2B;
    /* 0x48 */ s16 mNormalOilLoss;
    /* 0x4A */ s16 mShakeOilLoss;
    /* 0x4C */ f32 mFlameTrackRate;
};  // Size: 0x50

class daAlinkHIO_kandelaar_c0 {
public:
    static daAlinkHIO_kandelaar_c1 const m;
};

class daAlinkHIO_kandelaar_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_kandelaar_c();
    ~daAlinkHIO_kandelaar_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_kandelaar_c1 m;
#endif
};

class daAlinkHIO_magneBoots_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mEquipAnm;
    /* 0x14 */ f32 mInputFactor;
    /* 0x18 */ f32 mFeetPositionRatio;
    /* 0x1C */ f32 mWalkAnmSpeedMax;
    /* 0x20 */ f32 mWalkAnmSpeedMin;
    /* 0x24 */ f32 mWaterInputFactor;
    /* 0x28 */ f32 mWaterWalkAnmRate;
    /* 0x2C */ f32 mWaterStartWalkAnmRate;
    /* 0x30 */ f32 mMaxMagneFlySpeed;
    /* 0x34 */ f32 mMagneFlyAccelRate;
    /* 0x38 */ f32 mWaterVelocityY;
    /* 0x3C */ f32 mWaterVelocityX;
    /* 0x40 */ f32 mWaterVelRateSword;
    /* 0x44 */ f32 mZoraWaterInputFactor;
    /* 0x48 */ f32 mZoraWaterAnmSpeed;
};  // Size: 0x4C

class daAlinkHIO_magneBoots_c0 {
public:
    static daAlinkHIO_magneBoots_c1 const m;
};

class daAlinkHIO_magneBoots_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_magneBoots_c();
    ~daAlinkHIO_magneBoots_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_magneBoots_c1 m;
#endif
};

class daAlinkHIO_fmChain_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mGripAnm;
    /* 0x14 */ daAlinkHIO_anm_c mUnfinishedAnm;
};  // Size: 0x28

class daAlinkHIO_fmChain_c0 {
public:
    static daAlinkHIO_fmChain_c1 const m;
};

class daAlinkHIO_fmChain_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_fmChain_c();
    ~daAlinkHIO_fmChain_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_fmChain_c1 m;
#endif
};

class daAlinkHIO_hookshot_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mShootAnm;
    /* 0x14 */ daAlinkHIO_anm_c mRoofHangAnm;
    /* 0x28 */ daAlinkHIO_anm_c mWallHangAnm;
    /* 0x3C */ u8 mForceStick;
    /* 0x40 */ f32 mWaitAnmSpeed;
    /* 0x44 */ f32 mStartInterpolation;
    /* 0x48 */ f32 mMaxLength;
    /* 0x4C */ f32 mShootSpeed;
    /* 0x50 */ f32 mReturnSpeed;
    /* 0x54 */ f32 mStickReturnSpeed;
    /* 0x58 */ f32 mRoofHangRiseSpeed;
    /* 0x5C */ f32 mRoofHangDecendSpeed;
    /* 0x60 */ f32 mBossMaxLength;
    /* 0x64 */ f32 mBossShootSpeed;
    /* 0x68 */ f32 mBossReturnSpeed;
    /* 0x6C */ f32 mBossStickReturnSpeed;
};  // Size: 0x70

class daAlinkHIO_hookshot_c0 {
public:
    static daAlinkHIO_hookshot_c1 const m;
};

class daAlinkHIO_hookshot_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_hookshot_c();
    ~daAlinkHIO_hookshot_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_hookshot_c1 m;
#endif
};

class daAlinkHIO_spinner_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mKickAnm;
    /* 0x14 */ s16 mRideMoveTime;
    /* 0x16 */ s16 mBoardWaitTime;
    /* 0x18 */ s16 mRideRotAngleMax;
    /* 0x1A */ s16 mRideRotAngleMin;
    /* 0x1C */ s16 mBossRideMoveTime;
    /* 0x20 */ f32 mGravity;
    /* 0x24 */ f32 mMaxFallSpeed;
    /* 0x28 */ f32 mJumpRate;
    /* 0x2C */ f32 mRideSpeed;
    /* 0x30 */ f32 mDecelSpeedMax;
    /* 0x34 */ f32 mDecelSpeedMin;
    /* 0x38 */ f32 mDecelRate;
    /* 0x3C */ f32 mBossRideSpeed;
};  // Size: 0x40

class daAlinkHIO_spinner_c0 {
public:
    static daAlinkHIO_spinner_c1 const m;
};

class daAlinkHIO_spinner_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_spinner_c();
    ~daAlinkHIO_spinner_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_spinner_c1 m;
#endif
};

class daAlinkHIO_ironBall_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mCatchAnm;
    /* 0x14 */ s16 mThrowChainNum;
    /* 0x16 */ s16 mReturnChainRemoveNum;
    /* 0x18 */ f32 mWaitAnmSpeed;
    /* 0x1C */ f32 mPrepareAnmSpeed;
    /* 0x20 */ f32 mPrepareInterpolation;
    /* 0x24 */ f32 mTurnAnmSpeed;
    /* 0x28 */ f32 mTurnInterpolation;
    /* 0x2C */ f32 mPreThrowAnmSpeed;
    /* 0x30 */ f32 mPreThrowAnmInterpolation;
    /* 0x34 */ f32 mThrowAnmSpeed;
    /* 0x38 */ f32 mThrowInterpolation;
    /* 0x3C */ f32 mPullAnmSpeed;
    /* 0x40 */ f32 mPullInterpolation;
    /* 0x44 */ f32 mChainSpeedRate;
    /* 0x48 */ f32 mChainGravity;
    /* 0x4C */ f32 mReturnChainAccelY;
    /* 0x50 */ f32 mReturnChainRate;
    /* 0x54 */ f32 mThrowChainAccelZ;
    /* 0x58 */ f32 mThrowChainAccelY;
    /* 0x5C */ f32 mThrowChainGravity;
    /* 0x60 */ f32 mBallGravity;
    /* 0x64 */ f32 mThrowBallGravity;
    /* 0x68 */ f32 mBallMaxFallSpeed;
    /* 0x6C */ f32 field_0x6C;
    /* 0x70 */ f32 mThrowSpeedZ;
    /* 0x74 */ f32 mThrowSpeedY;
    /* 0x78 */ f32 mReturnBallGravity;
    /* 0x7C */ f32 mAttackRadius;
    /* 0x80 */ f32 mFeetPosRatio;
    /* 0x84 */ f32 mWalkAnmSpeedMax;
    /* 0x88 */ f32 mWalkAnmSpeedMin;
};  // Size: 0x8C

class daAlinkHIO_ironBall_c0 {
public:
    static daAlinkHIO_ironBall_c1 const m;
};

class daAlinkHIO_ironBall_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_ironBall_c();
    ~daAlinkHIO_ironBall_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_ironBall_c1 m;
#endif
};

class daAlinkHIO_copyRod_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mSwingAnm;
    /* 0x14 */ daAlinkHIO_anm_c mBigSwingAnm;
    /* 0x28 */ f32 mBallSpeed;
    /* 0x2c */ f32 mBallMaxDistance;
    /* 0x30 */ f32 mBallReturnSpeed;
    /* 0x34 */ f32 mBossBallMaxDistance;
};  // Size: 0x38

class daAlinkHIO_copyRod_c0 {
public:
    static daAlinkHIO_copyRod_c1 const m;
};

class daAlinkHIO_copyRod_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_copyRod_c();
    ~daAlinkHIO_copyRod_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_copyRod_c1 m;
#endif
};

class daAlinkHIO_pickUp_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mGrabAnm;
    /* 0x14 */ daAlinkHIO_anm_c mPlaceAnm;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2C;
};  // Size: 0x30

class daAlinkHIO_pickUp_c0 {
public:
    static daAlinkHIO_pickUp_c1 const m;
};

class daAlinkHIO_pickUp_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_pickUp_c();
    ~daAlinkHIO_pickUp_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_pickUp_c1 m;
#endif
};

class daAlinkHIO_board_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mJumpAnm;  // might be wrong
    /* 0x14 */ daAlinkHIO_anm_c mAirborneAnm;
    /* 0x28 */ daAlinkHIO_anm_c mLandAnm;
    /* 0x3C */ daAlinkHIO_anm_c mSpinAnm;
    /* 0x50 */ f32 mMaxSpeed;
    /* 0x54 */ f32 mPushAccel;
    /* 0x58 */ f32 mMinJumpSpeedY;
    /* 0x5C */ f32 mMaxJumpSpeedY;
    /* 0x60 */ f32 mStandAnmSpeed;
    /* 0x64 */ f32 mStandInterpolation;
    /* 0x68 */ f32 mSitAnmSpeed;
    /* 0x6C */ f32 mSitInterpolation;
    /* 0x70 */ f32 mStartMinSpeed;
    /* 0x74 */ f32 mStartAddSpeed;
    /* 0x78 */ f32 mPushAnmMaxSpeed;
    /* 0x7C */ f32 mFastPushSwitchSpeed;
    /* 0x80 */ f32 mFastPushInterpolation;
    /* 0x84 */ f32 mPushInterpolation;
    /* 0x88 */ f32 mPushAnmMinSpeed;
    /* 0x8C */ f32 mMaxPushSpeed;
    /* 0x90 */ f32 mSpinMinJumpSpeedY;
    /* 0x94 */ f32 mSpinMaxJumpSpeedY;
    /* 0x98 */ f32 mEffectMaxSpeed;
};  // Size: 0x9C

class daAlinkHIO_board_c0 {
public:
    static daAlinkHIO_board_c1 const m;
};

class daAlinkHIO_board_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_board_c();
    ~daAlinkHIO_board_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_board_c1 m;
#endif
};

class daAlinkHIO_bottle_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mStartDrinkAnm;
    /* 0x14 */ daAlinkHIO_anm_c mEndDrinkAnm;
    /* 0x28 */ daAlinkHIO_anm_c mOpenBottleAnm;
    /* 0x3C */ daAlinkHIO_anm_c mPourAnm;
    /* 0x50 */ daAlinkHIO_anm_c mSwingDownAnm;
    /* 0x64 */ daAlinkHIO_anm_c mSwingSideAnm;
    /* 0x78 */ daAlinkHIO_anm_c mGetAnm;
    /* 0x8C */ daAlinkHIO_anm_c mDrinkNastyAnm;
    /* 0xA0 */ daAlinkHIO_anm_c mScoopAnm;
};  // Size: 0xB4

class daAlinkHIO_bottle_c0 {
public:
    static daAlinkHIO_bottle_c1 const m;
};

class daAlinkHIO_bottle_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_bottle_c();
    ~daAlinkHIO_bottle_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_bottle_c1 m;
#endif
};

class daAlinkHIO_item_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mOneHandEquipAnm;
    /* 0x14 */ daAlinkHIO_anm_c mTwoHandEquipAnm;
    /* 0x28 */ s16 mItemFPTransitionTimer;
    /* 0x2A */ s16 mItemFPMaxUnk;
    /* 0x2C */ s16 mItemFPUpMaxUnk;
    /* 0x2E */ s16 mWiiItemFPDownMaxUnk;
    /* 0x30 */ s16 mWiiItemFPUpMaxUnk;
    /* 0x34 */ f32 mOneHandReleaseAnmSpeed;
    /* 0x38 */ f32 mTwoHandReleaseAnmSpeed;
};  // Size: 0x3C

class daAlinkHIO_item_c0 {
public:
    static daAlinkHIO_bow_c0 const mBow;
    static daAlinkHIO_boom_c0 const mBoomerang;
    static daAlinkHIO_bomb_c0 const mBomb;
    static daAlinkHIO_huLight_c0 const mLanternPL;
    static daAlinkHIO_kandelaar_c0 const mLantern;
    static daAlinkHIO_fmChain_c0 const mFmChain;
    static daAlinkHIO_magneBoots_c0 const mIronBoots;
    static daAlinkHIO_pickUp_c0 const mPickUp;
    static daAlinkHIO_board_c0 const mBoard;
    static daAlinkHIO_bottle_c0 const mBottle;
    static daAlinkHIO_hookshot_c0 const mHookshot;
    static daAlinkHIO_spinner_c0 const mSpinner;
    static daAlinkHIO_ironBall_c0 const mIronBall;
    static daAlinkHIO_copyRod_c0 const mCopyRod;
    static daAlinkHIO_zwLight_c0 const mZoraArmorPL;
    static daAlinkHIO_item_c1 const m;
};

class daAlinkHIO_item_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_item_c();
    ~daAlinkHIO_item_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_bow_c mBow;
    /* 0x00 */ daAlinkHIO_boom_c mBoomerang;
    /* 0x00 */ daAlinkHIO_bomb_c mBomb;
    /* 0x00 */ daAlinkHIO_light_c mLanternPL;
    /* 0x00 */ daAlinkHIO_kandelaar_c mLantern;
    /* 0x00 */ daAlinkHIO_fmChain_c mFmChain;
    /* 0x00 */ daAlinkHIO_magneBoots_c mIronBoots;
    /* 0x00 */ daAlinkHIO_pickUp_c mPickUp;
    /* 0x00 */ daAlinkHIO_board_c mBoard;
    /* 0x00 */ daAlinkHIO_bottle_c mBottle;
    /* 0x00 */ daAlinkHIO_hookshot_c mHookshot;
    /* 0x00 */ daAlinkHIO_spinner_c mSpinner;
    /* 0x00 */ daAlinkHIO_ironBall_c mIronBall;
    /* 0x00 */ daAlinkHIO_copyRod_c mCopyRod;
    /* 0x00 */ daAlinkHIO_light_c mZoraArmorPL;
    /* 0x00 */ daAlinkHIO_item_c1 m;
#endif
};

class daAlinkHIO_ladder_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mWallAttachAnm;
    /* 0x14 */ daAlinkHIO_anm_c mWallAttachMissAnm;
    /* 0x28 */ f32 mClimbUpStartASpeed;
    /* 0x2C */ f32 mClimbUpStartInterp;
    /* 0x30 */ f32 mClimbUpEndASpeed;
    /* 0x34 */ f32 mClimbUpEndInterp;
    /* 0x38 */ f32 mClimbDownStartASpeed;
    /* 0x3C */ f32 mClimbDownStartInterp;
    /* 0x40 */ f32 mClimbDownEndASpeed;
    /* 0x44 */ f32 mClimbDownEndInterp;
    /* 0x48 */ f32 mMoveMinASpeed;
    /* 0x4C */ f32 mMoveMaxSpeed;
    /* 0x50 */ f32 mMoveInterp;
    /* 0x54 */ f32 mClimbDownEndCF;
    /* 0x58 */ f32 mClimbUpEndCF;
    /* 0x5C */ f32 mWallHorizontalMinAnmSpeed;
    /* 0x60 */ f32 mWallHorizontalMaxAnmSpeed;
    /* 0x64 */ f32 mWallVerticalMinAnmSpeed;
    /* 0x68 */ f32 mWallVerticalMaxAnmSpeed;
    /* 0x6C */ f32 mWallApproachOffset;
};  // Size: 0x70

class daAlinkHIO_ladder_c0 {
public:
    static daAlinkHIO_ladder_c1 const m;
};

class daAlinkHIO_ladder_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_ladder_c();
    ~daAlinkHIO_ladder_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_ladder_c1 m;
#endif
};

class daAlinkHIO_roofHang_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mRoofHangAnm;
    /* 0x14 */ s16 mTurnAngleRate;
    /* 0x16 */ s16 mMaxTurnAngle;
    /* 0x18 */ s16 mMinTurnAngle;
    /* 0x1C */ f32 mStandbyASpeed;
    /* 0x20 */ f32 mStandbyInterp;
    /* 0x24 */ f32 mVertMoveMinASpeed;
    /* 0x28 */ f32 mVertMoveMaxASpeed;
    /* 0x2C */ f32 mVertMoveInterp;
    /* 0x30 */ f32 mVertMoveMinSpeed;
    /* 0x34 */ f32 mVertMoveMaxSpeed;
    /* 0x38 */ f32 mHorizMoveMinASpeed;
    /* 0x3C */ f32 mHorizMoveMaxASpeed;
    /* 0x40 */ f32 mHorizMoveInterp;
    /* 0x44 */ f32 mInvertASpeed;
    /* 0x48 */ f32 mInvertInterp;
};  // Size: 0x4C

class daAlinkHIO_roofHang_c0 {
public:
    static daAlinkHIO_roofHang_c1 const m;
};

class daAlinkHIO_roofHang_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_roofHang_c();
    ~daAlinkHIO_roofHang_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_roofHang_c1 m;
#endif
};

class daAlinkHIO_grab_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mPrepareAnm;
    /* 0x14 */ daAlinkHIO_anm_c mLiftBackAnm;
    /* 0x28 */ daAlinkHIO_anm_c mStandReverseAnm;
    /* 0x3C */ daAlinkHIO_anm_c mLiftAnm;
    /* 0x50 */ daAlinkHIO_anm_c mThrowAnm;
    /* 0x64 */ daAlinkHIO_anm_c mCarryAnm;
    /* 0x78 */ daAlinkHIO_anm_c mRecoilAnm;
    /* 0x8C */ daAlinkHIO_anm_c mFailAnm;
    /* 0xA0 */ daAlinkHIO_anm_c mCarryLiftAnm;
    /* 0xB4 */ daAlinkHIO_anm_c mCarryPlaceReverseAnm;
    /* 0xC8 */ daAlinkHIO_anm_c mIronBallThrowAnm;
    /* 0xDC */ s16 mForceWeightSpec;
    /* 0xDE */ s16 mFailStopTime;
    /* 0xE0 */ f32 field_0xE0;
};  // Size: 0xE4

class daAlinkHIO_grab_c0 {
public:
    static daAlinkHIO_grab_c1 const m;
};

class daAlinkHIO_grab_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_grab_c();
    ~daAlinkHIO_grab_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_grab_c1 m;
#endif
};

class daAlinkHIO_swim_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mFloatUpAnm;
    /* 0x14 */ daAlinkHIO_anm_c mDiveAnm;
    /* 0x28 */ daAlinkHIO_anm_c mUnderwaterDiveAnm;
    /* 0x3C */ daAlinkHIO_anm_c mDashAnm;
    /* 0x50 */ s16 mTurnRate;
    /* 0x52 */ s16 mMinTurn;
    /* 0x54 */ s16 mMaxTurn;
    /* 0x56 */ s16 mUnderwaterTurnRate;
    /* 0x58 */ s16 mUnderwaterMinTurn;
    /* 0x5A */ s16 mUnderwaterMaxTurn;
    /* 0x5C */ s16 field_0x5c;  // related to dash
    /* 0x5E */ s16 field_0x5e;  // related to dash
    /* 0x60 */ f32 mStartHeight;
    /* 0x64 */ f32 mFloatUpHeight;
    /* 0x68 */ f32 mForwardMaxSpeed;
    /* 0x6C */ f32 mStrafeMaxSpeed;
    /* 0x70 */ f32 mBackwardMaxSpeed;
    /* 0x74 */ f32 mUnderwaterMaxSpeed;
    /* 0x78 */ f32 mUnderwaterFallMaxSpeed;
    /* 0x7C */ f32 mAcceleration;
    /* 0x80 */ f32 mDeceleration;
    /* 0x84 */ f32 mMaxFallSpeed;
    /* 0x88 */ f32 mMaxFloatUpSpeed;
    /* 0x8C */ f32 mBuoyancy;
    /* 0x90 */ f32 mWaitAnmSpeed;
    /* 0x94 */ f32 mWaitInterpolation;
    /* 0x98 */ f32 mWaitUpDownShakeAmount;
    /* 0x9C */ f32 mForwardMinAnmSpeed;
    /* 0xA0 */ f32 mForwardMaxAnmSpeed;
    /* 0xA4 */ f32 mStrafeMinAnmSpeed;
    /* 0xA8 */ f32 mStrafeMaxAnmSpeed;
    /* 0xAC */ f32 mBackwardMinAnmSpeed;
    /* 0xB0 */ f32 mBackwardMaxAnmSpeed;
    /* 0xB4 */ f32 mUnderwaterMinAnmSpeed;
    /* 0xB8 */ f32 mUnderwaterMaxAnmSpeed;
    /* 0xBC */ f32 mMoveInterpolation;
    /* 0xC0 */ f32 mClimbHeight;
    /* 0xC4 */ f32 mClimbWaterSurfaceUnder;
    /* 0xC8 */ f32 mForwardMinSpeed;
    /* 0xCC */ f32 mStandingMaxFallSpeed;
    /* 0xD0 */ f32 mBootsMaxFallSpeed;
    /* 0xD4 */ f32 mNormalBuoyancyWaterDepth;
    /* 0xD8 */ f32 mZoraClothesBuoyancy;
    /* 0xDC */ f32 mUnderwaterButtonAdditionalSpeed;
    /* 0xE0 */ f32 mUnderwaterAdditionalAnmSpeed;
    /* 0xE4 */ f32 mBootsGravity;
    /* 0xE8 */ f32 mFloatUpSwimSpeedRate;
    /* 0xEC */ f32 mDashMaxSpeed;
    /* 0xF0 */ f32 mDashDeceleration;
    /* 0xF4 */ f32 mDashMinAnmSpeed;
    /* 0xF8 */ f32 mDashMaxAnmSpeed;
};  // Size: 0xFC

class daAlinkHIO_swim_c0 {
public:
    static daAlinkHIO_swim_c1 const m;
};

class daAlinkHIO_swim_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_swim_c();
    ~daAlinkHIO_swim_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_swim_c1 m;
#endif
};

class daAlinkHIO_wlMove_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mSlipAnm;
    /* 0x14 */ daAlinkHIO_anm_c mReverseAnm;
    /* 0x28 */ daAlinkHIO_anm_c mADashAnm;
    /* 0x3C */ daAlinkHIO_anm_c mDashReboundAnm;
    /* 0x50 */ s16 mTurnMax;
    /* 0x52 */ s16 mTurnMin;
    /* 0x54 */ s16 mTurnAngleRate;
    /* 0x56 */ s16 mADashDuration;
    /* 0x58 */ s16 mADashCooldownTime;
    /* 0x5A */ s16 mADashDurationSlow;
    /* 0x5C */ s16 mADashCooldownTimeSlow;
    /* 0x5E */ s16 mADashDurationSlow2;
    /* 0x60 */ s16 mADashCooldownTimeSlow2;
    /* 0x64 */ f32 mMaxSpeed;
    /* 0x68 */ f32 mAcceleration;
    /* 0x6C */ f32 mDeceleration;
    /* 0x70 */ f32 mIdleAnmSpeed;
    /* 0x74 */ f32 mWalkAnmSpeed;
    /* 0x78 */ f32 mBriskWalkAnmSpeed;
    /* 0x7C */ f32 mRunAnmSpeed;
    /* 0x80 */ f32 mQuickRunAnmSpeed;
    /* 0x84 */ f32 mIdleToWalkRate;
    /* 0x88 */ f32 mWalkToBriskWalkRate;
    /* 0x8C */ f32 mStandbyRunToRunRate;
    /* 0x90 */ f32 mRunToQuickRunRate;
    /* 0x94 */ f32 mNormalInterpolation;
    /* 0x98 */ f32 mWalkRunInterpolation;
    /* 0x9C */ f32 mRunAnmMinBlendRate;
    /* 0xA0 */ f32 mSlipStartRate;
    /* 0xA4 */ f32 mSlipInitSpeedRate;
    /* 0xA8 */ f32 mSlipDeceleration;
    /* 0xAC */ f32 mReverseSpeed;
    /* 0xB0 */ f32 mIdleReverseStartFrame;
    /* 0xB4 */ f32 mGazeMaxSpeed;
    /* 0xB8 */ f32 mGazeIdleAnmSpeed;
    /* 0xBC */ f32 mIdleInterpolation;
    /* 0xC0 */ f32 mReverseFallHeight;
    /* 0xC4 */ f32 mReverseFallInterpolation;
    /* 0xC8 */ f32 mADashMaxSpeed;
    /* 0xCC */ f32 mADashQuickRunAnmSpeed;
    /* 0xD0 */ f32 mADashAcceleration;
    /* 0xD4 */ f32 mADashInitSpeed;
    /* 0xD8 */ f32 mDashReboundHorizontalSpeed;
    /* 0xDC */ f32 mDashReboundVerticalSpeed;
    /* 0xE0 */ f32 mADashMaxSpeedSlow;
    /* 0xE4 */ f32 mADashInitSpeedSlow;
    /* 0xE8 */ f32 mADashQuickRunASpeedSlow;
    /* 0xEC */ f32 mADashAccelerationSlow;
    /* 0xF0 */ f32 mADashMaxSpeedSlow2;
    /* 0xF4 */ f32 mADashInitSpeedSlow2;
    /* 0xF8 */ f32 mADashQuickRunASpeedSlow2;
    /* 0xFC */ f32 mADashAccelerationSlow2;
};  // Size: 0x100

class daAlinkHIO_wlMove_c0 {
public:
    static daAlinkHIO_wlMove_c1 const m;
};

class daAlinkHIO_wlMove_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlMove_c();
    ~daAlinkHIO_wlMove_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_wlMove_c1 m;
#endif
};

class daAlinkHIO_wlMoveNoP_c1 {
public:
    /* 0x00 */ s16 mTurnMax;
    /* 0x02 */ s16 mTurnMin;
    /* 0x04 */ s16 mTurnAngleRate;
    /* 0x08 */ f32 mMaxSpeed;
    /* 0x0C */ f32 mAcceleration;
    /* 0x10 */ f32 mDeceleration;
    /* 0x14 */ f32 mIdleAnmSpeed;
    /* 0x18 */ f32 mWalkAnmSpeed;
    /* 0x1C */ f32 mJogAnmSpeed;
    /* 0x20 */ f32 mRunAnmSpeed;
    /* 0x24 */ f32 mIdleToWalkRate;
    /* 0x28 */ f32 mWalkToJogRate;
    /* 0x2C */ f32 mJogToRunRate;
    /* 0x30 */ f32 mWalkRunInterpolation;
    /* 0x34 */ f32 mRunAnmMinBlendRate;
    /* 0x38 */ f32 mSlideStartRate;
    /* 0x3C */ f32 mSlideInitSpeed;
    /* 0x40 */ f32 mSlideDeceleration;
    /* 0x44 */ f32 mReverseSpeed;
};  // Size: 0x48

class daAlinkHIO_wlMoveNoP_c0 {
public:
    static daAlinkHIO_wlMoveNoP_c1 const m;
};

class daAlinkHIO_wlMoveNoP_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlMoveNoP_c();
    ~daAlinkHIO_wlMoveNoP_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlMoveNoP_c1 m;
#endif
};

class daAlinkHIO_wlAtnMove_c1 {
public:
    /* 0x00 */ s16 mTurnAngleMax;
    /* 0x02 */ s16 mTurnAngleMin;
    /* 0x04 */ s16 mTurnAngleRate;
    /* 0x08 */ f32 mWalkAnmSpeed;
    /* 0x0C */ f32 mRunAnmSpeed;
    /* 0x10 */ f32 mWalkChangeRate;
    /* 0x14 */ f32 mRunChangeRate;
    /* 0x18 */ f32 mMaxSpeed;
    /* 0x1C */ f32 mAcceleration;
    /* 0x20 */ f32 mDeceleration;
    /* 0x24 */ f32 mRearWalkAnmSpeed;
    /* 0x28 */ f32 mRearWalkChangeRate;
    /* 0x2C */ f32 mRearMaxSpeed;
    /* 0x30 */ f32 mRearAcceleration;
    /* 0x34 */ f32 mRearDeceleration;
};  // Size: 0x38

class daAlinkHIO_wlAtnMove_c0 {
public:
    static daAlinkHIO_wlAtnMove_c1 const m;
};

class daAlinkHIO_wlAtnMove_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlAtnMove_c();
    ~daAlinkHIO_wlAtnMove_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlAtnMove_c1 m;
#endif
};

class daAlinkHIO_wlHowl_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mHowlAnm;
    /* 0x14 */ daAlinkHIO_anm_c mSittingHowlAnm;
    /* 0x28 */ daAlinkHIO_anm_c mThreatAnm;
    /* 0x3C */ daAlinkHIO_anm_c mThreatStanceAnm;
    /* 0x50 */ f32 mThreatMoveInterp;
    /* 0x54 */ f32 mThreatMoveRate;
    /* 0x58 */ f32 mThreatMoveMaxAnmSpeed;
    /* 0x5C */ f32 mThreatMoveMinAnmSpeed;
};  // Size: 0x60

class daAlinkHIO_wlHowl_c0 {
public:
    static daAlinkHIO_wlHowl_c1 const m;
};

class daAlinkHIO_wlHowl_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlHowl_c();
    ~daAlinkHIO_wlHowl_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlHowl_c1 m;
#endif
};

class daAlinkHIO_wlSideStep_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mSideJumpAnm;
    /* 0x14 */ daAlinkHIO_anm_c mSideLandAnm;
    /* 0x28 */ daAlinkHIO_anm_c mBackJumpAnm;
    /* 0x3C */ daAlinkHIO_anm_c mBackLandAnm;
    /* 0x50 */ f32 mSideJumpHorizSpeed;
    /* 0x54 */ f32 mSideJumpVertSpeed;
    /* 0x58 */ f32 mBackJumpHorizSpeed;
    /* 0x5C */ f32 mBackJumpVertSpeed;
    /* 0x60 */ f32 mFallHeight;
    /* 0x64 */ f32 mFallInterp;
};  // Size: 0x68

class daAlinkHIO_wlSideStep_c0 {
public:
    static daAlinkHIO_wlSideStep_c1 const m;
};

class daAlinkHIO_wlSideStep_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlSideStep_c();
    ~daAlinkHIO_wlSideStep_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlSideStep_c1 m;
#endif
};

class daAlinkHIO_wlBackJump_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mBackJumpAnm;
    /* 0x14 */ daAlinkHIO_anm_c mBackLandAnm;
    /* 0x28 */ f32 mBackJumpHorizSpeed;
    /* 0x2C */ f32 mBackJumpVertSpeed;
    /* 0x30 */ f32 mFallHeight;
    /* 0x34 */ f32 mFallInterp;
};  // Size: 0x38

class daAlinkHIO_wlBackJump_c0 {
public:
    static daAlinkHIO_wlBackJump_c1 const m;
};

class daAlinkHIO_wlBackJump_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlBackJump_c();
    ~daAlinkHIO_wlBackJump_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlBackJump_c1 m;
#endif
};

class daAlinkHIO_wlAutoJump_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mJumpAnm;
    /* 0x14 */ daAlinkHIO_anm_c mLandingAnm;
    /* 0x28 */ daAlinkHIO_anm_c mClimbAnm;
    /* 0x3C */ u8 mAlwaysMaxSpeedJump;
    /* 0x3E */ s16 field_0x3e;
    /* 0x40 */ s16 mJumpAngle;
    /* 0x42 */ s16 mWeakJumpAngle;
    /* 0x44 */ f32 mJumpLimitSpeed;
    /* 0x48 */ f32 mJumpMinSpeed;
    /* 0x4C */ f32 mJumpSpeedRate;
    /* 0x50 */ f32 mJumpFallInterp;
    /* 0x54 */ f32 mNormalFallInterp;
    /* 0x58 */ f32 mGravity;
    /* 0x5C */ f32 mMaxFallSpeed;
    /* 0x60 */ f32 mWeakJumpAnmSpeed;
    /* 0x64 */ f32 mWeakJumpLimitSpeed;
    /* 0x68 */ f32 mWeakJumpMinSpeed;
    /* 0x6C */ f32 mWeakJumpSpeedRate;
    /* 0x70 */ f32 mJumpMaxSpeed;
    /* 0x74 */ f32 mWeakJumpMaxSpeed;
    /* 0x78 */ f32 mHangLimitHeight;
};  // size = 0x7C

class daAlinkHIO_wlAutoJump_c0 {
public:
    static daAlinkHIO_wlAutoJump_c1 const m;
};

class daAlinkHIO_wlAutoJump_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlAutoJump_c();
    ~daAlinkHIO_wlAutoJump_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlAutoJump_c1 m;
#endif
};

class daAlinkHIO_wlPush_c1 {
public:
    /* 0x00 */ f32 mIdleAnmSpeed;
    /* 0x04 */ f32 mIdleInterp;
    /* 0x08 */ f32 mPushAnmSpeed;
    /* 0x0C */ f32 mPushInterp;
    /* 0x10 */ f32 mHeavyPushAnmSpeed;
};  // Size: 0x14

class daAlinkHIO_wlPush_c0 {
public:
    static daAlinkHIO_wlPush_c1 const m;
};

class daAlinkHIO_wlPush_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlPush_c();
    ~daAlinkHIO_wlPush_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlPush_c1 m;
#endif
};

class daAlinkHIO_wlLie_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mSittingAnm;
    /* 0x14 */ daAlinkHIO_anm_c mProneStartAnm;
    /* 0x28 */ s16 mProneTurnRate;
    /* 0x2A */ s16 mProneTurnMax;
    /* 0x2C */ s16 mProneTurnMin;
    /* 0x30 */ f32 mStandUpInterp;
    /* 0x34 */ f32 mProneMoveRate;
    /* 0x38 */ f32 mProneMaxAnmSpeed;
    /* 0x3C */ f32 mProneMinAnmSpeed;
    /* 0x40 */ f32 mProneMoveInterp;
    /* 0x44 */ f32 mSittingIdleAnmSpeed;
    /* 0x48 */ f32 mSittingIdleInterp;
};  // Size: 0x4C

class daAlinkHIO_wlLie_c0 {
public:
    static daAlinkHIO_wlLie_c1 const m;
};

class daAlinkHIO_wlLie_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlLie_c();
    ~daAlinkHIO_wlLie_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlLie_c1 m;
#endif
};

class daAlinkHIO_wlWallHang_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mPreGrabJumpAnm;
    /* 0x14 */ daAlinkHIO_anm_c mClimbAnm;
    /* 0x28 */ daAlinkHIO_anm_c mFallGrabAnm;
    /* 0x3C */ daAlinkHIO_anm_c mWallGrabAnm;
    /* 0x50 */ daAlinkHIO_anm_c mLedgeClimbAnm;
    /* 0x64 */ daAlinkHIO_anm_c mLedgeLandAnm;
    /* 0x78 */ f32 mAutoWalkHeight;
    /* 0x7C */ f32 mSmallJumpHeight;
    /* 0x80 */ f32 mWallGrabHeight;
    /* 0x84 */ f32 mClimbHeight;
    /* 0x88 */ f32 mHangingFootHeight;
    /* 0x8C */ f32 mJumpHorizontalSpeed;
    /* 0x90 */ f32 mJumpTargetHeightOffset;
    /* 0x94 */ f32 mJumpToFallInterp;
    /* 0x98 */ f32 mHangingFallInterp;
};  // Size: 0x9C

class daAlinkHIO_wlWallHang_c0 {
public:
    static daAlinkHIO_wlWallHang_c1 const m;
};

class daAlinkHIO_wlWallHang_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlWallHang_c();
    ~daAlinkHIO_wlWallHang_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlWallHang_c1 m;
#endif
};

class daAlinkHIO_wlDamNormal_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mForwardAnm;
    /* 0x14 */ daAlinkHIO_anm_c mBackwardAnm;
    /* 0x28 */ daAlinkHIO_anm_c mLeftAnm;
    /* 0x3C */ daAlinkHIO_anm_c mRightAnm;
    /* 0x50 */ f32 mInitSpeed;
    /* 0x54 */ f32 mAttackSpeedRate;
    /* 0x58 */ f32 mDeceleration;
    /* 0x5C */ f32 mIceDamageAnmSpeed;
};  // Size: 0x60

class daAlinkHIO_wlDamNormal_c0 {
public:
    static daAlinkHIO_wlDamNormal_c1 const m;
};

class daAlinkHIO_wlDamNormal_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlDamNormal_c();
    ~daAlinkHIO_wlDamNormal_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlDamNormal_c1 m;
#endif
};

class daAlinkHIO_wlDamLaHu_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mFrontRiseAnm;
    /* 0x14 */ daAlinkHIO_anm_c mBackRiseAnm;
    /* 0x28 */ daAlinkHIO_anm_c mLeftRiseAnm;
    /* 0x3C */ daAlinkHIO_anm_c mRightRiseAnm;
    /* 0x50 */ s16 mBodyRotationSpeed;
    /* 0x54 */ f32 mGravity;
    /* 0x58 */ f32 mDamageInterp;
    /* 0x5C */ f32 mHorizontalSpeed;
    /* 0x60 */ f32 mVerticalSpeed;
    /* 0x64 */ f32 mBounceSpeed;
    /* 0x68 */ f32 mDeceleration;
};  // Size: 0x6C

class daAlinkHIO_wlDamLarge_c0 {
public:
    static daAlinkHIO_wlDamLaHu_c1 const m;
};

class daAlinkHIO_wlDamHuge_c0 {
public:
    static daAlinkHIO_wlDamLaHu_c1 const m;
};

class daAlinkHIO_wlDamLaHu_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlDamLaHu_c(int);
    ~daAlinkHIO_wlDamLaHu_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlDamLaHu_c1 m;
#endif
};

class daAlinkHIO_wlDamCaught_c1 {
public:
    /* 0x00 */ s16 mCaughtTime;
    /* 0x02 */ s16 mInputDecayTime;
    /* 0x04 */ f32 mCaughtSpeed;
    /* 0x08 */ f32 mWaitInterp;
    /* 0x0C */ f32 mWaitAddAnmSpeed;
};  // Size: 0x10

class daAlinkHIO_wlDamCaught_c0 {
public:
    static daAlinkHIO_wlDamCaught_c1 const m;
};

class daAlinkHIO_wlDamCaught_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlDamCaught_c();
    ~daAlinkHIO_wlDamCaught_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlDamCaught_c1 m;
#endif
};

class daAlinkHIO_wlDamFall_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mLandingAnm;
    /* 0x14 */ daAlinkHIO_anm_c mRightRiseAnm;
    /* 0x28 */ s16 mSmallStopTime;
    /* 0x2A */ s16 mLargeStopTime;
    /* 0x2C */ f32 mSmallDamageHeight;
    /* 0x30 */ f32 mLargeDamageHeight;
    /* 0x34 */ f32 mSmallDamageLandingStartFrame;
    /* 0x38 */ f32 mAirAnmTransitionHeight;
    /* 0x3C */ f32 mAirAnmInterp;
};  // Size: 0x40

class daAlinkHIO_wlDamFall_c0 {
public:
    static daAlinkHIO_wlDamFall_c1 const m;
};

class daAlinkHIO_wlDamFall_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlDamFall_c();
    ~daAlinkHIO_wlDamFall_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlDamFall_c1 m;
#endif
};

class daAlinkHIO_wlDamage_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mRunningAnm;
    /* 0x14 */ daAlinkHIO_anm_c mPolygonAnm;
    /* 0x28 */ f32 mReturnWakeupAnmSpeed;
};  // Size: 0x2C

class daAlinkHIO_wlDamage_c0 {
public:
    static daAlinkHIO_wlDamage_c1 const m;
    static daAlinkHIO_wlDamNormal_c0 const mNormal;
    static daAlinkHIO_wlDamLarge_c0 const mLarge;
    static daAlinkHIO_wlDamHuge_c0 const mHuge;
    static daAlinkHIO_wlDamFall_c0 const mFall;
    static daAlinkHIO_wlDamCaught_c0 const mCapture;
};

class daAlinkHIO_wlDamage_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlDamage_c();
    ~daAlinkHIO_wlDamage_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_wlDamage_c1 m;
#endif
    /* 0x00 */ daAlinkHIO_wlDamNormal_c mNormal;
    /* 0x00 */ daAlinkHIO_wlDamLaHu_c mLarge;
    /* 0x00 */ daAlinkHIO_wlDamLaHu_c mHuge;
    /* 0x00 */ daAlinkHIO_wlDamFall_c mFall;
    /* 0x00 */ daAlinkHIO_wlDamCaught_c mCapture;
};

class daAlinkHIO_wlSlide_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mFrontSlideAnm;
    /* 0x14 */ daAlinkHIO_anm_c mBackSlideAnm;
    /* 0x28 */ daAlinkHIO_anm_c mLandingAnm;
    /* 0x3c */ daAlinkHIO_anm_c mSlipDownRecoveryAnm;
    /* 0x50 */ s16 mSlipDownRotationMax;
    /* 0x52 */ s16 mSlipDownRotationMin;
    /* 0x54 */ f32 mMaxSpeed;
    /* 0x58 */ f32 mAcceleration;
    /* 0x5c */ f32 mSlidingAngle;
    /* 0x60 */ f32 mClimbingAngle;
    /* 0x64 */ f32 mClimbingAnmSpeed;
    /* 0x68 */ f32 mClimbingMaxSpeed;
    /* 0x6c */ f32 mClimbingAnmSpeedWeak;
    /* 0x70 */ f32 mClimbingMaxSpeedWeak;
    /* 0x74 */ f32 mSlipDownAcceleration;
    /* 0x78 */ f32 mSlipDownMaxSpeed;
};  // Size: 0x7C

class daAlinkHIO_wlSlide_c0 {
public:
    static daAlinkHIO_wlSlide_c1 const m;
};

class daAlinkHIO_wlSlide_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlSlide_c();
    ~daAlinkHIO_wlSlide_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlSlide_c1 m;
#endif
};

class daAlinkHIO_wlRope_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mFlipAnm;
    /* 0x14 */ s16 mSwingAngle;
    /* 0x16 */ s16 mMovementSwayTransitionTime;
    /* 0x18 */ f32 mMovementMinAnmSpeed;
    /* 0x1c */ f32 mMovementMaxAnmSpeed;
    /* 0x20 */ f32 mMovementInterpolation;
    /* 0x24 */ f32 mMovementRate;
    /* 0x28 */ f32 mTwistClimbAnmSpeed;
    /* 0x2c */ f32 mFallInterpolation;
    /* 0x30 */ f32 mTwistClimbInterpolation;
    /* 0x34 */ f32 mBackMovementMaxAnmSpeed;
};  // Size: 0x38

class daAlinkHIO_wlRope_c0 {
public:
    static daAlinkHIO_wlRope_c1 const m;
};

class daAlinkHIO_wlRope_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlRope_c();
    ~daAlinkHIO_wlRope_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlRope_c1 m;
#endif
};

class daAlinkHIO_wlAtWait_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mAttackAnm;
    /* 0x14 */ s16 mStopTime;
    /* 0x16 */ s16 mComboMidStopTime;
    /* 0x18 */ f32 mSpeed;
    /* 0x1C */ f32 mSpeedAddForward;
    /* 0x20 */ f32 mAttackJudgmentStartForward;
    /* 0x24 */ f32 mAttackJudgmentEndForward;
    /* 0x28 */ f32 mComboMidCF;
    /* 0x2C */ f32 mComboMidStartF;
    /* 0x30 */ f32 mAttackRadiusOffset;
    /* 0x34 */ f32 mAttackRadius;
    /* 0x38 */ f32 mAttackHeight;
};  // Size: 0x3C

class daAlinkHIO_wlAtWaTl_c0 {
public:
    static daAlinkHIO_wlAtWait_c1 const m;
};

class daAlinkHIO_wlAtWaSc_c0 {
public:
    static daAlinkHIO_wlAtWait_c1 const m;
};

class daAlinkHIO_wlAtWaLr_c0 {
public:
    static daAlinkHIO_wlAtWait_c1 const m;
};

class daAlinkHIO_wlAtWait_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlAtWait_c(int, float);
    ~daAlinkHIO_wlAtWait_c();

    virtual void genMessage(JORMContext*);
#endif

public:
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ daAlinkHIO_wlAtWait_c1 m;
};  // Size: 0x74

class daAlinkHIO_wlAtRoll_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mAttackAnm;
    /* 0x14 */ f32 mAttackRadius;
    /* 0x18 */ f32 mAttackSpeed;
    /* 0x1c */ f32 mSpiderThreadEscapeStartF;
};  // Size: 0x20

class daAlinkHIO_wlAtRoll_c0 {
public:
    static daAlinkHIO_wlAtRoll_c1 const m;
};

class daAlinkHIO_wlAtRoll_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlAtRoll_c();
    ~daAlinkHIO_wlAtRoll_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_wlAtRoll_c1 m;
#endif
};  // Size: 0x54

class daAlinkHIO_wlAtNjump_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mAerialAttackAnm;
    /* 0x14 */ f32 mAttackInitSpeed;
    /* 0x18 */ f32 mMaxHorizontalSpeed;
    /* 0x1c */ f32 mMaxVerticalSpeed;
    /* 0x20 */ f32 mMinVerticalSpeed;
    /* 0x24 */ f32 mAerialAnmSpeed;
    /* 0x28 */ f32 mAerialInterpolation;
    /* 0x2c */ f32 mAttackRadiusOffset;
    /* 0x30 */ f32 mAttackRadius;
    /* 0x34 */ f32 mAttackHeight;
    /* 0x38 */ f32 mMinHorizontalSpeed;
};  // Size: 0x3C

class daAlinkHIO_wlAtNjump_c0 {
public:
    static daAlinkHIO_wlAtNjump_c1 const m;
};

class daAlinkHIO_wlAtNjump_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlAtNjump_c();
    ~daAlinkHIO_wlAtNjump_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_wlAtNjump_c1 m;
#endif
};  // Size: 0x70

class daAlinkHIO_wlAtCjump_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mAerialAttackAnm;
    /* 0x14 */ daAlinkHIO_anm_c mFlipKickAnm;
    /* 0x28 */ f32 mAttackInitSpeed;
    /* 0x2c */ f32 mMaxHorizontalSpeed;
    /* 0x30 */ f32 mMaxVerticalSpeed;
    /* 0x34 */ f32 mMinVerticalSpeed;
    /* 0x38 */ f32 mAerialAnmSpeed;
    /* 0x3c */ f32 mAerialInterpolation;
    /* 0x40 */ f32 mFlipKickPostStartF;
    /* 0x44 */ f32 mFlipKickPostAnmSpeed;
    /* 0x48 */ f32 mAttackRadiusOffset;
    /* 0x4c */ f32 mAttackRadius;
    /* 0x50 */ f32 mAttackHeight;
    /* 0x54 */ f32 mMinHorizontalSpeed;
    /* 0x58 */ f32 mHitDecelRate;
    /* 0x5c */ f32 mHitMinSpeed;
    /* 0x60 */ f32 mHitDecel;
};  // Size: 0x64

class daAlinkHIO_wlAtCjump_c0 {
public:
    static daAlinkHIO_wlAtCjump_c1 const m;
};

class daAlinkHIO_wlAtCjump_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlAtCjump_c();
    ~daAlinkHIO_wlAtCjump_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_wlAtCjump_c1 m;
#endif
};  // Size: 0x98

class daAlinkHIO_wlAtLand_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mNormalLandingAnm;
    /* 0x14 */ daAlinkHIO_anm_c mFrontSlideAnm;
    /* 0x28 */ daAlinkHIO_anm_c mBackSlideAnm;
    /* 0x3C */ f32 mSlideDeceleration;
};  // Size: 0x40

class daAlinkHIO_wlAtLand_c0 {
public:
    static daAlinkHIO_wlAtLand_c1 const m;
};

class daAlinkHIO_wlAtLand_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlAtLand_c();
    ~daAlinkHIO_wlAtLand_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_wlAtLand_c1 m;
#endif
};  // Size: 0x74

class daAlinkHIO_wlAtDown_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mLowStanceAnm;
    /* 0x14 */ daAlinkHIO_anm_c mPushDownAnm;
    /* 0x28 */ daAlinkHIO_anm_c mFrontRollAnm;
    /* 0x3C */ daAlinkHIO_anm_c mCrouchAnm;
    /* 0x50 */ daAlinkHIO_anm_c mBackRollAnm;
    /* 0x64 */ daAlinkHIO_anm_c mRollOverAnm;
    /* 0x78 */ daAlinkHIO_anm_c mProneGetUpAnm;
    /* 0x8C */ s16 field_0x8C;
    /* 0x8E */ s16 mFallDuration;
    /* 0x90 */ f32 mSideRollMovementWidth;
    /* 0x94 */ f32 mLandingSideRollMovementWidth;
    /* 0x98 */ f32 mLandingFrontBackMovementWidth;
};  // Size: 0x9C

class daAlinkHIO_wlAtDown_c0 {
public:
    static daAlinkHIO_wlAtDown_c1 const m;
};

class daAlinkHIO_wlAtDown_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlAtDown_c();
    ~daAlinkHIO_wlAtDown_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_wlAtDown_c1 m;
#endif
};  // Size: 0xD0

class daAlinkHIO_wlAtLock_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mFrontLandingAnm;
    /* 0x14 */ daAlinkHIO_anm_c mRearLandingAnm;
    /* 0x28 */ s16 mAutoActivationTime;
    /* 0x2C */ f32 mStartRadius;
    /* 0x30 */ f32 mMaxRadius;
    /* 0x34 */ f32 mRadiusAcceleration;
    /* 0x38 */ f32 mAttackSpeed;
    /* 0x3C */ f32 mAttackRadiusOffset;
    /* 0x40 */ f32 mAttackRadius;
    /* 0x44 */ f32 mAttackHeight;
    /* 0x48 */ f32 mGravity;
    /* 0x4C */ f32 mMaxJumpHeight;
    /* 0x50 */ f32 mMaxJumpDistance;
};  // Size: 0x54

class daAlinkHIO_wlAtLock_c0 {
public:
    static daAlinkHIO_wlAtLock_c1 const m;
};

class daAlinkHIO_wlAtLock_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlAtLock_c();
    ~daAlinkHIO_wlAtLock_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_wlAtLock_c1 m;
#endif
};  // Size: 0x88

class daAlinkHIO_wlAtBite_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mBitePreparationAnm;
    /* 0x14 */ daAlinkHIO_anm_c mHangingBitePreparationAnm;
    /* 0x28 */ s16 mBiteAttachTime;
    /* 0x2A */ s16 field_0x2A;
    /* 0x2C */ f32 mEnemyBiteThrowAnmSpeed;
    /* 0x30 */ f32 mEnemyBiteThrowInterpolation;
    /* 0x34 */ f32 mEnemyBiteThrowCF;
    /* 0x38 */ f32 mBitePreparationMidStartF;
    /* 0x3C */ f32 mBiteMidAnmSpeedMin;
    /* 0x40 */ f32 mHangingBiteMidAnmSpeedMin;
    /* 0x44 */ f32 mBiteMidAnmSpeedMax;
    /* 0x48 */ f32 mHangingBiteMidAnmSpeedMax;
    /* 0x4C */ f32 mBiteMidAnmSpeedAcceleration;
    /* 0x50 */ f32 mHangingBiteMidAnmSpeedAcceleration;
    /* 0x54 */ f32 mBiteMidAnmSpeedDeceleration;
    /* 0x58 */ f32 mHangingBiteMidAnmSpeedDeceleration;
};  // Size: 0x5C

class daAlinkHIO_wlAtBite_c0 {
public:
    static daAlinkHIO_wlAtBite_c1 const m;
};

class daAlinkHIO_wlAtBite_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlAtBite_c();
    ~daAlinkHIO_wlAtBite_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_wlAtBite_c1 m;
#endif
};  // Size: 0x90

class daAlinkHIO_wlAttack_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mJumpBackLandAnm;
    /* 0x14 */ s16 mChargeTime;
    /* 0x16 */ s16 mComboDuration;
    /* 0x18 */ f32 mFallHeight;
    /* 0x1C */ f32 mFallInterpolation;
    /* 0x20 */ f32 mReadyInterpolation;
    /* 0x24 */ f32 mAttackRange;
    /* 0x28 */ f32 mJumpBackSpeedH;
    /* 0x2C */ f32 mJumpBackSpeedV;
    /* 0x30 */ f32 mJumpAttackSpeedV;
    /* 0x34 */ f32 mJumpAttackSpeedH;
};  // Size: 0x38

class daAlinkHIO_wlAttack_c0 {
public:
    static  daAlinkHIO_wlAtWaLr_c0 const mHorizontalAttack;
    static  daAlinkHIO_wlAtWaSc_c0 const mScratchAttack;
    static  daAlinkHIO_wlAtWaTl_c0 const mTailAttack;
    static  daAlinkHIO_wlAtRoll_c0 const mAtRoll;
    static  daAlinkHIO_wlAtNjump_c0 const mWlAtNjump;
    static  daAlinkHIO_wlAtCjump_c0 const mWlAtCjump;
    static  daAlinkHIO_wlAtLand_c0 const mWlAtLand;
    static  daAlinkHIO_wlAtDown_c0 const mWlAtDown;
    static  daAlinkHIO_wlAtLock_c0 const mWlAtLock;
    static  daAlinkHIO_wlAtBite_c0 const mWlAtBite;
    static  daAlinkHIO_wlAttack_c1 const m;
};

class daAlinkHIO_wlAttack_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlAttack_c();
    ~daAlinkHIO_wlAttack_c();

    virtual void genMessage(JORMContext*);
#endif

public:
    /* 0x034 */ daAlinkHIO_wlAtWait_c mHorizontalAttack;
    /* 0x0A8 */ daAlinkHIO_wlAtWait_c mScratchAttack;
    /* 0x11C */ daAlinkHIO_wlAtWait_c mTailAttack;
    /* 0x190 */ daAlinkHIO_wlAtRoll_c mAtRoll;
    /* 0x1E4 */ daAlinkHIO_wlAtNjump_c mWlAtNjump;
    /* 0x254 */ daAlinkHIO_wlAtCjump_c mWlAtCjump;
    /* 0x2EC */ daAlinkHIO_wlAtLand_c mWlAtLand;
    /* 0x360 */ daAlinkHIO_wlAtDown_c mWlAtDown;
    /* 0x430 */ daAlinkHIO_wlAtLock_c mWlAtLock;
    /* 0x4B8 */ daAlinkHIO_wlAtBite_c mWlAtBite;
    /* 0x548 */ daAlinkHIO_wlAttack_c1 m;
};

class daAlinkHIO_wlPoint_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mForwardJumpAnm;
    /* 0x14 */ daAlinkHIO_anm_c mBackwardJumpAnm;
    /* 0x28 */ daAlinkHIO_anm_c mLandingAnm;
    /* 0x3C */ daAlinkHIO_anm_c mTagLandingAnm;
    /* 0x50 */ s16 mLockJumpStopTime;
    /* 0x54 */ f32 mIdleAnmSpeed;
    /* 0x58 */ f32 mIdleInterpolation;
    /* 0x5C */ f32 mAerialInterpolation;
    /* 0x60 */ f32 mJumpSpeed;
    /* 0x64 */ f32 mFailureJumpSpeed;
    /* 0x68 */ f32 mTagJumpSpeed;
    /* 0x6C */ f32 mLockLandingIceInitSpeed;
    /* 0x70 */ f32 mLockLandingIceDecelRate;
    /* 0x74 */ f32 mLockLandingIceMaxDecel;
    /* 0x78 */ f32 mLockLandingIceMinDecel;
};  // Size: 0x7C

class daAlinkHIO_wlPoint_c0 {
public:
    static daAlinkHIO_wlPoint_c1 const m;
};

class daAlinkHIO_wlPoint_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlPoint_c();
    ~daAlinkHIO_wlPoint_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlPoint_c1 m;
#endif
};

class daAlinkHIO_wlChain_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mLoweringStartAnm;
    /* 0x14 */ daAlinkHIO_anm_c mDigAnm;
    /* 0x28 */ daAlinkHIO_anm_c mDigFallAnm;
    /* 0x3C */ s16 mTensionWaitTime;
    /* 0x3E */ s16 mReleaseWaitTime;
    /* 0x40 */ f32 mWaitAnmSpeed;
    /* 0x44 */ f32 mWaitInterp;
    /* 0x48 */ f32 mSwingAnmSpeed;
    /* 0x4C */ f32 mSwingInterp;
    /* 0x50 */ f32 mFallSpeed;
    /* 0x54 */ f32 mSniffAnmSpeed;
    /* 0x58 */ f32 mSniffInterp;
};  // Size: 0x5C

class daAlinkHIO_wlChain_c0 {
public:
    static daAlinkHIO_wlChain_c1 const m;
};

class daAlinkHIO_wlChain_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlChain_c();
    ~daAlinkHIO_wlChain_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlChain_c1 m;
#endif
};

class daAlinkHIO_wlSwim_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mSurfacingAnm;
    /* 0x14 */ daAlinkHIO_anm_c mWaterSplashIdleAnm;
    /* 0x28 */ daAlinkHIO_anm_c mDashAnm;
    /* 0x3C */ s16 mTurnRate;
    /* 0x3E */ s16 mTurnMin;
    /* 0x40 */ s16 mTurnMax;
    /* 0x42 */ s16 mIdleTurnRate;
    /* 0x44 */ s16 mIdleTurnMin;
    /* 0x46 */ s16 mIdleTurnMax;
    /* 0x48 */ f32 mStartHeight;
    /* 0x4C */ f32 mSurfacingHeight;
    /* 0x50 */ f32 mMaxSpeed;
    /* 0x54 */ f32 mDeceleration;
    /* 0x58 */ f32 mMaxFallSpeed;
    /* 0x5C */ f32 mMaxSurfacingSpeed;
    /* 0x60 */ f32 mBuoyancy;
    /* 0x64 */ f32 mIdleAnmSpeed;
    /* 0x68 */ f32 mIdleInterp;
    /* 0x6C */ f32 mIdleUpDownSwayAmount;
    /* 0x70 */ f32 mMoveMinAnmSpeed;
    /* 0x74 */ f32 mMoveMaxAnmSpeed;
    /* 0x78 */ f32 mMoveInterp;
    /* 0x7C */ f32 mClimbHeight;
    /* 0x80 */ f32 mMaxSpeedWeak;
    /* 0x84 */ f32 mDecelerationWeak;
    /* 0x88 */ f32 mIdleAnmSpeedWeak;
    /* 0x8C */ f32 mMoveMinAnmSpeedWeak;
    /* 0x90 */ f32 mMoveMaxAnmSpeedWeak;
    /* 0x94 */ f32 mClimbBelowWaterSurface;
    /* 0x98 */ f32 mHeavyIdleUpDownSwayAmount;
    /* 0x9C */ f32 mHeavyBuoyancy;
    /* 0xA0 */ f32 mHeavyMaxFallSpeed;
    /* 0xA4 */ f32 mHeavyMaxSurfacingSpeed;
};  // Size: 0xA8

class daAlinkHIO_wlSwim_c0 {
public:
    static daAlinkHIO_wlSwim_c1 const m;
};

class daAlinkHIO_wlSwim_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlSwim_c();
    ~daAlinkHIO_wlSwim_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlSwim_c1 m;
#endif
};

class daAlinkHIO_wlGrab_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mPotGrabAnm;
    /* 0x14 */ daAlinkHIO_anm_c mPotReleaseAnm;
    /* 0x28 */ daAlinkHIO_anm_c mBoneGrabAnm;
    /* 0x3C */ daAlinkHIO_anm_c mBoneReleaseAnm;
    /* 0x50 */ daAlinkHIO_anm_c mThrowAnm;
};  // Size: 0x64

class daAlinkHIO_wlGrab_c0 {
public:
    static daAlinkHIO_wlGrab_c1 const m;
};

class daAlinkHIO_wlGrab_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlGrab_c();
    ~daAlinkHIO_wlGrab_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlGrab_c1 m;
#endif
};

class daAlinkHIO_wlBall_c1 {
public:
    /* 0x00 */ daAlinkHIO_anm_c mFullBodyAnm;
    /* 0x14 */ daAlinkHIO_anm_c mNeckAnm;
    /* 0x28 */ s16 mMpConsumptionTime;
    /* 0x2C */ f32 mLifeOrbGravity;
    /* 0x30 */ f32 mLifeOrbHorizSpeed;
    /* 0x34 */ f32 mLifeOrbVertSpeed;
};  // Size: 0x38

class daAlinkHIO_wlBall_c0 {
public:
    static daAlinkHIO_wlBall_c1 const m;
};

class daAlinkHIO_wlBall_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wlBall_c();
    ~daAlinkHIO_wlBall_c();

    virtual void genMessage(JORMContext*);
#endif

public:
#if DEBUG
    /* 0x00 */ daAlinkHIO_wlBall_c1 m;
#endif
};

class daAlinkHIO_wolf_c1 {
public:
    /* 0x00 */ s16 mMaxNeckTurnH;
    /* 0x02 */ s16 mMaxNeckTurnUp;
    /* 0x04 */ s16 mMaxNeckTurnDown;
    /* 0x06 */ s16 mMaxTiredNeckTurnH;
    /* 0x08 */ s16 mMaxTiredNeckTurnUp;
    /* 0x0A */ s16 mMaxTiredNeckTurnDown;
    /* 0x0C */ s16 mSensesLingerTime;  // ?
    /* 0x0E */ s16 mLightDropR;
    /* 0x10 */ s16 mLightDropG;
    /* 0x12 */ s16 mLightDropB;
    /* 0x14 */ f32 mUnderwaterInputRate;
};  // Size: 0x18

class daAlinkHIO_wolf_c0 {
public:
    static daAlinkHIO_wlMove_c0 const mWlMove;
    static daAlinkHIO_wlMoveNoP_c0 const mWlMoveNoP;
    static daAlinkHIO_wlAtnMove_c0 const mWlAtnMove;
    static daAlinkHIO_wlSideStep_c0 const mWlSideStep;
    static daAlinkHIO_wlBackJump_c0 const mWlBackJump;
    static daAlinkHIO_wlHowl_c0 const mWlHowl;
    static daAlinkHIO_wlAutoJump_c0 const mWlAutoJump;
    static daAlinkHIO_wlPush_c0 const mWlPush;
    static daAlinkHIO_wlLie_c0 const mWlLie;
    static daAlinkHIO_wlLight_c0 const mLight;
    static daAlinkHIO_wlWallHang_c0 const mWlWallHang;
    static daAlinkHIO_wlDamage_c0 const mWlDamage;
    static daAlinkHIO_wlSlide_c0 const mWlSlide;
    static daAlinkHIO_wlRope_c0 const mWlRope;
    static daAlinkHIO_wlAttack_c0 const mWlAttack;
    static daAlinkHIO_wlPoint_c0 const mWlPoint;
    static daAlinkHIO_wlChain_c0 const mWlChain;
    static daAlinkHIO_wlSwim_c0 const mWlSwim;
    static daAlinkHIO_wlGrab_c0 const mWlGrab;
    static daAlinkHIO_wlBall_c0 const mWlBall;
    static daAlinkHIO_wolf_c1 const m;
};

class daAlinkHIO_wolf_c : public daAlinkHIO_data_c {
public:
#if DEBUG
    daAlinkHIO_wolf_c();
    ~daAlinkHIO_wolf_c();

    virtual void genMessage(JORMContext*);
#else
    ~daAlinkHIO_wolf_c();
#endif

public:
#if DEBUG
    /* 0x34 */ daAlinkHIO_wlMove_c mWlMove;
    /* 0x00 */ daAlinkHIO_wlMoveNoP_c mWlMoveNoP;
    /* 0x00 */ daAlinkHIO_wlAtnMove_c mWlAtnMove;
    /* 0x00 */ daAlinkHIO_wlSideStep_c mWlSideStep;
    /* 0x00 */ daAlinkHIO_wlBackJump_c mWlBackJump;
    /* 0x00 */ daAlinkHIO_wlHowl_c mWlHowl;
    /* 0x00 */ daAlinkHIO_wlAutoJump_c mWlAutoJump;
    /* 0x00 */ daAlinkHIO_wlPush_c mWlPush;
    /* 0x00 */ daAlinkHIO_wlLie_c mWlLie;
    /* 0x00 */ daAlinkHIO_light_c mLight;
    /* 0x00 */ daAlinkHIO_wlWallHang_c mWlWallHang;
    /* 0x00 */ daAlinkHIO_wlDamage_c mWlDamage;
    /* 0x00 */ daAlinkHIO_wlSlide_c mWlSlide;
    /* 0x00 */ daAlinkHIO_wlRope_c mWlRope;
    /* 0x00 */ daAlinkHIO_wlAttack_c mWlAttack;
    /* 0x00 */ daAlinkHIO_wlPoint_c mWlPoint;
    /* 0x00 */ daAlinkHIO_wlChain_c mWlChain;
    /* 0x00 */ daAlinkHIO_wlSwim_c mWlSwim;
    /* 0x00 */ daAlinkHIO_wlGrab_c mWlGrab;
    /* 0x00 */ daAlinkHIO_wlBall_c mWlBall;
    /* 0x00 */ daAlinkHIO_wolf_c1 m;
#endif
};

class daAlinkHIO_c
#if DEBUG
    : public mDoHIO_entry_c
#endif
{
public:
    daAlinkHIO_c();
    virtual ~daAlinkHIO_c();

    void jumpStateUpdate(const cXyz*, const cXyz*, f32);
    void genMessage(JORMContext*);
#if DEBUG
    void readFileData(char*);
    size_t makeFileOutData(char*, char*);
    void listenPropertyEvent(const JORPropertyEvent*);

    /* 0x0008 */ daAlinkHIO_basic_c mBasic;
    /* 0x0094 */ daAlinkHIO_move_c mMove;
    /* 0x0120 */ daAlinkHIO_atnMove_c mAtnMove;
    /* 0x01A8 */ daAlinkHIO_noActAtnMove_c mNoActAtnMove;
    /* 0x0230 */ daAlinkHIO_frontRoll_c mFrontRoll;
    /* 0x02D4 */ daAlinkHIO_backJump_c mBackJump;
    /* 0x0340 */ daAlinkHIO_sideStep_c mSideStep;
    /* 0x03DC */ daAlinkHIO_slide_c mSlide;
    /* 0x0488 */ daAlinkHIO_cut_c mCut;
    /* 0x0FFC */ daAlinkHIO_guard_c mGuard;
    /* 0x11C8 */ daAlinkHIO_crouch_c mCrouch;
    /* 0x1248 */ daAlinkHIO_autoJump_c mAutoJump;
    /* 0x1330 */ daAlinkHIO_wallHang_c mWallHang;
    /* 0x1528 */ daAlinkHIO_pushpull_c mPushpull;
    /* 0x1598 */ daAlinkHIO_damage_c mDamage;
    /* 0x1A90 */ daAlinkHIO_horse_c mHorse;
    /* 0x1B30 */ daAlinkHIO_canoe_c mCanoe;
    /* 0x1BAC */ daAlinkHIO_item_c mItem;
    /* 0x2420 */ daAlinkHIO_ladder_c mLadder;
    /* 0x24C4 */ daAlinkHIO_roofHang_c mRoofHang;
    /* 0x2544 */ daAlinkHIO_grab_c mGrab;
    /* 0x265C */ daAlinkHIO_swim_c mSwim;
    /* 0x278C */ daAlinkHIO_wolf_c mWolf;
#else
    // TODO: What's with these first few members on retail?
    // The individual HIO members (e.g. mCut) have their class changed on retail compared to debug (_c -> _c0).
    // But it seems like the debug _c members might also exist on retail based on the dtor.
    // But they can't have the same name as they do on debug since those names are reserved for the _c0 version.
    /* 0x04 */ u8 field_0x4[0xC - 0x4];
    /* 0x0C */ daAlinkHIO_cut_c mCut_2;
    /* 0x0D */ u8 field_0xD[0x4B - 0xD];
    /* 0x4B */ daAlinkHIO_wolf_c mWolf_2;

    daAlinkHIO_basic_c0 mBasic;
    daAlinkHIO_move_c0 mMove;
    daAlinkHIO_atnMove_c0 mAtnMove;
    daAlinkHIO_noActAtnMove_c0 mNoActAtnMove;
    daAlinkHIO_frontRoll_c0 mFrontRoll;
    daAlinkHIO_backJump_c0 mBackJump;
    daAlinkHIO_sideStep_c0 mSideStep;
    daAlinkHIO_slide_c0 mSlide;
    daAlinkHIO_cut_c0 mCut;
    daAlinkHIO_guard_c0 mGuard;
    daAlinkHIO_crouch_c0 mCrouch;
    daAlinkHIO_autoJump_c0 mAutoJump;
    daAlinkHIO_wallHang_c0 mWallHang;
    daAlinkHIO_pushpull_c0 mPushpull;
    daAlinkHIO_damage_c0 mDamage;
    daAlinkHIO_horse_c0 mHorse;
    daAlinkHIO_canoe_c0 mCanoe;
    daAlinkHIO_item_c0 mItem;
    daAlinkHIO_ladder_c0 mLadder;
    daAlinkHIO_roofHang_c0 mRoofHang;
    daAlinkHIO_grab_c0 mGrab;
    daAlinkHIO_swim_c0 mSwim;
    daAlinkHIO_wolf_c0 mWolf;

    u8 padding[0x6C - 0x64];
#endif
};

class daAlink_c;
typedef int (daAlink_c::*daAlink_procFunc)();
typedef void (daAlink_c::*EffParamProc)();

struct daAlink_procInitTable {
    /* 0x0 */ daAlink_procFunc m_procFunc;
    /* 0xC */ u32 m_mode;
};  // Size: 0x10

class daAlink_c : public daPy_py_c {
public:
    enum daAlink_ANM {
        /* 0x000 */ ANM_ATN_RETURN_FROM_WALK,
        /* 0x001 */ ANM_ATN_RETURN_FROM_RUN,
        /* 0x002 */ ANM_ATN_BACK_WALK,
        /* 0x003 */ ANM_ATN_BACK_RUN,
        /* 0x004 */ ANM_ATN_LOCK_BACK_WALK,
        /* 0x005 */ ANM_ATN_LOCK_BACK_RUN,
        /* 0x006 */ ANM_ATN_WALK_LEFT,
        /* 0x007 */ ANM_ATN_LOCK_WALK_LEFT,
        /* 0x008 */ ANM_ATN_WALK_RIGHT,
        /* 0x009 */ ANM_ATN_LOCK_WALK_RIGHT,
        /* 0x00A */ ANM_ATN_RUN_LEFT,
        /* 0x00B */ ANM_ATN_LOCK_RUN_LEFT,
        /* 0x00C */ ANM_ATN_RUN_RIGHT,
        /* 0x00D */ ANM_ATN_LOCK_RUN_RIGHT,
        /* 0x00E */ ANM_WALK_HEAVY,
        /* 0x00F */ ANM_ATN_BACK_WALK_HEAVY,
        /* 0x010 */ ANM_ATN_WAIT_LEFT,
        /* 0x011 */ ANM_ATN_WAIT_RIGHT,
        /* 0x012 */ ANM_WALK,
        /* 0x013 */ ANM_RUN,
        /* 0x014 */ ANM_SWIM_WAIT,
        /* 0x015 */ ANM_STEP_TURN,
        /* 0x016 */ ANM_SMALL_GUARD,
        /* 0x017 */ ANM_STEP_TURN_TIRED,
        /* 0x018 */ ANM_SMALL_GUARD_TIRED,
        /* 0x019 */ ANM_WAIT,
        /* 0x01A */ ANM_WAIT_B,
        /* 0x01B */ ANM_WAIT_B_TO_A,
        /* 0x01C */ ANM_RUN_B,
        /* 0x01D */ ANM_SIDE_JUMP_LEFT,
        /* 0x01E */ ANM_SIDE_JUMP_RIGHT,
        /* 0x01F */ ANM_SIDE_JUMP_LEFT_LAND,
        /* 0x020 */ ANM_SIDE_JUMP_RIGHT_LAND,
        /* 0x021 */ ANM_BACK_JUMP,
        /* 0x022 */ ANM_BACK_JUMP_LAND,
        /* 0x023 */ ANM_FRONT_ROLL,
        /* 0x024 */ ANM_ROLL_CRASH,
        /* 0x025 */ ANM_BARRIER_RECOIL,  // ?
        /* 0x026 */ ANM_BACKFLIP,
        /* 0x027 */ ANM_BACKFLIP_LAND,
        /* 0x028 */ ANM_SLIP,
        /* 0x029 */ ANM_CROUCH,
        /* 0x02A */ ANM_CROUCH_START,
        /* 0x02B */ ANM_GUARD_LEFT,
        /* 0x02C */ ANM_GUARD_RIGHT,
        /* 0x02D */ ANM_CROUCH_DEF,  // ?
        /* 0x02E */ ANM_SHIELD_ATTACK,
        /* 0x02F */ ANM_SHIELD_ATTACK_RECOIL,
        /* 0x030 */ ANM_GUARD_BREAK,
        /* 0x031 */ ANM_JUMP_START,
        /* 0x032 */ ANM_JUMP,
        /* 0x033 */ ANM_JUMP_LAND,
        /* 0x034 */ ANM_CRAWL_START,
        /* 0x035 */ ANM_CRAWL,
        /* 0x036 */ ANM_WAIT_PUSH_PULL,
        /* 0x037 */ ANM_PUSH_LIGHT,
        /* 0x038 */ ANM_PUSH_START,
        /* 0x039 */ ANM_PUSH,
        /* 0x03A */ ANM_PULL_LIGHT,
        /* 0x03B */ ANM_PULL_START,
        /* 0x03C */ ANM_PULL,
        /* 0x03D */ ANM_HORSE_MOUNT_LEFT,
        /* 0x03E */ ANM_HORSE_MOUNT_RIGHT,
        /* 0x03F */ ANM_HORSE_MOUNT_BACK,
        /* 0x040 */ ANM_HORSE_GETOFF_LEFT,
        /* 0x041 */ ANM_HORSE_GETOFF_RIGHT,
        /* 0x042 */ ANM_HORSE_STOP,
        /* 0x043 */ ANM_HORSE_STOP_TO_STAND,
        /* 0x044 */ ANM_HORSE_STAND,
        /* 0x045 */ ANM_HORSE_JUMP_START,
        /* 0x046 */ ANM_HORSE_JUMP,
        /* 0x047 */ ANM_HORSE_JUMP_LAND,
        /* 0x048 */ ANM_HORSE_WAIT,
        /* 0x049 */ ANM_HORSE_WALK_A,
        /* 0x04A */ ANM_HORSE_WALK_B,
        /* 0x04B */ ANM_HORSE_DASH_A,
        /* 0x04C */ ANM_HORSE_DASH_B,
        /* 0x04D */ ANM_WSTARTH,  // ?
        /* 0x04E */ ANM_HORSE_DASH_START,
        /* 0x04F */ ANM_HORSE_LASH,
        /* 0x050 */ ANM_HORSE_CUT_TURN,
        /* 0x051 */ ANM_HORSE_WAIT_A,
        /* 0x052 */ ANM_HORSE_WAIT_B,
        /* 0x053 */ ANM_HORSE_DIE_RECOVER,
        /* 0x054 */ ANM_HORSE_TURN_B,
        /* 0x055 */ ANM_HORSE_CROUCH,
        /* 0x056 */ ANM_HORSE_DMG_LEFT,
        /* 0x057 */ ANM_HORSE_DMG_RIGHT,
        /* 0x058 */ ANM_HORSE_HANG_LEFT,
        /* 0x059 */ ANM_HORSE_HANG_RIGHT,
        /* 0x05A */ ANM_HORSE_TURN_LEFT,
        /* 0x05B */ ANM_HORSE_TURN_RIGHT,
        /* 0x05C */ ANM_HORSE_RIDE_LEFT,
        /* 0x05D */ ANM_HORSE_RIDE_RIGHT,
        /* 0x05E */ ANM_HORSE_FALL_LEFT,
        /* 0x05F */ ANM_HORSE_FALL_RIGHT,
        /* 0x060 */ ANM_HORSE_TAME_WAIT_B,
        /* 0x061 */ ANM_HORSE_TAME_WAIT_A_TO_B,
        /* 0x062 */ ANM_CUT_NM_VERTICAL,
        /* 0x063 */ ANM_CUT_NM_LEFT,
        /* 0x064 */ ANM_CUT_NM_RIGHT,
        /* 0x065 */ ANM_CUT_COMBO_STAB,
        /* 0x066 */ ANM_CUT_NM_STAB,
        /* 0x067 */ ANM_CUT_FINISH_LEFT,
        /* 0x068 */ ANM_CUT_FINISH_VERTICAL,
        /* 0x069 */ ANM_CUT_FINISH_STAB,
        /* 0x06A */ ANM_CUT_MORTAL_DRAW_A,
        /* 0x06B */ ANM_CUT_MORTAL_DRAW_B,
        /* 0x06C */ ANM_CUT_FINISH_RIGHT,
        /* 0x06D */ ANM_CUT_TWIRL,
        /* 0x06E */ ANM_CUT_TWIRL_LAND,
        /* 0x06F */ ANM_CUT_RECOIL_A,
        /* 0x070 */ ANM_CUT_RECOIL_B,
        /* 0x071 */ ANM_CUT_JUMP,
        /* 0x072 */ ANM_CUT_JUMP_LAND,
        /* 0x073 */ ANM_CUT_TURN_LEFT,
        /* 0x074 */ ANM_CUT_TURN_RIGHT,
        /* 0x075 */ ANM_CUT_TURN_CHARGE_START,
        /* 0x076 */ ANM_CUT_TURN_CHARGE,
        /* 0x077 */ ANM_CUT_TURN_CHARGE_MOVE,
        /* 0x078 */ ANM_CUT_FINISHING_BLOW_JUMP,
        /* 0x079 */ ANM_CUT_FINISHING_BLOW_FALL,
        /* 0x07A */ ANM_CUT_FINISHING_BLOW_STAB,
        /* 0x07B */ ANM_CUT_FINISHING_BLOW_MISS,
        /* 0x07C */ ANM_CUT_HEAD_JUMP,
        /* 0x07D */ ANM_CUT_HEAD,
        /* 0x07E */ ANM_CUT_HEAD_LAND,
        /* 0x07F */ ANM_DMG_SMALL_A,
        /* 0x080 */ ANM_DMG_SMALL_B,
        /* 0x081 */ ANM_DMG_SMALL_LEFT,
        /* 0x082 */ ANM_DMG_SMALL_RIGHT,
        /* 0x083 */ ANM_DMG_AIR_FRONT,
        /* 0x084 */ ANM_DMG_LARGE,
        /* 0x085 */ ANM_DMG_AIR_LEFT,
        /* 0x086 */ ANM_DMG_AIR_RIGHT,
        /* 0x087 */ ANM_DMG_AIR_LAND,
        /* 0x088 */ ANM_DMG_LARGE_LAND,
        /* 0x089 */ ANM_DMG_AIR_LEFT_LAND,
        /* 0x08A */ ANM_DMG_AIR_RIGHT_LAND,
        /* 0x08B */ ANM_DMG,
        /* 0x08C */ ANM_DMG_FALL,
        /* 0x08D */ ANM_DMG_FALL_RECOVER,
        /* 0x08E */ ANM_FINISH,
        /* 0x08F */ ANM_FINISH_END,
        /* 0x090 */ ANM_SERVICE_WAIT,
        /* 0x091 */ ANM_CLIMB_JUMP,
        /* 0x092 */ ANM_HANG_READY,
        /* 0x093 */ ANM_CLIMB_TOP_SMALL,
        /* 0x094 */ ANM_HANG_CLIMB_TOP,
        /* 0x095 */ ANM_HANG_ONE_HAND,
        /* 0x096 */ ANM_HANG_ONE_HAND_MOVE,
        /* 0x097 */ ANM_HANG_MOVE_LEFT,
        /* 0x098 */ ANM_HANG_MOVE_RIGHT,
        /* 0x099 */ ANM_LADDER_UP_START,
        /* 0x09A */ ANM_LADDER_UP_END_LEFT,
        /* 0x09B */ ANM_LADDER_UP_END_RIGHT,
        /* 0x09C */ ANM_LADDER_DOWN_START,
        /* 0x09D */ ANM_LADDER_DOWN_END_LEFT,
        /* 0x09E */ ANM_LADDER_DOWN_END_RIGHT,
        /* 0x09F */ ANM_LADDER_RIGHT_TO_LEFT,
        /* 0x0A0 */ ANM_LADDER_LEFT_TO_RIGHT,
        /* 0x0A1 */ ANM_CLIMB_HANG,
        /* 0x0A2 */ ANM_CLIMB_START,
        /* 0x0A3 */ ANM_CLIMB_LEFT,
        /* 0x0A4 */ ANM_CLIMB_RIGHT,
        /* 0x0A5 */ ANM_CLIMB_SLIDE_LEFT,
        /* 0x0A6 */ ANM_CLIMB_SLIDE_RIGHT,
        /* 0x0A7 */ ANM_CLIMB_HANG_MISS,
        /* 0x0A8 */ ANM_ROOF_UP,
        /* 0x0A9 */ ANM_ROOF_HANG_WAIT,
        /* 0x0AA */ ANM_ROOF_HANG_MOVE,
        /* 0x0AB */ ANM_ROOF_HANG_LEFT,
        /* 0x0AC */ ANM_ROOF_HANG_RIGHT,
        /* 0x0AD */ ANM_ROOF_HANG_TURN,
        /* 0x0AE */ ANM_ROOF_HANG,
        /* 0x0AF */ ANM_GRAB_UP_START,
        /* 0x0B0 */ ANM_GRAB_UP,
        /* 0x0B1 */ ANM_THROW,
        /* 0x0B2 */ ANM_THROW_HEAVY,
        /* 0x0B3 */ ANM_PICK_UP_LARGE_START,
        /* 0x0B4 */ ANM_PICK_UP_LARGE,
        /* 0x0B5 */ ANM_CARRY,
        /* 0x0B6 */ ANM_WAIT_TIRED,
        /* 0x0B7 */ ANM_WAIT_TO_TIRED,
        /* 0x0B8 */ ANM_TALK,
        /* 0x0B9 */ ANM_WAIT_START,
        /* 0x0BA */ ANM_FROZEN_FREE,  // ?
        /* 0x0BB */ ANM_BOOMERANG_CATCH,
        /* 0x0BC */ ANM_WALK_SLOPE,
        /* 0x0BD */ ANM_SLIDE_FORWARD,
        /* 0x0BE */ ANM_SLIDE_BACKWARD,
        /* 0x0BF */ ANM_SLIDE_FORWARD_END,
        /* 0x0C0 */ ANM_SLIDE_BACKWARD_END,
        /* 0x0C1 */ ANM_SWIM_A,
        /* 0x0C2 */ ANM_SWIM_RESURFACE,
        /* 0x0C3 */ ANM_ATN_SWIM_LEFT,
        /* 0x0C4 */ ANM_ATN_SWIM_RIGHT,
        /* 0x0C5 */ ANM_ATN_SWIM_BACK,
        /* 0x0C6 */ ANM_SWIM_DASH,
        /* 0x0C7 */ ANM_SWIM_DIVE,
        /* 0x0C8 */ ANM_SWIM_ZORA_START,
        /* 0x0C9 */ ANM_SWIM_C,
        /* 0x0CA */ ANM_SWIM_DMG_FRONT,
        /* 0x0CB */ ANM_SWIM_DMG_BACK,
        /* 0x0CC */ ANM_SWIM_DMG_LEFT,
        /* 0x0CD */ ANM_SWIM_DMG_RIGHT,
        /* 0x0CE */ ANM_SWIM_DMG_FREEZE,
        /* 0x0CF */ ANM_TRES_OPEN_SMALL,
        /* 0x0D0 */ ANM_TRES_OPEN_KICK,
        /* 0x0D1 */ ANM_TRES_OPEN_BIG,
        /* 0x0D2 */ ANM_GET_A,
        /* 0x0D3 */ ANM_GET_A_WAIT,
        /* 0x0D4 */ ANM_GET_A_WAIT2,  // same under anm, upper anm (0x26A)
        /* 0x0D5 */ ANM_GET_HOLDOUT,
        /* 0x0D6 */ ANM_TURN_BACK,
        /* 0x0D7 */ ANM_VJUMP_START,
        /* 0x0D8 */ ANM_WAIT_SIT,
        /* 0x0D9 */ ANM_WAIT_CANOE_LEFT,
        /* 0x0DA */ ANM_WAIT_CANOE_RIGHT,
        /* 0x0DB */ ANM_CANOE_ROW_LEFT,
        /* 0x0DC */ ANM_CANOE_ROW_RIGHT,
        /* 0x0DD */ ANM_CANOE_ROW_LEFT_BACK,
        /* 0x0DE */ ANM_CANOE_ROW_RIGHT_BACK,
        /* 0x0DF */ ANM_CANOE_CHANGE_HAND,
        /* 0x0E0 */ ANM_CANOE_FISH_LEFT,
        /* 0x0E1 */ ANM_CANOE_FISH_RIGHT,
        /* 0x0E2 */ ANM_CANOE_REEL,
        /* 0x0E3 */ ANM_CANOE_RELEASE,
        /* 0x0E4 */ ANM_CANOE_PADDLE_TO_ROD,
        /* 0x0E5 */ ANM_REEL,
        /* 0x0E6 */ ANM_REEL_LEFT_RIGHT,
        /* 0x0E7 */ ANM_LAND_UP,  // ?
        /* 0x0E8 */ ANM_DOOR_OPEN_LEFT,
        /* 0x0E9 */ ANM_DOOR_OPEN_RIGHT,
        /* 0x0EA */ ANM_DOOR_OPEN_LOCK_LEFT,
        /* 0x0EB */ ANM_DOOR_OPEN_LOCK_RIGHT,
        /* 0x0EC */ ANM_DOOR_OPEN_ROLL,
        /* 0x0ED */ ANM_DOOR_OPEN_SLIDE,
        /* 0x0EE */ ANM_PICK_UP,
        /* 0x0EF */ ANM_ROLL_JUMP,
        /* 0x0F0 */ ANM_SIDE_ROLL_LEFT,
        /* 0x0F1 */ ANM_SIDE_ROLL_RIGHT,
        /* 0x0F2 */ ANM_RIDE_WAIT,
        /* 0x0F3 */ ANM_RIDE_FRONT,
        /* 0x0F4 */ ANM_RIDE_CROUCH,
        /* 0x0F5 */ ANM_RIDE_STOP,
        /* 0x0F6 */ ANM_RIDE_JUMP_START,
        /* 0x0F7 */ ANM_RIDE_JUMP,
        /* 0x0F8 */ ANM_RIDE_JUMP_LAND,
        /* 0x0F9 */ ANM_RIDE_KICK,
        /* 0x0FA */ ANM_RIDE_CUT_TURN_CHARGE,
        /* 0x0FB */ ANM_RIDE_CUT_TURN,
        /* 0x0FC */ ANM_ROPE_SWING,  // monkey swing?
        /* 0x0FD */ ANM_TRANSFORM_TO_WOLF,
        /* 0x0FE */ ANM_TRANSFORM_TO_HUMAN,
        /* 0x0FF */ ANM_WAIT_WIND,
        /* 0x100 */ ANM_BOTTLE_DRINK_START,
        /* 0x101 */ ANM_BOTTLE_DRINK,
        /* 0x102 */ ANM_BOTTLE_DRINK_END,
        /* 0x103 */ ANM_BOTTLE_DRINK_NASTY,
        /* 0x104 */ ANM_BOTTLE_OPEN,
        /* 0x105 */ ANM_BOTTLE_POUR,
        /* 0x106 */ ANM_BOTTLE_FAIRY,
        /* 0x107 */ ANM_BOTTLE_SWING,
        /* 0x108 */ ANM_BOTTLE_SWING_DOWN,
        /* 0x109 */ ANM_BOTTLE_GET,
        /* 0x10A */ ANM_BOTTLE_SCOOP,
        /* 0x10B */ ANM_LANTERN_SWING,
        /* 0x10C */ ANM_OIL_BOTTLE_POUR_START,
        /* 0x10D */ ANM_OIL_BOTTLE_POUR,
        /* 0x10E */ ANM_OIL_BOTTLE_POUR_END,
        /* 0x10F */ ANM_GRASS_WHISTLE_START,
        /* 0x110 */ ANM_GRASS_WHISTLE_PLAY,
        /* 0x111 */ ANM_HAWK_CATCH,
        /* 0x112 */ ANM_HAWK_WAIT,
        /* 0x113 */ ANM_WAIT_D_B,        // ghost rats?
        /* 0x114 */ ANM_HORSE_WAIT_D_B,  // ?
        /* 0x115 */ ANM_MG_KICK,         // ?
        /* 0x116 */ ANM_DIE,
        /* 0x117 */ ANM_HORSE_DIE,
        /* 0x118 */ ANM_SWIM_DIE,
        /* 0x119 */ ANM_SWIM_DROWN,
        /* 0x11A */ ANM_ATN_COW,
        /* 0x11B */ ANM_COW_CATCH_START,
        /* 0x11C */ ANM_COW_CATCH,
        /* 0x11D */ ANM_COW_A_D,  // ?
        /* 0x11E */ ANM_COW_PRESS,
        /* 0x11F */ ANM_COW_THROW_LEFT,
        /* 0x120 */ ANM_COW_THROW_RIGHT,
        /* 0x121 */ ANM_ATN_RIGHT,
        /* 0x122 */ ANM_COW_MOVE_LEFT_RIGHT,
        /* 0x123 */ ANM_COW_STROKE,
        /* 0x124 */ ANM_ENTRANCE,
        /* 0x125 */ ANM_FALL,
        /* 0x126 */ ANM_APPEARANCE,
        /* 0x127 */ ANM_S_JUMP_START,  // ?
        /* 0x128 */ ANM_S_JUMP_END,    // ?
        /* 0x129 */ ANM_HOOKSHOT_FLY_LEFT,
        /* 0x12A */ ANM_HOOKSHOT_FLY_RIGHT,
        /* 0x12B */ ANM_HOOKSHOT_SHOOT,
        /* 0x12C */ ANM_HOOKSHOT_HANG_END_LEFT,
        /* 0x12D */ ANM_HOOKSHOT_HANG_END_RIGHT,
        /* 0x12E */ ANM_HOOKSHOT_HANG_LEFT,
        /* 0x12F */ ANM_HOOKSHOT_HANG_RIGHT,
        /* 0x130 */ ANM_HOOKSHOT_HANG_WAIT_LEFT,
        /* 0x131 */ ANM_HOOKSHOT_HANG_WAIT_RIGHT,
        /* 0x132 */ ANM_HOOKSHOT_HANG_SHOOT_LEFT,
        /* 0x133 */ ANM_HOOKSHOT_HANG_SHOOT_RIGHT,
        /* 0x134 */ ANM_HOOKSHOT_HANG_LEFT_START,
        /* 0x135 */ ANM_HOOKSHOT_HANG_RIGHT_START,
        /* 0x136 */ ANM_HOOKSHOT_WALL_END_LEFT,
        /* 0x137 */ ANM_HOOKSHOT_WALL_END_RIGHT,
        /* 0x138 */ ANM_HOOKSHOT_WALL_LEFT,
        /* 0x139 */ ANM_HOOKSHOT_WALL_RIGHT,
        /* 0x13A */ ANM_HOOKSHOT_WALL_WAIT_LEFT,
        /* 0x13B */ ANM_HOOKSHOT_WALL_WAIT_RIGHT,
        /* 0x13C */ ANM_HOOKSHOT_WALL_SHOOT_LEFT,
        /* 0x13D */ ANM_HOOKSHOT_WALL_SHOOT_RIGHT,
        /* 0x13E */ ANM_HVY_BOOTS_PUT_ON,
        /* 0x13F */ ANM_SUMOU_MIAU,     // ?
        /* 0x140 */ ANM_SUMOU_HAKEYOI,  // ?
        /* 0x141 */ ANM_SUMOU_FIGHT_WAIT,
        /* 0x142 */ ANM_SUMOU_FIGHT_STEP_LEFT,
        /* 0x143 */ ANM_SUMOU_FIGHT_STEP_RIGHT,
        /* 0x144 */ ANM_SUMOU_FIGHT_STEP,
        /* 0x145 */ ANM_SUMOU_TACKLE,
        /* 0x146 */ ANM_SUMOU_TACKLE_GORON,  // ?
        /* 0x147 */ ANM_SUMOU_DODGE_TACKLE,
        /* 0x148 */ ANM_SUMOU_TACKLE_MISS,
        /* 0x149 */ ANM_SUMOU_TACKLE_SHOCK,
        /* 0x14A */ ANM_SUMOU_TACKLE_WAIT,
        /* 0x14B */ ANM_SUMOU_TACKLE_WAIT_GORON,  // ?
        /* 0x14C */ ANM_SUMOU_TACKLE_STAGGER,
        /* 0x14D */ ANM_SUMOU_TACKLE_STAGGER_GORON,  // ?
        /* 0x14E */ ANM_SUMOU_TACKLE_SHOCK_RETURN,
        /* 0x14F */ ANM_SUMOU_PUNCH,
        /* 0x150 */ ANM_SUMOU_PUNCH_MISS_SHOCK,
        /* 0x151 */ ANM_SUMOU_PUNCH_MISS_SHOCK_RETURN,
        /* 0x152 */ ANM_SUMOU_PUNCH_SHOCK,
        /* 0x153 */ ANM_SUMOU_STAGGER,
        /* 0x154 */ ANM_SUMOU_PUSH_BACK,
        /* 0x155 */ ANM_SUMOU_PULL_BACK,
        /* 0x156 */ ANM_SUMOU_PULL_BACK_GORON,  // ?
        /* 0x157 */ ANM_SUMOU_WIN,
        /* 0x158 */ ANM_SUMOU_WIN_GORON,
        /* 0x159 */ ANM_SUMOU_LOSE,
        /* 0x15A */ ANM_SUMOU_LOSE_GORON,
        /* 0x15B */ ANM_SUMOU_KNOCKED_DOWN,
        /* 0x15C */ ANM_SUMOU_GET_UP,
        /* 0x15D */ ANM_SUMOU_SHIKO,
        /* 0x15E */ ANM_SUMOU_SHIKO_WAIT,
        /* 0x15F */ ANM_DMG_FBW,  // ?
        /* 0x160 */ ANM_IRONBALL_WAIT,
        /* 0x161 */ ANM_IRONBALL_WALK,
        /* 0x162 */ ANM_IRONBALL_ATTACK,
        /* 0x163 */ ANM_IRONBALL_TURN,
        /* 0x164 */ ANM_IRONBALL_THROW,
        /* 0x165 */ ANM_IRONBALL_PULL,
        /* 0x166 */ ANM_IRONBALL_CATCH,
        /* 0x167 */ ANM_IRONBALL_DEF,
        /* 0x168 */ ANM_MORPHEEL_HANG,
        /* 0x169 */ ANM_MORPHEEL_HANG_MISS,
        /* 0x16A */ ANM_MORPHEEL_HANG_WAIT,
        /* 0x16B */ ANM_MORPHEEL_HANG_WAIT_B,
        /* 0x16C */ ANM_MORPHEEL_CUT,
        /* 0x16D */ ANM_MORPHEEL_CUT_B,  // final hit of cycles 1 and 2
        /* 0x16E */ ANM_MORPHEEL_CUT_FINAL,
        /* 0x16F */ ANM_MORPHEEL_SPIT_OUT,
        /* 0x170 */ ANM_DRAGON_HANG_LEFT,
        /* 0x171 */ ANM_DRAGON_HANG_RIGHT,
        /* 0x172 */ ANM_DRAGON_HANG,
        /* 0x173 */ ANM_DRAGON_HANG_WAIT,
        /* 0x174 */ ANM_DRAGON_HANG_WAIT_B,
        /* 0x175 */ ANM_DRAGON_CUT,
        /* 0x176 */ ANM_DRAGON_CUT_B,  // final hit of cycles 1 and 2
        /* 0x177 */ ANM_DRAGON_CUT_FINAL,
        /* 0x178 */ ANM_COPYROD_SWING,
        /* 0x179 */ ANM_COPYROD_SWING_LARGE,
        /* 0x17A */ ANM_GET_MASTER_SWORD,  // ?
        /* 0x17B */ ANM_WAIT_F,
        /* 0x17C */ ANM_ELEC_STUN_GND,
        /* 0x17D */ ANM_ELEC_STUN_AIR,
        /* 0x17E */ ANM_KEY_CATCH,
        /* 0x17F */ ANM_HORSE_GET_ITEM,
        /* 0x180 */ ANM_TWGATE_PULLED_IN,
        /* 0x181 */ ANM_CHAIN_PICK_UP,
        /* 0x182 */ ANM_CHAIN_PULL,
        /* 0x183 */ ANM_CHAIN_PULL_FYRUS,
        /* 0x184 */ ANM_CHAIN_PULL_END,
        /* 0x185 */ ANM_WAIT_INSECT,
        /* 0x186 */ ANM_DEMO_MHOP,  // ?
        /* 0x187 */ ANM_PULL_IRONBALL_SWITCH,
        /* 0x188 */ ANM_TRADE_ITEM_PULL_OUT,
        /* 0x189 */ ANM_TRADE_ITEM_WAIT,
        /* 0x18A */ ANM_DEMO_KAMAE,  // ?
        /* 0x18B */ ANM_CUT_JUMP_LARGE_CHARGE,
        /* 0x18C */ ANM_CUT_JUMP_LARGE_START,
        /* 0x18D */ ANM_CUT_JUMP_LARGE,
        /* 0x18E */ ANM_CUT_JUMP_LARGE_LAND,
        /* 0x18F */ ANM_DEMO_MASTER_SWORD_STICK,
        /* 0x190 */ ANM_DEMO_MASTER_SWORD_WAIT,
        /* 0x191 */ ANM_DEMO_MASTER_SWORD_PULL,
        /* 0x192 */ ANM_GANON_CHANCE,
        /* 0x193 */ ANM_GANON_CHANCE_LOSING,
        /* 0x194 */ ANM_GANON_CHANCE_WINNING,
        /* 0x195 */ ANM_GANON_CHANCE_WIN,
        /* 0x196 */ ANM_GANON_CHANCE_LOSE,
        /* 0x197 */ ANM_GANON_FINISH,
        /* 0x198 */ ANM_GANON_FINISH_WAIT,
        /* 0x199 */ ANM_GANON_ON_HORSE,
        /* 0x19A */ ANM_DIVE_START,
        /* 0x19B */ ANM_DIVE,
        /* 0x19C */ ANM_ODOROKU,   // ?
        /* 0x19D */ ANM_ASHIMOTO,  // ?
        /* 0x19E */ ANM_MAX,
    };

    enum daAlink_UPPER {
        UPPER_0,
        UPPER_1,
        UPPER_2,
    };

    enum daAlink_UNDER {
        UNDER_0,
        UNDER_1,
        UNDER_2,
    };

    enum daAlink_FTANM {
        FTANM_UNK_0 = 0,
        FTANM_UNK_1 = 1,
        FTANM_UNK_3 = 3,
        FTANM_UNK_4 = 4,
        FTANM_UNK_5 = 5,
        FTANM_UNK_8 = 8,
        FTANM_UNK_9 = 9,
        FTANM_UNK_13 = 0x13,
        FTANM_UNK_14 = 0x14,
        FTANM_UNK_20 = 0x20,
        FTANM_UNK_21 = 0x21,
        FTANM_UNK_22 = 0x22,
        FTANM_UNK_23 = 0x23,
        FTANM_UNK_27 = 0x27,
        FTANM_UNK_2D = 0x2D,
        FTANM_UNK_39 = 0x39,
        FTANM_UNK_48 = 0x48,
        FTANM_UNK_75 = 0x75,
        FTANM_UNK_76 = 0x76,
        FTANM_UNK_77 = 0x77,
        FTANM_UNK_7A = 0x7A,
        FTANM_UNK_7B = 0x7B,
        FTANM_UNK_7C = 0x7C,
        FTANM_UNK_8A = 0x8A,
        FTANM_UNK_8B = 0x8B,
        FTANM_UNK_8C = 0x8C,
        FTANM_UNK_8D = 0x8D,
        FTANM_UNK_8E = 0x8E,
        FTANM_UNK_8F = 0x8F,
        FTANM_UNK_90 = 0x90,
        FTANM_UNK_91 = 0x91,
        FTANM_UNK_92 = 0x92,
        FTANM_UNK_93 = 0x93,
        FTANM_UNK_95 = 0x95,
        FTANM_UNK_96 = 0x96,
        FTANM_UNK_97 = 0x97,
        FTANM_UNK_98 = 0x98,
        FTANM_UNK_99 = 0x99,
        FTANM_UNK_9A = 0x9A,
        FTANM_UNK_9B = 0x9B,
        FTANM_UNK_9C = 0x9C,
        FTANM_UNK_9D = 0x9D,
        FTANM_UNK_9E = 0x9E,
        FTANM_UNK_9F = 0x9F,
        FTANM_UNK_A0 = 0xA0,
        FTANM_UNK_A1 = 0xA1,
        FTANM_UNK_A2 = 0xA2,
    };

    enum daAlink_WARP_MAT_MODE {
        WARP_MAT_MODE_0,
        WARP_MAT_MODE_1,
    };

    enum daAlink_WANM {
        /* 0x00 */ WANM_WAIT,
        /* 0x01 */ WANM_WALK_A,
        /* 0x02 */ WANM_WALK_B,
        /* 0x03 */ WANM_DASH_A,
        /* 0x04 */ WANM_JUMP_ATTACK_START,
        /* 0x05 */ WANM_JUMP_ATTACK,
        /* 0x06 */ WANM_JUMP_ATTACK_END,
        /* 0x07 */ WANM_WAIT_PP,  // ?
        /* 0x08 */ WANM_PUSH_LIGHT,
        /* 0x09 */ WANM_PUSH_START,
        /* 0x0A */ WANM_PUSH,
        /* 0x0B */ WANM_SIT,
        /* 0x0C */ WANM_CROUCH,
        /* 0x0D */ WANM_CROUCH_WALK,
        /* 0x0E */ WANM_WAIT_SIT,
        /* 0x0F */ WANM_TRES_OPEN,
        /* 0x10 */ WANM_VJMP_START,
        /* 0x11 */ WANM_BUMP_START,
        /* 0x12 */ WANM_BUMP_END,
        /* 0x13 */ WANM_VJMP_CLIMB,
        /* 0x14 */ WANM_VJMP_CLIMB_HANG,
        /* 0x15 */ WANM_TURN_BACK,
        /* 0x16 */ WANM_DMG_SMALL,
        /* 0x17 */ WANM_WAIT_TIRED,
        /* 0x18 */ WANM_WAIT_TO_TIRED,
        /* 0x19 */ WANM_HANG,
        /* 0x1A */ WANM_SLIP,
        /* 0x1B */ WANM_TURN,
        /* 0x1C */ WANM_ATN,
        /* 0x1D */ WANM_WALK_SLOPE,
        /* 0x1E */ WANM_SLIDE_BACKWARD,
        /* 0x1F */ WANM_SLIDE_BACKWARD_END,
        /* 0x20 */ WANM_SLIDE_FORWARD_START,
        /* 0x21 */ WANM_ROPE_WALK,
        /* 0x22 */ WANM_ROPE_CLIMB_LEFT,
        /* 0x23 */ WANM_ROPE_CLIMB_RIGHT,
        /* 0x24 */ WANM_ROPE_TURN,
        /* 0x25 */ WANM_ROPE_WAIT,
        /* 0x26 */ WANM_ROPE_FALTER,
        /* 0x27 */ WANM_DASH_B,
        /* 0x28 */ WANM_DMG_AIR_FRONT,
        /* 0x29 */ WANM_DMG_AIR_BACK,
        /* 0x2A */ WANM_DMG_AIR_LEFT,
        /* 0x2B */ WANM_DMG_AIR_RIGHT,
        /* 0x2C */ WANM_DMG_AIR_FRONT_GETUP,
        /* 0x2D */ WANM_DMG_AIR_BACK_GETUP,
        /* 0x2E */ WANM_DMG_AIR_LEFT_GETUP,
        /* 0x2F */ WANM_DMG_AIR_RIGHT_GETUP,
        /* 0x30 */ WANM_SERVICE_WAIT_A,
        /* 0x31 */ WANM_WAIT_B,
        /* 0x32 */ WANM_JUMP_B_START,
        /* 0x33 */ WANM_JUMP_B_TURN_START,
        /* 0x34 */ WANM_JUMP_B,
        /* 0x35 */ WANM_JUMP_B_END,
        /* 0x36 */ WANM_SWIM_RESURFACE,
        /* 0x37 */ WANM_SWIM_WAIT,
        /* 0x38 */ WANM_SWIM,
        /* 0x39 */ WANM_SWIM_DASH,
        /* 0x3A */ WANM_WAIT_SHAKE,
        /* 0x3B */ WANM_WAIT_START,
        /* 0x3C */ WANM_DMG_FRONT,
        /* 0x3D */ WANM_DMG_BACK,
        /* 0x3E */ WANM_DMG_LEFT,
        /* 0x3F */ WANM_DMG_RIGHT,
        /* 0x40 */ WANM_ATTACK_B_LEFT,
        /* 0x41 */ WANM_ATTACK_B_RIGHT,
        /* 0x42 */ WANM_ATTACK_B_FRONT,
        /* 0x43 */ WANM_ATTACK_B_TAIL,
        /* 0x44 */ WANM_ATN_LOCK_WALK_BACK,
        /* 0x45 */ WANM_ATN_LOCK_WALK_FRONT,
        /* 0x46 */ WANM_ATN_LOCK_RUN,
        /* 0x47 */ WANM_PICKUP_A,
        /* 0x48 */ WANM_SIDE_JUMP_LEFT_START,
        /* 0x49 */ WANM_SIDE_JUMP_LEFT_END,
        /* 0x4A */ WANM_SIDE_JUMP_RIGHT_START,
        /* 0x4B */ WANM_SIDE_JUMP_RIGHT_END,
        /* 0x4C */ WANM_BACK_JUMP_START,
        /* 0x4D */ WANM_BACK_JUMP_END,
        /* 0x4E */ WANM_PICKUP_B,
        /* 0x4F */ WANM_PICKUP_THROW,
        /* 0x50 */ WANM_ATTACK_A_START,
        /* 0x51 */ WANM_ATTACK_A,
        /* 0x52 */ WANM_ATTACK_A_END,
        /* 0x53 */ WANM_ATTACK_A_END_FRONT,
        /* 0x54 */ WANM_ATTACK_A_END_BACK,
        /* 0x55 */ WANM_ATTACK_T_START,
        /* 0x56 */ WANM_ATTACK_T,
        /* 0x57 */ WANM_HANDLE_HANG,
        /* 0x58 */ WANM_HANDLE_HANG_WAIT,
        /* 0x59 */ WANM_HANDLE_HANG_SW,  // ?
        /* 0x5A */ WANM_CUT_TURN_LEFT,
        /* 0x5B */ WANM_CUT_TURN_RIGHT,
        /* 0x5C */ WANM_HOWL,
        /* 0x5D */ WANM_HOWL_SIT,
        /* 0x5E */ WANM_HOWL_SUCCESS,
        /* 0x5F */ WANM_HOWL_END,
        /* 0x60 */ WANM_FALL_LAND,
        /* 0x61 */ WANM_FALL_LAND_START,
        /* 0x62 */ WANM_ATTACK_POE_START,
        /* 0x63 */ WANM_ATTACK_POE,
        /* 0x64 */ WANM_ATTACK_POE_END,
        /* 0x65 */ WANM_ATTACK_POE_MISS,
        /* 0x66 */ WANM_BACKFLIP_START,
        /* 0x67 */ WANM_BACKFLIP_END,
        /* 0x68 */ WANM_DIG,
        /* 0x69 */ WANM_DIG_IN,
        /* 0x6A */ WANM_DIG_UNDER,
        /* 0x6B */ WANM_SLIDE_W,  // ?
        /* 0x6C */ WANM_JUMP_KICK,
        /* 0x6D */ WANM_PULL_OUT,
        /* 0x6E */ WANM_CUT_TURN_CHARGE,
        /* 0x6F */ WANM_TRANSFORM_TO_WOLF,
        /* 0x70 */ WANM_TRANSFORM_TO_HUMAN,
        /* 0x71 */ WANM_WAIT_WIND,
        /* 0x72 */ WANM_SMELL,
        /* 0x73 */ WANM_DASH_START,
        /* 0x74 */ WANM_ATTACK_RECOIL_START,
        /* 0x75 */ WANM_ATTACK_RECOIL_END,
        /* 0x76 */ WANM_FLING_LEFT,
        /* 0x77 */ WANM_FLING_RIGHT,
        /* 0x78 */ WANM_CATCH_START,
        /* 0x79 */ WANM_CATCH,
        /* 0x7A */ WANM_ATTACK_RECOIL_GROUND,
        /* 0x7B */ WANM_DIE,
        /* 0x7C */ WANM_SWIM_DIE,
        /* 0x7D */ WANM_SWIM_DROWN,
        /* 0x7E */ WANM_WAIT_STUNNED,  // ?
        /* 0x7F */ WANM_WAIT_A_TO_S,   // ?
        /* 0x80 */ WANM_SERVICE_WAIT_D,
        /* 0x81 */ WANM_MIDNA_SHOCK,
        /* 0x82 */ WANM_ENTRANCE,
        /* 0x83 */ WANM_GET_A,
        /* 0x84 */ WANM_GET,
        /* 0x85 */ WANM_WAIT_KARGOROK,
        /* 0x86 */ WANM_CATCH_KARGOROK,
        /* 0x87 */ WANM_DMG_ELEC_GROUND,
        /* 0x88 */ WANM_DMG_ELEC_AIR,
        /* 0x89 */ WANM_TWGATE_PULL,
        /* 0x8A */ WANM_CHAIN_PULL,
        /* 0x8B */ WANM_CHAIN_PULL_END,
        /* 0x8C */ WANM_LISTEN,
        /* 0x8D */ WANM_PIG_GANON_CATCH_ST,
        /* 0x8E */ WANM_PIG_GANON_CATCH_END,
        /* 0x8F */ WANM_PIG_GANON_THROW_LEFT_START,
        /* 0x90 */ WANM_PIG_GANON_THROW_LEFT_END,
        /* 0x91 */ WANM_PIG_GANON_THROW_RIGHT_START,
        /* 0x92 */ WANM_PIG_GANON_THROW_RIGHT_END,
    };

    enum MODE_FLG {
        /* 0x00000001 */ MODE_IDLE = (1 << 0),
        /* 0x00000002 */ MODE_JUMP = (1 << 1),
        /* 0x00000004 */ MODE_DISABLE_ITEMS = (1 << 2),  // ?
        /* 0x00000008 */ MODE_HIT_STUN = (1 << 3),
        /* 0x00000010 */ MODE_CLIMB = (1 << 4),     // ?
        /* 0x00000020 */ MODE_UNK_20 = 0x20,        // related to wolf?
        /* 0x00000040 */ MODE_NO_COLLISION = 0x40,  // disables wall col checks
        /* 0x00000080 */ MODE_GUARD_ENABLED = 0x80,
        /* 0x00000100 */ MODE_UNK_100 = 0x100,
        /* 0x00000400 */ MODE_RIDING = 0x400,
        /* 0x00000800 */ MODE_UNK_800 = 0x800,
        /* 0x00001000 */ MODE_UNK_1000 =
            0x1000,  // affects things like anim set when controlling rod statue
        /* 0x00002000 */ MODE_UNK_2000 = 0x2000,
        /* 0x00004000 */ MODE_UNK_4000 = 0x4000,
        /* 0x00008000 */ MODE_UNK_8000 = 0x8000,
        /* 0x00010000 */ MODE_VINE_CLIMB = 0x10000,  // used for vine climbing
        /* 0x00020000 */ MODE_ROPE_WALK = 0x20000,
        /* 0x00040000 */ MODE_SWIMMING = 0x40000,  // this might be more than swimming
        /* 0x00100000 */ MODE_GRAB_PLACE = 0x100000,
        /* 0x00800000 */ MODE_UNK_800000 = 0x800000,    // moves camera down
        /* 0x01000000 */ MODE_UNK_1000000 = 0x1000000,  // attention related?
        /* 0x02000000 */ MODE_UNK_2000000 = 0x2000000,  // maybe related to snow/sand?
        /* 0x04000000 */ MODE_UNK_4000000 = 0x4000000,
        /* 0x08000000 */ MODE_UNK_8000000 = 0x8000000,
        /* 0x10000000 */ MODE_UNK_10000000 = 0x10000000,  // catch boomerang?
        /* 0x20000000 */ MODE_UNK_20000000 = 0x20000000,
        /* 0x40000000 */ MODE_UNK_40000000 = 0x40000000,
        MODE_PLAYER_FLY = 0x70C52,
    };

    enum daAlink_PROC {
        /* 0x000 */ PROC_PREACTION_UNEQUIP,
        /* 0x001 */ PROC_SERVICE_WAIT,
        /* 0x002 */ PROC_TIRED_WAIT,
        /* 0x003 */ PROC_WAIT,
        /* 0x004 */ PROC_MOVE,
        /* 0x005 */ PROC_ATN_MOVE,
        /* 0x006 */ PROC_ATN_ACTOR_WAIT,
        /* 0x007 */ PROC_ATN_ACTOR_MOVE,
        /* 0x008 */ PROC_WAIT_TURN,
        /* 0x009 */ PROC_MOVE_TURN,
        /* 0x00A */ PROC_SIDESTEP,
        /* 0x00B */ PROC_SIDESTEP_LAND,
        /* 0x00C */ PROC_SLIDE,
        /* 0x00D */ PROC_SLIDE_LAND,
        /* 0x00E */ PROC_FRONT_ROLL,
        /* 0x00F */ PROC_FRONT_ROLL_CRASH,
        /* 0x010 */ PROC_FRONT_ROLL_SUCCESS,
        /* 0x011 */ PROC_SIDE_ROLL,
        /* 0x012 */ PROC_BACK_JUMP,
        /* 0x013 */ PROC_BACK_JUMP_LAND,
        /* 0x014 */ PROC_SLIP,
        /* 0x015 */ PROC_AUTO_JUMP,
        /* 0x016 */ PROC_DIVE_JUMP,
        /* 0x017 */ PROC_ROLL_JUMP,
        /* 0x018 */ PROC_FALL,
        /* 0x019 */ PROC_LAND,
        /* 0x01A */ PROC_SMALL_JUMP,
        /* 0x01B */ PROC_STEP_MOVE,
        /* 0x01C */ PROC_CROUCH,
        /* 0x01D */ PROC_GUARD_SLIP,
        /* 0x01E */ PROC_GUARD_ATTACK,
        /* 0x01F */ PROC_GUARD_BREAK,
        /* 0x020 */ PROC_TURN_MOVE,
        /* 0x021 */ PROC_CUT_NORMAL,
        /* 0x022 */ PROC_CUT_FINISH,
        /* 0x023 */ PROC_CUT_FINISH_JUMP_UP,
        /* 0x024 */ PROC_CUT_FINISH_JUMP_UP_LAND,
        /* 0x025 */ PROC_CUT_REVERSE,
        /* 0x026 */ PROC_CUT_JUMP,
        /* 0x027 */ PROC_CUT_JUMP_LAND,
        /* 0x028 */ PROC_CUT_TURN,
        /* 0x029 */ PROC_CUT_TURN_CHARGE,
        /* 0x02A */ PROC_CUT_TURN_MOVE,
        /* 0x02B */ PROC_CUT_DOWN,
        /* 0x02C */ PROC_CUT_DOWN_LAND,
        /* 0x02D */ PROC_CUT_HEAD,
        /* 0x02E */ PROC_CUT_HEAD_LAND,
        /* 0x02F */ PROC_CUT_LARGE_JUMP_CHARGE,
        /* 0x030 */ PROC_CUT_LARGE_JUMP,
        /* 0x031 */ PROC_CUT_LARGE_JUMP_LAND,
        /* 0x032 */ PROC_DAMAGE,
        /* 0x033 */ PROC_LARGE_DAMAGE_UP,
        /* 0x034 */ PROC_LAND_DAMAGE,
        /* 0x035 */ PROC_CRAWL_START,
        /* 0x036 */ PROC_CRAWL_MOVE,
        /* 0x037 */ PROC_CRAWL_AUTO_MOVE,
        /* 0x038 */ PROC_CRAWL_END,
        /* 0x039 */ PROC_PULL_MOVE,
        /* 0x03A */ PROC_HORSE_RIDE,
        /* 0x03B */ PROC_HORSE_GETOFF,
        /* 0x03C */ PROC_HORSE_WAIT,
        /* 0x03D */ PROC_HORSE_TURN,
        /* 0x03E */ PROC_HORSE_JUMP,
        /* 0x03F */ PROC_HORSE_LAND,
        /* 0x040 */ PROC_HORSE_SUBJECTIVITY,
        /* 0x041 */ PROC_HORSE_CUT,
        /* 0x042 */ PROC_HORSE_CUT_CHARGE_READY,
        /* 0x043 */ PROC_HORSE_CUT_TURN,
        /* 0x044 */ PROC_HORSE_DAMAGE,
        /* 0x045 */ PROC_HORSE_BOW_SUBJECT,
        /* 0x046 */ PROC_HORSE_BOW_MOVE,
        /* 0x047 */ PROC_HORSE_GRAB_MOVE,
        /* 0x048 */ PROC_HORSE_BOOMERANG_SUBJECT,
        /* 0x049 */ PROC_HORSE_BOOMERANG_MOVE,
        /* 0x04A */ PROC_HORSE_HOOKSHOT_SUBJECT,
        /* 0x04B */ PROC_HORSE_HOOKSHOT_MOVE,
        /* 0x04C */ PROC_HORSE_BOTTLE_DRINK,
        /* 0x04D */ PROC_HORSE_COMEBACK,
        /* 0x04E */ PROC_HORSE_KANDELAAR_POUR,
        /* 0x04F */ PROC_HORSE_RUN,
        /* 0x050 */ PROC_HORSE_HANG,
        /* 0x051 */ PROC_HORSE_GET_KEY,
        /* 0x052 */ PROC_HORSE_LOOK_DOWN,
        /* 0x053 */ PROC_BOAR_RUN,
        /* 0x054 */ PROC_SWORD_UNEQUIP_SP,
        /* 0x055 */ PROC_HANG_START,
        /* 0x056 */ PROC_HANG_FALL_START,
        /* 0x057 */ PROC_HANG_UP,
        /* 0x058 */ PROC_HANG_WAIT,
        /* 0x059 */ PROC_HANG_MOVE,
        /* 0x05A */ PROC_HANG_CLIMB,
        /* 0x05B */ PROC_HANG_WALL_CATCH,
        /* 0x05C */ PROC_HANG_READY,
        /* 0x05D */ PROC_HANG_LEVER_DOWN,
        /* 0x05E */ PROC_BOW_SUBJECT,
        /* 0x05F */ PROC_BOW_MOVE,
        /* 0x060 */ PROC_BOOMERANG_SUBJECT,
        /* 0x061 */ PROC_BOOMERANG_MOVE,
        /* 0x062 */ PROC_BOOMERANG_CATCH,
        /* 0x063 */ PROC_COPY_ROD_SUBJECT,
        /* 0x064 */ PROC_COPY_ROD_MOVE,
        /* 0x065 */ PROC_COPY_ROD_SWING,
        /* 0x066 */ PROC_COPY_ROD_REVIVE,
        /* 0x067 */ PROC_LADDER_UP_START,
        /* 0x068 */ PROC_LADDER_UP_END,
        /* 0x069 */ PROC_LADDER_DOWN_START,
        /* 0x06A */ PROC_LADDER_DOWN_END,
        /* 0x06B */ PROC_LADDER_MOVE,
        /* 0x06C */ PROC_GRAB_READY,
        /* 0x06D */ PROC_GRAB_UP,
        /* 0x06E */ PROC_GRAB_MISS,
        /* 0x06F */ PROC_GRAB_THROW,
        /* 0x070 */ PROC_GRAB_PUT,
        /* 0x071 */ PROC_GRAB_WAIT,
        /* 0x072 */ PROC_GRAB_REBOUND,
        /* 0x073 */ PROC_GRAB_STAND,
        /* 0x074 */ PROC_INSECT_CATCH,
        /* 0x075 */ PROC_PICK_UP,
        /* 0x076 */ PROC_PICK_PUT,
        /* 0x077 */ PROC_HUMAN_ST_ESCAPE,
        /* 0x078 */ PROC_DK_CAUGHT,
        /* 0x079 */ PROC_SWIM_UP,
        /* 0x07A */ PROC_SWIM_WAIT,
        /* 0x07B */ PROC_SWIM_MOVE,
        /* 0x07C */ PROC_SWIM_DIVE,
        /* 0x07D */ PROC_SWIM_HOOKSHOT_SUBJECT,
        /* 0x07E */ PROC_SWIM_HOOKSHOT_MOVE,
        /* 0x07F */ PROC_SWIM_DAMAGE,
        /* 0x080 */ PROC_CLIMB_UP_START,
        /* 0x081 */ PROC_CLIMB_DOWN_START,
        /* 0x082 */ PROC_CLIMB_MOVE_UPDOWN,
        /* 0x083 */ PROC_CLIMB_MOVE_SIDE,
        /* 0x084 */ PROC_CLIMB_WAIT,
        /* 0x085 */ PROC_CLIMB_TO_ROOF,
        /* 0x086 */ PROC_ROOF_HANG_START,
        /* 0x087 */ PROC_ROOF_HANG_WAIT,
        /* 0x088 */ PROC_ROOF_HANG_FRONT_MOVE,
        /* 0x089 */ PROC_ROOF_HANG_SIDE_MOVE,
        /* 0x08A */ PROC_ROOF_HANG_TURN,
        /* 0x08B */ PROC_ROOF_SWITCH_HANG,
        /* 0x08C */ PROC_CANOE_RIDE,
        /* 0x08D */ PROC_CANOE_JUMP_RIDE,
        /* 0x08E */ PROC_CANOE_GETOFF,
        /* 0x08F */ PROC_CANOE_WAIT,
        /* 0x090 */ PROC_CANOE_ROW,
        /* 0x091 */ PROC_CANOE_PADDLE_SHIFT,
        /* 0x092 */ PROC_CANOE_PADDLE_PUT,
        /* 0x093 */ PROC_CANOE_PADDLE_GRAB,
        /* 0x094 */ PROC_CANOE_ROD_GRAB,
        /* 0x095 */ PROC_CANOE_FISHING_WAIT,
        /* 0x096 */ PROC_CANOE_FISHING_REEL,
        /* 0x097 */ PROC_CANOE_FISHING_GET,
        /* 0x098 */ PROC_CANOE_SUBJECTIVITY,
        /* 0x099 */ PROC_CANOE_BOW_SUBJECT,
        /* 0x09A */ PROC_CANOE_BOW_MOVE,
        /* 0x09B */ PROC_CANOE_GRAB_MOVE,
        /* 0x09C */ PROC_CANOE_BOOMERANG_SUBJECT,
        /* 0x09D */ PROC_CANOE_BOOMERANG_MOVE,
        /* 0x09E */ PROC_CANOE_HOOKSHOT_SUBJECT,
        /* 0x09F */ PROC_CANOE_HOOKSHOT_MOVE,
        /* 0x0A0 */ PROC_CANOE_BOTTLE_DRINK,
        /* 0x0A1 */ PROC_CANOE_KANDELAAR_POUR,
        /* 0x0A2 */ PROC_FISHING_CAST,
        /* 0x0A3 */ PROC_FISHING_FOOD,
        /* 0x0A4 */ PROC_SPINNER_READY,
        /* 0x0A5 */ PROC_SPINNER_WAIT,
        /* 0x0A6 */ PROC_BOARD_RIDE,
        /* 0x0A7 */ PROC_BOARD_WAIT,
        /* 0x0A8 */ PROC_BOARD_ROW,
        /* 0x0A9 */ PROC_BOARD_TURN,
        /* 0x0AA */ PROC_BOARD_JUMP,
        /* 0x0AB */ PROC_BOARD_SUBJECTIVITY,
        /* 0x0AC */ PROC_BOARD_CUT,
        /* 0x0AD */ PROC_BOARD_CUT_TURN,
        /* 0x0AE */ PROC_CHAIN_UP,
        /* 0x0AF */ PROC_CHAIN_STRONG_PULL,
        /* 0x0B0 */ PROC_DOOR_OPEN,
        /* 0x0B1 */ PROC_MONKEY_MOVE,
        /* 0x0B2 */ PROC_DEMO_BOOMERANG_CATCH,
        /* 0x0B3 */ PROC_BOTTLE_DRINK,
        /* 0x0B4 */ PROC_BOTTLE_OPEN,
        /* 0x0B5 */ PROC_BOTTLE_SWING,
        /* 0x0B6 */ PROC_BOTTLE_GET,
        /* 0x0B7 */ PROC_KANDELAAR_SWING,
        /* 0x0B8 */ PROC_KANDELAAR_POUR,
        /* 0x0B9 */ PROC_GRASS_WHISTLE_GET,
        /* 0x0BA */ PROC_GRASS_WHISTLE_WAIT,
        /* 0x0BB */ PROC_HAWK_CATCH,
        /* 0x0BC */ PROC_HAWK_SUBJECT,
        /* 0x0BD */ PROC_FLOOR_DOWN_REBOUND,
        /* 0x0BE */ PROC_GORON_RIDE_WAIT,
        /* 0x0BF */ PROC_GOAT_MOVE,
        /* 0x0C0 */ PROC_GOAT_CATCH,
        /* 0x0C1 */ PROC_GOAT_STROKE,
        /* 0x0C2 */ PROC_GORON_MOVE,
        /* 0x0C3 */ PROC_BOSS_ATN_WAIT,
        /* 0x0C4 */ PROC_HOOKSHOT_SUBJECT,
        /* 0x0C5 */ PROC_HOOKSHOT_MOVE,
        /* 0x0C6 */ PROC_HOOKSHOT_FLY,
        /* 0x0C7 */ PROC_HOOKSHOT_ROOF_WAIT,
        /* 0x0C8 */ PROC_HOOKSHOT_ROOF_SHOOT,
        /* 0x0C9 */ PROC_HOOKSHOT_ROOF_BOOTS,
        /* 0x0CA */ PROC_HOOKSHOT_WALL_WAIT,
        /* 0x0CB */ PROC_HOOKSHOT_WALL_SHOOT,
        /* 0x0CC */ PROC_MAGNE_BOOTS_FLY,
        /* 0x0CD */ PROC_BOOTS_EQUIP,
        /* 0x0CE */ PROC_SUMOU_READY,
        /* 0x0CF */ PROC_SUMOU_MOVE,
        /* 0x0D0 */ PROC_SUMOU_SIDE_MOVE,
        /* 0x0D1 */ PROC_SUMOU_ACTION,
        /* 0x0D2 */ PROC_SUMOU_STAGGER,
        /* 0x0D3 */ PROC_SUMOU_WIN_LOSE,
        /* 0x0D4 */ PROC_SUMOU_SHIKO,
        /* 0x0D5 */ PROC_LOOK_UP,
        /* 0x0D6 */ PROC_LOOK_UP_TO_GET_ITEM,
        /* 0x0D7 */ PROC_HAND_PAT,
        /* 0x0D8 */ PROC_IRON_BALL_SUBJECT,
        /* 0x0D9 */ PROC_IRON_BALL_MOVE,
        /* 0x0DA */ PROC_IRON_BALL_THROW,
        /* 0x0DB */ PROC_IRON_BALL_RETURN,
        /* 0x0DC */ PROC_BOSS_BODY_HANG,
        /* 0x0DD */ PROC_OCTAIEAL_SPIT,
        /* 0x0DE */ PROC_SCREAM_WAIT,
        /* 0x0DF */ PROC_GOAT_STOP_READY,
        /* 0x0E0 */ PROC_ZORA_MOVE,
        /* 0x0E1 */ PROC_LOOK_AROUND_TURN,
        /* 0x0E2 */ PROC_TRADE_ITEM_OUT,
        /* 0x0E3 */ PROC_NOT_USE_ITEM,
        /* 0x0E4 */ PROC_SWORD_READY,
        /* 0x0E5 */ PROC_SWORD_PUSH,
        /* 0x0E6 */ PROC_GANON_FINISH,
        /* 0x0E7 */ PROC_CUT_FAST_READY,
        /* 0x0E8 */ PROC_MASTER_SWORD_STICK,
        /* 0x0E9 */ PROC_MASTER_SWORD_PULL,
        /* 0x0EA */ PROC_DUNGEON_WARP_READY,
        /* 0x0EB */ PROC_DUNGEON_WARP,
        /* 0x0EC */ PROC_DUNGEON_WARP_SCN_START,
        /* 0x0ED */ PROC_WOLF_HOWL_DEMO,
        /* 0x0EE */ PROC_WOLF_SERVICE_WAIT,
        /* 0x0EF */ PROC_WOLF_TIRED_WAIT,
        /* 0x0F0 */ PROC_WOLF_MIDNA_RIDE_SHOCK,
        /* 0x0F1 */ PROC_WOLF_WAIT,
        /* 0x0F2 */ PROC_WOLF_MOVE,
        /* 0x0F3 */ PROC_WOLF_DASH,
        /* 0x0F4 */ PROC_WOLF_DASH_REVERSE,
        /* 0x0F5 */ PROC_WOLF_WAIT_TURN,
        /* 0x0F6 */ PROC_WOLF_ATN_AC_MOVE,
        /* 0x0F7 */ PROC_WOLF_SIDESTEP,
        /* 0x0F8 */ PROC_WOLF_SIDESTEP_LAND,
        /* 0x0F9 */ PROC_WOLF_BACKJUMP,
        /* 0x0FA */ PROC_WOLF_BACKJUMP_LAND,
        /* 0x0FB */ PROC_WOLF_HOWL,
        /* 0x0FC */ PROC_WOLF_AUTO_JUMP,
        /* 0x0FD */ PROC_WOLF_FALL,
        /* 0x0FE */ PROC_WOLF_LAND,
        /* 0x0FF */ PROC_WOLF_SIT,
        /* 0x100 */ PROC_WOLF_LIE_START,
        /* 0x101 */ PROC_WOLF_LIE_MOVE,
        /* 0x102 */ PROC_WOLF_LIE_AUTO_MOVE,
        /* 0x103 */ PROC_WOLF_HANG_READY,
        /* 0x104 */ PROC_WOLF_STEP_MOVE,
        /* 0x105 */ PROC_WOLF_HANG_WALL_CATCH,
        /* 0x106 */ PROC_WOLF_HANG_FALL_START,
        /* 0x107 */ PROC_WOLF_DAMAGE,
        /* 0x108 */ PROC_WOLF_LARGE_DAMAGE_UP,
        /* 0x109 */ PROC_WOLF_LAND_DAMAGE,
        /* 0x10A */ PROC_WOLF_SCREAM_WAIT,
        /* 0x10B */ PROC_WOLF_SLIP,
        /* 0x10C */ PROC_WOLF_SLIP_TURN,
        /* 0x10D */ PROC_WOLF_SLIP_TURN_LAND,
        /* 0x10E */ PROC_WOLF_SLIDE_READY,
        /* 0x10F */ PROC_WOLF_SLIDE,
        /* 0x110 */ PROC_WOLF_SLIDE_LAND,
        /* 0x111 */ PROC_WOLF_WAIT_SLIP,
        /* 0x112 */ PROC_WOLF_SLOPE_START,
        /* 0x113 */ PROC_WOLF_ROPE_MOVE,
        /* 0x114 */ PROC_WOLF_ROPE_HANG,
        /* 0x115 */ PROC_WOLF_ROPE_TURN,
        /* 0x116 */ PROC_WOLF_ROPE_STAGGER,
        /* 0x117 */ PROC_WOLF_ROPE_SUBJECTIVITY,
        /* 0x118 */ PROC_WOLF_TAG_JUMP,
        /* 0x119 */ PROC_WOLF_TAG_JUMP_LAND,
        /* 0x11A */ PROC_WOLF_ROLL_ATTACK_CHARGE,
        /* 0x11B */ PROC_WOLF_ROLL_ATTACK_MOVE,
        /* 0x11C */ PROC_WOLF_JUMP_ATTACK,
        /* 0x11D */ PROC_WOLF_JUMP_AT_KICK,
        /* 0x11E */ PROC_WOLF_JUMP_AT_SLIDE_LAND,
        /* 0x11F */ PROC_WOLF_JUMP_AT_NORMAL_LAND,
        /* 0x120 */ PROC_WOLF_WAIT_ATTACK,
        /* 0x121 */ PROC_WOLF_ROLL_ATTACK,
        /* 0x122 */ PROC_WOLF_DOWN_ATTACK,
        /* 0x123 */ PROC_WOLF_DOWN_AT_LAND,
        /* 0x124 */ PROC_WOLF_DOWN_AT_MISS_LAND,
        /* 0x125 */ PROC_WOLF_LOCK_ATTACK,
        /* 0x126 */ PROC_WOLF_LOCK_ATTACK_TURN,
        /* 0x127 */ PROC_WOLF_SWIM_UP,
        /* 0x128 */ PROC_WOLF_SWIM_WAIT,
        /* 0x129 */ PROC_WOLF_SWIM_MOVE,
        /* 0x12A */ PROC_WOLF_SWIM_END_WAIT,
        /* 0x12B */ PROC_WOLF_GRAB_UP,
        /* 0x12C */ PROC_WOLF_GRAB_PUT,
        /* 0x12D */ PROC_WOLF_GRAB_THROW,
        /* 0x12E */ PROC_WOLF_CHAIN_UP,
        /* 0x12F */ PROC_WOLF_PUSH,
        /* 0x130 */ PROC_WOLF_CHAIN_READY,
        /* 0x131 */ PROC_WOLF_CHAIN_WAIT,
        /* 0x132 */ PROC_WOLF_DIG,
        /* 0x133 */ PROC_WOLF_DIG_THROUGH,
        /* 0x134 */ PROC_WOLF_ATTACK_REVERSE,
        /* 0x135 */ PROC_WOLF_ENEMY_THROW,
        /* 0x136 */ PROC_WOLF_ENEMY_HANG_BITE,
        /* 0x137 */ PROC_WOLF_GIANT_PUZZLE,
        /* 0x138 */ PROC_WOLF_CARGO_CARRY,
        /* 0x139 */ PROC_WOLF_GET_SMELL,
        /* 0x13A */ PROC_WOLF_SMELL_WAIT,
        /* 0x13B */ PROC_WOLF_SNOW_ESCAPE,
        /* 0x13C */ PROC_WOLF_GANON_CATCH,
        /* 0x13D */ PROC_TOOL_DEMO,
        /* 0x13E */ PROC_SUBJECTIVITY,
        /* 0x13F */ PROC_SWIM_SUBJECTIVITY,
        /* 0x140 */ PROC_PEEP_SUBJECTIVITY,
        /* 0x141 */ PROC_POLY_DAMAGE,
        /* 0x142 */ PROC_ELEC_DAMAGE,
        /* 0x143 */ PROC_PUSH_PULL_WAIT,
        /* 0x144 */ PROC_PUSH_MOVE,
        /* 0x145 */ PROC_TALK,
        /* 0x146 */ PROC_OPEN_TREASURE,
        /* 0x147 */ PROC_UNEQUIP,
        /* 0x148 */ PROC_GET_ITEM,
        /* 0x149 */ PROC_TURN_BACK,
        /* 0x14A */ PROC_LOOK_WAIT,
        /* 0x14B */ PROC_DEMO_PUSH_PULL_WAIT,
        /* 0x14C */ PROC_DEMO_PUSH_MOVE,
        /* 0x14D */ PROC_METAMORPHOSE,
        /* 0x14E */ PROC_METAMORPHOSE_ONLY,
        /* 0x14F */ PROC_WARP,
        /* 0x150 */ PROC_DEAD,
        /* 0x151 */ PROC_FOG_DEAD,
        /* 0x152 */ PROC_LOOK_AROUND,
        /* 0x153 */ PROC_CAUGHT,
        /* 0x154 */ PROC_SAND_WALL_HIT,
        /* 0x155 */ PROC_LAVA_RETURN,
        /* 0x156 */ PROC_SWIM_FREEZE_RETURN,
        /* 0x157 */ PROC_GET_READY_SIT,
        /* 0x158 */ PROC_TW_GATE,
        /* 0x159 */ PROC_LARGE_DAMAGE,
        /* 0x15A */ PROC_LARGE_DAMAGE_WALL,
        /* 0x15B */ PROC_NOD,
        /* 0x15C */ PROC_EYE_AWAY,
        /* 0x15D */ PROC_GLARE,
        /* 0x15E */ PROC_HORSE_CALL_WAIT,
        /* 0x15F */ PROC_QUAKE_WAIT,
    };

    enum daAlink_ITEM_BTN {
        /* 0x01 */ BTN_X = (1 << 0),
        /* 0x02 */ BTN_Y = (1 << 1),
        /* 0x04 */ BTN_Z = (1 << 2),
        /* 0x08 */ BTN_B = (1 << 3),
        /* 0x10 */ BTN_A = (1 << 4),
        /* 0x20 */ BTN_L = (1 << 5),
        /* 0x40 */ BTN_R = (1 << 6),
    };

    /**
     * Moving direction calculated from an angle
     */
    enum daAlink_DIR {
        /* 0x0 */ DIR_FORWARD,
        /* 0x1 */ DIR_BACKWARD,
        /* 0x2 */ DIR_LEFT,
        /* 0x3 */ DIR_RIGHT,
        /* 0x4 */ DIR_NONE,
    };

    enum daAlink_EFFPROC {
        EFFPROC_FRONT_ROLL,
        EFFPROC_SLIP,
        EFFPROC_SMALL_LAND,
        EFFPROC_RUN,
        EFFPROC_LAND,
        EFFPROC_SUMOU,
        EFFPROC_NONE,
    };

    class firePointEff_c {
    public:
        /* 0x00 */ u8 field_0x0;
        /* 0x02 */ u16 field_0x2;
        /* 0x04 */ u32 field_0x4;
        /* 0x08 */ u32 field_0x8;
        /* 0x0C */ cXyz field_0xc;
        /* 0x18 */ cXyz field_0x18;
        /* 0x24 */ cXyz field_0x24;
    };

    class hsChainShape_c : public J3DPacket {
    public:
        hsChainShape_c() {}
        virtual void draw();
        virtual ~hsChainShape_c() {}
    };  // Size: 0x10

    static BOOL getE3Zhint();
    static const char* getAlinkArcName();
    static bool checkStageName(char const*);
    void tgHitCallback(fopAc_ac_c*, dCcD_GObjInf*, dCcD_GObjInf*);
    void coHitCallback(fopAc_ac_c*, dCcD_GObjInf*);
    void setMatrixWorldAxisRot(f32 (*)[4], s16, s16, s16, int, cXyz const*);
    int jointControll(int);
    void setUpperFront();
    void changeBlendRate(int);
    void resetRootMtx();
    bool modelCallBack(int);
    int headModelCallBack(int);
    int wolfModelCallBack(int);
    void setHatAngle();
    void calcHairAngle(s16*);
    void setHairAngle(cXyz*, f32, f32);
    void setLookPosFromOut(cXyz*);
    bool checkAttentionPosAngle(cXyz*);
    bool checkActorPosAngle(fopAc_ac_c*, cXyz**);
    cXyz* getNeckAimPos(cXyz*, int*, int);
    s16 getNeckAimAngle(cXyz*, s16*, s16*, s16*, s16*);
    void setEyeMove(cXyz*, s16, s16);
    void setNeckAngle();
    bool commonLineCheck(cXyz*, cXyz*);
    static s16 getMoveBGActorName(cBgS_PolyInfo&, int);
    fopAc_ac_c* checkGoronRide();
    void setMoveSlantAngle();
    int setArmMatrix();
    int setFootMatrix();
    void setMatrixOffset(f32*, f32);
    int setLegAngle(f32, daAlink_footData_c*, s16*, s16*, int);
    void footBgCheck();
    void handBgCheck();
    JKRHeap* setItemHeap();
    void setIdxMask(u16*, u16*);
    J3DAnmTransform* getAnimeResource(daPy_anmHeap_c*, u16, u32);
    J3DModel* initModel(J3DModelData*, u32, u32);
    J3DModel* initModel(u16, u32);
    J3DModel* initModelEnv(u16, u32);
    int initDemoModel(J3DModel**, char const*, u32);
    static int initDemoBck(mDoExt_bckAnm**, char const*);
    int createHeap();
    void setSelectEquipItem(int);
    BOOL checkBoarStart();
    BOOL checkCanoeStart();
    void playerInit();
    BOOL checkHorseStart(u32, int);
    int setStartProcInit();
    int create();
    int setRoomInfo();
    void setShapeAngleOnGround();
    void setStepsOffset();
    void iceSlipBgCheck();
    void setIceSlipSpeed();
    void setPolygonSpeed();
    bool checkWindSpeedOnAngle() const;
    bool checkWindSpeedOnAngleAnime(int) const;
    bool checkDashAnime() const;
    f32 checkWindWallRate(cXyz const&);
    void setWindSpeed();
    void setBodyPartPos();
    void setAttentionPos();
    void setMatrix();
    static int simpleAnmPlay(J3DAnmBase*);
    void setSwordPos();
    void setItemMatrix(int);
    void setWolfItemMatrix();
    void setHandIndex(daAlink_c::daAlink_ANM);
    void setSwordAtCollision(int);
    BOOL checkNoCollisionCorret();
    void decSwordBlur();
    void resetWolfAtCollision();
    void setWolfAtCollision();
    void resetAtCollision(int);
    void setAtCollision();
    void setWolfCollisionPos();
    void initLockAt();
    void cancelLockAt();
    void setCollisionPos();
    void setCollision();
    void setFrameCtrl(daPy_frameCtrl_c* i_ctrl, u8 i_attr, s16 i_start, s16 i_end,
                                     f32 i_rate, f32 i_frame);
    const daAlink_BckData* getMainBckData(daAlink_c::daAlink_ANM) const;
    BOOL checkUnderMove0BckNoArc(daAlink_c::daAlink_ANM) const;
    BOOL checkUnderMove1BckNoArc(daAlink_c::daAlink_ANM) const;
    int getUnderUpperAnime(daAlink_c::daAlink_ANM, J3DAnmTransform**,
                                          J3DAnmTransform**, int, u32);
    void setDoubleAnimeBlendRatio(f32);
    void commonDoubleAnime(J3DAnmTransform*, J3DAnmTransform*, J3DAnmTransform*,
                                          J3DAnmTransform*, f32, f32, f32, int);
    int setDoubleAnime(f32, f32, f32, daAlink_c::daAlink_ANM, daAlink_c::daAlink_ANM,
                                      int, f32);
    void commonSingleAnime(J3DAnmTransform*, J3DAnmTransform*, f32, f32, s16);
    void setSingleAnimeBase(daAlink_c::daAlink_ANM anmID);
    void setSingleAnimeBaseMorf(daAlink_c::daAlink_ANM anmID, f32);
    void setSingleAnimeBaseSpeed(daAlink_c::daAlink_ANM anmID, f32 speed, f32 morf);
    int setSingleAnime(daAlink_c::daAlink_ANM anmID, f32 rate, f32 start, s16 endF,
                                      f32 morf);
    int setSingleAnimeParam(daAlink_c::daAlink_ANM anmID, daAlinkHIO_anm_c const*);
    void animePlay(J3DAnmTransform*, daPy_frameCtrl_c*);
    void allAnimePlay();
    void setUpperAnimeMorf(f32);
    void setUpperAnimeBase(u16);
    void setUpperAnimeBaseMorf(u16, f32);
    void setUpperAnimeBaseSpeed(u16, f32, f32);
    int setUpperAnime(u16, daAlink_c::daAlink_UPPER, f32, f32, s16, f32);
    void setUpperAnimeParam(u16, daAlink_c::daAlink_UPPER, daAlinkHIO_anm_c const*);
    int resetUpperAnime(daAlink_c::daAlink_UPPER, f32);
    void setUnderAnimeMorf(f32);
    int setUnderAnime(u16, daAlink_c::daAlink_UNDER, f32, f32, s16, f32);
    int setUnderAnimeParam(u16, daAlink_c::daAlink_UNDER, daAlinkHIO_anm_c const*);
    int resetUnderAnime(daAlink_c::daAlink_UNDER, f32);
    void setOldRootQuaternion(s16, s16, s16);
    BOOL checkAtnLeftAnime();
    BOOL checkAtnRightAnime();
    f32 getMoveGroundAngleSpeedRate();
    void setBlendMoveAnime(f32);
    void setBlendAtnMoveAnime(f32);
    void setBlendAtnBackMoveAnime(f32);
    void setFaceBck(u16, int, u16);
    void setFaceBtp(u16, int, u16);
    void setFaceBtk(u16, int, u16);
    daAlink_FTANM setFaceBasicTexture(daAlink_c::daAlink_FTANM);
    void setFaceBasicAnime(daAlink_c::daAlink_ANM);
    void setFacePriTexture(daAlink_c::daAlink_FTANM);
    void setFacePriAnime(daAlink_c::daAlink_ANM);
    void resetFacePriBck();
    void resetFacePriBtp();
    void resetFacePriBtk();
    void resetFacePriTexture();
    void resetFacePriAnime();
    void playFaceTextureAnime();
    s16 getGroundAngle(cBgS_PolyInfo*, s16);
    s16 getRoofAngle(cBgS_PolyInfo*, s16);
    BOOL getWallEdgePos(cXyz const&, cM3dGPla*, cM3dGPla*, cXyz*, int);
    void setFrontWallType();
    BOOL checkWaterPolygonUnder();
    void setWaterY();
    void setHangWaterY();
    void setSandDownBgCheckWallH();
    void setBgCheckParam();
    BOOL checkNotJumpSinkLimit();
    BOOL checkNotItemSinkLimit();
    void setSandShapeOffset();
    bool checkLv2MiddleBossBgRide(s16);
    bool getSlidePolygon(cM3dGPla*);
    BOOL checkSlope() const;
    BOOL itemTriggerCheck(u8);
    BOOL itemButtonCheck(u8);
    BOOL itemButton();
    BOOL itemTrigger();
    BOOL spActionButton();
    BOOL spActionTrigger();
    BOOL midnaTalkTrigger() const;
    BOOL swordSwingTrigger();
    void setItemActionButtonStatus(u8);
    BOOL itemActionTrigger();
    void setStickData();
    void setAtnList();
    void setRStatus(u8);
    void setRStatusEmphasys(u8);
    void setDoStatus(u8);
    void setDoStatusEmphasys(u8);
    void setDoStatusContinuation(u8);
    void setBStatus(u8);
    BOOL checkAtnWaitAnime();
    void setTiredVoice(daPy_frameCtrl_c*);
    BOOL checkRestHPAnime();
    static int getDirectionFromAngle(s16 angle);
    bool checkAttentionState();
    s16 getShapeAngleYAtnActor();
    bool setShapeAngleToAtnActor(int);
    void initServiceWaitTime();
    bool checkZeroSpeedF() const;
    void setNormalSpeedF(f32, f32);
    f32 getStickAngleDistanceRate();
    void setSpeedAndAngleNormal();
    void setSpeedAndAngleAtn();
    int checkRequestTalkActor(dAttList_c*, fopAc_ac_c*);
    bool checkServiceWaitMode();
    void setJumpMode();
    f32 getMetamorphoseNearDis() const;
    f32 getMetamorphoseFarDis() const;
    s16 getMetamorphoseFarAngle() const;
    void setMidnaMsg();
    BOOL notTalk();
    BOOL setTalkStatus();
    f32 getFrontRollRate();
    void decideCommonDoStatus();
    void decideDoStatus();
    BOOL checkWaitAction();
    void setFallVoice();
    void setLandPassiveData();
    void setStepLandVibration();
    BOOL checkLandAction(int);
    BOOL checkSlideAction();
    BOOL checkAutoJumpAction();
    BOOL checkCutJumpInFly();
    BOOL checkFrontWallTypeAction();
    int checkItemActionInitStart();
    int checkItemChangeAutoAction();
    void setFastShotTimer();
    void cancelItemUseQuake(int);
    int cancelUpperItemReadyAnime(int);
    BOOL checkItemActorPointer();
    bool checkSwordTwirlAction();
    BOOL checkUpperItemActionFly();
    void checkItemButtonChange();
    BOOL checkUpperItemAction();
    void orderPeep();
    int orderTalk(int);
    int orderZTalk();
    int checkNormalAction();
    bool checkReadyItem();
    BOOL checkItemAction();
    BOOL checkRAction();
    BOOL checkMoveDoAction();
    BOOL checkSideRollAction(int);
    BOOL checkNoUpperAnime() const;
    BOOL checkOneHandItemEquipAnime() const;
    BOOL checkItemEquipAnime() const;
    BOOL checkEquipAnime() const;
    BOOL checkWindDashAnime() const;
    BOOL checkSwordTwirlAnime() const;
    void swordEquip(int);
    void swordUnequip();
    void itemEquip(u16 i_itemID);
    void itemUnequip(u16 i_itemID, f32 i_playSpeed);
    bool checkFastUnequip();
    void allUnequip(int);
    BOOL checkItemChangeFromButton();
    BOOL checkNextActionFromButton();
    BOOL checkGroundSpecialMode();
    int commonCheckNextAction(int);
    int checkNextAction(int);
    void commonChangeItem();
    void setItemAction();
    BOOL checkNextActionFromCrouch(int);
    int checkUpperReadyThrowAnime() const;
    void getBodyAngleXBasePos(cXyz*);
    s16 getBodyAngleXAtnActor(int);
    void setBodyAngleXReadyAnime(int);
    void setMagicArmorBrk(int);
    BOOL checkMagicArmorHeavy() const;
    BOOL checkHeavyStateOn(int, int);
    void initGravity();
    void setSpecialGravity(f32, f32, int);
    void transAnimeProc(cXyz*, f32, f32);
    void setFootSpeed();
    void posMove();
    void autoGroundHit();
    bool startPeepChange();
    void setLastSceneDamage(int, u32*);
    void setLastSceneMode(u32*);
    int startRestartRoom(u32, int, int, int);
    BOOL checkCoachGuardGame();
    void checkRoofRestart();
    BOOL checkRestartRoom();
    s16 getSceneExitMoveAngle();
    int checkSceneChange(int i_exitID);
    void voiceStartLevel(u32);
    void seStartSwordCut(u32);
    void seStartMapInfo(u32);
    void seStartMapInfoLevel(u32);
    void setBasAnime(daAlink_c::daAlink_UNDER);
    void initBasAnime();
    void resetBasAnime();
    BOOL checkSightLine(f32, cXyz*);
    void setMetamorphoseModel(int);
    void keepItemData();
    void returnKeepItemData();
    BOOL setItemModel();
    BOOL setItemActor();
    void makeItemType();
    BOOL checkZoraWearAbility() const;
    BOOL checkMagicArmorWearAbility() const;
    J3DModelData* loadAramBmd(u16, u32);
    void* loadAram(u16, u32);
    J3DAnmTevRegKey* loadAramItemBrk(u16, J3DModel*);
    J3DAnmTextureSRTKey* loadAramItemBtk(u16, J3DModel*);
    J3DAnmTexPattern* loadAramItemBtp(u16, J3DModel*);
    void changeItemBck(u16, f32);
    int checkGroupItem(int, int) const;
    int checkSetItemTrigger(int);
    int checkItemSetButton(int);
    static bool checkField();
    static bool checkBossRoom();
    static bool checkDungeon();
    static bool checkCastleTown();
    static bool checkCloudSea();
    static bool checkRoomOnly();
    static bool checkLv2DungeonRoomSpecial();
    static bool checkRoomSpecial();
    static bool checkRoom();
    static bool checkNotBattleStage();
    static bool checkNotHeavyBootsStage();
    static bool checkNotAutoJumpStage();
    static bool checkCastleTownUseItem(u16);
    int changeItemTriggerKeepProc(u8, int);
    int checkNewItemChange(u8);
    void deleteEquipItem(int playSound, int deleteKantera);
    void setLight();
    void setFrontRollCrashShock(u8);
    void changeWarpMaterial(daAlink_c::daAlink_WARP_MAT_MODE);
    void commonProcInit(daAlink_c::daAlink_PROC procID);
    BOOL commonProcInitNotSameProc(daAlink_PROC procID);
    int procPreActionUnequipInit(int, fopAc_ac_c*);
    int procPreActionUnequip();
    int procServiceWaitInit();
    int procServiceWait();
    int procTiredWaitInit();
    int procTiredWait();
    int procWaitInit();
    int procWait();
    int procMoveInit();
    int procMove();
    int procAtnMoveInit();
    int procAtnMove();
    int procAtnActorWaitInit();
    int procAtnActorWait();
    int procAtnActorMoveInit();
    int procAtnActorMove();
    int procWaitTurnInit();
    int procWaitTurn();
    int procMoveTurnInit(int);
    int procMoveTurn();
    int procSideStepInit(int);
    int procSideStep();
    int procSideStepLandInit();
    int procSideStepLand();
    int procSlideInit(s16);
    int procSlide();
    int procSlideLandInit(int);
    int procSlideLand();
    int procFrontRollInit();
    int procFrontRoll();
    int procFrontRollCrashInit();
    int procFrontRollCrash();
    int procFrontRollSuccessInit();
    int procFrontRollSuccess();
    int procSideRollInit(int);
    int procSideRoll();
    void backJumpSpeedDec();
    int procBackJumpInit(int);
    int procBackJump();
    int procBackJumpLandInit(int);
    int procBackJumpLand();
    int procSlipInit();
    int procSlip();
    int procAutoJumpInit(int);
    int procAutoJump();
    int procDiveJumpInit();
    int procDiveJump();
    int procRollJumpInit();
    int procRollJump();
    int procFallInit(int, f32);
    int procFall();
    int procLandInit(f32);
    int procLand();
    int procSmallJumpInit(int);
    int procSmallJump();
    int procStepMoveInit();
    int procStepMove();
    int procCrouchInit();
    int procCrouch();
    int procCoMetamorphoseInit();
    int procCoMetamorphose();
    int procCoMetamorphoseOnlyInit();
    int procCoMetamorphoseOnly();
    int procFloorDownReboundInit();
    int procFloorDownRebound();
    int procGoronRideWaitInit(fopAc_ac_c*);
    int procGoronRideWait();
    int execute();
    void setDrawHand();
    bool checkSwordDraw();
    bool checkShieldDraw();
    bool checkItemDraw();
    int initShadowScaleLight();
    int moveShadowScaleLight();
    void shadowDraw();
    void modelCalc(J3DModel*);
    void basicModelDraw(J3DModel*);
    void modelDraw(J3DModel*, int);
    void setWaterDropColor(J3DGXColorS10 const*);
    void initTevCustomColor();
    int draw();
    ~daAlink_c();
    bool checkNoSubjectModeCamera();
    bool acceptSubjectModeChange();
    int checkSubjectAction();
    s16 checkBodyAngleX(s16);
    BOOL setBodyAngleToCamera();
    void setSubjectMode();
    BOOL subjectCancelTrigger();
    int checkSubjectEnd(int);
    void searchPeepObj(fopAc_ac_c*, void*);
    int procCoSubjectivityInit();
    int procCoSubjectivity();
    int procCoSwimSubjectivityInit();
    int procCoSwimSubjectivity();
    int procCoPeepSubjectivityInit();
    int procCoPeepSubjectivity();
    daAlink_c();
    BOOL checkWolfEnemyThrowAnime() const {
        return checkUpperAnime(0x2BD) || checkUpperAnime(0x2BE);
    }
    void setMidnaTalkStatus(u8 status) { dComIfGp_setZStatus(status, 0); }
    void set3DStatus(u8 status, u8 direction) {
        dComIfGp_set3DStatus(status, direction, 0);
    }
    void checkCutTurnCharge();
    void checkLightSwordMtrl();
    BOOL checkSwordEquipAnime() const;
    BOOL checkCutDashAnime() const;
    BOOL checkCutDashEnemyHit(dCcD_GObjInf&);
    static u32 getSwordAtType();
    void initCutTurnAt(f32, int);
    bool checkCutFinishJumpUp();
    void changeCutFast();
    bool checkCutFastReady();
    void setSwordModel();
    void offSwordModel();
    BOOL checkCutTypeNoBlur() const;
    bool checkCutTurnInput() const;
    int getCutTurnDirection() const;
    void resetCombo(int);
    void checkComboCnt();
    void setCutType(u8);
    void setCylAtParam(u32, dCcG_At_Spl, u8, u8, int, f32, f32);
    void setSwordAtParam(dCcG_At_Spl i_spl, u8 i_hitMark, u8 i_AtSe, int i_atp,
                                        f32 param_4, f32 i_radius);
    static BOOL notSwordHitVibActor(fopAc_ac_c*);
    BOOL setSwordHitVibration(dCcD_GObjInf*);
    BOOL checkAtShieldHit(dCcD_GObjInf&);
    BOOL checkCutReverseAt(dCcD_GObjInf*);
    BOOL changeCutReverseProc(daAlink_c::daAlink_ANM);
    void setCutDash(int, int);
    BOOL checkForceSwordSwing();
    void setComboReserb();
    BOOL checkComboReserb();
    int commonCutAction();
    void setSwordVoiceSe(u32);
    void setSwordChargeVoiceSe();
    void setSwordComboVoice();
    bool checkCutTurnInputTrigger();
    int checkCutAction();
    int getCutDirection();
    BOOL checkCutCancelNextMode(int);
    BOOL checkDoCutAction();
    BOOL checkCutBackState();
    BOOL checkCutHeadState();
    BOOL checkDownAttackState();
    BOOL checkCutLargeTurnState() const;
    void cancelCutCharge();
    void initCutAtnActorSearch();
    void checkCutAtnActorChange();
    void setCutJumpSpeed(int);
    int procCutNormalInit(int);
    int procCutNormal();
    int procCutFinishInit(int);
    int procCutFinish();
    int procCutFinishJumpUpInit();
    int procCutFinishJumpUp();
    int procCutFinishJumpUpLandInit();
    int procCutFinishJumpUpLand();
    int procCutReverseInit(daAlink_c::daAlink_ANM);
    int procCutReverse();
    int procCutJumpInit(int);
    int procCutJump();
    int procCutJumpLandInit(int);
    int procCutJumpLand();
    int procCutTurnInit(int, int);
    int procCutTurn();
    int procCutTurnChargeInit();
    int procCutTurnCharge();
    int procCutTurnMoveInit(int);
    int procCutTurnMove();
    int procCutDownInit();
    int procCutDown();
    int procCutDownLandInit(fopEn_enemy_c*);
    int procCutDownLand();
    int procCutHeadInit();
    int procCutHead();
    int procCutHeadLandInit();
    int procCutHeadLand();
    int procCutLargeJumpChargeInit();
    int procCutLargeJumpCharge();
    int procCutLargeJumpInit();
    int procCutLargeJump();
    int procCutLargeJumpLandInit(int);
    int procCutLargeJumpLand();
    int procSwordUnequipSpInit();
    int procSwordUnequipSp();
    s16 getFreezeR() const;
    s16 getFreezeG() const;
    s16 getFreezeB() const;
    bool checkMiddleBossGoronRoom();
    void freezeTimerDamage();
    BOOL checkNoLandDamageSlidePolygon();
    void checkCutLandDamage();
    BOOL checkCaughtEscapeCutTurn();
    f32 damageMagnification(int i_checkZoraMag, int);
    int setDamagePoint(int i_dmgAmount, int i_checkZoraMag, int i_setDmgTimer, int);
    void setDamagePointNormal(int);
    void setLandDamagePoint(int);
    cXyz* getDamageVec(dCcD_GObjInf*);
    void setDashDamage();
    static BOOL checkIcePolygonDamage(cBgS_PolyInfo*);
    BOOL checkMagicArmorNoDamage();
    int checkPolyDamage();
    BOOL checkElecReturnDamage(dCcD_GObjInf&, fopAc_ac_c**);
    void damageTimerCount();
    bool checkHugeAttack(int) const;
    bool checkLargeAttack(int) const;
    BOOL checkDamageAction();
    int procDamageInit(dCcD_GObjInf*, int);
    int procDamage();
    int procCoLargeDamageInit(int, int, s16, s16, dCcD_GObjInf*, int);
    int procCoLargeDamage();
    int procLargeDamageUpInit(int, int, s16, s16);
    int procLargeDamageUp();
    int procCoLargeDamageWallInit(int, int, s16, s16);
    int procCoLargeDamageWall();
    int procCoPolyDamageInit();
    int procCoPolyDamage();
    int procLandDamageInit(int);
    int procLandDamage();
    int procCoElecDamageInit(fopAc_ac_c*, dCcD_GObjInf*, int);
    int procCoElecDamage();
    int procStEscapeInit();
    int procStEscape();
    int procDkCaughtInit(fpc_ProcID);
    int procDkCaught();
    void setScreamWaitAnime();
    int procScreamWaitInit();
    int procScreamWait();
    int procCoSandWallHitInit();
    int procCoSandWallHit();
    int procCoLavaReturnInit(int);
    int procCoLavaReturn();
    int procCoSwimFreezeReturnInit();
    int procCoSwimFreezeReturn();
    static BOOL checkEnemyGroup(fopAc_ac_c*);
    static BOOL checkSpecialNpc(fopAc_ac_c*);
    BOOL checkShieldAttackEmphasys();
    BOOL checkGuardActionChange();
    void stickArrowIncrement(int);
    void setArrowShieldActor(fopAc_ac_c*, int);
    BOOL checkWoodShieldEquipNotIronBall() const;
    void getArrowShieldOffset(cXyz const*, csXyz const*, cXyz*, cXyz*) const;
    void setArrowShieldPos(cXyz*, csXyz*, cXyz const*, cXyz const*) const;
    BOOL checkUpperGuardAnime() const;
    BOOL checkPlayerGuardAndAttack() const;
    BOOL checkGuardAccept();
    void setUpperGuardAnime(f32);
    void setShieldGuard();
    void setGuardSe(dCcD_GObjInf*);
    void setSmallGuard(dCcD_GObjInf*);
    int procGuardSlipInit(int, dCcD_GObjInf*);
    int procGuardSlip();
    int procGuardAttackInit();
    int procGuardAttack();
    int procGuardBreakInit();
    int procGuardBreak();
    int procTurnMoveInit(int);
    int procTurnMove();
    void getArrowFlyData(f32*, f32*, int) const;
    f32 getArrowIncAtR() const;
    s16 getBombArrowFlyExplodeTime() const;
    f32 getArrowIncAtMaxStart() const;
    f32 getArrowIncAtMax() const;
    BOOL checkBowAndSlingItem(int);
    void setSlingModel();
    cXyz* checkBowCameraArrowPosP(s16*, s16*);
    BOOL checkBowReadyAnime() const;
    BOOL checkBowAnime() const;
    void makeArrow();
    void deleteArrow();
    void setBowOrSlingStatus();
    void changeArrowType();
    BOOL cancelBowMoveRideNotAtn();
    BOOL cancelBowMove();
    void setBowReadyAnime();
    void setBowReloadAnime();
    BOOL checkUpperItemActionBow();
    void checkUpperItemActionBowFly();
    int checkNextActionBow();
    void setBowModel();
    bool checkBowGrabLeftHand() const;
    void setBowHangAnime();
    void setBowNormalAnime();
    void setBowSight();
    int procBowSubjectInit();
    int procBowSubject();
    int procBowMoveInit();
    int procBowMove();
    BOOL checkBoomerangLockAccept();
    f32 getBoomSpeed();
    f32 getBoomCatchSpeed() const;
    f32 getBoomFlyMax() const;
    f32 getBoomLockMax();
    s16 getBoomBgThroughTime() const;
    bool checkBossBabaRoom();
    void cancelBoomerangLock(fopAc_ac_c*);
    fopAc_ac_c* getBoomerangActor();
    bool checkBoomerangChargeEnd();
    BOOL checkBoomerangCarry(fopAc_ac_c*);
    void initBoomerangUpperAnimeSpeed(int);
    BOOL checkBoomerangAnime() const;
    BOOL checkBoomerangThrowAnime() const;
    void setBoomerangReadyQuake();
    void setBoomerangReadyAnime();
    void setThrowBoomerangAnime();
    void setBoomerangCatchAnime();
    void throwBoomerang();
    int returnBoomerang(int);
    int checkUpperItemActionBoomerang();
    void checkUpperItemActionBoomerangFly();
    int checkNextActionBoomerang();
    int checkBoomerangCatchAction();
    void setBoomerangSight();
    int procBoomerangSubjectInit();
    int procBoomerangSubject();
    int procBoomerangMoveInit();
    int procBoomerangMove();
    int procBoomerangCatchInit();
    int procBoomerangCatch();
    static BOOL checkLv6BossRoom();
    f32 getCopyRodBallSpeed() const;
    f32 getCopyRodBallReturnSpeed() const;
    f32 getCopyRodBallDisMax() const;
    fopAc_ac_c* getCopyRodControllActor();
    fopAc_ac_c* getCopyRodCameraActor();
    void initCopyRodUpperAnimeSpeed(int);
    static BOOL checkForestOldCentury();
    int checkCopyRodAnime() const;
    void setCopyRodControllAnime();
    void setCopyRodControllUpperSpeedRate();
    void setCopyRodModel();
    void setCopyRodReadyAnime();
    void throwCopyRod();
    int returnCopyRod();
    int checkUpperItemActionCopyRod();
    void checkUpperItemActionCopyRodFly();
    int checkNextActionCopyRod();
    void setCopyRodSight();
    int procCopyRodSubjectInit();
    int procCopyRodSubject();
    int procCopyRodMoveInit();
    int procCopyRodMove();
    int procCopyRodSwingInit();
    int procCopyRodSwing();
    int procCopyRodReviveInit();
    int procCopyRodRevive();
    void concatMagneBootMtx();
    void concatMagneBootInvMtx();
    void multVecMagneBootInvMtx(cXyz*);
    bool commonMagneLineCheck(cXyz*, cXyz*);
    BOOL checkBootsMoveAnime(int);
    int setHeavyBoots(int);
    s16 getMagneBootsLocalAngleY(s16, int);
    void setMagneBootsMtx(cBgS_PolyInfo*, int);
    int cancelMagneBootsOn();
    int checkMagneBootsFly();
    int procBootsEquipInit();
    int procBootsEquip();
    int procMagneBootsFlyInit();
    int procMagneBootsFly();
    s16 getBombExplodeTime() const;
    f32 getBombGravity() const;
    f32 getBombMaxFallSpeed() const;
    f32 getBombBoundRate() const;
    f32 getBombStopSpeedY() const;
    f32 getBombMaxSpeedY() const;
    f32 getBombEffScale() const;
    f32 getBombAtR() const;
    s16 getEnemyBombColorR() const;
    f32 getBombWaterGravity() const;
    f32 getBombWaterMaxFallSpeed() const;
    f32 getBombExplodeWaterEffectLimit() const;
    s16 getBombInsectLimitAngle() const;
    BOOL checkGrabLineCheck();
    f32 getGrabThrowRate();
    BOOL checkGrabThrowAnime() const;
    BOOL checkGrabAnime() const;
    BOOL checkGrabAnimeAndThrow() const;
    BOOL checkGrabCarryActor();
    BOOL checkGrabSlowMoveActor();
    BOOL checkGrabHeavyActor();
    BOOL checkGrabSideActor();
    void setGrabUpperAnime(f32);
    BOOL checkGrabRooster();
    void setGrabItemPos();
    void freeGrabItem();
    void setGrabUpperSpeedRate();
    void setCarryArmAngle(f32, f32);
    BOOL checkGrabNotThrow();
    BOOL checkNextActionGrab();
    void initGrabNextMode();
    void setGrabItemThrow();
    BOOL checkUpperGrabItemThrow(f32);
    void putObjLineCheck(dBgS_LinChk&, cXyz*, fopAc_ac_c*);
    bool grabLineCheck(cXyz*, cXyz*);
    void setGrabItemActor(fopAc_ac_c*);
    int procGrabReadyInit();
    int procGrabReady();
    int procGrabUpInit();
    int procGrabUp();
    int procGrabMiss();
    int procGrabThrowInit(int);
    int procGrabThrow();
    int procGrabPutInit();
    int procGrabPut();
    int procGrabWaitInit();
    int procGrabWait();
    int procGrabReboundInit(int);
    int procGrabRebound();
    int procGrabStandInit();
    int procGrabStand();
    BOOL checkInsectActorName(fopAc_ac_c*);
    int procInsectCatchInit();
    int procInsectCatch();
    int procPickUpInit();
    int procPickUp();
    int procPickPutInit(int);
    int procPickPut();
    BOOL checkSetChainPullAnime(s16);
    s16 getChainStickAngleY(s16) const;
    u8 checkChainEmphasys();
    BOOL searchFmChainPos();
    BOOL setFmChainPosFromOut(fopAc_ac_c*, cXyz*, int);
    int procFmChainUpInit();
    int procFmChainUp();
    int procFmChainStrongPullInit();
    int procFmChainStrongPull();
    void setWallGrabStatus(u8, u8);
    int getWallGrabStatus();
    BOOL wallGrabTrigger();
    BOOL wallGrabButton();
    int setPushPullKeepData(dBgW_Base::PushPullLabel, int);
    BOOL checkPushPullTurnBlock();
    BOOL checkPullBehindWall();
    void offGoatStopGame();
    BOOL checkGoatCatchActor(fopAc_ac_c*);
    f32 getGoatCatchDistance2();
    int endPushPull();
    f32 getPushPullAnimeSpeed();
    int procCoPushPullWaitInit(int);
    int procCoPushPullWait();
    int procCoPushMoveInit(int, int);
    int procCoPushMove();
    int procPullMoveInit(int);
    int procPullMove();
    void setGoatStopGameFail(fopAc_ac_c*);
    int procGoatMoveInit();
    int procGoatMove();
    int procGoatCatchInit(fopAc_ac_c*, f32);
    int procGoatCatch();
    int procGoatStrokeInit();
    int procGoatStroke();
    int procGoronMoveInit();
    int procGoronMove();
    BOOL checkSumouVsActor();
    void cancelSumouMode();
    BOOL sumouPunchTrigger();
    void setSumouPunchStatus();
    int procSumouReadyInit();
    int procSumouReady();
    int procSumouMoveInit();
    int procSumouMove();
    int procSumouSideMoveInit();
    int procSumouSideMove();
    int procSumouActionInit(BOOL i_doTrigger, BOOL i_punchTrigger, int unused);
    int procSumouAction();
    int procSumouStaggerInit();
    int procSumouStagger();
    int procSumouWinLoseInit();
    int procSumouWinLose();
    int procSumouShikoInit();
    int procSumouShiko();
    static BOOL checkHorseZeldaBowMode();
    static void setHorseZeldaDamage();
    static BOOL checkHorseDashAccept();
    static BOOL checkCowGame();
    int getReinRideDirection();
    int checkReinRideBgCheck();
    void commonInitForceRideRein();
    int initForceRideBoar();
    void initForceRideHorse();
    void rideGetOff();
    BOOL checkHorseNotDamageReaction() const;
    BOOL checkHorseWaitLashAnime() const;
    BOOL checkHorseReinLeftOnly() const;
    int getReinHandType() const;
    BOOL checkHorseLieAnime() const;
    BOOL checkHorseSubjectivity() const;
    void setHorseSwordUpAnime();
    void setHorseTurnUpperAnime(int);
    BOOL checkHorseNoUpperAnime() const;
    void getHorseReinHandPos(cXyz*, cXyz*);
    BOOL checkHorseNotGrab() const;
    void setHorseStirrup();
    void changeBoarRunRide();
    int setSyncHorsePos();
    int setSyncBoarPos();
    int setSyncBoarRunPos();
    BOOL setSyncRidePos();
    void setHorseTurnAnime();
    void getBaseHorseAnime(daAlink_c::daAlink_ANM*);
    int checkHorseSpecialProc();
    BOOL checkHorseServiceWaitAnime();
    int setSyncHorse(int);
    int setSyncBoar(int);
    int setSyncRide(int);
    void setBaseHorseAnimeFrame();
    void setBaseBoarAnime();
    void setBaseRideAnime();
    bool checkHorseSwordUpSpped();
    void setHorseSwordUp(int);
    int setRideSubjectAngle(s16);
    void setBodyAngleRideReadyAnime();
    BOOL checkHorseGetOffWallCheck(cXyz*, cXyz*, s16);
    int checkHorseGetOffDirection();
    void boarForceGetOff();
    void horseGetOffEnd();
    int checkNextActionHorse();
    BOOL checkHorseGetOff();
    int checkHorseGetOffAndSetDoStatus();
    int setHorseGetOff(int);
    int procHorseRideInit();
    int procHorseRide();
    int procHorseGetOffInit(int);
    int procHorseGetOff();
    int procHorseWaitInit();
    int procHorseWait();
    int procHorseTurnInit();
    int procHorseTurn();
    int procHorseJumpInit();
    int procHorseJump();
    int procHorseLandInit();
    int procHorseLand();
    int procHorseSubjectivityInit();
    int procHorseSubjectivity();
    int procHorseCutInit();
    int procHorseCut();
    int procHorseCutChargeReadyInit();
    int procHorseCutChargeReady();
    int procHorseCutTurnInit();
    int procHorseCutTurn();
    int procHorseDamageInit(dCcD_GObjInf*);
    int procHorseDamage();
    int procHorseBowSubjectInit();
    int procHorseBowSubject();
    int procHorseBowMoveInit();
    int procHorseBowMove();
    int procHorseGrabMoveInit();
    int procHorseGrabMove();
    int procHorseBoomerangSubjectInit();
    int procHorseBoomerangSubject();
    int procHorseBoomerangMoveInit();
    int procHorseBoomerangMove();
    int procHorseHookshotSubjectInit();
    int procHorseHookshotSubject();
    int procHorseHookshotMoveInit();
    int procHorseHookshotMove();
    int procHorseBottleDrinkInit(u16);
    int procHorseBottleDrink();
    int procHorseKandelaarPourInit();
    int procHorseKandelaarPour();
    int procHorseComebackInit();
    int procHorseComeback();
    int procHorseRunInit();
    int procHorseRun();
    int procHorseHangInit(dCcD_GObjInf*, int);
    int procHorseHang();
    int procHorseGetKeyInit();
    int procHorseGetKey();
    int procHorseLookDownInit();
    int procHorseLookDown();
    int procBoarRunInit();
    int procBoarRun();
    f32 getCanoeMaxSpeed() const;
    f32 getCanoeBackMaxSpeed() const;
    f32 getCanoeCres() const;
    f32 getCanoeSpeedRate() const;
    s16 getCanoeMaxRotSpeed() const;
    static cXyz* getCanoeLocalPaddleTop();
    BOOL checkCanoeRideTandem();
    BOOL checkFishingRodAndLureItem() const;
    void initFishingRodHand();
    MtxP getCanoePaddleMatrix();
    BOOL checkFishingRodGrab(fopAc_ac_c*) const;
    BOOL checkCanoePaddleGrab(fopAc_ac_c*) const;
    BOOL checkCanoeFishingRodGrabOrPut() const;
    void initCanoeRide();
    BOOL checkCanoeJumpRide();
    void setArmReelAnime();
    void setFishingWaitAction();
    void setFishGetFace();
    void setRideCanoeBasePos(fopAc_ac_c*);
    void setSyncCanoePos();
    int canoeCommon();
    int checkNextActionCanoe();
    int procCanoeRideInit();
    int procCanoeRide();
    int procCanoeJumpRideInit(fopAc_ac_c*);
    int procCanoeJumpRide();
    int procCanoeGetOffInit();
    int procCanoeGetOff();
    int procCanoeWaitInit(int);
    int procCanoeWait();
    int procCanoeRowInit(int);
    int procCanoeRow();
    int procCanoePaddleShiftInit(int);
    int procCanoePaddleShift();
    int procCanoePaddlePutInit(int);
    int procCanoePaddlePut();
    int procCanoePaddleGrabInit();
    int procCanoePaddleGrab();
    int procCanoeRodGrabInit();
    int procCanoeRodGrab();
    int procCanoeFishingWaitInit();
    int procCanoeFishingWait();
    int procCanoeFishingReelInit();
    int procCanoeFishingReel();
    int procCanoeFishingGetInit();
    int procCanoeFishingGet();
    int procCanoeSubjectivityInit();
    int procCanoeSubjectivity();
    int procCanoeBowSubjectInit();
    int procCanoeBowSubject();
    int procCanoeBowMoveInit();
    int procCanoeBowMove();
    int procCanoeGrabMoveInit();
    int procCanoeGrabMove();
    int procCanoeBoomerangSubjectInit();
    int procCanoeBoomerangSubject();
    int procCanoeBoomerangMoveInit();
    int procCanoeBoomerangMove();
    int procCanoeHookshotSubjectInit();
    int procCanoeHookshotSubject();
    int procCanoeHookshotMoveInit();
    int procCanoeHookshotMove();
    int procCanoeBottleDrinkInit(u16);
    int procCanoeBottleDrink();
    int procCanoeKandelaarPourInit();
    int procCanoeKandelaarPour();
    void setGroundFishingRodActor();
    int procFishingCastInit();
    int procFishingCast();
    int procFishingFoodInit();
    int procFishingFood();
    f32 getCrawlMoveAnmSpeed();
    f32 getCrawlMoveSpeed();
    void setCrawlMoveDirectionArrow();
    BOOL changeCrawlAutoMoveProc(cXyz*);
    int getCrawlMoveVec(cXyz*, cXyz*, cXyz*, int, int, u8*);
    void crawlBgCheck(cXyz*, cXyz*, int);
    BOOL checkCrawlSideWall(cXyz*, cXyz*, cXyz*, cXyz*, s16*, s16*);
    void decideCrawlDoStatus();
    BOOL checkNotCrawlStand(cXyz*);
    BOOL checkNotCrawlStand(cXyz*, cXyz*);
    BOOL checkCrawlInHoll(cXyz*, cXyz*, cXyz*, int);
    void setCrawlMoveHoll();
    void setCrawlMoveAngle();
    void stopHalfMoveAnime(f32);
    void setCrawlAutoMoveAimPos();
    int procCrawlStartInit();
    int procCrawlStart();
    int procCrawlMoveInit(s16, s16);
    int procCrawlMove();
    int procCrawlAutoMoveInit(int, cXyz*);
    int procCrawlAutoMove();
    int procCrawlEndInit(int, s16, s16);
    int procCrawlEnd();
    f32 getHangMoveAnmSpeed();
    int getHangDirectionFromAngle();
    BOOL hangMoveBgCheck(s16, cXyz*);
    BOOL changeHangMoveProc(int);
    bool checkHangFootWall();
    void setHangGroundY();
    int changeHangEndProc();
    void checkHangStartSideWall(s16);
    int procHangStartInit();
    int procHangStart();
    int procHangFallStartInit(cM3dGPla*);
    int procHangFallStart();
    int procHangUpInit(int);
    int procHangUp();
    int procHangWaitInit();
    int procHangWait();
    int procHangMoveInit(int);
    int procHangMove();
    int procHangClimbInit(f32);
    int procHangClimb();
    int procHangWallCatchInit();
    int procHangWallCatch();
    int procHangReadyInit();
    int procHangReady();
    int procHangLeverDownInit();
    int procHangLeverDown();
    int setDragonHangPos();
    int setOctaIealHangPos();
    int setBossBodyHangPos();
    int procBossBodyHangInit(fopAc_ac_c*);
    int procBossBodyHang();
    int getLadderUnitCount() const;
    int setLadderInit();
    void setLadderPosInit();
    void setLadderPos(int);
    f32 getLadderMoveAnmSpeed();
    int changeLadderMoveProc(int);
    int setMoveBGLadderCorrect();
    int checkLadderFall();
    int procLadderUpStartInit();
    int procLadderUpStart();
    int procLadderUpEndInit(int);
    int procLadderUpEnd();
    int procLadderDownStartInit();
    int procLadderDownStart();
    int procLadderDownEndInit(int);
    int procLadderDownEnd();
    int procLadderMoveInit(int, int, cXyz*);
    int procLadderMove();
    f32 getClimbMoveUpDownAnmSpeed();
    f32 getClimbMoveSideAnmSpeed();
    BOOL checkClimbCode(cBgS_PolyInfo&);
    BOOL setClimbInit();
    void setClimbShapeOffset();
    int getClimbDirectionFromAngle();
    void changeClimbMoveProc(int);
    BOOL checkClimbMoveUpDownProc(int);
    BOOL checkClimbMoveSideProc(int);
    int setMoveBGClimbCorrect();
    int checkBgCorrectClimbMove(cXyz*, cXyz*);
    BOOL checkClimbRoof(f32);
    int checkClimbGround(cXyz*, f32);
    BOOL checkBgClimbMove(int);
    int setClimbStartNotGround();
    int procClimbUpStartInit(int);
    int procClimbUpStart();
    int procClimbDownStartInit(s16);
    int procClimbDownStart();
    int procClimbMoveUpDownInit(int);
    int procClimbMoveUpDown();
    int procClimbMoveSideInit();
    int procClimbMoveSide();
    int procClimbWaitInit(int, int);
    int procClimbWait();
    int procClimbToRoofInit();
    int procClimbToRoof();
    f32 getRoofHangFMoveAnmSpeed() const;
    f32 getRoofHangSMoveAnmSpeed() const;
    void setRoofHangHandOnSE(cBgS_PolyInfo*);
    int checkRoofHangMovePos();
    int commonRoofHangProc();
    int checkNextActionRoofHang();
    int procRoofHangStartInit(cBgS_PolyInfo const&, cXyz const&, int);
    int procRoofHangStart();
    int procRoofHangWaitInit(int);
    int procRoofHangWait();
    int procRoofHangFrontMoveInit();
    int procRoofHangFrontMove();
    int procRoofHangSideMoveInit();
    int procRoofHangSideMove();
    int procRoofHangTurnInit();
    int procRoofHangTurn();
    void setRoofHangSwitch();
    int procRoofSwitchHangInit(fopAc_ac_c*);
    int procRoofSwitchHang();
    BOOL checkZoraWearMaskDraw();
    BOOL checkAcceptUseItemInWater(u16) const;
    void swimDeleteItem();
    bool getZoraSwim() const;
    f32 getSwimFrontMaxSpeed() const;
    f32 getSwimMaxFallSpeed() const;
    void checkOxygenTimer();
    void offOxygenTimer();
    BOOL checkPossibleWaterInMode() const;
    void setWaterInAnmRate(daPy_frameCtrl_c*, f32);
    void swimBgCheck(f32);
    void setSpeedAndAngleSwim();
    int checkNextActionSwim();
    int checkSwimAction(int);
    int checkSwimUpAction();
    void swimOutAfter(int);
    BOOL checkSwimFall();
    int checkSwimOutAction();
    void setSwimMoveAnime();
    bool checkSwimButtonAccept();
    bool checkUpSwimButtonAccept();
    BOOL checkSwimButtonMove();
    BOOL checkZoraSwimMove();
    BOOL checkSwimNeckUpDown() const;
    void setSwimUpDownOffset();
    int procSwimUpInit();
    int procSwimUp();
    int procSwimWaitInit(int);
    int procSwimWait();
    int procSwimMoveInit();
    int procSwimMove();
    int procSwimDiveInit();
    int procSwimDive();
    int procSwimHookshotSubjectInit();
    int procSwimHookshotSubject();
    int procSwimHookshotMoveInit();
    int procSwimHookshotMove();
    int procSwimDamageInit(dCcD_GObjInf*);
    int procSwimDamage();
    int procOctaIealSpitInit();
    int procOctaIealSpit();
    bool checkSnowCode() const;
    static bool checkSnowCodePolygon(cBgS_PolyInfo&);
    bool checkBoardRestart();
    int boardCommon(int);
    void setCommonBoardAnime(int);
    int checkNextActionBoard();
    int checkBoardSwordTriggerAction();
    int commonProcBoardInit(daAlink_c::daAlink_PROC);
    f32 getBoardRowAnmSpeed();
    void setBoardLandAnime();
    int procBoardRideInit();
    int procBoardRide();
    int procBoardWaitInit(fopAc_ac_c*);
    int procBoardWait();
    int procBoardRowInit();
    int procBoardRow();
    int procBoardTurnInit();
    int procBoardTurn();
    int procBoardJumpInit(f32, int);
    int procBoardJump();
    int procBoardSubjectivityInit();
    int procBoardSubjectivity();
    int procBoardCutInit();
    int procBoardCut();
    int procBoardCutTurnInit();
    int procBoardCutTurn();
    void hookshotAtHitCallBack(dCcD_GObjInf*, fopAc_ac_c*, dCcD_GObjInf*);
    void resetHookshotMode();
    BOOL setEnemyBombHookshot(fopAc_ac_c*);
    bool checkLv7BossRoom();
    BOOL checkHookshotStickBG(cBgS_PolyInfo&);
    void cancelHookshotCarry();
    void changeHookshotDrawModel();
    BOOL checkHookshotRoofLv7Boss();
    BOOL checkChaseHookshot();
    BOOL checkOctaIealSpecialCollect();
    BOOL checkBossOctaIealRoom();
    BOOL checkHookshotWait() const;
    void setHookshotCatchNow();
    void setHookshotModel();
    void setHookshotSight();
    void cancelHookshotShot();
    bool cancelHookshotMove();
    BOOL checkHookshotReadyMaterialOffMode() const;
    void setHookshotReadyMaterial();
    int initHookshotUpperAnimeSpeed(int);
    void initHookshotReady();
    void setHookshotReadyAnime();
    int checkUpperItemActionHookshot();
    int checkNextActionHookshot();
    void setHookshotReturnEnd();
    int setHookshotHangMoveBGCollect();
    void setHookshotTopPosFly();
    void setHookshotPos();
    void setHookshotRoofWaitAnime();
    void setHookshotWallWaitAnime();
    void hookshotRoofTurn();
    void initHookshotRoofWaitActor(fopAc_ac_c*);
    int checkNextHookPoint();
    int checkLandHookshotHang();
    int commonHookshotRoofWait();
    int commonHookshotWallWait();
    int procHookshotSubjectInit();
    int procHookshotSubject();
    int procHookshotMoveInit();
    int procHookshotMove();
    int procHookshotFlyInit();
    int procHookshotFly();
    int procHookshotRoofWaitInit(int, fopAc_ac_c*, int);
    int procHookshotRoofWait();
    int procHookshotRoofShootInit(fopAc_ac_c*);
    int procHookshotRoofShoot();
    int procHookshotRoofBootsInit(fopAc_ac_c*);
    int procHookshotRoofBoots();
    int procHookshotWallWaitInit(int, s16, int);
    int procHookshotWallWait();
    int procHookshotWallShootInit();
    int procHookshotWallShoot();
    f32 getSpinnerGravity() const;
    f32 getSpinnerMaxFallSpeed() const;
    f32 getSpinnerJumpRate() const;
    s16 getSpinnerRideMoveTime();
    f32 getSpinnerRideSpeedF();
    f32 getSpinnerRideDecSpeedMax() const;
    f32 getSpinnerRideDecSpeedMin() const;
    f32 getSpinnerRideDecSpeedRate() const;
    s16 getSpinnerRideRotAngleMax() const;
    s16 getSpinnerRideRotAngleMin() const;
    bool checkLv4BossRoom();
    void setSpinnerSyncPos();
    int procSpinnerReadyInit();
    int procSpinnerReady();
    void setSpinnerStatus(u8, u8);
    int procSpinnerWaitInit();
    int procSpinnerWait();
    int bottleModelCallBack();
    BOOL checkOilBottleItemNotGet(u16);
    void setBottleModel(u16);
    int commonBottleDrink(int);
    fopAc_ac_c* makeFairy(cXyz*, u32);
    int procBottleDrinkInit(u16);
    int procBottleDrink();
    int procBottleOpenInit(u16);
    int procBottleOpen();
    int procBottleSwingInit(fopAc_ac_c*, int);
    int procBottleSwing();
    int procBottleGetInit(int);
    int procBottleGet();
    BOOL checkWaterInKandelaarOffset(f32);
    void checkWaterInKandelaar(f32);
    void offKandelaarModel();
    int kandelaarModelCallBack();
    BOOL checkKandelaarEquipAnime() const;
    void preKandelaarDraw();
    void setKandelaarModel();
    void resetOilBottleModel();
    void commonKandelaarPourInit();
    int commonKandelaarPour(int);
    void initKandelaarSwing();
    int procKandelaarSwingInit();
    int procKandelaarSwing();
    int procKandelaarPourInit();
    int procKandelaarPour();
    void setGrassWhistleModel(int);
    void setHorseWhistleModel();
    int procGrassWhistleGetInit();
    int procGrassWhistleGet();
    int procGrassWhistleWaitInit(int, int, int, s16, cXyz*);
    int procGrassWhistleWait();
    int procCoHorseCallWaitInit(int);
    int procCoHorseCallWait();
    int procHawkCatchInit();
    int procHawkCatch();
    int procHawkSubjectInit();
    int procHawkSubject();
    bool checkIronBallThrowMode() const;
    bool checkIronBallThrowReturnMode() const;
    void setIronBallWaitUpperAnime(int);
    void checkIronBallDelete();
    void setIronBallReturn(int);
    void setIronBallBgCheckPos();
    void setIronBallModel();
    int setIronBallGroundVec(cXyz const*, cXyz*);
    void setIronBallChainPos();
    BOOL checkIronBallReturnChange();
    void setIronBallPos();
    BOOL checkIronBallAnime() const;
    void setIronBallReadyAnime();
    void setIronBallBaseAnime();
    BOOL checkUpperItemActionIronBall();
    int checkNextActionIronBall();
    int procIronBallSubjectInit();
    int procIronBallSubject();
    int procIronBallMoveInit();
    int procIronBallMove();
    int procIronBallThrowInit();
    int procIronBallThrow();
    int procIronBallReturnInit();
    int procIronBallReturn();
    BOOL checkEventRun() const;
    void createNpcTks(cXyz*, int, u32);
    int checkDemoAction();
    bool checkDemoMoveMode(u32) const;
    void setDemoMoveData(u32*, cXyz const*);
    void setNoDrawSwordShield(int, u16);
    void setDemoData();
    void resetDemoBck();
    void endHighModel();
    void resetSpecialEvent();
    void endDemoMode();
    fopAc_ac_c* getDemoLookActor();
    BOOL checkFlyAtnWait();
    void setGetItemFace(u16);
    BOOL checkGrabTalkActor(fopAc_ac_c*);
    int setTalkStartBack(cXyz*);
    void setShapeAngleToTalkActor();
    void setTalkAnime();
    u8 setTradeItemAnime();
    void setTradeItemOutHand();
    BOOL checkEndMessage(u32);
    u8 setDemoRightHandIndex(u16);
    u8 setDemoLeftHandIndex(u16);
    void setDemoRide(u16);
    void setDemoBodyBck(dDemo_actor_c*, u16);
    static BOOL checkFinalBattle();
    BOOL checkRestartDead(int, int);
    void setDeadRideSyncPos();
    BOOL checkDeadHP();
    BOOL checkDeadAction(int);
    void setHighModelBck(mDoExt_bckAnm*, u16);
    void setHighModelFaceBtk(u16);
    void setDemoBrk(J3DAnmTevRegKey**, J3DModel*, u16);
    f32 setStickAnmData(J3DAnmBase*, int, int, u16, int);
    int procDemoCommon();
    int procCoToolDemoInit();
    int procCoToolDemo();
    int procCoTalkInit();
    int procCoTalk();
    int procCoOpenTreasureInit();
    int procCoOpenTreasure();
    int procCoUnequipInit();
    int procCoUnequip();
    void setGetSubBgm(int);
    int procCoGetItemInit();
    int procCoGetItem();
    int procCoTurnBackInit();
    int procCoTurnBack();
    int procDoorOpenInit();
    int procDoorOpen();
    int procCoLookWaitInit();
    int procCoLookWait();
    int procCoDemoPushPullWaitInit();
    int procCoDemoPushMoveInit();
    void setMonkeyMoveAnime();
    int procMonkeyMoveInit();
    int procMonkeyMove();
    int procDemoBoomerangCatchInit();
    int procDemoBoomerangCatch();
    int procCoDeadInit(int);
    int procCoDead();
    int procCoLookAroundInit();
    int procCoLookAround();
    int procBossAtnWaitInit();
    int procCoQuakeWaitInit();
    int procCoCaughtInit();
    int procLookUpInit();
    int procLookUp();
    int procLookUpToGetItemInit();
    int procLookUpToGetItem();
    int procHandPatInit();
    int procHandPat();
    int procCoFogDeadInit();
    int procCoFogDead();
    int procWolfSmellWaitInit();
    int procCoNodInit();
    int procCoNod();
    int procCoEyeAwayInit();
    int procCoGlareInit();
    int procCoGlare();
    int procGoatStopReadyInit();
    int procGoatStopReady();
    int procCoGetReadySitInit();
    int procCoGetReadySit();
    int procCoTwGateInit();
    int procCoTwGate();
    int procWolfSnowEscapeInit();
    int procWolfSnowEscape();
    int procZoraMoveInit();
    int procZoraMove();
    int procLookAroundTurnInit();
    int procLookAroundTurn();
    int procTradeItemOutInit();
    int procTradeItemOut();
    static BOOL checkLetterItem(int);
    int procNotUseItemInit(int);
    int procNotUseItem();
    int procSwordReadyInit();
    int procSwordReady();
    void setSwordPushAnime();
    int procSwordPushInit();
    int procSwordPush();
    int procGanonFinishInit();
    int procGanonFinish();
    int procCutFastReadyInit();
    int procCutFastReady();
    int procMasterSwordStickInit();
    int procMasterSwordStick();
    int procMasterSwordPullInit();
    int procMasterSwordPull();
    static BOOL checkLv7DungeonShop();
    int procDungeonWarpReadyInit();
    int procDungeonWarpReady();
    int procDungeonWarpInit();
    int procDungeonWarp();
    int procDungeonWarpSceneStartInit();
    int procDungeonWarpSceneStart();
    bool checkAcceptWarp();
    void dungeonReturnWarp();
    void checkWarpStart();
    int warpModelTexScroll();
    int procCoWarpInit(int, int);
    int procCoWarp();
    int commonWaitTurnInit();
    int commonGrabPutInit();
    int commonLargeDamageUpInit(int, int, s16, s16);
    int commonFallInit(int);
    JPABaseEmitter* setEmitter(u32*, u16, cXyz const*, csXyz const*);
    JPABaseEmitter* setEmitterPolyColor(u32*, u16, cBgS_PolyInfo&, cXyz const*,
                                                       csXyz const*);
    JPABaseEmitter* setEmitterColor(u32*, u16, cXyz const*, csXyz const*);
    void stopDrawParticle(u32);
    void setEffectFrontRollParam();
    void setEffectSlipParam();
    void setEffectRunParam();
    void setEffectSmallLandParam();
    void setEffectLandParam();
    void setEffectSumouParam();
    void setFootEffectProcType(int);
    void setWolfFootOn(int);
    void setFootMark(cXyz*, u16, int);
    void setEffect();
    void setSumouEffect();
    void setWaterfallEffect(cXyz const*, u32*);
    void setWaterfallEffect();
    void setMagneBootsEffect();
    void setSwordChargeEffect();
    void setElecDamageEffect();
    void clearCutTurnEffectID();
    void setCutTurnEffect();
    void setHorseCutTurnEffect();
    void setCutLargeJumpLandEffect();
    void setBootsLightEffect();
    void setLightningSwordEffect();
    void setWolfRollAttackEffect();
    void setWolfDigEffect();
    void setWolfSwimEndEffect(JPABaseEmitter**, JPABaseEmitter**);
    void setWolfLockAttackEffect();
    void setWolfJumpAttackEffect();
    void setWolfBarrierHitEffect(dBgS_LinChk&);
    void setCutWaterDropEffect();
    void setWaterDropEffect();
    void setSwordUpColorEffect();
    void setSwordCutSplash();
    void setMetamorphoseEffectStartLink();
    void setMetamorphoseEffect();
    void setRunSplash();
    void resetFairyEffect();
    void setBottleEffect();
    void clearFirePointDamageEffect(int);
    void initFirePointDamageEffectAll();
    void initFirePointDamageEffect(cXyz const*, dCcD_GObjInf*);
    void setFirePointDamageEffect();
    void setFreezeEffect();
    void setWoodShieldBurnEffect();
    void clearWoodShieldBurnEffect();
    void setWoodShieldBurnOutEffect();
    void changeCommon();
    void changeWolf();
    void changeLink(int);
    void onWolfEyeUp();
    void offWolfEyeUp();
    BOOL wolfSenseTrigger();
    void setWolfSenceStatus(u8);
    BOOL wolfClawTrigger();
    void setWolfDigStatus(u8);
    BOOL checkWolfShapeReverse() const;
    BOOL checkWolfSideStep() const;
    void setWolfTailAngle();
    BOOL checkWolfSlowDash();
    void setSpeedAndAngleWolf();
    void setSpeedAndAngleWolfAtn();
    BOOL checkWolfAtnWait();
    BOOL checkUnderMove0BckNoArcWolf(daAlink_c::daAlink_WANM) const;
    void setBlendWolfMoveAnime(f32);
    BOOL checkWolfAtnMoveBack(s16);
    void setWolfAtnMoveDirection();
    void setBlendWolfAtnMoveAnime(f32);
    void setBlendWolfAtnBackMoveAnime(f32);
    int setDoubleAnimeWolf(f32, f32, f32, daAlink_c::daAlink_WANM,
                                          daAlink_c::daAlink_WANM, int, f32);
    void setSingleAnimeWolfBase(daAlink_c::daAlink_WANM);
    void setSingleAnimeWolfBaseMorf(daAlink_c::daAlink_WANM, f32);
    void setSingleAnimeWolfBaseSpeed(daAlink_c::daAlink_WANM, f32, f32);
    int setSingleAnimeWolf(daAlink_c::daAlink_WANM, f32, f32, s16, f32);
    void setSingleAnimeWolfParam(daAlink_c::daAlink_WANM, daAlinkHIO_anm_c const*);
    int checkWolfLandAction(int);
    BOOL checkMidnaUseAbility() const;
    void checkWolfUseAbility();
    int checkWolfGroundSpecialMode();
    int checkNextActionWolf(int);
    BOOL wolfSideBgCheck(s16);
    BOOL checkWolfAttackReverse(int);
    int checkWolfBarrierHitReverse();
    BOOL checkWolfBarrierWallHit(cBgS_PolyInfo&);
    void wolfBgCheck();
    void changeWolfBlendRate(int);
    void setWolfFootMatrix();
    void wolfFootBgCheck();
    BOOL checkWolfWaitSlipPolygon();
    f32 setWolfWaitSlip();
    void checkWolfAtnDoCharge();
    void setWolfChainPos();
    void setWolfAnmVoice();
    int procWolfServiceWaitInit(int);
    int procWolfServiceWait();
    int procWolfTiredWaitInit();
    int procWolfTiredWait();
    int procWolfMidnaRideShockInit();
    int procWolfMidnaRideShock();
    int procWolfWaitInit();
    int procWolfWait();
    int procWolfMoveInit();
    int procWolfMove();
    int procWolfDashInit();
    int procWolfDash();
    int procWolfDashReverseInit(int);
    int procWolfDashReverse();
    int procWolfAtnActorMoveInit();
    int procWolfAtnActorMove();
    int procWolfWaitTurnInit();
    int procWolfWaitTurn();
    int procWolfSideStepInit(int);
    int procWolfSideStep();
    int procWolfSideStepLandInit(s16);
    int procWolfSideStepLand();
    int procWolfBackJumpInit(int);
    int procWolfBackJump();
    int procWolfBackJumpLandInit(s16, s16);
    int procWolfBackJumpLand();
    int procWolfHowlInit(int);
    int procWolfHowl();
    int procWolfAutoJumpInit(int);
    int procWolfAutoJump();
    int procWolfFallInit(int, f32);
    int procWolfFall();
    int procWolfLandInit();
    int procWolfLand();
    int procWolfSitInit(int);
    int procWolfSit();
    int procWolfStepMoveInit();
    int procWolfStepMove();
    int procWolfSlipInit();
    int procWolfSlip();
    int procWolfSlipTurnInit(int);
    int procWolfSlipTurn();
    int procWolfSlipTurnLandInit();
    int procWolfSlipTurnLand();
    int procWolfSlideReadyInit(s16, int);
    int procWolfSlideReady();
    int procWolfSlideInit(s16, int);
    int procWolfSlide();
    int procWolfSlideLandInit();
    int procWolfSlideLand();
    int procWolfWaitSlipInit();
    int procWolfWaitSlip();
    int procWolfSlopeStartInit(int);
    int procWolfSlopeStart();
    void setWolfHowlNotHappen(int);
    int procWolfHowlDemoInit();
    int procWolfHowlDemo();
    fopAc_ac_c* checkWolfRopeHit(dCcD_GObjInf*, cXyz const*, int) const;
    int checkWolfRopeJumpHang();
    f32 getWolfRopeMoveSpeed();
    int setWolfRopePosY();
    s16 initWolfRopeShapeAngle();
    void wolfRopeSwingInc(f32);
    void setWolfRopeOffsetY(int);
    int getDirectionRopeMove() const;
    int procWolfRopeMoveInit(int, int);
    int procWolfRopeMove();
    int procWolfRopeHangInit(int);
    int procWolfRopeHang();
    int procWolfRopeTurnInit();
    int procWolfRopeTurn();
    int procWolfRopeStaggerInit(int);
    int procWolfRopeStagger();
    int procWolfRopeSubjectivityInit();
    int procWolfRopeSubjectivity();
    int getWolfTagJumpTime() const;
    const cXyz* checkMidnaLockJumpPoint() const;
    int procWolfTagJumpInit(fopAc_ac_c*);
    int procWolfTagJump();
    int procWolfTagJumpLandInit(fopAc_ac_c*);
    int procWolfTagJumpLand();
    int procWolfGiantPuzzleInit();
    int procWolfGiantPuzzle();
    void setWolfHangGroundY();
    int changeWolfHangEndProc();
    int procWolfHangReadyInit();
    int procWolfHangReady();
    int procWolfHangWallCatchInit(int);
    int procWolfHangWallCatch();
    int procWolfHangFallStartInit(cM3dGPla*);
    int procWolfHangFallStart();
    void setWolfHeadDamage();
    int procWolfDamageInit(dCcD_GObjInf*);
    int procWolfDamage();
    int procWolfLargeDamageUpInit(int, int, s16, s16);
    int procWolfLargeDamageUp();
    int procWolfLandDamageInit(int);
    int procWolfLandDamage();
    void setWolfScreamWaitAnime();
    int procWolfScreamWaitInit();
    int procWolfScreamWait();
    f32 getWolfLieMoveAnmSpeed();
    f32 getWolfLieMoveSpeed();
    BOOL checkMidnaDisappearMode() const;
    BOOL checkWolfLieContinue(int);
    int checkNextActionWolfFromLie();
    void setWolfLieMoveVoice(int);
    bool checkWolfLieCode();
    int procWolfLieStartInit(int);
    int procWolfLieStart();
    int procWolfLieMoveInit(int);
    int procWolfLieMove();
    int procWolfLieAutoMoveInit(int, cXyz*);
    int procWolfLieAutoMove();
    void setSpeedAndAngleSwimWolf();
    f32 getWolfSwimMoveAnmSpeed();
    int decideDoStatusSwimWolf();
    int procWolfSwimUpInit();
    int procWolfSwimUp();
    int procWolfSwimWaitInit(int);
    int procWolfSwimWait();
    int procWolfSwimMoveInit();
    int procWolfSwimMove();
    int procWolfSwimEndWaitInit(int);
    int procWolfSwimEndWait();
    void resetWolfEnemyBiteAll();
    void checkWolfEnemyThrowAction();
    void setWolfLockDomeModel();
    void setWolfBallModel();
    void resetWolfBallGrab();
    void checkWolfLockData();
    fopAc_ac_c* getWolfLockActorEnd();
    void searchWolfLockEnemy(fopAc_ac_c*, void*);
    void checkWolfComboCnt();
    BOOL checkWolfAttackAction();
    void setWolfEnemyThrowUpperAnime(daAlink_c::daAlink_WANM, f32);
    BOOL setWolfEnemyHangBitePos(fopEn_enemy_c*);
    void setWolfBiteDamage(fopEn_enemy_c*);
    BOOL checkWolfLockAttackChargeState();
    int procWolfRollAttackChargeInit();
    int procWolfRollAttackCharge();
    int procWolfRollAttackMoveInit();
    int procWolfRollAttackMove();
    int procWolfJumpAttackInit(int);
    int procWolfJumpAttack();
    int procWolfJumpAttackKickInit();
    int procWolfJumpAttackKick();
    int procWolfJumpAttackSlideLandInit(int, int, int);
    int procWolfJumpAttackSlideLand();
    int procWolfJumpAttackNormalLandInit(int);
    int procWolfJumpAttackNormalLand();
    int procWolfWaitAttackInit(int);
    int procWolfWaitAttack();
    int procWolfRollAttackInit(int, int);
    int procWolfRollAttack();
    int procWolfDownAttackInit();
    int procWolfDownAttack();
    int procWolfDownAtLandInit(fopEn_enemy_c*);
    int procWolfDownAtLand();
    int procWolfDownAtMissLandInit();
    int procWolfDownAtMissLand();
    int procWolfLockAttackInit(int);
    int procWolfLockAttack();
    int procWolfLockAttackTurnInit(int);
    int procWolfLockAttackTurn();
    int procWolfAttackReverseInit();
    int procWolfAttackReverse();
    int procWolfEnemyThrowInit(int);
    int procWolfEnemyThrow();
    int procWolfEnemyHangBiteInit();
    int procWolfEnemyHangBite();
    int procWolfGrabUpInit();
    int procWolfGrabUp();
    int procWolfGrabPutInit();
    int procWolfGrabPut();
    int procWolfGrabThrowInit();
    int procWolfGrabThrow();
    int procWolfPushInit();
    int procWolfPush();
    int procWolfCargoCarryInit();
    int procWolfCargoCarry();
    int procWolfChainUpInit();
    int procWolfChainUp();
    int procWolfGanonCatchInit();
    int procWolfGanonCatch();
    int procWolfChainReadyInit();
    int procWolfChainReady();
    int procWolfChainWaitInit();
    int procWolfChainWait();
    int procWolfDigInit();
    int procWolfDig();
    int procWolfDigThroughInit(int);
    int procWolfDigThrough();
    void setSmellSave();
    int procWolfGetSmellInit();
    int procWolfGetSmell();
    void setArcName(int);
    void setShieldArcName();
    void setOriginalHeap(JKRExpHeap**, u32);
    void setShieldChange();
    int loadModelDVD();
    void setShieldModel();
    int loadShieldModelDVD();
    void changeModelDataDirect(int);
    void changeModelDataDirectWolf(int);
    void initStatusWindow();
    void statusWindowExecute(cXyz const*, s16);
    void statusWindowDraw();
    void resetStatusWindow();
    fopAc_ac_c* getChainGrabActor() { return field_0x2844.getActor(); }
    bool checkCokkoGlide() const {
        return mProcID == PROC_AUTO_JUMP && mProcVar2.field_0x300c != 0;
    }
    BOOL checkCameraLargeDamage() const {
        return mProcID == PROC_LARGE_DAMAGE || mProcID == PROC_LARGE_DAMAGE_WALL ||
               mProcID == PROC_LARGE_DAMAGE_UP || mProcID == PROC_WOLF_LARGE_DAMAGE_UP;
    }
    const cXyz& getHsSubChainTopPos() const { return mIronBallBgChkPos; }
    BOOL checkCutHeadProc() const { return mProcID == PROC_CUT_HEAD; }
    fopAc_ac_c* getRideActor() { return mRideAcKeep.getActor(); }

    virtual bool checkAcceptDungeonWarpAlink(int) { return checkAcceptWarp(); }
    virtual daSpinner_c* getSpinnerActor() {
        daSpinner_c* spinnerActor;
        if (!checkSpinnerRide()) {
            spinnerActor = NULL;
        } else {
            spinnerActor = (daSpinner_c*)mRideAcKeep.getActor();
        }
        return (daSpinner_c*)spinnerActor;
    }
    virtual s16 getSumouCounter() const { return mProcVar2.field_0x300c; }
    virtual s16 checkSumouWithstand() const { return mProcVar3.field_0x300e; }
    virtual void setMidnaMsgNum(fopAc_ac_c* param_0, u16 pMsgNum) {
        mMidnaMsgNum = pMsgNum;
        mMidnaMsg = (daTagMmsg_c*)param_0;
    }
    virtual MtxP getModelMtx() { return mpLinkModel->getBaseTRMtx(); }
    virtual MtxP getInvMtx() { return mInvMtx; }
    virtual cXyz* getShadowTalkAtnPos() { return &field_0x375c; }
    virtual MtxP getLeftItemMatrix();
    virtual MtxP getRightItemMatrix();
    virtual MtxP getLeftHandMatrix();
    virtual MtxP getRightHandMatrix();
    virtual MtxP getLinkBackBone1Matrix() { return mpLinkModel->getAnmMtx(1); }
    virtual MtxP getWolfMouthMatrix() { return mpLinkModel->getAnmMtx(13); }
    virtual MtxP getWolfBackbone2Matrix() { return mpLinkModel->getAnmMtx(2); }
    virtual MtxP getBottleMtx() {
        if (mHeldItemModel != NULL && checkBottleItem(mEquipItem)) {
            return mHeldItemModel->getBaseTRMtx();
        }
        return NULL;
    }

    virtual MtxP getHeadMtx() {
        return mpLinkModel->getAnmMtx(field_0x30b4);
    }
    virtual f32 getGroundY() { return mLinkAcch.GetGroundH(); }
    virtual f32 getBaseAnimeFrameRate() const { return mUnderFrameCtrl[0].getRate(); }
    virtual fpc_ProcID getAtnActorID() const { return mAtnActorID; }
    virtual fpc_ProcID getItemID() const { return mItemAcKeep.getID(); }
    virtual fpc_ProcID getGrabActorID() const {
        if (mEquipItem == 0x102) {
            return mItemAcKeep.getID();
        } else {
            return mGrabItemAcKeep.getID();
        }
    }
    virtual void setForcePutPos(cXyz const& pPutPos) {
        mForcePutPos = pPutPos;
        onEndResetFlg1(ERFLG1_UNK_2000);
    }
    virtual BOOL checkPlayerGuard() const;
    virtual u32 checkPlayerFly() const {
        return checkModeFlg(MODE_SWIMMING | MODE_ROPE_WALK | MODE_VINE_CLIMB | MODE_UNK_800 |
                            MODE_NO_COLLISION | MODE_CLIMB | MODE_JUMP);
    }
    virtual BOOL checkFrontRoll() const { return mProcID == PROC_FRONT_ROLL; }
    virtual BOOL checkWolfDash() const { return mProcID == PROC_WOLF_DASH; }
    virtual BOOL checkAutoJump() const {
        return mProcID == PROC_AUTO_JUMP || mProcID == PROC_WOLF_AUTO_JUMP;
    }
    virtual bool checkSideStep() const {
        return (mProcID == PROC_SIDESTEP || mProcID == PROC_WOLF_SIDESTEP) &&
               mProcVar1.field_0x300a != 0;
    }
    virtual bool checkWolfTriggerJump() const {
        return mProcID == PROC_WOLF_SIDESTEP || mProcID == PROC_WOLF_JUMP_ATTACK;
    }
    virtual BOOL checkGuardBreakMode() const { return mProcID == PROC_GUARD_BREAK; }
    virtual bool checkLv3Slide() const {
        return mProcID == PROC_SLIDE && mProcVar3.field_0x300e != 0;
    }
    virtual bool checkWolfHowlDemoMode() const { return mProcID == PROC_WOLF_HOWL_DEMO; }
    virtual bool checkChainBlockPushPull();
    virtual BOOL checkElecDamage() const { return mProcID == PROC_ELEC_DAMAGE; }
    virtual BOOL checkEmptyBottleSwing() const {
        return mEquipItem == fpcNm_ITEM_EMPTY_BOTTLE && mProcID == PROC_BOTTLE_SWING;
    }
    virtual BOOL checkBottleSwingMode() const { return mProcID == PROC_BOTTLE_SWING; }
    virtual BOOL checkHawkWait() const { return mProcID == PROC_HAWK_SUBJECT; }
    virtual BOOL checkGoatThrow() const {
        return mProcID == PROC_GOAT_CATCH && mProcVar2.field_0x300c != 0;
    }
    virtual BOOL checkGoatThrowAfter() const {
        return mProcID == PROC_GOAT_CATCH && field_0x3478 > 0.0f;
    }
    virtual BOOL checkWolfTagLockJump() const {
        return mProcID == PROC_WOLF_TAG_JUMP && field_0x3198 == 0;
    }
    virtual BOOL checkWolfTagLockJumpLand() const { return mProcID == PROC_WOLF_TAG_JUMP_LAND; }
    virtual BOOL checkWolfRope();
    virtual BOOL checkWolfRopeHang() const { return mProcID == PROC_WOLF_ROPE_HANG; }
    virtual BOOL checkRollJump() const { return mProcID == PROC_ROLL_JUMP; }
    virtual BOOL checkGoronRideWait() const { return mProcID == PROC_GORON_RIDE_WAIT; }
    virtual BOOL checkWolfChain() const { return mProcID == PROC_WOLF_CHAIN_WAIT; }
    virtual BOOL checkWolfWait() const { return mProcID == PROC_WOLF_WAIT; }
    virtual BOOL checkWolfJumpAttack() const { return mProcID == PROC_WOLF_JUMP_ATTACK; }
    virtual BOOL checkWolfRSit() const { return mProcID == PROC_WOLF_SIT; }
    virtual BOOL checkBottleDrinkEnd() const {
        return mProcID == PROC_BOTTLE_DRINK && mProcVar3.field_0x300e != 0;
    }
    virtual BOOL checkWolfDig() const { return mProcID == PROC_WOLF_DIG; }
    virtual BOOL checkCutCharge() const { return mProcID == PROC_CUT_TURN_MOVE; }
    virtual BOOL checkCutTurnCharge() const {
        return mProcID == PROC_CUT_TURN_MOVE && mProcVar2.field_0x300c == 0;
    }
    virtual BOOL checkCutLargeJumpCharge() const {
        return mProcID == PROC_CUT_TURN_MOVE && mProcVar2.field_0x300c != 0;
    }
    virtual BOOL checkComboCutTurn() const {
        return mProcID == PROC_CUT_TURN && mComboCutCount != 0;
    }
    virtual BOOL checkClimbMove() const {
        return mProcID == PROC_CLIMB_MOVE_UPDOWN || mProcID == PROC_LADDER_MOVE ||
               mProcID == PROC_CLIMB_MOVE_SIDE;
    }
    virtual BOOL checkGrassWhistle() const { return mProcID == PROC_GRASS_WHISTLE_WAIT; }
    virtual BOOL checkBoarRun() const { return mProcID == PROC_BOAR_RUN; }
    virtual BOOL checkHorseRideNotReady() const {
        return checkHorseRide() && mProcID != PROC_HORSE_RIDE && mProcID != PROC_HORSE_GETOFF;
    }
    virtual f32 getSearchBallScale() const { return mSearchBallScale; }
    virtual int checkFastShotTime() { return mFastShotTime; }
    virtual f32 getBaseAnimeFrame() const;
    virtual void setAnimeFrame(f32);
    virtual BOOL checkWolfLock(fopAc_ac_c*) const;
    virtual bool cancelWolfLock(fopAc_ac_c*);
    virtual BOOL exchangeGrabActor(fopAc_ac_c*);
    virtual BOOL setForceGrab(fopAc_ac_c*, int, int);
    virtual u32 checkPlayerNoDraw();
    virtual void voiceStart(u32);
    virtual void seStartOnlyReverb(u32);
    virtual void seStartOnlyReverbLevel(u32);
    virtual void setOutPower(f32, short, int);
    virtual void setGrabCollisionOffset(f32, f32, cBgS_PolyInfo*);
    virtual void onFrollCrashFlg(u8, int);
    virtual MtxP getModelJointMtx(u16);
    virtual bool setHookshotCarryOffset(fpc_ProcID, cXyz const*);
    virtual BOOL checkCutJumpCancelTurn() const {
        return (mProcID == PROC_CUT_JUMP || mProcID == PROC_CUT_JUMP_LAND) && field_0x3198 != 2;
    }
    virtual bool checkIronBallReturn() const;
    virtual bool checkIronBallGroundStop() const;
    virtual BOOL checkSingleBoarBattleSecondBowReady() const {
        return mProcID == PROC_HORSE_BOW_SUBJECT && mProcVar2.field_0x300c != 0;
    }
    virtual void setClothesChange(int);
    virtual void setPlayerPosAndAngle(cXyz const*, short, int);
    virtual void setPlayerPosAndAngle(cXyz const*, csXyz const*);
    virtual void setPlayerPosAndAngle(f32 (*)[4]);
    virtual bool setThrowDamage(short, f32, f32, int, int, int);
    virtual bool checkSetNpcTks(cXyz*, int, int);
    virtual int setRollJump(f32, f32, short);
    virtual void cancelDungeonWarpReadyNeck() {
        if (mProcID != PROC_DUNGEON_WARP_READY) {
            return;
        }
        offModeFlg(MODE_UNK_100);
    }
    virtual void onSceneChangeArea(u8, u8, fopAc_ac_c*);
    virtual void onSceneChangeAreaJump(u8 exitID, u8 exitDirection, fopAc_ac_c* scexAc) {
        mExitID = exitID | 0x8000;
        mExitDirection = exitDirection;
        mpScnChg = (daScex_c*)scexAc;
    }
    virtual void onSceneChangeDead(u8 param_0, int param_1) {
        if (mProcID != PROC_DEAD) {
            return;
        }
        mProcVar5.field_0x3012 = param_0;
        field_0x3198 = param_1;
    }
    virtual u32 checkHorseRide() const;
    virtual u32 checkBoarRide() const;
    virtual u32 checkCanoeRide() const;
    virtual u32 checkBoardRide() const;
    virtual u32 checkSpinnerRide() const;
    virtual bool checkArrowChargeEnd() const;
    virtual bool checkNoEquipItem() const { return mEquipItem == fpcNm_ITEM_NONE; }
    virtual bool checkKandelaarSwing(int) const;
    virtual s16 getBoardCutTurnOffsetAngleY() const {
        if (mProcID == PROC_BOARD_CUT_TURN) {
            return mProcVar5.field_0x3012;
        }
        return 0;
    }
    virtual cXyz* getMagneBootsTopVec() { return &mMagneBootsTopVec; }
    virtual void setCargoCarry(fopAc_ac_c* p_actor) {
        mSpecialMode = SMODE_CARGO_CARRY;
        mCargoCarryAcKeep.setData(p_actor);
    }
    virtual void setGoronSideMove(fopAc_ac_c* p_actor) {
        mSpecialMode = SMODE_GORON_THROW;
        mCargoCarryAcKeep.setData(p_actor);
    }
    virtual void setSumouReady(fopAc_ac_c* p_actor) {
        mSpecialMode = SMODE_SUMO_READY;
        mCargoCarryAcKeep.setData(p_actor);
        mDemo.setDemoMode(1);
    }
    virtual void setSumouPushBackDirection(short param_0) {
        if (mProcID != PROC_SUMOU_MOVE) {
            return;
        }
        mProcVar4.field_0x3010 = param_0;
    }
    virtual void setSumouLoseHeadUp() {
        if (mProcID != PROC_SUMOU_WIN_LOSE) {
            return;
        }
        mSpecialMode = SMODE_SUMO_LOSE;
    }
    virtual cXyz* getKandelaarFlamePos();
    virtual bool checkUseKandelaar(int);
    virtual void setDkCaught(fopAc_ac_c*);
    virtual void onPressedDamage(cXyz const&, short);
    virtual bool checkPriActorOwn(fopAc_ac_c const* p_actor) const {
        return field_0x27f4 == p_actor;
    }
    virtual bool onWolfEnemyBiteAll(fopAc_ac_c*, daPy_FLG2);
    virtual bool checkWolfEnemyBiteAllOwn(fopAc_ac_c const* p_actor) const {
        return field_0x281c.getActorConst() == p_actor;
    }
    virtual void setWolfEnemyHangBiteAngle(short angle) {
        if (mProcID != PROC_WOLF_ENEMY_HANG_BITE) {
            return;
        }
        mProcVar4.field_0x3010 = angle;
    }
    virtual void setSumouGraspCancelCount(int param_0) {
        if (mProcID != PROC_SUMOU_MOVE) {
            return;
        }
        mProcVar2.field_0x300c = param_0;
    }
    virtual bool checkItemSwordEquip() const { return mEquipItem == 0x103; }
    virtual f32 getSinkShapeOffset() const { return mSinkShapeOffset; }
    virtual BOOL checkSinkDead() const { return field_0x2fbd == 0xFF; }
    virtual BOOL checkCutJumpMode() const { return mProcID == PROC_CUT_JUMP; }
    virtual s16 getGiantPuzzleAimAngle() const { return mProcVar2.mPuzzleAimAngle; }
    virtual u8 getSwordChangeWaitTimer() const { return mSwordChangeWaitTimer; }
    virtual BOOL checkMetamorphose() const {
        return mProcID == PROC_METAMORPHOSE && mProcVar1.field_0x300a == 0;
    }
    virtual BOOL checkWolfDownAttackPullOut() const { return mProcID == PROC_WOLF_DOWN_AT_LAND; }
    virtual cXyz* getMidnaAtnPos() const { return (cXyz*)&mMidnaAtnPos; }
    virtual bool checkCopyRodEquip() const { return mEquipItem == fpcNm_ITEM_COPY_ROD; }
    virtual void setKandelaarMtx(f32 (*)[4], int, int);
    virtual bool getStickAngleFromPlayerShape(short*) const;
    virtual bool checkSpinnerPathMove();
    virtual bool checkSpinnerTriggerAttack();
    virtual void onSpinnerPathForceRemove();
    virtual int getIronBallBgHit() const;
    virtual cXyz* getIronBallCenterPos();
    virtual bool checkCanoeFishingGetLeft() const {
        return mProcID == PROC_CANOE_FISHING_GET && mProcVar3.field_0x300e == 0;
    }
    virtual bool checkCanoeFishingGetRight() const {
        return mProcID == PROC_CANOE_FISHING_GET && mProcVar3.field_0x300e == 1;
    }
    virtual u8 checkBeeChildDrink() const { return field_0x2fd3; }
    virtual Z2WolfHowlMgr* getWolfHowlMgrP() { return &mZ2WolfHowlMgr; }
    virtual BOOL checkWolfHowlSuccessAnime() const {
        return checkUnderMove0BckNoArcWolf(WANM_HOWL_SUCCESS);
    }
    virtual void skipPortalObjWarp();
    virtual BOOL checkTreasureRupeeReturn(int) const;
    virtual void cancelGoronThrowEvent();
    virtual cXyz* getHookshotTopPos();
    virtual bool checkHookshotReturnMode() const;
    virtual bool checkHookshotShootReturnMode() const;
    virtual bool checkOctaIealHang() const {
        return mProcID == PROC_BOSS_BODY_HANG && field_0x32cc == 0;
    }
    virtual void cancelOctaIealHang() {
        if (mProcID != PROC_BOSS_BODY_HANG) {
            return;
        }
        mProcVar3.field_0x300e = 1;
    }
    virtual void cancelDragonHangBackJump() {
        if (mProcID != PROC_BOSS_BODY_HANG) {
            return;
        }
        mProcVar3.field_0x300e = -1;
    }
    virtual void setOctaIealWildHang() {
        if (mProcID != PROC_BOSS_BODY_HANG) {
            return;
        }
        mProcVar0.field_0x3008 = 1;
    }
    virtual bool checkDragonHangRide() const {
        return mProcID == PROC_BOSS_BODY_HANG && field_0x32cc != 0;
    }
    virtual void playerStartCollisionSE(u32 param_0, u32 param_1) {
        mZ2Link.startCollisionSE(param_0, param_1);
    }
    virtual void changeDragonActor(fopAc_ac_c*);
    virtual u8 getClothesChangeWaitTimer() const { return mClothesChangeWaitTimer; }
    virtual u8 getShieldChangeWaitTimer() const { return mShieldChangeWaitTimer; }
    virtual BOOL checkBootsOrArmorHeavy() const;
    virtual fpc_ProcID getBottleOpenAppearItem() const;
    virtual BOOL checkHorseStart() { return checkHorseStart(getLastSceneMode(), getStartMode()); }
    virtual BOOL checkCopyRodTopUse();

    static BOOL checkDebugMoveInput() {
        if (mDoCPd_c::isConnect(PAD_3)) {
            return mDoCPd_c::getHoldB(PAD_1) && mDoCPd_c::getAnalogR(PAD_1) > 0.8f &&
                   mDoCPd_c::getTrigA(PAD_1);
        }

        return FALSE;
    }

    u32 checkModeFlg(u32 pFlag) const { return mModeFlg & pFlag; }
    BOOL checkSmallUpperGuardAnime() const { return checkUpperAnime(0x16); }
    BOOL checkFmChainGrabAnime() const { return checkUpperAnime(0x62) || checkUpperAnime(0x2A0); }

    BOOL checkAttentionLock() { return mAttention->Lockon(); }

    bool checkUpperAnime(u16 i_idx) const { return mUpperAnmHeap[UPPER_2].getIdx() == i_idx; }
    bool checkUnderAnime(u16 i_idx) const { return mUnderAnmHeap[UNDER_2].getIdx() == i_idx; }

    bool checkNoSetUpperAnime() const { return mUpperAnmHeap[UPPER_2].checkNoSetIdx(); }
    bool checkSwimMoveHandAnime() const { return checkUpperAnime(0x23F); }
    bool checkZoraSwimDamageAnime() const { return checkUpperAnime(0xBF); }
    bool checkIronBallWaitAnime() const { return checkUpperAnime(0x19C); }
    bool checkHorseTurnLAnime() const { return checkUpperAnime(0x24C); }
    bool checkHorseTurnRAnime() const { return checkUpperAnime(0x24D); }
    bool checkHorseTurnAnime() const { return checkHorseTurnLAnime() || checkHorseTurnRAnime(); }
    bool checkHookshotShootAnime() const { return checkUpperAnime(dRes_ID_ALANM_BCK_HSSHOOT_e); }
    bool checkHookshotReadyAnime() const { return checkUpperAnime(dRes_ID_ALANM_BCK_HSWAIT_e); }
    BOOL checkHookshotAnime() const {
        return checkHookshotReadyAnime() || checkHookshotShootAnime();
    }
    BOOL checkBoomerangReadyAnime() const {
        return (mEquipItem == fpcNm_ITEM_BOOMERANG || mEquipItem == 0x102) && checkUpperAnime(0x54);
    }
    bool checkDkCaught2Anime() const { return checkUpperAnime(0x262); }
    BOOL checkCopyRodThrowAnime() const {
        return mEquipItem == fpcNm_ITEM_COPY_ROD && checkUpperAnime(0x53);
    }
    BOOL checkCutDashChargeAnime() const { return checkUpperAnime(0x83); }
    BOOL checkBoomerangAnimeAndReturnWait() const { return checkBoomerangAnime(); }
    BOOL checkTwoHandItemEquipAnime() const { return checkUpperAnime(0x245); }
    BOOL checkBarkAnime() const { return 0; }
    bool checkWolfGrabAnimeObj() const { return checkUpperAnime(0x2DA); }
    bool checkWolfGrabAnimeStick() const { return checkUpperAnime(0x2DB); }
    BOOL checkWolfGrabAnime() const { return checkWolfGrabAnimeObj() || checkWolfGrabAnimeStick(); }
    bool checkWolfSwimDashAnime() const { return checkUnderMove0BckNoArcWolf(WANM_SWIM_DASH); }
    bool checkKandelaarSwingAnime() const { return false; }
    bool checkBowChargeWaitAnime() const { return checkUpperAnime(0xA); }
    bool checkBowReloadAnime() const { return checkUpperAnime(0x9); }
    bool checkBowShootAnime() const { return checkUpperAnime(0xC); }
    bool checkBowWaitAnime() const { return checkUpperAnime(0xD); }
    BOOL checkGrabUpThrowAnime() const { return checkUpperAnime(0x170); }
    BOOL checkGrabSideThrowAnime() const {
        return checkUpperAnime(0x51) || mUpperAnmHeap[0].getIdx() == 0x51;
    }
    BOOL checkGrabHeavyThrowAnime() const { return checkUpperAnime(0x17B); }
    BOOL checkGrabAnimeUp() const { return checkUpperAnime(0x16C); }
    BOOL checkGrabAnimeSide() const { return checkUpperAnime(0x50); }
    BOOL checkGrabAnimeCarry() const { return checkUpperAnime(0x60); }
    bool checkHorseUnderDashStartAnime() const { return checkUnderAnime(0xDD); }
    bool checkHorseUnderLashAnime() const { return checkUnderAnime(0x1B3); }
    bool checkUpperHorseLieAnime() const { return checkUpperAnime(0x265); }
    bool checkHorseTiredAnime() const { return checkUpperAnime(0x263); }
    bool checkHorseSwordUpAnime() const { return checkUpperAnime(0xCC); }
    bool checkHorseUnderItemAnime() const { return checkUnderAnime(0x19); }
    BOOL checkHorseWalkStartAnm() const { return checkUnderMove0BckNoArc(ANM_WSTARTH); }
    bool checkIronBallPreSwingAnime() const { return checkUpperAnime(0x19A); }
    bool checkIronBallSwingAnime() const { return checkUpperAnime(0x19B); }
    bool checkDashDamageAnime() const { return checkUpperAnime(0xAD); }
    bool checkBoomerangCatchAnime() const { return checkUpperAnime(0x52); }
    BOOL checkCopyRodReadyAnime() const {
        return mEquipItem == fpcNm_ITEM_COPY_ROD && checkUpperAnime(0x54);
    }
    BOOL checkCanoeFishingWaitAnime() const {
        return checkUpperAnime(0x5D) || checkUpperAnime(0x260);
    }
    BOOL checkCopyRodControllAnime() const { return checkUpperAnime(0x202); }
    BOOL checkWolfHeadDamageAnime() const { return checkUpperAnime(0x2A7); }
    BOOL checkExchangeRodAnime() const { return checkUpperAnime(0x68); }
    BOOL checkReelAnime() const { return checkUpperAnime(0x1FB); }

    int checkWolfEyeUp() const { return mWolfEyeUp; }
    void onModeFlg(u32 flag) { mModeFlg |= flag; }
    void offModeFlg(u32 flag) { mModeFlg &= ~flag; }
    bool swordButton() { return itemButtonCheck(8); }
    daPy_actorKeep_c* getThrowBoomerangAcKeep() { return &mThrowBoomerangAcKeep; }
    int getStartRoomNo() { return fopAcM_GetParam(this) & 0x3F; }
    bool checkFisingRodLure() const { return mEquipItem == 0x105; }
    BOOL doTrigger() const { return mItemTrigger & BTN_A; }
    bool swordTrigger() { return itemTriggerCheck(BTN_B); }
    BOOL grassCancelTrigger() { return itemTriggerCheck(BTN_B); }
    BOOL arrowChangeTrigger() { return itemActionTrigger(); }
    bool peepSubjectCancelTrigger() { return itemTriggerCheck(0x8); }
    int getStartMode() { return (fopAcM_GetParam(this) >> 0xC) & 0x1F; }
    bool checkInputOnR() const { return field_0x33ac > 0.05f; }
    static int getSightBti() { return 0x5B; }
    bool checkBoomerangChargeEndWait() const {
        return mEquipItem != 0x102 && checkBoomerangAnime();
    }
    J3DAnmTransform* getNowAnmPackUpper(daAlink_UPPER param_0) {
        return mNowAnmPackUpper[param_0].getAnmTransform();
    }
    void setFacePriBck(u16 param_0) { setFaceBck(param_0, 1, 0xFFFF); }
    void setFacePriBtp(u16 param_0) { setFaceBtp(param_0, 1, 0xFFFF); }
    void setFacePriBtk(u16 param_0) { setFaceBtk(param_0, 1, 0xFFFF); }
    void setFaceBasicBck(u16 param_0) { setFaceBck(param_0, 0, 0xFFFF); }
    void setFaceBasicBtp(u16 param_0) { setFaceBtp(param_0, 0, 0xFFFF); }
    void setFaceBasicBtk(u16 param_0) { setFaceBtk(param_0, 0, 0xFFFF); }
    void setFaceDemoBck(u16 param_0) { setFaceBck(param_0, 0, 0); }
    void setFaceDemoBtp(u16 param_0) { setFaceBtp(param_0, 0, 0); }
    void setFaceDemoBtk(u16 param_0) { setFaceBtk(param_0, 0, 0); }

    void setGrabStatus(u8 i_status, u8 param_1) { setWallGrabStatus(i_status, param_1); }
    void setChainGrabStatus(u8 i_status) { setGrabStatus(i_status, 2); }

    void cancelFmChainGrabFromOut() {
        field_0x2fa3 = 0;
        field_0x2844.clearData();
    }

    int checkPlayerDemoMode() const { return mDemo.getDemoType(); }
    BOOL checkSpecialDemoMode() const {
        return mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_SPECIAL_e;
    }
    static bool checkMidnaChargeAttack() { return dComIfGs_isEventBit(0x501); }
    u16 getMidnaMsgNum() const { return mMidnaMsgNum; }
    u32 getStartEvent() { return fopAcM_GetParam(this) >> 0x18; }
    BOOL checkClimbFall() { return checkLadderFall(); }

    BOOL checkMidnaWolfDashAnime() const { return checkNoResetFlg1(FLG1_DASH_MODE); }
    BOOL checkMidnaClingAnime() const { return mMidnaAnm == 1; }
    BOOL checkMidnaLowClingAnime() const { return mMidnaAnm == 2; }
    BOOL checkMidnaLookAroundAnime() const { return mMidnaAnm == 3; }
    BOOL checkMidnaPanicAnime() const { return mMidnaAnm == 5; }
    BOOL checkMidnaWolfDeadAnime() const { return mMidnaAnm == 6; }
    BOOL checkMidnaWolfSwimDeadAnime() const { return mMidnaAnm == 7; }
    BOOL checkMidnaRopeWaitStaggerAnime() const { return mMidnaAnm == 8; }
    BOOL checkMidnaRopeMoveStaggerAnime() const { return mMidnaAnm == 9; }
    BOOL checkMidnaGanonCatchAnm() const { return mMidnaAnm == 10; }
    BOOL checkMidnaGanonThrowLeftAnm() const { return mMidnaAnm == 11; }
    BOOL checkMidnaGanonThrowRightAnm() const { return mMidnaAnm == 12; }
    BOOL checkMidnaDigInAnime() const { return mMidnaAnm == 13; }

    void clearMidnaMsgNum() {
        mMidnaMsgNum = 0xffff;
        mMidnaMsg = NULL;
    }

    void setEndGanonThrow() { mProcVar3.field_0x300e = 1; }

    s16 getProcNeckX() { return field_0x30a0; }
    s16 getMidnaProcNeckY() { return field_0x30d4; }
    f32 getEyeMoveRateY() { return field_0x33f4; }
    f32 getMidnaEyeMoveRateX() { return field_0x33f8; }

    J3DModel* getMidnaModel() {
        if (mClothesChangeWaitTimer != 0) {
            return NULL;
        } else {
            return mpWlMidnaModel;
        }
    }

    J3DModel* getMidnaMaskModel() { return mpWlMidnaMaskModel; }
    J3DModel* getMidnaHandModel() { return mpWlMidnaHandModel; }
    J3DModel* getMidnaHairHandModel() { return mpWlMidnaHairModel; }

    cXyz* getMidnaHairAtnPos() { return &mMidnaHairAtnPos; }

    const daAlink_AnmData* getAnmData(daAlink_ANM anmID) const { return &m_anmDataTable[anmID]; }
    const daAlink_FaceTexData* getFaceTexData(daAlink_FTANM i_anmID) const {
        return &m_faceTexDataTable[i_anmID];
    }

    s16 getCameraAngleX() const { return field_0x310a; }
    s16 getCameraAngleY() const { return field_0x310c; }
    cXyz* getSubjectEyePos() { return &field_0x3768; }

    u32 checkReinRide() const { return mRideStatus == 1 || mRideStatus == 2; }
    int getGrassHowlEventActor() const { return field_0x3198; }
    MtxP getShieldMtx() const { return mShieldModel->getBaseTRMtx(); }
    MtxP getMagneBootsMtx() { return mMagneBootMtx; }
    MtxP getMagneBootsInvMtx() { return mMagneBootInvMtx; }
    s16 getMagneBootsModelShapeAngle() const { return field_0x3118; }

    BOOL checkFishingCastMode() const {
        return (mProcID == PROC_FISHING_CAST && !(mItemAcKeep.getActor() != NULL && mItemAcKeep.getActor()->eventInfo.checkCommandDemoAccrpt())) ||
               (mProcID != PROC_FISHING_CAST && checkNoResetFlg2(FLG2_UNK_20000000) != 0);
    }

    BOOL setCanoeCast() {
        if (mProcVar2.field_0x300c == 0 && checkCanoeFishingWaitAnime()) {
            mProcVar2.field_0x300c = 1;
            return TRUE;
        } else {
            return FALSE;
        }
    }

    bool checkFishingRodGrabLeft() const { return mItemVar0.field_0x3018 == 0; }

    void setCanoeFishingWaitAngle(s16 i_angle) { field_0x311a = i_angle; }

    void setFishingArnmAngle(s16 i_angle) { field_0x3160.set(0, 0, i_angle); }
    void setFishingArm1Angle(const csXyz& i_angle) { mFishingArm1Angle = i_angle; }
    void setFishingArm2Angle(const csXyz& i_angle) { field_0x3160 = i_angle; }

    void endFishingCastWait() { offNoResetFlg2(FLG2_UNK_20000000); }

    void startFishingCastWait() {
        if (checkFishingRodItem(mEquipItem)) {
            onNoResetFlg2(FLG2_UNK_20000000);
        }
    }

    f32 getFishingReelFrame() const { return mUpperFrameCtrl[2].getFrame(); }

    void changeFishGetFace(u8 param_0) { field_0x2fc8 = param_0; }

    BOOL checkSlideMode() {
        return mProcID == PROC_SLIDE || mProcID == PROC_SLIDE_LAND ||
               mProcID == PROC_WOLF_SLIDE_READY || mProcID == PROC_WOLF_SLIDE_LAND ||
               mProcID == PROC_WOLF_SLIDE;
    }

    BOOL checkHorseGetItem() const {
        return (mProcID == PROC_GET_ITEM && checkHorseRide()) || mProcID == PROC_HORSE_GET_KEY;
    }

    bool checkFisingRodJewl() const {
        return (mEquipItem == 0x5C || mEquipItem == 0x5F) || mEquipItem == 0x5E;
    }

    bool checkFisingRodWorm() const { return mItemMode == 0x74; }

    bool checkFisingRodBee() const { return mItemMode == 0x76; }

    void fishingCastWaitAnimeStart() {
        if (mProcID == PROC_FISHING_CAST) {
            mProcVar3.field_0x300e = 0;
        }
    }

    void fishingCastWaitAnimeStop() {
        if (mProcID == PROC_FISHING_CAST) {
            mProcVar3.field_0x300e = 1;
        }
    }

    MtxP getCopyRodMtx() {
        if (mHeldItemModel != NULL) {
            return mHeldItemModel->getAnmMtx(0);
        }
        return NULL;
    }

    cXyz* getRootPosP() { return &field_0x3834; }
    const cXyz& getBoomerangCatchPos() const { return field_0x3534; }
    f32 getCopyRodBallDisFlyMax() const { return mSearchBallScale; }

    void clearIronBallActor() { field_0x173c.SetActor(this); }
    BOOL checkCanoeRideOwn(const fopAc_ac_c* param_0) const {
        return checkCanoeRide() && mRideAcKeep.getActorConst() == param_0;
    }
    BOOL checkBoarRideOwn(const fopAc_ac_c* i_actorP) const {
        return checkBoarRide() && mRideAcKeep.getActorConst() == i_actorP;
    }
    bool checkWolfDashMode() const { return checkNoResetFlg1(FLG1_DASH_MODE); }
    bool checkWolfLieWaterIn() const { return mWaterY > current.pos.y + 20.5f; }

    BOOL checkPowerGloveGet() { return false; }

    J3DModel* initModel(J3DModelData* p_modelData, u32 param_1) {
        return initModel(p_modelData, 0x80000, param_1);
    }

    J3DModel* initModelEnv(J3DModelData* p_modelData, u32 param_1) {
        return initModel(p_modelData, 0, param_1);
    }

    bool checkResetRootMtx(int param_0) const { return field_0x2f90 != 0 && param_0 == 1; }

    bool checkGrabGlide() { return checkGrabRooster(); }

    bool checkCopyRodRevive() const { return mProcID == PROC_COPY_ROD_REVIVE; }
    bool checkHorseGetOffMode() const { return mProcID == PROC_HORSE_GETOFF; }
    bool checkHorseHangMode() const { return mProcID == PROC_HORSE_HANG; }
    bool checkHorseBackInput() const { return mProcID == PROC_HORSE_WAIT; }
    bool checkHorseRideOn() const { return mProcID == PROC_HORSE_RIDE; }
    bool checkHorseTurnMode() const { return mProcID == PROC_HORSE_TURN; }
    bool checkGrabUp() const { return mProcID == PROC_GRAB_UP; }
    bool checkSpinnerRideWait() const {
        return mProcID == PROC_SPINNER_WAIT && mProcVar2.field_0x300c == 0;
    }
    bool checkCopyRodSwingMode() { return mProcID == PROC_COPY_ROD_SWING; }
    bool checkCopyRodSwingModeInit() {
        return mProcID == PROC_COPY_ROD_SWING && mProcVar2.field_0x300c != 0;
    }
    float transAnimeMoveX() { return field_0x3464; }
    float transAnimeMoveZ() { return field_0x3468; }
    bool checkRoofSwitchHang() const { return mProcID == PROC_ROOF_SWITCH_HANG; }
    bool checkCanoeWait() const { return mProcID == PROC_CANOE_WAIT; }
    bool checkCanoeSubjectivity() const { return mProcID == PROC_CANOE_SUBJECTIVITY; }

    fopAc_ac_c* getCopyRodActor() { return mCopyRodAcKeep.getActor(); }
    fopAc_ac_c* getHookshotRoofWaitActor() { return mCargoCarryAcKeep.getActor(); }

    BOOL checkRideOn() const { return mRideStatus != 0; }

    bool checkSwimDashMode() const { return checkNoResetFlg1(FLG1_DASH_MODE); }

    bool talkTrigger() const { return mItemTrigger & BTN_A; }
    J3DAnmTransform* getNowAnmPackUnder(daAlink_UNDER i_idx) {
        return mNowAnmPackUnder[i_idx].getAnmTransform();
    }
    BOOL doButton() const { return mItemButton & BTN_A; }
    void setGrassCancelStatus(u8 param_0) { setBStatus(param_0); }

    void seStartSystem(u32 i_seID) { mDoAud_seStart(i_seID, NULL, 0, 0); }
    bool checkIronBallEquip() const { return mEquipItem == fpcNm_ITEM_IRONBALL; }
    BOOL checkFishingRodEquip() const { return checkFishingRodItem(mEquipItem); }

    f32 getAnmSpeedStickRate(f32 param_0, f32 param_1) const {
        return param_0 + (field_0x33a8 * (param_1 - param_0));
    }

    bool escapeTrigger() {
        field_0x2fae |= 8;
        return mItemTrigger & (BTN_A | BTN_B);
    }

    void clearComboReserb() { offNoResetFlg2(FLG2_UNK_2); }

    void setDamageColorTime() { mDamageColorTime = 32 - (mDamageTimer % 16); }

    BOOL setEnemyBomb(fopAc_ac_c* i_actor) { return exchangeGrabActor(i_actor); }

    void decrementBombCnt() {
        if (mActiveBombNum != 0) {
            mActiveBombNum--;
        }
    }

    void decrementInsectBombCnt() {
        if (field_0x2fcf != 0) {
            field_0x2fcf--;
        }
    }

    // Gets the cardinal direction of the Left Stick relative to player facing angle
    int getDirectionFromShapeAngle() const {
        return getDirectionFromAngle(field_0x2fe2 - shape_angle.y);
    }

    // Gets the cardinal direction of the Left Stick relative to player angle
    int getDirectionFromCurrentAngle() const {
        return getDirectionFromAngle(field_0x2fe2 - current.angle.y);
    }

    bool checkAnmEnd(daPy_frameCtrl_c* i_frameCtrl) { return i_frameCtrl->checkAnmEnd(); }

    BOOL checkCrawlWaterIn() { return mWaterY > current.pos.y + 15.5f; }

    MtxP getWolfMidnaMatrix() { return mpLinkModel->getAnmMtx(0x19); }
    s16 getIceDamageWaitTimer() const { return mIceDamageWaitTimer; }
    const cXyz& getWindSpeed() const { return mWindSpeed; }
    const cXyz& getHsChainTopPos() const { return mHookshotTopPos; }
    const cXyz& getHsChainRootPos() const { return mHeldItemRootPos; }
    const cXyz& getHsSubChainRootPos() const { return field_0x3810; }
    const csXyz& getHsAngle() const { return field_0x316c; }

    s16 getHookshotStopTime() const { return field_0x3026; }
    bool getHookshotLeft() const { return field_0x3020 == 0; }

    static u16 getBallModelIdx() { return 0x25; }
    static u16 getBallBtkIdx() { return 0x49; }
    static u16 getBallBrkIdx() { return 0x41; }

    bool checkRootTransClearMode() { return field_0x2f99 & 7; }
    bool checkRootTransZClearMode() { return field_0x2f99 & 4; }
    bool checkRootTransXClearMode() { return field_0x2f99 & 1; }
    bool checkRootTransYClearMode() { return field_0x2f99 & 2; }
    bool checkRootTransClearContinueMode() { return field_0x2f99 & 8; }
    s16 checkWindStoneHowl() { return mProcVar4.field_0x3010; }
    u8 getCorrectCurveID() { return mZ2WolfHowlMgr.getCorrectCurveID(); }
    u8 getCorrectLineNum() { return mZ2WolfHowlMgr.getCorrectLineNum(); }
    u32 getWolfHowlTimer() { return mZ2WolfHowlMgr.getTimer(); }
    s8 getOnLineNum() { return mZ2WolfHowlMgr.getOnLineNum(); }
    Z2WolfHowlLine getCorrectLine(u8 i_lineIndex) { return mZ2WolfHowlMgr.getCorrectLine(i_lineIndex); }
    void startWindStoneSound(s8 i_curveID, Vec* i_pos) { mZ2WolfHowlMgr.startWindStoneSound(i_curveID, i_pos); }
    J3DModelData* getItemModelData() { return mpItemModelData; }

    cXyz* getIronBallChainPos() const { return mIronBallChainPos; }
    csXyz* getIronBallChainAngle() const { return mIronBallChainAngle; }
    int getIronBallHandChainNum() const { return mItemMode; }
    const cXyz& getIronBallChainHandRootPos() const { return mHookshotTopPos; }

    fopAc_ac_c* getAtnActor() { return mTargetedActor; }

    void itemHitSE(u32 param_1, u32 param_2, Z2SoundObjBase* param_3) {
        mZ2Link.startHitItemSE(param_1, param_2, param_3, -1.0f);
    }

    BOOL checkStartFall() { return getStartMode() == 3; }

    u8 getBStatus() { return dComIfGp_getAStatus(); }
    void setRStatus(u8 param_0, u8 param_1) { dComIfGp_setRStatus(param_0, param_1); }

    BOOL checkWindSpeedMoveXZ() const { return mWindSpeed.abs2XZ() > 1.0f; }

    BOOL checkHorseRideReady() const { return checkHorseRideOn() || checkHorseGetOffMode(); }

    void getHorseReinCenterLeftPos(cXyz* o_pos) {
        mDoMtx_multVec(getLeftHandMatrix(), &m_handLeftInSidePos, o_pos);
    }

    void getHorseReinCenterRightPos(cXyz* o_pos) {
        mDoMtx_multVec(getRightHandMatrix(), &m_handRightInSidePos, o_pos);
    }

    cXyz* getLineTopPosP() { return mSight.getPosP(); }

    cXyz* getBoomerangLockPosP() { return &mHeldItemRootPos; }

    BOOL boomerangLockTrigger() { return itemActionTrigger(); }

    BOOL checkBoomerangLockTrigger() { return boomerangLockTrigger(); }

    BOOL checkDemoSpinnerKeep() const {
        return mDemo.getDemoMode() == 14 && mDemo.getParam0() == 1;
    }

    BOOL checkDemoSpinnerKeepSpeed() const {
        return mDemo.getDemoMode() == 14 && mDemo.getParam0() == 2;
    }

    bool checkSpinnerRideOwn(const fopAc_ac_c* param_0) {
        return checkSpinnerRide() && mRideAcKeep.getActorConst() == param_0;
    }

    BOOL checkSpinnerReady() const { return mProcID == PROC_SPINNER_READY; }

    BOOL checkGameOverWindow() const { return mProcID == PROC_DEAD && field_0x3478 < 0.01f; }

    f32 getSlideLimit() const { return field_0x3470; }

    bool checkWindSpeedOnXZ() const { return field_0x35b8.abs2XZ() > 1.0f; }

    inline void startRestartRoomFromOut(int, u32, int);

    u16 getReadyItem() { return dComIfGp_getSelectItem(mSelectItemId); }

    static u32 getOtherHeapSize() { return 0xF0A60; }

    static daAlink_BckData const m_mainBckShield[20];
    static daAlink_BckData const m_mainBckSword[5];
    static daAlink_BckData const m_mainBckFishing[28];
    static daAlink_AnmData const m_anmDataTable[414];
    static daAlink_WlAnmData const m_wlAnmDataTable[147];
    static daAlink_FaceTexData const m_faceTexDataTable[];
    static Vec const m_handLeftOutSidePos;
    static Vec const m_handRightOutSidePos;
    static Vec const m_handLeftInSidePos;
    static Vec const m_handRightInSidePos;

    static daAlink_procInitTable m_procInitTable[];
    static daAlink_procFunc m_demoInitTable[];
    static EffParamProc m_fEffParamProc[];

    /* 0x0062C */ request_of_phase_process_class mPhaseReq;
    /* 0x00634 */ const char* mArcName;
    /* 0x00638 */ JKRExpHeap* mpArcHeap;
    /* 0x0063C */ request_of_phase_process_class mShieldPhaseReq;
    /* 0x00644 */ const char* mShieldArcName;
    /* 0x00648 */ JKRExpHeap* mpShieldArcHeap;
    /* 0x0064C */ J3DModelData* field_0x064C;
    /* 0x00650 */ J3DModel* mpLinkModel;
    /* 0x00654 */ J3DModel* mpLinkFaceModel;
    /* 0x00658 */ J3DModel* mpLinkHatModel;
    /* 0x0065C */ J3DModel* mpLinkHandModel;
    /* 0x00660 */ J3DModel* mpSwAModel;        // Ordon Sword model
    /* 0x00664 */ J3DModel* mpSwASheathModel;  // Ordon Sword sheath model
    /* 0x00668 */ J3DModel* mpSwMModel;        // Master Sword model
    /* 0x0066C */ J3DModel* mpSwMSheathModel;  // Master Sword sheath model
    /* 0x00670 */ J3DModel* mWoodSwordModel;
    /* 0x00674 */ J3DModel* mpWlChangeModel;
    /* 0x00678 */ J3DModel* mShieldModel;
    /* 0x0067C */ J3DAnmTevRegKey* mMagicArmorBodyBrk;
    /* 0x00680 */ J3DAnmTevRegKey* mMagicArmorHeadBrk;
    /* 0x00684 */ J3DModel* mpDemoFCBlendModel;
    /* 0x00688 */ J3DModel* mpDemoFCTongueModel;
    /* 0x0068C */ J3DAnmTextureSRTKey* field_0x068c;
    /* 0x00690 */ mDoExt_bckAnm* mpDemoFCTmpBck;
    /* 0x00694 */ J3DDeformData* mpDemoFCTmpBls;
    /* 0x00698 */ J3DAnmCluster* field_0x0698;
    /* 0x0069C */ mDoExt_blkAnm* field_0x069c;
    /* 0x006A0 */ J3DModel* mpDemoHLTmpModel;
    /* 0x006A4 */ mDoExt_bckAnm* mpDemoHLTmpBck;
    /* 0x006A8 */ J3DModel* mpDemoHRTmpModel;
    /* 0x006AC */ mDoExt_bckAnm* mpDemoHRTmpBck;
    /* 0x006B0 */ mDoExt_bckAnm* mpDemoHDTmpBck;
    /* 0x006B4 */ J3DAnmTransform* field_0x06b4;
    /* 0x006B8 */ J3DModel* mSwordModel;
    /* 0x006BC */ J3DModel* mSheathModel;
    /* 0x006C0 */ J3DModelData* field_0x06c0;
    /* 0x006C4 */ J3DAnmTextureSRTKey* m_nSwordBtk;
    /* 0x006C8 */ J3DAnmTextureSRTKey* m_mSwordBtk;
    /* 0x006CC */ J3DAnmTevRegKey* m_mSwordBrk;
    /* 0x006D0 */ J3DShape* field_0x06d0;
    /* 0x006D4 */ J3DShape* field_0x06d4;
    /* 0x006D8 */ J3DShape* field_0x06d8;
    /* 0x006DC */ J3DShape* field_0x06dc;
    /* 0x006E0 */ J3DShape* field_0x06e0;
    /* 0x006E4 */ J3DShape* field_0x06e4;
    /* 0x006E8 */ J3DShape* field_0x06e8;
    /* 0x006EC */ J3DShape* field_0x06ec;
    /* 0x006F0 */ J3DShape* field_0x06f0;
    /* 0x006F4 */ J3DAnmTevRegKey* field_0x06f4;
    /* 0x006F8 */ J3DAnmTevRegKey* field_0x06f8;
    /* 0x006FC */ J3DModel* mpKanteraModel;
    /* 0x00700 */ J3DModel* mpKanteraGlowModel;
    /* 0x00704 */ J3DAnmTextureSRTKey* mpKanteraGlowBtk;
    /* 0x00708 */ J3DModel* mHeldItemModel;
    /* 0x0070C */ J3DModel* mpHookTipModel;  // related to held item
    /* 0x00710 */ J3DModel* field_0x0710;    // related to held item
    /* 0x00714 */ J3DModel* field_0x0714;    // related to held item
    /* 0x00718 */ J3DAnmTextureSRTKey* field_0x0718;
    /* 0x0071C */ J3DAnmTextureSRTKey* field_0x071c;
    /* 0x00720 */ J3DAnmTextureSRTKey* field_0x0720;
    /* 0x00724 */ J3DAnmTevRegKey* field_0x0724;
    /* 0x00728 */ J3DAnmBase* field_0x0728;
    /* 0x0072C */ J3DAnmTexPattern* field_0x072c;
    /* 0x00730 */ mDoExt_bckAnm mItemBck;
    /* 0x0074C */ mDoExt_bckAnm mHookTipBck;
    /* 0x00768 */ J3DModelData* mpItemModelData;
    /* 0x0076C */ Z2SoundObjSimple* mpHookSound;
    /* 0x00770 */ hsChainShape_c* mpHookChain;
    /* 0x00774 */ dBgS_AcchCir* field_0x0774;
    /* 0x00778 */ dBgS_ObjAcch* field_0x0778;
    /* 0x0077C */ dBgS_ObjLinChk* mpHookshotLinChk;
    /* 0x00780 */ dBgS_ObjLinChk* field_0x780;
    /* 0x00784 */ J3DModel* mpLinkBootModels[2];
    /* 0x0078C */ J3DModel* mpWlChainModels[4];
    /* 0x0079C */ J3DModel* mpWlMidnaModel;
    /* 0x007A0 */ J3DModel* mpWlMidnaMaskModel;
    /* 0x007A4 */ J3DModel* mpWlMidnaHandModel;
    /* 0x007A8 */ J3DModel* mpWlMidnaHairModel;
    /* 0x007AC */ J3DAnmTevRegKey* mpDMidnaBrk;
    /* 0x007B0 */ J3DAnmTevRegKey* mpDMidnaMaskBrk;
    /* 0x007B4 */ J3DAnmTevRegKey* mpDMidnaHandBrk;
    /* 0x007B8 */ J3DAnmTevRegKey* mpDMidnaHairHandBrk;
    /* 0x007BC */ mDoExt_bckAnm* m_sWindowBck;
    /* 0x007C0 */ void* mpWarpTexData;
    /* 0x007C4 */ daPy_actorKeep_c mWolfLockAcKeep[10];
    /* 0x00814 */ dCcD_Stts field_0x814;
    /* 0x00850 */ dCcD_Cyl field_0x850[3];
    /* 0x00C04 */ dCcD_Cyl field_0xC04[3];
    /* 0x00FB8 */ dCcD_Sph field_0xFB8;
    /* 0x010F0 */ dCcD_Cyl mAtCyl;
    /* 0x0122C */ dCcD_Cps mAtCps[3];
    /* 0x015F8 */ dCcD_Cps mGuardAtCps;
    /* 0x0173C */ dCcD_Stts field_0x173c;  // related to ball and chain
    /* 0x01778 */ dCcD_Sph field_0x1778;
    /* 0x018B0 */ dBgS_AcchCir mAcchCir[3];
    /* 0x01970 */ dBgS_LinkAcch mLinkAcch;
    /* 0x01B48 */ dBgS_LinkLinChk mLinkLinChk;
    /* 0x01BB8 */ dBgS_RopeLinChk mRopeLinChk;
    /* 0x01C28 */ dBgS_BoomerangLinChk mBoomerangLinChk;
    /* 0x01C98 */ dBgS_ArrowLinChk mArrowLinChk;
    /* 0x01D08 */ dBgS_LinkGndChk mLinkGndChk;
    /* 0x01D5C */ dBgS_LinkRoofChk mLinkRoofChk;
    /* 0x01DAC */ dBgS_LinkLinChk mMagneLineChk;
    /* 0x01E1C */ cBgS_PolyInfo mPolyInfo1;
    /* 0x01E2C */ cBgS_PolyInfo mPolyInfo2;
    /* 0x01E3C */ cBgS_PolyInfo mPolyInfo3;
    /* 0x01E4C */ cBgS_PolyInfo mPolyInfo4;
    /* 0x01E5C */ dBgS_ObjLinChk mObjLinChk;
    /* 0x01ECC */ dBgS_ObjGndChk mObjGndChk;
    /* 0x01F20 */ mDoExt_MtxCalcAnmBlendTbl* field_0x1f20;
    /* 0x01F24 */ mDoExt_MtxCalcAnmBlendTblOld* field_0x1f24;
    /* 0x01F28 */ mDoExt_AnmRatioPack mNowAnmPackUnder[3];
    /* 0x01F40 */ mDoExt_AnmRatioPack mNowAnmPackUpper[3];
    /* 0x01F58 */ daPy_anmHeap_c mUnderAnmHeap[3];
    /* 0x01F94 */ daPy_anmHeap_c mUpperAnmHeap[3];
    /* 0x01FD0 */ daPy_frameCtrl_c mUnderFrameCtrl[3];
    /* 0x02018 */ daPy_frameCtrl_c mUpperFrameCtrl[3];
    /* 0x02060 */ mDoExt_MtxCalcOldFrame* field_0x2060;
    /* 0x02064 */ daAlink_sight_c mSight;
#if PLATFORM_WII
    u8 unk_0x20f0[4];
#endif
    /* 0x020F0 */ daPy_anmHeap_c mAnmHeap3;
    /* 0x02104 */ daPy_anmHeap_c mAnmHeap4;
    /* 0x02118 */ daPy_anmHeap_c mFaceBtpHeap;
    /* 0x0212C */ daPy_anmHeap_c mFaceBtkHeap;
    /* 0x02140 */ daPy_anmHeap_c mFaceBckHeap;
    /* 0x02154 */ J3DAnmTexPattern* field_0x2154;
    /* 0x02158 */ J3DAnmTextureSRTKey* mpFaceBtk;
    /* 0x0215C */ daPy_frameCtrl_c* field_0x215c;
    /* 0x02160 */ u16* field_0x2160;
    /* 0x02164 */ mDoExt_bckAnm field_0x2164;
    /* 0x02180 */ daAlink_matAnm_c* field_0x2180[2];
    /* 0x02188 */ dEyeHL_c mEyeHL1;
    /* 0x0219C */ dEyeHL_c mEyeHL2;
    /* 0x021B0 */ daPy_anmHeap_c mItemHeap[2];
    /* 0x021D8 */ daPy_anmHeap_c mAnmHeap9;
    /* 0x021EC */ daAlinkHIO_c* mpHIO;
    /* 0x021F0 */ daAlink_blur_c m_swordBlur;
    /* 0x027C8 */ dAlink_bottleWaterPcallBack_c field_0x27c8;
    /* 0x027E0 */ dAttention_c* mAttention;
    /* 0x027E4 */ dAttList_c* mAttList;
    /* 0x027E8 */ dAttList_c* mAttList2;
    /* 0x027EC */ fopAc_ac_c* mTargetedActor;
    /* 0x027F0 */ fopAc_ac_c* field_0x27f0;
    /* 0x027F4 */ fopAc_ac_c* field_0x27f4;
    /* 0x027F8 */ fopAc_ac_c* field_0x27f8;
    /* 0x027FC */ fopAc_ac_c* field_0x27fc;
    /* 0x02800 */ daTagMmsg_c* mMidnaMsg;
    /* 0x02804 */ fopAc_ac_c* field_0x2804;
    /* 0x02808 */ daScex_c* mpScnChg;
    /* 0x0280C */ daPy_actorKeep_c field_0x280c;
    /* 0x02814 */ daPy_actorKeep_c mRideAcKeep;
    /* 0x0281C */ daPy_actorKeep_c field_0x281c;
    /* 0x02824 */ daPy_actorKeep_c mItemAcKeep;  // name might be wrong
    /* 0x0282C */ daPy_actorKeep_c mGrabItemAcKeep;
    /* 0x02834 */ daPy_actorKeep_c mThrowBoomerangAcKeep;
    /* 0x0283C */ daPy_actorKeep_c mCopyRodAcKeep;
    /* 0x02844 */ daPy_actorKeep_c field_0x2844;
    /* 0x0284C */ daPy_actorKeep_c mHookTargetAcKeep;
    /* 0x02854 */ daPy_actorKeep_c mCargoCarryAcKeep;
    /* 0x0285C */ daPy_actorKeep_c field_0x285c;
    /* 0x02864 */ dMsgFlow_c mMsgFlow;
    /* 0x028B0 */ fpc_ProcID mShieldArrowIDs[16];
    /* 0x028F0 */ fpc_ProcID mMsgClassID;
    /* 0x028F4 */ fpc_ProcID mAtnActorID;
    /* 0x028F8 */ fpc_ProcID field_0x28f8;
    /* 0x028FC */ fpc_ProcID field_0x28fc;
    /* 0x02900 */ fpc_ProcID field_0x2900;
    /* 0x02904 */ daAlink_footData_c mFootData1[2];
    /* 0x02A4C */ daAlink_footData_c mFootData2[2];
    /* 0x02B94 */ f32 field_0x2b94;
    /* 0x02B98 */ f32 field_0x2b98;
    /* 0x02B98 */ f32 field_0x2b9c;
    /* 0x02BA0 */ f32 field_0x2ba0;
    /* 0x02BA4 */ f32 field_0x2ba4;
    /* 0x02BA8 */ f32 mSinkShapeOffset;
    /* 0x02BAC */ f32 mAcchCirWallH[3];
    /* 0x02BB8 */ Mtx mInvMtx;
    /* 0x02BE8 */ Mtx field_0x2be8;
    /* 0x02C18 */ Mtx mRootMtx;
    /* 0x02C48 */ Mtx mMagneBootMtx;
    /* 0x02C78 */ Mtx mMagneBootInvMtx;
    /* 0x02CA8 */ Z2CreatureLink mZ2Link;
    /* 0x02D78 */ u8* field_0x2d78;
    /* 0x02D7C */ daPy_frameCtrl_c* field_0x2d7c;
    /* 0x02D80 */ void* field_0x2d80;
    /* 0x02D84 */ Z2WolfHowlMgr mZ2WolfHowlMgr;
    /* 0x02E44 */ dJntCol_c field_0x2e44;
    /* 0x02E54 */ dPaPoF_c field_0x2e54;
    /* 0x02F1C */ dPaPo_c* field_0x2f1c;
    /* 0x02F20 */ dPa_hermiteEcallBack_c field_0x2f20;
    /* 0x02F38 */ dPa_hermiteEcallBack_c field_0x2f38;
    /* 0x02F50 */ const Vec* field_0x2f50;
    /* 0x02F54 */ const Vec* field_0x2f54;
    /* 0x02F58 */ dPath* field_0x2f58;
    /* 0x02F5C */ LIGHT_INFLUENCE field_0x2f5c;
    /* 0x02F7C */ u8 field_0x2f7c[16];
    /* 0x02F8C */ u8 field_0x2f8c;
    /* 0x02F8D */ u8 mItemTrigger;
    /* 0x02F8E */ u8 mItemButton;
    /* 0x02F8F */ u8 field_0x2f8f;
    /* 0x02F90 */ u8 field_0x2f90;
    /* 0x02F91 */ u8 field_0x2f91;
    /* 0x02F92 */ u8 field_0x2f92;
    /* 0x02F93 */ u8 field_0x2f93;
    /* 0x02F94 */ u8 field_0x2f94;
    /* 0x02F95 */ u8 field_0x2f95;
    /* 0x02F96 */ u8 field_0x2f96;
    /* 0x02F97 */ u8 field_0x2f97;
    /* 0x02F98 */ u8 field_0x2f98;
    /* 0x02F99 */ u8 field_0x2f99;
    /* 0x02F9A */ s8 mVoiceReverbIntensity;
    /* 0x02F9B */ u8 mMidnaAnm;
    /* 0x02F9C */ u8 mSelectItemId;
    /* 0x02F9D */ u8 field_0x2f9d;
    /* 0x02F9E */ u8 mEffProc;
    /* 0x02F9F */ u8 field_0x2f9f;
    /* 0x02FA0 */ u8 field_0x2fa0;
    /* 0x02FA1 */ u8 mRunCutComboCount;
    /* 0x02FA2 */ u8 mActiveBombNum;
    /* 0x02FA3 */ u8 field_0x2fa3;
    /* 0x02FA4 */ u8 mRollCrashFlg;
    /* 0x02FA5 */ u8 mPolySound;
    /* 0x02FA6 */ u8 field_0x2fa6;
    /* 0x02FA7 */ u8 field_0x2fa7;
    /* 0x02FA8 */ u8 mGndPolySpecialCode;
    /* 0x02FA9 */ u8 mWolfEyeUpTimer;
    /* 0x02FAA */ u8 mRideStatus;
    /* 0x02FAB */ u8 field_0x2fab;
    /* 0x02FAC */ u8 mExitDirection;
    /* 0x02FAD */ u8 mPeepExitID;
    /* 0x02FAE */ u8 field_0x2fae;
    /* 0x02FAF */ u8 field_0x2faf;
    /* 0x02FB0 */ u8 field_0x2fb0;
    /* 0x02FB1 */ u8 mWolfLockNum;
    /* 0x02FB2 */ u8 mMidnaTalkDelayTimer;
    /* 0x02FB3 */ u8 field_0x2fb3;
    /* 0x02FB4 */ u8 field_0x2fb4;
    /* 0x02FB5 */ u8 mDamageColorTime;
    /* 0x02FB6 */ u8 field_0x2fb6;
    /* 0x02FB7 */ u8 field_0x2fb7;
    /* 0x02FB8 */ u8 field_0x2fb8;
    /* 0x02FB9 */ u8 field_0x2fb9;
    /* 0x02FBA */ u8 mHotspringRecoverTimer;
    /* 0x02FBB */ u8 field_0x2fbb;
    /* 0x02FBC */ u8 field_0x2fbc;
    /* 0x02FBD */ u8 field_0x2fbd;
    /* 0x02FBE */ u8 field_0x2fbe;
    /* 0x02FBF */ u8 mClothesChangeWaitTimer;
    /* 0x02FC0 */ u8 field_0x2fc0;
    /* 0x02FC1 */ u8 field_0x2fc1;
    /* 0x02FC2 */ u8 field_0x2fc2;
    /* 0x02FC3 */ u8 field_0x2fc3;
    /* 0x02FC4 */ u8 field_0x2fc4;
    /* 0x02FC5 */ u8 field_0x2fc5;
    /* 0x02FC6 */ u8 field_0x2fc6;
    /* 0x02FC7 */ u8 field_0x2fc7;
    /* 0x02FC8 */ u8 field_0x2fc8;
    /* 0x02FC9 */ u8 field_0x2fc9;
    /* 0x02FCA */ u8 field_0x2fca;
    /* 0x02FCB */ u8 field_0x2fcb;
    /* 0x02FCC */ u8 field_0x2fcc;
    /* 0x02FCD */ u8 field_0x2fcd;
    /* 0x02FCE */ u8 field_0x2fce;
    /* 0x02FCF */ u8 field_0x2fcf;
    /* 0x02FD0 */ u8 field_0x2fd0;
    /* 0x02FD1 */ u8 mShieldChangeWaitTimer;
    /* 0x02FD2 */ u8 field_0x2fd2;
    /* 0x02FD3 */ u8 field_0x2fd3;
    /* 0x02FD4 */ u8 field_0x2fd4;
    /* 0x02FD5 */ u8 field_0x2fd5;
    /* 0x02FD6 */ u8 mSwordChangeWaitTimer;
    /* 0x02FD7 */ u8 field_0x2fd7;
    /* 0x02FD8 */ u8 field_0x2fd8;
    /* 0x02FD9 */ u8 field_0x2fd9[3];
    /* 0x02FDC */ u16 mEquipItem;
    /* 0x02FDE */ u16 field_0x2fde;
    /* 0x02FE0 */ s16 mStickAngle;
    /* 0x02FE2 */ s16 field_0x2fe2;  // related to current stick angle
    /* 0x02FE4 */ s16 field_0x2fe4;
    /* 0x02FE6 */ s16 field_0x2fe6;
    /* 0x02FE8 */ u16 mProcID;
    /* 0x02FEA */ u16 field_0x2fea;
    /* 0x02FEC */ s16 field_0x2fec;
    /* 0x02FEE */ s16 field_0x2fee;
    /* 0x02FF0 */ s16 field_0x2ff0;
    /* 0x02FF2 */ s16 field_0x2ff2;
    /* 0x02FF4 */ s16 field_0x2ff4;
    /* 0x02FF6 */ s16 field_0x2ff6;
    /* 0x02FF8 */ s16 field_0x2ff8;
    /* 0x02FFA */ s16 field_0x2ffa;
    /* 0x02FFC */ s16 field_0x2ffc;
    /* 0x02FFE */ s16 field_0x2ffe;
    /* 0x03000 */ s16 field_0x3000;
    /* 0x03002 */ s16 field_0x3002;
    /* 0x03004 */ s16 field_0x3004;
    /* 0x03006 */ s16 mIceDamageWaitTimer;
    // `mProcVar`'s are variables that are context dependent for each `PROC` action.
    // (The exact setup may need to be simplified later)
    union {
        s16 field_0x3008;
        s16 mHowlExitID;
        s16 mIceFreezeTimer;
    } /* 0x03008 */ mProcVar0;
    union {
        s16 field_0x300a;
        s16 mBoardSwordChargeTime;
        s16 mDigActorName;
    } /* 0x0300A */ mProcVar1;
    union {
        s16 field_0x300c;
        s16 mPuzzleAimAngle;
        s16 mDigType;
    } /* 0x0300C */ mProcVar2;
    union {
        s16 field_0x300e;
        s16 mDigChangeArea;
    } /* 0x0300E */ mProcVar3;
    union {
        s16 field_0x3010;
        s16 mDigExitID;
    } /* 0x03010 */ mProcVar4;
    union {
        s16 field_0x3012;
    } /* 0x03012 */ mProcVar5;
    /* 0x03014 */ s16 mFallVoiceInit;
    /* 0x03016 */ u8 field_0x3016[2];
    union {
        s16 field_0x3018;
        s16 mBottleItemHealAmount;
    } /* 0x03018 */ mItemVar0;
    /* 0x0301A */ s16 mItemMode;
    /* 0x0301C */ s16 field_0x301c;
    /* 0x0301E */ s16 field_0x301e;
    /* 0x03020 */ s16 field_0x3020;
    /* 0x03022 */ s16 field_0x3022;
    /* 0x03024 */ s16 field_0x3024;
    /* 0x03026 */ s16 field_0x3026;
    /* 0x03028 */ s16 field_0x3028;
    /* 0x0302A */ u8 field_0x302a[2];
    /* 0x0302C */ s16 field_0x302c[10];
    /* 0x03040 */ s16 field_0x3040[10];
    /* 0x03054 */ s16 field_0x3054[3];
    /* 0x0305A */ s16 field_0x305a[3];
    /* 0x03060 */ s16 field_0x3060;
    /* 0x03062 */ s16 field_0x3062;
    /* 0x03064 */ s16 field_0x3064;
    /* 0x03066 */ s16 field_0x3066[3];
    /* 0x0306C */ s16 field_0x306c;
    /* 0x0306E */ s16 field_0x306e;
    /* 0x03070 */ s16 field_0x3070;
    /* 0x03072 */ s16 field_0x3072;
    /* 0x03074 */ s16 field_0x3074;
    /* 0x03076 */ s16 field_0x3076;
    /* 0x03078 */ s16 field_0x3078;
    /* 0x0307A */ s16 mStickSpinTimer;
    /* 0x0307C */ s16 mPrevStickAngle;
    /* 0x0307E */ s16 field_0x307e;
    /* 0x03080 */ s16 field_0x3080;
    /* 0x03082 */ s16 field_0x3082;
    /* 0x03084 */ u16 field_0x3084;
    /* 0x03086 */ u16 field_0x3086;
    /* 0x03088 */ s16 field_0x3088;
    /* 0x0308A */ s16 field_0x308a;
    /* 0x0308C */ s16 field_0x308c;
    /* 0x0308E */ s16 mFastShotTime;
    /* 0x03090 */ s16 field_0x3090;
    /* 0x03092 */ s16 field_0x3092;
    /* 0x03094 */ s16 field_0x3094[3];
    /* 0x0309A */ s16 field_0x309a[3];
    /* 0x030A0 */ s16 field_0x30a0;
    /* 0x030A2 */ s16 field_0x30a2;
    /* 0x030A4 */ s16 field_0x30a4;
    /* 0x030A6 */ s16 field_0x30a6;
    /* 0x030A8 */ u16 field_0x30a8;
    /* 0x030AA */ u16 field_0x30aa;
    /* 0x030AC */ s16 mWolfEyeUp;
    /* 0x030AE */ s16 field_0x30ae;
    /* 0x030B0 */ s16 field_0x30b0;
    /* 0x030B2 */ s16 field_0x30b2;
    /* 0x030B4 */ u16 field_0x30b4;
    /* 0x030B6 */ u16 field_0x30b6;
    /* 0x030B8 */ u16 field_0x30b8;
    /* 0x030BA */ u16 field_0x30ba;
    /* 0x030BC */ u16 field_0x30bc;
    /* 0x030BE */ u16 field_0x30be;
    /* 0x030C0 */ u16 field_0x30c0;
    /* 0x030C2 */ u16 field_0x30c2;
    /* 0x030C4 */ u16 field_0x30c4;
    /* 0x030C6 */ u16 field_0x30c6;
    /* 0x030C8 */ s16 field_0x30c8;
    /* 0x030CA */ s16 field_0x30ca;
    /* 0x030CC */ s16 field_0x30cc;
    /* 0x030CE */ u16 mKeepItem;
    /* 0x030D0 */ s16 field_0x30d0;
    /* 0x030D2 */ s16 field_0x30d2;
    /* 0x030D4 */ s16 field_0x30d4;
    /* 0x030D6 */ s16 field_0x30d6;
    /* 0x030D8 */ u8 field_0x30d8[0x14];
    /* 0x030EC */ s16 field_0x30ec;
    /* 0x030EE */ s16 field_0x30ee;
    /* 0x030F0 */ u16 field_0x30f0;
    /* 0x030F2 */ u8 field_0x30f2[2];
    /* 0x030F4 */ s16 mSwordFlourishTimer;
    /* 0x030F6 */ s16 field_0x30f6;
    /* 0x030F8 */ s16 field_0x30f8;
    /* 0x030FA */ s16 field_0x30fa;
    /* 0x030FC */ u16 field_0x30fc;
    /* 0x030FE */ u16 field_0x30fe;
    /* 0x03100 */ s16 field_0x3100;
    /* 0x03102 */ s16 field_0x3102;
    /* 0x03104 */ s16 field_0x3104;
    /* 0x03106 */ u16 field_0x3106;
    /* 0x03108 */ s16 field_0x3108;
    /* 0x0310A */ s16 field_0x310a;
    /* 0x0310C */ s16 field_0x310c;
    /* 0x0310E */ s16 field_0x310e;
    /* 0x03110 */ s16 field_0x3110;
    /* 0x03112 */ u16 mExitID;
    /* 0x03114 */ s16 field_0x3114;
    /* 0x03116 */ s16 field_0x3116;
    /* 0x03118 */ s16 field_0x3118;
    /* 0x0311A */ s16 field_0x311a;
    /* 0x0311C */ s16 field_0x311c;
    /* 0x0311E */ s16 field_0x311e;
    /* 0x03120 */ u16 mMidnaMsgNum;
    /* 0x03122 */ s16 field_0x3122;
    /* 0x03124 */ s16 field_0x3124;
    /* 0x03126 */ s16 field_0x3126;
    /* 0x03128 */ s16 field_0x3128;
    /* 0x0312A */ csXyz field_0x312a[2];
    /* 0x03136 */ csXyz field_0x3136[2];
    /* 0x03142 */ csXyz field_0x3142[4];
    /* 0x0315A */ csXyz mFishingArm1Angle;
    /* 0x03160 */ csXyz field_0x3160;
    /* 0x03166 */ csXyz field_0x3166;
    /* 0x0316C */ csXyz field_0x316c;
    /* 0x03172 */ u8 field_0x3172[2];
    /* 0x03174 */ int field_0x3174;
    /* 0x03178 */ int field_0x3178;
    /* 0x0317C */ int field_0x317c;
    /* 0x03180 */ int field_0x3180;
    /* 0x03184 */ int mAlinkStaffId;
    /* 0x03188 */ int field_0x3188;
    /* 0x0318C */ int field_0x318c;
    /* 0x03190 */ int field_0x3190;
    /* 0x03194 */ int field_0x3194;
    /* 0x03198 */ int field_0x3198;
    /* 0x0319C */ int field_0x319c;
    /* 0x031A0 */ u32 mModeFlg;
    /* 0x031A4 */ int field_0x31a4;
    /* 0x031A8 */ u32 field_0x31a8[2];
    /* 0x031B0 */ u32 field_0x31b0[3];
    /* 0x031BC */ u32 field_0x31bc;
    /* 0x031C0 */ u32 field_0x31c0;
    /* 0x031C4 */ u32 field_0x31c4;
    /* 0x031C8 */ u32 field_0x31c8;
    /* 0x031CC */ u32 field_0x31cc;
    /* 0x031D0 */ u32 field_0x31d0;
    /* 0x031D4 */ u32 field_0x31d4;
    /* 0x031D8 */ u32 field_0x31d8[3];
    /* 0x031E4 */ u32 field_0x31e4;
    /* 0x031E8 */ u32 field_0x31e8[4];
    /* 0x031F8 */ u32 field_0x31f8;
    /* 0x031FC */ u32 field_0x31fc;
    /* 0x03200 */ u32 field_0x3200;
    /* 0x03204 */ u32 field_0x3204[6];
    /* 0x0321C */ u32 field_0x321c;
    /* 0x03220 */ u32 field_0x3220;
    /* 0x03224 */ u32 field_0x3224;
    /* 0x03228 */ u32 field_0x3228[2][3];
    /* 0x03240 */ u32 field_0x3240[6];
    /* 0x03258 */ u32 field_0x3258;
    /* 0x0325C */ u32 field_0x325c;
    /* 0x03260 */ u32 field_0x3260[2];
    /* 0x03268 */ u32 field_0x3268;
    /* 0x0326C */ u32 field_0x326c[4];
    /* 0x0327C */ u32 field_0x327c[3];
    /* 0x03288 */ u32 field_0x3288;
    /* 0x0328C */ u32 field_0x328c;
    /* 0x03290 */ u32 field_0x3290;
    /* 0x03294 */ u32 field_0x3294;
    /* 0x03298 */ u32 field_0x3298[2];
    /* 0x032A0 */ J3DGXColorS10 field_0x32a0[2];
    /* 0x032B0 */ J3DGXColorS10 field_0x32b0[2];
    /* 0x032C0 */ s16 field_0x32c0[2];
    /* 0x032C4 */ u16 field_0x32c4[2];
    /* 0x032C8 */ u32 field_0x32c8;
    /* 0x032CC */ uintptr_t field_0x32cc;
    /* 0x032D0 */ u32 field_0x32d0;
    /* 0x032D4 */ u32 field_0x32d4;
    /* 0x032D8 */ firePointEff_c field_0x32d8[4];
    /* 0x03398 */ f32 mNormalSpeed;
    /* 0x0339C */ f32 mSpeedModifier;
    /* 0x033A0 */ f32 field_0x33a0;
    /* 0x033A4 */ f32 field_0x33a4;
    /* 0x033A8 */ f32 field_0x33a8;
    /* 0x033AC */ f32 field_0x33ac;
    /* 0x033B0 */ f32 field_0x33b0;
    /* 0x033B4 */ f32 mWaterY;
    /* 0x033B8 */ f32 field_0x33b8;
    /* 0x033BC */ f32 field_0x33bc;
    /* 0x033C0 */ f32 field_0x33c0;
    /* 0x033C4 */ f32 mFallHeight;
    /* 0x033C8 */ f32 field_0x33c8;
    /* 0x033CC */ f32 field_0x33cc;
    /* 0x033D0 */ f32 field_0x33d0;
    /* 0x033D4 */ f32 field_0x33d4;
    /* 0x033D8 */ f32 field_0x33d8;
    /* 0x033DC */ f32 field_0x33dc;
    /* 0x033E0 */ f32 field_0x33e0;
    /* 0x033E4 */ f32 field_0x33e4;
    /* 0x033E8 */ f32 field_0x33e8;
    /* 0x033EC */ f32 field_0x33ec;
    /* 0x033F0 */ f32 field_0x33f0;
    /* 0x033F4 */ f32 field_0x33f4;
    /* 0x033F8 */ f32 field_0x33f8;
    /* 0x033FC */ f32 field_0x33fc;
    /* 0x03400 */ f32 field_0x3400;
    /* 0x03404 */ f32 field_0x3404;
    /* 0x03408 */ f32 field_0x3408;
    /* 0x0340C */ f32 field_0x340c;
    /* 0x03410 */ f32 field_0x3410;
    /* 0x03414 */ f32 field_0x3414;
    /* 0x03418 */ f32 field_0x3418;
    /* 0x0341C */ f32 field_0x341c;
    /* 0x03420 */ f32 field_0x3420;
    /* 0x03424 */ f32 field_0x3424;
    /* 0x03428 */ f32 field_0x3428;
    /* 0x0342C */ f32 field_0x342c;
    /* 0x03430 */ f32 field_0x3430;
    /* 0x03434 */ f32 mHeavySpeedMultiplier;
    /* 0x03438 */ f32 field_0x3438;
    /* 0x0343C */ f32 field_0x343c;
    /* 0x03440 */ f32 field_0x3440;
    /* 0x03444 */ f32 field_0x3444;
    /* 0x03448 */ f32 field_0x3448;
    /* 0x0344C */ f32 field_0x344c;
    /* 0x03450 */ u8 field_0x3450[4];
    /* 0x03454 */ f32 field_0x3454;
    /* 0x03458 */ f32 field_0x3458;
    /* 0x0345C */ f32 field_0x345c;
    /* 0x03460 */ f32 field_0x3460;
    /* 0x03464 */ f32 field_0x3464;
    /* 0x03468 */ f32 field_0x3468;
    /* 0x0346C */ f32 field_0x346c;
    /* 0x03470 */ f32 field_0x3470;
    /* 0x03474 */ f32 mSwordUpColorIntensity;
    /* 0x03478 */ f32 field_0x3478;
    /* 0x0347C */ f32 field_0x347c;
    /* 0x03480 */ f32 field_0x3480;
    /* 0x03484 */ f32 field_0x3484;
    /* 0x03488 */ f32 field_0x3488;
    /* 0x0348C */ f32 field_0x348c;
    /* 0x03490 */ f32 mSearchBallScale;
    /* 0x03494 */ f32 field_0x3494;
    /* 0x03498 */ cXyz field_0x3498;
    /* 0x034A4 */ cXyz field_0x34a4;
    /* 0x034B0 */ cXyz field_0x34b0;
    /* 0x034BC */ cXyz field_0x34bc;
    /* 0x034C8 */ cXyz field_0x34c8;
    /* 0x034D4 */ cXyz field_0x34d4;
    /* 0x034E0 */ cXyz field_0x34e0;
    /* 0x034EC */ cXyz field_0x34ec;
    /* 0x034F8 */ cXyz mLastJumpPos;
    /* 0x03504 */ cXyz field_0x3504;
    /* 0x03510 */ cXyz field_0x3510;
    /* 0x0351C */ cXyz field_0x351c;
    /* 0x03528 */ cXyz field_0x3528;
    /* 0x03534 */ cXyz field_0x3534;
    /* 0x03540 */ cXyz field_0x3540;
    /* 0x0354C */ cXyz field_0x354c[4];
    /* 0x0357C */ cXyz mLookPosFromOut;
    /* 0x03588 */ cXyz field_0x3588;
    /* 0x03594 */ cXyz field_0x3594;
    /* 0x035A0 */ cXyz field_0x35a0;
    /* 0x035AC */ cXyz mWindSpeed;
    /* 0x035B8 */ cXyz field_0x35b8;
    /* 0x035C4 */ cXyz field_0x35c4;
    /* 0x035D0 */ cXyz field_0x35d0;
    /* 0x035DC */ cXyz field_0x35dc;
    /* 0x035E8 */ cXyz field_0x35e8;
    /* 0x035F4 */ cXyz mMidnaAtnPos;
    /* 0x03600 */ cXyz mMidnaHairAtnPos;
    /* 0x0360C */ cXyz mKandelaarFlamePos;
    /* 0x03618 */ cXyz field_0x3618;
    /* 0x03624 */ cXyz field_0x3624;
    /* 0x03630 */ cXyz field_0x3630;
    /* 0x0363C */ cXyz field_0x363c[4];
    /* 0x0366C */ cXyz field_0x366c[4];
    /* 0x0369C */ cXyz field_0x369c;
    /* 0x036A8 */ cXyz mMagneBootsTopVec;
    /* 0x036B4 */ cXyz field_0x36b4;
    /* 0x036C0 */ cXyz field_0x36c0[4];
    /* 0x036F0 */ cXyz field_0x36f0[4];
    /* 0x03720 */ cXyz field_0x3720;
    /* 0x0372C */ cXyz field_0x372c;
    /* 0x03738 */ cXyz field_0x3738;
    /* 0x03744 */ cXyz field_0x3744;
    /* 0x03750 */ cXyz field_0x3750;
    /* 0x0375C */ cXyz field_0x375c;
    /* 0x03768 */ cXyz field_0x3768;
    /* 0x03774 */ cXyz mForcePutPos;
    /* 0x03780 */ cXyz field_0x3780;
    /* 0x0378C */ cXyz field_0x378c;
    /* 0x03798 */ cXyz field_0x3798;
    /* 0x037A4 */ cXyz field_0x37a4;
    /* 0x037B0 */ cXyz field_0x37b0[2];
    /* 0x037C8 */ cXyz field_0x37c8;
    /* 0x037D4 */ cXyz field_0x37d4;
    /* 0x037E0 */ cXyz mHeldItemRootPos;
    /* 0x037EC */ cXyz mHookshotTopPos;
    /* 0x037F8 */ cXyz mIronBallCenterPos;
    /* 0x03804 */ cXyz mIronBallBgChkPos;
    /* 0x03810 */ cXyz field_0x3810;
    /* 0x0381C */ cXyz field_0x381c;
    /* 0x03828 */ cXyz field_0x3828;
    /* 0x03834 */ cXyz field_0x3834;
    /* 0x03840 */ cXyz* mIronBallChainPos;
    /* 0x03844 */ csXyz* mIronBallChainAngle;
    /* 0x03848 */ cXyz* field_0x3848;
    /* 0x0384C */ cXyz* field_0x384c;
    /* 0x03850 */ daAlink_procFunc mpProcFunc;
};  // Size: 0x385C

static bool daAlink_checkLightBallA(fopAc_ac_c* p_actor);
static bool daAlink_checkLightBallB(fopAc_ac_c* p_actor);
static fopAc_ac_c* daAlink_searchCoach(fopAc_ac_c* param_0, void* param_1);

struct daAlink_cutParamTbl {
    /* 0x0 */ daAlink_c::daAlink_ANM m_anmID;
    /* 0x4 */ int field_0x4;
    /* 0x8 */ u8 m_cutType;
    /* 0x9 */ u8 m_atSe;
    /* 0xA */ u8 field_0xa;
    /* 0xB */ u8 field_0xb;
    /* 0xC */ f32 m_morf;
};  // Size: 0x10

struct daAlink_cutHorseParamTbl {
    /* 0x0 */ daAlink_c::daAlink_FTANM m_faceanm;
    /* 0x4 */ u16 m_upperAnm;
    /* 0x6 */ u16 m_faceBtk;
    /* 0x8 */ u8 field_0x8;
    /* 0x9 */ u8 field_0x9;
    /* 0xA */ u8 m_cutType;
};  // Size: 0xC

inline BOOL dComIfGs_isTransformLV(int i_no);
inline BOOL dComIfGs_isEventBit(const u16);

static fopAc_ac_c* daAlink_searchPortal(fopAc_ac_c* param_0, void* param_1);
static fopAc_ac_c* daAlink_searchCanoe(fopAc_ac_c* param_0, void* param_1);
static void* daAlink_searchBoar(fopAc_ac_c* param_0, void* param_1);
static fopAc_ac_c* daAlink_searchLightBall(fopAc_ac_c* p_actor, void* param_1);

inline daAlink_c* daAlink_getAlinkActorClass() {
    return (daAlink_c*)dComIfGp_getLinkPlayer();
}

#endif /* D_A_D_A_ALINK_H */
