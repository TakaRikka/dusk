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

class daE_RD_HIO_c : public JORReflexible {
public:
    daE_RD_HIO_c();
    virtual ~daE_RD_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x00 */ s8 field_0x4;

    /* ライダー (Rider) */
    /* 0x08 */ f32 model_size;              // 基本サイズ          (Basic size)
    /* 0x0C */ f32 field_0xc;               // リーダーサイズ比     (Leader size ratio)
    /* 0x10 */ f32 movement_speed;          // 移動速度            (Movement speed)

    /*   こん棒兵の場合   (In the case of the club soldier) */
    /* 0x14 */ f32 dash_speed;              // 突進速度            (Rush speed)
    /* 0x18 */ f32 battle_init_range;       // 戦闘開始範囲         (Battle starting range)
    /* 0x1C */ f32 attack_init_range;       // 攻撃開始範囲         (Attack starting range)
    /* 0x20 */ f32 swing_speed;             // 振り速さ            (Swing speed)

    /*   矢兵の場合   (In the case of arrow soldiers) */
    /* 0x24 */ s16 field_0x24;              // 号令→構えの間        (Command → Preparation)
    /* 0x26 */ u8 padding[2];
    /* 0x28 */ f32 attack_range;            // 攻撃範囲            (Attack range)
    /* 0x2C */ f32 mounted_launch_distance; // 騎乗発射距離         (Mounted launch distance)
    /* 0x30 */ f32 wolf_falling_power_y;    // 狼倒れ力Y           (Wolf falling power Y)
    /* 0x34 */ f32 wolf_falling_power_z;    // 狼倒れ力Z           (Wolf falling power Z)

    /* 0x38 */ u8 field_0x38;
    /* 0x39 */ u8 invulnerable;             // 不死身              (Invulnerability)
    /* 0x3A */ u8 eye_polygon;              // 目ポリゴン          (Eye polygon)
    /* 0x3B */ u8 one_hit_kill;             // 一撃必殺            (One hit kill)
    /* 0x3C */ f32 field_0x3c;              // 一騎（ダ）サイズ     (One-man army (Da) size)
    /* 0x40 */ f32 jump_g;                  // 飛びＧ              (Jump G)
    /* 0x44 */ f32 jump_z;                  // 飛びＺ              (Jump Z)
    /* 0x48 */ f32 field_0x48;              // 飛びＺ（騎乗停止）   (Jump Z (Stop riding))
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
    field_0xc = 1.2f;
    movement_speed = 3.5f;
    dash_speed = 14.0f;
    battle_init_range = 250.0f;
    attack_init_range = 300.0f;
    swing_speed = 1.1f;
    field_0x24 = 60;
    attack_range = 1300.0f;
    mounted_launch_distance = 2000.0f;
    wolf_falling_power_y = 3.0f;
    wolf_falling_power_z = 3.0f;
    field_0x38 = 1;
    invulnerable = 0;
    eye_polygon = 1;
    one_hit_kill = 0;
    field_0x3c = 75.0f;
    field_0x48 = 5.0f;
    jump_z = 10.0f;
    jump_y = 33.0f;
    jump_g = 5.0f;
}

static fopAc_ac_c* get_pla(fopAc_ac_c* a_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    fopAc_ac_c* actor = fopAcM_SearchByName(PROC_NPC_COACH);

    if (actor == NULL) {
        return dComIfGp_getPlayer(0);
    }
    f32 fVar1, fVar2, fVar3, fVar4;
    fVar1 = player->current.pos.x - a_this->current.pos.x;
    fVar3 = player->current.pos.z - a_this->current.pos.z;
    fVar2 = actor->current.pos.x - a_this->current.pos.x;
    fVar4 = actor->current.pos.z - a_this->current.pos.z;
    if (fVar1 * fVar1 + fVar3 * fVar3 > fVar2 * fVar2 + fVar4 * fVar4) {
        return actor;
    }

    return dComIfGp_getPlayer(0);
    
}

static void anm_init(e_rd_class* i_this, int i_anmID, f32 i_morf, u8 i_mode, f32 i_speed) {
    if (i_this->field_0x680 == 0) {
        if (i_this->mBossMode != 0) {
            if (i_anmID < 73) {
                i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_rdb", i_anmID),
                                            i_mode, i_morf, i_speed, 0.0f, -1.0f);
                i_this->mAnmID = i_anmID;
            }
        } else {
            i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, i_anmID),
                                        i_mode, i_morf, i_speed, 0.0f, -1.0f);
            i_this->mAnmID = i_anmID;

            if (i_anmID == BCK_RD_KYORO2) {
                i_this->mpModelMorf->setFrame(cM_rndF(30.0f));
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
        int jointNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_rd_class* i_this = (e_rd_class*)model->getUserArea();
        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
            if (jointNo == JNT_KOSI) {
                if (i_this->field_0x9c8 == 3) {
                    cMtx_YrotM(*calc_mtx, i_this->field_0x9ca);
                }
            } else if (jointNo == JNT_HIP1) {
                if (i_this->field_0x9c8 == 3) {
                    cMtx_XrotM(*calc_mtx, i_this->field_0x9ca);
                }

                cMtx_YrotM(*calc_mtx, i_this->field_0xae0);
                cMtx_ZrotM(*calc_mtx, i_this->field_0xabc[2] + (i_this->field_0xade - i_this->field_0xa32[2].x));
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
                    cMtx_YrotM(*calc_mtx, (i_this->field_0x9ce - i_this->field_0x9ca) / 2);
                    cMtx_ZrotM(*calc_mtx, (i_this->field_0xabc[0] + (i_this->field_0xa32[0].x - i_this->field_0x9cc)) / 2);
                }
            } else if (jointNo == JNT_KUBI) {
                cMtx_YrotM(*calc_mtx, i_this->field_0xa32[0].y);

                if (i_this->field_0x9c8 != 3 && i_this->field_0x9c8 != 4) {
                    cMtx_YrotM(*calc_mtx, (i_this->field_0x9ce - i_this->field_0x9ca) / 2);
                    cMtx_ZrotM(*calc_mtx, (i_this->field_0xabc[0] + (i_this->field_0xa32[0].x - i_this->field_0x9cc)) / 2);
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
        int jointNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_rd_class* i_this = (e_rd_class*)model->getUserArea();
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
        int jointNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_rd_class* i_this = (e_rd_class*)model->getUserArea();

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
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    J3DModel* model;
    J3DModelData* modelData;
    J3DMaterial* matNode_p;

    i_this->field_0x5bb = 0;

    if (i_this->field_0xafb != 0) {
        return 1;
    }

    model = i_this->mpModelMorf->getModel();
    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);

    if (i_this->mBossMode == 0 && i_this->field_0x968 != 0) {
        modelData = model->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            matNode_p = modelData->getMaterialNodePointer(i);
            matNode_p->getTevColor(0)->r = i_this->field_0x96c;
            matNode_p->getTevColor(0)->g = i_this->field_0x96c;
            matNode_p->getTevColor(0)->b = i_this->field_0x96c;
        }
    }

    if (i_this->mBossMode == 3) {
        modelData = model->getModelData();
        matNode_p = modelData->getMaterialNodePointer(3);
        J3DShape* shape = matNode_p->getShape();
        if (shape != NULL) {
            shape->hide();
        }
    }

    i_this->mpModelMorf->entryDL();

    if (i_this->mBossMode == 0 && i_this->field_0x968 != 0) {
        modelData = model->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            matNode_p = modelData->getMaterialNodePointer(i);
            matNode_p->getTevColor(0)->r = 0;
            matNode_p->getTevColor(0)->g = 0;
            matNode_p->getTevColor(0)->b = 0;
        }
    }

    if (i_this->field_0x5bc == 1) {
        g_env_light.setLightTevColorType_MAJI(i_this->field_0x694, &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->field_0x694);
    } else if (i_this->field_0x5bc >= 2) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpMorfBowAnm->getModel(), &a_this->tevStr);
        i_this->mpMorfBowAnm->entryDL();

        if (i_this->field_0x9a2 != 0) {
            g_env_light.setLightTevColorType_MAJI(i_this->field_0x694, &a_this->tevStr);
            mDoExt_modelUpdateDL(i_this->field_0x694);
        }
    }

    if (i_this->mBossMode != 0) {
        for (int i = 0; i < 14; i++) {
            if (i_this->mpBossArmorParts[i] != NULL) {
                g_env_light.setLightTevColorType_MAJI(i_this->mpBossArmorParts[i], &a_this->tevStr);
                mDoExt_modelUpdateDL(i_this->mpBossArmorParts[i]);
            }
        }

        if (i_this->field_0x6a0 != 0) {
            g_env_light.setLightTevColorType_MAJI(i_this->mpMorfHornAnm->getModel(), &a_this->tevStr);
            i_this->mpMorfHornAnm->entryDL();
        }
    }

    if (i_this->field_0x1297 != 0) {
        for (int i = 0; i < 2; i++) {
            g_env_light.setLightTevColorType_MAJI(i_this->mpEyeModels[i], &a_this->tevStr);

            modelData = i_this->mpEyeModels[i]->getModelData();
            for (u16 j = 0; j < modelData->getMaterialNum(); j++) {
                matNode_p = modelData->getMaterialNodePointer(j);
                matNode_p->getTevColor(1)->r = 0xFF;
                matNode_p->getTevColor(1)->g = 0x8A;
                matNode_p->getTevColor(1)->b = 0x18;
            }

            mDoExt_modelUpdateDL(i_this->mpEyeModels[i]);
        }
    }

    if (i_this->field_0x9bc != 2) {
        cXyz sp44;
        sp44.set(a_this->current.pos.x, a_this->current.pos.y + 100.0f, a_this->current.pos.z);
        i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &sp44,
                                                    1200.0f, 0.0f, a_this->current.pos.y,
                                                    i_this->mObjAcch.GetGroundH(), i_this->mObjAcch.m_gnd,
                                                    &a_this->tevStr, 0, 1.0f, 
                                                    dDlst_shadowControl_c::getSimpleTex());
        if (i_this->field_0x5bc == 1) {
            dComIfGd_addRealShadow(i_this->mShadowKey, i_this->field_0x694);
        }
    }

    if (i_this->field_0x5b7 == 14) {
        if (i_this->mBlureRate > 2) {
            mDoGph_gInf_c::setBlureRate(i_this->mBlureRate);
            mDoGph_gInf_c::onBlure();
        } else {
            mDoGph_gInf_c::offBlure();
        }
    }

    return 1;
}

static BOOL other_bg_check(e_rd_class* i_this, fopAc_ac_c* i_other) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz start;
    cXyz end;

    end = i_other->current.pos;
    end.y += 130.0f;
    start = a_this->current.pos;
    start.y = a_this->eyePos.y;

    lin_chk.Set(&start, &end, a_this);
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
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* actor = get_pla(a_this);
    cXyz sp18, sp24;

    sp18 = a_this->current.pos - actor->current.pos;
    cMtx_YrotS(*calc_mtx, -(actor->shape_angle.y));
    MtxPosition(&sp18, &sp24);

    if (sp24.z > param_2) {
        return TRUE;
    }

    return FALSE;
}

static e_rd_class* boss;

static u8 hio_set;

static daE_RD_HIO_c l_HIO;

static s16 S_find;

static s8 lbl_70_bss_AE;

static cXyz S_find_pos;

static u8 data_80519200;

static u8 data_80519201;

static fopAc_ac_c* target_info[10];

static int target_info_count;

static s8 data_80519230;

static void* s_b_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && dBomb_c::checkBombActor((fopAc_ac_c*)i_actor) && 
        !((dBomb_c*)i_actor)->checkStateExplode() && target_info_count < 10) {
        target_info[target_info_count] = (fopAc_ac_c*)i_actor;
        target_info_count++;
    }

    return NULL;
}

static dBomb_c* search_bomb(e_rd_class* i_this, int param_2) {
    if ((i_this->field_0xa20 & 0x10000000) == 0) {
        return NULL;
    }

    target_info_count = 0;
    for (int i = 0; i < 10; i++) {
        target_info[i] = NULL;
    }

    return NULL;
}

static BOOL way_bg_check(e_rd_class* i_this, f32 param_2) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz sp98, start, end;

    start = a_this->current.pos;
    start.y += 50.0f;
    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    sp98.x = 0.0f;
    sp98.y = 50.0f;
    sp98.z = param_2;
    MtxPosition(&sp98, &end);
    end += a_this->current.pos;
    lin_chk.Set(&start, &end, a_this);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return TRUE;
    }

    return FALSE;
}

static void ride_off(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    e_wb_class* bullbo = (e_wb_class*)fopAcM_SearchByID(i_this->mWbActorID);

    if (bullbo != NULL) {
        bullbo->mStatusFlags &= ~i_this->field_0x9be;
        i_this->field_0x9be = 0;

        if (bullbo->mActionID != 22 && bullbo->mActionID != 23 && bullbo->mActionID != 24) {
            if (bullbo->mActionID == 1) {
                if ((bullbo->mStatusFlags & 3) != 0) {
                    bullbo->mActionID = 6;
                } else {
                    bullbo->mActionID = 0;
                }

                bullbo->mActionMode = 0;
            } else if (bullbo->mActionID == 21) {
                bullbo->mActionMode = 0;
            } else if ((bullbo->mStatusFlags & 1) == 0) {
                if (bullbo->mActionID == 6 && (bullbo->mActionMode == 2 || bullbo->mActionMode == 3)) {
                    bullbo->mActionMode = 1;
                } else {
                    bullbo->mActionMode = 0;
                }

                bullbo->mActionID = 7;
                bullbo->field_0x698[1] = cM_rndF(30.0f) + 80.0f;
            }
        }

        a_this->current.angle.x = 0;
        i_this->mWbActorID = -1;
    }

    i_this->field_0x9bc = 0;
    if (i_this->mpMorfBowAnm != NULL) {
        i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 10), 0,
                                    1.0f, 1.0f, 0.0f, -1.0f);
        i_this->mpMorfBowAnm->setFrame(10.0f);
    }

    a_this->home.pos = a_this->current.pos;
}

static void* s_wb_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_WB && ((e_wb_class*)i_actor)->mActionID != ACTION_DROP &&
        ((e_wb_class*)i_actor)->mActionID != ACTION_DAMAGE && ((e_wb_class*)i_actor)->mActionID != ACTION_A_DAMAGE && ((e_wb_class*)i_actor)->mActionID != ACTION_STAND) {
        e_rd_class* i_this = (e_rd_class*)i_data;
        if (i_this->mBossMode == ((e_wb_class*)i_actor)->field_0x79d &&
            (data_80519230 != 0 || (((e_wb_class*)i_actor)->mStatusFlags & 3) != 3) && target_info_count < 10) {
            target_info[target_info_count] = (fopAc_ac_c*)i_actor;
            target_info_count++;
        }
    }

    return NULL;
}

static fpc_ProcID search_wb(e_rd_class* i_this, s16 param_2) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

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
    f32 fVar1 = 100.0f;

    f32 fVar2 = 1500.0f;
    // Hyrule Field - Bridge of Eldin
    if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0 && fopAcM_GetRoomNo(a_this) == 0) {
        fVar2 = HREG_F(0) + 5000.0f;
    }

    if (target_info_count != 0) {
        cXyz sp54, sp60;
        fopAc_ac_c* bullbo;
        int i = 0;
        while (i < target_info_count) {
            bullbo = (fopAc_ac_c*)target_info[i];
            sp54.x = bullbo->current.pos.x - a_this->eyePos.x;
            sp54.y = (bullbo->current.pos.y + 50.0f) - a_this->eyePos.y;
            sp54.z = bullbo->current.pos.z - a_this->eyePos.z;
            if (JMAFastSqrt(sp54.x * sp54.x + sp54.z * sp54.z) < fVar1) {
                if (param_2 == 1) {
                    return fopAcM_GetID(bullbo);
                }

                s16 sVar1 = a_this->shape_angle.y - cM_atan2s(sp54.x, sp54.z);
                if (sVar1 < param_2 && sVar1 > (s16)-param_2 && !other_bg_check(i_this, bullbo)) {
                    return fopAcM_GetID(bullbo);
                }
            }

            i++;
            if (i == target_info_count) {
                i = 0;
                fVar1 += 100.0f;
                if (fVar1 > fVar2) {
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
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    if (data_80519200 != 118 || fopAcM_GetRoomNo(a_this) != 1) {
        i_this->mWbActorID = search_wb(i_this, param_2);
        e_wb_class* bullbo = (e_wb_class*)fopAcM_SearchByID(i_this->mWbActorID);
        if (bullbo != NULL) {
            if (data_80519200 != 0) {
                if (i_this->field_0x5bc != 1) {
                    if ((bullbo->mStatusFlags & 1) == 0) {
                        return;
                    }
                } else if ((bullbo->mStatusFlags & 1) != 0) {
                    return;
                }
            }

            i_this->mAction = ACTION_WB_SEARCH;
            i_this->mMode = 0;

            if ((bullbo->mStatusFlags & 1) == 0) {
                bullbo->mStatusFlags |= 1;
                i_this->field_0x9be = 1;
            } else if ((bullbo->mStatusFlags & 2) == 0) {
                bullbo->mStatusFlags |= 2;
                i_this->field_0x9be = 2;
            }
        }
    }
}

static BOOL wb_init_ride(e_rd_class* i_this) {
    i_this->mWbActorID = search_wb(i_this, 1);
    e_wb_class* bullbo = (e_wb_class*)fopAcM_SearchByID(i_this->mWbActorID);

    if (bullbo != NULL) {
        i_this->mAction = ACTION_WB_RIDE;
        i_this->mMode = 0;
        i_this->field_0x9bc = 2;
        i_this->field_0x9be = i_this->field_0x5b6;
        bullbo->mStatusFlags |= (s8)i_this->field_0x5b6;
        bullbo->mActionID = 1;

        if (i_this->field_0x5bc != 0 && i_this->field_0x9be == 1) {
            i_this->field_0x5bd = 1;
        }

        return TRUE;
    }

    OS_REPORT("......CAN NOT_ RIDE  !!!!\n");
    return FALSE;
}

static BOOL pl_check(e_rd_class* i_this, f32 param_2, s16 param_3) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* actor = get_pla(a_this);

    if ((data_80519200 == 0 && dComIfGp_event_runCheck())) {
        return FALSE;
    }

    // Hidden Village
    if (strcmp(dComIfGp_getStartStageName(), "F_SP128") == 0 && actor->current.pos.z < -9800.0f) {
        return FALSE;
    }

    if (S_find != 0) {
        param_2 = 10000.0f;
    }

    if (i_this->mPlayerDistance < param_2) {
        s16 sVar1 = a_this->shape_angle.y - i_this->mPlayerAngleY;
        if (sVar1 < param_3 && sVar1 > (s16)-param_3 && !other_bg_check(i_this, actor)) {
            return TRUE;
        }

        for (int i = 0; i <= 2; i++) {
            if (i_this->field_0xd58[i].ChkCoHit()) {
                if (daPy_getPlayerActorClass() == dCc_GetAc(i_this->field_0xd58[i].GetCoHitObj()->GetAc())) {
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

static dBomb_c* bomb_view_check(e_rd_class* i_this) {
    return search_bomb(i_this, 1);
}

static dBomb_c* bomb_check(e_rd_class* i_this) {
    return search_bomb(i_this, 0);
}

static BOOL move_gake_check(e_rd_class* i_this, f32 param_2) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dBgS_GndChk gnd_chk;
    cXyz sp74, sp80;

    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    sp74.x = 0.0f;
    sp74.y = 100.0f;
    sp74.z = param_2;
    MtxPosition(&sp74, &sp80);
    sp80 += a_this->current.pos;
    gnd_chk.SetPos(&sp80);
    if ((a_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk)) > 300.0f) {
        return TRUE;
    }

    return FALSE;
}

static BOOL way_check(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz spac;

    // Hidden Village
    if (strcmp(dComIfGp_getStartStageName(), "F_SP128") == 0 && a_this->current.pos.z < -8400.0f) {
        spac.x = a_this->home.pos.x - a_this->current.pos.x;
        spac.z = a_this->home.pos.z - a_this->current.pos.z;
        i_this->field_0x5cc = cM_atan2s(spac.x, spac.z);
        return TRUE;
    }

    cXyz start, end;
    s16 sVar1 = cM_rndF(65535.0f);
    f32 fVar1 = 1000.0f;
    start = a_this->current.pos;
    start.y += 50.0f;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 16; j++) {
            cMtx_YrotS(*calc_mtx, sVar1);
            spac.x = 0.0f;
            spac.y = 0.0f;
            spac.z = fVar1;
            MtxPosition(&spac, &end);
            end += start;
            dBgS_LinChk lin_chk;
            lin_chk.Set(&start, &end, a_this);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                sVar1 += 0x1000;
            } else {
                i_this->field_0x5cc = sVar1;
                return TRUE;
            }
        }

        fVar1 -= 150.0f;
    }

    return FALSE;
}

static BOOL path_check(e_rd_class* i_this) {
    static u8 check_index[255];
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz start, end;

    start = a_this->current.pos;
    start.y += hREG_F(0) + 10.0f;

    dPnt* points = i_this->mPath->m_points;
    for (int i = 0; i < i_this->mPath->m_num; i++, points++) {
        if (i < 0xFF) {
            end.x = points->m_position.x;
            end.y = points->m_position.y + 10.0f + hREG_F(1);
            end.z = points->m_position.z;
            lin_chk.Set(&start, &end, a_this);
            if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
                check_index[i] = 1;
            } else {
                check_index[i] = 0;
            }
        }
    }

    f32 fVar2, fVar3, fVar4;
    f32 fVar1 = 100.0f;
    bool bVar1 = false;
    for (int i = 0; i < 100; i++) {
        points = i_this->mPath->m_points;
        for (int j = 0; j < i_this->mPath->m_num; j++, points++) {
            if (j < 0xFF && check_index[j] != 0) {
                fVar2 = a_this->current.pos.x - points->m_position.x;
                fVar3 = a_this->current.pos.y - points->m_position.y;
                fVar4 = a_this->current.pos.z - points->m_position.z;
                if (JMAFastSqrt(fVar2 * fVar2 + fVar3 * fVar3 + fVar4 * fVar4) < fVar1) {
                    i_this->field_0xb00 = j;
                    bVar1 = true;
                    break;
                }
            }
        }

        if (bVar1) break;
        fVar1 += 50.0f;
    }

    if (!bVar1) {
        return FALSE;
    }

    if (cM_rndF(1.0f) < 0.5f) {
        i_this->field_0xb01 = 1;
    } else {
        i_this->field_0xb01 = 0xFF;
    }

    return TRUE;
}

static void e_rd_normal(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    if (i_this->field_0x5b6 == 1 || i_this->field_0x5b6 == 2) {
        if (wb_init_ride(i_this)) {
            i_this->field_0x5b6 = 0;
        }
    } else {
        f32 fVar1 = 0.0f;
        s16 sVar1 = 0x4000;
        if (data_80519200 != 0) {
            sVar1 = 0x6000;
        }

        switch (i_this->mMode) {
            case 0:
                if (i_this->field_0x5bd != 0) {
                    anm_init(i_this, BCK_RD_ARMAMENT, 2.0f, 0, 1.0f);
                    i_this->mMode = 1;
                } else {
                    i_this->mMode = 2;
                }
                break;

            case 1:
                if ((int)i_this->mpModelMorf->getFrame() == 11) {
                    i_this->field_0x5bd = 0;
                }

                if (i_this->mpModelMorf->isStop()) {
                    i_this->mMode = 2;
                }
                break;

            case 2:
                if (i_this->field_0x990[0] == 0) {
                    if (i_this->mPath != NULL && path_check(i_this)) {
                        i_this->mAction = ACTION_JYUNKAI;
                        i_this->mMode = 0;
                        return;
                    }

                    if (way_check(i_this)) {
                        anm_init(i_this, BCK_RD_WALK, 10.0f, 2, 1.0f);
                        i_this->mMode = 3;
                        i_this->field_0x990[0] = cM_rndF(100.0f) + 100.0f;
                    }
                } else if (data_80519200 != 0) {
                    sVar1 = 0x7000;
                } else {
                    sVar1 = 0x6000;
                }
                break;

            case 3:
                fVar1 = l_HIO.movement_speed;
                if (a_this->speedF >= 3.0f && move_gake_check(i_this, 100.0f)) {
                    i_this->mMode = 4;
                    i_this->field_0x990[0] = cM_rndF(100.0f) + 50.0f;
                    anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
                } else {
                    if (i_this->field_0x990[0] == 0 || (a_this->speedF >= 3.0f && way_bg_check(i_this, 200.0f))) {
                        i_this->mMode = 2;
                        i_this->field_0x990[0] = cM_rndF(100.0f) + 50.0f;
                        anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
                    }
                }
                break;

            case 4:
                i_this->field_0x9c8 = 0xFF;
                if (i_this->field_0x990[0] == 0) {
                    i_this->field_0x5cc = a_this->current.angle.y + 0x8000 + (s16)cM_rndFX(4000.0f);
                    anm_init(i_this, BCK_RD_WALK, 10.0f, 2, 1.0f);
                    i_this->mMode = 3;
                    i_this->field_0x990[0] = cM_rndF(100.0f) + 100.0f;
                }
        }

        if (fVar1) {
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5cc, 8, 0x400);
            s16 sVar1 = a_this->current.angle.y - i_this->field_0x5cc;
            if (sVar1 > 0x400 || sVar1 < -0x400) {
                fVar1 = 0.0f;
            }
        }

        cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, 3.0f);
        f32 fVar2 = i_this->field_0x980;
        BOOL bVar1 = FALSE;
        if (i_this->field_0x9a1 == 1 || S_find != 0) {
            if (i_this->field_0x9a1 == 1) {
                bVar1 = TRUE;
            }

            fVar2 = 10000.0f;
            sVar1 = 0x7FFF;
        }

        if (((i_this->field_0x970 & 7) == 0 || bVar1) && i_this->mMode >= 2) {
            if ((i_this->field_0x970 & 8) != 0 && !bVar1) {
                wb_check(i_this, sVar1);
            } else {
                if (pl_check(i_this, fVar2, sVar1)) {
                    if (i_this->field_0x5bc == 1) {
                        i_this->mAction = ACTION_FIGHT_RUN;
                        i_this->mMode = 0xFFF6;
                        i_this->field_0x990[0] = 60;
                    } else if (i_this->field_0x5bc >= 2) {
                        i_this->mAction = ACTION_BOW_RUN;
                        i_this->mMode = 0xFFF6;
                    }
                }

                if (bomb_view_check(i_this) != NULL) {
                    i_this->mAction = ACTION_BOMB;
                    i_this->mMode = 0;
                }
            }
        }
    }
}

static void e_rd_fight_run(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp64, sp70;
    f32 fVar3 = 0.0f;
    s8 bVar1 = true;

    if (!pl_check(i_this, i_this->field_0x980 + 50.0f, 0x7FFF) && i_this->field_0x990[0] == 0) {
        if (i_this->field_0x99e == 0) {
            bVar1 = false;
        }
    } else {
        i_this->field_0x99e = 35;
    }

    f32 fVar1;
    if (data_80519200 != 0) {
        fVar1 = 24.0f;
    } else {
        fVar1 = l_HIO.dash_speed;
    }

    if (i_this->field_0x5bc != 1) {
        if (i_this->field_0x5bc >= 2) {
            i_this->mAction = ACTION_BOW_RUN;
        } else {
            i_this->mAction = ACTION_NORMAL;
            i_this->field_0x990[0] = cM_rndF(50.0f) + 50.0f;
            anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
        }

        i_this->mMode = 0;
    } else {
        i_this->mPlayerDistance -= fabsf(a_this->current.pos.y - player->current.pos.y) * 0.7f;

        switch (i_this->mMode) {
            case -20:
                if ((int)i_this->mpModelMorf->getFrame() == 11) {
                    i_this->field_0x5bd = 0;
                }

                if (i_this->mpModelMorf->isStop()) {
                    i_this->mMode = 0;
                }
                break;

            case -10:
                anm_init(i_this, BCK_RD_WAIT01, 7.0f, 2, 1.0f);
                i_this->field_0x990[1] = cM_rndF(10.0f) + 15.0f;
                i_this->mMode = 0xFFF7;
                break;

            case -9:
                if (i_this->field_0x990[1] == 0) {
                    i_this->mMode = 0;
                }
                break;

            case 0:
                if (i_this->field_0x5bd != 0) {
                    anm_init(i_this, BCK_RD_ARMAMENT, 2.0f, 0, 1.0f);
                    i_this->mMode = 0xFFEC;
                } else if (i_this->field_0x990[1] == 0) {
                    anm_init(i_this, BCK_RD_RUN, 5.0f, 2, cM_rndFX(0.05f) + 1.0f);
                    i_this->mMode = 1;
                }
                break;

            case 1:
                fVar3 = fVar1;
                if (i_this->mPlayerDistance < l_HIO.battle_init_range) {
                    anm_init(i_this, BCK_RD_KAMAE, 5.0f, 2, 1.0f);
                    i_this->mMode = 2;
                }
                break;

            case 2:
                fVar3 = l_HIO.movement_speed;
                if (i_this->mPlayerDistance > l_HIO.battle_init_range + 50.0f) {
                    i_this->mMode = 0;
                } else if (i_this->mPlayerDistance < l_HIO.battle_init_range - 50.0f) {
                    i_this->mMode = 3;
                }
                break;

            case 3:
                fVar3 = -l_HIO.movement_speed;
                if (i_this->mPlayerDistance > l_HIO.battle_init_range) {
                    i_this->mMode = 2;
                }
                break;

            case 5:
                if (i_this->field_0x990[3] != 0) {
                    i_this->field_0x9c8 = 0xFF;
                }

                fVar3 = 0.0f;
                a_this->speedF = fVar3;
                if (!move_gake_check(i_this, 100.0f)) {
                    i_this->mMode = 2;
                }
                break;
        }

        if (i_this->mMode < 5 && move_gake_check(i_this, 100.0f)) {
            i_this->mMode = 5;
            i_this->field_0x990[3] = cM_rndF(15.0f) + 30.0f;
            anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
        }

        if (i_this->mMode >= 0) {
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 4, 0x800);
            s16 sVar1 = a_this->current.angle.y - i_this->mPlayerAngleY;
            if (sVar1 > 0x400 || sVar1 < -0x400) {
                fVar3 = 0.0f;
            }
        }

        cLib_addCalc2(&a_this->speedF, fVar3, 1.0f, 5.0f);

        if (!bVar1) {
            i_this->mAction = ACTION_NORMAL;
            if (i_this->mMode == 5) {
                i_this->field_0x5cc = a_this->current.angle.y + 0x8000 + (s16)cM_rndFX(4000.0f);
                anm_init(i_this, BCK_RD_WALK, 10.0f, 2, 1.0f);
                i_this->mMode = 3;
                i_this->field_0x990[0] = cM_rndF(100.0f) + 100.0f;
                i_this->field_0x990[1] = 40;
            } else {
                i_this->mMode = 0;
                i_this->field_0x990[0] = cM_rndF(50.0f) + 50.0f;
                anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
            }
        } else {
            if (i_this->mPlayerDistance < l_HIO.battle_init_range && (i_this->field_0xa20 & cc_pl_cut_bit_get()) != 0 &&
                i_this->field_0x9f0 < 0.1f) {
                i_this->mAction = ACTION_AVOID;
                i_this->mMode = 0;
                i_this->field_0x998 = 20;
            } else if (i_this->field_0x5bd == 0 && i_this->mPlayerDistance < l_HIO.attack_init_range && i_this->field_0x990[2] == 0) {
                i_this->field_0x990[2] = cM_rndF(20.0f) + 20.0f;
                if (cM_rndF(1.0f) < 0.5f) {
                    i_this->mAction = ACTION_FIGHT;
                    i_this->mMode = 0;
                }
            }
        }

        if ((i_this->field_0x970 & 15) == 0) {
            wb_check(i_this, 0x3000);
            if (bomb_view_check(i_this) != NULL) {
                i_this->mAction = ACTION_BOMB;
                i_this->mMode = 0;
            }
        }

        if (i_this->mAction == ACTION_FIGHT_RUN && i_this->field_0x9c8 >= 0) {
            i_this->field_0x9c8 = 1;
        }
    }
}

