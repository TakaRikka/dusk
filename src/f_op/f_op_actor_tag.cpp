/**
 * f_op_actor_tag.cpp
 *
 */

#include "f_op/f_op_actor_tag.h"
#include "SSystem/SComponent/c_list.h"

node_list_class g_fopAcTg_Queue = {NULL, NULL, 0};

int fopAcTg_ToActorQ(create_tag_class* i_createTag) {
    return cTg_Addition(&g_fopAcTg_Queue, i_createTag);
}

int fopAcTg_ActorQTo(create_tag_class* i_createTag) {
    return cTg_SingleCutFromTree(i_createTag);
}

int fopAcTg_Init(create_tag_class* i_createTag, void* i_data) {
    cTg_Create(i_createTag, i_data);
    return 1;
}
