#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_evt.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_com_static.h"
#include "d/d_save.h"
#include "f_op/f_op_actor_mng.h"

static char* l_evtNameList[] = {
    NULL,
    "JUMP_DEMOSTAGE",
};

static char* l_resFileName = "TAGEVT";

char* daTag_Evt_c::mEvtCutList[] = {
    "WAIT",
    "TALK",
    "NEXT",
};

int daTag_Evt_c::create() {
    fopAcM_ct(this, daTag_Evt_c);
    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, l_resFileName);
    if (phase == cPhs_COMPLEATE_e) {
        eventInfo.setArchiveName(l_resFileName);
        strcpy(field_0x568, "TagEvt");
        getParam();
        field_0x572 = -1;
    }
    return phase;
}

int daTag_Evt_c::destroy() {
    dComIfG_resDelete(&mPhase, l_resFileName);
    return 1;
}

int daTag_Evt_c::execute() {
    BOOL bVar = true;
    eyePos.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    attention_info.position = eyePos;
    fopAcM_SetRoomNo(this, dComIfGp_roomControl_getStayNo());
    if (field_0x5E4 == 0 || field_0x5E4 == 1) {
        if (dComIfGp_event_runCheck()) {
            bVar = false;
            int iVar1 = dComIfGp_evmng_getMyStaffId(field_0x568, 0, 0);
            if (!eventInfo.checkCommandTalk()) {
                if (eventInfo.checkCommandDemoAccrpt()) {
                    if (dComIfGp_getEventManager().endCheck(field_0x572)) {
                        dComIfGp_event_reset();
                        field_0x570 = 0;
                        field_0x572 = -1;
                        bVar = true;
                    } else {
                        if (iVar1 != -1) {
                            int actIdx = dComIfGp_getEventManager().getMyActIdx(
                                iVar1, mEvtCutList, 3, 0, 0);
                            int iVar2 = 0;
                            switch (actIdx) {
                            case 0:
                                iVar2 = doEvtCutWait(iVar1);
                                break;
                            case 1:
                                iVar2 = doEvtCutTalk(iVar1);
                                break;
                            case 2:
                                iVar2 = doEvtCutNext(iVar1);
                            }
                            if (iVar2 != 0) {
                                dComIfGp_getEventManager().cutEnd(iVar1);
                            }
                        }
                    }
                }
            } else if (field_0x5DC != 0) {
                if (mMsgFlow.doFlow(this, NULL, 0) != 0) {
                    dComIfGp_event_reset();
                    u16 eventId = mMsgFlow.getEventId(NULL);
                    if (eventId != 0) {
                        daNpcMsg_setEvtNum(eventId);
                        field_0x570 = 1;
                        bVar = true;
                    } else {
                        fopAcM_delete(this);
                    }
                }
            } else {
                mMsgFlow.init(this, field_0x574, 0, NULL);
                field_0x5DC = 1;
            }
        }
        if (!isDelete() && cLib_calcTimer(&field_0x5D0) == 0) {
            cXyz var1 = daPy_getPlayerActorClass()->current.pos - current.pos;
            if (var1.absXZ() < scale.x && -scale.y < var1.y && var1.y < scale.y) {
                field_0x570 = 1;
            }
        }
        if (bVar != 0 && field_0x570 != 0) {
            field_0x572 = dComIfGp_getEventManager().getEventIdx(this, l_evtNameList[field_0x570], -1);
            fopAcM_orderOtherEventId(this, field_0x572, -1, -1, 0, 1);
        }
    } else if (field_0x5E4 == 2 && isDelete()) {
        fopAcM_delete(this);
    }
    return 1;
}

int daTag_Evt_c::draw() {
    return 1;
}

