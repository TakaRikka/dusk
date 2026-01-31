
#ifndef F_PC_LINE_TAG_H_
#define F_PC_LINE_TAG_H_

#include "SSystem/SComponent/c_tag.h"

typedef struct line_tag {
    /* 0x00 */ create_tag_class base;
    /* 0x14 */ int list_id;
} line_tag;

int fpcLnTg_Move(line_tag* i_lineTag, int i_newListID);
void fpcLnTg_QueueTo(line_tag* i_lineTag);
int fpcLnTg_ToQueue(line_tag* i_lineTag, int lineListID);
void fpcLnTg_Init(line_tag* i_lineTag, void* i_data);

#endif
