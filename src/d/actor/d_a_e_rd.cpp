/**
 * @file d_a_e_rd.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_rd.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_camera.h"
#include "m_Do/m_Do_graphic.h"
#include "d/d_bomb.h"
#include "d/actor/d_a_e_wb.h"
#include "d/d_msg_object.h"
#include "c/c_damagereaction.h"
#include "d/d_attention.h"
#include "d/actor/d_a_obj_h_saku.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_kankyo_mng.h"
#include "d/actor/d_a_e_rdb.h"
#include "d/actor/d_a_e_dn.h"
#include "d/actor/d_a_e_arrow.h"
#include "d/actor/d_a_npc_tk.h"
#include "Z2AudioLib/Z2Instances.h"
#include "f_op/f_op_actor_enemy.h"
#include <cstring>

class daE_RD_HIO_c : public JORReflexible {
public:
    daE_RD_HIO_c();
    virtual ~daE_RD_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x00 */ s8 field_0x4;

    /* ライダー (Rider) */
    /* 0x08 */ f32 model_size;              // 基本サイズ          (Basic size)
    /* 0x0C */ f32 leader_size_ratio;       // リーダーサイズ比     (Leader size ratio)
    /* 0x10 */ f32 movement_speed;          // 移動速度            (Movement speed)

    /*   こん棒兵の場合   (In the case of the club soldier) */
    /* 0x14 */ f32 dash_speed;              // 突進速度            (Rush speed)
    /* 0x18 */ f32 battle_init_range;       // 戦闘開始範囲         (Battle starting range)
    /* 0x1C */ f32 attack_init_range;       // 攻撃開始範囲         (Attack starting range)
    /* 0x20 */ f32 swing_speed;             // 振り速さ            (Swing speed)

    /*   矢兵の場合   (In the case of arrow soldiers) */
    /* 0x24 */ s16 command_to_ready_stance; // 号令→構えの間        (Command → Preparation)
    /* 0x28 */ f32 attack_range;            // 攻撃範囲            (Attack range)
    /* 0x2C */ f32 mounted_launch_distance; // 騎乗発射距離         (Mounted launch distance)
    /* 0x30 */ f32 wolf_falling_power_y;    // 狼倒れ力Y           (Wolf falling power Y)
    /* 0x34 */ f32 wolf_falling_power_z;    // 狼倒れ力Z           (Wolf falling power Z)

    /* 0x38 */ u8 field_0x38;
    /* 0x39 */ u8 invulnerable;             // 不死身              (Invulnerability)
    /* 0x3A */ u8 eye_polygon;              // 目ポリゴン          (Eye polygon)
    /* 0x3B */ u8 one_hit_kill;             // 一撃必殺            (One hit kill)
    /* 0x3C */ f32 ikki_boss_size;              // 一騎（ダ）サイズ     (One-man army (Da) size)
    /* 0x40 */ f32 jump_g;                  // 飛びＧ              (Jump G)
    /* 0x44 */ f32 jump_z;                  // 飛びＺ              (Jump Z)
    /* 0x48 */ f32 jump_z_suspended;        // 飛びＺ（騎乗停止）   (Jump Z (Stop riding))
    /* 0x4C */ f32 jump_y;                  // 飛びＹ              (Jump Y)
};

enum E_RD_RES_FILE_ID {
    /* BCK */
    /* 0x04 */ BCK_RD_ARMAMENT = 4,
    /* 0x05 */ BCK_RD_ATTACK01,
    /* 0x06 */ BCK_RD_AVOID,
    /* 0x07 */ BCK_RD_BOW_READY,
    /* 0x08 */ BCK_RD_BOW_RREADY,
    /* 0x09 */ BCK_RD_BOW_RSHOOT,
    /* 0x0A */ BCK_RD_BOW_SHOOT,
    /* 0x0B */ BCK_RD_DAMAGE_L,
    /* 0x0C */ BCK_RD_DAMAGE_R,
    /* 0x0D */ BCK_RD_DAMAGE_S,
    /* 0x0E */ BCK_RD_DAMAGE_W,
    /* 0x0F */ BCK_RD_DEAD_ARROW,
    /* 0x10 */ BCK_RD_DIEA,
    /* 0x11 */ BCK_RD_DIEA_UP,
    /* 0x12 */ BCK_RD_DIEB,
    /* 0x13 */ BCK_RD_DIEB_UP,
    /* 0x14 */ BCK_RD_DROWNEDA,
    /* 0x15 */ BCK_RD_DROWNEDB,
    /* 0x16 */ BCK_RD_FURA2,            // (Fura = Onomatopoeia for dizziness)
    /* 0x17 */ BCK_RD_GOSIGN,
    /* 0x18 */ BCK_RD_JUMP_A,
    /* 0x19 */ BCK_RD_JUMP_B,
    /* 0x1A */ BCK_RD_JUMP_C,
    /* 0x1B */ BCK_RD_KAMAE,            // (Kamae = Stance)
    /* 0x1C */ BCK_RD_KICK,
    /* 0x1D */ BCK_RD_KYORO2,           // (Kyoro = Onomatopoeia for looking around nervously)
    /* 0x1E */ BCK_RD_RATTACK01,
    /* 0x1F */ BCK_RD_RATTACK01_WAIT,
    /* 0x20 */ BCK_RD_RATTACK02,
    /* 0x21 */ BCK_RD_RATTACK02_WAIT,
    /* 0x22 */ BCK_RD_RATTACK03,
    /* 0x23 */ BCK_RD_RATTACK04,
    /* 0x24 */ BCK_RD_RDAMAGE,
    /* 0x25 */ BCK_RD_RDAMAGE_BACK,
    /* 0x26 */ BCK_RD_RRUN,
    /* 0x27 */ BCK_RD_RRUN02_BACK,
    /* 0x28 */ BCK_RD_RRUN_BACK,
    /* 0x29 */ BCK_RD_RSHOOT,
    /* 0x2A */ BCK_RD_RSHOOT_READY,
    /* 0x2B */ BCK_RD_RSHOOT_WAIT,
    /* 0x2C */ BCK_RD_RSHOOT_WAIT02,
    /* 0x2D */ BCK_RD_RSTEP_BACK,
    /* 0x2E */ BCK_RD_RSTEP_L,
    /* 0x2F */ BCK_RD_RSTEP_LBACK,
    /* 0x30 */ BCK_RD_RSTEP_R,
    /* 0x31 */ BCK_RD_RSTEP_RBACK,
    /* 0x32 */ BCK_RD_RUN,
    /* 0x33 */ BCK_RD_RUN02,
    /* 0x34 */ BCK_RD_RWAIT,
    /* 0x35 */ BCK_RD_RWAIT_BACK,
    /* 0x36 */ BCK_RD_RWALK,
    /* 0x37 */ BCK_RD_RWALK_BACK,
    /* 0x38 */ BCK_RD_SHOOT,
    /* 0x39 */ BCK_RD_SHOOT_READY,
    /* 0x3A */ BCK_RD_SHOOT_WAIT,
    /* 0x3B */ BCK_RD_SHOUT,
    /* 0x3C */ BCK_RD_SIGN,
    /* 0x3D */ BCK_RD_SLEEP_LOOKAROUND,
    /* 0x3E */ BCK_RD_SLEEP_STANDUP,
    /* 0x3F */ BCK_RD_SLEEP_WAIT,
    /* 0x40 */ BCK_RD_WAIT01,
    /* 0x41 */ BCK_RD_WALK,

    /* BMDR */
    /* 0x44 */ BMDR_RD = 0x44,
    /* 0x45 */ BMDR_RD_ARROW,
    /* 0x46 */ BMDR_RD_BARROW,
    /* 0x47 */ BMDR_RD_BOW,
    /* 0x48 */ BMDR_RD_CLUB,
    /* 0x49 */ BMDR_RD_CLUBB,
    /* 0x4A */ BMDR_RD_EYE,
};

enum Action {
    /* 0x00 */ ACTION_NORMAL        = 0,
    /* 0x03 */ ACTION_FIGHT_RUN     = 3,
    /* 0x04 */ ACTION_FIGHT         = 4,
    /* 0x05 */ ACTION_BOW_RUN       = 5,
    /* 0x06 */ ACTION_BOW           = 6,
    /* 0x07 */ ACTION_BOW2          = 7,
    /* 0x08 */ ACTION_BOW_IKKI      = 8,
    /* 0x0A */ ACTION_AVOID         = 10,
    /* 0x0B */ ACTION_WB_SEARCH     = 11,
    /* 0x0C */ ACTION_WB_RIDE       = 12,
    /* 0x0D */ ACTION_WB_RUN        = 13,
    /* 0x0E */ ACTION_WB_BJUMP      = 14,
    /* 0x13 */ ACTION_BOMB          = 19,
    /* 0x14 */ ACTION_S_DAMAGE      = 20,
    /* 0x15 */ ACTION_DAMAGE        = 21,
    /* 0x16 */ ACTION_DROP          = 22,
    /* 0x17 */ ACTION_A_DAMAGE      = 23,
    /* 0x18 */ ACTION_STAND         = 24,
    /* 0x19 */ ACTION_COMMANDER     = 25,
    /* 0x1A */ ACTION_BOW3          = 26,
    /* 0x1B */ ACTION_EXCITE        = 27,
    /* 0x1D */ ACTION_WATER         = 29,
    /* 0x28 */ ACTION_KIBA_START    = 40, // (Kiba = Fang)
    /* 0x29 */ ACTION_KIBA_END      = 41,
    /* 0x2A */ ACTION_IKKI_END      = 42,
    /* 0x2D */ ACTION_IKKI2_START   = 45,
    /* 0x2E */ ACTION_IKKI2_END     = 46,
    /* 0x2F */ ACTION_LV9_END       = 47,
    /* 0x32 */ ACTION_YAGURA        = 50, // (Yagura = Tower/Scaffold)
    /* 0x33 */ ACTION_JYUNKAI       = 51, // (Jyunkai = Patrol)
    /* 0x34 */ ACTION_SLEEP         = 52,
    /* 0x35 */ ACTION_TAG           = 53,
    /* 0x3C */ ACTION_REG           = 60,
};

enum Joint {
    /* 0x00 */ JNT_KOSI,
    /* 0x01 */ JNT_HIP1,
    /* 0x02 */ JNT_KOKAL,
    /* 0x03 */ JNT_MOMOL,
    /* 0x04 */ JNT_SUNEL1,
    /* 0x05 */ JNT_ASIL,
    /* 0x06 */ JNT_KOKAR,
    /* 0x07 */ JNT_MOMORR,
    /* 0x08 */ JNT_SUNER1,
    /* 0x09 */ JNT_ASIR,
    /* 0x0A */ JNT_MUNE1,
    /* 0x0B */ JNT_MUNE2,
    /* 0x0C */ JNT_KUBI,
    /* 0x0D */ JNT_HEAD,
    /* 0x0E */ JNT_HIMOL,
    /* 0x0F */ JNT_HIMOR,
    /* 0x10 */ JNT_SHOULDERL,
    /* 0x11 */ JNT_ARML1,
    /* 0x12 */ JNT_ARML2,
    /* 0x13 */ JNT_HANDL,
    /* 0x14 */ JNT_YUBIL,
    /* 0x15 */ JNT_SHOULDERR,
    /* 0x16 */ JNT_ARMR1,
    /* 0x17 */ JNT_ARMR2,
    /* 0x18 */ JNT_HANDR,
    /* 0x19 */ JNT_YUBIR,
};

daE_RD_HIO_c::daE_RD_HIO_c() {
    field_0x4 = -1;
    model_size = 1.2f;
    leader_size_ratio = 1.2f;
    movement_speed = 3.5f;
    dash_speed = 14.0f;
    battle_init_range = 250.0f;
    attack_init_range = 300.0f;
    swing_speed = 1.1f;
    command_to_ready_stance = 60;
    attack_range = 1300.0f;
    mounted_launch_distance = 2000.0f;
    wolf_falling_power_y = 3.0f;
    wolf_falling_power_z = 3.0f;
    field_0x38 = 1;
    invulnerable = 0;
    eye_polygon = 1;
    one_hit_kill = 0;
    ikki_boss_size = 75.0f;
    jump_z_suspended = 5.0f;
    jump_z = 10.0f;
    jump_y = 33.0f;
    jump_g = 5.0f;
}

#if DEBUG
void daE_RD_HIO_c::genMessage(JORMContext* ctext) {
    // Rider (a.k.a. Bulblins)
    ctext->genLabel("  ライダー", 0x80000001);
    // basic size
    ctext->genSlider("基本サイズ", &model_size, 0.0f, 5.0f);
    // leader size ratio
    ctext->genSlider("リーダーサイズ比", &leader_size_ratio, 0.0f, 2.0f);
    // movement speed
    ctext->genSlider("移動速度", &movement_speed, 0.0f, 20.0f);
    // ~~ In the case of a club-wielding soldier ~~
    ctext->genLabel(" ～～  こん棒兵の場合  ～～", 0x80000001);
    // speed rush
    ctext->genSlider("突進速度", &dash_speed, 0.0f, 40.0f);
    // combat start range
    ctext->genSlider("戦闘開始範囲", &battle_init_range, 0.0f, 2000.0f);
    // attack range
    ctext->genSlider("攻撃開始範囲", &attack_init_range, 0.0f, 1000.0f);
    // swing speed
    ctext->genSlider("振り速さ", &swing_speed, 0.0f, 3.0f);
    ctext->genLabel("  ", 0x80000001);
    // ~~ In the case of archers ~~
    ctext->genLabel(" ～～  矢兵の場合  ～～", 0x80000001);
    // Command → Ready stance
    ctext->genSlider("号令→構えの間", &command_to_ready_stance, 0, 200);
    // attack range
    ctext->genSlider("攻撃範囲", &attack_range, 0.0f, 2000.0f);
    // riding firing distance
    ctext->genSlider("騎乗発射距離", &mounted_launch_distance, 0.0f, 3000.0f);
    // wolf takedown power Y
    ctext->genSlider("狼倒れ力Y", &wolf_falling_power_y, 0.0f, 50.0f);
    // wolf takedown power Z
    ctext->genSlider("狼倒れ力Z", &wolf_falling_power_z, 0.0f, 50.0f);
    // flying Z (Rider suspended)
    ctext->genSlider("飛びＺ（騎乗停止）", &jump_z_suspended, 0.0f, 50.0f);
    // flying Z
    ctext->genSlider("飛びＺ", &jump_z, 0.0f, 50.0f);
    // flying y
    ctext->genSlider("飛びＹ", &jump_y, 0.0f, 50.0f);
    // jump g
    ctext->genSlider("飛びＧ", &jump_g, 0.0f, 20.0f);
    // invincible
    ctext->genCheckBox("不死身", &invulnerable, 0x1);
    // eye polygons
    ctext->genCheckBox("目ポリゴン", &eye_polygon, 0x1);
    // one-hit kill
    ctext->genCheckBox("一撃必殺", &one_hit_kill, 0x1);
    // ikki (da) size
    ctext->genSlider("一騎（ダ）サイズ", &ikki_boss_size, 0.0f, 150.0f);
}
#endif

static fopAc_ac_c* get_pla(fopAc_ac_c* actor) {
    fopAc_ac_c* pla = dComIfGp_getPlayer(0);
    // "coach" refers to the Ilia/Telma transport wagon:
    fopAc_ac_c* coach = fopAcM_SearchByName(PROC_NPC_COACH);

    if (coach == NULL) {
        return dComIfGp_getPlayer(0);
    }

    f32 pla_x, coach_x, pla_z, coach_z;
    pla_x = pla->current.pos.x - actor->current.pos.x;
    pla_z = pla->current.pos.z - actor->current.pos.z;
    coach_x = coach->current.pos.x - actor->current.pos.x;
    coach_z = coach->current.pos.z - actor->current.pos.z;
    // If the XZ-dist of Link is further than the coach, then focus the coach.
    if (SQUARE(pla_x) + SQUARE(pla_z) > SQUARE(coach_x) + SQUARE(coach_z)) {
        return coach;
    }

    return dComIfGp_getPlayer(0);
}

static void anm_init(e_rd_class* i_this, int i_no, f32 i_morf, u8 i_mode, f32 i_speed) {
    if (i_this->field_0x680 == 0) {
        if (i_this->actor_set != 0) {
            if (i_no < 73) {
                i_this->anm_p->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_rdb", i_no),
                                            i_mode, i_morf, i_speed, 0.0f, -1.0f);
                i_this->anm = i_no;
            }
        } else {
            i_this->anm_p->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, i_no),
                                        i_mode, i_morf, i_speed, 0.0f, -1.0f);
            i_this->anm = i_no;

            if (i_no == BCK_RD_KYORO2) {
                i_this->anm_p->setFrame(cM_rndF(30.0f));
            }
        }
    }
}

static void horn_anm_init(e_rd_class* i_this, int i_anmID, f32 i_morf, u8 i_mode, f32 i_speed) {
    if (i_this->field_0x6a0 != 0) {
        i_this->mpMorfHornAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_rdb", i_anmID),
                                      i_mode, i_morf, i_speed, 0.0f, -1.0f, NULL);
    }
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DJoint* joint = i_joint;
        int jointNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_rd_class* i_this = (e_rd_class*)model->getUserArea();
        fopEn_enemy_c* enemy = &i_this->enemy;
        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
            if (jointNo == JNT_KOSI) {
                if (i_this->field_0x9c8 == 3) {
                    cMtx_YrotM(*calc_mtx, s16(i_this->field_0x9ca));
                }
            } else if (jointNo == JNT_HIP1) {
                if (i_this->field_0x9c8 == 3) {
                    cMtx_XrotM(*calc_mtx, i_this->field_0x9ca);
                }

                cMtx_YrotM(*calc_mtx, s16(i_this->field_0xade.y));
                cMtx_ZrotM(*calc_mtx, -i_this->field_0xa32[2].x + i_this->field_0xade.x + i_this->field_0xabc[2]);
            } else if (jointNo == JNT_KOKAL) {
                cMtx_YrotM(*calc_mtx, i_this->field_0xaca);
            } else if (jointNo == JNT_SUNEL1) {
                cMtx_ZrotM(*calc_mtx, i_this->field_0xacc[0]);
            } else if (jointNo == JNT_KOKAR) {
                cMtx_YrotM(*calc_mtx, i_this->field_0xacc[1]);
            } else if (jointNo == JNT_SUNER1) {
                cMtx_ZrotM(*calc_mtx, i_this->field_0xacc[2]);
            } else if (jointNo == JNT_MUNE2) {
                cMtx_YrotM(*calc_mtx, i_this->field_0xa32[1].y);
                if (i_this->field_0x9c8 == 3) {
                    cMtx_XrotM(*calc_mtx, i_this->field_0x9ca);
                }

                cMtx_ZrotM(*calc_mtx, i_this->field_0xa32[1].x + i_this->field_0xabc[1]);

                if (i_this->field_0x9c8 == 4) {
                    f32 fVarTemp = i_this->field_0x9cc * -1.0f;
                    cMtx_YrotM(*calc_mtx, fVarTemp);
                    fVarTemp = i_this->field_0x9cc * 0.3f;
                    cMtx_XrotM(*calc_mtx, fVarTemp);
                }

                if (i_this->field_0x9c8 == 3) {
                    cMtx_ZrotM(*calc_mtx, i_this->field_0x9cc);
                }
            } else if (jointNo == JNT_HEAD) {
                if (i_this->field_0x9c8 != 3 && i_this->field_0x9c8 != 4) {
                    cMtx_YrotM(*calc_mtx, (-i_this->field_0x9ca + i_this->field_0x9ce) / 2);
                    cMtx_ZrotM(*calc_mtx, (-i_this->field_0x9cc + i_this->field_0xa32[0].x + i_this->field_0xabc[0]) / 2);
                }
            } else if (jointNo == JNT_KUBI) {
                cMtx_YrotM(*calc_mtx, i_this->field_0xa32[0].y);

                if (i_this->field_0x9c8 != 3 && i_this->field_0x9c8 != 4) {
                    cMtx_YrotM(*calc_mtx, (-i_this->field_0x9ca + i_this->field_0x9ce) / 2);
                    cMtx_ZrotM(*calc_mtx, (-i_this->field_0x9cc + i_this->field_0xa32[0].x + i_this->field_0xabc[0]) / 2);
                } else {
                    cMtx_ZrotM(*calc_mtx, i_this->field_0xa32[0].x + i_this->field_0xabc[0]);
                }
            } else if (jointNo == JNT_ARML1) {
                cMtx_YrotM(*calc_mtx, i_this->field_0xa32[3].y + i_this->field_0xabc[3]);
            } else if (jointNo == JNT_ARML2) {
                cMtx_YrotM(*calc_mtx, i_this->field_0xa32[4].y + i_this->field_0xac4);
            } else if (jointNo == JNT_ARMR1) {
                cMtx_YrotM(*calc_mtx, i_this->field_0xa32[5].y + i_this->field_0xac6);
            } else if (jointNo == JNT_ARMR2) {
                cMtx_YrotM(*calc_mtx, i_this->field_0xa32[6].y + i_this->field_0xac8);
            }

            if (i_this->field_0xada != 0) {
                if ((jointNo & 1) != 0) {
                    cMtx_YrotM(*calc_mtx, i_this->field_0xada);
                } else if ((jointNo & 2) != 0) {
                    cMtx_XrotM(*calc_mtx, i_this->field_0xada);
                } else if ((jointNo & 4) != 0) {
                    cMtx_ZrotM(*calc_mtx, i_this->field_0xada);
                }
            }

            model->setAnmMtx(jointNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int nodeCallBack_B(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DJoint* joint = i_joint;
        int jointNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_rd_class* i_this = (e_rd_class*)model->getUserArea();
        fopEn_enemy_c* enemy = &i_this->enemy;
        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);

            if (jointNo == JNT_HIP1 || jointNo == JNT_KOKAL) {
                cMtx_XrotM(*calc_mtx, i_this->field_0x9ca / 8);
            } else if (jointNo == JNT_ASIL || jointNo == JNT_KOKAR) {
                cMtx_YrotM(*calc_mtx, -(i_this->field_0x9ca / 3));
            }

            model->setAnmMtx(jointNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int nodeCallBack_bow(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DJoint* my_joint = i_joint;
        int jointNo = my_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_rd_class* i_this = (e_rd_class*)model->getUserArea();
        fopEn_enemy_c* enemy = &i_this->enemy;
        if (i_this != NULL && i_this->field_0x9a8 != 0) {
            MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
            cMtx_ZrotM(*calc_mtx, i_this->field_0x9a8);
            model->setAnmMtx(jointNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int daE_RD_Draw(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    J3DModel* model;
    J3DMaterial* matNode_p;
    J3DModelData* model_data;

    i_this->field_0x5bb = 0;
    if (i_this->field_0xafb != 0) {
        return 1;
    }

    model = i_this->anm_p->getModel();
    g_env_light.settingTevStruct(0, &enemy->current.pos, &enemy->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &enemy->tevStr);

    if (i_this->actor_set == 0 && i_this->field_0x968 != 0) {
        model_data = model->getModelData();
        for (u16 i = 0; i < model_data->getMaterialNum(); i++) {
            J3DMaterial* mat_node_p = model_data->getMaterialNodePointer(i);
            mat_node_p->getTevColor(0)->r = i_this->field_0x96c;
            mat_node_p->getTevColor(0)->g = i_this->field_0x96c;
            mat_node_p->getTevColor(0)->b = i_this->field_0x96c;
        }
    }

    if (i_this->actor_set == 3) {
        J3DShape* shape = model->getModelData()->getMaterialNodePointer(3)->getShape();
        if (shape != NULL) {
            shape->hide();
        }
    }

    i_this->anm_p->entryDL();

    if (i_this->actor_set == 0 && i_this->field_0x968 != 0) {
        J3DModelData* modelData = model->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            matNode_p = modelData->getMaterialNodePointer(i);
            matNode_p->getTevColor(0)->r = 0;
            matNode_p->getTevColor(0)->g = 0;
            matNode_p->getTevColor(0)->b = 0;
        }
    }

    if (i_this->weapon_type == 1) {
        g_env_light.setLightTevColorType_MAJI(i_this->arrow, &enemy->tevStr);
        mDoExt_modelUpdateDL(i_this->arrow);
    } else if (i_this->weapon_type >= 2) {
        g_env_light.setLightTevColorType_MAJI(i_this->bow_anm->getModel(), &enemy->tevStr);
        i_this->bow_anm->entryDL();

        if (i_this->field_0x9a2 != 0) {
            g_env_light.setLightTevColorType_MAJI(i_this->arrow, &enemy->tevStr);
            mDoExt_modelUpdateDL(i_this->arrow);
        }
    }

    if (i_this->actor_set != 0) {
        for (int i = 0; i < 14; i++) {
            if (i_this->armor_boss_part[i] != NULL) {
                g_env_light.setLightTevColorType_MAJI(i_this->armor_boss_part[i], &enemy->tevStr);
                mDoExt_modelUpdateDL(i_this->armor_boss_part[i]);
            }
        }

        if (i_this->field_0x6a0 != 0) {
            g_env_light.setLightTevColorType_MAJI(i_this->mpMorfHornAnm->getModel(), &enemy->tevStr);
            i_this->mpMorfHornAnm->entryDL();
        }
    }

    if (i_this->field_0x1297 != 0) {
        for (int i = 0; i < 2; i++) {
            g_env_light.setLightTevColorType_MAJI(i_this->eye_model[i], &enemy->tevStr);
            model_data = i_this->eye_model[i]->getModelData();
            for (u16 j = 0; j < model_data->getMaterialNum(); j++) {
                matNode_p = model_data->getMaterialNodePointer(j);
                matNode_p->getTevColor(1)->r = 0xFF;
                matNode_p->getTevColor(1)->g = 0x8A;
                matNode_p->getTevColor(1)->b = 0x18;
            }

            mDoExt_modelUpdateDL(i_this->eye_model[i]);
        }
    }

    if (i_this->ride_mode != 2) {
        cXyz pos;
        pos.set(enemy->current.pos.x, enemy->current.pos.y + 100.0f, enemy->current.pos.z);
        i_this->shadow_key = dComIfGd_setShadow(i_this->shadow_key, 1, model, &pos,
                                                1200.0f + BREG_F(19), 0.0f, enemy->current.pos.y,
                                                i_this->ObjAcch.GetGroundH(), i_this->ObjAcch.m_gnd,
                                                &i_this->enemy.tevStr, 0, 1.0f,
                                                dDlst_shadowControl_c::getSimpleTex());
        if (i_this->weapon_type == 1) {
            dComIfGd_addRealShadow(i_this->shadow_key, i_this->arrow);
        }
    }

    if (i_this->arg1 == 14) {
        if (i_this->blurRate > 2) {
            mDoGph_gInf_c::setBlureRate(i_this->blurRate);
            mDoGph_gInf_c::onBlure();
        } else {
            mDoGph_gInf_c::offBlure();
        }
    }

    return 1;
}

static BOOL other_bg_check(e_rd_class* i_this, fopAc_ac_c* i_other) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    dBgS_LinChk lin_chk;
    cXyz unused_vec;
    cXyz start;
    cXyz end;

    end = i_other->current.pos;
    end.y += 130.0f;
    start = enemy->current.pos;
    start.y = enemy->eyePos.y;

    lin_chk.Set(&start, &end, enemy);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        if (i_other != dComIfG_Bgsp().GetActorPointer(lin_chk)) {
            return TRUE;
        }
    }

    return FALSE;
}

static BOOL otoCheck(fopAc_ac_c* i_actor, f32 param_2) {
    SND_INFLUENCE* sound = dKy_Sound_get();
    fpc_ProcID uVar1;

    if (sound->actor_id != fpcM_ERROR_PROCESS_ID_e && sound->actor_id != fopAcM_GetID(i_actor)) {
        cXyz sp8c = sound->position - i_actor->current.pos;
        if (sp8c.abs() < param_2) {
            dBgS_LinChk lin_chk;
            cXyz start, end;
            end = sound->position;
            end.y += 100.0f;
            start = i_actor->current.pos;
            start.y += 100.0f;
            lin_chk.Set(&start, &end, i_actor);

            if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
                return sound->field_0xc;
            }
        }
    }

    return FALSE;
}

static BOOL pl_pass_check(e_rd_class* i_this, f32 param_2) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    fopAc_ac_c* actor = get_pla(enemy);
    cXyz mae, ato;

    mae = enemy->current.pos - actor->current.pos;
    cMtx_YrotS(*calc_mtx, -(actor->shape_angle.y));
    MtxPosition(&mae, &ato);

    if (ato.z > param_2) {
        return TRUE;
    }

    return FALSE;
}

static e_rd_class* boss;

static u8 hio_set;

static daE_RD_HIO_c l_HIO;

static s16 S_find;

static bool lbl_70_bss_AE;

static cXyz S_find_pos;

static u8 desert_substage;

static u8 data_80519201;

static fopAc_ac_c* target_info[10];

static int target_info_count;

static s8 data_80519230;

static void* s_b_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && dBomb_c::checkBombActor((fopAc_ac_c*)i_actor) && 
        !((dBomb_c*)i_actor)->checkStateExplode() && target_info_count < 10) {
        target_info[target_info_count] = (fopAc_ac_c*)i_actor;
        target_info_count++;
    }

    return NULL;
}

static dBomb_c* search_bomb(e_rd_class* i_this, int param_1) {
    if ((i_this->field_0xa20 & 0x10000000) == 0) {
        return NULL;
    }

    fopAc_ac_c* actor = &i_this->enemy;
    dBomb_c* bomb;

    target_info_count = 0;
    for (int i = 0; i < 10; i++) {
        target_info[i] = NULL;
    }

    // For whatever reason, the original devs omitted this function call. Not sure if intentionall or by mistake.
    // In any case, because target_info_count is left at 0 and only incremented
    // in s_b_sub, the entire rest of this function disappears in release asm.
#if 0
    fpcM_Search(s_b_sub, i_this);
#endif

    f32 threshold = 100.0f;
    f32 compare_val_f = 1500.0f;
    if (target_info_count != 0) {
        cXyz mae, vec, ato;
        for (int i = 0; i < target_info_count;) {
            bomb = (dBomb_c*)target_info[i];
            mae.x = bomb->current.pos.x - actor->eyePos.x;
            mae.y = 50.0f + bomb->current.pos.y - actor->eyePos.y;
            mae.z = bomb->current.pos.z - actor->eyePos.z;
            vec.x = bomb->current.pos.x - actor->current.pos.x;
            vec.z = bomb->current.pos.z - actor->current.pos.z;
            f32 dist1 = JMAFastSqrt(SQUARE(mae.x) + SQUARE(mae.z));
            f32 dist2 = JMAFastSqrt(SQUARE(vec.x) + SQUARE(vec.z));
            if (dist1 < threshold && !(dist2 > i_this->dis + 30.0f)
                && (!other_bg_check(i_this, bomb) || !param_1))
            {
                if (param_1) {
                    f32 abs_r = fabsf(50.0f + bomb->current.pos.y - actor->eyePos.y);
                    if (abs_r <= 300.0f) {
                        s16 angleY = actor->shape_angle.y - cM_atan2s(mae.x, mae.z);
                        if (angleY < 0) {
                            angleY = (-1 * angleY);
                        }

                        if ((u16)angleY < 20000) {
                            return bomb;
                        }

                        cMtx_YrotS(*calc_mtx, -actor->shape_angle.y);
                        MtxPosition(&mae, &ato);
                        if (fabsf(ato.x) < 500.0f && fabsf(ato.y) < 300.0f
                            && ato.z > -125.0f && ato.z < 500.0f)
                        {
                            return bomb;
                        }
                    }
                } else {
                    return bomb;
                }
            }

            i++;
            if (i == target_info_count) {
                i = 0;
                threshold += 100.0f;
                if (threshold > compare_val_f) {
                    return NULL;
                }
            }
        }
    } else {
        return NULL;
    }

    return NULL;
}

static BOOL way_bg_check(e_rd_class* i_this, f32 param_2) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    dBgS_LinChk lin_chk;
    cXyz mae, start, ato;

    start = enemy->current.pos;
    start.y += 50.0f;
    cMtx_YrotS(*calc_mtx, enemy->shape_angle.y);
    mae.x = 0.0f;
    mae.y = 50.0f;
    mae.z = param_2;
    MtxPosition(&mae, &ato);
    ato += enemy->current.pos;
    lin_chk.Set(&start, &ato, enemy);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return TRUE;
    }

    return FALSE;
}

static void ride_off(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    e_wb_class* boar = (e_wb_class*)fopAcM_SearchByID(i_this->boar_id);

    if (boar != NULL) {
        UNSET_FLAG(boar->mStatusFlags, i_this->boar_stand, u16);
        i_this->boar_stand = 0;

        if (boar->mActionID != 22 && boar->mActionID != 23 && boar->mActionID != 24) {
            if (boar->mActionID == 1) {
                if ((boar->mStatusFlags & 3) != 0) {
                    boar->mActionID = 6;
                } else {
                    boar->mActionID = 0;
                }

                boar->mActionMode = 0;
            } else if (boar->mActionID == 21) {
                boar->mActionMode = 0;
            } else if ((boar->mStatusFlags & 1) == 0) {
                if (boar->mActionID == 6 && (boar->mActionMode == 2 || boar->mActionMode == 3)) {
                    boar->mActionMode = 1;
                } else {
                    boar->mActionMode = 0;
                }

                boar->mActionID = 7;
                boar->field_0x698[1] = cM_rndF(30.0f) + 80.0f;
            }
        }

        enemy->current.angle.x = 0;
        i_this->boar_id = -1;
    }

    i_this->ride_mode = 0;
    if (i_this->bow_anm != NULL) {
        i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 10), 0,
                                    1.0f, 1.0f, 0.0f, -1.0f);
        i_this->bow_anm->setFrame(10.0f);
    }

    enemy->home.pos = enemy->current.pos;
}

