/**
 * @file d_a_e_bs.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_bs.h"
#include "f_op/f_op_actor_enemy.h"

class daE_BS_HIO_c : public JORReflexible {
public:
    daE_BS_HIO_c();
    virtual ~daE_BS_HIO_c() {}

    void genMessage(JORMContext* ctx);

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 base_size;
    /* 0x0C */ f32 move_speed;
    /* 0x10 */ f32 rush_speed;
    /* 0x14 */ f32 pl_recognize_dist;
    /* 0x18 */ f32 attack_start_range;
    /* 0x1C */ f32 battle_start_range;
};

enum daE_BS_ACTION {
    ACTION_APPEAR,
    ACTION_NORMAL,
    ACTION_FIGHT_RUN,
    ACTION_ATTACK,
    ACTION_DAMAGE,
};

daE_BS_HIO_c::daE_BS_HIO_c() {
    id = -1;
    base_size = 1.0f;
    move_speed = 3.0f;
    pl_recognize_dist = 3000.0f;
    rush_speed = 6.0f;
    attack_start_range = 200.0f;
    battle_start_range = 200.0f;
}

static void wepon_anm_init(e_bs_class* i_this, int i_anm) {
    i_this->weponModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_BS", i_anm), 2, 5.0f, 1.0f, 0.0f, -1.0f, NULL);
}

static void anm_init(e_bs_class* i_this, int i_anm, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->modelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_BS", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
    i_this->anm = i_anm;

    if (i_anm == 7 || i_anm == 4 || i_anm == 9) {
        wepon_anm_init(i_this, 10);
    } else {
        wepon_anm_init(i_this, 11);
    }
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_bs_class* a_this = (e_bs_class*)model->getUserArea();

        if (a_this != NULL) {
            MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
            if (jnt_no == 10) {
                cMtx_YrotM(*calc_mtx, a_this->head_rot_y);
                cMtx_XrotM(*calc_mtx, (a_this->field_0x6aa + a_this->field_0x6b2));
                cMtx_ZrotM(*calc_mtx, -a_this->head_rot_z);
            }

            model->setAnmMtx(jnt_no, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int daE_BS_Draw(e_bs_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    J3DModel* model = i_this->modelMorf->getModel();
    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);
    i_this->modelMorf->entryDL();

    J3DModel* wep_model = i_this->weponModelMorf->getModel();
    g_env_light.setLightTevColorType_MAJI(wep_model, &actor->tevStr);
    i_this->weponModelMorf->entryDL();

    if (i_this->is_draw_shadow) {
        cXyz sp8;
        sp8.set(actor->current.pos.x, 50.0f + actor->current.pos.y + BREG_F(18), actor->current.pos.z);

        i_this->shadowId = dComIfGd_setShadow(i_this->shadowId, 1, model, &sp8, 600.0f + BREG_F(19), 0.0f, actor->current.pos.y, i_this->acch.GetGroundH(), i_this->acch.m_gnd, &actor->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
        dComIfGd_addRealShadow(i_this->shadowId, wep_model);
    }

    return 1;
}

static BOOL player_way_check(e_bs_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    s16 angle_diff = actor->shape_angle.y - player->shape_angle.y;
    if (angle_diff < 0) {
        angle_diff = -angle_diff;
    }
    
    if ((u16)angle_diff < 0x6000) {
        return FALSE;
    }

    return TRUE;
}

static BOOL way_bg_check(e_bs_class* i_this, f32 i_dist, f32 i_height) {
    fopAc_ac_c* actor = &i_this->enemy;
    dBgS_LinChk linchk;
    cXyz offset;
    cXyz start;
    cXyz end;

    start = actor->current.pos;
    start.y += i_height;

    cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
    offset.x = 0.0f;
    offset.y = i_height;
    offset.z = i_dist;
    MtxPosition(&offset, &end);
    end += actor->current.pos;

    linchk.Set(&start, &end, actor);
    if (dComIfG_Bgsp().LineCross(&linchk)) {
        return TRUE;
    }

    return FALSE;
}

static int pl_check(e_bs_class* i_this, f32 i_range, s16 i_angle) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    
    if (i_this->player_dist < i_range) {
        s16 angle_diff = actor->shape_angle.y - i_this->angleY_to_player;
        if (angle_diff < i_angle && angle_diff > (s16)-i_angle && !fopAcM_otherBgCheck(actor, player)) {
            return 1;
        }
    }

    for (int i = 0; i < 2; i++) {
        if (i_this->ccSph[i].ChkCoHit()) {
            return 2;
        }
    }

    return 0;
}

static void e_bs_appear(e_bs_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    i_this->invulnerabilityTimer = 5;

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 4, 0.0f, 0, 0.0f);
        i_this->mode = 1;
        i_this->timers[0] = cM_rndF(20.0f);
        /* fallthrough */
    case 1:
        if (i_this->timers[0] == 0 && i_this->player_dist < i_this->appear_range) {
            i_this->mode = 2;
            i_this->modelMorf->setPlaySpeed(1.0f);
            actor->shape_angle.y = actor->current.angle.y = i_this->angleY_to_player;
            i_this->is_draw_shadow = TRUE;

            for (int i = 0; i <= 1; i++) {
                static u16 ap_name[] = {0x81D8, 0x81D9};
                dComIfGp_particle_set(ap_name[i], &actor->current.pos, &actor->shape_angle, NULL);
            }

            i_this->sound.startCreatureSound(Z2SE_EN_BS_APPEAR, 0, -1);
        }
        break;
    case 2:
        if (i_this->modelMorf->isStop()) {
            i_this->action = ACTION_FIGHT_RUN;
            i_this->mode = 0;
        }
        break;
    }
}

