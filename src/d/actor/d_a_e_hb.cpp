/**
 * @file d_a_e_hb.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_hb.h"
#include "d/actor/d_a_e_hb_leaf.h"
#include "f_op/f_op_actor_enemy.h"

enum daE_HB_ACTION {
    ACTION_STAY,
    ACTION_APPEAR,
    ACTION_APPEAR_V,
    ACTION_WAIT,
    ACTION_ATTACK,

    ACTION_CHANCE = 6,
    ACTION_S_DAMAGE,
    ACTION_DAMAGE
};

class daE_HB_HIO_c : public JORReflexible {
public:
#if DEBUG
    virtual void genMessage(JORMContext*);
#endif

    daE_HB_HIO_c();
    virtual ~daE_HB_HIO_c() {}

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 base_size;
    /* 0x0C */ f32 flower_size;
    /* 0x10 */ f32 appear_size;
    /* 0x14 */ s16 roof_reappear_interval;
    /* 0x16 */ s16 attack_freq;
    /* 0x18 */ u8 bite_pattern;
};

daE_HB_HIO_c::daE_HB_HIO_c() {
    id = -1;
    base_size = 1.0f;
    flower_size = 0.5f;
    appear_size = 1.0f;
    roof_reappear_interval = 70;
    attack_freq = 30;
    bite_pattern = 1;
}

static void anm_init(e_hb_class* i_this, int i_anm, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->modelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_hb", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
    i_this->anm = i_anm;
}

static BOOL leaf_anm_init(e_hb_class* i_this, int i_anm, f32 i_morf, u8 i_mode, f32 i_speed) {
    e_hb_leaf_class* pleaf = (e_hb_leaf_class*)fopAcM_SearchByID(i_this->leaf_actor_id);
    if (pleaf != NULL) {
        pleaf->mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_hb", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f, NULL);
        return TRUE;
    }

    return FALSE;
}

static int daE_HB_Draw(e_hb_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);

    if (i_this->field_0x850 == 0) {
        J3DModel* model = i_this->modelMorf->getModel();
        g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);
        i_this->modelMorf->entryDL();

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
            mDoExt_modelUpdateDL(i_this->thornModel[i]);
        }
    }

    return 1;
}

static BOOL pl_check(e_hb_class* i_this, f32 i_range) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (i_this->dist_to_player < i_range && (i_this->field_0x851 || !fopAcM_otherBgCheck(actor, player))) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void damage_check(e_hb_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    
    if (i_this->invulnerabilityTimer == 0) {
        i_this->ccStts.Move();

        if (i_this->atSph.ChkAtShieldHit()) {
            i_this->invulnerabilityTimer = 6;
            i_this->action = ACTION_S_DAMAGE;
            i_this->mode = 0;

            i_this->timers[0] = 10;
            i_this->timers[1] = 100;

            i_this->field_0x680 = player->shape_angle.y;
            i_this->field_0x1230 = 10;
            dComIfGp_getVibration().StartShock(4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            return;
        }

        s8 hit_type = 0;

        for (int i = 0; i < 4; i++) {
            if (i_this->kukiSph[i].ChkTgHit()) {
                i_this->invulnerabilityTimer = 10;
                i_this->atInfo.mpCollider = i_this->kukiSph[i].GetTgHitObj();

                if (i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                    hit_type = 2;
                } else if (i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_40 | AT_TYPE_BOMB) || i_this->action == ACTION_CHANCE) {
                    at_power_check(&i_this->atInfo);
                    if (daPy_getPlayerActorClass()->getCutType() != 0) {
                        hit_type = 1;
                    } else {
                        hit_type = 2;
                    }
                } else {
                    i_this->action = ACTION_S_DAMAGE;
                    i_this->mode = 0;
                    i_this->timers[0] = 10;
                    i_this->timers[1] = 100;
                    i_this->field_0x680 = player->shape_angle.y;
                    def_se_set(&i_this->sound, i_this->atInfo.mpCollider, 0x2D, NULL);
                    i_this->field_0x1230 = 10;
                }
                break;
            }
        }

        if (hit_type == 0 && i_this->ccSph.ChkTgHit()) {
            i_this->atInfo.mpCollider = i_this->ccSph.GetTgHitObj();
            fopAc_ac_c* tg_hit_actor = dCc_GetAc(i_this->atInfo.mpCollider->GetAc());

            if (i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG | AT_TYPE_40 | AT_TYPE_BOMB)) {
                hit_type = 2;
            }

            if (i_this->field_0x850 == 0) {
                i_this->invulnerabilityTimer = 6;
                i_this->action = ACTION_S_DAMAGE;
                i_this->mode = 0;
                i_this->timers[0] = 10;
                i_this->timers[1] = 100;
                i_this->field_0x680 = player->shape_angle.y;
                i_this->field_0x1230 = 10;
                def_se_set(&i_this->sound, i_this->atInfo.mpCollider, 0x2D, NULL);
            }
        }

        if (hit_type != 0) {
            int item_no = actor->field_0x565;
            if (item_no == 0xFF) {
                item_no = -1;
            }

            int item_bit = actor->field_0x564;
            if (item_bit == 0xFF) {
                item_bit = -1;
            }

            cXyz sp2C(0.0f, 0.0f, 0.0f);
            fopAc_ac_c* hit_actor = dCc_GetAc(i_this->atInfo.mpCollider->GetAc());
            if (hit_actor != NULL) {
                sp2C = actor->current.pos - hit_actor->current.pos;
            }

            e_hb_leaf_class* pleaf = (e_hb_leaf_class*)fopAcM_SearchByID(i_this->leaf_actor_id);
            if (pleaf != NULL) {
                pleaf->field_0x5bc = fopAcM_createBokkuri(fopAcM_GetSetId(actor), &actor->current.pos, item_no, item_bit, fopAcM_GetRoomNo(actor), &sp2C, 0, 0);
                daPy_getPlayerActorClass()->onEnemyDead();
            }

            i_this->field_0x124c = 0.0f;
            dComIfGp_particle_set(0x8335, &i_this->field_0x6a0[4], NULL, NULL);
            i_this->field_0x850 = 3;
            i_this->field_0x1230 = 10;
            actor->health = 0;

            def_se_set(&i_this->sound, i_this->atInfo.mpCollider, 0x21, NULL);
            i_this->sound.startCreatureSound(Z2SE_EN_DB_DRY, 0, -1);

            if (hit_type == 1) {
                dScnPly_c::setPauseTimer(4);
            }
        }
    }
}

