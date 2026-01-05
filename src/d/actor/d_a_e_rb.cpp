/**
 * @file d_a_e_rb.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_rb.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_enemy.h"

class daE_RB_HIO_c : public JORReflexible {
public:
    daE_RB_HIO_c();
    virtual ~daE_RB_HIO_c() {}

#if DEBUG
    void genMessage(JORMContext*);
#endif

    /* 0x4 */ s8 id;
    /* 0x8 */ f32 base_size;
    /* 0xC */ f32 ground_depth;
};

enum daE_RB_ACTION {
    ACTION_STAY,
    ACTION_APPEAR,
    ACTION_MOVE,
    ACTION_ATTACK,
    ACTION_DISAPPEAR = 5,
    ACTION_DAMAGE = 10,
};

daE_RB_HIO_c::daE_RB_HIO_c() {
    id = -1;
    base_size = 1.5f;
    ground_depth = -5.0f;
}

static void anm_init(e_rb_class* i_this, int i_anm, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->anm = i_anm;
    i_this->modelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_rb", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
}

static u8 hio_set;

static daE_RB_HIO_c l_HIO;

static int daE_RB_Draw(e_rb_class* i_this) {
    if (!i_this->isChild) {
        return 1;
    }

    if (i_this->height <= -70.0f) {
        return 1;
    }

    fopEn_enemy_c* enemy = &i_this->enemy;
    J3DModel* model = i_this->modelMorf->getModel();
    g_env_light.settingTevStruct(0, &enemy->current.pos, &enemy->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &enemy->tevStr);
    i_this->modelMorf->entryDL();

    if (i_this->field_0xa24 > 0.1f) {
        dComIfGd_setSimpleShadow(&enemy->current.pos, i_this->acch.GetGroundH(), (i_this->field_0xa24 * (40.0f + TREG_F(11))) * l_HIO.base_size, i_this->acch.m_gnd, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static void damage_check(e_rb_class* i_this) {
    fopEn_enemy_c* enemy = &i_this->enemy;

    dComIfGp_getPlayer(0);
    i_this->ccStts.Move();

    if (i_this->invincibilityTimer == 0) {
        if (i_this->ccSph.ChkTgHit()) {
            i_this->atInfo.mpCollider = i_this->ccSph.GetTgHitObj();
            cc_at_check(enemy, &i_this->atInfo);
    
            if (i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT)) {
                return;
            }

            if (i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK | AT_TYPE_WOLF_CUT_TURN | AT_TYPE_10000000 | AT_TYPE_MIDNA_LOCK)) {
                i_this->invincibilityTimer = 20;
            } else {
                i_this->invincibilityTimer = 10;
            }

            if ((i_this->atInfo.mHitType == 1 && daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_TURN_RIGHT) || daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_UNK_9) {
                i_this->rot_step = KREG_S(9) + 5000;
                i_this->rot_x = 0x2000;
                i_this->field_0xa30 = 60.0f + TREG_F(10);
            } else {
                i_this->rot_step = KREG_S(9) + 2000;
                i_this->rot_x = 0x1800;
                i_this->field_0xa30 = 50.0f + TREG_F(10);
            }

            i_this->field_0xa50 = i_this->atInfo.mHitDirection.y;

            if (i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG | AT_TYPE_HOOKSHOT | AT_TYPE_40 | AT_TYPE_SHIELD_ATTACK)) {
                enemy->health = 1;
            }

            if (enemy->health <= 0) {
                i_this->action = ACTION_DAMAGE;
                i_this->sound.startCreatureSound(Z2SE_EN_RB_V_DEATH, 0, -1);

                enemy->current.angle.y = i_this->atInfo.mHitDirection.y + 0x8000;
                enemy->speedF = 40.0f + AREG_F(5);

                i_this->field_0xa30 = 20.0f;
                i_this->rot_step = KREG_S(9) + 5000;
                i_this->rot_y = KREG_S(8) - 25000;
                i_this->death_rot_y = cM_rndF(65536.0f);
            } else {
                i_this->action = ACTION_MOVE;
                i_this->rot_y = cM_rndF(65536.0f);
            }

            i_this->mode = 0;
            i_this->field_0xa2c = 1.5f + JREG_F(4);
        }

        if (enemy->health <= 1) {
            enemy->health = 0;
            i_this->ccSph.SetTgHitMark((CcG_Tg_HitMark)3);
        }
    }
}

