/**
 * d_a_kytag05.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag05.h"
#include "f_op/f_op_actor_mng.h"

static int daKytag05_Draw(kytag05_class* i_this) {
    return 1;
}

static int daKytag05_Execute(kytag05_class* i_this) {
    return 1;
}

static int daKytag05_IsDelete(kytag05_class* i_this) {
    return 1;
}

static int daKytag05_Delete(kytag05_class* i_this) {
    return 1;
}

static int daKytag05_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, kytag05_class);
    kytag05_class* a_this = static_cast<kytag05_class*>(i_this);

    a_this->field_0x56c = fopAcM_GetParam(a_this) >> 8;
    a_this->mSceneListID = fopAcM_GetParam(a_this);
    a_this->field_0x568 = 100.0f * a_this->scale.x;

    if (!a_this->field_0x56c) {
        a_this->attention_info.distances[fopAc_attn_ETC_e] = 0x21;
        a_this->attention_info.flags |= fopAc_AttnFlag_ETC_e;
    }

    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daKytag05_Method = {
    (process_method_func)daKytag05_Create,  (process_method_func)daKytag05_Delete,
    (process_method_func)daKytag05_Execute, (process_method_func)daKytag05_IsDelete,
    (process_method_func)daKytag05_Draw,
};

actor_process_profile_definition g_profile_KYTAG05 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KYTAG05,
    &g_fpcLf_Method.base,
    sizeof(kytag05_class),
    0,
    0,
    &g_fopAc_Method.base,
    99,
    &l_daKytag05_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
