/**
 * f_pc_method_tag.cpp
 * Framework - Process Method Tag
 */

#include "f_pc/f_pc_method_tag.h"

s32 fpcMtdTg_Do(process_method_tag_class* i_methodTag) {
    return i_methodTag->method(i_methodTag->data);
}

s32 fpcMtdTg_ToMethodQ(node_list_class* i_nodelist, process_method_tag_class* i_methodTag) {
    return cTg_Addition(i_nodelist, &i_methodTag->create_tag);
}

void fpcMtdTg_MethodQTo(process_method_tag_class* i_methodTag) {
    cTg_SingleCut(&i_methodTag->create_tag);
}

s32 fpcMtdTg_Init(process_method_tag_class* i_methodTag, process_method_tag_func i_method, void* i_data) {
    cTg_Create(&i_methodTag->create_tag, i_methodTag);
    i_methodTag->method = i_method;
    i_methodTag->data = i_data;
    return 1;
}
