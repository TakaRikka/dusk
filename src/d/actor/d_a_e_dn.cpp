/**
 * @file d_a_e_dn.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_dn.h"
#include "d/d_cc_d.h"
#include "d/d_bomb.h"
#include "f_op/f_op_kankyo_mng.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_horse.h"
#include "f_op/f_op_actor_enemy.h"
#include "Z2AudioLib/Z2Instances.h"

class daE_DN_HIO_c : public JORReflexible {
public:
    daE_DN_HIO_c();
    virtual ~daE_DN_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 no;
    /* 0x08 */ f32 model_size;          // 基本サイズ
    /* 0x0C */ f32 movement_speed;      // 移動速度
    /* 0x10 */ f32 dash_speed;          // 突進速度
    /* 0x14 */ f32 battle_init_range;   // 戦闘開始範囲
    /* 0x18 */ f32 attack_init_range;   // 攻撃開始範囲
    /* 0x1C */ s16 defense_pause_time;          // 防御静止間
    /* 0x1E */ s16 soul_disappear_time_weak;          // 魂抜間 弱
    /* 0x20 */ s16 soul_disappear_time_strong;          // 魂抜間 強
    /* 0x22 */ u8 field_0x22;
    /* 0x23 */ u8 invulnerable;         // 不死身
    /* 0x24 */ u8 no_learn;           // 学習なし
};

enum Joint {
    /* 0x00 */ JNT_WORLD_ROOT,
    /* 0x01 */ JNT_BACKBONE01,
    /* 0x02 */ JNT_BACKBONE02,
    /* 0x03 */ JNT_NECK01,
    /* 0x04 */ JNT_NECK02,
    /* 0x05 */ JNT_FIN,
    /* 0x06 */ JNT_HEAD,
    /* 0x07 */ JNT_JAW1,
    /* 0x08 */ JNT_JAW2,
    /* 0x09 */ JNT_TONGUE,
    /* 0x0A */ JNT_SHOULDERL,
    /* 0x0B */ JNT_ARML1,
    /* 0x0C */ JNT_ARML2,
    /* 0x0D */ JNT_HANDL,
    /* 0x0E */ JNT_FINGERL1,
    /* 0x0F */ JNT_FINGERL2,
    /* 0x10 */ JNT_SHOULDERR,
    /* 0x11 */ JNT_ARMR1,
    /* 0x12 */ JNT_ARMR2,
    /* 0x13 */ JNT_HANDR1,
    /* 0x14 */ JNT_FINGERR1,
    /* 0x15 */ JNT_FINGERR2,
    /* 0x16 */ JNT_WAIST,
    /* 0x17 */ JNT_LEGL1,
    /* 0x18 */ JNT_LEGL2,
    /* 0x19 */ JNT_LEGL3,
    /* 0x1A */ JNT_FOOTL,
    /* 0x1B */ JNT_LEGR1,
    /* 0x1C */ JNT_LEGR2,
    /* 0x1D */ JNT_LEGR3,
    /* 0x1E */ JNT_FOOTR,
    /* 0x1F */ JNT_TAIL01,
    /* 0x20 */ JNT_TAIL02,
    /* 0x21 */ JNT_TAIL03,
    /* 0x22 */ JNT_TAIL04,
};

enum Action {
    /* 0x00 */ ACTION_NORMAL       = 0,
    /* 0x03 */ ACTION_FIGHT_RUN    = 3,
    /* 0x04 */ ACTION_ATTACK_0     = 4,
    /* 0x05 */ ACTION_ATTACK       = 5,
    /* 0x06 */ ACTION_TAIL_ATTACK  = 6,
    /* 0x07 */ ACTION_GUARD        = 7,
    /* 0x08 */ ACTION_JUMP         = 8,
    /* 0x09 */ ACTION_STAY         = 9,
    /* 0x0A */ ACTION_OTOREAC      = 10,
    /* 0x0B */ ACTION_BOMB_ACTION  = 11,
    /* 0x0C */ ACTION_DRAWBACK     = 12,
    /* 0x0D */ ACTION_WOLFBITE     = 13,
    /* 0x14 */ ACTION_S_DAMAGE     = 20,
    /* 0x15 */ ACTION_DAMAGE       = 21,
    /* 0x17 */ ACTION_BACKDROP     = 23,
    /* 0x18 */ ACTION_WATER        = 24,
    /* 0x19 */ ACTION_GAKEJUMP     = 25,
    /* 0x3C */ ACTION_REG          = 60,
};

enum Animation {
    /* 0x04 */ ANM_ATTACK_TAIL_01 = 4,
    /* 0x05 */ ANM_ATTACK_TAIL_02,
    /* 0x06 */ ANM_ATTACK_TAIL_03,
    /* 0x07 */ ANM_ATTACK_00,
    /* 0x08 */ ANM_ATTACK_01,
    /* 0x09 */ ANM_BLOWNOFF,
    /* 0x0A */ ANM_BRUSH_LEFT_UP,
    /* 0x0B */ ANM_BRUSH_RIGHT_UP,
    /* 0x0C */ ANM_DAMAGE_W,
    /* 0x0D */ ANM_DIE_LEFT,
    /* 0x0E */ ANM_DIE_LEFT_UP,
    /* 0x0F */ ANM_DIE_RIGHT,
    /* 0x10 */ ANM_DIE_RIGHT_UP,
    /* 0x11 */ ANM_DIE_W,
    /* 0x12 */ ANM_DRAWBACK,
    /* 0x13 */ ANM_DROWNED_A,
    /* 0x14 */ ANM_DROWNED_B,
    /* 0x15 */ ANM_FIND,
    /* 0x16 */ ANM_GUARD,
    /* 0x17 */ ANM_HANGED,
    /* 0x18 */ ANM_HANGED_DAMAGE,
    /* 0x19 */ ANM_HANGED_BRUSH,
    /* 0x1A */ ANM_HANGED_WAIT,
    /* 0x1B */ ANM_JUMP_A,
    /* 0x1C */ ANM_JUMP_B,
    /* 0x1D */ ANM_JUMP_C,
    /* 0x1E */ ANM_RUN,
    /* 0x1F */ ANM_WAIT_01,
    /* 0x20 */ ANM_WAIT_02,
    /* 0x21 */ ANM_WAIT_03,
    /* 0x22 */ ANM_WAIT_04,
    /* 0x23 */ ANM_WALK,
};

daE_DN_HIO_c::daE_DN_HIO_c() {
    no = -1;
    model_size = 1.1f;
    movement_speed = 7.5f;
    dash_speed = 25.0f;
    battle_init_range = 300.0f;
    attack_init_range = 350.0f;
    defense_pause_time = 3;
    soul_disappear_time_weak = 40;
    soul_disappear_time_strong = 80;
    field_0x22 = 1;
    invulnerable = 0;
    no_learn = 0;
}

#if DEBUG
void daE_DN_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("  リザードマン", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("基本サイズ", &model_size, 0.0f, 5.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("移動速度", &movement_speed, 0.0f, 20.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("突進速度", &dash_speed, 0.0f, 40.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("戦闘開始範囲", &battle_init_range, 0.0f, 2000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("攻撃開始範囲", &attack_init_range, 0.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genCheckBox("不死身", &invulnerable, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genCheckBox("学習なし", &no_learn, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("魂抜間 弱", &soul_disappear_time_weak, 0, 100, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("魂抜間 強", &soul_disappear_time_strong, 0, 100, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("防御静止間", &defense_pause_time, 0, 20, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

static void dn_disappear(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz work, pos;

    MTXCopy(i_this->anm_p->getModel()->getAnmMtx(JNT_BACKBONE02), *calc_mtx);
    work.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&work, &pos);

    fopAcM_createDisappear(actor, &pos, 13, 0, 21);
    fopAcM_delete(actor);

    int swBit = (fopAcM_GetParam(actor) & 0xFF000000) >> 24;
    if (swBit != 0xFF) {
        dComIfGs_onSwitch(swBit, fopAcM_GetRoomNo(actor));
    }
}

static void anm_init(e_dn_class* i_this, int i_anm_id, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->anm_p->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_dn", i_anm_id), i_attr, i_morf,
                                i_rate, 0.0f, -1.0f);
    i_this->anm_no = i_anm_id;
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        int jointNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_dn_class* i_this = (e_dn_class*)model->getUserArea();
        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);

            if (jointNo != JNT_WORLD_ROOT) {
                if (jointNo == JNT_WAIST) {
                    cMtx_YrotM(*calc_mtx, (s16)i_this->field_0x830.y);
                    cMtx_ZrotM(*calc_mtx, i_this->jnt_waist_z_rot + (-i_this->field_0x762[2].x + i_this->field_0x830.x));
                } else if (jointNo == JNT_LEGL1) {
                    cMtx_YrotM(*calc_mtx, i_this->jnt_legl1_y_rot);
                } else if (jointNo == JNT_LEGL2) {
                    cMtx_ZrotM(*calc_mtx, i_this->field_0x7fc[0]);
                } else if (jointNo == JNT_LEGR1) {
                    cMtx_YrotM(*calc_mtx, i_this->field_0x7fc[1]);
                } else if (jointNo == JNT_LEGR2) {
                    cMtx_ZrotM(*calc_mtx, i_this->field_0x7fc[2]);
                } else if (jointNo == JNT_BACKBONE02) {
                    cMtx_YrotM(*calc_mtx, (i_this->field_0x6fa / 2) + (-i_this->field_0x6f6 + i_this->field_0x762[1].y));
                    cMtx_ZrotM(*calc_mtx, i_this->field_0x762[1].x + i_this->field_0x7ec[1]);
                } else if (jointNo == JNT_NECK01 || jointNo == JNT_NECK02) {
                    cMtx_YrotM(*calc_mtx, i_this->field_0x762[0].y + (-i_this->field_0x6f6 + i_this->field_0x6fa));
                    cMtx_ZrotM(*calc_mtx, i_this->field_0x7ec[0] + (-i_this->field_0x6f8 + i_this->field_0x762[0].x));
                } else if (jointNo == JNT_ARML1) {
                    cMtx_ZrotM(*calc_mtx, i_this->field_0x762[3].y + i_this->jnt_arml1_z_rot_offset);
                } else if (jointNo == JNT_ARML2) {
                    cMtx_ZrotM(*calc_mtx, i_this->field_0x762[4].y + i_this->jnt_arml2_z_rot_offset);
                } else if (jointNo == JNT_ARMR1) {
                    cMtx_ZrotM(*calc_mtx, i_this->field_0x762[5].y + i_this->jnt_armr1_z_rot_offset);
                } else if (jointNo == JNT_ARMR2) {
                    cMtx_ZrotM(*calc_mtx, i_this->field_0x762[6].y + i_this->jnt_armr2_z_rot_offset);
                } else if (jointNo >= JNT_TAIL01 && jointNo <= JNT_TAIL04) {
                    cMtx_YrotM(*calc_mtx, i_this->field_0x80e[jointNo - 31].y + i_this->jnt_tail_y_rot_offset);
                    cMtx_ZrotM(*calc_mtx, (s16)i_this->field_0x80e[jointNo - 31].x);
                }
            }

            if (i_this->death_jnt_rot != 0) {
                if ((jointNo & 1) != 0) {
                    cMtx_YrotM(*calc_mtx, i_this->death_jnt_rot);
                } else if ((jointNo & 2) != 0) {
                    cMtx_XrotM(*calc_mtx, i_this->death_jnt_rot);
                } else if ((jointNo & 4) != 0) {
                    cMtx_ZrotM(*calc_mtx, i_this->death_jnt_rot);
                }
            }

            model->setAnmMtx(jointNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            if (jointNo == TREG_S(0)) {
                MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
                cMtx_YrotM(*calc_mtx, TREG_S(1));
                cMtx_XrotM(*calc_mtx, TREG_S(2));
                cMtx_ZrotM(*calc_mtx, TREG_S(3));
                model->setAnmMtx(jointNo, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }

            if (jointNo == TREG_S(4)) {
                MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
                cMtx_YrotM(*calc_mtx, TREG_S(5));
                cMtx_XrotM(*calc_mtx, TREG_S(6));
                cMtx_ZrotM(*calc_mtx, TREG_S(7));
                model->setAnmMtx(jointNo, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static int daE_DN_Draw(e_dn_class* i_this) {
    if (i_this->status != 0) {
        return 1;
    }

    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;

    J3DModel* model = i_this->anm_p->getModel();
    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);

    if (i_this->death_flag != 0) {
        J3DModelData* modelData = model->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* matNode_p = modelData->getMaterialNodePointer(i);
            matNode_p->getTevColor(0)->r = i_this->color;
            matNode_p->getTevColor(0)->g = i_this->color;
            matNode_p->getTevColor(0)->b = i_this->color;
        }
    }

    i_this->anm_p->entryDL();

    if (i_this->death_flag != 0) {
        J3DModelData* modelData = model->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* matNode_p = modelData->getMaterialNodePointer(i);
            matNode_p->getTevColor(0)->r = 0;
            matNode_p->getTevColor(0)->g = 0;
            matNode_p->getTevColor(0)->b = 0;
        }
    }

    g_env_light.setLightTevColorType_MAJI(i_this->knife_model, &actor->tevStr);
    mDoExt_modelUpdateDL(i_this->knife_model);

    if (i_this->skull_model != NULL) {
        g_env_light.setLightTevColorType_MAJI(i_this->skull_model, &actor->tevStr);
        mDoExt_modelUpdateDL(i_this->skull_model);
    }

    g_env_light.setLightTevColorType_MAJI(i_this->tate_model, &actor->tevStr);
    mDoExt_modelUpdateDL(i_this->tate_model);

    cXyz shadow_pos;
    shadow_pos.set(actor->current.pos.x, actor->current.pos.y + 50.0f + BREG_F(18), actor->current.pos.z);
    i_this->shadow_key = dComIfGd_setShadow(i_this->shadow_key, 1, model, &shadow_pos, BREG_F(19) + 950.0f, 0.0f, 
                                            actor->current.pos.y, i_this->objacch.GetGroundH(), 
                                            i_this->objacch.m_gnd, &i_this->actor.tevStr, 0, 1.0f, 
                                            dDlst_shadowControl_c::getSimpleTex());
    dComIfGd_addRealShadow(i_this->shadow_key, i_this->knife_model);
    if (i_this->skull_model != NULL) {
        dComIfGd_addRealShadow(i_this->shadow_key, i_this->skull_model);
    }
    dComIfGd_addRealShadow(i_this->shadow_key, i_this->tate_model);
    
    return 1;
}

static BOOL other_bg_check(e_dn_class* i_this, fopAc_ac_c* i_actor) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz unused, start, end;

    end = i_actor->current.pos;
    end.y += 100.0f;
    start = actor->current.pos;
    start.y = actor->eyePos.y;
    lin_chk.Set(&start, &end, actor);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return TRUE;
    }

    return FALSE;
}

static BOOL other_bg_check2(e_dn_class* i_this, cXyz* i_pos) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz unused, start, end;

    end = *i_pos;
    end.y += 100.0f;
    start = actor->current.pos;
    start.y = actor->eyePos.y;

    lin_chk.Set(&start, &end, actor);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return TRUE;
    }

    return FALSE;
}

u8 hio_set;

daE_DN_HIO_c l_HIO;

fopAc_ac_c* target_info[10];

int target_info_count;

static void* s_b_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && dBomb_c::checkBombActor((fopAc_ac_c*)i_actor) && !((dBomb_c*)i_actor)->checkStateExplode() && target_info_count < 10) {
        target_info[target_info_count] = (fopAc_ac_c*)i_actor;
        target_info_count++;
    }

    return NULL;
}

static dBomb_c* search_bomb(e_dn_class* i_this, int param_2) {
    if ((i_this->learn & 0x10000000) == 0) {
        return NULL;
    }

    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    dBomb_c* bomb;

    target_info_count = 0;
    for (int i = 0; i < 10; i++) {
        target_info[i] = NULL;
    }

    fpcM_Search(s_b_sub, i_this);

    f32 threshold = 50.0f;
    if (target_info_count != 0) {
        cXyz work, bomb_dist, sp5c;
        for (int i = 0; i < target_info_count;) {
            bomb = (dBomb_c*)target_info[i];
            work.x = bomb->current.pos.x - actor->eyePos.x;
            work.y = 50.0f + bomb->current.pos.y - actor->eyePos.y;
            work.z = bomb->current.pos.z - actor->eyePos.z;
            bomb_dist.x = bomb->current.pos.x - actor->current.pos.x;
            bomb_dist.y = bomb->current.pos.y - actor->current.pos.y;
            bomb_dist.z = bomb->current.pos.z - actor->current.pos.z;

            if (work.abs() < threshold && !(bomb_dist.abs() > i_this->pl_dir + 30.0f) && (!other_bg_check(i_this, bomb) || !param_2)) {
                if (param_2) {
                    f32 absVal = fabsf(50.0f + bomb->current.pos.y - actor->eyePos.y);
                    if (absVal <= 300.0f) {
                        s16 angle = actor->shape_angle.y - cM_atan2s(work.x, work.z);
                        if (angle < 0) {
                            angle = -1 * angle;
                        }

                        if ((u16)angle < 20000) { // ~110°
                            return bomb;
                        }

                        cMtx_YrotS(*calc_mtx, -actor->shape_angle.y);
                        MtxPosition(&work, &sp5c);

                        if (fabsf(sp5c.x) < 500.0f && fabsf(sp5c.y) < 300.0f && sp5c.z > -125.0f && sp5c.z < 500.0f) {
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
                threshold += 50.0f;
                if (threshold > 1500.0f) {
                    return NULL;
                }
            }
        }
    } else {
        return NULL;
    }
    
    return NULL;
}

static dBomb_c* bomb_view_check(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    return search_bomb(i_this, TRUE);
}

static dBomb_c* bomb_check(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    return search_bomb(i_this, FALSE);
}

static BOOL player_way_check(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    s16 way = actor->shape_angle.y - player->shape_angle.y;
    if (way < 0) {
        way = way * -1;
    }

    if ((u32)((s16)way & 0xFFFF) < 0x6000) {
        return FALSE;
    }

    return TRUE;
}

static int pl_check(e_dn_class* i_this, f32 search_area, s16 search_angle) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (i_this->pl_dir < search_area) {
        s16 angle = actor->shape_angle.y - i_this->search_angle_y;
        if (angle < search_angle && angle > (s16)-search_angle && !other_bg_check(i_this, player)) {
            return 1;
        }
    }
        
    for (int i = 0; i <= 2; i++) {
        if (i_this->cc_sph[i].ChkCoHit() != 0) {
            if (player == dCc_GetAc(i_this->cc_sph[i].GetCoHitObj()->GetAc())) {
                return 2;
            }
        }
    }
    
    return 0;
}

static BOOL move_gake_check(e_dn_class* i_this, f32 param_2, s8 param_3) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    dBgS_GndChk gnd_chk;
    cXyz work, pos;

    cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
    work.x = 0.0f;
    work.y = 100.0f;
    if (param_3 != 0) {
        work.z = -param_2;
    } else {
        work.z = param_2;
    }
    MtxPosition(&work, &pos);
    pos += actor->current.pos;
    gnd_chk.SetPos(&pos);

    if (actor->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 300.0f) {
        return TRUE;
    }

    return FALSE;
}

static BOOL jump_pos_check(e_dn_class* i_this, cXyz* i_pos) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    dBgS_GndChk gnd_chk;
    cXyz work, new_pos;
    f32 z_offset = KREG_F(16) + 300.0f;

    cMtx_YrotS(*calc_mtx, i_this->search_angle_y);
    work.x = 0.0f;
    work.y = 200.0f;

    int i = 0;
    while (i < KREG_S(7) + 10) {
        work.z = z_offset;
        MtxPosition(&work, &new_pos);
        new_pos += actor->current.pos;
        gnd_chk.SetPos(&new_pos);
        new_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

        if ((actor->current.pos.y - new_pos.y) < 200.0f) {
            *i_pos = new_pos;
            work.z += KREG_F(18) + 200.0f;
            MtxPosition(&work, &new_pos);
            new_pos += actor->current.pos;
            gnd_chk.SetPos(&new_pos);
            new_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

            if ((actor->current.pos.y - new_pos.y) < 200.0f) {
                *i_pos = new_pos;
            }

            return TRUE;
        }

        z_offset += 50.0f;
        i++;
    }

    return FALSE;
}

static void e_dn_stay(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;

    i_this->field_0x6f4 = 5;
    switch (i_this->mode) {
        case 0:
            anm_init(i_this, ANM_WAIT_01, 10.0f, J3DFrameCtrl::EMode_LOOP, cM_rndF(0.1f) + 0.9f);
            i_this->mode = 1;
            // fallthrough
        case 1:
            if (fopAcM_otoCheck(actor, 2000.0f) || daPy_getPlayerActorClass()->checkWolfBark()) {
                SND_INFLUENCE* sound = dKy_Sound_get();
                cXyz posDelta = sound->position - actor->current.pos;
                s16 angle = cM_atan2s(posDelta.y, JMAFastSqrt(posDelta.x * posDelta.x + posDelta.z * posDelta.z));
                if (angle > 0x1000) { // 22.5°
                    i_this->mode = 5;
                } else {
                    i_this->mode = 2;
                }

                i_this->timer[0] = cM_rndF(10.0f) + 10.0f;
                i_this->find_pos = sound->position;
            }
            break;

        case 2:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, ANM_FIND, 3.0f, J3DFrameCtrl::EMode_NONE, 1.5f);
                i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_FIND, -1);
                i_this->mode = 3;
            }
            break;

        case 3:
            if (i_this->anm_p->isStop()) {
                if (other_bg_check2(i_this, &i_this->find_pos)) {
                    i_this->timer[0] = cM_rndF(10.0f) + 20.0f;
                    i_this->mode = 4;
                } else {
                    i_this->action = ACTION_OTOREAC;
                    i_this->mode = 0;
                }
            }
            break;

        case 4:
            if (i_this->timer[0] == 0) {
                i_this->mode = 10;
                i_this->timer[0] = cM_rndF(10.0f) + 30.0f;
                anm_init(i_this, ANM_WAIT_03, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.5f);
            }
            break;

        case 5:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, ANM_FIND, 3.0f, J3DFrameCtrl::EMode_NONE, 1.5f);
                i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_FIND, -1);
                i_this->mode = 6;
            }
            break;

        case 6:
            if (i_this->anm_p->isStop()) {
                i_this->timer[0] = cM_rndF(10.0f) + 20.0f;
                i_this->mode = 7;
            }
            break;

        case 7:
            if (i_this->timer[0] == 0) {
                i_this->mode = 10;
                i_this->timer[0] = cM_rndF(40.0f) + 50.0f;
                anm_init(i_this, ANM_WAIT_04, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.3f);
                i_this->anm_p->setFrame(cM_rndF(75.0f));
            }
            break;

        case 8:
            if (i_this->anm_p->isStop()) {
                i_this->mode = 0;
            }
            break;

        case 10:
            if (i_this->timer[0] == 0) {
                i_this->mode = 0;
            }
    }

    if ((i_this->frame_counter & 15) == 0) { // Every 16 frames
        if (pl_check(i_this, i_this->pl_range, 0x4000) != 0) { // 90°
            i_this->action = ACTION_FIGHT_RUN;
            i_this->mode = -10;
            i_this->timer[0] = 60;
        } else if (bomb_view_check(i_this) != NULL) {
            i_this->action = ACTION_BOMB_ACTION;
            i_this->mode = 0;
        }
    }
}

