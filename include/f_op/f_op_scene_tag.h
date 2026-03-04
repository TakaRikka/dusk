#ifndef F_F_OP_SCENE_TAG_H_
#define F_F_OP_SCENE_TAG_H_

#include "f_pc/f_pc_node.h"

class scene_tag_class {
public:
    create_tag_class base;
};

void fopScnTg_QueueTo(scene_tag_class* i_sceneTag);
void fopScnTg_ToQueue(scene_tag_class* i_sceneTag);
void fopScnTg_Init(scene_tag_class* i_sceneTag, void* i_data);

extern node_list_class g_fopScnTg_SceneList;

#endif