static void* s_wb_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_WB) {
        e_wb_class* boar = (e_wb_class*) i_actor;
        e_rd_class* i_this = (e_rd_class*)i_data;
        if (boar->mActionID != ACTION_DROP && boar->mActionID != ACTION_DAMAGE
            && boar->mActionID != ACTION_A_DAMAGE && boar->mActionID != ACTION_STAND) {
            if (i_this->actor_set == boar->field_0x79d &&
                (data_80519230 != 0 || (boar->mStatusFlags & 3) != 3) && target_info_count < 10) {
                target_info[target_info_count] = (fopAc_ac_c*)i_actor;
                target_info_count++;
            }
        }
    }

    return NULL;
}

static fpc_ProcID search_wb(e_rd_class* i_this, s16 param_2) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    target_info_count = 0;
    for (int i = 0; i < 10; i++) {
        target_info[i] = NULL;
    }

    if (param_2 == 1) {
        data_80519230 = 1;
    } else {
        data_80519230 = 0;
    }

    fpcM_Search(s_wb_sub, i_this);
    f32 dist = 100.0f;

    f32 fVar2 = 1500.0f;
    // Hyrule Field - Bridge of Eldin
    if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0 && fopAcM_GetRoomNo(enemy) == 0) {
        fVar2 = hREG_F(0) + 5000.0f;
    }

    if (target_info_count != 0) {
        cXyz vec, sp60;
        fopAc_ac_c* actor;
        int i = 0;
        while (i < target_info_count) {
            actor = (fopAc_ac_c*)target_info[i];
            vec.x = actor->current.pos.x - enemy->eyePos.x;
            vec.y = (actor->current.pos.y + 50.0f) - enemy->eyePos.y;
            vec.z = actor->current.pos.z - enemy->eyePos.z;
            if (JMAFastSqrt(SQUARE(vec.x) + SQUARE(vec.z)) < dist) {
                if (param_2 == 1) {
                    return fopAcM_GetID(actor);
                }

                s16 range = enemy->shape_angle.y - cM_atan2s(vec.x, vec.z);
                if (range < param_2 && range > (s16)-param_2 && !other_bg_check(i_this, actor)) {
                    return fopAcM_GetID(actor);
                }
            }

            i++;
            if (i == target_info_count) {
                i = 0;
                dist += 100.0f;
                if (dist > fVar2) {
                    return -1;
                }
            }
        }
    } else {
        return -1;
    }

    return -1;
}

static void wb_check(e_rd_class* i_this, s16 param_2) {
    e_wb_class* boar;
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    if (desert_substage != 118 || fopAcM_GetRoomNo(enemy) != 1) {
        // Arbiter's Grounds Exterior - a.k.a. Bulblin camp
        // OR most other areas in the game. Except for (non-exhaustive):
        //  1. Outside Link's home
        //  2. Ordon Spring
        i_this->boar_id = search_wb(i_this, param_2);
        boar = (e_wb_class*)fopAcM_SearchByID(i_this->boar_id);
        if (boar != NULL) {
            if (desert_substage != 0) {
                if (i_this->weapon_type != 1) {
                    if ((boar->mStatusFlags & 1) == 0) {
                        return;
                    }
                } else if ((boar->mStatusFlags & 1) != 0) {
                    return;
                }
            }

            i_this->action = ACTION_WB_SEARCH;
            i_this->mode = 0;

            if ((boar->mStatusFlags & 1) == 0) {
                boar->mStatusFlags |= (u16) 1;
                i_this->boar_stand = 1;
            } else if ((boar->mStatusFlags & 2) == 0) {
                boar->mStatusFlags |= (u16) 2;
                i_this->boar_stand = 2;
            }
        }
    }
}

static BOOL wb_init_ride(e_rd_class* i_this) {
    i_this->boar_id = search_wb(i_this, 1);
    e_wb_class* boar = (e_wb_class*)fopAcM_SearchByID(i_this->boar_id);

    if (boar != NULL) {
        i_this->action = ACTION_WB_RIDE;
        i_this->mode = 0;
        i_this->ride_mode = 2;
        i_this->boar_stand = i_this->arg0;
        boar->mStatusFlags |= (s8)i_this->arg0;
        boar->mActionID = 1;

        if (i_this->weapon_type != 0 && i_this->boar_stand == 1) {
            i_this->field_0x5bd = 1;
        }

        return TRUE;
    }

    OS_REPORT("......CAN NOT_ RIDE  !!!!\n");
    return FALSE;
}

static BOOL pl_check(e_rd_class* i_this, f32 range, s16 angle) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    fopAc_ac_c* actor = get_pla(enemy);

    if ((desert_substage == 0 && dComIfGp_event_runCheck())) {
        return FALSE;
    }

    // Hidden Village
    if (strcmp(dComIfGp_getStartStageName(), "F_SP128") == 0 && actor->current.pos.z < -9800.0f) {
        return FALSE;
    }

    if (S_find != 0) {
        range = 10000.0f;
    }

    if (i_this->dis < range) {
        s16 target = enemy->shape_angle.y - i_this->angleY;
        if (target < angle && target > (s16)-angle && !other_bg_check(i_this, actor)) {
            return TRUE;
        }

        for (int i = 0; i <= 2; i++) {
            if (i_this->cc_sph[i].ChkCoHit()) {
                cCcD_Obj* hit_obj = i_this->cc_sph[i].GetCoHitObj();
                if (daPy_getPlayerActorClass() == dCc_GetAc(hit_obj->GetAc())) {
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

static dBomb_c* bomb_view_check(e_rd_class* i_this) {
    fopEn_enemy_c* unused_p = &i_this->enemy;
    return search_bomb(i_this, 1);
}

static dBomb_c* bomb_check(e_rd_class* i_this) {
    fopEn_enemy_c* unused_p = &i_this->enemy;
    return search_bomb(i_this, 0);
}

static BOOL move_gake_check(e_rd_class* i_this, f32 gake) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    dBgS_GndChk gnd_chk;
    cXyz mae, ato;

    cMtx_YrotS(*calc_mtx, enemy->shape_angle.y);
    mae.x = 0.0f;
    mae.y = 100.0f;
    mae.z = gake;
    MtxPosition(&mae, &ato);
    ato += enemy->current.pos;
    gnd_chk.SetPos(&ato);
    if ((enemy->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk)) > 300.0f) {
        return TRUE;
    }

    return FALSE;
}

static BOOL way_check(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz mae;

    // Hidden Village
    if (strcmp(dComIfGp_getStartStageName(), "F_SP128") == 0 && enemy->current.pos.z < -8400.0f) {
        mae.x = enemy->home.pos.x - enemy->current.pos.x;
        mae.z = enemy->home.pos.z - enemy->current.pos.z;
        i_this->target_ya = cM_atan2s(mae.x, mae.z);
        return TRUE;
    }

    cXyz start, ato;
    s16 angle = cM_rndF(65535.0f);
    f32 z = 1000.0f;
    start = enemy->current.pos;
    start.y += 50.0f;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 16; j++) {
            cMtx_YrotS(*calc_mtx, angle);
            mae.x = 0.0f;
            mae.y = 0.0f;
            mae.z = z;
            MtxPosition(&mae, &ato);
            ato += start;
            dBgS_LinChk lin_chk;
            lin_chk.Set(&start, &ato, enemy);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                ANGLE_ADD(angle, 0x1000);
            } else {
                i_this->target_ya = angle;
                return TRUE;
            }
        }

        z -= 150.0f;
    }

    return FALSE;
}

static BOOL path_check(e_rd_class* i_this) {
    static u8 check_index[255];
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    dBgS_LinChk lin_chk;
    cXyz start, end;

    start = enemy->current.pos;
    start.y += WREG_F(0) + 10.0f;

    dPnt* points = i_this->path->m_points;
    for (int i = 0; i < i_this->path->m_num; i++, points++) {
        if (i < 0xFF) {
            end.x = points->m_position.x;
            end.y = points->m_position.y + 10.0f + WREG_F(1);
            end.z = points->m_position.z;
            lin_chk.Set(&start, &end, enemy);
            if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
                check_index[i] = 1;
            } else {
                check_index[i] = 0;
            }
        }
    }

    f32 x, y, z;
    f32 dist = 100.0f;
    bool found = false;
    f32 reg_f27; // unused
    for (int i = 0; i < 100;) {
        points = i_this->path->m_points;
        for (int j = 0; j < i_this->path->m_num; j++, points++) {
            if (j < 0xFF && check_index[j] != 0) {
                x = enemy->current.pos.x - points->m_position.x;
                y = enemy->current.pos.y - points->m_position.y;
                z = enemy->current.pos.z - points->m_position.z;
                if (JMAFastSqrt(x * x + y * y + z * z) < dist) {
                    i_this->field_0xb00 = j;
                    found = true;
                    break;
                }
            }
        }

        if (found) {
            break;
        }

        ++i;
        reg_f27 = dist += 50.0f;
    }

    if (!found) {
        return FALSE;
    }

    if (cM_rndF(1.0f) < 0.5f) {
        i_this->path_dir = 1; // forward
    } else {
        i_this->path_dir = 0xFF; // backward
    }

    return TRUE;
}

static void e_rd_normal(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    if (i_this->arg0 == 1 || i_this->arg0 == 2) {
        if (wb_init_ride(i_this)) {
            i_this->arg0 = 0;
        }

        return;
    }

    cXyz unused_vec_0, unused_vec_1;
    f32 speed = 0.0f;
    s16 angle = 0x4000;
    if (desert_substage != 0) {
        angle = 0x6000;
    }

    switch (i_this->mode) {
        case 0:
            if (i_this->field_0x5bd != 0) {
                anm_init(i_this, BCK_RD_ARMAMENT, 2.0f, 0, 1.0f);
                i_this->mode = 1;
            } else {
                i_this->mode = 2;
            }
            break;

        case 1:
            if ((int)i_this->anm_p->getFrame() == 11) {
                i_this->field_0x5bd = 0;
            }

            if (i_this->anm_p->isStop()) {
                i_this->mode = 2;
            }
            break;

        case 2:
            if (i_this->timer[0] == 0) {
                if (i_this->path != NULL && path_check(i_this)) {
                    i_this->action = ACTION_JYUNKAI;
                    i_this->mode = 0;
                    return;
                }

                if (way_check(i_this)) {
                    anm_init(i_this, BCK_RD_WALK, 10.0f, 2, 1.0f);
                    i_this->mode = 3;
                    i_this->timer[0] = cM_rndF(100.0f) + 100.0f;
                }
            } else if (desert_substage != 0) {
                angle = 0x7000;
            } else {
                angle = 0x6000;
            }
            break;

        case 3:
            speed = l_HIO.movement_speed;
            if (enemy->speedF >= 3.0f && move_gake_check(i_this, 100.0f)) {
                i_this->mode = 4;
                i_this->timer[0] = cM_rndF(100.0f) + 50.0f;
                anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
            } else {
                if (i_this->timer[0] == 0 || (enemy->speedF >= 3.0f && way_bg_check(i_this, 200.0f))) {
                    i_this->mode = 2;
                    i_this->timer[0] = cM_rndF(100.0f) + 50.0f;
                    anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
                }
            }
            break;

        case 4:
            i_this->field_0x9c8 = 0xFF;
            if (i_this->timer[0] == 0) {
                i_this->target_ya = enemy->current.angle.y + 0x8000 + (s16)cM_rndFX(4000.0f);
                anm_init(i_this, BCK_RD_WALK, 10.0f, 2, 1.0f);
                i_this->mode = 3;
                i_this->timer[0] = cM_rndF(100.0f) + 100.0f;
            }
    }

    if (speed) {
        cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->target_ya, 8, 0x400);
        s16 range = enemy->current.angle.y - i_this->target_ya;
        if (range > 0x400 || range < -0x400) {
            speed = 0.0f;
        }
    }

    cLib_addCalc2(&enemy->speedF, speed, 1.0f, 3.0f);
    f32 range = i_this->attack_range;
    BOOL search_check = FALSE;
    if (i_this->look_timer == 1 || S_find != 0) {
        if (i_this->look_timer == 1) {
            search_check = TRUE;
        }

        range = 10000.0f;
        angle = 0x7FFF;
    }

    if (((i_this->counter & 7) == 0 || search_check) && i_this->mode >= 2) {
        if ((i_this->counter & 8) != 0 && !search_check) {
            wb_check(i_this, angle);
        } else {
            if (pl_check(i_this, range, angle)) {
                if (i_this->weapon_type == 1) {
                    i_this->action = ACTION_FIGHT_RUN;
                    i_this->mode = -10;
                    i_this->timer[0] = 60;
                } else if (i_this->weapon_type >= 2) {
                    i_this->action = ACTION_BOW_RUN;
                    i_this->mode = -10;
                }
            }

            if (bomb_view_check(i_this) != NULL) {
                i_this->action = ACTION_BOMB;
                i_this->mode = 0;
            }
        }
    }
}

static void e_rd_fight_run(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    fopAc_ac_c* pla = dComIfGp_getPlayer(0);
    cXyz sp64, sp70;
    f32 speed = 0.0f;
    s8 attack_flag = true;

    if (!pl_check(i_this, i_this->attack_range + 50.0f, 0x7FFF) && i_this->timer[0] == 0) {
        if (i_this->attack_timer == 0) {
            attack_flag = false;
        }
    } else {
        i_this->attack_timer = 35;
    }

    f32 dash_speed;
    if (desert_substage != 0) {
        dash_speed = 24.0f;
    } else {
        dash_speed = l_HIO.dash_speed;
    }

    if (i_this->weapon_type != 1) {
        if (i_this->weapon_type >= 2) {
            i_this->action = ACTION_BOW_RUN;
        } else {
            i_this->action = ACTION_NORMAL;
            i_this->timer[0] = cM_rndF(50.0f) + 50.0f;
            anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
        }

        i_this->mode = 0;
    } else {
        i_this->dis -= fabsf(enemy->current.pos.y - pla->current.pos.y) * 0.7f;

        switch (i_this->mode) {
            case -20:
                if ((int)i_this->anm_p->getFrame() == 11) {
                    i_this->field_0x5bd = 0;
                }

                if (i_this->anm_p->isStop()) {
                    i_this->mode = 0;
                }
                break;

            case -10:
                anm_init(i_this, BCK_RD_WAIT01, 7.0f, 2, 1.0f);
                i_this->timer[1] = cM_rndF(10.0f) + 15.0f;
                i_this->mode = 0xFFF7;
                break;

            case -9:
                if (i_this->timer[1] == 0) {
                    i_this->mode = 0;
                }
                break;

            case 0:
                if (i_this->field_0x5bd != 0) {
                    anm_init(i_this, BCK_RD_ARMAMENT, 2.0f, 0, 1.0f);
                    i_this->mode = 0xFFEC;
                } else if (i_this->timer[1] == 0) {
                    anm_init(i_this, BCK_RD_RUN, 5.0f, 2, cM_rndFX(0.05f) + 1.0f);
                    i_this->mode = 1;
                }
                break;

            case 1:
                speed = dash_speed;
                if (i_this->dis < l_HIO.battle_init_range) {
                    anm_init(i_this, BCK_RD_KAMAE, 5.0f, 2, 1.0f);
                    i_this->mode = 2;
                }
                break;

            case 2:
                speed = l_HIO.movement_speed;
                if (i_this->dis > l_HIO.battle_init_range + 50.0f) {
                    i_this->mode = 0;
                } else if (i_this->dis < l_HIO.battle_init_range - 50.0f) {
                    i_this->mode = 3;
                }
                break;

            case 3:
                speed = -l_HIO.movement_speed;
                if (i_this->dis > l_HIO.battle_init_range) {
                    i_this->mode = 2;
                }
                break;

            case 5:
                if (i_this->timer[3] != 0) {
                    i_this->field_0x9c8 = 0xFF;
                }

                speed = 0.0f;
                enemy->speedF = 0.0f;
                if (!move_gake_check(i_this, 100.0f)) {
                    i_this->mode = 2;
                }
                break;
        }

        if (i_this->mode < 5 && move_gake_check(i_this, 100.0f)) {
            i_this->mode = 5;
            i_this->timer[3] = cM_rndF(15.0f) + 30.0f;
            anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
        }

        if (i_this->mode >= 0) {
            cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->angleY, 4, 0x800);
            s16 range = enemy->current.angle.y - i_this->angleY;
            if (range > 0x400 || range < -0x400) {
                speed = 0.0f;
            }
        }

        cLib_addCalc2(&enemy->speedF, speed, 1.0f, 5.0f);

        if (!attack_flag) {
            i_this->action = ACTION_NORMAL;
            if (i_this->mode == 5) {
                i_this->target_ya = enemy->current.angle.y + 0x8000 + (s16)cM_rndFX(4000.0f);
                anm_init(i_this, BCK_RD_WALK, 10.0f, 2, 1.0f);
                i_this->mode = 3;
                i_this->timer[0] = cM_rndF(100.0f) + 100.0f;
                i_this->timer[1] = 40;
            } else {
                i_this->mode = 0;
                i_this->timer[0] = cM_rndF(50.0f) + 50.0f;
                anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
            }
        } else {
            if (i_this->dis < l_HIO.battle_init_range && (i_this->field_0xa20 & cc_pl_cut_bit_get()) != 0 &&
                i_this->field_0x9f0 < 0.1f) {
                i_this->action = ACTION_AVOID;
                i_this->mode = 0;
                i_this->damage_timer = 20;
            } else if (i_this->field_0x5bd == 0 && i_this->dis < l_HIO.attack_init_range && i_this->timer[2] == 0) {
                i_this->timer[2] = cM_rndF(20.0f) + 20.0f;
                f32 rnd = 0.5f;
                if (cM_rndF(1.0f) < rnd) {
                    i_this->action = ACTION_FIGHT;
                    i_this->mode = 0;
                }
            }
        }

        if ((i_this->counter & 15) == 0) {
            wb_check(i_this, 0x3000);
            if (bomb_view_check(i_this) != NULL) {
                i_this->action = ACTION_BOMB;
                i_this->mode = 0;
            }
        }

        if (i_this->action == ACTION_FIGHT_RUN && i_this->field_0x9c8 >= 0) {
            i_this->field_0x9c8 = 1;
        }
    }
}

static fopAc_ac_c* at_hit_check(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = &i_this->enemy;
    fopAc_ac_c* actor;
    if (i_this->mode >= 10) {
        return NULL;
    }

    if (i_this->at_sph.ChkAtHit()) {
        cCcD_Obj* at_hit_obj_p = i_this->at_sph.GetAtHitObj();
        actor = dCc_GetAc(at_hit_obj_p->GetAc());
        return actor;
    }

    return NULL;
}

static void e_rd_fight(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz sp3c, sp48;
    int frame = i_this->anm_p->getFrame();

    switch (i_this->mode) {
        case 0: {
            anm_init(i_this, BCK_RD_ATTACK01, TREG_F(14) + 6.0f, 0, 1.0f);

            if (l_HIO.field_0x38 == 0) {
                i_this->timer[0] = 25;
            } else if (l_HIO.field_0x38 == 1) {
                // in practice, only goes in here
                i_this->timer[0] = 15;
            } else if (l_HIO.field_0x38 == 2) {
                i_this->timer[0] = 10;
            }

            i_this->mode = 1;
            i_this->timer[1] = 5;
            i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_WAND_ATTACK, -1);
            break;
        }

        case 1: {
            if (i_this->timer[1] != 0) {
                cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->angleY, 4, 0x800);
            }

            if (frame == 30) {
                i_this->anm_p->setFrame(0.0f);
            } else if (i_this->timer[0] == 0) {
                anm_init(i_this, BCK_RD_ATTACK01, 2.0f, 0, l_HIO.swing_speed);
                i_this->anm_p->setFrame(30.0f);
                i_this->mode = 2;
            }
            break;
        }

        case 2: {
            if (frame >= 44 && frame <= 47) {
                i_this->field_0x9ab = 1;
            }

            if (i_this->anm_p->checkFrame(44.0f)) {
                i_this->sound.startCreatureSound(Z2SE_EN_RD_SWING_CLUB, 0, -1);
            }

            if (i_this->anm_p->checkFrame(50.0f)) {
                i_this->anm_p->setPlaySpeed(1.0f);
            }

            int iVar1;
            if (l_HIO.field_0x38 == 0) {
                iVar1 = 64;
            } else if (l_HIO.field_0x38 == 1) {
                // in practice, only goes in here
                iVar1 = 54;
            } else if (l_HIO.field_0x38 == 2) {
                iVar1 = 44;
            }

            if (frame >= iVar1) {
                i_this->mode = 3;
                if (l_HIO.field_0x38 == 0) {
                    i_this->timer[0] = 20;
                } else if (l_HIO.field_0x38 == 1) {
                    // in practice, only goes in here
                    i_this->timer[0] = 15;
                } else if (l_HIO.field_0x38 == 2) {
                    i_this->timer[0] = 10;
                }

                anm_init(i_this, BCK_RD_KAMAE, 5.0f, 2, 1.0f);
            }
        }
        // fallthrough intentional
        default:
            break;

        case 3:
        case 10: {
            if (i_this->timer[0] == 0) {
                i_this->action = ACTION_FIGHT_RUN;
                i_this->mode = 0;
            }
        }
    }

    cLib_addCalc0(&enemy->speedF, 1.0f, 3.0f);
    if (i_this->field_0x9ab != 0) {
        fopAc_ac_c* actor = at_hit_check(i_this);
        if (actor != NULL && fopAcM_GetName(actor) == PROC_ALINK) {
            if (daPy_getPlayerActorClass()->checkPlayerGuard()) {
                i_this->anm_p->setPlaySpeed(-1.0f);
                dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
                dKy_Sound_set(enemy->current.pos, 100, fopAcM_GetID(i_this), 5);
                i_this->mode = 10;
                i_this->timer[0] = 10;
            }
        }
    }
}

static void e_rd_bow_run(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz unused_vec_0, unused_vec_1;
    f32 speed = 0.0f;
    f32 dash_speed;
    if (desert_substage != 0) {
        dash_speed = 24.0f;
    } else {
        dash_speed = l_HIO.dash_speed;
    }
    s16 target_angle = fopAcM_searchPlayerAngleY(enemy);
    s8 bVar1 = 0;

    switch (i_this->mode) {
        case -20:
            if ((int)i_this->anm_p->getFrame() == 11) {
                i_this->field_0x5bd = 0;
            }

            if (i_this->anm_p->isStop()) {
                i_this->mode = 0;
            }
            break;

        case -10:
            anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
            i_this->timer[0] = cM_rndF(10.0f) + 15.0f;
            i_this->mode = 0xFFF7;
            break;

        case -9:
            i_this->field_0x9c8 = 1;
            if (i_this->timer[0] == 0) {
                i_this->mode = 0;
            }
            break;

        case 0:
            if (i_this->field_0x5bd != 0) {
                anm_init(i_this, BCK_RD_ARMAMENT, 2.0f, 0, 1.0f);
                i_this->mode = 0xFFEC;
            } else {
                anm_init(i_this, BCK_RD_RUN, 5.0f, 2, 1.0f);
                i_this->timer[0] = cM_rndF(50.0f) + 20.0f;
                if (i_this->dis < l_HIO.attack_range - 100.0f) {
                    i_this->mode = 1;
                } else if (i_this->dis > l_HIO.attack_range + 100.0f) {
                    i_this->mode = 2;
                } else {
                    bVar1 = 1;
                }
            }
            break;

        case 1:
            speed = dash_speed;
            ANGLE_ADD_2(target_angle, 0x8000);
            if (i_this->dis > l_HIO.attack_range || i_this->timer[0] == 0 || i_this->ObjAcch.ChkWallHit() || move_gake_check(i_this, 100.0f)) {
                bVar1 = 1;
            }

            i_this->timer[2] = 20;
            break;

        case 2:
            speed = dash_speed;
            if (i_this->dis < l_HIO.attack_range) {
                bVar1 = 1;
            }
            break;
    }

    cLib_addCalc2(&enemy->speedF, speed, 1.0f, 5.0f);
    if (i_this->mode >= 0) {
        cLib_addCalcAngleS2(&enemy->current.angle.y, target_angle, 4, 0x1000);
    }

    if (i_this->field_0x5bd == 0 && bVar1) {
        if (i_this->arg2 == 3) {
            i_this->action = ACTION_BOW2;
        } else {
            i_this->action = ACTION_BOW;
        }

        i_this->mode = 0;
        i_this->timer[1] = cM_rndF(150.0f);
    } else if (i_this->timer[2] == 0 && !pl_check(i_this, i_this->attack_range + 50.0f, 0x7FFF)) {
        i_this->action = ACTION_NORMAL;
        i_this->mode = 0;
        i_this->timer[0] = cM_rndF(50.0f) + 50.0f;
        anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
    } else if ((i_this->counter & 15) == 0) {
        wb_check(i_this, 0x3000);
        if (bomb_view_check(i_this) != NULL) {
            i_this->action = ACTION_BOMB;
            i_this->mode = 0;
        }
    }

    if (i_this->action != ACTION_BOW_RUN) {
        i_this->field_0x9c8 = 0;
    }
}

static void e_rd_bow(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz unused_vec_0, unused_vec_1;
    int frame = i_this->anm_p->getFrame();

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, BCK_RD_SHOOT_READY, 5.0f, 0, 1.0f);
            i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 7),
                                         0, 5.0f, 0.9f + TREG_F(3), 0.0f, -1.0f);
            i_this->mode = 1;
            enemy->speedF = 0.0f;
            break;

        case 1:
            i_this->field_0x9c8 = 4;
            if (frame >= 10) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 20) {
                i_this->sound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
            }

            if (i_this->anm_p->isStop()) {
                anm_init(i_this, BCK_RD_SHOOT_WAIT, 4.0f, 2, 1.0f);
                i_this->timer[0] = cM_rndF(10.0f) + 10.0f;
                i_this->mode = 2;
            }
            break;

        case 2:
            i_this->field_0x9c8 = 4;
            if (i_this->timer[0] == 0) {
                if (i_this->field_0x9a4 == 0 && !dMsgObject_isTalkNowCheck()) {
                    anm_init(i_this, BCK_RD_SHOOT, 1.0f, 0, 1.0f);
                    i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 10), 0, 1.0f, 1.0f, 0.0f, -1.0f);
                    i_this->mode = 3;
                } else {
                    i_this->action = ACTION_BOW_RUN;
                    i_this->mode = 0;
                }
            }

            i_this->field_0x9a2 = 1;
            break;

        case 3:
            if (frame <= 3 + XREG_S(5)) {
                i_this->field_0x9a2 = 1;
                i_this->field_0x9c8 = 4;
            }

            if (frame == 2 + XREG_S(7)) {
                i_this->arrow_flag = 1;
            }

            if (i_this->anm_p->isStop()) {
                if (i_this->timer[1] == 0 && (i_this->dis < l_HIO.attack_range - 200.0f || i_this->dis > l_HIO.attack_range)) {
                    if (!(i_this->dis < l_HIO.attack_range - 200.0f) || !way_bg_check(i_this, -100.0f)) {
                        i_this->action = ACTION_BOW_RUN;
                    }

                    i_this->mode = 0;
                } else {
                    i_this->mode = 0;
                }
            }
            break;
    }

    cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->angleY, 4, 0x1000);
}

static void* s_command3_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && i_actor != i_data && ((e_rd_class*)i_actor)->action == ACTION_COMMANDER) {
        return i_actor;
    }

    return NULL;
}

static s8 e_rd_bow2(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    fopAc_ac_c* actor = get_pla(enemy);
    cXyz unused_vec_0, unused_vec_1;
    int frame = i_this->anm_p->getFrame();
    s8 rt = 0;

    if (i_this->mode <= 1) {
        if (i_this->dis < i_this->attack_range && !other_bg_check(i_this, actor)) {
            i_this->mode = 2;
        }

        rt = 1;
    }

    switch (i_this->mode) {
        case 0:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
                i_this->mode = 1;
                i_this->timer[0] = cM_rndF(40.0f) + 60.0f;
            }
            break;

        case 1:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
                i_this->timer[0] = cM_rndF(100.0f) + 100.0f;
                i_this->mode = 0;
            }
            break;

        case 2:
            anm_init(i_this, BCK_RD_SHOOT_READY, 5.0f, 0, 1.0f);
            i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 7),
                                         0, 5.0f, 0.9f + TREG_F(3), 0.0f, -1.0f);
            i_this->mode = 3;
            enemy->speedF = 0.0f;
            break;

        case 3:
            i_this->field_0x9c8 = 4;
            if (frame >= 10) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 20) {
                i_this->sound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
            }

            if (i_this->anm_p->isStop()) {
                anm_init(i_this, BCK_RD_SHOOT_WAIT, 4.0f, 2, 1.0f);
                i_this->timer[0] = cM_rndF(10.0f);
                i_this->mode = 4;
            }
            break;

        case 4:
            i_this->field_0x9c8 = 4;
            if (i_this->timer[0] == 0) {
                if (i_this->dis > i_this->attack_range || other_bg_check(i_this, actor)) {
                    i_this->mode = 0;
                    return rt;
                }

                if (!dMsgObject_isTalkNowCheck() && i_this->field_0x9a4 == 0) {
                    anm_init(i_this, BCK_RD_SHOOT, 1.0f, 0, 1.0f);
                    i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 10), 0, 1.0f, 1.0f, 0.0f, -1.0f);
                    i_this->mode = 5;
                }
            }

            i_this->field_0x9a2 = 1;
            break;

        case 5:
            if (frame <= 3 + XREG_S(5)) {
                i_this->field_0x9a2 = 1;
                i_this->field_0x9c8 = 4;
            }

            if (frame == 2 + XREG_S(7)) {
                i_this->arrow_flag = 1;
            }

            if (i_this->anm_p->isStop()) {
                if (i_this->timer[1] == 0 && fabsf(enemy->current.pos.y - actor->current.pos.y) < 300.0f && i_this->dis < l_HIO.attack_range - 200.0f) {
                    if (!way_bg_check(i_this, -100.0f)) {
                        i_this->action = ACTION_BOW_RUN;
                        i_this->one_hit_kill = 0;
                    }

                    i_this->mode = 0;
                } else {
                    i_this->mode = 0;
                }
            }
            break;
    }

    cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->angleY, 4, 0x1000);
    return rt;
}

static void e_rd_bow_ikki(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz vec_0x24, vec_0x18;
    int frame = i_this->anm_p->getFrame();

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, BCK_RD_SHOOT_READY, 5.0f, 0, 1.0f);
            i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 7),
                                         0, 5.0f, 0.9f + TREG_F(3), 0.0f, -1.0f);
            i_this->mode = 1;
            enemy->speedF = 0.0f;
            break;

        case 1:
            if (frame >= 10) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 20) {
                i_this->sound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
            }

            if (i_this->anm_p->isStop()) {
                anm_init(i_this, BCK_RD_SHOOT_WAIT, 4.0f, 2, 1.0f);
                i_this->mode = 2;
            }
            break;

        case 2:
            if (i_this->timer[0] == 0 && i_this->field_0x9a4 == 0) {
                anm_init(i_this, BCK_RD_SHOOT, 1.0f, 0, 1.0f);
                i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 10), 0, 1.0f, 1.0f, 0.0f, -1.0f);
                i_this->mode = 3;
            }

            i_this->field_0x9a2 = 1;
            break;

        case 3:
            if (frame <= 3 + XREG_S(5)) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 2 + XREG_S(7)) {
                i_this->arrow_flag = 1;
            }

            if (i_this->anm_p->isStop()) {
                i_this->mode = 0;
                i_this->timer[0] = 10000;
            }
            break;
    }

    i_this->field_0x9c8 = 14;
    i_this->eye.set(34800.0f, 100.0f, -14900.0f);
    vec_0x24 = i_this->eye - enemy->current.pos;
    enemy->current.angle.y = cM_atan2s(vec_0x24.x, vec_0x24.z);
}

static void e_rd_avoid(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    int frame = i_this->anm_p->getFrame();

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, BCK_RD_AVOID, 1.0f, 0, 1.0f);
            i_this->mode = 1;
            enemy->speedF = -30.0f + XREG_F(1);
            enemy->speed.y = 20.0f + XREG_F(2);
            i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_DODGE, -1);
            break;

        case 1:
            if (frame == 6) {
                i_this->anm_p->setPlaySpeed(0.0f);
                i_this->mode = 2;
            }
            break;

        case 2:
            if (i_this->ObjAcch.ChkGroundHit()) {
                i_this->anm_p->setPlaySpeed(1.0f);
                i_this->mode = 3;
            }
            break;

        case 3:
            cLib_addCalc0(&enemy->speedF, 1.0f, 10.0f);
            if (i_this->anm_p->isStop()) {
                i_this->action = ACTION_FIGHT_RUN;
                i_this->mode = 0;
                i_this->timer[0] = 30;
            }
    }
}