static u8 hio_set;

static daE_BS_HIO_c l_HIO;

static void e_bs_normal(e_bs_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp28;
    cXyz sp1C;
    f32 move_speed = 0.0f;
    s16 search_angle = 0x4000;

    switch (i_this->mode) {
    case 0:
    case 1:
    case 2:
        if (i_this->timers[0] == 0) {
            s16 var_r27;
            if (way_bg_check(i_this, 200.0f, 50.0f)) {
                var_r27 = 32768.0f + cM_rndFX(10000.0f);
                i_this->timers[1] = 40;
            } else {
                sp1C.x = actor->home.pos.x + cM_rndFX(600.0f);
                sp1C.y = actor->home.pos.y;
                sp1C.z = actor->home.pos.z + cM_rndFX(600.0f);
                sp28 = sp1C - actor->current.pos;

                var_r27 = cM_atan2s(sp28.x, sp28.z) - actor->current.angle.y;
                if (var_r27 > 0x3000) {
                    var_r27 = 0x3000;
                } else if (var_r27 < -0x3000) {
                    var_r27 = -0x3000;
                }
            }

            i_this->field_0x5c8 = actor->current.angle.y + var_r27;

            anm_init(i_this, 9, 5.0f, 2, 1.0f);
            i_this->mode = 3;
            i_this->timers[0] = 100.0f + cM_rndF(100.0f);
        } else {
            search_angle = 0x7000;
        }
        break;
    case 3:
        move_speed = l_HIO.move_speed;
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->field_0x5c8, 8, 0x400);

        if (i_this->timers[0] == 0 || (i_this->timers[1] == 0 && way_bg_check(i_this, 200.0f, 50.0f))) {
            i_this->mode = 2;
            i_this->timers[0] = 50.0f + cM_rndF(100.0f);
            anm_init(i_this, 7, 10.0f, 2, 1.0f);
        }
        break;
    }

    cLib_addCalc2(&actor->speedF, move_speed, 1.0f, 3.0f);

    BOOL spC = fopAcM_otoCheck(actor, 2000.0f);
    if (((i_this->counter & 15) == 0 || spC) && (spC || pl_check(i_this, i_this->pl_recognize_dist, search_angle))) {
        i_this->action = ACTION_FIGHT_RUN;
        i_this->mode = -10;
        i_this->timers[0] = 60;
    }
}

