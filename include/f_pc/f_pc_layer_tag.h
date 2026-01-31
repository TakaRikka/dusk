
#ifndef F_PC_LAYER_TAG_H_
#define F_PC_LAYER_TAG_H_

#include "SSystem/SComponent/c_tag.h"

enum {
    fpcLy_ROOT_e    = 0,
    fpcLy_CURRENT_e = 0xFFFFFFFD,
    fpcLy_SPECIAL_e = 0xFFFFFFFE,
    fpcLy_NONE_e    = 0xFFFFFFFF,
};

typedef struct layer_class layer_class;


typedef struct layer_management_tag_class {
    /* 0x00 */ create_tag_class create_tag;
    /* 0x14 */ layer_class* layer;
    /* 0x18 */ u16 node_list_id;
    /* 0x1A */ u16 node_list_priority;
} layer_management_tag_class;

int fpcLyTg_QueueTo(layer_management_tag_class* i_layer_tag);
int fpcLyTg_ToQueue(layer_management_tag_class* i_layer_tag, unsigned int i_layerID, u16 i_listID,
                    u16 i_listPriority);
int fpcLyTg_Move(layer_management_tag_class* i_layer_tag, unsigned int i_layerID, u16 i_listID,
                 u16 i_listPriority);
int fpcLyTg_Init(layer_management_tag_class* i_layer_tag, unsigned int i_id, void* i_data);

#endif