static void e_rd_wb_search(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz mae, ato;
    f32 speed = 0.0f;
    s8 bVar1 = 0;
    fopAc_ac_c* actor = fopAcM_SearchByID(i_this->boar_id);
    e_wb_class* boar = (e_wb_class*)actor;
    if (actor == NULL) {
        bVar1 = 1;
    } else {
        if (boar->mActionID == 22 || boar->mActionID == 21 || boar->mActionID == 23 || boar->mActionID == 24) {
            bVar1 = 1;
        } else if (i_this->mode <= 3) {
            for (int i = 0; i <= 2; i++) {
                if (i_this->cc_sph[i].ChkCoHit()) {
                    cCcD_Obj* co_hit_obj_p = i_this->cc_sph[i].GetCoHitObj();
                    fopAc_ac_c* hit_actor = dCc_GetAc(co_hit_obj_p->GetAc());
                    if (hit_actor != NULL && hit_actor != actor && fopAcM_GetName(hit_actor) == PROC_E_WB) {
                        bVar1 = 1;
                    }
                }
            }

            if (i_this->ObjAcch.ChkWallHit()) {
                bVar1 = 1;
            }
        }
    }

    if (bVar1) {
        i_this->action = ACTION_NORMAL;
        i_this->mode = 0;
        ride_off(i_this);
    } else {
        if (i_this->mode >= 2 && boar->mActionID != 1) {
            boar->mActionID = 1;
            boar->mActionMode = 0;
        }

        switch (i_this->mode) {
            case 0:
                anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
                i_this->mode = 1;
                i_this->timer[0] = cM_rndF(15.0f) + 15.0f;
                // fallthrough
            case 1:
                i_this->field_0x9c8 = 11;
                if (i_this->timer[0] == 0) {
                    i_this->mode = 2;
                    if (actor->speedF > 5.0f) {
                        anm_init(i_this, BCK_RD_SIGN, 3.0f, 2, 1.0f);
                        i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_CALLING, -1);
                        i_this->timer[0] = cM_rndF(20.0f) + 30.0f;
                    } else {
                        i_this->timer[0] = 10;
                    }
                }
                break;

            case 2:
                mae = actor->current.pos - enemy->current.pos;
                cLib_addCalcAngleS2(&enemy->current.angle.y, cM_atan2s(mae.x, mae.z), 4, 0x800);
                i_this->field_0x9c8 = 11;

                if (i_this->timer[0] == 0 && actor->speedF < 1.0f) {
                    cXyz sp6c, sp78;
                    cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
                    mae.x = 100.0f;
                    mae.y = 0.0f;
                    if (i_this->boar_stand == 1) {
                        mae.z = TREG_F(0) + 20.0f;
                    } else {
                        mae.z = TREG_F(10) + -120.0f;
                    }
                    MtxPosition(&mae, &ato);

                    sp6c = (actor->current.pos + ato) - enemy->current.pos;
                    mae.x = -100.0f;
                    MtxPosition(&mae, &ato);

                    sp78 = (actor->current.pos + ato) - enemy->current.pos;
                    if (sp6c.abs() < sp78.abs()) {
                        i_this->field_0x9bd = 0;
                        ato = sp6c + enemy->current.pos;
                    } else {
                        i_this->field_0x9bd = 1;
                        ato = sp78 + enemy->current.pos;
                    }

                    mae = ato - enemy->current.pos;
                    i_this->target_ya = (s16) cM_atan2s(mae.x, mae.z);
                    i_this->mode = 3;
                    anm_init(i_this, BCK_RD_RUN, 5.0f, 2, 1.5f);
                }
                break;

            case 3:
                speed = l_HIO.dash_speed * 1.5f;
                cMtx_YrotS(*calc_mtx, actor->shape_angle.y);

                if (i_this->field_0x9bd == 0) {
                    mae.x = 100.0f;
                } else {
                    mae.x = -100.0f;
                }
                mae.y = 0.0f;
                if (i_this->boar_stand == 1) {
                    mae.z = TREG_F(0) + 20.0f;
                } else {
                    mae.z = TREG_F(10) + -120.0f;
                }
                MtxPosition(&mae, &ato);

                ato += actor->current.pos;
                mae = ato - enemy->current.pos;
                i_this->target_ya = (s16) cM_atan2s(mae.x, mae.z);
                cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->target_ya, 2, 0x400);
                mae = ato - enemy->current.pos;
                mae.y = 0.0f;

                if (mae.abs() < TREG_F(19) + 230.0f) {
                    i_this->mode = 4;
                    if (i_this->boar_stand == 1) {
                        anm_init(i_this, BCK_RD_ARMAMENT, 2.0f, 0, 1.0f);
                    } else {
                        anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
                        i_this->timer[0] = 10;
                    }
                }
                break;

            case 4:
                cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
                mae.x = 0.0f;
                mae.y = 0.0f;
                if (i_this->boar_stand == 1) {
                    mae.z = 20.0f;
                } else {
                    mae.z = -20.0f;
                }
                MtxPosition(&mae, &i_this->field_0x5c0);

                i_this->field_0x5c0 += actor->current.pos;
                mae = i_this->field_0x5c0 - enemy->current.pos;
                cLib_addCalcAngleS2(&enemy->current.angle.y, (s16)cM_atan2s(mae.x, mae.z), 2, 0x1000);

                if (i_this->weapon_type != 0 && i_this->boar_stand == 1) {
                    if ((int)i_this->anm_p->getFrame() == 11) {
                        i_this->field_0x5bd = 1;
                    }

                    if (i_this->anm_p->isStop()) {
                        i_this->mode = 5;
                        anm_init(i_this, BCK_RD_JUMP_A, 2.0f, 0, 1.0f);
                        i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_JUMP_RIDING, -1);
                    }
                } else if (i_this->timer[0] == 0) {
                    i_this->mode = 5;
                    anm_init(i_this, BCK_RD_JUMP_A, 2.0f, 0, 1.0f);
                    i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_JUMP_RIDING, -1);
                }
                break;

            case 5:
                if (i_this->anm_p->isStop()) {
                    i_this->mode = 6;
                    anm_init(i_this, BCK_RD_JUMP_B, 10.0f, 0, 1.0f);
                    i_this->ride_mode = 1;
                    i_this->field_0x9c4 = 30.0f;
                    i_this->timer[3] = 20;
                }
                break;

            case 6:
                if (i_this->field_0x9c0 <= 0.0f) {
                    i_this->action = ACTION_WB_RIDE;
                    i_this->mode = 0;
                    boar->mActionID = 4;
                    boar->mActionMode = 0;
                }
        }

        cLib_addCalc2(&enemy->speedF, speed, 1.0f, 3.0f);
        i_this->eye = actor->eyePos;
    }
}

static void e_rd_wb_ride(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz sp1c, sp28;
    fopAc_ac_c* actor = fopAcM_SearchByID(i_this->boar_id);

    if (actor == NULL) {
        i_this->action = ACTION_NORMAL;
        i_this->mode = 0;
        ride_off(i_this);
        return;
    }

    e_wb_class* boar = (e_wb_class*) actor;
    if ((boar->mStatusFlags & 3) == 0) {
        OS_REPORT("......BOSS DL !!!!\n");
        if (boar->mActionID == 1) {
            boar->mActionID = 0;
        }
        fopAcM_delete(enemy);
        return;
    }

    switch (i_this->mode) {
        case 0: {
            i_this->mode = 1;
            if (i_this->actor_set == 0) {
                anm_init(i_this, BCK_RD_JUMP_C, 1.0f, 0, 1.0f);
            }
        }
        // fallthrough
        case 1: {
            if (i_this->anm_p->isStop() || i_this->actor_set != 0) {
                if (i_this->actor_set != 0) {
                    boar->mActionMode = 0;
                    i_this->mode = 0;

                    if (i_this->actor_set == 3) {
                        boar->mActionID = 17;
                        // Hyrule Field
                        if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0) {
                            boar->mActionMode = -100;
                            i_this->action = ACTION_IKKI2_START;
                        } else {
                            i_this->action = ACTION_WB_RUN;
                        }
                    } else if (i_this->actor_set == 2) {
                        boar->mActionID = 15;
                        i_this->action = ACTION_WB_RUN;
                    } else if (i_this->actor_set == 4) {
                        boar->mActionID = 19;
                        i_this->action = ACTION_LV9_END;
                    } else if (cDmr_SkipInfo == 0) {
                        i_this->action = ACTION_KIBA_START;
                        boar->mActionID = 30;
                    } else {
                        boar->mActionID = 8;
                        i_this->action = ACTION_WB_RUN;
                        boar->field_0x79c = 10;
                        Z2GetAudioMgr()->subBgmStart(Z2BGM_HORSE_BATTLE);
                    }
                } else {
                    boar->mActionID = 6;
                    boar->mActionMode = 0;
                    i_this->action = ACTION_WB_RUN;
                    i_this->mode = 0;
                }
            }
        }
    }
}

static void e_rd_wb_run(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz sp3c, sp48;
    fopAc_ac_c* actor = (fopAc_ac_c*) fopAcM_SearchByID(i_this->boar_id);

    if (actor == NULL) {
        fopAcM_delete(enemy);
        return;
    }

    e_wb_class* boar = (e_wb_class*) actor;
    if (boar->field_0x17e1) {
        fopAcM_setStageLayer(enemy);
    }

    if ((boar->mStatusFlags & 3) == 0) {
        fopAcM_delete(enemy);
        return;
    }

    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        dAttention_c* attention = dComIfGp_getAttention();
        if (attention->Lockon() && enemy == attention->LockonTarget(0)) {
            boar->field_0x6c0 = 1;
        }
    }

    if ((boar->mStatusFlags & 1) == 0 && boar->mActionID == ACTION_BOW2 && i_this->mode != 40) {
        anm_init(i_this, BCK_RD_RRUN02_BACK, 5.0f, 2, 1.0f);
        i_this->mode = 40;

        if (i_this->bow_anm != NULL) {
            i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, BCK_RD_BOW_SHOOT),
                                         0, 1.0f, 1.0f, 0.0f, -1.0f);
        }
    }

    f32 player_dist = fopAcM_searchPlayerDistanceXZ(enemy);
    int frame = i_this->anm_p->getFrame();

    switch (i_this->mode) {
        case 0:
            if ((boar->mStatusFlags & 4) != 0) {
                if (i_this->boar_stand == 1) {
                    if (boar->field_0x6d0 < 0) {
                        anm_init(i_this, BCK_RD_RSTEP_L, 10.0f, 2, 1.0f);
                    } else {
                        anm_init(i_this, BCK_RD_RSTEP_R, 10.0f, 2, 1.0f);
                    }
                } else {
                    anm_init(i_this, BCK_RD_RSTEP_BACK, 10.0f, 2, 1.0f);
                }
            } else {
                if ((boar->mStatusFlags & 8) != 0) {
                    if (i_this->boar_stand == 1) {
                        anm_init(i_this, BCK_RD_RDAMAGE, 3.0f, 0, 1.0f);
                    } else {
                        anm_init(i_this, BCK_RD_RDAMAGE_BACK, 3.0f, 0, 1.0f);
                    }

                    i_this->timer[1] = cM_rndF(50.0f) + 50.0f;
                } else {
                    if ((boar->mStatusFlags & 16) != 0) {
                        if (i_this->boar_stand == 1) {
                            anm_init(i_this, BCK_RD_RRUN, 5.0f, 2, 1.0f);
                        } else {
                            anm_init(i_this, BCK_RD_RRUN_BACK, 5.0f, 2, 1.0f);
                        }
                    } else {
                        if ((boar->mStatusFlags & 32) != 0) {
                            if (i_this->boar_stand == 1) {
                                anm_init(i_this, BCK_RD_RWAIT, 5.0f, 2, 1.0f);
                            } else {
                                anm_init(i_this, BCK_RD_RWAIT_BACK, 5.0f, 2, 1.0f);
                            }
                        }
                    }
                }
            }

            i_this->anm_p->setPlaySpeed(boar->mpModelMorf->getPlaySpeed());

            if (i_this->boar_stand == 1) {
                i_this->field_0x9c8 = 2;
            } else {
                i_this->field_0x9c8 = 1;
                if (i_this->weapon_type == 1 && actor->speedF > 10.0f) {
                    if (player_dist < TREG_F(11) + 850.0f && player_dist < TREG_F(11) + 750.0f
                        && i_this->timer[2] == 0) {
                        s16 range = enemy->shape_angle.y - i_this->angleY;
                        if (range > 0x1000 && range < 0x4000) {
                            i_this->mode = 10;
                        } else if (range < -0x1000 && range > -0x4000) {
                            i_this->mode = 20;
                        }

                        if (daPy_getPlayerActorClass()->checkHorseRide() && dComIfGp_getHorseActor()->speedF >= 20.0f) {
                            i_this->timer[2] = cM_rndF(20.0f) + 20.0f;
                        }
                    }
                } else {
                    if (i_this->timer[1] == 0 && i_this->weapon_type >= 2) {
                        anm_init(i_this, BCK_RD_RSHOOT_READY, 5.0f, 0, 1.0f);
                        i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, BCK_RD_BOW_RREADY), 0, 5.0f, 1.0f, 0.0f, -1.0f);
                        i_this->mode = 30;
                    }
                }
            }
            break;

        case 10:
            anm_init(i_this, BCK_RD_RATTACK01_WAIT, 10.0f, 2, 1.0f);
            i_this->mode = 11;
            // fallthrough
        case 11:
            if (player_dist > 800.0f + TREG_F(10)) {
                i_this->mode = 0;
                if (i_this->boar_stand == 1) {
                    anm_init(i_this, BCK_RD_RRUN, 10.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_RRUN_BACK, 10.0f, 2, 1.0f);
                }
            } else if (player_dist < 550.0f && i_this->timer[2] == 0) {
                i_this->mode = 25;
                if (daPy_getPlayerActorClass()->checkHorseRide()) {
                    anm_init(i_this, BCK_RD_RATTACK03, 5.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_RATTACK01, 5.0f, 0, 1.0f);
                }
            }
            break;

        case 20:
            anm_init(i_this, BCK_RD_RATTACK02_WAIT, 10.0f, 2, 1.0f);
            i_this->mode = 21;
            // fallthrough
        case 21:
            i_this->field_0x9ab = 1;
            if (player_dist > 800.0f + TREG_F(10)) {
                i_this->mode = 0;
                if (i_this->boar_stand == 1) {
                    anm_init(i_this, BCK_RD_RRUN, 10.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_RRUN_BACK, 10.0f, 2, 1.0f);
                }
            } else if (player_dist < 550.0f && i_this->timer[2] == 0) {
                i_this->mode = 25;
                if (daPy_getPlayerActorClass()->checkHorseRide()) {
                    anm_init(i_this, BCK_RD_RATTACK04, 5.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_RATTACK02, 5.0f, 0, 1.0f);
                }
            }
            break;

        case 25:
            if (frame <= 23) {
                i_this->field_0x9ab = 1;
            }

            if (i_this->anm_p->checkFrame(15.0f)) {
                i_this->sound.startCreatureSound(Z2SE_EN_RD_SWING_CLUB, 0, -1);
            }

            if (i_this->anm_p->isStop()) {
                if (i_this->boar_stand == 1) {
                    anm_init(i_this, BCK_RD_RRUN, 10.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_RRUN_BACK, 10.0f, 2, 1.0f);
                    i_this->timer[2] = cM_rndF(20.0f) + 20.0f;
                }

                i_this->mode = 0;
            }
            break;

        case 30:
            i_this->field_0x9c8 = 3;
            if (frame == 14) {
                i_this->sound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
            }

            if (i_this->anm_p->isStop()) {
                i_this->mode = 31;
                anm_init(i_this, BCK_RD_RSHOOT_WAIT, 4.0f, 2, 1.0f);
            }

            i_this->field_0x9a2 = 1;
            break;

        case 31:
            i_this->field_0x9c8 = 3;
            if (i_this->timer[1] == 0 && i_this->dis < l_HIO.mounted_launch_distance
                && i_this->field_0x9a4 == 0 && !dComIfGp_event_runCheck()) {
                anm_init(i_this, BCK_RD_RSHOOT, 1.0f, 0, 1.0f);
                i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 10),
                                                0, 1.0f, 1.0f, 0.0f, -1.0f);
                i_this->mode = 32;
            }

            i_this->field_0x9a2 = 1;
            break;

        case 32:
            if (frame <= 3 + XREG_S(5)) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 2 + XREG_S(7)) {
                i_this->arrow_flag = 1;
            }

            i_this->field_0x9c8 = 3;

            if (i_this->anm_p->isStop()) {
                anm_init(i_this, BCK_RD_RSHOOT_READY, 5.0f, 0, 1.0f);
                i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 8),
                                                0, 5.0f, 1.0f, 0.0f, -1.0f);
                i_this->mode = 30;

                if (i_this->ride_mode == 2 && i_this->field_0x5bb != 0) {
                    i_this->timer[1] = cM_rndF(30.0f) + 20.0f;
                } else {
                    i_this->timer[1] = cM_rndF(50.0f) + 30.0f;
                }
            }
            break;

        case 40:
            if (boar->mActionID != 7) {
                i_this->mode = 0;
                anm_init(i_this, BCK_RD_RWAIT_BACK, 5.0f, 2, 1.0f);
            }
            break;

        case 50:
            if (i_this->timer[3] == 0) {
                i_this->mode = 0;
                if (i_this->boar_stand == 1) {
                    anm_init(i_this, BCK_RD_RRUN, 10.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_RRUN_BACK, 10.0f, 2, 1.0f);
                    i_this->timer[2] = cM_rndF(50.0f) + 50.0f;
                }
            }
    }

    if (i_this->boar_stand == 2 && i_this->anm == BCK_RD_RRUN_BACK) {
        i_this->sound.startCreatureVoiceLevel(Z2SE_EN_RD_V_RIDING_YELL, -1);
    }
}

static void* s_wbrun_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_WB) {
        e_wb_class* boar = (e_wb_class*)i_actor;
        fopAc_ac_c* actor = (fopAc_ac_c*)i_data;
        if (boar->field_0x79d == 0) {
            cXyz sp24(boar->mEnemy.current.pos - actor->current.pos);
            if (sp24.abs() > KREG_F(11) + 7000.0f) {
                return i_actor;
            }
        }
    }

    return NULL;
}

static void e_rd_wb_run_B(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz unused_vec_0, unused_vec_1;
    fopAc_ac_c* actor = fopAcM_SearchByID(i_this->boar_id);
    dAttention_c* attention;

    if (actor != NULL) {
        if (i_this->actor_set == 2 || i_this->actor_set == 3) {
            fopAcM_OffStatus(enemy, 0);
            enemy->attention_info.flags = 0;
        }

        e_wb_class* boar = (e_wb_class*) actor;
        if (daPy_getPlayerActorClass()->checkHorseRide()) {
            attention = dComIfGp_getAttention();
            if (attention->Lockon() && enemy == attention->LockonTarget(0)) {
                boar->field_0x6c0 = 1;
            }
        }

        switch (i_this->mode) {
            case 0: {
                if ((boar->mStatusFlags & 0x400) != 0) {
                    anm_init(i_this, e_rdb_class::BCK_RB_RSTARTLOOP, 10.0f, 2, 1.0f);
                } else if ((boar->mStatusFlags & 0x800) != 0) {
                    anm_init(i_this, e_rdb_class::BCK_RB_RSTART, 10.0f, 2, 1.0f);
                }

                i_this->anm_p->setPlaySpeed(boar->mpModelMorf->getPlaySpeed());
                break;
            }
        }

        if (i_this->anm == e_rdb_class::BCK_RB_RGUARD_F || i_this->anm == e_rdb_class::BCK_RB_RDAMAGEB 
            || i_this->anm == e_rdb_class::BCK_RB_RGUARD_L || i_this->anm == e_rdb_class::BCK_RB_RGUARD_R) {
            i_this->field_0x9c8 = 0;

            if (i_this->anm_p->isStop()) {
                anm_init(i_this, e_rdb_class::BCK_RB_RRUN, 5.0f, 2, 1.0f);
            }
        } else if (i_this->anm == e_rdb_class::BCK_RB_RPLAY_RUNR || i_this->anm == e_rdb_class::BCK_RB_RPLAY_RUNL 
                   || i_this->anm == e_rdb_class::BCK_RB_RPLAY_STOP) {
            if ((int)i_this->anm_p->getFrame() == 21) {
                i_this->sound.startCreatureVoice(Z2SE_EN_RDB_HORN, -1);
            }
        } else if (i_this->anm == e_rdb_class::BCK_RB_RRUN) {
            if (i_this->timer[1] == 0) {
                i_this->timer[1] = cM_rndF(100.0f) + 70.0f;
                i_this->timer[2] = cM_rndF(30.0f) + 30.0f;
            }

            if (i_this->timer[2] != 0) {
                i_this->field_0x9c8 = 1;
            }

            if (enemy->speedF < 5.0f) {
                anm_init(i_this, e_rdb_class::BCK_RB_RWAIT, 5.0f, 2, 1.0f);
            }
        } else {
            i_this->field_0x9c8 = 1;
            if (i_this->timer[3] < 200) {
                i_this->timer[3] = cM_rndF(100.0f) + 200.0f;
            }

            if (enemy->speedF > 10.0f) {
                anm_init(i_this, e_rdb_class::BCK_RB_RRUN, 5.0f, 2, 1.0f);
            }
        }

        
        if (fpcM_Search(s_wbrun_sub, i_this) != NULL && i_this->timer[3] == 0) {
            i_this->run_flag = 1;
            i_this->timer[3] = cM_rndF(100.0f) + 200.0f;
        }

        // check if we need to jump a fence
        if ((boar->mStatusFlags & 0x100) != 0) {
            i_this->action = ACTION_WB_BJUMP;
            i_this->mode = 0;
        }
    }
}

static void e_rd_wb_bjump(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz unused_vec_0, unused_vec_1;
    fopAc_ac_c* actor = fopAcM_SearchByID(i_this->boar_id);
    dAttention_c* attention;

    if (actor != NULL) {
        e_wb_class* boar = (e_wb_class*) actor;
        if (daPy_getPlayerActorClass()->checkHorseRide()) {
            attention = dComIfGp_getAttention();
            if (attention->Lockon() && enemy == attention->LockonTarget(0)) {
                boar->field_0x6c0 = 1;
            }
        }

        switch (i_this->mode) {
            case 0:
                i_this->field_0x680 = 0;
                anm_init(i_this, BCK_RD_RSTEP_RBACK, 3.0f, 0, 1.0f);
                i_this->mode = 1;
                i_this->sound.startCreatureVoice(Z2SE_EN_RDB_V_HOICK, -1);
                break;

            case 1:
                if (i_this->anm_p->isStop()) {
                    i_this->mode = 2;
                    anm_init(i_this, BCK_RD_RUN, 2.0f, 0, 1.0f);
                }
                break;

            case 2:
                if ((boar->mStatusFlags & 0x200) != 0) {
                    anm_init(i_this, BCK_RD_RUN02, 2.0f, 0, 1.0f);
                    i_this->mode = 3;
                }
                break;

            case 3:
                if (i_this->anm_p->isStop()) {
                    i_this->action = ACTION_WB_RUN;
                    i_this->mode = 0;
                }
        }
    }
}

static void e_rd_bomb_action(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz mae, ato;
    dBomb_c* bomb = bomb_check(i_this);
    if (bomb == NULL) {
        int _; // needed to force dbg stack into r31
        i_this->action = ACTION_FIGHT_RUN;
        i_this->mode = 0;
        return;
    }

    mae.x = bomb->current.pos.x - enemy->current.pos.x;
    mae.z = bomb->current.pos.z - enemy->current.pos.z;
    s16 angle = cM_atan2s(mae.x, mae.z);
    f32 speed = 0.0f;
    s16 range;

    switch (i_this->mode) {
        case 0:
            i_this->mode = 1;
            anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
            i_this->timer[1] = cM_rndF(10.0f) + 10.0f;
            // fallthrough
        case 1:
            if (i_this->timer[1] == 0 && bomb->speedF < 0.1f) {
                i_this->mode = 2;
                anm_init(i_this, BCK_RD_RUN, 5.0f, 2, cM_rndFX(0.05f) + 1.0f);
            }
            break;

        case 2:
            if (bomb->speedF < 0.1f && !bomb->checkStateCarry()) {
                if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < 250.0f) {
                    range = i_this->angleY - enemy->shape_angle.y;
                    if (range < 0x4000 && range > -0x4000) {
                        i_this->mode = 5;
                        break;
                    }
                }
            }

            ANGLE_ADD_2(angle, 0x8000);
            speed = l_HIO.dash_speed;
            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) > 600.0f) {
                i_this->mode = 3;
                anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
            }
            break;

        case 3:
            angle = i_this->angleY;
            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < 500.0f) {
                i_this->mode = 0;
            }
            break;

        case 5:
            speed = l_HIO.dash_speed;
            i_this->field_0x9ad = 0;
            if (bomb->speedF > 0.1f) {
                i_this->mode = 0;
            } else if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < 100.0f) {
                i_this->timer[0] = 20;
                i_this->mode = 6;
                anm_init(i_this, BCK_RD_KICK, 3.0f, 0, 1.0f);
            }
            break;

        case 6:
            i_this->field_0x9ad = 0;
            angle = i_this->angleY;
            if ((int)i_this->anm_p->getFrame() <= 10) {
                cMtx_YrotS(*calc_mtx, angle);
                mae.x = TREG_F(15) + -30.0f;
                mae.y = 0.0f;
                mae.z = TREG_F(16) + 60.0f;
                MtxPosition(&mae, &ato);
                ato += enemy->current.pos;
                cLib_addCalc2(&bomb->current.pos.x, ato.x, 1.0f, 15.0f);
                cLib_addCalc2(&bomb->current.pos.z, ato.z, 1.0f, 15.0f);
                if ((int)i_this->anm_p->getFrame() == 10) {
                    bomb->speedF = 30.0f;
                    bomb->speed.y = 30.0f;
                    bomb->current.angle.y = enemy->shape_angle.y;
                }
            }

            if (i_this->anm_p->isStop()) {
                anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
                i_this->mode = 3;
            }
    }

    cLib_addCalc2(&enemy->speedF, speed, 1.0f, 5.0f);
    cLib_addCalcAngleS2(&enemy->current.angle.y, angle, 4, 0x1000);
    i_this->field_0x9c8 = 11;
    i_this->eye = bomb->current.pos;
}

static void e_rd_s_damage(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    switch (i_this->mode) {
        case 0:
            i_this->mode = 1;
            i_this->timer[0] = 10;
            
            if (i_this->boar_stand != 0) {
                e_wb_class* boar = (e_wb_class*)fopAcM_SearchByID(i_this->boar_id);
                if (boar != NULL) {
                    UNSET_FLAG(boar->mStatusFlags, i_this->boar_stand, u16);
                }

                i_this->boar_stand = 0;
                i_this->boar_id = -1;
            }
            break;

        case 1:
            if (i_this->timer[1] != 0) {
                i_this->enemy.onHeadLockFlg();
            }

            if (i_this->anm_p->isStop()) {
                fopAc_ac_c* actor;
                if (i_this->prev_action == 7) {
                    i_this->action = ACTION_BOW2;
                } else if (i_this->prev_action == 26) {
                    i_this->action = ACTION_BOW3;
                } else if (!other_bg_check(i_this, actor = dComIfGp_getPlayer(0))) {
                    i_this->action = ACTION_FIGHT_RUN;
                    i_this->timer[0] = 40;
                } else {
                    i_this->action = ACTION_NORMAL;
                }

                i_this->mode = 0;
            }
            break;
    }

    cLib_addCalc0(&enemy->speedF, 1.0f, 3.0f);
}

static int kado_check(e_rd_class* i_this) {
    static int kado_bit[2] = {
        1, 2,
    };

    static f32 kado_check_x[2] = {
        80.0f, -80.0f,
    };

    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    dBgS_LinChk lin_chk;
    cXyz mae, start, ato;
    BOOL rt = FALSE;

    cMtx_YrotS(*calc_mtx, i_this->jump_angle.y);
    for (int i = 0; i < 2; i++) {
        start = enemy->current.pos;
        start.y += 100.0f + JREG_F(0);
        mae.x = JREG_F(1);
        mae.y = JREG_F(2);
        mae.z = 70.0f + JREG_F(3);
        MtxPosition(&mae, &ato);
        start += ato;
        mae.y = 50.0f + JREG_F(4);
        mae.z = -150.0f + JREG_F(5);
        mae.x = (1.0f + JREG_F(6)) * kado_check_x[i];
        MtxPosition(&mae, &ato);
        ato += start;
        lin_chk.Set(&start, &ato, enemy);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            rt |= kado_bit[i];
        }
    }

    if (rt == 3) {
        rt = FALSE;
    }

    return rt;
}

static void rd_disappear(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz mae, ato;

    MTXCopy(i_this->anm_p->getModel()->getAnmMtx(11), *calc_mtx);
    mae.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&mae, &ato);

    u8 type = 0;
    if (fopAcM_SearchByName(PROC_NPC_COACH) != NULL) {
        fopAcM_createDisappear(enemy, &ato, 10, type, 51);
    } else {
        fopAcM_createDisappear(enemy, &ato, 10, type, 11);
    }

    fopAcM_delete(enemy);

    int bitsw = (fopAcM_GetParam(enemy) & 0xFF000000) >> 24;
    if (bitsw != 0xFF) {
        dComIfGs_onSwitch(bitsw, fopAcM_GetRoomNo(enemy));
    }

    if (i_this->boar_stand != 0) {
        e_wb_class* boar = (e_wb_class*)fopAcM_SearchByID(i_this->boar_id);
        if (boar != NULL) {
            UNSET_FLAG(boar->mStatusFlags, i_this->boar_stand, u16);
            if (boar->mActionID == 1) {
                boar->mActionID = 0;
            }
        }
    }
}

static BOOL body_gake(e_rd_class* i_this) {
    J3DModel* model = i_this->anm_p->getModel();
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz mae, ato;

    MTXCopy(model->getAnmMtx(11), *calc_mtx);
    mae.set(40.0f + BREG_F(10), BREG_F(11), BREG_F(12));
    MtxPosition(&mae, &ato);

    dBgS_ObjGndChk gnd_chk;
    ato.y += 100.0f;
    gnd_chk.SetPos(&ato);
    ato.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

    if ((enemy->current.pos.y - ato.y) > 300.0f) {
        return TRUE;
    }

    return FALSE;
}

