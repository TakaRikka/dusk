/**
 * d_a_tag_howl.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_howl.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

int daTag_Howl_c::create() {
    fopAcM_ct(this, daTag_Howl_c);
    getParam();
    return cPhs_COMPLEATE_e;
}

int daTag_Howl_c::destroy() {
    return 1;
}

int daTag_Howl_c::execute() {
    fopAcM_SetRoomNo(this, dComIfGp_roomControl_getStayNo());
    attention_info.flags = 0;

    if (isAreaCheck()) {
        attention_info.flags |= fopAc_AttnFlag_ETC_e;
        attention_info.distances[fopAc_attn_ETC_e] = 0x41;
    }

    return 1;
}

int daTag_Howl_c::draw() {
    return 1;
}

u8 daTag_Howl_c::isAreaCheck() {
    if (getSwitchBit() != 0xFF) {
        if (dComIfGs_isSaveSwitch(getSwitchBit())) {
            return false;
        }
    }

    cXyz vec_to_player = daPy_getPlayerActorClass()->current.pos - current.pos;
    if (daPy_py_c::checkNowWolf()) {
        if (vec_to_player.absXZ() < scale.x && -scale.y < vec_to_player.y &&
            vec_to_player.y < scale.y)
        {
            return true;
        }
    }

    return false;
}

int daTag_Howl_c::getParam() {
    mCurveID = fopAcM_GetParam(this) & 0xF;
    field_0x569 = fopAcM_GetParam(this) >> 0x1E;
    scale.x *= 100.0f;
    scale.y *= 100.0f;
    scale.z *= 100.0f;
    return 1;
}

u8 daTag_Howl_c::getSwitchBit() {
    return (fopAcM_GetParam(this) >> 4) & 0xFF;
}

static int daTag_Howl_Create(void* i_this) {
    return static_cast<daTag_Howl_c*>(i_this)->create();
}

static int daTag_Howl_Delete(void* i_this) {
    return static_cast<daTag_Howl_c*>(i_this)->destroy();
}

static int daTag_Howl_Execute(void* i_this) {
    return static_cast<daTag_Howl_c*>(i_this)->execute();
}

static int daTag_Howl_Draw(void* i_this) {
    return static_cast<daTag_Howl_c*>(i_this)->draw();
}

static int daTag_Howl_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daTag_Howl_MethodTable = {
    (process_method_func)daTag_Howl_Create,  (process_method_func)daTag_Howl_Delete,
    (process_method_func)daTag_Howl_Execute, (process_method_func)daTag_Howl_IsDelete,
    (process_method_func)daTag_Howl_Draw,

};

extern actor_process_profile_definition g_profile_TAG_HOWL = {
    fpcLy_CURRENT_e,          // mLayerID
    7,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_TAG_HOWL,            // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daTag_Howl_c),     // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    283,                      // mPriority
    &daTag_Howl_MethodTable,  // sub_method
    0x40000,                  // mStatus
    fopAc_ACTOR_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
};