static void e_bs_fight_run(e_bs_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp18;
    cXyz spC;
    f32 move_speed = 0.0f;
    int anm_frame = i_this->modelMorf->getFrame();
    
    switch (i_this->mode) {
    case -10:
        anm_init(i_this, 7, 5.0f, 2, 1.0f);
        i_this->timers[1] = 15.0f + cM_rndF(10.0f);
        i_this->mode = -9;
        break;
    case -9:
        if (i_this->timers[1] == 0) {
            i_this->mode = 0;
        }
        break;
    case 0:
        anm_init(i_this, 9, 5.0f, 2, 2.0f + cM_rndFX(0.1f));
        i_this->mode = 1;
        /* fallthrough */
    case 1:
        move_speed = l_HIO.rush_speed;
        if (i_this->player_dist < l_HIO.attack_start_range) {
            i_this->mode = 2;
            i_this->modelMorf->setPlaySpeed(1.5f);
        }
        break;
    case 2:
        move_speed = l_HIO.move_speed;
        if (i_this->player_dist > (30.0f + l_HIO.attack_start_range)) {
            i_this->mode = 0;
        } else if (i_this->player_dist < (l_HIO.attack_start_range - 30.0f)) {
            if (cM_rndF(1.0f) < 0.35f) {
                i_this->mode = 3;
                anm_init(i_this, 9, 5.0f, 2, -1.5f);
            } else {
                i_this->mode = 5;
                i_this->timers[0] = 20.0f + cM_rndF(20.0f);
                anm_init(i_this, 8, 5.0f, 2, 1.0f);
            }
        }
        break;
    case 3:
        move_speed = -l_HIO.move_speed;
        if (i_this->player_dist > l_HIO.attack_start_range) {
            if (cM_rndF(1.0f) < 0.35f) {
                i_this->mode = 2;
            } else {
                i_this->mode = 5;
                i_this->timers[0] = 20.0f + cM_rndF(20.0f);
                anm_init(i_this, 8, 5.0f, 2, 1.0f);
            }
        }
        break;
    case 5:
        if (i_this->timers[0] == 0 || i_this->player_dist > (50.0f + l_HIO.attack_start_range)) {
            i_this->mode = 0;
        }
        break;
    }

    cLib_addCalc2(&actor->speedF, move_speed, 1.0f, 3.0f);

    if (i_this->mode >= 0) {
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->angleY_to_player, 4, 0x800);
    }

    if (!pl_check(i_this, 50.0f + i_this->pl_recognize_dist, 0x7FFF) && i_this->timers[0] == 0) {
        i_this->action = ACTION_NORMAL;
        i_this->mode = 0;
        i_this->timers[0] = 50.0f + cM_rndF(50.0f);
        anm_init(i_this, 7, 10.0f, 2, 1.0f);
        return;
    }

    if (i_this->angleX_to_player < 0x1000 && i_this->angleX_to_player > -0x1000 && player_way_check(i_this) && i_this->player_dist < l_HIO.battle_start_range && i_this->timers[2] == 0) {
        i_this->timers[2] = 10.0f + cM_rndF(20.0f);
        
        f32 attack_chance = 0.5f;
        if (cM_rndF(1.0f) < attack_chance) {
            i_this->action = ACTION_ATTACK;
            i_this->mode = 0;
        }
    }

    i_this->field_0x6a8 = 1;
}

static fopAc_ac_c* at_hit_check(e_bs_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (i_this->mode >= 10) {
        return NULL;
    }

    if (i_this->atSph.ChkAtHit()) {
        return dCc_GetAc(i_this->atSph.GetAtHitObj()->GetAc());
    }

    return NULL;
}

static void e_bs_attack(e_bs_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp18;
    cXyz spC;

    int anm_frame = i_this->modelMorf->getFrame();
    i_this->field_0x6a8 = 1;

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 5, 5.0f + TREG_F(14), 0, 1.0f);
        i_this->mode = 1;
        break;
    case 1:
        if (anm_frame < 10) {
            cLib_addCalcAngleS2(&actor->current.angle.y, i_this->angleY_to_player, 2, 0x800);
        }
        
        if (anm_frame >= 13 && anm_frame <= 20) {
            i_this->is_wep_attack = TRUE;
            if (anm_frame == 13) {
                i_this->sound.startCreatureSound(Z2SE_EN_BS_ATTACK_SPEAR, 0, -1);
            }
        }

        if (i_this->modelMorf->isStop()) {
            i_this->action = ACTION_FIGHT_RUN;
            i_this->mode = 0;
        }
        break;
    }

    cLib_addCalc0(&actor->speedF, 1.0f, 5.0f);

    if (i_this->is_wep_attack) {
        fopAc_ac_c* at_hit_actor = at_hit_check(i_this);
        if (at_hit_actor != NULL && fopAcM_GetName(at_hit_actor) == PROC_ALINK && daPy_getPlayerActorClass()->checkPlayerGuard()) {
            i_this->modelMorf->setPlaySpeed(0.0f);
            i_this->action = ACTION_FIGHT_RUN;
            i_this->mode = 0;
            i_this->timers[2] = 10.0f + cM_rndF(20.0f);
        }
    }
}

