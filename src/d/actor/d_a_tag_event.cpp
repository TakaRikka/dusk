#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_event.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_horse.h"

static fopAc_ac_c* daTag_getBk(u32 param_0) {
    return fopAcM_searchFromName("Bk", 0xF, param_0);
}

u8 daTag_Event_c::getEventNo() {
    return fopAcM_GetParam(this) >> 0x18;
}

u8 daTag_Event_c::getSwbit() {
    return fopAcM_GetParam(this) >> 0x8;
}

u8 daTag_Event_c::getSwbit2() {
    return fopAcM_GetParam(this) >> 0x10;
}

u8 daTag_Event_c::getType() {
    return fopAcM_GetParam(this);
}

u16 daTag_Event_c::getInvalidEventFlag() {
    return home.angle.x & 0x7FFF;
}

u16 daTag_Event_c::getAreaType() {
    return home.angle.x & 0x8000;
}

u16 daTag_Event_c::getValidEventFlag() {
    return home.angle.z;
}

BOOL daTag_Event_c::horseRodeo() {
    if (getType() == 5) {
        return true;
    } else {
        return false;
    }
}

BOOL daTag_Event_c::arrivalTerms() {
    int swbit2 = getSwbit2();
    if (swbit2 != 0xFF) {
        if (!dComIfGs_isSwitch(swbit2, fopAcM_GetRoomNo(this))) {
            return false;
        }
    }

    u16 invalid_flag = getInvalidEventFlag();
    if (invalid_flag != 0x7FFF && invalid_flag != 0 &&
        dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[invalid_flag]))
    {
        return false;
    }

    u16 valid_flag = getValidEventFlag();
    if (valid_flag != 0xFFFF && valid_flag != 0 &&
        !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[valid_flag]))
    {
        return false;
    }

    return true;
}

void daTag_Event_c::demoInitProc() {
    field_0x56e = 0;
}

void daTag_Event_c::demoEndProc() {}

void daTag_Event_c::demoProc() {}

int daTag_Event_c::create() {
    fopAcM_ct(this, daTag_Event_c);

    int swbit = getSwbit();
    int room_no = fopAcM_GetRoomNo(this);

    mEventIdx = dComIfGp_getEventManager().getEventIdx(this, getEventNo());
    mMapToolId = -1;
    mMapEventIdx = -1;

    dStage_MapEvent_dt_c* event_data = dEvt_control_c::searchMapEventData(getEventNo(), room_no);
    if (event_data != NULL) {
        mMapToolId = event_data->field_0x5;
        mMapEventIdx = dComIfGp_getEventManager().getEventIdx(this, mMapToolId);
    }

    eventInfo.setEventId(mEventIdx);
    eventInfo.setMapToolId(getEventNo());

    if (mEventIdx != -1 || horseRodeo() && (swbit == 0xFF || !dComIfGs_isSwitch(swbit, room_no)))
    {
        setActio(ACTION_ARRIVAL);
    } else {
        setActio(ACTION_WAIT);
    }

    shape_angle.z = 0;
    shape_angle.x = 0;
    current.angle.z = 0;
    current.angle.x = 0;

    scale.x *= 100.0f;
    scale.y *= 100.0f;
    scale.z *= 100.0f;

    if (horseRodeo()) {
            /* Main Event - Epona rescued flag */
        if (dComIfGs_isEventBit(dSv_event_flag_c::M_023)) {
            return cPhs_ERROR_e;
        }

        scale.x *= 10.0f;
        scale.y *= 10.0f;
        scale.z *= 10.0f;
    }

    return cPhs_COMPLEATE_e;
}

int daTag_Event_c::actionNext() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        mEventIdx = mMapEventIdx;
        mMapEventIdx = -1;

        dStage_MapEvent_dt_c* event_data =
            dEvt_control_c::searchMapEventData(mMapToolId, fopAcM_GetRoomNo(this));
        if (event_data != NULL) {
            mMapToolId = event_data->field_0x5;
            mMapEventIdx = dComIfGp_getEventManager().getEventIdx(this, mMapToolId);
        } else {
            mMapToolId = -1;
        }

        setActio(ACTION_EVENT);
        actionEvent();
    } else {
        fopAcM_orderOtherEventId(this, mMapEventIdx, mMapToolId, 0xFFFF, 0, 1);
    }

    return 1;
}

int daTag_Event_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mEventIdx)) {
        dComIfGp_event_reset();

        if (mMapEventIdx != -1) {
            setActio(ACTION_NEXT);
            fopAcM_orderOtherEventId(this, mMapEventIdx, mMapToolId, 0xFFFF, 0, 1);
        } else {
            setActio(ACTION_WAIT);
            demoEndProc();
            mMapToolId = -1;
        }
    } else {
        demoProc();
    }

    return 1;
}

