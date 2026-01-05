/**
 * @file d_a_npc_kdk.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_kdk.h"

actor_process_profile_definition g_profile_NPC_KDK = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_KDK,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpcKdk_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  320,                    // mPriority
  NULL,                   // sub_method
  0x00044107,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
