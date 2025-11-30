/**
 * @file d_a_npc_henna0.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_henna0.h"
extern actor_process_profile_definition g_profile_NPC_HENNA0 = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_HENNA0,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(d_a_npc_henna0), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  343,                    // mPriority
  NULL,                   // sub_method
  0x00044100,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