static void e_rb_stay(e_rb_class* i_this) {
    i_this->invincibilityTimer = 3;

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 5, 1.0f, 0, 0.0f);
        i_this->mode = 1;
        i_this->height = -200.0f;
        /* fallthrough */
    case 1:
        break;
    }
}

static s8 e_rb_appear(e_rb_class* i_this) {
    fopEn_enemy_c* enemy = &i_this->enemy;

    switch (i_this->mode) {
    case 0:
        if (i_this->acch.ChkGroundHit()) {
            i_this->height = -70.0f;
            anm_init(i_this, 5, 1.0f, 0, 1.0f);
            i_this->mode = 2;
            i_this->field_0xa4c = hREG_S(0) - 4000;
            i_this->invincibilityTimer = 40;
            i_this->sound.startCreatureSound(Z2SE_EN_RB_APPEAR, 0, -1);
            break;
        }

        i_this->invincibilityTimer = 3;
        i_this->action = ACTION_STAY;
        i_this->mode = 0;
        return 0;
    case 1:
        if (i_this->modelMorf->isStop()) {
            i_this->mode = 2;
        }
        break;
    case 2:
        if (i_this->field_0xa4c >= hREG_S(2) + 4000) {
            i_this->action = ACTION_MOVE;
            i_this->mode = 0;
            i_this->timers[1] = 800.0f + cM_rndF(300.0f);
        }
        break;
    }

    int anm_frame = i_this->modelMorf->getFrame();
    cLib_addCalcAngleS2(&i_this->field_0xa4c, hREG_S(2) + 4000, 1, hREG_S(3) + 200);
    enemy->shape_angle.y += i_this->field_0xa4c;

    f32 height_step;
    if (anm_frame < KREG_S(5) + 26) {
        height_step = 1.0f + hREG_F(0);
    } else {
        height_step = 3.0f + hREG_F(1);
    }

    cLib_addCalc2(&i_this->height, l_HIO.ground_depth, 1.0f, height_step);
    i_this->field_0xa24 = 1.0f - (i_this->height / -70.0f);
    return 2;
}

static void e_rb_move(e_rb_class* i_this) {
    fopEn_enemy_c* enemy = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    cXyz spC;
    spC.x = i_this->field_0xa10.x - enemy->current.pos.x;
    spC.z = i_this->field_0xa10.z - enemy->current.pos.z;

    f32 speed_target;
    switch (i_this->mode) {
    case 0:
        i_this->mode = 1;
        i_this->timers[0] = 200.0f + cM_rndF(400.0f);
        break;
    case 1:
        speed_target = 33.0f + hREG_F(10);

        if (i_this->timers[0] == 0) {
            s16 temp_r0_2 = (player->shape_angle.y - i_this->angleToPlayer) + 0x8000;
            if (temp_r0_2 < 0x1000 && temp_r0_2 > -0x1000) {
                i_this->action = ACTION_ATTACK;
                i_this->mode = 0;
            }
        } else if (i_this->timers[1] == 0 && i_this->acch.ChkGroundHit()) {
            i_this->action = ACTION_DISAPPEAR;
            i_this->mode = 0;
        }
        break;
    }

    if (i_this->anm == 5 && i_this->modelMorf->isStop()) {
        anm_init(i_this, 8, 3.0f, 2, 1.0f);
    }

    cLib_addCalcAngleS2(&enemy->current.angle.y, cM_atan2s(spC.x, spC.z), 2, 0x800);

    f32 temp_f1 = JMAFastSqrt((spC.x * spC.x) + (spC.z * spC.z));
    f32 speed_step;
    if (temp_f1 < 200.0f) {
        speed_target *= 0.005f * temp_f1;
        speed_step = 1.0f + TREG_F(19);
    } else {
        speed_step = 1.0f;
    }

    cLib_addCalc2(&enemy->speedF, speed_target, 1.0f, speed_step);
    cLib_addCalcAngleS2(&i_this->field_0xa4c, KREG_S(2) + 4000, 1, 100);
    enemy->shape_angle.y += i_this->field_0xa4c;

    cLib_addCalc2(&i_this->height, l_HIO.ground_depth, 1.0f, 1.0f);
    i_this->sound.startCreatureSoundLevel(Z2SE_EN_RB_MOVE, enemy->speedF, -1);
}

