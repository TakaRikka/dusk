#ifndef F_F_OP_OVERLAP_H_
#define F_F_OP_OVERLAP_H_

#include "f_pc/f_pc_leaf.h"
#include "SSystem/SComponent/c_request.h"

struct overlap_task_class {
    /* 0x00 */ leafdraw_class base;
    /* 0xC0 */ leafdraw_method_class* submethod;
    /* 0xC4 */ request_base_class request;
    /* 0xC8 */ fpc_ProcID scene_id;
};  // Size: 0xCC

struct overlap_process_profile_definition {
    /* 0x00 */ leaf_process_profile_definition base;
    /* 0x24 */ leafdraw_method_class* sub_method;
};  // Size: 0x28

static s32 fopOvlp_Draw(void* param_1);

DUSK_GAME_EXTERN leafdraw_method_class g_fopOvlp_Method;

#endif
