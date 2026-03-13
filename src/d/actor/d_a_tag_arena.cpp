/**
 * d_a_tag_arena.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_arena.h"

static int daTagArena_Create(fopAc_ac_c* i_this) {
    daTagArena_c* arena_tag = (daTagArena_c*)i_this;
    int id = fopAcM_GetID(i_this);
    return arena_tag->create();
}

static int daTagArena_Delete(daTagArena_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTagArena_c();
    return 1;
}

static actor_method_class l_daTagArena_Method = {
    (process_method_func)daTagArena_Create,
    (process_method_func)daTagArena_Delete,
};

actor_process_profile_definition g_profile_Tag_Arena = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Tag_Arena_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagArena_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Tag_Arena_e,
    /* Actor SubMtd */ &l_daTagArena_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
