/**
 * f_pc_node.cpp
 * Framework - Process Node
 */

#include "f_pc/f_pc_node.h"
#include "f_pc/f_pc_layer_iter.h"
#include "f_pc/f_pc_debug_sv.h"

int fpcNd_DrawMethod(nodedraw_method_class* i_method_class, void* i_data) {
    return fpcMtd_Method(i_method_class->draw_method, i_data);
}

int fpcNd_Draw(process_node_class* i_procNode) {
    int ret = 0;
    process_node_class* var_r28 = i_procNode;

    if (i_procNode->unk_0x1A8 == 0) {
        layer_class* save_layer = fpcLy_CurrentLayer();
        fpcLy_SetCurrentLayer(&var_r28->layer);
        ret = fpcNd_DrawMethod(i_procNode->nodedraw_method, i_procNode);
        fpcLy_SetCurrentLayer(save_layer);
    }

    return ret;
}

int fpcNd_Execute(process_node_class* i_procNode) {
    int ret = 0;
    layer_class* save_layer = fpcLy_CurrentLayer();

#if DEBUG
    if (fpcBs_Is_JustOfType(g_fpcNd_type, i_procNode->base.subtype) == 0) {
        if (g_fpcDbSv_service[11] != NULL) {
            g_fpcDbSv_service[11](i_procNode);
        }
        return 0;
    }
#endif

    fpcLy_SetCurrentLayer(&i_procNode->layer);
    ret = fpcMtd_Execute(&i_procNode->nodedraw_method->base, i_procNode);
    fpcLy_SetCurrentLayer(save_layer);
    return ret;
}

int g_fpcNd_type;

void* fpcNd_IsCreatingFromUnder(void* i_procNode) {
    if (i_procNode != NULL && fpcBs_Is_JustOfType(g_fpcNd_type, ((process_node_class*)i_procNode)->base.subtype) != FALSE)
    {
        layer_class* layer;
        layer = &((process_node_class*)i_procNode)->layer;
        if (fpcLy_IsCreatingMesg(layer) == FALSE) {
            return (process_node_class*)fpcLyIt_Judge(
                layer, (fpcLyIt_JudgeFunc)fpcNd_IsCreatingFromUnder, NULL);
        } else {
            return i_procNode;
        }
    }

    return NULL;
}

BOOL g_fpcNd_IsCheckOfDeleteTiming = TRUE;

int fpcNd_IsDeleteTiming(process_node_class* i_procNode) {
    if (g_fpcNd_IsCheckOfDeleteTiming == TRUE && fpcNd_IsCreatingFromUnder(i_procNode) != NULL) {
        return 0;
    }
    
    return 1;
}

int fpcNd_IsDelete(process_node_class* i_procNode) {
    return fpcMtd_IsDelete(&i_procNode->nodedraw_method->base, i_procNode);
}

int fpcNd_Delete(process_node_class* i_procNode) {
    if (!fpcLy_IsDeletingMesg(&i_procNode->layer) &&
        fpcMtd_Delete(&i_procNode->nodedraw_method->base, i_procNode) == 1)
    {
        i_procNode->base.subtype = 0;
        return fpcLy_Delete(&i_procNode->layer);
    }

    return 0;
}

int fpcNd_Create(process_node_class* i_procNode) {
    int ret;
    layer_class* save_layer;
    if (i_procNode->base.state.init_state == 0) {
        node_process_profile_definition* pprofile =
            (node_process_profile_definition*)i_procNode->base.profile;
        i_procNode->base.subtype = fpcBs_MakeOfType(&g_fpcNd_type);
        i_procNode->nodedraw_method =
            (nodedraw_method_class*)pprofile->sub_methods;
        fpcLy_Create(&i_procNode->layer, i_procNode, i_procNode->layer_nodelist, 16);
        i_procNode->unk_0x1A8 = 0;
    }

    save_layer = fpcLy_CurrentLayer();
    fpcLy_SetCurrentLayer(&i_procNode->layer);
    ret = fpcMtd_Create(&i_procNode->nodedraw_method->base, i_procNode);
    fpcLy_SetCurrentLayer(save_layer);
    return ret;
}

nodedraw_method_class g_fpcNd_Method = {
    (process_method_func)fpcNd_Create, (process_method_func)fpcNd_Delete,
    (process_method_func)fpcNd_Execute, (process_method_func)fpcNd_IsDelete,
    (process_method_func)fpcNd_Draw,
};
