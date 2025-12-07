/**
 * @file d_a_b_bh.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_bh.h"
#include "d/actor/d_a_b_bq.h"
#include "d/d_bomb.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "c/c_damagereaction.h"
#include "m_Do/m_Do_controller_pad.h"
#include "Z2AudioLib/Z2Instances.h"

#define ACTION_WAIT       0
#define ACTION_ATTACK_1   5
#define ACTION_BOMB_EAT   10
#define ACTION_DOWN       11
#define ACTION_B_WAIT     20
#define ACTION_B_ATTACK_1 21
#define ACTION_B_BOMB_EAT 22
#define ACTION_B_DOWN     23
#define ACTION_START      50

enum B_BH_RES_FILE_ID {
    /* BCK */
    /* 0x05 */ BCK_BH_ATTACK = 5,
    /* 0x06 */ BCK_BH_ATTACKWAIT,
    /* 0x07 */ BCK_BH_DAMAGE,
    /* 0x08 */ BCK_BH_EAT,
    /* 0x09 */ BCK_BH_EATWAIT,
    /* 0x0A */ BCK_BH_NODAMAGE,
    /* 0x0B */ BCK_BH_ROUT,
    /* 0x0C */ BCK_BH_WAIT,
    /* 0x0D */ BCK_BH_WAIT01,
    /* 0x0E */ BCK_BH_WAIT02,

    /* BMDR */
    /* 0x11 */ BMDR_BH_KUKI = 0x11,
    /* 0x12 */ BMDR_BH_LEAF,

    /* BMDV */
    /* 0x15 */ BMDV_BH = 0x15,
};

daB_BH_HIO_c::daB_BH_HIO_c() {
    no = -1;
    model_size = 1.25f;
    attack_freq_a = 350;
    attack_freq_b = 250;
    down_revive_time = 250;
}

#if DEBUG
void daB_BH_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("  ボスデグババ手", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("本体サイズ", &model_size, 0.0f, 5.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("攻撃頻度\x87\x40", &attack_freq_a, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("攻撃頻度\x87\x41", &attack_freq_b, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("ダウン→復活時間", &down_revive_time, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
}
#endif

static void anm_init(b_bh_class* i_this, int i_anmID, f32 i_morf, u8 i_attr, f32 i_speed) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_BH", i_anmID), i_attr, i_morf, i_speed, 0.0f, -1.0f);
    i_this->mAnm = i_anmID;
}

static int daB_BH_Draw(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    
    J3DModel* model = i_this->mpModelMorf->getModel();
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);

    J3DModelData* modelData = model->getModelData();
    fopAcM_setEffectMtx(a_this, modelData);

    i_this->mpModelMorf->entryDL();

    for (int i = 0; i < 17; i++) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpStemModels[i], &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mpStemModels[i]);
        
        if (i_this->mpLeafModels[i] != NULL) {
            g_env_light.setLightTevColorType_MAJI(i_this->mpLeafModels[i], &a_this->tevStr);
            mDoExt_modelUpdateDL(i_this->mpLeafModels[i]);
        }
    }

    return 1;
}

/* 805B33ED 0003+00 data_805B33ED None */
static u8 l_initHIO;

static daB_BH_HIO_c l_HIO;

static b_bh_class* bh[2];

static void b_bh_wait(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(a_this->parentActorID);
    cXyz sp34;

    f32 temp_f31 = 30.0f + TREG_F(12);
    if (bq_p->field_0x6fb != 0) {
        if (i_this->mID == 0) {
            i_this->mTimers[1] = 100.0f + cM_rndF(50.0f);
        } else {
            i_this->mTimers[1] = 50.0f + cM_rndF(50.0f);
        }
    }

    if (dComIfGp_event_runCheck()) {
        if (i_this->mID == 0) {
            i_this->mTimers[1] = 350.0f + cM_rndF(50.0f);
        } else {
            i_this->mTimers[1] = 250.0f + cM_rndF(50.0f);
        }
    }

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_BH_WAIT, 10.0f, 2, 1.0f);
        i_this->mMode = 1;
        if (a_this->health <= 1) {
            i_this->mTimers[1] = ((f32)l_HIO.attack_freq_a / 2) + cM_rndF((f32)l_HIO.attack_freq_a / 2);
        } else {
            i_this->mTimers[1] = l_HIO.attack_freq_a + cM_rndF(l_HIO.attack_freq_a);
        }
        break;
    case 1:
        if (i_this->mTimers[0] == 0) {
            i_this->mTimers[0] = 10.0f + cM_rndF(30.0f);
            i_this->field_0x674.x = i_this->mBasePos.x + cM_rndFX(300.0f);
            i_this->field_0x674.z = i_this->mBasePos.z + cM_rndFX(300.0f);
            i_this->field_0x674.y = BREG_F(3) + (i_this->mBasePos.y + (550.0f * l_HIO.model_size) + cM_rndFX(200.0f));
            i_this->field_0x690 = 0.0f;

            if (i_this->mTimers[1] == 0 && bq_p->field_0x6fe == 0) {
                fopAc_ac_c* const player = dComIfGp_getPlayer(0);
                if ((i_this->mBasePos - player->current.pos).abs() < 2800.0f) {
                    a_this->speedF = 0.0f;
                    i_this->mAction = ACTION_ATTACK_1;
                    i_this->mMode = 0;

                    if (bq_p->field_0x6fa == 0 && bh[1 - i_this->mID]->mAction == ACTION_WAIT && bh[1 - i_this->mID]->mTimers[3] == 0) {
                        i_this->field_0x6a0 = 1;
                        bh[1 - i_this->mID]->mAction = ACTION_ATTACK_1;
                        bh[1 - i_this->mID]->mMode = 0;
                        bh[1 - i_this->mID]->field_0x6a0 = 1;
                        bq_p->field_0x6fa = 2.0f + cM_rndF(2.99f);
                    } else {
                        bq_p->field_0x6fe = 200.0f + cM_rndF(150.0f);
                        if (bq_p->field_0x6fa != 0) {
                            bq_p->field_0x6fa--;
                        }
                    }
                }
            }

            i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_WAIT, -1);
        }

        sp34.x = temp_f31 * cM_ssin(i_this->field_0x668 * (TREG_S(0) + 1900));
        sp34.y = temp_f31 * cM_ssin(i_this->field_0x668 * (TREG_S(1) + 2200));
        sp34.z = temp_f31 * cM_scos(i_this->field_0x668 * (TREG_S(2) + 2000));

        cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x674.x + sp34.x, 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x674.z + sp34.z, 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x674.y + sp34.y, 0.1f, 50.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.05f);

        if (i_this->field_0x674.y - a_this->current.pos.y > 400.0f) {
            i_this->field_0x686 = -0x4000;
        }

        if (a_this->current.pos.y > a_this->home.pos.y) {
            if (a_this->old.pos.y <= a_this->home.pos.y) {
                if (i_this->field_0xf20 != 0) {
                    i_this->field_0xf20 = 0;
                    i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_AWAKE, -1);
                } else {
                    i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_APPEAR, -1);
                }
                i_this->field_0xf21 = 1;
            }
        }
        break;
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, i_this->field_0x684, 8, 0x400);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, i_this->field_0x686, 8, 0x400);
    
    cXyz pos(i_this->mBasePos);
    pos.y = -23.0f;
    fopAcM_effHamonSet(&i_this->field_0xf24, &pos, 3.5f + KREG_F(12), 0.05f);
}

