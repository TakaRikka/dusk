#ifndef D_A_OBJ_KGATE_H
#define D_A_OBJ_KGATE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjKGate_c
 * @brief Coro Gate
 *
 * @details
 *
 */
class daObjKGate_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int create1st();
    int checkAreaL(cXyz const*, cXyz const*);
    int checkAreaR(cXyz const*, cXyz const*);
    BOOL checkOpen();
    BOOL checkDirL(fopAc_ac_c*);
    BOOL checkDirR(fopAc_ac_c*);
    void setAtkSE();
    void setCrkSE();
    void action();
    void action_typeA();
    void action_typeB();
    void actionWaitEvent();
    void actionEvent();
    void actionDead();
    void event_proc_call();
    void demoProc();
    int getDemoAction();
    
    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    void setAction(u8 i_action) { mAction = i_action; }
    u8 getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getEventID() { return fopAcM_GetParamBit(this, 0x14, 8); }
    u8 getNameArg() { return fopAcM_GetParamBit(this, 0x10, 4); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpGateModel;
    /* 0x5AC */ J3DModel* mpGateModel2;
    /* 0x5B0 */ J3DModel* mpKeyModel;
    /* 0x5B4 */ J3DModel* mpHookModel;
    /* 0x5B8 */ dCcD_Stts field_0x5b8;
    /* 0x5F4 */ dCcD_Cyl field_0x5f4[4];
    /* 0xAE4 */ dBgW* mpBgW;
    /* 0xAE8 */ Mtx field_0xae8;
    /* 0xB18 */ Mtx field_0xb18;
    /* 0xB48 */ u8 field_0xB48[0xB78 - 0xB48];
    /* 0xB78 */ s16 mGateRAngle;
    /* 0xB7A */ s16 mGateLAngle;
    /* 0xB7C */ u8 field_0xB7C[0xB94 - 0xB7C];
    /* 0xB94 */ f32 field_0xb94;
    /* 0xB98 */ f32 field_0xb98;
    /* 0xB9C */ int mCounter;
    /* 0xBA0 */ s16 mGateLMove;
    /* 0xBA2 */ s16 mGateRMove;
    /* 0xBA4 */ u8 mAction;
    /* 0xBA5 */ u8 mMapToolID;
    /* 0xBA6 */ s16 mEventID;
    /* 0xBA8 */ int mStaffID;
    /* 0xBAC */ u8 mTimer;
    /* 0xBAD */ bool mCreatedSmokeEff;
    /* 0xBAE */ s16 field_0xbae;
    /* 0xBB0 */ s16 field_0xbb0;
    /* 0xBB2 */ s16 field_0xbb2;
    /* 0xBB4 */ f32 field_0xbb4;
    /* 0xBB8 */ cXyz field_0xbb8;
    /* 0xBC4 */ cXyz field_0xbc4;
    /* 0xBD0 */ s16 field_0xbd0;
    /* 0xBD2 */ u8 field_0xBD2[0xBD4 - 0xBD2];
    /* 0xBD4 */ cXyz mEffPos;
    /* 0xBE0 */ csXyz mKeyRot;
    /* 0xBE8 */ f32 field_0xbe8;
    /* 0xBEC */ u8 mNameArg;
    /* 0xBED */ u8 field_0xbed;
    /* 0xBEE */ u8 field_0xBEE[0xBF0 - 0xBEE];
    /* 0xBF0 */ cXyz field_0xbf0;
    /* 0xBFC */ cXyz field_0xbfc;
};

#endif /* D_A_OBJ_KGATE_H */