static void e_rd_damage(e_rd_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopEn_enemy_c* enemy = (fopEn_enemy_c*) actor;
    cXyz mae, ato;

    if (actor->health <= 0) {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    }

    i_this->field_0x6d0 = 0.0f;

    if (daPy_py_c::checkNowWolf() != 0) {
        if (i_this->field_0xafa == 0 && enemy->checkWolfDownStartFlg()) {
            if (i_this->timer[0] > 40) {
                i_this->timer[0] = 90;
                OS_REPORT("      RD WOLFHIT LONG  \n");
            } else if (i_this->timer[0] > 20) {
                i_this->timer[0] = 70;
                OS_REPORT("      RD WOLFHIT MID  \n");
            } else {
                i_this->timer[0] = 50;
                OS_REPORT("      RD WOLFHIT SHORT \n");
            }

            i_this->mode = 30;
            i_this->timer[1] = 16;
            i_this->field_0xafa = 1;
        }
    } else if (enemy->checkCutDownHitFlg()) {
        enemy->offCutDownHitFlg();
        i_this->mode = 3;
        i_this->timer[0] = 100;
        i_this->timer[1] = 45;
        actor->health = 0;
        i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_DEATH, -1);
        i_this->field_0x968 = 1;
        daPy_getPlayerActorClass()->onEnemyDead();
        i_this->field_0xadc = 15 + TREG_S(7);
        enemy->offDownFlg();
    }

    s16 angle;
    switch (i_this->mode) {
        case 0:
            anm_init(i_this, BCK_RD_DIEB, 3.0f, 0, 1.0f);
            i_this->mode = 1;
            actor->speedF = 0.0f;
            ride_off(i_this);
            i_this->field_0xab8 = 8000.0f + BREG_F(10);

            if (actor->health <= 0) {
                i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_DEATH, -1);
                i_this->field_0x968 = 1;
                daPy_getPlayerActorClass()->onEnemyDead();

                if (desert_substage != 0) {
                    i_this->field_0x5ba = 0;
                }
            } else {
                i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_DAMAGE, -1);
            }

            i_this->timer[0] = 0;
            break;

        case 1:
            if (i_this->ObjAcch.ChkWallHit() != 0 && i_this->timer[0] == 0) {
                int kado_kabe;
                if ((kado_kabe = kado_check(i_this)) != 0) {
                    OS_REPORT("              ..KADO KABE ..%x\n", kado_kabe);
                    if (kado_kabe == 2) {
                        i_this->field_0x9f6 = 0x1000;
                        ANGLE_ADD(i_this->jump_angle.y, TREG_S(8) - 7000);
                    } else {
                        i_this->field_0x9f6 = -0x1000;
                        ANGLE_ADD(i_this->jump_angle.y, -(TREG_S(8) - 7000));
                    }

                    i_this->field_0xab8 = 8000.0f + BREG_F(10);
                    i_this->field_0xa24 = 100.0f + BREG_F(4);
                    i_this->field_0xa2c = 100.0f + BREG_F(5);
                    i_this->timer[0] = 60;
                    break;
                }

                i_this->mode = 10;
                actor->speed.y = 0.0f;
                i_this->jump_z *= 0.2f;
                ANGLE_ADD_2(i_this->jump_angle.y, 0x8000);
                i_this->field_0xaf0 = 5 + BREG_S(7);
                i_this->field_0xa24 = 100.0f + BREG_F(4);
                i_this->field_0xa2c = 100.0f + BREG_F(5);
                i_this->field_0x9f6 = 0;
            } else {
                cLib_addCalcAngleS2(&i_this->jump_angle.x, -0x4000, 1, 0x300 + BREG_S(4));
                actor->current.angle.y += i_this->field_0x9f6;
                actor->shape_angle.y = actor->current.angle.y;

                if (i_this->jump_angle.x <= -0x3000 && i_this->ObjAcch.ChkGroundHit()) {
                    i_this->field_0xa1e = 10;

                    if (i_this->field_0x1294 != 0) {
                        i_this->sound.startCreatureSound(Z2SE_CM_BODYFALL_ASASE_M, 0, -1);
                    } else {
                        i_this->sound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
                    }

                    i_this->mode = 2;

                    if (i_this->field_0xa1f == 0) {
                        anm_init(i_this, BCK_RD_DIEA, 3.0f, 0, 1.0f);
                        angle = (s16) i_this->jump_angle.y;
                    } else {
                        angle = i_this->jump_angle.y + 0x8000;
                    }

                    actor->current.angle.y = angle;

                    if (i_this->damage_flag != 0) {
                        i_this->jump_z = 0.0f;
                        i_this->field_0xab8 = 2000.0f + BREG_F(12);
                    } else {
                        if (i_this->field_0xa1f == 0) {
                            i_this->field_0xa24 = -100.0f - BREG_F(6);
                            i_this->field_0xa2c = -100.0f - BREG_F(7);
                        } else {
                            i_this->field_0xa24 = 100.0f + BREG_F(4);
                            i_this->field_0xa2c = 100.0f + BREG_F(5);
                        }

                        i_this->field_0xa28 = -0x5000;
                        i_this->field_0xa30 = -0x4000;
                        i_this->jump_z *= 0.5f;
                        actor->speed.y = 20.0f + TREG_F(6);
                        i_this->field_0x9f6 /= 2;
                        i_this->field_0xab8 = 8000.0f + BREG_F(12);
                        i_this->field_0x125d = 1;
                    }
                }
            }
            break;

        case 2:
            actor->current.angle.y += i_this->field_0x9f6;
            actor->shape_angle.y = actor->current.angle.y;
            cLib_addCalcAngleS2(&i_this->jump_angle.x, -0x4000, 1, 0x300 + BREG_S(4));

            if (i_this->ObjAcch.ChkGroundHit()) {
                if (actor->health > 0 && daPy_py_c::checkNowWolf() == 0) {
                    enemy->onDownFlg();
                }

                dKy_Sound_set(actor->current.pos, 100, fopAcM_GetID(i_this), 5);
                i_this->field_0xa1e = 10;

                if (i_this->field_0xa1f == 0) {
                    anm_init(i_this, BCK_RD_DIEA, 3.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_DIEB, 3.0f, 0, 1.0f);
                }

                i_this->jump_z = 0.0f;
                i_this->jump_angle.x = -0x4000;

                if (i_this->field_0xa1f == 0) {
                    angle = s16(i_this->jump_angle.y);
                    for (int i = 0; i < 4; i++) {
                        i_this->field_0xad2[i] = cM_rndF(10.0f);
                    }
                } else {
                    angle = i_this->jump_angle.y + 0x8000;
                }
                actor->current.angle.y = angle;

                if (daPy_py_c::checkNowWolf() != 0) {
                    i_this->timer[0] = 80;
                    i_this->timer[1] = 55;
                } else {
                    i_this->timer[0] = cM_rndF(10.0f) + 60.0f;
                    i_this->timer[1] = 35;
                }

                i_this->mode = 3;
                i_this->field_0xab8 = 1500.0f + BREG_F(13);
                i_this->field_0x125d = 1;
            }
            break;

        case 3:
            if (body_gake(i_this)) {
                i_this->jump_z = -20.0f + VREG_F(8);
                i_this->jump_angle.x -= s16(0x300 + VREG_S(7));
            } else {
                i_this->jump_z = 0.0f;
            }

            if (actor->health <= 0 && i_this->timer[1] == 0) {
                rd_disappear(i_this);
                return;
            }

            if (daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_DOWN && i_this->timer[0] == 0) {
                i_this->jump_angle.x = -0x4000;
                enemy->offDownFlg();
                i_this->mode = 20;

                if (i_this->field_0xa1f == 0) {
                    anm_init(i_this, BCK_RD_DIEA_UP, 3.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_DIEB_UP, 3.0f, 0, 1.0f);
                }
            }
            break;

        case 10:
            if (i_this->field_0xaf0 != 0) {
                cLib_addCalcAngleS2(&i_this->jump_angle.x, 0, 1, 0x300 + BREG_S(4));
            } else {
                cLib_addCalcAngleS2(&i_this->jump_angle.x, -0x4000, 1, 0x800 + BREG_S(5));
                if (i_this->ObjAcch.ChkGroundHit()) {
                    i_this->mode = 2;
                    if (i_this->field_0xa1f == 0) {
                        anm_init(i_this, BCK_RD_DIEA, 3.0f, 0, 1.0f);
                        i_this->field_0xa24 = -100.0f - BREG_F(6);
                        i_this->field_0xa2c = -100.0f - BREG_F(7);
                    } else {
                        i_this->field_0xa24 = 100.0f + BREG_F(4);
                        i_this->field_0xa2c = 100.0f + BREG_F(5);
                    }

                    i_this->field_0xa28 = -0x5000;
                    i_this->field_0xa30 = -0x4000;
                    i_this->jump_z *= 0.5f;
                    actor->speed.y = 20.0f + TREG_F(6);

                    if (i_this->field_0xa1f == 0) {
                        angle = s16(i_this->jump_angle.y);
                    } else {
                        angle = i_this->jump_angle.y + 0x8000;
                    }

                    actor->current.angle.y = angle;
                    i_this->field_0xab8 = 8000.0f + BREG_F(12);
                }
            }
            break;

        case 20:
            if (i_this->anm_p->getFrame() >= 35.0f) {
                i_this->field_0xa18.x = i_this->field_0xa18.z = 0;
            }

            if (i_this->anm_p->isStop()) {
                i_this->jump_angle.x = i_this->jump_angle.y = 0;
                anm_init(i_this, BCK_RD_WAIT01, 0.0f, 2, 1.0f);
                i_this->timer[0] = 5;
                i_this->mode = 21;

                enemy->offDownFlg();
            }
            break;

        case 21:
            if (i_this->timer[0] == 0) {
                i_this->damage_timer = 0;
                if (pl_check(i_this, 10000.0f, 0x6000)) {
                    i_this->action = ACTION_NORMAL;
                    i_this->mode = 0;
                } else {
                    i_this->timer[0] = cM_rndF(20.0f) + 20.0f;
                    anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
                    i_this->mode = 22;
                }
            }
            break;

        case 22:
            if (i_this->timer[0] == 0) {
                i_this->action = ACTION_NORMAL;
                i_this->mode = 0;
                i_this->timer[0] = cM_rndF(50.0f) + 50.0f;
                anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
            }
            break;

        case 30:
            if (!enemy->checkWolfDownPullFlg()) {
                i_this->timer[1] = 0;
            }

            if (i_this->timer[0] == 0) {
                i_this->mode = 31;
                if (i_this->field_0xa1f == 0) {
                    anm_init(i_this, BCK_RD_DIEA_UP, 3.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_DIEB_UP, 3.0f, 0, 1.0f);
                }

                i_this->timer[0] = 10;
            } else if (i_this->timer[1] == 1) {
                enemy->onWolfDownPullEndFlg();
                enemy->offCutDownHitFlg();
                actor->health = 0;
                i_this->timer[0] = 1000;
                i_this->timer[1] = 35;
                i_this->mode = 3;
            }
            break;

        case 31:
            if (i_this->timer[0] == 0) {
                enemy->offDownFlg();
                i_this->mode = 20;
                i_this->field_0xafa = 0;
            }
    }

    if (enemy->checkDownFlg()) {
        MTXCopy(i_this->anm_p->getModel()->getAnmMtx(11), *calc_mtx);
        mae.set(BREG_F(0), BREG_F(1), BREG_F(2));
        MtxPosition(&mae, &ato);
        enemy->setDownPos(&ato);
        i_this->field_0x9ad = 0;
    }

    if (i_this->field_0xadc != 0) {
        i_this->field_0xadc--;
        i_this->field_0xada = i_this->field_0xadc * cM_ssin(i_this->field_0xadc * (0x3800 + TREG_S(9))) * (100.0f + TREG_F(5));
    }
}

static s16 gake_check(e_rd_class* i_this, f32 param_2) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    dBgS_GndChk gnd_chk;
    dBgS_LinChk lin_chk;
    cXyz spf8, sp104, mae, ato;
    s16 angle = enemy->shape_angle.y;

    for (int i = 0; i < 16; i++) {
        cMtx_YrotS(*calc_mtx, angle);
        mae.x = 0.0f;
        mae.y = 100.0f;
        mae.z = param_2;
        MtxPosition(&mae, &ato);
        ato += enemy->current.pos;
        gnd_chk.SetPos(&ato);

        if (enemy->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 300.0f) {
            mae = enemy->current.pos;
            mae.y += 100.0f;
            lin_chk.Set(&mae, &ato, enemy);
            if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
                return angle;
            }
        }

        ANGLE_ADD(angle, 0x1000);
    }

    return enemy->shape_angle.y;
}

static void* s_bikkuri_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && i_actor != i_data) {
        if (((e_rd_class*)i_actor)->action == ACTION_BOW3 && ((e_rd_class*)i_actor)->mode <= 2) {
            cXyz sp28 = ((fopAc_ac_c*)i_actor)->current.pos - ((fopAc_ac_c*)i_data)->current.pos;
            if (sp28.abs() < KREG_F(11) + 350.0f) {
                anm_init((e_rd_class*)i_actor, 29, 5.0f, 2, 1.3f);
                ((e_rd_class*)i_actor)->mode = 3;
                ((e_rd_class*)i_actor)->timer[0] = cM_rndF(20.0f) + 40.0f;
            }
        }
    }

    return NULL;
}

static void* s_saku_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_H_Saku) {
        cXyz sp14 = ((daObjH_Saku_c*)i_actor)->field_0x5e0 - ((fopAc_ac_c*)i_data)->current.pos;
        if (sp14.abs() < 200.0f) {
            return i_actor;
        }
    }

    return NULL;
}

static void e_rd_drop(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    f32 speed = 0.0f;

    switch (i_this->mode) {
        case 0:
            i_this->sound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_DAMAGE, -1);
            anm_init(i_this, BCK_RD_DAMAGE_S, 2.0f, 0, 1.0f);
            i_this->mode = 1;

            if (desert_substage != 0) {
                i_this->demo_mode = 10;
            }
            break;

        case 1:
            i_this->sound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_DAMAGE, -1);
            
            if (i_this->anm_p->isStop()) {
                anm_init(i_this, BCK_RD_FURA2, 0.0f, 2, 1.0f);
                i_this->mode = 2;

                if (i_this->demo_mode != 0) {
                    i_this->target_ya = i_this->angleY;
                } else if (strcmp(dComIfGp_getStartStageName(), "F_SP128") == 0) {
                    // Hidden Village
                    i_this->target_ya = enemy->home.angle.y;
                } else {
                    i_this->target_ya = gake_check(i_this, 200.0f);
                }

                fpcM_Search(s_bikkuri_sub, i_this);
                i_this->timer[0] = 60;
            }
            break;

        case 2:
            i_this->sound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_DAMAGE, -1);
            speed = 4.0f;
            cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->target_ya, 8, 0x400);

            if (i_this->timer[0] == 0) {
                i_this->action = ACTION_A_DAMAGE;
                i_this->mode = 0;
                return;
            }

            if (move_gake_check(i_this, KREG_F(6) + 30.0f)) {
                i_this->mode = 3;
                i_this->jump_angle.y = enemy->shape_angle.y + 0x8000;
                i_this->jump_angle.x = 0;
                i_this->anm_p->setPlaySpeed(0.3f);
                i_this->field_0xaf8 = 0;
                i_this->timer[0] = 20;
            }
            break;

        case 3:
            if (i_this->timer[0] == 19) {
                daObjH_Saku_c* h_saku = (daObjH_Saku_c*)fpcM_Search(s_saku_sub, i_this);
                if (h_saku != NULL) {
                    h_saku->BreakSet();
                }
            }

            speed = 3.0f;
            
            if (i_this->jump_angle.x > -0x4000) {
                enemy->speed.y = 0.0f;
                i_this->sound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_DAMAGE, -1);
            } else {
                if (i_this->jump_angle.x > -0x6800) {
                    enemy->speed.y = -fabsf(cM_scos(i_this->jump_angle.x) * (TREG_F(8) + 70.0f));
                    i_this->sound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_FALLING, -1);
                } else {
                    i_this->sound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_FALLING, -1);
                }
            }

            cLib_addCalcAngleS2(&i_this->jump_angle.x, -0x7000, 2, i_this->field_0xaf8);
            cLib_addCalcAngleS2(&i_this->field_0xaf8, 0x800, 1, 0x50);
            i_this->field_0x9ab = 2;

            if (i_this->timer[0] == 0 && enemy->current.pos.y <= i_this->ObjAcch.GetGroundH() + 100.0f + KREG_F(17)) {
                if (i_this->arg1 == 13 || i_this->arg1 == 14) {
                    i_this->action = ACTION_REG;
                    i_this->mode = 1;
                    i_this->field_0xafb = 1;
                    i_this->field_0xaf2 = 0;
                } else {
                    enemy->current.pos.y = i_this->ObjAcch.GetGroundH() + 100.0f + KREG_F(17);
                    i_this->jump_angle.x = -0x4000;
                    i_this->action = ACTION_DAMAGE;
                    i_this->mode = 0;
                    i_this->jump_z = 0.0f;
                    enemy->health = 0;
                }
            }
            break;
    }

    cLib_addCalc2(&enemy->speedF, speed, 1.0f, 3.0f);
}

static void e_rd_a_damage(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    i_this->damage_timer = 3;
    fopAcM_OffStatus(enemy, 0);
    enemy->attention_info.flags = 0;

    switch (i_this->mode) {
        case 0:
            i_this->mode = 1;
            anm_init(i_this, BCK_RD_DEAD_ARROW, 5.0f, 0, 1.0f);
            i_this->timer[0] = 80;

            if (desert_substage != 0) {
                i_this->field_0x5ba = 0;
            }

            dKy_Sound_set(enemy->current.pos, 100, fopAcM_GetID(i_this), 5);
            break;

        case 1:
            if (i_this->timer[0] == 0) {
                rd_disappear(i_this);
            }
            break;
    }

    cLib_addCalc0(&enemy->speedF, 1.0f, 3.0f);
}

static void e_rd_stand(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    switch (i_this->mode) {
        case 0:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
                i_this->mode = 1;
                i_this->timer[0] = cM_rndF(40.0f) + 60.0f;
            }
            break;

        case 1:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
                i_this->timer[0] = cM_rndF(100.0f) + 100.0f;
                i_this->mode = 0;
            }
            break;
    }

    f32 range = i_this->attack_range;
    s16 angle = 0x4000;
    BOOL search_check = FALSE;

    if (i_this->look_timer == 1 || S_find != 0) {
        if (i_this->look_timer == 1) {
            search_check = TRUE;
        }
        range = 10000.0f;
        angle = 0x7FFF;
    }

    if ((i_this->counter & 7) == 0 && fopAcM_GetRoomNo(enemy) == 0) {
        if (S_find != 0) {
            wb_check(i_this, angle);
            if (i_this->boar_stand != 0) {
                return;
            }
        }

        if (pl_check(i_this, 500.0f, 0x4000)) {
            i_this->action = ACTION_FIGHT_RUN;
            i_this->mode = -10;
            i_this->timer[0] = 60;
        }

        return;
    }

    if (((i_this->counter & 7) == 0 || search_check) && pl_check(i_this, range, angle) && i_this->weapon_type == 1) {
        OS_REPORT(" <<<<<<<<<<<<<< RD RIDE 4\n");
        i_this->action = ACTION_FIGHT_RUN;
        i_this->mode = -10;
        i_this->timer[0] = 60;
    }
}

static s8 e_rd_bow3(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    if (i_this->damage_timer == 0 && i_this->mode != 5 && fpcM_Search(s_command3_sub, i_this) == NULL) {
        i_this->mode = 5;
        i_this->timer[1] = cM_rndF(20.0f) + 40.0f;
        i_this->timer[0] = cM_rndF(20.0f) + 80.0f;
    }

    cXyz unused_vec_0, unused_vec_1;
    int frame = i_this->anm_p->getFrame();
    s8 bVar1 = 1;
    s8 rt = 0;

    switch (i_this->mode) {
        case 0:
            bVar1 = 0;
            anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
            i_this->mode = 1;
            break;

        case 1:
            bVar1 = 0;
            break;

        case 3:
            if (i_this->timer[0] == 0) {
                i_this->mode = 0;
            }
            break;

        case 5:
            if (i_this->timer[1] == 1) {
                anm_init(i_this, BCK_RD_KYORO2, 5.0f, 2, 1.3f);
            }

            if (i_this->timer[0] == 10) {
                anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
            }

            if (i_this->timer[0] == 0) {
                i_this->action = ACTION_BOW2;
                i_this->mode = 0;
            }
            break;

        case 10:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, BCK_RD_SHOOT_READY, 5.0f, 0, 1.0f);
                i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 7),
                0, 5.0f, 0.9f + TREG_F(3), 0.0f, -1.0f);
                i_this->mode = 11;
                enemy->speedF = 0.0f;
            }
            break;

        case 11:
            i_this->field_0x9c8 = 4;
            if (frame >= 10) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 20) {
                i_this->sound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
            }

            if (i_this->anm_p->isStop()) {
                anm_init(i_this, BCK_RD_SHOOT_WAIT, 4.0f, 2, 1.0f);
                i_this->timer[0] = cM_rndF(10.0f) + 10.0f;
                i_this->mode = 12;
            }
            break;

        case 12:
            i_this->field_0x9c8 = 4;
            
            if (i_this->timer[0] == 0 && !dMsgObject_isTalkNowCheck() && i_this->field_0x9a4 == 0) {
                anm_init(i_this, BCK_RD_SHOOT, 1.0f, 0, 1.0f);
                i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 10), 0, 1.0f, 1.0f, 0.0f, -1.0f);
                i_this->mode = 13;
            }

            i_this->field_0x9a2 = 1;
            break;

        case 13:
            if (frame <= 3 + XREG_S(5)) {
                i_this->field_0x9a2 = 1;
                i_this->field_0x9c8 = 4;
            }

            if (frame == 2 + XREG_S(7)) {
                i_this->arrow_flag = 1;
            }

            if (i_this->anm_p->isStop()) {
                i_this->mode = 0;
            }
    }

    if (bVar1) {
        cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->angleY, 4, 0x1000);
    }

    if (i_this->dis < 500.0f) {
        i_this->action = ACTION_BOW_RUN;
        i_this->mode = 0;
        i_this->one_hit_kill = 0;
    }

    return rt;
}

static void* s_command2_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD
        && ((e_rd_class*)i_actor)->action == ACTION_BOW3 && ((e_rd_class*)i_actor)->mode < 10) {
        ((e_rd_class*)i_actor)->timer[0] = cM_rndF(10.0f) + 10.0f;
        ((e_rd_class*)i_actor)->mode = 10;
    }

    return NULL;
}

static void* s_command4_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && i_actor != i_data && ((e_rd_class*)i_actor)->action == ACTION_BOW3) {
        return i_actor;
    }

    return NULL;
}

static void e_rd_commander(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    s8 bVar1 = 0;
    s8 bVar2 = 0;
    boss = i_this;

    if (i_this->timer[2] == 0 && fpcM_Search(s_command4_sub, i_this) == NULL) {
        i_this->action = ACTION_BOW2;
        i_this->mode = 0;
    } else {
        switch (i_this->mode) {
            case 0:
                i_this->mode = 1;
                break;

            case 1:
                bVar1 = 1;
                if (i_this->timer[0] == 0) {
                    anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
                    i_this->mode = 2;
                    i_this->timer[0] = cM_rndF(40.0f) + 60.0f;
                }
                break;

            case 2:
                bVar1 = 1;
                if (i_this->timer[0] == 0) {
                    anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
                    i_this->timer[0] = cM_rndF(100.0f) + 100.0f;
                    i_this->mode = 1;
                }
                break;

            case 5:
                anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
                i_this->timer[0] = 30;
                i_this->mode = 6;
                break;

            case 6:
                bVar2 = 1;
                if (i_this->timer[0] == 0) {
                    anm_init(i_this, BCK_RD_GOSIGN, 3.0f, 0, 1.0f);
                    i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_COMMANDER, -1);
                    i_this->mode = 7;

                    if (dComIfGp_event_runCheck()) {
                        i_this->timer[0] = l_HIO.command_to_ready_stance + 2;
                    } else {
                        i_this->timer[0] = 10;
                    }
                }
                break;

            case 7:
                bVar2 = 1;
                if (i_this->timer[0] == 1) {
                    fpcM_Search(s_command2_sub, i_this);
                }

                if (i_this->timer[0] == 0 && i_this->anm_p->isStop()) {
                    anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
                    if (!dComIfGp_event_runCheck() && !pl_check(i_this, i_this->attack_range, 0x4000)) {
                        i_this->mode = 10;
                        i_this->timer[1] = TREG_S(8) + 350;
                    } else {
                        if (dComIfGp_event_runCheck()) {
                            i_this->timer[0] = 80;
                        } else {
                            i_this->timer[0] = TREG_S(3);
                        }

                        i_this->mode = 6;
                    }
                }
                break;

            case 10:
                anm_init(i_this, BCK_RD_GOSIGN, 3.0f, 0, 1.0f);
                mDoAud_seStart(Z2SE_EN_RD_V_CALLING, 0, 0, 0);
                fpcM_Search(s_command2_sub, i_this);
                i_this->mode = 11;
                break;

            case 11:
                if (i_this->anm_p->isStop()) {
                    if (i_this->timer[1] == 0) {
                        anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
                        i_this->timer[0] = cM_rndF(30.0f) + 30.0f;
                        i_this->mode = 1;
                    } else {
                        i_this->mode = 10;
                    }
                }
        }

        if (bVar1 && pl_check(i_this, i_this->attack_range, 0x4000)) {
            i_this->mode = 5;
            if (i_this->sw != 0) {
                dComIfGs_onSwitch(i_this->sw, fopAcM_GetRoomNo(enemy));
            }
        }

        if (bVar2) {
            cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->angleY, 2, 0x800);
        }
    }
}

static void e_rd_excite(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz sp40;
    f32 speed = 0.0f;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, BCK_RD_WAIT01, 4.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->timer[0] = cM_rndF(20.0f) + 20.0f;
            i_this->target_ya = enemy->current.angle.y;
            i_this->field_0x5c0 = S_find_pos;
            break;

        case 1:
            i_this->field_0x9c8 = 11;
            i_this->eye = S_find_pos;

            if (i_this->timer[0] == 0) {
                if (i_this->one_hit_kill != 0) {
                    i_this->mode = 3;
                    anm_init(i_this, BCK_RD_KYORO2, 5.0f, 2, 1.3f);
                    i_this->timer[0] = cM_rndF(20.0f) + 40.0f;
                } else {
                    i_this->mode = 2;
                    anm_init(i_this, BCK_RD_RUN, 2.0f, 2, cM_rndFX(0.05f) + 1.0f);
                    i_this->timer[0] = cM_rndF(15.0f) + 30.0f;
                }

                sp40 = i_this->field_0x5c0 - enemy->current.pos;
                i_this->target_ya = (s16) cM_atan2s(sp40.x, sp40.z);
            }
            break;

        case 2:
            i_this->field_0x9c8 = 11;
            i_this->eye = i_this->field_0x5c0;
            speed = l_HIO.dash_speed;
            sp40 = i_this->field_0x5c0 - enemy->current.pos;
            if (i_this->timer[0] == 0 || move_gake_check(i_this, 100.0f) || sp40.abs() < 100.0f) {
                i_this->mode = 3;
                anm_init(i_this, BCK_RD_KYORO2, 5.0f, 2, 1.3f);
                i_this->timer[0] = cM_rndF(20.0f) + 40.0f;
            }
            break;

        case 3:
            if (i_this->timer[0] == 0) {
                i_this->action = i_this->prev_action;
                if (i_this->action == ACTION_STAND) {
                    i_this->action = ACTION_NORMAL;
                }

                i_this->mode = 0;
            }
            break;
    }

    cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->target_ya, 2, 0x800);
    s16 range = enemy->current.angle.y - i_this->target_ya;
    if (range > 0x400 || range < -0x400) {
        speed = 0.0f;
    }

    cLib_addCalc2(&enemy->speedF, speed, 1.0f, 5.0f);
}

static void e_rd_water(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    i_this->damage_timer = 10;

    switch (i_this->mode) {
        case 0:
        case 1: {
            enemy->speed.y = 0.0f;
            if (i_this->field_0xa1f == 0) {
                anm_init(i_this, BCK_RD_DROWNEDA, 3.0f, 2, 1.0f);
            } else {
                anm_init(i_this, BCK_RD_DROWNEDB, 3.0f, 2, 1.0f);
            }

            i_this->timer[0] = 60;
            i_this->field_0x9f0 = i_this->jump_z = enemy->speedF = 0.0f;
            cXyz yogan = enemy->current.pos;
            yogan.y = i_this->waterY;
            if (i_this->mode == 0) {
                fopKyM_createWpillar(&yogan, 1.5f, 0);
            } else {
                OS_REPORT("      RD YOGAN  \n");
                fopKyM_createMpillar(&yogan, 1.0f);
            }

            i_this->mode = 10;
            break;
        }

        case 10:
            enemy->speed.y = 0.0f;
            cLib_addCalc2(&enemy->current.pos.y, i_this->waterY, 0.5f, 5.0f);
            cLib_addCalcAngleS2(&i_this->jump_angle.x, -0x4000, 4, 0x400 + BREG_S(4));

            if (i_this->timer[0] == 0) {
                rd_disappear(i_this);
            }
            break;
    }
}

static void e_rd_kiba_start(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    fopAcM_OffStatus(enemy, 0);
    enemy->attention_info.flags = 0;
    i_this->damage_timer = 10;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, e_rdb_class::BCK_RB_RWAIT02, 1.0f, 2, 1.0f);
            i_this->mode++;
            i_this->field_0x6a0 = 0;
            break;

        case 2:
            anm_init(i_this, e_rdb_class::BCK_RB_RCOMEON, 10.0f, 0, 1.0f);
            i_this->mode++;
            break;

        case 3:
            if (i_this->anm_p->isStop()) {
                anm_init(i_this, e_rdb_class::BCK_RB_RWAIT02, 5.0f, 2, 1.0f);
                i_this->mode++;
            }
            break;

        case 5:
            i_this->field_0x6a0 = 1;
            anm_init(i_this, e_rdb_class::BCK_RB_RPLAY_STOP2, 20.0f, 0, 1.0f);
            horn_anm_init(i_this, 6, 2.0f, 0, 1.0f);
            i_this->mode++;
            break;

        case 6:
            if ((int)i_this->anm_p->getFrame() == 21) {
                i_this->sound.startCreatureVoice(Z2SE_EN_RDB_HORN, -1);
            }

            if (i_this->anm_p->isStop()) {
                i_this->action = ACTION_WB_RUN;
                i_this->mode = 0;
                fopAcM_OnStatus(enemy, 0); // statement does nothing.
                enemy->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            }
    }
}

static void e_rd_ikki2_start(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    fopAcM_OffStatus(enemy, 0);
    enemy->attention_info.flags = 0;
    i_this->damage_timer = 10;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, e_rdb_class::BCK_RB_ROP_DEMO2_WAIT, 1.0f, 2, 1.0f);
            i_this->mode++;
            break;
            
        case 2:
            anm_init(i_this, e_rdb_class::BCK_RB_ROP_DEMO2_EXCITE, 5.0f, 0, 1.0f);
            i_this->sound.startCreatureVoice(Z2SE_EN_RDB_V_DEMO_EXCITE, -1);
            i_this->mode++;
            break;

        case 3:
            if (i_this->anm_p->isStop()) {
                anm_init(i_this, e_rdb_class::BCK_RB_ROP_DEMO2_WAIT, 10.0f, 2, 1.0f);
                i_this->mode = 4;
            }
            break;

        case 4:
            break;
    }
}

static void e_rd_kiba_end(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    fopAcM_OffStatus(enemy, 0);
    enemy->attention_info.flags = 0;
    i_this->damage_timer = 10;
    e_wb_class* boar = (e_wb_class*)fopAcM_SearchByID(i_this->boar_id);

    switch (i_this->mode) {
        case 0:
            i_this->field_0x680 = 0;
            anm_init(i_this, BCK_RD_RSHOOT_WAIT, 1.0f, 0, 1.0f);
            i_this->mode++;
            i_this->timer[0] = 14;
            boar->field_0x7a2 = 1;
            break;

        case 1:
            if (i_this->timer[0] == 1) {
                cXyz mae, ato;
                cMtx_YrotS(*calc_mtx, enemy->shape_angle.y);
                mae.x = BREG_F(8) + 40.0f;
                mae.y = BREG_F(9) + 20.0f;
                mae.z = BREG_F(10) + 57.0f;
                MtxPosition(&mae, &i_this->field_0x6b0);
                i_this->field_0x6bc.y = enemy->shape_angle.y + BREG_S(0);
                mae.x = BREG_F(11) + 80.0f;
                mae.y = BREG_F(12);
                mae.z = BREG_F(13);
                MtxPosition(&mae, &ato);
                i_this->field_0x6a4 += ato;
                i_this->field_0x6a0 = 2;
            }

            if (i_this->anm_p->isStop()) {
                anm_init(i_this, BCK_RD_RSHOOT_WAIT02, 2.0f, 2, 1.0f);
                i_this->mode++;
                i_this->timer[0] = 110;
            }
            break;

        case 2:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, BCK_RD_RSTEP_BACK, 2.0f, 0, 1.0f);
                i_this->mode++;
                i_this->sound.startCreatureVoice(Z2SE_EN_RDB_V_AWAKE, -1);
                i_this->timer[0] = 10;
            }
            break;

        case 3:
            if (i_this->timer[0] == 0) {
                boar->field_0x7a2 = 0;
            }

            if (i_this->anm_p->isStop()) {
                anm_init(i_this, BCK_RD_SHOUT, 5.0f, 2, 1.0f);
                i_this->mode++;
                i_this->timer[0] = 20;
            }
            break;

        case 4:
            if (i_this->timer[0] != 0) {
                i_this->field_0x9c8 = 1;
            }

            if ((boar->mStatusFlags & 0x100) != 0) {
                i_this->action = ACTION_WB_BJUMP;
                i_this->mode = 0;
                i_this->timer[1] = 2000;
                i_this->timer[2] = 1000;
            }
            break;
    }
}

static void e_rd_ikki_end(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    e_wb_class* boar = (e_wb_class*)fopAcM_SearchByID(i_this->boar_id);

    fopAcM_OffStatus(enemy, 0);
    enemy->attention_info.flags = 0;
    i_this->damage_timer = 10;
    boar->field_0x1434 = fopAcM_GetID(i_this);

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, BCK_RD_RRUN02_BACK, 1.0f, 0, 1.0f);
            i_this->mode++;
            i_this->ride_mode = 0;

            if (daPy_getPlayerActorClass()->checkHorseRide() && boar != NULL) {
                UNSET_FLAG(boar->mStatusFlags, i_this->boar_stand, u16);
                i_this->boar_stand = 0;
                boar->mActionID = 16;
                boar->mActionMode = 0;
                boar->field_0x169e = 10;
            }

            enemy->speedF = 0.0f;
            enemy->speed.y = 40.0f + KREG_F(7);
            i_this->jump_z = 70.0f + KREG_F(8);

            if (fopAcM_searchPlayerAngleY(enemy) < 0) {
                i_this->jump_angle.y = 0x4000;
            } else {
                i_this->jump_angle.y = -0x4000;
            }

            i_this->sound.startCreatureVoice(Z2SE_EN_RDB_V_DEATH, -1);
            break;

        case 1:
            if (i_this->anm == BCK_RD_RRUN02_BACK) {
                if (i_this->anm_p->isStop()) {
                    anm_init(i_this, BCK_RD_JUMP_A, 1.0f, 2, 1.0f);
                }
            }

            if (enemy->speed.y < 0.0f) {
                cLib_addCalc2(&i_this->jump_z, KREG_F(9), 0.1f, 1.0f);
                enemy->current.angle.y += i_this->field_0x9f6;
            }
            break;
    }
}

static void e_rd_ikki2_end(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    e_wb_class* boar = (e_wb_class*)fopAcM_SearchByID(i_this->boar_id);

    fopAcM_OffStatus(enemy, 0);
    enemy->attention_info.flags = 0;
    i_this->damage_timer = 10;
    boar->field_0x1434 = fopAcM_GetID(i_this);

    switch (i_this->mode) {
        case 0:
            if (daPy_getPlayerActorClass()->checkHorseRide()) {
                anm_init(i_this, BCK_RD_RRUN_BACK, 0.0f, 0, 1.0f);
                i_this->mode = 1;
                boar->mActionID = 18;
                boar->mActionMode = 0;
                boar->field_0x169e = 90;
                mDoAud_bgmStop(30);
            }
            break;

        case 1:
            if (i_this->anm_p->isStop()) {
                anm_init(i_this, BCK_RD_RSHOOT_READY, 3.0f, 2, 1.0f);
            }
            break;

        case 10:
            anm_init(i_this, BCK_RD_RSHOOT, 2.0f, 0, 1.0f);
            i_this->mode = 11;
            i_this->ride_mode = 0;

            if (boar != NULL) {
                UNSET_FLAG(boar->mStatusFlags, i_this->boar_stand, u16);
                i_this->boar_stand = 0;
            }

            enemy->speedF = 0.0f;
            enemy->speed.y = 50.0f + KREG_F(7);
            i_this->jump_z = 60.0f + KREG_F(8);
            i_this->jump_angle.y = 0x4000;

            i_this->sound.startCreatureVoice(Z2SE_EN_RDB_V_DEATH, -1);
            break;

        case 11:
            if (enemy->speed.y < 0.0f) {
                cLib_addCalc2(&i_this->jump_z, KREG_F(9), 0.1f, 1.0f);
            }
            break;
    }
}

