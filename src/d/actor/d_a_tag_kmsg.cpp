#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_kmsg.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_player.h"
#include "SSystem/SComponent/c_counter.h"
#include <string>

struct EventListItem {
    char* mEventName;
    u32 field_0x4;
};

static EventListItem l_evtList[3] = {
    {"", 0},
    {"EXTINCTION", 2},
    {"PURCHASE", 1},
};

static char* l_resNameList[3] = {
    "",
    "Bans1",
    "sekizoA",
};

int daTag_KMsg_c::create() {
    int r27 = 0;

    eyePos = attention_info.position = current.pos;

    fopAcM_ct(this, daTag_KMsg_c);

    if (getType() == KMSG_TYPE_3) {
        int rv = dComIfG_resLoad(&mPhase, "Lv6Gate");
        if (rv != cPhs_COMPLEATE_e) {
            return rv;
        }
    }
    if (getType() == KMSG_TYPE_2 || getType() == KMSG_TYPE_5) {
        scale.y *= 10.0f;
    }
    mFlowNodeNo = getFlowNodeNo();
    mEventIdx = -1;

    OS_REPORT(
        "\t(%s:%d) flowNo:%d, BitSW:%02x, dis:%d, angle:%d, eye:%f, attn:%f, scale:%f,%f,%f<%08x>(%d/%04x)\n",
        fopAcM_getProcNameString(this),
        getType(),
        getFlowNodeNo(),
        getBitSW(),
        getTalkDis(),
        getTalkAngle(),
        getEyePosOffset(),
        getAttnPosOffset(),
        scale.x, scale.y, scale.z,
        fopAcM_GetParam(this),
        getChkType(),
        home.angle.x
    );

    if (isDelete()) {
        return cPhs_ERROR_e;
    } else {
        return cPhs_COMPLEATE_e;
    }
}

int daTag_KMsg_c::Delete() {
    OS_REPORT("|%06d:%x|daTag_KMsg_c -> Delete\n", g_Counter.mCounter0, this);
    fopAcM_RegisterDeleteID(this, "TAG_KMSG");
    this->~daTag_KMsg_c();
    return 1;
}

