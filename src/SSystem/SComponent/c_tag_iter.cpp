/**
 * c_tag_iter.cpp
 *
 */

#include "SSystem/SComponent/c_tag_iter.h"
#include "SSystem/SComponent/c_tag.h"

int cTgIt_MethodCall(create_tag_class* tag, method_filter* filter) {
    return filter->mpMethodFunc((node_class*)tag->mpTagData, filter->mpUserData);
}

void* cTgIt_JudgeFilter(create_tag_class* tag, judge_filter* filter) {
    return filter->mpJudgeFunc((node_class*)tag->mpTagData, filter->mpUserData);
}
