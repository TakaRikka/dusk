/**
 * d_a_tag_escape.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_escape.h"
#include "f_pc/f_pc_name.h"

int daTagEscape_c::create() {
    fopAcM_ct(this, daTagEscape_c);
    return cPhs_COMPLEATE_e;
}

static int daTagEscape_Create(fopAc_ac_c* i_this) {
    daTagEscape_c* escape = static_cast<daTagEscape_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return escape->create();
}

static int daTagEscape_Delete(daTagEscape_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTagEscape_c();
    return 1;
}

static actor_method_class l_daTagEscape_Method = {
    (process_method_func)daTagEscape_Create,
    (process_method_func)daTagEscape_Delete,
    (process_method_func)NULL,
    (process_method_func)NULL,
    (process_method_func)NULL,
};

actor_process_profile_definition g_profile_Tag_Escape = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Tag_Escape_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagEscape_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Tag_Escape_e,
    /* Actor SubMtd */ &l_daTagEscape_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