static fopAc_ac_c* at_hit_check(e_rd_class* i_this) {
    if (i_this->mMode >= 10) {
        return NULL;
    }

    if (i_this->field_0x1100.ChkAtHit()) {
        return i_this->field_0x1100.GetAtHitObj()->GetAc();
    }

    return NULL;
}

static void e_rd_fight(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp3c, sp48;
    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, BCK_RD_ATTACK01, HREG_F(0) + 6.0f, 0, 1.0f);

            if (l_HIO.field_0x38 == 0) {
                i_this->field_0x990[0] = 25;
            } else if (l_HIO.field_0x38 == 1) {
                i_this->field_0x990[0] = 15;
            } else if (l_HIO.field_0x38 == 2) {
                i_this->field_0x990[0] = 10;
            }

            i_this->mMode = 1;
            i_this->field_0x990[1] = 5;
            i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_WAND_ATTACK, -1);
            break;

        case 1:
            if (i_this->field_0x990[1] != 0) {
                cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 4, 0x800);
            }

            if (frame == 30) {
                i_this->mpModelMorf->setFrame(0.0f);
            } else if (i_this->field_0x990[0] == 0) {
                anm_init(i_this, BCK_RD_ATTACK01, 2.0f, 0, l_HIO.swing_speed);
                i_this->mpModelMorf->setFrame(30.0f);
                i_this->mMode = 2;
            }
            break;

        case 2:
            if (frame >= 44 && frame <= 47) {
                i_this->field_0x9ab = 1;
            }
            
            if (i_this->mpModelMorf->checkFrame(44.0f)) {
                i_this->mSound.startCreatureSound(Z2SE_EN_RD_SWING_CLUB, 0, -1);
            }

            if (i_this->mpModelMorf->checkFrame(50.0f)) {
                i_this->mpModelMorf->setPlaySpeed(1.0f);
            }

            int iVar1;
            if (l_HIO.field_0x38 == 0) {
                iVar1 = 64;
            } else if (l_HIO.field_0x38 == 1) {
                iVar1 = 54;
            } else if (l_HIO.field_0x38 == 2) {
                iVar1 = 44;
            }

            if (frame >= iVar1) {
                i_this->mMode = 3;
                if (l_HIO.field_0x38 == 0) {
                    i_this->field_0x990[0] = 20;
                } else if (l_HIO.field_0x38 == 1) {
                    i_this->field_0x990[0] = 15;
                } else if (l_HIO.field_0x38 == 2) {
                    i_this->field_0x990[0] = 10;
                }

                anm_init(i_this, BCK_RD_KAMAE, 5.0f, 2, 1.0f);
            }
            break;

        case 3:
        case 10:
            if (i_this->field_0x990[0] == 0) {
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mMode = 0;
            }
            break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);
    
    if (i_this->field_0x9ab != 0) {
        fopAc_ac_c* actor = at_hit_check(i_this);
        if (actor != NULL && fopAcM_GetName(actor) == PROC_ALINK) {
            if (daPy_getPlayerActorClass()->checkPlayerGuard()) {
                i_this->mpModelMorf->setPlaySpeed(-1.0f);
                dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
                dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(a_this), 5);
                i_this->mMode = 10;
                i_this->field_0x990[0] = 10;
            }
        }
    }
}

static void e_rd_bow_run(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    f32 fVar1 = 0.0f;
    f32 fVar2;
    if (data_80519200 != 0) {
        fVar2 = 24.0f;
    } else {
        fVar2 = l_HIO.dash_speed;
    }
    s16 playerAngleY = fopAcM_searchPlayerAngleY(a_this);
    s8 bVar1 = 0;

    switch (i_this->mMode) {
        case -20:
            if ((int)i_this->mpModelMorf->getFrame() == 11) {
                i_this->field_0x5bd = 0;
            }

            if (i_this->mpModelMorf->isStop()) {
                i_this->mMode = 0;
            }
            break;

        case -10:
            anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
            i_this->field_0x990[0] = cM_rndF(10.0f) + 15.0f;
            i_this->mMode = 0xFFF7;
            break;

        case -9:
            i_this->field_0x9c8 = 1;
            if (i_this->field_0x990[0] == 0) {
                i_this->mMode = 0;
            }
            break;

        case 0:
            if (i_this->field_0x5bd != 0) {
                anm_init(i_this, BCK_RD_ARMAMENT, 2.0f, 0, 1.0f);
                i_this->mMode = 0xFFEC;
            } else {
                anm_init(i_this, BCK_RD_RUN, 5.0f, 2, 1.0f);
                i_this->field_0x990[0] = cM_rndF(50.0f) + 20.0f;
                if (i_this->mPlayerDistance < l_HIO.attack_range - 100.0f) {
                    i_this->mMode = 1;
                } else if (i_this->mPlayerDistance > l_HIO.attack_range + 100.0f) {
                    i_this->mMode = 2;
                } else {
                    bVar1 = 1;
                }
            }
            break;

        case 1:
            fVar1 = fVar2;
            playerAngleY += 0x8000;
            if (i_this->mPlayerDistance > l_HIO.attack_range || i_this->field_0x990[0] == 0 || i_this->mObjAcch.ChkWallHit() || move_gake_check(i_this, 100.0f)) {
                bVar1 = 1;
            }

            i_this->field_0x990[2] = 20;
            break;

        case 2:
            fVar1 = fVar2;
            if (i_this->mPlayerDistance < l_HIO.attack_range) {
                bVar1 = 1;
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, 5.0f);
    if (i_this->mMode >= 0) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, playerAngleY, 4, 0x1000);
    }

    if (i_this->field_0x5bd == 0 && bVar1) {
        if (i_this->field_0x5b8 == 3) {
            i_this->mAction = ACTION_BOW2;
        } else {
            i_this->mAction = ACTION_BOW;
        }

        i_this->mMode = 0;
        i_this->field_0x990[1] = cM_rndF(150.0f);
    } else if (i_this->field_0x990[2] == 0 && !pl_check(i_this, i_this->field_0x980 + 50.0f, 0x7FFF)) {
        i_this->mAction = ACTION_NORMAL;
        i_this->mMode = 0;
        i_this->field_0x990[0] = cM_rndF(50.0f) + 50.0f;
        anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
    } else if ((i_this->field_0x970 & 15) == 0) {
        wb_check(i_this, 0x3000);
        if (bomb_view_check(i_this) != NULL) {
            i_this->mAction = ACTION_BOMB;
            i_this->mMode = 0;
        }
    }

    if (i_this->mAction != ACTION_BOW_RUN) {
        i_this->field_0x9c8 = 0;
    }
}

static void e_rd_bow(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, BCK_RD_SHOOT_READY, 5.0f, 0, 1.0f);
            i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 7), 0, 5.0f, 0.9f, 0.0f, -1.0f);
            i_this->mMode = 1;
            a_this->speedF = 0.0f;
            break;

        case 1:
            i_this->field_0x9c8 = 4;
            if (frame >= 10) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 20) {
                i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_RD_SHOOT_WAIT, 4.0f, 2, 1.0f);
                i_this->field_0x990[0] = cM_rndF(10.0f) + 10.0f;
                i_this->mMode = 2;
            }
            break;

        case 2:
            i_this->field_0x9c8 = 4;
            if (i_this->field_0x990[0] == 0) {
                if (i_this->field_0x9a4 == 0 && !dMsgObject_isTalkNowCheck()) {
                    anm_init(i_this, BCK_RD_SHOOT, 1.0f, 0, 1.0f);
                    i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 10), 0, 1.0f, 1.0f, 0.0f, -1.0f);
                    i_this->mMode = 3;
                } else {
                    i_this->mAction = ACTION_BOW_RUN;
                    i_this->mMode = 0;
                }
            }

            i_this->field_0x9a2 = 1;
            break;

        case 3:
            if (frame <= 3) {
                i_this->field_0x9a2 = 1;
                i_this->field_0x9c8 = 4;
            }

            if (frame == 2) {
                i_this->field_0x9a3 = 1;
            }

            if (i_this->mpModelMorf->isStop()) {
                if (i_this->field_0x990[1] == 0 && (i_this->mPlayerDistance < l_HIO.attack_range - 200.0f || i_this->mPlayerDistance > l_HIO.attack_range)) {
                    if (!(i_this->mPlayerDistance < l_HIO.attack_range - 200.0f) || !way_bg_check(i_this, -100.0f)) {
                        i_this->mAction = ACTION_BOW_RUN;
                    }

                    i_this->mMode = 0;
                } else {
                    i_this->mMode = 0;
                }
            }
            break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 4, 0x1000);
}

static void* s_command3_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && i_actor != i_data && ((e_rd_class*)i_actor)->mAction == ACTION_COMMANDER) {
        return i_actor;
    }

    return NULL;
}

static s8 e_rd_bow2(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* actor = get_pla(a_this);
    int frame = i_this->mpModelMorf->getFrame();
    s8 rv = 0;

    if (i_this->mMode <= 1) {
        if (i_this->mPlayerDistance < i_this->field_0x980 && !other_bg_check(i_this, actor)) {
            i_this->mMode = 2;
        }

        rv = 1;
    }

    switch (i_this->mMode) {
        case 0:
            if (i_this->field_0x990[0] == 0) {
                anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
                i_this->mMode = 1;
                i_this->field_0x990[0] = cM_rndF(40.0f) + 60.0f;
            }
            break;

        case 1:
            if (i_this->field_0x990[0] == 0) {
                anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
                i_this->field_0x990[0] = cM_rndF(100.0f) + 100.0f;
                i_this->mMode = 0;
            }
            break;

        case 2:
            anm_init(i_this, BCK_RD_SHOOT_READY, 5.0f, 0, 1.0f);
            i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 7), 0, 5.0f, 0.9f, 0.0f, -1.0f);
            i_this->mMode = 3;
            a_this->speedF = 0.0f;
            break;

        case 3:
            i_this->field_0x9c8 = 4;
            if (frame >= 10) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 20) {
                i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_RD_SHOOT_WAIT, 4.0f, 2, 1.0f);
                i_this->field_0x990[0] = cM_rndF(10.0f);
                i_this->mMode = 4;
            }
            break;

        case 4:
            i_this->field_0x9c8 = 4;
            if (i_this->field_0x990[0] == 0) {
                if (i_this->mPlayerDistance > i_this->field_0x980 || other_bg_check(i_this, actor)) {
                    i_this->mMode = 0;
                    return rv;
                }

                if (!dMsgObject_isTalkNowCheck() && i_this->field_0x9a4 == 0) {
                    anm_init(i_this, BCK_RD_SHOOT, 1.0f, 0, 1.0f);
                    i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 10), 0, 1.0f, 1.0f, 0.0f, -1.0f);
                    i_this->mMode = 5;
                }
            }

            i_this->field_0x9a2 = 1;
            break;

        case 5:
            if (frame <= 3) {
                i_this->field_0x9a2 = 1;
                i_this->field_0x9c8 = 4;
            }

            if (frame == 2) {
                i_this->field_0x9a3 = 1;
            }

            if (i_this->mpModelMorf->isStop()) {
                if (i_this->field_0x990[1] == 0 && fabsf(a_this->current.pos.y - actor->current.pos.y) < 300.0f && i_this->mPlayerDistance < l_HIO.attack_range - 200.0f) {
                    if (!way_bg_check(i_this, -100.0f)) {
                        i_this->mAction = ACTION_BOW_RUN;
                        i_this->field_0x1295 = 0;
                    }

                    i_this->mMode = 0;
                } else {
                    i_this->mMode = 0;
                }
            }
            break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 4, 0x1000);
    return rv;
}

static void e_rd_bow_ikki(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp2c;
    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, BCK_RD_SHOOT_READY, 5.0f, 0, 1.0f);
            i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 7), 0, 5.0f, 0.9f, 0.0f, -1.0f);
            i_this->mMode = 1;
            a_this->speedF = 0.0f;
            break;

        case 1:
            if (frame >= 10) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 20) {
                i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_RD_SHOOT_WAIT, 4.0f, 2, 1.0f);
                i_this->mMode = 2;
            }
            break;

        case 2:
            if (i_this->field_0x990[0] == 0 && i_this->field_0x9a4 == 0) {
                anm_init(i_this, BCK_RD_SHOOT, 1.0f, 0, 1.0f);
                i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 10), 0, 1.0f, 1.0f, 0.0f, -1.0f);
                i_this->mMode = 3;
            }

            i_this->field_0x9a2 = 1;
            break;

        case 3:
            if (frame <= 3) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 2) {
                i_this->field_0x9a3 = 1;
            }

            if (i_this->mpModelMorf->isStop()) {
                i_this->mMode = 0;
                i_this->field_0x990[0] = 10000;
            }
            break;
    }

    i_this->field_0x9c8 = 14;
    i_this->field_0x9d4.set(34800.0f, 100.0f, -14900.0f);
    sp2c = i_this->field_0x9d4 - a_this->current.pos;
    a_this->current.angle.y = cM_atan2s(sp2c.x, sp2c.z);
}

static void e_rd_avoid(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, BCK_RD_AVOID, 1.0f, 0, 1.0f);
            i_this->mMode = 1;
            a_this->speedF = -30.0f;
            a_this->speed.y = 20.0f;
            i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DODGE, -1);
            break;

        case 1:
            if (frame == 6) {
                i_this->mpModelMorf->setPlaySpeed(0.0f);
                i_this->mMode = 2;
            }
            break;

        case 2:
            if (i_this->mObjAcch.ChkGroundHit()) {
                i_this->mpModelMorf->setPlaySpeed(1.0f);
                i_this->mMode = 3;
            }
            break;

        case 3:
            cLib_addCalc0(&a_this->speedF, 1.0f, 10.0f);
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mMode = 0;
                i_this->field_0x990[0] = 30;
            }
    }
}

static void e_rd_wb_search(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp54, sp60;
    f32 fVar1 = 0.0f;
    s8 bVar1 = 0;
    fopAc_ac_c* bullbo_actor = fopAcM_SearchByID(i_this->mWbActorID);
    e_wb_class* bullbo = (e_wb_class*)bullbo_actor;
    if (bullbo == NULL) {
        bVar1 = 1;
    } else {
        if (bullbo->mActionID == 22 || bullbo->mActionID == 21 || bullbo->mActionID == 23 || bullbo->mActionID == 24) {
            bVar1 = 1;
        } else if (i_this->mMode <= 3) {
            for (int i = 0; i <= 2; i++) {
                if (i_this->field_0xd58[i].ChkCoHit()) {
                    e_wb_class* actor_p = (e_wb_class*)dCc_GetAc(i_this->field_0xd58[i].GetCoHitObj()->GetAc());
                    if (actor_p != NULL && actor_p != bullbo && fopAcM_GetName(actor_p) == PROC_E_WB) {
                        bVar1 = 1;
                    }
                }
            }

            if (i_this->mObjAcch.ChkWallHit()) {
                bVar1 = 1;
            }
        }
    }

    if (bVar1) {
        i_this->mAction = ACTION_NORMAL;
        i_this->mMode = 0;
        ride_off(i_this);
    } else {
        if (i_this->mMode >= 2 && bullbo->mActionID != 1) {
            bullbo->mActionID = 1;
            bullbo->mActionMode = 0;
        }

        switch (i_this->mMode) {
            case 0:
                anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
                i_this->mMode = 1;
                i_this->field_0x990[0] = cM_rndF(15.0f) + 15.0f;
                // fallthrough
            case 1:
                i_this->field_0x9c8 = 11;
                if (i_this->field_0x990[0] == 0) {
                    i_this->mMode = 2;
                    if (bullbo_actor->speedF > 5.0f) {
                        anm_init(i_this, BCK_RD_SIGN, 3.0f, 2, 1.0f);
                        i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_CALLING, -1);
                        i_this->field_0x990[0] = cM_rndF(20.0f) + 30.0f;
                    } else {
                        i_this->field_0x990[0] = 10;
                    }
                }
                break;

            case 2:
                sp54 = bullbo_actor->current.pos - a_this->current.pos;
                cLib_addCalcAngleS2(&a_this->current.angle.y, cM_atan2s(sp54.x, sp54.z), 4, 0x800);
                i_this->field_0x9c8 = 11;

                if (i_this->field_0x990[0] == 0 && bullbo_actor->speedF < 1.0f) {
                    cXyz sp6c, sp78;
                    cMtx_YrotS(*calc_mtx, bullbo_actor->shape_angle.y);
                    sp54.x = 100.0f;
                    sp54.y = 0.0f;
                    if (i_this->field_0x9be == 1) {
                        sp54.z = TREG_F(0) + 20.0f;
                    } else {
                        sp54.z = TREG_F(10) + -120.0f;
                    }
                    MtxPosition(&sp54, &sp60);

                    sp6c = (bullbo_actor->current.pos + sp60) - a_this->current.pos;
                    sp54.x = -100.0f;
                    MtxPosition(&sp54, &sp60);

                    sp78 = (bullbo_actor->current.pos + sp60) - a_this->current.pos;
                    if (sp6c.abs() < sp78.abs()) {
                        i_this->field_0x9bd = 0;
                        sp60 = sp6c + a_this->current.pos;
                    } else {
                        i_this->field_0x9bd = 1;
                        sp60 = sp78 + a_this->current.pos;
                    }

                    sp54 = sp60 - a_this->current.pos;
                    i_this->field_0x5cc = cM_atan2s(sp54.x, sp54.z);
                    i_this->mMode = 3;
                    anm_init(i_this, BCK_RD_RUN, 5.0f, 2, 1.5f);
                }
                break;

            case 3:
                fVar1 = l_HIO.dash_speed * 1.5f;
                cMtx_YrotS(*calc_mtx, bullbo_actor->shape_angle.y);

                if (i_this->field_0x9bd == 0) {
                    sp54.x = 100.0f;
                } else {
                    sp54.x = -100.0f;
                }
                sp54.y = 0.0f;
                if (i_this->field_0x9be == 1) {
                    sp54.z = TREG_F(0) + 20.0f;
                } else {
                    sp54.z = TREG_F(10) + -120.0f;
                }
                MtxPosition(&sp54, &sp60);

                sp60 += bullbo_actor->current.pos;
                sp54 = sp60 - a_this->current.pos;
                i_this->field_0x5cc = cM_atan2s(sp54.x, sp54.z);
                cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5cc, 2, 0x400);
                sp54 = sp60 - a_this->current.pos;
                sp54.y = 0.0f;

                if (sp54.abs() < TREG_F(19) + 230.0f) {
                    i_this->mMode = 4;
                    if (i_this->field_0x9be == 1) {
                        anm_init(i_this, BCK_RD_ARMAMENT, 2.0f, 0, 1.0f);
                    } else {
                        anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
                        i_this->field_0x990[0] = 10;
                    }
                }
                break;

            case 4:
                cMtx_YrotS(*calc_mtx, bullbo_actor->shape_angle.y);
                sp54.x = 0.0f;
                sp54.y = 0.0f;
                if (i_this->field_0x9be == 1) {
                    sp54.z = 20.0f;
                } else {
                    sp54.z = -20.0f;
                }
                MtxPosition(&sp54, &i_this->field_0x5c0);

                i_this->field_0x5c0 += bullbo_actor->current.pos;
                sp54 = i_this->field_0x5c0 - a_this->current.pos;
                cLib_addCalcAngleS2(&a_this->current.angle.y, (s16)cM_atan2s(sp54.x, sp54.z), 2, 0x1000);

                if (i_this->field_0x5bc != 0 && i_this->field_0x9be == 1) {
                    if ((int)i_this->mpModelMorf->getFrame() == 11) {
                        i_this->field_0x5bd = 1;
                    }

                    if (i_this->mpModelMorf->isStop()) {
                        i_this->mMode = 5;
                        anm_init(i_this, BCK_RD_JUMP_A, 2.0f, 0, 1.0f);
                        i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_JUMP_RIDING, -1);
                    }
                } else if (i_this->field_0x990[0] == 0) {
                    i_this->mMode = 5;
                    anm_init(i_this, BCK_RD_JUMP_A, 2.0f, 0, 1.0f);
                    i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_JUMP_RIDING, -1);
                }
                break;

            case 5:
                if (i_this->mpModelMorf->isStop()) {
                    i_this->mMode = 6;
                    anm_init(i_this, BCK_RD_JUMP_B, 10.0f, 0, 1.0f);
                    i_this->field_0x9bc = 1;
                    i_this->field_0x9c4 = 30.0f;
                    i_this->field_0x990[3] = 20;
                }
                break;

            case 6:
                if (i_this->field_0x9c0 <= 0.0f) {
                    i_this->mAction = ACTION_WB_RIDE;
                    i_this->mMode = 0;
                    bullbo->mActionID = 4;
                    bullbo->mActionMode = 0;
                }
        }

        cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, 3.0f);
        i_this->field_0x9d4 = bullbo_actor->eyePos;
    }
}

static void e_rd_wb_ride(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp1c, sp28;
    e_wb_class* bullbo = (e_wb_class*)fopAcM_SearchByID(i_this->mWbActorID);

    if (bullbo == NULL) {
        i_this->mAction = ACTION_NORMAL;
        i_this->mMode = 0;
        ride_off(i_this);
    } else if ((bullbo->mStatusFlags & 3) == 0) {
        OS_REPORT("......BOSS DL !!!!\n");
        if (bullbo->mActionID == 1) {
            bullbo->mActionID = 0;
        }
        fopAcM_delete(a_this);
    } else {
        switch (i_this->mMode) {
            case 0:
                i_this->mMode = 1;
                if (i_this->mBossMode == 0) {
                    anm_init(i_this, BCK_RD_JUMP_C, 1.0f, 0, 1.0f);
                }
                // fallthrough
            case 1:
                if (i_this->mpModelMorf->isStop() || i_this->mBossMode != 0) {
                    if (i_this->mBossMode != 0) {
                        bullbo->mActionMode = 0;
                        i_this->mMode = 0;

                        if (i_this->mBossMode == 3) {
                            bullbo->mActionID = 17;
                            // Hyrule Field
                            if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0) {
                                bullbo->mActionMode = -100;
                                i_this->mAction = ACTION_IKKI2_START;
                            } else {
                                i_this->mAction = ACTION_WB_RUN;
                            }
                        } else if (i_this->mBossMode == 2) {
                            bullbo->mActionID = 15;
                            i_this->mAction = ACTION_WB_RUN;
                        } else if (i_this->mBossMode == 4) {
                            bullbo->mActionID = 19;
                            i_this->mAction = ACTION_LV9_END;
                        } else if (cDmr_SkipInfo == 0) {
                            i_this->mAction = ACTION_KIBA_START;
                            bullbo->mActionID = 30;
                        } else {
                            bullbo->mActionID = 8;
                            i_this->mAction = ACTION_WB_RUN;
                            bullbo->field_0x79c = 10;
                            Z2GetAudioMgr()->subBgmStart(Z2BGM_HORSE_BATTLE);
                        }
                    } else {
                        bullbo->mActionID = 6;
                        bullbo->mActionMode = 0;
                        i_this->mAction = ACTION_WB_RUN;
                        i_this->mMode = 0;
                    }
                }
            }
    }
}

