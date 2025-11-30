#ifndef D_A_OBJ_SCANNON_TEN_H
#define D_A_OBJ_SCANNON_TEN_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjSCannonTen_c
 * @brief Sky Cannon (Lake Hylia, Fixed)
 *
 * @details
 *
 */
class daObjSCannonTen_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    enum daSCannonTen_exeMode {
        MODE_WAIT,
        MODE_ORDER_EVT,
        MODE_ACTION_EVT,
        MODE_END,
    };

    daObjSCannonTen_c();
    virtual ~daObjSCannonTen_c();
    int createHeap();
    int create();
    int Delete();
    int draw();
    int execute();
    void middleExe();
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
    void demoInitFireEnd();
    void demoExeFireEnd();
    void demoInitFinish();
    void demoExeFinish();
    void init();
    void setModelMtx();
    void changeScene();
    void initEmtSmoke();
    void exeEmtSmoke();
    void initEmtLine();
    void exeEmtLine();
    void delEmtAll();

    static void (daObjSCannonTen_c::*s_exeProc[])();
    static void (daObjSCannonTen_c::*s_demoExeProc[][2])();

private:
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ mDoExt_bckAnm mBck;
    /* 0x594 */ dBgW* mpBgW;
    /* 0x598 */ cBgS_PolyInfo mGroundPoly;
    /* 0x5A8 */ Mtx mBgMtx;
    /* 0x5D8 */ f32 mGroundY;
    /* 0x5DC */ u32 mShadowKey;
    /* 0x5E0 */ int mTimer;
    /* 0x5E4 */ int mStaffId;
    /* 0x5E8 */ u8 field_0x5E8[0x5EC - 0x5E8];
    /* 0x5EC */ s16 mEvtIdx;
    /* 0x5EE */ u16 mHeadJointNo;
    /* 0x5F0 */ u8 mMode;
    /* 0x5F4 */ JPABaseEmitter* mpEmtSmk[3];
    /* 0x600 */ JPABaseEmitter* mpEmtLine;
};

STATIC_ASSERT(sizeof(daObjSCannonTen_c) == 0x604);


#endif /* D_A_OBJ_SCANNON_TEN_H */