static void e_bs_damage(e_bs_class* i_this) {
    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 6, 1.0f + TREG_F(14), 0, 1.0f);
        i_this->mode = 1;
        break;
    case 1:
        if (i_this->modelMorf->isStop()) {
            i_this->action = ACTION_FIGHT_RUN;
            i_this->mode = 0;
        }
        break;
    }
}

static void damage_check(e_bs_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    
    if (i_this->invulnerabilityTimer == 0) {
        i_this->ccStts.Move();

        dCcU_AtInfo sp1C;
        for (int i = 0; i < 2; i++) {
            if (i_this->ccSph[i].ChkTgHit()) {
                i_this->atInfo.mpCollider = i_this->ccSph[i].GetTgHitObj();
                cc_at_check(actor, &i_this->atInfo);

                if (i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                    actor->health -= 10;
                }

                if (i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK | AT_TYPE_WOLF_CUT_TURN | AT_TYPE_10000000 | AT_TYPE_MIDNA_LOCK)) {
                    i_this->invulnerabilityTimer = 20;
                } else {
                    i_this->invulnerabilityTimer = 10;
                }

                if (i_this->atInfo.mAttackPower <= 1) {
                    i_this->invulnerabilityTimer = KREG_S(8) + 10;
                }

                if (actor->health <= 0) {
                    for (int j = 0; j <= 2; j++) {
                        static u16 ap_name[] = {0x81DA, 0x81DB, 0x81DC};
                        dComIfGp_particle_set(ap_name[j], &actor->current.pos, &actor->shape_angle, NULL);
                    }

                    i_this->sound.startCreatureVoice(Z2SE_EN_BS_V_DEAD, -1);
                    i_this->sound.startCreatureSound(Z2SE_EN_BS_DEAD, 0, -1);

                    fopAcM_delete(actor);
                    fopAcM_createItemFromEnemyID(0x1F, &actor->current.pos, -1, -1, NULL, NULL, NULL, NULL);

                    int swbit = (fopAcM_GetParam(actor) & 0xFF0000) >> 0x10;
                    if (swbit != 0xFF) {
                        dComIfGs_onSwitch(swbit, fopAcM_GetRoomNo(actor));
                    }
                } else {
                    i_this->action = ACTION_DAMAGE;
                    i_this->mode = 0;

                    i_this->sound.startCreatureVoice(Z2SE_EN_BS_V_DAMAGE, -1);
                    i_this->sound.startCreatureSound(Z2SE_EN_BS_SHAKE_BONES, 0, -1);

                    i_this->field_0x6b8 = 30.0f + YREG_F(0);
                    i_this->field_0x6bc = i_this->atInfo.mHitDirection.y;
                    actor->speedF = 0.0f;
                }
                break;
            }
        }

        for (int i = 0; i < 2; i++) {
            if (actor->health <= 1) {
                actor->health = 0;
                i_this->ccSph[i].SetTgHitMark((CcG_Tg_HitMark)3);
            }
        }
    }
}

