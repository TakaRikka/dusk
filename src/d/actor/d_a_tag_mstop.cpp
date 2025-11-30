/**
 * d_a_tag_mstop.cpp
 * Tag - Midna Stop
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_mstop.h"
#include "d/actor/d_a_midna.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_npc4.h"
#include "f_op/f_op_actor_mng.h"

int daTagMstop_c::create() {
    fopAcM_ct(this, daTagMstop_c);

    field_0x56b = fopAcM_GetParam(this) >> 16;
    field_0x56a = (fopAcM_GetParam(this) >> 24) & 0xF;

    if (field_0x56a == 1 || field_0x56a == 2) {
        field_0x56c = 1;

        if (field_0x56a == 2) {
            field_0x56a = 0;
        }

    } else {
        field_0x56c = 0;
    }

    if ((fopAcM_GetParam(this) >> 28) & 1) {
        field_0x568 = 0xFF;
        mSwitch = 0xFF;
        field_0x570 = shape_angle.x;
        field_0x572 = shape_angle.y;
    } else {
        field_0x568 = shape_angle.x;
        mSwitch = (shape_angle.x >> 8) & 0xFF;
        field_0x570 = 0xFFFF;
        field_0x572 = 0xFFFF;
    }

    f32 tmp = scale.x;
    tmp *= 10000.0f * scale.x;

    field_0x5c0 = tmp;
    field_0x5c4 = current.pos.y + scale.y * 100.0f;

    csXyz pos_angle;

    daNpcF_getPlayerInfoFromPlayerList(fopAcM_GetParam(this) & 0xFFFF, fopAcM_GetRoomNo(this),
                                       field_0x5c8, pos_angle);

    u8 idx = field_0x56b;

    if (!checkNoAttention()) {
        s32 room = fopAcM_GetRoomNo(this);
        cXyz* pos = &dComIfGp_getRoomArrow(room)
                         ->m_entries[dComIfGp_getRoomCamera(room)->m_entries[idx].m_arrow_idx]
                         .position;

        eyePos.set(pos->x, pos->y, pos->z);
        attention_info.position = eyePos;
    }

    shape_angle.y = fopAcM_searchPlayerAngleY(this);
    return cPhs_COMPLEATE_e;
}

static int daTagMstop_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagMstop_c*>(i_this)->create();
}

daTagMstop_c::~daTagMstop_c() {}

static int daTagMstop_Delete(daTagMstop_c* i_this) {
    i_this->~daTagMstop_c();
    return 1;
}

void daTagMstop_c::eventOrder() {
    eventInfo.onCondition(dEvtCnd_CANTALK_e);
    fopAcM_orderSpeakEvent(this, 0, 0);
}

int daTagMstop_c::execute() {
    if (field_0x56c) {
        fopAcM_seStartCurrentLevel(this, Z2SE_OBJ_DARK_GATE, 0);
    }

    daMidna_c* midna_p = daPy_py_c::getMidnaActor();

    if (midna_p == NULL) {
        return 1;
    }

    daPy_py_c* player_p = daPy_getLinkPlayerActorClass();

    if (checkNoAttention()) {
        attention_info.position = midna_p->attention_info.position;
        shape_angle.y = midna_p->shape_angle.y;
        eyePos = attention_info.position;
    } else {
        shape_angle.y = fopAcM_searchPlayerAngleY(this);
    }

    if (field_0x56e == 4) {
        if (field_0x5c8.abs2(player_p->current.pos) < 2500.0f) {
            dComIfGp_event_reset();
            player_p->cancelOriginalDemo();
            field_0x56e = 0;
        }
    } else if (field_0x56e == 3) {
        if (player_p->checkHorseRide()) {
            player_p->setPlayerPosAndAngle(&field_0x5c8, player_p->shape_angle.y, 0);
            dComIfGp_event_reset();
            field_0x56e = 0;
        } else {
            field_0x56e = 4;
            player_p->changeOriginalDemo();
            field_0x56a == 0 ? player_p->changeDemoMode(3, 0, 0, 0) :
                               player_p->changeDemoMode(2, 0, 0, 0);

            player_p->changeDemoPos0(&field_0x5c8);
        }
    } else if (eventInfo.checkCommandTalk()) {
        if (field_0x56e == 2) {
            if (!midna_p->checkShadowModelDraw() || midna_p->checkShadowReturnEnd()) {
                dComIfGp_getEvent().reset(this);
                fopAcM_orderPotentialEvent(this, 0x400, 0x14f, 1);
                field_0x56e = 3;
            }
        } else if (!midna_p->checkShadowModeTalkWait()) {
            if (field_0x56e == 0) {
                mMsgFlow.init(this, (u16)shape_angle.z, 0, 0);
                mDoAud_seStart(Z2SE_NAVI_TALK_START, 0, 0, 0);
                field_0x56e = 1;
            } else if (mMsgFlow.doFlow(this, 0, 0)) {
                field_0x56e = 2;
                mDoAud_seStart(Z2SE_NAVI_TALK_END, 0, 0, 0);

                if (midna_p->checkShadowModelDraw()) {
                    midna_p->setShadowReturn();
                }
            }
        }
    } else if ((mSwitch != 0xFF && fopAcM_isSwitch(this, mSwitch)) ||
               (field_0x572 != 0xFFFF &&
                dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[field_0x572])))
    {
        fopAcM_delete(this);
        return 1;
    } else if ((field_0x568 != 0xFF && !fopAcM_isSwitch(this, field_0x568)) ||
               (field_0x570 != 0xFFFF &&
                !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[field_0x570])))
    {
        return 1;
    } else if (current.pos.y <= player_p->current.pos.y && field_0x5c4 >= player_p->current.pos.y &&
               fopAcM_searchPlayerDistanceXZ2(this) < field_0x5c0)
    {
        eventOrder();
    }

    return 1;
}

static int daTagMstop_Execute(daTagMstop_c* i_this) {
    return i_this->execute();
}

static int daTagMstop_Draw(daTagMstop_c* i_this) {
    return 1;
}

static actor_method_class l_daTagMstop_Method = {
    (process_method_func)daTagMstop_Create, (process_method_func)daTagMstop_Delete,
    (process_method_func)daTagMstop_Execute, NULL, (process_method_func)daTagMstop_Draw};

extern actor_process_profile_definition g_profile_Tag_Mstop = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Tag_Mstop,         // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daTagMstop_c),   // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    257 ,                   // mPriority
    &l_daTagMstop_Method,   // sub_method
    0x44000,                // mStatus
    fopAc_ENV_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
