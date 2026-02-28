#ifndef D_A_MG_FSHOP_H
#define D_A_MG_FSHOP_H

#include "d/d_bg_s_acch.h"
#include "f_op/f_op_actor.h"
#include "SSystem/SComponent/c_phase.h"

class J3DModel;
class J3DModelData;
class dBgW;

struct fs_weed_s {
    /* 0x00 */ cXyz field_0x00[15];
    /* 0xB4 */ f32 field_0xb4;
    /* 0xB8 */ f32 field_0xb8;
    /* 0xBC */ s16 field_0xbc;
};

struct fs_tsubo_s {
    /* 0x00 */ cXyz field_0x00;
    /* 0x0C */ csXyz field_0x0c; // unused
    /* 0x14 */ J3DModel* model;
    /* 0x18 */ u8 field_0x18[0x1C - 0x18];
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ s16 field_0x20;
    /* 0x22 */ s16 field_0x22;
    /* 0x24 */ s16 field_0x24;
};

struct fs_rod_s {
    /* 0x00 */ cXyz field_0x00;
    /* 0x0C */ csXyz field_0x0C; // unused
    /* 0x14 */ J3DModel* model;
    /* 0x18 */ J3DModel* ringModel[6];
    /* 0x30 */ mDoExt_3DlineMat0_c line_mat;
    /* 0x4C */ int field_0x4c;
    /* 0x50 */ u8 field_0x50[0x54 - 0x50];
    /* 0x54 */ f32 field_0x54;
    /* 0x58 */ s16 rot_x;
};

struct fs_lure_s {
    /* 0x00 */ cXyz field_0x00;
    /* 0x0C */ csXyz field_0x0c;
    /* 0x14 */ J3DModel* model;
    /* 0x18 */ J3DModel* hookModel[2];
    /* 0x20 */ u8 field_0x20[0x24 - 0x20];
    /* 0x24 */ int field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ s16 field_0x30;
    /* 0x32 */ s16 field_0x32;
    /* 0x34 */ s16 field_0x34;
    /* 0x36 */ s16 field_0x36;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ f32 field_0x3c;
};

struct fs_koro2_s {
    /* 0x00 */ J3DModel* model;
    /* 0x04 */ Mtx bgMtx;
    /* 0x34 */ s8 part_id;
    /* 0x38 */ cXyz pos;
    /* 0x44 */ cXyz size;
    /* 0x50 */ s16 rot_y;
    /* 0x54 */ cXyz world_pos;
    /* 0x60 */ u8 field_0x60;
    /* 0x64 */ dBgW* bgw;
};

/**
 * @ingroup actors-unsorted
 * @class fshop_class
 * @brief Hena's Shop
 *
 * @details
 *
 */
class fshop_class {
public:
    /* 0x0000 */ fopAc_ac_c actor;
    /* 0x0568 */ request_of_phase_process_class mPhase;
    /* 0x0570 */ s16 field_0x0570;
    /* 0x0572 */ s16 field_0x0572;
    /* 0x0574 */ fs_lure_s mLure[48];
    /* 0x1174 */ fs_rod_s mRod[3];
    /* 0x1288 */ fs_weed_s mWeed[60];
    /* 0x3F88 */ mDoExt_3DlineMat0_c field_0x3f88;
    /* 0x3FA4 */ fs_tsubo_s mTsubo[2];
    /* 0x3FF4 */ J3DModel* hatModel;
    /* 0x3FF8 */ s16 field_0x3ff8;
    /* 0x3FFA */ s16 field_0x3ffa;
    /* 0x3FFC */ f32 field_0x3ffc;
    /* 0x4000 */ f32 field_0x4000;
    /* 0x4004 */ J3DModel* photoModel;
    /* 0x4008 */ fshop_class* field_0x4008;
    /* 0x400C */ u8 field_0x400c;
    /* 0x400D */ u8 field_0x400d;
    /* 0x400E */ u8 field_0x400e;
    /* 0x4010 */ s16 field_0x4010;
    /* 0x4014 */ cXyz field_0x4014;
    /* 0x4020 */ csXyz field_0x4020;
    /* 0x4028 */ J3DModel* mpA_crwaku_model;
    /* 0x402C */ dBgW* koro2WakuBgw;
    /* 0x4030 */ Mtx field_0x4030;
    /* 0x4060 */ s16 field_0x4060;
    /* 0x4062 */ s16 field_0x4062;
    /* 0x4064 */ cXyz field_0x4064;
    /* 0x4070 */ J3DModel* ballModel;
    /* 0x4074 */ dBgS_AcchCir ballAcchcir;
    /* 0x40B4 */ dBgS_ObjAcch ballAcch;
    /* 0x428C */ u8 field_0x428c;
    /* 0x428D */ u8 field_0x428d;
#if PLATFORM_SHIELD
    u8 unk_dbg_padding[0x6];
    J3DModel* dbg_models[2];
#endif
    /* 0x4290 */ fs_koro2_s mKoro2[100];
    /* 0x6B30 */ J3DModel* canoeModel;
    /* 0x6B34 */ s16 field_0x6b34;
    /* 0x6B38 */ Mtx field_0x6b38;
    /* 0x6B68 */ dBgW* tableBgw;
    /* 0x6B6C */ u32 field_0x6b6c[2];
    /* 0x6B74 */ u8 field_0x6B74[0x6B7C - 0x6B74];
    /* 0x6B7C */ u8 field_0x6b7c;
};

#endif /* D_A_MG_FSHOP_H */
