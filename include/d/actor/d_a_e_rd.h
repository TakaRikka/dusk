#ifndef D_A_E_RD_H
#define D_A_E_RD_H
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_jnt_col.h"
#include "d/d_msg_flow.h"
#include "d/d_path.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
/**
 * @ingroup actors-enemies
 * @class e_rd_class
 * @brief Rider (Bulblin / King Bulblin on Boar)
 *
 * @details Bulblin enemy that rides a Bullbo (wild boar). Handles mounting,
 * riding, combat, and dismounting behavior, as well as on-foot actions when
 * not riding. Bulblins appear as regular riders, 
 * while King Bulblin serves as the boss variant of the boar rider.
 *
 */
class e_rd_class {
public:
    e_rd_class();

    /* 0x0000 */ fopEn_enemy_c enemy;
    /* 0x05AC */ request_of_phase_process_class phase;
    /* 0x05B4 */ s16 mode;
    /* 0x05B6 */ u8 arg0;
    /* 0x05B7 */ u8 arg1;
    /* 0x05B8 */ u8 arg2;
    /* 0x05B9 */ u8 field_0x5b9;
    /* 0x05BA */ u8 scene_no;
    /* 0x05BB */ u8 field_0x5bb;
    /* 0x05BC */ u8 weapon_type;
    /* 0x05BD */ s8 armament_flag;
    /* 0x05C0 */ cXyz find_pos;
    /* 0x05CC */ s16 target_ya;
    /* 0x05D0 */ mDoExt_McaMorfSO* anm_p;
    /* 0x05D4 */ Z2CreatureEnemy sound;
    /* 0x0678 */ char* resName;
    /* 0x067C */ int anm;
    /* 0x0680 */ int field_0x680;
    /* 0x0684 */ dJntCol_c mJntCol;
    /* 0x0694 */ J3DModel* arrow_model;
    /* 0x0698 */ mDoExt_McaMorfSO* bow_anm;
    /* 0x069C */ mDoExt_McaMorf* horn_anm;
    /* 0x06A0 */ s8 horn_mode;
    /* 0x06A4 */ cXyz horn_pos;
    /* 0x06B0 */ cXyz horn_spd;
    /* 0x06BC */ csXyz horn_rot;
    /* 0x06C2 */ s8 run_flag;
    /* 0x06C3 */ u8 field_0x6c3;
    /* 0x06C4 */ J3DModel* eye_model[2];
    /* 0x06CC */ f32 field_0x6cc;
    /* 0x06D0 */ f32 field_0x6d0;
    /* 0x06D4 */ J3DModel* armor_boss_part[14];
    /* 0x070C */ s8 part_break[14];
    /* 0x071C */ cXyz part_pos[14];
    /* 0x07C4 */ cXyz part_spd[14];
    /* 0x086C */ csXyz part_angle[14];
    /* 0x08C0 */ u32 armor_break_eff[14][3];
    /* 0x0968 */ s8 field_0x968;
    /* 0x096C */ f32 field_0x96c;
    /* 0x0970 */ s16 counter;
    /* 0x0972 */ s16 action;
    /* 0x0974 */ s16 old_action;
    /* 0x0978 */ f32 dis;
    /* 0x097C */ s16 angleY;
    /* 0x0980 */ f32 attack_range;
    /* 0x0984 */ f32 waterY;
    /* 0x0988 */ u32 shadow_key;
    /* 0x098C */ fpc_ProcID boar_id;
    /* 0x0990 */ s16 timer[4];
    /* 0x0998 */ s16 damage_timer;
    /* 0x099A */ s16 field_0x99a;
    /* 0x099C */ s16 yagura_timer;
    /* 0x099E */ s16 attack_timer;
    /* 0x09A0 */ s8 field_0x9a0;
    /* 0x09A1 */ s8 look_timer;
    /* 0x09A2 */ s8 arrow_draw;
    /* 0x09A3 */ s8 arrow_flag;
    /* 0x09A4 */ s8 field_0x9a4;
    /* 0x09A5 */ u8 field_0x9a5;
    /* 0x09A6 */ s16 bow_shake_timer;
    /* 0x09A8 */ s16 field_0x9a8;
    /* 0x09AA */ u8 field_0x9aa;
    /* 0x09AB */ s8 field_0x9ab;
    /* 0x09AC */ s8 field_0x9ac;
    /* 0x09AD */ s8 field_0x9ad;
    /* 0x09B0 */ cXyz center_body;
    /* 0x09BC */ s8 ride_mode;
    /* 0x09BD */ s8 field_0x9bd;
    /* 0x09BE */ s8 boar_stand;
    /* 0x09BF */ u8 field_0x9bf;
    /* 0x09C0 */ f32 mount_jump_y;
    /* 0x09C4 */ f32 mount_jump_speed;
    /* 0x09C8 */ s8 aim_type;
    /* 0x09C9 */ u8 field_0x9c9;
    /* 0x09CA */ s16 aim_angle_y;
    /* 0x09CC */ s16 aim_angle_x;
    /* 0x09CE */ s16 head_angle_y;
    /* 0x09D0 */ s16 head_shake;
    /* 0x09D4 */ cXyz eye;
    /* 0x09E0 */ cXyz field_0x9e0; // unused, dbg offset 0xa18
    /* 0x09EC */ f32 jump_z;
    /* 0x09F0 */ f32 field_0x9f0;
    /* 0x09F4 */ s16 field_0x9f4;
    /* 0x09F6 */ s16 field_0x9f6;
    /* 0x09F8 */ s8 damage_flag;
    /* 0x09FC */ cXyz field_0x9fc; // unused, dbg offset 0xa34
    /* 0x0A08 */ u8 field_0xa08[0x0A0C - 0x0A08];
    /* 0x0A0C */ csXyz jump_angle;
    /* 0x0A12 */ csXyz field_0xa12;
    /* 0x0A18 */ csXyz field_0xa18;
    /* 0x0A1E */ s8 jump_timer;
    /* 0x0A1F */ s8 field_0xa1f;
    /* 0x0A20 */ u32 field_0xa20;
    /* 0x0A24 */ f32 field_0xa24;
    /* 0x0A28 */ s16 field_0xa28;
    /* 0x0A2C */ f32 field_0xa2c;
    /* 0x0A30 */ s16 field_0xa30;
    /* 0x0A32 */ csXyz field_0xa32[11];
    /* 0x0A74 */ csXyz field_0xa74[11];
    /* 0x0AB8 */ f32 field_0xab8;
    /* 0x0ABC */ s16 field_0xabc[4];
    /* 0x0AC4 */ s16 field_0xac4;
    /* 0x0AC6 */ s16 field_0xac6;
    /* 0x0AC8 */ s16 field_0xac8;
    /* 0x0ACA */ s16 field_0xaca;
    /* 0x0ACC */ s16 field_0xacc[3];
    /* 0x0AD2 */ s16 field_0xad2[4];
    /* 0x0ADA */ s16 field_0xada;
    /* 0x0ADC */ s16 field_0xadc;
    /* 0x0ADE */ csXyz field_0xade; // dbg offset 0xb1c
    /* 0x0AE4 */ csXyz field_0xae4;
    /* 0x0AEC */ f32 field_0xaec;
    /* 0x0AF0 */ s16 field_0xaf0;
    /* 0x0AF2 */ s16 field_0xaf2;
    /* 0x0AF4 */ u8 field_0xaf4[0xaf8 - 0xaf4];
    /* 0x0AF8 */ s16 field_0xaf8;
    /* 0x0AFA */ s8 field_0xafa;
    /* 0x0AFB */ s8 field_0xafb;
    /* 0x0AFC */ dPath* ppd;
    /* 0x0B00 */ s8 jyunkai_no; /* 巡回ポイントNo (Patrol point index) */
    /* 0x0B01 */ s8 dir;
    /* 0x0B04 */ dBgS_AcchCir AcchCir;
    /* 0x0B44 */ dBgS_ObjAcch Bgc;
    /* 0x0D1C */ dCcD_Stts Stts;
    /* 0x0D58 */ dCcD_Sph cc_sph[3];
    /* 0x1100 */ dCcD_Sph at_sph;
    /* 0x1238 */ dCcU_AtInfo at_info;
    /* 0x125C */ u8 hio_set;
    /* 0x125D */ s8 field_0x125d;
    /* 0x1260 */ u32 field_0x1260;
    /* 0x1264 */ u32 field_0x1264;
    /* 0x1268 */ u32 w_eff_id[3];
    /* 0x1274 */ u32 fire_eff_id[2];
    /* 0x127C */ cXyz fire_pos;
    /* 0x1288 */ cXyz fire_eff;
    /* 0x1294 */ s8 field_0x1294;
    /* 0x1295 */ s8 one_hit_kill;
    /* 0x1296 */ s8 field_0x1296;
    /* 0x1297 */ s8 field_0x1297;
    /* 0x1298 */ s8 field_0x1298;
    /* 0x1299 */ u8 field_0x1299;
    /* 0x129A */ s8 actor_set;
    /* 0x129B */ u8 field_0x129b;
    /* 0x129C */ s8 horn_timer;
    /* 0x12A0 */ int sw;
    /* 0x12A4 */ s16 demo_mode;
    /* 0x12A6 */ s16 demo_timer;
    /* 0x12A8 */ cXyz demo_cam_eye;
    /* 0x12B4 */ cXyz demo_cam_ctr;
    /* 0x12C0 */ cXyz demo_cam_way;
    /* 0x12CC */ cXyz demo_cam_target;
    /* 0x12D8 */ cXyz demo_cam_eye_spd;
    /* 0x12E4 */ cXyz demo_cam_way_spd;
    /* 0x12F0 */ cXyz field_0x12f0;
    /* 0x12FC */ u8 field_0x12fc[0x1300 - 0x12fc];
    /* 0x1300 */ f32 demo_cam_eye_z;
    /* 0x1304 */ u8 blurRate;
    /* 0x1308 */ f32 demo_cam_zoom;
    /* 0x130C */ f32 demo_cam_morf;
    /* 0x1310 */ u8 field_0x1310[0x131c - 0x1310];
    /* 0x131C */ dMsgFlow_c MsgFlow;
};

STATIC_ASSERT(sizeof(e_rd_class) == 0x1368);

#endif /* D_A_E_RD_H */