static void e_dn_otoreac(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz work, unused;
    f32 dash_speed = 0.0f;
    i_this->field_0x6f4 = 2;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, ANM_WAIT_01, 10.0f, J3DFrameCtrl::EMode_LOOP, cM_rndF(0.2f) + 1.5f);
            i_this->timer[0] = cM_rndF(10.0f) + 20.0f;
            i_this->mode = 1;
            // fallthrough
        case 1:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, ANM_RUN, 5.0f, J3DFrameCtrl::EMode_LOOP, cM_rndFX(0.05f) + 1.0f);
                i_this->mode = 2;
            }
            break;

        case 2:
            dash_speed = l_HIO.dash_speed;
            if ((i_this->frame_counter & 3) == 0) { // Every 4 frames
                if ((i_this->frame_counter & 4) != 0) { // Every 5 frames
                    i_this->set_smoke_flag = 1;
                } else {
                    i_this->set_smoke_flag = 2;
                }
            }

            work = i_this->find_pos - actor->current.pos;
            cLib_addCalcAngleS2(&actor->current.angle.y, cM_atan2s(work.x, work.z), 2, 0xC00);
            if (work.abs() < 250.0f || i_this->objacch.ChkWallHit() != 0) {
                i_this->action = ACTION_NORMAL;
                i_this->mode = 0;
                i_this->timer[0] = cM_rndF(50.0f) + 50.0f;
                anm_init(i_this, ANM_WAIT_03, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;


    }

    cLib_addCalc2(&actor->speedF, dash_speed, 1.0f, 5.0f);
    if ((i_this->frame_counter & 15) == 0) { // Every 16 frames
        if (pl_check(i_this, i_this->pl_range, 0x4000) != 0) { // 90°
            i_this->action = ACTION_FIGHT_RUN;
            i_this->mode = -10;
            i_this->timer[0] = 60;
        } else if (bomb_view_check(i_this) != NULL) {
            i_this->action = ACTION_BOMB_ACTION;
            i_this->mode = 0;
        }
    }
}

static void e_dn_bomb_action(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    s16 angle;
    cXyz work, bomb_pos_target;
    dBomb_c* bomb = bomb_check(i_this);
    if (bomb == NULL) {
        i_this->action = ACTION_FIGHT_RUN;
        i_this->mode = 0;
        return;
    }

    work.x = bomb->current.pos.x - actor->current.pos.x;
    work.z = bomb->current.pos.z - actor->current.pos.z;
    s16 target_angle_y = cM_atan2s(work.x, work.z);
    f32 dash_speed = 0.0f;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, ANM_FIND, 3.0f, J3DFrameCtrl::EMode_NONE, 1.5f);
            i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_FIND_BOMB, -1);
            i_this->mode = 1;
            break;

        case 1:
            if (i_this->anm_p->isStop()) {
                i_this->mode = 2;
            }
            break;

        case 2:
            i_this->mode = 3;
            anm_init(i_this, ANM_WAIT_01, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->timer[1] = cM_rndF(10.0f) + 10.0f;
            // fallthrough
        case 3:
            if (i_this->timer[1] == 0 && bomb->speedF < 0.1f) {
                i_this->mode = 4;
                anm_init(i_this, ANM_RUN, 5.0f, J3DFrameCtrl::EMode_LOOP, cM_rndFX(0.05f) + 1.0f);
            }
            break;

        case 4:
            if (i_this->arg1 == 1 && bomb->speedF < 0.1f && !bomb->checkStateCarry() && JMAFastSqrt(work.x * work.x + work.z * work.z) < 250.0f) {
                angle = i_this->search_angle_y - actor->shape_angle.y;
                if (angle < 0x4000 && angle > -0x4000) { // ±90°
                    i_this->mode = 10;
                    break;
                }
            }

            target_angle_y += 0x8000; // 180°
            dash_speed = l_HIO.dash_speed;
            if (JMAFastSqrt(work.x * work.x + work.z * work.z) > 600.0f) {
                i_this->mode = 5;
                anm_init(i_this, ANM_WAIT_01, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            
            break;

        case 5:
            i_this->sound.startCreatureSoundLevel(Z2SE_EN_DN_V_LOOK_BOMB, 0, -1);
            target_angle_y = i_this->search_angle_y;
            if (JMAFastSqrt(work.x * work.x + work.z * work.z) < 500.0f) {
                i_this->mode = 2;
            }
            break;

        case 10:
            i_this->field_0x6f2 = 0;
            dash_speed = l_HIO.dash_speed;
            if (JMAFastSqrt(work.x * work.x + work.z * work.z) < 100.0f) {
                i_this->timer[0] = 20;
                i_this->mode = 11;
                anm_init(i_this, ANM_RUN, 5.0f, J3DFrameCtrl::EMode_NONE, cM_rndFX(0.05f) + 1.0f);
                i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_KICK_BOMB, -1);
            }

            if (bomb->speedF > 0.1f) {
                i_this->mode = 2;
            }
            break;

        case 11:
            i_this->field_0x6f2 = 0;
            target_angle_y = i_this->search_angle_y;

            if (i_this->timer[0] >= 10) {
                cMtx_YrotS(*calc_mtx, target_angle_y);
                work.x = TREG_F(15) + -30.0f;
                work.y = 0.0f;
                work.z = TREG_F(16) + 60.0f;
                MtxPosition(&work, &bomb_pos_target);
                bomb_pos_target += actor->current.pos;

                cLib_addCalc2(&bomb->current.pos.x, bomb_pos_target.x, 1.0f, 15.0f);
                cLib_addCalc2(&bomb->current.pos.z, bomb_pos_target.z, 1.0f, 15.0f);

                if (i_this->timer[0] == 10) {
                    bomb->speedF = 30.0f;
                    bomb->speed.y = 30.0f;
                    bomb->current.angle.y = actor->shape_angle.y;
                }
            }

            if (i_this->timer[0] == 0) {
                anm_init(i_this, ANM_WAIT_01, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 5;
            }
    }

    cLib_addCalc2(&actor->speedF, dash_speed, 1.0f, 5.0f);
    cLib_addCalcAngleS2(&actor->current.angle.y, target_angle_y, 4, 0x1000);
    i_this->field_0x6f4 = 2;
    i_this->find_pos = bomb->current.pos;
}

static void e_dn_normal(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp44, sp50;
    f32 movement_speed = 0.0f;
    s16 search_angle, angle, angle_delta;
    search_angle = 0x4000; // 90°

    switch (i_this->mode) {
        case 0:
        case 1:
        case 2:
            if (i_this->timer[0] == 0) {
                if (fopAcM_wayBgCheck(actor, 200.0f, 50.0f) || move_gake_check(i_this, 200.0f, 0)) {
                    angle = cM_rndFX(10000.0f) + 32768.0f;
                    i_this->timer[1] = 40;
                } else {
                    sp50.x = actor->home.pos.x + cM_rndFX(600.0f);
                    sp50.y = actor->home.pos.y;
                    sp50.z = actor->home.pos.z + cM_rndFX(600.0f);
                    sp44 = sp50 - actor->current.pos;

                    angle = cM_atan2s(sp44.x, sp44.z) - actor->current.angle.y;
                    if (angle > 0x3000) { // 67.5°
                        angle = 0x3000;
                    } else if (angle < -0x3000) {
                        angle = -0x3000;
                    }
                }

                i_this->cur_angle_y_target = actor->current.angle.y + angle;
                anm_init(i_this, ANM_WALK, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 3;
                i_this->timer[0] = cM_rndF(100.0f) + 100.0f;
            } else {
                search_angle = 0x7000; // 157.5°
            }
            break;

        case 3:
            movement_speed = l_HIO.movement_speed;
            angle = actor->current.angle.y;
            cLib_addCalcAngleS2(&actor->current.angle.y, i_this->cur_angle_y_target, 2, 0x400);

            /* If the actor is facing roughly 22.5° from the target y, stop moving */
            angle_delta = actor->current.angle.y - i_this->cur_angle_y_target;
            if (angle_delta > 0x1000 || angle_delta < -0x1000) {
                movement_speed = 0.0f;
            }

            /* Calculate how much the actor turned */
            angle -= actor->current.angle.y;
            /* Scale turn strength */
            angle *= (s16)(YREG_S(5) + 2);

            /* Ensure the targeted angle is no more than ±22.5° */
            s16 max_turn = YREG_S(6) + 0x1000;
            if (angle > max_turn) {
                angle = max_turn;
            } else if (angle < (s16)-max_turn) {
                angle = -max_turn;
            }

            cLib_addCalcAngleS2(&i_this->jnt_tail_y_rot_offset, angle, 2, 0x400);

            if (i_this->timer[0] == 0 || 
                (i_this->timer[1] == 0 && (fopAcM_wayBgCheck(actor, 200.0f, 50.0f) || move_gake_check(i_this, 200.0f, 0)))) {
                i_this->mode = 2;
                i_this->timer[0] = cM_rndF(100.0f) + 50.0f;
                anm_init(i_this, ANM_WAIT_03, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;
    }

    cLib_addCalc2(&actor->speedF, movement_speed, 1.0f, 3.0f);
    BOOL oto_check = fopAcM_otoCheck(actor, 2000.0f) | daPy_getPlayerActorClass()->checkWolfBark();
    /* Run the following checks every 16 frames or immediately if the above sound check is true */
    if ((i_this->frame_counter & 15) == 0 || oto_check) {
        if (oto_check || pl_check(i_this, i_this->pl_range, search_angle) != 0) {
            i_this->action = ACTION_FIGHT_RUN;
            i_this->mode = -10;
            i_this->timer[0] = 60;
        }

        if (bomb_view_check(i_this) != NULL) {
            i_this->action = ACTION_BOMB_ACTION;
            i_this->mode = 0;
        }
    }
}

static void e_dn_drawback(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, ANM_DRAWBACK, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 1;
            actor->speedF = KREG_F(4) + -5.0f;
            i_this->invulnerability_timer = 10;
            i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_DRAWBACK, -1);
            i_this->cur_angle_y_target = player->shape_angle.y + 0x8000; // Target current angle towards player
            break;

        case 1:
            i_this->actor.onHeadLockFlg();
            cLib_addCalcAngleS2(&actor->current.angle.y, i_this->cur_angle_y_target, 4, 0x400);
            cLib_addCalc0(&actor->speedF, 1.0f, KREG_F(5) + 0.1f);

            if (i_this->anm_p->isStop()) {
                i_this->action = ACTION_FIGHT_RUN;
                i_this->mode = 0;
                i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_AWAKE, -1);
            }
            break;
    }

    if (move_gake_check(i_this, KREG_F(9) + 50.0f, 1)) {
        i_this->action = ACTION_BACKDROP;
        i_this->mode = 0;
    }
}

static void e_dn_wolfbite(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    i_this->invulnerability_timer = 10;
    cLib_addCalc0(&actor->speedF, 1.0f, 2.0f);

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, ANM_HANGED, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 1;
            i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_DRAWBACK, -1);
            actor->health -= (s16)10;
            break;

        case 1:
            if (i_this->anm_p->isStop()) {
                anm_init(i_this, ANM_HANGED_WAIT, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 2;
            }
            break;

        case 2:
            if (i_this->anm_no == ANM_HANGED_DAMAGE) {
                if (i_this->anm_p->isStop()) {
                    anm_init(i_this, ANM_HANGED_WAIT, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                }
            }

            if (actor->health <= 0 || actor->checkWolfBiteDamage()) {
                actor->offWolfBiteDamage();
                anm_init(i_this, ANM_HANGED_DAMAGE, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                actor->health -= (s16)10;
                if (actor->health <= 0) {
                    player->offWolfEnemyHangBite();
                    i_this->field_0x750 = (actor->shape_angle.y - 0x8000) - player->shape_angle.y;
                    i_this->field_0x74c = 150.0f;
                    i_this->action = ACTION_DAMAGE;
                    i_this->mode = 0;
                    i_this->field_0x724.y = player->shape_angle.y + 0x8000;
                    actor->speed.y = 0.0f;
                    i_this->field_0x704 = -1.0f;
                    i_this->unk_timer_4 = DREG_S(2) + 50;
                    i_this->sound.startCollisionSE(Z2SE_HIT_WOLFBITE, 32);
                    i_this->field_0x710 = 1;
                    i_this->invulnerability_timer = 1000;
                    i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_DEATH, -1);
                    return;
                }

                i_this->sound.startCollisionSE(Z2SE_HIT_WOLFBITE, 30);
                i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_DAMAGE, -1);
            }

            if (!player->checkWolfEnemyHangBiteOwn(actor)) {
                anm_init(i_this, ANM_HANGED_BRUSH, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 3;
            }
            break;

        case 3:
            if (i_this->anm_p->isStop()) {
                i_this->action = ACTION_FIGHT_RUN;
                i_this->mode = 0;
                i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_AWAKE, -1);
            }
    }
}

