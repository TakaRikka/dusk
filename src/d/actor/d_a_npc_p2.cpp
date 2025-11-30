/**
 * @file d_a_npc_p2.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_p2.h"

static bool daNpc_P2Create(void* param_0) {
    return true;
}

static bool daNpc_P2Delete(void* param_0) {
    return true;
}

static bool daNpc_P2Execute(void* param_0) {
    return true;
}

static bool daNpc_P2Draw(void* param_0) {
    return true;
}

static bool daNpc_P2IsDelete(void* param_0) {
    return true;
}

static actor_method_class daNpc_P2MethodTable = {
    (process_method_func)daNpc_P2Create,
    (process_method_func)daNpc_P2Delete,
    (process_method_func)daNpc_P2Execute,
    (process_method_func)daNpc_P2IsDelete,
    (process_method_func)daNpc_P2Draw,
};

extern actor_process_profile_definition g_profile_NPC_P2 = {
  fpcLy_CURRENT_e,       // mLayerID
  7,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_NPC_P2,           // mProcName
  &g_fpcLf_Method.base, // sub_method
  0x00000001,            // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  428,                   // mPriority
  &daNpc_P2MethodTable,  // sub_method
  0x020C4407,            // mStatus
  fopAc_ACTOR_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
