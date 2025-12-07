#ifndef D_D_ATTENTION_H
#define D_D_ATTENTION_H

#include "SSystem/SComponent/c_angle.h"
#include "m_Do/m_Do_ext.h"
#include "f_pc/f_pc_base.h"
#include "m_Do/m_Do_hostIO.h"

class fopAc_ac_c;

class dAttHint_c {
public:
    fpc_ProcID getPId(void*);
    fopAc_ac_c* convPId(fpc_ProcID);
    int request(fopAc_ac_c*, int);
    void init();
    void proc();

    fopAc_ac_c* getZHintTarget() { return convPId(field_0x8); }

private:
    /* 0x0 */ fpc_ProcID mHintActorID;
    /* 0x4 */ int mPriority;
    /* 0x8 */ fpc_ProcID field_0x8;
};

class dAttCatch_c {
public:
    fopAc_ac_c* convPId(fpc_ProcID);
    void init();
    void proc();
    int request(fopAc_ac_c*, u8, f32, f32, f32, s16, int);

    fopAc_ac_c* getCatghTarget() { return convPId(mCatghTargetID); }
    u8 getChangeItem() { return mChangeItem; }

private:
    /* 0x00 */ fpc_ProcID mRequestActorID;
    /* 0x04 */ int field_0x4;
    /* 0x08 */ f32 mDistance;
    /* 0x0C */ u8 mCatchItemNo;
    /* 0x10 */ fpc_ProcID mCatghTargetID;
    /* 0x14 */ u8 mChangeItem;
};  // Size: 0x18

class dAttParam_c : public JORReflexible {
public:
#if DEBUG
    /* 0x04 */ s8 mHIOChildNo;
#endif

    /* 0x00 */ u16 mFlags;
    /* 0x04 */ f32 field_0x4;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 mDangerBGMDistance;
    /* 0x14 */ f32 mBGMDistMargin;
    /* 0x18 */ f32 mSWModeDisable;
    /* 0x1C */ f32 mSelCursorScaleX;
    /* 0x20 */ f32 mSelCursorScaleY;
    /* 0x24 */ f32 mAttnCursorScaleX;
    /* 0x28 */ f32 mAttnCursorScaleY;
    /* 0x2C */ f32 mSelCursorOffsetY;
    /* 0x30 */ f32 mAttnCursorOffsetY;
    /* 0x34 */ u8 mAttnCursorAppearFrames;
    /* 0x35 */ u8 mAttnCursorDisappearFrames;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ f32 field_0x3c;
#if DEBUG
    /* 0x44 */ s32 mDebugDispPosX;
    /* 0x48 */ s32 mDebugDispPosY;
#endif

public:
    dAttParam_c() {}
    dAttParam_c(s32);

#if DEBUG
    void connectHIO(char* i_name) {
        mHIOChildNo = mDoHIO_CREATE_CHILD(i_name, this);
    }

    void releaseHIO() {
        mDoHIO_DELETE_CHILD(mHIOChildNo);
    }

    virtual void genMessage(JORMContext*);
#endif
    virtual ~dAttParam_c();

    enum EFlag {
        EFlag_HOLD_MODE    = (1 << 0),
        EFlag_SWMODE_DEBUG = (1 << 1),
        EFlag_VALID        = (1 << 2),
        EFlag_KEEP         = (1 << 3),
        EFlag_ARROW_OFF    = (1 << 4),
        EFlag_LIST_DEBUG   = (1 << 14),
        EFlag_MARGIN_DEBUG = (1 << 15),
    };

    bool CheckFlag(u16 flag) { return flag & mFlags ? true : false; }

    f32 FreeStick() {
#if DEBUG
        return mSWModeDisable;
#else
        return -0.9f;
#endif
    }

    /* 0x40 vtable */
};  // Size: 0x44

class dAttLook_c {
public:
    fopAc_ac_c* convPId(fpc_ProcID);
    void init();
    void proc();
    int request(fopAc_ac_c*, f32, f32, f32, s16, int);

    fopAc_ac_c* getLookTarget() { return convPId(mLookTargetID); }

private:
    fpc_ProcID mRequestActorID;
    int field_0x4;
    f32 mDistance;
    fpc_ProcID mLookTargetID;
};  // Size: 0x10

class dAttList_c {
public:
    ~dAttList_c() {}
    dAttList_c() {}
    fopAc_ac_c* getActor();
    void setActor(fopAc_ac_c*);

    fpc_ProcID getPid() { return mActorID; }

    /* 0x00 */ f32 mWeight;
    /* 0x04 */ f32 mDistance;
    /* 0x08 */ cSAngle mAngle;
    /* 0x0C */ u32 mType;
    /* 0x10 */ fpc_ProcID mActorID;
};  // Size: 0x14

