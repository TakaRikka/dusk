/**
 * f_pc_layer.cpp
 * Framework - Process Layer
 */

#include "f_pc/f_pc_layer.h"
#include "f_pc/f_pc_method_iter.h"
#include "f_pc/f_pc_method_tag.h"

void fpcLy_CancelQTo(process_method_tag_class* i_methods) {
    fpcMtdTg_MethodQTo(i_methods);
}

s32 fpcLy_ToCancelQ(layer_class* i_layer, process_method_tag_class* i_methods) {
    return fpcMtdTg_ToMethodQ(&i_layer->cancel_list, i_methods);
}

BOOL fpcLy_CancelMethod(process_method_tag_class* i_layer) {
    if (fpcMtdTg_Do(i_layer) == 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

s32 fpcLy_IntoQueue(layer_class* i_layer, int i_treeListNo, create_tag_class* i_createTag, int i_no) {
    int ret = cTg_InsertToTree(&i_layer->node_tree, i_treeListNo, i_createTag, i_no);
    return ret;
}

s32 fpcLy_ToQueue(layer_class* i_layer, int i_treeListNo, create_tag_class* i_createTag) {
    int ret = cTg_AdditionToTree(&i_layer->node_tree, i_treeListNo, i_createTag);
    return ret;
}

s32 fpcLy_QueueTo(layer_class* i_layer, create_tag_class* i_createTag) {
    i_layer;

    int ret = cTg_SingleCutFromTree(i_createTag);
    return ret;
}

BOOL fpcLy_IsDeletingMesg(layer_class* i_layer) {
    BOOL ret;
    if (i_layer->counts.delete_count > 0) {
        ret = TRUE;
    } else {
        ret = FALSE;
    }

    return ret;
}

void fpcLy_DeletingMesg(layer_class* i_layer) {
    i_layer->counts.delete_count++;
}

void fpcLy_DeletedMesg(layer_class* i_layer) {
    if (i_layer->counts.delete_count > 0) {
        i_layer->counts.delete_count--;
    }
}

BOOL fpcLy_IsCreatingMesg(layer_class* i_layer) {
    BOOL ret;
    if (i_layer->counts.create_count > 0) {
        ret = TRUE;
    } else {
        ret = FALSE;
    }

    return ret;
}

void fpcLy_CreatingMesg(layer_class* i_layer) {
    i_layer->counts.create_count++;
}

void fpcLy_CreatedMesg(layer_class* i_layer) {
    if (i_layer->counts.create_count > 0) {
        i_layer->counts.create_count--;
    }
}

layer_class l_fpcLy_Crear = {
    NULL, NULL, 0, 0xFFFFFFFF, NULL, 0, NULL, NULL, NULL, 0, 0, 0,
};

static node_list_class l_fpcLy_LayerList = {
    NULL,
    NULL,
    0,
};

layer_class* fpcLy_RootLayer() {
    return (layer_class*)l_fpcLy_LayerList.mpHead;
}

static layer_class* l_fpcLy_CurrLayer_p;

void fpcLy_SetCurrentLayer(layer_class* i_layer) {
    l_fpcLy_CurrLayer_p = i_layer;
}

layer_class* fpcLy_CurrentLayer() {
    return l_fpcLy_CurrLayer_p;
}

layer_class* fpcLy_Search(fpc_ProcID i_id) {
    layer_class* layer_p = fpcLy_RootLayer();
    while (layer_p != NULL) {
        if (layer_p->layer_id == i_id) {
            return layer_p;
        }
        layer_p = (layer_class*)layer_p->node.mpNextNode;
    }

    return NULL;
}

layer_class* fpcLy_Layer(fpc_ProcID i_id) {
    if (i_id == 0 || fpcLy_RootLayer()->layer_id == i_id) {
        return fpcLy_RootLayer();
    } else if (i_id == fpcLy_CURRENT_e || fpcLy_CurrentLayer()->layer_id == i_id) {
        return fpcLy_CurrentLayer();
    } else {
        return fpcLy_Search(i_id);
    }
}

void fpcLy_Regist(layer_class* i_layer) {
    cLs_Addition(&l_fpcLy_LayerList, (node_class*)i_layer);
}

s32 fpcLy_Delete(layer_class* i_layer) {
    if (i_layer->node_tree.mpLists->mSize == 0 && i_layer->cancel_list.mSize == 0) {
        cLs_SingleCut((node_class*)i_layer);
        *i_layer = l_fpcLy_Crear;
        return 1;
    }
    
    return 0;
}

int fpcLy_Cancel(layer_class* i_layer) {
    return fpcMtdIt_Method(&i_layer->cancel_list, (fpcMtdIt_MethodFunc)fpcLy_CancelMethod);
}

void fpcLy_Create(layer_class* i_layer, void* i_node, node_list_class* i_nodeList, int i_numLists) {
    static BOOL IsInitOfLayerList = TRUE;
    static fpc_ProcID layer_id = 0;

    *i_layer = l_fpcLy_Crear;
    cNd_Create((node_class*)i_layer, NULL);

    i_layer->layer_id = layer_id++;
    i_layer->process_node = (process_node_class*)i_node;

    if (IsInitOfLayerList == TRUE) {
        IsInitOfLayerList = FALSE;
        cLs_Create(&l_fpcLy_LayerList);
        fpcLy_SetCurrentLayer(i_layer);
    }

    i_layer->node_tree.mpLists = i_nodeList;
    i_layer->node_tree.mNumLists = i_numLists;
    cTr_Create(&i_layer->node_tree, i_layer->node_tree.mpLists, i_layer->node_tree.mNumLists);
    fpcLy_Regist(i_layer);
}