static void b_bh_attack_1(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz sp2C;
    cXyz sp20;
    f32 target_speed = 0.0f;
    f32 var_f30 = 100.0f + YREG_F(2);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_BH_ATTACKWAIT, 5.0f, 2, 1.0f);
        i_this->mMode = 2;
        i_this->mTimers[0] = NREG_S(0) + 35;

        if (i_this->field_0x6a0 != 0) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_SP_ATTACK, -1);
        } else {
            i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_ATTACK, -1);
        }
        break;
    case 2:
        if (i_this->mTimers[0] == NREG_S(4) + 8) {
            anm_init(i_this, BCK_BH_ATTACK, 3.0f, 0, 1.0f);
        }

        a_this->current.angle.y = i_this->field_0x684;

        if (i_this->field_0x6a0 != 0) {
            if (i_this->mID == 0) {
                a_this->current.angle.y += 0x600;
            } else {
                a_this->current.angle.y -= 0x600;
            }
        }

        a_this->current.angle.x = i_this->field_0x686;

        sp2C.x = 0.0f;
        sp2C.y = (550.0f * l_HIO.model_size) + NREG_F(0);
        sp2C.z = -300.0f + NREG_F(1);
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        MtxPosition(&sp2C, &sp20);
        i_this->field_0x674 = i_this->mBasePos + sp20;

        cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x674.x, 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x674.z, 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x674.y, 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.1f);

        if (i_this->mTimers[0] == 0) {
            i_this->mMode = 3;
            i_this->mTimers[0] = 25;
            i_this->field_0x6a0 = 0;
        }
        break;
    case 3:
        if (i_this->mTimers[0] == 22) {
            i_this->mSound.startCreatureSound(Z2SE_EN_BH_ATTACK, 0, -1);
        }

        target_speed = 200.0f + YREG_F(0);
        var_f30 = 100.0f + YREG_F(1);
        if (i_this->mTimers[0] == 0 || i_this->field_0x688 < (500.0f + YREG_F(6)) || i_this->field_0x68c > (1700.0f + YREG_F(5))) {
            i_this->mTimers[0] = YREG_S(2) + 20;
            i_this->mMode = 4;
        }
        break;
    case 4:
        if (i_this->mTimers[0] == 0) {
            i_this->mAction = ACTION_WAIT;
            i_this->mMode = 0;
            i_this->mTimers[0] = 60.0f + cM_rndF(50.0f);
            i_this->field_0x674.x = i_this->mBasePos.x + cM_rndFX(300.0f);
            i_this->field_0x674.z = i_this->mBasePos.z + cM_rndFX(300.0f);
            i_this->field_0x674.y = BREG_F(3) + (i_this->mBasePos.y + (550.0f * l_HIO.model_size) + cM_rndFX(200.0f));
            i_this->field_0x690 = 0.0f;
        }
        break;
    }

    if (i_this->mAnm == BCK_BH_ATTACK && i_this->mpModelMorf->isStop()) {
        anm_init(i_this, BCK_BH_WAIT02, 5.0f, 2, 1.0f);
    }

    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, var_f30);
    if (a_this->speedF >= 20.0f) {
        i_this->field_0xce0 = 1;
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 8, 0x800);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 8, 0x400);
}

static void b_bh_bombeat(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(a_this->parentActorID);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_BH_EAT, 3.0f, 0, 1.0f);
        i_this->mMode = 1;
        i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_EAT, -1);

        i_this->health--;
        if (i_this->health <= 0 && bh[1 - i_this->mID]->mAction == ACTION_DOWN && bh[1 - i_this->mID]->health == 0) {
            bh[1 - i_this->mID]->mTimers[0] = 0;
            bh[1 - i_this->mID]->health = 0;
            bq_p->mDemoMode = 1;
            bq_p->field_0x123c = i_this->mID;
        }
        break;
    case 1:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, BCK_BH_EATWAIT, 1.0f, 2, 1.0f);
            i_this->mMode = 2;
            i_this->mTimers[0] = 40;
        }
        break;
    case 2:
        if (i_this->mTimers[0] == 0) {
            i_this->mMode = 3;
            anm_init(i_this, BCK_BH_DAMAGE, 1.0f, 0, 1.0f);
            i_this->field_0x90c = 2;
            i_this->field_0xf22 = 1;

            i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_DAMAGE, -1);
            i_this->mTimers[0] = 20;
        }
        break;
    case 3:
        cLib_addCalc2(&i_this->field_0x914, 0.3f + JREG_F(4), 1.0f, 0.05f);

        if (i_this->mTimers[0] == 0) {
            cLib_addCalcAngleS2(&a_this->shape_angle.x, -0x4000, 4, 0x200);
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = ACTION_DOWN;
            i_this->mMode = 0;
        }
        break;
    }
}

static s8 b_bh_down(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(i_this->parentActorID);

    cXyz sp2C;
    s8 sp8 = 0;
    cXyz sp20 = i_this->field_0x6b0 - a_this->current.pos;
    sp20.y = 0.0f;
    f32 temp_f31;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_BH_ROUT, 1.0f, 2, 1.0f);
        i_this->field_0x690 = 0.0f;
        i_this->mMode = 1;
        i_this->mTimers[0] = 200;
        break;
    case 1:
        if (sp20.abs() >600.0f) {
            sp8 = 1;
        }

        if (i_this->mTimers[0] > 100) {
            i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_BH_V_ROUT, -1);
        }

        temp_f31 = 100.0f + JREG_F(0);
        sp2C.x = temp_f31 * cM_ssin(i_this->field_0x668 * (TREG_S(0) + 900));
        sp2C.z = temp_f31 * cM_scos(i_this->field_0x668 * (TREG_S(2) + 1000));

        cLib_addCalc2(&a_this->current.pos.x, i_this->mBasePos.x + sp2C.x, 0.1f, 50.0f * i_this->field_0x690);
        cLib_addCalc2(&a_this->current.pos.z, i_this->mBasePos.z + sp2C.z, 0.1f, 50.0f * i_this->field_0x690);
        cLib_addCalc2(&a_this->current.pos.y, 700.0f + i_this->mBasePos.y + JREG_F(1), 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->mBasePos.y, (a_this->home.pos.y - 1600.0f) + JREG_F(5), 1.0f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.01f);

        if (i_this->mBasePos.y <= 50.0f + (a_this->home.pos.y - 1600.0f) + JREG_F(5)) {
            i_this->mMode = 2;
            i_this->mTimers[0] = l_HIO.down_revive_time;
        }

        if (a_this->current.pos.y < a_this->home.pos.y) {
            if (a_this->old.pos.y >= a_this->home.pos.y) {
                i_this->mAnmSe = Z2SE_EN_BH_SPLASH_DOWN;
                if (i_this->mID == 0) {
                    i_this->mAnmSeTimer = 1;
                } else {
                    i_this->mAnmSeTimer = 3;
                }
            }
        }
        break;
    case 2:
        if (a_this->health <= 0) {
            i_this->mMode = 10;
        } else if (i_this->mTimers[0] == 0) {
            i_this->mAction = ACTION_WAIT;
            i_this->mMode = 0;
            i_this->mBasePos = a_this->home.pos;
            a_this->current.pos.x = i_this->mBasePos.x;
            a_this->current.pos.z = i_this->mBasePos.z;
            i_this->field_0xf20 = 1;
            i_this->mTimers[3] = 60;
        }
        break;
    case 10:
        if (bq_p->field_0x6f9 & 4) {
            i_this->mBasePos = a_this->home.pos;
            a_this->current.pos.x = i_this->mBasePos.x;
            a_this->current.pos.z = i_this->mBasePos.z;
            a_this->health = 3;

            anm_init(i_this, BCK_BH_WAIT, 10.0f, 2, 1.0f);
            i_this->mAction = ACTION_B_WAIT;
            i_this->mMode = 1;

            if (i_this->mID == 0) {
                i_this->mTimers[1] = 300;
            } else {
                i_this->mTimers[1] = 400;
            }
        }
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.x, -0x4000, 4, 0x200);
    cLib_addCalc2(&i_this->field_0x914, 0.3f + JREG_F(4), 1.0f, 0.05f);

    if (a_this->current.pos.y > a_this->home.pos.y - 100.0f) {
        cXyz pos(i_this->mBasePos);
        pos.y = -23.0f;
        fopAcM_effHamonSet(&i_this->field_0xf24, &pos, 3.5f + KREG_F(12), 0.05f);
    }
    return sp8;
}