int daTag_Event_c::actionReady() {
    int swbit = getSwbit();

    if (eventInfo.checkCommandDemoAccrpt()) {
        demoInitProc();
        setActio(ACTION_EVENT);
        actionEvent();

        if (swbit != 0xFF) {
            dComIfGs_onSwitch(swbit, fopAcM_GetRoomNo(this));
        }

        if (horseRodeo() && dComIfGp_getHorseActor() != NULL) {
            dComIfGp_getHorseActor()->onRodeoMode();
        }
    } else {
        if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(this))) {
            setActio(ACTION_WAIT);
        } else {
            fopAcM_orderOtherEventId(this, mEventIdx, getEventNo(), 0xFFFF, 0, 1);
        }
    }

    return 1;
}

BOOL daTag_Event_c::checkArea() {
    cXyz pos;
    daPy_py_c* player = dComIfGp_getLinkPlayer();

    if (getAreaType() == 0x8000) {
        pos = player->current.pos;

        cXyz start(current.pos.x - scale.x * 0.5f, current.pos.y, current.pos.z - scale.z * 0.5f);
        cXyz end(current.pos.x + scale.x * 0.5f, current.pos.y + scale.y,
                 current.pos.z + scale.z * 0.5f);

        if (start.x <= pos.x && pos.x <= end.x && start.y <= pos.y && pos.y <= end.y &&
            start.z <= pos.z && pos.z <= end.z)
        {
            return true;
        }
    } else {
        pos = player->current.pos - current.pos;

        if (pos.y < 0.0f) {
            pos.y = -pos.y;
        }

        if (pos.abs2XZ() < scale.x * scale.x && pos.y <= scale.y) {
            return true;
        }
    }

    return false;
}

int daTag_Event_c::actionHunt() {
    int swbit = getSwbit();

    if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(this))) {
        setActio(ACTION_WAIT);
    } else if (arrivalTerms() && checkArea()) {
        setActio(ACTION_READY);
        fopAcM_orderOtherEventId(this, mEventIdx, getEventNo(), 0xFFFF, 0, 1);
    }

    return 1;
}

int daTag_Event_c::actionArrival() {
    setActio(ACTION_HUNT);
    actionHunt();
    return 1;
}

int daTag_Event_c::actionWait() {
    return true;
}

int daTag_Event_c::actionHunt2() {
    int swbit = getSwbit();

    if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(this))) {
        setActio(ACTION_WAIT);
    } else if (arrivalTerms() && daTag_getBk(field_0x573) == NULL) {
        if (mHunt2Timer > 0) {
            mHunt2Timer--;
        } else {
            setActio(ACTION_READY);
            fopAcM_orderOtherEventId(this, mEventIdx, getEventNo(), 0xFFFF, 0, 1);
        }
    } else {
        mHunt2Timer = 65;
    }

    return 1;
}

int daTag_Event_c::execute() {
    if (home.roomNo != dComIfGp_roomControl_getStayNo()) {
        return 0;
    }

    switch (mAction) {
    case ACTION_ARRIVAL:
        actionArrival();
        break;
    case ACTION_HUNT:
        actionHunt();
        break;
    case ACTION_HUNT2:
        actionHunt2();
        break;
    case ACTION_READY:
        actionReady();
        break;
    case ACTION_NEXT:
        actionNext();
        break;
    case ACTION_EVENT:
        actionEvent();
        break;
    default:
        actionWait();
    }

    return 1;
}

int daTag_Event_c::draw() {
    return 1;
}

static int daTag_Event_Draw(daTag_Event_c* i_this) {
    return i_this->draw();
}

static int daTag_Event_Execute(daTag_Event_c* i_this) {
    i_this->execute();
    return 1;
}

static int daTag_Event_IsDelete(daTag_Event_c* i_this) {
    return 1;
}

static int daTag_Event_Delete(daTag_Event_c* i_this) {
    i_this->~daTag_Event_c();
    return 1;
}

static int daTag_Event_Create(fopAc_ac_c* i_this) {
    return static_cast<daTag_Event_c*>(i_this)->create();
}

static actor_method_class l_daTag_Event_Method = {
    (process_method_func)daTag_Event_Create,  (process_method_func)daTag_Event_Delete,
    (process_method_func)daTag_Event_Execute, (process_method_func)daTag_Event_IsDelete,
    (process_method_func)daTag_Event_Draw,
};

actor_process_profile_definition g_profile_TAG_EVENT = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_TAG_EVENT,
    &g_fpcLf_Method.base,
    sizeof(daTag_Event_c),
    0,
    0,
    &g_fopAc_Method.base,
    281,
    &l_daTag_Event_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_6_e,
};
