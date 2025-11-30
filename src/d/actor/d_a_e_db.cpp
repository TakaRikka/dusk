/**
 * @file d_a_e_db.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_db.h"
#include "d/actor/d_a_e_db_leaf.h"
#include "f_op/f_op_kankyo_mng.h"
#include "f_op/f_op_actor_enemy.h"

class daE_DB_HIO_c : public JORReflexible {
public:
    daE_DB_HIO_c();
    virtual ~daE_DB_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 base_size;
    /* 0x0C */ f32 flower_size;
    /* 0x10 */ f32 appear_size;
    /* 0x14 */ s16 roof_reappear_interval;
    /* 0x16 */ s16 attack_freq;
};

enum daE_DB_ACTION {
    ACTION_STAY,
    ACTION_APPEAR,
    ACTION_APPEAR_V,
    ACTION_WAIT,
    ACTION_ATTACK,
    ACTION_ATTACK_S,
    ACTION_CHANCE,
    ACTION_S_DAMAGE,
    ACTION_DAMAGE,

    ACTION_MK_ROOF = 10,

    ACTION_ESCAPE = 20,
    ACTION_E_DEAD,
};

daE_DB_HIO_c::daE_DB_HIO_c() {
    id = -1;
    base_size = 1.0f;
    flower_size = 0.5f;
    appear_size = 1.2f;
    roof_reappear_interval = 70;
    attack_freq = 30;
}

static void anm_init(e_db_class* i_this, int i_anm, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->modelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_db", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
    i_this->anm = i_anm;
}

static BOOL leaf_anm_init(e_db_class* i_this, int i_anm, f32 i_morf, u8 i_mode, f32 i_speed) {
    e_db_leaf_class* pleaf = (e_db_leaf_class*)fopAcM_SearchByID(i_this->leaf_actor_id);
    if (pleaf != NULL) {
        pleaf->mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_db", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f, NULL);
        return TRUE;
    }

    return FALSE;
}

static int daE_DB_Draw(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);

    J3DModel* model = i_this->modelMorf->getModel();
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);

    if (i_this->field_0x852 != 0) {
        J3DModelData* modelData = model->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* material = modelData->getMaterialNodePointer(i);
            material->getTevColor(0)->r = i_this->field_0x854;
            material->getTevColor(0)->g = i_this->field_0x854;
            material->getTevColor(0)->b = i_this->field_0x854;
        }
    }

    i_this->modelMorf->entryDL();

    if (i_this->action != ACTION_MK_ROOF) {
        cXyz sp8;
        sp8.set(actor->current.pos.x, 50.0f + actor->current.pos.y + BREG_F(18), actor->current.pos.z);
        i_this->shadowId = dComIfGd_setShadow(i_this->shadowId, 1, model, &sp8, 600.0f + BREG_F(19), 0.0f, actor->current.pos.y, fopAcM_gc_c::getGroundY(), *fopAcM_gc_c::getGroundCheck(), &actor->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    static GXColor l_color = {0x14, 0x0F, 0x00, 0xFF};
    i_this->stalkLine.update(12, l_color, &actor->tevStr);
    dComIfGd_set3DlineMat(&i_this->stalkLine);

    for (int i = 1; i < 11; i++) {
        if (i_this->thornModel[i] != NULL) {
            g_env_light.setLightTevColorType_MAJI(i_this->thornModel[i], &actor->tevStr);

            if (i_this->field_0x852 != 0) {
                J3DModelData* modelData = i_this->thornModel[i]->getModelData();
                for (u16 j = 0; j < modelData->getMaterialNum(); j++) {
                    J3DMaterial* material = modelData->getMaterialNodePointer(j);
                    material->getTevColor(0)->r = i_this->field_0x854;
                    material->getTevColor(0)->g = i_this->field_0x854;
                    material->getTevColor(0)->b = i_this->field_0x854;
                }
            }

            mDoExt_modelUpdateDL(i_this->thornModel[i]);
        }
    }

    return 1;
}

static BOOL pl_check(e_db_class* i_this, f32 i_range) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (i_this->dist_to_player < i_range && !fopAcM_otherBgCheck(actor, player)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void damage_check(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    
    if (i_this->invulnerabilityTimer == 0) {
        i_this->ccStts.Move();

        if (i_this->atSph.ChkAtShieldHit()) {
            if (i_this->action == ACTION_ESCAPE) {
                actor->speed.y = 10.0f;
                actor->speedF = -15.0f;

                i_this->field_0x860 = 0;
                actor->shape_angle.x = -0x1000;
                i_this->invulnerabilityTimer = 5;
            } else {
                i_this->invulnerabilityTimer = 6;
                i_this->action = ACTION_S_DAMAGE;
                i_this->mode = 0;

                i_this->field_0x680 = player->shape_angle.y;
                i_this->field_0x1238 = 10;
            }
            
            dComIfGp_getVibration().StartShock(4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            return;
        }

        s8 hit_type = 0;

        if (i_this->action < ACTION_ESCAPE) {
            for (int i = 0; i < 4; i++) {
                if (i_this->kukiSph[i].ChkTgHit()) {
                    i_this->invulnerabilityTimer = 10;
                    i_this->atInfo.mpCollider = i_this->kukiSph[i].GetTgHitObj();

                    if (i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                        hit_type = 1;
                    } else {
                        if (i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_BOMB)) {
                            i_this->action = ACTION_E_DEAD;
                            i_this->atInfo.mpActor = dCc_GetAc(i_this->atInfo.mpCollider->GetAc());

                            f32 var_f31 = actor->current.pos.x - i_this->atInfo.mpActor->current.pos.x;
                            f32 var_f30 = actor->current.pos.z - i_this->atInfo.mpActor->current.pos.z;
                            i_this->field_0x680 = cM_atan2s(-var_f31, -var_f30);
                            i_this->mode = 0;
                            return;
                        }

                        at_power_check(&i_this->atInfo);
                        hit_type = 1;
                    }
                    break;
                }
            }
        }

        if (i_this->ccSph.ChkTgHit()) {
            i_this->field_0x1238 = 10;
            i_this->atInfo.mpCollider = i_this->ccSph.GetTgHitObj();
            cc_at_check(actor, &i_this->atInfo);

            i_this->invulnerabilityTimer = 6;

            if ((dComIfGp_event_runCheck() || i_this->field_0x850 != 0) && i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG | AT_TYPE_40)) {
                hit_type = 1;
            } else if (i_this->atInfo.mHitType == 16) {
                if (i_this->action == ACTION_ESCAPE) {
                    i_this->field_0x858 = 60.0f;
                    i_this->field_0x85c = i_this->atInfo.mHitDirection.y;
                    i_this->mode = 10;
                } else {
                    i_this->action = ACTION_S_DAMAGE;
                    i_this->mode = 0;
                    i_this->field_0x680 = player->shape_angle.y;
                }
            } else if (i_this->action < ACTION_ESCAPE) {
                i_this->action = ACTION_S_DAMAGE;
                i_this->mode = 0;
                i_this->field_0x680 = i_this->atInfo.mHitDirection.y;
                actor->health = 30;
            } else if (i_this->action == ACTION_ESCAPE) {
                if (actor->health <= 0) {
                    i_this->field_0x680 = i_this->atInfo.mHitDirection.y;
                    i_this->mode = 0;
                    i_this->action = ACTION_E_DEAD;
                    return;
                } else {
                    i_this->field_0x858 = 60.0f;
                    i_this->field_0x85c = i_this->atInfo.mHitDirection.y;
                    i_this->mode = 10;
                }
            }
        }

        if (actor->health <= 10) {
            actor->health = 0;
            i_this->ccSph.SetTgHitMark((CcG_Tg_HitMark)3);
        }

        if (hit_type != 0) {
            if (i_this->action == ACTION_ESCAPE) {
                i_this->field_0x858 = 30.0f;
                i_this->field_0x85c = -player->shape_angle.y;
                i_this->mode = 10;
                actor->speed.y = 5.0f;
            } else {
                i_this->action = ACTION_ESCAPE;
                i_this->mode = 0;
                i_this->field_0x84e = 5000;
                i_this->field_0x850 = 0;
                actor->speedF = 0.0f;
                i_this->timers[1] = 45;

                dComIfGp_particle_set(0x81C1, &i_this->field_0x69c[4], NULL, NULL);

                i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_CUT, -1);
                def_se_set(&i_this->sound, i_this->atInfo.mpCollider, 0x21, NULL);

                if (i_this->arg0 == 1) {
                    i_this->field_0x864 = 20.0f;
                    actor->speed.y = 5.0f;
                    actor->field_0x560 = 10;
                    actor->health = 10;
                } else if (i_this->arg0 == 2) {
                    i_this->field_0x864 = 20.0f;
                    actor->speed.y = 5.0f;
                } else {
                    i_this->field_0x864 = -10.0f;
                    actor->speed.y = 30.0f;
                }
            }
        }
    }
}

static u8 l_HIOInit;

static daE_DB_HIO_c l_HIO;

static void e_db_stay(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    cXyz sp14;
    i_this->invulnerabilityTimer = 15;

    switch (i_this->mode) {
    case 0:
        i_this->field_0x674.y = 0.0f;
        /* fallthrough */
    case 1:
        anm_init(i_this, 0xD, 10.0f, 0, 1.0f);
        if (leaf_anm_init(i_this, 0x14, 10.0f, 2, 0.0f)) {
            i_this->mode = 5;
            i_this->field_0x68c = 0.0f;
            i_this->timers[0] = 50;
        }
        break;
    case 2:
        if (i_this->field_0x850 != 0) {
            i_this->field_0x674.y = -170.0f;
        } else {
            i_this->field_0x674.y = 170.0f;
        }
        i_this->mode = 1;
        break;
    case 5:
        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x, 0.2f, 10.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.y, actor->home.pos.y + i_this->field_0x674.y, 0.2f, 10.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z, 0.2f, 10.0f * i_this->field_0x68c);
        cLib_addCalc2(&i_this->field_0x68c, 1.0f, 1.0f, 0.1f);

        sp14 = actor->current.pos - actor->home.pos;
        sp14.y = 0.0f;
        if (sp14.abs() < 20.0f) {
            cLib_addCalc0(&i_this->field_0x674.y, 1.0f, 4.0f);

            if (i_this->field_0x850 != 0) {
                cLib_addCalcAngleS2(&actor->shape_angle.x, 0x4000, 4, 0x200);
            } else {
                cLib_addCalcAngleS2(&actor->shape_angle.x, -0x4000, 4, 0x200);
            }

            cLib_addCalc2(&i_this->field_0x844, 0.4f, 1.0f, 0.02f);
            cLib_addCalc2(&i_this->size, l_HIO.flower_size, 1.0f, 0.05f);
        }

        if (i_this->timers[0] == 0 && (i_this->arg3 == 1 || pl_check(i_this, i_this->pl_search_range))) {
            if (i_this->field_0x850 != 0) {
                i_this->action = ACTION_APPEAR_V;
            } else {
                i_this->action = ACTION_APPEAR;
            }
            i_this->mode = 0;
        }
        break;
    }
}

