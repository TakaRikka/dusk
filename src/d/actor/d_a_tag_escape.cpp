/**
 * d_a_tag_escape.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_escape.h"
#include "d/d_procname.h"

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
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_Escape,
    &g_fpcLf_Method.base,
    sizeof(daTagEscape_c),
    0,
    0,
    &g_fopAc_Method.base,
    397,
    &l_daTagEscape_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
