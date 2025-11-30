/**
 * @file d_a_e_mm.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_mm.h"
#include "d/d_s_play.h"

#include "f_op/f_op_actor_enemy.h"

enum daE_MM_ACTION {
    ACTION_NORMAL,
    ACTION_DASH,
    ACTION_DEFENCE,
    ACTION_MAGNE_WAIT,
    ACTION_TURN,

    ACTION_DAMAGE = 10,
};

class daE_MM_HIO_c : public JORReflexible {
public:
    daE_MM_HIO_c();
    virtual ~daE_MM_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 base_size;
    /* 0x0C */ f32 move_speed;
    /* 0x10 */ f32 charge_speed;
    /* 0x14 */ f32 pl_search_range;
    /* 0x18 */ s16 reflect_chance_time;
    /* 0x1C */ f32 donketsu_base_size;
    /* 0x20 */ f32 donketsu_move_speed;
    /* 0x24 */ f32 donketsu_charge_speed;
    /* 0x28 */ f32 donketsu_pl_search_range;
    /* 0x2C */ f32 donketsu_reflect_speed;
    /* 0x30 */ s16 donketsu_reflect_chance_time;
    /* 0x32 */ s16 donketsu_angle;
};

daE_MM_HIO_c::daE_MM_HIO_c() {
    id = -1;
    base_size = 1.4f;
    move_speed = 2.0f;
    charge_speed = 15.0f;
    pl_search_range = 1000.0f;
    reflect_chance_time = 50;
    donketsu_base_size = 2.5f;
    donketsu_move_speed = 2.0f;
    donketsu_charge_speed = 8.0f;
    donketsu_pl_search_range = 1000.0f;
    donketsu_reflect_speed = 10.0f;
    donketsu_reflect_chance_time = 75;
    donketsu_angle = 300;
}

static void anm_init(e_mm_class* i_this, int i_anm, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->modelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_MM", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
    i_this->anm = i_anm;
}

