#ifndef D_A_OBJ_KSHUTTER_H
#define D_A_OBJ_KSHUTTER_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjKshtr_c
 * @brief Lakebed Temple Boss Door
 *
 * @details
 *
*/

class daObjKshtr_c : public dBgS_MoveBgActor {
public:
    typedef cPhs__Step (daObjKshtr_c::*PhaseFunc)();
    typedef void (daObjKshtr_c::*ActionFunc)();
    typedef BOOL (daObjKshtr_c::*DemoFunc)();

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    void offDzb();
    int CreateHeap();
    void initKey();
    cPhs__Step phase_0();
    cPhs__Step phase_1();
    cPhs__Step phase_2();
    cPhs__Step create1st();
    void event_proc_call();
    void event_proc_call2();
    int Execute(Mtx**);
    BOOL checkArea();
    BOOL checkOpen();
    int getDemoAction();
    BOOL demoProc2();
    BOOL adjustmentProc();
    BOOL keyUnlockInit();
    BOOL keyUnlock();
    BOOL openInit();
    BOOL openProc();
    BOOL openProc_type1();
    BOOL openProc_type2();
    BOOL openProc_typeL3Boss();
    void demoProc();
    BOOL demoJail1();
    BOOL demoJail2();
    BOOL demoJail11();
    BOOL demoJail21();
    void anmInit();
    void actionWaitEvent();
    void actionEvent();
    void actionDead();
    void actionWaitEvent2();
    void actionOpen();
    void actionEvent2();
    void actionDead2();
    int Draw();
    int Delete();
    u8 checkKey() { return fopAcM_GetParamBit(this, 31, 1); }
    u8 getEvId() { return fopAcM_GetParamBit(this, 16, 8); }
    u8 getType() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }
    void setAction(u8 action) { mAction = action; }
private:
    /* 0x5A0 */ request_of_phase_process_class mPhase1;
    /* 0x5A8 */ request_of_phase_process_class mPhase2;
    /* 0x5B0 */ J3DModel* mpModel;
    /* 0x5B4 */ mDoExt_bckAnm* mpBckAnm;
    /* 0x5B8 */ Mtx mMtx;
    /* 0x5E8 */ u8 mAction;
    /* 0x5E9 */ u8 mDemoMode;
    /* 0x5EA */ u8 mType;
    /* 0x5EB */ u8 mSwNo;
    /* 0x5EC */ bool field_0x5ec;
    /* 0x5ED */ bool mIsCheckKey;
    /* 0x5EE */ u8 mEvId;
    /* 0x5EF */ bool mIsWolf;
    /* 0x5F0 */ s16 field_0x5f0[2];
    /* 0x5F4 */ u8 field_0x5f4[0x5fc - 0x5f4];
    /* 0x5FC */ f32 field_0x5fc;
    /* 0x600 */ int mStaffId;
    /* 0x604 */ u8 field_0x604;
    /* 0x605 */ u8 field_0x605;
    /* 0x606 */ u8 mTimer;
    /* 0x608 */ cXyz mSePos;
    /* 0x614 */ fpc_ProcID mKeyHoleId;
    /* 0x618 */ u8 mPhase;
    /* 0x619 */ bool field_0x619;
    /* 0x61A */ s16 mHomeAngleZ;
    /* 0x61C */ s16 mHomeAngleX;
    /* 0x61E */ bool field_0x61e;
};

STATIC_ASSERT(sizeof(daObjKshtr_c) == 0x620);

#endif /* D_A_OBJ_KSHUTTER_H */
