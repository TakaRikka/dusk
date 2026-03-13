/**
 * @file d_a_path_line.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_path_line.h"
#include "f_op/f_op_actor_mng.h"

actor_process_profile_definition2 g_profile_PATH_LINE = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 9,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_PATH_LINE_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ 0x00000574,
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_PATH_LINE_e,
    /* Actor SubMtd */ NULL,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_NOPAUSE_e,
    /* Group        */ fopAc_UNK_GROUP_5_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
    /* Unknown      */ 0                      // field_0x30,
};