static int way_check(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    s16 sVar1 = actor->shape_angle.y - i_this->search_angle_y;
    if (sVar1 <= 0x4000 && sVar1 >= -0x4000) {
        return 0;
    }

    if (sVar1 < 0) {
        return 1;
    }

    return 2;
}

int learn_check;

static void* shot_s_sub(void* i_actor, void* i_data) {
    e_dn_class* i_this = (e_dn_class*)i_data;
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    if ((fopAcM_IsActor(i_actor) && (learn_check & 0x80000000) != 0 && fopAcM_GetName(i_actor) == PROC_ARROW && (fopAcM_GetParam(i_actor) == 1 || 
        fopAcM_GetParam(i_actor) == 2))) {
        cXyz sp28(actor->current.pos - ((fopAc_ac_c*)i_actor)->current.pos);
        if (sp28.abs() < 1000.0f) {
            return i_actor;
        }
    }

    return NULL;
}

static void e_dn_fight_run(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp4c, sp58;
    f32 fVar1 = 0.0f;
    int frame = i_this->anm_p->getFrame();
    BOOL wayCheck;
    s8 sVar4 = 1;

    if (pl_check(i_this, i_this->pl_range + 50.0f, 0x7FFF) == 0 && i_this->timer[0] == 0) {
        if (i_this->unk_timer_1 == 0) {
            sVar4 = 0;
        }
    } else {
        i_this->unk_timer_1 = 40;
    }

    switch (i_this->mode) {
        case -10:
            anm_init(i_this, ANM_WAIT_01, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->timer[1] = cM_rndF(10.0f) + 15.0f;
            i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_FIND, -1);
            i_this->mode = -9;
            break;

        case -9:
            cLib_addCalc0(&actor->speedF, 1.0f, 5.0f);
            if (i_this->timer[1] == 0) {
                i_this->mode = 0;
            }
            break;

        case 0:
            anm_init(i_this, ANM_RUN, 5.0f, J3DFrameCtrl::EMode_LOOP, cM_rndFX(0.05f) + 1.0f);
            i_this->mode = 1;
            i_this->unk_counter_1 = 0;
            // fallthrough
        case 1:
            fVar1 = l_HIO.dash_speed;
            if ((i_this->frame_counter & 3) == 0) { // Every 4 frames
                if ((i_this->frame_counter & 4) != 0) { // Every 5 frames
                    i_this->set_smoke_flag = 1;
                } else {
                    i_this->set_smoke_flag = 2;
                }
            }

            if (i_this->objacch.ChkWallHit() != 0 && !fopAcM_wayBgCheck(actor, 200.0f, 120.0f)) {
                i_this->unk_counter_1++;
            } else if (i_this->pl_dir < l_HIO.battle_init_range) {
                anm_init(i_this, ANM_WAIT_02, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 2;
                i_this->timer[2] = JREG_F(1) + (cM_rndF(30.0f) + 30.0f);
            }

            if (i_this->unk_counter_1 > 20) {
                i_this->action = ACTION_JUMP;
                i_this->mode = 0;
                return;
            }

            if (move_gake_check(i_this, KREG_F(6) + 50.0f, 0)) {
                i_this->action = ACTION_GAKEJUMP;
                i_this->mode = 0;
                i_this->invulnerability_timer = 100;
                actor->speedF = 0.0f;
                return;
            }
            break;

        case 2:
            i_this->snap_angle_y_flag = 1;
            if (frame == 7 || frame == 21) {
                actor->speed.y = XREG_F(4) + 10.0f;
                if (i_this->pl_dir > (l_HIO.battle_init_range + 250.0f)) {
                    i_this->mode = 0;
                    actor->current.angle.y = actor->shape_angle.y;
                    break;
                }

                if (i_this->pl_dir > (l_HIO.battle_init_range + 100.0f)) {
                    actor->current.angle.y = i_this->search_angle_y;
                } else if (i_this->pl_dir < (l_HIO.battle_init_range - 100.0f)) {
                    actor->current.angle.y = -i_this->search_angle_y;
                } else {
                    actor->current.angle.y = cM_rndFX(32763.0f);
                }

                actor->speedF = cM_rndF(XREG_F(5) + 5.0f) + 5.0f;
                i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_BREATH, -1);
            }

            if (frame == 13 || frame == 27) {
                actor->speedF = 0.0f;
                i_this->set_smoke_flag = 3;
                if (frame == 13) {
                    i_this->sound.startCreatureSound(Z2SE_EN_DN_FN_RUN_L, 0, -1);
                } else {
                    i_this->sound.startCreatureSound(Z2SE_EN_DN_FN_RUN_R, 0, -1);
                }
            }

            actor->gravity = XREG_F(6) + -5.0f;
            break;
    }

    s16 sVar1;
    if (i_this->mode >= 0) {
        if (i_this->snap_angle_y_flag == 0) {
            sVar1 = actor->current.angle.y;
            cLib_addCalcAngleS2(&actor->current.angle.y, i_this->search_angle_y, 2, 0x800);
            sVar1 -= actor->current.angle.y;
            s16 sVar2 = actor->current.angle.y - i_this->search_angle_y;
            if (sVar2 > 0x800 || sVar2 < -0x800) {
                fVar1 = 0.0f;
            }

            cLib_addCalc2(&actor->speedF, fVar1, 1.0f, 5.0f);
        } else {
            sVar1 = actor->shape_angle.y;
            cLib_addCalcAngleS2(&actor->shape_angle.y, i_this->search_angle_y, 4, 0x800);
            sVar1 -= actor->shape_angle.y;
        }

        sVar1 *= (s16)(YREG_S(5) + 2);
        s16 sVar4 = YREG_S(6) + 0x1000;
        if (sVar1 > sVar4) {
            sVar1 = sVar4;
        } else if (sVar1 < (s16)-sVar4) {
            sVar1 = -sVar4;
        }
        cLib_addCalcAngleS2(&i_this->jnt_tail_y_rot_offset, sVar1, 2, 0x400);
    }

    u8 def = 0;
    learn_check = i_this->learn;
    if (fpcM_Search(shot_s_sub, i_this) != NULL || ((i_this->learn & 0x1000) != 0 && daPy_getPlayerActorClass()->checkHookshotShootReturnMode())) {
        def = 1;
        OS_REPORT(" DEF ON !!\n");
    }

    if (sVar4 == 0) {
        if (i_this->unk_timer_1 == 0) {
            i_this->action = ACTION_NORMAL;
            i_this->mode = 0;
            i_this->timer[0] = cM_rndF(50.0f) + 50.0f;
            anm_init(i_this, ANM_WAIT_03, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);

            if (i_this->snap_angle_y_flag != 0) {
                actor->current.angle.y = actor->shape_angle.y;
            }

            return;
        }
    } else if (
        dComIfGp_checkPlayerStatus1(0, 0x2000000) != 0 || 
        (((i_this->search_angle_x < 0x1000 && i_this->search_angle_x > -0x1000) || def != 0) && player_way_check(i_this))
    ) {
        if (
            (def != 0 || (i_this->pl_dir < l_HIO.battle_init_range && (i_this->learn & cc_pl_cut_bit_get()) != 0 &&
            ((daPy_getPlayerActorClass()->getCutAtFlg() != 0 || daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_JUMP) ||
            (i_this->learn & 0x400) != 0))) && i_this->field_0x74c < 1.0f
        ) {
            i_this->action = ACTION_GUARD;
            if (cc_pl_cut_bit_get() == 0x100) {
                i_this->mode = 0;
            } else {
                i_this->mode = 5;
            }

            i_this->field_0x708 = TREG_F(10) + 30.0f;
            i_this->field_0x70c = actor->shape_angle.y;
            i_this->timer[0] = 0;
            i_this->invulnerability_timer = 15;
        } else if (i_this->pl_dir < l_HIO.attack_init_range) {
            if (i_this->timer[2] == 0) {
                i_this->timer[2] = JREG_F(1) + (cM_rndF(30.0f) + 30.0f);
                f32 fVar1 = 1.0f;
                if (cM_rndF(1.0f) < fVar1) {
                    wayCheck = way_check(i_this);
                    if (!wayCheck && cM_rndF(1.0f) < 0.75f) {
                        if (i_this->pl_dir < (l_HIO.attack_init_range - 100.0f) + TREG_F(7)) {
                            i_this->action = ACTION_ATTACK_0;
                        } else {
                            i_this->action = ACTION_ATTACK;
                        }

                        i_this->mode = 0;
                    } else {
                        i_this->action = ACTION_TAIL_ATTACK;
                        i_this->mode = 0;
                    }
                }
            }
        }
    }

    i_this->field_0x6f4 = 1;
    if ((i_this->frame_counter & 15) == 0 && bomb_view_check(i_this) != NULL) { // Every 16 frames
        i_this->action = ACTION_BOMB_ACTION;
        i_this->mode = 0;
    }

    if (
        i_this->arg1 == 1 && move_gake_check(i_this, KREG_F(8) + 150.0f, 1) &&
        (i_this->frame_counter & 15) == 0 && cM_rndF(1.0f) < 0.5f // Every 16 frames
    ) {
        i_this->field_0x700 = cM_rndF(10.0f) + 10.0f;
        i_this->field_0x701 = cM_rndF(1.9f);
    }

    if (move_gake_check(i_this, KREG_F(9) + 50.0f, 1)) {
        if (i_this->arg1 == 1) {
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->action = ACTION_TAIL_ATTACK;
                i_this->mode = 0;
            } else {
                i_this->action = ACTION_GAKEJUMP;
                i_this->mode = 0;
                i_this->field_0x5c8 = actor->current.pos;
                i_this->field_0x5bc = actor->current.pos;
                i_this->invulnerability_timer = 100;
            }

            actor->speedF = 0.0f;
        } else {
            i_this->action = ACTION_BACKDROP;
            i_this->mode = 0;
        }
    }

    if (i_this->action != 3 && i_this->snap_angle_y_flag != 0) {
        actor->current.angle.y = actor->shape_angle.y;
    }
}

