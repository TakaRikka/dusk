/**
 * c_list.cpp
 *
 */

#include "SSystem/SComponent/c_list.h"
#include "SSystem/SComponent/c_node.h"
#include <dolphin/types.h>

void cLs_Init(node_list_class* list) {
    list->mpHead = NULL;
    list->mpTail = NULL;
    list->mSize = 0;
}

int cLs_SingleCut(node_class* node) {
    node_list_class* list = (node_list_class*)node->mpData;

    if (node == list->mpHead)
        list->mpHead = node->mpNextNode;
    if (node == list->mpTail)
        list->mpTail = node->mpPrevNode;

    cNd_SingleCut(node);
    cNd_ClearObject(node);

    int ret;
    if (--list->mSize > 0) {
        ret = 1;
    } else {
        ret = 0;
    }

    return ret;
}

int cLs_Addition(node_list_class* list, node_class* node) {
    if (list->mpTail == NULL) {
        list->mpHead = node;
    } else {
        cNd_Addition(list->mpTail, node);
    }

    list->mpTail = cNd_Last(node);
    cNd_SetObject(node, list);
    list->mSize = cNd_LengthOf(list->mpHead);
    return list->mSize;
}

int cLs_Insert(node_list_class* list, int idx, node_class* node) {
    node_class* pExisting = cNd_Order(list->mpHead, idx);
    if (pExisting == NULL) {
        return cLs_Addition(list, node);
    }

    cNd_SetObject(node, list);
    cNd_Insert(pExisting, node);
    list->mpHead = cNd_First(node);
    list->mSize = cNd_LengthOf(list->mpHead);
    return list->mSize;
}

node_class* cLs_GetFirst(node_list_class* list) {
    if (list->mSize != 0) {
        node_class* pHead = list->mpHead;
        cLs_SingleCut(pHead);
        return pHead;
    }

    return NULL;
}

void cLs_Create(node_list_class* list) {
    cLs_Init(list);
}
