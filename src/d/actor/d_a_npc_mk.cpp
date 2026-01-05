/**
 * @file d_a_npc_mk.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_mk.h"

static bool daNpc_Mk_Create(void* param_0) {
    return true;
}

static bool daNpc_Mk_Delete(void* param_0) {
    return true;
}

static bool daNpc_Mk_Execute(void* param_0) {
    return true;
}

static bool daNpc_Mk_Draw(void* param_0) {
    return true;
}

static bool daNpc_Mk_IsDelete(void* param_0) {
    return true;
}

static actor_method_class daNpc_Mk_MethodTable = {
    (process_method_func)daNpc_Mk_Create,
    (process_method_func)daNpc_Mk_Delete,
    (process_method_func)daNpc_Mk_Execute,
    (process_method_func)daNpc_Mk_IsDelete,
    (process_method_func)daNpc_Mk_Draw,
};

actor_process_profile_definition g_profile_NPC_MK = {
  fpcLy_CURRENT_e,       // mLayerID
  7,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_NPC_MK,           // mProcName
  &g_fpcLf_Method.base, // sub_method
  0x00000001,            // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  427,                   // mPriority
  &daNpc_Mk_MethodTable, // sub_method
  0x020C4407,            // mStatus
  fopAc_ACTOR_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
