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

bool daTag_Howl_c::isAreaCheck() {
    cXyz vec_to_player;

    if (getSwitchBit() != 0xFF) {
        if (dComIfGs_isSaveSwitch(getSwitchBit())) {
            return false;
        }
    }

    vec_to_player = daPy_getPlayerActorClass()->current.pos - current.pos;
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
    field_0x569 = (fopAcM_GetParam(this) & 0xC0000000) >> 30;
    scale.x *= 100.0f;
    scale.y *= 100.0f;
    scale.z *= 100.0f;
    return 1;
}

u8 daTag_Howl_c::getSwitchBit() {
    u8 param = (fopAcM_GetParam(this) & 0xFF0) >> 4;
    return param;
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

actor_process_profile_definition g_profile_TAG_HOWL = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_TAG_HOWL_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTag_Howl_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_TAG_HOWL_e,
    /* Actor SubMtd */ &daTag_Howl_MethodTable,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