int daTag_Evt_c::getParam() {
    field_0x5E0 = fopAcM_GetParam(this) & 0xFFF;
    field_0x5E2 = (fopAcM_GetParam(this) >> 0xC) & 0xFFF;
    if ((fopAcM_GetParam(this) & 0xF000000) != 0xF000000) {
        field_0x5D8 = (fopAcM_GetParam(this) & 0xF000000) >> 0x18;
    } else {
        field_0x5D8 = -1;
    }

    field_0x5E4 = fopAcM_GetParam(this) >> 0x1E;
    field_0x5DD = home.angle.x;
    field_0x5DE = (u16)home.angle.x >> 8;
    field_0x5D4 = home.angle.z & 0xFF;
    if ((home.angle.z & 0xFF00) != 0xFF00) {
        field_0x5D0 = ((home.angle.z & 0xFF00) >> 8) * 0x1E;
    } else {
        field_0x5D0 = 0;
    }
    field_0x574 = -1;
    scale.x *= 100.0f;
    scale.y *= 100.0f;
    scale.z *= 100.0f;
    if (field_0x5E4 == 1) {
        scale.x *= 10.0f;
        scale.y *= 10.0f;
        scale.z *= 10.0f;
    }
    home.angle.x = 0;
    home.angle.z = 0;
    return 1;
}

int daTag_Evt_c::isDelete() {
    if (field_0x5E0 != 0xFFF || field_0x5E2 != 0xFFF) {
        if (field_0x5E0 == 0xFFF ||
            (field_0x5E0 != 0xFFF &&
             dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[field_0x5E0]) != 0))
        {
            if (field_0x5E2 != 0xFFF) {
                return dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[field_0x5E2]);
            } else {
                return false;
            }
        } else {
            return true;
        }
    } else if (field_0x5DD != 0xFF || field_0x5DE != 0xFF) {
        if (field_0x5DD == 0xFF ||
            (field_0x5DD != 0xFF && dComIfGs_isSwitch(field_0x5DD, fopAcM_GetRoomNo(this)) != 0))
        {
            if (field_0x5DE != 0xFFF) {
                return dComIfGs_isSwitch(field_0x5DE, fopAcM_GetRoomNo(this));
            } else {
                return false;
            }
        } else {
            return true;
        }
    } else {
        return false;
    }
}

int daTag_Evt_c::doEvtCutWait(int param_0) {
    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        int* pTimer = dComIfGp_evmng_getMyIntegerP(param_0, "timer");
        mTimer = (pTimer == NULL ? 0 : *pTimer);
    }
    return cLib_calcTimer(&mTimer) == 0;
}

int daTag_Evt_c::doEvtCutTalk(int param_0) {
    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        mMsgFlow.init(this, *dComIfGp_evmng_getMyIntegerP(param_0, "flowNodeNo"), 0, NULL);
        return 0;
    } else {
        return mMsgFlow.doFlow(this, NULL, 0);
    }
}

int daTag_Evt_c::doEvtCutNext(int param_0) {
    if (dComIfGp_getEventManager().getIsAddvance(param_0)) {
        if ((field_0x5E4 == 0 || field_0x5E4 == 1) && field_0x5DE != 0xFF) {
            dComIfGs_onSwitch(field_0x5DE, fopAcM_GetRoomNo(this));
        }
        dStage_changeScene(field_0x5D4, 0.0f, 0, fopAcM_GetRoomNo(this), 0,
                           dComIfG_play_c::getLayerNo(0));
    }
    return 0;
}

static int daTag_Evt_Create(void* i_this) {
    return static_cast<daTag_Evt_c*>(i_this)->create();
}

static int daTag_Evt_Delete(void* i_this) {
    return static_cast<daTag_Evt_c*>(i_this)->destroy();
}

static int daTag_Evt_Execute(void* i_this) {
    return static_cast<daTag_Evt_c*>(i_this)->execute();
}

static int daTag_Evt_Draw(void* i_this) {
    return static_cast<daTag_Evt_c*>(i_this)->draw();
}

static int daTag_Evt_IsDelete(void* i_this) {
    return 1;
}

daTag_Evt_c::~daTag_Evt_c() {}

static actor_method_class daTag_Evt_MethodTable = {
    (process_method_func)daTag_Evt_Create,  (process_method_func)daTag_Evt_Delete,
    (process_method_func)daTag_Evt_Execute, (process_method_func)daTag_Evt_IsDelete,
    (process_method_func)daTag_Evt_Draw,
};

actor_process_profile_definition g_profile_TAG_EVT = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_TAG_EVT,
    &g_fpcLf_Method.base,
    sizeof(daTag_Evt_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x011C,
    &daTag_Evt_MethodTable,
    0x40000,
    0,
    fopAc_CULLBOX_CUSTOM_e,
};