static void e_db_appear(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    int anm_frame = i_this->modelMorf->getFrame();

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 5, 10.0f, 0, 1.0f);
        leaf_anm_init(i_this, 0x13, 10.0f, 2, 1.0f);
        i_this->mode = 1;

        i_this->field_0x674.x = actor->home.pos.x + cM_rndFX(100.0f);
        i_this->field_0x674.z = actor->home.pos.z + cM_rndFX(100.0f);
        if (i_this->field_0x850 != 0) {
            i_this->field_0x674.y = actor->home.pos.y - (170.0f + cM_rndFX(50.0f));
        } else {
            i_this->field_0x674.y = 170.0f + actor->home.pos.y + cM_rndFX(50.0f);
        }

        i_this->field_0x68c = 0.2f;
        i_this->sound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);
        i_this->sound.startCreatureSound(Z2SE_EN_DB_APPEAR, 0, -1);
        break;
    case 1:
        if (anm_frame >= 15 && anm_frame <= 35) {
            i_this->field_0x1239 = 2;
        }

        i_this->field_0xb14 = 1;
        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x, 0.1f, 30.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z, 0.1f, 30.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y, 0.1f, 30.0f * i_this->field_0x68c);
        cLib_addCalc2(&i_this->field_0x68c, 1.0f, 1.0f, 0.05f);

        if (i_this->modelMorf->isStop()) {
            i_this->action = ACTION_WAIT;
            i_this->mode = 0;
        }
        break;
    }

    if (i_this->field_0x850 != 0) {
        cLib_addCalcAngleS2(&actor->shape_angle.y, (i_this->angle_to_player + 0x8000), 8, 0x800);
        cLib_addCalcAngleS2(&actor->shape_angle.x, -0x8000, 8, 0x400);
    } else {
        cLib_addCalcAngleS2(&actor->shape_angle.y, i_this->angle_to_player, 8, 0x800);
        cLib_addCalcAngleS2(&actor->shape_angle.x, 0, 8, 0x400);
    }

    cLib_addCalc2(&i_this->size, l_HIO.appear_size, 1.0f, 0.1f);
}

static void e_db_appear_v(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    
    cXyz spC;
    f32 temp_f31 = 60.0f + TREG_F(17);

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 5, 5.0f, 0, 1.0f);
        leaf_anm_init(i_this, 0x13, 5.0f, 2, 1.0f);
        i_this->mode = 1;
        i_this->timers[0] = JREG_S(6) + 30;
        i_this->field_0x68c = 0.0f;
        i_this->modelMorf->setFrame(15.0f);
        /* fallthrough */
    case 1:
        spC.x = temp_f31 * cM_ssin(i_this->counter * (TREG_S(0) + 1200));
        spC.z = temp_f31 * cM_scos(i_this->counter * (TREG_S(2) + 1500));

        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x + spC.x, 0.1f, 40.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z + spC.z, 0.1f, 40.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.y, actor->home.pos.y - (400.0f + JREG_F(18)), 0.5f, 40.0f * i_this->field_0x68c);
        cLib_addCalc2(&i_this->field_0x68c, 1.0f, 1.0f, 0.2f + JREG_F(5));

        if (i_this->timers[0] == 0) {
            i_this->action = ACTION_WAIT;
            i_this->mode = 0;
            i_this->field_0x848 = 0.0f;
        }
        break;
    }

    cLib_addCalcAngleS2(&actor->shape_angle.x, 0x4000, 4, 0x400);
    cLib_addCalc2(&i_this->field_0x844, 0.5f + JREG_F(4), 1.0f, 0.05f);
    cLib_addCalc2(&i_this->size, l_HIO.appear_size, 1.0f, 0.15f);
    i_this->field_0xb14 = 1;
}

static void e_db_wait(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp48;
    f32 temp_f31 = 30.0f + TREG_F(12);

    if ((i_this->counter & 15) == 0 && cM_rndF(1.0f) < 0.75f) {
        i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_WAIT, -1);
    }

    switch (i_this->mode) {
    case -1:
        if (i_this->modelMorf->isStop()) {
            i_this->mode = 0;
        } else {
            cXyz sp3C(0.0f, 90.0f, -100.0f);
            cXyz sp30;
            cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
            MtxPosition(&sp3C, &sp30);
            i_this->field_0x674 = actor->home.pos + sp30;

            cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x, 0.3f, 30.0f);
            cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y, 0.3f, 30.0f);
            cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z, 0.3f, 30.0f);
            break;
        }
    case 0:
        anm_init(i_this, 0x12, 10.0f, 2, 1.0f);
        leaf_anm_init(i_this, 0x14, 10.0f, 2, 1.0f);
        i_this->mode = 1;

        if (i_this->arg2 == 1) {
            i_this->timers[1] = 30.0f + cM_rndF(20.0f);
        } else {
            i_this->timers[1] = l_HIO.attack_freq + cM_rndF(l_HIO.attack_freq);
        }

        i_this->timers[0] = 0;
        break;
    case 1:
        if (i_this->timers[0] == 0) {
            i_this->timers[0] = 10.0f + cM_rndF(30.0f);
            i_this->field_0x674.x = actor->home.pos.x + cM_rndFX(100.0f);
            i_this->field_0x674.z = actor->home.pos.z + cM_rndFX(100.0f);

            if (i_this->field_0x850 != 0) {
                i_this->field_0x674.y = actor->home.pos.y - (170.0f + cM_rndFX(50.0f));
            } else {
                i_this->field_0x674.y = 170.0f + actor->home.pos.y + cM_rndFX(50.0f);
            }

            i_this->field_0x68c = 0.0f;
        }

        sp48.x = temp_f31 * cM_ssin(i_this->counter * (TREG_S(0) + 1900));
        sp48.y = temp_f31 * cM_ssin(i_this->counter * (TREG_S(1) + 2200));
        sp48.z = temp_f31 * cM_scos(i_this->counter * (TREG_S(2) + 2000));

        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x + sp48.x, 0.1f, 20.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z + sp48.z, 0.1f, 20.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y + sp48.y, 0.1f, 20.0f * i_this->field_0x68c);
        cLib_addCalc2(&i_this->field_0x68c, 1.0f, 1.0f, 0.05f);

        if (i_this->arg3 != 1 && !pl_check(i_this, 100.0f + i_this->pl_search_range)) {
            i_this->action = ACTION_STAY;
            i_this->mode = 2;
            i_this->sound.startCreatureSound(Z2SE_EN_DB_HIKKOMU, 0, -1);
        } else if (!daPy_getPlayerActorClass()->getDkCaught() && !daPy_getPlayerActorClass()->getDkCaught2() && i_this->timers[1] == 0 && pl_check(i_this, 700.0f)) {
            i_this->action = ACTION_ATTACK;
            i_this->mode = 0;
        }
        break;
    }

    if (i_this->field_0x850 != 0) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cLib_addCalcAngleS2(&actor->shape_angle.y, (i_this->angle_to_player + 0x8000), 8, 0x800);
        
        cXyz sp24 = player->eyePos - actor->current.pos;
        int sp8 = cM_atan2s(sp24.y, JMAFastSqrt(SQUARE(sp24.x) + SQUARE(sp24.z)));
        cLib_addCalcAngleS2(&actor->shape_angle.x, (sp8 + 0x8000), 8, 0x400);
        i_this->field_0xb14 = 1;
    } else {
        cLib_addCalcAngleS2(&actor->shape_angle.y, i_this->angle_to_player, 8, 0x800);
        cLib_addCalcAngleS2(&actor->shape_angle.x, 0, 8, 0x400);
    }
}

static void e_db_mk_roof(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz spC;
    f32 temp_f31 = 30.0f + TREG_F(12);

    if ((i_this->counter & 15) == 0 && cM_rndF(1.0f) < 0.75f) {
        i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_WAIT, -1);
    }

    switch (i_this->mode) {
    case 0:
        if (i_this->timers[0] == 0) {
            anm_init(i_this, 0x12, 10.0f, 2, 1.0f);
            leaf_anm_init(i_this, 0x14, 10.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->timers[1] = 50.0f + cM_rndF(50.0f);
            i_this->timers[0] = 0;
            i_this->size = l_HIO.appear_size;
        } else {
            i_this->size = l_HIO.flower_size;
        }
        break;
    case 1:
        if (i_this->timers[0] == 0) {
            i_this->timers[0] = 10.0f + cM_rndF(30.0f);
            i_this->field_0x674.x = actor->home.pos.x + cM_rndFX(70.0f);
            i_this->field_0x674.z = actor->home.pos.z + cM_rndFX(70.0f);
            i_this->field_0x674.y = (actor->home.pos.y - 250.0f) + BREG_F(1);
            i_this->field_0x68c = 0.0f;
        }

        spC.x = temp_f31 * cM_ssin(i_this->counter * (TREG_S(0) + 1900));
        spC.z = temp_f31 * cM_scos(i_this->counter * (TREG_S(2) + 2000));
        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x + spC.x, 0.1f, 20.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z + spC.z, 0.1f, 20.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y, 0.1f, 20.0f * i_this->field_0x68c);
        cLib_addCalc2(&i_this->field_0x68c, 1.0f, 1.0f, 0.05f);
        break;
    }

    actor->shape_angle.x = 0x4000;
    actor->shape_angle.z = 0;
}

