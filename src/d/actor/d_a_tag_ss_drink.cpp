/**
 * d_a_tag_ss_drink.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_ss_drink.h"
#include "d/actor/d_a_player.h"
#include "d/d_procname.h"
#include "d/d_msg_object.h"
#include "d/actor/d_a_myna.h"

int daTag_SSDrink_c::create() {
    fopAcM_ct(this, daTag_SSDrink_c);
    field_0x5D2 = getTypeFromParam();
    setFlowNodeNumber(getFlowNodeNum());
    setValueNumber(getValue());
    field_0x5D4 = 0;
    initialize();
    return 4;
}

int daTag_SSDrink_c::Delete() {
    return 1;
}

int daTag_SSDrink_c::Execute() {
    if (chkEvent()) {
        if (mProcessFunc) {
            (this->*mProcessFunc)(0);
        }
        orderEvent();
    }
    setAttnPos();
    return 1;
}

int daTag_SSDrink_c::Draw() {
    return 1;
}

int daTag_SSDrink_c::getTypeFromParam() {
    switch (fopAcM_GetParamBit(this, 0, 4)) {
    case 0:
        field_0x5D3 = 0x69;
        return 0;
    case 1:
        field_0x5D3 = 0x62;
        return 1;
    case 2:
        field_0x5D3 = 0x63;
        return 2;
    case 3:
        field_0x5D3 = 0x64;
        return 3;
    case 4:
        field_0x5D3 = 0x68;
        return 4;
    case 5:
        field_0x5D3 = 0x67;
        return 5;
    case 6:
        field_0x5D3 = 0x6A;
        return 6;
    default:
        field_0x5D3 = 0x60;
        return 7;
    }
}

u8 daTag_SSDrink_c::getSwitchFromParam() {
    return fopAcM_GetParamBit(this, 4, 8);
}

u16 daTag_SSDrink_c::getFlowNodeNum() {
    u16 num = home.angle.x;
    bool bVar1 = false;
    if (num == 0xFFFF || num == 0) {
        bVar1 = true;
    }

    u16 num2;
    if (!bVar1) {
        num2 = num;
    } else {
        num2 = 0xFFFF;
    }

    return num2 & 0xFFFF;
}

u16 daTag_SSDrink_c::getValue() {
    return fopAcM_GetParamBit(this, 12, 16);
}

void daTag_SSDrink_c::restart() {
    current.angle.set(0, home.angle.y, 0);
    shape_angle = current.angle;
    setProcess(&daTag_SSDrink_c::wait);
}

void daTag_SSDrink_c::initialize() {
    fopAcM_setCullSizeBox(this, -30.0f, -15.0f, -30.0f, 30.0f, 45.0f, 30.0f);
    attention_info.flags = 0;
    attention_info.distances[fopAc_attn_CARRY_e] = 6;
    fopAcM_OnCarryType(this, fopAcM_CARRY_SIDE);
    restart();
    Execute();
}

int daTag_SSDrink_c::checkProcess(ProcessFunc i_processFunc) {
    return mProcessFunc == i_processFunc;
}

int daTag_SSDrink_c::setProcess(ProcessFunc i_processFunc) {
    int retVal = 0;

    if (checkProcess(i_processFunc)) {
        return 0;
    } else {
        mEventType = 2;
        if (mProcessFunc) {
            retVal = (this->*mProcessFunc)(0);
        }
        mEventType = 0;
        mProcessFunc = i_processFunc;
        if (mProcessFunc) {
            retVal = (this->*mProcessFunc)(0);
        }
        mEventType = 1;
    }

    return retVal;
}

void daTag_SSDrink_c::setAttnPos() {
    attention_info.position = current.pos;
    eyePos = current.pos;
}

int daTag_SSDrink_c::chkEvent() {
    int retVal = 1;
    if (!dComIfGp_getEvent()->isOrderOK()) {
        retVal = 0;
        if (eventInfo.checkCommandTalk()) {
            if (!checkProcess(&daTag_SSDrink_c::talk) == 0) {
                retVal = (this->*mProcessFunc)(0);
            } else {
                if (dComIfGp_event_chkTalkXY() == 0 || dComIfGp_evmng_ChkPresentEnd() != 0) {
                    setProcess(&daTag_SSDrink_c::talk);
                }
            }
            return retVal;
        } else if (eventInfo.checkCommandCatch()) {
            if (field_0x5D4 == 0) {
                if (getParentPtr() != NULL) {
                    ((daMyna_c*)getParentPtr())->onEventFlag(0);
                }
                dMsgObject_addTotalPrice(getValueNumber());
                field_0x5D4 = 1;
            }
            return retVal;
        }
    }
    return retVal;
}

int daTag_SSDrink_c::orderEvent() {
    if (!dComIfGp_getLinkPlayer()->checkWolf() && fopAcM_isSwitch(this, getSwitchFromParam()) &&
        field_0x5D3 != 0x60 && getFlowNodeNum() != 0xFFFF)
    {
        attention_info.flags = (fopAc_AttnFlag_TALKREAD_e | fopAc_AttnFlag_SPEAK_e);
    } else {
        attention_info.flags = 0;
    }

    if (attention_info.flags == (fopAc_AttnFlag_TALKREAD_e | fopAc_AttnFlag_SPEAK_e)) {
        if (fopAcM_searchPlayerDistanceXZ(this) <= 160.0f && fopAcM_seenPlayerAngleY() <= 0x2000) {
            attention_info.distances[fopAc_attn_TALK_e] = 0x9D;
            attention_info.distances[fopAc_attn_SPEAK_e] = 0x9D;
            eventInfo.onCondition(1);
        }
    }

    return 1;
}

int daTag_SSDrink_c::wait(void* param_0) {
    switch (mEventType) {
    case 1:
        if (!eventInfo.checkCommandCatch()) {
            if (fopAcM_isSwitch(this, getSwitchFromParam())) {
                if (fopAcM_searchPlayerDistanceXZ(this) <= 160.0f &&
                    fopAcM_seenPlayerAngleY() <= 0x2000)
                {
                    fopAc_ac_c* player = dComIfGp_getPlayer(0);
                    cXyz local_28 = attention_info.position - player->attention_info.position;
                    dComIfGp_att_CatchRequest(this, field_0x5D3, 200.0f, local_28.y + 100.0f,
                                              local_28.y - 100.0f, 0x4000, 1);
                    eventInfo.onCondition(dEvtCnd_40_e);
                }
                if (field_0x5D4 != 0) {
                    field_0x5D4 = 0;
                }
            }
        }
    case 0:
    case 2:
    default:
        return 1;
    }
}

int daTag_SSDrink_c::talk(void* param_0) {
    int retVal = 0;
    switch (mEventType) {
    case 0:
        field_0x578.init(this, getFlowNodeNum(), 0, NULL);
        break;
    case 1:
        if (field_0x578.doFlow(this, NULL, 0) != 0) {
            dComIfGp_event_reset();
            setProcess(&daTag_SSDrink_c::wait);
            retVal = 1;
        }
        break;
    case 2:
    default:
        break;
    }
    return retVal;
}

static int daTag_SSDrink_Create(void* i_this) {
    return static_cast<daTag_SSDrink_c*>(i_this)->create();
}

static int daTag_SSDrink_Delete(void* i_this) {
    return static_cast<daTag_SSDrink_c*>(i_this)->Delete();
}

static int daTag_SSDrink_Execute(void* i_this) {
    return static_cast<daTag_SSDrink_c*>(i_this)->Execute();
}

static int daTag_SSDrink_Draw(void* i_this) {
    return static_cast<daTag_SSDrink_c*>(i_this)->Draw();
}

static int daTag_SSDrink_IsDelete(void* i_this) {
    return 1;
}

daTag_SSDrink_c::~daTag_SSDrink_c() {}

void daObj_SSBase_c::setSoldOut() {
    /* empty function */
}

static actor_method_class daTag_SSDrink_MethodTable = {
    (process_method_func)daTag_SSDrink_Create,  (process_method_func)daTag_SSDrink_Delete,
    (process_method_func)daTag_SSDrink_Execute, (process_method_func)daTag_SSDrink_IsDelete,
    (process_method_func)daTag_SSDrink_Draw,
};

actor_process_profile_definition g_profile_TAG_SSDRINK = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_TAG_SSDRINK,
    &g_fpcLf_Method.base,
    sizeof(daTag_SSDrink_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x46,
    &daTag_SSDrink_MethodTable,
    0x64100,
    5,
    fopAc_CULLBOX_CUSTOM_e,
};
