/**
 * d_a_tag_gra.cpp
 * Tag - Goron A
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_gra.h"

static int daTagGra_Create(fopAc_ac_c* i_this) {
    daTagGra_c* gra_tag = (daTagGra_c*)i_this;
    return gra_tag->create();
}

static int daTagGra_Delete(daTagGra_c* i_this) {
    i_this->~daTagGra_c();
    return 1;
}

static actor_method_class l_daTagGra_Method = {
    (process_method_func)daTagGra_Create,
    (process_method_func)daTagGra_Delete
};

actor_process_profile_definition g_profile_TAG_GRA = {
    fpcLy_CURRENT_e,        // mLayerID
    3,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_TAG_GRA,           // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daTagGra_c),     // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    20,                     // mPriority
    &l_daTagGra_Method,     // sub_method
    0x40000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
