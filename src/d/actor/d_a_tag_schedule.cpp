/**
 * d_a_tag_schedule.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_schedule.h"

static int daTagSchedule_Create(fopAc_ac_c* i_this) {
    daTagSchedule_c* schedule_tag = (daTagSchedule_c*)i_this;
    int id = fopAcM_GetID(i_this);
    return schedule_tag->create();
}

static int daTagSchedule_Delete(daTagSchedule_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTagSchedule_c();
    return 1;
}

static actor_method_class l_daTagSchedule_Method = {
    (process_method_func)daTagSchedule_Create,
    (process_method_func)daTagSchedule_Delete
};

actor_process_profile_definition g_profile_Tag_Schedule = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Tag_Schedule_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagSchedule_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Tag_Schedule_e,
    /* Actor SubMtd */ &l_daTagSchedule_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