static void e_db_attack(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz sp78;
    cXyz sp6C;
    s16 spA = 0;

    if (i_this->field_0x850 != 0) {
        sp78 = player->eyePos - actor->current.pos;
        spA = (s16)cM_atan2s(sp78.y, JMAFastSqrt(SQUARE(sp78.x) + SQUARE(sp78.z)));
    }

    switch (i_this->mode) {
    case 0:
        i_this->mode = 1;
        i_this->timers[0] = 17;
        i_this->field_0x68c = 0.0f;
        i_this->field_0xb15 = 0;

        i_this->sound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);
        i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_ATTACK, -1);

        leaf_anm_init(i_this, 0x13, 5.0f, 2, 1.0f);
        anm_init(i_this, 6, 5.0f, 0, 1.0f);
        /* fallthrough */
    case 1:
        if (i_this->field_0x850 != 0) {
            cLib_addCalcAngleS2(&actor->shape_angle.y, (i_this->angle_to_player + 0x8000), 8, 0x800);
            sp78.x = 0.0f;
            sp78.y = -170.0f + TREG_F(3);
            sp78.z = 100.0f + TREG_F(4);
        } else {
            cLib_addCalcAngleS2(&actor->shape_angle.y, i_this->angle_to_player, 8, 0x800);
            sp78.x = 0.0f;
            sp78.y = 170.0f + TREG_F(3);
            sp78.z = -100.0f + TREG_F(4);
        }

        cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
        MtxPosition(&sp78, &sp6C);
        i_this->field_0x674 = actor->home.pos + sp6C;

        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x, 0.1f, 20.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z, 0.1f, 20.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y, 0.1f, 20.0f * i_this->field_0x68c);
        cLib_addCalc2(&i_this->field_0x68c, 1.0f, 1.0f, 0.1f);

        if (i_this->timers[0] == 0) {
            i_this->mode = 2;
            i_this->timers[1] = 10;

            if (i_this->field_0x850 != 0) {
                cMtx_YrotS(*calc_mtx, (actor->shape_angle.y + 0x8000));
                cMtx_XrotM(*calc_mtx, -spA);
                sp78.x = 0.0f;
                sp78.y = TREG_F(5);
                sp78.z = 350.0f + TREG_F(6);
                MtxPosition(&sp78, &sp6C);
                i_this->field_0x674 = actor->current.pos + sp6C;
            } else {
                cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
                sp78.x = 0.0f;
                sp78.y = 0.0f;
                sp78.z = 350.0f + TREG_F(6);
                MtxPosition(&sp78, &sp6C);
                i_this->field_0x674 = actor->home.pos + sp6C;
                i_this->field_0x674.y = player->current.pos.y + 100.0f;
            }

            actor->speed.x = fabsf(i_this->field_0x674.x - actor->current.pos.x);
            actor->speed.y = fabsf(i_this->field_0x674.y - actor->current.pos.y);
            actor->speed.z = fabsf(i_this->field_0x674.z - actor->current.pos.z);
            i_this->field_0x68c = 0.0f;

            i_this->atSph.StartCAt(actor->current.pos);

            if (i_this->field_0x850 == 0) {
                if (cM_rndF(1.0f) < 0.5f) {
                    i_this->field_0x860 = 0x4000;
                } else {
                    i_this->field_0x860 = -0x4000;
                }
            }

            i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_BITE, -1);
        }
        break;
    case 2:
        if (i_this->timers[1] != 0) {
            i_this->field_0xb14 = 1;
        }

        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x, 0.8f, actor->speed.x * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z, 0.8f, actor->speed.z * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y, 0.8f, actor->speed.y * i_this->field_0x68c);
        cLib_addCalc2(&i_this->field_0x68c, 0.2f, 1.0f, 0.05f);
        cLib_addCalcAngleS2(&actor->shape_angle.z, i_this->field_0x860, 2, 0x2000);

        if (!daPy_getPlayerActorClass()->getDkCaught() && !daPy_getPlayerActorClass()->getDkCaught2() && i_this->atSph.ChkAtHit() && player == dCc_GetAc(i_this->atSph.GetAtHitObj()->GetAc())) {
            OS_REPORT("E_DB//////////////AT  SET 1 !!\n");
            i_this->mode = 5;
            i_this->field_0x68c = 20.0f;
            anm_init(i_this, 7, 2.0f, 2, 1.0f);
            i_this->timers[0] = 120;

            daPy_getPlayerActorClass()->setDkCaught(actor);
            dComIfGp_getVibration().StartShock(6, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        } else if (i_this->modelMorf->isStop()) {
            i_this->mode = 3;
            i_this->timers[0] = 0;
        }
        break;
    case 3:
        if (i_this->timers[0] == 0) {
            i_this->action = ACTION_WAIT;
            i_this->mode = 0;
        }
        break;
    case 5:
        i_this->field_0x851 = 1;
        i_this->sound.startCreatureVoiceLevel(Z2SE_EN_DB_V_BITING, -1);
        i_this->ccSph.OffCoSetBit();
        spA = 0;

        if (daPy_getPlayerActorClass()->getDkCaught()) {
            if (i_this->timers[0] == 30) {
                dComIfGp_setItemLifeCount(-1.0f, 0);
            }

            if ((i_this->counter & 15) == 0) {
                dComIfGp_getVibration().StartShock(3, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            }

            sp78 = player->current.pos - actor->home.pos;
            s16 sp8 = cM_atan2s(sp78.x, sp78.z);
            sp78.y = 0.0f;

            f32 temp_f1 = sp78.abs();
            if (temp_f1 > (100.0f + YREG_F(2))) {
                f32 var_f30 = (0.1f + YREG_F(0)) * (temp_f1 - (100.0f + YREG_F(2)));
                if (var_f30 > (100.0f + YREG_F(1))) {
                    var_f30 = 100.0f + YREG_F(1);
                }
                daPy_getPlayerActorClass()->setOutPower(var_f30, (sp8 + 0x8000), 0);
            }

            if (i_this->field_0x850 == 0) {
                if (i_this->field_0x860 == 0) {
                    if (cM_rndF(1.0f) < 0.5f) {
                        i_this->field_0x860 = 0x4000;
                    } else {
                        i_this->field_0x860 = -0x4000;
                    }
                }

                cMtx_YrotS(*calc_mtx, sp8);
                cLib_addCalcAngleS2(&actor->shape_angle.z, i_this->field_0x860 + ((2000.0f + KREG_F(17)) * cM_ssin((i_this->counter * (KREG_S(0) + 12000)))), 2, 0x2000);
                sp78.x = 0.0f;
                sp78.y = (100.0f + XREG_F(2)) - 30.0f;
                sp78.z = XREG_F(3) - 70.0f;
                MtxPosition(&sp78, &i_this->field_0x674);
                i_this->field_0x674 += player->current.pos;
                cLib_addCalcAngleS2(&actor->shape_angle.y, sp8, 2, 0x1000);
            } else {
                spA = -0x4000;
                i_this->field_0x674 = player->eyePos;
                i_this->field_0x674.y += 80.0f + JREG_F(8);
            }

            cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x, 1.0f, i_this->field_0x68c);
            cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z, 1.0f, i_this->field_0x68c);
            cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y, 1.0f, i_this->field_0x68c);
            cLib_addCalc2(&i_this->field_0x68c, 200.0f, 1.0f, 10.0f);
        } else {
            i_this->action = ACTION_WAIT;
            i_this->mode = -1;
            anm_init(i_this, 0x10, 2.0f, 0, 1.0f);
            i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_RELEASE, -1);
            i_this->field_0x858 = 20.0f;
            i_this->field_0x85c = actor->shape_angle.y;
        }
        break;
    }

    if (i_this->field_0xb14 != 0) {
        f32 temp_f31 = 50.0f + JREG_F(11);
        actor->current.pos.y -= temp_f31;
        actor->old.pos.y -= temp_f31;

        i_this->acch.CrrPos(dComIfG_Bgsp());

        actor->current.pos.y += temp_f31;
        actor->old.pos.y += temp_f31;
    }

    if (i_this->field_0x850 != 0) {
        cLib_addCalcAngleS2(&actor->shape_angle.x, (spA + 0x8000), 8, 0x400);
        i_this->field_0xb14 = 1;
    } else {
        cLib_addCalcAngleS2(&actor->shape_angle.x, -spA, 8, 0x400);
    }
}

static void e_db_attack_s(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* temp_r28 = dComIfGp_getPlayer(0);
    cXyz sp28;

    sp28 = temp_r28->eyePos - actor->current.pos;
    s16 temp_r27 = (s16)cM_atan2s(sp28.y, JMAFastSqrt(SQUARE(sp28.x) + SQUARE(sp28.z)));

    switch (i_this->mode) {
    case 0:
        i_this->sound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);
        i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_ATTACK, -1);

        leaf_anm_init(i_this, 0x13, 5.0f, 2, 1.0f);
        anm_init(i_this, 6, 5.0f, 0, 1.0f);
        i_this->atSph.StartCAt(actor->current.pos);

        i_this->timers[0] = YREG_S(7) + 15;
        i_this->field_0x68c = 0.0f;
        i_this->mode = 1;
        /* fallthrough */
    case 1:
        i_this->field_0x674 = temp_r28->eyePos;
        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x, 0.8f, 50.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z, 0.8f, 50.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y, 0.8f, 50.0f * i_this->field_0x68c);
        cLib_addCalc2(&i_this->field_0x68c, 1.0f, 1.0f, 0.3f);

        sp28 = actor->current.pos - actor->home.pos;
        if (i_this->timers[0] == 0 || sp28.abs() > 400.0f + YREG_F(17)) {
            i_this->action = 3;
            i_this->mode = 0;
        }
        break;
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, (i_this->angle_to_player + 0x8000), 8, 0x800);
    cLib_addCalcAngleS2(&actor->shape_angle.x, (temp_r27 + 0x8000), 8, 0x400);
    i_this->field_0xb14 = 1;
}