static void action(e_bs_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp54;
    cXyz sp48;

    i_this->field_0x6a8 = 0;
    i_this->player_dist = fopAcM_searchPlayerDistanceXZ(actor);
    i_this->angleY_to_player = fopAcM_searchPlayerAngleY(actor);
    i_this->angleX_to_player = fopAcM_searchPlayerAngleX(actor);
    i_this->pl_recognize_dist = l_HIO.pl_recognize_dist;

    damage_check(i_this);

    s8 attn_ON = TRUE;
    s8 is_sound_pl_search = FALSE;

    switch (i_this->action) {
    case ACTION_APPEAR:
        e_bs_appear(i_this);
        attn_ON = FALSE;
        break;
    case ACTION_NORMAL:
        e_bs_normal(i_this);
        break;
    case ACTION_FIGHT_RUN:
        e_bs_fight_run(i_this);
        is_sound_pl_search = TRUE;
        break;
    case ACTION_ATTACK:
        e_bs_attack(i_this);
        is_sound_pl_search = TRUE;
        break;
    case ACTION_DAMAGE:
        e_bs_damage(i_this);
        break;
    }

    if (is_sound_pl_search) {
        i_this->sound.setLinkSearch(true);
    } else {
        i_this->sound.setLinkSearch(false);
    }

    if (fopAcM_otherBgCheck(player, actor)) {
        attn_ON = FALSE;
    }

    if (attn_ON) {
        fopAcM_OnStatus(actor, 0);
        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&actor->shape_angle.x, actor->current.angle.x, 2, 0x2000);

    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    sp54.x = 0.0f;
    sp54.y = 0.0f;
    sp54.z = actor->speedF;
    MtxPosition(&sp54, &sp48);
    actor->speed.x = sp48.x;
    actor->speed.z = sp48.z;

    actor->current.pos += actor->speed * l_HIO.base_size;
    actor->speed.y += actor->gravity;
    actor->gravity = -5.0f;

    if (actor->speed.y < -60.0f) {
        actor->speed.y = -60.0f;
    }

    cXyz* cc_move_p = i_this->ccStts.GetCCMoveP();
    if (cc_move_p != NULL) {
        actor->current.pos.x += 0.2f * cc_move_p->x;
        actor->current.pos.z += 0.2f * cc_move_p->z;
    }

    if (i_this->field_0x6b8 > 0.1f) {
        sp54.x = 0.0f;
        sp54.y = 0.0f;
        sp54.z = -i_this->field_0x6b8;
        cMtx_YrotS(*calc_mtx, i_this->field_0x6bc);
        MtxPosition(&sp54, &sp48);
        actor->current.pos += sp48;

        cLib_addCalc0(&i_this->field_0x6b8, 1.0f, 7.0f + TREG_F(12));
    }

    i_this->acch.CrrPos(dComIfG_Bgsp());

    s16 spC = 0;
    s16 spA = 0;

    if (i_this->field_0x6a8 > 0) {
        if (i_this->field_0x6a8 == 5) {
            if ((i_this->counter & 15) == 0 && cM_rndF(1.0f) < 0.3f) {
                i_this->field_0x6b4 = cM_rndFX(2500.0f);
            }
        } else {
            if (i_this->field_0x6a8 == 1) {
                sp54 = player->eyePos - actor->current.pos;
            } else if (i_this->field_0x6a8 == 2) {
                sp54 = i_this->field_0x6c8 - actor->current.pos;
            } else {
                sp54 = player->eyePos - actor->current.pos;
            }
            sp54.y += -(150.0f + TREG_F(2)) * l_HIO.base_size;

            spC = cM_atan2s(sp54.x, sp54.z) - actor->shape_angle.y;
            spA = actor->shape_angle.x + cM_atan2s(sp54.y, JMAFastSqrt((sp54.x * sp54.x) + (sp54.z * sp54.z)));

            if (spC > 10000) {
                spC = 10000;
            } else if (spC < -10000) {
                spC = -10000;
            }

            if (spA > 10000) {
                spA = 10000;
            } else if (spA < -10000) {
                spA = -10000;
            }

            cLib_addCalcAngleS2(&i_this->field_0x6aa, (s16)spC, 2, 0x1000);
            cLib_addCalcAngleS2(&i_this->head_rot_z, (s16)spA, 2, 0x1000);
        }

        i_this->field_0x6b4 = 0;
    } else {
        cLib_addCalcAngleS2(&i_this->field_0x6aa, 0, 2, 0x1000);
        cLib_addCalcAngleS2(&i_this->head_rot_z, 0, 2, 0x1000);
        i_this->field_0x6b4 = 0;
    }

    cLib_addCalcAngleS2(&i_this->field_0x6b2, i_this->field_0x6b4, 4, 0x400);

    if (i_this->field_0x6a8 >= 0) {
        if ((i_this->counter & 31) == 0 && cM_rndF(1.0f) < 0.5f) {
            i_this->field_0x6ae = cM_rndFX(4000.0f);
        }
    } else {
        i_this->field_0x6ae = 0;
    }

    cLib_addCalcAngleS2(&i_this->head_rot_y, i_this->field_0x6ae, 4, 0x400);
}