static void e_rd_wb_run(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp3c, sp48;
    fopAc_ac_c* base_bulb_p = (fopAc_ac_c*) fopAcM_SearchByID(i_this->mWbActorID);

    if (base_bulb_p == NULL) {
        fopAcM_delete(a_this);
        return;
    }

    e_wb_class* bullbo = (e_wb_class*) base_bulb_p;
    if (bullbo->field_0x17e1) {
        fopAcM_setStageLayer(a_this);
    }

    if ((bullbo->mStatusFlags & 3) == 0) {
        fopAcM_delete(a_this);
        return;
    }

    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        dAttention_c* attention = dComIfGp_getAttention();
        if (attention->Lockon() && a_this == attention->LockonTarget(0)) {
            bullbo->field_0x6c0 = 1;
        }
    }

    if ((bullbo->mStatusFlags & 1) == 0 && bullbo->mActionID == ACTION_BOW2 && i_this->mMode != 40) {
        anm_init(i_this, BCK_RD_RRUN02_BACK, 5.0f, 2, 1.0f);
        i_this->mMode = 40;

        if (i_this->mpMorfBowAnm != NULL) {
            i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, BCK_RD_BOW_SHOOT),
                                         0, 1.0f, 1.0f, 0.0f, -1.0f);
        }
    }

    f32 playerDistance = fopAcM_searchPlayerDistanceXZ(a_this);
    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
        case 0:
            if ((bullbo->mStatusFlags & 4) != 0) {
                if (i_this->field_0x9be == 1) {
                    if (bullbo->field_0x6d0 < 0) {
                        anm_init(i_this, BCK_RD_RSTEP_L, 10.0f, 2, 1.0f);
                    } else {
                        anm_init(i_this, BCK_RD_RSTEP_R, 10.0f, 2, 1.0f);
                    }
                } else {
                    anm_init(i_this, BCK_RD_RSTEP_BACK, 10.0f, 2, 1.0f);
                }
            } else {
                if ((bullbo->mStatusFlags & 8) != 0) {
                    if (i_this->field_0x9be == 1) {
                        anm_init(i_this, BCK_RD_RDAMAGE, 3.0f, 0, 1.0f);
                    } else {
                        anm_init(i_this, BCK_RD_RDAMAGE_BACK, 3.0f, 0, 1.0f);
                    }

                    i_this->field_0x990[1] = cM_rndF(50.0f) + 50.0f;
                } else {
                    if ((bullbo->mStatusFlags & 16) != 0) {
                        if (i_this->field_0x9be == 1) {
                            anm_init(i_this, BCK_RD_RRUN, 5.0f, 2, 1.0f);
                        } else {
                            anm_init(i_this, BCK_RD_RRUN_BACK, 5.0f, 2, 1.0f);
                        }
                    } else {
                        if ((bullbo->mStatusFlags & 32) != 0) {
                            if (i_this->field_0x9be == 1) {
                                anm_init(i_this, BCK_RD_RWAIT, 5.0f, 2, 1.0f);
                            } else {
                                anm_init(i_this, BCK_RD_RWAIT_BACK, 5.0f, 2, 1.0f);
                            }
                        }
                    }
                }
            }

            i_this->mpModelMorf->setPlaySpeed(bullbo->mpModelMorf->getPlaySpeed());

            if (i_this->field_0x9be == 1) {
                i_this->field_0x9c8 = 2;
            } else {
                i_this->field_0x9c8 = 1;
                if (i_this->field_0x5bc == 1 && base_bulb_p->speedF > 10.0f) {
                    if (playerDistance < TREG_F(11) + 850.0f && playerDistance < TREG_F(11) + 750.0f
                        && i_this->field_0x990[2] == 0) {
                        s16 sVar1 = a_this->shape_angle.y - i_this->mPlayerAngleY;
                        if (sVar1 > 0x1000 && sVar1 < 0x4000) {
                            i_this->mMode = 10;
                        } else if (sVar1 < -0x1000 && sVar1 > -0x4000) {
                            i_this->mMode = 20;
                        }

                        if (daPy_getPlayerActorClass()->checkHorseRide() && dComIfGp_getHorseActor()->speedF >= 20.0f) {
                            i_this->field_0x990[2] = cM_rndF(20.0f) + 20.0f;
                        }
                    }
                } else {
                    if (i_this->field_0x990[1] == 0 && i_this->field_0x5bc >= 2) {
                        anm_init(i_this, BCK_RD_RSHOOT_READY, 5.0f, 0, 1.0f);
                        i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, BCK_RD_BOW_RREADY), 0, 5.0f, 1.0f, 0.0f, -1.0f);
                        i_this->mMode = 30;
                    }
                }
            }
            break;

        case 10:
            anm_init(i_this, BCK_RD_RATTACK01_WAIT, 10.0f, 2, 1.0f);
            i_this->mMode = 11;
            // fallthrough
        case 11:
            if (playerDistance > 800.0f + TREG_F(10)) {
                i_this->mMode = 0;
                if (i_this->field_0x9be == 1) {
                    anm_init(i_this, BCK_RD_RRUN, 10.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_RRUN_BACK, 10.0f, 2, 1.0f);
                }
            } else if (playerDistance < 550.0f && i_this->field_0x990[2] == 0) {
                i_this->mMode = 25;
                if (daPy_getPlayerActorClass()->checkHorseRide()) {
                    anm_init(i_this, BCK_RD_RATTACK03, 5.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_RATTACK01, 5.0f, 0, 1.0f);
                }
            }
            break;

        case 20:
            anm_init(i_this, BCK_RD_RATTACK02_WAIT, 10.0f, 2, 1.0f);
            i_this->mMode = 21;
            // fallthrough
        case 21:
            i_this->field_0x9ab = 1;
            if (playerDistance > 800.0f + TREG_F(10)) {
                i_this->mMode = 0;
                if (i_this->field_0x9be == 1) {
                    anm_init(i_this, BCK_RD_RRUN, 10.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_RRUN_BACK, 10.0f, 2, 1.0f);
                }
            } else if (playerDistance < 550.0f && i_this->field_0x990[2] == 0) {
                i_this->mMode = 25;
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

            if (i_this->mpModelMorf->checkFrame(15.0f)) {
                i_this->mSound.startCreatureSound(Z2SE_EN_RD_SWING_CLUB, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                if (i_this->field_0x9be == 1) {
                    anm_init(i_this, BCK_RD_RRUN, 10.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_RRUN_BACK, 10.0f, 2, 1.0f);
                    i_this->field_0x990[2] = cM_rndF(20.0f) + 20.0f;
                }

                i_this->mMode = 0;
            }
            break;

        case 30:
            i_this->field_0x9c8 = 3;
            if (frame == 14) {
                i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                i_this->mMode = 31;
                anm_init(i_this, BCK_RD_RSHOOT_WAIT, 4.0f, 2, 1.0f);
            }

            i_this->field_0x9a2 = 1;
            break;

        case 31:
            i_this->field_0x9c8 = 3;
            if (i_this->field_0x990[1] == 0 && i_this->mPlayerDistance < l_HIO.mounted_launch_distance
                && i_this->field_0x9a4 == 0 && !dComIfGp_event_runCheck()) {
                anm_init(i_this, BCK_RD_RSHOOT, 1.0f, 0, 1.0f);
                i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 10),
                                                0, 1.0f, 1.0f, 0.0f, -1.0f);
                i_this->mMode = 32;
            }

            i_this->field_0x9a2 = 1;
            break;

        case 32:
            if (frame <= 3 + XREG_S(5)) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 2 + XREG_S(7)) {
                i_this->field_0x9a3 = 1;
            }

            i_this->field_0x9c8 = 3;

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_RD_RSHOOT_READY, 5.0f, 0, 1.0f);
                i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 8),
                                                0, 5.0f, 1.0f, 0.0f, -1.0f);
                i_this->mMode = 30;

                if (i_this->field_0x9bc == 2 && i_this->field_0x5bb != 0) {
                    i_this->field_0x990[1] = cM_rndF(30.0f) + 20.0f;
                } else {
                    i_this->field_0x990[1] = cM_rndF(50.0f) + 30.0f;
                }
            }
            break;

        case 40:
            if (bullbo->mActionID != 7) {
                i_this->mMode = 0;
                anm_init(i_this, BCK_RD_RWAIT_BACK, 5.0f, 2, 1.0f);
            }
            break;

        case 50:
            if (i_this->field_0x990[3] == 0) {
                i_this->mMode = 0;
                if (i_this->field_0x9be == 1) {
                    anm_init(i_this, BCK_RD_RRUN, 10.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_RRUN_BACK, 10.0f, 2, 1.0f);
                    i_this->field_0x990[2] = cM_rndF(50.0f) + 50.0f;
                }
            }
    }

    if (i_this->field_0x9be == 2 && i_this->mAnmID == BCK_RD_RRUN_BACK) {
        i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_RIDING_YELL, -1);
    }
}

static void* s_wbrun_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_WB) {
        e_wb_class* bullbo = (e_wb_class*)i_actor;
        if (bullbo->field_0x79d == 0) {
            cXyz sp24(bullbo->mEnemy.current.pos - ((fopAc_ac_c*)i_data)->current.pos);
            if (sp24.abs() > KREG_F(11) + 7000.0f) {
                return i_actor;
            }
        }
    }

    return NULL;
}

static void e_rd_wb_run_B(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    e_wb_class* bullbo = (e_wb_class*)fopAcM_SearchByID(i_this->mWbActorID);
    dAttention_c* attention;

    if (bullbo != NULL) {
        if (i_this->mBossMode == 2 || i_this->mBossMode == 3) {
            fopAcM_OffStatus(a_this, 0);
            a_this->attention_info.flags = 0;
        }

        if (daPy_getPlayerActorClass()->checkHorseRide()) {
            attention = dComIfGp_getAttention();
            if (attention->Lockon() && a_this == attention->LockonTarget(0)) {
                bullbo->field_0x6c0 = 1;
            }
        }

        switch (i_this->mMode) {
            case 0: {
                if ((bullbo->mStatusFlags & 0x400) != 0) {
                    anm_init(i_this, e_rdb_class::BCK_RB_RSTARTLOOP, 10.0f, 2, 1.0f);
                } else if ((bullbo->mStatusFlags & 0x800) != 0) {
                    anm_init(i_this, e_rdb_class::BCK_RB_RSTART, 10.0f, 2, 1.0f);
                }

                i_this->mpModelMorf->setPlaySpeed(bullbo->mpModelMorf->getPlaySpeed());
                break;
            }
        }

        if (i_this->mAnmID == e_rdb_class::BCK_RB_RGUARD_F || i_this->mAnmID == e_rdb_class::BCK_RB_RDAMAGEB 
            || i_this->mAnmID == e_rdb_class::BCK_RB_RGUARD_L || i_this->mAnmID == e_rdb_class::BCK_RB_RGUARD_R) {
            i_this->field_0x9c8 = 0;

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, e_rdb_class::BCK_RB_RRUN, 5.0f, 2, 1.0f);
            }
        } else if (i_this->mAnmID == e_rdb_class::BCK_RB_RPLAY_RUNR || i_this->mAnmID == e_rdb_class::BCK_RB_RPLAY_RUNL 
                   || i_this->mAnmID == e_rdb_class::BCK_RB_RPLAY_STOP) {
            if ((int)i_this->mpModelMorf->getFrame() == 21) {
                i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_HORN, -1);
            }
        } else if (i_this->mAnmID == e_rdb_class::BCK_RB_RRUN) {
            if (i_this->field_0x990[1] == 0) {
                i_this->field_0x990[1] = cM_rndF(100.0f) + 70.0f;
                i_this->field_0x990[2] = cM_rndF(30.0f) + 30.0f;
            }

            if (i_this->field_0x990[2] != 0) {
                i_this->field_0x9c8 = 1;
            }

            if (a_this->speedF < 5.0f) {
                anm_init(i_this, e_rdb_class::BCK_RB_RWAIT, 5.0f, 2, 1.0f);
            }
        } else {
            i_this->field_0x9c8 = 1;
            if (i_this->field_0x990[3] < 200) {
                i_this->field_0x990[3] = cM_rndF(100.0f) + 200.0f;
            }

            if (a_this->speedF > 10.0f) {
                anm_init(i_this, e_rdb_class::BCK_RB_RRUN, 5.0f, 2, 1.0f);
            }
        }

        
        if (fpcM_Search(s_wbrun_sub, i_this) != NULL && i_this->field_0x990[3] == 0) {
            i_this->field_0x6c2 = 1;
            i_this->field_0x990[3] = cM_rndF(100.0f) + 200.0f;
        }

        // check if we need to jump a fence
        if ((bullbo->mStatusFlags & 0x100) != 0) {
            i_this->mAction = ACTION_WB_BJUMP;
            i_this->mMode = 0;
        }
    }
}

static void e_rd_wb_bjump(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    e_wb_class* bullbo = (e_wb_class*)fopAcM_SearchByID(i_this->mWbActorID);
    dAttention_c* attention;

    if (bullbo != NULL) {
        if (daPy_getPlayerActorClass()->checkHorseRide()) {
            attention = dComIfGp_getAttention();
            if (attention->Lockon() && i_this == (e_rd_class*)attention->LockonTarget(0)) {
                bullbo->field_0x6c0 = 1;
            }
        }

        switch (i_this->mMode) {
            case 0:
                i_this->field_0x680 = 0;
                anm_init(i_this, BCK_RD_RSTEP_RBACK, 3.0f, 0, 1.0f);
                i_this->mMode = 1;
                i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_HOICK, -1);
                break;

            case 1:
                if (i_this->mpModelMorf->isStop()) {
                    i_this->mMode = 2;
                    anm_init(i_this, BCK_RD_RUN, 2.0f, 0, 1.0f);
                }
                break;

            case 2:
                if ((bullbo->mStatusFlags & 0x200) != 0) {
                    anm_init(i_this, BCK_RD_RUN02, 2.0f, 0, 1.0f);
                    i_this->mMode = 3;
                }
                break;

            case 3:
                if (i_this->mpModelMorf->isStop()) {
                    i_this->mAction = ACTION_WB_RUN;
                    i_this->mMode = 0;
                }
        }
    }
}

static void e_rd_bomb_action(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp48, sp54;
    dBomb_c* bomb = bomb_check(i_this);
    if (bomb == NULL) {
        i_this->mAction = ACTION_FIGHT_RUN;
        i_this->mMode = 0;
        return;
    }

    sp48.x = bomb->current.pos.x - a_this->current.pos.x;
    sp48.z = bomb->current.pos.z - a_this->current.pos.z;
    s16 sVar1 = cM_atan2s(sp48.x, sp48.z);
    f32 fVar1 = 0.0f;
    s16 sVar2;

    switch (i_this->mMode) {
        case 0:
            i_this->mMode = 1;
            anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
            i_this->field_0x990[1] = cM_rndF(10.0f) + 10.0f;
            // fallthrough
        case 1:
            if (i_this->field_0x990[1] == 0 && bomb->speedF < 0.1f) {
                i_this->mMode = 2;
                anm_init(i_this, BCK_RD_RUN, 5.0f, 2, cM_rndFX(0.05f) + 1.0f);
            }
            break;

        case 2:
            if (bomb->speedF < 0.1f && !bomb->checkStateCarry()) {
                if (JMAFastSqrt(sp48.x * sp48.x + sp48.z * sp48.z) < 250.0f) {
                    sVar2 = i_this->mPlayerAngleY - a_this->shape_angle.y;
                    if (sVar2 < 0x4000 && sVar2 > -0x4000) {
                        i_this->mMode = 5;
                        break;
                    }
                }
            }

            sVar1 += 0x8000;
            fVar1 = l_HIO.dash_speed;
            if (JMAFastSqrt(sp48.x * sp48.x + sp48.z * sp48.z) > 600.0f) {
                i_this->mMode = 3;
                anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
            }
            break;

        case 3:
            sVar1 = i_this->mPlayerAngleY;
            if (JMAFastSqrt(sp48.x * sp48.x + sp48.z * sp48.z) < 500.0f) {
                i_this->mMode = 0;
            }
            break;

        case 5:
            fVar1 = l_HIO.dash_speed;
            i_this->field_0x9ad = 0;
            if (bomb->speedF > 0.1f) {
                i_this->mMode = 0;
            } else if (JMAFastSqrt(sp48.x * sp48.x + sp48.z * sp48.z) < 100.0f) {
                i_this->field_0x990[0] = 20;
                i_this->mMode = 6;
                anm_init(i_this, BCK_RD_KICK, 3.0f, 0, 1.0f);
            }
            break;

        case 6:
            i_this->field_0x9ad = 0;
            sVar1 = i_this->mPlayerAngleY;
            if ((int)i_this->mpModelMorf->getFrame() <= 10) {
                cMtx_YrotS(*calc_mtx, sVar1);
                sp48.x = TREG_F(15) + -30.0f;
                sp48.y = 0.0f;
                sp48.z = TREG_F(16) + 60.0f;
                MtxPosition(&sp48, &sp54);
                sp54 += a_this->current.pos;
                cLib_addCalc2(&bomb->current.pos.x, sp54.x, 1.0f, 15.0f);
                cLib_addCalc2(&bomb->current.pos.z, sp54.z, 1.0f, 15.0f);
                if ((int)i_this->mpModelMorf->getFrame() == 10) {
                    bomb->speedF = 30.0f;
                    bomb->speed.y = 30.0f;
                    bomb->current.angle.y = a_this->shape_angle.y;
                }
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
                i_this->mMode = 3;
            }
    }

    cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, 5.0f);
    cLib_addCalcAngleS2(&a_this->current.angle.y, sVar1, 4, 0x1000);
    i_this->field_0x9c8 = 11;
    i_this->field_0x9d4 = bomb->current.pos;
}

static void e_rd_s_damage(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    switch (i_this->mMode) {
        case 0:
            i_this->mMode = 1;
            i_this->field_0x990[0] = 10;
            
            if (i_this->field_0x9be != 0) {
                e_wb_class* bullbo = (e_wb_class*)fopAcM_SearchByID(i_this->mWbActorID);
                if (bullbo != NULL) {
                    bullbo->mStatusFlags &= ~i_this->field_0x9be;
                }

                i_this->field_0x9be = 0;
                i_this->mWbActorID = -1;
            }
            break;

        case 1:
            if (i_this->field_0x990[1] != 0) {
                a_this->onHeadLockFlg();
            }

            if (i_this->mpModelMorf->isStop()) {
                if (i_this->field_0x974 == 7) {
                    i_this->mAction = ACTION_BOW2;
                } else if (i_this->field_0x974 == 26) {
                    i_this->mAction = ACTION_BOW3;
                } else if (!other_bg_check(i_this, dComIfGp_getPlayer(0))) {
                    i_this->mAction = ACTION_FIGHT_RUN;
                    i_this->field_0x990[0] = 40;
                } else {
                    i_this->mAction = ACTION_NORMAL;
                }

                i_this->mMode = 0;
            }
            break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);
}

static int kado_check(e_rd_class* i_this) {
    static int kado_bit[2] = {
        1, 2,
    };

    static f32 kado_check_x[2] = {
        80.0f, -80.0f,
    };

    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz sp9c, spa8, spb4;
    BOOL rv = FALSE;

    cMtx_YrotS(*calc_mtx, i_this->field_0xa0e);
    for (int i = 0; i < 2; i++) {
        spa8 = a_this->current.pos;
        spa8.y += 100.0f;
        sp9c.x = 0.0f;
        sp9c.y = 0.0f;
        sp9c.z = 70.0f;
        MtxPosition(&sp9c, &spb4);
        spa8 += spb4;
        sp9c.y = 50.0f;
        sp9c.z = -150.0f;
        sp9c.x = 1.0f * kado_check_x[i];
        MtxPosition(&sp9c, &spb4);
        spb4 += spa8;
        lin_chk.Set(&spa8, &spb4, a_this);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            rv |= kado_bit[i];
        }
    }

    if (rv == 3) {
        rv = FALSE;
    }

    return rv;
}

static void rd_disappear(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp1c, sp28;

    MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(11), *calc_mtx);
    sp1c.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp1c, &sp28);

    fopAc_ac_c* actor = fopAcM_SearchByName(PROC_NPC_COACH);
    if (actor != NULL) {
        fopAcM_createDisappear(a_this, &sp28, 10, 0, 51);
    } else {
        fopAcM_createDisappear(a_this, &sp28, 10, 0, 11);
    }

    fopAcM_delete(a_this);

    int swBit = fopAcM_GetParam(a_this) >> 24;
    if (swBit != 0xFF) {
        dComIfGs_onSwitch(swBit, fopAcM_GetRoomNo(a_this));
    }

    if (i_this->field_0x9be != 0) {
        e_wb_class* bullbo = (e_wb_class*)fopAcM_SearchByID(i_this->mWbActorID);
        if (bullbo != NULL) {
            bullbo->mStatusFlags &= ~i_this->field_0x9be;
            if (bullbo->mActionID == 1) {
                bullbo->mActionID = 0;
            }
        }
    }
}

static BOOL body_gake(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    J3DModel* model = i_this->mpModelMorf->getModel();
    cXyz sp7c, sp88;

    MTXCopy(model->getAnmMtx(11), *calc_mtx);
    sp7c.set(40.0f, 0.0f, 0.0f);
    MtxPosition(&sp7c, &sp88);

    dBgS_ObjGndChk gnd_chk;
    sp88.y += 100.0f;
    gnd_chk.SetPos(&sp88);
    sp88.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

    if ((a_this->current.pos.y - sp88.y) > 300.0f) {
        return TRUE;
    }

    return FALSE;
}

static void e_rd_damage(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp34, sp40;

    if (a_this->health <= 0) {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    }

    i_this->field_0x6d0 = 0.0f;

    if (daPy_py_c::checkNowWolf() != 0) {
        if (i_this->field_0xafa == 0 && a_this->checkWolfDownStartFlg()) {
            if (i_this->field_0x990[0] > 40) {
                i_this->field_0x990[0] = 90;
                OS_REPORT("      RD WOLFHIT LONG  \n");
            } else if (i_this->field_0x990[0] > 20) {
                i_this->field_0x990[0] = 70;
                OS_REPORT("      RD WOLFHIT MID  \n");
            } else {
                i_this->field_0x990[0] = 50;
                OS_REPORT("      RD WOLFHIT SHORT \n");
            }

            i_this->mMode = 30;
            i_this->field_0x990[1] = 16;
            i_this->field_0xafa = 1;
        }
    } else if (a_this->checkCutDownHitFlg()) {
        a_this->offCutDownHitFlg();
        i_this->mMode = 3;
        i_this->field_0x990[0] = 100;
        i_this->field_0x990[1] = 45;
        a_this->health = 0;
        i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DEATH, -1);
        i_this->field_0x968 = 1;
        daPy_getPlayerActorClass()->onEnemyDead();
        i_this->field_0xadc = 15;
        a_this->offDownFlg();
    }

    s16 sVar1;
    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, BCK_RD_DIEB, 3.0f, 0, 1.0f);
            i_this->mMode = 1;
            a_this->speedF = 0.0f;
            ride_off(i_this);
            i_this->field_0xab8 = 8000.0f;

            if (a_this->health <= 0) {
                i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DEATH, -1);
                i_this->field_0x968 = 1;
                daPy_getPlayerActorClass()->onEnemyDead();

                if (data_80519200 != 0) {
                    i_this->field_0x5ba = 0;
                }
            } else {
                i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DAMAGE, -1);
            }

            i_this->field_0x990[0] = 0;
            break;

        case 1:
            if (i_this->mObjAcch.ChkWallHit() != 0 && i_this->field_0x990[0] == 0) {
                int iVar1 = kado_check(i_this);
                if (iVar1 != 0) {
                    OS_REPORT("              ..KADO KABE ..%x\n", iVar1);
                    if (iVar1 == 2) {
                        i_this->field_0x9f6 = 0x1000;
                        i_this->field_0xa0e += -7000;
                    } else {
                        i_this->field_0x9f6 = -0x1000;
                        i_this->field_0xa0e += 7000;
                    }

                    i_this->field_0xab8 = 8000.0f;
                    i_this->field_0xa24 = 100.0f;
                    i_this->field_0xa2c = 100.0f;
                    i_this->field_0x990[0] = 60;
                } else {
                    i_this->mMode = 10;
                    a_this->speed.y = 0.0f;
                    i_this->field_0x9ec *= 0.2f;
                    i_this->field_0xa0e += 0x8000;
                    i_this->field_0xaf0 = 5;
                    i_this->field_0xa24 = 100.0f;
                    i_this->field_0xa2c = 100.0f;
                    i_this->field_0x9f6 = 0;
                }
            } else {
                cLib_addCalcAngleS2(&i_this->field_0xa0c, -0x4000, 1, 0x300);
                a_this->current.angle.y += i_this->field_0x9f6;
                a_this->shape_angle.y = a_this->current.angle.y;

                if (i_this->field_0xa0c <= -0x3000 && i_this->mObjAcch.ChkGroundHit()) {
                    i_this->field_0xa1e = 10;

                    if (i_this->field_0x1294 != 0) {
                        i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_ASASE_M, 0, -1);
                    } else {
                        i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
                    }

                    i_this->mMode = 2;

                    if (i_this->field_0xa1f == 0) {
                        anm_init(i_this, BCK_RD_DIEA, 3.0f, 0, 1.0f);
                        sVar1 = i_this->field_0xa0e;
                    } else {
                        sVar1 = i_this->field_0xa0e + 0x8000;
                    }

                    a_this->current.angle.y = sVar1;

                    if (i_this->field_0x9f8 != 0) {
                        i_this->field_0x9ec = 0.0f;
                        i_this->field_0xab8 = 2000.0f;
                    } else {
                        if (i_this->field_0xa1f == 0) {
                            i_this->field_0xa24 = -100.0f;
                            i_this->field_0xa2c = -100.0f;
                        } else {
                            i_this->field_0xa24 = 100.0f;
                            i_this->field_0xa2c = 100.0f;
                        }

                        i_this->field_0xa28 = -0x5000;
                        i_this->field_0xa30 = -0x4000;
                        i_this->field_0x9ec *= 0.5f;
                        a_this->speed.y = 20.0f;
                        i_this->field_0x9f6 /= 2;
                        i_this->field_0xab8 = 8000.0f;
                        i_this->field_0x125d = 1;
                    }
                }
            }
            break;

        case 2:
            a_this->current.angle.y += i_this->field_0x9f6;
            a_this->shape_angle.y = a_this->current.angle.y;
            cLib_addCalcAngleS2(&i_this->field_0xa0c, -0x4000, 1, 0x300);

            if (i_this->mObjAcch.ChkGroundHit()) {
                if (a_this->health > 0 && daPy_py_c::checkNowWolf() == 0) {
                    a_this->onDownFlg();
                }

                dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(a_this), 5);
                i_this->field_0xa1e = 10;

                if (i_this->field_0xa1f == 0) {
                    anm_init(i_this, BCK_RD_DIEA, 3.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_DIEB, 3.0f, 0, 1.0f);
                }

                i_this->field_0x9ec = 0.0f;
                i_this->field_0xa0c = -0x4000;

                if (i_this->field_0xa1f == 0) {
                    sVar1 = i_this->field_0xa0e;
                    for (int i = 0; i < 4; i++) {
                        i_this->field_0xad2[i] = cM_rndF(10.0f);
                    }
                } else {
                    sVar1 = i_this->field_0xa0e + 0x8000;
                }
                a_this->current.angle.y = sVar1;

                if (daPy_py_c::checkNowWolf() != 0) {
                    i_this->field_0x990[0] = 80;
                    i_this->field_0x990[1] = 55;
                } else {
                    i_this->field_0x990[0] = cM_rndF(10.0f) + 60.0f;
                    i_this->field_0x990[1] = 35;
                }

                i_this->mMode = 3;
                i_this->field_0xab8 = 1500.0f;
                i_this->field_0x125d = 1;
            }
            break;

        case 3:
            if (body_gake(i_this)) {
                i_this->field_0x9ec = -20.0f;
                i_this->field_0xa0c -= 0x300;
            } else {
                i_this->field_0x9ec = 0.0f;
            }

            if (a_this->health <= 0 && i_this->field_0x990[1] == 0) {
                rd_disappear(i_this);
                return;
            }

            if (daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_DOWN && i_this->field_0x990[0] == 0) {
                i_this->field_0xa0c = -0x4000;
                a_this->offDownFlg();
                i_this->mMode = 20;

                if (i_this->field_0xa1f == 0) {
                    anm_init(i_this, BCK_RD_DIEA_UP, 3.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_DIEB_UP, 3.0f, 0, 1.0f);
                }
            }
            break;

        case 10:
            if (i_this->field_0xaf0 != 0) {
                cLib_addCalcAngleS2(&i_this->field_0xa0c, 0, 1, 0x300);
            } else {
                cLib_addCalcAngleS2(&i_this->field_0xa0c, -0x4000, 1, 0x800);
                if (i_this->mObjAcch.ChkGroundHit()) {
                    i_this->mMode = 2;
                    if (i_this->field_0xa1f == 0) {
                        anm_init(i_this, BCK_RD_DIEA, 3.0f, 0, 1.0f);
                        i_this->field_0xa24 = -100.0f;
                        i_this->field_0xa2c = -100.0f;
                    } else {
                        i_this->field_0xa24 = 100.0f;
                        i_this->field_0xa2c = 100.0f;
                    }

                    i_this->field_0xa28 = -0x5000;
                    i_this->field_0xa30 = -0x4000;
                    i_this->field_0x9ec *= 0.5f;
                    a_this->speed.y = 20.0f;

                    if (i_this->field_0xa1f == 0) {
                        sVar1 = i_this->field_0xa0e;
                    } else {
                        sVar1 = i_this->field_0xa0e + 0x8000;
                    }

                    a_this->current.angle.y = sVar1;
                    i_this->field_0xab8 = 8000.0f;
                }
            }
            break;

        case 20:
            if (i_this->mpModelMorf->getFrame() >= 35.0f) {
                i_this->field_0xa1c = 0;
                i_this->field_0xa18 = 0;
            }

            if (i_this->mpModelMorf->isStop()) {
                i_this->field_0xa0e = 0;
                i_this->field_0xa0c = 0;
                anm_init(i_this, BCK_RD_WAIT01, 0.0f, 2, 1.0f);
                i_this->field_0x990[0] = 5;
                i_this->mMode = 21;

                a_this->offDownFlg();
            }
            break;

        case 21:
            if (i_this->field_0x990[0] == 0) {
                i_this->field_0x998 = 0;
                if (pl_check(i_this, 10000.0f, 0x6000)) {
                    i_this->mAction = ACTION_NORMAL;
                    i_this->mMode = 0;
                } else {
                    i_this->field_0x990[0] = cM_rndF(20.0f) + 20.0f;
                    anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
                    i_this->mMode = 22;
                }
            }
            break;

        case 22:
            if (i_this->field_0x990[0] == 0) {
                i_this->mAction = ACTION_NORMAL;
                i_this->mMode = 0;
                i_this->field_0x990[0] = cM_rndF(50.0f) + 50.0f;
                anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
            }
            break;

        case 30:
            if (!a_this->checkWolfDownPullFlg()) {
                i_this->field_0x990[1] = 0;
            }

            if (i_this->field_0x990[0] == 0) {
                i_this->mMode = 31;
                if (i_this->field_0xa1f == 0) {
                    anm_init(i_this, BCK_RD_DIEA_UP, 3.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, BCK_RD_DIEB_UP, 3.0f, 0, 1.0f);
                }

                i_this->field_0x990[0] = 10;
            } else if (i_this->field_0x990[1] == 1) {
                a_this->onWolfDownPullEndFlg();
                a_this->offCutDownHitFlg();
                a_this->health = 0;
                i_this->field_0x990[0] = 1000;
                i_this->field_0x990[1] = 35;
                i_this->mMode = 3;
            }
            break;

        case 31:
            if (i_this->field_0x990[0] == 0) {
                a_this->offDownFlg();
                i_this->mMode = 20;
                i_this->field_0xafa = 0;
            }
    }

    if (a_this->checkDownFlg()) {
        MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(11), *calc_mtx);
        sp34.set(BREG_F(0), BREG_F(1), BREG_F(2));
        MtxPosition(&sp34, &sp40);
        a_this->setDownPos(&sp40);
        i_this->field_0x9ad = 0;
    }

    if (i_this->field_0xadc != 0) {
        i_this->field_0xadc--;
        i_this->field_0xada = i_this->field_0xadc * cM_ssin(i_this->field_0xadc * 0x3800) * 100.0f;
    }
}

static s16 gake_check(e_rd_class* i_this, f32 param_2) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dBgS_GndChk gnd_chk;
    dBgS_LinChk lin_chk;
    cXyz spf8, sp104, start, end;
    s16 sVar1 = a_this->shape_angle.y;

    for (int i = 0; i < 16; i++, sVar1 += 0x1000) {
        cMtx_YrotS(*calc_mtx, sVar1);
        start.x = 0.0f;
        start.y = 100.0f;
        start.z = param_2;
        MtxPosition(&start, &end);
        end += a_this->current.pos;
        gnd_chk.SetPos(&end);

        if (a_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 300.0f) {
            start = a_this->current.pos;
            start.y += 100.0f;
            lin_chk.Set(&start, &end, a_this);
            if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
                return sVar1;
            }
        }
    }

    return a_this->shape_angle.y;
}

static void* s_bikkuri_sub(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && i_actor != i_data) {
        if (((e_rd_class*)i_actor)->mAction == ACTION_BOW3 && ((e_rd_class*)i_actor)->mMode <= 2) {
            cXyz sp28 = ((fopAc_ac_c*)i_actor)->current.pos - ((fopAc_ac_c*)i_data)->current.pos;
            if (sp28.abs() < KREG_F(11) + 350.0f) {
                anm_init((e_rd_class*)i_actor, 29, 5.0f, 2, 1.3f);
                ((e_rd_class*)i_actor)->mMode = 3;
                ((e_rd_class*)i_actor)->field_0x990[0] = cM_rndF(20.0f) + 40.0f;
            }
        }
    }

    return NULL;
}