static void b_bh_b_wait(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(a_this->parentActorID);

    cXyz sp40;
    cXyz sp34;
    cXyz sp28;
    f32 temp_f31 = 30.0f + TREG_F(12);

    if (bq_p->field_0x6fb != 0) {
        if (i_this->mID == 0) {
            i_this->mTimers[1] = 100.0f + cM_rndF(50.0f);
        } else {
            i_this->mTimers[1] = 50.0f + cM_rndF(50.0f);
        }
    }

    if (dComIfGp_event_runCheck()) {
        if (bq_p->field_0x6ec == 0) {
            if (i_this->mID == 0) {
                i_this->mTimers[1] = 350.0f + cM_rndF(50.0f);
            } else {
                i_this->mTimers[1] = 250.0f + cM_rndF(50.0f);
            }
        } else if (i_this->mID == 0) {
            i_this->mTimers[1] = 150.0f + cM_rndF(50.0f);
        } else {
            i_this->mTimers[1] = 70.0f + cM_rndF(50.0f);
        }
    }

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_BH_WAIT, 10.0f, 2, 1.0f);
        i_this->mMode = 1;
        i_this->mTimers[1] = l_HIO.attack_freq_b + cM_rndF(l_HIO.attack_freq_b);
        break;
    case 1:
        if (i_this->mTimers[0] == 0) {
            i_this->mTimers[0] = 10.0f + cM_rndF(30.0f);
            i_this->field_0x674.x = i_this->field_0x6b0.x + cM_rndFX(300.0f);
            i_this->field_0x674.z = i_this->field_0x6b0.z + cM_rndFX(300.0f);
            i_this->field_0x674.y = BREG_F(3) + (i_this->field_0x6b0.y + (550.0f * l_HIO.model_size) + cM_rndFX(200.0f));
            i_this->field_0x690 = 0.0f;

            if (i_this->mTimers[1] == 0 && bq_p->field_0x6fe == 0) {
                fopAc_ac_c* const player = dComIfGp_getPlayer(0);
                if ((i_this->field_0x6b0 - player->current.pos).abs() < 2800.0f) {
                    a_this->speedF = 0.0f;
                    i_this->mAction = ACTION_B_ATTACK_1;
                    i_this->mMode = 0;
                    bq_p->field_0x6fe = 100.0f + cM_rndF(100.0f);
                }
            }

            i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_WAIT, -1);
        }

        sp28.x = temp_f31 * cM_ssin(i_this->field_0x668 * (TREG_S(0) + 1900));
        sp28.y = temp_f31 * cM_ssin(i_this->field_0x668 * (TREG_S(1) + 2200));
        sp28.z = temp_f31 * cM_scos(i_this->field_0x668 * (TREG_S(2) + 2000));
        cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x674.x + sp28.x, 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x674.z + sp28.z, 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x674.y + sp28.y, 0.1f, 50.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.05f);

        if (i_this->field_0x674.y - a_this->current.pos.y > 400.0f) {
            i_this->field_0x686 = -0x4000;
        }

        if (a_this->current.pos.y > a_this->home.pos.y) {
            if (a_this->old.pos.y <= a_this->home.pos.y) {
                if (i_this->field_0xf20 != 0) {
                    i_this->field_0xf20 = 0;
                    i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_AWAKE, -1);
                } else {
                    i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_APPEAR, -1);
                }
                i_this->field_0xf21 = 1;
            }
        }
        break;
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, i_this->field_0x684, 8, 0x400);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, i_this->field_0x686, 8, 0x400);
}

static void b_bh_b_attack_1(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(a_this->parentActorID);

    cXyz sp28;
    cXyz sp1C;
    f32 target_speed = 0.0f;
    f32 speed_step = 100.0f + YREG_F(2);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_BH_ATTACKWAIT, 5.0f, 2, 1.0f);
        i_this->mMode = 2;
        i_this->mTimers[0] = NREG_S(0) + 35;
        i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_ATTACK, -1);
        break;
    case 2:
        if (i_this->mTimers[0] == NREG_S(4) + 8) {
            anm_init(i_this, BCK_BH_ATTACK, 3.0f, 0, 1.0f);
        }

        a_this->current.angle.y = i_this->field_0x684;
        a_this->current.angle.x = i_this->field_0x686;

        sp28.x = 0.0f;
        sp28.y = (550.0f * l_HIO.model_size) + NREG_F(0);
        sp28.z = -300.0f + NREG_F(1);
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        MtxPosition(&sp28, &sp1C);
        i_this->field_0x674 = i_this->field_0x6b0 + sp1C;

        cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x674.x, 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x674.z, 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x674.y, 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.1f);

        if (i_this->mTimers[0] == 0) {
            i_this->mMode = 3;
            i_this->mTimers[0] = 25;
        }
        break;
    case 3:
        if (i_this->mTimers[0] == 22) {
            i_this->mSound.startCreatureSound(Z2SE_EN_BH_ATTACK, 0, -1);
        }

        target_speed = 200.0f + YREG_F(0);
        speed_step = 100.0f + YREG_F(1);

        if (i_this->mTimers[0] == 0 || i_this->field_0x688 < (500.0f + YREG_F(6)) || i_this->field_0x68c > (1700.0f + YREG_F(5))) {
            i_this->mTimers[0] = YREG_S(3) + 10;
            i_this->mMode = 4;
        }
        break;
    case 4:
        if (i_this->mTimers[0] == 0) {
            i_this->mAction = ACTION_B_WAIT;
            i_this->mMode = 0;
            i_this->mTimers[0] = 60.0f + cM_rndF(50.0f);
            i_this->field_0x674.x = i_this->field_0x6b0.x + cM_rndFX(300.0f);
            i_this->field_0x674.z = i_this->field_0x6b0.z + cM_rndFX(300.0f);
            i_this->field_0x674.y = BREG_F(3) + (i_this->field_0x6b0.y + (550.0f * l_HIO.model_size) + cM_rndFX(200.0f));
            i_this->field_0x690 = 0.0f;

            if (bq_p->field_0x6ec != 0) {
                bq_p->field_0x6ec--;
            }
        }
        break;
    }

    if (i_this->mAnm == BCK_BH_ATTACK && i_this->mpModelMorf->isStop()) {
        anm_init(i_this, BCK_BH_WAIT02, 5.0f, 2, 1.0f);
    }

    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, speed_step);
    if (a_this->speedF >= 20.0f) {
        i_this->field_0xce0 = 1;
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 8, 0x800);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 8, 0x400);
}