static int daE_MM_Draw(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    J3DModel* model = i_this->modelMorf->getModel();

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);
    i_this->modelMorf->entryDL();

    cXyz spC;
    spC.set(actor->current.pos.x, 100.0f + actor->current.pos.y, actor->current.pos.z);
    i_this->shadowId = dComIfGd_setShadow(i_this->shadowId, 1, model, &spC, 700.0f, 0.0f, actor->current.pos.y, i_this->acch.GetGroundH(), i_this->acch.m_gnd, &actor->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static BOOL pl_check(e_mm_class* i_this, f32 i_range, s16) {
    fopAc_ac_c* actor = &i_this->enemy;

    if (i_this->dist_to_pl < i_range && !fopAcM_otherBgCheck(actor, dComIfGp_getPlayer(0)) && !dComIfGp_checkPlayerStatus0(0, 0x100)) {
        return 1;
    }

    return 0;
}

static u8 l_HIOInit;

static daE_MM_HIO_c l_HIO;

static void damage_checkMetOn(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    if (actor->health <= 0 || i_this->action == ACTION_DAMAGE) {
        return;
    }

    fopEn_enemy_c* enemy = &i_this->enemy;
    setMidnaBindEffect(enemy, &i_this->sound, &actor->current.pos, &actor->scale);

    if ((i_this->field_0xb99 & 4) && i_this->action != ACTION_MAGNE_WAIT) {
        i_this->field_0x6a4 = 10;
        i_this->field_0x672 = 0;

        anm_init(i_this, 0xB, 3.0f, 0, 1.0f);

        i_this->ccSph.OffTgNoHitMark();
        i_this->action = ACTION_MAGNE_WAIT;
        i_this->mode = 0;

        i_this->ccSph.SetTgType(0xD8FBFFFF);
        i_this->sound.startCreatureSound(Z2SE_EN_MM_MET_TAKEN, 0, -1);
        return;
    }

    if (i_this->field_0x6a4 == 0) {
        if (i_this->field_0xb99 & 1) {
            actor->speedF = -20.0f + BREG_F(14);
            if (actor->argument == 1) {
                actor->speedF = -1.0f * l_HIO.donketsu_reflect_speed;
            }

            i_this->action = ACTION_DASH;
            i_this->mode = 5;
            i_this->field_0x6a4 = 10;

            anm_init(i_this, 0xD, 10.0f, 2, 1.0f);
            i_this->ccSph.SetTgHitMark(CcG_Tg_UNK_MARK_2);

            if (actor->argument == 1) {
                i_this->timers[0] = l_HIO.donketsu_reflect_chance_time;
            } else {
                i_this->timers[0] = l_HIO.reflect_chance_time;
            }

            i_this->field_0xb99 &= ~1;
        } else if (i_this->field_0xb99 & 2) {
            i_this->field_0x6a4 = 10;
            i_this->action = ACTION_DEFENCE;
            i_this->field_0xb99 &= ~2;
            i_this->mode = 0;
        } else if (i_this->field_0xb99 & 8) {
            i_this->action = ACTION_DAMAGE;
            i_this->mode = 3;
            i_this->field_0x6a8 = 25.0f;
            i_this->field_0x6a4 = 10;
            actor->speedF = 0.0f;
        } else if (i_this->field_0xb99 & 16) {
            i_this->action = ACTION_DAMAGE;
            i_this->mode = 0;
            i_this->field_0x6a8 = 25.0f;
            actor->speedF = 0.0f;
            i_this->field_0x6a4 = 10;
            i_this->mode = 0;
            actor->health = 0;
            i_this->sound.startCreatureVoice(Z2SE_EN_MM_V_DEAD, -1);
        }
    }
}

static void damage_check(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (actor->health > 0) {
        fopEn_enemy_c* enemy = &i_this->enemy;
        setMidnaBindEffect(enemy, &i_this->sound, &actor->current.pos, &actor->scale);

        if (i_this->field_0x6a4 == 0) {
            i_this->ccStts.Move();

            if (i_this->ccSph.ChkAtShieldHit()) {
                actor->speedF = -20.0f + BREG_F(14);
                if (actor->argument == 1) {
                    actor->speedF = -1.0f * l_HIO.donketsu_reflect_speed;
                }

                i_this->action = ACTION_DASH;
                i_this->mode = 5;
                anm_init(i_this, 0xD, 10.0f, 2, 1.0f);
                i_this->sound.startCreatureVoice(Z2SE_EN_MM_V_NO_DAMAGE, -1);

                if (actor->argument == 1) {
                    i_this->timers[0] = l_HIO.donketsu_reflect_chance_time;
                } else {
                    i_this->timers[0] = l_HIO.reflect_chance_time;
                }

                i_this->atInfo.mpCollider = i_this->ccSph.GetAtHitObj();
                cCcD_Obj* sp20 = i_this->atInfo.mpCollider;
                i_this->field_0x6a4 = 10;
                return;
            }

            if (i_this->ccSph.ChkTgHit()) {
                i_this->atInfo.mpCollider = i_this->ccSph.GetTgHitObj();
                s16 prev_hp = actor->health;
                cc_at_check(actor, &i_this->atInfo);

                i_this->action = ACTION_DAMAGE;
                actor->speedF = 0.0f;
                i_this->mode = 3;
                i_this->field_0x6a4 = 10;

                if (prev_hp == actor->health) {
                    i_this->sound.startCreatureVoice(Z2SE_EN_MM_V_NO_DAMAGE, -1);
                } else {
                    i_this->sound.startCreatureVoice(Z2SE_EN_MM_V_DAMAGE, -1);
                }

                i_this->field_0x6a8 = 25.0f;
                i_this->field_0x6ac = i_this->atInfo.mHitDirection.y;
            }

            if (actor->health <= 1) {
                fopAcM_OffStatus(actor, 0);
                actor->attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;

                i_this->ccSph.OffAtSetBit();
                i_this->ccSph.OffTgSetBit();

                i_this->action = ACTION_DAMAGE;
                i_this->mode = 0;
                i_this->field_0x6a4 = 1000;
                actor->health = 0;

                i_this->ccSph.SetTgHitMark(CcG_Tg_UNK_MARK_3);
                i_this->sound.startCreatureVoice(Z2SE_EN_MM_V_DEAD, -1);
            }

            if (i_this->ccWindSph.ChkTgHit()) {
                dCcD_GObjInf* hit_gobj = i_this->ccWindSph.GetTgHitGObj();
                if (hit_gobj->GetAtMtrl() == dCcD_MTRL_WIND) {
                    cXyz* hit_vec_p = hit_gobj->GetAtVecP();
                    *hit_vec_p *= 0.5f + NREG_F(2);

                    cLib_addCalc(&i_this->field_0xbf4.x, hit_vec_p->x, 0.1f, 100.0f, 0.0f);
                    cLib_addCalc(&i_this->field_0xbf4.y, hit_vec_p->y, 0.1f, 100.0f, 0.0f);
                    cLib_addCalc(&i_this->field_0xbf4.z, hit_vec_p->z, 0.1f, 100.0f, 0.0f);
                    actor->current.pos += i_this->field_0xbf4;
                }
            }
        }
        
    }
}

static bool wall_check(e_mm_class* i_this, f32 param_1) {
    fopAc_ac_c* actor = &i_this->enemy;
    dBgS_LinChk linchk;
    cXyz sp24;
    cXyz start;
    cXyz end;

    cMtx_YrotS(*calc_mtx, actor->shape_angle.y);

    sp24.x = 0.0f;
    sp24.y = 40.0f + BREG_F(8);
    sp24.z = -50.0f + BREG_F(9);
    MtxPosition(&sp24, &start);
    start += actor->current.pos;
    i_this->field_0xbc4.set(start);

    sp24.x = 0.0f;
    sp24.y = 40.0f + BREG_F(8);
    sp24.z = param_1;
    MtxPosition(&sp24, &end);
    end += actor->current.pos;
    i_this->field_0xbd0.set(end);

    linchk.Set(&start, &end, actor);

    if (!dComIfG_Bgsp().LineCross(&linchk)) {
        return FALSE;
    }

    return TRUE;
}

static void mGround_WaterEffSet(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    J3DModel* model = i_this->modelMorf->getModel();

    for (int i = 0; i < 4; i++) {
        static u16 mDropEff[] = {
            ID_ZI_J_DOWNWTRA_A,
            ID_ZI_J_DOWNWTRA_B,
            ID_ZI_J_DOWNWTRA_C,
            ID_ZI_J_DOWNWTRA_D,
        };

        i_this->waterEffEmtIds[i] = dComIfGp_particle_set(i_this->waterEffEmtIds[i], mDropEff[i], &actor->current.pos, NULL, NULL);
        
        JPABaseEmitter* emt = dComIfGp_particle_getEmitter(i_this->waterEffEmtIds[i]);
        if (emt != NULL) {
            emt->setGlobalRTMatrix(model->getAnmMtx(0));
        }
    }
}

static void mWaterEffSet(e_mm_class* i_this, bool param_1) {
    fopAc_ac_c* actor = &i_this->enemy;
    J3DModel* model = i_this->modelMorf->getModel();

    if (i_this->acch.ChkWaterHit()) {
        for (int i = 0; i < 6; i++) {
            if (!param_1 || i != 2) {
                static u16 mDushEff[] = {
                    dPa_RM(ID_ZF_S_MMRUNWAVE00),
                    dPa_RM(ID_ZF_S_MMRUNWAVE01),
                    dPa_RM(ID_ZF_S_MMRUNWAVE02),
                    ID_ZI_J_DASHWTRA_A,
                    ID_ZI_J_DASHWTRA_B,
                    ID_ZI_J_DASHWTRA_C,
                };

                i_this->field_0xb9c[i] = dComIfGp_particle_set(i_this->field_0xb9c[i], mDushEff[i], &actor->current.pos, NULL, NULL);
                
                JPABaseEmitter* emt = dComIfGp_particle_getEmitter(i_this->field_0xb9c[i]);
                if (emt != NULL) {
                    emt->setGlobalRTMatrix(model->getAnmMtx(0));
                    if (param_1 && i == 1) {
                        emt->setRate(2.0f);
                    }
                }
            }
        }

        if (param_1 && i_this->field_0xc01 == 0) {
            i_this->field_0xc01 = 1;
            
            JPABaseEmitter* emt = dComIfGp_particle_set(dPa_RM(ID_ZF_S_MMSTOPWAVE00), &actor->current.pos, NULL, NULL);
            if (emt != NULL) {
                emt->setGlobalRTMatrix(model->getAnmMtx(0));
            }
        }
    }
}

static bool way_gake_check(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    dBgS_GndChk gndchk;
    cXyz sp14;
    cXyz sp8;

    if (wall_check(i_this, 200.0f + BREG_F(10)) || i_this->acch.ChkWallHit()) {
        return FALSE;
    }

    cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
    sp14.x = 0.0f;
    sp14.y = 200.0f + BREG_F(11);
    sp14.z = 200.0f + BREG_F(12);
    MtxPosition(&sp14, &sp8);
    sp8 += actor->current.pos;

    i_this->field_0xbdc.set(sp8);
    gndchk.SetPos(&sp8);

    sp8.y = dComIfG_Bgsp().GroundCross(&gndchk);
    if (-G_CM3D_F_INF == sp8.y) {
        return TRUE;
    }

    i_this->field_0xbe8.set(sp8);
    if (actor->current.pos.y - sp8.y > 200.0f) {
        return TRUE;
    }

    return FALSE;
}

static void e_mm_normal(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp38;
    cXyz sp2C;

    f32 move_speed = 0.0f;

    if (way_gake_check(i_this)) {
        anm_init(i_this, 0xE, 10.0f, 2, 1.0f);
        i_this->target_ya = actor->current.angle.y + (s16)(32768.0f + cM_rndFX(10000.0f));
        i_this->mode = 0;
        i_this->action = ACTION_TURN;
        return;
    }

    switch (i_this->mode) {
    case 0:
        if (actor->argument == 1) {
            move_speed = l_HIO.donketsu_move_speed;
        } else {
            move_speed = l_HIO.move_speed;
        }

        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->target_ya, 8, 0x100);

        if ((int)i_this->modelMorf->getFrame() == 15 || (int)i_this->modelMorf->getFrame() == 28) {
            u32 sp1C = 1;
            if (i_this->field_0x672 == 0) {
                sp1C = 2;
            }

            if (!i_this->acch.ChkWaterHit()) {
                i_this->sound.startCreatureSound(Z2SE_EN_MM_WALK_LND, sp1C, -1);
            } else {
                i_this->sound.startCreatureSound(Z2SE_EN_MM_WALK_WTR, sp1C, -1);
            }
        }

        if (i_this->timers[0] == 0 || (i_this->timers[1] == 0 && fopAcM_wayBgCheck(actor, 200.0f, 50.0f))) {
            i_this->mode = 1;
            i_this->timers[0] = 50.0f + cM_rndF(100.0f);
            anm_init(i_this, 8, 10.0f, 2, 1.0f);
            i_this->sound.startCreatureVoice(Z2SE_EN_MM_V_KYORO, -1);
        }
        break;
    case 1:
        if (!i_this->modelMorf->getFrame()) {
            i_this->sound.startCreatureVoice(Z2SE_EN_MM_V_KYORO, -1);
        }

        s16 sp8;
        if (i_this->timers[0] == 0) {
            s16 var_r27;
            if (fopAcM_wayBgCheck(actor, 200.0f, 50.0f)) {
                var_r27 = 32768.0f + cM_rndFX(10000.0f);
                i_this->timers[1] = TREG_S(5) + 80;
            } else {
                sp2C.x = actor->home.pos.x + cM_rndFX(600.0f);
                sp2C.y = actor->home.pos.y;
                sp2C.z = actor->home.pos.z + cM_rndFX(600.0f);
                sp38 = sp2C - actor->current.pos;

                var_r27 = sp38.atan2sX_Z() - actor->current.angle.y;
                if (var_r27 > 0x3000) {
                    var_r27 = 0x3000;
                } else if (var_r27 < -0x3000) {
                    var_r27 = -0x3000;
                }
            }

            i_this->target_ya = actor->current.angle.y + var_r27;
            anm_init(i_this, 0xE, 10.0f, 2, 1.0f);
            i_this->mode = 0;
            i_this->timers[0] = 100.0f + cM_rndF(100.0f);
        } else {
            sp8 = 0x6000;
        }
        break;
    }

    cLib_addCalc2(&actor->speedF, move_speed, 1.0f, 3.0f);

    f32 pl_search_range;
    if (actor->argument == 1) {
        pl_search_range = l_HIO.donketsu_pl_search_range;
    } else {
        pl_search_range = l_HIO.pl_search_range;
    }

    if (i_this->field_0x5b4 != 0xFF && i_this->field_0x5b4 != 0) {
        pl_search_range = 10.0f * i_this->field_0x5b4;
    }

    if (pl_check(i_this, pl_search_range, 0x5000) && fopAcM_searchActorDistanceY(actor, dComIfGp_getPlayer(0)) < 100.0f) {
        i_this->action = ACTION_DASH;
        i_this->mode = 0;
    }
}