static void e_dn_jump(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    
    i_this->field_0x6f4 = 1;
    switch (i_this->mode) {
        case 0:
            anm_init(i_this, ANM_JUMP_A, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 1;
            break;

        case 1:
            if (!i_this->anm_p->isStop()) {
                break;
            }

            actor->speed.y = KREG_F(13) + 50.0f;
            i_this->mode = 2;
            i_this->set_smoke_flag = 3;
            i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_JUMP, -1);
            // fallthrough
        case 2:
            actor->speedF =KREG_F(13) +  30.0f;
            if (!(actor->speed.y <= 0.0f)) {
                break;
            }

            anm_init(i_this, ANM_JUMP_B, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 3;
            break;

        case 3:
            if (!i_this->objacch.ChkGroundHit()) {
                break;
            }

            anm_init(i_this, ANM_JUMP_C, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 4;
            i_this->set_smoke_flag = 3;
            i_this->timer[0] = 3;
            i_this->sound.startCreatureSound(Z2SE_EN_DN_FN_R, 0, -1);
            break;

        case 4:
            if (i_this->timer[0] == 1) {
                i_this->sound.startCreatureSound(Z2SE_EN_DN_FN_L, 0, -1);
            }

            cLib_addCalc0(&actor->speedF, 1.0f, 10.0f);
            if (!i_this->anm_p->isStop()) {
                break;
            }

            i_this->action = ACTION_FIGHT_RUN;
            i_this->mode = 0;
    }
}

static void e_dn_gakejump(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp24, sp30;

    i_this->field_0x6f4 = 1;
    switch (i_this->mode) {
        case 0:
            if (jump_pos_check(i_this, &i_this->field_0x5bc)) {
                anm_init(i_this, ANM_JUMP_A, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 2;
                actor->current.angle.y = i_this->search_angle_y;
                break;
            }

            if (i_this->anm_no != ANM_WAIT_01) {
                anm_init(i_this, ANM_WAIT_01, 5.0f, J3DFrameCtrl::EMode_LOOP, cM_rndF(0.1f) + 0.9f);
                i_this->timer[0] = cM_rndF(15.0f) + 35.0f;
                i_this->mode = 1;
            }

            cLib_addCalcAngleS2(&actor->current.angle.y, i_this->search_angle_y, 4, 0x800);
            break;

        case 1:
            i_this->field_0x6f4 = 2;
            cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
            sp24.x = 0.0f;
            sp24.y = 0.0f;
            sp24.z = 100.0f;
            MtxPosition(&sp24, &i_this->find_pos);
            i_this->find_pos += actor->current.pos;

            if (i_this->timer[0] != 0) {
                break;
            }

            i_this->mode = 0;
            break;

        case 2:
            if (!i_this->anm_p->isStop()) {
                break;
            }

            if (jump_pos_check(i_this, &i_this->field_0x5bc)) {
                i_this->mode = 3;
                i_this->set_smoke_flag = 3;
                i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_JUMP, -1);
                i_this->timer[0] = 20;
                i_this->timer[1] = 60;
                i_this->field_0x5c8 = actor->current.pos;
            } else {
                anm_init(i_this, ANM_WAIT_01, 5.0f, J3DFrameCtrl::EMode_LOOP, cM_rndF(0.1f) + 0.9f);
                i_this->timer[0] = cM_rndF(15.0f) + 35.0f;
                i_this->mode = 1;
                break;
            }
            // fallthrough
        case 3:
            i_this->field_0x5d8 = 1;
            sp24 = i_this->field_0x5bc - actor->current.pos;
            i_this->cur_angle_y_target = cM_atan2s(sp24.x, sp24.z);
            i_this->field_0x5d6 = -cM_atan2s(sp24.y, JMAFastSqrt(sp24.x * sp24.x + sp24.z * sp24.z));
            actor->speedF = KREG_F(13) + 50.0f;

            if (i_this->timer[0] == 1) {
                anm_init(i_this, ANM_JUMP_B, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            }

            if (sp24.abs() < (actor->speedF * 1.1f)) {
                actor->current.pos = i_this->field_0x5bc;
                anm_init(i_this, ANM_JUMP_C, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 4;
                i_this->set_smoke_flag = 3;
                i_this->timer[0] = 3;
                i_this->sound.startCreatureSound(Z2SE_EN_DN_FN_R, 0, -1);
                actor->speedF = 0.0f;
                i_this->invulnerability_timer = 0;
            }

            if (!(i_this->timer[1] == 0 || i_this->objacch.ChkWallHit() != 0)) {
                break;
            }

            i_this->action = ACTION_FIGHT_RUN;
            i_this->mode = 0;
            break;

        case 4:
            if (i_this->timer[0] == 1) {
                i_this->sound.startCreatureSound(Z2SE_EN_DN_FN_L, 0, -1);
            }

            if (!i_this->anm_p->isStop()) {
                break;
            }

            i_this->action = ACTION_FIGHT_RUN;
            i_this->mode = 0;
    }
}

static fopAc_ac_c* at_hit_check(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    
    if (i_this->mode >= 10) {
        return NULL;
    }
    
    if (i_this->at_sph.ChkAtHit() != 0) {
        return dCc_GetAc(i_this->at_sph.GetAtHitObj()->GetAc());
    }

    return NULL;
}

static void e_dn_attack_0(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp2c, sp38;
    int frame = i_this->anm_p->getFrame();

    i_this->field_0x6f4 = 1;
    switch (i_this->mode) {
        case 0:
            anm_init(i_this, ANM_ATTACK_TAIL_01, TREG_F(14) + 6.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_KNIFE, -1);
            i_this->mode = 1;
            // fallthrough
        case 1:
            if (frame >= 22 && frame <= 28) {
                i_this->at_chk_flag = 1;
                i_this->tail_at_sph_flag = 0;
            }

            if (i_this->anm_p->isStop()) {
                i_this->action = ACTION_FIGHT_RUN;
                i_this->mode = 0;
            }
    }

    cLib_addCalc0(&actor->speedF, 1.0f, 5.0f);

    if (i_this->at_chk_flag != 0) {
        fopAc_ac_c* actor_p = at_hit_check(i_this);
        if (actor_p != NULL && fopAcM_GetName(actor_p) == PROC_ALINK && daPy_getPlayerActorClass()->checkPlayerGuard()) {
            dComIfGp_getVibration().StartShock(3, 31, cXyz(0.0f, 1.0f, 0.0f));
        }
    }
}

static void e_dn_attack(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp34, sp40;
    int frame = i_this->anm_p->getFrame();

    i_this->field_0x6f4 = 1;
    s16 sVar1, sVar2;
    switch (i_this->mode) {
        case 0:
            anm_init(i_this, ANM_ATTACK_TAIL_02, TREG_F(14) + 6.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_KNIFE2_A, -1);
            i_this->mode = 1;
            // fallthrough
        case 1:
            if (frame < 10 || (frame >= 25 && frame <= 30)) {
                sVar1 = actor->current.angle.y;
                cLib_addCalcAngleS2(&actor->current.angle.y, i_this->search_angle_y, 2, 0x800);
                sVar1 -= actor->current.angle.y;
                sVar1 *= (s16)(YREG_S(5) + 2);
                s16 sVar3 = YREG_S(6) + 0x1000;
                if (sVar1 > sVar3) {
                    sVar1 = sVar3;
                } else if (sVar1 < (s16)-sVar3) {
                    sVar1 = -sVar3;
                }
                cLib_addCalcAngleS2(&i_this->jnt_tail_y_rot_offset, sVar1, 2, 0x400);
            }

            if (frame >= 13 && frame <= 18) {
                i_this->at_chk_flag = 1;
                i_this->tail_at_sph_flag = 0;
            }

            if (frame >= 25 && frame <= 30) {
                i_this->at_chk_flag = 1;
                i_this->tail_at_sph_flag = 1;
                cMtx_YrotS(*calc_mtx, actor->current.angle.y);
                sp34.x = 0.0f;
                sp34.y = 0.0f;
                sp34.z = BREG_F(17) + 25.0f;
                MtxPosition(&sp34, &sp40);
                actor->current.pos += sp40;
            }

            if (i_this->anm_p->isStop()) {
                i_this->action = ACTION_FIGHT_RUN;
                i_this->mode = 0;
            }
    }

    cLib_addCalc0(&actor->speedF, 1.0f, 5.0f);
    if (i_this->at_chk_flag != 0) {
        fopAc_ac_c* actor_p = at_hit_check(i_this);
        if (actor_p != NULL && fopAcM_GetName(actor_p) == PROC_ALINK && daPy_getPlayerActorClass()->checkPlayerGuard()) {
            i_this->anm_p->setPlaySpeed(0.0f);
            i_this->action = ACTION_FIGHT_RUN;
            i_this->mode = 0;
        }
    }
}

static void tail_eff_set(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;

    J3DModel* model = i_this->anm_p->getModel();
    i_this->dn_tail_blur_a_prtcl = dComIfGp_particle_set(i_this->dn_tail_blur_a_prtcl, dPa_RM(ID_ZI_S_DNTAILBLUR_A), &actor->current.pos, NULL, NULL);
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->dn_tail_blur_a_prtcl);
    if (emitter != NULL) {
        MTXCopy(model->getAnmMtx(JNT_WORLD_ROOT), *calc_mtx);
        emitter->setGlobalRTMatrix(*calc_mtx);
    }

    i_this->dn_tail_blur_b_prtcl = dComIfGp_particle_set(i_this->dn_tail_blur_b_prtcl, dPa_RM(ID_ZI_S_DNTAILBLUR_B), &actor->current.pos, NULL, NULL);
    emitter = dComIfGp_particle_getEmitter(i_this->dn_tail_blur_b_prtcl);
    if (emitter != NULL) {
        MTXCopy(model->getAnmMtx(JNT_WORLD_ROOT), *calc_mtx);
        emitter->setGlobalRTMatrix(*calc_mtx);
    }
}

static void e_dn_tail_attack(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp30, sp3c;
    int frame = i_this->anm_p->getFrame();

    i_this->field_0x6f4 = 1;
    switch (i_this->mode) {
        case 0:
            anm_init(i_this, ANM_ATTACK_01, TREG_F(14) + 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 1;
            break;

        case 1:
            if (frame >= 19) {
                tail_eff_set(i_this);
            }

            if (frame >= 33 && frame <= 41) {
                i_this->at_chk_flag = 1;
                i_this->tail_at_sph_flag = 2;
                cMtx_YrotS(*calc_mtx, i_this->search_angle_y);
                sp30.x = 0.0f;
                sp30.y = 0.0f;
                sp30.z = BREG_F(18) + 30.0f;
                MtxPosition(&sp30, &sp3c);
                actor->current.pos += sp3c;
            }

            if (frame == 40) {
                i_this->sound.startCreatureSound(Z2SE_EN_DN_FN_RUN_R, 0, -1);
            } else if (frame == 43) {
                i_this->sound.startCreatureSound(Z2SE_EN_DN_FN_RUN_L, 0, -1);
            }

            if (i_this->anm_p->isStop()) {
                i_this->action = ACTION_FIGHT_RUN;
                i_this->mode = 0;
            }
    }

    actor->current.angle.y += i_this->angle_y_offset;
    actor->shape_angle.y += i_this->angle_y_offset;
    cLib_addCalcAngleS2(&i_this->angle_y_offset, 0, 1, 300);
    cLib_addCalc0(&actor->speedF, 1.0f, 3.0f);
}

static void e_dn_guard(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;

    i_this->guard_flag = 1;
    i_this->field_0x6f4 = 1;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, ANM_WAIT_02, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 1;
            actor->speed.y = KREG_F(0) + 20.0f;
            actor->speedF = KREG_F(1) + -8.0f;
            i_this->set_smoke_flag = 3;
            break;

        case 1:
            i_this->invulnerability_timer = 5;
            if (i_this->anm_p->getFrame() >= 5.0f) {
                i_this->mode = 5;
                i_this->timer[0] = KREG_S(1) + 20;
            }
            break;

        case 5:
            anm_init(i_this, ANM_GUARD, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_GUARD, -1);
            i_this->mode = 6;
            // fallthrough
        case 6:
            i_this->invulnerability_timer = 5;
            if (i_this->objacch.ChkGroundHit()) {
                cLib_addCalc0(&actor->speedF, 1.0f, 10.0f);
            }

            if (i_this->anm_p->getFrame() >= 8.0f) {
                i_this->anm_p->setPlaySpeed(0.0f);
                i_this->mode = 7;
                actor->speedF = 0.0f;
            }
            break;

        case 7:
            i_this->invulnerability_timer = 5;
            if (cc_pl_cut_bit_get() == 0x80 || cc_pl_cut_bit_get() == 0x800) {
                i_this->timer[0] = KREG_S(0) + 20;
            }

            if (i_this->timer[0] == 0) {
                i_this->mode = 8;
                i_this->anm_p->setPlaySpeed(1.0f);
            }
            break;

        case 8:
            if (i_this->anm_p->isStop()) {
                i_this->action = ACTION_FIGHT_RUN;
                i_this->mode = 0;
            }
    }
}

static void e_dn_s_damage(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;

    switch (i_this->mode) {
        case 0:
            i_this->mode = 1;
            i_this->timer[0] = 20;
            anm_init(i_this, ANM_WAIT_01, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            break;

        case 1:
            if (i_this->timer[0] == 0) {
                i_this->action = ACTION_FIGHT_RUN;
                i_this->mode = 0;
                i_this->timer[0] = 40;
            }
    }

    cLib_addCalc0(&actor->speedF, 1.0f, 3.0f);
}

static BOOL kado_check(e_dn_class* i_this) {
    static int kado_bit[2] = {
        1, 2,
    };

    static f32 kado_check_x[2] = {
        80.0f, -80.0f,
    };

    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz work, start, end;
    BOOL kado = FALSE;

    cMtx_YrotS(*calc_mtx, i_this->field_0x724.y);
    for (int i = 0; i < 2; i++) {
        start = actor->current.pos;
        start.y += JREG_F(0) + 100.0f;
        work.x = JREG_F(1);
        work.y = JREG_F(2);
        work.z = JREG_F(3) + 70.0f;
        MtxPosition(&work, &end);
        start += end;
        work.y = JREG_F(4) + 50.0f;
        work.z = JREG_F(5) + -150.0f;
        work.x = (JREG_F(6) + 1.0f) * kado_check_x[i];
        MtxPosition(&work, &end);
        end += start;
        lin_chk.Set(&start, &end, actor);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            kado |= kado_bit[i];
        }
    }

    if (kado == 3) {
        kado = FALSE;
    }

    return kado;
}

static BOOL body_gake(e_dn_class* i_this) {
    J3DModel* model = i_this->anm_p->getModel();
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz work, body_pos;

    MTXCopy(model->getAnmMtx(JNT_BACKBONE02), *calc_mtx);
    work.set(BREG_F(10), BREG_F(11), BREG_F(12));
    MtxPosition(&work, &body_pos);

    dBgS_ObjGndChk gnd_chk;
    body_pos.y += 100.0f;
    gnd_chk.SetPos(&body_pos);
    body_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

    if ((actor->current.pos.y - body_pos.y) > 300.0f) {
        return TRUE;
    }

    return FALSE;
}

static void e_dn_damage(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp34, sp40;

    if (actor->health <= 0) {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    }

    if (daPy_py_c::checkNowWolf() != 0) {
        if (i_this->field_0x6f3 == 0 && actor->checkWolfDownStartFlg()) {
            if (i_this->timer[0] > 40) {
                i_this->timer[0] = 90;
                OS_REPORT("      DN WOLFHIT LONG  \n");
            } else if (i_this->timer[0] > 20) {
                i_this->timer[0] = 70;
                OS_REPORT("      DN WOLFHIT MID  \n");
            } else {
                i_this->timer[0] = 50;
                OS_REPORT("      DN WOLFHIT SHORT \n");
            }

            i_this->mode = 30;
            i_this->field_0x6f3 = 1;

            if (i_this->arg1 == 1) {
                i_this->timer[1] = l_HIO.soul_disappear_time_strong + 1;
            } else {
                i_this->timer[1] = l_HIO.soul_disappear_time_weak + 1;
            }

        }
    } else if (actor != NULL && actor->checkCutDownHitFlg()) {
        actor->offCutDownHitFlg();
        i_this->mode = 3;
        actor->health = 0;
        i_this->timer[0] = 100;
        i_this->timer[1] = 46;
        actor->offDownFlg();
        actor->health = 0;
        i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_DEATH, -1);
        i_this->death_flag = 1;
        daPy_getPlayerActorClass()->onEnemyDead();
        i_this->death_jnt_rot_counter = TREG_S(7) + 15;
    }

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, ANM_BLOWNOFF, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 1;
            actor->speedF = 0.0f;
            i_this->field_0x7e8 = BREG_F(10) + 8000.0f;

            if (actor->health <= 0) {
                i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_DEATH, -1);
                i_this->death_flag = 1;
                daPy_getPlayerActorClass()->onEnemyDead();
            } else {
                i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_DAMAGE2, -1);
            }

            i_this->timer[0] = 0;
            break;

        case 1:
            if (i_this->objacch.ChkWallHit() != 0 && i_this->timer[0] == 0) {
                int kado_kabe = kado_check(i_this);
                if (kado_kabe != 0) {
                    OS_REPORT("              ..KADO KABE ..%x\n", kado_kabe);
                    if (kado_kabe == 2) {
                        i_this->field_0x70e = 0xC00;
                        i_this->field_0x724.y += TREG_S(8) + -7000;
                    } else {
                        i_this->field_0x70e = -0xC00;
                        i_this->field_0x724.y += -(TREG_S(8) - 7000);
                    }

                    i_this->field_0x7e8 = BREG_F(10) + 8000.0f;
                    i_this->field_0x754 = BREG_F(4) + 100.0f;
                    i_this->field_0x75c = BREG_F(5) + 100.0f;
                    i_this->unk_timer_4 = DREG_S(1) + 40;
                    i_this->timer[0] = 60;
                } else {
                    i_this->mode = 10;
                    actor->speed.y = 0.0f;
                    i_this->field_0x704 *= 0.2f;
                    i_this->field_0x724.y += 0x8000;
                    i_this->field_0x7e8 = BREG_F(10) + 8000.0f;
                    i_this->unk_timer_5 = BREG_S(7) + 5;
                    i_this->field_0x754 = BREG_F(4) + 100.0f;
                    i_this->field_0x75c = BREG_F(5) + 100.0f;
                    i_this->unk_timer_4 = DREG_S(1) + 40;
                    i_this->field_0x70e = 0;
                }
                break;
            }

            cLib_addCalcAngleS2(&i_this->field_0x724.x, -0x4000, 1, BREG_S(4) + 0x300);
            actor->current.angle.y += i_this->field_0x70e;
            actor->shape_angle.y = actor->current.angle.y;

            if (i_this->field_0x724.x <= -0x3000 && i_this->objacch.ChkGroundHit()) {
                i_this->mode = 2;
                i_this->unk_timer_3 = 10;
                i_this->sound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);

                if (i_this->left_right_flag == 0) {
                    anm_init(i_this, ANM_DIE_LEFT, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                } else {
                    anm_init(i_this, ANM_DIE_RIGHT, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                }

                if (i_this->field_0x710 != 0) {
                    i_this->field_0x704 = 0.0f;
                    i_this->field_0x7e8 = BREG_F(12) + 2000.0f;
                } else {
                    if (i_this->left_right_flag == 0) {
                        i_this->field_0x754 = -100.0f - BREG_F(6);
                        i_this->field_0x75c = -100.0f - BREG_F(7);
                    } else {
                        i_this->field_0x754 = BREG_F(4) + 100.0f;
                        i_this->field_0x75c = BREG_F(5) + 100.0f;
                    }

                    i_this->field_0x758 = -0x5000;
                    i_this->field_0x760 = -0x4000;
                    i_this->unk_timer_4 = DREG_S(1) + 40;
                    i_this->field_0x704 *= 0.5f;
                    actor->speed.y = TREG_F(6) + 20.0f;
                    i_this->field_0x70e /= 2;
                    actor->current.angle.y = i_this->field_0x724.y;
                    i_this->field_0x7e8 = BREG_F(12) + 8000.0f;

                    J3DModel* model = i_this->anm_p->getModel();
                    MTXCopy(model->getAnmMtx(JNT_BACKBONE02), *calc_mtx);
                    sp34.set(0.0f, 0.0f, 0.0f);
                    MtxPosition(&sp34, &sp40);

                    fopAcM_effSmokeSet1(&i_this->field_0x10dc, &i_this->field_0x10e0, &sp40, NULL, 1.8f, &actor->tevStr, 1);
                }
            }
            break;

        case 2:
            actor->current.angle.y += i_this->field_0x70e;
            actor->shape_angle.y = actor->current.angle.y;
            cLib_addCalcAngleS2(&i_this->field_0x724.x, -0x4000, 1, BREG_S(4) + 0x300);

            if (i_this->objacch.ChkGroundHit()) {
                if (actor->health > 0 && !daPy_py_c::checkNowWolf()) {
                    actor->onDownFlg();
                }

                dKy_Sound_set(actor->current.pos, 100, fopAcM_GetID(&i_this->actor), 5);
                i_this->unk_timer_3 = 10;

                if (i_this->left_right_flag == 0) {
                    anm_init(i_this, ANM_DIE_LEFT, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                } else {
                    anm_init(i_this, ANM_DIE_RIGHT, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                }

                i_this->field_0x704 = 0.0f;
                i_this->field_0x724.x = -0x4000;

                for (int i = 0; i < 4; i++) {
                    i_this->field_0x802[i] = cM_rndF(10.0f);
                }

                actor->current.angle.y = i_this->field_0x724.y;
                if (daPy_py_c::checkNowWolf() != 0) {
                    i_this->timer[0] = 80;
                    i_this->timer[1] = 55;
                } else {
                    i_this->timer[0] = cM_rndF(10.0f) + 60.0f;
                    i_this->timer[1] = 35;
                }

                i_this->mode = 3;
                i_this->field_0x7e8 = BREG_F(13) + 1500.0f;

                J3DModel* model = i_this->anm_p->getModel();
                MTXCopy(model->getAnmMtx(JNT_BACKBONE02), *calc_mtx);
                sp34.set(0.0f, 0.0f, 0.0f);
                MtxPosition(&sp34, &sp40);

                fopAcM_effSmokeSet1(&i_this->field_0x10dc, &i_this->field_0x10e0, &sp40, NULL, 1.0f, &actor->tevStr, 1);
            }
            break;

        case 3:
            if (body_gake(i_this)) {
                i_this->field_0x704 = VREG_F(8) + -20.0f;
                i_this->field_0x724.x -= (s16)(VREG_S(7) + 0x300);
            }

            if (actor->health <= 0 && i_this->timer[1] == 0) {
                MTXCopy(i_this->anm_p->getModel()->getAnmMtx(JNT_BACKBONE02), *calc_mtx);
                sp34.set(0.0f, 0.0f, 0.0f);
                MtxPosition(&sp34, &sp40);
                
                s8 unused = 0;

                dn_disappear(i_this);
                fopAcM_delete(actor);
                return;
            }

            if (daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_DOWN && i_this->timer[0] == 0) {
                i_this->mode = 20;
                if (i_this->left_right_flag == 0) {
                    anm_init(i_this, ANM_DIE_LEFT_UP, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                } else {
                    anm_init(i_this, ANM_DIE_RIGHT_UP, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                }

                i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_UP, -1);

                if (actor != NULL) {
                    actor->offDownFlg();
                }
            }
            break;

        case 10:
            if (i_this->unk_timer_5 != 0) {
                cLib_addCalcAngleS2(&i_this->field_0x724.x, 0, 1, BREG_S(4) + 0x300);
            } else {
                cLib_addCalcAngleS2(&i_this->field_0x724.x, -0x4000, 1, BREG_S(5) + 0x800);
                if (i_this->objacch.ChkGroundHit()) {
                    i_this->mode = 2;
                    if (i_this->left_right_flag == 0) {
                        i_this->field_0x754 = -100.0f - BREG_F(6);
                        i_this->field_0x75c = -100.0f - BREG_F(7);
                    } else {
                        i_this->field_0x754 = BREG_F(4) + 100.0f;
                        i_this->field_0x75c = BREG_F(5) + 100.0f;
                    }

                    i_this->field_0x758 = -0x5000;
                    i_this->field_0x760 = -0x4000;
                    i_this->field_0x704 *= 0.5f;
                    actor->speed.y = TREG_F(6) + 20.0f;
                    actor->current.angle.y = i_this->field_0x724.y;
                    i_this->field_0x7e8 = BREG_F(12) + 8000.0f;
                }
            }
            break;

        case 20:
            if (i_this->anm_p->getFrame() >= 35.0f) {
                i_this->field_0x730.x = i_this->field_0x730.z = 0;
            }

            if (i_this->anm_p->isStop()) {
                i_this->field_0x724.x = i_this->field_0x724.y = 0;
                anm_init(i_this, ANM_WAIT_01, 0.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->timer[0] = 5;
                i_this->mode = 21;

                if (actor != NULL) {
                    actor->offDownFlg();
                }
            }
            break;

        case 21:
        case 22:
            i_this->invulnerability_timer = 0;
            if (i_this->timer[0] == 0) {
                i_this->action = ACTION_NORMAL;
                i_this->mode = 0;
                i_this->timer[0] = cM_rndF(50.0f) + 50.0f;
                anm_init(i_this, ANM_WAIT_03, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;

        case 30:
            if (!actor->checkWolfDownPullFlg()) {
                i_this->timer[1] = 0;
            }

            if (i_this->timer[0] == 0) {
                i_this->mode = 31;
                if (i_this->left_right_flag == 0) {
                    anm_init(i_this, ANM_BRUSH_LEFT_UP, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                } else {
                    anm_init(i_this, ANM_BRUSH_RIGHT_UP, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                }

                i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_UP, -1);
                i_this->timer[0] = 10;
            } else if (i_this->timer[1] == 1) {
                actor->onWolfDownPullEndFlg();
                actor->offCutDownHitFlg();
                actor->health = 0;
                i_this->timer[0] = 1000;
                i_this->timer[1] = 35;
                i_this->mode = 3;
            }
            break;

        case 31:
            if (i_this->timer[0] == 0) {
                actor->offDownFlg();
                i_this->mode = 20;
                i_this->field_0x6f3 = 0;
            }
    }

    if (actor != NULL && actor->checkDownFlg()) {
        i_this->field_0x6f2 = 0;
    }

    if (i_this->death_jnt_rot_counter != 0) {
        i_this->death_jnt_rot_counter--;
        i_this->death_jnt_rot = i_this->death_jnt_rot_counter * cM_ssin(i_this->death_jnt_rot_counter * (TREG_S(9) + 0x3800)) * (TREG_F(5) + 100.0f);
    }
}

static void e_dn_backdrop(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    f32 zero = 0.0f;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, ANM_DRAWBACK, 3.0f, J3DFrameCtrl::EMode_NONE, 0.5f);
            i_this->mode = 1;
            i_this->field_0x724.y = (s16)actor->shape_angle.y;
            i_this->field_0x724.x = 0;
            actor->speedF = -3.0f;
            i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_DEATH, -1);
            // fallthrough
        case 1:
            if (i_this->field_0x724.x < -0x4000) {
                actor->speedF = -6.0f;
            }

            if (actor->speed.y < -20.0f) {
                i_this->mode = 2;
                actor->speedF = 0.0f;
            }
            // fallthrough
        case 2:
            // Target: -157.5 degrees, Step: (field_0x724.x - (-157.5 degrees)) / 2, Max Step: 11.25 degrees
            cLib_addCalcAngleS2(&i_this->field_0x724.x, -0x7000, 2, 0x800);

            if (i_this->mode == 2 && actor->current.pos.y <= (KREG_F(17) + (i_this->objacch.GetGroundH() + 100.0f))) {
                actor->current.pos.y = KREG_F(17) + (i_this->objacch.GetGroundH() + 100.0f);
                i_this->field_0x724.x = -0x4000;
                i_this->action = ACTION_DAMAGE;
                i_this->mode = 0;
                i_this->field_0x704 = 0.0f;
                actor->health = 0;
            }
    }
}

static void e_dn_water(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp20, sp2c;

    MTXCopy(i_this->anm_p->getModel()->getAnmMtx(JNT_BACKBONE02), *calc_mtx);
    sp20.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp20, &sp2c);

    i_this->invulnerability_timer = 10;
    switch (i_this->mode) {
        case 0:
        case 1: {
            actor->speed.y = 0.0f;
            if (i_this->left_right_flag == 0) {
                anm_init(i_this, ANM_DROWNED_A, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            } else {
                anm_init(i_this, ANM_DROWNED_B, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }

            i_this->timer[0] = 60;
            i_this->field_0x708 = i_this->field_0x704 = actor->speedF = 0.0f;

            cXyz pos = actor->current.pos;
            pos.y = i_this->field_0xa5c;
            if (i_this->mode == 0) {
                fopKyM_createWpillar(&pos, 1.5f, 0);
            } else {
                fopKyM_createMpillar(&pos, 1.0f);
            }

            i_this->mode = 10;
            break;
        }

        case 10:
            actor->speed.y = 0.0f;
            cLib_addCalc2(&actor->current.pos.y, i_this->field_0xa5c, 0.5f, 5.0f);
            cLib_addCalcAngleS2(&i_this->field_0x724.x, -0x4000, 4, BREG_S(4) + 0x400);

            if (i_this->timer[0] == 0) {
                dn_disappear(i_this);
            }
            break;
    }
}

static void e_dn_reg(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp34, sp40;
    f32 dash_speed = 0.0f;
    int frame = i_this->anm_p->getFrame();
    
    i_this->field_0x6f2 = 0;
    i_this->invulnerability_timer = 10;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, ANM_RUN, 5.0f, J3DFrameCtrl::EMode_LOOP, cM_rndFX(0.05f) + 1.0f);
            i_this->mode = 1;

            if ((i_this->arg0 & 1) != 0) {
                actor->current.pos.set(-8613.0f, 2000.0f, 8017.0f);
                actor->current.angle.set(0, 0x19A0, 0);
            } else {
                actor->current.pos.set(-8044.0f, 2000.0f, 8616.0f);
                actor->current.angle.set(0, -0x629E, 0);
            }

            actor->old.pos = actor->current.pos;
            i_this->cur_angle_y_target = actor->current.angle.y;
            i_this->timer[0] = nREG_S(0) + 14;
            // fallthrough
        case 1:
            dash_speed = l_HIO.dash_speed;
            if ((i_this->frame_counter & 3) == 0) { // Every 4 frames
                if ((i_this->frame_counter & 4) != 0) { // Every 5 frames
                    i_this->set_smoke_flag = 1;
                } else {
                    i_this->set_smoke_flag = 2;
                }
            }

            if (i_this->timer[0] == 0) {
                fopAc_ac_c* player = dComIfGp_getPlayer(0);
                cMtx_YrotS(*calc_mtx, (s16)player->shape_angle.y);

                if ((i_this->arg0 & 1) != 0) {
                    sp34.x = 100.0f;
                } else {
                    sp34.x = -100.0f;
                }
                sp34.y = 0.0f;
                sp34.z = 0.0f;
                MtxPosition(&sp34, &sp40);
                sp40 += player->current.pos;
                sp34 = sp40 - actor->current.pos;
                i_this->cur_angle_y_target = cM_atan2s(sp34.x, sp34.z);
            }
            break;

        case 2:
            if ((i_this->arg0 & 1) != 0) {
                i_this->field_0x750 = actor->shape_angle.y - (0x4000 << 2) + 0x4000;
                i_this->field_0x724.y = actor->shape_angle.y + 0x4000;
                i_this->field_0x70e = (cM_rndFX(800.0f) + 3000.0f);
            } else {
                i_this->field_0x750 = actor->shape_angle.y - 0x4000;
                i_this->field_0x724.y = actor->shape_angle.y - 0x4000;
                i_this->field_0x70e = -(cM_rndFX(800.0f) + 3000.0f);
            }

            i_this->field_0x74c = 150.0f;
            i_this->action = ACTION_DAMAGE;
            i_this->mode = 0;
            actor->speed.y = TREG_F(13) + 40.0f + cM_rndF(10.0f);
            i_this->field_0x704 = TREG_F(18) + -30.0f;
            i_this->unk_timer_4 = DREG_S(2) + 50;
            actor->health = 0;
            break;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->cur_angle_y_target, 2, 0x800);
    s16 sVar1 = actor->current.angle.y - i_this->cur_angle_y_target;
    if (sVar1 > 0x800 || sVar1 < -0x800) {
        dash_speed = 0.0f;
    }

    actor->speedF = dash_speed;
    i_this->field_0x6f4 = 1;
}

static void wolfkick_damage(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    i_this->field_0x750 = (actor->shape_angle.y - 0x8000) - i_this->at_info.mHitDirection.y;
    i_this->field_0x74c = 150.0f;
    i_this->action = ACTION_DAMAGE;
    i_this->mode = 0;
    i_this->field_0x724.y = player->shape_angle.y + 0x8000;
    actor->speed.y = 0.0f;
    i_this->field_0x704 = -1.0f;
    i_this->unk_timer_4 = DREG_S(2) + 50;
    i_this->sound.startCollisionSE(Z2SE_HIT_WOOD_WEAPON, 31);
    i_this->field_0x710 = 1;
}

static void big_damage(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;

    i_this->field_0x750 = (actor->shape_angle.y - 0x8000) - i_this->at_info.mHitDirection.y;
    i_this->field_0x74c = 150.0f;
    i_this->action = ACTION_DAMAGE;
    i_this->mode = 0;
    i_this->field_0x724.y = i_this->at_info.mHitDirection.y;
    actor->speed.y = TREG_F(13) + 25.0f + cM_rndF(10.0f);
    i_this->field_0x704 = TREG_F(18) + -20.0f;
    i_this->unk_timer_4 = DREG_S(2) + 50;
    if ((i_this->at_info.mHitBit & 0x80) != 0) {
        OS_REPORT("SPIN CUT HIT !!\n");
        i_this->field_0x70e = cM_rndFX(800.0f) + 3000.0f;
    } else if ((i_this->at_info.mHitBit & 0x800) != 0) {
        OS_REPORT("SPIN CUT L HIT !!\n");
        i_this->field_0x70e = -(cM_rndFX(800.0f) + 3000.0f);
    } else {
        i_this->field_0x70e = cM_rndFX(700.0f);
    }

    i_this->field_0x710 = 0;
}

static void small_damage(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;

    i_this->action = ACTION_S_DAMAGE;
    i_this->mode = 0;
    i_this->unk_timer_4 = DREG_S(6) + 50;
    i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_DAMAGE, -1);

    if (i_this->snap_angle_y_flag != 0) {
        actor->current.angle.y = actor->shape_angle.y;
    }

    i_this->field_0x750 = (actor->shape_angle.y - 0x8000) - i_this->at_info.mHitDirection.y;
    i_this->field_0x74c = 150.0f;
    i_this->field_0x708 = TREG_F(0) + 25.0f;
    i_this->field_0x70c = i_this->at_info.mHitDirection.y;
}

static void damage_check(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (l_HIO.no_learn != 0) {
        i_this->learn = 0;
    }

    i_this->stts.Move();

    if (i_this->invulnerability_timer == 0) {
        fopEn_enemy_c* enemy = (fopEn_enemy_c*)actor;
        dCcU_AtInfo atInfo;
        if (l_HIO.invulnerable != 0) {
            actor->health = 100;
        }

        for (int i = 0; i <= 2; i++) {
            if (i_this->cc_sph[i].ChkTgHit() != 0) {
                i_this->at_info.mpCollider = i_this->cc_sph[i].GetTgHitObj();
                if (player->getCutType() != daPy_py_c::CUT_TYPE_WOLF_B_LEFT && player->getCutType() != daPy_py_c::CUT_TYPE_WOLF_B_RIGHT &&
                    i_this->at_info.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK)) {
                    if (player->onWolfEnemyHangBite(enemy)) {
                        OS_REPORT("DN PL BITE HANG \n");
                        i_this->action = ACTION_WOLFBITE;
                        i_this->mode = 0;
                        i_this->invulnerability_timer = 1000;
                        i_this->sound.startCollisionSE(Z2SE_HIT_WOLFBITE, 31);
                    }
                    return;
                }

                i_this->unk_timer_6 = 3;
                if (i_this->at_info.mpCollider->ChkAtType(AT_TYPE_10000000)) {
                    wolfkick_damage(i_this);
                    i_this->invulnerability_timer = 1000;
                } else {
                    at_power_check(&i_this->at_info);
                    s16 sVar1 = actor->health;
                    cc_at_check(actor, &i_this->at_info);

                    if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_HEAD_JUMP) {
                        i_this->at_info.mHitStatus = 0;
                    }

                    if (i_this->at_info.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                        i_this->invulnerability_timer = 20;
                    } else {
                        i_this->invulnerability_timer = 10;
                    }

                    if (i_this->at_info.mAttackPower <= 1) {
                        i_this->invulnerability_timer = 10;
                    }

                    i_this->learn |= i_this->at_info.mHitBit;
                    if (i_this->at_info.mHitType == 16 || i_this->at_info.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                        actor->health = sVar1;
                        i_this->action = ACTION_DRAWBACK;
                        i_this->mode = 0;

                        if (i_this->snap_angle_y_flag != 0) {
                            actor->current.angle.y = actor->shape_angle.y;
                        }

                        i_this->field_0x750 = (actor->shape_angle.y - 0x8000) - i_this->at_info.mHitDirection.y;
                        i_this->field_0x74c = KREG_F(9) + 100.0f;
                        i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_DAMAGE, -1);
                        break;
                    }

                    if (daPy_getPlayerActorClass()->checkHorseRide() != 0 && dComIfGp_getHorseActor()->speedF >= 20.0f && i_this->at_info.mHitType == 1) {
                        i_this->at_info.mAttackPower = 20;
                        if (cM_rndF(1.0f) < 0.5f) {
                            i_this->at_info.mHitBit |= 0x80;
                        } else {
                            i_this->at_info.mHitBit |= 0x800;
                        }
                    }

                    if (actor->health <= 0 || i_this->at_info.mHitStatus != 0) {
                        if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP && player->checkCutJumpCancelTurn()) {
                            small_damage(i_this);
                            i_this->invulnerability_timer = 3;
                        } else {
                            big_damage(i_this);
                            i_this->invulnerability_timer = 1000;
                        }
                    } else {
                        small_damage(i_this);
                    }

                    actor->speedF = 0.0f;
                }
                break;
            }
        }

        for (int i = 0; i <= 2; i++) {
            if (actor->health <= 1) {
                i_this->cc_sph[i].SetTgHitMark((CcG_Tg_HitMark)3);
            }
        }
    }
}