static void b_bh_b_bombeat(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_BH_EAT, 3.0f, 0, 1.0f);
        i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_EAT, -1);
        i_this->mMode = 1;
        break;
    case 1:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, BCK_BH_EATWAIT, 1.0f, 2, 1.0f);
            i_this->mMode = 2;
            i_this->mTimers[0] = 60;
        }
        break;
    case 2:
        if (i_this->mTimers[0] == 0) {
            i_this->mMode = 3;
            anm_init(i_this, BCK_BH_DAMAGE, 1.0f, 0, 1.0f);
            i_this->field_0x90c = 2;
            i_this->field_0xf22 = 1;
            i_this->mTimers[0] = 20;
        }
        break;
    case 3:
        cLib_addCalc2(&i_this->field_0x914, 0.3f + JREG_F(4), 1.0f, 0.05f);

        if (i_this->mTimers[0] == 0) {
            cLib_addCalcAngleS2(&a_this->shape_angle.x, -0x4000, 4, 0x200);
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = ACTION_B_DOWN;
            i_this->mMode = 0;
            a_this->health--;
        }
        break;
    }
}

static s8 b_bh_b_down(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(i_this->parentActorID);

    cXyz sp18;
    s8 var_r27 = 0;
    cXyz spC = i_this->field_0x6b0 - i_this->current.pos;
    spC.y = 0.0f;
    f32 temp_f31;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_BH_ROUT, 1.0f, 2, 1.0f);
        i_this->field_0x690 = 0.0f;
        i_this->mMode = 1;
        i_this->mTimers[0] = 200;
        break;
    case 1:
        if (spC.abs() > 600.0f) {
            var_r27 = 1;
        }

        if (i_this->mTimers[0] > 100) {
            i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_BH_V_ROUT, -1);
        }

        temp_f31 = 100.0f + JREG_F(0);
        sp18.x = temp_f31 * cM_ssin(i_this->field_0x668 * (TREG_S(0) + 900));
        sp18.z = temp_f31 * cM_scos(i_this->field_0x668 * (TREG_S(2) + 1000));

        cLib_addCalc2(&i_this->current.pos.x, i_this->field_0x6b0.x + sp18.x, 0.1f, 50.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->current.pos.z, i_this->field_0x6b0.z + sp18.z, 0.1f, 50.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->current.pos.y, (i_this->field_0x6b0.y - 1000.0f) + JREG_F(17), 0.1f, 30.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.1f);

        if (i_this->current.pos.y <= 50.0f + (i_this->field_0x6b0.y - 1000.0f) + JREG_F(17)) {
            i_this->mMode = 2;
        }

        if (i_this->current.pos.y < i_this->home.pos.y) {
            if (i_this->old.pos.y >= i_this->home.pos.y) {
                i_this->mAnmSe = Z2SE_EN_BH_SPLASH_DOWN;
                if (i_this->mID == 0) {
                    i_this->mAnmSeTimer = 1;
                } else {
                    i_this->mAnmSeTimer = 3;
                }
            }
        }
        break;
    case 2:
        if (bq_p->field_0x6fc != 0) {
            if (i_this->health > 0) {
                i_this->mAction = ACTION_B_WAIT;
                i_this->mMode = 0;
                i_this->field_0xf20 = 1;
            } else {
                i_this->mMode = 3;
            }
        }
    case 3:
        break;
    }

    cLib_addCalcAngleS2(&i_this->shape_angle.x, -0x4000, 4, 0x200);
    return var_r27;
}

static void b_bh_start(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    s16 sp8 = 0;
    cXyz sp18;

    f32 temp_f31 = 50.0f + TREG_F(12);
    b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(a_this->parentActorID);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_BH_WAIT, 10.0f, 2, 1.0f);
        if (i_this->mID != 0) {
            i_this->mpModelMorf->setFrame(10.0f);
        }
        i_this->mMode = 1;
        break;
    case 1:
        i_this->mMode = 2;
        break;
    case 2:
        if (bq_p->field_0x6f9 != 0) {
            bq_p->field_0x6f9--;
            dComIfGp_particle_set(0x82C9, &a_this->home.pos, NULL, NULL);
            dComIfGp_particle_set(0x82CA, &a_this->home.pos, NULL, NULL);
            i_this->mMode = 3;
            mDoAud_seStart(Z2SE_EN_BH_ABUKU, &a_this->home.pos, 0, 0);
        }
        break;
    case 3:
        if (bq_p->field_0x6f9 != 0) {
            bq_p->field_0x6f9--;
            i_this->mMode = 4;
            i_this->mBasePos = a_this->home.pos;

            if (i_this->mID == 0) {
                a_this->current.pos.y = a_this->home.pos.y - 500.0f;
            } else {
                a_this->current.pos.y = a_this->home.pos.y - 800.0f;
            }

            i_this->field_0x674.x = i_this->mBasePos.x;
            i_this->field_0x674.z = i_this->mBasePos.z;
            i_this->field_0x674.y = 200.0f + i_this->mBasePos.y + (550.0f * l_HIO.model_size);
            i_this->field_0x690 = 0.0f;
            i_this->mBasePos.y -= 1000.0f;
        }
        break;
    case 4:
    case 5:
        sp18.x = temp_f31 * cM_ssin(i_this->field_0x668 * (TREG_S(0) + 1900));
        sp18.z = temp_f31 * cM_scos(i_this->field_0x668 * (TREG_S(2) + 2000));

        cLib_addCalc2(&i_this->current.pos.x, i_this->field_0x674.x + sp18.x, 0.1f, 50.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->current.pos.z, i_this->field_0x674.z + sp18.z, 0.1f, 50.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->current.pos.y, i_this->field_0x674.y, 0.1f, 50.0f * i_this->field_0x690);
        cLib_addCalc2(&i_this->mBasePos.y, a_this->home.pos.y, 0.1f, 10.0f * i_this->field_0x690);

        if (i_this->field_0x674.y - a_this->current.pos.y > 150.0f) {
            i_this->field_0x686 = -0x4000;
            sp8 = 12000.0f * cM_ssin(bq_p->mDemoModeTimer * (AREG_S(0) + 1000));
        } else if (i_this->mID == 0 && !(i_this->field_0x668 & 7) && cM_rndF(1.0f) < 0.5f) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_WAIT, -1);
        }

        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.05f);

        if (bq_p->field_0x6f9 != 0) {
            bq_p->field_0x6f9--;
            if (i_this->mMode == 4) {
                i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_APPEAR, -1);
                i_this->field_0xf21 = 1;
                i_this->mMode = 5;
            } else {
                i_this->mAction = ACTION_WAIT;
                i_this->mMode = 0;
            }
        }
        break;
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, i_this->field_0x684, 8, 0x400);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, i_this->field_0x686, 8, 0x100);
    cLib_addCalcAngleS2(&a_this->shape_angle.z, sp8, 2, 0x200);

    if (i_this->mMode >= 2 && bq_p->mDemoMode == 0 && mDoCPd_c::getTrigStart(PAD_1)) {
        i_this->mAction = ACTION_WAIT;
        i_this->mMode = 0;
        i_this->mBasePos = a_this->home.pos;
        a_this->current.pos.y = a_this->home.pos.y - 500.0f;
        a_this->shape_angle.z = 0;
    }
}

