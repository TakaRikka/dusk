/**
 * c_list_iter.cpp
 *
 */

#include "SSystem/SComponent/c_list_iter.h"
#include "SSystem/SComponent/c_list.h"
#include "dolphin/types.h"

int cLsIt_Method(node_list_class* list, cNdIt_MethodFunc method, void* data) {
    if (list->mSize > 0)
        return cNdIt_Method(list->mpHead, method, data);
    else
        return 1;
}

void* cLsIt_Judge(node_list_class* list, cNdIt_JudgeFunc judge, void* data) {
    if (list->mSize > 0)
        return cNdIt_Judge(list->mpHead, judge, data);
    else
        return NULL;
}