static u8 l_HIOInit;

static daE_HB_HIO_c l_HIO;

static void e_hb_stay(e_hb_class* i_this) {
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
            i_this->field_0x690 = 0.0f;
            i_this->timers[0] = 50;
        }
        break;
    case 2:
        if (i_this->field_0x851 != 0) {
            i_this->field_0x674.y = -170.0f;
        } else {
            i_this->field_0x674.y = 170.0f;
        }
        i_this->mode = 1;
        break;
    case 5:
        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x, 0.2f, 10.0f * i_this->field_0x690);
        cLib_addCalc2(&actor->current.pos.y, actor->home.pos.y + i_this->field_0x674.y, 0.2f, 10.0f * i_this->field_0x690);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z, 0.2f, 10.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.1f);

        sp14 = actor->current.pos - actor->home.pos;
        sp14.y = 0.0f;
        if (sp14.abs() < 20.0f) {
            cLib_addCalc0(&i_this->field_0x674.y, 1.0f, 4.0f);

            if (i_this->field_0x851 != 0) {
                cLib_addCalcAngleS2(&actor->shape_angle.x, 0x4000, 4, 0x200);
            } else {
                cLib_addCalcAngleS2(&actor->shape_angle.x, -0x4000, 4, 0x200);
            }

            cLib_addCalc2(&i_this->field_0x848, 0.4f, 1.0f, 0.02f);
            cLib_addCalc2(&i_this->size, l_HIO.flower_size, 1.0f, 0.05f);
        }

        if (i_this->timers[0] == 0 && (i_this->arg2 == 1 || pl_check(i_this, i_this->pl_search_range))) {
            if (i_this->field_0x851 != 0) {
                i_this->action = ACTION_APPEAR_V;
            } else {
                i_this->action = ACTION_APPEAR;
            }
            i_this->mode = 0;
        }
        break;
    }
}

static void e_hb_appear(e_hb_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    int anm_frame = i_this->modelMorf->getFrame();

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 5, 10.0f, 0, 1.0f);
        leaf_anm_init(i_this, 0x13, 10.0f, 2, 1.0f);
        i_this->mode = 1;

        i_this->field_0x674.x = actor->home.pos.x + cM_rndFX(100.0f);
        i_this->field_0x674.z = actor->home.pos.z + cM_rndFX(100.0f);
        if (i_this->field_0x851 != 0) {
            i_this->field_0x674.y = actor->home.pos.y - (170.0f + cM_rndFX(50.0f));
        } else {
            i_this->field_0x674.y = 170.0f + actor->home.pos.y + cM_rndFX(50.0f);
        }

        i_this->field_0x690 = 0.2f;
        i_this->sound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);
        i_this->sound.startCreatureSound(Z2SE_EN_DB_APPEAR, 0, -1);
        break;
    case 1:
        if (anm_frame >= 15 && anm_frame <= 35) {
            i_this->field_0x1231 = 2;
        }

        i_this->field_0xb0c = 1;
        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x, 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z, 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y, 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.05f);

        if (i_this->modelMorf->isStop()) {
            i_this->action = ACTION_WAIT;
            i_this->mode = 0;
        }
        break;
    }

    if (i_this->field_0x851 != 0) {
        cLib_addCalcAngleS2(&actor->shape_angle.y, (i_this->angle_to_player + 0x8000), 8, 0x800);
        cLib_addCalcAngleS2(&actor->shape_angle.x, -0x8000, 8, 0x400);
    } else {
        cLib_addCalcAngleS2(&actor->shape_angle.y, i_this->angle_to_player, 8, 0x800);
        cLib_addCalcAngleS2(&actor->shape_angle.x, 0, 8, 0x400);
    }

    cLib_addCalc2(&i_this->size, l_HIO.appear_size, 1.0f, 0.1f);
}

