#ifndef D_A_MG_ROD_H
#define D_A_MG_ROD_H

#include "Z2AudioLib/Z2Creature.h"
#include "d/actor/d_a_mg_fish.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_msg_flow.h"
#include "f_op/f_op_actor_mng.h"

#define MG_ROD_LURE_LINE_LEN 106
#define MG_ROD_UKI_LINE_LEN  100

enum mg_catch_type {
    MG_CATCH_FISH,
    MG_CATCH_BT,   // Boot
    MG_CATCH_SP,   // Spoon
    MG_CATCH_LH,   // Rupee Bag
    MG_CATCH_SG,   // Skullfish
    MG_CATCH_BB,   // Bombfish
    MG_CATCH_BIN,  // Empty Bottle
    MG_CATCH_KN,   // Can
    MG_CATCH_ED,   // Branch
    MG_CATCH_SY,   // Wheel
};

enum mg_lure_type {
    MG_LURE_FR,  // Frog
    MG_LURE_PE,  // Swimmer
    MG_LURE_PO,  // Popper
    MG_LURE_WS,  // Spinner
    MG_LURE_SP,  // Spoon (Sinking)
};

enum mg_rod_kind {
    MG_ROD_KIND_LURE,  // Henna Fishing Rod
    MG_ROD_KIND_UKI,  // Player Fishing Rod
};

enum mg_rod_uki_action {
    ACTION_UKI_READY,
    ACTION_UKI_STANDBY,

    ACTION_UKI_HIT = 5,
    ACTION_UKI_CATCH,
};

enum mg_rod_lure_action {
    ACTION_LURE_ONBOAT,
    ACTION_LURE_STANDBY,

    ACTION_LURE_CAST = 3,
    ACTION_LURE_ACTION,
    ACTION_LURE_HIT,
    ACTION_LURE_CATCH,
    ACTION_LURE_BARE,
    ACTION_LURE_HEART,
};

struct mg_rod_s {
    /* 0x0 */ cXyz field_0x0[16];
};

struct mg_line_s {
    /* 0x0 */ cXyz pos[100];
};

struct mg_hook_s {
    /* 0x00 */ cXyz field_0x0[2];
    /* 0x18 */ csXyz field_0x18;
};

struct wd_ss;

/**
 * @ingroup actors-items
 * @class dmg_rod_class
 * @brief Fishing Rod
 *
 * @details
 *
*/

class dmg_rod_class {
public:
    f32 getRodStickX() { return rod_stick_x; }
    f32 getRodStickY() { return rod_stick_y; }
    bool checkRodEquipPermission() { return field_0x1514 == 0; }
    bool checkReelSpin() { return reel_btn_flags != 0; }

