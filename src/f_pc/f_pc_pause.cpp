/**
 * f_pc_pause.cpp
 * Framework - Process Pause
 */

#include "f_pc/f_pc_pause.h"
#include "f_pc/f_pc_node.h"
#include "f_pc/f_pc_layer_iter.h"

s32 fpcPause_IsEnable(void* i_proc, u8 i_flag) {
    if ((((base_process_class*)i_proc)->pause_flag & i_flag) == i_flag) {
        return 1;
    } else {
        return 0;
    }
}

s32 fpcPause_Enable(void* i_proc, u8 i_flag) {
    ((base_process_class*)i_proc)->pause_flag |= i_flag;

    if (fpcBs_Is_JustOfType(g_fpcNd_type, ((base_process_class*)i_proc)->subtype)) {
        fpcLyIt_OnlyHere(&((process_node_class*)i_proc)->layer, (fpcLyIt_OnlyHereFunc)fpcPause_Enable,
                         (void*)i_flag);
    }
    return 1;
}

s32 fpcPause_Disable(void* i_proc, u8 i_flag) {
    u8 var_r31 = 0xFF - i_flag;
    ((base_process_class*)i_proc)->pause_flag &= var_r31;

    if (fpcBs_Is_JustOfType(g_fpcNd_type, ((base_process_class*)i_proc)->subtype)) {
        fpcLyIt_OnlyHere(&((process_node_class*)i_proc)->layer, (fpcLyIt_OnlyHereFunc)fpcPause_Disable, (void*)i_flag);
    }

    return 1;
}

void fpcPause_Init(void* i_proc) {
    ((base_process_class*)i_proc)->pause_flag = 0;
}