static void e_db_chance(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    cXyz sp14;
    f32 temp_f31 = 60.0f + TREG_F(17);

    switch (i_this->mode) {
    case 0:
        if (i_this->field_0x850 != 0) {
            anm_init(i_this, 0xA, 5.0f, 2, 1.0f);
        } else {
            anm_init(i_this, 9, 5.0f, 2, 1.0f);
        }

        leaf_anm_init(i_this, 0x14, 5.0f, 2, 1.0f);
        i_this->mode = 1;
        i_this->timers[0] = 60;
        i_this->field_0x68c = 0.0f;
        /* fallthrough */
    case 1:
        sp14.x = temp_f31 * cM_ssin(i_this->counter * (TREG_S(0) + 900));
        sp14.z = temp_f31 * cM_scos(i_this->counter * (TREG_S(2) + 1000));

        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x + sp14.x, 0.1f, 20.0f * i_this->field_0x68c);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z + sp14.z, 0.1f, 20.0f * i_this->field_0x68c);

        if (i_this->field_0x850 != 0) {
            cLib_addCalc2(&actor->current.pos.y, actor->home.pos.y - (230.0f + TREG_F(18)), 0.1f, 20.0f * i_this->field_0x68c);
        } else {
            cLib_addCalc2(&actor->current.pos.y, 230.0f + actor->home.pos.y + TREG_F(18), 0.1f, 20.0f * i_this->field_0x68c);
        }

        cLib_addCalc2(&i_this->field_0x68c, 1.0f, 1.0f, 0.1f);

        if (i_this->timers[0] <= 50 && i_this->timers[0] >= 10) {
            i_this->sound.startCreatureSoundLevel(Z2SE_EN_DB_V_FAINT, 0, -1);
        }

        if (i_this->timers[0] == 0) {
            i_this->mode++;
            anm_init(i_this, 0x11, 3.0f, 0, 1.0f);
            leaf_anm_init(i_this, 0x13, 3.0f, 2, 1.0f);
            i_this->sound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);
        }
        break;
    case 2:
        if (i_this->modelMorf->isStop()) {
            i_this->action = ACTION_WAIT;
            i_this->mode = 0;
            i_this->field_0x848 = 0.0f;
        }
        break;
    }

    if (i_this->field_0x850 != 0) {
        cLib_addCalcAngleS2(&actor->shape_angle.x, 0x4000, 4, 0x400);
    } else {
        cLib_addCalcAngleS2(&actor->shape_angle.x, -0x4000, 4, 0x400);
    }

    cLib_addCalc2(&i_this->field_0x844, 0.5f + JREG_F(4), 1.0f, 0.05f);
}

static void e_db_s_damage(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp10;

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 0xB, 3.0f, 0, 1.0f);
        leaf_anm_init(i_this, 0x13, 3.0f, 2, 0.0f);
        i_this->sound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);

        if (i_this->field_0x850 != 0) {
            cMtx_YrotS(*calc_mtx, (actor->shape_angle.y + 0x8000));
        } else {
            cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
        }

        sp10.x = 0.0f;
        sp10.y = 15.0f;
        sp10.z = -50.0f + TREG_F(16);
        MtxPosition(&sp10, &actor->speed);
        i_this->mode = 1;
        i_this->timers[0] = 10;
        i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_DAMAGE_S, -1);
        break;
    case 1:
        actor->current.pos += actor->speed;
        sp10.x = actor->current.pos.x - actor->home.pos.x;
        sp10.z = actor->current.pos.z - actor->home.pos.z;

        if (JMAFastSqrt(SQUARE(sp10.x) + SQUARE(sp10.z)) > 250.0f + nREG_F(8)) {
            actor->speed.zero();
        } else {
            actor->speed.x *= 0.92f;
            actor->speed.y *= 0.92f;
            actor->speed.z *= 0.92f;
        }

        if (i_this->timers[0] == 0) {
            i_this->action = ACTION_CHANCE;
            i_this->mode = 0;
        }
        break;
    }

    if (i_this->field_0x850 != 0) {
        cLib_addCalcAngleS2(&actor->shape_angle.x, 0x4000, 4, 0x400);
    } else {
        cLib_addCalcAngleS2(&actor->shape_angle.x, -0x4000, 4, 0x400);
    }
}

static void e_db_damage(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp1C;

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 0xB, 3.0f, 0, 1.0f);
        leaf_anm_init(i_this, 0x13, 3.0f, 2, 0.0f);
        i_this->sound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);
        cMtx_YrotS(*calc_mtx, i_this->field_0x680);

        sp1C.x = 0.0f;
        if (i_this->arg0 != 1) {
            if (i_this->arg0 == 2) {
                sp1C.y = 20.0f + AREG_F(1);
            } else {
                sp1C.y = 30.0f;
            }
            sp1C.z = -50.0f;
        } else {
            sp1C.y = 0.0f;
            sp1C.z = -20.0f;
        }
        MtxPosition(&sp1C, &actor->speed);

        i_this->mode = 1;
        i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_DAMAGE, -1);
        break;
    case 1:
        sp1C = actor->current.pos - actor->home.pos;
        if (sp1C.abs() > (450.0f + TREG_F(15))) {
            actor->current.pos = actor->old.pos;
            actor->speed.x *= -0.1f;
            actor->speed.z *= -0.1f;
            i_this->mode++;
        }
    case 2:
        s16 var_r27;
        if (i_this->arg0 == 2) {
            var_r27 = 0;
        } else {
            var_r27 = -0x8000;
        }
        cLib_addCalcAngleS2(&actor->shape_angle.y, (i_this->field_0x680 + var_r27), 2, 0x2000);
        actor->current.pos += actor->speed;

        if (i_this->arg0 != 1) {
            if (i_this->arg0 == 2) {
                actor->speed.y -= 1.0f + AREG_F(0);
            } else {
                actor->speed.y -= 5.0f;
            }
        }

        f32 temp_f31 = ((40.0f + TREG_F(15)) * i_this->size) * l_HIO.base_size;

        actor->current.pos.y -= temp_f31;
        actor->old.pos.y -= temp_f31;
        i_this->acch.CrrPos(dComIfG_Bgsp());
        actor->current.pos.y += temp_f31;
        actor->old.pos.y += temp_f31;

        if (i_this->modelMorf->isStop()) {
            if (i_this->arg0 == 1) {
                i_this->action = ACTION_MK_ROOF;
            } else {
                i_this->action = ACTION_WAIT;
            }

            i_this->mode = 0;
        }
    }
}

static void kuki_control1(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp40;
    cXyz sp34;
    cXyz sp28;

    int i;
    cXyz* var_r31 = i_this->field_0x69c;
    *var_r31 = actor->home.pos;

    cXyz sp1C;
    sp40.x = 0.0f;
    sp40.y = 0.0f;
    sp40.z = i_this->field_0x840;

    var_r31++;

    f32 temp_f30;
    f32 temp_f25;
    f32 temp_f29;
    f32 var_f27;
    f32 var_f26;

    f32 var_f31 = 50.0f + TREG_F(2);
    sp28.x = 0.0f;
    sp28.z = 0.0f;

    if (i_this->action == ACTION_CHANCE) {
        sp28.y = 0.0f;
        var_f31 = 70.0f + JREG_F(1);
        sp1C.set(0.0f, -30.0f + JREG_F(3), 0.0f);
    } else if (i_this->action == ACTION_MK_ROOF) {
        sp28.y = 0.0f;
        var_f31 = 70.0f + JREG_F(1);
        sp1C.set(0.0f, 30.0f + JREG_F(3), 0.0f);
    } else {
        sp28.y = -(200.0f + TREG_F(0));
    }
    
    f32 temp_f28 = i_this->field_0x848;

    for (i = 1; i < 12; i++, var_r31++) {
        if (i_this->action == ACTION_CHANCE || i_this->action == ACTION_MK_ROOF) {
            var_f27 = var_f31 * cM_ssin((i_this->counter * (TREG_S(0) + 1000)) + (i * (TREG_S(1) + 12000)));
            var_f26 = var_f31 * cM_scos((i_this->counter * (TREG_S(2) + 1300)) + (i * (TREG_S(3) + 10000)));
        } else {
            var_f27 = var_f31 * cM_ssin((i_this->counter * (TREG_S(0) + 2000)) + (i * (TREG_S(1) + 12000)));
            var_f26 = var_f31 * cM_scos((i_this->counter * (TREG_S(2) + 2300)) + (i * (TREG_S(3) + 10000)));

            static s16 pow_xa[] = {
                0x0000,
                0x0000,
                0x1000,
                0x2000,
                0x3000,
                0x4000,
                0x3000,
                0x2000,
                0x1000,
                0x0000,
                0x0000,
            };

            if (i_this->field_0x850 != 0) {
                cMtx_YrotS(*calc_mtx, (s16)actor->shape_angle.y);
                cMtx_XrotM(*calc_mtx, pow_xa[i - 1] + 0x8000);
            } else {
                cMtx_YrotS(*calc_mtx, (s16)actor->shape_angle.y);
                cMtx_XrotM(*calc_mtx, pow_xa[i - 1]);
            }

            MtxScale(temp_f28, temp_f28, temp_f28, 1);
            MtxPosition(&sp28, &sp1C);
        }

        temp_f30 = var_f27 + (sp1C.x + (var_r31[0].x - var_r31[-1].x));
        temp_f25 = (var_r31[0].y - var_r31[-1].y) - sp1C.y;
        temp_f29 = var_f26 + (sp1C.z + (var_r31[0].z - var_r31[-1].z));

        s16 spA, sp8;
        sp8 = (s16)cM_atan2s(temp_f30, temp_f29);
        spA = -cM_atan2s(temp_f25, JMAFastSqrt(SQUARE(temp_f30) + SQUARE(temp_f29)));
        cMtx_YrotS(*calc_mtx, sp8);
        cMtx_XrotM(*calc_mtx, spA);
        MtxPosition(&sp40, &sp34);

        *var_r31 = var_r31[-1] + sp34;
    }
}

