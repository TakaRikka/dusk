/**
 * @file d_a_path_line.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_path_line.h"
#include "f_op/f_op_actor_mng.h"

extern actor_process_profile_definition2 g_profile_PATH_LINE = {
  fpcLy_CURRENT_e,       // mLayerID
  9,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_PATH_LINE,        // mProcName
  &g_fpcLf_Method.base, // sub_method
  0x00000574,            // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  758,                   // mPriority
  NULL,                  // sub_method
  0x00060000,            // mStatus
  fopAc_UNK_GROUP_5_e,   // mActorType
  fopAc_CULLBOX_0_e,     // cullType
  0                      // field_0x30
};
