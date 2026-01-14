
#ifndef F_PC_METHOD_TAG_H_
#define F_PC_METHOD_TAG_H_

#include "SSystem/SComponent/c_tag.h"

typedef int (*process_method_tag_func)(void*);

typedef struct process_method_tag_class {
    /* 0x00 */ create_tag_class create_tag;
    /* 0x14 */ process_method_tag_func method;
    /* 0x18 */ void* data;
} process_method_tag_class;

int fpcMtdTg_Do(process_method_tag_class* i_methodTag);
int fpcMtdTg_ToMethodQ(node_list_class* i_nodelist, process_method_tag_class* i_methodTag);
void fpcMtdTg_MethodQTo(process_method_tag_class* i_methodTag);
int fpcMtdTg_Init(process_method_tag_class* i_methodTag, process_method_tag_func i_method, void* i_data);

#endif