static void kuki_control1(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz sp40;
    cXyz sp34;
    cXyz sp28;

    int i;
    cXyz* temp_r31 = i_this->field_0x6bc;
    *temp_r31 = i_this->mBasePos;

    cXyz sp1C;
    sp40.x = 0.0f;
    sp40.y = 0.0f;
    sp40.z = i_this->field_0x910;

    temp_r31++;

    f32 temp_f28 = 60.0f + TREG_F(2);
    sp28.x = 0.0f;
    sp28.z = 0.0f;
    sp28.y = -(200.0f + TREG_F(0));

    f32 temp_f29 = i_this->field_0x918;

    for (i = 1; i < 18; i++, temp_r31++) {
        f32 temp_f26 = temp_f28 * cM_ssin((i_this->field_0x668 * (TREG_S(0) + 2000)) + (i * (TREG_S(1) + 7000)));
        f32 temp_f25 = temp_f28 * cM_scos((i_this->field_0x668 * (TREG_S(2) + 2300)) + (i * (TREG_S(3) + 5000)));

        static s16 pow_xa[] = {
            0x0000, 0x0000, 0x1000, 0x1800, 0x2000, 0x2800,
            0x3000, 0x3800, 0x4000, 0x3800, 0x3000, 0x2800,
            0x2000, 0x1800, 0x1000, 0x0800, 0x0000,
        };

        cMtx_YrotS(*calc_mtx, (s16)a_this->shape_angle.y);
        cMtx_ZrotM(*calc_mtx, (s16)i_this->field_0xa26.z);
        cMtx_XrotM(*calc_mtx, (s16)pow_xa[i - 1]);
        MtxScale(temp_f29, temp_f29, temp_f29, 1);
        MtxPosition(&sp28, &sp1C);

        f32 temp_f31 = temp_f26 + (sp1C.x + (temp_r31[0].x - temp_r31[-1].x));
        f32 temp_f27 = (temp_r31[0].y - temp_r31[-1].y) - sp1C.y;
        f32 temp_f30 = temp_f25 + (sp1C.z + (temp_r31[0].z - temp_r31[-1].z));

        s16 spA, sp8;
        sp8 = (s16)cM_atan2s(temp_f31, temp_f30);
        spA = -cM_atan2s(temp_f27, JMAFastSqrt((temp_f31 * temp_f31) + (temp_f30 * temp_f30)));

        cMtx_YrotS(*calc_mtx, sp8);
        cMtx_XrotM(*calc_mtx, spA);
        MtxPosition(&sp40, &sp34);
        *temp_r31 = *(temp_r31 - 1) + sp34;
    }
}

static void kuki_control2(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz sp28;
    cXyz sp1C;

    int i;
    s16 temp_r27, temp_r26;

    cXyz* temp_r31 = &i_this->field_0x6bc[17];
    *temp_r31 = a_this->current.pos;

    sp28.x = 0.0f;
    sp28.y = 0.0f;
    sp28.z = i_this->field_0x910;

    i = 16;
    temp_r31--;
    csXyz* var_r30 = &i_this->field_0x794[16];

    for (; i >= 0; i--, temp_r31--, var_r30--) {
        f32 temp_f29 = temp_r31[0].x - temp_r31[1].x;
        f32 temp_f31 = temp_r31[0].y - temp_r31[1].y;
        f32 temp_f30 = temp_r31[0].z - temp_r31[1].z;
        temp_r27 = -cM_atan2s(temp_f31, temp_f30);
        temp_r26 = (s16)cM_atan2s(temp_f29, JMAFastSqrt((temp_f31 * temp_f31) + (temp_f30 * temp_f30)));

        cMtx_XrotS(*calc_mtx, temp_r27);
        cMtx_YrotM(*calc_mtx, temp_r26);
        MtxPosition(&sp28, &sp1C);
        *temp_r31 = *(temp_r31 + 1) + sp1C;

        var_r30->x = temp_r27;
        var_r30->y = temp_r26;
    }
}

static void kuki_control3(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz sp20;

    sp20 = i_this->field_0x6bc[0] - i_this->mBasePos;
    cXyz* var_r28 = &i_this->field_0x6bc[0];

    for (int i = 0; i < 18; i++, var_r28++) {
        *var_r28 -= sp20 * ((18 - i) / 17.0f);
    }

    i_this->field_0xa20 = sp20.abs();
}

static void* s_b_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && dBomb_c::checkBombActor((fopAc_ac_c*)i_actor) && !((dBomb_c*)i_actor)->checkStateExplode() && !dComIfGp_event_runCheck()) {
        cXyz vec_bomb_to_bh = ((fopAc_ac_c*)i_actor)->current.pos - ((fopAc_ac_c*)i_data)->eyePos;
        if (vec_bomb_to_bh.abs() < 200.0f) {
            fopAcM_delete((fopAc_ac_c*)i_actor);
            return i_actor;
        }
    }

    return NULL;
}

