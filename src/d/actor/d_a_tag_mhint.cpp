/**
 * d_a_tag_mhint.cpp
 * Tag - Midna Hint
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_mhint.h"
#include "d/actor/d_a_midna.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_procname.h"
#include "d/d_meter2_info.h"

int daTagMhint_c::create() {
    fopAcM_ct(this, daTagMhint_c);

    field_0x574 = fopAcM_GetParam(this) & 0x3FF;
    field_0x57c = (fopAcM_GetParam(this) >> 10) & 0x3FF;
    field_0x578 = (fopAcM_GetParam(this) >> 20) & 0x1FF;
    field_0x57a = field_0x578;

    if ((fopAcM_GetParam(this) >> 0x1F)) {
        field_0x56a = 1;
    } else {
        field_0x56a = 0;
    }

    field_0x56b = (fopAcM_GetParam(this) >> 0x1D) & 1;

    if (daPy_getPlayerActorClass() != NULL) {
        field_0x572 = fopAcM_GetID(daPy_getPlayerActorClass()) == parentActorID;
    } else {
        field_0x572 = 0;
    }

    field_0x568 = shape_angle.x & 0xFF;
    mSwitch = (shape_angle.x >> 8) & 0xFF;
    field_0x56d = shape_angle.y & 0xFF;
    mToolEventID = (shape_angle.y >> 8) & 0xFF;

    mEventID = dComIfGp_getEventManager().getEventIdx(this, mToolEventID);
    if ((fopAcM_GetParam(this) >> 0x1E) & 1) {
        scale.x *= 10.0f;
        scale.y *= 10.0f;
    }

    field_0x5cc = scale.x * (scale.x * 10000.0f);
    field_0x5d0 = current.pos.y + scale.y * 100.0f;

    if (!checkNoAttention()) {
        stage_arrow_data_class* arrow_data_p =
            &dComIfGp_getRoomArrow(fopAcM_GetRoomNo(this))
                 ->m_entries[dComIfGp_getRoomCamera(fopAcM_GetRoomNo(this))
                                ->m_entries[field_0x56d]
                                .m_arrow_idx];

        eyePos.set(arrow_data_p->position.x, arrow_data_p->position.y,
                    arrow_data_p->position.z);
        attention_info.position = eyePos;
    }

    return cPhs_COMPLEATE_e;
}

static int daTagMhint_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagMhint_c*>(i_this)->create();
}

daTagMhint_c::~daTagMhint_c() {}

static int daTagMhint_Delete(daTagMhint_c* i_this) {
    i_this->~daTagMhint_c();
    return 1;
}

void daTagMhint_c::eventOrder() {
    if (!dComIfGp_event_runCheck()) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);

        if (!daPy_getPlayerActorClass()->checkPlayerFly() ||
            dComIfGp_checkPlayerStatus0(0, 0x100000))
        {
            if (field_0x56a != 0) {
                if (checkEventID()) {
                    fopAcM_orderOtherEventId(this, mEventID, mToolEventID, 0xFFFF, 0, 1);
                } else {
                    fopAcM_orderSpeakEvent(this, 0, 0);
                }
            } else if (!daPy_getPlayerActorClass()->checkMagneBootsOn() ||
                       strcmp(dComIfGp_getStartStageName(), "D_MN04B") == 0)
            {
                dComIfGp_att_ZHintRequest(this, 0x1FF);
                if (field_0x56f == 0) {
                    field_0x56f = 1;

                    if (field_0x57e == 0) {
                        mDoAud_seStart(Z2SE_NAVI_CALLVOICE, NULL, 0, 0);
                    }

                    field_0x57e = 60;
                }
            }
        }
    }
}

int daTagMhint_c::execute() {
    daMidna_c* midna_p = daPy_py_c::getMidnaActor();
    if (midna_p == NULL) {
        return 1;
    }

    if (field_0x57e != 0) {
        field_0x57e--;
    }

    if (checkNoAttention()) {
        attention_info.position = midna_p->attention_info.position;
        shape_angle.y = midna_p->shape_angle.y;
        eyePos = attention_info.position;
    } else {
        shape_angle.y = fopAcM_searchPlayerAngleY(this);
    }

    if (eventInfo.checkCommandDemoAccrpt()) {
        if (dComIfGp_evmng_endCheck(mEventID)) {
            dComIfGp_event_reset();
            fopAcM_orderSpeakEvent(this, 3, 0);
            field_0x570 = 1;
        }
    } else if (eventInfo.checkCommandTalk()) {
        if (!midna_p->checkShadowModeTalkWait()) {
            if (field_0x56e == 0) {
                mMsgFlow.init(this, shape_angle.z & 0xFFFF, 0, NULL);
                field_0x56e = 1;
                mDoAud_seStart(Z2SE_NAVI_TALK_START, NULL, 0, 0);
                field_0x571 = 0;
            } else if (mMsgFlow.doFlow(this, NULL, 0)) {
                int sp28;
                if (mMsgFlow.getEventId(&sp28) == 7) {
                    dMeter2Info_setPauseStatus(1);
                }

                if (field_0x571 == 0) {
                    field_0x571 = 1;
                    mDoAud_seStart(Z2SE_NAVI_TALK_END, NULL, 0, 0);
                }

                bool tmp;
                if (midna_p->checkShadowModelDraw()) {
                    midna_p->setShadowReturn();
                    tmp = midna_p->checkShadowReturnEnd();
                } else {
                    tmp = 1;
                }

                if (tmp) {
                    dComIfGp_event_reset();
                    field_0x570 = 0;

                    if (field_0x572 != 0 || (field_0x56b != 0 && mSwitch != 0xFF)) {
                        if (field_0x56b != 0) {
                            fopAcM_onSwitch(this, mSwitch);
                        }

                        fopAcM_delete(this);
                        field_0x578 = field_0x57a;
                    }
                }
            }
        }
    } else {
        if (field_0x570 != 0) {
            eventInfo.onCondition(dEvtCnd_CANTALK_e);
            fopAcM_orderSpeakEvent(this, 3, 0);
            return 1;
        }

        if (field_0x57c != 0x3FF &&
            dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[field_0x57c]))
        {
            return 1;
        }

        if (mSwitch != 0xFF && fopAcM_isSwitch(this, mSwitch)) {
            fopAcM_delete(this);
            return 1;
        }

        field_0x56e = 0;

        daPy_py_c* player_p = daPy_getPlayerActorClass();
        if (current.pos.y <= player_p->current.pos.y && field_0x5d0 >= player_p->current.pos.y &&
            fopAcM_searchPlayerDistanceXZ2(this) < field_0x5cc)
        {
            if ((field_0x574 == 0x3FF ||
                 dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[field_0x574])) &&
                (field_0x568 == 0xFF || fopAcM_isSwitch(this, field_0x568)))
            {
                if (field_0x578 != 0x1FF && field_0x578 != 0 && !dComIfGp_event_runCheck()) {
                    field_0x578--;
                }

                if (field_0x578 == 0 || field_0x578 == 0x1FF) {
                    eventOrder();
                }
            } else {
                field_0x578 = field_0x57a;
            }
        } else {
            field_0x578 = field_0x57a;
        }
    }

    if (!eventInfo.chkCondition(dEvtCnd_CANTALK_e)) {
        field_0x56f = 0;
    }

    return 1;
}

static int daTagMhint_Execute(daTagMhint_c* i_this) {
    return i_this->execute();
}

static int daTagMhint_Draw(daTagMhint_c* i_this) {
    return 1;
}

static actor_method_class l_daTagMhint_Method = {
    (process_method_func)daTagMhint_Create,  (process_method_func)daTagMhint_Delete,
    (process_method_func)daTagMhint_Execute, (process_method_func)NULL,
    (process_method_func)daTagMhint_Draw,
};

extern actor_process_profile_definition g_profile_Tag_Mhint = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_Mhint,
    &g_fpcLf_Method.base,
    sizeof(daTagMhint_c),
    0,
    0,
    &g_fopAc_Method.base,
    254,
    &l_daTagMhint_Method,
    0x44000,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
