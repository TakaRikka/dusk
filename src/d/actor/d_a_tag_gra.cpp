/**
 * d_a_tag_gra.cpp
 * Tag - Goron A
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_gra.h"

static int daTagGra_Create(fopAc_ac_c* i_this) {
    daTagGra_c* gra_tag = (daTagGra_c*)i_this;
    int id = fopAcM_GetID(i_this);
    return gra_tag->create();
}

static int daTagGra_Delete(daTagGra_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTagGra_c();
    return 1;
}

static actor_method_class l_daTagGra_Method = {
    (process_method_func)daTagGra_Create,
    (process_method_func)daTagGra_Delete
};

actor_process_profile_definition g_profile_TAG_GRA = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 3,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_TAG_GRA_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagGra_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_TAG_GRA_e,
    /* Actor SubMtd */ &l_daTagGra_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