static void* s_saku_sub(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_H_Saku) {
        cXyz sp14 = ((daObjH_Saku_c*)i_actor)->field_0x5e0 - ((fopAc_ac_c*)i_data)->current.pos;
        if (sp14.abs() < 200.0f) {
            return i_actor;
        }
    }

    return NULL;
}

static void e_rd_drop(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    f32 fVar1 = 0.0f;

    switch (i_this->mMode) {
        case 0:
            i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_DAMAGE, -1);
            anm_init(i_this, BCK_RD_DAMAGE_S, 2.0f, 0, 1.0f);
            i_this->mMode = 1;

            if (data_80519200 != 0) {
                i_this->mDemoMode = 10;
            }
            break;

        case 1:
            i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_DAMAGE, -1);
            
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_RD_FURA2, 0.0f, 2, 1.0f);
                i_this->mMode = 2;

                if (i_this->mDemoMode != 0) {
                    i_this->field_0x5cc = i_this->mPlayerAngleY;
                } else if (strcmp(dComIfGp_getStartStageName(), "F_SP128") == 0) {
                    // Hidden Village
                    i_this->field_0x5cc = a_this->home.angle.y;
                } else {
                    i_this->field_0x5cc = gake_check(i_this, 200.0f);
                }

                fpcM_Search(s_bikkuri_sub, i_this);
                i_this->field_0x990[0] = 60;
            }
            break;

        case 2:
            i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_DAMAGE, -1);
            fVar1 = 4.0f;
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5cc, 8, 0x400);

            if (i_this->field_0x990[0] == 0) {
                i_this->mAction = ACTION_A_DAMAGE;
                i_this->mMode = 0;
                return;
            }

            if (move_gake_check(i_this, KREG_F(6) + 30.0f)) {
                i_this->mMode = 3;
                i_this->field_0xa0e = a_this->shape_angle.y + 0x8000;
                i_this->field_0xa0c = 0;
                i_this->mpModelMorf->setPlaySpeed(0.3f);
                i_this->field_0xaf8 = 0;
                i_this->field_0x990[0] = 20;
            }
            break;

        case 3:
            if (i_this->field_0x990[0] == 19) {
                daObjH_Saku_c* h_saku = (daObjH_Saku_c*)fpcM_Search(s_saku_sub, i_this);
                if (h_saku != NULL) {
                    h_saku->BreakSet();
                }
            }

            fVar1 = 3.0f;
            
            if (i_this->field_0xa0c > -0x4000) {
                a_this->speed.y = 0.0f;
                i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_DAMAGE, -1);
            } else {
                if (i_this->field_0xa0c > -0x6800) {
                    a_this->speed.y = -fabsf(cM_scos(i_this->field_0xa0c) * (TREG_F(8) + 70.0f));
                    i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_FALLING, -1);
                } else {
                    i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_RD_V_SNIPED_FALLING, -1);
                }
            }

            cLib_addCalcAngleS2(&i_this->field_0xa0c, -0x7000, 2, i_this->field_0xaf8);
            cLib_addCalcAngleS2(&i_this->field_0xaf8, 0x800, 1, 0x50);
            i_this->field_0x9ab = 2;

            if (i_this->field_0x990[0] == 0 && a_this->current.pos.y <= (KREG_F(17) + i_this->mObjAcch.GetGroundH() + 100.0f)) {
                if (i_this->field_0x5b7 == 13 || i_this->field_0x5b7 == 14) {
                    i_this->mAction = ACTION_REG;
                    i_this->mMode = 1;
                    i_this->field_0xafb = 1;
                    i_this->field_0xaf2 = 0;
                } else {
                    a_this->current.pos.y = KREG_F(17) + i_this->mObjAcch.GetGroundH() + 100.0f;
                    i_this->field_0xa0c = -0x4000;
                    i_this->mAction = ACTION_DAMAGE;
                    i_this->mMode = 0;
                    i_this->field_0x9ec = 0.0f;
                    a_this->health = 0;
                }
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, 3.0f);
}

static void e_rd_a_damage(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    i_this->field_0x998 = 3;
    fopAcM_OffStatus(a_this, 0);
    a_this->attention_info.flags = 0;

    switch (i_this->mMode) {
        case 0:
            i_this->mMode = 1;
            anm_init(i_this, BCK_RD_DEAD_ARROW, 5.0f, 0, 1.0f);
            i_this->field_0x990[0] = 80;

            if (data_80519200 != 0) {
                i_this->field_0x5ba = 0;
            }

            dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(a_this), 5);
            break;

        case 1:
            if (i_this->field_0x990[0] == 0) {
                rd_disappear(i_this);
            }
            break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);
}

static void e_rd_stand(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    switch (i_this->mMode) {
        case 0:
            if (i_this->field_0x990[0] == 0) {
                anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
                i_this->mMode = 1;
                i_this->field_0x990[0] = cM_rndF(40.0f) + 60.0f;
            }
            break;

        case 1:
            if (i_this->field_0x990[0] == 0) {
                anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
                i_this->field_0x990[0] = cM_rndF(100.0f) + 100.0f;
                i_this->mMode = 0;
            }
            break;
    }

    f32 fVar1 = i_this->field_0x980;
    s16 sVar1 = 0x4000;
    BOOL bVar1 = FALSE;

    if (i_this->field_0x9a1 == 1 || S_find != 0) {
        if (i_this->field_0x9a1 == 1) {
            bVar1 = TRUE;
        }
        fVar1 = 10000.0f;
        sVar1 = 0x7FFF;
    }

    if ((i_this->field_0x970 & 7) == 0 && fopAcM_GetRoomNo(a_this) == 0) {
        if ((S_find == 0 || (wb_check(i_this, sVar1), i_this->field_0x9be == 0)) && pl_check(i_this, 500.0f, 0x4000)) {
            i_this->mAction = ACTION_FIGHT_RUN;
            i_this->mMode = -10;
            i_this->field_0x990[0] = 60;
        }
    } else if (((i_this->field_0x970 & 7) == 0 || bVar1) && pl_check(i_this, fVar1, sVar1) && i_this->field_0x5bc == 1) {
        OS_REPORT(" <<<<<<<<<<<<<< RD RIDE 4\n");
        i_this->mAction = ACTION_FIGHT_RUN;
        i_this->mMode = -10;
        i_this->field_0x990[0] = 60;
    }
}

static s8 e_rd_bow3(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    if (i_this->field_0x998 == 0 && i_this->mMode != 5 && fpcM_Search(s_command3_sub, i_this) == NULL) {
        i_this->mMode = 5;
        i_this->field_0x990[1] = cM_rndF(20.0f) + 40.0f;
        i_this->field_0x990[0] = cM_rndF(20.0f) + 80.0f;
    }

    int frame = i_this->mpModelMorf->getFrame();
    s8 bVar1 = 1;

    switch (i_this->mMode) {
        case 0:
            bVar1 = 0;
            anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
            i_this->mMode = 1;
            break;

        case 1:
            bVar1 = 0;
            break;

        case 3:
            if (i_this->field_0x990[0] == 0) {
                i_this->mMode = 0;
            }
            break;

        case 5:
            if (i_this->field_0x990[1] == 1) {
                anm_init(i_this, BCK_RD_KYORO2, 5.0f, 2, 1.3f);
            }

            if (i_this->field_0x990[0] == 10) {
                anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
            }

            if (i_this->field_0x990[0] == 0) {
                i_this->mAction = ACTION_BOW2;
                i_this->mMode = 0;
            }
            break;

        case 10:
            if (i_this->field_0x990[0] == 0) {
                anm_init(i_this, BCK_RD_SHOOT_READY, 5.0f, 0, 1.0f);
                i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 7), 0, 5.0f, 0.9f, 0.0f, -1.0f);
                i_this->mMode = 11;
                a_this->speedF = 0.0f;
            }
            break;

        case 11:
            i_this->field_0x9c8 = 4;
            
            if (frame >= 10) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 20) {
                i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_RD_SHOOT_WAIT, 4.0f, 2, 1.0f);
                i_this->field_0x990[0] = cM_rndF(10.0f) + 10.0f;
                i_this->mMode = 12;
            }
            break;

        case 12:
            i_this->field_0x9c8 = 4;
            
            if (i_this->field_0x990[0] == 0 && !dMsgObject_isTalkNowCheck() && i_this->field_0x9a4 == 0) {
                anm_init(i_this, BCK_RD_SHOOT, 1.0f, 0, 1.0f);
                i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 10), 0, 1.0f, 1.0f, 0.0f, -1.0f);
                i_this->mMode = 13;
            }

            i_this->field_0x9a2 = 1;
            break;

        case 13:
            if (frame <= 3) {
                i_this->field_0x9a2 = 1;
                i_this->field_0x9c8 = 4;
            }

            if (frame == 2) {
                i_this->field_0x9a3 = 1;
            }

            if (i_this->mpModelMorf->isStop()) {
                i_this->mMode = 0;
            }
    }

    if (bVar1) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 4, 0x1000);
    }

    if (i_this->mPlayerDistance < 500.0f) {
        i_this->mAction = ACTION_BOW_RUN;
        i_this->mMode = 0;
        i_this->field_0x1295 = 0;
    }

    return 0;
}

static void* s_command2_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && ((e_rd_class*)i_actor)->mAction == ACTION_BOW3 && ((e_rd_class*)i_actor)->mMode < 10) {
        ((e_rd_class*)i_actor)->field_0x990[0] = cM_rndF(10.0f) + 10.0f;
        ((e_rd_class*)i_actor)->mMode = 10;
    }

    return NULL;
}

static void* s_command4_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && i_actor != i_data && ((e_rd_class*)i_actor)->mAction == ACTION_BOW3) {
        return i_actor;
    }

    return NULL;
}

static void e_rd_commander(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    s8 bVar1 = 0;
    s8 bVar2 = 0;
    boss = i_this;

    if (i_this->field_0x990[2] == 0 && fpcM_Search(s_command4_sub, i_this) == NULL) {
        i_this->mAction = ACTION_BOW2;
        i_this->mMode = 0;
    } else {
        switch (i_this->mMode) {
            case 0:
                i_this->mMode = 1;
                break;

            case 1:
                bVar1 = 1;
                if (i_this->field_0x990[0] == 0) {
                    anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
                    i_this->mMode = 2;
                    i_this->field_0x990[0] = cM_rndF(40.0f) + 60.0f;
                }
                break;

            case 2:
                bVar1 = 1;
                if (i_this->field_0x990[0] == 0) {
                    anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
                    i_this->field_0x990[0] = cM_rndF(100.0f) + 100.0f;
                    i_this->mMode = 1;
                }
                break;

            case 5:
                anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
                i_this->field_0x990[0] = 30;
                i_this->mMode = 6;
                break;

            case 6:
                bVar2 = 1;
                if (i_this->field_0x990[0] == 0) {
                    anm_init(i_this, BCK_RD_GOSIGN, 3.0f, 0, 1.0f);
                    i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_COMMANDER, -1);
                    i_this->mMode = 7;

                    if (dComIfGp_event_runCheck()) {
                        i_this->field_0x990[0] = l_HIO.field_0x24 + 2;
                    } else {
                        i_this->field_0x990[0] = 10;
                    }
                }
                break;

            case 7:
                bVar2 = 1;
                if (i_this->field_0x990[0] == 1) {
                    fpcM_Search(s_command2_sub, i_this);
                }

                if (i_this->field_0x990[0] == 0 && i_this->mpModelMorf->isStop()) {
                    anm_init(i_this, BCK_RD_WAIT01, 5.0f, 2, 1.0f);
                    if (!dComIfGp_event_runCheck() && !pl_check(i_this, i_this->field_0x980, 0x4000)) {
                        i_this->mMode = 10;
                        i_this->field_0x990[1] = TREG_S(8) + 350;
                    } else {
                        if (dComIfGp_event_runCheck()) {
                            i_this->field_0x990[0] = 80;
                        } else {
                            i_this->field_0x990[0] = TREG_S(3);
                        }

                        i_this->mMode = 6;
                    }
                }
                break;

            case 10:
                anm_init(i_this, BCK_RD_GOSIGN, 3.0f, 0, 1.0f);
                mDoAud_seStart(Z2SE_EN_RD_V_CALLING, 0, 0, 0);
                fpcM_Search(s_command2_sub, i_this);
                i_this->mMode = 11;
                break;

            case 11:
                if (i_this->mpModelMorf->isStop()) {
                    if (i_this->field_0x990[1] == 0) {
                        anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
                        i_this->field_0x990[0] = cM_rndF(30.0f) + 30.0f;
                        i_this->mMode = 1;
                    } else {
                        i_this->mMode = 10;
                    }
                }
        }

        if (bVar1 && pl_check(i_this, i_this->field_0x980, 0x4000)) {
            i_this->mMode = 5;
            if (i_this->field_0x12a0 != 0) {
                dComIfGs_onSwitch(i_this->field_0x12a0, fopAcM_GetRoomNo(a_this));
            }
        }

        if (bVar2) {
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 2, 0x800);
        }
    }
}

static void e_rd_excite(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp40;
    f32 fVar1 = 0.0f;

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, BCK_RD_WAIT01, 4.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->field_0x990[0] = cM_rndF(20.0f) + 20.0f;
            i_this->field_0x5cc = a_this->current.angle.y;
            i_this->field_0x5c0 = S_find_pos;
            break;

        case 1:
            i_this->field_0x9c8 = 11;
            i_this->field_0x9d4 = S_find_pos;

            if (i_this->field_0x990[0] == 0) {
                if (i_this->field_0x1295 != 0) {
                    i_this->mMode = 3;
                    anm_init(i_this, BCK_RD_KYORO2, 5.0f, 2, 1.3f);
                    i_this->field_0x990[0] = cM_rndF(20.0f) + 40.0f;
                } else {
                    i_this->mMode = 2;
                    anm_init(i_this, BCK_RD_RUN, 2.0f, 2, cM_rndFX(0.05f) + 1.0f);
                    i_this->field_0x990[0] = cM_rndF(15.0f) + 30.0f;
                }

                sp40 = i_this->field_0x5c0 - a_this->current.pos;
                i_this->field_0x5cc = cM_atan2s(sp40.x, sp40.z);
            }
            break;

        case 2:
            i_this->field_0x9c8 = 11;
            i_this->field_0x9d4 = i_this->field_0x5c0;
            fVar1 = l_HIO.dash_speed;
            sp40 = i_this->field_0x5c0 - a_this->current.pos;
            if (i_this->field_0x990[0] == 0 || move_gake_check(i_this, 100.0f) || sp40.abs() < 100.0f) {
                i_this->mMode = 3;
                anm_init(i_this, BCK_RD_KYORO2, 5.0f, 2, 1.3f);
                i_this->field_0x990[0] = cM_rndF(20.0f) + 40.0f;
            }
            break;

        case 3:
            if (i_this->field_0x990[0] == 0) {
                i_this->mAction = i_this->field_0x974;
                if (i_this->mAction == ACTION_STAND) {
                    i_this->mAction = ACTION_NORMAL;
                }

                i_this->mMode = 0;
            }
            break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5cc, 2, 0x800);
    s16 sVar1 = a_this->current.angle.y - i_this->field_0x5cc;
    if (sVar1 > 0x400 || sVar1 < -0x400) {
        fVar1 = 0.0f;
    }

    cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, 5.0f);
}

static void e_rd_water(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp28;

    i_this->field_0x998 = 10;

    switch (i_this->mMode) {
        case 0:
        case 1:
            a_this->speed.y = 0.0f;
            if (i_this->field_0xa1f == 0) {
                anm_init(i_this, BCK_RD_DROWNEDA, 3.0f, 2, 1.0f);
            } else {
                anm_init(i_this, BCK_RD_DROWNEDB, 3.0f, 2, 1.0f);
            }

            i_this->field_0x990[0] = 60;
            a_this->speedF = 0.0f;
            i_this->field_0x9ec = 0.0f;
            i_this->field_0x9f0 = 0.0f;
            
            sp28 = a_this->current.pos;
            sp28.y = i_this->field_0x984;
            if (i_this->mMode == 0) {
                fopKyM_createWpillar(&sp28, 1.5f, 0);
            } else {
                OS_REPORT("      RD YOGAN  \n");
                fopKyM_createMpillar(&sp28, 1.0f);
            }

            i_this->mMode = 10;
            break;

        case 10:
            a_this->speed.y = 0.0f;
            cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x984, 0.5f, 5.0f);
            cLib_addCalcAngleS2(&i_this->field_0xa0c, -0x4000, 4, 0x400);

            if (i_this->field_0x990[0] == 0) {
                rd_disappear(i_this);
            }
            break;
    }
}

static void e_rd_kiba_start(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    fopAcM_OffStatus(a_this, 0);
    a_this->attention_info.flags = 0;
    i_this->field_0x998 = 10;

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, e_rdb_class::BCK_RB_RWAIT02, 1.0f, 2, 1.0f);
            i_this->mMode++;
            i_this->field_0x6a0 = 0;
            break;

        case 2:
            anm_init(i_this, e_rdb_class::BCK_RB_RCOMEON, 10.0f, 0, 1.0f);
            i_this->mMode++;
            break;

        case 3:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, e_rdb_class::BCK_RB_RWAIT02, 5.0f, 2, 1.0f);
                i_this->mMode++;
            }
            break;

        case 5:
            i_this->field_0x6a0 = 1;
            anm_init(i_this, e_rdb_class::BCK_RB_RPLAY_STOP2, 20.0f, 0, 1.0f);
            horn_anm_init(i_this, 6, 2.0f, 0, 1.0f);
            i_this->mMode++;
            break;

        case 6:
            if ((int)i_this->mpModelMorf->getFrame() == 21) {
                i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_HORN, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_WB_RUN;
                i_this->mMode = 0;
                a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            }
    }
}

static void e_rd_ikki2_start(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    fopAcM_OffStatus(a_this, 0);
    a_this->attention_info.flags = 0;
    i_this->field_0x998 = 10;

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, e_rdb_class::BCK_RB_ROP_DEMO2_WAIT, 1.0f, 2, 1.0f);
            i_this->mMode++;
            break;
            
        case 2:
            anm_init(i_this, e_rdb_class::BCK_RB_ROP_DEMO2_EXCITE, 5.0f, 0, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_DEMO_EXCITE, -1);
            i_this->mMode++;
            break;

        case 3:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, e_rdb_class::BCK_RB_ROP_DEMO2_WAIT, 10.0f, 2, 1.0f);
                i_this->mMode = 4;
            }
            break;

        case 4:
            break;
    }
}

static void e_rd_kiba_end(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    fopAcM_OffStatus(a_this, 0);
    a_this->attention_info.flags = 0;
    i_this->field_0x998 = 10;
    e_wb_class* bullbo = (e_wb_class*)fopAcM_SearchByID(i_this->mWbActorID);

    switch (i_this->mMode) {
        case 0:
            i_this->field_0x680 = 0;
            anm_init(i_this, BCK_RD_RSHOOT_WAIT, 1.0f, 0, 1.0f);
            i_this->mMode++;
            i_this->field_0x990[0] = 14;
            bullbo->field_0x7a2 = 1;
            break;

        case 1:
            if (i_this->field_0x990[0] == 1) {
                cXyz sp28, sp34;
                cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
                sp28.x = BREG_F(8) + 40.0f;
                sp28.y = BREG_F(9) + 20.0f;
                sp28.z = BREG_F(10) + 57.0f;
                MtxPosition(&sp28, &i_this->field_0x6b0);
                i_this->field_0x6be = a_this->shape_angle.y;
                sp28.x = BREG_F(11) + 80.0f;
                sp28.y = BREG_F(12);
                sp28.z = BREG_F(13);
                MtxPosition(&sp28, &sp34);
                i_this->field_0x6a4 += sp34;
                i_this->field_0x6a0 = 2;
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_RD_RSHOOT_WAIT02, 2.0f, 2, 1.0f);
                i_this->mMode++;
                i_this->field_0x990[0] = 110;
            }
            break;

        case 2:
            if (i_this->field_0x990[0] == 0) {
                anm_init(i_this, BCK_RD_RSTEP_BACK, 2.0f, 0, 1.0f);
                i_this->mMode++;
                i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_AWAKE, -1);
                i_this->field_0x990[0] = 10;
            }
            break;

        case 3:
            if (i_this->field_0x990[0] == 0) {
                bullbo->field_0x7a2 = 0;
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_RD_SHOUT, 5.0f, 2, 1.0f);
                i_this->mMode++;
                i_this->field_0x990[0] = 20;
            }
            break;

        case 4:
            if (i_this->field_0x990[0] != 0) {
                i_this->field_0x9c8 = 1;
            }

            if ((bullbo->mStatusFlags & 0x100) != 0) {
                i_this->mAction = ACTION_WB_BJUMP;
                i_this->mMode = 0;
                i_this->field_0x990[1] = 2000;
                i_this->field_0x990[2] = 1000;
            }
            break;
    }
}

static void e_rd_ikki_end(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    e_wb_class* bullbo = (e_wb_class*)fopAcM_SearchByID(i_this->mWbActorID);

    fopAcM_OffStatus(a_this, 0);
    a_this->attention_info.flags = 0;
    i_this->field_0x998 = 10;
    bullbo->field_0x1434 = fopAcM_GetID(a_this);

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, BCK_RD_RRUN02_BACK, 1.0f, 0, 1.0f);
            i_this->mMode++;
            i_this->field_0x9bc = 0;

            if (daPy_getPlayerActorClass()->checkHorseRide() && bullbo != NULL) {
                bullbo->mStatusFlags &= ~i_this->field_0x9be;
                i_this->field_0x9be = 0;
                bullbo->mActionID = 16;
                bullbo->mActionMode = 0;
                bullbo->field_0x169e = 10;
            }

            a_this->speedF = 0.0f;
            a_this->speed.y = 40.0f;
            i_this->field_0x9ec = 70.0f;

            if (fopAcM_searchPlayerAngleY(a_this) < 0) {
                i_this->field_0xa0e = 0x4000;
            } else {
                i_this->field_0xa0e = -0x4000;
            }

            i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_DEATH, -1);
            break;

        case 1:
            if (i_this->mAnmID == BCK_RD_RRUN02_BACK) {
                if (i_this->mpModelMorf->isStop()) {
                    anm_init(i_this, BCK_RD_JUMP_A, 1.0f, 2, 1.0f);
                }
            }

            if (a_this->speed.y < 0.0f) {
                cLib_addCalc2(&i_this->field_0x9ec, 0.0f, 0.1f, 1.0f);
                a_this->current.angle.y += i_this->field_0x9f6;
            }
            break;
    }
}

static void e_rd_ikki2_end(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    e_wb_class* bullbo = (e_wb_class*)fopAcM_SearchByID(i_this->mWbActorID);

    fopAcM_OffStatus(a_this, 0);
    a_this->attention_info.flags = 0;
    i_this->field_0x998 = 10;
    bullbo->field_0x1434 = fopAcM_GetID(a_this);

    switch (i_this->mMode) {
        case 0:
            if (daPy_getPlayerActorClass()->checkHorseRide()) {
                anm_init(i_this, BCK_RD_RRUN_BACK, 0.0f, 0, 1.0f);
                i_this->mMode = 1;
                bullbo->mActionID = 18;
                bullbo->mActionMode = 0;
                bullbo->field_0x169e = 90;
                mDoAud_bgmStop(30);
            }
            break;

        case 1:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_RD_RSHOOT_READY, 3.0f, 2, 1.0f);
            }
            break;

        case 10:
            anm_init(i_this, BCK_RD_RSHOOT, 2.0f, 0, 1.0f);
            i_this->mMode = 11;
            i_this->field_0x9bc = 0;

            if (bullbo != NULL) {
                bullbo->mStatusFlags &= ~i_this->field_0x9be;
                i_this->field_0x9be = 0;
            }

            a_this->speedF = 0.0f;
            a_this->speed.y = 50.0f;
            i_this->field_0x9ec = 60.0f;
            i_this->field_0xa0e = 0x4000;

            i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_DEATH, -1);
            break;

        case 11:
            if (a_this->speed.y < 0.0f) {
                cLib_addCalc2(&i_this->field_0x9ec, 0.0f, 0.1f, 1.0f);
            }
            break;
    }
}

static void* s_rdb_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RDB) {
        return i_actor;
    }

    return NULL;
}

static void e_rd_lv9_end(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    e_wb_class* bullbo = (e_wb_class*)fopAcM_SearchByID(i_this->mWbActorID);

    i_this->field_0x998 = 10;

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, e_rdb_class::BCK_RB_RWALK, 0.0f, 2, 1.0f);
            i_this->mMode = 1;
            break;

        case 1:
            if ((bullbo->mStatusFlags & 8) != 0) {
                anm_init(i_this, e_rdb_class::BCK_RB_RWAIT, 10.0f, 2, 1.0f);
                i_this->mMode = 2;
            }
            break;

        case 2:
            if ((bullbo->mStatusFlags & 8) != 0) {
                anm_init(i_this, e_rdb_class::BCK_RB_LV9_END03, 5.0f, 0, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_LV9_END03, -1);
                i_this->mMode = 3;
            }
            break;

        case 3:
            if (i_this->mAnmID == e_rdb_class::BCK_RB_LV9_END03) {
                if (i_this->mpModelMorf->isStop()) {
                    anm_init(i_this, e_rdb_class::BCK_RB_RWAIT, 10.0f, 2, 1.0f);
                }
            } else {
                e_rdb_class* king_bulblin = (e_rdb_class*)fpcM_Search(s_rdb_sub, i_this);
                if (king_bulblin != NULL && king_bulblin->mDemoMode == 14) {
                    anm_init(i_this, e_rdb_class::BCK_RB_RNEIGH, 3.0f, 0, 1.0f);
                    bullbo->mActionMode++;
                    i_this->mMode = 4;
                }
            }
            break;

        case 4:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, e_rdb_class::BCK_RB_RRUN, 3.0f, 2, 1.0f);
                i_this->mMode = 5;
            }
            break;

        case 5:
            break;
    }
}

static void* s_boom_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && ((e_rd_class*)i_actor)->field_0x99a != 0) {
        return i_actor;
    }

    return NULL;
}

static void wolfkick_damage(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    i_this->mAction = ACTION_DAMAGE;
    i_this->mMode = 0;
    i_this->field_0xa0e = player->shape_angle.y + 0x8000;
    a_this->speed.y = l_HIO.wolf_falling_power_y;
    i_this->field_0x9ec = -l_HIO.wolf_falling_power_z;
    i_this->mSound.startCollisionSE(Z2SE_HIT_WOOD_WEAPON, 31);
    i_this->field_0x9f8 = 1;
    i_this->field_0x998 = 1000;
}

static void big_damage(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    i_this->mAction = ACTION_DAMAGE;
    i_this->mMode = 0;
    a_this->speed.y = l_HIO.jump_y + 2.0f;

    if (i_this->field_0x9bc == 2) {
        fopAc_ac_c* bullbo = fopAcM_SearchByID(i_this->mWbActorID);
        if (bullbo != NULL && bullbo->speedF >= 20.0f) {
            i_this->field_0x9ec = bullbo->speedF;
            if (i_this->field_0x9ec > 40.0f) {
                i_this->field_0x9ec = 40.0f;
            }
        } else {
            i_this->field_0x9ec = -l_HIO.field_0x48;
        }

        i_this->field_0xa0e = a_this->shape_angle.y + (s16)cM_rndFX(3000.0f);
    } else {
        i_this->field_0xa0e = i_this->mAtInfo.mHitDirection.y;
        i_this->field_0x9ec = -l_HIO.jump_z;
    }

    if ((i_this->mAtInfo.mHitBit & 0x80) != 0) {
        OS_REPORT("SPIN CUT HIT !!\n");
        i_this->field_0x9f6 = cM_rndFX(2000.0f) + 4000.0f;
    } else if ((i_this->mAtInfo.mHitBit & 0x800) != 0) {
        OS_REPORT("SPIN CUT L HIT !!\n");
        i_this->field_0x9f6 = -(cM_rndFX(2000.0f) + 4000.0f);
    } else {
        if (i_this->field_0x9bc != 0) {
            i_this->field_0x9f6 = cM_rndFX(3000.0f);
        } else {
            if (fopAcM_GetName(i_this->mAtInfo.mpActor) == PROC_E_WB) {
                i_this->field_0x9f6 = cM_rndFX(8000.0f);
                a_this->speed.y = cM_rndF(15.0f) + 35.0f;
                i_this->field_0x9ec = -(i_this->mAtInfo.mpActor->speedF * 0.5f);
            } else {
                i_this->field_0x9f6 = cM_rndFX(1000.0f);
            }
        }
    }

    i_this->field_0x9f8 = 0;
    i_this->field_0x998 = 1000;

}

static void small_damage(e_rd_class* i_this, int param_2) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    if (i_this->mAction != ACTION_S_DAMAGE) {
        i_this->field_0x974 = i_this->mAction;
    }

    i_this->mAction = ACTION_S_DAMAGE;
    i_this->mMode = 0;
    i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DAMAGE, -1);

    if (param_2 == 0) {
        s16 sVar1 = a_this->shape_angle.y - i_this->mPlayerAngleY;
        if (sVar1 < -0x4000 || sVar1 > 0x4000) {
                anm_init(i_this, BCK_RD_DAMAGE_W, 2.0f, 0, 1.0f);
            } else if (sVar1 < 0) {
                anm_init(i_this, BCK_RD_DAMAGE_L, 2.0f, 0, 1.0f);
            } else {
                anm_init(i_this, BCK_RD_DAMAGE_R, 2.0f, 0, 1.0f);
            }
    } else {
        anm_init(i_this, BCK_RD_DAMAGE_W, 2.0f, 0, 1.0f);
    }

    i_this->field_0x9f0 = 20.0f;
    i_this->field_0x9f4 = i_this->mAtInfo.mHitDirection.y;
}

