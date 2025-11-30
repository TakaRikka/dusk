/**
 * d_a_tag_theB_hint.cpp
 * Tag - Telma B Hint
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_theB_hint.h"

static int daTagTheBHint_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagTheBHint_c*>(i_this)->create();
}

static int daTagTheBHint_Delete(daTagTheBHint_c* i_this) {
    i_this->~daTagTheBHint_c();
    return 1;
}

static int daTagTheBHint_Execute(daTagTheBHint_c* i_this) {
    return i_this->execute();
}

static actor_method_class l_daTagTheBHint_Method = {
    (process_method_func)daTagTheBHint_Create,
    (process_method_func)daTagTheBHint_Delete,
    (process_method_func)daTagTheBHint_Execute
};

extern actor_process_profile_definition g_profile_Tag_TheBHint = {
    fpcLy_CURRENT_e,          // mLayerID
    7,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_Tag_TheBHint,        // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daTagTheBHint_c),  // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    265,                      // mPriority
    &l_daTagTheBHint_Method,  // sub_method
    0x40000,                  // mStatus
    fopAc_ACTOR_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
};