    /* 0x0000 */ fopAc_ac_c actor;
    /* 0x0568 */ request_of_phase_process_class phase;
    /* 0x0570 */ char* arcname;
    /* 0x0574 */ u8 arg0;
    /* 0x0575 */ u8 arg1;
    /* 0x0576 */ u8 arg2;
    /* 0x0578 */ s16 counter;
    /* 0x057A */ s16 timers[10];
    /* 0x0590 */ f32 water_surface_y;
    /* 0x0594 */ f32 field_0x594;
    /* 0x0598 */ f32 field_0x598;
    /* 0x059C */ f32 field_0x59c;
    /* 0x05A0 */ u8 field_0x5A0[0x05A4 - 0x05A0];
    /* 0x05A4 */ mg_rod_s mg_rod;  // below to mg_line_s part of mg_rod_s?
    /* 0x0664 */ J3DModel* rod_uki_model[15];
    /* 0x06A0 */ f32 field_0x6a0;
    /* 0x06A4 */ f32 field_0x6a4;
    /* 0x06A8 */ u8 field_0x6a8;
    /* 0x06AC */ cXyz rod_tip_pos;
    /* 0x06B8 */ cXyz field_0x6b8;
    /* 0x06C4 */ s16 rod_angle_y;
    /* 0x06C8 */ cXyz field_0x6c8;
    /* 0x06D4 */ cXyz field_0x6d4;
    /* 0x06E0 */ f32 cast_momentum;
    /* 0x06E4 */ f32 field_0x6e4;
    /* 0x06E8 */ s8 field_0x6e8;
    /* 0x06EC */ cXyz field_0x6ec;
    /* 0x06F8 */ f32 field_0x6f8;
    /* 0x06FC */ f32 cast_power;
    /* 0x0700 */ mDoExt_McaMorf* rod_modelMorf;
    /* 0x0704 */ J3DModel* unk_ring_model[6];
    /* 0x071C */ csXyz field_0x71c[6];
    /* 0x0740 */ u8 field_0x740[0x0744 - 0x0740];
    /* 0x0744 */ cXyz field_0x744;
    /* 0x0750 */ cXyz field_0x750;
    /* 0x075C */ s16 field_0x75c;
    /* 0x075E */ s16 field_0x75e;
    /* 0x0760 */ f32 field_0x760;
    /* 0x0764 */ cXyz field_0x764;
    /* 0x0770 */ mg_line_s mg_line;  // below to mg_hook_s part of mg_line_s?
    /* 0x0C20 */ f32 field_0xc20[98];
    /* 0x0DA8 */ u8 field_0xDA8[0x0DB0 - 0x0DA8];
    /* 0x0DB0 */ f32 field_0xdb0[98];
    /* 0x0F38 */ u8 field_0xF38[0x0F40 - 0x0F38];
    /* 0x0F40 */ mDoExt_3DlineMat0_c linemat;
    /* 0x0F5C */ f32 field_0xf5c;
    /* 0x0F60 */ f32 field_0xf60;
    /* 0x0F64 */ f32 field_0xf64;
    /* 0x0F68 */ f32 field_0xf68;
    /* 0x0F6C */ u8 field_0xF6C[0x0F6E - 0x0F6C];
    /* 0x0F6E */ s8 field_0xf6e;
    /* 0x0F6F */ u8 field_0xF6F[0x0F74 - 0x0F6F];
    /* 0x0F74 */ f32 field_0xf74;
    /* 0x0F78 */ f32 field_0xf78;
    /* 0x0F7C */ s16 kind;
    /* 0x0F7E */ s16 action;
    /* 0x0F80 */ s8 lure_type;
    /* 0x0F81 */ s8 field_0xf81;
    /* 0x0F84 */ J3DModel* lure_model[5];
    /* 0x0F98 */ J3DModel* hook_model[2];
    /* 0x0FA0 */ J3DModel* ring_model;
    /* 0x0FA4 */ u8 field_0xFA4[0x0FAC - 0x0FA4];
    /* 0x0FAC */ mg_hook_s mg_hook[2];
    /* 0x0FEC */ f32 field_0xfec;
    /* 0x0FF0 */ s16 field_0xff0;
    /* 0x0FF2 */ s16 field_0xff2;
    /* 0x0FF4 */ s16 field_0xff4;
    /* 0x0FF8 */ f32 field_0xff8;
    /* 0x0FFC */ s16 lure_yaw_offset;
    /* 0x0FFE */ s16 lure_yaw_target;
    /* 0x1000 */ s16 lure_pitch_offset;
    /* 0x1002 */ s16 lure_pitch_target;
    /* 0x1004 */ s16 field_0x1004;
    /* 0x1006 */ s16 field_0x1006;
    /* 0x1008 */ s8 field_0x1008;
    /* 0x1009 */ s8 field_0x1009;
    /* 0x100A */ s8 field_0x100a;
    /* 0x100B */ u8 field_0x100b;
    /* 0x100C */ u8 field_0x100c;
    /* 0x100D */ s8 is_hook_in_water;
    /* 0x100E */ s8 field_0x100e;
    /* 0x100F */ s8 field_0x100f;
    /* 0x1010 */ cXyz field_0x1010;
    /* 0x101C */ s16 field_0x101c[1];
    /* 0x101E */ s16 field_0x101e;
    /* 0x1020 */ s16 field_0x1020;
    /* 0x1022 */ s16 field_0x1022;
    /* 0x1024 */ J3DModel* uki_model;
    /* 0x1028 */ J3DModel* uki_saki_model;
    /* 0x102C */ s8 esa_kind;
    /* 0x102D */ s8 hook_kind;
    /* 0x102E */ s8 field_0x102e;
    /* 0x102F */ u8 field_0x102f;
    /* 0x1030 */ J3DModel* esa_model[2];
    /* 0x1038 */ u8 field_0x1038[0x103C - 0x1038];
    /* 0x103C */ s16 field_0x103c[8];
    /* 0x104C */ u8 field_0x104C[0x104E - 0x104C];
    /* 0x104E */ s16 field_0x104e[8];
    /* 0x1060 */ cXyz hook_pos;
    /* 0x106C */ cXyz field_0x106c;
    /* 0x1078 */ cXyz field_0x1078;
    /* 0x1084 */ csXyz field_0x1084;
    /* 0x108A */ s16 field_0x108a;
    /* 0x108C */ s16 field_0x108c;
    /* 0x108E */ s16 field_0x108e;
    /* 0x1090 */ f32 field_0x1090;
    /* 0x1094 */ f32 field_0x1094;
    /* 0x1098 */ u8 field_0x1098[0x109C - 0x1098];
    /* 0x109C */ s16 field_0x109c;
    /* 0x109E */ s16 field_0x109e;
    /* 0x10A0 */ s16 field_0x10a0;
    /* 0x10A2 */ s16 field_0x10a2;
    /* 0x10A4 */ s8 field_0x10a4;
    /* 0x10A5 */ s8 field_0x10a5;
    /* 0x10A6 */ s8 field_0x10a6;
    /* 0x10A7 */ s8 field_0x10a7;
    /* 0x10A8 */ s8 field_0x10a8;
    /* 0x10A9 */ s8 field_0x10a9;
    /* 0x10AA */ u8 vibmode;
    /* 0x10AB */ u8 vib_timer;
    /* 0x10AC */ fpc_ProcID mg_fish_id;
    /* 0x10B0 */ s16 field_0x10b0;
    /* 0x10B4 */ Z2Creature sound;
    /* 0x1144 */ fpc_ProcID boat_actor_id;
    /* 0x1148 */ s8 field_0x1148;
    /* 0x114A */ s16 field_0x114a;
    /* 0x114C */ s16 field_0x114c;
    /* 0x114E */ s16 field_0x114e;
    /* 0x1150 */ s16 field_0x1150;
    /* 0x1154 */ f32 field_0x1154;
    /* 0x1158 */ cXyz field_0x1158;
    /* 0x1164 */ u8 field_0x1164[0x1165 - 0x1164];
    /* 0x1165 */ s8 cast_bob_timer;
    /* 0x1168 */ u32 sibuki_eff[4];
    /* 0x1178 */ u32 emitter_id;
    /* 0x117C */ u32 field_0x117c;
    /* 0x1180 */ u32 field_0x1180[2];
    /* 0x1188 */ f32 field_0x1188;
    /* 0x118C */ f32 field_0x118c;
    /* 0x1190 */ dBgS_AcchCir acchcir;
    /* 0x11D0 */ dBgS_ObjAcch acch;
    /* 0x13A8 */ u8 field_0x13a8;
    /* 0x13AC */ f32 field_0x13ac;
    /* 0x13B0 */ f32 field_0x13b0;
    /* 0x13B4 */ s16 play_cam_mode;
    /* 0x13B6 */ s16 play_cam_timer;
    /* 0x13B8 */ cXyz play_cam_eye;
    /* 0x13C4 */ cXyz play_cam_center;
    /* 0x13D0 */ cXyz play_cam_eye_target;
    /* 0x13DC */ cXyz play_cam_center_target;
    /* 0x13E8 */ cXyz play_cam_eye_speed;
    /* 0x13F4 */ cXyz play_cam_center_speed;
    /* 0x1400 */ s16 field_0x1400;
    /* 0x1402 */ s16 field_0x1402;
    /* 0x1404 */ s16 field_0x1404;
    /* 0x1406 */ u8 field_0x1406;
    /* 0x1407 */ u8 field_0x1407;
    /* 0x1408 */ f32 play_cam_fovy;
    /* 0x140C */ f32 field_0x140c;
    /* 0x1410 */ f32 camera_morf_rate;
    /* 0x1414 */ f32 field_0x1414;
    /* 0x1418 */ s16 field_0x1418;
    /* 0x141A */ s16 field_0x141a;
    /* 0x141C */ f32 field_0x141c;
    /* 0x1420 */ f32 field_0x1420;
    /* 0x1424 */ f32 field_0x1424;
    /* 0x1428 */ f32 field_0x1428;
    /* 0x142C */ f32 field_0x142c;
    /* 0x1430 */ u8 field_0x1430[0x1434 - 0x1430];
    /* 0x1434 */ cXyz field_0x1434;
    /* 0x1440 */ cXyz field_0x1440;
    /* 0x144C */ cXyz field_0x144c;
    /* 0x1458 */ cXyz field_0x1458;
    /* 0x1464 */ f32 field_0x1464;
    /* 0x1468 */ u8 field_0x1468[0x146C - 0x1468];
    /* 0x146C */ u8 input_cooldown;
    /* 0x146D */ s8 msg_flow_state;
    /* 0x146E */ u8 field_0x146E[0x1470 - 0x146E];
    /* 0x1470 */ u8 field_0x1470;
    /* 0x1472 */ s16 field_0x1472;
    /* 0x1474 */ dMsgFlow_c msgflow;
    /* 0x14C0 */ u8 field_0x14c0;
    /* 0x14C2 */ u16 field_0x14c2;
    /* 0x14C4 */ u16 catch_flow_id;
    /* 0x14C6 */ u8 field_0x14C6[0x14C8 - 0x14C6];
    /* 0x14C8 */ f32 rod_stick_x;
    /* 0x14CC */ f32 rod_stick_y;
    /* 0x14D0 */ f32 field_0x14d0;
    /* 0x14D4 */ f32 field_0x14d4;
    /* 0x14D8 */ f32 rod_substick_x;
    /* 0x14DC */ f32 rod_substick_y;
    /* 0x14E0 */ f32 prev_rod_substick_y;
    /* 0x14E4 */ u8 field_0x14e4;
    /* 0x14E5 */ u8 field_0x14E5[0x14EC - 0x14E5];
    /* 0x14EC */ f32 field_0x14ec;
    /* 0x14F0 */ int reel_btn_flags;
    /* 0x14F4 */ f32 reel_speed;
    /* 0x14F8 */ s16 field_0x14f8;
    /* 0x14FA */ s16 field_0x14fa;
    /* 0x14FC */ s16 field_0x14fc;
    /* 0x14FE */ u8 field_0x14FE[0x1504 - 0x14FE];
    /* 0x1504 */ f32 field_0x1504;
    /* 0x1508 */ f32 field_0x1508;
    /* 0x150C */ f32 field_0x150c;
    /* 0x1510 */ u8 field_0x1510[0x1514 - 0x1510];
    /* 0x1514 */ u8 field_0x1514;
    /* 0x1515 */ u8 field_0x1515;
    /* 0x1518 */ dCcD_Stts ccStts;
    /* 0x1554 */ dCcD_Sph ccSph;
    /* 0x168C */ u8 field_0x168c;
    /* 0x168D */ u8 field_0x168d;
    /* 0x168E */ u8 HIOInit;
};

STATIC_ASSERT(sizeof(dmg_rod_class) == 0x1690);

#endif /* D_A_MG_ROD_H */
