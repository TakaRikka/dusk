/**
 * f_pc_create_tag.cpp
 * Framework - Process Create Tag
 */

#include "f_pc/f_pc_create_tag.h"

node_list_class g_fpcCtTg_Queue = {NULL, NULL, 0};

void fpcCtTg_ToCreateQ(create_tag* i_createTag) {
    cTg_Addition(&g_fpcCtTg_Queue, &i_createTag->base);
}

void fpcCtTg_CreateQTo(create_tag* i_createTag) {
    cTg_SingleCut(&i_createTag->base);
}

s32 fpcCtTg_Init(create_tag* i_createTag, void* i_data) {
    cTg_Create(&i_createTag->base, i_data);
    return 1;
}
