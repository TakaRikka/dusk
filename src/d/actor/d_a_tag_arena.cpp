/**
 * d_a_tag_arena.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_arena.h"

static int daTagArena_Create(fopAc_ac_c* i_this) {
    daTagArena_c* arena_tag = (daTagArena_c*)i_this;
    return arena_tag->create();
}

static int daTagArena_Delete(daTagArena_c* i_this) {
    i_this->~daTagArena_c();
    return 1;
}

static actor_method_class l_daTagArena_Method = {
    (process_method_func)daTagArena_Create,
    (process_method_func)daTagArena_Delete,
};

actor_process_profile_definition g_profile_Tag_Arena = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Tag_Arena,         // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daTagArena_c),   // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    333,                    // mPriority
    &l_daTagArena_Method,   // sub_method
    0x40000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
