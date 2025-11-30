#ifndef D_A_OBJ_SCANNON_H
#define D_A_OBJ_SCANNON_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daSCannon_c
 * @brief Sky Cannon (City in the Sky)
 *
 * @details
 *
 */
class daSCannon_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    struct demoTable_s {
        /* 0x0 */ void (daSCannon_c::*(*demo_proc_table)[2])();
        /* 0x4 */ char** cut_table;
        /* 0x8 */ int cut_num;
    };

    enum daSCannon_exeMode {
        MODE_WAIT,
        MODE_ORDER_EVT,
        MODE_ACTION_EVT,
        MODE_END,
    };

    enum daSCannon_demoType {
        DEMOTYPE_WARP_END,
        DEMOTYPE_FIRE_FIRST,
        DEMOTYPE_FIRE_TKS,
        DEMOTYPE_FIRE_SECOND,  
    };

    daSCannon_c();
    virtual ~daSCannon_c();
    int create();
    int execute();
    int draw();
    int Delete();
    void setModelMtx();
    void setPtlModelMtx();
    int createHeap();
    void middleExe();
    void orderEvtInit();
    void exeModeWait();
    void exeModeOrderEvt();
    void exeModeActionEvt();
    void exeModeEnd();
    void demoExe();
    void demoModeInit(int);
    void demoInitLinkIn();
    void demoExeLinkIn();
    void demoInitSet();
    void demoExeSet();
    void demoInitMove();
    void demoExeMove();
    void demoInitFire();
    void demoExeFire();
    void demoInitFinish();
    void demoExeFinish();
    void demoInitWarpEndWait();
    void demoExeWarpEndWait();
    void demoInitWarpEndMove();
    void demoExeWarpEndMove();
    void changeScene();
    void init();
    void initEmtRt(u16, int, u16 const*, JPABaseEmitter**);
    void exeEmtRt(u16, int, JPABaseEmitter**);
    void initEmtLine();
    void exeEmtLine();
    void delEmtAll();
    void setCannonRepair();

    int getSw1() { return fopAcM_GetParamBit(this, 0, 8); }
    int getSw2() { return fopAcM_GetParamBit(this, 8, 8); }

    static const demoTable_s s_demoTable[];
    static void (daSCannon_c::*s_exeProc[])();
    static void (daSCannon_c::*s_demoExeProc_WarpEnd[][2])();
    static void (daSCannon_c::*s_demoExeProc_FireTks[][2])();
    static void (daSCannon_c::*s_demoExeProc_FireFirst[][2])();
    static void (daSCannon_c::*s_demoExeProc_FireSecond[][2])();

private:
    /* 0x574 */ request_of_phase_process_class mZevPhase;
    /* 0x57C */ request_of_phase_process_class mPtlPhase;
    /* 0x584 */ mDoExt_bckAnm mBck;
    /* 0x5A0 */ cBgS_PolyInfo mGroundPoly;
    /* 0x5B0 */ Mtx mBgMtx;
    /* 0x5E0 */ dBgW* mpBgW;
    /* 0x5E4 */ J3DModel* mpModels[2];
    /* 0x5EC */ J3DModel* mpPtlModel;
    /* 0x5F0 */ mDoExt_btkAnm* mpCrashBtk;
    /* 0x5F4 */ mDoExt_btkAnm* mpPtlBtk;
    /* 0x5F8 */ f32 mGroundY;
    /* 0x5FC */ u32 mShadowKey;
    /* 0x600 */ int mTimer;
    /* 0x604 */ int mStaffId;
    /* 0x608 */ int mDemoFinishTimer;
    /* 0x60C */ int mLayerNo;
    /* 0x610 */ s16 mEvtIdx;
    /* 0x612 */ u16 mHeadJointNo;
    /* 0x614 */ u16 mCrashJointNo;
    /* 0x616 */ u8 mMode;
    /* 0x617 */ u8 mIsRepaired;
    /* 0x618 */ u8 mDemoType;
    /* 0x619 */ u8 field_0x619;
    /* 0x61A */ u8 mIsPortal;
    /* 0x61B */ u8 mDrawShadow;
    /* 0x61C */ JPABaseEmitter* mpEmtApp[3];
    /* 0x628 */ JPABaseEmitter* mpEmtSmk[3];
    /* 0x634 */ JPABaseEmitter* mpEmtLine;
};

STATIC_ASSERT(sizeof(daSCannon_c) == 0x638);


#endif /* D_A_OBJ_SCANNON_H */
