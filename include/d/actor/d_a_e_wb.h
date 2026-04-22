#ifndef D_A_E_WB_H
#define D_A_E_WB_H

#include "f_op/f_op_actor_mng.h"
#include "d/actor/d_a_horse.h"
#include "SSystem/SComponent/c_phase.h"
#include "d/d_cc_uty.h"

// copypasta, fix when actual names known
enum daE_WB_ANM {
    ANM_APPEAR = 7,
    ANM_ATTACK,
    ANM_ATTACK_A,
    ANM_ATTACK_B,
    ANM_ATTACK_C,
    ANM_BOMB_DAMAGE,
    ANM_CORE_DAMAGE,
    ANM_DAMAGE_WAIT,
    ANM_DEAD,
    ANM_LOOK_M,
    ANM_NO_DAMAGE,
    ANM_RETURN_01,
    ANM_RETURN_02,
    ANM_RUNAWAY,
    ANM_TEST_MOTION,
    ANM_TODOME,
    ANM_WAIT_01,
};

enum daE_WB_ACT {
    /* 0x00 */ ACT_WAIT,
    /* 0x01 */ ACT_WAIT2,
    /* 0x04 */ ACT_RIDE = 0x04,
    /* 0x05 */ ACT_F_WAIT,
    /* 0x06 */ ACT_C_F_RUN,
    /* 0x07 */ ACT_A_RUN,
    /* 0x08 */ ACT_B_WAIT,
    /* 0x09 */ ACT_B_WAIT2,
    /* 0x0A */ ACT_B_RUN,
    /* 0x0B */ ACT_B_RUN2,
    /* 0x0F */ ACT_B_IKKI = 0x0F,
    /* 0x10 */ ACT_B_IKKI_END,
    /* 0x11 */ ACT_B_IKKI2,
    /* 0x12 */ ACT_B_IKKI2_END,
    /* 0x13 */ ACT_B_LV9_END,
    /* 0x15 */ ACT_S_DAMAGE = 0x15,
    /* 0x16 */ ACT_DAMAGE,
    /* 0x17 */ ACT_BG_DAMAGE,
    /* 0x18 */ ACT_LR_DAMAGE,
    /* 0x1E */ ACT_KIBA_START = 0x1E,
    /* 0x1F */ ACT_KIBA_END,
    /* 0x32 */ ACT_EVENT = 0x32,
    /* 0x65 */ ACT_PL_RIDE = 0x65,
    /* 0x66 */ ACT_PL_RIDE2,
    /* 0x67 */ ACT_PL_RIDE_NOW,
    /* 0x6E */ ACT_CRV_WAIT = 0x6E,
};

struct himo_s {
    /* 0x0 */ cXyz node[16];
};


/**
 * @ingroup actors-enemies
 * @class e_wb_class
 * @brief Bullbo (Wild Boar)
 *
 * @details
 *
 */
class e_wb_class {
public:
    BOOL checkWait();
    void setPlayerRideNow();
    void setPlayerRide();
    void getOff();
    BOOL checkDownDamage();
    BOOL checkNormalRideMode() const;
    void setRunRideMode();

    MtxP getRideMtx() { return anm_p->getModel()->getAnmMtx(15); }
    f32 nowAnimeFrame() const { return anm_p->getFrame(); }
    s16 getWaitRollAngle() const { return wait_roll_angle; }
    BOOL checkGetOff() const { return enemy.speedF < 3.0f; }
    f32 rideSpeedRate() const { return enemy.speedF / ride_speed_max; }
    f32 getAnimeFrameRate() { return anm_p->getFrame() / anm_p->getEndFrame(); }
    BOOL checkAnmLoopFrame() { return anm_p->checkFrame(0.0f); }

