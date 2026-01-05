/**
 * d_a_tag_instruction.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_instruction.h"
#include "d/d_procname.h"

static int daTagInst_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, daTagInst_c);
    return cPhs_COMPLEATE_e;
}

static int daTagInst_Delete(daTagInst_c* i_this) {
    i_this->~daTagInst_c();
    return 1;
}

static actor_method_class l_daTagInst_Method = {
    (process_method_func)daTagInst_Create,
    (process_method_func)daTagInst_Delete,
    (process_method_func)NULL,
    (process_method_func)NULL,
    (process_method_func)NULL,
};

actor_process_profile_definition g_profile_Tag_Instruction = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_Instruction,
    &g_fpcLf_Method.base,
    sizeof(daTagInst_c),
    0,
    0,
    &g_fopAc_Method.base,
    334,
    &l_daTagInst_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