static void damage_check(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    if ((i_this->mAction == ACTION_ATTACK_1 || i_this->mAction == ACTION_B_ATTACK_1) && i_this->mCcSph.ChkAtShieldHit()) {
        if (i_this->mAction == ACTION_B_ATTACK_1) {
            b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(a_this->parentActorID);
            if (bq_p->field_0x6ec != 0) {
                bq_p->field_0x6ec--;
            }
        }

        i_this->field_0x69e = 6;
        i_this->mAction = ACTION_WAIT;
        i_this->mMode = 1;
        i_this->mTimers[1] = l_HIO.attack_freq_a + cM_rndF(l_HIO.attack_freq_a);
        i_this->mTimers[0] = 0;
        anm_init(i_this, BCK_BH_NODAMAGE, 3.0f, 0, 1.0f);
        i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_NODAMAGE, -1);
        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
    } else {
        cXyz sp1C = i_this->field_0x6b0 - a_this->current.pos;
        sp1C.y = 0.0f;

        if (i_this->field_0xa24 != 0) {
            i_this->mCcSph.OffCoSetBit();

            if (fpcM_Search(s_b_sub, i_this) || (DEBUG && mDoCPd_c::getTrigA(PAD_3))) {
                if (i_this->field_0xa25 != 0) {
                    i_this->mAction = ACTION_B_BOMB_EAT;
                    dComIfGs_onOneZoneSwitch(14, -1);
                } else {
#if DEBUG
                    if (mDoCPd_c::getTrigA(PAD_3)) {
                        b_bq_class* var_r25 = (b_bq_class*)fopAcM_SearchByID(a_this->parentActorID);
                        var_r25->field_0x6fd = 1;
                        if (i_this->mID == 0) {
                            a_this->health = 0;
                            bh[1 -  i_this->mID]->mAction = ACTION_DOWN;
                            bh[1 -  i_this->mID]->mMode = 10;
                            bh[1 -  i_this->mID]->health = 0;
                            bh[1 -  i_this->mID]->current.pos.y = bh[1 -  i_this->mID]->home.pos.y - 700.0f;
                        }
                    }
#endif
                    i_this->mAction = ACTION_BOMB_EAT;
                    b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(a_this->parentActorID);
                    if (bq_p->field_0x6fd == 0) {
                        bq_p->mDemoMode = 20;
                        bq_p->field_0x123c = i_this->mID;
                        bq_p->field_0x6fd = 1;
                    }
                }

                i_this->mMode = 0;
                i_this->field_0x69e = 10;
                return;
            }
        } else {
            i_this->mCcSph.OnCoSetBit();
        }

        if (i_this->field_0x69e == 0 && i_this->mCcSph.ChkTgHit()) {
            i_this->field_0x69e = 6;
            i_this->mAtInfo.mpCollider = i_this->mCcSph.GetTgHitObj();
            def_se_set(&i_this->mSound, i_this->mAtInfo.mpCollider, 0x2D, NULL);

            if (i_this->mAction != ACTION_BOMB_EAT && i_this->mAction != ACTION_B_BOMB_EAT) {
                anm_init(i_this, BCK_BH_NODAMAGE, 3.0f, 0, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_BH_V_NODAMAGE, -1);
            }
        }

        if (i_this->mAnm == BCK_BH_NODAMAGE && i_this->mpModelMorf->isStop()) {
            anm_init(i_this, BCK_BH_WAIT, 10.0f, 2, 1.0f);
        }
    }
}

static void action(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz spA0;
    cXyz sp94;

    spA0 = player->current.pos - a_this->current.pos;
    spA0.y += 100.0f;

    i_this->field_0x684 = (s16)cM_atan2s(spA0.x, spA0.z);
    i_this->field_0x686 = -cM_atan2s(spA0.y, JMAFastSqrt((spA0.x * spA0.x) + (spA0.z * spA0.z)));
    i_this->field_0x688 = (i_this->current.pos - player->eyePos).abs();
    i_this->field_0x68c = (i_this->mBasePos - i_this->current.pos).abs();

    damage_check(i_this);

    i_this->field_0xa24 = 0;

    s8 spC = 0;
    s8 spB = 0;
    s8 spA = 0;
    s8 sp9 = 1;
    s8 sp8 = 0;

    spA0 = i_this->mBasePos - a_this->current.pos;
    spA0.y = 0.0f;

    if (spA0.abs() < 700.0f) {
        sp9 = 0;
    }

    switch (i_this->mAction) {
    case ACTION_WAIT:
        b_bh_wait(i_this);
        spC = 1;
        spB = 1;
        i_this->field_0xa24 = 1;
        break;
    case ACTION_ATTACK_1:
        b_bh_attack_1(i_this);
        spC = 1;
        spB = 1;
        spA = 1;
        i_this->field_0xa24 = 1;
        break;
    case ACTION_BOMB_EAT:
        b_bh_bombeat(i_this);
        spB = 1;
        break;
    case ACTION_DOWN:
        sp9 = b_bh_down(i_this);
        spB = 1;
        break;
    case ACTION_B_WAIT:
        b_bh_b_wait(i_this);
        spC = 1;
        spB = 1;
        sp8 = 1;
        i_this->field_0xa24 = 1;
        break;
    case ACTION_B_ATTACK_1:
        b_bh_b_attack_1(i_this);
        spC = 1;
        spB = 1;
        spA = 1;
        sp8 = 1;
        i_this->field_0xa24 = 1;
        break;
    case ACTION_B_BOMB_EAT:
        b_bh_b_bombeat(i_this);
        spB = 1;
        break;
    case ACTION_B_DOWN:
        sp9 = b_bh_b_down(i_this);
        spB = 1;
        break;
    case ACTION_START:
        b_bh_start(i_this);
        break;
    }

    if (sp8 != 0) {
        b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(a_this->parentActorID);
        if (bq_p->field_0x6fb == 1) {
            i_this->mAction = ACTION_B_DOWN;
            i_this->mMode = 0;
        }
    }

    if (spA != 0) {
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
        spA0.x = 0.0f;
        spA0.y = 0.0f;
        spA0.z = a_this->speedF;
        MtxPosition(&spA0, &a_this->speed);
        i_this->current.pos += a_this->speed;

        cXyz* cc_move_p = i_this->mColliderStts.GetCCMoveP();
        if (cc_move_p != NULL) {
            a_this->current.pos.x += cc_move_p->x;
            a_this->current.pos.y += cc_move_p->y;
            a_this->current.pos.z += cc_move_p->z;
        }
    }

    if (sp9 != 0) {
        f32 temp_f31 = -150.0f + ZREG_F(3);

        a_this->current.pos.y += temp_f31;
        a_this->old.pos.y += temp_f31;

        i_this->mAcch.CrrPos(dComIfG_Bgsp());

        a_this->current.pos.y -= temp_f31;
        a_this->old.pos.y -= temp_f31;
    }

    if (spC != 0) {
        fopAcM_OnStatus(a_this, 0);
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    }

    spA0 = i_this->current.pos - i_this->mBasePos;
    i_this->field_0x910 = i_this->field_0x914 * ((0.2f + BREG_F(0)) * spA0.abs());
    if (i_this->field_0x910 > (50.0f + BREG_F(1)) * l_HIO.model_size) {
        i_this->field_0x910 = (50.0f + BREG_F(1)) * l_HIO.model_size;
    }

    cLib_addCalc2(&i_this->field_0x914, 1.0f, 1.0f, 0.01f);

    kuki_control1(i_this);
    kuki_control2(i_this);
    kuki_control3(i_this);

    cLib_addCalc2(&i_this->field_0x918, 1.0f, 1.0f, 0.05f);

    if (i_this->field_0xf21 != 0) {
        i_this->field_0xf21 = 0;
        i_this->mAnmSe = Z2SE_EN_BH_SPLASH_UP;

        if (i_this->mID == 0) {
            i_this->mAnmSeTimer = 1;
        } else {
            i_this->mAnmSeTimer = 3;
        }
    
        cXyz pos(a_this->current.pos);
        pos.y = 0.0f;
        cXyz size(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);

        for (int i = 0; i < 5; i++) {
            static u16 eno[] = {0x82CD, 0x82CE, 0x82F3, 0x82CB, 0x82CC};
            dComIfGp_particle_set(eno[i], &pos, NULL, &size);
        }
    }

    if (i_this->field_0xf22 != 0) {
        i_this->field_0xf22 = 0;

        cXyz pos(a_this->current.pos);
        cXyz size(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);

        for (int i = 0; i < 4; i++) {
            static u16 eno[] = {0x82D9, 0x82DA, 0x82DB, 0x82DC};
            dComIfGp_particle_set(eno[i], &pos, &a_this->shape_angle, &size);
        }

        i_this->mSound.startCreatureSound(Z2SE_OBJ_BOMB_EXPLODE, 0, -1);
    }
}

