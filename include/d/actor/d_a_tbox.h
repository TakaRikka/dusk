#ifndef D_A_TBOX_H
#define D_A_TBOX_H

#include "SSystem/SComponent/c_lib.h"
#include "d/d_cc_d.h"
#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_hostIO.h"

class dBgW;

class daTboxBase_c : public dBgS_MoveBgActor {
protected:
    virtual BOOL checkSmallTbox() = 0;

    /* 0x5A0 */ dCcD_Stts mStts;
    /* 0x5DC */ dCcD_Cyl mCyl;
};

STATIC_ASSERT(sizeof(daTboxBase_c) == 0x718);

struct daTbox_ModelInfo {
    /* 0x00 */ char* mArcName;
    /* 0x04 */ int mModelResNo;
    /* 0x08 */ int mBckResNo;
    /* 0x0C */ int mClosedDzbResNo;
    /* 0x10 */ int mOpenDzbResNo;
    /* 0x14 */ int mEffectResNo;
    /* 0x18 */ int mBrkResNo;
};

STATIC_ASSERT(sizeof(daTbox_ModelInfo) == 0x1C);

class daTbox_c;
typedef int (daTbox_c::*daTbox_actionFn)();
typedef void (daTbox_c::*daTbox_modeFn)();

/**
 * @ingroup actors-unsorted
 * @class daTbox_c
 * @brief Treasure Box
 *
 * @details
 *
 */
class daTbox_c : public daTboxBase_c {
public:
    enum Shape {
        SHAPE_SMALL = 0,
        SHAPE_LARGE = 1,
        SHAPE_BOSSKEY = 2,
    };

    enum Mode {
        MODE_EXEC_WAIT = 0,
        MODE_EXEC = 1,
    };

    daTbox_ModelInfo* getModelInfo();
    cPhs__Step commonShapeSet();
    cPhs__Step effectShapeSet();
    cPhs__Step envShapeSet();
    cPhs__Step bgCheckSet();
    void lightReady();
    void setLightPos();
    int checkEnv();
    int checkAppear();
    int checkOpen();
    void clrDzb();
    void setDzb();
    void surfaceProc();
    int checkNormal();
    int checkEnvEffectTbox();
    u32 calcHeapSize();
    int CreateHeap();
    void CreateInit();
    void initPos();
    void initAnm();
    int boxCheck();
    void demoProcOpen();
    void lightColorProc();
    void environmentProc();
    void lightUpProc();
    void lightDownProc();
    void dropProcInitCall();
    void dropProcInit();
    int calcJumpGoalAndAngle(cXyz*, s16*);
    bool getDropSAngle(s16*);
    int getDir();
    void setRotAxis(cXyz const*, cXyz const*);
    void dropProcInit2();
    void dropProc();
    void demoInitAppear();
    void demoProcAppear();
    int demoProc();
    void OpenInit_com();
    void OpenInit();
    int actionWait();
    int actionDemo();
    int actionDemo2();
    int actionDropDemo();
    u8 getBombItemNo(u8, u8);
    u8 getBombItemNo2(u8, u8, u8);
    u8 getBombItemNo3(u8, u8, u8, u8);
    u8 getBombItemNoMain(u8);
    int setGetDemoItem();
    int actionOpenWait();
    int actionNotOpenDemo();
    int checkDrop();
    void settingDropDemoCamera();
    int actionSwOnWait();
    int actionSwOnWait2();
    int actionDropWait();
    int actionGenocide();
    int actionDropWaitForWeb();
    int actionDropForWeb();
    void initBaseMtx();
    void setBaseMtx();
    void mode_proc_call();
    void mode_exec_wait();
    void mode_exec();
    cPhs__Step create1st();
    int Execute(Mtx**);
    int Draw();
    int Delete();
    int Create() { return true; }

    virtual BOOL checkSmallTbox() { return TRUE; }