static void kuki_control2(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp28;
    cXyz sp1C;

    int i;
    s16 temp_r27, temp_r26;
    cXyz* var_r31 = &i_this->field_0x69c[11];
    csXyz* var_r30 = &i_this->field_0x72c[11];

    *var_r31 = actor->current.pos;

    sp28.x = 0.0f;
    sp28.y = 0.0f;
    sp28.z = i_this->field_0x840;

    var_r31--;
    var_r30--;

    for (i = 10; i >= 0; i--, var_r31--, var_r30--) {
        f32 temp_f31 = var_r31[0].x - var_r31[1].x;
        f32 temp_f29 = var_r31[0].y - var_r31[1].y;
        f32 temp_f30 = var_r31[0].z - var_r31[1].z;

        temp_r26 = (s16)cM_atan2s(temp_f31, temp_f30);
        temp_r27 = -cM_atan2s(temp_f29, JMAFastSqrt(SQUARE(temp_f31) + SQUARE(temp_f30)));
        cMtx_YrotS(*calc_mtx, temp_r26);
        cMtx_XrotM(*calc_mtx, temp_r27);
        MtxPosition(&sp28, &sp1C);
        *var_r31 = var_r31[1] + sp1C;
        var_r30->x = temp_r27;
        var_r30->y = temp_r26;
    }
}

static void kuki_control3(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp20;

    sp20 = i_this->field_0x69c[0] - actor->home.pos;

    cXyz* var_r29 = i_this->field_0x69c;
    for (int i = 0; i < 12; i++, var_r29++) {
        *var_r29 -= sp20 * ((12 - i) / 11.0f);
    }
}

static void kuki_ha_set(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz* ppos = i_this->field_0x69c;
    csXyz* prot = i_this->field_0x72c;

    for (int i = 0; i < 12; i++, ppos++, prot++) {
        if (i > 0 && i < 11) {
            if (i_this->thornModel[i] != NULL) {
                mDoMtx_stack_c::transS(ppos->x, ppos->y, ppos->z);
                mDoMtx_stack_c::YrotM(prot->y);
                mDoMtx_stack_c::XrotM(prot->x);
                mDoMtx_stack_c::ZrotM(i << 13);
                mDoMtx_stack_c::scaleM(i_this->thorn_size[i], i_this->thorn_size[i], i_this->thorn_size[i]);
                i_this->thornModel[i]->setBaseTRMtx(mDoMtx_stack_c::get());
            }
        }
    }
}

static void kuki_control1_e(e_db_class* i_this) {
    fopAc_ac_c* sp18 = &i_this->enemy;
    cXyz sp58;
    cXyz sp4C;
    Vec sp40;
    dBgS_GndChk sp64;
    int i;

    i_this->field_0x69c[0] = sp18->current.pos;

    cXyz* var_r30 = &i_this->field_0x69c[1];
    csXyz* sp10 = &i_this->field_0x72c[1];
    f32* var_r28 = &i_this->field_0x7a4[1];

    sp58.x = 0.0f;
    sp58.y = 0.0f;
    sp58.z = i_this->field_0x840;
    f32 temp_f27 = i_this->field_0x864;
    cLib_addCalc2(&i_this->field_0x864, -10.0f, 1.0f, 0.5f);

    cXyz sp34;
    cXyz sp28;
    i_this->field_0x84c += i_this->field_0x84e;

    for (i = 1; i < 12; i++, var_r30++, sp10++, var_r28++) {
        cMtx_YrotS(*calc_mtx, sp18->shape_angle.y);
        sp28.x = (10.0f + KREG_F(13)) * cM_ssin(i_this->field_0x84c + (i * 10000));
        sp28.y = 0.0f;
        sp28.z = -5.0f + KREG_F(15);
        MtxPosition(&sp28, &sp34);
        f32 temp_f31 = sp34.x + (var_r30[0].x - var_r30[-1].x);
        f32 var_f29 = var_r30[0].y + temp_f27;

        if (var_f29 <= *var_r28) {
            var_f29 = *var_r28;
        }

        f32 temp_f28 = var_f29 - var_r30[-1].y;
        f32 temp_f30 = sp34.z + (var_r30[0].z - var_r30[-1].z);

        s16 spA, sp8;
        sp8 = (s16)cM_atan2s(temp_f31, temp_f30);
        spA = -cM_atan2s(temp_f28, JMAFastSqrt(SQUARE(temp_f31) + SQUARE(temp_f30)));

        cMtx_YrotS(*calc_mtx, sp8);
        cMtx_XrotM(*calc_mtx, spA);
        MtxPosition(&sp58, &sp4C);
        *var_r30 = var_r30[-1] + sp4C;
        sp10->x = spA;
        sp10->y = sp8;

        if (((i_this->counter + i) & 3) == 0) {
            sp40 = *var_r30;
            sp40.y += 100.0f;

            sp64.SetPos(&sp40);
            *var_r28 = 5.0f + dComIfG_Bgsp().GroundCross(&sp64);
            if (*var_r28 - var_r30->y > 200.0f) {
                *var_r28 = var_r30->y;
            }
        } else {
            *var_r28 = 5.0f + i_this->acch.GetGroundH();
        }
    }
}

