/**
 * f_pc_priority.cpp
 * Framework - Process Priority
 */

#include "f_pc/f_pc_priority.h"
#include "f_pc/f_pc_base.h"
#include "f_pc/f_pc_layer.h"

s32 fpcPi_IsInQueue(process_priority_class* i_procPriority) {
    return cTg_IsUse(&i_procPriority->base);
}

s32 fpcPi_QueueTo(process_priority_class* i_procPriority) {
    cTg_SingleCut(&i_procPriority->base);
    fpcLy_CancelQTo(&i_procPriority->method_tag);
    return 1;
}

static node_list_class l_fpcPi_Queue;

s32 fpcPi_ToQueue(process_priority_class* i_procPriority) {
    fpc_ProcID layer = i_procPriority->queue_info.layer_id;

    if (cTg_Addition(&l_fpcPi_Queue, &i_procPriority->base)) {
        if (layer != fpcLy_CURRENT_e) {
            layer_class* pLayer = fpcLy_Layer(layer);

            if (!fpcLy_ToCancelQ(pLayer, &i_procPriority->method_tag)) {
                cTg_SingleCut(&i_procPriority->base);
                return 0;
            }
        }
        return 1;
    }

    return 0;
}

process_priority_class* fpcPi_GetFromQueue() {
    process_priority_class* i_procPriority = (process_priority_class*)cTg_GetFirst(&l_fpcPi_Queue);

    if (i_procPriority != NULL) {
        process_priority_class* ppriority = &((base_process_class*)i_procPriority->base.mpTagData)->priority;
        fpcLy_CancelQTo(&ppriority->method_tag);
        return ppriority;
    }

    return NULL;
}

s32 fpcPi_Delete(process_priority_class* i_procPriority) {
    static priority_id crear = {
        fpcLy_NONE_e,
        0xFFFF,
        fpcPi_NONE_e,
    };

    fpcPi_QueueTo(i_procPriority);
    i_procPriority->queue_info = crear;
    return 1;
}

s32 fpcPi_IsNormal(fpc_ProcID i_layerID, u16 i_listID, u16 i_priority) {
    if ((i_layerID < fpcLy_SPECIAL_e) && (i_listID < 0xFFFE) && (i_priority < fpcPi_SPECIAL_e))
        return 1;

    return 0;
}

s32 fpcPi_Change(process_priority_class* i_procPriority, fpc_ProcID i_layerID, u16 i_listID, u16 i_priority) {
    base_process_class* process = (base_process_class*)i_procPriority->base.mpTagData;
    BOOL changed = FALSE;

    if (process->state.init_state == 3)
        return 0;

    if (!fpcPi_IsNormal(i_layerID, i_listID, i_priority))
        return 0;

    i_procPriority->queue_info = i_procPriority->current_info;

    if (i_layerID != fpcLy_CURRENT_e && i_procPriority->current_info.layer_id != i_layerID) {
        i_procPriority->queue_info.layer_id = i_layerID;
        changed = TRUE;
    }

    if (i_listID != 0xFFFD && i_procPriority->current_info.list_id != i_listID) {
        i_procPriority->queue_info.list_id = i_listID;
        changed = TRUE;
    }

    if (i_priority != fpcPi_CURRENT_e && i_procPriority->current_info.list_priority != i_priority) {
        i_procPriority->queue_info.list_priority = i_priority;
        changed = TRUE;
    }

    if (process->state.init_state == 0 || process->state.init_state == 1) {
        i_procPriority->current_info = i_procPriority->queue_info;
        return 1;
    }

    if (changed == TRUE) {
        int ret = fpcPi_ToQueue(i_procPriority);
        return ret;
    }

    return 0;
}

s32 fpcPi_Handler() {
    process_priority_class* i_procPriority;
    while (i_procPriority = fpcPi_GetFromQueue()) {
        base_process_class* process = (base_process_class*)i_procPriority->base.mpTagData;
        layer_management_tag_class* pLayerTag = &process->layer_tag;
        line_tag* pLineTag = &process->line_tag_;

        if (fpcLyTg_Move(pLayerTag, i_procPriority->queue_info.layer_id, i_procPriority->queue_info.list_id,
                         i_procPriority->queue_info.list_priority) == 1)
        {
            fpcLnTg_Move(pLineTag, i_procPriority->current_info.list_id);
            i_procPriority->current_info = i_procPriority->queue_info;
        } else {
            return 0;
        }
    }

    return 1;
}

s32 fpcPi_Init(process_priority_class* i_procPriority, void* i_data, fpc_ProcID i_layerID, u16 i_listID,
               u16 i_priority) {
    if (!fpcPi_IsNormal(i_layerID, i_listID, i_priority))
        return 0;

    i_procPriority->queue_info.layer_id = i_layerID;
    i_procPriority->queue_info.list_id = i_listID;
    i_procPriority->queue_info.list_priority = i_priority;

    i_procPriority->current_info = i_procPriority->queue_info;

    cTg_Create(&i_procPriority->base, i_data);
    fpcMtdTg_Init(&i_procPriority->method_tag, (process_method_tag_func)fpcPi_Delete, i_procPriority);
    return 1;
}
