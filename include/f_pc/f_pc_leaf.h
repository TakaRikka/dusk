
#ifndef F_PC_LEAF_H_
#define F_PC_LEAF_H_

#include "f_pc/f_pc_base.h"
#include "f_pc/f_pc_draw_priority.h"
#include "f_pc/f_pc_method.h"
#include "f_pc/f_pc_profile.h"
#include "d/d_procname.h"

typedef struct leafdraw_method_class {
    /* 0x00 */ process_method_class base;
    /* 0x10 */ process_method_func draw_method;
} leafdraw_method_class;

typedef struct leafdraw_class {
    /* 0x00 */ base_process_class base;
    /* 0xB8 */ leafdraw_method_class* leaf_methods;
    /* 0xBC */ s8 unk_0xBC;
    /* 0xBD */ u8 unk_0xBD;
    /* 0xBE */ draw_priority_class draw_priority;
} leafdraw_class;

typedef struct leaf_process_profile_definition {
    /* 0x00 */ process_profile_definition base;
    /* 0x1C */ leafdraw_method_class* sub_method; // Subclass methods
    /* 0x20 */ s16 priority; // mDrawPriority
} leaf_process_profile_definition;

s16 fpcLf_GetPriority(const leafdraw_class* i_leaf);
int fpcLf_DrawMethod(leafdraw_method_class* i_method, void* i_process);
int fpcLf_Draw(leafdraw_class* i_method);
int fpcLf_Execute(leafdraw_class* i_leaf);
int fpcLf_IsDelete(leafdraw_class* i_leaf);
int fpcLf_Delete(leafdraw_class* i_leaf);
int fpcLf_Create(leafdraw_class* i_leaf);

extern int g_fpcLf_type;
extern leafdraw_method_class g_fpcLf_Method;

#endif
