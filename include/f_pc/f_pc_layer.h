
#ifndef F_PC_LAYER_H_
#define F_PC_LAYER_H_

#include "SSystem/SComponent/c_list.h"
#include "SSystem/SComponent/c_node.h"
#include "SSystem/SComponent/c_tree.h"
#include "f_pc/f_pc_base.h"

typedef struct create_tag_class create_tag_class;
typedef struct process_method_tag_class process_method_tag_class;
typedef struct process_node_class process_node_class;

typedef struct layer_class {
    /* 0x00 */ node_class node;
    /* 0x0C */ fpc_ProcID layer_id;
    /* 0x10 */ node_lists_tree_class node_tree;
    /* 0x18 */ process_node_class* process_node;
    /* 0x1C */ node_list_class cancel_list;
    struct {
        /* 0x28 */ s16 create_count;
        /* 0x2A */ s16 delete_count;
    } counts;
} layer_class;

void fpcLy_SetCurrentLayer(layer_class* i_layer);
layer_class* fpcLy_CurrentLayer();
layer_class* fpcLy_RootLayer();
layer_class* fpcLy_Layer(fpc_ProcID i_id);
layer_class* fpcLy_Search(fpc_ProcID i_id);
void fpcLy_Regist(layer_class* i_layer);

void fpcLy_CreatedMesg(layer_class* i_layer);
void fpcLy_CreatingMesg(layer_class* i_layer);
void fpcLy_DeletedMesg(layer_class* i_layer);
void fpcLy_DeletingMesg(layer_class* i_layer);
BOOL fpcLy_IsCreatingMesg(layer_class* i_layer);
BOOL fpcLy_IsDeletingMesg(layer_class* i_layer);

int fpcLy_IntoQueue(layer_class* i_layer, int i_treeListNo, create_tag_class* i_createTag, int i_no);
int fpcLy_ToQueue(layer_class* i_layer, int treeListIdx, create_tag_class* i_createTag);
int fpcLy_QueueTo(layer_class* i_layer, create_tag_class* i_createTag);

int fpcLy_Cancel(layer_class* i_layer);
BOOL fpcLy_CancelMethod(process_method_tag_class* i_layer);

void fpcLy_CancelQTo(process_method_tag_class* i_methods);
int fpcLy_ToCancelQ(layer_class* i_layer, process_method_tag_class* i_methods);

void fpcLy_Create(layer_class* i_layer, void* i_node, node_list_class* i_nodeList, int i_numLists);

int fpcLy_Delete(layer_class* i_layer);

#endif
