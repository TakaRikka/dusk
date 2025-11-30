/**
 * c_tree.cpp
 *
 */

#include "SSystem/SComponent/c_tree.h"
#include "SSystem/SComponent/c_list.h"

int cTr_SingleCut(node_class* node) {
    return cLs_SingleCut(node);
}

int cTr_Addition(node_lists_tree_class* tree, int listIdx, node_class* node) {
    if (listIdx >= tree->mNumLists)
        return 0;

    return cLs_Addition(&tree->mpLists[listIdx], node);
}

int cTr_Insert(node_lists_tree_class* tree, int listIdx, node_class* node, int idx) {
    if (listIdx >= tree->mNumLists)
        return 0;

    return cLs_Insert(&tree->mpLists[listIdx], idx, node);
}

void cTr_Create(node_lists_tree_class* tree, node_list_class* lists, int numLists) {
    tree->mpLists = lists;
    tree->mNumLists = numLists;

    while (numLists-- > 0)
        cLs_Create(lists++);
}