class dAttDraw_c {
public:
    ~dAttDraw_c() {}
    dAttDraw_c() {}
    void setAnm(u8, f32);
    void setAlphaAnm(u8, u8);
    void alphaAnm();
    void draw(cXyz&, f32 (*)[4]);

    /* 0x000 */ J3DModel* mModel[2];
    /* 0x008 */ mDoExt_bckAnm mNoticeCursorBck[2];
    /* 0x040 */ mDoExt_bpkAnm mNoticeCursorBpk[2];
    /* 0x070 */ mDoExt_brkAnm mNoticeCursorBrk[2];
    /* 0x0A0 */ mDoExt_brkAnm mNoticeCursor02Brk[2];
    /* 0x0D0 */ mDoExt_btkAnm mNoticeCursorBtk[2];
    /* 0x100 */ mDoExt_bckAnm mImpactBck;
    /* 0x11C */ mDoExt_bpkAnm mImpactBpk;
    /* 0x134 */ mDoExt_brkAnm mImpactBrk;
    /* 0x14C */ mDoExt_btkAnm mImpactBtk;
    /* 0x164 */ f32 mCursorSizeX;
    /* 0x168 */ f32 mCursorSizeY;
    /* 0x16C */ f32 mCursorOffsetY;
    /* 0x170 */ u8 mDrawType;
    /* 0x171 */ u8 mAlphaAnmFrameMax;
    /* 0x172 */ u8 mAlphaAnmFrame;
    /* 0x173 */ u8 field_0x173;
    /* 0x174 */ u8 mAlphaAnmPlayDirection;
    /* 0x175 */ u8 field_0x175;
};

STATIC_ASSERT(sizeof(dAttDraw_c) == 0x178);

class dAttDraw_CallBack_c : public mDoExt_McaMorfCallBack1_c {
public:
    virtual int execute(u16, J3DTransformInfo*);
};

struct dist_entry {
    /* 0x00 */ f32 mDistMax;
    /* 0x04 */ f32 mDistMaxRelease;
    /* 0x08 */ f32 mDistanceAdjust;
    /* 0x0C */ f32 mUpperY;
    /* 0x10 */ f32 mLowerY;
    /* 0x14 */ f32 mWeight;
    /* 0x18 */ u32 mAngleSelect;
};  // Size: 0x1C

struct type_tbl_entry {
    s16 type;
    u16 mask;
};

class dAttention_c {
public:
    enum EState {
        EState_NONE,
        EState_LOCK,
        EState_RELEASE,
    };

    dAttention_c() {}
    dAttention_c(fopAc_ac_c*, u32);
    ~dAttention_c();
    dAttList_c* GetLockonList(s32);
    dAttList_c* getActionBtnB();
    dAttList_c* getActionBtnXY();
    int chkAttMask(u32, u32);
    f32 calcWeight(int, fopAc_ac_c*, f32, s16, s16, u32*);
    void setList(int, fopAc_ac_c*, f32, f32, cSAngle, u32);
    void initList(u32);
    int makeList();
    void setOwnerAttentionPos();
    int SelectAttention(fopAc_ac_c*);
    void sortList();
    void stockAttention();
    fopAc_ac_c* nextAttention();
    int freeAttention();
    bool chaseAttention();
    f32 EnemyDistance(fopAc_ac_c*);
    void runSoundProc();
    void runDrawProc();
    void runDebugDisp();
    void checkButton();
    bool triggerProc();
    int lostCheck();
    void judgementStatus4Hold();
    void judgementStatus4Switch();
    int Run();
    void Draw();
    void lockSoundStart(u32);
    fopAc_ac_c* LockonTarget(s32);
    f32 LockonReleaseDistanse();
    fpc_ProcID LockonTargetPId(s32);
    fopAc_ac_c* ActionTarget(s32);
    fopAc_ac_c* CheckObjectTarget(s32);
    bool LockonTruth();
    int checkDistance(cXyz*, s16, cXyz*, f32, f32, f32, f32);
    bool LockEdge() { return chkFlag(8) || chkFlag(0x20); }
    int GetCheckObjectCount() { return mCheckObjectCount; }
    void keepLock(int timer) { mAttnBlockTimer = timer; }
    static dist_entry& getDistTable(int i_no) { return dist_table[i_no]; }

#if DEBUG
    void runDebugDisp0();
#endif