static void part_break(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    for (int i = 0; i < 14; i++) {
        if (i_this->field_0x70c[i] == 0) {
            i_this->field_0x70c[i] = 1;
            i_this->mSound.startCreatureSound(Z2SE_EN_RDB_BREAK_ARMOR, 0, -1);

            cXyz sp54 = i_this->field_0x71c[i] - a_this->current.pos;
            cMtx_YrotS(*calc_mtx, cM_atan2s(sp54.x, sp54.z));
            sp54.x = cM_rndFX(10.0f);
            sp54.y = cM_rndF(5.0f) + 20.0f;
            sp54.z = cM_rndF(10.0f) + 30.0f;
            MtxPosition(&sp54, &i_this->field_0x7c4[i]);

            i_this->field_0x86c[i].y = cM_rndF(65536.0f);
            i_this->field_0x86c[i].x = cM_rndF(65536.0f);

            u16 uVar1 = 1;
            if (i_this->mAtInfo.mHitStatus != 0) {
                uVar1 = 3;
            } else {
                uVar1 = 1;
            }

            dComIfGp_setHitMark(uVar1, a_this, &i_this->field_0x71c[i], &a_this->shape_angle, NULL, 0);
            return;
        }
    }
}

static void damage_check(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* bullbo = fopAcM_SearchByID(i_this->mWbActorID);
    e_wb_class* bullbo_p = (e_wb_class*)bullbo;
    int iVar1;

    if (i_this->field_0x9bc == 2 && bullbo != NULL && (bullbo_p->mStatusFlags & 0xC0) != 0) {
        if (i_this->mAction == ACTION_IKKI2_END) {
            i_this->mMode = 10;
            return;
        }

        i_this->mAction = ACTION_DAMAGE;
        i_this->mMode = 0;
        i_this->field_0x998 = 1000;

        if ((bullbo_p->mStatusFlags & 0x80) != 0) {
            i_this->field_0xa0e = a_this->shape_angle.y + 0x8000 + (s16)cM_rndFX(3000.0f);
            i_this->field_0x9f6 = (s16)cM_rndFX(1000.0f);
        } else {
            i_this->field_0xa0e = a_this->shape_angle.y + (s16)cM_rndFX(8000.0f);
            i_this->field_0x9f6 = (s16)cM_rndFX(3000.0f);
        }

        a_this->speed.y = cM_rndF(15.0f) + 25.0f;
        i_this->field_0x9ec = -20.0f;
        return;
    }

    i_this->mStts.Move();

    if (i_this->field_0x998 == 0) {
        if (i_this->mBossMode != 0) {
            a_this->health = 100;
        }

        for (int i = 0; i <= 2; i++) {
            if (i_this->field_0xd58[i].ChkTgHit() != 0) {
                i_this->field_0x998 = 6;
                i_this->mAtInfo.mpCollider = i_this->field_0xd58[i].GetTgHitObj();
                if (i_this->mBossMode == 3) {
                    s16 sVar1 = a_this->shape_angle.y - i_this->mPlayerAngleY;
                    at_power_check(&i_this->mAtInfo);
                    if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_ARROW) || i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB)) {
                        u16 uVar1;
                        if (dComIfGp_getHorseActor()->speedF > 20.0f || sVar1 > 0x7800 || sVar1 < -0x7800) {
                            cc_at_check(a_this, &i_this->mAtInfo);
                            dScnPly_c::setPauseTimer(0);
                            bullbo_p->mLapCount++;
                            if (bullbo_p->mLapCount >= 3) {
                                mDoAud_bgmStop(30);
                                i_this->mAction = ACTION_IKKI2_END;
                                i_this->mMode = 0;
                                i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_DAMAGE_L, -1);
                            } else {
                                bullbo_p->field_0x169e = 25;
                                anm_init(i_this, e_rdb_class::BCK_RB_RDAMAGEB, 2.0f, 0, 1.0f);
                                i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_DAMAGE, -1);
                            }

                            uVar1 = 3;
                            bullbo_p->field_0x698[3] = 100;
                            dComIfGs_onSaveDunSwitch(7);
                        } else {
                            uVar1 = 2;
                            anm_init(i_this, e_rdb_class::BCK_RB_RGUARD_F, 2.0f, 0, 1.0f);
                        }

                        cXyz sp34, sp40;
                        cMtx_YrotS(*calc_mtx, bullbo->shape_angle.y);
                        sp34.set(0.0f, nREG_F(9) + 150.0f, nREG_F(10) + 200.0f);
                        MtxPosition(&sp34, &sp40);

                        sp40 += a_this->current.pos;
                        cXyz sp4c(2.0f, 2.0f, 2.0f);
                        dComIfGp_setHitMark(uVar1, a_this, &sp40, NULL, &sp4c, 0);
                        return;
                    } else {
                        if (i_this->mAtInfo.mHitType != 1) {
                            return;
                        }

                        anm_init(i_this, e_rdb_class::BCK_RB_RGUARD_F, 2.0f, 0, 1.0f);
                        bullbo_p->field_0x169e = 25;
                        dScnPly_c::setPauseTimer(0);

                        cXyz sp58, sp64;
                        cMtx_YrotS(*calc_mtx, bullbo->shape_angle.y);

                        if ((s16)(a_this->shape_angle.y - i_this->mPlayerAngleY) < 0) {
                            sp58.set(100.0f, nREG_F(9) + 150.0f, nREG_F(10) + 400.0f);
                        } else {
                            sp58.set(-100.0f, nREG_F(9) + 150.0f, nREG_F(10) + 400.0f);
                        }
                        MtxPosition(&sp58, &sp64);

                        sp64 += a_this->current.pos;
                        cXyz sp70(2.0f, 2.0f, 2.0f);
                        dComIfGp_setHitMark(2, a_this, &sp64, NULL, &sp70, 0);
                        dComIfGp_getVibration().StartShock(8, 31, cXyz(0.0f, 1.0f, 0.0f));
                        i_this->mSound.startCollisionSE(Z2SE_HIT_SWORD, 40);

                        i_this->field_0x998 = 30;
                        return;
                    }
                }

                i_this->field_0x129c = 3;

                if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_10000000)) {
                    wolfkick_damage(i_this);
                    break;
                }

                    at_power_check(&i_this->mAtInfo);
                    if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                        i_this->field_0x990[1] = 30;
                    } else {
                        i_this->field_0x990[1] = 1;
                    }

                    if (i_this->field_0x9bc == 2 && i_this->mAtInfo.mAttackPower <= 10) {
                        if (fpcM_Search(s_boom_sub, i_this) != NULL) {
                            return;
                        }

                        i_this->field_0x99a = 5;
                    }

                    if (i_this->field_0x1295 != 0 && (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT) || 
                        (i_this->mAtInfo.mHitType != 1 && i_this->mAtInfo.mHitType != 16 && i_this->mAtInfo.mHitType != 2))) {
                        i_this->field_0x998 = 1000;
                        i_this->mAction = ACTION_DROP;
                        i_this->mMode = 0;
                        a_this->health = 0;
                        cc_at_check(a_this, &i_this->mAtInfo);
                        return;
                    }

                    if (i_this->mAtInfo.mHitType == 15) {
                        a_this->health = 0;
                        if (i_this->field_0x9bc == 0) {
                            i_this->mAction = ACTION_A_DAMAGE;
                            i_this->mMode = 0;
                            i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DAMAGE, -1);
                            a_this->health = 0;
                            cc_at_check(a_this, &i_this->mAtInfo);
                            return;
                        }
                    }

                    cc_at_check(a_this, &i_this->mAtInfo);
                    if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_HEAD_JUMP) {
                        a_this->health = 0;
                    }

                    if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                        i_this->field_0x998 = 20;
                    } else {
                        i_this->field_0x998 = 10;
                    }

                    if (i_this->mAtInfo.mAttackPower <= 1) {
                        i_this->field_0x998 = 10;
                    }

                    i_this->field_0xa20 |= i_this->mAtInfo.mHitBit;

                    if (daPy_getPlayerActorClass()->checkHorseRide() != 0 && dComIfGp_getHorseActor()->speedF >= 20.0f && i_this->mAtInfo.mHitType == 1) {
                        i_this->mAtInfo.mAttackPower = 20;
                    }

                    if (a_this->health <= 0 || i_this->mAtInfo.mHitStatus != 0 || i_this->field_0x9bc != 0) {
                        if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP && player->checkCutJumpCancelTurn()) {
                            small_damage(i_this, i);
                            i_this->field_0x998 = 3;
                        } else {
                            if (i_this->mBossMode != 0 && bullbo != NULL) {
                                i_this->field_0x998 = 20;
                                if (i_this->mBossMode == 2) {
                                    bullbo_p->mLapCount++;
                                    if (bullbo_p->mLapCount == 1) {
                                        a_this->health = 0;
                                        /* dSv_event_flag_c::M_055 - Main Event - Did damage at least once during joust/one-on-one battle */
                                        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[0x58]);
                                        part_break(i_this);
                                        bullbo_p->field_0x169e = 20;
                                        dScnPly_c::setPauseTimer(0);
                                    }

                                    if (l_HIO.one_hit_kill != 0 || bullbo_p->mLapCount >= 2) {
                                        mDoAud_bgmStop(30);
                                        i_this->mAction = ACTION_IKKI_END;
                                        i_this->mMode = 0;
                                        
                                        if ((i_this->mAtInfo.mHitBit & 0x80) != 0) {
                                            i_this->field_0x9f6 = 0x300;
                                        } else {
                                            i_this->field_0x9f6 = cM_rndFX(100.0f);
                                        }

                                        dScnPly_c::setPauseTimer(10);
                                        return;
                                    } else {
                                        if (i_this->field_0x680 == 0) {
                                            iVar1 = i_this->mAnmID;
                                            anm_init(i_this, e_rdb_class::BCK_RB_RDOWNB, 2.0f, 0, 1.0f);
                                            i_this->field_0x680 = iVar1;
                                        }

                                        i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_DAMAGE_L, -1);
                                        return;
                                    }
                                }

                                bullbo_p->mLapCount++;
                                bullbo_p->mPathAdjustCounter++;
                                part_break(i_this);
                                if (bullbo_p->mLapCount == 1 || bullbo_p->mLapCount == 2 || bullbo_p->mLapCount == 3 ||
                                    bullbo_p->mLapCount == 4 || bullbo_p->mLapCount == 6) {
                                    part_break(i_this);
                                }

                                s8 sVar2 = 8 - bullbo_p->mLapCount;
                                if (sVar2 > 8) {
                                    sVar2 = 8;
                                }
                                dComIfGs_BossLife_public_Set(sVar2);

                                if (bullbo_p->mLapCount == 5) {
                                    Z2GetAudioMgr()->changeSubBgmStatus(2);
                                }

                                if (bullbo_p->mLapCount >= 8) {
                                    i_this->mAction = ACTION_KIBA_END;
                                    i_this->mMode = 0;
                                    mDoAud_seStart(Z2SE_EN_RDB_V_FAINT, 0, 0, 0);
                                    bullbo_p->mActionID = 31;
                                    bullbo_p->mActionMode = 0;
                                    mDoAud_bgmStop(30);
                                    return;
                                }

                                i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_DAMAGE, -1);

                                if (i_this->mAnmID == e_rdb_class::BCK_RB_RPLAY_RUNL || i_this->mAnmID == e_rdb_class::BCK_RB_RPLAY_RUNR 
                                    || i_this->mAnmID == e_rdb_class::BCK_RB_RPLAY_STOP) {
                                    i_this->mAnmID = i_this->field_0x680;
                                    i_this->field_0x680 = 0;
                                }

                                if (i_this->field_0x680 == 0) {
                                    i_this->field_0x990[3] = cM_rndF(100.0f) + 200.0f;
                                    iVar1 = i_this->mAnmID;

                                    if ((s16)(a_this->shape_angle.y - i_this->mPlayerAngleY) < 0) {
                                        anm_init(i_this, e_rdb_class::BCK_RB_RDAMAGEL, 2.0f, 0, 1.0f);
                                    } else {
                                        anm_init(i_this, e_rdb_class::BCK_RB_RDAMAGER, 2.0f, 0, 1.0f);
                                    }

                                    i_this->field_0x680 = iVar1;
                                }

                                if (i_this->mAction != ACTION_WB_RUN) {
                                    i_this->mAction = ACTION_WB_RUN;
                                    i_this->mMode = 0;
                                }
                                break;  
                            }

                            if (i_this->field_0x9bc != 0 && i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                                i_this->field_0x998 = 20;
                                if (i_this->mAction == ACTION_WB_RUN) {
                                    anm_init(i_this, BCK_RD_RRUN02_BACK, 5.0f, 2, 1.0f);
                                    i_this->mMode = 50;
                                    i_this->field_0x990[3] = cM_rndF(20.0f) + 50.0f;
                                }
                            } else {
                                big_damage(i_this);
                            }
                        }
                    } else {
                        int iVar2 = i;
                        if (i_this->mAtInfo.mHitType == 16) {
                            iVar2 = 0;
                        }

                        small_damage(i_this, iVar2);
                    }

                    a_this->speedF = 0.0f;
                    if (i_this->mpMorfBowAnm != NULL) {
                        i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 10), 0, 1.0f, 1.0f, 0.0f, -1.0f);
                    }
                    break;
            }
        }

        for (int i = 0; i < 3; i++) {
            if (a_this->health <= 1) {
                i_this->field_0xd58[i].SetTgHitMark((CcG_Tg_HitMark)3);
            }
        }
    }
}

static void* s_other_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && ((e_rd_class*)i_actor)->field_0x5b7 != 6) {
        return i_actor;
    }

    return NULL;
}

static void* s_ep_sub(void* i_actor, void* i_data) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_Lv1Cdl00) {
        if (!fopAcM_isSwitch((fopAc_ac_c*)i_actor, fopAcM_GetParam(i_actor) & 0xFF)) {
            f32 fVar1, fVar2;
            fVar2 = ((fopAc_ac_c*)i_actor)->current.pos.x - player->current.pos.x;
            fVar1 = ((fopAc_ac_c*)i_actor)->current.pos.z - player->current.pos.z;
            if (JMAFastSqrt(fVar2 * fVar2 + fVar1 * fVar1) < NREG_F(17) + 600.0f) {
                return i_actor;
            }
        }
    }

    return NULL;
}

static s8 e_rd_yagura(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    int frame = i_this->mpModelMorf->getFrame();
    s8 rv = 1;
    s8 bVar1 = 0;

    fpcM_Search(s_b_sub, i_this);

    switch (i_this->mMode) {
        case 0:
            if (i_this->field_0x99c != 0) {
                a_this->home.pos.y = a_this->current.pos.y;
                return 0;
            }

            rv = 0;
            bVar1 = 1;

            if (i_this->field_0x990[0] == 0) {
                anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
                i_this->mMode = 1;
                i_this->field_0x990[0] = cM_rndF(40.0f) + 60.0f;
            }
            break;

        case 1:
            rv = 0;
            bVar1 = 1;

            if (i_this->field_0x990[0] == 0) {
                anm_init(i_this, BCK_RD_WAIT01, 10.0f, 2, 1.0f);
                i_this->field_0x990[0] = cM_rndF(100.0f) + 100.0f;
                i_this->mMode = 0;
            }
            break;

        case 5:
            i_this->field_0x9c8 = 4;

            if (i_this->field_0x990[0] == 0) {
                anm_init(i_this, BCK_RD_SHOUT, 5.0f, 0, 1.0f);
                i_this->mMode = 10;
                i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_FIND_LINK_YELL, -1);
                dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(a_this), 3);
            }
            break;

        case 10:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_RD_SHOOT_READY, 5.0f, 0, 1.0f);
                i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 7), 0, 5.0f, 0.9f, 0.0f, -1.0f);
                i_this->mMode = 11;
                a_this->speedF = 0.0f;
            }
            break;

        case 11:
            i_this->field_0x9c8 = 4;

            if (frame >= 10) {
                i_this->field_0x9a2 = 1;
            }

            if (frame == 20) {
                i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_DRAW_NORMAL, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_RD_SHOOT_WAIT, 4.0f, 2, 1.0f);
                i_this->field_0x990[0] = cM_rndF(10.0f) + 10.0f;
                i_this->mMode = 12;
            }
            break;

        case 12:
            i_this->field_0x9c8 = 4;

            if (i_this->field_0x990[0] == 0 && i_this->field_0x9a4 == 0) {
                anm_init(i_this, BCK_RD_SHOOT, 1.0f, 0, 1.0f);
                i_this->mpMorfBowAnm->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 10), 0, 1.0f, 1.0f, 0.0f, -1.0f);
                i_this->mMode = 13;
            }

            i_this->field_0x9a2 = 1;
            break;

        case 13:
            if (frame <= 3) {
                i_this->field_0x9a2 = 1;
                i_this->field_0x9c8 = 4;
            }

            if (frame == 2) {
                i_this->field_0x9a3 = 1;
            }

            if (i_this->mpModelMorf->isStop()) {
                if (pl_check(i_this, 10000.0f, 0x7FFF)) {
                    i_this->mMode = 10;
                    anm_init(i_this, BCK_RD_SHOUT, 5.0f, 0, 1.0f);
                    dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(a_this), 3);
                } else {
                    i_this->mMode = 0;
                }
            }
            break;

        case 14:
            break;
    }

    if (fabsf(i_this->mObjAcch.GetGroundH() - a_this->home.pos.y) > 10.0f) {
        i_this->mAction = ACTION_DAMAGE;
        i_this->mMode = 0;
        i_this->field_0xa0e = a_this->shape_angle.y;
        a_this->speed.y = 20.0f;
        i_this->field_0x9ec = -20.0f;
        i_this->field_0x998 = 1000;
        i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DEATH, -1);
        a_this->health = 0;
        return 0;
    }

    if (bVar1 && pl_check(i_this, i_this->field_0x980, 0x4000) && (i_this->field_0x980 >= 8000.0f || fpcM_Search(s_ep_sub, i_this) != NULL)) {
        anm_init(i_this, BCK_RD_WAIT01, 4.0f, 2, 1.0f);
        i_this->field_0x990[0] = 30;
        i_this->mMode = 5;
        if (S_find == 0 && (u8)lbl_70_bss_AE == 0 && !daPy_getPlayerActorClass()->checkBoarRide() && !dComIfGp_event_runCheck() &&
            i_this->mDemoMode == 0 && fpcM_Search(s_other_sub, i_this) != NULL) {
            i_this->mDemoMode = 1;
        }
    }

    if (rv != 0) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 4, 0x1000);
    }

    return rv;
}

static void e_rd_jyunkai(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    f32 fVar1 = 0.0f;
    s16 sVar1 = 0x4000;
    cXyz sp58, sp64;
    dPnt* points;
    s8 sVar3;

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, BCK_RD_WALK, 10.0f, 2, 1.0f);
            i_this->mMode = 1;
            points = i_this->mPath->m_points;
            sVar3 = i_this->field_0xb00;
            i_this->field_0x5c0.x = points[sVar3].m_position.x;
            i_this->field_0x5c0.y = points[sVar3].m_position.y;
            i_this->field_0x5c0.z = points[sVar3].m_position.z;
            // fallthrough
        case 1:
            fVar1 = l_HIO.movement_speed;
            sp58 = i_this->field_0x5c0 - a_this->current.pos;
            i_this->field_0x5cc = cM_atan2s(sp58.x, sp58.z);

            if (sp58.abs() < 50.0f) {
                i_this->field_0xb00 += i_this->field_0xb01;
                if (i_this->field_0xb00 >= (i_this->mPath->m_num & 0xFF)) {
                    if ((i_this->mPath->m_closed & 1) != 0) {
                        i_this->field_0xb00 = 0;
                    } else {
                        i_this->field_0xb01 = 0xFF;
                        i_this->field_0xb00 = i_this->mPath->m_num - 2;
                    }
                } else if (i_this->field_0xb00 < 0) {
                    i_this->field_0xb01 = 1;
                    i_this->field_0xb00 = 1;
                }

                i_this->mMode = 2;
                i_this->field_0x990[0] = cM_rndF(100.0f) + 100.0f;
                anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
            }
            break;
            
        case 2:
            sVar1 = 0x6000;

            if (i_this->field_0x990[0] == 0) {
                i_this->mMode = 0;
            }
            break;

        case 10:
            anm_init(i_this, BCK_RD_SHOUT, 5.0f, 0, 1.0f);
            i_this->mMode = 11;
            dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(a_this), 3);
            S_find = 20;
            break;

        case 11:
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 2, 0x800);
            
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mMode = -10;
                i_this->field_0x990[0] = 60;
                return;
            }
    }

    if (fVar1) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5cc, 8, 0x400);
        s16 sVar2 = a_this->current.angle.y - i_this->field_0x5cc;
        if (sVar2 > 0x400 || sVar2 < -0x400) {
            fVar1 = 0.0f;
        }
    }

    cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, 3.0f);

    f32 fVar2 = i_this->field_0x980;
    BOOL bVar1 = FALSE;

    if (i_this->field_0x9a1 == 1|| S_find != 0) {
        if (i_this->field_0x9a1 == 1) {
            bVar1 = TRUE;
        }

        fVar2 = 10000.0f;
        sVar1 = 0x7FFF;
    }

    if (i_this->mMode < 10) {
        if (bVar1) {
            i_this->mMode = 2;
            i_this->field_0x990[0] = cM_rndF(100.0f) + 100.0f;
            anm_init(i_this, BCK_RD_KYORO2, 10.0f, 2, 1.0f);
        } else if ((i_this->field_0x970 & 7) == 0 && pl_check(i_this, fVar2, sVar1)) {
            if (S_find == 0) {
                i_this->mMode = 10;
            } else {
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mMode = -10;
                i_this->field_0x990[0] = 60;
            }
        }
    }
}

static void e_rd_sleep(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    int frame = i_this->mpModelMorf->getFrame();
    f32 fVar1 = i_this->field_0x980;
    s16 sVar1 = 0x4000;
    BOOL bVar1 = FALSE;

    if (i_this->field_0x9a1 == 1 || S_find != 0) {
        if (i_this->field_0x9a1 == 1) {
            bVar1 = TRUE;
        } else if ((i_this->field_0x970 & 15) == 0) {
            bVar1 = TRUE;
        }

        fVar1 = 5000.0f;
        sVar1 = 0x7FFF;
    }

    s8 cVar1 = 0;

    switch (i_this->mMode) {
        case 0:
            i_this->field_0x990[0] = cM_rndF(100.0f) + 200.0f;
            anm_init(i_this, BCK_RD_SLEEP_WAIT, 10.0f, 2, 1.0f);
            i_this->mMode = 1;
            break;

        case 1:
            if (bVar1 || i_this->field_0x990[0] == 0) {
                anm_init(i_this, BCK_RD_SLEEP_LOOKAROUND, 5.0f, 0, 1.0f);
                i_this->mMode = 2;
            }
            break;

        case 2:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mMode = 0;
            }

            if (fopAcM_GetRoomNo(a_this) == 0) {
                if (bVar1) {
                    wb_check(i_this, sVar1);
                }

                if (i_this->mPlayerDistance > 600.0f) {
                    return;
                }
            }

            if (frame > 60 && frame < 200 && (((i_this->field_0x970 & 7) == 0) || bVar1) && pl_check(i_this, fVar1, sVar1)) {
                cVar1 = 1;
            }
            break;

        case 5:
            if (i_this->field_0x990[0] == 0) {
                cVar1 = 1;
            }
            break;

        case 10:
            anm_init(i_this, BCK_RD_SHOUT, 5.0f, 0, 1.0f);
            i_this->mMode = 11;
            dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(a_this), 3);
            S_find = 20;
            break;

        case 11:
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 2, 0x800);

            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mMode = -10;
                i_this->field_0x990[0] = 60;
                return;
            }
    }

    if (i_this->mMode < 5) {
        for (int i = 0; i <= 2; i++) {
            if (i_this->field_0xd58[i].ChkCoHit()) {
                if (daPy_getPlayerActorClass() == dCc_GetAc(i_this->field_0xd58[i].GetCoHitObj()->GetAc())) {
                    cVar1 = 2;
                    break;
                }
            }
        }
    }

    if (i_this->mMode < 10) {
        if (cVar1 == 1) {
            if (S_find == 0) {
                i_this->mMode = 10;
            } else {
                i_this->mAction = ACTION_FIGHT_RUN;
                i_this->mMode = -10;
                i_this->field_0x990[0] = 60;
                OS_REPORT("......RD FIND 1!!!\n");
            }
        } else if (cVar1 == 2) {
            i_this->mMode = 5;
            anm_init(i_this, BCK_RD_KYORO2, 7.0f, 2, 1.5f);
            i_this->field_0x990[0] = cM_rndF(10.0f) + 20.0f;
            OS_REPORT("......RD FIND 2!!!\n");
        }
    }

    if (i_this->field_0x980 >= 9900.0f) {
        i_this->mAction = ACTION_STAND;
        i_this->mMode = 0;
        i_this->field_0x990[0] = 0;
    }
}

static int rd_count;

static void* s_tag_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && ((e_rd_class*)i_actor)->field_0x5ba != 0) {
        rd_count++;
    }

    return NULL;
}

static void e_rd_tag(e_rd_class* i_this) {
    // FAKEMATCH
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    i_this->field_0x9ad = 0;
    i_this->field_0x998 = 10;

    if (i_this->mMode) {
        // Fakematch to prevent the compiler from optimizing out the unused comparison
        a_this->speedF = a_this->speedF;
    }

    if (S_find != 0 && (i_this->field_0x970 & 31) == 0 && !dComIfGp_event_runCheck() && i_this->mPlayerDistance < 3000.0f && i_this->mPlayerDistance > 500.0f && 
        i_this->field_0x5bb != 0 && !fopAcM_otherBgCheck(a_this, player)) {
        rd_count = 0;
        fpcM_Search(s_tag_sub, i_this);

        if (rd_count < 6) {
            csXyz cStack_40(0, i_this->mPlayerAngleY, 0);
            cXyz sp38;
            sp38.x = cM_rndFX(200.0f) + a_this->home.pos.x;
            sp38.y = a_this->home.pos.y;
            sp38.z = cM_rndFX(200.0f) + a_this->home.pos.z;

            fpc_ProcID i_parameters;
            if ((rd_count & 3) == 3) {
                i_parameters = 0xFF011323;
            } else {
                i_parameters = 0xFF011123;
            }
            fopAcM_createChild(PROC_E_RD, fopAcM_GetID(a_this), i_parameters, &sp38, fopAcM_GetRoomNo(a_this), &cStack_40, NULL, -1, NULL);
        }
    }
}

static void e_rd_reg(e_rd_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    i_this->field_0x9ad = 0;
    i_this->field_0x998 = 10;

    if (i_this->field_0xafb == 0) {
        i_this->mAction = ACTION_BOW2;
        i_this->mMode = 1;
        i_this->field_0x980 = 0.0f;
        return;
    }

    if (i_this->mMode != 0) {
        return;
    }

    if (i_this->field_0x5b7 != 14) {
        return;
    }

    if (i_this->mDemoMode != 0) {
        return;
    }

    f32 fVar1, fVar2;
    fVar1 = player->current.pos.x - -5551.0f;
    fVar2 = player->current.pos.z - 5525.0f;
    if (JMAFastSqrt(fVar1 * fVar1 + fVar2 * fVar2) < 600.0f) {
        i_this->mDemoMode = 20;
    }
}