static void anm_se_set(e_bs_class* i_this) {
    s8 do_sound = FALSE;

    if (i_this->anm == 4) {
        if (i_this->modelMorf->checkFrame(38.0f) || i_this->modelMorf->checkFrame(54.0f)) {
            do_sound = TRUE;
        }
    } else if (i_this->anm == 5) {
        if (i_this->modelMorf->checkFrame(9.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_BS_V_ATTACK, -1);
        }
        
        if (i_this->modelMorf->checkFrame(14.0f) || i_this->modelMorf->checkFrame(29.0f)) {
            do_sound = TRUE;
        }
    } else if (i_this->anm == 6) {
        if (i_this->modelMorf->checkFrame(16.0f)) {
            do_sound = TRUE;
        }
    } else if (i_this->anm == 8) {
        if (i_this->modelMorf->checkFrame(7.0f) || i_this->modelMorf->checkFrame(19.0f)) {
            do_sound = TRUE;
        }
    } else if (i_this->anm == 9) {
        if (i_this->modelMorf->checkFrame(14.0f) || i_this->modelMorf->checkFrame(29.0f)) {
            do_sound = TRUE;
        }
    }

    if (do_sound) {
        i_this->sound.startCreatureSound(Z2SE_BABYSTAL_FOOTNOTE, 0, -1);
    }
}

