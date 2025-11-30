/**
 * c_node.cpp
 *
 */

#include "SSystem/SComponent/c_node.h"
#include <dolphin/types.h>

void cNd_Join(node_class* node_a, node_class* node_b) {
    node_a->mpNextNode = node_b;
    node_b->mpPrevNode = node_a;
}

int cNd_LengthOf(node_class* node) {
    int count = 0;
    while (node) {
        count++;
        node = NODE_GET_NEXT(node);
    }
    return count;
}

node_class* cNd_First(node_class* node) {
    node_class* ret = NULL;
    while (node) {
        ret = node;
        node = NODE_GET_PREV(node);
    }
    return ret;
}

node_class* cNd_Last(node_class* node) {
    node_class* ret = NULL;
    while (node) {
        ret = node;
        node = NODE_GET_NEXT(node);
    }
    return ret;
}

node_class* cNd_Order(node_class* node, int idx) {
    node_class* ret = NULL;
    int i = 0;
    while (i < idx && node) {
        ret = node;
        i++;
        node = NODE_GET_NEXT(node);
    }

    return i < idx ? ret : NULL;
}

void cNd_SingleCut(node_class* node) {
    node_class* prev = node->mpPrevNode;
    node_class* next = node->mpNextNode;

    if (prev)
        prev->mpNextNode = node->mpNextNode;
    if (next)
        next->mpPrevNode = node->mpPrevNode;
    node->mpPrevNode = NULL;
    node->mpNextNode = NULL;
}

void cNd_Cut(node_class* node) {
    if (node->mpPrevNode)
        node->mpPrevNode->mpNextNode = NULL;
    node->mpPrevNode = NULL;
}

void cNd_Addition(node_class* node_a, node_class* node_b) {
    node_class* pLast = cNd_Last(node_a);
    cNd_Join(pLast, node_b);
}

void cNd_Insert(node_class* node_a, node_class* node_b) {
    if (node_a->mpPrevNode == NULL) {
        cNd_Addition(node_b, node_a);
    } else {
        node_class* prev = node_a->mpPrevNode;
        cNd_Cut(node_a);
        cNd_Addition(prev, node_b);
        cNd_Addition(node_b, node_a);
    }
}

void cNd_SetObject(node_class* node, void* data) {
    while (node) {
        node->mpData = data;
        node = NODE_GET_NEXT(node);
    }
}

void cNd_ClearObject(node_class* node) {
    cNd_SetObject(node, NULL);
}

void cNd_ForcedClear(node_class* node) {
    node->mpPrevNode = NULL;
    node->mpNextNode = NULL;
    node->mpData = NULL;
}

void cNd_Create(node_class* node, void* data) {
    node->mpPrevNode = NULL;
    node->mpNextNode = NULL;
    node->mpData = data;
}
