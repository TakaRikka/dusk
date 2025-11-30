/**
 * d_a_kytag17.cpp
 * Light Mask Tag
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag17.h"
#include "d/d_procname.h"

static int daKytag17_Draw(kytag17_class* i_this) {
    return 1;
}

static int daKytag17_Execute(kytag17_class* i_this) {
    return 1;
}

static int daKytag17_IsDelete(kytag17_class* i_this) {
    return 1;
}

static int daKytag17_Delete(kytag17_class* i_this) {
    return 1;
}

static int daKytag17_Create(fopAc_ac_c* i_this) {
    kytag17_class* a_this = (kytag17_class*)i_this;

    fopAcM_ct(a_this, kytag17_class);

    a_this->mParameters = fopAcM_GetParam(a_this);
    g_env_light.light_mask_type = a_this->mParameters;
    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daKytag17_Method = {
    (process_method_func)daKytag17_Create, (process_method_func)daKytag17_Delete,
    (process_method_func)daKytag17_Execute, (process_method_func)daKytag17_IsDelete,
    (process_method_func)daKytag17_Draw};

extern actor_process_profile_definition g_profile_KYTAG17 = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_KYTAG17,           // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(kytag17_class),  // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    111,                    // mPriority
    &l_daKytag17_Method,    // sub_method
    0x44000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_0_e,      // cullType   
};
