#ifndef D_A_OBJ_TOBYHOUSE_H
#define D_A_OBJ_TOBYHOUSE_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjTobyHouse_c
 * @brief Fyer's House
 *
 * @details
 *
 */
class daObjTobyHouse_c : public dBgS_MoveBgActor {
public:
    typedef void (daObjTobyHouse_c::*actionFunc)();

    enum Action {
        ACTION_WAIT,
        ACTION_ORDER_EVENT,
        ACTION_EVENT,
        ACTION_DEAD,
    };

    enum Type {
        TYPE_0,
        TYPE_1,
    };

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    void action();
    void actionWait();
    void actionOrderEvent();
    void actionEvent();
    void actionDead();
    void demoProc();
    void sceneChange();
    int Draw();
    BOOL checkLODModel();
    int Delete();

    bool checkWater() { return fopAcM_GetParamBit(this, 31, 1); }
    void setAction(u8 action) { mAction = action; }

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mModel;
    /* 0x5AC */ J3DModel* mLODModel;
    /* 0x5B0 */ mDoExt_bckAnm* mBcks[3];
    /* 0x5BC */ mDoExt_bckAnm* mActiveBck;
    /* 0x5C0 */ int field_0x5c0;
    /* 0x5C4 */ int field_0x5c4;
    /* 0x5C8 */ int field_0x5c8;
    /* 0x5CC */ int mStaffId;
    /* 0x5D0 */ int mEventIdIdx;
    /* 0x5D4 */ s16 mEventIds[2];
    /* 0x5D8 */ u16 mBirlPJnt;
    /* 0x5DA */ u16 mBirlCannonJnt;
    /* 0x5DC */ u8 field_0x5dc;
    /* 0x5DD */ u8 mAction;
    /* 0x5DE */ u8 field_0x5de;
    /* 0x5E0 */ s16 field_0x5e0;
    /* 0x5E2 */ u8 field_0x5e2;
    /* 0x5E3 */ u8 field_0x5e3;
    /* 0x5E4 */ u8 field_0x5e4;
    /* 0x5E5 */ u8 mType;
    /* 0x5E8 */ f32 mHeightOffset;
    /* 0x5EC */ u16 mDemoTimer;
    /* 0x5F0 */ JPABaseEmitter* field_0x5f0;
    /* 0x5F4 */ JPABaseEmitter* field_0x5f4;
    /* 0x5F8 */ JPABaseEmitter* field_0x5f8;
    /* 0x5FC */ JPABaseEmitter* field_0x5fc;
};

STATIC_ASSERT(sizeof(daObjTobyHouse_c) == 0x600);

#if DEBUG

class daObjTobyHouse_HIO_c : public mDoHIO_entry_c {
public:
    daObjTobyHouse_HIO_c();
    ~daObjTobyHouse_HIO_c() {}
    void genMessage(JORMContext* ctx);

    /* 0x06 */ u8 mHawkeyeMediumLOD;
    /* 0x07 */ u8 field_0x07;
    /* 0x08 */ u8 mDrawCollision;
    /* 0x0A */ s16 mShakingCycle;
    /* 0x0C */ f32 mShakingAmplitude;
    /* 0x10 */ f32 mLODDistance;
};

#endif


#endif /* D_A_OBJ_TOBYHOUSE_H */