    JKRSolidHeap* getHeap() { return heap; }
    fopAc_ac_c* getCatghTarget() { return mCatghTarget.getCatghTarget(); }
    fopAc_ac_c* getZHintTarget() { return mZHintTarget.getZHintTarget(); }
    u8 getCatchChgItem() { return mCatghTarget.getChangeItem(); }
    BOOL chkFlag(u32 flag) { return (mFlags & flag) ? TRUE : FALSE; }
    void setFlag(u32 flag) { mFlags |= flag; }
    void clrFlag(u32 flag) { mFlags &= ~flag; }
    int GetActionCount() { return mActionCount; }
    int GetLockonCount() { return mLockonCount; }
    void LockSoundOn() { clrFlag(0x400000); }
    void LockSoundOff() { setFlag(0x400000); }
    bool Lockon() { return LockonTruth() || chkFlag(0x20000000); }  // only matches with -O2?
    int ZHintRequest(fopAc_ac_c* param_1, int param_2) {
        return mZHintTarget.request(param_1, param_2);
    }

    void LookRequest(fopAc_ac_c* param_0, f32 i_horizontalDist, f32 i_upDist, f32 i_downDist,
                     s16 i_angle, int param_5) {
        mLookTarget.request(param_0, i_horizontalDist, i_upDist, i_downDist, i_angle, param_5);
    }

    void CatchRequest(fopAc_ac_c* param_0, u8 param_1, f32 i_horizontalDist, f32 i_upDist,
                      f32 i_downDist, s16 i_angle, int param_5) {
        mCatghTarget.request(param_0, param_1, i_horizontalDist, i_upDist, i_downDist, i_angle,
                             param_5);
    }

    fopAc_ac_c* getLookTarget() {
        return mLookTarget.getLookTarget();
    }

    void Init(fopAc_ac_c* param_0, u32 i_padNo) {
        mpPlayer = param_0;
        mPadNo = i_padNo;
    }

    void offAttnDraw() {
        draw[0].field_0x173 = 3;
        draw[1].field_0x173 = 3;
    }

    static type_tbl_entry loc_type_tbl[3];
    static type_tbl_entry act_type_tbl[5];
    static dist_entry dist_table[234];
    static int loc_type_num;
    static int act_type_num;
    static type_tbl_entry chk_type_tbl[1];
    static int chk_type_num;

public:
    /* 0x000 */ fopAc_ac_c* mpPlayer;
    /* 0x004 */ fpc_ProcID mLockTargetID;
    /* 0x008 */ dAttDraw_CallBack_c mpDrawCallback;
    /* 0x00C */ u32 mPadNo;
    /* 0x010 */ u32 mPlayerAttentionFlags;
    /* 0x014 */ u8 field_0x14[4];
    /* 0x018 */ JKRSolidHeap* heap;
    /* 0x01C */ cXyz mDrawAttnPos;
    /* 0x028 */ dAttDraw_c draw[2];
    /* 0x318 */ fpc_ProcID mTargetActorID;
    /* 0x31C */ cXyz mOwnerAttnPos;
    /* 0x328 */ s8 field_0x328;
    /* 0x329 */ u8 mAttnStatus;
    /* 0x32A */ u8 field_0x32a;
    /* 0x32B */ u8 field_0x32b;
    /* 0x32C */ u8 field_0x32c;
    /* 0x32D */ u8 field_0x32d;
    /* 0x32E */ u8 field_0x32e;
    /* 0x32F */ u8 field_0x32f;
    /* 0x330 */ u8 field_0x330;
    /* 0x331 */ u8 field_0x331;
    /* 0x332 */ u8 field_0x332;
    /* 0x333 */ u8 field_0x333;
    /* 0x334 */ u32 mFlags;
    /* 0x338 */ dAttList_c mLockOnList[8];
    /* 0x3D8 */ int mLockonCount;
    /* 0x3DC */ int mLockOnOffset;
    /* 0x3E0 */ dAttList_c mActionList[4];
    /* 0x430 */ int mActionCount;
    /* 0x434 */ int mActionOffset;
    /* 0x438 */ dAttList_c mCheckObjectList[4];
    /* 0x488 */ int mCheckObjectCount;
    /* 0x48C */ int mCheckObjectOffset;
    /* 0x490 */ dAttHint_c mZHintTarget;
    /* 0x49C */ dAttCatch_c mCatghTarget;
    /* 0x4B4 */ dAttLook_c mLookTarget;
    /* 0x4C4 */ int mAttnBlockTimer;
#if PLATFORM_WII || PLATFORM_SHIELD
    /* 0x4C8 */ u8 field_0x4c8[0x4D0 - 0x4C8];
#endif
    /* 0x4C8 */ dAttParam_c mAttParam;
#if PLATFORM_WII || PLATFORM_SHIELD
    /* 0x51C */ u8 field_0x50c[0x520 - 0x51C];
#else
    /* 0x50C */ u8 field_0x50c[0x514 - 0x50c];
#endif
    /* 0x514 */ fpc_ProcID mEnemyActorID;
    /* 0x518 */ f32 mEnemyDist;
};  // Size: 0x51C

STATIC_ASSERT(sizeof(dAttention_c) == 0x51C);

#endif /* D_D_ATTENTION_H */
