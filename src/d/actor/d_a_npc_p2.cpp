/**
 * @file d_a_npc_p2.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_p2.h"

static BOOL daNpc_P2Create(void* param_0) {
    return TRUE;
}

static BOOL daNpc_P2Delete(void* param_0) {
    return TRUE;
}

static BOOL daNpc_P2Execute(void* param_0) {
    return TRUE;
}

static BOOL daNpc_P2Draw(void* param_0) {
    return TRUE;
}

static BOOL daNpc_P2IsDelete(void* param_0) {
    return TRUE;
}

static actor_method_class daNpc_P2MethodTable = {
    (process_method_func)daNpc_P2Create,
    (process_method_func)daNpc_P2Delete,
    (process_method_func)daNpc_P2Execute,
    (process_method_func)daNpc_P2IsDelete,
    (process_method_func)daNpc_P2Draw,
};

actor_process_profile_definition g_profile_NPC_P2 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_P2_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ 0x00000001,
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_P2_e,
    /* Actor SubMtd */ &daNpc_P2MethodTable,
    /* Status       */ fopAcStts_UNK_0x2000000_e | fopAcStts_UNK_0x80000_e | fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e | fopAcStts_FREEZE_e | fopAcStts_UNK_0x4_e | fopAcStts_UNK_0x2_e | fopAcStts_UNK_0x1_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
