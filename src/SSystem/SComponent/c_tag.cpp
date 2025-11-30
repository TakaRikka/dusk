/**
 * c_tag.cpp
 *
 */

#include "SSystem/SComponent/c_tag.h"
#include "SSystem/SComponent/c_list.h"
#include "SSystem/SComponent/c_tree.h"

int cTg_IsUse(create_tag_class* tag) {
    return tag->mbIsUse;
}

int cTg_SingleCutFromTree(create_tag_class* tag) {
    if (tag->mbIsUse == true) {
        tag->mbIsUse = false;
        cTr_SingleCut(&tag->mpNode);
        return true;
    } else {
        return false;
    }
}

int cTg_AdditionToTree(node_lists_tree_class* tree, int listIdx, create_tag_class* tag) {
    if (!tag->mbIsUse) {
        int ret = cTr_Addition(tree, listIdx, &tag->mpNode);
        if (ret) {
            tag->mbIsUse = true;
            return ret;
        }
    }

    return 0;
}

int cTg_InsertToTree(node_lists_tree_class* tree, int listIdx, create_tag_class* tag, int idx) {
    if (!tag->mbIsUse) {
        int ret = cTr_Insert(tree, listIdx, &tag->mpNode, idx);
        if (ret) {
            tag->mbIsUse = true;
            return ret;
        }
    }

    return 0;
}

node_class* cTg_GetFirst(node_list_class* list) {
    create_tag_class* tag = (create_tag_class*)cLs_GetFirst(list);
    if (tag != NULL) {
        tag->mbIsUse = false;
        return &tag->mpNode;
    }

    return NULL;
}

int cTg_SingleCut(create_tag_class* tag) {
    if (tag->mbIsUse == 1) {
        tag->mbIsUse = false;
        cLs_SingleCut(&tag->mpNode);
        return 1;
    }

    return 0;
}

int cTg_Addition(node_list_class* list, create_tag_class* tag) {
    if (!tag->mbIsUse) {
        int ret = cLs_Addition(list, &tag->mpNode);
        if (ret) {
            tag->mbIsUse = true;
            return ret;
        }
    }

    return 0;
}

void cTg_Create(create_tag_class* tag, void* data) {
    cNd_Create(&tag->mpNode, NULL);
    tag->mpTagData = data;
    tag->mbIsUse = false;
}