static void e_rb_attack(e_rb_class* i_this) {
    fopEn_enemy_c* enemy = &i_this->enemy;
    dComIfGp_getPlayer(0);
    cXyz sp18;
    cXyz spC;

    f32 speed_target = TREG_F(3);
    f32 speed_step = 1.0f;

    switch (i_this->mode) {
    case 0:
        i_this->timers[0] = 20;
        i_this->mode = 1;
        anm_init(i_this, 4, 3.0f, 0, 1.0f);
        break;
    case 1:
        if (i_this->modelMorf->isStop()) {
            anm_init(i_this, 6, 3.0f, 0, 1.0f);
            i_this->mode = 2;
            i_this->timers[0] = AREG_S(7) + 45;
            enemy->current.angle.y = i_this->angleToPlayer;
        }
        break;
    case 2:
        cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->angleToPlayer, 4, 0x400);
        speed_target = 30.0f + TREG_F(6);
        speed_step = 2.0f + TREG_F(8);

        if (i_this->timers[0] == 0 || i_this->acch.ChkWallHit()) {
            i_this->action = ACTION_MOVE;
            i_this->mode = 0;

            if (i_this->timers[0] != 0) {
                i_this->invincibilityTimer = 4;

                if (i_this->ccSph.ChkAtHit()) {
                    i_this->field_0xa30 = 20.0f + TREG_F(10);
                } else {
                    i_this->field_0xa30 = 40.0f + TREG_F(11);
                }

                i_this->field_0xa50 = i_this->angleToPlayer;
                i_this->rot_step = KREG_S(9) + 2000;
                i_this->rot_x = 0x1000;
            }
        } else if (i_this->ccSph.ChkAtShieldHit()) {
            i_this->invincibilityTimer = AREG_S(2) + 5;
            i_this->field_0xa30 = 50.0f + AREG_F(13);
            i_this->field_0xa50 = i_this->angleToPlayer;
            i_this->rot_step = AREG_S(5) + 1000;
            i_this->rot_x = AREG_S(6) + 0x1000;
            i_this->field_0xa2c = 0.8f + JREG_F(6);
            i_this->timers[0] = AREG_S(7) + 10;
        }
        break;
    }

    cLib_addCalc2(&enemy->speedF, speed_target, 1.0f, speed_step);
    enemy->shape_angle.y += i_this->field_0xa4c;
    i_this->sound.startCreatureSoundLevel(Z2SE_EN_RB_MOVE, enemy->speedF, -1);
}

static s8 e_rb_disappear(e_rb_class* i_this) {
    fopEn_enemy_c* enemy = &i_this->enemy;
    s8 ret = 0;

    switch (i_this->mode) {
    case 0:
        i_this->mode = 1;
        i_this->timers[0] = 20;
        i_this->sound.startCreatureSound(Z2SE_EN_RB_BACKDOWN, 0, -1);
        break;
    case 1:
        if (i_this->timers[0] == 0) {
            i_this->invincibilityTimer = 4;
        } else {
            ret = 1;
        }

        cLib_addCalc2(&i_this->height, -70.0f, 1.0f, 2.0f);
        if (i_this->height <= -70.0f) {
            i_this->action = ACTION_STAY;
            i_this->mode = 0;
            i_this->timers[0] = 100.0f + cM_rndF(100.0f);
        }
        break;
    }

    cLib_addCalc0(&enemy->speedF, 1.0f, 2.5f);
    i_this->field_0xa24 = 1.0f - (i_this->height / -70.0f);
    enemy->shape_angle.y += i_this->field_0xa4c;
    return ret;
}

