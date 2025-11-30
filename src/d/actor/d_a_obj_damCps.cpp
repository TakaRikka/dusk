/**
 * @file d_a_obj_damCps.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_damCps.h"

extern actor_process_profile_definition g_profile_Obj_DamCps = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_DamCps,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(fopAc_ac_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  629,                    // mPriority
  NULL,                   // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
