/**
 * d_s_title.cpp
 * Title Screen Scene
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_s_title.h"
#include "f_op/f_op_scene.h"
#include "f_pc/f_pc_leaf.h"

extern scene_process_profile_definition g_profile_WARNING_SCENE = {
    fpcLy_ROOT_e,           // mLayerID     
    1,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_WARNING_SCENE,     // mProcName               
    &g_fpcNd_Method.base,  // sub_method                  
    0x204,                  // mSize        
    0,                      // mSizeOther              
    0,                      // mParameters              
    &g_fopScn_Method.base, // sub_method                   
    NULL,                   // mpMtd
};

extern scene_process_profile_definition g_profile_WARNING2_SCENE = {
    fpcLy_ROOT_e,           // mLayerID          
    1,                      // mListID   
    fpcPi_CURRENT_e,        // mListPrio    
    PROC_WARNING2_SCENE,    // mProcName                     
    &g_fpcNd_Method.base,  // sub_method                       
    0x204,                  // mSize             
    0,                      // mSizeOther                
    0,                      // mParameters                
    &g_fopScn_Method.base, // sub_method                        
    NULL,                   // mpMtd
};