static void e_mm_dash(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp40;
    cXyz sp34;

    f32 move_speed = 0.0f;
    f32 var_f30 = 3.0f;
    s16 spA = 0;
    s16 sp8 = 0;
    int var_r27 = 1;

    if (i_this->mode != 4 && way_gake_check(i_this)) {
        i_this->mode = 4;
        anm_init(i_this, 0xA, 3.0f, 0, 1.0f);
        mWaterEffSet(i_this, 1);
    }
 
    switch (i_this->mode) {
    case 0:
        i_this->field_0xc01 = 0;
        anm_init(i_this, 7, 3.0f, 0, 1.0f);
        i_this->mode = 1;
        break;
    case 1:
        if ((int)i_this->modelMorf->getFrame() == 3) {
            i_this->sound.startCreatureVoice(Z2SE_EN_MM_V_FIND, -1);
        }

        if (i_this->modelMorf->isStop()) {
            anm_init(i_this, 0xC, 3.0f, 2, 1.0f);
            i_this->mode = 2;
        }
        break;
    case 2:
        if (i_this->field_0x672 == 0) {
            var_r27 = 2;
        }

        if ((int)i_this->modelMorf->getFrame() == 9 || (int)i_this->modelMorf->getFrame() == 19) {
            if (!i_this->acch.ChkWaterHit()) {
                i_this->sound.startCreatureSound(Z2SE_EN_MM_WALK_LND, var_r27, -1);
            } else {
                i_this->sound.startCreatureSound(Z2SE_EN_MM_WALK_WTR, var_r27, -1);
            }
        }

        i_this->target_ya = i_this->angle_to_pl;
        spA = 0x300;
        sp8 = (i_this->target_ya - actor->current.angle.y);
        if (sp8 < 0x200 && sp8 > -0x200) {
            anm_init(i_this, 9, 3.0f, 2, 1.0f);
            i_this->mode = 3;
            i_this->timers[0] = 70;
            i_this->sound.startCreatureVoice(Z2SE_EN_MM_V_RUSH, -1);
        }
        break;
    case 3:
        spA = 0x300;
        if (actor->argument == 1) {
            move_speed = l_HIO.donketsu_charge_speed;
        } else {
            move_speed = l_HIO.charge_speed;
        }

        i_this->field_0xb98 = 1;

        if ((i_this->timers[0] & 3) == 0) {
            fopAcM_effSmokeSet2(&i_this->field_0xc04, &i_this->field_0xc08, &actor->current.pos, &actor->shape_angle, 1.5f * actor->scale.x, &actor->tevStr);
        }

        if (i_this->field_0x672 == 0) {
            var_r27 = 2;
        }

        mWaterEffSet(i_this, 0);

        if ((int)i_this->modelMorf->getFrame() == 5 || (int)i_this->modelMorf->getFrame() == 8) {
            if (!i_this->acch.ChkWaterHit()) {
                i_this->sound.startCreatureSound(Z2SE_EN_MM_RUN_LND, var_r27, -1);
            } else {
                i_this->sound.startCreatureSound(Z2SE_EN_MM_RUN_WTR, var_r27, -1);
            }
        }

        if (wall_check(i_this, 100.0f + BREG_F(13))) {
            actor->speedF = -20.0f + BREG_F(14);
            if (actor->argument == 1) {
                actor->speedF = -1.0f * l_HIO.donketsu_reflect_speed;
            }

            i_this->mode = 5;
            anm_init(i_this, 0xD, 10.0f, 2, 1.0f);

            if (actor->argument == 1) {
                i_this->timers[0] = l_HIO.donketsu_reflect_chance_time;
            } else {
                i_this->timers[0] = l_HIO.reflect_chance_time;
            }

            if (i_this->field_0x672 != 0) {
                i_this->sound.startCreatureSound(Z2SE_EN_MM_R2W_MET, 0, -1);
            } else {
                i_this->sound.startCreatureSound(Z2SE_EN_MM_R2W_NOMET, 0, -1);
            }
        }

        if (i_this->timers[0] == 0) {
            if (!i_this->acch.ChkWaterHit()) {
                i_this->sound.startCreatureSound(Z2SE_EN_MM_BRAKE_LND, 0, -1);
            } else {
                i_this->sound.startCreatureSound(Z2SE_EN_MM_BRAKE_WTR, 0, -1);
            }

            i_this->mode = 4;
            anm_init(i_this, 0xA, 3.0f, 0, 1.0f);
        }
        break;
    case 4:
        move_speed = 0.0f;
        var_f30 = 1.0f + TREG_F(13);

        if (actor->speedF < 0.1f) {
            i_this->action = ACTION_NORMAL;
            i_this->mode = 0;
            i_this->timers[0] = 0;
            anm_init(i_this, 0xE, 20.0f, 2, 1.0f);
        }

        mWaterEffSet(i_this, 1);

        if ((i_this->timers[0] & 3) == 0) {
            fopAcM_effSmokeSet2(&i_this->field_0xc04, &i_this->field_0xc08, &actor->current.pos, &actor->shape_angle, 1.5f + XREG_F(3), &actor->tevStr);
        }
        break;
    case 5:
        move_speed = 0.0f;
        var_f30 = 1.0f + TREG_F(13);

        if (actor->speedF >= 1.0f) {
            mWaterEffSet(i_this, 1);
        }

        if (actor->speedF < 0.1f && i_this->timers[0] == 0) {
            i_this->action = ACTION_NORMAL;
            i_this->mode = 0;
        }
        break;
    }

    if (actor->argument == 1) {
        spA = l_HIO.donketsu_angle;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->target_ya, 1, spA);
    cLib_addCalc2(&actor->speedF, move_speed, 1.0f, var_f30);
}