static void e_hb_appear_v(e_hb_class* i_this) {
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
        i_this->field_0x690 = 0.0f;
        i_this->modelMorf->setFrame(15.0f);
        /* fallthrough */
    case 1:
        spC.x = temp_f31 * cM_ssin(i_this->counter * (TREG_S(0) + 1200));
        spC.z = temp_f31 * cM_scos(i_this->counter * (TREG_S(2) + 1500));

        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x + spC.x, 0.1f, 40.0f * i_this->field_0x690);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z + spC.z, 0.1f, 40.0f * i_this->field_0x690);
        cLib_addCalc2(&actor->current.pos.y, actor->home.pos.y - (400.0f + JREG_F(18)), 0.5f, 40.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.2f + JREG_F(5));

        if (i_this->timers[0] == 0) {
            i_this->action = ACTION_WAIT;
            i_this->mode = 0;
            i_this->field_0x84c = 0.0f;
        }
        break;
    }

    cLib_addCalcAngleS2(&actor->shape_angle.x, 0x4000, 4, 0x400);
    cLib_addCalc2(&i_this->field_0x848, 0.5f + JREG_F(4), 1.0f, 0.05f);
    cLib_addCalc2(&i_this->size, l_HIO.appear_size, 1.0f, 0.15f);
    i_this->field_0xb0c = 1;
}

static void e_hb_wait(e_hb_class* i_this) {
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

        if (i_this->arg1 == 1) {
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

            if (i_this->field_0x851 != 0) {
                i_this->field_0x674.y = actor->home.pos.y - (170.0f + cM_rndFX(50.0f));
            } else {
                i_this->field_0x674.y = 170.0f + actor->home.pos.y + cM_rndFX(50.0f);
            }

            i_this->field_0x690 = 0.0f;
        }

        sp48.x = temp_f31 * cM_ssin(i_this->counter * (TREG_S(0) + 1900));
        sp48.y = temp_f31 * cM_ssin(i_this->counter * (TREG_S(1) + 2200));
        sp48.z = temp_f31 * cM_scos(i_this->counter * (TREG_S(2) + 2000));

        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x + sp48.x, 0.1f, 20.0f * i_this->field_0x690);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z + sp48.z, 0.1f, 20.0f * i_this->field_0x690);
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y + sp48.y, 0.1f, 20.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.05f);

        if (i_this->timers[2] == 0) {
            if (i_this->arg2 != 1 && !pl_check(i_this, 100.0f + i_this->pl_search_range)) {
                i_this->action = ACTION_STAY;
                i_this->mode = 2;
                i_this->sound.startCreatureSound(Z2SE_EN_DB_HIKKOMU, 0, -1);
            } else if (i_this->timers[1] == 0 && pl_check(i_this, 700.0f)) {
                i_this->action = ACTION_ATTACK;
                i_this->mode = 0;
            }
        }
        break;
    }

    if (i_this->field_0x851 != 0) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cLib_addCalcAngleS2(&actor->shape_angle.y, (i_this->angle_to_player + 0x8000), 8, 0x800);
        
        cXyz sp24 = player->eyePos - actor->current.pos;
        int sp8 = cM_atan2s(sp24.y, JMAFastSqrt(SQUARE(sp24.x) + SQUARE(sp24.z)));
        cLib_addCalcAngleS2(&actor->shape_angle.x, (sp8 + 0x8000), 8, 0x400);
        i_this->field_0xb0c = 1;
    } else {
        cLib_addCalcAngleS2(&actor->shape_angle.y, i_this->angle_to_player, 8, 0x800);
        cLib_addCalcAngleS2(&actor->shape_angle.x, 0, 8, 0x400);
    }
}

