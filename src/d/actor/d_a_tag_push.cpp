#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_push.h"
#include "d/actor/d_a_npc4.h"
#include "d/d_procname.h"

void* daTag_Push_c::srchActor(void* param_0, void* param_1) {
    BOOL bVar1 = false;
    if (param_0 != NULL && fopAcM_IsActor(param_0)) {
        if (fopAcM_IsExecuting(fopAcM_GetID(param_0))) {
            switch (((daTag_Push_c*)param_1)->getId()) {
            case 1:
                bVar1 = fopAcM_GetName(param_0) == PROC_NPC_TARO;
                break;
            case 2:
                bVar1 = fopAcM_GetName(param_0) == PROC_NPC_JAGAR;
                break;
            case 3:
                bVar1 = fopAcM_GetName(param_0) == PROC_NPC_LEN;
                break;
            case 4:
                bVar1 = fopAcM_GetName(param_0) == PROC_NPC_TARO;
                break;
            case 5:
                bVar1 = fopAcM_GetName(param_0) == PROC_NPC_MARO;
                break;
            case 6:
                bVar1 = fopAcM_GetName(param_0) == PROC_NPC_BESU;
                break;
            case 7:
                bVar1 = fopAcM_GetName(param_0) == PROC_NPC_BOU;
                break;
            case 8:
                bVar1 = fopAcM_GetName(param_0) == PROC_NPC_GRS;
                break;
            case 9:
                bVar1 = fopAcM_GetName(param_0) == PROC_NPC_GRO;
                break;
            }
        }
    }

    if (!bVar1) {
        param_0 = NULL;
    }
    return param_0;
}

int daTag_Push_c::create() {
    fopAcM_ct(this, daTag_Push_c);
    scale.x *= 100.0f;
    scale.y *= 100.0f;
    scale.z = scale.x;
    if (isDelete()) {
        return cPhs_ERROR_e;
    } else {
        return cPhs_COMPLEATE_e;
    }
}

int daTag_Push_c::Delete() {
    return 1;
}

int daTag_Push_c::Execute() {
    if (home.roomNo == dComIfGp_roomControl_getStayNo()) {
        fopAc_ac_c* actor = mActorMngr.getActorP();
        if (isDelete()) {
            if (actor != NULL && fopAcM_GetName(actor) != PROC_NPC_JAGAR &&
                fopAcM_GetName(actor) != PROC_NPC_GRS && fopAcM_GetName(actor) != PROC_NPC_GRO)
            {
                static_cast<daNpcT_c*>(actor)->setCommander(NULL);
            }
            fopAcM_delete(this);
            return 1;
        } else {
            if (!dComIfGp_event_runCheck()) {
                if (actor == NULL) {
                    actor = (fopAc_ac_c*)(fpcM_Search(srchActor, this));
                    if (actor != NULL) {
                        mActorMngr.entry(actor);
                    }
                }
                if (actor != NULL &&
                    daPy_getPlayerActorClass()->eventInfo.chkCondition(dEvtCnd_CANTALK_e) != false)
                {
                    if (chkPointInArea(cXyz(daPy_getPlayerActorClass()->current.pos))) {
                        if (fopAcM_GetName(actor) == PROC_NPC_GRS ||
                            fopAcM_GetName(actor) == PROC_NPC_GRO)
                        {
                            static_cast<daNpcF_c*>(actor)->setIntDemander(this);
                            static_cast<daNpcF_c*>(actor)->setIntFlowNodeNo(getFlowNodeNo());
                            static_cast<daNpcF_c*>(actor)->onInterrupt(getId());
                        } else {
                            static_cast<daNpcT_c*>(actor)->setCommander(this);
                        }
                    }
                }
            }
            attention_info.flags = 0;
            attention_info.position = current.pos;
            eyePos = attention_info.position;
            return 1;
        }
    }
    return 0;
}

int daTag_Push_c::Draw() {
    return 1;
}

int daTag_Push_c::isDelete() {
    bool retVal = false;
    if (getBitSW2() != 0xFF) {
        if (dComIfGs_isSwitch(getBitSW2(), fopAcM_GetRoomNo(this))) {
            retVal = true;
        }
    }
    return retVal;
}

int daTag_Push_c::chkPointInArea(cXyz param_0) {
    if ((getBitSW() != 0xFF && dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) ||
        getBitSW() == 0xFF)
    {
        return daNpcT_chkPointInArea(param_0, current.pos, scale, shape_angle.y, 1);
    } else {
        return 0;
    }
}

static int daTag_Push_Create(void* i_this) {
    return static_cast<daTag_Push_c*>(i_this)->create();
}

static int daTag_Push_Delete(void* i_this) {
    return static_cast<daTag_Push_c*>(i_this)->Delete();
}

static int daTag_Push_Execute(void* i_this) {
    return static_cast<daTag_Push_c*>(i_this)->Execute();
}

static int daTag_Push_Draw(void* i_this) {
    return static_cast<daTag_Push_c*>(i_this)->Draw();
}

static int daTag_Push_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daTag_Push_MethodTable = {
    (process_method_func)daTag_Push_Create,  (process_method_func)daTag_Push_Delete,
    (process_method_func)daTag_Push_Execute, (process_method_func)daTag_Push_IsDelete,
    (process_method_func)daTag_Push_Draw,
};

extern actor_process_profile_definition g_profile_TAG_PUSH = {
    fpcLy_CURRENT_e,
    8,
    fpcLy_CURRENT_e,
    PROC_TAG_PUSH,
    &g_fpcLf_Method.base,
    sizeof(daTag_Push_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x123,
    &daTag_Push_MethodTable,
    0x44000,
    0,
    fopAc_CULLBOX_CUSTOM_e,
};