static void action(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* actor = get_pla(a_this);
    cXyz sp25c, sp268;

    i_this->field_0x9c8 = 0;

    if (actor == dComIfGp_getPlayer(0)) {
        i_this->mPlayerDistance = fopAcM_searchPlayerDistance(a_this);
        if (daPy_getPlayerActorClass()->checkHorseRide()) {
            i_this->mPlayerDistance -= BREG_F(17) + 100.0f;
        }

        i_this->mPlayerAngleY = fopAcM_searchPlayerAngleY(a_this);
    } else {
        sp25c = actor->current.pos - a_this->current.pos;
        i_this->mPlayerDistance = sp25c.abs();
        i_this->mPlayerAngleY = cM_atan2s(sp25c.x, sp25c.z);
        i_this->field_0x980 = NREG_F(7) + 10000.0f;
    }

    if (data_80519200 != 0) {
        if (i_this->field_0x9a0 == 0) {
            int iVar1 = dKy_getEnvlight()->daytime / 15.0f;
            if (iVar1 >= 6 && iVar1 <= 17) {
                i_this->field_0x980 = 10000.0f;
            }
        } else {
            i_this->field_0x9a0--;
        }
    }

    s16 sVar1 = i_this->mAction;
    damage_check(i_this);
    if (sVar1 == ACTION_WB_SEARCH && i_this->mAction != ACTION_WB_SEARCH) {
        e_wb_class* bullbo_p = (e_wb_class*)fopAcM_SearchByID(i_this->mWbActorID);
        if (bullbo_p != NULL && bullbo_p->mActionID == 1) {
            bullbo_p->mActionID = 0;
        }
    }

    u8 cVar1 = i_this->field_0x5ba;
    s8 cVar2 = 0;
    s8 bVar1 = 1;
    s8 cVar3 = 0;
    a_this->offHeadLockFlg();

    switch (i_this->mAction) {
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
            if (i_this->mBossMode != 0) {
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

    if (cVar3 != 0 && otoCheck(a_this, 1500.0f)) {
        SND_INFLUENCE* sound = dKy_Sound_get();
        if (!pl_check(i_this, 500.0f, 0x7FFF)) {
            sp25c = sound->position - a_this->current.pos;
            if (sp25c.abs() < sound->field_0xc * 120) {
                i_this->field_0x974 = i_this->mAction;
                i_this->mAction = ACTION_EXCITE;
                i_this->mMode = 0;
                S_find_pos = sound->position;
            }
        }
    }

    if (bVar1) {
        dBgS_ObjGndChk obj_gnd_chk;
        dBgS_ObjGndChk_Spl spl_chk;
        sp25c = a_this->current.pos;
        sp25c.y += 150.0f;
        spl_chk.SetPos(&sp25c);
        obj_gnd_chk.SetPos(&sp25c);

        i_this->field_0x984 = dComIfG_Bgsp().GroundCross(&spl_chk);

        f32 fVar3 = i_this->field_0x984 - dComIfG_Bgsp().GroundCross(&obj_gnd_chk);
        if (a_this->current.pos.y < i_this->field_0x984 && fVar3 > 100.0f) {
            i_this->mAction = ACTION_WATER;
            if (dComIfG_Bgsp().GetPolyAtt0(spl_chk) == 6) {
                i_this->mMode = 1;
            } else {
                i_this->mMode = 0;
            }

            a_this->current.pos.y = i_this->field_0x984;
        }
    }

    if (cVar2 != 0) {
        i_this->mSound.setLinkSearch(true);
        if (data_80519200 != 0 && (i_this->field_0x5b7 == 6 || i_this->field_0x5b7 == 7 || i_this->field_0x5b7 == 8)) {
            S_find = 20;
        }
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    if (cVar1) {
        if (data_80519200 != 0) {
            if (!dComIfGp_event_runCheck() && i_this->field_0x99c == 0 && i_this->mPlayerDistance > 700.0f && i_this->field_0x5bb != 0) {
                fopAcM_delete(a_this);
            }
        } else if (daPy_getPlayerActorClass()->checkHorseRide() && dComIfGp_getHorseActor()->speedF >= 30.0f && i_this->field_0x5bb != 0) {
            fopAcM_delete(a_this);
        }
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x1000);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 2, 0x1000);
    cLib_addCalcAngleS2(&a_this->shape_angle.z, a_this->current.angle.z, 2, 0x1000);


    if (i_this->field_0x9bc == 0) {
        a_this->attention_info.distances[2] = 3;
        if (i_this->field_0x9ec) {
            cMtx_YrotS(*calc_mtx, i_this->field_0xa0e);
            if (i_this->field_0xaf0 == 0) {
                sp25c.z = i_this->field_0x9ec;
            } else {
                sp25c.z = 0.0f;
                a_this->speed.y = 0.0f;
            }

            a_this->gravity = -(l_HIO.jump_g - 1.0f);
        } else {
            cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
            sp25c.z = a_this->speedF;
            a_this->gravity = -5.0f;
        }
        sp25c.x = 0.0f;
        sp25c.y = 0.0f;
        MtxPosition(&sp25c, &sp268);

        a_this->speed.x = sp268.x;
        a_this->speed.z = sp268.z;
        a_this->current.pos += a_this->speed;
        a_this->speed.y += a_this->gravity;
        if (a_this->speed.y < -100.0f) {
            a_this->speed.y = -100.0f;
        }

        if (i_this->field_0x9ad != 0) {
            cXyz* pcVar1 = i_this->mStts.GetCCMoveP();
            if (pcVar1 != NULL) {
                a_this->current.pos.x += pcVar1->x;
                a_this->current.pos.z += pcVar1->z;
            }
        }

        cLib_addCalc0(&i_this->field_0x9c0, 1.0f, 3.0f);
        
        if (i_this->field_0x9f0 > 0.1f) {
            sp25c.x = 0.0f;
            sp25c.y = 0.0f;
            sp25c.z = -i_this->field_0x9f0;
            cMtx_YrotS(*calc_mtx, i_this->field_0x9f4);
            MtxPosition(&sp25c, &sp268);
            a_this->current.pos += sp268;
            cLib_addCalc0(&i_this->field_0x9f0, 1.0f, 7.0f);
        }
    } else {
        a_this->attention_info.distances[fopAc_attn_BATTLE_e] = 34;
        fopAc_ac_c* bullboActor = fopAcM_SearchByID(i_this->mWbActorID);
        e_wb_class* bullbo = (e_wb_class*)bullboActor;
        if (bullbo == NULL) {
            i_this->mAction = ACTION_NORMAL;
            i_this->mMode = 0;
            return;
        }

        a_this->speedF = bullboActor->speedF;
        if (daPy_getPlayerActorClass()->checkHorseRide() && dComIfGp_getHorseActor()->speedF >= 30.0f) {
            a_this->speed = bullboActor->speed;
            a_this->speed.y = 0.0f;
        } else {
            a_this->speed.set(0.0f, 0.0f, 0.0f);
        }

        if (i_this->field_0x9be == 1) {
            MTXCopy(bullbo->mpModelMorf->getModel()->getAnmMtx(bullbo->field_0x688 + 15), *calc_mtx);
        } else {
            MTXCopy(bullbo->mpModelMorf->getModel()->getAnmMtx(bullbo->field_0x688 + 16), *calc_mtx);
        }

        if (i_this->mBossMode != 0) {
            sp25c.set(0.0f, BREG_F(4) + -65.0f, 0.0f);
        } else {
            sp25c.set(0.0f, BREG_F(4) + -45.0f, 0.0f);
        }
        MtxPosition(&sp25c, &i_this->field_0x9b0);

        if (i_this->field_0x9bc == 2) {
            a_this->current.pos = i_this->field_0x9b0;
            a_this->current.angle = bullboActor->shape_angle;
            a_this->shape_angle = a_this->current.angle;

            sp25c.x = 10.0f;
            MtxPosition(&sp25c, &sp268);
            sp25c = sp268 - i_this->field_0x9b0;
            a_this->current.angle.x += (s16)-cM_atan2s(sp25c.y, JMAFastSqrt(sp25c.x * sp25c.x + sp25c.z * sp25c.z));
            a_this->shape_angle.x = a_this->current.angle.x;

            if (i_this->field_0x9be == 1) {
                bullbo->field_0x1434 = fopAcM_GetID(a_this);
            }
        } else {
            sp25c = i_this->field_0x9b0 - a_this->current.pos;
            cMtx_YrotS(*calc_mtx, cM_atan2s(sp25c.x, sp25c.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(sp25c.y, JMAFastSqrt(sp25c.x * sp25c.x + sp25c.z * sp25c.z)));
            sp25c.x = 0.0f;
            sp25c.y = 0.0f;
            sp25c.z = TREG_F(3) + 20.0f;
            MtxPosition(&sp25c, &sp268);

            a_this->current.pos += sp268;
            sp25c = i_this->field_0x9b0 - a_this->current.pos;
            if (sp25c.abs() < 30.0f || i_this->field_0x990[3] == 0) {
                i_this->field_0x9bc = 2;
            }

            cLib_addCalcAngleS2(&a_this->current.angle.y, bullboActor->shape_angle.y, 4, 0x800);
        }

        i_this->field_0x9c0 += i_this->field_0x9c4;
        i_this->field_0x9c4 -= 4.5f;

        if (i_this->field_0x9c0 <= 0.0f) {
            i_this->field_0x9c0 = 0.0f;
            i_this->field_0x9c4 = 0.0f;
        }
    }

    s16 sVar2 = 0;
    i_this->field_0x9a4 = 0;
    cXyz sp274;
    s16 sVar3 = 0x1000;

    if (i_this->field_0x9c8 == -1) {
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        sp25c.x = 0.0f;
        sp25c.y = KREG_F(8) + 70.0f;
        sp25c.z = 50.0f;
        MtxPosition(&sp25c, &sp274);

        sp274 += a_this->current.pos;
        i_this->field_0x9c8 = 2;
        sVar3 = KREG_S(7) + 0x800;
    } else if (i_this->field_0x9c8 >= 11) {
        i_this->field_0x9c8 -= 10;
        sp274 = i_this->field_0x9d4;
    } else {
        sp274 = actor->eyePos;
    }

    if (i_this->field_0x9bc == 2) {
        sVar3 = 0x1000;
    }

    if (i_this->field_0x9c8 != 0) {
        sp25c = sp274 - a_this->current.pos;
        sp25c.y += TREG_F(2) + -150.0f;
        s16 sVar4 = cM_atan2s(sp25c.x, sp25c.z) - a_this->shape_angle.y;
        sVar2 = a_this->shape_angle.x + cM_atan2s(sp25c.y, JMAFastSqrt(sp25c.x * sp25c.x + sp25c.z * sp25c.z));
        if (i_this->field_0x9c8 <= 2) {
            if (sVar4 > 0x3E80) {
                sVar4 = 16000;
            } else if (sVar4 < -16000) {
                sVar4 = -16000;
            }

            if (sVar2 > 0x3E80) {
                sVar2 = 16000;
            } else if (sVar2 < -16000) {
                sVar2 = -16000;
            }

            cLib_addCalcAngleS2(&i_this->field_0x9ca, sVar4, 2, sVar3);
        } else if (i_this->field_0x9c8 == 5) {
            if ((i_this->field_0x970 & 15) == 0 && cM_rndF2(1.0f) < 0.3f) {
                i_this->field_0x9d0 = cM_rndFX2(2000.0f);
            }
        } else {
            sVar1 = XREG_S(5) + 30000;
            if (sVar1 < sVar4) {
                sVar4 = sVar1;
                i_this->field_0x9a4 = 1;
            } else if (sVar4 < -(XREG_S(6) + 25000)) {
                sVar4 = -(XREG_S(6) + 25000);
                i_this->field_0x9a4 = 1;
            }

            cLib_addCalcAngleS2(&i_this->field_0x9ca, sVar4 / 2, 2, 0x1000);
            sVar2 = -cM_atan2s(sp25c.y, JMAFastSqrt(sp25c.x * sp25c.x + sp25c.z * sp25c.z));
            if (sVar2 > 0x2AF8) {
                sVar2 = 11000;
                i_this->field_0x9a4 = 1;
            } else if (sVar2 < -8000) {
                sVar2 = -8000;
                i_this->field_0x9a4 = 1;
            }
        }
    } else {
        cLib_addCalcAngleS2(&i_this->field_0x9ca, 0, 2, 0x1000);
        i_this->field_0x9d0 = 0;
    }

    cLib_addCalcAngleS2(&i_this->field_0x9cc, sVar2, 2, sVar3);
    cLib_addCalcAngleS2(&i_this->field_0x9ce, i_this->field_0x9d0, 4, 0x500);

    if (fabsf(i_this->field_0xa2c) > 1.0f) {
        i_this->field_0xa74[1].x = i_this->field_0xa2c * cM_scos(i_this->field_0xa30) * (BREG_F(0) + 70.0f);
        i_this->field_0xa74[2].x += i_this->field_0xa74[1].x;
        cLib_addCalc0(&i_this->field_0xa2c, 1.0f, 3.0f);
        i_this->field_0xa30 += 4000;
    }

    if (fabsf(i_this->field_0xa24) > 1.0f) {
        i_this->field_0xa74[0].x = i_this->field_0xa24 * cM_scos(i_this->field_0xa28) * (BREG_F(2) + 70.0f);
        cLib_addCalc0(&i_this->field_0xa24, 1.0f, 3.0f);
        i_this->field_0xa28 += 4000;
    }

    if (a_this->speed.y < 0.0f && i_this->mObjAcch.ChkGroundHit()) {
        cLib_addCalc0(&i_this->field_0xa2c, 1.0f, 30.0f);
        cLib_addCalc0(&i_this->field_0xa24, 1.0f, 30.0f);
    }

    if (i_this->field_0x9ec) {
        sVar1 = i_this->field_0xa0e - a_this->shape_angle.y;
        i_this->field_0xa74[3].y = 0x4000 - sVar1;
        if (i_this->field_0xa74[3].y > 0x1770) {
            i_this->field_0xa74[3].y = 6000;
        } else if (i_this->field_0xa74[3].y < -12000) {
            i_this->field_0xa74[3].y = -12000;
        }

        i_this->field_0xa74[4].y = 0x4000 - sVar1;
        if (i_this->field_0xa74[4].y > 0x3A98) {
            i_this->field_0xa74[4].y = 15000;
        } else if (i_this->field_0xa74[4].y < 0) {
            i_this->field_0xa74[4].y = 0;
        }

        i_this->field_0xa74[5].y = sVar1 + 0x4000;
        if (i_this->field_0xa74[5].y > 0x1770) {
            i_this->field_0xa74[5].y = 6000;
        } else if (i_this->field_0xa74[5].y < -12000) {
            i_this->field_0xa74[5].y = -12000;
        }

        i_this->field_0xa74[6].y = sVar1 + 0x4000;
        if (i_this->field_0xa74[6].y > 0x3A98) {
            i_this->field_0xa74[6].y = 15000;
        } else if (i_this->field_0xa74[6].y < 0) {
            i_this->field_0xa74[6].y = 0;
        }

        cMtx_YrotS(*calc_mtx, sVar1);
        sp25c.x = 0.0f;
        sp25c.y = 0.0f;
        sp25c.z = 12000.0f;
        MtxPosition(&sp25c, &sp268);
        i_this->field_0xae6 = -sp268.x;
        i_this->field_0xae4 = -sp268.z * 1.3f;

        if (i_this->field_0xae4 > 10000) {
            i_this->field_0xae4 = 10000;
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
        i_this->field_0xa74[i].z = 0;
        i_this->field_0xa74[i].x = 0;

        if (i >= 3 && i <= 6) {
            if (i_this->field_0xacc[i] == 0) {
                if (i == 4 || i == 6) {
                    sVar3 = 0x600;
                } else {
                    sVar3 = 0x800;
                }
                cLib_addCalcAngleS2(&i_this->field_0xa74[i].y, 0, 1, sVar3);
            } else {
                i_this->field_0xacc[i]--;
            }
        } else {
            i_this->field_0xa74[i].y = 0;
        }

        if (i >= 3) {
            i_this->field_0xabc[i] = i_this->field_0xab8 * cM_scos(i_this->field_0x970 * (BREG_S(0) + 4000) + i * (BREG_S(1) + 20000));
        }
    }

    cLib_addCalc0(&i_this->field_0xab8, 1.0f, 100.0f);
    cLib_addCalcAngleS2(&i_this->field_0xae0, i_this->field_0xae6, 2, 0x800);
    cLib_addCalcAngleS2(&i_this->field_0xade, i_this->field_0xae4, 2, 0x800);
    i_this->field_0xae4 = 0;
    i_this->field_0xae6 = 0;
    s32 sVar4 = 0;
    s16 sVar5 = 0;

    if (i_this->field_0xa1e != 0) {
        i_this->field_0xa1e--;
        if (i_this->field_0xa0c != 0) {
            Vec sp280;
            cXyz sp28c, sp298;
            dBgS_GndChk gnd_chk;
            f32 fVar2 = 75.0f;

            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(1), *calc_mtx);
            sp25c.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&sp25c, &sp28c);
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

                if (sp280.y != -G_CM3D_F_INF) {
                    sVar5 = -cM_atan2s(sp280.y - sp28c.y, sp280.z - sp28c.z);
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
                    sVar4 = cM_atan2s(sp280.y - sp28c.y, sp280.x - sp28c.x);
                    if (sVar4 > 0x3000 || sVar4 < -0x3000) {
                        sVar4 = 0;
                    }
                }
            }

            dBgS_LinChk lin_chk;
            cXyz sp2a4, start, end;

            start = a_this->current.pos;
            start.y += JREG_F(5) + 30.0f;
            J3DModel* model = i_this->mpModelMorf->getModel();
            sp2a4.set(0.0f, 0.0f, 0.0f);
            MTXCopy(model->getAnmMtx(BREG_S(8) + 13), *calc_mtx);
            MtxPosition(&sp2a4, &end);
            end.y += JREG_F(6) + 30.0f;

            lin_chk.Set(&start, &end, a_this);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                sp2a4 = start - end;
                cMtx_YrotS(*calc_mtx, cM_atan2s(sp2a4.x, sp2a4.z));
                sp2a4.x = 0.0f;
                sp2a4.y = 0.0f;
                sp2a4.z = TREG_F(11) + 50.0f;
                MtxPosition(&sp2a4, &sp268);
                a_this->current.pos += sp268;
            }

            i_this->field_0xa18 = sVar5;
            i_this->field_0xa1c = sVar4;
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0xa12, i_this->field_0xa18, 1, 0x400);
    cLib_addCalcAngleS2(&i_this->field_0xa16, i_this->field_0xa1c, 1, 0x400);

    if (i_this->field_0x129c != 0) {
        i_this->field_0x129c--;
        if (i_this->field_0x129c == 0) {
            dComIfGp_particle_set(0x15E, &a_this->eyePos, &a_this->shape_angle, NULL);
        }
    }

    if (i_this->field_0x125d != 0) {
        MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(11), *calc_mtx);
        sp25c.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&sp25c, &sp268);

        dBgS_ObjGndChk_Spl spl_chk_2;
        sp25c = sp268;
        sp25c.y += 100.0f;
        spl_chk_2.SetPos(&sp25c);
        if (fabsf(dComIfG_Bgsp().GroundCross(&spl_chk_2) - sp268.y) < 50.0f) {
            static u16 w_eff_name[3] = {
                0x8258,
                0x8259,
                0x825A,
            };

            cXyz sp2c8(1.0f, 1.0f, 1.0f);
            csXyz cStack_330(0, 0, 0);
            for (int i = 0; i < 3; i++) {
                i_this->field_0x1268[i] = dComIfGp_particle_set(i_this->field_0x1268[i], w_eff_name[i], &sp268, &a_this->tevStr, 
                                                                &cStack_330, &sp2c8, 0xFF, 0, -1, NULL, NULL, NULL);
            }
        } else {
            fopAcM_effSmokeSet1(&i_this->field_0x1260, &i_this->field_0x1264, &sp268, NULL, 1.3f, &a_this->tevStr, 1);
        }

        i_this->field_0x125d = 0;
    }

    cXyz sp2d4(1.3f, 1.3f, 1.3f);
    setMidnaBindEffect(a_this, &i_this->mSound, &a_this->eyePos, &sp2d4);
}

static void fire_eff_set(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp48, sp54;
    int iVar1;
    u16 uVar1[2];
    f32 fVar1;

    if (i_this->field_0x5bc == 4) {
        iVar1 = 2;
        uVar1[0] = 0x1DF;
        uVar1[1] = 0x1DE;
        i_this->mSound.startCreatureSoundLevel(Z2SE_OBJ_BOMB_IGNITION, 0, -1);
        fVar1 = NREG_F(18) + 2.0f;
        sp48.x = NREG_F(0) + 62.0f;
        sp48.y = NREG_F(1) + 22.0f;
        sp48.z = NREG_F(2) + -50.0f;
    } else {
        iVar1 = 1;
        uVar1[0] = 0x8113;
        i_this->mSound.startCreatureSoundLevel(Z2SE_OBJ_ARROW_FIRE_READY, 0, -1);
        fVar1 = 1.0f;
        sp48.x = NREG_F(0) + 90.0f;
        sp48.y = NREG_F(1) + 10.0f;
        sp48.z = NREG_F(2) + -60.0f;
    }

    MtxPosition(&sp48, &sp54);
    i_this->field_0x1288 = sp54 - i_this->field_0x127c;
    i_this->field_0x1288 *= 0.9f;
    i_this->field_0x127c = sp54;

    cXyz sp60(fVar1, fVar1, fVar1);
    for (int i = 0 ; i < iVar1; i++) {
        i_this->field_0x1274[i] = dComIfGp_particle_set(i_this->field_0x1274[i], uVar1[i], &sp54, &a_this->shape_angle, &sp60);

        if (i == 0) {
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x1274[i]);
            if (emitter != NULL) {
                emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                emitter->setUserWork((uintptr_t)&i_this->field_0x1288);
            }
        }
    }
}

static void* s_wb_sub2(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_WB && target_info_count < 2) {
        target_info[target_info_count] = (fopAc_ac_c*)i_actor;
        target_info_count++;
    }

    return NULL;
}

static void* s_lv9dn_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_DN && ((e_dn_class*)i_actor)->status != 0) {
        ((e_dn_class*)i_actor)->status = 0;
        return i_actor;
    }

    return NULL;
}

static void* s_lv9dn_sub2(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_DN && ((e_dn_class*)i_actor)->status == 0) {
        ((e_dn_class*)i_actor)->status = 1;
        ((e_dn_class*)i_actor)->mode = 0;
    }
    
    return NULL;
}

static void* s_lv9dn_sub3(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_DN) {
        ((e_dn_class*)i_actor)->mode = 2;
    }

    return NULL;
}

static void* s_lv9rd_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && ((e_rd_class*)i_actor)->field_0xafb != 0) {
        ((e_rd_class*)i_actor)->field_0xafb = 0;
    }

    return NULL;
}

static u8 data_80519338;

static void* s_lv9rd_sub2(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && ((e_rd_class*)i_actor)->field_0x5b7 == data_80519338 && ((e_rd_class*)i_actor)->mAction != ACTION_DROP) {
        e_rd_class* i_this = (e_rd_class*)i_actor;
        fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

        i_this->mAction = ACTION_DROP;
        i_this->mMode = 2;
        anm_init(i_this, BCK_RD_FURA2, 2.0f, 2, 1.0f);
        i_this->field_0x990[0] = 1000;
        i_this->field_0x5cc = a_this->home.angle.y;
        dComIfGp_setHitMark(1, a_this, &a_this->eyePos, NULL, NULL, 0);
        i_this->mSound.startCollisionSE(Z2SE_HIT_WOOD_WEAPON, 31);
        return i_actor;
    }
        
    return NULL;
}

static void* s_lv9rd_sub3(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD && (((e_rd_class*)i_actor)->field_0x5b7 == 13 || ((e_rd_class*)i_actor)->field_0x5b7 == 14)) {
        ((e_rd_class*)i_actor)->field_0x9a4 = 1;
        ((e_rd_class*)i_actor)->field_0x980 = 50000.0f;
    }

    return NULL;
}

static void* s_lv9arrow_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_ARROW) {
        ((e_arrow_class*)i_actor)->field_0xa14 = 1;
    }

    return NULL;
}

static void* s_lv9arrow_sub2(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_ARROW) {
        ((e_arrow_class*)i_actor)->field_0xa14 = 0;
        ((e_arrow_class*)i_actor)->mAction = ACTION_BOW_RUN;
        ((e_arrow_class*)i_actor)->mMode = 0;
    }

    return NULL;
}

static void cam_3d_morf(e_rd_class* i_this, f32 param_2) {
    cLib_addCalc2(&i_this->mDemoCamCenter.x, i_this->field_0x12cc.x, param_2, i_this->field_0x12e4.x * i_this->field_0x130c);
    cLib_addCalc2(&i_this->mDemoCamCenter.y, i_this->field_0x12cc.y, param_2, i_this->field_0x12e4.y * i_this->field_0x130c);
    cLib_addCalc2(&i_this->mDemoCamCenter.z, i_this->field_0x12cc.z, param_2, i_this->field_0x12e4.z * i_this->field_0x130c);
    cLib_addCalc2(&i_this->mDemoCamEye.x, i_this->field_0x12c0.x, param_2, i_this->field_0x12d8.x * i_this->field_0x130c);
    cLib_addCalc2(&i_this->mDemoCamEye.y, i_this->field_0x12c0.y, param_2, i_this->field_0x12d8.y * i_this->field_0x130c);
    cLib_addCalc2(&i_this->mDemoCamEye.z, i_this->field_0x12c0.z, param_2, i_this->field_0x12d8.z * i_this->field_0x130c);
}

static void cam_spd_set(e_rd_class* i_this) {
    i_this->field_0x12d8.x = fabsf(i_this->field_0x12c0.x - i_this->mDemoCamEye.x);
    i_this->field_0x12d8.y = fabsf(i_this->field_0x12c0.y - i_this->mDemoCamEye.y);
    i_this->field_0x12d8.z = fabsf(i_this->field_0x12c0.z - i_this->mDemoCamEye.z);
    i_this->field_0x12e4.x = fabsf(i_this->field_0x12cc.x - i_this->mDemoCamCenter.x);
    i_this->field_0x12e4.y = fabsf(i_this->field_0x12cc.y - i_this->mDemoCamCenter.y);
    i_this->field_0x12e4.z = fabsf(i_this->field_0x12cc.z - i_this->mDemoCamCenter.z);
    i_this->field_0x130c = 0.0f;
}