static void* s_rdb_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RDB) {
        return i_actor;
    }

    return NULL;
}

static void e_rd_lv9_end(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    e_wb_class* boar = (e_wb_class*)fopAcM_SearchByID(i_this->boar_id);

    i_this->damage_timer = 10;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, e_rdb_class::BCK_RB_RWALK, 0.0f, 2, 1.0f);
            i_this->mode = 1;
            break;

        case 1:
            if ((boar->mStatusFlags & 8) != 0) {
                anm_init(i_this, e_rdb_class::BCK_RB_RWAIT, 10.0f, 2, 1.0f);
                i_this->mode = 2;
            }
            break;

        case 2:
            if ((boar->mStatusFlags & 8) != 0) {
                anm_init(i_this, e_rdb_class::BCK_RB_LV9_END03, 5.0f, 0, 1.0f);
                i_this->sound.startCreatureVoice(Z2SE_EN_RDB_V_LV9_END03, -1);
                i_this->mode = 3;
            }
            break;

        case 3:
            if (i_this->anm == e_rdb_class::BCK_RB_LV9_END03) {
                if (i_this->anm_p->isStop()) {
                    anm_init(i_this, e_rdb_class::BCK_RB_RWAIT, 10.0f, 2, 1.0f);
                }
            } else {
                e_rdb_class* rdb = (e_rdb_class*)fpcM_Search(s_rdb_sub, i_this);
                if (rdb != NULL && rdb->mDemoMode == 14) {
                    anm_init(i_this, e_rdb_class::BCK_RB_RNEIGH, 3.0f, 0, 1.0f);
                    boar->mActionMode++;
                    i_this->mode = 4;
                }
            }
            break;

        case 4:
            if (i_this->anm_p->isStop()) {
                anm_init(i_this, e_rdb_class::BCK_RB_RRUN, 3.0f, 2, 1.0f);
                i_this->mode = 5;
            }
            break;

        case 5:
            break;
    }
}

static void* s_boom_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD) {
        e_rd_class* i_this = (e_rd_class*) i_actor;
        if (i_this->field_0x99a != 0) {
            return i_this;
        }
    }

    return NULL;
}

static void wolfkick_damage(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    fopAc_ac_c* pla = dComIfGp_getPlayer(0);

    i_this->action = ACTION_DAMAGE;
    i_this->mode = 0;
    i_this->jump_angle.y = pla->shape_angle.y + 0x8000;
    enemy->speed.y = l_HIO.wolf_falling_power_y;
    i_this->jump_z = -l_HIO.wolf_falling_power_z;
    i_this->sound.startCollisionSE(Z2SE_HIT_WOOD_WEAPON, 31);
    i_this->damage_flag = 1;
    i_this->damage_timer = 1000;
}

static void big_damage(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    i_this->action = ACTION_DAMAGE;
    i_this->mode = 0;
    enemy->speed.y = l_HIO.jump_y + 2.0f;

    if (i_this->ride_mode == 2) {
        fopAc_ac_c* actor = fopAcM_SearchByID(i_this->boar_id);
        if (actor != NULL && actor->speedF >= 20.0f) {
            i_this->jump_z = actor->speedF;
            if (i_this->jump_z > 40.0f) {
                i_this->jump_z = 40.0f;
            }
        } else {
            i_this->jump_z = -l_HIO.jump_z_suspended;
        }

        i_this->jump_angle.y = enemy->shape_angle.y + (s16)cM_rndFX(3000.0f);
    } else {
        i_this->jump_angle.y = i_this->AtInfo.mHitDirection.y;
        i_this->jump_z = -l_HIO.jump_z;
    }

    if ((i_this->AtInfo.mHitBit & 0x80) != 0) {
        OS_REPORT("SPIN CUT HIT !!\n");
        i_this->field_0x9f6 = cM_rndFX(2000.0f) + 4000.0f;
    } else if ((i_this->AtInfo.mHitBit & 0x800) != 0) {
        OS_REPORT("SPIN CUT L HIT !!\n");
        i_this->field_0x9f6 = -(cM_rndFX(2000.0f) + 4000.0f);
    } else {
        if (i_this->ride_mode != 0) {
            i_this->field_0x9f6 = cM_rndFX(3000.0f);
        } else {
            if (fopAcM_GetName(i_this->AtInfo.mpActor) == PROC_E_WB) {
                i_this->field_0x9f6 = cM_rndFX(8000.0f);
                enemy->speed.y = cM_rndF(15.0f) + (35.0f + TREG_F(17));
                i_this->jump_z = -(i_this->AtInfo.mpActor->speedF * 0.5f);
            } else {
                i_this->field_0x9f6 = cM_rndFX(1000.0f);
            }
        }
    }

    i_this->damage_flag = 0;
    i_this->damage_timer = 1000;

}

static void small_damage(e_rd_class* i_this, int param_2) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    if (i_this->action != ACTION_S_DAMAGE) {
        i_this->prev_action = i_this->action;
    }

    i_this->action = ACTION_S_DAMAGE;
    i_this->mode = 0;
    i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_DAMAGE, -1);

    if (param_2 == 0) {
        s16 range = enemy->shape_angle.y - i_this->angleY;
        if (range < -0x4000 || range > 0x4000) {
                anm_init(i_this, BCK_RD_DAMAGE_W, 2.0f, 0, 1.0f);
            } else if (range < 0) {
                anm_init(i_this, BCK_RD_DAMAGE_L, 2.0f, 0, 1.0f);
            } else {
                anm_init(i_this, BCK_RD_DAMAGE_R, 2.0f, 0, 1.0f);
            }
    } else {
        anm_init(i_this, BCK_RD_DAMAGE_W, 2.0f, 0, 1.0f);
    }

    i_this->field_0x9f0 = 20.0f + TREG_F(10);
    i_this->field_0x9f4 = i_this->AtInfo.mHitDirection.y;
}

static void part_break(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz unused_vec_0, unused_vec_1;

    for (int i = 0; i < 14; i++) {
        if (i_this->field_0x70c[i] == 0) {
            i_this->field_0x70c[i] = 1;
            i_this->sound.startCreatureSound(Z2SE_EN_RDB_BREAK_ARMOR, 0, -1);

            cXyz sp54 = i_this->field_0x71c[i] - enemy->current.pos;
            cMtx_YrotS(*calc_mtx, cM_atan2s(sp54.x, sp54.z));
            sp54.x = cM_rndFX(10.0f);
            sp54.y = cM_rndF(5.0f) + 20.0f;
            sp54.z = cM_rndF(10.0f) + 30.0f;
            MtxPosition(&sp54, &i_this->field_0x7c4[i]);

            i_this->field_0x86c[i].y = cM_rndF(65536.0f);
            i_this->field_0x86c[i].x = cM_rndF(65536.0f);

            u16 uVar1;
            if (i_this->AtInfo.mHitStatus != 0) {
                uVar1 = 3;
            } else {
                uVar1 = 1;
            }

            dComIfGp_setHitMark(uVar1, enemy, &i_this->field_0x71c[i], &enemy->shape_angle, NULL, 0);
            return;
        }
    }
}

static void damage_check(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    daPy_py_c* pla = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* actor = fopAcM_SearchByID(i_this->boar_id);
    e_wb_class* boar = (e_wb_class*)actor;

    if (i_this->ride_mode == 2 && boar != NULL && (boar->mStatusFlags & 0xC0) != 0) {
        if (i_this->action == ACTION_IKKI2_END) {
            i_this->mode = 10;
        } else {
            i_this->action = ACTION_DAMAGE;
            i_this->mode = 0;
            i_this->damage_timer = 1000;

            if ((boar->mStatusFlags & 0x80) != 0) {
                i_this->jump_angle.y = enemy->shape_angle.y + 0x8000 + (s16)cM_rndFX(3000.0f);
                i_this->field_0x9f6 = (s16)cM_rndFX(1000.0f);
            } else {
                i_this->jump_angle.y = enemy->shape_angle.y + (s16)cM_rndFX(8000.0f);
                i_this->field_0x9f6 = (s16)cM_rndFX(3000.0f);
            }

            enemy->speed.y = cM_rndF(15.0f) + (25.0f + TREG_F(13));
            i_this->jump_z = -20.0f + TREG_F(18);
        }

        return;
    }

    i_this->mStts.Move();

    if (i_this->damage_timer == 0) {
        if (i_this->actor_set != 0
#if DEBUG
            || l_HIO.invulnerable
#endif
        ) {
            enemy->health = 100;
        }

        for (int i = 0; i <= 2; i++) {
            if (i_this->cc_sph[i].ChkTgHit() != 0) {
                i_this->damage_timer = 6;
                i_this->AtInfo.mpCollider = i_this->cc_sph[i].GetTgHitObj();
                if (i_this->actor_set == 3) {
                    s16 range = enemy->shape_angle.y - i_this->angleY;
                    at_power_check(&i_this->AtInfo);
                    if (i_this->AtInfo.mpCollider->ChkAtType(AT_TYPE_ARROW) || i_this->AtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB)) {
                        u16 uVar1;
                        if (dComIfGp_getHorseActor()->speedF > 20.0f || range > 0x7800 || range < -0x7800) {
                            cc_at_check(enemy, &i_this->AtInfo);
                            dScnPly_c::setPauseTimer(0);
                            boar->mLapCount++;
                            if (boar->mLapCount >= 3) {
                                mDoAud_bgmStop(30);
                                i_this->action = ACTION_IKKI2_END;
                                i_this->mode = 0;
                                i_this->sound.startCreatureVoice(Z2SE_EN_RDB_V_DAMAGE_L, -1);
                            } else {
                                boar->field_0x169e = 25;
                                anm_init(i_this, e_rdb_class::BCK_RB_RDAMAGEB, 2.0f, 0, 1.0f);
                                i_this->sound.startCreatureVoice(Z2SE_EN_RDB_V_DAMAGE, -1);
                            }

                            uVar1 = 3;
                            boar->field_0x698[3] = 100;
                            dComIfGs_onSaveDunSwitch(7);
                        } else {
                            uVar1 = 2;
                            anm_init(i_this, e_rdb_class::BCK_RB_RGUARD_F, 2.0f, 0, 1.0f);
                        }

                        cXyz mae, ato;
                        cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
                        mae.set(0.0f, nREG_F(9) + 150.0f, nREG_F(10) + 200.0f);
                        MtxPosition(&mae, &ato);

                        ato += enemy->current.pos;
                        cXyz sc(2.0f, 2.0f, 2.0f);
                        dComIfGp_setHitMark(uVar1, enemy, &ato, NULL, &sc, 0);
                    } else if (i_this->AtInfo.mHitType == 1) {
                        anm_init(i_this, e_rdb_class::BCK_RB_RGUARD_F, 2.0f, 0, 1.0f);
                        boar->field_0x169e = 25;
                        dScnPly_c::setPauseTimer(0);

                        cXyz mae, ato;
                        cMtx_YrotS(*calc_mtx, actor->shape_angle.y);

                        s16 angle_diff = enemy->shape_angle.y - i_this->angleY;
                        if (angle_diff < 0) {
                            mae.set(100.0f, nREG_F(9) + 150.0f, nREG_F(10) + 400.0f);
                        } else {
                            mae.set(-100.0f, nREG_F(9) + 150.0f, nREG_F(10) + 400.0f);
                        }
                        MtxPosition(&mae, &ato);

                        ato += enemy->current.pos;
                        cXyz sc(2.0f, 2.0f, 2.0f);
                        dComIfGp_setHitMark(2, enemy, &ato, NULL, &sc, 0);
                        dComIfGp_getVibration().StartShock(8, 31, cXyz(0.0f, 1.0f, 0.0f));
                        i_this->sound.startCollisionSE(Z2SE_HIT_SWORD, 40);

                        i_this->damage_timer = 30;
                    }

                    return;
                }

                i_this->field_0x129c = 3;

                if (i_this->AtInfo.mpCollider->ChkAtType(AT_TYPE_10000000)) {
                    wolfkick_damage(i_this);
                    break;
                }

                    at_power_check(&i_this->AtInfo);
                    if (i_this->AtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                        i_this->timer[1] = 30;
                    } else {
                        i_this->timer[1] = 1;
                    }

                    if (i_this->ride_mode == 2 && i_this->AtInfo.mAttackPower <= 10) {
                        if (fpcM_Search(s_boom_sub, i_this) != NULL) {
                            return;
                        }

                        i_this->field_0x99a = 5;
                    }

                    s16 cur_health = enemy->health; // unused
                    OS_REPORT(" HIT AT  %d\n", i_this->AtInfo.mAttackPower);
                    if (i_this->one_hit_kill != 0 && (i_this->AtInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT) || 
                        (i_this->AtInfo.mHitType != 1 && i_this->AtInfo.mHitType != 16 && i_this->AtInfo.mHitType != 2))) {
                        i_this->damage_timer = 1000;
                        i_this->action = ACTION_DROP;
                        i_this->mode = 0;
                        enemy->health = 0;
                        cc_at_check(enemy, &i_this->AtInfo);
                        return;
                    }

                    if (i_this->AtInfo.mHitType == 15) {
                        enemy->health = 0;
                        if (i_this->ride_mode == 0) {
                            i_this->action = ACTION_A_DAMAGE;
                            i_this->mode = 0;
                            i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_DAMAGE, -1);
                            enemy->health = 0;
                            cc_at_check(enemy, &i_this->AtInfo);
                            return;
                        }
                    }

                    cc_at_check(enemy, &i_this->AtInfo);
                    if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_HEAD_JUMP) {
                        enemy->health = 0;
                    }

                    if (i_this->AtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                        i_this->damage_timer = 20;
                    } else {
                        i_this->damage_timer = 10;
                    }

                    if (i_this->AtInfo.mAttackPower <= 1) {
                        i_this->damage_timer = 10 + KREG_S(8);
                    }

                    i_this->field_0xa20 |= i_this->AtInfo.mHitBit;

                    if (daPy_getPlayerActorClass()->checkHorseRide() != 0 && dComIfGp_getHorseActor()->speedF >= 20.0f && i_this->AtInfo.mHitType == 1) {
                        i_this->AtInfo.mAttackPower = 20;
                    }

                    if (enemy->health <= 0 || i_this->AtInfo.mHitStatus != 0 || i_this->ride_mode != 0) {
                        if (pla->getCutType() == daPy_py_c::CUT_TYPE_JUMP && pla->checkCutJumpCancelTurn()) {
                            small_damage(i_this, i);
                            i_this->damage_timer = 3 + NREG_S(7);
                        } else {
                            if (i_this->actor_set != 0 && boar != NULL) {
                                i_this->damage_timer = 20;
                                if (i_this->actor_set == 2) {
                                    boar->mLapCount++;
                                    if (boar->mLapCount == 1) {
                                        enemy->health = 0;
                                        /* dSv_event_flag_c::M_055 - Main Event - Did damage at least once during joust/one-on-one battle */
                                        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[0x58]);
                                        part_break(i_this);
                                        boar->field_0x169e = 20;
                                        dScnPly_c::setPauseTimer(0);
                                    }

                                    if (l_HIO.one_hit_kill != 0 || boar->mLapCount >= 2) {
                                        mDoAud_bgmStop(30);
                                        i_this->action = ACTION_IKKI_END;
                                        i_this->mode = 0;

                                        if ((i_this->AtInfo.mHitBit & 0x80) != 0) {
                                            i_this->field_0x9f6 = 0x300 + YREG_S(0);
                                        } else {
                                            i_this->field_0x9f6 = cM_rndFX(100.0f);
                                        }

                                        dScnPly_c::setPauseTimer(10);
                                    } else {
                                        if (i_this->field_0x680 == 0) {
                                            int anm_idx = i_this->anm;
                                            anm_init(i_this, e_rdb_class::BCK_RB_RDOWNB, 2.0f, 0, 1.0f);
                                            i_this->field_0x680 = anm_idx;
                                        }

                                        i_this->sound.startCreatureVoice(Z2SE_EN_RDB_V_DAMAGE_L, -1);
                                    }

                                    return;
                                }

                                boar->mLapCount++;
                                boar->mPathAdjustCounter++;
                                part_break(i_this);
                                if (boar->mLapCount == 1 || boar->mLapCount == 2 || boar->mLapCount == 3 ||
                                    boar->mLapCount == 4 || boar->mLapCount == 6) {
                                    part_break(i_this);
                                }

                                s8 life = 8 - boar->mLapCount;
                                if (life > 8) {
                                    life = 8;
                                }
                                dComIfGs_BossLife_public_Set(life);

                                if (boar->mLapCount == 5) {
                                    Z2GetAudioMgr()->changeSubBgmStatus(2);
                                }

                                if (boar->mLapCount >= 8) {
                                    i_this->action = ACTION_KIBA_END;
                                    i_this->mode = 0;
                                    mDoAud_seStart(Z2SE_EN_RDB_V_FAINT, 0, 0, 0);
                                    boar->mActionID = 31;
                                    boar->mActionMode = 0;
                                    mDoAud_bgmStop(30);
                                    return;
                                }

                                i_this->sound.startCreatureVoice(Z2SE_EN_RDB_V_DAMAGE, -1);

                                if (i_this->anm == e_rdb_class::BCK_RB_RPLAY_RUNL || i_this->anm == e_rdb_class::BCK_RB_RPLAY_RUNR 
                                    || i_this->anm == e_rdb_class::BCK_RB_RPLAY_STOP) {
                                    i_this->anm = i_this->field_0x680;
                                    i_this->field_0x680 = 0;
                                }

                                if (i_this->field_0x680 == 0) {
                                    i_this->timer[3] = cM_rndF(100.0f) + 200.0f;
                                    int anm_no = i_this->anm;

                                    s16 angle_diff = enemy->shape_angle.y - i_this->angleY;
                                    if (angle_diff < 0) {
                                        anm_init(i_this, e_rdb_class::BCK_RB_RDAMAGEL, 2.0f, 0, 1.0f);
                                    } else {
                                        anm_init(i_this, e_rdb_class::BCK_RB_RDAMAGER, 2.0f, 0, 1.0f);
                                    }

                                    i_this->field_0x680 = anm_no;
                                }

                                if (i_this->action != ACTION_WB_RUN) {
                                    i_this->action = ACTION_WB_RUN;
                                    i_this->mode = 0;
                                }
                                break;  
                            }

                            if (i_this->ride_mode != 0 && i_this->AtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                                i_this->damage_timer = 20;
                                if (i_this->action == ACTION_WB_RUN) {
                                    anm_init(i_this, BCK_RD_RRUN02_BACK, 5.0f, 2, 1.0f);
                                    i_this->mode = 50;
                                    i_this->timer[3] = cM_rndF(20.0f) + 50.0f;
                                }
                            } else {
                                big_damage(i_this);
                            }
                        }
                    } else {
                        int iVar2 = i;
                        if (i_this->AtInfo.mHitType == 16) {
                            iVar2 = 0;
                        }

                        small_damage(i_this, iVar2);
                    }

                    enemy->speedF = 0.0f;
                    if (i_this->bow_anm != NULL) {
                        i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 10), 0, 1.0f, 1.0f, 0.0f, -1.0f);
                    }
                    break;
            }
        }

        for (int j = 0; j <= 2; j++) {
            if (enemy->health <= 1) {
                i_this->cc_sph[j].SetTgHitMark(CcG_Tg_UNK_MARK_3);
            }
        }
    }
}

static void* s_other_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && ((e_rd_class*)i_actor)->arg1 != 6) {
        return i_actor;
    }

    return NULL;
}

static void* s_ep_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_Lv1Cdl00) {
        if (!fopAcM_isSwitch((fopAc_ac_c*)i_actor, fopAcM_GetParam(i_actor) & 0xFF)) {
            f32 x, z;
            x = ((fopAc_ac_c*)i_actor)->current.pos.x - player->current.pos.x;
            z = ((fopAc_ac_c*)i_actor)->current.pos.z - player->current.pos.z;
            if (JMAFastSqrt(x * x + z * z) < NREG_F(17) + 600.0f) {
                return i_actor;
            }
        }
    }

    return NULL;
}

static s8 e_rd_yagura(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz unused_vec_0, unused_vec_1;
    int frame = i_this->anm_p->getFrame();
    s8 rt = 1;
    s8 bVar1 = 0;

    fpcM_Search(s_b_sub, i_this);

    switch (i_this->mode) {
        case 0:
            if (i_this->yagura_timer != 0) {
                enemy->home.pos.y = enemy->current.pos.y;
                return 0;
            }

            rt = 0;
            bVar1 = 1;

            if (i_this->timer[0] == 0) {
                anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
                i_this->mode = 1;
                i_this->timer[0] = cM_rndF(40.0f) + 60.0f;
            }
            break;

        case 1:
            rt = 0;
            bVar1 = 1;

            if (i_this->timer[0] == 0) {
                anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
                i_this->timer[0] = cM_rndF(100.0f) + 100.0f;
                i_this->mode = 0;
            }
            break;

        case 5:
            i_this->field_0x9c8 = 4;

            if (i_this->timer[0] == 0) {
                anm_init(i_this, BCK_RD_SHOUT, 5.0f, 0, 1.0f);
                i_this->mode = 10;
                i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_FIND_LINK_YELL, -1);
                dKy_Sound_set(enemy->current.pos, 100, fopAcM_GetID(i_this), 3);
            }
            break;

        case 10:
            if (i_this->anm_p->isStop()) {
                anm_init(i_this, BCK_RD_SHOOT_READY, 5.0f, 0, 1.0f);
                i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 7),
                                             0, 5.0f, 0.9f + TREG_F(3), 0.0f, -1.0f);
                i_this->mode = 11;
                enemy->speedF = 0.0f;
            }
            break;

        case 11:
            i_this->field_0x9c8 = 4;

            if (frame >= 10) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 20) {
                i_this->sound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
            }

            if (i_this->anm_p->isStop()) {
                anm_init(i_this, BCK_RD_SHOOT_WAIT, 4.0f, 2, 1.0f);
                i_this->timer[0] = cM_rndF(10.0f) + 10.0f;
                i_this->mode = 12;
            }
            break;

        case 12:
            i_this->field_0x9c8 = 4;

            if (i_this->timer[0] == 0 && i_this->field_0x9a4 == 0) {
                anm_init(i_this, BCK_RD_SHOOT, 1.0f, 0, 1.0f);
                i_this->bow_anm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, 10), 0, 1.0f, 1.0f, 0.0f, -1.0f);
                i_this->mode = 13;
            }

            i_this->field_0x9a2 = 1;
            break;

        case 13:
            if (frame <= 3 + XREG_S(5)) {
                i_this->field_0x9a2 = 1;
                i_this->field_0x9c8 = 4;
            }

            if (frame == 2 + XREG_S(7)) {
                i_this->arrow_flag = 1;
            }

            if (i_this->anm_p->isStop()) {
                if (pl_check(i_this, 10000.0f, 0x7FFF)) {
                    i_this->mode = 10;
                    anm_init(i_this, BCK_RD_SHOUT, 5.0f, 0, 1.0f);
                    dKy_Sound_set(enemy->current.pos, 100, fopAcM_GetID(i_this), 3);
                } else {
                    i_this->mode = 0;
                }
            }
            break;

        case 14:
            break;
    }

    if (fabsf(i_this->ObjAcch.GetGroundH() - enemy->home.pos.y) > 10.0f) {
        i_this->action = ACTION_DAMAGE;
        i_this->mode = 0;
        i_this->jump_angle.y = s16(enemy->shape_angle.y);
        enemy->speed.y = 20.0f;
        i_this->jump_z = -20.0f;
        i_this->damage_timer = 1000;
        i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_DEATH, -1);
        enemy->health = 0;
        return 0;
    }

    if (bVar1 && pl_check(i_this, i_this->attack_range, 0x4000) && (i_this->attack_range >= 8000.0f || fpcM_Search(s_ep_sub, i_this) != NULL)) {
        anm_init(i_this, BCK_RD_WAIT01, 4.0f, 2, 1.0f);
        i_this->timer[0] = 30;
        i_this->mode = 5;
        if (S_find == 0 && !lbl_70_bss_AE && !daPy_getPlayerActorClass()->checkBoarRide()
            && !dComIfGp_event_runCheck() && i_this->demo_mode == 0
            && fpcM_Search(s_other_sub, i_this) != NULL) {
            i_this->demo_mode = 1;
        }
    }

    if (rt != 0) {
        cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->angleY, 4, 0x1000);
    }

    return rt;
}

static void e_rd_jyunkai(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    int frame = i_this->anm_p->getFrame(); // unused
    f32 speed = 0.0f;
    s16 angle = 0x4000;
    cXyz vec, sp64;

    switch (i_this->mode) {
        case 0: {
            anm_init(i_this, BCK_RD_WALK, 10.0f, 2, 1.0f);
            i_this->mode = 1;
            dPnt* pnt_p = i_this->path->m_points;
            pnt_p += i_this->field_0xb00;
            i_this->field_0x5c0.x = pnt_p->m_position.x;
            i_this->field_0x5c0.y = pnt_p->m_position.y;
            i_this->field_0x5c0.z = pnt_p->m_position.z;
        }
        // fallthrough
        case 1:
            speed = l_HIO.movement_speed;
            vec = i_this->field_0x5c0 - enemy->current.pos;
            i_this->target_ya = cM_atan2s(vec.x, vec.z);

            if (vec.abs() < 50.0f) {
                i_this->field_0xb00 += i_this->path_dir;
                if (i_this->field_0xb00 >= (i_this->path->m_num & 0xFF)) {
                    if (dPath_ChkClose(i_this->path)) {
                        i_this->field_0xb00 = 0;
                    } else {
                        i_this->path_dir = 0xFF;
                        i_this->field_0xb00 = i_this->path->m_num - 2;
                    }
                } else if (i_this->field_0xb00 < 0) {
                    i_this->path_dir = 1;
                    i_this->field_0xb00 = 1;
                }

                i_this->mode = 2;
                i_this->timer[0] = cM_rndF(100.0f) + 100.0f;
                anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
            }
            break;

        case 2:
            angle = 0x6000;

            if (i_this->timer[0] == 0) {
                i_this->mode = 0;
            }
            break;

        case 10:
            anm_init(i_this, BCK_RD_SHOUT, 5.0f, 0, 1.0f);
            i_this->mode = 11;
            dKy_Sound_set(enemy->current.pos, 100, fopAcM_GetID(i_this), 3);
            S_find = 20;
            break;

        case 11:
            cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->angleY, 2, 0x800);
            if (i_this->anm_p->isStop()) {
                i_this->action = ACTION_FIGHT_RUN;
                i_this->mode = -10;
                i_this->timer[0] = 60;
                return;
            }
    }

    if (speed) {
        cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->target_ya, 8, 0x400);
        s16 range = enemy->current.angle.y - i_this->target_ya;
        if (range > 0x400 || range < -0x400) {
            speed = 0.0f;
        }
    }

    cLib_addCalc2(&enemy->speedF, speed, 1.0f, 3.0f);

    f32 range = i_this->attack_range;
    BOOL bVar1 = FALSE;

    if (i_this->look_timer == 1|| S_find != 0) {
        if (i_this->look_timer == 1) {
            bVar1 = TRUE;
        }

        range = 10000.0f;
        angle = 0x7FFF;
    }

    if (i_this->mode < 10) {
        if (bVar1) {
            i_this->mode = 2;
            i_this->timer[0] = cM_rndF(100.0f) + 100.0f;
            anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
        } else if ((i_this->counter & 7) == 0 && pl_check(i_this, range, angle)) {
            if (S_find == 0) {
                i_this->mode = 10;
            } else {
                i_this->action = ACTION_FIGHT_RUN;
                i_this->mode = -10;
                i_this->timer[0] = 60;
            }
        }
    }
}

static void e_rd_sleep(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    int frame = i_this->anm_p->getFrame();
    f32 range = i_this->attack_range;
    s16 sVar1 = 0x4000;
    BOOL look_check = FALSE;

    if (i_this->look_timer == 1 || S_find != 0) {
        if (i_this->look_timer == 1) {
            look_check = TRUE;
        } else if ((i_this->counter & 15) == 0) {
            look_check = TRUE;
        }

        range = 5000.0f;
        sVar1 = 0x7FFF;
    }

    s8 find = 0;

    switch (i_this->mode) {
        case 0:
            i_this->timer[0] = cM_rndF(100.0f) + 200.0f;
            anm_init(i_this, BCK_RD_SLEEP_WAIT, 10.0f, 2, 1.0f);
            i_this->mode = 1;
            break;

        case 1:
            if (look_check || i_this->timer[0] == 0) {
                anm_init(i_this, BCK_RD_SLEEP_LOOKAROUND, 5.0f, 0, 1.0f);
                i_this->mode = 2;
            }
            break;

        case 2:
            if (i_this->anm_p->isStop()) {
                i_this->mode = 0;
            }

            if (fopAcM_GetRoomNo(enemy) == 0) {
                if (look_check) {
                    wb_check(i_this, sVar1);
                }

                if (i_this->dis > 600.0f) {
                    return;
                }
            }

            if (frame > 60 && frame < 200 && (((i_this->counter & 7) == 0) || look_check) && pl_check(i_this, range, sVar1)) {
                find = 1;
            }
            break;

        case 5:
            if (i_this->timer[0] == 0) {
                find = 1;
            }
            break;

        case 10:
            anm_init(i_this, BCK_RD_SHOUT, 5.0f, 0, 1.0f);
            i_this->mode = 11;
            dKy_Sound_set(enemy->current.pos, 100, fopAcM_GetID(i_this), 3);
            S_find = 20;
            break;

        case 11:
            cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->angleY, 2, 0x800);

            if (i_this->anm_p->isStop()) {
                i_this->action = ACTION_FIGHT_RUN;
                i_this->mode = -10;
                i_this->timer[0] = 60;
                return;
            }
    }

    if (i_this->mode < 5) {
        for (int i = 0; i <= 2; i++) {
            if (i_this->cc_sph[i].ChkCoHit()) {
                cCcD_Obj* co_hit_obj_p = i_this->cc_sph[i].GetCoHitObj();
                if (daPy_getPlayerActorClass() == dCc_GetAc(co_hit_obj_p->GetAc())) {
                    find = 2;
                    break;
                }
            }
        }
    }

    if (i_this->mode < 10) {
        if (find == 1) {
            if (S_find == 0) {
                i_this->mode = 10;
            } else {
                i_this->action = ACTION_FIGHT_RUN;
                i_this->mode = -10;
                i_this->timer[0] = 60;
                OS_REPORT("......RD FIND 1!!!\n");
            }
        } else if (find == 2) {
            i_this->mode = 5;
            anm_init(i_this, BCK_RD_KYORO2, 7.0f, 2, 1.5f);
            i_this->timer[0] = cM_rndF(10.0f) + 20.0f;
            OS_REPORT("......RD FIND 2!!!\n");
        }
    }

    if (i_this->attack_range >= 9900.0f) {
        i_this->action = ACTION_STAND;
        i_this->mode = 0;
        i_this->timer[0] = 0;
    }
}

static int rd_count;

static void* s_tag_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && ((e_rd_class*)i_actor)->field_0x5ba != 0) {
        rd_count++;
    }

    return NULL;
}

static void e_rd_tag(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    fopAc_ac_c* pla = dComIfGp_getPlayer(0);

    i_this->field_0x9ad = 0;
    i_this->damage_timer = 10;

    switch (i_this->mode) {
        case 0:
            break;
    }

    if (S_find != 0 && (i_this->counter & 31) == 0 && !dComIfGp_event_runCheck() && i_this->dis < 3000.0f && i_this->dis > 500.0f && 
        i_this->field_0x5bb != 0 && !fopAcM_otherBgCheck(enemy, pla)) {
        rd_count = 0;
        fpcM_Search(s_tag_sub, i_this);

        if (rd_count < 6) {
            csXyz angl(0, i_this->angleY, 0);
            cXyz pos;
            pos.x = cM_rndFX(200.0f) + enemy->home.pos.x;
            pos.y = enemy->home.pos.y;
            pos.z = cM_rndFX(200.0f) + enemy->home.pos.z;

            fpc_ProcID parameter;
            if ((rd_count & 3) == 3) {
                parameter = 0xFF011323;
            } else {
                parameter = 0xFF011123;
            }
            fopAcM_createChild(PROC_E_RD, fopAcM_GetID(enemy), parameter, &pos, fopAcM_GetRoomNo(enemy), &angl, NULL, -1, NULL);
        }
    }
}

static void e_rd_reg(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = &i_this->enemy;
    fopAc_ac_c* pla = dComIfGp_getPlayer(0);

    i_this->field_0x9ad = 0;
    i_this->damage_timer = 10;

    if (i_this->field_0xafb == 0) {
        i_this->action = ACTION_BOW2;
        i_this->mode = 1;
        i_this->attack_range = 0.0f;
        return;
    }

    if (i_this->mode != 0) {
        return;
    }

    if (i_this->arg1 != 14) {
        return;
    }

    if (i_this->demo_mode == 0) {
        f32 x, z;
        x = pla->current.pos.x - -5551.0f;
        z = pla->current.pos.z - 5525.0f;
        if (JMAFastSqrt(SQUARE(x)+ SQUARE(z)) < 600.0f) {
            i_this->demo_mode = 20;
        }
    }
}

