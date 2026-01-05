/**
 * d_a_andsw2.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_andsw2.h"
#include "d/d_com_inf_game.h"
#include "d/d_lib.h"
#include "d/d_procname.h"

u8 daAndsw2_c::getEventNo() {
    return home.angle.x & 0xFF;
}

u8 daAndsw2_c::getSwbit() {
    return fopAcM_GetParam(this) >> 0x10;
}

u8 daAndsw2_c::getSwbit2() {
    return fopAcM_GetParam(this) >> 0x18;
}

u8 daAndsw2_c::getType() {
    return fopAcM_GetParam(this) >> 0x8;
}

u8 daAndsw2_c::getTimer() {
    return home.angle.z & 0xFF;
}

u8 daAndsw2_c::getNum() {
    return fopAcM_GetParam(this) & 0xFF;
}

u8 daAndsw2_c::getTopSw() {
    if (getSwbit2() != 0xFF) {
        return getSwbit2();
    } else if (getSwbit() != 0xFF) {
        return getSwbit() + 1;
    } else {
        return 0xFF;
    }
}

u8 daAndsw2_c::getSwAns() {
    return (home.angle.x >> 8) & 0xFF;
}

BOOL daAndsw2_c::chkAllSw2() {
    int top_sw = getTopSw();
    int num = getNum();

    if (num == 0xFF || num == 0 || top_sw == 0xFF) {
        return false;
    }

    for (int i = 0; i < num; i++) {
        if (!dComIfGs_isSwitch(top_sw + i, fopAcM_GetRoomNo(this))) {
            return false;
        }
    }

    return true;
}

bool daAndsw2_c::chkSwStatus() {
    int top_sw = getTopSw();
    int num = getNum();
    u8 status = 0;

    if (num == 0xFF || num == 0 || top_sw == 0xFF) {
        return false;
    }

    for (int i = 0; i < num; i++) {
        status |= dComIfGs_isSwitch(top_sw + i, fopAcM_GetRoomNo(this)) << i;
    }

    if (status == getSwAns()) {
        return true;
    }

    return false;
}

bool daAndsw2_c::chkSwStatus2() {
    int top_sw = getTopSw();
    int num = getNum();

    if (num == 0xFF || num == 0 || top_sw == 0xFF) {
        return false;
    }

    for (int i = 0; i < num; i++) {
        if (dComIfGs_isSwitch(top_sw + i, fopAcM_GetRoomNo(this))) {
            return true;
        }
    }

    return false;
}

daAndsw2_c::~daAndsw2_c() {}

int daAndsw2_c::create() {
    int sw = getSwbit();
    fopAcM_ct(this, daAndsw2_c);

    switch (getType()) {
    case 0:
        if (sw == 0xFF || dComIfGs_isSwitch(sw, fopAcM_GetRoomNo(this))) {
            setActio(ACT_WAIT_e);
        } else {
            setActio(ACT_ON_ALL_e);
        }

        mEventIdx = dComIfGp_getEventManager().getEventIdx(this, getEventNo());
        break;
    case 1:
    case 6:
        if (sw == 0xFF) {
            setActio(ACT_WAIT_e);
        } else if (dComIfGs_isSwitch(sw, fopAcM_GetRoomNo(this))) {
            setActio(ACT_OFF_e);
        } else {
            setActio(ACT_ON_ALL_e);
        }

        mEventIdx = dComIfGp_getEventManager().getEventIdx(this, getEventNo());
        break;
    case 2:
        if (sw == 0xFF || dComIfGs_isSwitch(sw, fopAcM_GetRoomNo(this))) {
            setActio(ACT_WAIT_e);
        } else {
            setActio(ACT_ON_ALL_e);
        }

        mEventIdx = dComIfGp_getEventManager().getEventIdx(this, getEventNo());
        break;
    case 3:
        if (sw == 0xFF || dComIfGs_isSwitch(sw, fopAcM_GetRoomNo(this))) {
            setActio(ACT_OFF_e);
        } else {
            setActio(ACT_ON_ALL_e);
        }

        mEventIdx = dComIfGp_getEventManager().getEventIdx(this, getEventNo());
        break;
    case 4:
        setActio(ACT_ON_ALL_e);
        break;
    case 5:
        setActio(ACT_ON_ALL_e);
        mEventIdx = dComIfGp_getEventManager().getEventIdx(this, getEventNo());
        break;
    default:
        setActio(ACT_WAIT_e);
        break;
    }

    shape_angle.z = 0;
    shape_angle.x = 0;
    current.angle.z = 0;
    current.angle.x = 0;

    return cPhs_COMPLEATE_e;
}

static int daAndsw2_actionOnAll(daAndsw2_c* i_this) {
    u8 var_r29 = false;

    switch (i_this->getType()) {
    case 0:
    case 1:
    case 5:
    case 6: {
        BOOL temp_r0 = i_this->chkAllSw2() != 0;
        var_r29 = temp_r0;
        break;
    }
    case 2:
    case 3: {
        BOOL temp_r0_2 = i_this->chkSwStatus() != 0;
        var_r29 = temp_r0_2;
        break;
    }
    case 4:
        if (!dComIfGs_isSwitch(i_this->getSwbit(), fopAcM_GetRoomNo(i_this))) {
            BOOL temp_r0 = i_this->chkSwStatus2() != 0;
            var_r29 = temp_r0;
        }
        break;
    }

    if (var_r29) {
        if (i_this->getTimer() != 0xFF) {
            i_this->mTimer = i_this->getTimer() * 15;
            i_this->setActio(daAndsw2_c::ACT_TIMER_e);
        } else if (i_this->mEventIdx != -1 && i_this->getType() != 4) {
            i_this->setActio(daAndsw2_c::ACT_ORDER_e);
        } else {
            dComIfGs_onSwitch(i_this->getSwbit(), fopAcM_GetRoomNo(i_this));

            if (i_this->getType() == 1 || i_this->getType() == 3 || i_this->getType() == 6) {
                i_this->setActio(daAndsw2_c::ACT_OFF_e);
            } else if (i_this->getType() != 4) {
                i_this->setActio(daAndsw2_c::ACT_WAIT_e);
            }
        }
    }

    return 1;
}

static int daAndsw2_actionTimer(daAndsw2_c* i_this) {
    if ((i_this->getType() == 1 && !i_this->chkAllSw2()) ||
        (i_this->getType() == 3 && !i_this->chkSwStatus()) ||
        (i_this->getType() == 5 && !i_this->chkAllSw2()) ||
        (i_this->getType() == 6 && !i_this->chkAllSw2()))
    {
        i_this->setActio(daAndsw2_c::ACT_ON_ALL_e);
    } else if (i_this->mTimer > 0) {
        i_this->mTimer--;
    } else if (i_this->mEventIdx != -1) {
        i_this->setActio(daAndsw2_c::ACT_ORDER_e);
    } else {
        dComIfGs_onSwitch(i_this->getSwbit(), fopAcM_GetRoomNo(i_this));
        if (i_this->getType() == 1 || i_this->getType() == 5) {
            i_this->setActio(daAndsw2_c::ACT_WAIT_e);
        } else if (i_this->getType() == 3 || i_this->getType() == 6) {
            i_this->setActio(daAndsw2_c::ACT_OFF_e);
        }
    }

    return 1;
}

static int daAndsw2_actionOrder(daAndsw2_c* i_this) {
    if (i_this->eventInfo.checkCommandDemoAccrpt()) {
        i_this->setActio(daAndsw2_c::ACT_EVENT_e);
        dComIfGs_onSwitch(i_this->getSwbit(), fopAcM_GetRoomNo(i_this));
    } else if ((i_this->getType() == 1 && !i_this->chkAllSw2()) ||
               (i_this->getType() == 3 && !i_this->chkSwStatus()) ||
               (i_this->getType() == 5 && !i_this->chkAllSw2()) ||
               (i_this->getType() == 6 && !i_this->chkAllSw2()))
    {
        i_this->setActio(daAndsw2_c::ACT_ON_ALL_e);
    } else {
        if (i_this->getType() == 5) {
            u8 switch_no = dLib_getEventSwitchNo(i_this->getEventNo());
            if (fopAcM_isSwitch(i_this, switch_no)) {
                dComIfGs_onSwitch(i_this->getSwbit(), fopAcM_GetRoomNo(i_this));
                i_this->setActio(daAndsw2_c::ACT_OFF_e);
            }
        }

        fopAcM_orderOtherEventId(i_this, i_this->mEventIdx, i_this->getEventNo(), 0xFFFF, 0, 1);
    }

    return 1;
}

static int daAndsw2_actionEvent(daAndsw2_c* i_this) {
    if (dComIfGp_evmng_endCheck(i_this->mEventIdx)) {
        if (i_this->getType() == 1 || i_this->getType() == 3 || i_this->getType() == 5 ||
            i_this->getType() == 6)
        {
            i_this->setActio(daAndsw2_c::ACT_OFF_e);
        } else {
            i_this->setActio(daAndsw2_c::ACT_WAIT_e);
        }

        dComIfGp_event_reset();
    }

    return 1;
}

static int daAndsw2_actionOff(daAndsw2_c* i_this) {
    u8 var_r29 = false;

    switch (i_this->getType()) {
    case 0:
    case 1:
    case 5:
    case 6: {
        BOOL temp_r0 = i_this->chkAllSw2() != 0;
        var_r29 = temp_r0;
        break;
    }
    case 2:
    case 3: {
        BOOL temp_r0_2 = i_this->chkSwStatus() != 0;
        var_r29 = temp_r0_2;
        break;
    }
    case 4:
        break;
    }

    if (!var_r29) {
        i_this->setActio(daAndsw2_c::ACT_ON_ALL_e);
        dComIfGs_offSwitch(i_this->getSwbit(), fopAcM_GetRoomNo(i_this));
    }

    return 1;
}

static int daAndsw2_actionWait(daAndsw2_c* i_this) {
    return 1;
}

int daAndsw2_c::execute() {
    typedef int (*actionFunc)(daAndsw2_c*);
    static actionFunc l_action[] = {
        &daAndsw2_actionOnAll, &daAndsw2_actionTimer, &daAndsw2_actionOrder,
        &daAndsw2_actionEvent, &daAndsw2_actionOff,   &daAndsw2_actionWait,
    };

    (*l_action[mAction])(this);
    return 1;
}

static int daAndsw2_Execute(daAndsw2_c* i_this) {
    return i_this->execute();
}

static int daAndsw2_Delete(daAndsw2_c* i_this) {
    i_this->~daAndsw2_c();
    return 1;
}

static int daAndsw2_Create(fopAc_ac_c* i_this) {
    return static_cast<daAndsw2_c*>(i_this)->create();
}

static actor_method_class l_daAndsw2_Method = {
    (process_method_func)daAndsw2_Create,
    (process_method_func)daAndsw2_Delete,
    (process_method_func)daAndsw2_Execute,
};

actor_process_profile_definition g_profile_ANDSW2 = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_ANDSW2,            // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daAndsw2_c),     // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    303,                    // mPriority
    &l_daAndsw2_Method,     // sub_method
    0x44000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_6_e,      // cullType
};
