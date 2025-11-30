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

static bool daNpc_shopMaro_Execute(void* i_this) {
    return false;
}

static bool daNpc_shopMaro_Draw(void* i_this) {
    return false;
}

static bool daNpc_shopMaro_IsDelete(void* i_this) {
    return true;
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

extern actor_process_profile_definition g_profile_NPC_SMARO = {
    fpcLy_CURRENT_e,             // mLayerID
    7,                           // mListID
    fpcPi_CURRENT_e,             // mListPrio
    PROC_NPC_SMARO,              // mProcName
    &g_fpcLf_Method.base,       // sub_method
    sizeof(daNpc_shopMaro_c),    // mSize
    0,                           // mSizeOther
    0,                           // mParameters
    &g_fopAc_Method.base,        // sub_method
    365,                         // mPriority
    &daNpc_shopMaro_MethodTable, // sub_method
    0x00040000,                  // mStatus
    fopAc_ACTOR_e,               // mActorType
    fopAc_CULLBOX_0_e,           // cullType
  };