static void e_mm_defence(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp14;
    cXyz sp8;

    switch (i_this->mode) {
    case 0:
        if (actor->argument == 1) {
            anm_init(i_this, 5, 3.0f, 2, 0.5f);
        } else {
            anm_init(i_this, 5, 3.0f, 2, 1.0f);
        }

        i_this->mode = 1;
        i_this->timers[0] = 30;
        break;
    case 1:
        if (i_this->timers[0] == 0 && !i_this->modelMorf->isStop()) {
            i_this->action = ACTION_DASH;
            i_this->mode = 0;
        }
        break;
    }

    cLib_addCalc0(&actor->speedF, 1.0f, 3.0f);
}

static void e_mm_magne_wait(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp18;
    cXyz spC;

    switch (i_this->mode) {
    case 0:
        if ((int)i_this->modelMorf->getFrame() == 16) {
            i_this->sound.startCreatureVoice(Z2SE_EN_MM_V_SURPRISE, -1);
        }

        if (i_this->modelMorf->isStop()) {
            i_this->action = ACTION_NORMAL;
            i_this->mode = 0;
            i_this->timers[0] = 0;
            anm_init(i_this, 0xE, 20.0f, 2, 1.0f);
        }
    }

    cLib_addCalc0(&actor->speedF, 1.0f, 3.0f);
}