static void action(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz work, sp24c;

    i_this->field_0x6f4 = 0;
    i_this->pl_dir = fopAcM_searchPlayerDistanceXZ(actor);
    i_this->search_angle_y = fopAcM_searchPlayerAngleY(actor);
    i_this->search_angle_x = fopAcM_searchPlayerAngleX(actor);
    damage_check(i_this);
    i_this->snap_angle_y_flag = 0;

    s8 pl_search = 0;
    s8 bVar2 = 1;
    actor->offHeadLockFlg();
    if (fopAcM_otherBgCheck(player, actor)) {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    } else {
        fopAcM_OnStatus(actor, 0);
        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    }

    switch (i_this->action) {
        case ACTION_NORMAL:
            e_dn_normal(i_this);
            break;

        case ACTION_FIGHT_RUN:
            e_dn_fight_run(i_this);
            pl_search = 1;
            break;

        case ACTION_ATTACK_0:
            e_dn_attack_0(i_this);
            pl_search = 1;
            break;

        case ACTION_ATTACK:
            e_dn_attack(i_this);
            pl_search = 1;
            break;

        case ACTION_TAIL_ATTACK:
            e_dn_tail_attack(i_this);
            pl_search = 1;
            break;

        case ACTION_GUARD:
            e_dn_guard(i_this);
            pl_search = 1;
            break;

        case ACTION_JUMP:
            e_dn_jump(i_this);
            pl_search = 1;
            break;

        case ACTION_STAY:
            e_dn_stay(i_this);
            break;

        case ACTION_OTOREAC:
            e_dn_otoreac(i_this);
            break;

        case ACTION_DRAWBACK:
            e_dn_drawback(i_this);
            pl_search = 1;
            break;

        case ACTION_WOLFBITE:
            e_dn_wolfbite(i_this);
            break;

        case ACTION_BOMB_ACTION:
            e_dn_bomb_action(i_this);
            break;

        case ACTION_S_DAMAGE:
            e_dn_s_damage(i_this);
            break;

        case ACTION_DAMAGE:
            e_dn_damage(i_this);
            break;

        case ACTION_BACKDROP:
            e_dn_backdrop(i_this);
            break;

        case ACTION_WATER:
            e_dn_water(i_this);
            bVar2 = 0;
            break;

        case ACTION_GAKEJUMP:
            e_dn_gakejump(i_this);
            pl_search = 1;
            break;

        case ACTION_REG:
            e_dn_reg(i_this);
            break;
    }

    if (bVar2) {
        dBgS_ObjGndChk obj_gnd_chk;
        dBgS_ObjGndChk_Spl spl_chk;
        work = actor->current.pos;
        work.y += 150.0f;
        spl_chk.SetPos(&work);
        obj_gnd_chk.SetPos(&work);
        i_this->field_0xa5c = dComIfG_Bgsp().GroundCross(&spl_chk);
        f32 fVar1 = i_this->field_0xa5c - dComIfG_Bgsp().GroundCross(&obj_gnd_chk);
        if (actor->current.pos.y < i_this->field_0xa5c && fVar1 > 100.0f) {
            OS_REPORT("DN YOGAN \n");
            i_this->action = ACTION_WATER;
            if (dComIfG_Bgsp().GetPolyAtt0(spl_chk) == 6) {
                i_this->mode = 1;
            } else {
                i_this->mode = 0;
            }

            actor->current.pos.y = i_this->field_0xa5c;
        }
    }

    if (pl_search) {
        i_this->sound.setLinkSearch(true);
    } else {
        i_this->sound.setLinkSearch(false);
    }

    if (i_this->snap_angle_y_flag == 0) {
        cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 2, 0x2000);
    }

    cLib_addCalcAngleS2(&actor->shape_angle.x, actor->current.angle.x, 2, 0x2000);
    cLib_addCalcAngleS2(&actor->shape_angle.z, actor->current.angle.z, 2, 0x2000);

    if (i_this->field_0x5d8 != 0) {
        cMtx_YrotS(*calc_mtx, i_this->cur_angle_y_target);
        cMtx_XrotM(*calc_mtx, i_this->field_0x5d6);
        work.x = 0.0f;
        work.y = 0.0f;
        work.z = actor->speedF;
        MtxPosition(&work, &sp24c);
        actor->current.pos += sp24c;
        work = i_this->field_0x5bc - i_this->field_0x5c8;
        f32 fVar1 = work.abs();
        work = i_this->field_0x5bc - actor->current.pos;
        f32 fVar2 = work.abs();
        f32 fVar3 = fVar1 * (NREG_F(19) + 0.3f);
        if (fVar3 > NREG_F(18) + 250.0f) {
            fVar3 = NREG_F(18) + 250.0f;
        }

        i_this->cur_pos_y_offset = fVar3 * cM_ssin((fVar2 / fVar1) * 32768.0f);
        i_this->field_0x5d8 = 0;
    } else {
        cLib_addCalc0(&i_this->cur_pos_y_offset, 1.0f, 30.0f);

        if (i_this->field_0x704) {
            cMtx_YrotS(*calc_mtx, i_this->field_0x724.y);
            if (i_this->unk_timer_5 == 0) {
                work.z = i_this->field_0x704;
            } else {
                work.z = 0.0f;
                actor->speed.y = 0.0f;
            }

            actor->gravity = -4.0f;
        } else {
            cMtx_YrotS(*calc_mtx, actor->current.angle.y);
            work.z = actor->speedF;
        }
        work.x = 0.0f;
        work.y = 0.0f;
        MtxPosition(&work, &sp24c);

        actor->speed.x = sp24c.x;
        actor->speed.z = sp24c.z;
        actor->speed.y += actor->gravity;
        actor->current.pos += actor->speed;
        actor->gravity = -5.0f;

        if (actor->speed.y < -100.0f) {
            actor->speed.y = -100.0f;
        }

        if (i_this->action != ACTION_STAY && i_this->field_0x6f2 != 0) {
            cXyz* move_pos = i_this->stts.GetCCMoveP();
            if (move_pos != NULL) {
                actor->current.pos.x += move_pos->x;
                actor->current.pos.z += move_pos->z;
            }
        }

        if (i_this->field_0x708 > 0.1f) {
            work.x = 0.0f;
            work.y = 0.0f;
            work.z = -i_this->field_0x708;
            cMtx_YrotS(*calc_mtx, i_this->field_0x70c);
            MtxPosition(&work, &sp24c);
            actor->current.pos += sp24c;
            cLib_addCalc0(&i_this->field_0x708, 1.0f, TREG_F(12) + 7.0f);
        }
    }

    s16 max_step, sVar2, sVar3;
    sVar2 = 0;
    sVar3 = 0;
    max_step = 0x800;
    if (i_this->field_0x700 != 0) {
        i_this->field_0x700--;
        i_this->field_0x6f4 = 2;
        cMtx_YrotS(*calc_mtx, actor->shape_angle.y);

        if (i_this->field_0x701 != 0) {
            work.x = 100.0f;
        } else {
            work.x = -100.0f;
        }
        work.y = JREG_F(4) + 100.0f;
        work.z = -100.0f;
        MtxPosition(&work, &i_this->find_pos);
        i_this->find_pos += actor->current.pos;
        max_step = 0x400;
    }

    if (i_this->field_0x6f4 != 0) {
        if (i_this->field_0x6f4 == 5) {
            if ((i_this->frame_counter & 15) == 0 && cM_rndF(1.0f) < 0.3f) { // Every 16 frames
                i_this->field_0x6fc = cM_rndFX(2500.0f);
            }
        } else {
            if (i_this->field_0x6f4 == 1) {
                work = player->eyePos - actor->current.pos;
            } else if (i_this->field_0x6f4 == 2) {
                work = i_this->find_pos - actor->current.pos;
            } else {
                work = player->eyePos - actor->current.pos;
            }
            work.y += -(TREG_F(2) + 150.0f) * l_HIO.model_size;

            sVar2 = cM_atan2s(work.x, work.z) - actor->shape_angle.y;
            sVar3 = actor->shape_angle.x + cM_atan2s(work.y, JMAFastSqrt(work.x * work.x + work.z * work.z));
            if (sVar2 > 0x2710) {
                sVar2 = 10000;
            } else if (sVar2 < -10000) {
                sVar2 = -10000;
            }

            if (sVar3 > 0x2710) {
                sVar3 = 10000;
            } else if (sVar3 < -20000) {
                sVar3 = -20000;
            }

            cLib_addCalcAngleS2(&i_this->field_0x6f6, sVar2 / 2, 2, max_step);
            cLib_addCalcAngleS2(&i_this->field_0x6f8, sVar3 / 2, 2, max_step);
        }

        i_this->field_0x6fc = 0;
    } else {
        cLib_addCalcAngleS2(&i_this->field_0x6f6, 0, 2, max_step);
        cLib_addCalcAngleS2(&i_this->field_0x6f8, 0, 2, max_step);
        i_this->field_0x6fc = 0;
    }

    cLib_addCalcAngleS2(&i_this->field_0x6fa, i_this->field_0x6fc, 4, 0x400);

    if (i_this->field_0x6fe != 0) {
        i_this->field_0x6fe--;
        i_this->field_0x6fa = i_this->field_0x6fe * cM_ssin(i_this->field_0x6fe * (BREG_S(5) + 12000)) * (BREG_F(18) + 200.0f);
    }

    if (i_this->field_0x74c > 1.0f) {
        cMtx_YrotS(*calc_mtx, i_this->field_0x750);
        work.x = 0.0f;
        work.y = 0.0f;
        work.z = (TREG_F(7) + 120.0f) * i_this->field_0x74c;
        if (work.z > 15000.0f) {
            work.z = 15000.0f;
        }
        MtxPosition(&work, &sp24c);
        
        i_this->field_0x7a4[0].y = sp24c.x;
        i_this->field_0x7a4[0].x = sp24c.z;
        i_this->field_0x7a4[1].y = sp24c.x;
        i_this->field_0x7a4[1].x = sp24c.z;
        cLib_addCalc0(&i_this->field_0x74c, 0.5f, TREG_F(7) + 10.0f);
    }

    if (fabsf(i_this->field_0x75c) > 1.0f) {
        i_this->field_0x7a4[1].x = i_this->field_0x75c * cM_scos(i_this->field_0x760) * (BREG_F(0) + 70.0f);
        i_this->field_0x7a4[2].x += i_this->field_0x7a4[1].x;
        cLib_addCalc0(&i_this->field_0x75c, 1.0f, BREG_F(1) + 3.0f);
        i_this->field_0x760 += BREG_S(0) + 4000;
    }

    if (fabsf(i_this->field_0x754) > 1.0f) {
        i_this->field_0x7a4[0].x = i_this->field_0x754 * cM_scos(i_this->field_0x758) * (BREG_F(2) + 70.0f);
        cLib_addCalc0(&i_this->field_0x754, 1.0f, BREG_F(3) + 3.0f);
        i_this->field_0x758 += BREG_S(1) + 4000;
    }

    if (actor->speed.y < 0.0f && i_this->objacch.ChkGroundHit()) {
        cLib_addCalc0(&i_this->field_0x75c, 1.0f, 30.0f);
        cLib_addCalc0(&i_this->field_0x754, 1.0f, 30.0f);
    }

    if (i_this->field_0x704) {
        s16 sVar4 = i_this->field_0x724.y - actor->shape_angle.y;
        i_this->field_0x7a4[3].y = 0x4000 + -sVar4;
        if (i_this->field_0x7a4[3].y > 6000) { // ~33°
            i_this->field_0x7a4[3].y = 6000;
        } else if (i_this->field_0x7a4[3].y < -9000) { // ~49.4°
            i_this->field_0x7a4[3].y = -9000;
        }

        i_this->field_0x7a4[4].y = 0x4000 + -sVar4;
        if (i_this->field_0x7a4[4].y > 17000) { // ~93.4°
            i_this->field_0x7a4[4].y = 17000;
        } else if (i_this->field_0x7a4[4].y < 0) {
            i_this->field_0x7a4[4].y = 0;
        }

        i_this->field_0x7a4[5].y = sVar4 + 0x4000;
        if (i_this->field_0x7a4[5].y > 6000) {
            i_this->field_0x7a4[5].y = 6000;
        } else if (i_this->field_0x7a4[5].y < -9000) {
            i_this->field_0x7a4[5].y = -9000;
        }

        i_this->field_0x7a4[6].y = sVar4 + 0x4000;
        if (i_this->field_0x7a4[6].y > 17000) {
            i_this->field_0x7a4[6].y = 17000;
        } else if (i_this->field_0x7a4[6].y < 0) {
            i_this->field_0x7a4[6].y = 0;
        }

        cMtx_YrotS(*calc_mtx, sVar4);
        work.x = 0.0f;
        work.y = 0.0f;
        work.z = BREG_F(8) + 12000.0f;
        MtxPosition(&work, &sp24c);
        i_this->field_0x836.y = -sp24c.x;
        i_this->field_0x836.x = -sp24c.z * 1.3f;
        
        if (i_this->field_0x836.x > (s16)(BREG_S(6) + 10000)) {
            i_this->field_0x836.x = BREG_S(6) + 10000;
        }

        i_this->field_0x836.x += BREG_S(7) + 6000;
    }

    static s16 j_spd[11] = {
        0x4000, 0x4000, 0x4000,
        0x1000, 0x1000, 0x1000,
        0x1000, 0x4000, 0x4000,
        0x4000, 0x4000
    };

    for (int i = 0; i < 11; i++) {
        cLib_addCalcAngleS2(&i_this->field_0x762[i].x, i_this->field_0x7a4[i].x, 2, j_spd[i]);
        cLib_addCalcAngleS2(&i_this->field_0x762[i].y, i_this->field_0x7a4[i].y, 2, j_spd[i]);
        cLib_addCalcAngleS2(&i_this->field_0x762[i].z, i_this->field_0x7a4[i].z, 2, j_spd[i]);
        i_this->field_0x7a4[i].x = i_this->field_0x7a4[i].z = 0;

        if (i >= 3 && i <= 6) {
            if (i_this->field_0x802[i - 3] == 0) {
                s16 max_step;
                if (i == 4 || i == 6) {
                    max_step = 0x600; // ~8.4°
                } else {
                    max_step = 0x800; // 11.25°
                }

                cLib_addCalcAngleS2(&i_this->field_0x7a4[i].y, 0, 1, max_step);
            } else {
                i_this->field_0x802[i - 3]--;
            }
        } else {
            i_this->field_0x7a4[i].y = 0;
        }

        if (i >= 3) {
            i_this->field_0x7ec[i] = i_this->field_0x7e8 * cM_scos(i_this->frame_counter * (BREG_S(0) + 4000) + i * (BREG_S(1) + 20000));
        }
    }

    cLib_addCalc0(&i_this->field_0x7e8, 1.0f, BREG_F(11) + 100.0f);
    cLib_addCalcAngleS2(&i_this->field_0x830.y, i_this->field_0x836.y, 2, 0x800);
    cLib_addCalcAngleS2(&i_this->field_0x830.x, i_this->field_0x836.x, 2, 0x800);
    i_this->field_0x836.y = i_this->field_0x836.x = 0;
    if (i_this->unk_timer_4 != 0) {
        i_this->unk_timer_4--;
    }

    f32 fVar1 = i_this->unk_timer_4 * (DREG_F(13) + 1.0f);
    if (fVar1 > 30.0f) {
        fVar1 = 30.0f;
    }
    fVar1 *= fVar1;

    for (int i = 0; i < 4; i++) {
        i_this->field_0x80e[i].x = fVar1 * (cM_ssin(i_this->frame_counter * (DREG_S(5) + 5000) + i * (DREG_S(7) + -15000)) * (DREG_F(18) + 5.0f));
        i_this->field_0x80e[i].y = fVar1 * (cM_ssin(i_this->frame_counter * (DREG_S(5) + 0x12C0) + i * (DREG_S(7) + -13000)) * (DREG_F(18) + 5.0f));
    }

    cLib_addCalcAngleS2(&i_this->jnt_tail_y_rot_offset, 0, 4, 0x100);
    if (i_this->unk_timer_3 != 0) {
        i_this->unk_timer_3--;
        if (i_this->field_0x724.x != 0) {
            Vec sp258;
            cXyz sp264, sp270;
            dBgS_GndChk gnd_chk;
            s16 sVar6, sVar7;
            sVar6 = 0;
            sVar7 = 0;
            fVar1 = 100.0f;

            MTXCopy(i_this->anm_p->getModel()->getAnmMtx(JNT_BACKBONE02), *calc_mtx);
            work.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&work, &sp264);
            sp264.y += 100.0f;
            sp264.y += 100.0f;
            gnd_chk.SetPos(&sp264);
            sp264.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

            if (sp264.y != -G_CM3D_F_INF) {
                sp258.x = sp264.x;
                sp258.y = sp264.y + 100.0f;
                sp258.z = sp264.z + fVar1;
                gnd_chk.SetPos(&sp258);
                sp258.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

                if (sp258.y != -G_CM3D_F_INF) {
                    f32 z, y;
                    y = sp258.y - sp264.y;
                    z = sp258.z - sp264.z;
                    sVar7 = -cM_atan2s(y, z);
                    if (sVar7 > 0x3000 || sVar7 < -0x3000) {
                        sVar7 = 0;
                    }
                }

                sp258.x = sp264.x + fVar1;
                sp258.y = sp264.y + 100.0f;
                sp258.z = sp264.z;
                gnd_chk.SetPos(&sp258);
                sp258.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
                if (sp258.y != -G_CM3D_F_INF) {
                    f32 x, y;
                    y = sp258.y - sp264.y;
                    x = sp258.x - sp264.x;
                    sVar6 = (s16)cM_atan2s(y, x);
                    if (sVar6 > 0x3000 || sVar6 < -0x3000) {
                        sVar6 = 0;
                    }
                }
            }

            dBgS_LinChk lin_chk;
            cXyz work, start, end;

            start = actor->current.pos;
            start.y += JREG_F(5) + 30.0f;
            J3DModel* model = i_this->anm_p->getModel();
            work.set(0.0f, 0.0f, 0.0f);
            MTXCopy(model->getAnmMtx((int)BREG_S(8) + JNT_NECK02), *calc_mtx);
            MtxPosition(&work, &end);
            end.y += JREG_F(6) + 30.0f;
            lin_chk.Set(&start, &end, actor);

            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                work = start - end;
                cMtx_YrotS(*calc_mtx, cM_atan2s(work.x, work.z));
                work.x = 0.0f;
                work.y = 0.0f;
                work.z = TREG_F(11) + 50.0f;
                MtxPosition(&work, &sp24c);
                actor->current.pos += sp24c;
            }

            i_this->field_0x730.x = sVar7;
            i_this->field_0x730.z = sVar6;
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0x72a.x, i_this->field_0x730.x, 1, 0x400);
    cLib_addCalcAngleS2(&i_this->field_0x72a.z, i_this->field_0x730.z, 1, 0x400);

    if (i_this->set_smoke_flag != 0) {
        J3DModel* model = i_this->anm_p->getModel();
        work.set(0.0f, 0.0f, 0.0f);

        static int foot_idx[2] = {
            JNT_FOOTL, JNT_FOOTR,
        };
        for (int i = 0; i < 2; i++) {
            if (i_this->set_smoke_flag == 3 || (i_this->set_smoke_flag == 2 && i == 1) || (i_this->set_smoke_flag == 1 && i == 0)) {
                MTXCopy(model->getAnmMtx(foot_idx[i]), *calc_mtx);
                MtxPosition(&work, &sp24c);
                fopAcM_effSmokeSet2(&i_this->field_0x10e8[i], &i_this->field_0x10f0[i], &sp24c, &actor->shape_angle,
                                    TREG_F(8) + 1.0f, &actor->tevStr);
            }
        }

        i_this->set_smoke_flag = 0;
    }

    if (i_this->unk_timer_6 != 0) {
        i_this->unk_timer_6--;
        if (i_this->unk_timer_6 == 0) {
            dComIfGp_particle_set(ID_ZI_J_TUBA00, &actor->eyePos, &actor->shape_angle, NULL);
        }
    }

    cXyz sp2a4(1.3f, 1.3f, 1.3f);
    setMidnaBindEffect(actor, &i_this->sound, &actor->eyePos, &sp2a4);
}