static s8 e_db_escape(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz sp60;
    cXyz sp54;

    i_this->atInfo.field_0x18 = 0;

    dBgS_ObjGndChk_Spl sp6C;
    sp60 = actor->current.pos;
    sp60.y += 100.0f;
    sp6C.SetPos(&sp60);

    f32 temp_f30 = dComIfG_Bgsp().GroundCross(&sp6C);
    f32 player_distance = fopAcM_searchPlayerDistance(actor);
    
    s8 spD = 1;
    s8 spC = 2;
    s8 spB = 1;

    switch (i_this->mode) {
    case 0:
        spC = 1;
        i_this->field_0x840 = 25.0f + TREG_F(8);
        if (i_this->acch.ChkGroundHit()) {
            actor->speed.y = 20.0f;
            i_this->mode = 1;
            anm_init(i_this, 0xF, 10.0f, 2, 1.0f);
            i_this->atSph.StartCAt(actor->current.pos);
            i_this->timers[0] = 200.0f + cM_rndF(100.0f);
            fopAcM_effSmokeSet1(&i_this->field_0x1258, &i_this->field_0x125c, &actor->current.pos, &actor->shape_angle, 1.0f + NREG_F(18), &actor->tevStr, 1);
        }
        break;
    case 1:
        cLib_addCalcAngleS2(&actor->shape_angle.y, i_this->angle_to_player, 8, 0x400);

        if (i_this->timers[0] == 0 && i_this->arg0 == 0) {
            i_this->mode = 2;
        } else if (i_this->timers[1] == 0) {
            s16 sp12 = i_this->angle_to_player - actor->shape_angle.y;
            if (sp12 < 0x1000 && sp12 > -0x1000 && player_distance < (300.0f + KREG_F(7)) && !daPy_getPlayerActorClass()->getDkCaught() && !daPy_getPlayerActorClass()->getDkCaught2()) {
                i_this->mode = 15;
                i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_BITE, -1);

                if (cM_rndF(1.0f) < 0.5f) {
                    i_this->field_0x860 = 0x4000;
                } else {
                    i_this->field_0x860 = -0x4000;
                }

                actor->speed.y = 30.0f + KREG_F(8);
                actor->speedF = 15.0f + KREG_F(9);
                actor->shape_angle.x = -0x2000;
                i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_ATTACK, -1);
            }
        }
        break;
    case 2:
        sp60 = actor->home.pos - actor->current.pos;
        sp60.y = 0.0f;
        cLib_addCalcAngleS2(&actor->shape_angle.y, cM_atan2s(sp60.x, sp60.z), 8, 0x800);

        if (sp60.abs() < 100.0f) {
            i_this->mode = 3;
            i_this->timers[0] = 25;
            leaf_anm_init(i_this, 0x13, 10.0f, 2, 1.0f);
            i_this->sound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);
        }
        break;
    case 3:
        spC = 0;
        cLib_addCalc0(&i_this->field_0x840, 1.0f, 1.0f);
        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x, 0.2f, fabsf(actor->speed.x));
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z, 0.2f, fabsf(actor->speed.z));
        cLib_addCalcAngleS2(&actor->shape_angle.x, -0x4000, 4, 0x400);

        if (i_this->timers[0] == 0) {
            i_this->action = ACTION_APPEAR;
            i_this->mode = 0;
            actor->health = 30;
            i_this->atInfo.field_0x18 = 45;
        }
        break;
    case 10:
        anm_init(i_this, 0xB, 3.0f, 0, 1.0f);
        i_this->mode = 11;
        i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_DAMAGE, -1);
        break;
    case 11:
        if (i_this->modelMorf->isStop()) {
            i_this->mode = 1;
            anm_init(i_this, 0xF, 5.0f, 2, 1.0f);
        }
        break;
    case 15:
        spC = 1;
        if (i_this->field_0x860 == 0) {
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0x860 = 0x4000;
            } else {
                i_this->field_0x860 = -0x4000;
            }
        }

        i_this->ccSph.OffCoSetBit();

        cLib_addCalcAngleS2(&actor->shape_angle.x, 0x2000, 2, 0x400);
        cLib_addCalcAngleS2(&actor->shape_angle.z, i_this->field_0x860, 2, 0x800);

        if (i_this->timers[1] == 0) {
            i_this->field_0xb14 = 2;
            if (!daPy_getPlayerActorClass()->getDkCaught() && !daPy_getPlayerActorClass()->getDkCaught2() && i_this->atSph.ChkAtHit() && !i_this->atSph.ChkAtShieldHit()) {
                OS_REPORT("E_DB//////////////AT  SET 1 %d !!\n", daPy_getPlayerActorClass()->getDkCaught2());
                i_this->mode = 20;
                i_this->field_0x68c = 15.0f;
                i_this->timers[0] = 120;

                daPy_getPlayerActorClass()->onDkCaught2();
                OS_REPORT("E_DB//////////////AT  SET 2 %d !!\n", daPy_getPlayerActorClass()->getDkCaught2());

                i_this->field_0x85e = actor->shape_angle.y - player->shape_angle.y;
                anm_init(i_this, 7, 2.0f, 2, 1.0f);
                break;
            }
        }
        
        if (i_this->acch.ChkGroundHit()) {
            i_this->mode = 1;
            i_this->timers[1] = 30;
            anm_init(i_this, 0xF, 10.0f, 2, 1.0f);
            actor->speedF = 7.5f;
            fopAcM_effSmokeSet1(&i_this->field_0x1258, &i_this->field_0x125c, &actor->current.pos, &actor->shape_angle, 1.0f + NREG_F(18), &actor->tevStr, 1);
        }
        break;
    case 20:
        i_this->field_0x851 = 1;
        i_this->sound.startCreatureVoiceLevel(Z2SE_EN_DB_V_BITING, -1);
        spD = 0;
        daPy_py_c::setLookPos(&actor->current.pos);
        spC = 0;

        i_this->ccSph.OffCoSetBit();
        i_this->acch.CrrPos(dComIfG_Bgsp());

        if (!i_this->acch.ChkWallHit() && daPy_getPlayerActorClass()->getDkCaught2()) {
            if (i_this->timers[0] == 30) {
                dComIfGp_setItemLifeCount(-1.0f, 0);
            }

            if ((i_this->counter & 15) == 0) {
                dComIfGp_getVibration().StartShock(3, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            }

            s16 sp10 = player->shape_angle.y + i_this->field_0x85e;
            cMtx_YrotS(*calc_mtx, sp10);
            sp60.x = 0.0f;
            sp60.y = (100.0f + XREG_F(2)) - 30.0f;
            sp60.z = (XREG_F(3) - 50.0f) - 20.0f;
            MtxPosition(&sp60, &i_this->field_0x674);
            i_this->field_0x674 += player->current.pos;

            cLib_addCalcAngleS2(&actor->shape_angle.y, sp10, 2, 0x2000);
            cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x, 1.0f, i_this->field_0x68c);
            cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z, 1.0f, i_this->field_0x68c);
            cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y, 1.0f, i_this->field_0x68c);
            cLib_addCalc2(&i_this->field_0x68c, 200.0f, 1.0f, 10.0f);

            cLib_addCalcAngleS2(&actor->shape_angle.z, i_this->field_0x860 + (2000.0f + KREG_F(17)) * cM_ssin(i_this->counter * (KREG_S(0) + 12000)), 2, 0x2000);
            cLib_addCalcAngleS2(&actor->shape_angle.x, 0, 2, 0x800);
        } else {
            i_this->mode = 21;
            i_this->timers[1] = 30;
            anm_init(i_this, 0x10, 2.0f, 0, 1.0f);
            i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_RELEASE, -1);

            if (i_this->acch.ChkWallHit()) {
                actor->speedF = 0.0f;
            } else {
                actor->speedF = -10.0f;
            }

            actor->speed.y = 15.0f;
            daPy_getPlayerActorClass()->offDkCaught2();
            spC = 1;
        }
        break;
    case 21:
        if (i_this->modelMorf->isStop()) {
            anm_init(i_this, 0xF, 10.0f, 2, 1.0f);
            i_this->mode = 1;
        }
        break;
    case 50:
        spB = 0;
        spC = 0;
        spD = 0;

        cLib_addCalc2(&actor->current.pos.y, 20.0f + (temp_f30 + TREG_F(7)), 0.1f, 5.0f);
        cLib_addCalc0(&actor->speed.y, 0.1f, 0.25f);

        actor->current.pos.y += actor->speed.y;

        cLib_addCalcAngleS2(&actor->shape_angle.z, i_this->field_0x860, 2, 0x800);
        cLib_addCalcAngleS2(&i_this->field_0x84e, 0, 1, 150);
        cLib_addCalcAngleS2(&actor->shape_angle.x, (600.0f + KREG_F(17)) * cM_ssin(i_this->counter * (KREG_S(0) + 1000)), 2, 0x800);

        if (i_this->timers[0] == 0) {
            fopAcM_createDisappear(actor, &actor->current.pos, 9, 0, 8);
            fopAcM_delete(actor);
            fopAcM_onActor(actor);
            
            u8 swbit = (fopAcM_GetParam(actor) & 0xFF000000) >> 0x18;
            if (swbit != 0xFF) {
                dComIfGs_onSwitch(swbit, fopAcM_GetRoomNo(actor));
            }
        }
        break;
    }

    if (spC != 0) {
        s16 spE;
        if (spC == 2) {
            cLib_addCalcAngleS2(&actor->shape_angle.x, 0, 2, 0x800);
            cLib_addCalcAngleS2(&actor->shape_angle.z, 0, 2, 0x800);

            if (i_this->timers[1] < 10) {
                cLib_addCalc2(&actor->speedF, 7.5f, 1.0f, 1.0f);
            }

            spE = (5000.0f + TREG_F(13)) * cM_ssin((65536.0f * i_this->modelMorf->getFrame()) / 11.0f);
        } else {
            spE = 0;
        }

        cMtx_YrotS(*calc_mtx, spE + actor->shape_angle.y);
        sp60.x = 0.0f;
        sp60.y = 0.0f;
        sp60.z = actor->speedF;
        MtxPosition(&sp60, &sp54);
        actor->speed.x = sp54.x;
        actor->speed.z = sp54.z;
        actor->current.pos += actor->speed;
        actor->speed.y -= 3.0f;

        if (actor->speed.y < -80.0f) {
            actor->speed.y = -80.0f;
        }

        if (i_this->field_0x858 > 0.1f) {
            sp60.x = 0.0f;
            sp60.y = 0.0f;
            sp60.z = -i_this->field_0x858;
            cMtx_YrotS(*calc_mtx, i_this->field_0x85c);
            MtxPosition(&sp60, &sp54);
            actor->current.pos += sp54;
        }

        cXyz* cc_move_p = i_this->ccStts.GetCCMoveP();
        if (cc_move_p != NULL) {
            actor->current.pos.x += 0.3f * cc_move_p->x;
            actor->current.pos.z += 0.3f * cc_move_p->z;
        }

        f32 temp_f31 = ((40.0f + TREG_F(15)) * i_this->size) * l_HIO.base_size;
        actor->current.pos.y -= temp_f31;
        actor->old.pos.y -= temp_f31;
        i_this->acch.CrrPos(dComIfG_Bgsp());
        actor->current.pos.y += temp_f31;
        actor->old.pos.y += temp_f31;
    }

    if (spB != 0 && (actor->current.pos.y - 35.0f) + TREG_F(8) < temp_f30) {
        i_this->mode = 50;
        anm_init(i_this, 0xE, 20.0f, 0, 1.0f);
        i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_DEATH, -1);
        i_this->field_0x852 = 1;
        daPy_getPlayerActorClass()->onEnemyDead();

        i_this->invulnerabilityTimer = 200;
        i_this->timers[0] = 80;

        if (cM_rndF(1.0f) < 0.5f) {
            i_this->field_0x860 = 0x4000;
        } else {
            i_this->field_0x860 = -0x4000;
        }

        if (actor->speed.y < -20.0f) {
            actor->speed.y = -7.0f;
        } else {
            actor->speed.y = 0.0f;
        }

        fopKyM_createWpillar(&actor->current.pos, 1.8f, 0);
        i_this->sound.startCreatureSound(Z2SE_CM_BODYFALL_WATER_S, 0, -1);
    }

    if (actor->home.pos.y - actor->current.pos.y > 2000.0f) {
        fopAcM_delete(actor);
        OS_REPORT("E_DB///NARAKU DELETE \n");

        u8 swbit = (fopAcM_GetParam(actor) & 0xFF000000) >> 0x18;
        if (swbit != 0xFF) {
            dComIfGs_onSwitch(swbit, fopAcM_GetRoomNo(actor));
        }
    }

    return spD;
}

