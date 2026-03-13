#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_push.h"
#include "d/actor/d_a_npc4.h"
#include "f_pc/f_pc_name.h"

void* daTag_Push_c::srchActor(void* param_0, void* param_1) {
    daTag_Push_c* push = (daTag_Push_c*)param_0;
    BOOL bVar1 = false;
    if (push != NULL && fopAcM_IsActor(push)) {
        if (fopAcM_IsExecuting(fopAcM_GetID(param_0))) {
            int id = ((daTag_Push_c*)param_1)->getId();
            switch (id) {
            case 1:
                bVar1 = fopAcM_GetName(push) == fpcNm_NPC_TARO_e;
                break;
            case 2:
                bVar1 = fopAcM_GetName(push) == fpcNm_NPC_JAGAR_e;
                break;
            case 3:
                bVar1 = fopAcM_GetName(push) == fpcNm_NPC_LEN_e;
                break;
            case 4:
                bVar1 = fopAcM_GetName(push) == fpcNm_NPC_TARO_e;
                break;
            case 5:
                bVar1 = fopAcM_GetName(push) == fpcNm_NPC_MARO_e;
                break;
            case 6:
                bVar1 = fopAcM_GetName(push) == fpcNm_NPC_BESU_e;
                break;
            case 7:
                bVar1 = fopAcM_GetName(push) == fpcNm_NPC_BOU_e;
                break;
            case 8:
                bVar1 = fopAcM_GetName(push) == fpcNm_NPC_GRS_e;
                break;
            case 9:
                bVar1 = fopAcM_GetName(push) == fpcNm_NPC_GRO_e;
                break;
            }
        }
    }

    if (!bVar1) {
        push = NULL;
    }
    return push;
}

int daTag_Push_c::create() {
    int var_r29 = 0;
    fopAcM_ct(this, daTag_Push_c);
    scale.x *= 100.0f;
    scale.y *= 100.0f;
    scale.z = scale.x;

    OS_REPORT("\t(%s:%d) PlayerNo:%d, BitSW:%02x, BitSW2:%02x, flowNo:%d<%08x>\n",
              fopAcM_getProcNameString(this), getId(), getPlayerListNo(), getBitSW(), getBitSW2(),
              getFlowNodeNo(), fopAcM_GetParam(this));

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
            if (actor != NULL && fopAcM_GetName(actor) != fpcNm_NPC_JAGAR_e &&
                fopAcM_GetName(actor) != fpcNm_NPC_GRS_e && fopAcM_GetName(actor) != fpcNm_NPC_GRO_e)
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
                        if (fopAcM_GetName(actor) == fpcNm_NPC_GRS_e ||
                            fopAcM_GetName(actor) == fpcNm_NPC_GRO_e)
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

static int daTag_Push_IsDelete(void*) {
    return 1;
}

static actor_method_class daTag_Push_MethodTable = {
    (process_method_func)daTag_Push_Create,  (process_method_func)daTag_Push_Delete,
    (process_method_func)daTag_Push_Execute, (process_method_func)daTag_Push_IsDelete,
    (process_method_func)daTag_Push_Draw,
};

actor_process_profile_definition g_profile_TAG_PUSH = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 8,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_TAG_PUSH_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTag_Push_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_TAG_PUSH_e,
    /* Actor SubMtd */ &daTag_Push_MethodTable,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