static void demo_camera(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = (camera_class*) dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera_2 = (camera_class*) dComIfGp_getCamera(0);
    daNPC_TK_c* hawk_p = (daNPC_TK_c*) fopAcM_SearchByName(PROC_NPC_TK);
    fopAc_ac_c* a_hwk = hawk_p;
    cXyz sp38, sp44, sp50, sp5c;
    s8 bVar1 = 0;
    s16 sVar1 = 0;

    switch (i_this->mDemoMode + 1) {
        case 2:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->mDemoMode = 2;
            i_this->field_0x12a6 = 0;
            i_this->mDemoCamFovy = 55.0f;
            camera->mCamera.SetTrimSize(3);

            i_this->mDemoCamCenter = a_this->current.pos;
            i_this->mDemoCamCenter.y = a_this->eyePos.y - 80.0f + 60.0f;
            i_this->field_0x1300 = 2000.0f;
            a_this->current.angle.y = i_this->mPlayerAngleY;
            // fallthrough
        case 3:
            if (i_this->field_0x12a6 < 85) {
                mDoMtx_stack_c::YrotS(a_this->current.angle.y);
                mDoMtx_stack_c::XrotM(0x640);
                sp38.x = 0.0f;
                sp38.y = TREG_F(3);
                sp38.z = i_this->field_0x1300;
                mDoMtx_stack_c::multVec(&sp38, &i_this->mDemoCamEye);
                i_this->mDemoCamEye += a_this->current.pos;
                cLib_addCalc2(&i_this->mDemoCamCenter.y, a_this->eyePos.y - 80.0f + 60.0f, 0.1f, 20.0f);
                cLib_addCalc2(&i_this->field_0x1300, 400.0f, 0.5f, 150.0f);
            } else if (data_80519200 == 124) {
                lbl_70_bss_AE = 1;
                target_info_count = 0;
                for (int i = 0; i < 2; i++) {
                    target_info[i] = NULL;
                }

                fpcM_Search(s_wb_sub2, i_this);

                if (target_info[0] != NULL && target_info[1] != NULL) {
                    i_this->mDemoCamCenter = target_info[0]->current.pos + ((target_info[1]->current.pos - target_info[0]->current.pos) * 0.5f);
                    i_this->mDemoCamCenter.y += 200.0f;

                    if (i_this->field_0x12a6 == 85) {
                        i_this->mDemoCamEye = i_this->mDemoCamCenter;
                        i_this->mDemoCamEye.y += 100.0f;
                        i_this->mDemoCamEye.z += 900.0f;
                    } else {
                        cLib_addCalc2(&i_this->mDemoCamEye.x, i_this->mDemoCamCenter.x, 0.2f, 100.0f);
                        cLib_addCalc2(&i_this->mDemoCamEye.y, i_this->mDemoCamCenter.y + 100.0f, 0.2f, 100.0f);
                        cLib_addCalc2(&i_this->mDemoCamEye.z, i_this->mDemoCamCenter.z + 900.0f, 0.2f, 100.0f);
                    }
                }

                if (i_this->field_0x12a6 == 220) {
                    bVar1 = true;
                }
            } else if (i_this->field_0x12a6 == 140) {
                bVar1 = true;
            }
            break;

        case 11:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->mDemoMode = 11;
            i_this->field_0x12a6 = 0;
            i_this->mDemoCamFovy = 55.0f;
            dComIfGp_offCameraAttentionStatus(0, 8);
            camera->mCamera.SetTrimSize(3);

            i_this->mDemoCamCenter = a_this->current.pos;
            i_this->mDemoCamCenter.y = (a_this->eyePos.y - 80.0f) + 60.0f + TREG_F(5);
            i_this->mDemoCamEye = camera_2->lookat.eye;
            a_this->current.angle.y = i_this->mPlayerAngleY;
            i_this->field_0x1300 = 2000.0f;
            // fallthrough
        case 12:
            mDoMtx_stack_c::YrotS(a_this->current.angle.y);
            mDoMtx_stack_c::XrotM(0x640);
            sp38.x = 0.0f;
            sp38.y = TREG_F(3);
            sp38.z = i_this->field_0x1300;
            mDoMtx_stack_c::multVec(&sp38, &sp44);
            sp44.x += a_this->current.pos.x;
            sp44.z += a_this->current.pos.z;
            sp44.y += a_this->home.pos.y;

            cLib_addCalc2(&i_this->field_0x1300, 400.0f, 0.5f, 150.0f);

            if (i_this->field_0x12a6 < 20) {
                i_this->mDemoCamEye = sp44;
            } else {
                cLib_addCalc2(&i_this->mDemoCamEye.x, sp44.x, 0.1f, 10.0f);
                cLib_addCalc2(&i_this->mDemoCamEye.z, sp44.z, 0.1f, 10.0f);
            }

            cLib_addCalc2(&i_this->mDemoCamCenter.x, a_this->eyePos.x, 0.1f, 10.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, a_this->eyePos.z, 0.1f, 10.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, a_this->eyePos.y - 80.0f + 60.0f, 0.1f, 100.0f);

            if (i_this->field_0x12a6 == 100) {
                bVar1 = true;
            }
            break;

        case 20:
            if (player->getClothesChangeWaitTimer() == 0) {
                i_this->mDemoMode = 21;
                i_this->field_0x12a6 = 0;
            }
            break;

        case 21:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->mDemoMode = 21;
            i_this->field_0x12a6 = 0;
            i_this->mDemoCamFovy = 55.0f;
            camera->mCamera.SetTrimSize(3);

            i_this->mDemoCamCenter = camera_2->lookat.center;
            i_this->mDemoCamEye = camera_2->lookat.eye;
            i_this->field_0x12cc.set(-7875.0f, 2125.0f, 7895.0f);
            i_this->field_0x12c0.set(-7527.0f, 2084.0f, 7552.0f);
            cam_spd_set(i_this);
            i_this->field_0x12e4 *= 2.0f;
            i_this->field_0x12f0 = player->current.pos;
            i_this->field_0x12f0.y = 2000.0f;

            daPy_getPlayerActorClass()->changeOriginalDemo();
            if (daPy_py_c::checkNowWolf()) {
                daPy_getPlayerActorClass()->changeDemoMode(71, 0, 0, 0);
                i_this->mDemoMode = 19;
            }
            // fallthrough
        case 22:
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&i_this->field_0x12f0, player->shape_angle.y, 0);

            if (i_this->mDemoMode != 19) {
                if (i_this->field_0x12a6 == 1) {
                    Z2GetAudioMgr()->bgmStreamPrepare(0x2000055);
                    Z2GetAudioMgr()->bgmStreamPlay();
                }

                cam_3d_morf(i_this, BREG_F(17) + 0.2f);
                cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.1f, 1.0f, BREG_F(17) + 0.002f);

                if (i_this->field_0x12a6 == 70) {
                    dComIfGs_onSwitch(75, fopAcM_GetRoomNo(a_this));
                }

                if (i_this->field_0x12a6 >= 70) {
                    sp44.set(-5182.0f, 2000.0f, 5237.0f);
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp44, 0xFFFFE051, 0);
                }

                if (i_this->field_0x12a6 == 120 || i_this->field_0x12a6 == 130
                    || i_this->field_0x12a6 == 140 || i_this->field_0x12a6 == 150) {
                    fpcM_Search(s_lv9dn_sub, i_this);
                }

                if (i_this->field_0x12a6 > 160) {
                    cLib_addCalc2(&i_this->mDemoCamFovy, 55.0f, 0.05f, 0.5f);
                } else {
                    cLib_addCalc2(&i_this->mDemoCamFovy, 45.0f, 0.05f, 0.5f);
                }

                if (i_this->field_0x12a6 == 180) {
                    i_this->field_0x12cc.set(-7241.0f, 4310.0f, 7269.0f);
                    i_this->field_0x12c0.set(-6913.0f, 4139.0f, 6948.0f);
                    cam_spd_set(i_this);
                    i_this->mDemoMode = 22;
                    i_this->field_0x12a6 = 0;
                    fpcM_Search(s_lv9rd_sub, i_this);
                    daPy_getPlayerActorClass()->changeDemoMode(23, 0, 0, 0);
                }
            }
            break;

        case 23:
            cam_3d_morf(i_this, BREG_F(17) + 0.2f);
            cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.15f, 1.0f, BREG_F(17) + 0.005f);
            cLib_addCalc2(&i_this->mDemoCamFovy, 55.0f, 0.1f, 0.5f);

            if (i_this->field_0x12a6 == 13) {
                fpcM_Search(s_lv9dn_sub2,i_this);
            }

            if (i_this->field_0x12a6 == 17 || i_this->field_0x12a6 == 27 || i_this->field_0x12a6 == 37 || i_this->field_0x12a6 == 47) {
                fpcM_Search(s_lv9dn_sub, i_this);
            }

            if (i_this->field_0x12a6 == 55) {
                i_this->mDemoCamCenter.set(-8330.0f, 4359.0f, 8449.0f);
                i_this->mDemoCamEye.set(-8183.0f, 4264.0f, 7991.0f);
                i_this->mDemoCamFovy = 55.0f;
                i_this->mDemoMode = 23;
                i_this->field_0x12a6 = 0;
            }
            break;

        case 24:
            i_this->field_0x9a4 = 1;

            if (i_this->field_0x12a6 == 20) {
                i_this->field_0x980 = 50000.0f;
            }

            if (i_this->field_0x12a6 < 40) {
                i_this->mDemoCamEye.y += 2.0f;
            }

            if (i_this->field_0x12a6 == 40) {
                i_this->mDemoCamCenter.set(-8033.0f, 4259.0f, 8143.0f);
                i_this->mDemoCamEye.set(-8337.0f, 4488.0f, 8451.0f);
            }

            if (i_this->field_0x12a6 >= 40) {
                cLib_addCalc2(&i_this->mDemoCamFovy, 30.0f, 0.05f, 0.5f);
            }

            if (i_this->field_0x12a6 == 110) {
                i_this->field_0x9a4 = 0;
                i_this->mDemoMode = 24;
                i_this->field_0x12a6 = 0;
                i_this->field_0x12cc.set(-4910.0f, 2012.0f, 4976.0f);
                i_this->field_0x12c0.set(-5235.0f, 2179.0f, 5302.0f);
                cam_spd_set(i_this);
            }
            break;

        case 25:
            i_this->field_0x9a4 = 1;
            i_this->field_0x980 = 0.0f;

            if (i_this->field_0x12a6 >= 4) {
                i_this->mBlureRate = 200;
                sVar1 = cM_scos(i_this->field_0x12a6 * 0x500) * 2500.0f;
                i_this->field_0x130c = 0.03f;
                cam_3d_morf(i_this, 0.5f);

                if (i_this->field_0x12a6 == 4) {
                    fpcM_Search(s_lv9arrow_sub, i_this);
                }

                if (i_this->field_0x12a6 == 10) {
                    fpcM_Search(s_lv9dn_sub2, i_this);
                }

                if (i_this->field_0x12a6 == 20) {
                    daPy_getPlayerActorClass()->changeDemoMode(20, 1, 0, 0);
                    hawk_p->mSound.startCreatureVoice(Z2SE_HAWK_V_REGI_DEMO_1, -1);
                }

                if (i_this->field_0x12a6 == 41) {
                    mDoGph_gInf_c::fadeOut(0.5f, g_blackColor);
                }

                if (i_this->field_0x12a6 == 43) {
                    i_this->mDemoMode = 25;
                    i_this->field_0x12a6 = 0;
                    Z2GetAudioMgr()->seStart(Z2SE_AL_FISHING_HIT, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                    i_this->mBlureRate = 0;
                }
            }
            break;

        case 26:
            if (i_this->field_0x12a6 == 40) {
                mDoGph_gInf_c::fadeIn(0.5f, g_blackColor);
                i_this->mDemoCamFovy = 55.0f;
                i_this->mDemoCamCenter.set(-5197.0f, 1712.0f, 5039.0f);
                i_this->mDemoCamEye.set(-5215.0f, 2108.0f, 5327.0f);
                i_this->field_0x12cc.set(-5025.0f, 2270.0f, 4909.0f);
                i_this->field_0x12c0.set(-5215.0f, 2108.0f, 5327.0f);
                cam_spd_set(i_this);
                fpcM_Search(s_lv9arrow_sub2, i_this);
            }

            if (i_this->field_0x12a6 == 80) {
                i_this->mDemoMode = 26;
                i_this->field_0x12a6 = 0;
                daPy_getPlayerActorClass()->changeDemoMode(72, 3, 0, 0);
            }
            break;

        case 27:
            cam_3d_morf(i_this, BREG_F(17) + 0.2f);
            cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.5f, 1.0f, BREG_F(17) + 0.01f);

            if (i_this->field_0x12a6 == 55) {
                i_this->mDemoCamCenter.set(-5529.0f, 2457.0f, 5589.0f);
                i_this->mDemoCamEye.set(-5232.0f, 2201.0f, 5295.0f);
                i_this->field_0x12cc.set(-5580.0f, 2188.0f, 5640.0f);
                i_this->field_0x12c0.set(-5232.0f, 2201.0f, 5295.0f);
                cam_spd_set(i_this);
                i_this->mDemoMode = 27;
                i_this->field_0x12a6 = 0;
            }
            break;

        case 28:
            fpcM_Search(s_lv9rd_sub3, i_this);

            if (i_this->field_0x12a6 == 40) {
                data_80519338 = 14;
                fpcM_Search(s_lv9rd_sub2, i_this);
            } else if (i_this->field_0x12a6 == 60) {
                data_80519338 = 13;
                fpcM_Search(s_lv9rd_sub2, i_this);
            }

            if (i_this->field_0x12a6 == 10 || i_this->field_0x12a6 == 20 || i_this->field_0x12a6 == 30 || i_this->field_0x12a6 == 40) {
                fpcM_Search(s_lv9dn_sub, i_this);
            }

            if (i_this->field_0x12a6 < 100) {
                cLib_addCalc2(&i_this->mDemoCamFovy, 25.0f, 0.2f, 1.0f);
            } else if (i_this->field_0x12a6 >= 120) {
                cLib_addCalc2(&i_this->mDemoCamFovy, 55.0f, 0.05f, 0.4f);
                cam_3d_morf(i_this, BREG_F(17) + 0.2f);
                cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.1f, 1.0f, BREG_F(17) + 0.01f);

                if (i_this->field_0x12a6 == 177) {
                    sp44.set(-5821.0f, 2000.0f, 5850.0f);
                    dBomb_c::createNormalBombExplode(&sp44);
                    dComIfGs_onTmpBit(dSv_event_tmp_flag_c::tempBitLabels[0x76]);
                }

                if (i_this->field_0x12a6 == 180) {
                    dComIfGp_getVibration().StartShock(8, 79, cXyz(0.0f, 1.0f, 0.0f));
                    fpcM_Search(s_lv9dn_sub3, i_this);
                }
            }

            if (i_this->field_0x12a6 != 230) {
                break;
            }

            i_this->mDemoMode = 28;
            i_this->field_0x12a6 = 0;
            i_this->field_0x12cc.set(-5406.0f, 2168.0f, 5468.0f);
            i_this->field_0x12c0.set(-5058.0f, 2181.0f, 5124.0f);
            cam_spd_set(i_this);
            // fallthrough
        case 29:
            if (i_this->field_0x12a6 == 1) {
                player->changeDemoMode(20, 0, 0, 0);
            }

            if (i_this->field_0x12a6 < 25) {
                cam_3d_morf(i_this, BREG_F(17) + 0.2f);
                cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.2f, 1.0f, BREG_F(17) + 0.01f);

                if (i_this->field_0x12a6 == 23) {
                    hawk_p->setResistanceDemo();
                }
            } else {
                if (i_this->field_0x12a6 == 25) {
                    i_this->field_0x130c = 0.0f;
                }

                sp5c = a_hwk->current.pos;
                cLib_addCalc2(&i_this->mDemoCamCenter.x, sp5c.x, 0.2f, i_this->field_0x130c * 100.0f);
                cLib_addCalc2(&i_this->mDemoCamCenter.y, sp5c.y + BREG_F(13), 0.2f, i_this->field_0x130c * 100.0f);
                cLib_addCalc2(&i_this->mDemoCamCenter.z, sp5c.z, 0.2f, i_this->field_0x130c * 100.0f);

                cLib_addCalc2(&i_this->field_0x130c, 1.0f, 1.0f, 0.03f);

                if (i_this->field_0x12a6 >= 55) {
                    if (i_this->field_0x12a6 == 110) {
                        hawk_p->mSound.startCreatureVoice(Z2SE_HAWK_V_REGI_DEMO_2, -1);
                    }

                    if (i_this->field_0x12a6 < 135) {
                        cLib_addCalc2(&i_this->mDemoCamFovy, 20.0f, 0.2f, 1.0f);
                    } else {
                        cLib_addCalc2(&i_this->mDemoCamFovy, 55.0f, 0.1f, 0.5f);
                    }
                }

                if (i_this->field_0x12a6 == 205) {
                    i_this->mDemoMode = 29;
                    i_this->field_0x12a6 = 0;
                    daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
                }
            }
            break;

        case 30:
            i_this->mDemoCamFovy = 55.0f;
            i_this->mDemoCamEye.set(-3963.0f, 147.0f, 8094.0f);
            sp5c = a_hwk->current.pos;

            if (i_this->field_0x12a6 == 0) {
                i_this->mDemoCamCenter = sp5c;
            } else {
                cLib_addCalc2(&i_this->mDemoCamCenter.x, sp5c.x, 0.2f, 100.0f);
                cLib_addCalc2(&i_this->mDemoCamCenter.y, sp5c.y, 0.2f, 100.0f);
                cLib_addCalc2(&i_this->mDemoCamCenter.z, sp5c.z, 0.2f, 100.0f);

                if (i_this->field_0x12a6 == 140) {
                    i_this->field_0x12cc.set(-4080.0f, 134.0f, 8190.0f);
                    i_this->field_0x12c0 = i_this->mDemoCamEye;
                    cam_spd_set(i_this);
                    i_this->mDemoMode = 30;
                    i_this->field_0x12a6 = 0;
                }
            }
            break;

        case 31:
            cam_3d_morf(i_this, BREG_F(17) + 0.1f);
            cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.1f, 1.0f, BREG_F(17) + 0.002f);

            if (i_this->field_0x12a6 == 70) {
                i_this->mDemoCamCenter.set(-4527.0f, 143.0f, 8305.0f);
                i_this->mDemoCamEye.set(-4676.0f, 161.0f, 8282.0f);
                i_this->field_0x12cc.set(-4552.0f, 143.0f, 8461.0f);
                i_this->field_0x12c0.set(-4701.0f, 161.0f, 8438.0f);
                cam_spd_set(i_this);
                i_this->mDemoMode = 31;
                i_this->field_0x12a6 = 0;
                sp44.set(-5397.0f, 2000.0f, 5921.0f);
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp44, 0x213F, 0);
            }
            break;

        case 32:
            cam_3d_morf(i_this, BREG_F(17) + 0.05f);
            cLib_addCalc2(&i_this->field_0x130c, BREG_F(16) + 0.05f, 1.0f, BREG_F(17) + 0.001f);

            if (i_this->field_0x12a6 == 70) {
                i_this->mDemoMode = 32;
                i_this->field_0x12a6 = 0;
                i_this->mDemoCamCenter.set(-5275.0f, 2327.0f, 5812.0f);
                i_this->mDemoCamEye.set(-5367.0f, 2689.0f, 5495.0f);
                i_this->mDemoCamFovy = 30.0f;
            }
            break;

        case 33:
            if (i_this->field_0x12a6 == 60) {
                i_this->mDemoCamCenter.set(-3906.0f, 42.0f, 8198.0f);
                i_this->mDemoCamEye.set(-4274.0f, 272.0f, 7969.0f);
                i_this->mDemoMode = 33;
                i_this->field_0x12a6 = 0;
                dComIfGs_onTmpBit(dSv_event_tmp_flag_c::tempBitLabels[0x75]);
            }
            break;

        case 34:
            if (i_this->field_0x12a6 == 65) {
                daPy_getPlayerActorClass()->changeDemoMode(60, 1, 0, 0);
            }

            if (i_this->field_0x12a6 == 70) {
                i_this->mDemoCamCenter.set(-5712.0f, 2308.0f, 5828.0f);
                i_this->mDemoCamEye.set(-5286.0f, 2104.0f, 5957.0f);
                i_this->mDemoCamFovy = 45.0f;
                i_this->mDemoMode = 34;
                i_this->field_0x12a6 = 0;
            }
            break;

        case 35:
            cLib_addCalc2(&i_this->mDemoCamFovy, 40.0f, 0.05f, 0.02f);
            if (i_this->field_0x12a6 == 80) {
                i_this->mDemoCamCenter.set(-4712.0f, 870.0f, 7432.0f);
                i_this->mDemoCamEye.set(-4852.0f, 1171.0f, 7072.0f);
                i_this->mDemoCamFovy = 55.0f;
                i_this->mDemoMode = 35;
                i_this->field_0x12a6 = 0;
                dComIfGs_onTmpBit(dSv_event_tmp_flag_c::tempBitLabels[0x74]);
            }
            break;

        case 36:
            if (i_this->field_0x12a6 == 120) {
                bVar1 = true;
                int swBit = (fopAcM_GetParam(a_this) & 0xFF000000) >> 24;
                if (swBit != 0xFF) {
                    dComIfGs_onSwitch(swBit, fopAcM_GetRoomNo(a_this));
                }
            }
            break;
    }

    if (bVar1) {
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->mDemoMode = -1;
    }

    if (i_this->mDemoMode > 0) {
        cXyz sp68, sp74;
        sp68 = i_this->mDemoCamCenter;
        sp74 = i_this->mDemoCamEye;
        camera->mCamera.Set(sp68, sp74, sVar1, i_this->mDemoCamFovy);
        i_this->field_0x12a6++;
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

    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp98, spa4, spb0;

    if (i_this->field_0x5b8 == 11) {
        fopAcM_delete(a_this);
        return 1;
    }

    if (S_find != 0) {
        S_find--;
    }

    i_this->field_0x970++;

    if (i_this->mBossMode == 0) {
        i_this->field_0x1297 = i_this->field_0x1298;
    } else {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);

        // Bridge of Eldin Battle
        if (strcmp(dComIfGp_getStartStageName(), "F_SP102") == 0 && player->current.pos.y < -2000.0f) {
            cDmr_SkipInfo = 50;
        }

        if (cDmr_SkipInfo != 0) {
            cDmr_SkipInfo--;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (i_this->field_0x990[i] != 0) {
            i_this->field_0x990[i]--;
        }
    }

    if (i_this->field_0x998 != 0) {
        i_this->field_0x998--;
    }

    if (i_this->field_0xaf0 != 0) {
        i_this->field_0xaf0--;
    }

    if (i_this->field_0x99a != 0) {
        i_this->field_0x99a--;
    }

    if (i_this->field_0x99e != 0) {
        i_this->field_0x99e--;
    }

    if (i_this->field_0x99c != 0) {
        i_this->field_0x99c--;
    }

    if (otoCheck(a_this, 1000.0f) || daPy_getPlayerActorClass()->checkWolfBark()) {
        i_this->field_0x9a1 = cM_rndF(10.0f) + 10.0f;
    }

    if (i_this->field_0x9a1 != 0) {
        i_this->field_0x9a1--;
    }

    i_this->field_0x9a2 = 0;
    i_this->field_0x9ad = 1;
    action(i_this);

    if (i_this->field_0x9bc != 0) {
        i_this->field_0x9ad = 0;
    }

    if (i_this->field_0x9bc != 2 && i_this->field_0xaf0 == 0 && i_this->mAction != ACTION_IKKI_END) {
        spa4.set(0.0f, 0.0f, 0.0f);
        if (i_this->field_0x9ec) {
            cMtx_YrotS(*calc_mtx, i_this->field_0xa0e);
            sp98.x = 0.0f;
            sp98.y = TREG_F(8);
            sp98.z = i_this->field_0xaec;
            MtxPosition(&sp98, &spa4);
            cLib_addCalc2(&i_this->field_0xaec, i_this->field_0x9ec * (TREG_F(9) + 2.0f), 1.0f, TREG_F(17) + 5.0f);
        } else {
            cLib_addCalc0(&i_this->field_0xaec, 1.0f, TREG_F(17) + 5.0f);
        }

        if (i_this->mAnmID == BCK_RD_DIEB && i_this->field_0xa0c != 0 && i_this->field_0xa1f == 0) {
            spa4.y += KREG_F(0) + -20.0f;
        }

        a_this->current.pos += spa4;
        a_this->old.pos += spa4;
        i_this->mObjAcch.CrrPos(dComIfG_Bgsp());
        a_this->current.pos -= spa4;
        a_this->old.pos -= spa4;

        if (!i_this->mObjAcch.ChkGroundHit()) {
            if (a_this->speed.y < -10.0f) {
                i_this->field_0xaf2++;
                if (i_this->field_0xaf2 == 20 && i_this->mBossMode == 0) {
                    i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_DEATH, -1);
                }
            }
        } else {
            if (i_this->field_0xaf2 >= ZREG_S(8) + 20 && i_this->field_0x5b7 != 13 && i_this->field_0x5b7 != 14) {
                i_this->mAction = ACTION_DAMAGE;
                i_this->mMode = 0;
                a_this->speed.y = 0.0f;
                i_this->field_0x9ec = -1.0f;
                i_this->field_0xa0c = -0x3000;
                i_this->field_0x998 = 1000;
                a_this->health = 0;
            }

            i_this->field_0xaf2 = 0;
        }

        i_this->field_0x1294 = 0;

        if (i_this->mObjAcch.GetGroundH() != -G_CM3D_F_INF && i_this->mObjAcch.ChkWaterHit() && i_this->mObjAcch.m_wtr.GetHeight() > a_this->current.pos.y) {
            i_this->field_0x1294 = 1;
        }
    }

    if (i_this->field_0xafb == 0) {
        mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y + i_this->field_0x9c0, a_this->current.pos.z);
        mDoMtx_stack_c::XrotM((s16) i_this->field_0xa12);
        mDoMtx_stack_c::ZrotM((s16) i_this->field_0xa16);
        mDoMtx_stack_c::YrotM((s16) i_this->field_0xa0e);
        mDoMtx_stack_c::XrotM((s16) i_this->field_0xa0c);
        mDoMtx_stack_c::YrotM(-i_this->field_0xa0e);
        mDoMtx_stack_c::YrotM((s16) a_this->shape_angle.y);
        mDoMtx_stack_c::XrotM((s16) a_this->shape_angle.x);
        mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);

        f32 fVar1 = l_HIO.model_size * a_this->scale.x;
        if (i_this->mBossMode != 0) {
            fVar1 *= l_HIO.field_0xc;
        }
        mDoMtx_stack_c::scaleM(fVar1, fVar1, fVar1);

        J3DModel* my_model_p = i_this->mpModelMorf->getModel();
        my_model_p->setBaseTRMtx(mDoMtx_stack_c::get());
        i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
        int frame = i_this->mpModelMorf->getFrame();

        if (i_this->mAnmID == BCK_RD_RUN && (frame == 1 || frame == 10)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_RUNNING_BREATH, -1);
        }

        if (i_this->mAnmID == BCK_RD_KYORO2 && (frame == 5 || frame == 26)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_RUNNING_BREATH, -1);
        }

        if ((i_this->mAnmID == BCK_RD_SHOOT_READY && frame == 10) || (i_this->mAnmID == BCK_RD_RSHOOT_READY && frame == 5) || (i_this->mAnmID == BCK_RD_ARMAMENT && frame == 10)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_READY_WEAPON, -1);
        }

        if ((i_this->mAnmID == BCK_RD_WALK && (i_this->mpModelMorf->checkFrame(1.0f) || i_this->mpModelMorf->checkFrame(15.0f))) ||
            ((i_this->mAnmID == BCK_RD_RUN || i_this->mAnmID == BCK_RD_RUN02) && (i_this->mpModelMorf->checkFrame(1.0f) || i_this->mpModelMorf->checkFrame(7.0f)))) {
            if (i_this->field_0x1294 != 0) {
                i_this->mSound.startCreatureSound(Z2SE_RIDER_FOOTNOTE_WATER, 0, -1);
            } else {
                i_this->mSound.startCreatureSound(Z2SE_RIDER_FOOTNOTE, 0, -1);
            }
        }

        if (i_this->mBossMode != 0) {
            if (i_this->mAnmID == e_rdb_class::BCK_RB_RCOMEON) {
                if (i_this->mpModelMorf->checkFrame(30.0f)) {
                    i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_LAUGH, -1);
                }
            } else if (i_this->mAnmID == e_rdb_class::BCK_RB_RJUMP_C && i_this->mpModelMorf->checkFrame(21.0f)) {
                i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_HOICK2, -1);
            }
        }

        if (i_this->field_0x680 != 0 && i_this->mpModelMorf->isStop()) {
            int i_anmID;
            if (a_this->speedF >= 10.0f) {
                i_anmID = BCK_RD_SHOUT;
            } else {
                i_anmID = BCK_RD_WALK;
            }
            i_this->field_0x680 = 0;
            anm_init(i_this, i_anmID, 10.0f, 2, 1.0f);
        }

        i_this->mpModelMorf->modelCalc();
        
        if (i_this->mAnmID == BCK_RD_SLEEP_WAIT || dComIfGp_checkPlayerStatus0(0, 0x200000)) {
            i_this->field_0x1297 = 0;
        }

        if (i_this->field_0x1297 != 0) {
            J3DModel* model = i_this->mpModelMorf->getModel();
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
                s16 sVar2, sVar1;
                if (i == 0) {
                    sp98.set(38.0f, 0.0f, 0.0f);
                    MtxPosition(&sp98, &spa4);
                    sp98 = camera->lookat.eye - spa4;

                    sVar1 = cM_atan2s(sp98.x, sp98.z);
                    sVar2 = -cM_atan2s(sp98.y, JMAFastSqrt(sp98.x * sp98.x + sp98.z * sp98.z));
                    fVar1 = sp98.abs() * (0.001f + JREG_F(8));
                    if (fVar1 > 2.0f + JREG_F(17)) {
                        fVar1 = 2.0f + JREG_F(17);
                    }

                    dScnKy_env_light_c* env_light = dKy_getEnvlight();
                    int my_scale = env_light->daytime / 15.0f;
                    fVar1 *= i_this->field_0x6cc * time_scale[my_scale];
                    sp98.set(38.0f, 0.0f, 6.0f);
                } else {
                    sp98.set(38.0f, 0.0f, -6.0f);
                }

                MtxPosition(&sp98, &spa4);
                MtxTrans(spa4.x, spa4.y, spa4.z, 0);
                cMtx_YrotM(*calc_mtx, sVar1);
                cMtx_XrotM(*calc_mtx, sVar2);
                MtxScale(fVar1, fVar1, fVar1, 1);

                i_this->mpEyeModels[i]->setBaseTRMtx(*calc_mtx);
                MtxPull();
            }
        }
    }

    if (i_this->field_0xafb == 0) {
        J3DModel* model = i_this->mpModelMorf->getModel();
        MTXCopy(model->getAnmMtx(11), *calc_mtx);
        sp98.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&sp98, &spa4);
        sp98.set(YREG_F(1), YREG_F(2) + 100.0f, YREG_F(3));
        MtxPosition(&sp98, &spb0);

        if (spa4.y < spb0.y) {
            i_this->field_0xa1f = 0;
        } else {
            i_this->field_0xa1f = 1;
        }

        cXyz spbc(0.0f, 0.0f, 0.0f);
        
        if (i_this->field_0x998 != 0) {
            spbc.set(-20000.0f, 200000.0f, 30000.0f);
        }

        if (i_this->mBossMode != 0) {
            MTXCopy(model->getAnmMtx(AREG_S(9) + 6), *calc_mtx);
        } else {
            MTXCopy(model->getAnmMtx(13), *calc_mtx);
        }

        sp98.set(20.0f, 0.0f, 0.0f);
        MtxPosition(&sp98, &a_this->eyePos);

        if (daPy_py_c::checkNowWolf()) {
            spbc.y += 30.0f;
            i_this->field_0xd58[0].SetC(a_this->eyePos + spbc);
            i_this->field_0xd58[0].SetR(50.0f);
        } else {
            i_this->field_0xd58[0].SetC(a_this->eyePos + spbc);
            i_this->field_0xd58[0].SetR(35.0f);
        }

        if (i_this->field_0x9bc == 2) {
            a_this->attention_info.position = a_this->current.pos;
            a_this->attention_info.position.y += 190.0f;

            if (i_this->mBossMode != 0) {
                a_this->attention_info.position.y += 120.0f;
            }
        } else {
            a_this->attention_info.position = a_this->eyePos;
            a_this->attention_info.position.y += 30.0f;
        }

        if (i_this->mBossMode != 0) {
            f32 fVar2 = 60.0f;
            f32 fVar1 = 0.0f;
            if (i_this->mBossMode == 2) {
                fVar2 = 90.0f;
                fVar1 = l_HIO.field_0x3c;
            }

            sp98.set(ZREG_F(0), ZREG_F(1), ZREG_F(2));
            MTXCopy(model->getAnmMtx(2), *calc_mtx);
            MtxPosition(&sp98, &spa4);
            i_this->field_0xd58[2].SetC(spa4 + spbc);
            i_this->field_0xd58[2].SetR((fVar2 + ZREG_F(3)) + fVar1);

            sp98.set(ZREG_F(4), ZREG_F(5), ZREG_F(6));
            MTXCopy(model->getAnmMtx(22), *calc_mtx);
            MtxPosition(&sp98, &spa4);
            i_this->field_0xd58[1].SetC(spa4 + spbc);
            i_this->field_0xd58[1].SetR(fVar2 + ZREG_F(7));
        } else {
            sp98.set(BREG_F(14), BREG_F(15), BREG_F(16));
            MTXCopy(model->getAnmMtx(12), *calc_mtx);
            MtxPosition(&sp98, &spa4);
            i_this->field_0xd58[2].SetC(spa4 + spbc);
            i_this->field_0xd58[2].SetR(35.0f);

            sp98.set(BREG_F(11) + -10.0f, BREG_F(12), BREG_F(13));
            MTXCopy(model->getAnmMtx(10), *calc_mtx);
            MtxPosition(&sp98, &spa4);
            i_this->field_0xd58[1].SetC(spa4 + spbc);
            i_this->field_0xd58[1].SetR(35.0f);
        }

        for (int i = 0; i <= 2; i++) {
            if (i_this->field_0x9ad != 0) {
                i_this->field_0xd58[i].OnCoSetBit();
            } else {
                i_this->field_0xd58[i].OffCoSetBit();
            }

            dComIfG_Ccsp()->Set(&i_this->field_0xd58[i]);

            if (i_this->mBossMode == 1 && i_this->field_0x9bc == 2) {
                i_this->field_0xd58[i].OnTgNoHitMark();
            } else {
                i_this->field_0xd58[i].OffTgNoHitMark();
            }

            if (i_this->mBossMode == 3) {
                for (int j = 0; j < 3; j++) {
                    i_this->field_0xd58[j].SetTgType(0x2022);
                    i_this->field_0xd58[j].OnTgNoHitMark();
                }
            }
        }
    }

    if (i_this->field_0x9ab == 2) {
        MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(11), *calc_mtx);
        sp98.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&sp98, &spa4);
        i_this->field_0x1100.SetR(l_HIO.model_size * 50.0f);
        i_this->field_0x1100.SetC(spa4);
        dComIfG_Ccsp()->Set(&i_this->field_0x1100);
        i_this->field_0x9ab = 0;
    }

    if (i_this->field_0x5bc == 1) {
        if (i_this->field_0x5bd == 0) {
            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(24), *calc_mtx);
        } else {
            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(11), *calc_mtx);
            cMtx_YrotM(*calc_mtx, 6000);
            cMtx_XrotM(*calc_mtx, 10000);
            cMtx_ZrotM(*calc_mtx, 18000);
            MtxTrans(-30.0f, -50.0f, 20.0f, 1);
        }

        i_this->field_0x694->setBaseTRMtx(*calc_mtx);

        if (i_this->field_0x9ab != 0) {
            if (daPy_getPlayerActorClass()->checkHorseRide() || daPy_getPlayerActorClass()->checkBoarRide()) {
                sp98.set(0.0f, nREG_F(15) + 50.0f, 0.0f);
                i_this->field_0x1100.SetR((nREG_F(16) + 60.0f) * l_HIO.model_size);
            } else {
                sp98.set(0.0f, 30.0f, 0.0f);
                i_this->field_0x1100.SetR(l_HIO.model_size * 50.0f);
            }

            MtxPosition(&sp98, &spa4);

            if (i_this->field_0x9ac == 0) {
                i_this->field_0x9ac = 1;
                i_this->field_0x1100.StartCAt(spa4);
            } else {
                i_this->field_0x1100.MoveCAt(spa4);
            }

            i_this->field_0x9ab = 0;
        } else {
            cXyz spc8(-200000.0f, -200000.0f, -200000.0f);
            i_this->field_0x1100.SetC(spc8);
            i_this->field_0x1100.SetR(-200.0f);
            i_this->field_0x9ac = 0;
        }

        dComIfG_Ccsp()->Set(&i_this->field_0x1100);
    }

    if (i_this->field_0x5bc >= 2) {
        J3DModel* model = i_this->mpModelMorf->getModel();

        if (i_this->field_0x9a6 != 0) {
            i_this->field_0x9a6--;
        }

        i_this->field_0x9a8 = i_this->field_0x9a6 * cM_ssin(i_this->field_0x9a6 * (TREG_S(9) + 0x7800)) * (TREG_F(5) + 100.0f);

        model = i_this->mpMorfBowAnm->getModel();
        if (i_this->field_0x5bd == 0) {
            model->setBaseTRMtx(i_this->mpModelMorf->getModel()->getAnmMtx(19));
        } else {
            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(11), *calc_mtx);
            cMtx_YrotM(*calc_mtx, 6000);
            cMtx_XrotM(*calc_mtx, 10000);
            cMtx_ZrotM(*calc_mtx, 18000);
            MtxTrans(-20.0f, -20.0f, 30.0f, 1);
            model->setBaseTRMtx(*calc_mtx);
        }

        i_this->mpMorfBowAnm->play(0, 0);
        i_this->mpMorfBowAnm->modelCalc();

        if (i_this->field_0x9a2 != 0) {
            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(24), *calc_mtx);
            i_this->field_0x694->setBaseTRMtx(*calc_mtx);

            if (i_this->field_0x5bc >= 3) {
                if (i_this->field_0x9bc != 2 || i_this->field_0x990[1] <= 18) {
                    fire_eff_set(i_this);
                }
            } else {
                i_this->field_0x127c.set(-200000.0f, -200000.0f, -200000.0f);
            }
        }

        if (i_this->field_0x9a3 != 0) {
            if (i_this->field_0x5bc == 3) {
                i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_SHOT_FIRE, 0, -1);
            } else {
                i_this->mSound.startCreatureSound(Z2SE_OBJ_ARROW_SHOT_NORMAL, 0, -1);
            }

            i_this->mSound.startCreatureVoice(Z2SE_EN_RD_V_SHOT_ARROW, -1);

            sp98.set(0.0f, 0.0f, 0.0f);
            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(24), *calc_mtx);
            MtxPosition(&sp98, &spa4);

            csXyz local_148;
            int iVar1 = 0;

            if (i_this->mAction == ACTION_BOW_IKKI) {
                sp98 = i_this->field_0x9d4 - spa4;
                local_148.y = cM_atan2s(sp98.x, sp98.z);
                local_148.x = -cM_atan2s(sp98.y, JMAFastSqrt(sp98.x * sp98.x + sp98.z * sp98.z));
            } else {
                fopAc_ac_c* actor = get_pla(a_this);
                if (actor != dComIfGp_getPlayer(0)) {
                    iVar1 |= 32;
                }

                sp98 = actor->eyePos;

                if (i_this->field_0x1296 != 0) {
                    f32 mult_fact = TREG_F(7) + 15.0f;
                    sp98.x += actor->speed.x * mult_fact;
                    sp98.z += actor->speed.z * mult_fact;
                }

                sp98 -= spa4;
                sp98.y -= 30.0f;
                local_148.y = cM_atan2s(sp98.x, sp98.z);

                if (i_this->field_0x9bc == 2 && !pl_pass_check(i_this, YREG_F(18))) {
                    s16 sVar3 = cM_rndF(1000.0f) + 2000.0f;
                    if (cM_rndF(1.0f) < 0.5f) {
                        sVar3 *= -1;
                    }

                    local_148.y += sVar3;
                }

                local_148.x = -cM_atan2s(sp98.y, JMAFastSqrt(sp98.x * sp98.x + sp98.z * sp98.z));

                // NOT Hyrule Field
                if (strcmp(dComIfGp_getStartStageName(), "F_SP121") != 0 && i_this->field_0x1296 == 0) {
                    local_148.x += (s16)(cM_rndFX(200.0f) + -500.0f);
                    local_148.y += (s16)cM_rndFX(100.0f);
                }
            }

            local_148.z = 0;
            if (i_this->field_0x5bc == 3) {
                iVar1 |= 1;
            } else if (i_this->field_0x5bc == 4) {
                iVar1 |= 2;
            }

            if (i_this->field_0x5b8 == 11) {
                iVar1 |= 16;
            }

            fopAcM_createChild(PROC_E_ARROW, fopAcM_GetID(a_this), iVar1, &spa4, fopAcM_GetRoomNo(a_this), &local_148, NULL, -1, NULL);
            i_this->field_0x9a3 = 0;
            i_this->field_0x9a6 = TREG_S(8) + 15;
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
        0x828C,
        0x828D,
        0x828E,
    };

    if (i_this->mBossMode != 0) {
        for (int i = 0; i < 14; i++) {
            if (i_this->mpBossArmorParts[i] != NULL) {
                if (i_this->field_0x70c[i] == 0) {
                    if (i_this->mBossMode == 3) {
                        MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(ikki2_boss_part_idx[i]), *calc_mtx);
                    } else {
                        MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(boss_part_idx[i]), *calc_mtx);
                    }

                    i_this->mpBossArmorParts[i]->setBaseTRMtx(*calc_mtx);
                    sp98.set(0.0f, 0.0f, 0.0f);
                    MtxPosition(&sp98, &i_this->field_0x71c[i]);
                } else {
                    i_this->field_0x71c[i] += i_this->field_0x7c4[i];
                    i_this->field_0x7c4[i].y -= 3.0f;
                    i_this->field_0x86c[i].y += 0x900;
                    i_this->field_0x86c[i].x += 0xB00;

                    mDoMtx_stack_c::transS(i_this->field_0x71c[i].x, i_this->field_0x71c[i].y, i_this->field_0x71c[i].z);
                    mDoMtx_stack_c::YrotM(i_this->field_0x86c[i].y);
                    mDoMtx_stack_c::XrotM(i_this->field_0x86c[i].x);
                    f32 my_scale = l_HIO.field_0xc * (l_HIO.model_size * a_this->scale.x);
                    mDoMtx_stack_c::scaleM(my_scale, my_scale, my_scale);

                    i_this->mpBossArmorParts[i]->setBaseTRMtx(mDoMtx_stack_c::get());

                    for (int j = 0; j < 3; j++) {
                        i_this->field_0x8c0[i][j] = dComIfGp_particle_set(i_this->field_0x8c0[i][j], eno[j],
                                                                          &i_this->field_0x71c[i], NULL, NULL);
                    }

                    if (i_this->field_0x71c[i].y < a_this->current.pos.y - 200.0f) {
                        i_this->mpBossArmorParts[i] = NULL;
                    }
                }
            }
        }

        if (i_this->field_0x6a0 != 0) {
            if (i_this->field_0x6a0 == 1) {
                MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(YREG_S(3) + 15), *calc_mtx);
                sp98.set(0.0f, 0.0f, 0.0f);
                MtxPosition(&sp98, &i_this->field_0x6a4);
            } else {
                i_this->field_0x6a4 += i_this->field_0x6b0;
                i_this->field_0x6b0.y -= 5.0f;
                MtxTrans(i_this->field_0x6a4.x, i_this->field_0x6a4.y, i_this->field_0x6a4.z, 0);
                f32 fVar4 = l_HIO.field_0xc * ( l_HIO.model_size * a_this->scale.x);
                MtxScale(fVar4, fVar4, fVar4, 1);
                MtxTrans(BREG_F(5) + 80.0f, BREG_F(6) + 50.0f, BREG_F(7), 1);
                cMtx_YrotM(*calc_mtx, i_this->field_0x6be);
                cMtx_XrotM(*calc_mtx, 0x7FFF);
                cMtx_ZrotM(*calc_mtx, i_this->field_0x6c0);
                MtxTrans(-(BREG_F(5) + 80.0f), -(BREG_F(6) + 50.0f), -(BREG_F(7) + 0.0f), 1);
                i_this->field_0x6be += 0x200;
                i_this->field_0x6c0 += 0xF00;
            }

            i_this->mpMorfHornAnm->getModel()->setBaseTRMtx(*calc_mtx);
        }

        if (i_this->field_0x6c2 != 0) {
            i_this->field_0x6c2 = 0;
            if (i_this->field_0x680 == 0) {
                int iVar2 = i_this->mAnmID;
                if (iVar2 == e_rdb_class::BCK_RB_RRUN) {
                    s16 angle_diff = a_this->shape_angle.y - i_this->mPlayerAngleY;
                    if (angle_diff < 0) {
                        anm_init(i_this, e_rdb_class::BCK_RB_RPLAY_RUNR, 2.0f, 0, 1.0f);
                    } else {
                        anm_init(i_this, e_rdb_class::BCK_RB_RPLAY_RUNL, 2.0f, 0, 1.0f);
                    }

                    horn_anm_init(i_this, e_rdb_class::BCK_HORN_RPLAY_RUN, 2.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, e_rdb_class::BCK_RB_RPLAY_STOP, 2.0f, 0, 1.0f);
                    horn_anm_init(i_this, e_rdb_class::BCK_HORN_RPLAY_STOP, 2.0f, 0, 1.0f);
                }

                i_this->field_0x680 = iVar2;
            }
        }

        if (i_this->field_0x6a0 != 0) {
            i_this->mpMorfHornAnm->play(NULL, 0, 0);
            i_this->mpMorfHornAnm->modelCalc();
        }
    }

    if (i_this->mBossMode == 0 && i_this->field_0x9bc == 0) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(11), mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&spa4);
        sp98 = player->current.pos - spa4;
        s16 atan_val = cM_atan2s(sp98.x, sp98.z);
        cMtx_YrotS(*calc_mtx, atan_val);
        sp98.x = 0.0f;
        sp98.y = 0.0f;
        sp98.z = (BREG_F(12) + 40.0f) - 30.0f;
        MtxPosition(&sp98, &spb0);
        spb0 += spa4;
        a_this->setDownPos(&spb0);
    }

    cXyz spd4(a_this->eyePos);
    spd4.y += NREG_F(7) + 130.0f;
    a_this->setHeadLockPos(&spd4);
    demo_camera(i_this);
    i_this->field_0x5bb = 1;
    a_this->attention_info.flags |= fopAc_AttnFlag_UNK_0x200000;

    return 1;
}

