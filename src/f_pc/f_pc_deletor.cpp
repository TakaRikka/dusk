/**
 * f_pc_deletor.cpp
 * Framework - Process Deletor
 */

#include "f_pc/f_pc_deletor.h"
#include "SSystem/SComponent/c_list_iter.h"
#include "f_pc/f_pc_creator.h"
#include "f_pc/f_pc_executor.h"
#include "f_pc/f_pc_load.h"
#include "f_pc/f_pc_node.h"
#include "f_pc/f_pc_debug_sv.h"
#include "JSystem/JUtility/JUTAssert.h"

BOOL fpcDt_IsComplete() {
    return fpcDtTg_IsEmpty();
}

s32 fpcDt_deleteMethod(base_process_class* i_proc) {
    fpc_ProcID id = i_proc->id;
    layer_class* layer = i_proc->delete_tag.layer;
    s16 profname = i_proc->profname;

    fpcLy_SetCurrentLayer(layer);
    fpcLnTg_QueueTo(&i_proc->line_tag_);

    if (fpcBs_Delete(i_proc) == 1) {
        fpcLy_DeletedMesg(layer);
        fpcLd_Free(profname);
        return 1;
    }

    return 0;
}

void fpcDt_Handler() {
#if DEBUG
    if (g_fpcDbSv_service[6] != NULL) {
        g_fpcDbSv_service[6](&g_fpcDtTg_Queue.mSize);
    }
#endif
    cLsIt_Method(&g_fpcDtTg_Queue, (cNdIt_MethodFunc)fpcDtTg_Do, (void*)fpcDt_deleteMethod);
}

s32 fpcDt_ToQueue(base_process_class* i_proc) {
    if (i_proc->unk_0xA != 1 && fpcBs_IsDelete(i_proc) == 1) {
        if (fpcPi_IsInQueue(&i_proc->priority) == 1) {
            fpcPi_Delete(&i_proc->priority);
        }

        i_proc->delete_tag.layer = i_proc->layer_tag.layer;
        fpcDtTg_ToDeleteQ(&i_proc->delete_tag);
        fpcLy_DeletingMesg(i_proc->layer_tag.layer);
        return 1;
    }

    return 0;
}

s32 fpcDt_ToDeleteQ(base_process_class* i_proc) {
    if (i_proc->unk_0xA == 1) {
        return 0;
    }

    if (cTg_IsUse(&i_proc->delete_tag.base)) {
        return 1;
    }

    if (fpcBs_Is_JustOfType(g_fpcNd_type, i_proc->subtype)) {
        if (fpcNd_IsDeleteTiming((process_node_class*)i_proc) == 0) {
            return 0;
        }

        layer_class* layer = &((process_node_class*)i_proc)->layer;
        
        if (fpcLy_Cancel(layer) == 0) {
            JUT_ASSERT(196, FALSE);
        }

        if (fpcLyIt_OnlyHereLY(layer, (fpcLyIt_OnlyHereFunc)fpcDt_ToDeleteQ, NULL) == 0)
        {
            return 0;
        }
    }

    if (fpcDt_ToQueue(i_proc) == 1) {
        if (fpcEx_IsExist(i_proc->id) == 1) {
            if (fpcEx_ExecuteQTo(i_proc) == 0) {
                return 0;
            }
        } else {
            if (fpcCt_Abort(i_proc) == 0) {
                return 0;
            }
        }

        i_proc->state.init_state = 3;
        return 1;
    }
    
    return 0;
}

s32 fpcDt_Delete(void* i_proc) {
    base_process_class* proc = (base_process_class*)i_proc;
    
    if (proc != NULL) {
#if DEBUG
        if (!fpcBs_Is_JustOfType(g_fpcBs_type, proc->type)) {
            if (g_fpcDbSv_service[12] != NULL) {
                g_fpcDbSv_service[12](proc);
            }
            
            return 0;
        }
#endif
        if (fpcCt_IsDoing(proc) == TRUE)
            return 0;

        if (proc->state.init_state == 3)
            return 0;

        int ret = fpcDt_ToDeleteQ(proc);
#if DEBUG
        if (ret == 0) {
            if (g_fpcDbSv_service[5] != NULL) {
                g_fpcDbSv_service[5](proc);
            }
        }
#endif
        return ret;
    }
    
    return 1;
}