static void anm_se_set(e_dn_class* i_this) {
    int frame = i_this->anm_p->getFrame();

    if (i_this->anm_no == ANM_ATTACK_01) {
        if (frame == 35) {
            i_this->sound.startCreatureSound(Z2SE_EN_DN_TAIL, 0, -1);
        } else if (frame == 28) {
            i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_TAIL, -1);
        }
    } else if (i_this->anm_no == ANM_ATTACK_TAIL_01) {
        if (frame == 18) {
            i_this->sound.startCreatureSound(Z2SE_EN_DN_KNIFE, 0, -1);
        }
    } else if (i_this->anm_no == ANM_ATTACK_TAIL_02) {
        if (frame == 10) {
            i_this->sound.startCreatureSound(Z2SE_EN_DN_KNIFE2_A, 0, -1);
        }

        if (frame == 24) {
            i_this->sound.startCreatureSound(Z2SE_EN_DN_KNIFE2_B, 0, -1);
        }
    } else if (i_this->anm_no == ANM_WALK) {
        if (frame == 3) {
            i_this->sound.startCreatureSound(Z2SE_EN_DN_FN_L, 0, -1);
        } else if (frame == 18) {
            i_this->sound.startCreatureSound(Z2SE_EN_DN_FN_R, 0, -1);
        }
    } else if (i_this->anm_no == ANM_RUN) {
        if (frame == 10) {
            i_this->sound.startCreatureSound(Z2SE_EN_DN_FN_RUN_L, 0, -1);
        } else if (frame == 4) {
            i_this->sound.startCreatureSound(Z2SE_EN_DN_FN_RUN_R, 0, -1);
            i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_BREATH, -1);
        }
    } else if (i_this->anm_no == ANM_WAIT_03) {
        if (frame == 15) {
            i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_SEARCH, -1);
        }
    } else if (i_this->anm_no == ANM_WAIT_04 && frame == 15) {
        i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_SEARCH2, -1);
    }
}