static void anm_se_set(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    f32 particle_target_size = 0.0f;
    if (i_this->mAnm == BCK_BH_WAIT && a_this->current.pos.y > 50.0f) {
        particle_target_size = 1.0f;
    }

    cLib_addCalc2(&i_this->mMouthMizuParticleSize, particle_target_size, 1.0f, 0.05f);

    for (int i = 0; i < 5; i++) {
        static u16 g_y_i[] = {0x82F5, 0x82F6, 0x82F7, 0x82F8, 0x82F4};
        i_this->mMouthMizuEmitterIDs[i] = dComIfGp_particle_set(i_this->mMouthMizuEmitterIDs[i], g_y_i[i], &a_this->current.pos, NULL, NULL);

        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mMouthMizuEmitterIDs[i]);
        if (emitter != NULL) {
            static int g_y_ji[] = {12, 13, 15, 16, 12};
            
            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(g_y_ji[i]), *calc_mtx);
            MtxScale(i_this->mMouthMizuParticleSize, i_this->mMouthMizuParticleSize, i_this->mMouthMizuParticleSize, 1);
            emitter->setGlobalSRTMatrix(*calc_mtx);
        }
    }

    if (i_this->mAnmSeTimer != 0) {
        i_this->mAnmSeTimer--;
        if (i_this->mAnmSeTimer == 0) {
            i_this->mSound.startCreatureSound(i_this->mAnmSe, 0, -1);
        }
    }
}

static int daB_BH_Execute(b_bh_class* i_this) {
    if (cDmrNowMidnaTalk()) {
        return 1;
    }

    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz sp30;
    cXyz sp24;

    b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(a_this->parentActorID);
    if (bq_p->mDisableDraw == FALSE) {
        i_this->field_0xa25 = 1;
        if (i_this->mAction == ACTION_WAIT) {
            i_this->mAction = ACTION_B_WAIT;
        }
    }

    i_this->field_0x668++;

    for (int i = 0; i < 5; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->field_0x69e != 0) {
        i_this->field_0x69e--;
    }

    if (i_this->field_0xa25 != 0) {
        i_this->field_0xa26 = bq_p->shape_angle;

        J3DModel* bq_model = bq_p->mpMorf->getModel();
        bq_model->setBaseTRMtx(mDoMtx_stack_c::get());

        if (i_this->mID == 0) {
            MTXCopy(bq_model->getAnmMtx(0x1D), *calc_mtx);
            i_this->field_0xa26.z += 0x3000;
        } else {
            MTXCopy(bq_model->getAnmMtx(0x26), *calc_mtx);
            i_this->field_0xa26.z -= 0x3000;
        }

        sp30.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&sp30, &i_this->mBasePos);
        cMtx_YrotS(*calc_mtx, bq_p->shape_angle.y);

        if (i_this->mID == 0) {
            sp30.x = 500.0f;
        } else {
            sp30.x = -500.0f;
        }

        sp30.y = 0.0f;
        sp30.z = 0.0f;
        MtxPosition(&sp30, &sp24);
        i_this->field_0x6b0 = sp24 + i_this->mBasePos;
    } else {
        i_this->field_0x6b0 = i_this->mBasePos;
    }

    action(i_this);
    anm_se_set(i_this);

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(a_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);

    f32 size = l_HIO.model_size;
    mDoMtx_stack_c::scaleM(size, size, size);
    
    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    i_this->mpModelMorf->modelCalc();

    MTXCopy(model->getAnmMtx(BREG_S(4) + 10), *calc_mtx);

    sp30.set(300.0f + BREG_F(3), BREG_F(4), BREG_F(5));
    MtxPosition(&sp30, &a_this->eyePos);
    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += 250.0f + BREG_F(6);

    sp30.set(200.0f + BREG_F(3), BREG_F(4), BREG_F(5));
    MtxPosition(&sp30, &sp24);

    i_this->mCcSph.SetC(sp24);
    i_this->mCcSph.SetR((100.0f + YREG_F(3)) * l_HIO.model_size);

    if (i_this->field_0x688 > 1000.0f && dComIfGp_checkPlayerStatus0(0, 0x80000)) {
        i_this->mTgSph.SetR(2.5f * ((100.0f + YREG_F(3)) * l_HIO.model_size));
    } else {
        sp24.y -= 12000.0f;
    }

    i_this->mTgSph.SetC(sp24);
    dComIfG_Ccsp()->Set(&i_this->mCcSph);
    dComIfG_Ccsp()->Set(&i_this->mTgSph);

    if (i_this->field_0xce0 != 0) {
        i_this->mCcSph.OnAtSetBit();
        i_this->field_0xce0 = 0;
    } else {
        i_this->mCcSph.OffAtSetBit();
    }

    f32 temp_f30 = i_this->field_0xa20 * (0.002f + YREG_F(4));

    for (int i = 0; i < 3; i++) {
        if (i_this->field_0x900[i] == 0) {
            if (i_this->field_0x90c == 2) {
                i_this->field_0x90c = 0;
                i_this->field_0x900[i] = 2;
                i_this->field_0x906[i] = 17;
                i_this->field_0x8fc = 2.0f + YREG_F(8);
                i_this->field_0x8f8 = 4000;
                break;
            }
        } else {
            if (i_this->field_0x900[i] == 1) {
                if (i_this->field_0x909[i] == 0) {
                    i_this->field_0x909[i] = YREG_S(5) + 2;
                    i_this->field_0x8d4[i_this->field_0x906[i]] = 1;
                    i_this->field_0x906[i]++;

                    if (i_this->field_0x906[i] >= 18) {
                        i_this->field_0x900[i] = 0;
                    }
                }
            } else {
                i_this->mSound.startCreatureSoundLevel(Z2SE_EN_BH_BURST, 0, -1);

                if (i_this->field_0x909[i] == 0) {
                    i_this->field_0x909[i] = (YREG_S(5) + 1);
                    i_this->field_0x8d4[i_this->field_0x906[i]] = 1;
                    i_this->field_0x906[i]--;

                    if (i_this->field_0x906[i] < 0) {
                        i_this->field_0x900[i] = 0;
                    }
                }
            }

            if (i_this->field_0x909[i] != 0) {
                i_this->field_0x909[i]--;
            }
        }
    }

    for (int i = 0; i < 17; i++) {
        if (i_this->field_0x8d4[i] != 0) {
            i_this->field_0x8d4[i] += i_this->field_0x8f8;
            if (i_this->field_0x8d4[i] < 0) {
                i_this->field_0x8d4[i] = 0;
            }

            i_this->field_0x88c[i] = 1.0f + (i_this->field_0x8fc * cM_ssin((s16)i_this->field_0x8d4[i]));
        }

        MtxTrans(i_this->field_0x6bc[i].x, i_this->field_0x6bc[i].y, i_this->field_0x6bc[i].z, 0);
        cMtx_XrotM(*calc_mtx, i_this->field_0x794[i].x);
        cMtx_YrotM(*calc_mtx, i_this->field_0x794[i].y + 0x8000);
        MtxPush();
        MtxScale(l_HIO.model_size * i_this->field_0x88c[i], l_HIO.model_size * i_this->field_0x88c[i], l_HIO.model_size + temp_f30, 1);
        MtxTrans(0.0f, 0.0f, -10.0f - XREG_F(4), 1);
        i_this->mpStemModels[i]->setBaseTRMtx(*calc_mtx);
        MtxPull();

        if (i_this->mpLeafModels[i] != 0) {
            MtxScale(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size, 1);
            MtxTrans(0.0f, 0.0f, -10.0f - XREG_F(4), 1);
            cMtx_YrotM(*calc_mtx, i_this->field_0x9a8[i].y);
            cMtx_ZrotM(*calc_mtx, i_this->field_0x9a8[i].z);
            i_this->mpLeafModels[i]->setBaseTRMtx(*calc_mtx);
        }
    }

    return 1;
}

