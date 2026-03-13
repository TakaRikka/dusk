/**
 * d_a_tag_assistance.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_assistance.h"

void daTagAssist_c::create_init() {
    attention_info.flags = 0;
    field_0x570 = -1;
}

int daTagAssist_c::create() {
    fopAcM_ct(this, daTagAssist_c);
    create_init();
    return cPhs_COMPLEATE_e;
}

int daTagAssist_c::execute() {
    if (fopAcM_searchPlayerDistanceXZ(this) < scale.x * 100.0f) {
        // empty block
    }
    return 1;
}

static int daTagAssist_Create(fopAc_ac_c* i_this) {
    daTagAssist_c* tag_assist = (daTagAssist_c*)i_this;
    int id = fopAcM_GetID(i_this);
    return tag_assist->create();
}

static int daTagAssist_Execute(daTagAssist_c* i_this) {
    return i_this->execute();
}

static int daTagAssist_IsDelete(daTagAssist_c* i_this) {
    return 1;
}

static int daTagAssist_Delete(daTagAssist_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTagAssist_c();
    return 1;
}

static actor_method_class l_daTagAssist_Method = {
    (process_method_func)daTagAssist_Create,
    (process_method_func)daTagAssist_Delete,
    (process_method_func)daTagAssist_Execute,
    (process_method_func)daTagAssist_IsDelete
};

actor_process_profile_definition g_profile_Tag_Assist = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Tag_Assist_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagAssist_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Tag_Assist_e,
    /* Actor SubMtd */ &l_daTagAssist_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