static void e_mm_turn(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->target_ya, 8, 0x200);

    if ((int)i_this->modelMorf->getFrame() == 9 || (int)i_this->modelMorf->getFrame() == 19) {
        u32 var_r27 = 1;
        if (i_this->field_0x672 == 0) {
            var_r27 = 2;
        }

        if (!i_this->acch.ChkWaterHit()) {
            i_this->sound.startCreatureSound(Z2SE_EN_MM_WALK_LND, var_r27, -1);
        } else {
            i_this->sound.startCreatureSound(Z2SE_EN_MM_WALK_WTR, var_r27, -1);
        }
    }

    if (BREG_S(1) != 0) {
        OS_REPORT("abs ( actor->current.angle.sy - i_this->target_ya ) %x\n", abs ( actor->current.angle.y - i_this->target_ya ));
    }

    if (abs(actor->current.angle.y - i_this->target_ya) < 0x400) {
        if (!way_gake_check(i_this)) {
            i_this->mode = 1;
            i_this->timers[0] = 50.0f + cM_rndF(100.0f);
            anm_init(i_this, 8, 10.0f, 2, 1.0f);
            i_this->action = ACTION_NORMAL;
            return;
        }

        i_this->target_ya += 0x2000;
    }

    cLib_addCalc0(&actor->speedF, 1.0f, 3.0f);
}