int c_start;

static int daE_DN_Execute(e_dn_class* i_this) {
    if (i_this->status != 0) {
        return 1;
    }

    if (c_start == 0 && dComIfGp_event_runCheck() && 
        (fopAcM_getTalkEventPartner((fopAc_ac_c*)daPy_getLinkPlayerActorClass()) == daPy_py_c::getMidnaActor() || daPy_getPlayerActorClass()->eventInfo.checkCommandDoor())) {
        return 1;
    }

    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz work;
    cXyz pos;
    cXyz downPos;
    i_this->frame_counter++;
    for (int i = 0; i < 4; i++) {
        if (i_this->timer[i] != 0) {
            i_this->timer[i]--;
        }
    }

    if (i_this->invulnerability_timer != 0) {
        i_this->invulnerability_timer--;
    }

    if (i_this->unk_timer_2 != 0) {
        i_this->unk_timer_2--;
    }

    if (i_this->unk_timer_5 != 0) {
        i_this->unk_timer_5--;
    }

    if (i_this->unk_timer_1 != 0) {
        i_this->unk_timer_1--;
    }

    i_this->field_0x6f2 = 1;
    action(i_this);

    if (i_this->unk_timer_5 == 0 && i_this->field_0x5d8 == 0) {
        if (i_this->field_0x704) {
            cMtx_YrotS(*calc_mtx, i_this->field_0x724.y);
            work.x = 0.0f;
            work.y = TREG_F(8);
            work.z = i_this->field_0x83c;
            MtxPosition(&work, &pos);
            actor->current.pos += pos;
            actor->old.pos += pos;
            cLib_addCalc2(&i_this->field_0x83c, i_this->field_0x704 * (2.0f + TREG_F(9)), 1.0f, 5.0f + TREG_F(17));
        } else {
            cLib_addCalc0(&i_this->field_0x83c, 1.0f, 5.0f + TREG_F(17));
        }

        i_this->objacch.CrrPos(dComIfG_Bgsp());
        if (i_this->field_0x704) {
            actor->current.pos -= pos;
            actor->old.pos -= pos;
        }


        if (!i_this->objacch.ChkGroundHit()) {
            if (actor->speed.y < -10.0f) {
                i_this->unk_counter_2++;
                if (i_this->unk_counter_2 == 13) {
                    i_this->sound.startCreatureVoice(Z2SE_EN_DN_V_DEATH, -1);
                }
            }
        } else {
            if (i_this->unk_counter_2 >= 13 + ZREG_S(8)) {
                i_this->action = ACTION_DAMAGE;
                i_this->mode = 0;
                actor->speed.y = 0.0f;
                i_this->field_0x704 = -1.0;
                i_this->field_0x724.x = -0x3000;
                i_this->unk_timer_4 = 50 + DREG_S(2);
                i_this->invulnerability_timer = 1000;
                actor->health = 0;
            }

            i_this->unk_counter_2 = 0;
        }
    }

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y + i_this->cur_pos_y_offset, actor->current.pos.z);
    mDoMtx_stack_c::XrotM((s16) i_this->field_0x72a.x);
    mDoMtx_stack_c::ZrotM((s16) i_this->field_0x72a.z);
    mDoMtx_stack_c::YrotM((s16) i_this->field_0x724.y);
    mDoMtx_stack_c::XrotM((s16) i_this->field_0x724.x);
    mDoMtx_stack_c::YrotM(-i_this->field_0x724.y);
    mDoMtx_stack_c::YrotM((s16) actor->shape_angle.y);
    mDoMtx_stack_c::XrotM((s16) actor->shape_angle.x);
    mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.model_size * actor->scale.x, l_HIO.model_size * actor->scale.x, l_HIO.model_size * actor->scale.x);

    J3DModel* model = i_this->anm_p->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->anm_p->play(0, 0);
    anm_se_set(i_this);
    i_this->anm_p->modelCalc();
    MTXCopy(model->getAnmMtx(JNT_BACKBONE02), *calc_mtx);

    work.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&work, &pos);
    work.set(YREG_F(1), YREG_F(2), -200.0f + YREG_F(3));
    MtxPosition(&work, &downPos);
    if (pos.y < downPos.y) {
        i_this->left_right_flag = 0;
    }
    else {
        i_this->left_right_flag = 1;
    }

    cXyz sp50(0.0f, 0.0f, 0.0f);
    if (i_this->invulnerability_timer != 0) {
        sp50.set(-20000.0f, 200000.0f, 30000.0f);
    }

    MTXCopy(model->getAnmMtx(JNT_HEAD), *calc_mtx);
    work.set(30.0f + KREG_F(12), 30.0f + KREG_F(13), 0.0f + KREG_F(14));
    MtxPosition(&work, &actor->eyePos);

    i_this->cc_sph[0].SetC(actor->eyePos + sp50);
    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        i_this->cc_sph[0].SetR(70.0f);
    } else {
        i_this->cc_sph[0].SetR(50.0f);
    }
    
    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += 70.0f + BREG_F(7);

    MTXCopy(model->getAnmMtx(JNT_BACKBONE01), *calc_mtx);
    work.set(-30.0f + BREG_F(11), BREG_F(12), BREG_F(13));
    MtxPosition(&work, &pos);
    i_this->cc_sph[1].SetC(pos + sp50);
    i_this->cc_sph[1].SetR(45.0f);

    MTXCopy(model->getAnmMtx(JNT_BACKBONE02), *calc_mtx);
    work.set(20.0f + BREG_F(14), BREG_F(15), BREG_F(16));
    MtxPosition(&work, &pos);
    i_this->cc_sph[2].SetC(pos + sp50);
    i_this->cc_sph[2].SetR(45.0f);

    for (int i = 0; i <= 2; i++) {
        if (i_this->field_0x6f2 != 0) {
            i_this->cc_sph[i].OnCoSetBit();
        } else {
            i_this->cc_sph[i].OffCoSetBit();
        }

        dComIfG_Ccsp()->Set(&i_this->cc_sph[i]);
    }

    MTXCopy(i_this->anm_p->getModel()->getAnmMtx(JNT_HANDR1), *calc_mtx);
    i_this->knife_model->setBaseTRMtx(*calc_mtx);

    if (i_this->at_chk_flag != 0 && i_this->tail_at_sph_flag <= 1) {
        work.set(BREG_F(7), 60.0f + BREG_F(8), BREG_F(9));
        MtxPosition(&work, &pos);
        if (i_this->at_sph_center_flag == 0) {
            i_this->at_sph_center_flag = 1;
            i_this->at_sph.StartCAt(pos);
        } else {
            i_this->at_sph.MoveCAt(pos);
        }

        if (i_this->tail_at_sph_flag == 1) {
            i_this->at_sph.SetR(l_HIO.model_size * 80.0f);
        } else {
            i_this->at_sph.SetR(l_HIO.model_size * 50.0f);
        }

        i_this->at_sph.SetAtAtp(1);
    }

    if (i_this->skull_model != NULL) {
        if (i_this->field_0x694 == 0) {
            MTXCopy(i_this->anm_p->getModel()->getAnmMtx(JNT_HEAD), mDoMtx_stack_c::get());
            mDoMtx_stack_c::multVecZero(&i_this->skull_trans);
        } else {
            i_this->skull_trans += i_this->field_0x6a4;
            if (i_this->field_0x6a4.y <= i_this->objacch.GetGroundH()) {
                i_this->skull_model = NULL;
            }

            i_this->field_0x6a4.y -= 5.0f;
            mDoMtx_stack_c::transS(i_this->skull_trans.x, i_this->skull_trans.y, i_this->skull_trans.z);
            mDoMtx_stack_c::YrotM(i_this->skull_y_rot);
            mDoMtx_stack_c::XrotM(i_this->skull_x_rot);
        }

        i_this->skull_model->setBaseTRMtx(mDoMtx_stack_c::get());
    }
    
    MTXCopy(i_this->anm_p->getModel()->getAnmMtx(JNT_ARML2), *calc_mtx);
    i_this->tate_model->setBaseTRMtx(*calc_mtx);

    if (i_this->at_chk_flag != 0 && i_this->tail_at_sph_flag == 2) {
        MTXCopy(i_this->anm_p->getModel()->getAnmMtx(JNT_TAIL04), *calc_mtx);
        work.set(BREG_F(7), BREG_F(8), BREG_F(9));
        MtxPosition(&work, &pos);

        if (i_this->at_sph_center_flag == 0) {
            i_this->at_sph_center_flag = 1;
            i_this->at_sph.StartCAt(pos);
        }  else {
            i_this->at_sph.MoveCAt(pos);
        }

        i_this->at_sph.SetAtAtp(2);
        i_this->at_sph.SetR((70.0f + BREG_F(10)) * l_HIO.model_size);
    }

    if (i_this->at_chk_flag == 0) {
        cXyz sp68(-23500.0f, -20000.0f, -50000.0f);
        i_this->at_sph.SetC(actor->current.pos + sp68);
        i_this->at_sph_center_flag = 0;
    } else {
        i_this->at_chk_flag = 0;
    }

    dComIfG_Ccsp()->Set(&i_this->at_sph);

    if (i_this->guard_flag != 0 && i_this->unk_timer_2 == 0) {
        MTXCopy(i_this->anm_p->getModel()->getAnmMtx(JNT_ARML2), *calc_mtx);
        work.set(50.0f + JREG_F(5), JREG_F(6), 30.0f + JREG_F(7));
        MtxPosition(&work, &pos);
        i_this->tate_sph.SetC(pos);
        i_this->tate_sph.SetR((70.0f + JREG_F(8)) * l_HIO.model_size);

        if (i_this->tate_sph.ChkTgHit()) {
            def_se_set(&i_this->sound, i_this->tate_sph.GetTgHitObj(), 40, 0);
            dScnPly_c::setPauseTimer(l_HIO.defense_pause_time);
            i_this->unk_timer_2 = 10;
            cMtx_YrotS(*calc_mtx, (s16)actor->shape_angle.y);
            work.x = KREG_F(0);
            work.y = 120.0f + KREG_F(1);
            work.z = 50.0f + KREG_F(2);
            MtxPosition(&work, &pos);
            pos += actor->current.pos;
            dComIfGp_setHitMark(2, NULL, &pos, &actor->shape_angle, NULL, 0);
        }
    } else {
        cXyz sp74(-23500.0f, -20000.0f, -30000.0f);
        i_this->tate_sph.SetC(actor->current.pos + sp74);
    }

    i_this->guard_flag = 0;
    dComIfG_Ccsp()->Set(&i_this->tate_sph);
    if (i_this->death_flag != 0) {
        cLib_addCalc2(&i_this->color, -20.0f, 1.0f, 0.4f);
    }

    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    MTXCopy(i_this->anm_p->getModel()->getAnmMtx(JNT_BACKBONE02), mDoMtx_stack_c::get());
    mDoMtx_stack_c::multVecZero(&pos);
    work = player->current.pos - pos;
    s16 tans_val = cM_atan2s(work.x, work.z);
    cMtx_YrotS(*calc_mtx, tans_val);
    work.x = 0.0;
    work.y = NREG_F(18) + 45.0f;
    work.z = (BREG_F(12) + 60.0f) - 30.0f;
    MtxPosition(&work, &downPos);
    downPos += pos;
    // The following pokes a hole in the "c-style actors don't inherit" theory....
    ((fopEn_enemy_c*) i_this)->setDownPos(&downPos);
    cXyz sp80(actor->eyePos);
    sp80.y += NREG_F(7) + 180.0f;
    ((fopEn_enemy_c*) i_this)->setHeadLockPos(&sp80);

    actor->attention_info.flags |= fopAc_AttnFlag_UNK_0x200000;

    if ((actor->current.pos.y - actor->home.pos.y) < -5000.0f) {
        fopAcM_delete(actor);
        OS_REPORT("DN NARAKU\n");
        int swBit = (fopAcM_GetParam(actor) & 0xFF000000) >> 24;
        if (swBit != 0xFF) {
            dComIfGs_onSwitch(swBit, fopAcM_GetRoomNo(actor));
        }
    }

    // City in the Sky - Dynalfos Room:
    //  i.e. where you can scare the 2 lizard dudes with the clawshot, make them fall to their deaths,
    //  then the fan in the middle turns on.
    if (strcmp(dComIfGp_getStartStageName(), "D_MN07") == 0 && fopAcM_GetRoomNo(actor) == 8
        && actor->health != 0 && actor->current.pos.y <= -300.0f) {
        i_this->action = ACTION_DAMAGE;
        i_this->mode = 0;
        i_this->field_0x704 = 0.0;
        actor->health = 0;
    }

    return 1;
}