    u32 getEvent() { return fopAcM_GetParam(this) >> 24; }
    int getShapeType() { return (fopAcM_GetParam(this) >> 20) & 0xf; }
    int getSwNo() { return (fopAcM_GetParam(this) >> 12) & 0xff; }
    int getTboxNo() { return (fopAcM_GetParam(this) >> 6) & 0x3f; }
    int getFuncType() { return fopAcM_GetParam(this) & 0x3f; }
    u32 getSwType() { return field_0x980 & 0xf; }
    int getItemNo() { return (field_0x982 >> 8) & 0xff; }
    int getPathId() { return field_0x982 & 0xff; }
    void flagClr() { mFlags = 0; }
    void flagOn(u16 i_flag) { mFlags |= i_flag; }
    void flagOff(u16 i_flag) { mFlags &= ~i_flag; }
    u16 flagCheck(u16 i_flag) { return mFlags & i_flag; }
    void setAction(daTbox_actionFn i_actionFn) { mpActionFn = i_actionFn; }
    void action() { (this->*mpActionFn)(); }
    void setDrawMtx(Mtx i_mtx) {
        MTXCopy(i_mtx, mDrawMtx);
        field_0x9fc = 1;
    }

private:
    /* 0x718 */ u8 field_0x718;
    /* 0x71C */ request_of_phase_process_class mPhase;
    /* 0x724 */ J3DModel* mpModel;
    /* 0x728 */ J3DModel* mpSlimeModel;
    /* 0x72C */ mDoExt_bckAnm* mpAnm;
    /* 0x730 */ J3DModel* mpEffectModel;
    /* 0x734 */ mDoExt_brkAnm* mpEffectAnm;
    /* 0x738 */ dBgW* mpOpenBgW;
    /* 0x73C */ dBgW* mpBgCollision;
    /* 0x740 */ daTbox_actionFn mpActionFn;
    /* 0x74C */ int mStaffId;
    /* 0x750 */ f32 field_0x750;
    /* 0x754 */ u16 mFlags;
    /* 0x756 */ u16 mDemoFrame;
    /* 0x758 */ bool field_0x758;
    /* 0x759 */ u8 field_0x759;
    /* 0x75A */ u16 field_0x75a;
    /* 0x75C */ u8 mTimer;
    /* 0x760 */ dBgS_ObjAcch mAcch;
    /* 0x938 */ dBgS_AcchCir mAcchCir;
    /* 0x978 */ u8 mTboxNo;
    /* 0x97A */ s16 mEventId;
    /* 0x97C */ bool field_0x97c;
    /* 0x97D */ bool field_0x97d;
    /* 0x97E */ u8 field_0x97e;
    /* 0x97F */ bool mParamsInit;
    /* 0x980 */ u16 field_0x980;
    /* 0x982 */ u16 field_0x982;
    /* 0x984 */ s16 field_0x984;
    /* 0x988 */ Mtx field_0x988;
    /* 0x9B8 */ cXyz mRotAxis;
    /* 0x9C4 */ s16 field_0x9c4;
    /* 0x9C6 */ s16 field_0x9c6;
    /* 0x9C8 */ u8 field_0x9c8;
    /* 0x9C9 */ u8 field_0x9c9;
    /* 0x9CA */ s16 field_0x9ca;
    /* 0x9CC */ u8 field_0x9cc;
    /* 0x9CD */ u8 mMode;
    /* 0x9D0 */ LIGHT_INFLUENCE mLight;
    /* 0x9F0 */ f32 mAllcolRatio;
    /* 0x9F4 */ int field_0x9f4;
    /* 0x9F8 */ u32 mOpenSeId;
    /* 0x9FC */ u8 field_0x9fc;
    /* 0x9FD */ u8 field_0x9fd;
    /* 0xA00 */ Mtx mDrawMtx;
};

STATIC_ASSERT(sizeof(daTbox_c) == 0xA30);

class daTbox_HIO_c : public mDoHIO_entry_c {
public:
    daTbox_HIO_c();
    virtual ~daTbox_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ u8 mItemNo;
    /* 0x05 */ u8 mUseDebugItemNo;
    /* 0x06 */ u8 mClearSwitch;
    /* 0x07 */ u8 mDemoType;
    /* 0x08 */ u8 mDebugLightColorEnabled;
    /* 0x09 */ u8 mTimerDisplay;
    /* 0x0A */ u8 mCheckDisplay;
    /* 0x0B */ u8 mSaveOff;
    /* 0x0C */ f32 mGravity;
    /* 0x10 */ cXyz mLightOffset;
    /* 0x1C */ s16 mLightColorR;
    /* 0x1E */ s16 mLightColorG;
    /* 0x20 */ s16 mLightColorB;
    /* 0x24 */ f32 mLightPower;
    /* 0x28 */ f32 mFluctuationRate;
    /* 0x2C */ f32 mEnvColorInfluence;
    /* 0x30 */ s16 mFluctuationSpeed;
    /* 0x32 */ u8 mForPlacementCheck;
};

STATIC_ASSERT(sizeof(daTbox_HIO_c) == 0x34);

#endif /* D_A_TBOX_H */