static void e_mm_damage(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    switch (i_this->mode) {
    case 0:
        anm_init(i_this, 6, 3.0f, 0, 1.0f);
        i_this->mode = 1;
        break;
    case 1:
        if ((int)i_this->modelMorf->getFrame() == 25) {
            if (!i_this->acch.ChkWaterHit()) {
                i_this->sound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
            } else {
                i_this->sound.startCreatureSound(Z2SE_CM_BODYFALL_ASASE_S, 0, -1);
            }

            if (i_this->acch.ChkWaterHit()) {
                mGround_WaterEffSet(i_this);
            }
        }

        if (i_this->modelMorf->isStop()) {
            i_this->timers[0] = 18;
            i_this->mode = 2;
        }
        break;
    case 2:
        if (i_this->timers[0] == 0) {
            i_this->field_0x672 = 0;

            fopAc_ac_c* armor_actor;
            if (actor->argument == 1 && fopAcM_SearchByID(i_this->armorChildId, &armor_actor) && armor_actor != NULL) {
                fopAcM_delete(armor_actor);
            }

            if (actor->argument == 1) {
                fopAcM_createDisappear(actor, &actor->current.pos, 8.0f * actor->scale.x, 0, 0x25);
            } else {
                fopAcM_createDisappear(actor, &actor->current.pos, 8.0f * actor->scale.x, 0, 0x14);
            }

            if (i_this->bitSw != 0xFF) {
                fopAcM_onSwitch(actor, i_this->bitSw);
            }

            fopAcM_delete(actor);
        }
        break;
    case 3:
        anm_init(i_this, 4, 3.0f, 0, 1.0f);
        i_this->mode = 4;
        break;
    case 4:
        if (i_this->modelMorf->isStop()) {
            i_this->field_0xb99 ^= 8;
            i_this->action = ACTION_DASH;
            i_this->mode = 0;
        }
        break;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->field_0x6ac, 2, 0x800);
}