static void e_db_e_dead(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp24;
    cXyz sp18;
    i_this->invulnerabilityTimer = 6;
    dBgS_ObjGndChk_Spl sp30;

    sp24 = actor->current.pos;
    sp24.y += 100.0f;
    sp30.SetPos(&sp24);
    f32 temp_f30 = dComIfG_Bgsp().GroundCross(&sp30);

    switch (i_this->mode) {
    case 0:
        i_this->field_0x840 = 25.0f + TREG_F(8);
        cMtx_YrotS(*calc_mtx, i_this->field_0x680);
        sp24.x = 0.0f;
        sp24.y = 30.0f;
        sp24.z = -40.0f;
        MtxPosition(&sp24, &actor->speed);

        anm_init(i_this, 0xC, 3.0f, 2, 1.0f);
        i_this->mode = 1;
        i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_DEATH, -1);
        i_this->field_0x852 = 1;
        daPy_getPlayerActorClass()->onEnemyDead();
        break;
    case 1:
        if (i_this->acch.ChkWallHit()) {
            actor->speed.x *= -0.3f;
            actor->speed.z *= -0.3f;
            i_this->mode = 2;
        }
    case 2:
        if (i_this->acch.ChkGroundHit()) {
            actor->speed.y = 20.0f;
            i_this->mode = 3;
            i_this->timers[0] = 50;
            actor->speed.x *= 0.2f;
            actor->speed.z *= 0.2f;

            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0x860 = 0x4000;
            } else {
                i_this->field_0x860 = -0x4000;
            }

            fopAcM_effSmokeSet1(&i_this->field_0x1258, &i_this->field_0x125c, &actor->current.pos, &actor->shape_angle, 1.0f + NREG_F(18), &actor->tevStr, 1);
        }
        break;
    case 3:
    case 4:
        if (i_this->acch.ChkGroundHit()) {
            actor->speed.z = 0.0f;
            actor->speed.x = 0.0f;

            if (i_this->mode == 3) {
                anm_init(i_this, 0xE, 10.0f, 0, 1.0f);
                i_this->mode = 4;
            }
        }

        cLib_addCalcAngleS2(&actor->shape_angle.z, i_this->field_0x860, 4, 0x400);
        cLib_addCalcAngleS2(&i_this->field_0x84e, 0, 1, 150);

        if (i_this->timers[0] == 0) {
            fopAcM_createDisappear(actor, &actor->current.pos, 9, 0, 8);

            if (i_this->arg0 == 1) {
                actor->current.pos = actor->home.pos;
                actor->old = actor->current;
                i_this->field_0x674 = actor->home.pos;
                i_this->size = 0.0f;
                i_this->action = ACTION_MK_ROOF;
                i_this->mode = 0;
                i_this->timers[0] = l_HIO.roof_reappear_interval;
                actor->health = 30;
                i_this->field_0x852 = 0;
                return;
            }

            fopAcM_delete(actor);
            fopAcM_onActor(actor);

            u8 swbit = (fopAcM_GetParam(actor) & 0xFF000000) >> 0x18;
            if (swbit != 0xFF) {
                dComIfGs_onSwitch(swbit, fopAcM_GetRoomNo(actor));
            }
        }
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, (i_this->field_0x680 + 0x8000), 2, 0x1000);
    cLib_addCalcAngleS2(&actor->shape_angle.x, 0, 2, 0x800);

    actor->current.pos += actor->speed;
    actor->speed.y -= 3.0f;

    f32 temp_f29 = 15.0f * cM_scos(actor->shape_angle.z);
    f32 temp_f31 = ((25.0f + temp_f29 + TREG_F(15)) * i_this->size) * l_HIO.base_size;

    actor->current.pos.y -= temp_f31;
    actor->old.pos.y -= temp_f31;
    i_this->acch.CrrPos(dComIfG_Bgsp());
    actor->current.pos.y += temp_f31;
    actor->old.pos.y += temp_f31;

    if (((actor->current.pos.y - 35.0f) + TREG_F(8)) < temp_f30) {
        i_this->action = ACTION_ESCAPE;
        i_this->mode = 50;
        anm_init(i_this, 0xE, 20.0f, 0, 1.0f);
        i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_DEATH, -1);
        i_this->field_0x852 = 1;
        daPy_getPlayerActorClass()->onEnemyDead();

        i_this->invulnerabilityTimer = 200;
        i_this->timers[0] = 80;

        if (cM_rndF(1.0f) < 0.5f) {
            i_this->field_0x860 = 0x4000;
        } else {
            i_this->field_0x860 = -0x4000;
        }

        if (actor->speed.y < -20.0f) {
            actor->speed.y = -7.0f;
        } else {
            actor->speed.y = 0.0f;
        }

        fopKyM_createWpillar(&actor->current.pos, 1.8f, 0);
        i_this->sound.startCreatureSound(Z2SE_CM_BODYFALL_WATER_S, 0, -1);
    }

    if ((actor->home.pos.y - actor->current.pos.y) > 2000.0f) {
        fopAcM_delete(actor);
        OS_REPORT("E_DB///NARAKU DELETE \n");

        u8 swbit = (fopAcM_GetParam(actor) & 0xFF000000) >> 0x18;
        if (swbit != 0xFF) {
            dComIfGs_onSwitch(swbit, fopAcM_GetRoomNo(actor));
        }
    }
}

static void action(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp70;
    cXyz sp64;

    i_this->angle_to_player = fopAcM_searchPlayerAngleY(actor);
    i_this->dist_to_player = (actor->home.pos - player->current.pos).abs();
    actor->field_0x566 = 0;
    i_this->field_0x1239 = 1;

    damage_check(i_this);

    if (i_this->invulnerabilityTimer != 0 && i_this->field_0x851 != 0) {
        daPy_getPlayerActorClass()->offDkCaught();
        daPy_getPlayerActorClass()->offDkCaught2();
    }

    s8 attn_ON = FALSE;
    s8 spA = FALSE;
    s8 kuki_sph_ON = FALSE;
    s8 sound_pl_search = FALSE;

    i_this->ccSph.OnCoSetBit();
    i_this->field_0x851 = 0;

    switch (i_this->action) {
    case ACTION_STAY:
        e_db_stay(i_this);
        i_this->field_0x1239 = 0;
        break;
    case ACTION_APPEAR:
        e_db_appear(i_this);
        attn_ON = TRUE;
        kuki_sph_ON = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        break;
    case ACTION_APPEAR_V:
        e_db_appear_v(i_this);
        attn_ON = TRUE;
        kuki_sph_ON = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        break;
    case ACTION_WAIT:
        e_db_wait(i_this);
        attn_ON = TRUE;
        kuki_sph_ON = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        break;
    case ACTION_ATTACK:
        e_db_attack(i_this);
        attn_ON = TRUE;
        kuki_sph_ON = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        break;
    case ACTION_ATTACK_S:
        e_db_attack_s(i_this);
        attn_ON = TRUE;
        kuki_sph_ON = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        break;
    case ACTION_CHANCE:
        e_db_chance(i_this);
        attn_ON = TRUE;
        kuki_sph_ON = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        break;
    case ACTION_S_DAMAGE:
        e_db_s_damage(i_this);
        attn_ON = TRUE;
        kuki_sph_ON = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        break;
    case ACTION_DAMAGE:
        e_db_damage(i_this);
        attn_ON = TRUE;
        break;
    case ACTION_MK_ROOF:
        e_db_mk_roof(i_this);
        kuki_sph_ON = TRUE;
        sound_pl_search = TRUE;
        break;
    case ACTION_ESCAPE:
        attn_ON = e_db_escape(i_this);
        spA = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        fopAcM_OffStatus(actor, 0x10000);
        break;
    case ACTION_E_DEAD:
        e_db_e_dead(i_this);
        spA = TRUE;
        break;
    }

    if (sound_pl_search) {
        i_this->sound.setLinkSearch(true);
    } else {
        i_this->sound.setLinkSearch(false);
    }

    if (attn_ON) {
        fopAcM_OnStatus(actor, 0);
        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    }

    if (!spA) {
        sp70 = actor->current.pos - actor->home.pos;
        i_this->field_0x840 = i_this->field_0x844 * ((0.2f + TREG_F(1)) * sp70.abs());
        if (i_this->field_0x840 > 25.0f + TREG_F(7)) {
            i_this->field_0x840 = 25.0f + TREG_F(7);
        }

        cLib_addCalc2(&i_this->field_0x844, 1.0f, 1.0f, 0.01f);
        kuki_control1(i_this);
        kuki_control2(i_this);
        kuki_control3(i_this);
        kuki_ha_set(i_this);

        e_db_leaf_class* pleaf = (e_db_leaf_class*)fopAcM_SearchByID(i_this->leaf_actor_id);
        if (pleaf != NULL) {
            pleaf->mpMorf->play(NULL, 0, 0);
        }

        cLib_addCalc2(&i_this->field_0x848, 1.0f, 1.0f, 0.05f);
    } else {
        kuki_control1_e(i_this);
        kuki_ha_set(i_this);
    }

    cXyz* sp1C = i_this->field_0x69c;
    cXyz* pkukipos = i_this->stalkLine.getPos(0);
    cXyz hide_offset(-30000.0f, 300000.0f, 30000.0f);
    int kuki_no = 0;

    for (int i = 0; i < 12; i++, pkukipos++, sp1C++) {
        *pkukipos = *sp1C;
        if (i == 3 || i == 5 || i == 7 || i == 9) {
            if (kuki_sph_ON) {
                i_this->kukiSph[kuki_no].SetC(*sp1C);
                i_this->kukiSph[kuki_no].SetR(30.0f);
            } else {
                i_this->kukiSph[kuki_no].SetC(*sp1C + hide_offset);
            }

            dComIfG_Ccsp()->Set(&i_this->kukiSph[kuki_no]);
            kuki_no++;
        }
    }

    cLib_addCalc0(&i_this->field_0x858, 1.0f, 5.0f + TREG_F(12));

    if (actor->speed.y < -100.0f) {
        actor->speed.y = -100.0f;
    }

    cXyz effsize(1.0f, 1.0f, 1.0f);
    setMidnaBindEffect((fopEn_enemy_c*)actor, &i_this->sound, &actor->eyePos, &effsize);
}

static void eff_set(e_db_class* i_this, int param_1) {
    fopAc_ac_c* actor = &i_this->enemy;
    J3DModel* model = i_this->modelMorf->getModel();

    if (i_this->field_0x1238 != 0) {
        i_this->field_0x1238--;
        i_this->field_0x1239 = 3;
    }

    f32 var_f31;
    if (i_this->field_0x1239 != 1) {
        var_f31 = 0.0f;
    } else {
        var_f31 = 1.0f;
    }
    cLib_addCalc2(&i_this->field_0x1254, var_f31, 1.0f, 0.05f);

    for (int i = 0; i <= 2; i++) {
        static u16 p_name[] = {0x81C4, 0x81C5, 0x81C6};
        static int p_idx[] = {2, 2, 6};

        if (param_1 == 0) {
            i_this->field_0x123c[i] = dComIfGp_particle_set(i_this->field_0x123c[i], p_name[i], &actor->current.pos, NULL, NULL);
        }

        JPABaseEmitter* pemitter = dComIfGp_particle_getEmitter(i_this->field_0x123c[i]);
        if (pemitter != NULL) {
            if (param_1 != 0) {
                pemitter->deleteAllParticle();
                dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0x123c[i]);
            } else {
                MTXCopy(model->getAnmMtx(p_idx[i]), *calc_mtx);
                MtxScale(i_this->field_0x1254, i_this->field_0x1254, i_this->field_0x1254, 1);
                pemitter->setGlobalSRTMatrix(*calc_mtx);
            }
        }
    }

    if (i_this->field_0x1239 != 0) {
        if (i_this->field_0x1239 == 1) {
            cXyz sp8(actor->current.pos);
            sp8.y += 100.0f;

            if (fopAcM_gc_c::gndCheck(&sp8)) {
                sp8.y = fopAcM_gc_c::getGroundY();
            }

            i_this->field_0x124c = dComIfGp_particle_set(i_this->field_0x124c, 0x81DD, &sp8, NULL, NULL);
        } else if (i_this->field_0x1239 == 2) {
            i_this->field_0x1248 = dComIfGp_particle_set(i_this->field_0x1248, 0x81C2, &actor->current.pos, NULL, NULL);
            
            JPABaseEmitter* pemitter = dComIfGp_particle_getEmitter(i_this->field_0x1248);
            if (pemitter != NULL) {
                MTXCopy(model->getAnmMtx(1), *calc_mtx);
                pemitter->setGlobalRTMatrix(*calc_mtx);
            }
        } else if (i_this->field_0x1239 == 3) {
            i_this->field_0x1250 = dComIfGp_particle_set(i_this->field_0x1250, 0x81C3, &actor->current.pos, NULL, NULL);
            
            JPABaseEmitter* pemitter = dComIfGp_particle_getEmitter(i_this->field_0x1250);
            if (pemitter != NULL) {
                MTXCopy(model->getAnmMtx(1), *calc_mtx);
                pemitter->setGlobalRTMatrix(*calc_mtx);
            }
        }

        i_this->field_0x1239 = 0;
    }
}