    J3DModel* getModel() {
        if (anm_p != NULL) {
            return anm_p->getModel();
        }

        return NULL;
    }

public:
    /* 0x0000 */ fopEn_enemy_c enemy;                   ///< @brief Base enemy class instance.
    /* 0x05AC */ request_of_phase_process_class phase;  ///< @brief Phase process request handler.
    /* 0x05B4 */ s16 ride_mode;  ///< @brief Current sub-action mode within the run action.
    /* 0x05B8 */ char* resName;   ///< @brief Resource name for loading models/animations.
    /* 0x05BC */ u8 arg0;        ///< @brief Creation parameter 1.
    /* 0x05BD */ u8 arg1;       ///< @brief Creation parameter 2.
    /* 0x05BE */ u8 field_0x5be;
    /* 0x05BF */ bool no_draw;
    /* 0x05C0 */ u8 field_0x5c0;
    /* 0x05C4 */ cXyz eye;
    /* 0x05D0 */ cXyz field_0x5d0;
    /* 0x05DC */ s16 target_ya;  ///< @brief Target facing angle for turning towards path point.
    /* 0x05DE */ s16 field_0x5de;
    /* 0x05E0 */ mDoExt_McaMorfSO* anm_p;  ///< @brief Pointer to model morph animator.
    /* 0x05E4 */ f32 anm_spd;                 ///< @brief Animation playback speed.
    /* 0x05E8 */ int anmID;                     ///< @brief Current animation ID.
    /* 0x05EC */ Z2CreatureRide sound;         ///< @brief Z2 sound creature for ride effects.
    /* 0x0688 */ int field_0x688;
    /* 0x068C */ s8 path_set;  ///< @brief Flag indicating if path has been initialized.
    /* 0x068E */ s16 counter;
    /* 0x0690 */ s16 action;  ///< @brief High-level action ID.
    /* 0x0692 */ s16 old_action;
    /* 0x0694 */ int shadow_key;     ///< @brief Shadow drawing key.
    /* 0x0698 */ s16 timer[4];
    /* 0x06A0 */ s16 wait_timer;
    /* 0x06A2 */ u8 field_0x6a2[0x6ae - 0x6a2];
    /* 0x06AE */ csXyz rotation;
    /* 0x06B4 */ csXyz rotation_spd;
    /* 0x06BA */ s16 damage_timer;
    /* 0x06BC */ u8 field_0x6bc;
    /* 0x06BD */ s8 pursuit_flg;   ///< @brief Flag indicating pursuit mode.
    /* 0x06BE */ u16 status_flag;  ///< @brief Bitfield for various status flags.
    /* 0x06C0 */ s8 field_0x6c0;
    /* 0x06C4 */ f32 x_check;
    /* 0x06C8 */ int path_ten;  ///< @brief Current index in the path points array.
    /* 0x06CC */ int dir;     ///< @brief Direction to iterate path points (+1 or -1).
    /* 0x06D0 */ s16 range;
    /* 0x06D2 */ s16 turn_step;  ///< @brief Step value for turning angle adjustment.
    /* 0x06D4 */ s16 field_0x6d4;
    /* 0x06D6 */ s16 field_0x6d6;
    /* 0x06D8 */ s16 field_0x6d8;
    /* 0x06DA */ s16 field_0x6da;
    /* 0x06DC */ s16 field_0x6dc;
    /* 0x06DE */ csXyz field_0x6de;
    /* 0x06E4 */ s8 field_0x6e4;
    /* 0x06E5 */ u8 field_0x6e5[0x6ec - 0x6e5];
    /* 0x06EC */ csXyz field_0x6ec[29];
    /* 0x079A */ s16 body_angle;  ///< @brief Body tilt angle based on turning.
    /* 0x079C */ u8 start_timer;
    /* 0x079D */ s8 leader;
    /* 0x079E */ s8 lap_num;  ///< @brief Lap or progress count affecting path generation.
    /* 0x079F */ s8 path_adjust_count;  ///< @brief Counter for periodic path adjustment.
    /* 0x07A0 */ s16 dodge_angle;
    /* 0x07A2 */ s8 kiba;
    /* 0x07A4 */ s16 field_0x7a4;
    /* 0x07A6 */ s8 field_0x7a6;
    /* 0x07A7 */ s8 field_0x7a7;
    /* 0x07A8 */ f32 pass;
    /* 0x07AC */ dBgS_AcchCir AcchCir;
    /* 0x07EC */ dBgS_ObjAcch Bgc;
    /* 0x09C4 */ dCcD_Stts Stts;
    /* 0x0A00 */ dCcD_Sph cc_sph[7];
    /* 0x1288 */ dCcD_Sph at_sph;
    /* 0x13C0 */ dCcU_AtInfo at_info;
    /* 0x13E4 */ u8 field_0x13e4;
    /* 0x13E8 */ u32 field_0x13e8[2];
    /* 0x13F0 */ u32 field_0x13f0;
    /* 0x13F4 */ u32 field_0x13f4;
    /* 0x13F8 */ u32 field_0x13f8;
    /* 0x13FC */ u32 field_0x13fc;
    /* 0x1400 */ u32 field_0x1400;
    /* 0x1404 */ u32 field_0x1404;
    /* 0x1408 */ u32 field_0x1408[2];
    /* 0x1410 */ u32 field_0x1410;
    /* 0x1414 */ u32 field_0x1414;
    /* 0x1418 */ u32 field_0x1418;
    /* 0x141C */ u32 field_0x141c;
    /* 0x1420 */ u32 w_eff_id[3];
    /* 0x142C */ u8 movement_type;    ///< @brief Type of movement (1 for normal, 2 for faster).
    /* 0x142D */ u8 collision_flg;  ///< @brief Flags for collision states.
    /* 0x142E */ u8 landing_flag;     ///< @brief Flag for landing after jump.
    /* 0x142F */ s8 gake_flg;   
    /* 0x1430 */ s8 demo_stop;
    /* 0x1432 */ s16 dash_timer;
    /* 0x1434 */ fpc_ProcID rd_id;  // Rider actor ID? ///< @brief Process ID of rider actor.
    /* 0x1438 */ cXyz rein_hand_pos[2];
    /* 0x1450 */ himo_s himo[2];
    /* 0x15D0 */ mDoExt_3DlineMat1_c himo_mat[2];
    /* 0x1648 */ mDoExt_3DlineMat1_c himo_tex;
    /* 0x1684 */ f32 anm_time;  ///< @brief Duration or timer for animation.
    /* 0x1688 */ s16 field_0x1688;
    /* 0x168A */ s16 field_0x168a;
    /* 0x168C */ f32 rein_spd;
    /* 0x1690 */ f32 acceleration;
    /* 0x1694 */ u8 field_0x1694[0x169e - 0x1694];
    /* 0x169E */ s16 demo_mode;
    /* 0x16A0 */ s16 demo_timer;
    /* 0x16A4 */ cXyz demo_cam_eye;
    /* 0x16B0 */ cXyz demo_cam_eye_mae;
    /* 0x16BC */ cXyz demo_cam_ctr;
    /* 0x16C8 */ cXyz demo_cam_ctr_mae;
    /* 0x16D4 */ cXyz demo_cam_target;
    /* 0x16E0 */ cXyz demo_cam_way;
    /* 0x16EC */ cXyz demo_cam_eye_spd;
    /* 0x16F8 */ cXyz demo_cam_way_spd;
    /* 0x1704 */ s16 demo_cam_ya;
    /* 0x1708 */ f32 demo_cam_eye_z;
    /* 0x170C */ f32 demo_cam_eye_x;
    /* 0x1710 */ f32 demo_cam_morf;
    /* 0x1714 */ f32 demo_cam_zoom;
    /* 0x1718 */ f32 field_0x1718;
    /* 0x171C */ f32 field_0x171c;
    /* 0x1720 */ s8 field_0x1720;
    /* 0x1721 */ s8 field_0x1721;
    /* 0x1722 */ s8 saku_burn;
    /* 0x1723 */ u8 field_0x1723[0x17c4 - 0x1723];
    /* 0x17C4 */ u32 anm_se_eff;
    /* 0x17C8 */ u8 field_0x17c8[0x17D0 - 0x17C8];
    /* 0x17D0 */ u32 w_eff_id2[4];
    /* 0x17E0 */ u8 field_0x17e0;
    /* 0x17E1 */ u8 field_0x17e1;
    /* 0x17E2 */ s16 wait_roll_angle;  ///< @brief Roll angle during wait state.
    /* 0x17E4 */ u8 field_0x17e4[0x17e8 - 0x17e4];
    /* 0x17E8 */ f32 ride_speed_max;  ///< @brief Speed rate for riding calculations.
#if TARGET_PC
    cXyz himo_mat_interp_prev[2][16];
    cXyz himo_mat_interp_curr[2][16];
    cXyz himo_tex_interp_prev[2];
    cXyz himo_tex_interp_curr[2];
    bool himo_interp_prev_valid;
    bool himo_interp_curr_valid;
    s8 demo_cam_sync_ticks;
#endif
};

STATIC_ASSERT(sizeof(e_wb_class) == 0x17EC);


#endif /* D_A_E_WB_H */
