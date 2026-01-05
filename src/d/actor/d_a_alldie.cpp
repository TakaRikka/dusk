/**
 * d_a_alldie.cpp
 * Activates a switch when all enemies are defeated 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_alldie.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

u8 daAlldie_c::getEventNo() {
    return fopAcM_GetParam(this) >> 0x18;
}

u8 daAlldie_c::getSwbit() {
    return fopAcM_GetParam(this) >> 0x8;
}

int daAlldie_c::actionWait() {
    return 1;
}

int daAlldie_c::actionCheck() {
    if (fopAcM_myRoomSearchEnemy(fopAcM_GetRoomNo(this)) == NULL) {
        mAction = ACT_TIMER;
        mTimer = 65;
    }

    return 1;
}

int daAlldie_c::actionTimer() {
    if (fopAcM_myRoomSearchEnemy(fopAcM_GetRoomNo(this)) != NULL) {
        mAction = ACT_CHECK;
    } else {
        if (mTimer > 0) {
            mTimer--;
        } else {
            if (mEventIdx == -1) {
                mAction = ACT_WAIT;
            } else {
                mAction = ACT_ORDER;
            }

            dComIfGs_onSwitch(getSwbit(), fopAcM_GetRoomNo(this));
        }
    }

    return 1;
}

int daAlldie_c::actionOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        mAction = ACT_EVENT;
    } else {
        fopAcM_orderOtherEventId(this, mEventIdx, getEventNo(), -1, 0, 1);
    }

    return 1;
}

int daAlldie_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mEventIdx)) {
        dComIfGp_getEvent()->reset();

        if (mNextEventIdx != -1) {
            mAction = ACT_NEXT;
            fopAcM_orderOtherEventId(this, mNextEventIdx, mMapToolID, -1, 0, 1);
        } else {
            mAction = ACT_WAIT;
            mMapToolID = -1;
        }
    }

    return 1;
}

int daAlldie_c::actionNext() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        mEventIdx = mNextEventIdx;
        s8 roomNo = fopAcM_GetRoomNo(this);

        mNextEventIdx = -1;
        dStage_MapEvent_dt_c* map_evt = dEvt_control_c::searchMapEventData(mMapToolID, roomNo);

        if (map_evt != NULL) {
            mMapToolID = map_evt->field_0x5;
            mNextEventIdx = dComIfGp_getEventManager().getEventIdx(this, mMapToolID);
        } else {
            mMapToolID = -1;
        }

        mAction = ACT_EVENT;
        actionEvent();
    } else {
        fopAcM_orderOtherEventId(this, mNextEventIdx, mMapToolID, -1, 0, 1);
    }

    return 1;
}

int daAlldie_c::execute() {
    switch (mAction) {
    case ACT_CHECK:
        actionCheck();
        break;
    case ACT_TIMER:
        actionTimer();
        break;
    case ACT_ORDER:
        actionOrder();
        break;
    case ACT_EVENT:
        actionEvent();
        break;
    case ACT_NEXT:
        actionNext();
        break;
    default:
        actionWait();
        break;
    }

    return 1;
}

static int daAlldie_Draw(daAlldie_c*) {
    return 1;
}

static int daAlldie_Execute(daAlldie_c* i_this) {
    i_this->execute();
    return 1;
}

static int daAlldie_IsDelete(daAlldie_c*) {
    return 1;
}

static int daAlldie_Delete(daAlldie_c* i_this) {
    i_this->~daAlldie_c();
    return 1;
}

int daAlldie_c::create() {
    fopAcM_ct(this, daAlldie_c);

    s8 roomNo = fopAcM_GetRoomNo(this);

    if (!dComIfGs_isSwitch(getSwbit(), fopAcM_GetRoomNo(this))) {
        mAction = ACT_CHECK;
    } else {
        mAction = ACT_WAIT;
    }

    shape_angle.z = 0;
    shape_angle.x = 0;
    current.angle.z = 0;
    current.angle.x = 0;

    mEventIdx = dComIfGp_getEventManager().getEventIdx(this, getEventNo());
    mMapToolID = -1;
    mNextEventIdx = -1;

    dStage_MapEvent_dt_c* map_evt = dEvt_control_c::searchMapEventData(getEventNo(), roomNo);
    if (map_evt != NULL) {
        mMapToolID = map_evt->field_0x5;
        mNextEventIdx = dComIfGp_getEventManager().getEventIdx(this, mMapToolID);
    }

    eventInfo.setEventId(mEventIdx);
    eventInfo.setMapToolId(getEventNo());

    return cPhs_COMPLEATE_e;
}

static int daAlldie_Create(fopAc_ac_c* i_this) {
    return static_cast<daAlldie_c*>(i_this)->create();
}

static actor_method_class l_daAlldie_Method = {
    (process_method_func)daAlldie_Create,
    (process_method_func)daAlldie_Delete,
    (process_method_func)daAlldie_Execute,
    (process_method_func)daAlldie_IsDelete,
    (process_method_func)daAlldie_Draw,
};

actor_process_profile_definition g_profile_ALLDIE = {
    fpcLy_CURRENT_e,
    2,
    fpcPi_CURRENT_e,
    PROC_ALLDIE,
    &g_fpcLf_Method.base,
    sizeof(daAlldie_c),
    0,
    0,
    &g_fopAc_Method.base,
    278,
    &l_daAlldie_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_6_e,
};
