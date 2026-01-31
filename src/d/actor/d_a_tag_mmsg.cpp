#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_mmsg.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

int daTagMmsg_c::create() {
    fopAcM_ct(this, daTagMmsg_c);

    field_0x570 = fopAcM_GetParam(this) & 0x3FF;
    field_0x572 = (fopAcM_GetParam(this) >> 10) & 0x3FF;
    field_0x56b = (fopAcM_GetParam(this) >> 29) & 0x1;
    field_0x568 = shape_angle.x;
    field_0x569 = (shape_angle.x >> 8) & 0xFF;

    if ((fopAcM_GetParam(this) >> 30) & 0x1) {
        scale.x *= 10.0f;
        scale.y *= 10.0f;
    }

    field_0x574 = scale.x * (10000.0f * scale.x);
    field_0x578 = current.pos.y + scale.y * 100.0f;
    mAttention = shape_angle.y;

    if (!checkNoAttention()) {
        stage_arrow_data_class* arrowData =
            &dComIfGp_getRoomArrow(fopAcM_GetRoomNo(this))
                 ->m_entries[dComIfGp_getRoomCamera(fopAcM_GetRoomNo(this))->m_entries[mAttention].m_arrow_idx];
        eyePos.set(arrowData->posX, arrowData->posY, arrowData->posZ);
        attention_info.position = eyePos;
    }

    shape_angle.y = fopAcM_searchPlayerAngleY(this);
    return 4;
}

static int daTagMmsg_Create(fopAc_ac_c* i_this) {
    daTagMmsg_c* mmsg = static_cast<daTagMmsg_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return mmsg->create();
}

daTagMmsg_c::~daTagMmsg_c() {}

static int daTagMmsg_Delete(daTagMmsg_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTagMmsg_c();
    return 1;
}

int daTagMmsg_c::execute() {
    daMidna_c* midna = daPy_py_c::getMidnaActor();
    if (midna == NULL) {
        return 1;
    }

    if (field_0x572 != 0x3FF && dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[field_0x572]))
    {
        return 1;
    }

    if (field_0x569 != 0xFF && fopAcM_isSwitch(this, field_0x569)) {
        fopAcM_delete(this);
        return 1;
    }

    if (mUseFlg && !dComIfGp_event_runCheck() && field_0x56b && field_0x569 != 0xFF) {
        fopAcM_onSwitch(this, field_0x569);
        fopAcM_delete(this);
        return 1;
    }

    daPy_py_c* player = daPy_getLinkPlayerActorClass();
    if ((current.pos.y <= player->current.pos.y) && (field_0x578 >= player->current.pos.y) &&
        (fopAcM_searchPlayerDistanceXZ2(this) < field_0x574) &&
        (field_0x570 == 0x3FF ||
         dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[field_0x570])) &&
        (field_0x568 == 0xFF || fopAcM_isSwitch(this, field_0x568)))
    {
        player->setMidnaMsgNum(this, shape_angle.z);
    }

    return 1;
}

static int daTagMmsg_Execute(daTagMmsg_c* i_this) {
    return i_this->execute();
}

static int daTagMmsg_Draw(daTagMmsg_c* i_this) {
    return 1;
}

static actor_method_class l_daTagMmsg_Method = {
    (process_method_func)daTagMmsg_Create,  (process_method_func)daTagMmsg_Delete,
    (process_method_func)daTagMmsg_Execute, (process_method_func)NULL,
    (process_method_func)daTagMmsg_Draw,
};

actor_process_profile_definition g_profile_Tag_Mmsg = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Tag_Mmsg,          // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daTagMmsg_c),    // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    255,                    // mPriority
    &l_daTagMmsg_Method,    // sub_method
    0x44000,                // mStatus
    fopAc_ENV_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