static int daE_DN_IsDelete(e_dn_class* i_this) {
    return 1;
}

static int daE_DN_Delete(e_dn_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    fpc_ProcID no = fopAcM_GetID(i_this);

    dComIfG_resDelete(&i_this->phase, "E_dn");

    if (i_this->hio_init != 0) {
        hio_set = 0;
        mDoHIO_DELETE_CHILD(l_HIO.no);
    }

    if (actor->heap != NULL) {
        i_this->anm_p->stopZelAnime();
    }

    return 1;
}

Vec jv_offset = { 0.0f, 0.0f, 0.0f };

dJntColData_c jc_data[12] = {
    {0, 1, 2, 40.0f, &jv_offset}, {0, 1, 3, 30.0f, &jv_offset},
    {0, 1, 22, 40.0f, &jv_offset}, {0, 1, 11, 15.0f, &jv_offset},
    {0, 1, 12, 10.0f, &jv_offset}, {0, 1, 17, 15.0f, &jv_offset},
    {0, 1, 18, 10.0f, &jv_offset}, {0, 1, 23, 20.0f, &jv_offset},
    {0, 1, 24, 15.0f, &jv_offset}, {0, 1, 27, 20.0f, &jv_offset},
    {0, 1, 28, 15.0f, &jv_offset}, {0, 1, 32, 20.0f, &jv_offset},
};

static int useHeapInit(fopAc_ac_c* actor) {
    e_dn_class* i_this = (e_dn_class*)actor;

    i_this->anm_p = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_dn", 38), NULL, NULL,
                                               (J3DAnmTransform*)dComIfG_getObjectRes("E_dn", 31), 0, 1.0f,
                                               0, -1, &i_this->sound, 0x80000, 0x11000084);
    if (i_this->anm_p == NULL || i_this->anm_p->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = i_this->anm_p->getModel();
    model->setUserArea((uintptr_t)i_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        if (i != 0) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
    }

    if (i_this->jc.init(actor, &jc_data[0], model, 12) == 0) {
        return 0;
    }

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_dn", 39);
    JUT_ASSERT(5420, modelData != NULL);
    i_this->knife_model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->knife_model == NULL) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("E_dn", 41);
    JUT_ASSERT(5432, modelData != NULL);
    i_this->tate_model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->tate_model == NULL) {
        return 0;
    }

    if (i_this->arg1 == 1) {
        modelData = (J3DModelData*)dComIfG_getObjectRes("E_dn", 40);
        JUT_ASSERT(5443, modelData != NULL);
        i_this->skull_model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->skull_model == NULL) {
            return 0;
        }
    }

    return 1;
}

static cPhs__Step daE_DN_Create(fopAc_ac_c* actor) {
    e_dn_class* i_this = (e_dn_class*)actor;

    fopAcM_ct(actor, e_dn_class);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->phase, "E_dn");
    if (phase == cPhs_COMPLEATE_e) {
        int swBit = (fopAcM_GetParam(actor) & 0xFF000000) >> 24;
        if (swBit != 0xFF) {
            if (dComIfGs_isSwitch(swBit, fopAcM_GetRoomNo(actor))) {
                return cPhs_ERROR_e;
            }
        }

        OS_REPORT("E_dn PARAM %x\n", fopAcM_GetParam(actor));
        i_this->arg0 = fopAcM_GetParam(actor);
        i_this->arg3 = 1;
        i_this->arg1 = (fopAcM_GetParam(actor) & 0xF00) >> 8;
        i_this->arg2 = (fopAcM_GetParam(actor) & 0xF000) >> 12;

        if ((actor->home.angle.z & 0xFF) == 0 || (actor->home.angle.z & 0xFF) == 0xFF) {
            i_this->pl_range = 100000.0f;
        } else {
            i_this->pl_range = (actor->home.angle.z & 0xFF) * 100.0f;
        }

        OS_REPORT("E_dn AZ %x\n", actor->home.angle.z);
        OS_REPORT("E_dn PR %d\n", (int)i_this->pl_range);
        actor->home.angle.z = actor->current.angle.z = actor->shape_angle.z = 0;
        u8 param = (fopAcM_GetParam(actor) & 0xFF0000) >> 16;
        OS_REPORT("E_dn//////////////E_DN SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(actor, useHeapInit, 0x3BF0)) {
            OS_REPORT("//////////////E_DN SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (hio_set == 0) {
            i_this->hio_init = 1;
            hio_set = 1;
            l_HIO.no = mDoHIO_CREATE_CHILD("リザードマン", &l_HIO);
        }

        fopAcM_OnStatus(actor, fopAcM_STATUS_UNK_0x100);
        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(actor, i_this->anm_p->getModel()->getBaseTRMtx());
        fopAcM_SetMin(actor, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(actor, 200.0f, 200.0f, 200.0f);

        i_this->objacch.Set(fopAcM_GetPosition_p(actor), fopAcM_GetOldPosition_p(actor), actor, 1,
                             &i_this->acchcir, fopAcM_GetSpeed_p(actor), NULL, NULL);
        i_this->acchcir.SetWall(80.0f, 100.0f);

        if (i_this->arg1 == 1) {
            actor->field_0x560 = actor->health = 100;
        } else {
            actor->field_0x560 = actor->health = 100;
        }
        i_this->stts.Init(200, 0, actor);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xD8FBFDFf, 0x3}, 0x75}}, // mObj
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
            i_this->cc_sph[i].SetStts(&i_this->stts);
        }

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xD}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
            } // mSphAttr
        };
        i_this->at_sph.Set(at_sph_src);
        i_this->at_sph.SetStts(&i_this->stts);

        static dCcD_SrcSph tate_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xD8FBFDFF, 0x3}, 0x0}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_STONE, 0x5, 0x0, 0x0, 0x3}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 100.0f} // mSph
            } // mSphAttr
        };
        i_this->tate_sph.Set(tate_sph_src);
        i_this->tate_sph.SetStts(&i_this->stts);

        i_this->frame_counter = (int)cM_rndF(65535.0f) & 0xFF00;

        if (i_this->arg0 == 0) {
            i_this->action = ACTION_STAY;
        }

        if ((i_this->arg0 == 13 || i_this->arg0 == 14) || (i_this->arg0 == 15 || i_this->arg0 == 16)) {
            i_this->status = 1;
            i_this->action = ACTION_REG;
            fopAcM_OnStatus(actor, fopAcM_STATUS_UNK_0x4000);
            i_this->acchcir.SetWall(80.0f, 2.0f);
        }

        i_this->sound.init(&actor->current.pos, &actor->eyePos, 3, 1);
        i_this->sound.setEnemyName("E_dn");
        i_this->at_info.mpSound = &i_this->sound;

        c_start = 1;
        daE_DN_Execute(i_this);
        c_start = 0;
    }

    return phase;
}

e_dn_class::e_dn_class() {}

AUDIO_INSTANCES;

static actor_method_class l_daE_DN_Method = {
    (process_method_func)daE_DN_Create,
    (process_method_func)daE_DN_Delete,
    (process_method_func)daE_DN_Execute,
    (process_method_func)daE_DN_IsDelete,
    (process_method_func)daE_DN_Draw,
};

actor_process_profile_definition g_profile_E_DN = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_DN,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_dn_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  118,                    // mPriority
  &l_daE_DN_Method,       // sub_method
  0x00040000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