static u8 jv_offset[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static int daE_RD_IsDelete(e_rd_class* i_this) {
    return 1;
}

static int daE_RD_Delete(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    dComIfG_resDelete(&i_this->mPhase, i_this->mResName);
    if (i_this->field_0x125c != 0) {
        hio_set = 0;
    }

    if (a_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static void ride_game_actor_set(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp70, i_pos;
    csXyz i_angle;

    OS_REPORT("E3 2005 ACTOR SET %d\n", i_this->mBossMode);
    static cXyz set_pos[6] = {
        cXyz(0.0f, 1500.0f, 2500.0f),
        cXyz(800.0f, 1500.0f, -4800.0f),
        cXyz(-650.0f, 1500.0f, -4500.0f),
        cXyz(1100.0f, 1500.0f, -5500.0f),
        cXyz(0.0f, 1500.0f, -5800.0f),
        cXyz(-1100.0f, 1500.0f, -5500.0f),
    };

    dBgS_GndChk gnd_chk;
    int iVar1 = 6;
    if (i_this->mBossMode >= 2) {
        iVar1 = 1;
    }

    for (int i = 0; i < iVar1; i++) {
        u32 i_parameters;
        if (i_this->mBossMode == 1) {
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            sp70 = set_pos[i];

            if (i > 0 && cDmr_SkipInfo == 0) {
                sp70.z -= 4000.0f;
            }

            MtxPosition(&sp70, &i_pos);
            i_pos += player->current.pos;
            gnd_chk.SetPos(&i_pos);
            i_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
            i_angle = player->shape_angle;
            i_angle.y += 0x4000;
            i_parameters = 0x80000005;
        } else if (i_this->mBossMode == 2) {
            i_pos.set(34789.0f, -290.0f, -36177.0f);
            i_angle.set(0, 0, 0);
            i_parameters = 0x80000007;
        } else if (i_this->mBossMode == 3) {
            i_pos.set(-93620.0f, -5750.0f, 49650.0f);
            i_angle.set(0, -0x8000, 0);
            i_parameters = 0x80000008;
        } else if (i_this->mBossMode == 4) {
            i_pos = a_this->home.pos;
            i_angle = a_this->home.angle;
            i_parameters = 0x80000009;
        }

        if (i == 0) {
            a_this->home.pos = i_pos;
            a_this->current.pos = a_this->home.pos;
            a_this->old.pos = a_this->home.pos;
        } else {
            i_angle = player->shape_angle;
            i_parameters = (i << 8 | 0x80000006);
        }

        fopAcM_create(PROC_E_WB, i_parameters, &i_pos, fopAcM_GetRoomNo(a_this), &i_angle, NULL, -1);
    }
}

static void coach_game_actor_set(e_rd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    csXyz i_angle;

    OS_REPORT("COATH GAME ACTOR SET \n");

    static cXyz set_pos[3] = {
        cXyz(-57464.0f, -8000.0f, 82137.0f),
        cXyz(-58006.0f, -8000.0f, 79302.0f),
        cXyz(-56848.0f, -8000.0f, 84472.0f),
    };

    dBgS_GndChk gnd_chk;
    i_angle.set(0, 0, 0);
    i_angle.y = player->shape_angle.y;

    for (int i = 0; i < NREG_S(7) + 3; i++) {
        fopAcM_create(PROC_E_WB, (i + 1) * 0x100 | 0x8000000a, &set_pos[i], fopAcM_GetRoomNo(a_this), &i_angle, NULL, -1);
    }
}

static s8 instanceCount;

static int useHeapInit(fopAc_ac_c* a_this) {
    e_rd_class* i_this = (e_rd_class*)a_this;
    J3DModel* model;
    J3DModelData* modelData;

    if (i_this->mBossMode != 0) {
        i_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_rdb", e_rdb_class::BMDR_RB), NULL, NULL,
                                                   (J3DAnmTransform*)dComIfG_getObjectRes("E_rdb", e_rdb_class::BCK_RB_RWAIT), 2, 1.0f,
                                                   0, -1, &i_this->mSound, 0x80000, 0x11000084);
        if (i_this->mpModelMorf == NULL || i_this->mpModelMorf->getModel() == NULL) {
            return 0;
        }

        model = i_this->mpModelMorf->getModel();
        model->setUserArea((uintptr_t)i_this);

        for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_B);
        }

        if (i_this->mBossMode == 1) {
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
            if (i_this->mBossMode == 3) {
                if (i < 2) {
                    modelData = (J3DModelData*)dComIfG_getObjectRes("E_rdb", ikki2_boss_part_bmd[i]);
                    JUT_ASSERT(10672, modelData != NULL);
                    i_this->mpBossArmorParts[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
                    if (i_this->mpBossArmorParts[i] == NULL) {
                        return 0;
                    }
                } else {
                    i_this->field_0x70c[i] = 1;
                }
            } else if (i_this->mBossMode == 1 || i == 13) {
                modelData = (J3DModelData*)dComIfG_getObjectRes("E_rdb", boss_part_bmd[i]);
                JUT_ASSERT(10687, modelData != NULL);
                i_this->mpBossArmorParts[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
                if (i_this->mpBossArmorParts[i] == NULL) {
                    return 0;
                }
            } else {
                i_this->field_0x70c[i] = 1;
            }
        }
    } else {
        i_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes(i_this->mResName, BMDR_RD), NULL, NULL,
                                                   (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, BCK_RD_WAIT01), 0, 1.0f,
                                                   0, -1, &i_this->mSound, 0x80000, 0x11000084);
        if (i_this->mpModelMorf == NULL || i_this->mpModelMorf->getModel() == NULL) {
            return 0;
        }

        model = i_this->mpModelMorf->getModel();
        model->setUserArea((uintptr_t)i_this);
        mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
        model->setBaseTRMtx(mDoMtx_stack_c::get());

        for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }

        if (i_this->field_0x5bc == 1) {
            if (boss != NULL) {
                modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, BMDR_RD_CLUBB);
            } else {
                modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, BMDR_RD_CLUB);
            }

            JUT_ASSERT(10762, modelData != NULL);

            i_this->field_0x694 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            if (i_this->field_0x694 == NULL) {
                return 0;
            }

            i_this->field_0x694->setBaseTRMtx(mDoMtx_stack_c::get());
        } else if (i_this->field_0x5bc >= 2) {
            i_this->mpMorfBowAnm = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes(i_this->mResName, BMDR_RD_BOW), NULL, NULL,
                                                       (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, BCK_RD_BOW_SHOOT), 0, 1.0f,
                                                       0, -1, NULL, 0x80000, 0x11000084);
            if (i_this->mpMorfBowAnm == NULL || i_this->mpMorfBowAnm->getModel() == NULL) {
                return 0;
            }

            model = i_this->mpMorfBowAnm->getModel();
            model->setUserArea((uintptr_t)i_this);
            model->setBaseTRMtx(mDoMtx_stack_c::get());

            for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
                if (i == 2 || i == 3) {
                    model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_bow);
                }
            }

            if (i_this->field_0x5bc == 4) {
                modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, BMDR_RD_BARROW);
            } else {
                modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, BMDR_RD_ARROW);
            }

            JUT_ASSERT(10810, modelData != NULL);

            i_this->field_0x694 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            if (i_this->field_0x694 == NULL) {
                return 0;
            }

            i_this->field_0x694->setBaseTRMtx(mDoMtx_stack_c::get());
        }

        modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, BMDR_RD_EYE);
        JUT_ASSERT(10823, modelData != NULL);

        for (u16 i = 0; i < 2; i++) {
            i_this->mpEyeModels[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            if (i_this->mpEyeModels[i] == NULL) {
                return 0;
            }
            i_this->mpEyeModels[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }

    return 1;
}

static cPhs__Step daE_RD_Create(fopAc_ac_c* a_this) {
    e_rd_class* i_this = (e_rd_class*)a_this;

    fopAcM_ct(a_this, e_rd_class);

    i_this->field_0x5b6 = fopAcM_GetParam(a_this);
    i_this->field_0x5b7 = i_this->field_0x5b6;
    i_this->field_0x5b8 = fopAcM_GetParam(a_this) >> 12 & 15;
    if (i_this->field_0x5b8 == 15) {
        i_this->field_0x5b8 = 0;
    }

    if (((i_this->field_0x5b6 == 4 || i_this->field_0x5b6 == 5) || i_this->field_0x5b6 == 11) || i_this->field_0x5b6 == 12) {
        if (i_this->field_0x5b6 == 4) {
            i_this->mBossMode = 1;
        } else if (i_this->field_0x5b6 == 5) {
            i_this->mBossMode = 2;
        } else if (i_this->field_0x5b6 == 11) {
            i_this->mBossMode = 3;
        } else if (i_this->field_0x5b6 == 12) {
            i_this->mBossMode = 4;
        }

        i_this->mResName = "E_rdb";
    } else {
        i_this->mResName = "E_RD";
    }

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->mPhase, i_this->mResName);
    if (phase == cPhs_COMPLEATE_e) {
        if (strcmp(dComIfGp_getStartStageName(), "F_SP124") == 0) {
            // Gerudo Desert
            data_80519200 = 124;
        } else {
            if (strcmp(dComIfGp_getStartStageName(), "F_SP118") == 0) {
                // Arbiter's Grounds Exterior
                data_80519200 = 118;
            } else {
                data_80519200 = 0;
            }
        }

        int swBit = fopAcM_GetParam(a_this) >> 24;
        if (swBit != 0xFF) {
            if (dComIfGs_isSwitch(swBit, fopAcM_GetRoomNo(a_this))) {
                return cPhs_ERROR_e;
            }
        }

        if (i_this->field_0x5b6 == 10) {
            coach_game_actor_set(i_this);
            return cPhs_ERROR_e;
        }

        if (i_this->mBossMode != 0) {
            boss = i_this;
            i_this->field_0x5b6 = 1;
        }

        i_this->field_0x5bc = fopAcM_GetParam(a_this) >> 8 & 15;
        if (i_this->field_0x5bc == 15) {
            i_this->field_0x5bc = 0;
        }

        if (i_this->field_0x5bc >= 4) {
            return cPhs_ERROR_e;
        }

        i_this->field_0x5ba = fopAcM_GetParam(a_this) >> 16;
        if (i_this->field_0x5ba == 0xFF) {
            i_this->field_0x5ba = 0;
        }

        if (i_this->field_0x5ba == 2) {
            fopAcM_setStageLayer(a_this);
        }

        i_this->field_0x5b9 = fopAcM_GetParam(a_this) >> 24;
        if (i_this->field_0x5b8 == 3 || i_this->field_0x5b8 == 4 || i_this->field_0x5b8 == 5 || i_this->field_0x5b8 == 6 || i_this->field_0x5b8 == 7) {
            if (i_this->field_0x5b8 != 5) {
                i_this->field_0x1295 = 1;
            }

            if (i_this->field_0x5b8 == 4) {
                i_this->field_0x1296 = 1;
            }

            if (i_this->field_0x5b8 == 6) {
                i_this->field_0x1299 = 1;
                i_this->mAction = ACTION_COMMANDER;
                i_this->mMode = 0;
                i_this->field_0x990[2] = 20;
                i_this->field_0x1296 = 1;
            } else if (i_this->field_0x5b8 == 7) {
                i_this->mAction = ACTION_BOW3;
                i_this->field_0x1296 = 1;
                i_this->field_0x998 = 20;
            } else {
                i_this->mAction = ACTION_BOW2;
            }

            fopAcM_setCullSizeFar(a_this, 30000.0f);
            i_this->field_0x5b8 = 3;
        } else if (i_this->field_0x5b8 == 10) {
            i_this->mAction = ACTION_BOW_IKKI;
            i_this->field_0x990[0] = 142;
        } else if (i_this->field_0x5b8 == 11) {
            fopAcM_create(PROC_E_RDY, fopAcM_GetParam(a_this), &a_this->home.pos, 
                          fopAcM_GetRoomNo(a_this), &a_this->home.angle, NULL, -1);
        }

        if ((a_this->home.angle.z & 0xFF) == 0 || (a_this->home.angle.z & 0xFF) == 0xFF) {
            i_this->field_0x980 = 100000.0f;
        } else {
            i_this->field_0x980 = (a_this->home.angle.z & 0xFF) * 100.0f;
        }

        i_this->field_0x12a0 = a_this->home.angle.z >> 8 & 0xFF;

        if (i_this->field_0x12a0 == 0xFF) {
            i_this->field_0x12a0 = 0;
        }

        fopAcM_OnStatus(a_this, fopAcM_STATUS_UNK_0x100);
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        if (i_this->field_0x5b6 == 6) {
            i_this->mAction = ACTION_YAGURA;
            i_this->field_0x99c = 30;
            S_find = 0;
            lbl_70_bss_AE = 0;

            if (fopAcM_GetRoomNo(a_this) == 0) {
                fopAcM_setCullSizeFar(a_this, 30000.0f);
            }
        } else if (i_this->field_0x5b6 == 7) {
            i_this->mAction = ACTION_NORMAL;
            if ((u32)(a_this->home.angle.x & 0xFF) != 0xFF) {
                i_this->mPath = dPath_GetRoomPath(a_this->home.angle.x & 0xFF, fopAcM_GetRoomNo(a_this));
                OS_REPORT("//////////////E_RD PPD %x!!\n", i_this->mPath);
                if (i_this->mPath == NULL) {
                    OS_REPORT("......RD   NONONONONONO PATH !!!!\n");
                    return cPhs_ERROR_e;
                }

                OS_REPORT("//////////////E_RD P0.y %f\n", i_this->mPath->m_points->m_position.y);
            }
        } else if (i_this->field_0x5b6 == 8) {
            i_this->mAction = ACTION_SLEEP;
            if (fopAcM_GetRoomNo(a_this) == 0) {
                fopAcM_setCullSizeFar(a_this, 30000.0f);
            }
        } else if (i_this->field_0x5b6 == 9) {
            i_this->mAction = ACTION_TAG;
            fopAcM_OffStatus(a_this, 0);
            a_this->attention_info.flags = 0;
        } else if (i_this->field_0x5b6 == 35) {
            i_this->mAction = ACTION_FIGHT_RUN;
            i_this->mMode = 0;
            i_this->field_0x99c = 200;
        }

        if (i_this->field_0x5bc == 1 && i_this->field_0x5b6 == 0) {
            i_this->mAction = ACTION_STAND;
            i_this->mMode = 0;
        }

        if (i_this->field_0x5b6 == 13 || i_this->field_0x5b6 == 14) {
            i_this->mAction = ACTION_REG;
            i_this->mMode = 0;
            i_this->field_0xafb = 1;
            fopAcM_OffStatus(a_this, fopAcM_STATUS_UNK_0x100);
        }

        if (i_this->field_0x5b6 > 2) {
            i_this->field_0x5b6 = 0;
        }

        a_this->shape_angle.x = 0;
        a_this->current.angle.x = 0;
        a_this->home.angle.x = 0;
        a_this->shape_angle.z = 0;
        a_this->current.angle.z = 0;
        a_this->home.angle.z = 0;

        u32 i_size;
        if (i_this->mBossMode == 1) {
            i_size = 0xAD60;
        } else if (i_this->mBossMode == 2) {
            i_size = 0x3C00;
        } else if (i_this->mBossMode == 3) {
            i_size = 0x4400;
        } else {
            i_size = 0x4FF0;
        }

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, i_size)) {
            OS_REPORT("//////////////E_RD SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (i_this->mBossMode != 0) {
            ride_game_actor_set(i_this);
        }

        if (hio_set == 0) {
            i_this->field_0x125c = 1;
            hio_set = 1;
            l_HIO.field_0x4 = -1;
        }

        fopAcM_SetMtx(a_this, i_this->mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(a_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(a_this, 200.0f, 200.0f, 200.0f);
        i_this->mObjAcch.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1,
                             &i_this->mAcchCir, fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(50.0f, 50.0f);

        a_this->health = 40;
        a_this->field_0x560 = 40;

        i_this->mStts.Init(150, 0, a_this);

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
            i_this->field_0xd58[i].Set(cc_sph_src);
            i_this->field_0xd58[i].SetStts(&i_this->mStts);

            if (i_this->mBossMode != 0) {
                i_this->field_0xd58[i].SetTgType(0xD8FAFD3F);
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
        i_this->field_0x1100.Set(at_sph_src);
        i_this->field_0x1100.SetStts(&i_this->mStts);

        i_this->field_0x970 = instanceCount | (u16)(cM_rndF(65535.0f)) & 0xFF00;
        instanceCount++;

        i_this->mSound.init(&a_this->current.pos, &a_this->eyePos, 3, 1);

        if (i_this->mBossMode != 0) {
            i_this->mSound.setEnemyName("E_rdb");
        } else {
            i_this->mSound.setEnemyName("E_rd");
        }

        i_this->mAtInfo.mPowerType = 2;
        i_this->mAtInfo.mpSound = &i_this->mSound;

        if (boss != NULL) {
            fopAcM_setCullSizeFar(a_this, 30000.0f);
        }

        i_this->field_0x9a0 = 5;

        if (!dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo())) {
            i_this->field_0x1298 = l_HIO.eye_polygon;

            if (data_80519200 != 0 || strcmp(dComIfGp_getStartStageName(), "F_SP115") == 0 ||
                (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0 && fopAcM_GetRoomNo(a_this) == 0)) {
                // Lake Hylia or Hyrule Field
                fopAcM_OffStatus(a_this, fopAcM_STATUS_UNK_0x4000);
            }
        } else if (i_this->mBossMode != 4) {
            fopAcM_OffStatus(a_this, fopAcM_STATUS_UNK_0x4000);
        }

        if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0 && fopAcM_GetRoomNo(a_this) == 0) {
            // Hyrule Field - Bridge of Eldin
            data_80519201 = 1;
        } else {
            data_80519201 = 0;
        }

        c_start = 1;
        daE_RD_Execute(i_this);
        c_start = 0;

        if (i_this->mAction == ACTION_TAG) {
            i_this->field_0xafb = 1;
            i_this->mSound.setEnemyName(NULL);
        }
    }

    return phase;
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
