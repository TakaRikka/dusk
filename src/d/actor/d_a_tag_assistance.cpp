/**
 * d_a_tag_assistance.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_assistance.h"

static int daTagAssist_Create(fopAc_ac_c* i_this) {
    daTagAssist_c* tag_assist = (daTagAssist_c*)i_this;
    return tag_assist->create();
}

static int daTagAssist_Execute(daTagAssist_c* i_this) {
    return i_this->execute();
}

static int daTagAssist_IsDelete(daTagAssist_c* i_this) {
    return 1;
}

static int daTagAssist_Delete(daTagAssist_c* i_this) {
    i_this->~daTagAssist_c();
    return 1;
}

static actor_method_class l_daTagAssist_Method = {
    (process_method_func)daTagAssist_Create,
    (process_method_func)daTagAssist_Delete,
    (process_method_func)daTagAssist_Execute,
    (process_method_func)daTagAssist_IsDelete
};

extern actor_process_profile_definition g_profile_Tag_Assist = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Tag_Assist,        // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daTagAssist_c),  // mSize
    0,                      // mSizeOther
    0,                      // mParameters    
    &g_fopAc_Method.base,   // sub_method
    266,                    // mPriority
    &l_daTagAssist_Method,  // sub_method
    0x40000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