static void e_hb_attack(e_hb_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz sp78;
    cXyz sp6C;
    s16 spA = 0;

    sp78 = player->eyePos - actor->current.pos;
    spA = (s16)cM_atan2s(sp78.y, JMAFastSqrt(SQUARE(sp78.x) + SQUARE(sp78.z)));

    switch (i_this->mode) {
    case 0:
        i_this->mode = 1;
        i_this->timers[0] = 17;
        i_this->field_0x690 = 0.0f;
        i_this->field_0xb0d = 0;

        i_this->sound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);
        i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_ATTACK, -1);

        leaf_anm_init(i_this, 0x13, 5.0f, 2, 1.0f);
        anm_init(i_this, 6, 5.0f, 0, 1.0f);
        /* fallthrough */
    case 1:
        if (i_this->field_0x851 != 0) {
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

        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x, 0.1f, 20.0f * i_this->field_0x690);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z, 0.1f, 20.0f * i_this->field_0x690);
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y, 0.1f, 20.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.1f);

        if (i_this->timers[0] == 0) {
            i_this->mode = 2;
            i_this->timers[1] = 10;

            if (i_this->field_0x851 != 0) {
                cMtx_YrotS(*calc_mtx, (actor->shape_angle.y + 0x8000));
                cMtx_XrotM(*calc_mtx, -spA);
                sp78.x = 0.0f;
                sp78.y = TREG_F(5);
                sp78.z = 350.0f + TREG_F(6);
                MtxPosition(&sp78, &sp6C);
                i_this->field_0x674 = actor->current.pos + sp6C;
            } else {
                cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
                cMtx_XrotM(*calc_mtx, (-spA + 0x800));
                sp78.x = 0.0f;
                sp78.y = 0.0f;
                sp78.z = 350.0f + TREG_F(6);
                MtxPosition(&sp78, &sp6C);
                i_this->field_0x674 = actor->current.pos + sp6C;
            }

            actor->speed.x = fabsf(i_this->field_0x674.x - actor->current.pos.x);
            actor->speed.y = fabsf(i_this->field_0x674.y - actor->current.pos.y);
            actor->speed.z = fabsf(i_this->field_0x674.z - actor->current.pos.z);
            i_this->field_0x690 = 0.0f;

            i_this->atSph.StartCAt(actor->current.pos);

            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0x68c = 0x4000;
            } else {
                i_this->field_0x68c = -0x4000;
            }

            i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_BITE, -1);
        }
        break;
    case 2:
        if (i_this->timers[1] != 0) {
            i_this->field_0xb0c = 1;
        }

        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x, 0.8f, actor->speed.x * i_this->field_0x690);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z, 0.8f, actor->speed.z * i_this->field_0x690);

        if (l_HIO.bite_pattern == 1) {
            cLib_addCalcAngleS2(&actor->shape_angle.z, i_this->field_0x68c, 2, 0x2000);
            cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y - 30.0f, 0.8f, actor->speed.y * i_this->field_0x690);
        } else {
            cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y, 0.8f, actor->speed.y * i_this->field_0x690);
        }

        cLib_addCalc2(&i_this->field_0x690, 0.2f, 1.0f, 0.05f);

        if (!daPy_getPlayerActorClass()->getDkCaught() && !daPy_getPlayerActorClass()->getStCaught() && i_this->atSph.ChkAtHit()) {
            OS_REPORT("E_HB//////////////AT  SET 1 !!\n");
            i_this->mode = 5;
            i_this->field_0x690 = 20.0f;
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
        i_this->field_0x852 = 1;
        i_this->sound.startCreatureVoiceLevel(Z2SE_EN_DB_V_BITING, -1);
        i_this->ccSph.OffCoSetBit();
        spA = 0;

        if (daPy_getPlayerActorClass()->getDkCaught()) {
            if (i_this->timers[0] == 30) {
                dComIfGp_setItemLifeCount(-1.0f, 0);
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

            if (i_this->field_0x851 == 0) {
                cMtx_YrotS(*calc_mtx, sp8);
                cLib_addCalcAngleS2(&actor->shape_angle.z, i_this->field_0x68c + ((2000.0f + KREG_F(17)) * cM_ssin((i_this->counter * (KREG_S(0) + 12000)))), 2, 0x2000);
                sp78.x = 0.0f;
                sp78.y = (100.0f + XREG_F(2)) - 30.0f;
                sp78.z = XREG_F(3) - 50.0f;
                MtxPosition(&sp78, &i_this->field_0x674);
                i_this->field_0x674 += player->current.pos;
                cLib_addCalcAngleS2(&actor->shape_angle.y, sp8, 2, 0x1000);
            } else {
                spA = -0x4000;
                i_this->field_0x674 = player->eyePos;
                i_this->field_0x674.y += 70.0f + JREG_F(8);
            }

            cLib_addCalc2(&actor->current.pos.x, i_this->field_0x674.x, 1.0f, i_this->field_0x690);
            cLib_addCalc2(&actor->current.pos.z, i_this->field_0x674.z, 1.0f, i_this->field_0x690);
            cLib_addCalc2(&actor->current.pos.y, i_this->field_0x674.y, 1.0f, i_this->field_0x690);
            cLib_addCalc2(&i_this->field_0x690, 200.0f, 1.0f, 10.0f);
        } else {
            i_this->action = ACTION_WAIT;
            i_this->mode = -1;
            anm_init(i_this, 0x10, 2.0f, 0, 1.0f);
            i_this->sound.startCreatureVoice(Z2SE_EN_DB_V_RELEASE, -1);
        }
        break;
    }

    if (i_this->field_0xb0c != 0) {
        f32 temp_f31 = 50.0f + JREG_F(11);
        actor->current.pos.y -= temp_f31;
        actor->old.pos.y -= temp_f31;

        i_this->acch.CrrPos(dComIfG_Bgsp());

        actor->current.pos.y += temp_f31;
        actor->old.pos.y += temp_f31;
    }

    if (i_this->field_0x851 != 0) {
        cLib_addCalcAngleS2(&actor->shape_angle.x, (spA + 0x8000), 8, 0x400);
        i_this->field_0xb0c = 1;
    } else {
        cLib_addCalcAngleS2(&actor->shape_angle.x, -spA, 8, 0x400);
    }
}

