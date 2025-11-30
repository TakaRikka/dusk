/**
 * c_node_iter.cpp
 *
 */

#include "SSystem/SComponent/c_node_iter.h"
#include "SSystem/SComponent/c_node.h"
#include <dolphin/types.h>

int cNdIt_Method(node_class* node, cNdIt_MethodFunc method, void* data) {
    int ret = 1;
    node_class* pNext = NODE_GET_NEXT(node);

    while (node) {
        if (!method(node, data))
            ret = 0;
        node = pNext;
        pNext = NODE_GET_NEXT(pNext);
    }

    return ret;
}

void* cNdIt_Judge(node_class* node, cNdIt_JudgeFunc judge, void* data) {
    node_class* pNext = NODE_GET_NEXT(node);

    while (node) {
        void* ret = judge(node, data);
        if (ret != NULL)
            return ret;
        node = pNext;
        pNext = NODE_GET_NEXT(pNext);
    }

    return NULL;
}
