#ifndef C_TAG_H
#define C_TAG_H

#include "SSystem/SComponent/c_node.h"
#include <dolphin/types.h>

typedef struct node_list_class node_list_class;
typedef struct node_lists_tree_class node_lists_tree_class;

typedef struct create_tag_class {
    node_class mpNode;
    void* mpTagData;
    s8 mbIsUse;
} create_tag_class;

int cTg_IsUse(create_tag_class* pTag);
int cTg_SingleCutFromTree(create_tag_class* pTag);
int cTg_AdditionToTree(node_lists_tree_class* pTree, int listIdx, create_tag_class* pTag);
int cTg_InsertToTree(node_lists_tree_class* pTree, int listIdx, create_tag_class* pTag, int idx);
node_class* cTg_GetFirst(node_list_class* pTag);
int cTg_SingleCut(create_tag_class* pTag);
int cTg_Addition(node_list_class* pList, create_tag_class* pTag);
void cTg_Create(create_tag_class* pTag, void* pData);

#endif /* C_TAG_H */