int daTag_KMsg_c::Execute() {
    bool var_r28 = false;
    if (home.roomNo == dComIfGp_roomControl_getStayNo()) {
        if (isDelete()) {
            fopAcM_delete(this);
            return 1;
        }
        if (getType() == KMSG_TYPE_3) {
            if (field_0x5c3 == 0) {
                attention_info.flags = fopAc_AttnFlag_ETC_e;
                attention_info.distances[fopAc_attn_ETC_e] = 0x4a;
            } else {
                attention_info.flags = 0;
            }
        } else {
            attention_info.distances[fopAc_attn_TALK_e] = daNpcT_getDistTableIdx(getTalkDis(), getTalkAngle());
            attention_info.distances[fopAc_attn_SPEAK_e] = attention_info.distances[fopAc_attn_TALK_e];
            if (getAttnPosOffset() != G_CM3D_F_INF) {
                attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
            } else {
                attention_info.flags = fopAc_AttnFlag_SPEAK_e;
            }
        }
        if ((getType() == KMSG_TYPE_1 || getType() == KMSG_TYPE_4 || getType() == KMSG_TYPE_5) &&
            daPy_py_c::checkNowWolf())
        {
            attention_info.flags = 0;
        }

        if (dComIfGp_event_runCheck()) {
            if (eventInfo.checkCommandTalk()) {
                u16 iVar10 = 0;
                switch (getType()) {
                case KMSG_TYPE_1:
                    iVar10 = 2;
                    break;
                }
                if (iVar10 != 0) {
                    if (strlen(l_evtList[iVar10].mEventName) != 0) {
                        if (strlen(l_resNameList[l_evtList[iVar10].field_0x4]) != 0) {
                            eventInfo.setArchiveName(l_resNameList[l_evtList[iVar10].field_0x4]);
                            dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
                        }
                        mEventIdx =
                            dComIfGp_getEventManager().getEventIdx(this, l_evtList[iVar10].mEventName, 0xff);
                        dComIfGp_getEvent()->reset(this);
                        fopAcM_orderChangeEventId(this, mEventIdx, 1, 0xffff);
                    }

                } else {
                    var_r28 = true;
                    if (!field_0x5c4) {
                        mMsgFlow.init(this, mFlowNodeNo, 0, NULL);
                        field_0x5c4 = true;
                    }
                    if (mMsgFlow.doFlow(this, NULL, 0) != 0) {
                        dComIfGp_event_reset();
                        field_0x5c4 = false;
                    }
                }
            } else {
                if (eventInfo.checkCommandDemoAccrpt() && dComIfGp_getEventManager().endCheck(mEventIdx)) {
                    dComIfGp_event_reset();
                    mEventIdx = -1;
                } else {
                    int myStaffId = dComIfGp_getEventManager().getMyStaffId("KMsg", this, -1);
                    if (myStaffId != -1) {
                        dComIfGp_getEventManager().cutEnd(myStaffId);
                    }
                }
            }
        } else if (getType() == KMSG_TYPE_3) {
            if (field_0x5c2 != 0x0) {
                attention_info.flags &= ~fopAc_AttnFlag_CARRY_e;
                fopAcM_cancelCarryNow(this);
                if (strlen(l_evtList[1].mEventName) != 0) {
                    if (strlen(l_resNameList[l_evtList[1].field_0x4]) != 0) {
                        eventInfo.setArchiveName(l_resNameList[l_evtList[1].field_0x4]);
                        dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
                    }
                    mEventIdx = dComIfGp_getEventManager().getEventIdx(this, l_evtList[1].mEventName, 0xff);
                    fopAcM_orderOtherEventId(this, mEventIdx, 0xff, 0xffff, 4, 1);
                }
            }
        } else {
            eventInfo.onCondition(1);
            if (getType() != KMSG_TYPE_1 && daNpcT_chkDoBtnIsSpeak(this)) {
                if (getChkType()) {
                    dComIfGp_setDoStatusForce(0x80, 2);
                } else {
                    dComIfGp_setDoStatus(8, 0);
                }
            }
        }
        if (getType() == KMSG_TYPE_3) {
            attention_info.position = current.pos;
            if (getAttnPosOffset() != G_CM3D_F_INF) {
                attention_info.position.y += getAttnPosOffset();
            }
            eyePos = current.pos;
        } else {
            attention_info.position = current.pos;
            if (getType() == KMSG_TYPE_2 || getType() == KMSG_TYPE_5) {
                attention_info.position.y += scale.y;
            }
            eyePos = attention_info.position;
            if (getAttnPosOffset() != G_CM3D_F_INF) {
                attention_info.position.y += getAttnPosOffset();
            }
            eyePos.y += getEyePosOffset();
        }

#if VERSION != VERSION_SHIELD_DEBUG
        if (var_r28) {
            f32 f31;
            if (getAttnPosOffset() != G_CM3D_F_INF) {
                f31 = -0.5f * getAttnPosOffset();
            } else {
                f31 = -20.0f;
            }

            cSAngle angle = shape_angle.y;
            cXyz vec60 = attention_info.position;
            vec60.y += f31;
            cXyz vec54;
            vec54.x = vec60.x + 80.0f * angle.Sin();
            vec54.y = vec60.y;
            vec54.z = vec60.z + 80.0f * angle.Cos();
            dBgS_LinChk lin_chk;
            lin_chk.ClrSttsWallOff();
            lin_chk.OnBackFlag();
            lin_chk.OnFrontFlag();
            lin_chk.Set(&vec54, &vec60, NULL);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                cXyz vec48 = lin_chk.GetCross() + cXyz(vec54 - vec60).norm() * 10.0f;
                attention_info.position.x = vec48.x;
                attention_info.position.z = vec48.z;
            }
        }
#endif

        return 1;
    }
    return 0;
}

int daTag_KMsg_c::Draw() {
    return 1;
}

int daTag_KMsg_c::isDelete() {
    switch(getType()) {
    case KMSG_TYPE_0:
        return 0;
    case KMSG_TYPE_1:
        return daNpcT_chkEvtBit(0x40) == 0;
    case KMSG_TYPE_2: 
        return 0;
    case KMSG_TYPE_3:
        return getBitSW() != 0xff && dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this));
    case KMSG_TYPE_4:
        return 0;
    case KMSG_TYPE_5:
        return 0;
    default:
        return 0;
    }
}

static int daTag_KMsg_Create(void* i_this) {
    return static_cast<daTag_KMsg_c*>(i_this)->create();
}

static int daTag_KMsg_Delete(void* i_this) {
    return static_cast<daTag_KMsg_c*>(i_this)->Delete();
}

static int daTag_KMsg_Execute(void* i_this) {
    return static_cast<daTag_KMsg_c*>(i_this)->Execute();
}

static int daTag_KMsg_Draw(void* i_this) {
    return static_cast<daTag_KMsg_c*>(i_this)->Draw();
}

static int daTag_KMsg_IsDelete(void* param_0) {
    return 1;
}

static actor_method_class daTag_KMsg_MethodTable = {
    (process_method_func)daTag_KMsg_Create,
    (process_method_func)daTag_KMsg_Delete,
    (process_method_func)daTag_KMsg_Execute,
    (process_method_func)daTag_KMsg_IsDelete,
    (process_method_func)daTag_KMsg_Draw,
};

actor_process_profile_definition g_profile_TAG_KMSG = {
    fpcLy_CURRENT_e,
    8,
    fpcPi_CURRENT_e,
    PROC_TAG_KMSG,
    &g_fpcLf_Method.base,
    sizeof(daTag_KMsg_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x122,
    &daTag_KMsg_MethodTable,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