static void e_hb_chance(e_hb_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    cXyz sp14;
    f32 temp_f31 = 60.0f + TREG_F(17);

    switch (i_this->mode) {
    case 0:
        if (i_this->field_0x851 != 0) {
            anm_init(i_this, 0xA, 5.0f, 2, 1.0f);
        } else {
            anm_init(i_this, 9, 5.0f, 2, 1.0f);
        }

        leaf_anm_init(i_this, 0x14, 5.0f, 2, 1.0f);
        i_this->mode = 1;
        i_this->timers[0] = 60;
        i_this->field_0x690 = 0.0f;
        /* fallthrough */
    case 1:
        sp14.x = temp_f31 * cM_ssin(i_this->counter * (TREG_S(0) + 900));
        sp14.z = temp_f31 * cM_scos(i_this->counter * (TREG_S(2) + 1000));

        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x + sp14.x, 0.1f, 20.0f * i_this->field_0x690);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z + sp14.z, 0.1f, 20.0f * i_this->field_0x690);

        if (i_this->field_0x851 != 0) {
            cLib_addCalc2(&actor->current.pos.y, actor->home.pos.y - (230.0f + TREG_F(18)), 0.1f, 20.0f * i_this->field_0x690);
        } else {
            cLib_addCalc2(&actor->current.pos.y, 230.0f + actor->home.pos.y + TREG_F(18), 0.1f, 20.0f * i_this->field_0x690);
        }

        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.1f);

        if (i_this->timers[0] <= 50 && i_this->timers[0] >= 10) {
            i_this->sound.startCreatureSoundLevel(0x70088, 0, -1);
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
            i_this->field_0x84c = 0.0f;
        }
        break;
    }

    if (i_this->field_0x851 != 0) {
        cLib_addCalcAngleS2(&actor->shape_angle.x, 0x4000, 4, 0x400);
    } else {
        cLib_addCalcAngleS2(&actor->shape_angle.x, -0x4000, 4, 0x400);
    }

    cLib_addCalc2(&i_this->field_0x848, 0.5f + JREG_F(4), 1.0f, 0.05f);
}

static void e_hb_s_damage(e_hb_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp10;

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 0xB, 3.0f, 0, 1.0f);
        leaf_anm_init(i_this, 0x13, 3.0f, 2, 0.0f);
        i_this->sound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);

        if (i_this->field_0x851 != 0) {
            cMtx_YrotS(*calc_mtx, (actor->shape_angle.y + 0x8000));
        } else {
            cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
        }

        sp10.x = 0.0f;
        sp10.y = 15.0f;
        sp10.z = -50.0f + TREG_F(16);
        MtxPosition(&sp10, &actor->speed);
        i_this->mode = 1;
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
        if (i_this->timers[1] == 0) {
            i_this->action = ACTION_WAIT;
            i_this->mode = 0;
            i_this->field_0x84c = 0.0f;
        }
        break;
    }

    if (i_this->field_0x851 != 0) {
        cLib_addCalcAngleS2(&actor->shape_angle.x, 0x4000, 4, 0x400);
    } else {
        cLib_addCalcAngleS2(&actor->shape_angle.x, -0x4000, 4, 0x400);
    }
}

static void e_hb_damage(e_hb_class* i_this) {
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
            i_this->action = ACTION_WAIT;
            i_this->mode = 0;
        }
    }
}