static void e_rb_damage(e_rb_class* i_this) {
    fopEn_enemy_c* enemy = &i_this->enemy;
    i_this->invincibilityTimer = 10;

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 7, 3.0f, 0, 1.0f);
        i_this->mode = 1;
        i_this->timers[0] = cM_rndF(10.0f);
        break;
    case 1:
        if (i_this->timers[0] == 0) {
            enemy->shape_angle.y = -1.0f * i_this->rot_y;
            i_this->rot_y += i_this->rot_step;

            cLib_addCalcAngleS2(&i_this->rot_step, 0, 1, KREG_S(6) + 105);
            if (i_this->rot_step == 0) {
                i_this->mode = 2;
                i_this->timers[0] = 35.0f + cM_rndF(10.0f);
            }
        }
        break;
    case 2:
        cLib_addCalcAngleS2(&enemy->shape_angle.y, -0x6978, 1, (KREG_S(7) + 0x300));
        i_this->rot_y = -enemy->shape_angle.y;

        if (i_this->timers[0] == 0) {
            fopAcM_createDisappear(enemy, &enemy->eyePos, 7, 0, enemy->field_0x564);
            fopAcM_delete(enemy);
        }
        break;
    }

    cLib_addCalc0(&enemy->speedF, 1.0f, 1.0f + AREG_F(2));
    if (enemy->speedF < 30.0f) {
        cLib_addCalcAngleS2(&enemy->current.angle.y, i_this->rot_y + AREG_S(3) + 0x4000, 1, 4000);

        cLib_addCalcAngleS2(&i_this->rot_x, AREG_S(1) + 0x27D0, 1, 100);
        if (i_this->rot_x >= 0x2000) {
            cLib_addCalc2(&i_this->height, 10.0f + (l_HIO.ground_depth + AREG_F(0)), 1.0f, 1.0f);
        }
    }
}