static void anm_se_set(e_db_class* i_this) {
    if (i_this->anm == 5) {
        if (i_this->modelMorf->checkFrame(22.0f)) {
            i_this->sound.startCreatureSound(Z2SE_EN_DB_BERON, 0, -1);
        }
    } else if (i_this->anm == 6) {
        if (i_this->modelMorf->checkFrame(21.0f)) {
            i_this->sound.startCreatureSound(Z2SE_EN_DB_ATTACK, 0, -1);
        }
    } else if (i_this->anm == 17) {
        if (i_this->modelMorf->checkFrame(15.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_AWAKE, -1);
        }
    } else if (i_this->anm == 15) {
        if (i_this->modelMorf->checkFrame(3.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_CREEP, -1);
        }
    }
}

static int daE_DB_Execute(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp4C;
    cXyz sp40;

    if (actor->health == 1000) {
        cXyz effsize(1.0f, 1.0f, 1.0f);
        u16 effname = 0x109;
        cXyz effpos(actor->current.pos);

        dComIfGp_particle_set(effname, &effpos, NULL, &effsize);
        fopAcM_delete(actor);
        return 1;
    }

    i_this->counter++;

    for (int i = 0; i < 3; i++) {
        if (i_this->timers[i] != 0) {
            i_this->timers[i]--;
        }
    }

    if (i_this->invulnerabilityTimer != 0) {
        i_this->invulnerabilityTimer--;
    }

    action(i_this);
    anm_se_set(i_this);

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::XrotM(actor->shape_angle.x);
    mDoMtx_stack_c::ZrotM(actor->shape_angle.z);

    if (i_this->action != ACTION_E_DEAD) {
        cLib_addCalcAngleS2(&actor->shape_angle.z, 0, 8, 0x400);
    }

    f32 size = l_HIO.base_size * i_this->size;
    mDoMtx_stack_c::scaleM(size, size, size);

    J3DModel* model = i_this->modelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->modelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    i_this->modelMorf->modelCalc();

    cXyz hide_offset(-30000.0f, 10000.0f, -20000.0f);

    MTXCopy(model->getAnmMtx(1), *calc_mtx);
    sp4C.set(30.0f + JREG_F(0), KREG_F(1), KREG_F(2));

    if (i_this->field_0x850 != 0) {
        actor->eyePos = i_this->field_0x69c[YREG_S(3) + 6];
        actor->attention_info.position = actor->eyePos;
        i_this->ccSph.SetTgType(0xD8FBFDFF);
    } else {
        MtxPosition(&sp4C, &actor->eyePos);
        actor->attention_info.position = actor->eyePos;
        actor->attention_info.position.y += 60.0f;
        i_this->ccSph.SetTgType(0xD8FBFDFF);
    }

    MtxPosition(&sp4C, &sp40);
    if (i_this->action == ACTION_STAY || i_this->invulnerabilityTimer != 0) {
        sp40 += hide_offset;
    }

    i_this->ccSph.SetC(sp40);
    i_this->ccSph.SetR(((40.0f + JREG_F(2)) * l_HIO.base_size) * i_this->size);
    dComIfG_Ccsp()->Set(&i_this->ccSph);

    if (i_this->field_0xb14 != 0) {
        i_this->atSph.MoveCAt(sp40);
        i_this->field_0xb14 = 0;
    } else {
        i_this->atSph.SetC(sp40 + hide_offset);
    }

    i_this->atSph.SetR(((30.0f + JREG_F(3)) * l_HIO.base_size) * i_this->size);
    dComIfG_Ccsp()->Set(&i_this->atSph);
    eff_set(i_this, 0);

    if (i_this->field_0x852 != 0) {
        cLib_addCalc2(&i_this->field_0x854, -20.0f, 1.0f, 0.4f);
    }

    return 1;
}

static int daE_DB_IsDelete(e_db_class* i_this) {
    return 1;
}

static int daE_DB_Delete(e_db_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fpc_ProcID id = fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->phase, "E_db");

    if (i_this->HIOInit) {
        l_HIOInit = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (actor->heap != NULL) {
        i_this->modelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_db_class* a_this = (e_db_class*)i_this;

    a_this->modelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_db", 0x17), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_db", 0x12), 2, 1.0f, 0, -1, &a_this->sound, 0x80000, 0x11000084);
    if (a_this->modelMorf == NULL || a_this->modelMorf->getModel() == NULL) {
        return 0;
    }

    if (!a_this->stalkLine.init(1, 12, (ResTIMG*)dComIfG_getObjectRes("E_db", 0x1C), 1)) {
        return 0;
    }

    f32* psize = a_this->stalkLine.getSize(0);
    for (int i = 0; i < 12; i++, psize++) {
        *psize = 5.0f + (0.1f * XREG_S(0));
    }

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_db", 0x19);
    JUT_ASSERT(3622, modelData != NULL);

    for (int i = 1; i < 11; i++) {
        if (i & 1) {
            a_this->thornModel[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            if (a_this->thornModel[i] == NULL) {
                return 0;
            }

            a_this->thorn_size[i] = 1.0f + cM_rndF(1.0f);
        }
    }

    return 1;
}

static int daE_DB_Create(fopAc_ac_c* i_this) {
    e_db_class* a_this = (e_db_class*)i_this;
    fopAcM_ct(i_this, e_db_class);

    int phase_state = dComIfG_resLoad(&a_this->phase, "E_db");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_DB PARAM %x\n", fopAcM_GetParam(i_this));
        
        u8 swbit = ((fopAcM_GetParam(i_this) & 0xFF000000) >> 0x18);
        if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(i_this))) {
            OS_REPORT("E_DB やられ後なので再セットしません\n");
            return cPhs_ERROR_e;
        }

        a_this->arg0 = fopAcM_GetParam(i_this);
        a_this->arg1 = (fopAcM_GetParam(i_this) & 0xFF00) >> 8;
        a_this->arg2 = (fopAcM_GetParam(i_this) & 0xF0000) >> 16;
        a_this->arg3 = (fopAcM_GetParam(i_this) & 0xF00000) >> 20;

        if (a_this->arg0 == 0xFF) {
            a_this->arg0 = 0;
        }

        if (a_this->arg1 == 0xFF || a_this->arg1 == 0) {
            a_this->pl_search_range = 500.0f;
        } else {
            a_this->pl_search_range = 100.0f * a_this->arg1;
        }

        OS_REPORT("E_DB//////////////E_DB SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x4060)) {
            OS_REPORT("//////////////E_DB SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_DB SET 2 !!\n");

        if (!l_HIOInit) {
            a_this->HIOInit = TRUE;
            l_HIOInit = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("デグババ", &l_HIO);
        }

        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        fopAcM_SetMtx(i_this, a_this->modelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_this, -300.0f, -400.0f, -300.0f);
        fopAcM_SetMax(i_this, 300.0f, 400.0f, 300.0f);

        i_this->health = 30;
        i_this->field_0x560 = 30;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x25}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
            } // mSphAttr
        };

        static dCcD_SrcSph kuki_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x0}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 20.0f} // mSph
            } // mSphAttr
        };
    
        a_this->ccStts.Init(100, 0, i_this);

        a_this->ccSph.Set(cc_sph_src);
        a_this->ccSph.SetStts(&a_this->ccStts);

        a_this->atSph.Set(at_sph_src);
        a_this->atSph.SetStts(&a_this->ccStts);

        for (int i = 0; i < 4; i++) {
            a_this->kukiSph[i].Set(kuki_sph_src);
            a_this->kukiSph[i].SetStts(&a_this->ccStts);
        }

        a_this->acch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->acchcir, fopAcM_GetSpeed_p(i_this), NULL, NULL);

        a_this->acchcir.SetWall(30.0f, 70.0f);
        a_this->sound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        a_this->sound.setEnemyName("E_db");

        a_this->atInfo.mpSound = &a_this->sound;
        a_this->atInfo.mPowerType = 1;
        a_this->atInfo.field_0x18 = 45;
        a_this->counter = cM_rndF(65535.0f);
        a_this->size = l_HIO.flower_size;

        if (a_this->arg0 == 1) {
            a_this->action = ACTION_MK_ROOF;
            a_this->mode = 0;
            i_this->current.angle.x = -0x8000;
            i_this->shape_angle.x = -0x8000;
            a_this->field_0x850 = 1;
        } else if (a_this->arg0 == 2) {
            i_this->current.angle.x = -0x8000;
            i_this->shape_angle.x = -0x8000;
            a_this->field_0x850 = 1;
        }

        a_this->leaf_actor_id = fopAcM_createChild(PROC_E_DB_LEAF, fopAcM_GetID(i_this), 0, &i_this->current.pos, fopAcM_GetRoomNo(i_this), &i_this->shape_angle, NULL, -1, NULL);
        daE_DB_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_DB_Method = {
    (process_method_func)daE_DB_Create,
    (process_method_func)daE_DB_Delete,
    (process_method_func)daE_DB_Execute,
    (process_method_func)daE_DB_IsDelete,
    (process_method_func)daE_DB_Draw,
};

extern actor_process_profile_definition g_profile_E_DB = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_E_DB,              // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(e_db_class),     // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    136,                    // mPriority
    &l_daE_DB_Method,       // sub_method
    0x10050120,             // mStatus
    fopAc_ENEMY_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
