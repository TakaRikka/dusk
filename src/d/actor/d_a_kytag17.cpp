/**
 * d_a_kytag17.cpp
 * Light Mask Tag
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag17.h"
#include "f_pc/f_pc_name.h"

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

actor_process_profile_definition g_profile_KYTAG17 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_KYTAG17_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(kytag17_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_KYTAG17_e,
    /* Actor SubMtd */ &l_daKytag17_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