static void action(e_rb_class* i_this) {
    fopEn_enemy_c* enemy = &i_this->enemy;
    cXyz sp18;
    cXyz spC;

    damage_check(i_this);

    s8 attention_ON = FALSE;
    s8 var_r28 = 0;
    s8 sp8 = 1;

    i_this->ccSph.OffAtSetBit();
   
    switch (i_this->action) {
    case ACTION_STAY:
        e_rb_stay(i_this);
        break;
    case ACTION_APPEAR:
        var_r28 = e_rb_appear(i_this);
        i_this->ccSph.OnAtSetBit();
        break;
    case ACTION_MOVE:
        e_rb_move(i_this);
        attention_ON = TRUE;
        var_r28 = 1;
        i_this->ccSph.OnAtSetBit();
        break;
    case ACTION_ATTACK:
        e_rb_attack(i_this);
        attention_ON = TRUE;
        var_r28 = 1;
        i_this->ccSph.OnAtSetBit();
        break;
    case ACTION_DISAPPEAR:
        attention_ON = e_rb_disappear(i_this);
        var_r28 = 2;
        i_this->ccSph.OnAtSetBit();
        break;
    case ACTION_DAMAGE:
        e_rb_damage(i_this);
        var_r28 = 1;
        sp8 = 0;
        i_this->ccSph.OnAtSetBit();
        break;
    }

    cMtx_YrotS(*calc_mtx, enemy->current.angle.y);
    sp18.x = 0.0f;
    sp18.y = 0.0f;
    if (i_this->action == ACTION_DAMAGE) {
        sp18.z = enemy->speedF;
    } else {
        sp18.z = enemy->speedF * cM_scos(i_this->rot_x);
    }

    MtxPosition(&sp18, &spC);
    enemy->current.pos.x += spC.x;
    enemy->current.pos.z += spC.z;
    enemy->current.pos.y += enemy->speed.y;
    enemy->speed.y -= 3.0f;

    if (i_this->field_0xa30 > 0.1f) {
        sp18.x = 0.0f;
        sp18.y = 0.0f;
        sp18.z = -i_this->field_0xa30;
        cMtx_YrotS(*calc_mtx, i_this->field_0xa50);
        MtxPosition(&sp18, &spC);
        enemy->current.pos += spC;
        cLib_addCalc0(&i_this->field_0xa30, 1.0f, 3.0f + TREG_F(12));
    }

    cXyz* cc_move_p = i_this->ccStts.GetCCMoveP();
    if (cc_move_p != NULL) {
        enemy->current.pos.x += cc_move_p->x;
        enemy->current.pos.y += cc_move_p->y;
        enemy->current.pos.z += cc_move_p->z;
    }

    i_this->acch.CrrPos(dComIfG_Bgsp());

    if (attention_ON) {
        fopAcM_OnStatus(enemy, 0);
        enemy->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(enemy, 0);
        enemy->attention_info.flags = 0;
    }

    if (var_r28 == 2) {
        if (!(i_this->counter & 7)) {
            fopAcM_effSmokeSet1(&i_this->smokeEmtrID, &i_this->field_0xa60, &enemy->current.pos, NULL, l_HIO.base_size * (0.6f + TREG_F(14)), &enemy->tevStr, 1);
        }
    } else if (var_r28 != 0 && enemy->speedF > 1.0f && !(i_this->counter & 1)) {
        fopAcM_effSmokeSet2(&i_this->smokeEmtrID, &i_this->field_0xa60, &enemy->current.pos, NULL, l_HIO.base_size * (1.0f + TREG_F(12)), &enemy->tevStr);
    }

    if (sp8 != 0) {
        i_this->rot_y += i_this->rot_step;
        cLib_addCalcAngleS2(&i_this->rot_step, 0, 1, 50);
        cLib_addCalcAngleS2(&i_this->rot_x, 0, 8, 50);
    }
}

static void e_rb_base_0(e_rb_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (i_this->distToPlayer < 100.0f * i_this->appearRange) {
        i_this->action = ACTION_APPEAR;
        i_this->mode = 0;
        i_this->field_0xa64 = 0;
    }

    i_this->field_0xa10 = player->current.pos + (player->speed * (50.0f + hREG_F(13)));
}

static e_rb_class* child[10];

static int target_info_count;

static void* s_s_sub(void* i_actor, void* i_data) {
    fopAc_ac_c* parent;
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RB &&
        fopAcM_SearchByID(((fopAc_ac_c*)i_actor)->parentActorID, &parent) && parent != NULL &&
        (fopAc_ac_c*)i_data == parent && target_info_count < 10)
    {
        child[target_info_count] = (e_rb_class*)i_actor;
        target_info_count++;
    }

    return NULL;
}

