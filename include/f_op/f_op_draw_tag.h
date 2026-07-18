#ifndef F_F_OP_DRAW_TAG
#define F_F_OP_DRAW_TAG

#include "SSystem/SComponent/c_tree.h"

typedef struct create_tag_class create_tag_class;

DUSK_GAME_EXTERN node_lists_tree_class g_fopDwTg_Queue;

void fopDwTg_DrawQTo(create_tag_class* i_createTag);
void fopDwTg_CreateQueue();
bool fopDwTg_Init(create_tag_class* i_createTag, void* i_process);
void fopDwTg_ToDrawQ(create_tag_class* i_createTag, int priority);

#endif