static int daE_BS_Execute(e_bs_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp5C;
    cXyz sp50;
    cXyz sp44;

    i_this->counter++;

    for (int i = 0; i < 4; i++) {
        if (i_this->timers[i] != 0) {
            i_this->timers[i]--;
        }
    }

    if (i_this->invulnerabilityTimer != 0) {
        i_this->invulnerabilityTimer--;
    }

    action(i_this);

    cLib_addCalc2(&i_this->field_0x690, actor->current.pos.y, 1.0f, 2.0f + fabsf(actor->speed.y));

    mDoMtx_stack_c::transS(actor->current.pos.x, i_this->field_0x690, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::XrotM(actor->shape_angle.x);
    mDoMtx_stack_c::scaleM(l_HIO.base_size, l_HIO.base_size, l_HIO.base_size);

    J3DModel* model = i_this->modelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->modelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    anm_se_set(i_this);
    i_this->modelMorf->modelCalc();

    cXyz sp38(0.0f, 0.0f, 0.0f);
    cXyz sph_hide_pos(-200000.0f, -200000.0f, -150000.0f);
    if (i_this->invulnerabilityTimer != 0) {
        sp38 += sph_hide_pos;
    }

    MTXCopy(model->getAnmMtx(10), *calc_mtx);
    sp5C.set(KREG_F(12), KREG_F(13), KREG_F(14));
    MtxPosition(&sp5C, &actor->eyePos);

    i_this->ccSph[0].SetC(actor->eyePos + sp38);
    i_this->ccSph[0].SetR(15.0f * l_HIO.base_size);

    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += 20.0f + BREG_F(7);

    MTXCopy(model->getAnmMtx(1), *calc_mtx);
    sp5C.set(BREG_F(11), BREG_F(12), BREG_F(13));
    MtxPosition(&sp5C, &sp50);

    i_this->ccSph[1].SetC(sp50 + sp38);
    i_this->ccSph[1].SetR(35.0f * l_HIO.base_size);

    dComIfG_Ccsp()->Set(&i_this->ccSph[0]);
    dComIfG_Ccsp()->Set(&i_this->ccSph[1]);

    i_this->weponModelMorf->play(&actor->eyePos, 0, 0);

    MTXCopy(i_this->modelMorf->getModel()->getAnmMtx(8), *calc_mtx);
    i_this->weponModelMorf->getModel()->setBaseTRMtx(*calc_mtx);
    i_this->weponModelMorf->modelCalc();

    if (i_this->is_wep_attack) {
        sp5C.set(BREG_F(7), 120.0f + BREG_F(8), BREG_F(9));
        MtxPosition(&sp5C, &sp50);

        if (i_this->field_0x6a7 == 0) {
            i_this->field_0x6a7 = 1;
            i_this->atSph.StartCAt(sp50);
        } else {
            i_this->atSph.MoveCAt(sp50);
        }

        i_this->atSph.SetR(35.0f * l_HIO.base_size);
    }

    if (!i_this->is_wep_attack) {
        i_this->atSph.SetC(sph_hide_pos);
        i_this->field_0x6a7 = 0;
    } else {
        i_this->is_wep_attack = FALSE;
    }

    dComIfG_Ccsp()->Set(&i_this->atSph);

    cXyz effsize(0.5f, 0.5f, 0.5f);
    setMidnaBindEffect((fopEn_enemy_c*)i_this, &i_this->sound, &actor->eyePos, &effsize);
    return 1;
}

static int daE_BS_IsDelete(e_bs_class* i_this) {
    return 1;
}

static int daE_BS_Delete(e_bs_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fpc_ProcID id = fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->phase, "E_BS");

    if (i_this->HIOInit) {
        hio_set = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (actor->heap != NULL) {
        i_this->modelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_bs_class* a_this = (e_bs_class*)i_this;

    a_this->modelMorf = new mDoExt_McaMorfSO((J3DModelData* )dComIfG_getObjectRes("E_BS", 0xE), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_BS", 7), 0, 1.0f, 0, -1, &a_this->sound, 0x80000, 0x11000084);
    if (a_this->modelMorf == NULL || a_this->modelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = a_this->modelMorf->getModel();
    model->setUserArea((uintptr_t)a_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        if (i == 10) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
    }

    a_this->weponModelMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("E_BS", 0xF), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_BS", 0xA), 0, 1.0f, 0, -1, 1, 0, 0x80000, 0x11000084);
    if (a_this->weponModelMorf == NULL || a_this->weponModelMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int daE_BS_Create(fopAc_ac_c* i_this) {
    e_bs_class* a_this = (e_bs_class*)i_this;
    fopAcM_ct(i_this, e_bs_class);
    
    int phase_state = dComIfG_resLoad(&a_this->phase, "E_BS");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_BS PARAM %x\n", fopAcM_GetParam(i_this));
        int swbit = (fopAcM_GetParam(i_this) & 0xFF0000) >> 0x10;
        if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(i_this))) {
            return cPhs_ERROR_e;
        }
    
        a_this->prm0 = fopAcM_GetParam(i_this);
        a_this->prm1 = (fopAcM_GetParam(i_this) & 0xFF00) >> 8;

        if (a_this->prm1 == 0xFF) {
            a_this->appear_range = 500.0f;
        } else {
            a_this->appear_range = 100.0f * a_this->prm1;
        }

        int prm3 = (fopAcM_GetParam(i_this) & 0xFF0000) >> 0x10;
        OS_REPORT("E_BS//////////////E_BS SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x18C0)) {
            OS_REPORT("//////////////E_BS SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (!hio_set) {
            a_this->HIOInit = TRUE;
            hio_set = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("ベビースタル", &l_HIO);
        }

        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        fopAcM_SetMtx(i_this, a_this->modelMorf->getModel()->getBaseTRMtx());

        a_this->acch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->acchcir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->acchcir.SetWall(80.0f, 100.0f);

        i_this->health = 40;
        i_this->field_0x560 = 40;

        a_this->ccStts.Init(150, 0, i_this);

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

        for (int i = 0; i < 2; i++) {
            a_this->ccSph[i].Set(cc_sph_src);
            a_this->ccSph[i].SetStts(&a_this->ccStts);
        }

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
            } // mSphAttr
        };

        a_this->atSph.Set(at_sph_src);
        a_this->atSph.SetStts(&a_this->ccStts);

        a_this->counter = (u16)cM_rndF(65535.0f) & 0xFF00;
        if (a_this->prm0 == 0) {
            a_this->action = ACTION_NORMAL;
            a_this->is_draw_shadow = TRUE;
        } else {
            a_this->action = ACTION_APPEAR;
        }

        a_this->sound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        a_this->sound.setEnemyName("E_bs");

        a_this->atInfo.mpSound = &a_this->sound;
        a_this->atInfo.field_0x18 = 34;
        a_this->atInfo.mPowerType = 4;
        a_this->field_0x690 = i_this->current.pos.y;
    
        daE_BS_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_BS_Method = {
    (process_method_func)daE_BS_Create,
    (process_method_func)daE_BS_Delete,
    (process_method_func)daE_BS_Execute,
    (process_method_func)daE_BS_IsDelete,
    (process_method_func)daE_BS_Draw,
};

actor_process_profile_definition g_profile_E_BS = {
    fpcLy_CURRENT_e,       // mLayerID
    7,                     // mListID
    fpcPi_CURRENT_e,       // mListPrio
    PROC_E_BS,             // mProcName
    &g_fpcLf_Method.base, // sub_method
    sizeof(e_bs_class),    // mSize
    0,                     // mSizeOther
    0,                     // mParameters
    &g_fopAc_Method.base,  // sub_method
    124,                   // mPriority
    &l_daE_BS_Method,      // sub_method
    0x10040100,            // mStatus
    fopAc_ENEMY_e,         // mActorType
    fopAc_CULLBOX_0_e,     // cullType
};
