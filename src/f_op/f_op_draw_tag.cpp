/**
 * f_op_draw_tag.cpp
 *
 */

#include "f_op/f_op_draw_tag.h"
#include "SSystem/SComponent/c_list.h"
#include "SSystem/SComponent/c_tag.h"
#include "global.h"

node_lists_tree_class g_fopDwTg_Queue = {NULL, 0};

void fopDwTg_ToDrawQ(create_tag_class* i_createTag, int i_priority) {
    cTg_AdditionToTree(&g_fopDwTg_Queue, i_priority, i_createTag);
}

void fopDwTg_DrawQTo(create_tag_class* i_createTag) {
    cTg_SingleCutFromTree(i_createTag);
}

bool fopDwTg_Init(create_tag_class* i_createTag, void* i_process) {
    cTg_Create(i_createTag, i_process);
    return true;
}

void fopDwTg_CreateQueue() {
    static node_list_class lists[1000];

    cTr_Create(&g_fopDwTg_Queue, lists, ARRAY_SIZEU(lists));
}
