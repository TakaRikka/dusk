#ifndef F_F_OP_VIEW_H_
#define F_F_OP_VIEW_H_

#include "SSystem/SComponent/c_xyz.h"
#include <dolphin/mtx.h>
#include "f_pc/f_pc_leaf.h"

struct view_process_profile_definition {
    /* 0x00 */ leaf_process_profile_definition base;
    /* 0x24 */ leafdraw_method_class* sub_method; // Subclass methods
    /* 0x28 */ u8 unk_0x28;
    /* 0x2C */ u32 unk_0x2C;
    /* 0x30 */ u32 unk_0x30;
    /* 0x34 */ u32 unk_0x34;
    /* 0x38 */ u32 unk_0x38;
};

class lookat_class {
public:
    /* 0x00 */ cXyz eye;
    /* 0x0C */ cXyz center;
    /* 0x18 */ cXyz up;
};

struct scissor_class {
    /* 0x0 */ f32 x_orig;
    /* 0x4 */ f32 y_orig;
    /* 0x8 */ f32 width;
    /* 0xC */ f32 height;
};

struct view_port_class {
    /* 0x00 */ f32 x_orig;
    /* 0x04 */ f32 y_orig;
    /* 0x08 */ f32 width;
    /* 0x0C */ f32 height;
    /* 0x10 */ f32 near_z;
    /* 0x14 */ f32 far_z;
    /* 0x18 */ scissor_class scissor;
};

struct view_class {
    /* 0x000 */ leafdraw_class base;
    /* 0x0C0 */ leafdraw_method_class* sub_method;
    /* 0x0C4 */ u8 field_0xc4;
    /* 0x0C8 */ f32 near;
    /* 0x0CC */ f32 far;
    /* 0x0D0 */ f32 fovy;
    /* 0x0D4 */ f32 aspect;
    /* 0x0D8 */ lookat_class lookat;
    /* 0x0FC */ s16 bank;
    /* 0x100 */ Mtx44 projMtx;
    /* 0x140 */ Mtx viewMtx;
    /* 0x170 */ Mtx invViewMtx;
    /* 0x1A0 */ Mtx44 projViewMtx;
    /* 0x1E0 */ Mtx viewMtxNoTrans;
};

extern leafdraw_method_class g_fopVw_Method;

#endif