static void e_rb_base_1(e_rb_class* i_this) {
    fopAc_ac_c* enemy = &i_this->enemy;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    target_info_count = 0;

    for (int i = 0; i < 10; i++) {
        child[i] = NULL;
    }

    fpcM_Search(s_s_sub, i_this);

    int child_move_count = 0;
    int child_stay_count = 0;
    if (target_info_count != 0) {
        dBgS_ObjGndChk sp108;
        cXyz sp74;
        cXyz sp68;
        cXyz sp5C(player->current.pos + (player->speed * (15.0f + hREG_F(13))));

        cLib_addCalc2(&i_this->field_0xa10.x, sp5C.x, 0.1f, 33.0f + hREG_F(10));
        cLib_addCalc2(&i_this->field_0xa10.z, sp5C.z, 0.1f, 33.0f + hREG_F(10));

        switch (i_this->mode) {
        case 0:
            i_this->mode = 1;
            i_this->timers[0] = 100;
            i_this->field_0xa28 = 500.0f;
            /* fallthrough */
        case 1:
            cLib_addCalc2(&i_this->field_0xa28, 500.0f + JREG_F(0), 0.05f, 5.0f);
            if (i_this->timers[0] == 0) {
                i_this->mode = 2;
                i_this->timers[0] = 100;
            }
            break;
        case 2:
            cLib_addCalc2(&i_this->field_0xa28, 150.0f + JREG_F(1), 0.05f, 5.0f);
            if (i_this->timers[0] == 0) {
                i_this->mode = 1;
                i_this->timers[0] = 700;
            }
            break;
        }

        for (int i = 0; i < target_info_count; i++) {
            if (child[i]->action != ACTION_ATTACK) {
                cMtx_YrotS(*calc_mtx, i_this->field_0xa4e + ((0x10000 / target_info_count) * i));
                sp74.x = 0.0f;
                sp74.y = 0.0f;
                sp74.z = i_this->field_0xa28;
                MtxPosition(&sp74, &sp68);
                child[i]->field_0xa10 = i_this->field_0xa10 + sp68;

                if (child[i]->action == ACTION_STAY && child[i]->timers[0] == 0) {
                    if (i_this->distToPlayer < 100.0f * i_this->field_0xa69) {
                        dBgS_LinChk sp98;
                        cXyz sp50(player->current.pos);
                        cXyz sp44(child[i]->field_0xa10);
                        sp50.y += 100.0f;

                        sp98.Set(&sp50, &sp44, NULL);
                        if (dComIfG_Bgsp().LineCross(&sp98)) {
                            cM3dGPla sp80;
                            dComIfG_Bgsp().GetTriPla(sp98, &sp80);

                            cXyz* sp14 = sp80.GetNP();
                            child[i]->field_0xa10 = sp98.GetCross();

                            child[i]->field_0xa10.x += 50.0f * sp14->x;
                            child[i]->field_0xa10.y += 50.0f * sp14->y;
                            child[i]->field_0xa10.z += 50.0f * sp14->z;
                        }

                        sp68 = child[i]->field_0xa10;
                        sp68.y += 200.0f;
                        sp108.SetPos(&sp68);
                        
                        f32 ground_y = dComIfG_Bgsp().GroundCross(&sp108);
                        if (-G_CM3D_F_INF != ground_y) {
                            child[i]->enemy.current.pos = child[i]->enemy.old.pos = child[i]->field_0xa10;
                            child[i]->enemy.old.pos.y = ground_y;
                            child[i]->enemy.current.pos.y = ground_y;
                            child[i]->action = ACTION_APPEAR;
                            child[i]->mode = 0;
                        }
                    }
                }
            }

            if (child[i]->action == ACTION_STAY) {
                child_stay_count++;
            } else if (child[i]->action >= ACTION_MOVE) {
                child_move_count++;
                if ((i_this->distToPlayer >= 100.0f * i_this->field_0xa69) && child[i]->timers[1] >= 40) {
                    child[i]->timers[1] = 10.0f + cM_rndF(30.0f);
                }
            }
        }

        if (child_stay_count == target_info_count) {
            i_this->action = ACTION_STAY;
            i_this->mode = 0;
        } else if (child_move_count == target_info_count) {
            i_this->field_0xa64 = 1;
        }

        if (i_this->field_0xa64 != 0) {
            i_this->field_0xa4e += (s16)(AREG_S(6) + 300);
        }
        return;
    }

    if (i_this->swbit != 0xFF) {
        dComIfGs_onSwitch(i_this->swbit, fopAcM_GetRoomNo(enemy));
        fopAcM_delete(enemy);
        return;
    }

    i_this->field_0xa65++;
    if (i_this->field_0xa65 > 150) {
        i_this->field_0xa65 = 0;
        i_this->action = ACTION_STAY;
        i_this->mode = 0;

        for (int i = 0; i < i_this->childNum; i++) {
            fopAcM_createChild(PROC_E_RB, fopAcM_GetID(enemy), 1, &enemy->current.pos, fopAcM_GetRoomNo(enemy), NULL, NULL, -1, NULL);
        }
    }
}

