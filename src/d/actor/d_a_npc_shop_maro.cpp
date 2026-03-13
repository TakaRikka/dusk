/**
 * @file d_a_npc_shop_maro.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_shop_maro.h"

static int daNpc_shopMaro_Create(void* i_this) {
    return static_cast<daNpc_shopMaro_c*>(i_this)->create();
}

static int daNpc_shopMaro_Delete(void* i_this) {
    static_cast<daNpc_shopMaro_c*>(i_this)->deleteObject();
    return 1;
}

static BOOL daNpc_shopMaro_Execute(void* i_this) {
    return FALSE;
}

static BOOL daNpc_shopMaro_Draw(void* i_this) {
    return FALSE;
}

static BOOL daNpc_shopMaro_IsDelete(void* i_this) {
    return TRUE;
}

int daNpc_shopMaro_c::create() {
    fopAcM_ct(this, daNpc_shopMaro_c);
    return cPhs_ERROR_e;
}

static actor_method_class daNpc_shopMaro_MethodTable = {
    (process_method_func)daNpc_shopMaro_Create,
    (process_method_func)daNpc_shopMaro_Delete,
    (process_method_func)daNpc_shopMaro_Execute,
    (process_method_func)daNpc_shopMaro_IsDelete,
    (process_method_func)daNpc_shopMaro_Draw,
};

actor_process_profile_definition g_profile_NPC_SMARO = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_SMARO_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_shopMaro_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_SMARO_e,
    /* Actor SubMtd */ &daNpc_shopMaro_MethodTable,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