static void action(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    fopAc_ac_c* actor = get_pla(enemy);
    cXyz mae, ato;

    i_this->field_0x9c8 = 0;

    if (actor == dComIfGp_getPlayer(0)) {
        i_this->dis = fopAcM_searchPlayerDistance(enemy);
        if (daPy_getPlayerActorClass()->checkHorseRide()) {
            i_this->dis -= BREG_F(17) + 100.0f;
        }

        i_this->angleY = fopAcM_searchPlayerAngleY(enemy);
    } else {
        mae = actor->current.pos - enemy->current.pos;
        i_this->dis = mae.abs();
        i_this->angleY = cM_atan2s(mae.x, mae.z);
        i_this->attack_range = NREG_F(7) + 10000.0f;
    }

    if (desert_substage != 0) {
        if (i_this->field_0x9a0 == 0) {
            dScnKy_env_light_c* kankyo = dKy_getEnvlight();
            int timeH = kankyo->daytime / 15.0f;
            if (timeH >= 6 && timeH <= 17) {
                i_this->attack_range = 10000.0f;
            }
        } else {
            i_this->field_0x9a0--;
        }
    }

    s16 curr_action = i_this->action;
    damage_check(i_this);
    if (curr_action == ACTION_WB_SEARCH && i_this->action != ACTION_WB_SEARCH) {
        e_wb_class* boar = (e_wb_class*)fopAcM_SearchByID(i_this->boar_id);
        if (boar != NULL && boar->mActionID == 1) {
            boar->mActionID = 0;
        }
    }

    u8 cVar1 = i_this->field_0x5ba;
    s8 cVar2 = 0;
    s8 bVar1 = 1;
    s8 cVar3 = 0;
    i_this->enemy.offHeadLockFlg();

    switch (i_this->action) {
        case ACTION_NORMAL:
            e_rd_normal(i_this);
            cVar3 = 1;
            break;

        case ACTION_FIGHT_RUN:
            e_rd_fight_run(i_this);
            cVar2 = 1;
            break;

        case ACTION_FIGHT:
            e_rd_fight(i_this);
            cVar2 = 1;
            break;

        case ACTION_BOW_RUN:
            e_rd_bow_run(i_this);
            cVar2 = 1;
            break;

        case ACTION_BOW:
            e_rd_bow(i_this);
            cVar2 = 1;
            break;

        case ACTION_BOW2:
            cVar3 = e_rd_bow2(i_this);
            cVar2 = 1;
            break;

        case ACTION_BOW_IKKI:
            e_rd_bow_ikki(i_this);
            bVar1 = 0;
            break;

        case ACTION_AVOID:
            e_rd_avoid(i_this);
            break;

        case ACTION_WB_SEARCH:
            e_rd_wb_search(i_this);
            break;

        case ACTION_WB_RIDE:
            e_rd_wb_ride(i_this);
            cVar1 = 0;
            break;

        case ACTION_WB_RUN:
            if (i_this->actor_set != 0) {
                e_rd_wb_run_B(i_this);
            } else {
                e_rd_wb_run(i_this);
            }
            cVar1 = 0;
            break;

        case ACTION_WB_BJUMP:
            e_rd_wb_bjump(i_this);
            cVar1 = 0;
            break;

        case ACTION_BOMB:
            e_rd_bomb_action(i_this);
            break;

        case ACTION_S_DAMAGE:
            e_rd_s_damage(i_this);
            break;

        case ACTION_DAMAGE:
            e_rd_damage(i_this);
            break;

        case ACTION_DROP:
            e_rd_drop(i_this);
            break;

        case ACTION_A_DAMAGE:
            e_rd_a_damage(i_this);
            break;

        case ACTION_STAND:
            e_rd_stand(i_this);
            cVar3 = 1;
            break;

        case ACTION_COMMANDER:
            e_rd_commander(i_this);
            cVar2 = 1;
            break;

        case ACTION_BOW3:
            cVar3 = e_rd_bow3(i_this);
            cVar2 = 1;
            break;

        case ACTION_EXCITE:
            e_rd_excite(i_this);
            break;

        case ACTION_WATER:
            e_rd_water(i_this);
            bVar1 = 0;
            break;

        case ACTION_KIBA_START:
            e_rd_kiba_start(i_this);
            cVar1 = 0;
            break;

        case ACTION_KIBA_END:
            e_rd_kiba_end(i_this);
            cVar1 = 0;
            break;

        case ACTION_IKKI_END:
            e_rd_ikki_end(i_this);
            break;

        case ACTION_IKKI2_START:
            e_rd_ikki2_start(i_this);
            break;

        case ACTION_IKKI2_END:
            e_rd_ikki2_end(i_this);
            break;

        case ACTION_LV9_END:
            e_rd_lv9_end(i_this);
            break;

        case ACTION_YAGURA:
            cVar2 = e_rd_yagura(i_this);
            break;

        case ACTION_JYUNKAI:
            e_rd_jyunkai(i_this);
            break;

        case ACTION_SLEEP:
            e_rd_sleep(i_this);
            break;

        case ACTION_TAG:
            e_rd_tag(i_this);
            break;

        case ACTION_REG:
            e_rd_reg(i_this);
    }

    if (cVar3 != 0 && otoCheck(enemy, 1500.0f)) {
        SND_INFLUENCE* sound = dKy_Sound_get();
        if (!pl_check(i_this, 500.0f, 0x7FFF)) {
            mae = sound->position - enemy->current.pos;
            if (mae.abs() < sound->field_0xc * 120) {
                i_this->prev_action = i_this->action;
                i_this->action = ACTION_EXCITE;
                i_this->mode = 0;
                S_find_pos = sound->position;
            }
        }
    }

    if (bVar1) {
        dBgS_ObjGndChk obj_gnd_chk;
        dBgS_ObjGndChk_Spl spl_chk;
        mae = enemy->current.pos;
        mae.y += 150.0f;
        spl_chk.SetPos(&mae);
        obj_gnd_chk.SetPos(&mae);

        i_this->waterY = dComIfG_Bgsp().GroundCross(&spl_chk);

        f32 waterY = i_this->waterY - dComIfG_Bgsp().GroundCross(&obj_gnd_chk);
        if (enemy->current.pos.y < i_this->waterY && waterY > 100.0f) {
            i_this->action = ACTION_WATER;
            if (dComIfG_Bgsp().GetPolyAtt0(spl_chk) == 6) {
                i_this->mode = 1;
            } else {
                i_this->mode = 0;
            }

            enemy->current.pos.y = i_this->waterY;
        }
    }

    if (cVar2 != 0) {
        i_this->sound.setLinkSearch(true);
        if (desert_substage != 0 && (i_this->arg1 == 6 || i_this->arg1 == 7 || i_this->arg1 == 8)) {
            S_find = 20;
        }
    } else {
        i_this->sound.setLinkSearch(false);
    }

    if (cVar1) {
        if (desert_substage != 0) {
            if (!dComIfGp_event_runCheck() && i_this->yagura_timer == 0 && i_this->dis > 700.0f && i_this->field_0x5bb != 0) {
                fopAcM_delete(enemy);
            }
        } else if (daPy_getPlayerActorClass()->checkHorseRide() && dComIfGp_getHorseActor()->speedF >= 30.0f && i_this->field_0x5bb != 0) {
            fopAcM_delete(enemy);
        }
    }

    cLib_addCalcAngleS2(&enemy->shape_angle.y, enemy->current.angle.y, 2, 0x1000);
    cLib_addCalcAngleS2(&enemy->shape_angle.x, enemy->current.angle.x, 2, 0x1000);
    cLib_addCalcAngleS2(&enemy->shape_angle.z, enemy->current.angle.z, 2, 0x1000);


    if (i_this->ride_mode == 0) {
        enemy->attention_info.distances[fopAc_attn_BATTLE_e] = 3;
        if (i_this->jump_z) {
            cMtx_YrotS(*calc_mtx, i_this->jump_angle.y);
            if (i_this->field_0xaf0 == 0) {
                mae.z = i_this->jump_z;
            } else {
                mae.z = 0.0f;
                enemy->speed.y = 0.0f;
            }

            enemy->gravity = -(l_HIO.jump_g - 1.0f);
        } else {
            cMtx_YrotS(*calc_mtx, enemy->current.angle.y);
            mae.z = enemy->speedF;
            enemy->gravity = -5.0f;
        }
        mae.x = 0.0f;
        mae.y = 0.0f;
        MtxPosition(&mae, &ato);

        enemy->speed.x = ato.x;
        enemy->speed.z = ato.z;
        enemy->current.pos += enemy->speed;
        enemy->speed.y += enemy->gravity;
        if (enemy->speed.y < -100.0f) {
            enemy->speed.y = -100.0f;
        }

        if (i_this->field_0x9ad != 0) {
            cXyz* pcVar1 = i_this->mStts.GetCCMoveP();
            if (pcVar1 != NULL) {
                enemy->current.pos.x += pcVar1->x;
                enemy->current.pos.z += pcVar1->z;
            }
        }

        cLib_addCalc0(&i_this->field_0x9c0, 1.0f, 3.0f);
        
        if (i_this->field_0x9f0 > 0.1f) {
            mae.x = 0.0f;
            mae.y = 0.0f;
            mae.z = -i_this->field_0x9f0;
            cMtx_YrotS(*calc_mtx, i_this->field_0x9f4);
            MtxPosition(&mae, &ato);
            enemy->current.pos += ato;
            cLib_addCalc0(&i_this->field_0x9f0, 1.0f, 7.0f + TREG_F(12));
        }
    } else {
        enemy->attention_info.distances[fopAc_attn_BATTLE_e] = 34;
        fopAc_ac_c* actor = fopAcM_SearchByID(i_this->boar_id);
        if (actor == NULL) {
            i_this->action = ACTION_NORMAL;
            i_this->mode = 0;
            return;
        }

        enemy->speedF = actor->speedF;
        if (daPy_getPlayerActorClass()->checkHorseRide() && dComIfGp_getHorseActor()->speedF >= 30.0f) {
            enemy->speed = actor->speed;
            enemy->speed.y = 0.0f;
        } else {
            enemy->speed.set(0.0f, 0.0f, 0.0f);
        }

        e_wb_class* boar = (e_wb_class*)actor;
        if (i_this->boar_stand == 1) {
            MTXCopy(boar->mpModelMorf->getModel()->getAnmMtx(boar->field_0x688 + 15), *calc_mtx);
        } else {
            MTXCopy(boar->mpModelMorf->getModel()->getAnmMtx(boar->field_0x688 + 16), *calc_mtx);
        }

        if (i_this->actor_set != 0) {
            mae.set(0.0f, BREG_F(4) + -65.0f, 0.0f);
        } else {
            mae.set(0.0f, BREG_F(4) + -45.0f, 0.0f);
        }
        MtxPosition(&mae, &i_this->field_0x9b0);

        if (i_this->ride_mode == 2) {
            enemy->current.pos = i_this->field_0x9b0;
            enemy->shape_angle = enemy->current.angle = actor->shape_angle;

            mae.x = 10.0f;
            MtxPosition(&mae, &ato);
            mae = ato - i_this->field_0x9b0;
            ANGLE_ADD(enemy->current.angle.x,
                      -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z)));
            enemy->shape_angle.x = enemy->current.angle.x;

            if (i_this->boar_stand == 1) {
                boar->field_0x1434 = fopAcM_GetID(i_this);
            }
        } else {
            mae = i_this->field_0x9b0 - enemy->current.pos;
            cMtx_YrotS(*calc_mtx, cM_atan2s(mae.x, mae.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z)));
            mae.x = 0.0f;
            mae.y = 0.0f;
            mae.z = TREG_F(3) + 20.0f;
            MtxPosition(&mae, &ato);

            enemy->current.pos += ato;
            mae = i_this->field_0x9b0 - enemy->current.pos;
            if (mae.abs() < 30.0f || i_this->timer[3] == 0) {
                i_this->ride_mode = 2;
            }

            cLib_addCalcAngleS2(&enemy->current.angle.y, actor->shape_angle.y, 4, 0x800);
        }

        i_this->field_0x9c0 += i_this->field_0x9c4;
        i_this->field_0x9c4 -= 4.5f + TREG_F(5);

        if (i_this->field_0x9c0 <= 0.0f) {
            i_this->field_0x9c0 = 0.0f;
            i_this->field_0x9c4 = 0.0f;
        }
    }

    s16 sp_0x2a = 0;
    s16 sp_0x28 = 0;
    s16 sp_0x26 = 16000;
    i_this->field_0x9a4 = 0;
    cXyz ato2;
    s16 sp_0x24 = 0x1000;

    if (i_this->field_0x9c8 == -1) {
        cMtx_YrotS(*calc_mtx, enemy->shape_angle.y);
        mae.x = 0.0f;
        mae.y = KREG_F(8) + 70.0f;
        mae.z = 50.0f;
        MtxPosition(&mae, &ato2);

        ato2 += enemy->current.pos;
        i_this->field_0x9c8 = 2;
        sp_0x24 = KREG_S(7) + 0x800;
    } else if (i_this->field_0x9c8 >= 11) {
        i_this->field_0x9c8 -= (s8) 10;
        ato2 = i_this->eye;
    } else {
        ato2 = actor->eyePos;
    }

    if (i_this->ride_mode == 2) {
        sp_0x24 = 0x1000;
    }

    if (i_this->field_0x9c8 != 0) {
        mae = ato2 - enemy->current.pos;
        mae.y += TREG_F(2) + -150.0f;
        sp_0x2a = cM_atan2s(mae.x, mae.z) - enemy->shape_angle.y;
        sp_0x28 = enemy->shape_angle.x + cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));
        if (i_this->field_0x9c8 <= 2) {
            if (sp_0x2a > sp_0x26) {
                sp_0x2a = sp_0x26;
            } else if (sp_0x2a < (s16) -sp_0x26) {
                sp_0x2a = -sp_0x26;
            }

            if (sp_0x28 > sp_0x26) {
                sp_0x28 = sp_0x26;
            } else if (sp_0x28 < (s16) -sp_0x26) {
                sp_0x28 = -sp_0x26;
            }

            cLib_addCalcAngleS2(&i_this->field_0x9ca, sp_0x2a, 2, sp_0x24);
        } else if (i_this->field_0x9c8 == 5) {
            if ((i_this->counter & 15) == 0 && cM_rndF2(1.0f) < 0.3f) {
                i_this->field_0x9d0 = cM_rndFX2(2000.0f);
            }
        } else {
            sp_0x26 = XREG_S(5) + 30000;
            s16 sp_0x22 = (XREG_S(6) + 25000);
            if (sp_0x2a > sp_0x26) {
                sp_0x2a = sp_0x26;
                i_this->field_0x9a4 = 1;
            } else if (sp_0x2a < (s16) -sp_0x22) {
                sp_0x2a = -sp_0x22;
                i_this->field_0x9a4 = 1;
            }

            cLib_addCalcAngleS2(&i_this->field_0x9ca, sp_0x2a / 2, 2, 0x1000);
            sp_0x28 = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));
            if (sp_0x28 > 0x2AF8) {
                sp_0x28 = 11000;
                i_this->field_0x9a4 = 1;
            } else if (sp_0x28 < -8000) {
                sp_0x28 = -8000;
                i_this->field_0x9a4 = 1;
            }
        }
    } else {
        cLib_addCalcAngleS2(&i_this->field_0x9ca, 0, 2, 0x1000);
        i_this->field_0x9d0 = 0;
    }

    cLib_addCalcAngleS2(&i_this->field_0x9cc, sp_0x28, 2, sp_0x24);
    cLib_addCalcAngleS2(&i_this->field_0x9ce, i_this->field_0x9d0, 4, 0x500);

    if (fabsf(i_this->field_0xa2c) > 1.0f) {
        i_this->field_0xa74[1].x = i_this->field_0xa2c * cM_scos(i_this->field_0xa30) * (BREG_F(0) + 70.0f);
        i_this->field_0xa74[2].x += i_this->field_0xa74[1].x;
        cLib_addCalc0(&i_this->field_0xa2c, 1.0f, 3.0f + BREG_F(1));
        i_this->field_0xa30 += s16(4000 + BREG_S(0));
    }

    if (fabsf(i_this->field_0xa24) > 1.0f) {
        i_this->field_0xa74[0].x = i_this->field_0xa24 * cM_scos(i_this->field_0xa28) * (BREG_F(2) + 70.0f);
        cLib_addCalc0(&i_this->field_0xa24, 1.0f, 3.0f + BREG_F(3));
        i_this->field_0xa28 += s16(4000 + BREG_S(1));
    }

    if (enemy->speed.y < 0.0f && i_this->ObjAcch.ChkGroundHit()) {
        cLib_addCalc0(&i_this->field_0xa2c, 1.0f, 30.0f);
        cLib_addCalc0(&i_this->field_0xa24, 1.0f, 30.0f);
    }

    if (i_this->jump_z) {
        sp_0x2a = i_this->jump_angle.y - enemy->shape_angle.y;
        i_this->field_0xa74[3].y = -sp_0x2a + 0x4000;
        if (i_this->field_0xa74[3].y > 0x1770) {
            i_this->field_0xa74[3].y = 6000;
        } else if (i_this->field_0xa74[3].y < -12000) {
            i_this->field_0xa74[3].y = -12000;
        }

        i_this->field_0xa74[4].y = -sp_0x2a + 0x4000;
        if (i_this->field_0xa74[4].y > 0x3A98) {
            i_this->field_0xa74[4].y = 15000;
        } else if (i_this->field_0xa74[4].y < 0) {
            i_this->field_0xa74[4].y = 0;
        }

        i_this->field_0xa74[5].y = sp_0x2a + 0x4000;
        if (i_this->field_0xa74[5].y > 0x1770) {
            i_this->field_0xa74[5].y = 6000;
        } else if (i_this->field_0xa74[5].y < -12000) {
            i_this->field_0xa74[5].y = -12000;
        }

        i_this->field_0xa74[6].y = sp_0x2a + 0x4000;
        if (i_this->field_0xa74[6].y > 0x3A98) {
            i_this->field_0xa74[6].y = 15000;
        } else if (i_this->field_0xa74[6].y < 0) {
            i_this->field_0xa74[6].y = 0;
        }

        cMtx_YrotS(*calc_mtx, sp_0x2a);
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = 12000.0f;
        MtxPosition(&mae, &ato);
        i_this->field_0xae4.y = -ato.x;
        i_this->field_0xae4.x = -ato.z * 1.3f;

        if (i_this->field_0xae4.x > 10000) {
            i_this->field_0xae4.x = 10000;
        }
    }

    static s16 j_spd[11] = {
        0x4000, 0x4000, 0x4000,
        0x1000, 0x1000, 0x1000,
        0x1000, 0x4000, 0x4000,
        0x4000, 0x4000,
    };

    for (int i = 0; i < 11; i++) {
        cLib_addCalcAngleS2(&i_this->field_0xa32[i].x, i_this->field_0xa74[i].x, 2, j_spd[i]);
        cLib_addCalcAngleS2(&i_this->field_0xa32[i].y, i_this->field_0xa74[i].y, 2, j_spd[i]);
        cLib_addCalcAngleS2(&i_this->field_0xa32[i].z, i_this->field_0xa74[i].z, 2, j_spd[i]);
        i_this->field_0xa74[i].x = i_this->field_0xa74[i].z = 0;

        if (i >= 3 && i <= 6) {
            if (i_this->field_0xad2[i - 3] == 0) {
                s16 sp_0x20;
                if (i == 4 || i == 6) {
                    sp_0x20 = 0x600;
                } else {
                    sp_0x20 = 0x800;
                }

                cLib_addCalcAngleS2(&i_this->field_0xa74[i].y, 0, 1, sp_0x20);
            } else {
                i_this->field_0xad2[i - 3]--;
            }
        } else {
            i_this->field_0xa74[i].y = 0;
        }

        if (i >= 3) {
            i_this->field_0xabc[i] = i_this->field_0xab8 * cM_scos(i_this->counter * (BREG_S(0) + 4000) + i * (BREG_S(1) + 20000));
        }
    }

    cLib_addCalc0(&i_this->field_0xab8, 1.0f, 100.0f + BREG_F(11));
    cLib_addCalcAngleS2(&i_this->field_0xade.y, i_this->field_0xae4.y, 2, 0x800);
    cLib_addCalcAngleS2(&i_this->field_0xade.x, i_this->field_0xae4.x, 2, 0x800);
    i_this->field_0xae4.y = i_this->field_0xae4.x = 0;
    s16 sVar4 = 0;
    s16 sVar5 = 0;

    if (i_this->field_0xa1e != 0) {
        i_this->field_0xa1e--;
        if (i_this->jump_angle.x != 0) {
            Vec sp280;
            cXyz sp28c, sp298;
            dBgS_GndChk gnd_chk;
            f32 fVar2 = 75.0f;

            MTXCopy(i_this->anm_p->getModel()->getAnmMtx(1), *calc_mtx);
            mae.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&mae, &sp28c);
            sp28c.y += 100.0f;
            sp28c.y += 100.0f;
            gnd_chk.SetPos(&sp28c);
            sp28c.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

            if (sp28c.y != -G_CM3D_F_INF) {
                sp280.x = sp28c.x;
                sp280.y = sp28c.y + 100.0f;
                sp280.z = sp28c.z + fVar2;
                gnd_chk.SetPos(&sp280);
                sp280.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

                f32 x, y, z;
                if (sp280.y != -G_CM3D_F_INF) {
                    y = sp280.y - sp28c.y;
                    z = sp280.z - sp28c.z;
                    sVar5 = -cM_atan2s(y, z);
                    if (sVar5 > 0x3000 || sVar5 < -0x3000) {
                        sVar5 = 0;
                    }
                }

                sp280.x = sp28c.x + fVar2;
                sp280.y = sp28c.y + 100.0f;
                sp280.z = sp28c.z;
                gnd_chk.SetPos(&sp280);
                sp280.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

                if (sp280.y != -G_CM3D_F_INF) {
                    y = sp280.y - sp28c.y;
                    x = sp280.x - sp28c.x;
                    sVar4 = (s16) cM_atan2s(y, x);
                    if (sVar4 > 0x3000 || sVar4 < -0x3000) {
                        sVar4 = 0;
                    }
                }
            }

            dBgS_LinChk lin_chk;
            cXyz sp2a4, start, end;

            start = enemy->current.pos;
            start.y += JREG_F(5) + 30.0f;
            J3DModel* model = i_this->anm_p->getModel();
            sp2a4.set(0.0f, 0.0f, 0.0f);
            MTXCopy(model->getAnmMtx(BREG_S(8) + 13), *calc_mtx);
            MtxPosition(&sp2a4, &end);
            end.y += JREG_F(6) + 30.0f;

            lin_chk.Set(&start, &end, enemy);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                sp2a4 = start - end;
                cMtx_YrotS(*calc_mtx, cM_atan2s(sp2a4.x, sp2a4.z));
                sp2a4.x = 0.0f;
                sp2a4.y = 0.0f;
                sp2a4.z = TREG_F(11) + 50.0f;
                MtxPosition(&sp2a4, &ato);
                enemy->current.pos += ato;
            }

            i_this->field_0xa18.x = sVar5;
            i_this->field_0xa18.z = sVar4;
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0xa12.x, i_this->field_0xa18.x, 1, 0x400);
    cLib_addCalcAngleS2(&i_this->field_0xa12.z, i_this->field_0xa18.z, 1, 0x400);

    if (i_this->field_0x129c != 0) {
        i_this->field_0x129c--;
        if (i_this->field_0x129c == 0) {
            dComIfGp_particle_set(ID_ZI_J_TUBA00, &enemy->eyePos, &enemy->shape_angle, NULL);
        }
    }

    if (i_this->field_0x125d != 0) {
        J3DModel* model_p = i_this->anm_p->getModel();
        MTXCopy(model_p->getAnmMtx(11), *calc_mtx);
        mae.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&mae, &ato);

        dBgS_ObjGndChk_Spl spl_chk_2;
        mae = ato;
        mae.y += 100.0f;
        spl_chk_2.SetPos(&mae);
        if (fabsf(dComIfG_Bgsp().GroundCross(&spl_chk_2) - ato.y) < 50.0f) {
            static u16 w_eff_name[3] = {
                dPa_RM(ID_ZI_S_ENEMY_DOWNWTRA_A),
                dPa_RM(ID_ZI_S_ENEMY_DOWNWTRA_B),
                dPa_RM(ID_ZI_S_ENEMY_DOWNWTRA_C),
            };

            f32 scale = 1.0f;
            cXyz sc(scale, scale, scale);
            csXyz rot(0, 0, 0);
            for (int i = 0; i < 3; i++) {
                i_this->enemy_downWtrA[i] = dComIfGp_particle_set(i_this->enemy_downWtrA[i], w_eff_name[i], &ato, &enemy->tevStr, 
                                                                &rot, &sc, 0xFF, 0, -1, NULL, NULL, NULL);
            }
        } else {
            fopAcM_effSmokeSet1(&i_this->field_0x1260, &i_this->field_0x1264, &ato, NULL, 1.3f, &enemy->tevStr, 1);
        }

        i_this->field_0x125d = 0;
    }

    cXyz size(1.3f, 1.3f, 1.3f);
    setMidnaBindEffect(enemy, &i_this->sound, &enemy->eyePos, &size);
}

static void fire_eff_set(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz mae, ato;
    int iVar1;
    u16 uVar1[2];
    f32 scale;

    if (i_this->weapon_type == 4) {
        iVar1 = 2;
        uVar1[0] = 0x1DF;
        uVar1[1] = 0x1DE;
        i_this->sound.startCreatureSoundLevel(Z2SE_OBJ_BOMB_IGNITION, 0, -1);
        scale = NREG_F(18) + 2.0f;
        mae.x = NREG_F(0) + 62.0f;
        mae.y = NREG_F(1) + 22.0f;
        mae.z = NREG_F(2) + -50.0f;
    } else {
        iVar1 = 1;
        uVar1[0] = 0x8113;
        i_this->sound.startCreatureSoundLevel(Z2SE_OBJ_ARROW_FIRE_READY, 0, -1);
        scale = 1.0f;
        mae.x = NREG_F(0) + 90.0f;
        mae.y = NREG_F(1) + 10.0f;
        mae.z = NREG_F(2) + -60.0f;
    }

    MtxPosition(&mae, &ato);
    i_this->field_0x1288 = ato - i_this->field_0x127c;
    i_this->field_0x1288 *= 0.9f;
    i_this->field_0x127c = ato;

    cXyz sc(scale, scale, scale);
    for (int i = 0 ; i < iVar1; i++) {
        i_this->fire_eff[i] = dComIfGp_particle_set(i_this->fire_eff[i], uVar1[i], &ato, &enemy->shape_angle, &sc);

        if (i == 0) {
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->fire_eff[i]);
            if (emitter != NULL) {
                emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                emitter->setUserWork((uintptr_t)&i_this->field_0x1288);
            }
        }
    }
}

static void* s_wb_sub2(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_WB) {
        e_wb_class* boar = (e_wb_class*) i_actor;
        fopAc_ac_c* actor = (fopAc_ac_c*) i_data;
        if (target_info_count < 2) {
            target_info[target_info_count] = (fopAc_ac_c*) i_actor;
            target_info_count++;
        }
    }

    return NULL;
}

static void* s_lv9dn_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_DN && ((e_dn_class*)i_actor)->status != 0) {
        ((e_dn_class*)i_actor)->status = 0;
        return i_actor;
    }

    return NULL;
}

static void* s_lv9dn_sub2(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_DN && ((e_dn_class*)i_actor)->status == 0) {
        ((e_dn_class*)i_actor)->status = 1;
        ((e_dn_class*)i_actor)->mode = 0;
    }
    
    return NULL;
}

static void* s_lv9dn_sub3(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_DN) {
        ((e_dn_class*)i_actor)->mode = 2;
    }

    return NULL;
}

static void* s_lv9rd_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && ((e_rd_class*)i_actor)->field_0xafb != 0) {
        ((e_rd_class*)i_actor)->field_0xafb = 0;
    }

    return NULL;
}

static u8 data_80519338;

static void* s_lv9rd_sub2(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD
        && ((e_rd_class*)i_actor)->arg1 == data_80519338 && ((e_rd_class*)i_actor)->action != ACTION_DROP) {
        ((e_rd_class*)i_actor)->action = ACTION_DROP;
        ((e_rd_class*)i_actor)->mode = 2;
        anm_init((e_rd_class*)i_actor, BCK_RD_FURA2, 2.0f, 2, 1.0f);
        ((e_rd_class*)i_actor)->timer[0] = 1000;
        ((e_rd_class*)i_actor)->target_ya = ((fopEn_enemy_c*)i_actor)->home.angle.y;
        dComIfGp_setHitMark(1, ((fopEn_enemy_c*)i_actor), &((fopEn_enemy_c*)i_actor)->eyePos, NULL, NULL, 0);
        ((e_rd_class*)i_actor)->sound.startCollisionSE(Z2SE_HIT_WOOD_WEAPON, 31);
        return i_actor;
    }

    return NULL;
}

static void* s_lv9rd_sub3(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD
        && (((e_rd_class*)i_actor)->arg1 == 13 || ((e_rd_class*)i_actor)->arg1 == 14)) {
        ((e_rd_class*)i_actor)->field_0x9a4 = 1;
        ((e_rd_class*)i_actor)->attack_range = 50000.0f;
    }

    return NULL;
}

static void* s_lv9arrow_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_ARROW) {
        ((e_arrow_class*)i_actor)->field_0xa14 = 1;
    }

    return NULL;
}

static void* s_lv9arrow_sub2(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_ARROW) {
        ((e_arrow_class*)i_actor)->field_0xa14 = 0;
        ((e_arrow_class*)i_actor)->mAction = ACTION_BOW_RUN;
        ((e_arrow_class*)i_actor)->mMode = 0;
    }

    return NULL;
}

static void cam_3d_morf(e_rd_class* i_this, f32 param_2) {
    cLib_addCalc2(&i_this->demo_cam_center.x, i_this->field_0x12cc.x, param_2, i_this->field_0x12e4.x * i_this->field_0x130c);
    cLib_addCalc2(&i_this->demo_cam_center.y, i_this->field_0x12cc.y, param_2, i_this->field_0x12e4.y * i_this->field_0x130c);
    cLib_addCalc2(&i_this->demo_cam_center.z, i_this->field_0x12cc.z, param_2, i_this->field_0x12e4.z * i_this->field_0x130c);
    cLib_addCalc2(&i_this->demo_cam_eye.x, i_this->field_0x12c0.x, param_2, i_this->field_0x12d8.x * i_this->field_0x130c);
    cLib_addCalc2(&i_this->demo_cam_eye.y, i_this->field_0x12c0.y, param_2, i_this->field_0x12d8.y * i_this->field_0x130c);
    cLib_addCalc2(&i_this->demo_cam_eye.z, i_this->field_0x12c0.z, param_2, i_this->field_0x12d8.z * i_this->field_0x130c);
}

static void cam_spd_set(e_rd_class* i_this) {
    i_this->field_0x12d8.x = fabsf(i_this->field_0x12c0.x - i_this->demo_cam_eye.x);
    i_this->field_0x12d8.y = fabsf(i_this->field_0x12c0.y - i_this->demo_cam_eye.y);
    i_this->field_0x12d8.z = fabsf(i_this->field_0x12c0.z - i_this->demo_cam_eye.z);
    i_this->field_0x12e4.x = fabsf(i_this->field_0x12cc.x - i_this->demo_cam_center.x);
    i_this->field_0x12e4.y = fabsf(i_this->field_0x12cc.y - i_this->demo_cam_center.y);
    i_this->field_0x12e4.z = fabsf(i_this->field_0x12cc.z - i_this->demo_cam_center.z);
    i_this->field_0x130c = 0.0f;
}