static void e_rb_base(e_rb_class* i_this) {
    switch (i_this->action) {
    case ACTION_STAY:
        e_rb_base_0(i_this);
        return;
    case ACTION_APPEAR:
        e_rb_base_1(i_this);
        return;
    }
}

static int daE_RB_Execute(e_rb_class* i_this) {
    fopEn_enemy_c* enemy = &i_this->enemy;
    cXyz sp30;
    cXyz sp24;

    i_this->angleToPlayer = fopAcM_searchPlayerAngleY(enemy);
    i_this->distToPlayer = fopAcM_searchPlayerDistanceXZ(enemy);
    i_this->counter++;

    for (int i = 0; i < 4; i++) {
        if (i_this->timers[i] != 0) {
            i_this->timers[i]--;
        }
    }

    if (!i_this->isChild) {
        e_rb_base(i_this);
        return 1;
    }

    if (!fopAcM_SearchByID(enemy->parentActorID, &i_this->parent) || i_this->parent == NULL) {
        return 1;
    }

    if (i_this->invincibilityTimer != 0) {
        i_this->invincibilityTimer--;
    }

    action(i_this);

    mDoMtx_stack_c::transS(enemy->current.pos.x, enemy->current.pos.y + (i_this->height * l_HIO.base_size), enemy->current.pos.z);
    if (i_this->action == ACTION_DAMAGE) {
        mDoMtx_stack_c::YrotM(i_this->death_rot_y);
    }
    mDoMtx_stack_c::YrotM(i_this->rot_y);
    mDoMtx_stack_c::XrotM(i_this->rot_x);
    mDoMtx_stack_c::YrotM(enemy->shape_angle.y);

    f32 var_f31 = 0.0f;
    if (i_this->field_0xa2c > 0.01f) {
        var_f31 = i_this->field_0xa2c * ((0.2f + JREG_F(2)) * cM_ssin(i_this->counter * 8000));
        cLib_addCalc0(&i_this->field_0xa2c, 1.0f, 0.05f + JREG_F(3));
    }

    cXyz size(l_HIO.base_size * (1.0f + var_f31), l_HIO.base_size * (1.0f - var_f31), l_HIO.base_size * (1.0f + var_f31));
    mDoMtx_stack_c::scaleM(size);

    J3DModel* model = i_this->modelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->modelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(enemy)));
    i_this->modelMorf->modelCalc();

    enemy->eyePos = enemy->current.pos;
    enemy->eyePos.y += 40.0f + i_this->height;
    enemy->attention_info.position = enemy->eyePos;
    enemy->attention_info.position.y += 50.0f;

    cXyz ccsph_center(enemy->eyePos);
    if (i_this->invincibilityTimer != 0) {
        ccsph_center.y += 30000.0f;
    }
    i_this->ccSph.SetC(ccsph_center);
    i_this->ccSph.SetR((35.0f + JREG_F(3)) * l_HIO.base_size);
    dComIfG_Ccsp()->Set(&i_this->ccSph);

    if (i_this->action != ACTION_STAY) {
        setMidnaBindEffect(enemy, &i_this->sound, &enemy->eyePos, &size);
    }

    return 1;
}

static int daE_RB_IsDelete(e_rb_class* i_this) {
    return 1;
}

