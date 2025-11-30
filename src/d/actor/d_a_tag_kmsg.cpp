#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_kmsg.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_player.h"
#include "SSystem/SComponent/c_counter.h"
#include "string.h"

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
    attention_info.position = current.pos;
    eyePos = attention_info.position;
    fopAcM_ct(this, daTag_KMsg_c);
    int rv;
    if (getType() == KMSG_TYPE_3) {
        rv = dComIfG_resLoad(&mPhase, "Lv6Gate");
        if (rv != cPhs_COMPLEATE_e) {
            return rv;
        }
    }
    if (getType() == KMSG_TYPE_2 || getType() == KMSG_TYPE_5) {
        scale.y *= 10.0f;
    }
    mFlowNodeNo = getFlowNodeNo();
    mEventIdx = -1;

    if (isDelete()) {
        return cPhs_ERROR_e;
    } else {
        return cPhs_COMPLEATE_e;
    }
}

int daTag_KMsg_c::Delete() {
    OS_REPORT("|%06d:%x|daTag_KMsg_c -> Delete\n", g_Counter.mCounter0, this);
    fopAcM_GetID(this);
    this->~daTag_KMsg_c();
    return 1;
}

// NONMATCHING Various issues: stack, getTalkAngle
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
#if VERSION == VERSION_SHIELD_DEBUG
            attention_info.distances[fopAc_attn_TALK_e] = daNpcT_getDistTableIdx(getTalkDis(), getTalkAngle());
#else
            attention_info.distances[fopAc_attn_TALK_e] = daNpcT_getDistTableIdx(getTalkAngle(), getTalkDis());
#endif
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
                        dComIfGp_getEvent().reset(this);
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
                if (getChkType() != KMSG_TYPE_0) {
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
                cXyz vec48;
                cXyz vec30 = vec54 - vec60;
                vec48 = lin_chk.GetCross() + vec30.norm() * 10.0f;
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
        bool rv = false;
        if (getBitSW() != 0xff && dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
            rv = true;
        }
        return rv;
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

static void* daTag_KMsg_MethodTable[8] = {
    (void*)daTag_KMsg_Create,
    (void*)daTag_KMsg_Delete,
    (void*)daTag_KMsg_Execute,
    (void*)daTag_KMsg_IsDelete,
    (void*)daTag_KMsg_Draw,
};

extern void* g_profile_TAG_KMSG[12] = {
    (void*)0xFFFFFFFD, (void*)0x0008FFFD,
    (void*)0x02EB0000, (void*)&g_fpcLf_Method,
    (void*)0x000005CC, (void*)NULL,
    (void*)NULL,       (void*)&g_fopAc_Method,
    (void*)0x01220000, (void*)&daTag_KMsg_MethodTable,
    (void*)0x00044000, (void*)0x000E0000,
};
