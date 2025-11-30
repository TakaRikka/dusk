#ifndef D_A_OBJ_STOPPER_H
#define D_A_OBJ_STOPPER_H

#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "d/d_particle.h"
#include "dolphin/types.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjStopper_c
 * @brief Stopper
 *
 * @details
 *
 */
class daObjStopper_c : public dBgS_MoveBgActor {
public:
    enum ActionType {
        ACTION_WAIT,
        ACTION_WAIT_ORDER_EVENT,
        ACTION_ORDER_EVENT,
        ACTION_EVENT,
        ACTION_DEAD,
        ACTION_WAIT_CLOSE,
        ACTION_CLOSE,
        ACTION_CLOSE_INIT,
    };

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int Execute(Mtx**);
    void action();
    void actionWait();
    void actionWaitOrderEvent();
    void actionOrderEvent();
    void actionEvent();
    void actionDead();
    void actionWaitClose();
    void actionCloseInit();
    void actionClose();
    void hint_action1();
    void hint_action2();
    void hint_action3();
    int Draw();
    int Delete();

    void setOpen() { mAction = ACTION_WAIT_CLOSE; }
    void setAction(u8 action) { mAction = action; }
    void startClose() { setAction(ACTION_CLOSE_INIT); }
    u8 getEventNo() { return fopAcM_GetParamBit(this, 16, 8); }
    u8 getEventNo2() { return fopAcM_GetParamBit(this, 24, 8); }
    u16 getEventNo3() { return field_0x9f8 & 0xff; }
    u8 getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwNo2() { return fopAcM_GetParamBit(this, 8, 8); }

private:
    /* 0x5A0 */ u8 field_0x5a0[8];  // unused
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ dBgS_ObjAcch mAcch;
    /* 0x784 */ dBgS_AcchCir mAcchCir;
    /* 0x7C4 */ dCcD_Stts field_0x7c4;  // unused
    /* 0x800 */ dCcD_Cyl field_0x800;   // unused
    /* 0x93C */ u8 mAction;
    /* 0x93D */ u8 field_0x93d;
    /* 0x93E */ u8 field_0x93e[6];  // unused
    /* 0x944 */ s16 mEventIDs[3];
    /* 0x94A */ u8 mMapToolIDs[3];
    /* 0x94D */ u8 field_0x94d;
    /* 0x94E */ u8 field_0x94e;
    /* 0x94F */ u8 field_0x94f;  // unused
    /* 0x950 */ f32 field_0x950;
    /* 0x954 */ u16 field_0x954;
    /* 0x956 */ u16 field_0x956;
    /* 0x958 */ f32 field_0x958;
    /* 0x95C */ u8 field_0x95c;
    /* 0x95D */ u8 field_0x95d;
    /* 0x95E */ u8 field_0x95e;
    /* 0x95F */ u8 field_0x95f;
    /* 0x960 */ s16 field_0x960;
    /* 0x962 */ u8 field_0x962[2];  // unused
    /* 0x964 */ f32 field_0x964;
    /* 0x968 */ f32 field_0x968;
    /* 0x96C */ f32 field_0x96c;
    /* 0x970 */ f32 field_0x970;
    /* 0x974 */ f32 field_0x974;
    /* 0x978 */ f32 field_0x978;
    /* 0x97C */ s16 field_0x97c;
    /* 0x97E */ s16 field_0x97e;
    /* 0x980 */ s16 field_0x980;
    /* 0x982 */ s16 field_0x982;
    /* 0x984 */ dPa_followEcallBack field_0x984[2];
    /* 0x9AC */ dPa_followEcallBack field_0x9ac[1];
    /* 0x9C0 */ dPa_followEcallBack field_0x9c0[2];
    /* 0x9E8 */ s16 field_0x9e8;
    /* 0x9EA */ s16 field_0x9ea;
    /* 0x9EC */ s16 field_0x9ec;
    /* 0x9EE */ s16 field_0x9ee;
    /* 0x9F0 */ csXyz field_0x9f0;
    /* 0x9F6 */ s16 field_0x9f6;
    /* 0x9F8 */ u16 field_0x9f8;
    /* 0x9FA */ s16 field_0x9fa;
    /* 0x9FC */ u8 field_0x9fc;
};

STATIC_ASSERT(sizeof(daObjStopper_c) == 0xA00);

#endif /* D_A_OBJ_STOPPER_H */
