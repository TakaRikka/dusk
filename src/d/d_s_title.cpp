/**
 * d_s_title.cpp
 * Title Screen Scene
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_s_title.h"
#include "f_op/f_op_scene.h"
#include "f_pc/f_pc_leaf.h"

scene_process_profile_definition g_profile_WARNING_SCENE = {
    /* Layer ID     */ fpcLy_ROOT_e,
    /* List ID      */ 1,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_WARNING_SCENE_e,
    /* Proc SubMtd  */ &g_fpcNd_Method.base,
    /* Size         */ 0x204,
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopScn_Method.base,
    /* Scene SubMtd */ NULL,
};

scene_process_profile_definition g_profile_WARNING2_SCENE = {
    /* Layer ID     */ fpcLy_ROOT_e,
    /* List ID      */ 1,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_WARNING2_SCENE_e,
    /* Proc SubMtd  */ &g_fpcNd_Method.base,
    /* Size         */ 0x204,
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopScn_Method.base,
    /* Scene SubMtd */ NULL,
};
