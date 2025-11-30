/**
 * c_tree_iter.cpp
 *
 */

#include "SSystem/SComponent/c_tree_iter.h"
#include "SSystem/SComponent/c_list.h"
#include "SSystem/SComponent/c_list_iter.h"
#include "SSystem/SComponent/c_tree.h"
#include <dolphin/types.h>

int cTrIt_Method(node_lists_tree_class* tree, cNdIt_MethodFunc method, void* data) {
    node_list_class* list = tree->mpLists;
    int i = tree->mNumLists;
    int ret = 1;
    while (i-- > 0) {
        if (cLsIt_Method(list++, method, data) == 0)
            ret = 0;
    }
    return ret;
}

void* cTrIt_Judge(node_lists_tree_class* tree, cNdIt_JudgeFunc judge, void* data) {
    node_list_class* list = tree->mpLists;
    int i = tree->mNumLists;
    void* pJudgeRet;

    while (i-- > 0) {
        pJudgeRet = cLsIt_Judge(list++, judge, data);
        if (pJudgeRet != NULL)
            return pJudgeRet;
    }

    return NULL;
}