static void demo_camera(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    daPy_py_c* pla = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = (camera_class*) dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = (camera_class*) dComIfGp_getCamera(0);
    daNPC_TK_c* taka = (daNPC_TK_c*) fopAcM_SearchByName(PROC_NPC_TK);
    fopAc_ac_c* actor = taka;
    cXyz mae, ato, sp50, target;
    bool sp_0x9 = true; // unused
    s8 bVar1 = false;
    s16 sVar1 = 0;

    switch (i_this->demo_mode + 1) {
        case 2:
            if (!enemy->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(enemy, 2, 0xFFFF, 0);
                enemy->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->demo_mode = 2;
            i_this->demo_timer = 0;
            i_this->demo_cam_zoom = 55.0f;
            camera->mCamera.SetTrimSize(3);

            i_this->demo_cam_center = enemy->current.pos;
            i_this->demo_cam_center.y = enemy->eyePos.y - 80.0f + 60.0f + TREG_F(5);
            i_this->field_0x1300 = 2000.0f;
            enemy->current.angle.y = i_this->angleY;
            // fallthrough
        case 3:
            if (i_this->demo_timer < 85) {
                mDoMtx_stack_c::YrotS(s16(enemy->current.angle.y));
                mDoMtx_stack_c::XrotM(0x640);
                mae.x = 0.0f;
                mae.y = TREG_F(3);
                mae.z = i_this->field_0x1300;
                mDoMtx_stack_c::multVec(&mae, &i_this->demo_cam_eye);
                i_this->demo_cam_eye += enemy->current.pos;
                cLib_addCalc2(&i_this->demo_cam_center.y, enemy->eyePos.y - 80.0f + 60.0f + TREG_F(5), 0.1f, 20.0f);
                cLib_addCalc2(&i_this->field_0x1300, 400.0f, 0.5f, 150.0f);
            } else if (desert_substage == 124) {
                // main Gerudo Desert
                lbl_70_bss_AE = true;
                target_info_count = 0;
                for (int i = 0; i < 2; i++) {
                    target_info[i] = NULL;
                }

                fpcM_Search(s_wb_sub2, i_this);

                if (target_info[0] != NULL && target_info[1] != NULL) {
                    i_this->demo_cam_center = target_info[0]->current.pos + ((target_info[1]->current.pos - target_info[0]->current.pos) * 0.5f);
                    i_this->demo_cam_center.y += 200.0f + BREG_F(7);

                    if (i_this->demo_timer == 85) {
                        i_this->demo_cam_eye = i_this->demo_cam_center;
                        i_this->demo_cam_eye.y += 100.0f + VREG_F(8);
                        i_this->demo_cam_eye.z += 900.0f + VREG_F(9);
                    } else {
                        cLib_addCalc2(&i_this->demo_cam_eye.x, i_this->demo_cam_center.x, 0.2f, 100.0f);
                        cLib_addCalc2(&i_this->demo_cam_eye.y, i_this->demo_cam_center.y + 100.0f + VREG_F(8), 0.2f, 100.0f);
                        cLib_addCalc2(&i_this->demo_cam_eye.z, i_this->demo_cam_center.z + 900.0f + VREG_F(9), 0.2f, 100.0f);
                    }
                }

                if (i_this->demo_timer == 220) {
                    bVar1 = true;
                }
            } else if (i_this->demo_timer == 140) {
                bVar1 = true;
            }
            break;

        case 11:
            if (!enemy->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(enemy, 2, 0xFFFF, 0);
                enemy->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->demo_mode = 11;
            i_this->demo_timer = 0;
            i_this->demo_cam_zoom = 55.0f;
            dComIfGp_offCameraAttentionStatus(0, 8);
            camera->mCamera.SetTrimSize(3);

            i_this->demo_cam_center = enemy->current.pos;
            i_this->demo_cam_center.y = (enemy->eyePos.y - 80.0f) + 60.0f + TREG_F(5);
            i_this->demo_cam_eye = camera0->lookat.eye;
            enemy->current.angle.y = i_this->angleY;
            i_this->field_0x1300 = 2000.0f;
            // fallthrough
        case 12:
            mDoMtx_stack_c::YrotS(s16(enemy->current.angle.y));
            mDoMtx_stack_c::XrotM(0x640);
            mae.x = 0.0f;
            mae.y = TREG_F(3);
            mae.z = i_this->field_0x1300;
            mDoMtx_stack_c::multVec(&mae, &ato);
            ato.x += enemy->current.pos.x;
            ato.z += enemy->current.pos.z;
            ato.y += enemy->home.pos.y;

            cLib_addCalc2(&i_this->field_0x1300, 400.0f, 0.5f, 150.0f);

            if (i_this->demo_timer < 20) {
                i_this->demo_cam_eye = ato;
            } else {
                cLib_addCalc2(&i_this->demo_cam_eye.x, ato.x, 0.1f, 10.0f);
                cLib_addCalc2(&i_this->demo_cam_eye.z, ato.z, 0.1f, 10.0f);
            }

            cLib_addCalc2(&i_this->demo_cam_center.x, enemy->eyePos.x, 0.1f, 10.0f);
            cLib_addCalc2(&i_this->demo_cam_center.z, enemy->eyePos.z, 0.1f, 10.0f);
            cLib_addCalc2(&i_this->demo_cam_center.y, enemy->eyePos.y - 80.0f + 60.0f + TREG_F(5), 0.1f, 100.0f);

            if (i_this->demo_timer == s16(100 + KREG_S(8))) {
                bVar1 = true;
            }
            break;

        case 20:
            if (pla->getClothesChangeWaitTimer() == 0) {
                i_this->demo_mode = 21;
                i_this->demo_timer = 0;
            }
            break;

        case 21:
            if (!enemy->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(enemy, 2, 0xFFFF, 0);
                enemy->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->demo_mode = 21;
            i_this->demo_timer = 0;
            i_this->demo_cam_zoom = 55.0f;
            camera->mCamera.SetTrimSize(3);

            i_this->demo_cam_center = camera0->lookat.center;
            i_this->demo_cam_eye = camera0->lookat.eye;
            i_this->field_0x12cc.set(-7875.0f, 2125.0f, 7895.0f);
            i_this->field_0x12c0.set(-7527.0f, 2084.0f, 7552.0f);
            cam_spd_set(i_this);
            i_this->field_0x12e4 *= 2.0f;
            i_this->field_0x12f0 = pla->current.pos;
            i_this->field_0x12f0.y = 2000.0f;

            daPy_getPlayerActorClass()->changeOriginalDemo();
            if (daPy_py_c::checkNowWolf()) {
                daPy_getPlayerActorClass()->changeDemoMode(71, 0, 0, 0);
                i_this->demo_mode = 19;
            }
            // fallthrough
        case 22:
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&i_this->field_0x12f0, pla->shape_angle.y, 0);

            if (i_this->demo_mode != 19) {
                if (i_this->demo_timer == 1) {
                    Z2GetAudioMgr()->bgmStreamPrepare(0x2000055);
                    Z2GetAudioMgr()->bgmStreamPlay();
                }

                cam_3d_morf(i_this, BREG_F(17) + 0.2f);
                cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.1f, 1.0f, BREG_F(17) + 0.002f);

                if (i_this->demo_timer == 70) {
                    dComIfGs_onSwitch(75, fopAcM_GetRoomNo(enemy));
                }

                if (i_this->demo_timer >= 70) {
                    ato.set(-5182.0f, 2000.0f, 5237.0f);
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&ato, -8111, 0);
                }

                if (i_this->demo_timer == 120 || i_this->demo_timer == 130
                    || i_this->demo_timer == 140 || i_this->demo_timer == 150) {
                    fpcM_Search(s_lv9dn_sub, i_this);
                }

                if (i_this->demo_timer > 160) {
                    cLib_addCalc2(&i_this->demo_cam_zoom, 55.0f, 0.05f, 0.5f);
                } else {
                    cLib_addCalc2(&i_this->demo_cam_zoom, 45.0f, 0.05f, 0.5f);
                }

                if (i_this->demo_timer == 180) {
                    i_this->field_0x12cc.set(-7241.0f, 4310.0f, 7269.0f);
                    i_this->field_0x12c0.set(-6913.0f, 4139.0f, 6948.0f);
                    cam_spd_set(i_this);
                    i_this->demo_mode = 22;
                    i_this->demo_timer = 0;
                    fpcM_Search(s_lv9rd_sub, i_this);
                    daPy_getPlayerActorClass()->changeDemoMode(23, 0, 0, 0);
                }
            }
            break;

        case 23:
            cam_3d_morf(i_this, BREG_F(17) + 0.2f);
            cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.15f, 1.0f, BREG_F(17) + 0.005f);
            cLib_addCalc2(&i_this->demo_cam_zoom, 55.0f, 0.1f, 0.5f);

            if (i_this->demo_timer == 13) {
                fpcM_Search(s_lv9dn_sub2,i_this);
            }

            if (i_this->demo_timer == 17 || i_this->demo_timer == 27 || i_this->demo_timer == 37 || i_this->demo_timer == 47) {
                fpcM_Search(s_lv9dn_sub, i_this);
            }

            if (i_this->demo_timer == 55) {
                i_this->demo_cam_center.set(-8330.0f, 4359.0f, 8449.0f);
                i_this->demo_cam_eye.set(-8183.0f, 4264.0f, 7991.0f);
                i_this->demo_cam_zoom = 55.0f;
                i_this->demo_mode = 23;
                i_this->demo_timer = 0;
            }
            break;

        case 24:
            i_this->field_0x9a4 = 1;

            if (i_this->demo_timer == 20) {
                i_this->attack_range = 50000.0f;
            }

            if (i_this->demo_timer < 40) {
                i_this->demo_cam_eye.y += 2.0f;
            }

            if (i_this->demo_timer == 40) {
                i_this->demo_cam_center.set(-8033.0f, 4259.0f, 8143.0f);
                i_this->demo_cam_eye.set(-8337.0f, 4488.0f, 8451.0f);
            }

            if (i_this->demo_timer >= 40) {
                cLib_addCalc2(&i_this->demo_cam_zoom, 30.0f, 0.05f, 0.5f);
            }

            if (i_this->demo_timer == 110) {
                i_this->field_0x9a4 = 0;
                i_this->demo_mode = 24;
                i_this->demo_timer = 0;
                i_this->field_0x12cc.set(-4910.0f, 2012.0f, 4976.0f);
                i_this->field_0x12c0.set(-5235.0f, 2179.0f, 5302.0f);
                cam_spd_set(i_this);
            }
            break;

        case 25:
            i_this->field_0x9a4 = 1;
            i_this->attack_range = 0.0f;

            if (i_this->demo_timer >= 4) {
                i_this->blurRate = 200 + VREG_S(7);
                sVar1 = cM_scos(i_this->demo_timer * 0x500) * 2500.0f;
                i_this->field_0x130c = 0.03f + BREG_F(16);
                cam_3d_morf(i_this, 0.5f + BREG_F(17));

                if (i_this->demo_timer == 4) {
                    fpcM_Search(s_lv9arrow_sub, i_this);
                }

                if (i_this->demo_timer == 10) {
                    fpcM_Search(s_lv9dn_sub2, i_this);
                }

                if (i_this->demo_timer == 20) {
                    daPy_getPlayerActorClass()->changeDemoMode(20, 1, 0, 0);
                    taka->mSound.startCreatureVoice(Z2SE_HAWK_V_REGI_DEMO_1, -1);
                }

                if (i_this->demo_timer == 41) {
                    mDoGph_gInf_c::fadeOut(0.5f, g_blackColor);
                }

                if (i_this->demo_timer == 43) {
                    i_this->demo_mode = 25;
                    i_this->demo_timer = 0;
                    Z2GetAudioMgr()->seStart(Z2SE_AL_FISHING_HIT, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                    i_this->blurRate = 0;
                }
            }
            break;

        case 26:
            if (i_this->demo_timer == 40) {
                mDoGph_gInf_c::fadeIn(0.5f, g_blackColor);
                i_this->demo_cam_zoom = 55.0f;
                i_this->demo_cam_center.set(-5197.0f, 1712.0f, 5039.0f);
                i_this->demo_cam_eye.set(-5215.0f, 2108.0f, 5327.0f);
                i_this->field_0x12cc.set(-5025.0f, 2270.0f, 4909.0f);
                i_this->field_0x12c0.set(-5215.0f, 2108.0f, 5327.0f);
                cam_spd_set(i_this);
                fpcM_Search(s_lv9arrow_sub2, i_this);
            }

            if (i_this->demo_timer == 80) {
                i_this->demo_mode = 26;
                i_this->demo_timer = 0;
                daPy_getPlayerActorClass()->changeDemoMode(72, 3, 0, 0);
            }
            break;

        case 27:
            cam_3d_morf(i_this, BREG_F(17) + 0.2f);
            cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.5f, 1.0f, BREG_F(17) + 0.01f);

            if (i_this->demo_timer == 55) {
                i_this->demo_cam_center.set(-5529.0f, 2457.0f, 5589.0f);
                i_this->demo_cam_eye.set(-5232.0f, 2201.0f, 5295.0f);
                i_this->field_0x12cc.set(-5580.0f, 2188.0f, 5640.0f);
                i_this->field_0x12c0.set(-5232.0f, 2201.0f, 5295.0f);
                cam_spd_set(i_this);
                i_this->demo_mode = 27;
                i_this->demo_timer = 0;
            }
            break;

        case 28:
            fpcM_Search(s_lv9rd_sub3, i_this);

            if (i_this->demo_timer == 40) {
                data_80519338 = 14;
                fpcM_Search(s_lv9rd_sub2, i_this);
            } else if (i_this->demo_timer == 60) {
                data_80519338 = 13;
                fpcM_Search(s_lv9rd_sub2, i_this);
            }

            if (i_this->demo_timer == 10 || i_this->demo_timer == 20 || i_this->demo_timer == 30 || i_this->demo_timer == 40) {
                fpcM_Search(s_lv9dn_sub, i_this);
            }

            if (i_this->demo_timer < 100) {
                cLib_addCalc2(&i_this->demo_cam_zoom, 25.0f, 0.2f, 1.0f);
            } else if (i_this->demo_timer >= 120) {
                cLib_addCalc2(&i_this->demo_cam_zoom, 55.0f, 0.05f, 0.4f);
                cam_3d_morf(i_this, BREG_F(17) + 0.2f);
                cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.1f, 1.0f, BREG_F(17) + 0.01f);

                if (i_this->demo_timer == 177) {
                    ato.set(-5821.0f, 2000.0f, 5850.0f);
                    dBomb_c::createNormalBombExplode(&ato);
                    dComIfGs_onTmpBit((u16) dSv_event_tmp_flag_c::tempBitLabels[0x76]);
                }

                if (i_this->demo_timer == 180) {
                    dComIfGp_getVibration().StartShock(8, 79, cXyz(0.0f, 1.0f, 0.0f));
                    fpcM_Search(s_lv9dn_sub3, i_this);
                }
            }

            if (i_this->demo_timer != 230) {
                break;
            }

            i_this->demo_mode = 28;
            i_this->demo_timer = 0;
            i_this->field_0x12cc.set(-5406.0f, 2168.0f, 5468.0f);
            i_this->field_0x12c0.set(-5058.0f, 2181.0f, 5124.0f);
            cam_spd_set(i_this);
            // fallthrough
        case 29:
            if (i_this->demo_timer == 1) {
                pla->changeDemoMode(20, 0, 0, 0);
            }

            if (i_this->demo_timer < 25) {
                cam_3d_morf(i_this, BREG_F(17) + 0.2f);
                cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.2f, 1.0f, BREG_F(17) + 0.01f);

                if (i_this->demo_timer == 23) {
                    taka->setResistanceDemo();
                }
            } else {
                if (i_this->demo_timer == 25) {
                    i_this->field_0x130c = 0.0f;
                }

                target = actor->current.pos;
                cLib_addCalc2(&i_this->demo_cam_center.x, target.x, 0.2f, i_this->field_0x130c * 100.0f);
                cLib_addCalc2(&i_this->demo_cam_center.y, target.y + BREG_F(13), 0.2f, i_this->field_0x130c * 100.0f);
                cLib_addCalc2(&i_this->demo_cam_center.z, target.z, 0.2f, i_this->field_0x130c * 100.0f);

                cLib_addCalc2(&i_this->field_0x130c, 1.0f, 1.0f, 0.03f);

                if (i_this->demo_timer >= 55) {
                    if (i_this->demo_timer == 110) {
                        taka->mSound.startCreatureVoice(Z2SE_HAWK_V_REGI_DEMO_2, -1);
                    }

                    if (i_this->demo_timer < 135) {
                        cLib_addCalc2(&i_this->demo_cam_zoom, 20.0f, 0.2f, 1.0f);
                    } else {
                        cLib_addCalc2(&i_this->demo_cam_zoom, 55.0f, 0.1f, 0.5f);
                    }
                }

                if (i_this->demo_timer == 205) {
                    i_this->demo_mode = 29;
                    i_this->demo_timer = 0;
                    daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
                }
            }
            break;

        case 30:
            i_this->demo_cam_zoom = 55.0f;
            i_this->demo_cam_eye.set(-3963.0f, 147.0f, 8094.0f);
            target = actor->current.pos;

            if (i_this->demo_timer == 0) {
                i_this->demo_cam_center = target;
            } else {
                cLib_addCalc2(&i_this->demo_cam_center.x, target.x, 0.2f, 100.0f);
                cLib_addCalc2(&i_this->demo_cam_center.y, target.y, 0.2f, 100.0f);
                cLib_addCalc2(&i_this->demo_cam_center.z, target.z, 0.2f, 100.0f);

                if (i_this->demo_timer == 140) {
                    i_this->field_0x12cc.set(-4080.0f, 134.0f, 8190.0f);
                    i_this->field_0x12c0 = i_this->demo_cam_eye;
                    cam_spd_set(i_this);
                    i_this->demo_mode = 30;
                    i_this->demo_timer = 0;
                }
            }
            break;

        case 31:
            cam_3d_morf(i_this, BREG_F(17) + 0.1f);
            cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.1f, 1.0f, BREG_F(17) + 0.002f);

            if (i_this->demo_timer == 70) {
                i_this->demo_cam_center.set(-4527.0f, 143.0f, 8305.0f);
                i_this->demo_cam_eye.set(-4676.0f, 161.0f, 8282.0f);
                i_this->field_0x12cc.set(-4552.0f, 143.0f, 8461.0f);
                i_this->field_0x12c0.set(-4701.0f, 161.0f, 8438.0f);
                cam_spd_set(i_this);
                i_this->demo_mode = 31;
                i_this->demo_timer = 0;
                ato.set(-5397.0f, 2000.0f, 5921.0f);
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&ato, 0x213F, 0);
            }
            break;

        case 32:
            cam_3d_morf(i_this, BREG_F(17) + 0.05f);
            cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.05f, 1.0f, BREG_F(17) + 0.001f);

            if (i_this->demo_timer == 70) {
                i_this->demo_mode = 32;
                i_this->demo_timer = 0;
                i_this->demo_cam_center.set(-5275.0f, 2327.0f, 5812.0f);
                i_this->demo_cam_eye.set(-5367.0f, 2689.0f, 5495.0f);
                i_this->demo_cam_zoom = 30.0f;
            }
            break;

        case 33:
            if (i_this->demo_timer == 60) {
                i_this->demo_cam_center.set(-3906.0f, 42.0f, 8198.0f);
                i_this->demo_cam_eye.set(-4274.0f, 272.0f, 7969.0f);
                i_this->demo_mode = 33;
                i_this->demo_timer = 0;
                dComIfGs_onTmpBit((u16) dSv_event_tmp_flag_c::tempBitLabels[0x75]);
            }
            break;

        case 34:
            if (i_this->demo_timer == 65) {
                daPy_getPlayerActorClass()->changeDemoMode(60, 1, 0, 0);
            }

            if (i_this->demo_timer == 70) {
                i_this->demo_cam_center.set(-5712.0f, 2308.0f, 5828.0f);
                i_this->demo_cam_eye.set(-5286.0f, 2104.0f, 5957.0f);
                i_this->demo_cam_zoom = 45.0f;
                i_this->demo_mode = 34;
                i_this->demo_timer = 0;
            }
            break;

        case 35:
            cLib_addCalc2(&i_this->demo_cam_zoom, 40.0f, 0.05f, 0.02f);
            if (i_this->demo_timer == 80) {
                i_this->demo_cam_center.set(-4712.0f, 870.0f, 7432.0f);
                i_this->demo_cam_eye.set(-4852.0f, 1171.0f, 7072.0f);
                i_this->demo_cam_zoom = 55.0f;
                i_this->demo_mode = 35;
                i_this->demo_timer = 0;
                /* Hyrule Castle - Resistance leaves Flag*/
                dComIfGs_onTmpBit((u16) dSv_event_tmp_flag_c::tempBitLabels[0x74]);
            }
            break;

        case 36:
            if (i_this->demo_timer == 120) {
                bVar1 = true;
                int bitsw = (fopAcM_GetParam(enemy) & 0xFF000000) >> 24;
                if (bitsw != 0xFF) {
                    dComIfGs_onSwitch(bitsw, fopAcM_GetRoomNo(enemy));
                }
            }
            break;
    }

    if (bVar1) {
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->demo_mode = -1;
    }

    if (i_this->demo_mode > 0) {
        cXyz center, eye;
        center = i_this->demo_cam_center;
        eye = i_this->demo_cam_eye;
        camera->mCamera.Set(center, eye, sVar1, i_this->demo_cam_zoom);
        i_this->demo_timer++;
    }
}

static int c_start;

static int daE_RD_Execute(e_rd_class* i_this) {
    if (c_start == 0 && dComIfGp_event_runCheck()) {
        if (data_80519201 != 0) {
            return 1;
        }

        if (cDmrNowMidnaTalk() || daPy_getPlayerActorClass()->eventInfo.checkCommandDoor()) {
            return 1;
        }
    }

    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    cXyz mae, ato, ato2;

    if (i_this->arg2 == 11) {
        fopAcM_delete(enemy);
        return 1;
    }

    if (S_find != 0) {
        S_find--;
    }

    i_this->counter++;

    if (i_this->actor_set == 0) {
        i_this->field_0x1297 = i_this->field_0x1298;
    } else {
        fopAc_ac_c* pla = dComIfGp_getPlayer(0);

        // Bridge of Eldin Battle
        if (strcmp(dComIfGp_getStartStageName(), "F_SP102") == 0 && pla->current.pos.y < -2000.0f) {
            cDmr_SkipInfo = 50;
        }

        if (cDmr_SkipInfo != 0) {
            cDmr_SkipInfo--;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (i_this->timer[i] != 0) {
            i_this->timer[i]--;
        }
    }

    if (i_this->damage_timer != 0) {
        i_this->damage_timer--;
    }

    if (i_this->field_0xaf0 != 0) {
        i_this->field_0xaf0--;
    }

    if (i_this->field_0x99a != 0) {
        i_this->field_0x99a--;
    }

    if (i_this->attack_timer != 0) {
        i_this->attack_timer--;
    }

    if (i_this->yagura_timer != 0) {
        i_this->yagura_timer--;
    }

    if (otoCheck(enemy, 1000.0f) || daPy_getPlayerActorClass()->checkWolfBark()) {
        i_this->look_timer = cM_rndF(10.0f) + 10.0f;
    }

    if (i_this->look_timer != 0) {
        i_this->look_timer--;
    }

    i_this->field_0x9a2 = 0;
    i_this->field_0x9ad = 1;
    action(i_this);

    if (i_this->ride_mode != 0) {
        i_this->field_0x9ad = 0;
    }

    if (i_this->ride_mode != 2 && i_this->field_0xaf0 == 0 && i_this->action != ACTION_IKKI_END) {
        ato.set(0.0f, 0.0f, 0.0f);
        if (i_this->jump_z) {
            cMtx_YrotS(*calc_mtx, i_this->jump_angle.y);
            mae.x = 0.0f;
            mae.y = TREG_F(8);
            mae.z = i_this->field_0xaec;
            MtxPosition(&mae, &ato);
            cLib_addCalc2(&i_this->field_0xaec, i_this->jump_z * (TREG_F(9) + 2.0f), 1.0f, TREG_F(17) + 5.0f);
        } else {
            cLib_addCalc0(&i_this->field_0xaec, 1.0f, TREG_F(17) + 5.0f);
        }

        if (i_this->anm == BCK_RD_DIEB && i_this->jump_angle.x != 0 && i_this->field_0xa1f == 0) {
            ato.y += KREG_F(0) + -20.0f;
        }

        enemy->current.pos += ato;
        enemy->old.pos += ato;
        i_this->ObjAcch.CrrPos(dComIfG_Bgsp());
        enemy->current.pos -= ato;
        enemy->old.pos -= ato;

        if (!i_this->ObjAcch.ChkGroundHit()) {
            if (enemy->speed.y < -10.0f) {
                i_this->field_0xaf2++;
                if (i_this->field_0xaf2 == 20 && i_this->actor_set == 0) {
                    i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_DEATH, -1);
                }
            }
        } else {
            if (i_this->field_0xaf2 >= ZREG_S(8) + 20 && i_this->arg1 != 13 && i_this->arg1 != 14) {
                i_this->action = ACTION_DAMAGE;
                i_this->mode = 0;
                enemy->speed.y = 0.0f;
                i_this->jump_z = -1.0f;
                i_this->jump_angle.x = -0x3000;
                i_this->damage_timer = 1000;
                enemy->health = 0;
            }

            i_this->field_0xaf2 = 0;
        }

        i_this->field_0x1294 = 0;

        if (i_this->ObjAcch.GetGroundH() != -G_CM3D_F_INF && i_this->ObjAcch.ChkWaterHit() && i_this->ObjAcch.m_wtr.GetHeight() > enemy->current.pos.y) {
            i_this->field_0x1294 = 1;
        }
    }

    if (i_this->field_0xafb == 0) {
        mDoMtx_stack_c::transS(enemy->current.pos.x, enemy->current.pos.y + i_this->field_0x9c0, enemy->current.pos.z);
        mDoMtx_stack_c::XrotM((s16) i_this->field_0xa12.x);
        mDoMtx_stack_c::ZrotM((s16) i_this->field_0xa12.z);
        mDoMtx_stack_c::YrotM((s16) i_this->jump_angle.y);
        mDoMtx_stack_c::XrotM((s16) i_this->jump_angle.x);
        mDoMtx_stack_c::YrotM(-i_this->jump_angle.y);
        mDoMtx_stack_c::YrotM((s16) enemy->shape_angle.y);
        mDoMtx_stack_c::XrotM((s16) enemy->shape_angle.x);
        mDoMtx_stack_c::ZrotM(enemy->shape_angle.z);

        f32 scale = l_HIO.model_size * enemy->scale.x;
        if (i_this->actor_set != 0) {
            scale *= l_HIO.leader_size_ratio;
        }
        mDoMtx_stack_c::scaleM(scale, scale, scale);

        J3DModel* my_model_p = i_this->anm_p->getModel();
        my_model_p->setBaseTRMtx(mDoMtx_stack_c::get());
        i_this->anm_p->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(enemy)));
        int frame = i_this->anm_p->getFrame();

        if (i_this->anm == BCK_RD_RUN && (frame == 1 || frame == 10)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_RUNNING_BREATH, -1);
        }

        if (i_this->anm == BCK_RD_KYORO2 && (frame == 5 || frame == 26)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_RUNNING_BREATH, -1);
        }

        if ((i_this->anm == BCK_RD_SHOOT_READY && frame == 10) || (i_this->anm == BCK_RD_RSHOOT_READY && frame == 5) || (i_this->anm == BCK_RD_ARMAMENT && frame == 10)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_READY_WEAPON, -1);
        }

        if ((i_this->anm == BCK_RD_WALK && (i_this->anm_p->checkFrame(1.0f) || i_this->anm_p->checkFrame(15.0f))) ||
            ((i_this->anm == BCK_RD_RUN || i_this->anm == BCK_RD_RUN02) && (i_this->anm_p->checkFrame(1.0f) || i_this->anm_p->checkFrame(7.0f)))) {
            if (i_this->field_0x1294 != 0) {
                i_this->sound.startCreatureSound(Z2SE_RIDER_FOOTNOTE_WATER, 0, -1);
            } else {
                i_this->sound.startCreatureSound(Z2SE_RIDER_FOOTNOTE, 0, -1);
            }
        }

        if (i_this->actor_set != 0) {
            if (i_this->anm == e_rdb_class::BCK_RB_RCOMEON) {
                if (i_this->anm_p->checkFrame(30.0f)) {
                    i_this->sound.startCreatureVoice(Z2SE_EN_RDB_V_LAUGH, -1);
                }
            } else if (i_this->anm == e_rdb_class::BCK_RB_RJUMP_C && i_this->anm_p->checkFrame(21.0f)) {
                i_this->sound.startCreatureVoice(Z2SE_EN_RDB_V_HOICK2, -1);
            }
        }

        if (i_this->field_0x680 != 0 && i_this->anm_p->isStop()) {
            int anm;
            if (enemy->speedF >= 10.0f) {
                anm = BCK_RD_SHOUT;
            } else {
                anm = BCK_RD_WALK;
            }
            i_this->field_0x680 = 0;
            anm_init(i_this, anm, 10.0f, 2, 1.0f);
        }

        i_this->anm_p->modelCalc();
        
        if (i_this->anm == BCK_RD_SLEEP_WAIT || dComIfGp_checkPlayerStatus0(0, 0x200000)) {
            i_this->field_0x1297 = 0;
        }

        if (i_this->field_0x1297 != 0) {
            J3DModel* model = i_this->anm_p->getModel();
            cLib_addCalc2(&i_this->field_0x6cc, i_this->field_0x6d0, 1.0f, 0.02f);
            i_this->field_0x6d0 = 1.0f;
            MTXCopy(model->getAnmMtx(13), *calc_mtx);
            camera_class* camera = (camera_class*) dComIfGp_getCamera(0);
            
            static f32 time_scale[25] = {
                1.2f, 1.2f, 1.1f, 1.0f,
                0.8f, 0.6f, 0.4f, 0.2f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.2f, 0.5f,
                0.8f, 1.2f, 1.2f, 1.2f,
                1.2f,
            };

            for (int i = 0; i < 2; i++) {
                MtxPush();
                s16 x, y;
                if (i == 0) {
                    mae.set(38.0f, 0.0f, 0.0f);
                    MtxPosition(&mae, &ato);
                    mae = camera->lookat.eye - ato;

                    y = cM_atan2s(mae.x, mae.z);
                    x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));
                    scale = mae.abs() * (0.001f + JREG_F(8));
                    if (scale > 2.0f + JREG_F(17)) {
                        scale = 2.0f + JREG_F(17);
                    }

                    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
                    int timeH = kankyo->daytime / 15.0f;
                    scale *= i_this->field_0x6cc * time_scale[timeH];
                    mae.set(38.0f, 0.0f, 6.0f);
                } else {
                    mae.set(38.0f, 0.0f, -6.0f);
                }

                MtxPosition(&mae, &ato);
                MtxTrans(ato.x, ato.y, ato.z, 0);
                cMtx_YrotM(*calc_mtx, y);
                cMtx_XrotM(*calc_mtx, x);
                MtxScale(scale, scale, scale, 1);

                i_this->eye_model[i]->setBaseTRMtx(*calc_mtx);
                MtxPull();
            }
        }
    }

    if (i_this->field_0xafb == 0) {
        J3DModel* model = i_this->anm_p->getModel();
        MTXCopy(model->getAnmMtx(11), *calc_mtx);
        mae.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&mae, &ato);
        mae.set(YREG_F(1), YREG_F(2) + 100.0f, YREG_F(3));
        MtxPosition(&mae, &ato2);

        if (ato.y < ato2.y) {
            i_this->field_0xa1f = 0;
        } else {
            i_this->field_0xa1f = 1;
        }

        cXyz cr(0.0f, 0.0f, 0.0f);
        
        if (i_this->damage_timer != 0) {
            cr.set(-20000.0f, 200000.0f, 30000.0f);
        }

        if (i_this->actor_set != 0) {
            MTXCopy(model->getAnmMtx(AREG_S(9) + 6), *calc_mtx);
        } else {
            MTXCopy(model->getAnmMtx(13), *calc_mtx);
        }

        mae.set(20.0f, 0.0f, 0.0f);
        MtxPosition(&mae, &enemy->eyePos);

        if (daPy_py_c::checkNowWolf()) {
            cr.y += 30.0f;
            i_this->cc_sph[0].SetC(enemy->eyePos + cr);
            i_this->cc_sph[0].SetR(50.0f);
        } else {
            i_this->cc_sph[0].SetC(enemy->eyePos + cr);
            i_this->cc_sph[0].SetR(35.0f);
        }

        if (i_this->ride_mode == 2) {
            enemy->attention_info.position = enemy->current.pos;
            enemy->attention_info.position.y += 190.0f + JREG_F(8);

            if (i_this->actor_set != 0) {
                enemy->attention_info.position.y += 120.0f + JREG_F(9);
            }
        } else {
            enemy->attention_info.position = enemy->eyePos;
            enemy->attention_info.position.y += 30.0f;
        }

        if (i_this->actor_set != 0) {
            f32 rad = 60.0f;
            f32 size = 0.0f;
            if (i_this->actor_set == 2) {
                rad = 90.0f;
                size = l_HIO.ikki_boss_size;
            }

            mae.set(ZREG_F(0), ZREG_F(1), ZREG_F(2));
            MTXCopy(model->getAnmMtx(2), *calc_mtx);
            MtxPosition(&mae, &ato);
            i_this->cc_sph[2].SetC(ato + cr);
            i_this->cc_sph[2].SetR((rad + ZREG_F(3)) + size);

            mae.set(ZREG_F(4), ZREG_F(5), ZREG_F(6));
            MTXCopy(model->getAnmMtx(22), *calc_mtx);
            MtxPosition(&mae, &ato);
            i_this->cc_sph[1].SetC(ato + cr);
            i_this->cc_sph[1].SetR(rad + ZREG_F(7));
        } else {
            mae.set(BREG_F(14), BREG_F(15), BREG_F(16));
            MTXCopy(model->getAnmMtx(12), *calc_mtx);
            MtxPosition(&mae, &ato);
            i_this->cc_sph[2].SetC(ato + cr);
            i_this->cc_sph[2].SetR(35.0f);

            mae.set(BREG_F(11) + -10.0f, BREG_F(12), BREG_F(13));
            MTXCopy(model->getAnmMtx(10), *calc_mtx);
            MtxPosition(&mae, &ato);
            i_this->cc_sph[1].SetC(ato + cr);
            i_this->cc_sph[1].SetR(35.0f);
        }

        for (int i = 0; i <= 2; i++) {
            if (i_this->field_0x9ad != 0) {
                i_this->cc_sph[i].OnCoSetBit();
            } else {
                i_this->cc_sph[i].OffCoSetBit();
            }

            dComIfG_Ccsp()->Set(&i_this->cc_sph[i]);

            if (i_this->actor_set == 1 && i_this->ride_mode == 2) {
                i_this->cc_sph[i].OnTgNoHitMark();
            } else {
                i_this->cc_sph[i].OffTgNoHitMark();
            }

            if (i_this->actor_set == 3) {
                for (int j = 0; j <= 2; j++) {
                    i_this->cc_sph[j].SetTgType(0x2022);
                    i_this->cc_sph[j].OnTgNoHitMark();
                }
            }
        }
    }

    if (i_this->field_0x9ab == 2) {
        MTXCopy(i_this->anm_p->getModel()->getAnmMtx(11), *calc_mtx);
        mae.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&mae, &ato);
        i_this->at_sph.SetR(l_HIO.model_size * 50.0f);
        i_this->at_sph.SetC(ato);
        dComIfG_Ccsp()->Set(&i_this->at_sph);
        i_this->field_0x9ab = 0;
    }

    if (i_this->weapon_type == 1) {
        if (i_this->field_0x5bd == 0) {
            MTXCopy(i_this->anm_p->getModel()->getAnmMtx(24), *calc_mtx);
        } else {
            MTXCopy(i_this->anm_p->getModel()->getAnmMtx(11), *calc_mtx);
            cMtx_YrotM(*calc_mtx, 6000);
            cMtx_XrotM(*calc_mtx, 10000);
            cMtx_ZrotM(*calc_mtx, 18000);
            MtxTrans(-30.0f, -50.0f, 20.0f, 1);
        }

        i_this->arrow->setBaseTRMtx(*calc_mtx);

        if (i_this->field_0x9ab != 0) {
            if (daPy_getPlayerActorClass()->checkHorseRide() || daPy_getPlayerActorClass()->checkBoarRide()) {
                mae.set(0.0f, nREG_F(15) + 50.0f, 0.0f);
                i_this->at_sph.SetR((nREG_F(16) + 60.0f) * l_HIO.model_size);
            } else {
                mae.set(0.0f, 30.0f, 0.0f);
                i_this->at_sph.SetR(l_HIO.model_size * 50.0f);
            }

            MtxPosition(&mae, &ato);

            if (i_this->field_0x9ac == 0) {
                i_this->field_0x9ac = 1;
                i_this->at_sph.StartCAt(ato);
            } else {
                i_this->at_sph.MoveCAt(ato);
            }

            i_this->field_0x9ab = 0;
        } else {
            cXyz cr(-200000.0f, -200000.0f, -200000.0f);
            i_this->at_sph.SetC(cr);
            i_this->at_sph.SetR(-200.0f);
            i_this->field_0x9ac = 0;
        }

        dComIfG_Ccsp()->Set(&i_this->at_sph);
    }

    if (i_this->weapon_type >= 2) {
        J3DModel* model = i_this->anm_p->getModel();

        if (i_this->bow_shake_timer != 0) {
            i_this->bow_shake_timer--;
        }

        i_this->field_0x9a8 = i_this->bow_shake_timer * cM_ssin(i_this->bow_shake_timer * (TREG_S(9) + 0x7800)) * (TREG_F(5) + 100.0f);

        model = i_this->bow_anm->getModel();
        if (i_this->field_0x5bd == 0) {
            model->setBaseTRMtx(i_this->anm_p->getModel()->getAnmMtx(19));
        } else {
            MTXCopy(i_this->anm_p->getModel()->getAnmMtx(11), *calc_mtx);
            cMtx_YrotM(*calc_mtx, 6000);
            cMtx_XrotM(*calc_mtx, 10000);
            cMtx_ZrotM(*calc_mtx, 18000);
            MtxTrans(-20.0f, -20.0f, 30.0f, 1);
            model->setBaseTRMtx(*calc_mtx);
        }

        i_this->bow_anm->play(0, 0);
        i_this->bow_anm->modelCalc();

        if (i_this->field_0x9a2 != 0) {
            MTXCopy(i_this->anm_p->getModel()->getAnmMtx(24), *calc_mtx);
            i_this->arrow->setBaseTRMtx(*calc_mtx);

            if (i_this->weapon_type >= 3) {
                if (i_this->ride_mode != 2 || i_this->timer[1] <= 18) {
                    fire_eff_set(i_this);
                }
            } else {
                i_this->field_0x127c.set(-200000.0f, -200000.0f, -200000.0f);
            }
        }

        if (i_this->arrow_flag) {
            if (i_this->weapon_type == 3) {
                i_this->sound.startCreatureSound(Z2SE_OBJ_ARROW_SHOT_FIRE, 0, -1);
            } else {
                i_this->sound.startCreatureSound(Z2SE_OBJ_ARROW_SHOT_NORMAL, 0, -1);
            }

            i_this->sound.startCreatureVoice(Z2SE_EN_RD_V_SHOT_ARROW, -1);

            mae.set(0.0f, 0.0f, 0.0f);
            MTXCopy(i_this->anm_p->getModel()->getAnmMtx(24), *calc_mtx);
            MtxPosition(&mae, &ato);

            csXyz angl;
            int parameter = 0;

            if (i_this->action == ACTION_BOW_IKKI) {
                mae = i_this->eye - ato;
                angl.y = cM_atan2s(mae.x, mae.z);
                angl.x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));
            } else {
                fopAc_ac_c* actor = get_pla(enemy);
                if (actor != dComIfGp_getPlayer(0)) {
                    parameter |= 32;
                }

                mae = actor->eyePos;

                if (i_this->field_0x1296 != 0) {
                    f32 mult_fact = TREG_F(7) + 15.0f;
                    mae.x += actor->speed.x * mult_fact;
                    mae.z += actor->speed.z * mult_fact;
                }

                mae -= ato;
                mae.y -= 30.0f;
                angl.y = cM_atan2s(mae.x, mae.z);

                if (i_this->ride_mode == 2 && !pl_pass_check(i_this, YREG_F(18))) {
                    s16 sVar3 = cM_rndF(1000.0f) + 2000.0f;
                    if (cM_rndF(1.0f) < 0.5f) {
                        sVar3 *= -1;
                    }

                    angl.y += sVar3;
                }

                angl.x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));

                // NOT Hyrule Field
                if (strcmp(dComIfGp_getStartStageName(), "F_SP121") != 0 && i_this->field_0x1296 == 0) {
                    ANGLE_ADD(angl.x, cM_rndFX(200.0f) + -500.0f);
                    ANGLE_ADD(angl.y, cM_rndFX(100.0f));
                }
            }

            angl.z = 0;
            if (i_this->weapon_type == 3) {
                parameter |= 1;
            } else if (i_this->weapon_type == 4) {
                parameter |= 2;
            }

            if (i_this->arg2 == 11) {
                parameter |= 16;
            }

            fopAcM_createChild(PROC_E_ARROW, fopAcM_GetID(enemy), parameter, &ato, fopAcM_GetRoomNo(enemy), &angl, NULL, -1, NULL);
            i_this->arrow_flag = 0;
            i_this->bow_shake_timer = TREG_S(8) + 15;
        }
    }

    if (i_this->field_0x968 != 0) {
        cLib_addCalc2(&i_this->field_0x96c, -20.0f, 1.0f, 0.4f);
    }

    static int boss_part_idx[14] = {
        12, 29, 17, 25, 1, 1, 13,
        19, 6, 18, 14, 2, 6, 2,
    };
    static int ikki2_boss_part_idx[2] = {
        14, 19,
    };
    static u16 eno[3] = {
        dPa_RM(ID_ZM_S_RB_BREAKARMOR00),
        dPa_RM(ID_ZM_S_RB_BREAKARMOR01),
        dPa_RM(ID_ZM_S_RB_BREAKARMOR02),
    };

    if (i_this->actor_set != 0) {
        for (int i = 0; i < 14; i++) {
            if (i_this->armor_boss_part[i] != NULL) {
                if (i_this->field_0x70c[i] == 0) {
                    if (i_this->actor_set == 3) {
                        MTXCopy(i_this->anm_p->getModel()->getAnmMtx(ikki2_boss_part_idx[i]), *calc_mtx);
                    } else {
                        MTXCopy(i_this->anm_p->getModel()->getAnmMtx(boss_part_idx[i]), *calc_mtx);
                    }

                    i_this->armor_boss_part[i]->setBaseTRMtx(*calc_mtx);
                    mae.set(0.0f, 0.0f, 0.0f);
                    MtxPosition(&mae, &i_this->field_0x71c[i]);
                } else {
                    i_this->field_0x71c[i] += i_this->field_0x7c4[i];
                    i_this->field_0x7c4[i].y -= 3.0f;
                    ANGLE_ADD(i_this->field_0x86c[i].y, 0x900);
                    ANGLE_ADD(i_this->field_0x86c[i].x, 0xB00);

                    mDoMtx_stack_c::transS(i_this->field_0x71c[i].x, i_this->field_0x71c[i].y, i_this->field_0x71c[i].z);
                    mDoMtx_stack_c::YrotM(i_this->field_0x86c[i].y);
                    mDoMtx_stack_c::XrotM(i_this->field_0x86c[i].x);
                    f32 my_scale = l_HIO.leader_size_ratio * (l_HIO.model_size * enemy->scale.x);
                    mDoMtx_stack_c::scaleM(my_scale, my_scale, my_scale);

                    i_this->armor_boss_part[i]->setBaseTRMtx(mDoMtx_stack_c::get());

                    for (int j = 0; j < 3; j++) {
                        i_this->armor_break_eff[i][j] = dComIfGp_particle_set(i_this->armor_break_eff[i][j], eno[j],
                                                                          &i_this->field_0x71c[i], NULL, NULL);
                    }

                    if (i_this->field_0x71c[i].y < enemy->current.pos.y - 200.0f) {
                        i_this->armor_boss_part[i] = NULL;
                    }
                }
            }
        }

        if (i_this->field_0x6a0 != 0) {
            if (i_this->field_0x6a0 == 1) {
                MTXCopy(i_this->anm_p->getModel()->getAnmMtx(YREG_S(3) + 15), *calc_mtx);
                mae.set(0.0f, 0.0f, 0.0f);
                MtxPosition(&mae, &i_this->field_0x6a4);
            } else {
                i_this->field_0x6a4 += i_this->field_0x6b0;
                i_this->field_0x6b0.y -= 5.0f;
                MtxTrans(i_this->field_0x6a4.x, i_this->field_0x6a4.y, i_this->field_0x6a4.z, 0);
                f32 scale = l_HIO.leader_size_ratio * ( l_HIO.model_size * enemy->scale.x);
                MtxScale(scale, scale, scale, 1);
                MtxTrans(BREG_F(5) + 80.0f, BREG_F(6) + 50.0f, BREG_F(7), 1);
                cMtx_YrotM(*calc_mtx, i_this->field_0x6bc.y);
                cMtx_XrotM(*calc_mtx, 0x7FFF);
                cMtx_ZrotM(*calc_mtx, i_this->field_0x6bc.z);
                MtxTrans(-(BREG_F(5) + 80.0f), -(BREG_F(6) + 50.0f), -(BREG_F(7) + 0.0f), 1);
                ANGLE_ADD(i_this->field_0x6bc.y, 0x200);
                ANGLE_ADD(i_this->field_0x6bc.z, 0xF00);
            }

            i_this->mpMorfHornAnm->getModel()->setBaseTRMtx(*calc_mtx);
        }

        if (i_this->run_flag) {
            i_this->run_flag = 0;
            if (i_this->field_0x680 == 0) {
                int anm_no = i_this->anm;
                if (anm_no == e_rdb_class::BCK_RB_RRUN) {
                    s16 run_range = enemy->shape_angle.y - i_this->angleY;
                    if (run_range < 0) {
                        anm_init(i_this, e_rdb_class::BCK_RB_RPLAY_RUNR, 2.0f, 0, 1.0f);
                    } else {
                        anm_init(i_this, e_rdb_class::BCK_RB_RPLAY_RUNL, 2.0f, 0, 1.0f);
                    }

                    horn_anm_init(i_this, e_rdb_class::BCK_HORN_RPLAY_RUN, 2.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, e_rdb_class::BCK_RB_RPLAY_STOP, 2.0f, 0, 1.0f);
                    horn_anm_init(i_this, e_rdb_class::BCK_HORN_RPLAY_STOP, 2.0f, 0, 1.0f);
                }

                i_this->field_0x680 = anm_no;
            }
        }

        if (i_this->field_0x6a0 != 0) {
            i_this->mpMorfHornAnm->play(NULL, 0, 0);
            i_this->mpMorfHornAnm->modelCalc();
        }
    }

    if (i_this->actor_set == 0 && i_this->ride_mode == 0) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        MTXCopy(i_this->anm_p->getModel()->getAnmMtx(11), mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&ato);
        mae = player->current.pos - ato;
        s16 atan_val = cM_atan2s(mae.x, mae.z);
        cMtx_YrotS(*calc_mtx, atan_val);
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = (BREG_F(12) + 40.0f) - 30.0f;
        MtxPosition(&mae, &ato2);
        ato2 += ato;
        i_this->enemy.setDownPos(&ato2);
    }

    cXyz spd4(enemy->eyePos);
    spd4.y += NREG_F(7) + 130.0f;
    i_this->enemy.setHeadLockPos(&spd4);
    demo_camera(i_this);
    i_this->field_0x5bb = 1;
    enemy->attention_info.flags |= fopAc_AttnFlag_UNK_0x200000;

    return 1;
}

