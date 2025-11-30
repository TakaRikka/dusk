/**
 * d_a_tag_evtmsg.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_evtmsg.h"
#include "d/actor/d_a_npc4.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

char* daTag_EvtMsg_c::mEvtCutNameList[] = {"", "TALK", "WAIT"};

EvtCutFunc daTag_EvtMsg_c::mEvtCutList[] = {
    NULL,
    &daTag_EvtMsg_c::ECut_talk,
    &daTag_EvtMsg_c::ECut_wait,
};

static char* l_myName = "EvtMsg";

int daTag_EvtMsg_c::create() {
    fopAcM_ct(this, daTag_EvtMsg_c);

    scale.x *= 100.0f;
    scale.y *= 100.0f;
    scale.z = scale.x;

    if (home.angle.z != 0xFFFF) {
        mFlowID = home.angle.z;
    } else {
        mFlowID = -1;
    }

    mEventID = -1;
    field_0x572 = 0;

    return isDelete() == TRUE ? cPhs_ERROR_e : cPhs_COMPLEATE_e;
}

int daTag_EvtMsg_c::Delete() {
    return 1;
}

int daTag_EvtMsg_c::Execute() {
    dEvent_manager_c& evt_mng = dComIfGp_getEventManager();

    if (home.roomNo == dComIfGp_roomControl_getStayNo()) {
        if (dComIfGp_event_runCheck() != 0) {
            int staff_id;
            BOOL reset = false;
            if (eventInfo.checkCommandTalk()) {
                if (field_0x572 != 0) {
                    mMsgFlow.init(this, mFlowID, 0, NULL);
                }

                if (mMsgFlow.doFlow(this, NULL, 0)) {
                    reset = true;
                }
            } else if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1 &&
                       evt_mng.endCheck(mEventID))
            {
                mEventID = -1;
                reset = true;
            } else {
                staff_id = evt_mng.getMyStaffId(l_myName, this, -1);
                if (staff_id != -1) {
                    int evtCutNo = evt_mng.getMyActIdx(staff_id, mEvtCutNameList, 3, 0, 0);
                    if ((this->*mEvtCutList[evtCutNo])(staff_id) != 0) {
                        evt_mng.cutEnd(staff_id);
                    }
                }
            }

            if (reset) {
                if (getOffSwBit() != 0xFF) {
                    dComIfGs_onSwitch(getOffSwBit(), fopAcM_GetRoomNo(this));
                }

                dComIfGp_event_reset();
            }
        } else if (isDelete()) {
            fopAcM_delete(this);
        } else if (mFlowID != -1) {
            if (chkPointInArea(daPy_getPlayerActorClass()->current.pos)) {
                if (getProcType() == 0) {
                    s16 var_r0 = (s16)(fopAcM_searchPlayerAngleY(this) + 0x7FFF);
                    var_r0 = var_r0 - daPy_getPlayerActorClass()->current.angle.y;

                    if (var_r0 < 0) {
                        var_r0 = -var_r0;
                    }

                    if (var_r0 <= 0x1000) {
                        mEventID = evt_mng.getEventIdx(this, "DEFAULT_EVT_TALK", 0xFF);
                        fopAcM_orderOtherEventId(this, mEventID, 0xFF, 0xFFFF, 0, 1);
                    }
                } else {
                    eventInfo.onCondition(dEvtCnd_CANTALK_e);
                    fopAcM_orderSpeakEvent(this, 0, 0);
                }
            }
        }
        attention_info.flags = 0;
        attention_info.position = current.pos;
        eyePos = attention_info.position;
        return 1;
    }

    return 0;
}

int daTag_EvtMsg_c::Draw() {
    return 1;
}

u32 daTag_EvtMsg_c::getOnEvtBit() {
    u32 bit = fopAcM_GetParam(this) & 0xFFF;
    if (bit != 0xFFF) {
        return bit;
    }

    return -1;
}

u32 daTag_EvtMsg_c::getOffEvtBit() {
    u32 bit = (fopAcM_GetParam(this) >> 0xC) & 0xFFF;
    if (bit != 0xFFF) {
        return bit;
    }

    return -1;
}

u8 daTag_EvtMsg_c::getOnSwBit() {
    return home.angle.x & 0xFF;
}

u8 daTag_EvtMsg_c::getOffSwBit() {
    return (home.angle.x >> 8) & 0xFF;
}

u8 daTag_EvtMsg_c::getProcType() {
    return fopAcM_GetParam(this) & 0x2000000;
}

BOOL daTag_EvtMsg_c::isDelete() {
    BOOL ret = false;

    if (getOffEvtBit() != -1) {
        if (daNpcF_chkEvtBit(getOffEvtBit())) {
            ret = true;
        }
    }

    if (getOffSwBit() != 0xFF) {
        if (dComIfGs_isSwitch(getOffSwBit(), fopAcM_GetRoomNo(this))) {
            ret = true;
        }
    }

    return ret;
}

BOOL daTag_EvtMsg_c::chkPointInArea(cXyz param_0) {
    if (getOnEvtBit() == -1 || (getOnEvtBit() != -1 && !daNpcF_chkEvtBit(getOnEvtBit())) ||
        getOnSwBit() == 0xFF ||
        (getOnSwBit() != 0xFF && !dComIfGs_isSwitch(getOnSwBit(), fopAcM_GetRoomNo(this))))
    {
        return daNpcF_chkPointInArea(param_0, current.pos, scale, shape_angle.y);
    }

    return 0;
}

BOOL daTag_EvtMsg_c::ECut_talk(int i_staffID) {
    if (dComIfGp_getEventManager().getIsAddvance(i_staffID)) {
        mMsgFlow.init(this, mFlowID, 0, NULL);
    }

    if (mMsgFlow.doFlow(this, NULL, 0)) {
        return true;
    }

    return false;
}

BOOL daTag_EvtMsg_c::ECut_wait(int i_staffID) {
    dEvent_manager_c& evt_mng = dComIfGp_getEventManager();
    int timer = 0;

    int* data_p = dComIfGp_evmng_getMyIntegerP(i_staffID, "timer");
    if (data_p != NULL) {
        timer = *data_p;
    }

    if (evt_mng.getIsAddvance(i_staffID)) {
        mTimer = timer;
    }

    if (cLib_calcTimer(&mTimer) == 0) {
        return 1;
    }

    return 0;
}

static int daTag_EvtMsg_Create(void* i_this) {
    return static_cast<daTag_EvtMsg_c*>(i_this)->create();
}

static int daTag_EvtMsg_Delete(void* i_this) {
    return static_cast<daTag_EvtMsg_c*>(i_this)->Delete();
}

static int daTag_EvtMsg_Execute(void* i_this) {
    return static_cast<daTag_EvtMsg_c*>(i_this)->Execute();
}

static int daTag_EvtMsg_Draw(void* i_this) {
    return static_cast<daTag_EvtMsg_c*>(i_this)->Draw();
}

static int daTag_EvtMsg_IsDelete(void* i_this) {
    return 1;
}

daTag_EvtMsg_c::~daTag_EvtMsg_c() {}

static actor_method_class daTag_EvtMsg_MethodTable = {
    (process_method_func)daTag_EvtMsg_Create,  (process_method_func)daTag_EvtMsg_Delete,
    (process_method_func)daTag_EvtMsg_Execute, (process_method_func)daTag_EvtMsg_IsDelete,
    (process_method_func)daTag_EvtMsg_Draw,
};

extern actor_process_profile_definition g_profile_TAG_EVTMSG = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_TAG_MSG,
    &g_fpcLf_Method.base,
    sizeof(daTag_EvtMsg_c),
    0,
    0,
    &g_fopAc_Method.base,
    285,
    &daTag_EvtMsg_MethodTable,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
