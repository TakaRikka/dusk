/**
 * d_a_tag_instruction.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_instruction.h"
#include "f_pc/f_pc_name.h"

static int daTagInst_Create(fopAc_ac_c* i_this) {
    int id = fopAcM_GetID(i_this);
    fopAcM_ct(i_this, daTagInst_c);
    return cPhs_COMPLEATE_e;
}

static int daTagInst_Delete(daTagInst_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTagInst_c();
    return 1;
}

static actor_method_class l_daTagInst_Method = {
    (process_method_func)daTagInst_Create,
    (process_method_func)daTagInst_Delete,
    (process_method_func)NULL,
    (process_method_func)NULL,
    (process_method_func)NULL,
};

actor_process_profile_definition g_profile_Tag_Instruction = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Tag_Instruction_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagInst_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Tag_Instruction_e,
    /* Actor SubMtd */ &l_daTagInst_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