static int daB_BH_IsDelete(b_bh_class* i_this) {
    return 1;
}

static int daB_BH_Delete(b_bh_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fpc_ProcID id = fopAcM_GetID(a_this);
    
    dComIfG_resDelete(&i_this->mPhase, "B_BH");
    if (i_this->mInitHIO) {
        l_initHIO = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.no);
    }

    if (a_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    b_bh_class* a_this = (b_bh_class*)i_this;

    a_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("B_BH", BMDV_BH), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("B_BH", 12), 2, 1.0f, 0, -1, &a_this->mSound, 0, 0x11000084);
    if (a_this->mpModelMorf == NULL || a_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    void* modelData = dComIfG_getObjectRes("B_BH", BMDR_BH_KUKI);
    JUT_ASSERT(2475, modelData != NULL);

    for (int i = 0; i < 17; i++) {
        a_this->mpStemModels[i] = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
        if (a_this->mpStemModels[i] == NULL) {
            return 0;
        }

        a_this->field_0x88c[i] = 1.0f;
    }

    modelData = dComIfG_getObjectRes("B_BH", BMDR_BH_LEAF);
    JUT_ASSERT(2491, modelData != NULL);

    for (int i = 0; i < 17; i++) {
        if ((i & 3) == 2) {
            a_this->mpLeafModels[i] = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
            if (a_this->mpLeafModels[i] == NULL) {
                return 0;
            }

            a_this->field_0x960[i] = cM_rndFX(0.3f) + 1.2f;
            a_this->field_0x9a8[i].y = cM_rndF(0x10000);
            a_this->field_0x9a8[i].z = cM_rndF(0x10000);
        }
    }

    return 1;
}

static int daB_BH_Create(fopAc_ac_c* i_this) {
    b_bh_class* a_this = (b_bh_class*)i_this;
    fopAcM_ct(a_this, b_bh_class);

    int phase_state = dComIfG_resLoad(&a_this->mPhase, "B_BH");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("B_BH PARAM %x\n", fopAcM_GetParam(i_this));
        a_this->mID = (fopAcM_GetParam(i_this) >> 0x00) & 0xFF;
        a_this->field_0x5b5 = (fopAcM_GetParam(i_this) >> 0x08) & 0xFF;
        a_this->field_0x5b6 = (fopAcM_GetParam(i_this) >> 0x10) & 0xF;
        a_this->field_0x5b7 = (fopAcM_GetParam(i_this) >> 0x14) & 0xF;

        if (a_this->mID == 0xFF) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("B_BH//////////////B_BH SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0xDB20)) {
            OS_REPORT("//////////////B_BH SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////B_BH SET 2 !!\n");

        if (!l_initHIO) {
            a_this->mInitHIO = TRUE;
            l_initHIO = TRUE;
            l_HIO.no = mDoHIO_CREATE_CHILD("ボスババ触手", &l_HIO);
        }

        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        i_this->health = 1;
        i_this->field_0x560 = 1;

        a_this->mColliderStts.Init(0xFD, 0, i_this);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xd}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_HARD_BODY, 0x0, 0x1, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_STONE, 0x2, 0x0, 0x0, 0x3}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };
    
        static dCcD_SrcSph tg_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x0}}, // mObj
                {dCcD_SE_HARD_BODY, 0x0, 0x1, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        a_this->mCcSph.Set(cc_sph_src);
        a_this->mCcSph.SetStts(&a_this->mColliderStts);
        
        a_this->mTgSph.Set(tg_sph_src);
        a_this->mTgSph.SetStts(&a_this->mColliderStts);
        a_this->mTgSph.OnTgNoHitMark();

        a_this->mAtInfo.mpSound = &a_this->mSound;
        a_this->mAtInfo.field_0x18 = 0x2D;

        a_this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->mAcchCir.SetWall(150.0f, 250.0f);

        a_this->mSound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);

        i_this->attention_info.distances[fopAc_attn_BATTLE_e] = 22;

        i_this->home.pos.y -= 50.0f;
        a_this->mBasePos = i_this->home.pos;

        bh[a_this->mID] = a_this;

        if (dComIfG_play_c::getLayerNo(0) == 4) {
            a_this->mAction = ACTION_DOWN;
            a_this->mMode = 0;
            a_this->mBasePos.y = i_this->home.pos.y - 1600.0f;
            i_this->current.pos.y = i_this->home.pos.y - 700.0f;
        }

        u8 swbit = fopAcM_GetParam(i_this) >> 0x10;
        if (!dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(i_this)) && cDmr_SkipInfo == 0) {
            a_this->mAction = ACTION_START;
            a_this->mMode = 0;
            a_this->mBasePos.y = i_this->home.pos.y - 1600.0f;
            i_this->current.pos.y = i_this->home.pos.y - 700.0f;
        }

        a_this->field_0x668 = cM_rndF(0xFFFF);
        daB_BH_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daB_BH_Method = {
    (process_method_func)daB_BH_Create,
    (process_method_func)daB_BH_Delete,
    (process_method_func)daB_BH_Execute,
    (process_method_func)daB_BH_IsDelete,
    (process_method_func)daB_BH_Draw,
};

extern actor_process_profile_definition g_profile_B_BH = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_B_BH,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(b_bh_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  211,                    // mPriority
  &l_daB_BH_Method,       // sub_method
  0x00044000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
