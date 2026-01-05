/**
 * f_op_draw_iter.cpp
 *
 */

#include "f_op/f_op_draw_iter.h"
#include "SSystem/SComponent/c_list.h"
#include "SSystem/SComponent/c_tag.h"
#include "f_op/f_op_draw_tag.h"

static int l_fopDwTg_id;

create_tag_class* fopDwIt_GetTag() {
    while (l_fopDwTg_id + 1 < g_fopDwTg_Queue.mNumLists) {
        node_list_class* list = &g_fopDwTg_Queue.mpLists[++l_fopDwTg_id];
        node_class* node = list->mpHead;
    
        if (node != NULL) {
            return (create_tag_class*)node;
        }
    }

    return NULL;
}

create_tag_class* fopDwIt_Begin() {
    create_tag_class* tag = (create_tag_class*)g_fopDwTg_Queue.mpLists->mpHead;
    l_fopDwTg_id = 0;

    if (tag != NULL) {
        return tag;
    }
    return fopDwIt_GetTag();
}

create_tag_class* fopDwIt_Next(create_tag_class* i_createTag) {
    (void)&i_createTag;
    create_tag_class* tag = (create_tag_class*)i_createTag->mpNode.mpNextNode;

    if (tag == NULL) {
        return fopDwIt_GetTag();
    }
    return tag;
}