static void action(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp40;
    cXyz sp34;

    i_this->angle_to_pl = fopAcM_searchPlayerAngleY(actor);
    i_this->dist_to_pl = fopAcM_searchPlayerDistance(actor);

    if (i_this->field_0x672 != 0) {
        damage_checkMetOn(i_this);
    } else {
        damage_check(i_this);
    }

    i_this->field_0xb98 = 0;
    BOOL sp30 = FALSE;

    switch (i_this->action) {
    case ACTION_NORMAL:
        e_mm_normal(i_this);
        break;
    case ACTION_DASH:
        e_mm_dash(i_this);
        sp30 = TRUE;
        break;
    case ACTION_DEFENCE:
        e_mm_defence(i_this);
        break;
    case ACTION_MAGNE_WAIT:
        e_mm_magne_wait(i_this);
        break;
    case ACTION_TURN:
        e_mm_turn(i_this);
        break;
    case ACTION_DAMAGE:
        e_mm_damage(i_this);
        break;
    }

    i_this->sound.setLinkSearch(sp30);

    cXyz* cc_move_p = i_this->ccStts.GetCCMoveP();
    if (cc_move_p != NULL) {
        actor->current.pos.x += cc_move_p->x;
        actor->current.pos.y += cc_move_p->y;
        actor->current.pos.z += cc_move_p->z;
    }

    cMtx_YrotS(*calc_mtx, actor->current.angle.y);

    sp40.x = 0.0f;
    sp40.y = 0.0f;
    sp40.z = actor->speedF * actor->scale.x;
    MtxPosition(&sp40, &sp34);
    actor->speed.x = sp34.x;
    actor->speed.z = sp34.z;
    actor->speed.y -= 3.0f;
    actor->current.pos += actor->speed;

    if (i_this->field_0x6a8 > 0.1f) {
        sp40.x = 0.0f;
        sp40.y = 0.0f;
        sp40.z = -i_this->field_0x6a8;
        cMtx_YrotS(*calc_mtx, i_this->field_0x6ac);
        MtxPosition(&sp40, &sp34);
        actor->current.pos += sp34;

        cLib_addCalc0(&i_this->field_0x6a8, 1.0f, 3.0f + TREG_F(12));
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 4, 0x1000);
    actor->shape_angle.x = actor->current.angle.x;
    i_this->acchcir.SetWall(50.0f, 80.0f);
    actor->current.pos += i_this->field_0x674;
    i_this->acch.CrrPos(dComIfG_Bgsp());

    if (actor->health > 1) {
        if (-G_CM3D_F_INF == i_this->acch.GetGroundH()) {
            if (actor->current.pos.y < actor->home.pos.y - 3000.0f) {
                i_this->sound.startCreatureSound(Z2SE_EN_MM_FALL_LND, 0, -1);
                i_this->sound.startCreatureVoice(Z2SE_EN_MM_V_DEAD, -1);

                i_this->field_0x6a8 = 0.0f;
                actor->speedF = 0.0f;
                actor->health = 0;

                fopAc_ac_c* armor_actor;
                if (i_this->field_0x672 != 0 && fopAcM_SearchByID(i_this->armorChildId, &armor_actor) && armor_actor != NULL) {
                    fopAcM_delete(armor_actor);
                }

                fopAcM_createDisappear(actor, &actor->current.pos, 8.0f * actor->scale.x, 0, 0xFF);

                if (i_this->bitSw != 0xFF) {
                    fopAcM_onSwitch(actor, i_this->bitSw);
                }

                fopAcM_delete(actor);
            }
        } else if (i_this->acch.ChkWaterHit() && i_this->acch.m_wtr.GetHeight() - 50.0f > actor->current.pos.y) {
            i_this->action = 10;
            i_this->mode = 0;
            i_this->sound.startCreatureSound(Z2SE_EN_MM_FALL_WTR, 0, -1);
            i_this->sound.startCreatureVoice(Z2SE_EN_MM_V_DEAD, -1);
            mGround_WaterEffSet(i_this);
            i_this->field_0x6a8 = 0.0f;
            actor->speedF = 0.0f;
            actor->health = 0;
            return;
        }

        if (i_this->acch.ChkGroundHit()) {
            if (i_this->field_0x698 - 500.0f > actor->current.pos.y) {
                i_this->action = ACTION_DAMAGE;
                i_this->mode = 0;
                i_this->sound.startCreatureSound(Z2SE_EN_MM_FALL_LND, 0, -1);
                i_this->sound.startCreatureVoice(Z2SE_EN_MM_V_DEAD, -1);
                i_this->field_0x6a8 = 0.0f;
                actor->speedF = 0.0f;
                actor->health = 0;
            } else if (i_this->field_0x698 - 100.0f > actor->current.pos.y) {
                if (!i_this->acch.ChkWaterHit()) {
                    i_this->sound.startCreatureSound(Z2SE_EN_MM_FALL_LND, 0, -1);
                } else {
                    i_this->sound.startCreatureSound(Z2SE_EN_MM_FALL_WTR, 0, -1);
                }

                mGround_WaterEffSet(i_this);
            }

            i_this->field_0x698 = actor->current.pos.y;
        }
    }
}

