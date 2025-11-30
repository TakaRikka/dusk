/**
 * d_a_tag_schedule.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_schedule.h"

static int daTagSchedule_Create(fopAc_ac_c* i_this) {
    daTagSchedule_c* schedule_tag = (daTagSchedule_c*)i_this;
    return schedule_tag->create();
}

static int daTagSchedule_Delete(daTagSchedule_c* i_this) {
    i_this->~daTagSchedule_c();
    return 1;
}

static actor_method_class l_daTagSchedule_Method = {
    (process_method_func)daTagSchedule_Create,
    (process_method_func)daTagSchedule_Delete
};

extern actor_process_profile_definition g_profile_Tag_Schedule = {
    fpcLy_CURRENT_e,            // mLayerID
    7,                          // mListID
    fpcPi_CURRENT_e,            // mListPrio
    PROC_Tag_Schedule,          // mProcName
    &g_fpcLf_Method.base,      // sub_method
    sizeof(daTagSchedule_c),    // mSize
    0,                          // mSizeOther
    0,                          // mParameters
    &g_fopAc_Method.base,       // sub_method
    396,                        // mPriority
    &l_daTagSchedule_Method,    // sub_method
    0x40000,                    // mStatus
    fopAc_ACTOR_e,              // mActorType
    fopAc_CULLBOX_CUSTOM_e,     // cullType
};
