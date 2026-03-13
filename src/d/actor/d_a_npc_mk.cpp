/**
 * @file d_a_npc_mk.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_mk.h"

static BOOL daNpc_Mk_Create(void* param_0) {
    return TRUE;
}

static BOOL daNpc_Mk_Delete(void* param_0) {
    return TRUE;
}

static BOOL daNpc_Mk_Execute(void* param_0) {
    return TRUE;
}

static BOOL daNpc_Mk_Draw(void* param_0) {
    return TRUE;
}

static BOOL daNpc_Mk_IsDelete(void* param_0) {
    return TRUE;
}

static actor_method_class daNpc_Mk_MethodTable = {
    (process_method_func)daNpc_Mk_Create,
    (process_method_func)daNpc_Mk_Delete,
    (process_method_func)daNpc_Mk_Execute,
    (process_method_func)daNpc_Mk_IsDelete,
    (process_method_func)daNpc_Mk_Draw,
};

actor_process_profile_definition g_profile_NPC_MK = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_MK_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ 0x00000001,
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_MK_e,
    /* Actor SubMtd */ &daNpc_Mk_MethodTable,
    /* Status       */ fopAcStts_UNK_0x2000000_e | fopAcStts_UNK_0x80000_e | fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e | fopAcStts_FREEZE_e | fopAcStts_UNK_0x4_e | fopAcStts_UNK_0x2_e | fopAcStts_UNK_0x1_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