static u8 jv_offset[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static int daE_RD_IsDelete(e_rd_class*) {
    return 1;
}

static int daE_RD_Delete(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    fopAcM_RegisterDeleteID(i_this, "E_RD");

    dComIfG_resDelete(&i_this->phase, i_this->resName);
    if (i_this->hio_set != 0) {
        hio_set = 0;
        mDoHIO_DELETE_CHILD(l_HIO.field_0x4);
    }

    if (enemy->heap != NULL) {
        i_this->anm_p->stopZelAnime();
    }

    return 1;
}

static void ride_game_actor_set(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    fopAc_ac_c* pla = dComIfGp_getPlayer(0);
    cXyz mae, ato;
    csXyz angl;

    OS_REPORT("E3 2005 ACTOR SET %d\n", i_this->actor_set);
    static cXyz set_pos[6] = {
        cXyz(0.0f, 1500.0f, 2500.0f),
        cXyz(800.0f, 1500.0f, -4800.0f),
        cXyz(-650.0f, 1500.0f, -4500.0f),
        cXyz(1100.0f, 1500.0f, -5500.0f),
        cXyz(0.0f, 1500.0f, -5800.0f),
        cXyz(-1100.0f, 1500.0f, -5500.0f),
    };

    dBgS_GndChk gnd_chk;
    u32 parameter;
    int num = 6;
    if (i_this->actor_set >= 2) {
        num = 1;
    }

    for (int i = 0; i < num; i++) {
        if (i_this->actor_set == 1) {
            cMtx_YrotS(*calc_mtx, pla->shape_angle.y);
            mae = set_pos[i];

            if (i > 0 && cDmr_SkipInfo == 0) {
                mae.z -= 4000.0f;
            }

            MtxPosition(&mae, &ato);
            ato += pla->current.pos;
            gnd_chk.SetPos(&ato);
            ato.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
            angl = pla->shape_angle;
            ANGLE_ADD(angl.y, 0x4000);
            parameter = 0x80000005;
        } else if (i_this->actor_set == 2) {
            ato.set(34789.0f, -290.0f, -36177.0f);
            angl.set(0, 0, 0);
            parameter = 0x80000007;
        } else if (i_this->actor_set == 3) {
            ato.set(-93620.0f, -5750.0f, 49650.0f);
            angl.set(0, -0x8000, 0);
            parameter = 0x80000008;
        } else if (i_this->actor_set == 4) {
            ato = a_this->home.pos;
            angl = a_this->home.angle;
            parameter = 0x80000009;
        }

        if (i == 0) {
            a_this->home.pos = ato;
            a_this->current.pos = a_this->home.pos;
            a_this->old.pos = a_this->home.pos;
        } else {
            angl = pla->shape_angle;
            parameter = (i << 8 | 0x80000006);
        }

        fopAcM_create(PROC_E_WB, parameter, &ato, fopAcM_GetRoomNo(a_this), &angl, NULL, -1);
    }
}

static void coach_game_actor_set(e_rd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;
    fopAc_ac_c* pla = dComIfGp_getPlayer(0);
    cXyz ununsed_vec_0, ununsed_vec_1;
    csXyz angl;

    OS_REPORT("COATH GAME ACTOR SET \n");

    static cXyz set_pos[3] = {
        cXyz(-57464.0f, -8000.0f, 82137.0f),
        cXyz(-58006.0f, -8000.0f, 79302.0f),
        cXyz(-56848.0f, -8000.0f, 84472.0f),
    };

    dBgS_GndChk gnd_chk;
    angl.set(0, 0, 0);
    angl.y = pla->shape_angle.y;

    for (int i = 0; i < NREG_S(7) + 3; i++) {
        fopAcM_create(PROC_E_WB, (i + 1) * 0x100 | 0x8000000a, &set_pos[i], fopAcM_GetRoomNo(enemy), &angl, NULL, -1);
    }
}

static s8 instanceCount;

static int useHeapInit(fopAc_ac_c* a_this) {
    e_rd_class* i_this = (e_rd_class*)a_this;
    J3DModelData* modelData;
    J3DModel* model;

    if (i_this->actor_set != 0) {
        i_this->anm_p = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_rdb", e_rdb_class::BMDR_RB), NULL, NULL,
                                                   (J3DAnmTransform*)dComIfG_getObjectRes("E_rdb", e_rdb_class::BCK_RB_RWAIT), 2, 1.0f,
                                                   0, -1, &i_this->sound, 0x80000, 0x11000084);
        if (i_this->anm_p == NULL || i_this->anm_p->getModel() == NULL) {
            return 0;
        }

        model = i_this->anm_p->getModel();
        model->setUserArea((uintptr_t)i_this);

        for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_B);
        }

        if (i_this->actor_set == 1) {
            i_this->mpMorfHornAnm = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("E_rdb", e_rdb_class::BMDR_RB_HORN), 
                                                       NULL, NULL, NULL, 2, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11000084);
            if (i_this->mpMorfHornAnm == NULL || i_this->mpMorfHornAnm->getModel() == NULL) {
                return 0;
            }

            i_this->field_0x6a0 = 1;
        }

        static int boss_part_bmd[14] = {
            e_rdb_class::BMDR_SHOULDERL_ARMOR, e_rdb_class::BMDR_LEGR_ARMOR, 
            e_rdb_class::BMDR_SHOULDERR_ARMOR, e_rdb_class::BMDR_LEGL_ARMOR, 
            e_rdb_class::BMDR_MUNE2_ARMOR, e_rdb_class::BMDR_SENA2_ARMOR, 
            e_rdb_class::BMDR_ARML_ARMOR, e_rdb_class::BMDR_ARML2_ARMOR, 
            e_rdb_class::BMDR_FACE_ARMOR, e_rdb_class::BMDR_ARMR_ARMOR, 
            e_rdb_class::BMDR_ARMR2_ARMOR, e_rdb_class::BMDR_SENA_ARMOR, 
            e_rdb_class::BMDR_HEAD_ARMOR, e_rdb_class::BMDR_MUNE_ARMOR,
        };
        static int ikki2_boss_part_bmd[2] = {
            86, 87,
        };

        for (int i = 0; i < 14; i++) {
            if (i_this->actor_set == 3) {
                if (i < 2) {
                    modelData = (J3DModelData*)dComIfG_getObjectRes("E_rdb", ikki2_boss_part_bmd[i]);
                    JUT_ASSERT(10672, modelData != NULL);
                    i_this->armor_boss_part[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
                    if (i_this->armor_boss_part[i] == NULL) {
                        return 0;
                    }
                } else {
                    i_this->field_0x70c[i] = 1;
                }
            } else if (i_this->actor_set == 1 || i == 13) {
                modelData = (J3DModelData*)dComIfG_getObjectRes("E_rdb", boss_part_bmd[i]);
                JUT_ASSERT(10687, modelData != NULL);
                i_this->armor_boss_part[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
                if (i_this->armor_boss_part[i] == NULL) {
                    return 0;
                }
            } else {
                i_this->field_0x70c[i] = 1;
            }
        }
    } else {
        i_this->anm_p = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes(i_this->resName, BMDR_RD), NULL, NULL,
                                                   (J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, BCK_RD_WAIT01), 0, 1.0f,
                                                   0, -1, &i_this->sound, 0x80000, 0x11000084);
        if (i_this->anm_p == NULL || i_this->anm_p->getModel() == NULL) {
            return 0;
        }

        J3DModel* model2 = i_this->anm_p->getModel();
        model2->setUserArea((uintptr_t)i_this);
        mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
        model2->setBaseTRMtx(mDoMtx_stack_c::get());

        for (u16 i = 0; i < model2->getModelData()->getJointNum(); i++) {
            model2->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }

        if (i_this->weapon_type == 1) {
            if (boss != NULL) {
                modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->resName, BMDR_RD_CLUBB);
            } else {
                modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->resName, BMDR_RD_CLUB);
            }

            JUT_ASSERT(10762, modelData != NULL);

            i_this->arrow = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            if (i_this->arrow == NULL) {
                return 0;
            }

            i_this->arrow->setBaseTRMtx(mDoMtx_stack_c::get());
        } else if (i_this->weapon_type >= 2) {
            i_this->bow_anm = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes(i_this->resName, BMDR_RD_BOW), NULL, NULL,
                                                       (J3DAnmTransform*)dComIfG_getObjectRes(i_this->resName, BCK_RD_BOW_SHOOT), 0, 1.0f,
                                                       0, -1, NULL, 0x80000, 0x11000084);
            if (i_this->bow_anm == NULL || i_this->bow_anm->getModel() == NULL) {
                return 0;
            }

            model2 = i_this->bow_anm->getModel();
            model2->setUserArea((uintptr_t)i_this);
            model2->setBaseTRMtx(mDoMtx_stack_c::get());

            for (u16 i = 0; i < model2->getModelData()->getJointNum(); i++) {
                if (i == 2 || i == 3) {
                    model2->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_bow);
                }
            }

            if (i_this->weapon_type == 4) {
                modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->resName, BMDR_RD_BARROW);
            } else {
                modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->resName, BMDR_RD_ARROW);
            }

            JUT_ASSERT(10810, modelData != NULL);

            i_this->arrow = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            if (i_this->arrow == NULL) {
                return 0;
            }

            i_this->arrow->setBaseTRMtx(mDoMtx_stack_c::get());
        }

        modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->resName, BMDR_RD_EYE);
        JUT_ASSERT(10823, modelData != NULL);

        for (u16 i = 0; i < 2; i++) {
            i_this->eye_model[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            if (i_this->eye_model[i] == NULL) {
                return 0;
            }
            i_this->eye_model[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }

    return 1;
}

static cPhs_Step daE_RD_Create(fopAc_ac_c* actor) {
    e_rd_class* i_this = (e_rd_class*)actor;

    fopAcM_ct(&i_this->enemy, e_rd_class);

    i_this->arg0 = fopAcM_GetParam(actor);
    i_this->arg1 = i_this->arg0;
    i_this->arg2 = (fopAcM_GetParam(actor) & 0xF000) >> 12;
    if (i_this->arg2 == 15) {
        i_this->arg2 = 0;
    }

    if (((i_this->arg0 == 4 || i_this->arg0 == 5) || i_this->arg0 == 11) || i_this->arg0 == 12) {
        if (i_this->arg0 == 4) {
            i_this->actor_set = 1;
        } else if (i_this->arg0 == 5) {
            i_this->actor_set = 2;
        } else if (i_this->arg0 == 11) {
            i_this->actor_set = 3;
        } else if (i_this->arg0 == 12) {
            i_this->actor_set = 4;
        }

        i_this->resName = "E_rdb";
    } else {
        i_this->resName = "E_RD";
    }

    cPhs_Step phase_state = dComIfG_resLoad(&i_this->phase, i_this->resName);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (strcmp(dComIfGp_getStartStageName(), "F_SP124") == 0) {
            // Gerudo Desert
            desert_substage = 124;
        } else {
            if (strcmp(dComIfGp_getStartStageName(), "F_SP118") == 0) {
                // Arbiter's Grounds Exterior - a.k.a. Bulblin camp
                desert_substage = 118;
            } else {
                desert_substage = 0;
            }
        }

        int swBit = (fopAcM_GetParam(actor) & 0xFF000000) >> 24;
        if (swBit != 0xFF) {
            if (dComIfGs_isSwitch(swBit, fopAcM_GetRoomNo(actor))) {
                return cPhs_ERROR_e;
            }
        }

        if (i_this->arg0 == 10) {
            coach_game_actor_set(i_this);
            return cPhs_ERROR_e;
        }

        if (i_this->actor_set != 0) {
            boss = i_this;
            i_this->arg0 = 1;
        }

        i_this->weapon_type = (fopAcM_GetParam(actor) & 0xF00) >> 8;
        if (i_this->weapon_type == 15) {
            i_this->weapon_type = 0;
        }

        if (i_this->weapon_type >= 4) {
            return cPhs_ERROR_e;
        }

        i_this->field_0x5ba = (fopAcM_GetParam(actor) & 0xFF0000) >> 16;
        if (i_this->field_0x5ba == 0xFF) {
            i_this->field_0x5ba = 0;
        }

        if (i_this->field_0x5ba == 2) {
            fopAcM_setStageLayer(actor);
        }

        i_this->field_0x5b9 = (fopAcM_GetParam(actor) & 0xFF000000) >> 24;
        if (i_this->arg2 == 3 || i_this->arg2 == 4 || i_this->arg2 == 5 || i_this->arg2 == 6 || i_this->arg2 == 7) {
            if (i_this->arg2 != 5) {
                i_this->one_hit_kill = 1;
            }

            if (i_this->arg2 == 4) {
                i_this->field_0x1296 = 1;
            }

            if (i_this->arg2 == 6) {
                i_this->field_0x1299 = 1;
                i_this->action = ACTION_COMMANDER;
                i_this->mode = 0;
                i_this->timer[2] = 20;
                i_this->field_0x1296 = 1;
            } else if (i_this->arg2 == 7) {
                i_this->action = ACTION_BOW3;
                i_this->field_0x1296 = 1;
                i_this->damage_timer = 20;
            } else {
                i_this->action = ACTION_BOW2;
            }

            fopAcM_setCullSizeFar(actor, 30000.0f);
            i_this->arg2 = 3;
        } else if (i_this->arg2 == 10) {
            i_this->action = ACTION_BOW_IKKI;
            i_this->timer[0] = 142 + XREG_S(2);
        } else if (i_this->arg2 == 11) {
            fopAcM_create(PROC_E_RDY, fopAcM_GetParam(actor), &actor->home.pos, 
                          fopAcM_GetRoomNo(actor), &actor->home.angle, NULL, -1);
        }

        if ((actor->home.angle.z & 0xFF) == 0 || (actor->home.angle.z & 0xFF) == 0xFF) {
            i_this->attack_range = 100000.0f;
        } else {
            i_this->attack_range = (actor->home.angle.z & 0xFF) * 100.0f;
        }

        i_this->sw = actor->home.angle.z >> 8 & 0xFF;

        if (i_this->sw == 0xFF) {
            i_this->sw = 0;
        }

        fopAcM_OnStatus(actor, fopAcM_STATUS_UNK_0x100);
        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        if (i_this->arg0 == 6) {
            i_this->action = ACTION_YAGURA;
            i_this->yagura_timer = 30;
            S_find = 0;
            lbl_70_bss_AE = false;

            if (fopAcM_GetRoomNo(actor) == 0) {
                fopAcM_setCullSizeFar(actor, 30000.0f);
            }
        } else if (i_this->arg0 == 7) {
            i_this->action = ACTION_NORMAL;
            u8 path_no = actor->home.angle.x & 0xFF;
            if ((u32)(path_no) != 0xFF) {
                i_this->path = dPath_GetRoomPath(path_no, fopAcM_GetRoomNo(actor));
                OS_REPORT("//////////////E_RD PPD %x!!\n", i_this->path);
                if (i_this->path == NULL) {
                    OS_REPORT("......RD   NONONONONONO PATH !!!!\n");
                    return cPhs_ERROR_e;
                }

                dPnt* pnt_p = i_this->path->m_points;
                OS_REPORT("//////////////E_RD P0.y %f\n", pnt_p->m_position.y);
            }
        } else if (i_this->arg0 == 8) {
            i_this->action = ACTION_SLEEP;
            if (fopAcM_GetRoomNo(actor) == 0) {
                fopAcM_setCullSizeFar(actor, 30000.0f);
            }
        } else if (i_this->arg0 == 9) {
            i_this->action = ACTION_TAG;
            fopAcM_OffStatus(actor, 0);
            actor->attention_info.flags = 0;
        } else if (i_this->arg0 == 35) {
            i_this->action = ACTION_FIGHT_RUN;
            i_this->mode = 0;
            i_this->yagura_timer = 200;
        }

        if (i_this->weapon_type == 1 && i_this->arg0 == 0) {
            i_this->action = ACTION_STAND;
            i_this->mode = 0;
        }

        if (i_this->arg0 == 13 || i_this->arg0 == 14) {
            i_this->action = ACTION_REG;
            i_this->mode = 0;
            i_this->field_0xafb = 1;
            fopAcM_OffStatus(actor, fopAcM_STATUS_UNK_0x100);
        }

        if (i_this->arg0 > 2) {
            i_this->arg0 = 0;
        }

        actor->home.angle.z = actor->current.angle.z = actor->shape_angle.z
        = actor->home.angle.x = actor->current.angle.x = actor->shape_angle.x = 0;

        u32 size;
        if (i_this->actor_set == 1) {
            size = 0xAD60;
        } else if (i_this->actor_set == 2) {
            size = 0x3C00;
        } else if (i_this->actor_set == 3) {
            size = 0x4400;
        } else {
            size = 0x4FF0;
        }

        if (!fopAcM_entrySolidHeap(actor, useHeapInit, size)) {
            OS_REPORT("//////////////E_RD SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (i_this->actor_set != 0) {
            ride_game_actor_set(i_this);
        }

        if (hio_set == 0) {
            i_this->hio_set = 1;
            hio_set = 1;
            // "rider"
            l_HIO.field_0x4 = mDoHIO_CREATE_CHILD("ライダー", &l_HIO);
        }

        fopAcM_SetMtx(actor, i_this->anm_p->getModel()->getBaseTRMtx());
        fopAcM_SetMin(actor, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(actor, 200.0f, 200.0f, 200.0f);
        i_this->ObjAcch.Set(fopAcM_GetPosition_p(actor), fopAcM_GetOldPosition_p(actor), actor, 1,
                             &i_this->AcchCir, fopAcM_GetSpeed_p(actor), NULL, NULL);
        i_this->AcchCir.SetWall(50.0f, 50.0f);

        actor->field_0x560 = actor->health = 40;

        i_this->mStts.Init(150, 0, actor);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };
        for (int i = 0; i <= 2; i++) {
            i_this->cc_sph[i].Set(cc_sph_src);
            i_this->cc_sph[i].SetStts(&i_this->mStts);

            if (i_this->actor_set != 0) {
                i_this->cc_sph[i].SetTgType(0xD8FAFD3F);
            }
        }

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x1d}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
            } // mSphAttr
        };
        i_this->at_sph.Set(at_sph_src);
        i_this->at_sph.SetStts(&i_this->mStts);

        i_this->counter = instanceCount | (u16(cM_rndF(65535.0f)) & 0xFF00);
        instanceCount++;

        i_this->sound.init(&actor->current.pos, &actor->eyePos, 3, 1);

        if (i_this->actor_set != 0) {
            i_this->sound.setEnemyName("E_rdb");
        } else {
            i_this->sound.setEnemyName("E_rd");
        }

        i_this->AtInfo.mPowerType = 2;
        i_this->AtInfo.mpSound = &i_this->sound;

        if (boss != NULL) {
            fopAcM_setCullSizeFar(actor, 30000.0f);
        }

        i_this->field_0x9a0 = 5;

        stage_stag_info_class* stage_info = dComIfGp_getStage()->getStagInfo();
        if (!dStage_stagInfo_GetSTType(stage_info)) {
            i_this->field_0x1298 = l_HIO.eye_polygon;

            if (desert_substage != 0 || strcmp(dComIfGp_getStartStageName(), "F_SP115") == 0 ||
                (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0 && fopAcM_GetRoomNo(actor) == 0)) {
                // Lake Hylia or Hyrule Field
                fopAcM_OffStatus(actor, fopAcM_STATUS_UNK_0x4000);
            }
        } else if (i_this->actor_set != 4) {
            fopAcM_OffStatus(actor, fopAcM_STATUS_UNK_0x4000);
        }

        if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0 && fopAcM_GetRoomNo(actor) == 0) {
            // Hyrule Field - Bridge of Eldin
            data_80519201 = 1;
        } else {
            data_80519201 = 0;
        }

        c_start = 1;
        daE_RD_Execute(i_this);
        c_start = 0;

        if (i_this->action == ACTION_TAG) {
            i_this->field_0xafb = 1;
            i_this->sound.setEnemyName(NULL);
        }
    }

    return phase_state;
}

e_rd_class::e_rd_class() {}

AUDIO_INSTANCES

static actor_method_class l_daE_RD_Method = {
    (process_method_func)daE_RD_Create,
    (process_method_func)daE_RD_Delete,
    (process_method_func)daE_RD_Execute,
    (process_method_func)daE_RD_IsDelete,
    (process_method_func)daE_RD_Draw,
};

actor_process_profile_definition g_profile_E_RD = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_RD,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_rd_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  154,                    // mPriority
  &l_daE_RD_Method,       // sub_method
  0x00044000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