static int daE_MM_Execute(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp20;
    cXyz sp14;

    if (actor->argument == 1) {
        actor->scale.set(l_HIO.donketsu_base_size, l_HIO.donketsu_base_size, l_HIO.donketsu_base_size);
        fopAc_ac_c* armor_actor;
        if (i_this->armorChildId != 0 && fopAcM_SearchByID(i_this->armorChildId, &armor_actor) && armor_actor != NULL) {
            armor_actor->scale.set(l_HIO.donketsu_base_size, l_HIO.donketsu_base_size, l_HIO.donketsu_base_size);
        }
    }

    i_this->lifetime++;

    for (int i = 0; i < 4; i++) {
        if (i_this->timers[i] != 0) {
            i_this->timers[i]--;
        }
    }

    if (i_this->field_0x6a4 != 0) {
        i_this->field_0x6a4--;
    }

    action(i_this);

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::XrotM(actor->shape_angle.x);
    mDoMtx_stack_c::scaleM(actor->scale.x, actor->scale.y, actor->scale.z);

    J3DModel* model = i_this->modelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    int sp8 = 0;
    if (actor->argument == 1) {
        sp8 = 3;
    }

    i_this->modelMorf->play(sp8, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    i_this->modelMorf->modelCalc();

    actor->eyePos = actor->current.pos;
    actor->eyePos.y += (40.0f + JREG_F(2)) * actor->scale.x;
    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += 40.0f * actor->scale.x;

    if (i_this->field_0x672 == 0) {
        sp14 = actor->current.pos;
        sp14.y += (40.0f + JREG_F(4)) * actor->scale.x;

        if (i_this->field_0x6a4 != 0) {
            sp14.x += 35353.0f;
        }

        i_this->ccSph.SetC(sp14);
        i_this->ccSph.SetR((40.0f + JREG_F(3)) * actor->scale.x);
        dComIfG_Ccsp()->Set(&i_this->ccSph);

        i_this->ccWindSph.SetC(sp14);
        i_this->ccWindSph.SetR((40.0f + JREG_F(3)) * actor->scale.x);
        dComIfG_Ccsp()->Set(&i_this->ccWindSph);

        if (i_this->field_0xb98 != 0) {
            i_this->ccSph.OnAtSetBit();
        } else {
            i_this->ccSph.OffAtSetBit();
        }
    }

    return 1;
}

static int daE_MM_IsDelete(e_mm_class* i_this) {
    return 1;
}

static int daE_MM_Delete(e_mm_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    fopAcM_GetID(actor);
    dComIfG_resDelete(&i_this->phase, "E_MM");

    if (i_this->field_0xc00) {
        l_HIOInit = 0;
    }

    if (actor->heap != NULL) {
        i_this->sound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_mm_class* a_this = (e_mm_class*)i_this;

    int bmd_index = 0x13;
    if (i_this->argument == 1) {
        bmd_index = 0x12;
    }

    a_this->modelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_MM", bmd_index), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_MM", 0xD), 2, 1.0f, 0, -1, &a_this->sound, 0x80000, 0x11000084);
    if (a_this->modelMorf == NULL || a_this->modelMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int daE_MM_Create(fopAc_ac_c* actor) {
    fopAcM_ct(actor, e_mm_class);
    e_mm_class* i_this = (e_mm_class*)actor;

    int phase_state = dComIfG_resLoad(&i_this->phase, "E_MM");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_MM PARAM %x\n", fopAcM_GetParam(actor));
        i_this->field_0x5b4 = fopAcM_GetParam(actor);
        i_this->bitSw = (fopAcM_GetParam(actor) & 0xFF00) >> 8;

        if (i_this->bitSw != 0xFF && fopAcM_isSwitch(actor, i_this->bitSw)) {
            return cPhs_ERROR_e;
        }
    
        OS_REPORT("E_MM・i_this->bitSw %x\n", i_this->bitSw);
        OS_REPORT("E_MM//////////////E_MM SET 1 !!\n");
    
        u32 heap_size = 0x1AF0;
        if (actor->argument == 1) {
            heap_size = 0x1BB0;
        }
    
        if (!fopAcM_entrySolidHeap(actor, useHeapInit, heap_size)) {
            OS_REPORT("//////////////E_MM SET NON !!\n");
            return cPhs_ERROR_e;
        }
    
        OS_REPORT("//////////////E_MM SET 2 !!\n");

        if (!l_HIOInit) {
            i_this->field_0xc00 = TRUE;
            l_HIOInit = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("マグネメット", &l_HIO);
        }

        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(actor, i_this->modelMorf->getModel()->getBaseTRMtx());

        i_this->ccStts.Init(100, 0, actor);

        actor->health = 20;
        actor->field_0x560 = 20;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0xd8fbfdff, 0x43}, 0x75}}, // mObj
                {dCcD_SE_SOFT_BODY, 0x0, 0x1, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x406}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        static dCcD_SrcSph cc_wind_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0x200, 0x11}, 0x0}}, // mObj
                {dCcD_SE_SOFT_BODY, 0x0, 0x1, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        i_this->ccSph.Set(cc_sph_src);
        i_this->ccSph.SetStts(&i_this->ccStts);
        i_this->ccWindSph.Set(cc_wind_sph_src);
        i_this->ccWindSph.SetStts(&i_this->ccStts);

        i_this->acch.Set(fopAcM_GetPosition_p(actor), fopAcM_GetOldPosition_p(actor), actor, 1, &i_this->acchcir, fopAcM_GetSpeed_p(actor), NULL, NULL);
        i_this->acchcir.SetWall(50.0f, 60.0f);

        i_this->field_0x698 = actor->current.pos.y;
        i_this->sound.init(&actor->current.pos, &actor->eyePos, 3, 1);
        i_this->sound.setEnemyName("E_mm");
        i_this->atInfo.mpSound = &i_this->sound;
        i_this->atInfo.mPowerType = 9;

        if (BREG_S(0) != 0) {
            actor->argument = 1;
        }

        if (actor->argument == 1) {
            i_this->ccStts.SetWeight(254);
            i_this->ccSph.SetAtAtp(2);

            actor->scale.set(l_HIO.donketsu_base_size, l_HIO.donketsu_base_size, l_HIO.donketsu_base_size);
            actor->health = 40;
            actor->field_0x560 = 40;
        } else {
            i_this->ccStts.SetWeight(200);
            actor->scale.set(l_HIO.base_size, l_HIO.base_size, l_HIO.base_size);
        }

        i_this->armorChildId = fopAcM_createChild(PROC_E_MM_MT, fopAcM_GetID(actor), 0, &actor->current.pos, fopAcM_GetRoomNo(actor), &actor->shape_angle, &actor->scale, actor->argument, NULL);
        i_this->field_0x672 = 1;

        daE_MM_Execute(i_this);
    }

    return phase_state;
}

static actor_method_class l_daE_MM_Method = {
    (process_method_func)daE_MM_Create,
    (process_method_func)daE_MM_Delete,
    (process_method_func)daE_MM_Execute,
    (process_method_func)daE_MM_IsDelete,
    (process_method_func)daE_MM_Draw,
};

extern actor_process_profile_definition g_profile_E_MM = {
  fpcLy_CURRENT_e,       // mLayerID
  7,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_E_MM,             // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(e_mm_class),    // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  167,                   // mPriority
  &l_daE_MM_Method,      // sub_method
  0x00040120,            // mStatus
  fopAc_ENEMY_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