static void kuki_control1(e_hb_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp40;
    cXyz sp34;
    cXyz sp28;

    int i;
    cXyz* var_r31 = i_this->field_0x6a0;
    *var_r31 = actor->home.pos;

    cXyz sp1C;
    sp40.x = 0.0f;
    sp40.y = 0.0f;
    sp40.z = i_this->field_0x844;

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
    } else {
        sp28.y = -(200.0f + TREG_F(0));
    }
    
    f32 temp_f28 = i_this->field_0x84c;

    for (i = 1; i < 12; i++, var_r31++) {
        if (i_this->action == ACTION_CHANCE) {
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

            if (i_this->field_0x851 != 0) {
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

static void kuki_control2(e_hb_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp28;
    cXyz sp1C;

    int i;
    s16 temp_r27, temp_r26;
    cXyz* var_r31 = &i_this->field_0x6a0[11];
    csXyz* var_r30 = &i_this->field_0x730[11];

    *var_r31 = actor->current.pos;

    sp28.x = 0.0f;
    sp28.y = 0.0f;
    sp28.z = i_this->field_0x844;

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

static void kuki_control3(e_hb_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp20;

    sp20 = i_this->field_0x6a0[0] - actor->home.pos;

    cXyz* var_r29 = i_this->field_0x6a0;
    for (int i = 0; i < 12; i++, var_r29++) {
        *var_r29 -= sp20 * ((12 - i) / 11.0f);
    }
}

static void kuki_ha_set(e_hb_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz* ppos = i_this->field_0x6a0;
    csXyz* prot = i_this->field_0x730;

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

static f32 dummyFloat() {
    f32 temp = -10.0f;
    temp += 10000.0f;
    temp += 65536.0f;
    temp += -5.0f;
    return temp;
}

static void action(e_hb_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp70;
    cXyz sp64;

    i_this->angle_to_player = fopAcM_searchPlayerAngleY(actor);
    i_this->dist_to_player = (actor->home.pos - player->current.pos).abs();
    actor->field_0x566 = 0;
    i_this->field_0x1231 = 1;

    damage_check(i_this);

    if (i_this->invulnerabilityTimer != 0 && i_this->field_0x852 != 0) {
        daPy_getPlayerActorClass()->offDkCaught();
        daPy_getPlayerActorClass()->offDkCaught2();
    }

    s8 attn_ON = FALSE;
    s8 spA = FALSE;
    s8 kuki_sph_ON = FALSE;
    s8 sound_pl_search = FALSE;

    i_this->ccSph.OnCoSetBit();
    i_this->field_0x852 = 0;

    switch (i_this->action) {
    case ACTION_STAY:
        e_hb_stay(i_this);
        i_this->field_0x1231 = 0;
        break;
    case ACTION_APPEAR:
        e_hb_appear(i_this);
        attn_ON = TRUE;
        kuki_sph_ON = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        break;
    case ACTION_APPEAR_V:
        e_hb_appear_v(i_this);
        attn_ON = TRUE;
        kuki_sph_ON = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        break;
    case ACTION_WAIT:
        e_hb_wait(i_this);
        attn_ON = TRUE;
        kuki_sph_ON = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        break;
    case ACTION_ATTACK:
        e_hb_attack(i_this);
        attn_ON = TRUE;
        kuki_sph_ON = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        break;
    case ACTION_CHANCE:
        e_hb_chance(i_this);
        attn_ON = TRUE;
        kuki_sph_ON = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        break;
    case ACTION_S_DAMAGE:
        e_hb_s_damage(i_this);
        attn_ON = TRUE;
        kuki_sph_ON = TRUE;
        actor->field_0x566 = 1;
        sound_pl_search = TRUE;
        break;
    case ACTION_DAMAGE:
        e_hb_damage(i_this);
        attn_ON = TRUE;
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
        i_this->field_0x844 = i_this->field_0x848 * ((0.2f + TREG_F(1)) * sp70.abs());
        if (i_this->field_0x844 > 25.0f + TREG_F(7)) {
            i_this->field_0x844 = 25.0f + TREG_F(7);
        }

        cLib_addCalc2(&i_this->field_0x848, 1.0f, 1.0f, 0.01f);
        kuki_control1(i_this);
        kuki_control2(i_this);
        kuki_control3(i_this);
        kuki_ha_set(i_this);

        e_hb_leaf_class* pleaf = (e_hb_leaf_class*)fopAcM_SearchByID(i_this->leaf_actor_id);
        if (pleaf != NULL) {
            pleaf->mpMorf->play(NULL, 0, 0);
        }

        cLib_addCalc2(&i_this->field_0x84c, 1.0f, 1.0f, 0.05f);
    } else {
        kuki_ha_set(i_this);
    }

    cXyz* sp1C = i_this->field_0x6a0;
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

    cLib_addCalc0(&i_this->field_0x854, 1.0f, 5.0f + TREG_F(12));

    cXyz effsize(1.0f, 1.0f, 1.0f);
    setMidnaBindEffect((fopEn_enemy_c*)actor, &i_this->sound, &actor->eyePos, &effsize);
}

static void eff_set(e_hb_class* i_this, int param_1) {
    fopAc_ac_c* actor = &i_this->enemy;
    J3DModel* model = i_this->modelMorf->getModel();

    if (i_this->field_0x1230 != 0) {
        i_this->field_0x1230--;
        i_this->field_0x1231 = 3;
    }

    f32 var_f31;
    if (i_this->field_0x1231 != 1) {
        var_f31 = 0.0f;
    } else {
        var_f31 = 1.0f;
    }
    cLib_addCalc2(&i_this->field_0x124c, var_f31, 1.0f, 0.05f);

    for (int i = 0; i <= 2; i++) {
        static u16 p_name[] = {0x8339, 0x833A, 0x833B};
        static int p_idx[] = {2, 2, 6};

        if (param_1 == 0) {
            i_this->field_0x1234[i] = dComIfGp_particle_set(i_this->field_0x1234[i], p_name[i], &actor->current.pos, NULL, NULL);
        }

        JPABaseEmitter* pemitter = dComIfGp_particle_getEmitter(i_this->field_0x1234[i]);
        if (pemitter != NULL) {
            if (param_1 != 0) {
                pemitter->deleteAllParticle();
                dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0x1234[i]);
            } else {
                MTXCopy(model->getAnmMtx(p_idx[i]), *calc_mtx);
                MtxScale(i_this->field_0x124c, i_this->field_0x124c, i_this->field_0x124c, 1);
                pemitter->setGlobalSRTMatrix(*calc_mtx);
            }
        }
    }

    if (i_this->field_0x1231 != 0) {
        if (i_this->field_0x1231 == 1) {
            cXyz sp8(actor->current.pos);
            sp8.y += 100.0f;

            if (fopAcM_gc_c::gndCheck(&sp8)) {
                sp8.y = fopAcM_gc_c::getGroundY();
            }

            i_this->field_0x1244 = dComIfGp_particle_set(i_this->field_0x1244, 0x8338, &sp8, NULL, NULL);
        } else if (i_this->field_0x1231 == 2) {
            i_this->field_0x1240 = dComIfGp_particle_set(i_this->field_0x1240, 0x8336, &actor->current.pos, NULL, NULL);
            
            JPABaseEmitter* pemitter = dComIfGp_particle_getEmitter(i_this->field_0x1240);
            if (pemitter != NULL) {
                MTXCopy(model->getAnmMtx(1), *calc_mtx);
                pemitter->setGlobalRTMatrix(*calc_mtx);
            }
        } else if (i_this->field_0x1231 == 3) {
            i_this->field_0x1248 = dComIfGp_particle_set(i_this->field_0x1248, 0x8337, &actor->current.pos, NULL, NULL);
            
            JPABaseEmitter* pemitter = dComIfGp_particle_getEmitter(i_this->field_0x1248);
            if (pemitter != NULL) {
                MTXCopy(model->getAnmMtx(1), *calc_mtx);
                pemitter->setGlobalRTMatrix(*calc_mtx);
            }
        }

        i_this->field_0x1231 = 0;
    }
}

static void anm_se_set(e_hb_class* i_this) {
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
static int daE_HB_Execute(e_hb_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp4C;
    cXyz sp40;

    if (i_this->field_0x850 != 0) {
        i_this->field_0x850--;
        if (i_this->field_0x850 == 0) {
            fopAcM_delete(actor);
        }
        return 1;
    }

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

    cLib_addCalcAngleS2(&actor->shape_angle.z, 0, 8, 0x400);
    
    f32 size = l_HIO.base_size * i_this->size;
    mDoMtx_stack_c::scaleM(size, size, size);

    J3DModel* model = i_this->modelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->modelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    i_this->modelMorf->modelCalc();

    cXyz hide_offset(-30000.0f, 10000.0f, -20000.0f);

    MTXCopy(model->getAnmMtx(1), *calc_mtx);
    sp4C.set(30.0f + JREG_F(0), KREG_F(1), KREG_F(2));

    if (i_this->field_0x851 != 0) {
        actor->eyePos = i_this->field_0x6a0[YREG_S(3) + 6];
        actor->attention_info.position = actor->eyePos;
        i_this->ccSph.SetTgType(0xD8FBFDBF);
    } else {
        MtxPosition(&sp4C, &actor->eyePos);
        actor->attention_info.position = actor->eyePos;
        actor->attention_info.position.y += 60.0f;
        i_this->ccSph.SetTgType(0xD8FBFDFF);
    }

    MtxPosition(&sp4C, &sp40);
    if (i_this->invulnerabilityTimer != 0) {
        sp40 += hide_offset;
    }

    i_this->ccSph.SetC(sp40);
    i_this->ccSph.SetR(((40.0f + JREG_F(2)) * l_HIO.base_size) * i_this->size);
    dComIfG_Ccsp()->Set(&i_this->ccSph);

    if (i_this->field_0xb0c != 0) {
        i_this->atSph.MoveCAt(sp40);
        i_this->field_0xb0c = 0;
    } else {
        i_this->atSph.SetC(sp40 + hide_offset);
    }

    i_this->atSph.SetR(((30.0f + JREG_F(3)) * l_HIO.base_size) * i_this->size);
    dComIfG_Ccsp()->Set(&i_this->atSph);
    eff_set(i_this, 0);
    return 1;
}

static int daE_HB_IsDelete(e_hb_class* i_this) {
    return 1;
}

static int daE_HB_Delete(e_hb_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fpc_ProcID id = fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->phase, "E_hb");

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
    e_hb_class* a_this = (e_hb_class*)i_this;

    a_this->modelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_hb", 0x17), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_hb", 0x12), 2, 1.0f, 0, -1, &a_this->sound, 0x80000, 0x11000084);
    if (a_this->modelMorf == NULL || a_this->modelMorf->getModel() == NULL) {
        return 0;
    }

    if (!a_this->stalkLine.init(1, 12, (ResTIMG*)dComIfG_getObjectRes("E_hb", 0x1D), 1)) {
        return 0;
    }

    f32* psize = a_this->stalkLine.getSize(0);
    for (int i = 0; i < 12; i++, psize++) {
        *psize = 5.0f + (0.1f * XREG_S(0));
    }

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_hb", 0x19);
    JUT_ASSERT(3031, modelData != NULL);

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

static int daE_HB_Create(fopAc_ac_c* i_this) {
    e_hb_class* a_this = (e_hb_class*)i_this;
    fopAcM_ct(i_this, e_hb_class);

    int phase_state = dComIfG_resLoad(&a_this->phase, "E_hb");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_HB PARAM %x\n", fopAcM_GetParam(i_this));
        
        u8 swbit = ((fopAcM_GetParam(i_this) & 0xFF000000) >> 0x18);
        if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(i_this))) {
            OS_REPORT("E_HB やられ後なので再セットしません\n");
            return cPhs_ERROR_e;
        }

        a_this->arg0 = fopAcM_GetParam(i_this) & 0xF;
        a_this->arg1 = (fopAcM_GetParam(i_this) & 0x30) >> 4;
        a_this->arg2 = (fopAcM_GetParam(i_this) & 0xC0) >> 6;
        a_this->arg3 = (fopAcM_GetParam(i_this) & 0xFF00) >> 8;

        if (a_this->arg0 == 0xF) {
            a_this->arg0 = 0;
        }

        if (a_this->arg3 == 0xFF || a_this->arg3 == 0) {
            a_this->pl_search_range = 500.0f;
        } else {
            a_this->pl_search_range = 100.0f * a_this->arg3;
        }

        OS_REPORT("E_HB//////////////E_HB SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x4060)) {
            OS_REPORT("//////////////E_HB SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_HB SET 2 !!\n");

        if (!l_HIOInit) {
            a_this->HIOInit = TRUE;
            l_HIOInit = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("デグババ（ボックリ）", &l_HIO);
        }

        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        fopAcM_SetMtx(i_this, a_this->modelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_this, -300.0f, -400.0f, -300.0f);
        fopAcM_SetMax(i_this, 300.0f, 400.0f, 300.0f);

        i_this->health = 30;
        i_this->field_0x560 = 30;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0xd}, {0xd8fbfdff, 0x3}, 0x25}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
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
                {0x0, {{0x0, 0x0, 0xd}, {0xd8fbfdff, 0x3}, 0x0}}, // mObj
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

        a_this->acchcir.SetWall(50.0f, 50.0f);
        a_this->sound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        a_this->sound.setEnemyName("E_hb");

        a_this->atInfo.mpSound = &a_this->sound;
        a_this->atInfo.mPowerType = 1;
        a_this->atInfo.field_0x18 = 45;
        a_this->counter = cM_rndF(65535.0f);
        a_this->size = l_HIO.flower_size;

        if (a_this->arg0 == 2) {
            i_this->current.angle.x = -0x8000;
            i_this->shape_angle.x = -0x8000;
            a_this->field_0x851 = 1;
        }

        a_this->leaf_actor_id = fopAcM_createChild(PROC_E_HB_LEAF, fopAcM_GetID(i_this), 0, &i_this->current.pos, fopAcM_GetRoomNo(i_this), &i_this->shape_angle, NULL, -1, NULL);
        i_this->field_0x565 = (fopAcM_GetParam(i_this) & 0xFF0000) >> 0x10;
        i_this->field_0x564 = (fopAcM_GetParam(i_this) & 0xFF000000) >> 0x18;

        daE_HB_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_HB_Method = {
    (process_method_func)daE_HB_Create,
    (process_method_func)daE_HB_Delete,
    (process_method_func)daE_HB_Execute,
    (process_method_func)daE_HB_IsDelete,
    (process_method_func)daE_HB_Draw,
};

extern actor_process_profile_definition g_profile_E_HB = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_E_HB,              // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(e_hb_class),     // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    140,                    // mPriority
    &l_daE_HB_Method,       // sub_method
    0x10050100,             // mStatus
    fopAc_ENEMY_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