static int daE_RB_Delete(e_rb_class* i_this) {
    fopEn_enemy_c* enemy = &i_this->enemy;
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->phase, "E_rb");

    if (i_this->HIOInit) {
        hio_set = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (enemy->heap != NULL) {
        i_this->modelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_rb_class* a_this = (e_rb_class*)i_this;

    a_this->modelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_rb", 0xB), NULL, NULL, NULL, 0, 1.0f, 0, -1, &a_this->sound, 0x80000, 0x11000084);
    if (a_this->modelMorf == NULL || a_this->modelMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int daE_RB_Create(fopAc_ac_c* i_this) {
    e_rb_class* a_this = (e_rb_class*)i_this;
    fopAcM_ct(i_this, e_rb_class);

    int phase_state = dComIfG_resLoad(&a_this->phase, "E_rb");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_rb PARAM %x\n", fopAcM_GetParam(i_this));
    
        a_this->isChild = fopAcM_GetParam(i_this);
        if (a_this->isChild == 0xFF) {
            a_this->isChild = 0;
        }
    
        a_this->childNum = (fopAcM_GetParam(i_this) & 0xFF00) >> 8;
        if (a_this->childNum > 10) {
            a_this->childNum = 10;
        }
    
        if (!a_this->isChild) {
            a_this->swbit = i_this->current.angle.x;
            OS_REPORT("Switch   : %x\n", a_this->swbit);
            i_this->current.angle.x = 0;

            if (a_this->swbit != 0xFF && dComIfGs_isSwitch(a_this->swbit, fopAcM_GetRoomNo(i_this))) {
                OS_REPORT("E_RB やられ後なので再セットしません\n");
                return cPhs_ERROR_e;
            }

            for (int i = 0; i < a_this->childNum; i++) {
                fopAcM_createChild(PROC_E_RB, fopAcM_GetID(i_this), 1, &i_this->current.pos, fopAcM_GetRoomNo(i_this), NULL, NULL, -1, NULL);
            }

            a_this->appearRange = (fopAcM_GetParam(i_this) & 0xFF0000) >> 0x10;
            if (a_this->appearRange == 0xFF || a_this->appearRange == 0) {
                a_this->appearRange = 10;
            }

            a_this->field_0xa69 = (fopAcM_GetParam(i_this) & 0xFF000000) >> 0x18;
            if (a_this->field_0xa69 == 0xFF || a_this->field_0xa69 == 0) {
                a_this->field_0xa69 = 0xF;
            }

            return phase_state;
        }
    
        OS_REPORT("E_rb//////////////E_RB SET 1 !!\n");
        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0xB80)) {
            OS_REPORT("//////////////E_RB SET NON !!\n");
            return cPhs_ERROR_e;
        }
    
        OS_REPORT("//////////////E_RB SET 2 !!\n");
        if (!hio_set) {
            a_this->HIOInit = TRUE;
            hio_set = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("リーバ", &l_HIO);
        }

        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(i_this, a_this->modelMorf->getModel()->getBaseTRMtx());
        i_this->health = 1;
        i_this->field_0x560 = 1;

        a_this->ccStts.Init(100, 0, i_this);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{AT_TYPE_1000, 0x1, 0xd}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        a_this->ccSph.Set(cc_sph_src);
        a_this->ccSph.SetStts(&a_this->ccStts);

        i_this->old.pos = i_this->current.pos;

        a_this->acch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->acchcir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->acch.OnLineCheck();
        a_this->acchcir.SetWall(50.0f, 50.0f);

        a_this->sound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        a_this->sound.setEnemyName("E_rb");
        a_this->atInfo.mpSound = &a_this->sound;
        a_this->counter = cM_rndF(65536.0f);
        i_this->field_0x564 = 40;
        daE_RB_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_RB_Method = {
    (process_method_func)daE_RB_Create,
    (process_method_func)daE_RB_Delete,
    (process_method_func)daE_RB_Execute,
    (process_method_func)daE_RB_IsDelete,
    (process_method_func)daE_RB_Draw,
};

actor_process_profile_definition g_profile_E_RB = {
    fpcLy_CURRENT_e,       // mLayerID
    7,                     // mListID
    fpcPi_CURRENT_e,       // mListPrio
    PROC_E_RB,             // mProcName
    &g_fpcLf_Method.base, // sub_method
    sizeof(e_rb_class),    // mSize
    0,                     // mSizeOther
    0,                     // mParameters
    &g_fopAc_Method.base,  // sub_method
    153,                   // mPriority
    &l_daE_RB_Method,      // sub_method
    0x10040120,            // mStatus
    fopAc_ENEMY_e,         // mActorType
    fopAc_CULLBOX_0_e,     // cullType
};
