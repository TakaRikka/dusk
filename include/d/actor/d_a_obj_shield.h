#ifndef D_A_OBJ_SHIELD_H
#define D_A_OBJ_SHIELD_H

#include "d/actor/d_a_itembase.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daItemShield_c
 * @brief Ordon Shield
 *
 * @details
 *
 */
class daItemShield_c : public daItemBase_c {
public:
    enum Status {
        STATUS_INIT,
        STATUS_WAIT,
        STATUS_ORDERGETDEMO,
        STATUS_GETDEMO,
    };

    enum Action {
        ACTION_WAIT_CAM_DEMO,
        ACTION_ORDER_CAM_DEMO,
        ACTION_CAM_DEMO,
        ACTION_CAM_DEMO_END,
    };

    typedef int (daItemShield_c::*actionFunc)();
    typedef void (daItemShield_c::*camActionFunc)();

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    virtual int __CreateHeap();
    int create();
    void bg_check();
    int action_proc_call();
    int actionInit();
    int actionWaitInit();
    int actionWait();
    int initActionOrderGetDemo();
    int actionOrderGetDemo();
    int actionGetDemo();
    void event_proc_call();
    void actionWaitCamDemo();
    void actionOrderCamDemo();
    void actionCamDemo();
    void actionCamDemoEnd();
    int execute();
    int draw();
    int _delete();

    u8 getSwBit2() { return (fopAcM_GetParam(this) >> 8) & 0xff; }
    u8 getSwBit() { return (fopAcM_GetParam(this) >> 16) & 0xff; }
    u8 getEvId() { return (fopAcM_GetParam(this) >> 24) & 0xff; }
    void setStatus(u8 status) { mStatus = status; }
    void setAction(u8 action) { mAction = action; }

private:
    /* 0x92C */ fpc_ProcID mItemId;
    /* 0x930 */ int mRotAngleCoeff;
    /* 0x934 */ u8 mStatus;
    /* 0x935 */ u8 field_0x935;
    /* 0x936 */ u8 field_0x936;
    /* 0x937 */ bool field_0x937;
    /* 0x938 */ cXyz field_0x938;
    /* 0x944 */ csXyz field_0x944;
    /* 0x94A */ u8 field_0x94a;
    /* 0x94B */ u8 mAction;
    /* 0x94C */ u8 field_0x94c;
    /* 0x94D */ u8 mEvId;
    /* 0x94E */ s16 mEventIdx;
    /* 0x950 */ s16 field_0x950;
    /* 0x952 */ s16 field_0x952;
    /* 0x954 */ s16 field_0x954;
};

STATIC_ASSERT(sizeof(daItemShield_c) == 0x958);


#endif /* D_A_OBJ_SHIELD_H */
