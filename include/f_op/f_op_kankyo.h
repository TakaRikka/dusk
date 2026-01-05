#ifndef F_OP_F_OP_KANKYO_H
#define F_OP_F_OP_KANKYO_H

#include "SSystem/SComponent/c_xyz.h"
#include "f_pc/f_pc_leaf.h"

struct kankyo_class {
    /* 0x00 */ leafdraw_class base;
    /* 0xC0 */ int type;
    /* 0xC4 */ create_tag_class draw_tag;
    /* 0xD8 */ leafdraw_method_class* sub_method;
    /* 0xDC */ cXyz pos;
    /* 0xE8 */ cXyz scale;
    /* 0xF4 */ u32 parameters;
};

struct kankyo_process_profile_definition {
    /* 0x00 */ leaf_process_profile_definition base;
    /* 0x24 */ leafdraw_method_class* sub_method;
};  // Size: 0x28

void fopKy_IsKankyo(void* i_this);

extern leafdraw_method_class g_fopKy_Method;

#endif /* F_OP_F_OP_KANKYO_H */
