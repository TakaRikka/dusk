/**
 * @file d_a_e_gb.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_gb.h"
#include "res/Object/E_gb.h"
#include "d/d_cc_d.h"
#include "d/d_camera.h"
#include "d/d_bomb.h"
#include "d/actor/d_a_obj_smallkey.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"

enum Head_Action {
    /* 0x0 */ HEAD_ACTION_WAIT,
    /* 0x1 */ HEAD_ACTION_ATTACK_1,
    /* 0x2 */ HEAD_ACTION_ATTACK_2,
    /* 0x3 */ HEAD_ACTION_DAMAGE,
    /* 0x5 */ HEAD_ACTION_END = 0x5,
    /* 0xA */ HEAD_ACTION_START = 0xA,
};

enum Flower_Action {
    /* 0x0 */ FLOWER_ACTION_WAIT,
    /* 0x2 */ FLOWER_ACTION_EATBOMB = 0x2,
    /* 0x3 */ FLOWER_ACTION_CHANCE,
    /* 0xA */ FLOWER_ACTION_END = 0xA,
};

class daE_GB_HIO_c : public JORReflexible{
public:
    virtual ~daE_GB_HIO_c() {}
    daE_GB_HIO_c();

    void genMessage(JORMContext*);

    /* 0x04 */ s8 no;
    /* 0x08 */ f32 face_size;
    /* 0x0C */ f32 flower_size;
    /* 0x10 */ f32 stem_size;
    /* 0x14 */ s16 attack_frequency;
    /* 0x18 */ f32 attack_spd;
    /* 0x1C */ s16 attack_range;
    /* 0x1E */ s16 post_attack_pause_timer;
};

daE_GB_HIO_c::daE_GB_HIO_c() {
    no = -1;
    face_size = 1.0f;
    flower_size = 1.0f;
    stem_size = 1.0f;
    attack_frequency = 15;
    attack_spd = 65.0f;
    attack_range = 12;
    post_attack_pause_timer = 10;
}

#if DEBUG
void daE_GB_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("  デカババ　　", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genLabel("　　　大幅なサイズ変更は姿勢生成に影響があるので注意　　　", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("顔サイズ", &face_size, 0.0f, 5.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("花サイズ", &flower_size, 0.0f, 5.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("茎サイズ", &stem_size, 0.0f, 5.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("攻撃頻度", &attack_frequency, 0, 30000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("攻撃速度", &attack_spd, 0.0f, 200.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("攻後距離", &attack_range, 0, 30, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("攻後停間", &post_attack_pause_timer, 0, 200, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

static void head_anm_init(e_gb_class* i_this, int i_idx, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->anmP->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gb", i_idx), i_mode,
                                 i_morf, i_speed, 0.0f, -1.0f, NULL);
    i_this->headAnmNo = i_idx;
}

static void body_anm_init(e_gb_class* i_this, int i_idx, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->flowerAnmP->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gb", i_idx), i_mode,
                                 i_morf, i_speed, 0.0f, -1.0f, NULL);
    i_this->bodyAnmNo = i_idx;
}

static int daE_GB_Draw(e_gb_class* i_this) {
    fopEn_enemy_c* actor = &i_this->actor;

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);

    J3DModel* model = i_this->flowerAnmP->getModel();
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);

    i_this->brkAnmP->entry(model->getModelData());
    i_this->flowerAnmP->entryDL();

    if (i_this->keyModelP != NULL) {
        g_env_light.setLightTevColorType_MAJI(i_this->keyModelP, &actor->tevStr);
        mDoExt_modelUpdateDL(i_this->keyModelP);
    }

    if (i_this->status != 0) {
        if (i_this->stemModelP[17] != NULL) {
            model = i_this->anmP->getModel();
            g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);
            i_this->anmP->entryDL();

            for (int i = 0; i < 17; i++) {
                if (i_this->stemModelP[i] != NULL) {
                    g_env_light.setLightTevColorType_MAJI(i_this->stemModelP[i], &actor->tevStr);
                    mDoExt_modelUpdateDL(i_this->stemModelP[i]);
                }
            }
        }

        cXyz pos;
        pos.set(actor->current.pos.x, actor->current.pos.y + 100.0f + BREG_F(18), actor->current.pos.z);
        i_this->shadowKey = dComIfGd_setShadow(i_this->shadowKey, 1, model, &pos, BREG_F(19) + 1400.0f, 0.0f, actor->current.pos.y,
                                                 i_this->objAcch.GetGroundH(), i_this->objAcch.m_gnd,
                                                 &i_this->actor.tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

        for (int i = 0; i < 17; i++) {
            if (i_this->stemModelP[i] != NULL) {
                dComIfGd_addRealShadow(i_this->shadowKey, i_this->stemModelP[i]);
            }
        }
    }

    return 1;
}

static u8 data_806C7928;

static daE_GB_HIO_c l_HIO;

static void e_gb_wait(e_gb_class* i_this) {
    fopEn_enemy_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz work, target_offset;
    f32 offset = TREG_F(12) + 30.0f;
    #if VERSION == VERSION_SHIELD_DEBUG
    f32 pl_dist = (actor->home.pos - player->current.pos).abs();
    #else
    f32 pl_dist = (actor->home.pos - dComIfGp_getPlayer(0)->current.pos).abs();
    #endif
    s8 near_attack_flag = 0;

    switch (i_this->mode) {
        case -2:
            head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_REVIVE_e, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->timer[1] = l_HIO.attack_frequency + cM_rndF(l_HIO.attack_frequency);
            i_this->mode = 1;
            i_this->currentPosTargetStep = 1.0f;
            break;

        case 0:
            head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_WAIT_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mode = 1;
            i_this->timer[1] = l_HIO.attack_frequency + cM_rndF(l_HIO.attack_frequency);
            // fallthrough
        case 1:
            if (i_this->headAnmNo != dRes_ID_E_GB_BCK_GB_WAIT_e && i_this->anmP->isStop()) {
                head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_WAIT_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }

            if (i_this->timer[1] == 0 && pl_dist < YREG_F(17) + 1000.0f) {
                near_attack_flag = 1;
                OS_REPORT("E_gb NEAR ATTACK d\n");
            }

            if (i_this->timer[0] == 0) {
                i_this->timer[0] = cM_rndF(30.0f) + 10.0f;
                cMtx_YrotS(*calc_mtx, actor->current.angle.y);
                work.x = cM_rndFX(100.0f);
                work.y = BREG_F(3) + (l_HIO.face_size * 450.0f + cM_rndFX(100.0f));
                work.z = BREG_F(4) + (cM_rndFX(100.0f) + 200.0f);
                MtxPosition(&work, &i_this->currentPosTarget);
                i_this->currentPosTarget += i_this->field_0x6d4;
                i_this->currentPosTargetStep = 0.0f;

                if (i_this->timer[1] == 0 && pl_dist < YREG_F(19) + 1500.0f) {
                    near_attack_flag = 1;
                }
            }

            target_offset.x = cM_ssin(i_this->frameCounter * (TREG_S(0) + 0x76C)) * offset;
            target_offset.y = cM_ssin(i_this->frameCounter * (TREG_S(1) + 0x898)) * offset;
            target_offset.z = cM_scos(i_this->frameCounter * (TREG_S(2) + 2000)) * offset;
            cLib_addCalc2(&actor->current.pos.x, i_this->currentPosTarget.x + target_offset.x, 0.1f, i_this->currentPosTargetStep * 30.0f);
            cLib_addCalc2(&actor->current.pos.z, i_this->currentPosTarget.z + target_offset.z, 0.1f, i_this->currentPosTargetStep * 30.0f);
            cLib_addCalc2(&actor->current.pos.y, i_this->currentPosTarget.y + target_offset.y, 0.1f, i_this->currentPosTargetStep * 30.0f);
            cLib_addCalc2(&i_this->currentPosTargetStep, 1.0f, 1.0f, 0.05f);
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->angleYTarget, 8, 0x100);
    cLib_addCalcAngleS2(&actor->shape_angle.y, i_this->angleYTarget, 8, 0x800);
    cLib_addCalcAngleS2(&actor->shape_angle.x, i_this->angleXTarget, 8, 0x400);

    if (near_attack_flag != 0) {
        actor->speedF = 0.0f;
        i_this->headAction = HEAD_ACTION_ATTACK_1;
        i_this->mode = 0;
    }
}

static void e_gb_attack_1(e_gb_class* i_this) {
    fopEn_enemy_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz work, pos_target_offset;
    f32 attack_spd = 0.0f;
    f32 step = YREG_F(2) + 20.0f;
    #if VERSION == VERSION_SHIELD_DEBUG
    f32 pl_dist = (actor->home.pos - player->current.pos).abs();
    #else
    f32 pl_dist = (actor->home.pos - dComIfGp_getPlayer(0)->current.pos).abs();
    #endif

    switch (i_this->mode) {
        case 0:
            if (pl_dist < TREG_F(11) + 1200.0f && cM_rndF(1.0f) < 0.5f) {
                i_this->headAction = HEAD_ACTION_ATTACK_2;
                i_this->mode = 0;
                i_this->timer[0] = 0;
                return;
            }

            head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_ATTACK_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->sound.startCreatureVoice(Z2SE_EN_GB_V_ATTACK, -1);
            i_this->mode = 1;
            i_this->currentPosTargetStep = 0.0f;
            break;

        case 1:
            cLib_addCalcAngleS2(&actor->current.angle.y, i_this->angleYTarget, 2, 0x800);
            cLib_addCalcAngleS2(&actor->current.angle.x, i_this->angleXTarget, 2, 0x800);
            cMtx_YrotS(*calc_mtx, actor->current.angle.y);

            work.x = 0.0f;
            work.y = l_HIO.face_size * 450.0f + NREG_F(0);
            work.z = NREG_F(1);
            MtxPosition(&work, &pos_target_offset);
            i_this->currentPosTarget = i_this->field_0x6d4 + pos_target_offset;

            cLib_addCalc2(&actor->current.pos.x, i_this->currentPosTarget.x, 0.1f, i_this->currentPosTargetStep * 30.0f);
            cLib_addCalc2(&actor->current.pos.z, i_this->currentPosTarget.z, 0.1f, i_this->currentPosTargetStep * 30.0f);
            cLib_addCalc2(&actor->current.pos.y, i_this->currentPosTarget.y, 0.1f, i_this->currentPosTargetStep * 30.0f);
            cLib_addCalc2(&i_this->currentPosTargetStep, 1.0f, 1.0f, 0.1f);

            if (i_this->anmP->isStop()) {
                i_this->mode = 3;
                i_this->timer[0] = l_HIO.attack_range;
                head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_ATTACK_DASH_e, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->sound.startCreatureVoice(Z2SE_EN_GB_V_ATTACK_DASH, -1);
            }
            break;

        case 3:
            attack_spd = l_HIO.attack_spd;
            step = l_HIO.attack_spd * 0.5f;
            if (i_this->timer[0] == 0) {
                i_this->timer[0] = l_HIO.post_attack_pause_timer;
                i_this->mode = 4;
            }
            break;

        case 4:
            if (i_this->timer[0] == 0) {
                i_this->headAction = HEAD_ACTION_WAIT;
                i_this->mode = 0;
                i_this->timer[0] = cM_rndF(10.0f) + 10.0f;
                i_this->currentPosTargetStep = 0.0f;
                return;
            }
            break;

        case 5:
            step = NREG_F(4) + 5.0f;
            if (i_this->timer[0] == 0 && fabsf(actor->speedF) < 0.1f) {
                i_this->headAction = HEAD_ACTION_WAIT;
                i_this->mode = 0;
                i_this->timer[0] = cM_rndF(10.0f) + 10.0f;
                i_this->currentPosTargetStep = 0.0f;
            }
    }

    cLib_addCalc2(&actor->speedF, attack_spd, 1.0f, step);
    if (actor->speedF >= 10.0f) {
        i_this->atSetBitFlag = 1;
        if (i_this->mode != 5 && (i_this->objAcch.ChkWallHit() != 0 || i_this->headSph.ChkAtShieldHit())) {
            i_this->mode = 5;
            i_this->field_0x948 = VREG_S(8) + 5000;
            i_this->xRot = VREG_S(4) + 3000;
            i_this->field_0x94c = VREG_F(5) + 3000.0f;

            head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_WAIT_e, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);

            i_this->timer[0] = 0;
            actor->speedF = NREG_F(11) + -30.0f;
        }
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 8, 0x800);
    cLib_addCalcAngleS2(&actor->shape_angle.x, actor->current.angle.x, 8, 0x400);
}

static void e_gb_attack_2(e_gb_class* i_this) {
    fopEn_enemy_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz work, pos_target_offset;
    f32 attack_spd = 0.0f;
    f32 step = YREG_F(2) + 20.0f;
    f32 frame = i_this->anmP->getFrame();

    switch (i_this->mode) {
        case 0:
            head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_ATTACK_BEAT_SIGN_e, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->timer[0] = cM_rndF(10.0f) + 49.0f;
            i_this->mode = 1;
            i_this->sound.startCreatureSound(Z2SE_EN_GB_V_DOSUN, 0, -1);
            break;

        case 1:
            actor->speedF = 0.0f;
            actor->speed.y = 0.0f;

            cLib_addCalc2(&actor->current.pos.y, actor->home.pos.y + 200.0f, 0.05f, 3.0f);
            cLib_addCalcAngleS2(&actor->current.angle.y, i_this->angleYTarget, 2, 0x800);
            cLib_addCalcAngleS2(&actor->current.angle.x, 0, 2, 0x800);
            cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 8, 0x800);
            cLib_addCalcAngleS2(&actor->shape_angle.x, actor->current.angle.x, 8, 0x400);

            if (i_this->timer[0] == 0) {
                head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_ATTACK_BEAT_e, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->sound.startCreatureVoice(Z2SE_EN_GB_V_ATTACK, -1);
                i_this->sound.startCreatureSound(Z2SE_EN_GB_DOSUN_DASH, 0, -1);
                i_this->mode = 2;
            }
            break;

        case 2:
            work.x = player->current.pos.x - actor->current.pos.x;
            work.z = player->current.pos.z - actor->current.pos.z;
            if (frame >= 16.0f) {
                actor->speedF = 0.0f;
                attack_spd = 0.0f;
            } else if (frame >= 6.0f) {
                attack_spd = JREG_F(17) + 50.0f;
                if (frame == 6.0f) {
                    actor->speed.y = JREG_F(15) + 50.0f;
                }

                if (JMAFastSqrt(work.x * work.x + work.z * work.z) < TREG_F(12) + 100.0f) {
                    actor->speedF = 0.0f;
                    attack_spd = 0.0f;
                }
            } else {
                actor->speed.y = 0.0f;
            }

            if (frame >= 10.0f && frame <= 20.0f) {
                i_this->atSetBitFlag = 1;
            }

            actor->speedF = attack_spd;
            if (actor->speed.y <= 0.0f && actor->current.pos.y <= (actor->home.pos.y + 150.0f + KREG_F(6))) {
                actor->current.pos.y = actor->home.pos.y + 150.0f + KREG_F(6);
                if (actor->speed.y < -30.0f) {
                    fopAcM_effSmokeSet1(&i_this->field_0xe74, &i_this->field_0xe78, &actor->eyePos, NULL, TREG_F(18) + 1.7f, &actor->tevStr, 1);
                    dComIfGp_getVibration().StartShock((int)YREG_S(2) + VIBMODE_S_POWER5, 15, cXyz(0.0f, 1.0f, 0.0f));
                    i_this->sound.startCreatureSound(Z2SE_EN_GB_DOSUN, 0, -1);
                }

                actor->speed.y = 0.0f;
                if (i_this->anmP->isStop()) {
                    work = actor->current.pos - actor->home.pos;
                    if (cM_rndF(1.0f) < 0.5f && work.abs() < TREG_F(12) + 1000.0f) {
                        i_this->mode = 0;
                    } else {
                        i_this->headAction = HEAD_ACTION_WAIT;
                        i_this->mode = 0;
                        i_this->timer[0] = cM_rndF(10.0f) + 10.0f;
                        i_this->currentPosTargetStep = 0.0f;
                    }
                }
            }
    }
}

static void e_gb_damage(e_gb_class* i_this) {
    fopEn_enemy_c* actor = &i_this->actor;
    cXyz work;

    i_this->invulnerabilityTimer = 35;
    switch (i_this->mode) {
        case 0:
            i_this->mode = 1;
            if ((s16)(i_this->angleYTarget - actor->current.angle.y) < 0) {
                actor->current.angle.y += (s16)(KREG_S(6) + 0x2000);
            } else {
                actor->current.angle.y -= (s16)(KREG_S(6) + 0x2000);
            }

            cMtx_YrotS(*calc_mtx, actor->current.angle.y);
            work.x = 0.0f;
            work.y = AREG_F(2) + 50.0f;
            work.z = AREG_F(1) + 800.0f;
            MtxPosition(&work, &i_this->currentPosTarget);
            i_this->currentPosTarget += actor->home.pos;
            actor->speed.x = fabsf(actor->current.pos.x - i_this->currentPosTarget.x) * 0.2f;
            actor->speed.y = fabsf(actor->current.pos.y - i_this->currentPosTarget.y) * 0.2f;
            actor->speed.z = fabsf(actor->current.pos.z - i_this->currentPosTarget.z) * 0.2f;
            i_this->timer[0] = KREG_S(7) + 15;
            // fallthrough
        case 1:
            if (i_this->timer[0] == 0) {
                if (actor->health <= 0) {
                    i_this->headAction = HEAD_ACTION_END;
                    i_this->mode = 0;
                } else {
                    i_this->headAction = HEAD_ACTION_WAIT;
                    i_this->mode = -2;
                }
            }
    }

    cLib_addCalc2(&actor->current.pos.x, i_this->currentPosTarget.x, 0.1f, actor->speed.x);
    cLib_addCalc2(&actor->current.pos.y, i_this->currentPosTarget.y, 0.1f, actor->speed.y);
    cLib_addCalc2(&actor->current.pos.z, i_this->currentPosTarget.z, 0.1f, actor->speed.z);
    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&actor->shape_angle.x, 0, 4, 0x800);
}

static void e_gb_end(e_gb_class* i_this) {
    static u16 eno[2] = {
        dPa_RM(ID_ZM_S_GS_FEELERDEATH00),
        dPa_RM(ID_ZM_S_GS_FEELERDEATH01),
    };

    fopEn_enemy_c* actor = &i_this->actor;

    i_this->invulnerabilityTimer = 20;
    switch (i_this->mode) {
        case 0:
            head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_DIE_e, 30.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 1;
            break;

        case 1:
            i_this->mode = 2;
            i_this->sound.startCreatureSound(Z2SE_EN_GB_BOMBROOT, 0, -1);
            break;

        case 2:
            if ((i_this->frameCounter & 1) == 0) {
                i_this->stemModelP[i_this->stemModelIdx] = NULL;
                if (i_this->stemModelIdx == 17) {
                    i_this->mode = 10;
                    fopAcM_createDisappear(actor, &actor->eyePos, 12, 0, actor->field_0x564);
                    i_this->status = 0;
                    i_this->flowerAction = FLOWER_ACTION_CHANCE;
                    i_this->flowerMode = 0;
                    i_this->timer[0] = 80;
                    i_this->field_0x668 = 1;
                    i_this->sound.stopAnime();
                    Z2GetAudioMgr()->unMuteSceneBgm(33);
                } else {
                    for (int i = 0; i < 2; i++) {
                        dComIfGp_particle_set(eno[i], &i_this->field_0x6e4[i_this->stemModelIdx], NULL, NULL);
                    }
                }

                i_this->stemModelIdx++;
            }
            break;

        case 10:
            break;
    }

    actor->current.pos.y -= 5.0f;
}

static void e_gb_start(e_gb_class* i_this) {
    fopEn_enemy_c* actor = &i_this->actor;
    cXyz work;

    i_this->invulnerabilityTimer = 20;
    switch (i_this->mode) {
        case 0:
            i_this->mode = 1;
            head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_WAIT_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->demoMode = 1;
            break;

        case 2:
            actor->current.pos.set(-6112.0f, KREG_F(6) + 4520.0f, KREG_F(7) + 10800.0f);
            i_this->mode = 3;
            break;

        case 3:
            cLib_addCalc2(&actor->current.pos.x, -5450.0f, 0.5f, KREG_F(8) + 30.0f);
            break;

        case 4:
            cMtx_YrotS(*calc_mtx, actor->home.angle.y);
            work.x = NREG_F(0) + 20.0f;
            work.y = NREG_F(1) + 500.0f;
            work.z = NREG_F(2);
            MtxPosition(&work, &i_this->currentPosTarget);
            i_this->currentPosTarget += actor->home.pos;

            cLib_addCalc2(&actor->current.pos.x, i_this->currentPosTarget.x, 0.1f, NREG_F(3) + 80.0f);
            cLib_addCalc2(&actor->current.pos.y, i_this->currentPosTarget.y, 0.1f, NREG_F(3) + 80.0f);
            cLib_addCalc2(&actor->current.pos.z, i_this->currentPosTarget.z, 0.1f, NREG_F(3) + 80.0f);

            cLib_addCalcAngleS2(&actor->current.angle.y, -0x4000, 16, 0x200);
            cLib_addCalcAngleS2(&actor->shape_angle.y, 0x4000, 16, 0x200);
            cLib_addCalcAngleS2(&actor->shape_angle.x, 0x2000, 16, 0x100);
    }
}

static void kuki_control1(e_gb_class* i_this) {
    static s16 pow_xa[17] = {
        -0x3000, -0x2800, -0x2000,
        -0x1800, -0x1000, 0x0000,
        0x1000, 0x1800, 0x2000,
        0x2800, 0x3000, 0x3800,
        0x3000, 0x2000, 0x1000,
        0x0000, 0x0000,
    };

    static s16 pow_xa_chance[17] = {
        -0x3800, -0x3000, -0x2800,
        -0x2000, -0x1800, -0x1000,
        -0x0800, 0x0000,  0x0000,
        0x0000, 0x0000, 0x0000,
        0x0800, 0x0000, 0x0800,
        0x0000, 0x0800,
    };

    static f32 wav_d[17] = {
        0.0f, 0.2f, 0.4f, 
        0.5f, 0.6f, 0.8f, 1.0f,
        1.0f, 1.0f, 1.0f, 
        1.0f, 1.0f, 0.8f, 0.6f,
        0.4f, 0.2f, 0.0f,
    };

    fopEn_enemy_c* actor = &i_this->actor;
    int i;
    cXyz sp9c, spa8, spb4;
    cXyz* pcVar1 = i_this->field_0x6e4;
    *pcVar1 = i_this->field_0x6d4;
    cXyz spc0;

    sp9c.x = 0.0f;
    sp9c.y = 0.0f;
    sp9c.z = i_this->field_0x93c;
    pcVar1++;
    spb4.x = 0.0f;
    spb4.z = 0.0f;
    spb4.y = -(200.0f + TREG_F(0));
    f32 fVar8;
    f32 fVar4;
    f32 fVar2;
    f32 fVar3;
    f32 fVar5;
    f32 fVar7;
    f32 fVar6;
    s16 sVar3;
    s16 sVar2;
    f32 fVar1 = i_this->field_0x944;

    for (i = 1; i < 18; i++, pcVar1++) {
        if (i_this->headAction != HEAD_ACTION_END) {
            if (i_this->field_0x94c > 1.0f) {
                fVar6 = i_this->field_0x94c * wav_d[i - 1] * 0.035f;
                fVar3 = fVar6 * cM_ssin(i_this->frameCounter * (7000 + TREG_S(0)) + i * (7000 + TREG_S(1)));
                fVar7 = fVar6 * cM_scos(i_this->frameCounter * (7700 + TREG_S(2)) + i * (5000 + TREG_S(3)));
            } else {
                fVar6 = (60.0f + TREG_F(8)) * wav_d[i - 1];
                fVar3 = fVar6 * cM_ssin(i_this->frameCounter * (2000 + TREG_S(0)) + i * (7000 + TREG_S(1)));
                fVar7 = fVar6 * cM_scos(i_this->frameCounter * (2300 + TREG_S(2)) + i * (5000 + TREG_S(3)));
            }

            cMtx_YrotS(*calc_mtx, (s16)actor->current.angle.y);
            if (i_this->field_0x6e0 != 0) {
                cMtx_XrotM(*calc_mtx, (s16)pow_xa_chance[i - 1]);
            } else {
                cMtx_XrotM(*calc_mtx, (s16)pow_xa[i - 1]);
            }

            MtxScale(fVar1, fVar1, fVar1, 1);
            MtxPosition(&spb4, &spc0);
            fVar4 = fVar3 + (spc0.x + (pcVar1->x - (pcVar1 - 1)->x));
            fVar5 = (pcVar1->y - (pcVar1 - 1)->y) - spc0.y;
            fVar2 = (pcVar1->z - (pcVar1 - 1)->z) + spc0.z + fVar7;
        } else {
            fVar6 = (20.0f + TREG_F(8)) * wav_d[i - 1];
            fVar3 = fVar6 * cM_ssin(i_this->frameCounter * (1100 + TREG_S(0)) + i * (7000 + TREG_S(1)));
            fVar7 = fVar6 * cM_scos(i_this->frameCounter * (1500 + TREG_S(2)) + i * (5000 + TREG_S(3)));
            fVar4 = fVar3 + (pcVar1->x - (pcVar1 - 1)->x);
            fVar8 = pcVar1->y - 5.0f;
            
            if (fVar8 < (i_this->objAcch.GetGroundH() + 5.0f + YREG_F(18))) {
                fVar8 = i_this->objAcch.GetGroundH() + 5.0f + YREG_F(18);
            }

            fVar5 = fVar8 - (pcVar1 - 1)->y;
            fVar2 = pcVar1->z - (pcVar1 - 1)->z + fVar7;
        }

        sVar2 = (s16)cM_atan2s(fVar4, fVar2);
        sVar3 = -cM_atan2s(fVar5, JMAFastSqrt(fVar4 * fVar4 + fVar2 * fVar2));
        cMtx_YrotS(*calc_mtx, sVar2);
        cMtx_XrotM(*calc_mtx, sVar3);
        MtxPosition(&sp9c, &spa8);
        *pcVar1 = *(pcVar1 - 1) + spa8;
    }
}

static void kuki_control2(e_gb_class* i_this) {
    fopEn_enemy_c* actor = &i_this->actor;
    cXyz sp58, sp64;

    int i;
    s16 sVar1;
    s16 sVar2;
    cXyz* pcVar1 = &i_this->field_0x6e4[17];
    csXyz* pcVar2 = &i_this->field_0x7bc[17];
    *pcVar1 = actor->current.pos;
    sp58.x = 0.0f;
    sp58.y = 0.0f;
    sp58.z = i_this->field_0x93c;
    pcVar1--;
    pcVar2--;

    f32 diffx;
    f32 diffy;
    f32 diffz;
    
    for (i = 16; i >= 0; i--, pcVar1--, pcVar2--) {
        diffx = pcVar1[0].x - pcVar1[1].x;
        diffy = pcVar1[0].y - pcVar1[1].y;
        diffz = pcVar1[0].z - pcVar1[1].z;
        sVar1 = -cM_atan2s(diffy, diffz);
        sVar2 = (s16)cM_atan2s(diffx, JMAFastSqrt(diffy * diffy + diffz * diffz));
        cMtx_XrotS(*calc_mtx, sVar1);
        cMtx_YrotM(*calc_mtx, sVar2);
        MtxPosition(&sp58, &sp64);
        pcVar1[0] = pcVar1[1] + sp64;
        pcVar2->x = (s16)sVar1;
        pcVar2->y = (s16)sVar2;
    }
}

static void kuki_control3(e_gb_class* i_this) {
    fopEn_enemy_c* actor = &i_this->actor;
    cXyz sp40;
    sp40 = i_this->field_0x6e4[0] - i_this->field_0x6d4;
    cXyz* pcVar1 = i_this->field_0x6e4;

    for (int i = 0; i < 18; i++, pcVar1++) {
        f32 fVar1 = ((18 - i) / 17.0f);
        *pcVar1 -= sp40 * fVar1;
    }

    i_this->field_0x964 = sp40.abs();
}

static void* s_b_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);

    if (fopAcM_IsActor(i_actor) && dBomb_c::checkBombActor((fopAc_ac_c*)i_actor) && !((dBomb_c*)i_actor)->checkStateExplode()) {
        cXyz sp28 = ((fopAc_ac_c*)i_actor)->current.pos - ((fopAc_ac_c*)i_data)->home.pos;
        if (fabsf(sp28.y) < NREG_F(6) + 110.0f && JMAFastSqrt(sp28.x * sp28.x + sp28.z * sp28.z) < NREG_F(7) + 150.0f) {
            fopAcM_delete(((fopAc_ac_c*)i_actor));
            return i_actor;
        }
    }
    return NULL;
}

static void damage_check(e_gb_class* i_this) {
    fopEn_enemy_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    i_this->stts.Move();
    if (i_this->invulnerabilityTimer == 0) {
        if (i_this->headSph.ChkAtShieldHit()) {
            i_this->invulnerabilityTimer = 6;
            i_this->atInfo.mpCollider = i_this->headSph.GetAtHitObj();
            def_se_set(&i_this->sound, i_this->atInfo.mpCollider, 0x2D, NULL);
            dComIfGp_getVibration().StartShock(6, 31, cXyz(0.0f, 1.0f, 0.0f));
        } else if (i_this->headSph.ChkTgHit() != 0) {
            i_this->invulnerabilityTimer = 6;
            i_this->atInfo.mpCollider = i_this->headSph.GetTgHitObj();
            cc_at_check(actor, &i_this->atInfo);
            if (i_this->atInfo.mHitType == HIT_TYPE_STUN) {
                i_this->headAction = HEAD_ACTION_ATTACK_1;
                i_this->mode = 5;
                i_this->field_0x948 = VREG_S(8) + 5000;
                i_this->xRot = VREG_S(4) + 3000;
                i_this->field_0x94c = VREG_F(5) + 3000.0f;

                head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_WAIT_e, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->sound.startCreatureVoice(Z2SE_EN_GB_V_NODAMAGE, -1);
                i_this->timer[0] = 30;
            } else {
                i_this->headAction = HEAD_ACTION_DAMAGE;
                i_this->mode = 0;

                head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_DAMAGE_A_e, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                if (actor->health <= 0) {
                    i_this->sound.startCreatureVoice(Z2SE_EN_GB_V_DIE, -1);
                } else {
                    i_this->sound.startCreatureVoice(Z2SE_EN_GB_V_DAMAGE, -1);
                }
            }

            OS_REPORT("E_gb AI.POW %d\n", i_this->atInfo.mAttackPower);
        }
    }
}

static void action(e_gb_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz work;
    cXyz new_speed;
    // FAKEMATCH???
    #if VERSION == VERSION_SHIELD_DEBUG
    work = player->current.pos - actor->current.pos;
    #else
    work = dComIfGp_getPlayer(0)->current.pos - actor->current.pos;
    #endif
    work.y += 100.0f;

    i_this->angleYTarget = (s16)cM_atan2s(work.x, work.z);
    i_this->angleXTarget = -cM_atan2s(work.y, JMAFastSqrt(work.x * work.x + work.z * work.z));
    i_this->distToPlayer = fopAcM_searchPlayerDistance(actor);

    damage_check(i_this);

    s8 bVar1 = 0;
    s8 sVar1 = 0;
    s8 bVar2 = 1;
    work = actor->current.pos - actor->home.pos;
    cMtx_YrotS(*calc_mtx, cM_atan2s(work.x, work.z));
    work.x = 0.0f;
    work.y = KREG_F(0) + 30.0f;
    work.z = KREG_F(1) + 150.0f;
    MtxPosition(&work, &i_this->field_0x6d4);
    i_this->field_0x6d4 += actor->home.pos;
    i_this->field_0x6e0 = 0;
    actor->field_0x566 = 0;
    s8 link_search = 1;

    switch (i_this->headAction) {
        case HEAD_ACTION_WAIT:
            e_gb_wait(i_this);
            bVar1 = 1;
            actor->field_0x566 = 1;
            break;

        case HEAD_ACTION_ATTACK_1:
            e_gb_attack_1(i_this);
            bVar1 = 1;
            sVar1 = 1;
            actor->field_0x566 = 1;
            break;

        case HEAD_ACTION_ATTACK_2:
            e_gb_attack_2(i_this);
            bVar1 = 1;
            sVar1 = 2;
            bVar2 = 0;
            actor->field_0x566 = 1;
            break;

        case HEAD_ACTION_DAMAGE:
            e_gb_damage(i_this);
            actor->field_0x566 = 1;
            bVar1 = 1;
            link_search = 0;
            break;

        case HEAD_ACTION_END:
            e_gb_end(i_this);
            bVar1 = 1;
            actor->field_0x566 = 1;
            link_search = 0;
            break;

        case HEAD_ACTION_START:
            e_gb_start(i_this);
            bVar2 = 0;
            i_this->field_0x6e0 = 1;
            actor->field_0x566 = 1;
            link_search = 0;
    }

    if (link_search != 0) {
        i_this->sound.setLinkSearch(true);
    } else {
        i_this->sound.setLinkSearch(false);
    }

    if (sVar1 == 1) {
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        cMtx_XrotM(*calc_mtx, actor->current.angle.x);
        work.x = 0.0f;
        work.y = 0.0f;
        work.z = actor->speedF;
        MtxPosition(&work, &actor->speed);
        actor->current.pos += actor->speed;
    } else if (sVar1 == 2) {
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        work.x = 0.0f;
        work.y = 0.0f;
        work.z = actor->speedF;

        MtxPosition(&work, &new_speed);
        actor->speed.x = new_speed.x;
        actor->speed.z = new_speed.z;
        actor->current.pos += actor->speed;
        actor->speed.y -= JREG_F(12) + 10.0f;
    }

    if (bVar2 != 0) {
        f32 fVar1;
        if (i_this->headAction != HEAD_ACTION_END) {
            fVar1 = ZREG_F(3) + 150.0f;
        } else {
            fVar1 = 60.0f;
        }

        i_this->objAcch.CrrPos(dComIfG_Bgsp());
        if (actor->current.pos.y < fVar1 + i_this->objAcch.GetGroundH()) {
            actor->current.pos.y = fVar1 + i_this->objAcch.GetGroundH();
        }
    }

    if (bVar1 != 0) {
        fopAcM_OnStatus(actor, 0);
        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    }

    work = actor->current.pos - i_this->field_0x6d4;
    i_this->field_0x93c = i_this->field_0x940 * (work.abs() * (BREG_F(0) + 0.1f));

    if (i_this->field_0x93c > (35.0f + BREG_F(1)) * l_HIO.face_size) {
        i_this->field_0x93c = (35.0f + BREG_F(1)) * l_HIO.face_size;
    }

    cLib_addCalc2(&i_this->field_0x940, 1.0f, 1.0f, 0.01f);
    kuki_control1(i_this);
    kuki_control2(i_this);
    kuki_control3(i_this);
    cLib_addCalc2(&i_this->field_0x944, 1.0f, 1.0f, 0.05f);

    if (i_this->field_0x94c > 0.1f) {
        if (i_this->field_0x948 != 0) {
            i_this->yRot += i_this->field_0x948;
            cLib_addCalcAngleS2(&i_this->xRot, 0, 1, VREG_S(3) + 0x96);
        } else {
            i_this->xRot = i_this->field_0x94c * cM_scos((s16)i_this->field_0x94a);
        }

        i_this->field_0x94a += (s16)(10000 + VREG_S(2));
        cLib_addCalc0(&i_this->field_0x94c, 1.0f, VREG_F(2) + 150.0f);
    } else {
        i_this->xRot = 0;
    }

    cXyz eff_size(2.0f, 2.0f, 2.0f);
    setMidnaBindEffect(actor, &i_this->sound, &actor->eyePos, &eff_size);
}

static void e_gf_wait(e_gb_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz work;

    switch (i_this->flowerMode) {
        case 0:
            i_this->flowerMode = 1;
            break;

        case 1:
        default:
            break;
    }
}

static void e_gf_chance(e_gb_class* i_this) {
    fopEn_enemy_c* actor = &i_this->actor;
    cXyz work;
    f32 frame = i_this->flowerAnmP->getFrame();
    s8 bVar1 = 0;

    switch (i_this->flowerMode) {
        case 0:
            if (i_this->timer[0] == 30) {
                i_this->demoMode = 10;
            }

            if (i_this->timer[0] == 0) {
                body_anm_init(i_this, dRes_ID_E_GB_BCK_GF_OPEN_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                mDoAud_seStart(Z2SE_EN_GF_OPEN, &actor->home.pos, 0, 0);
                i_this->flowerMode = 1;
            }
            break;

        case 1:
            bVar1 = 1;
            if (i_this->flowerAnmP->isStop()) {
                body_anm_init(i_this, dRes_ID_E_GB_BCK_GF_WAIT_e, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->flowerMode = 2;
            }
            // fallthrough
        case 2:
            bVar1 = 1;
    }

    if (bVar1 && fpcM_Search(s_b_sub, i_this) != NULL) {
        i_this->flowerAction = FLOWER_ACTION_EATBOMB;
        i_this->flowerMode = 0;
    }
}

static void e_gf_eatbomb(e_gb_class* i_this) {
    static u16 eno[5] = {
        dPa_RM(ID_ZM_S_GF_BOMBDAMAGE00),
        dPa_RM(ID_ZM_S_GF_BOMBDAMAGE01),
        dPa_RM(ID_ZM_S_GF_BOMBDAMAGE02),
        dPa_RM(ID_ZM_S_GF_BOMBDAMAGE03),
        dPa_RM(ID_ZM_S_GF_BOMBDAMAGE04),
    };

    fopEn_enemy_c* actor = &i_this->actor;
    cXyz work;

    switch (i_this->flowerMode) {
        case 0:
            body_anm_init(i_this, dRes_ID_E_GB_BCK_GF_EAT_e, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            mDoAud_seStart(Z2SE_EN_GF_EAT, &actor->home.pos, 0, 0);
            i_this->flowerMode = 1;
            // fallthrough
        case 1:
            if (i_this->flowerAnmP->isStop()) {
                body_anm_init(i_this, dRes_ID_E_GB_BCK_GF_BURST_e, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                mDoAud_seStart(Z2SE_EN_GF_BURST, &actor->home.pos, 0, 0);
                i_this->flowerMode = 2;
            }
            break;

        case 2:
            if (i_this->flowerAnmP->isStop()) {
                i_this->flowerAction = FLOWER_ACTION_END;
                i_this->flowerMode = 0;
                i_this->brkAnmP->setPlaySpeed(1.0f);
            }
            break;
    }

    if (i_this->bodyAnmNo == dRes_ID_E_GB_BCK_GF_BURST_e) {
        for (int i = 0; i < 5; i++) {
            i_this->field_0xe7c[i] = dComIfGp_particle_set(i_this->field_0xe7c[i], eno[i], 
                                                           &actor->home.pos, NULL, NULL);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0xe7c[i]);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(i_this->flowerAnmP->getModel()->getAnmMtx(GF_JNT_FLOWER_5_e));
            }
        }
    }
}

static void e_gf_end(e_gb_class* i_this) {
    fopEn_enemy_c* actor = &i_this->actor;
    cXyz work;

    switch (i_this->flowerMode) {
        case 0:
            body_anm_init(i_this, dRes_ID_E_GB_BCK_GF_DIE_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            mDoAud_seStart(Z2SE_EN_GF_DIE, &actor->home.pos, 0, 0);
            i_this->flowerMode = 1;
            // fallthrough
        case 1:
            if (i_this->flowerAnmP->isStop()) {
                i_this->flowerMode = 3;
                i_this->timer[0] = 40;
            }
            break;

        case 3:
            if (i_this->timer[0] == 20) {
                int sw_bit = (fopAcM_GetParam(actor) & 0xFF0000) >> 16;
                dComIfGs_onSwitch(sw_bit, fopAcM_GetRoomNo(actor));
            }

            if (i_this->timer[0] == 0) {
                daKey_c* key_p = (daKey_c*)fopAcM_SearchByName(PROC_Obj_SmallKey);
                cXyz pos(actor->home.pos);
                pos.z += YREG_F(12);
                if (key_p != NULL) {
                    key_p->setPos(pos);
                    key_p->startMove(BREG_F(7) + 60.0f, BREG_F(8) + 4.0f);
                }

                fopAcM_createDisappear(actor, &pos, 20, 0, actor->field_0x564);
                fopAcM_delete(actor);
            }
    }
}

static void damage_check2(e_gb_class* i_this) {
    fopEn_enemy_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (i_this->flowerInvulnerabilityTimer == 0) {
        if (i_this->bodyCyl.ChkTgHit() != 0) {
            i_this->flowerInvulnerabilityTimer = 6;
            i_this->atInfo.mpCollider = i_this->bodyCyl.GetTgHitObj();
            def_se_set(&i_this->sound, i_this->atInfo.mpCollider, 0x2D, NULL);

            if (i_this->bodyAnmNo == dRes_ID_E_GB_BCK_GF_WAIT_CLOSE_e || i_this->bodyAnmNo == dRes_ID_E_GB_BCK_GF_BOUND_CLOSE_e) {
                body_anm_init(i_this, dRes_ID_E_GB_BCK_GF_BOUND_CLOSE_e, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                mDoAud_seStart(Z2SE_EN_GF_BOUND_CLOSE, &actor->home.pos, 0, 0);
            } else if (i_this->bodyAnmNo == dRes_ID_E_GB_BCK_GF_OPEN_e || i_this->bodyAnmNo == dRes_ID_E_GB_BCK_GF_WAIT_e || i_this->bodyAnmNo == dRes_ID_E_GB_BCK_GF_BOUND_e) {
                body_anm_init(i_this, dRes_ID_E_GB_BCK_GF_BOUND_e, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                mDoAud_seStart(Z2SE_EN_GF_BOUND, &actor->home.pos, 0, 0);
            }
        }

        if (i_this->bodyAnmNo == dRes_ID_E_GB_BCK_GF_BOUND_e && i_this->flowerAnmP->isStop()) {
            body_anm_init(i_this, dRes_ID_E_GB_BCK_GF_WAIT_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->flowerAnmP->setFrame(47.0f);
        } else if (i_this->bodyAnmNo == dRes_ID_E_GB_BCK_GF_BOUND_CLOSE_e && i_this->flowerAnmP->isStop()) {
            body_anm_init(i_this, dRes_ID_E_GB_BCK_GF_WAIT_CLOSE_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
    }
}

static void action2(e_gb_class* i_this) {
    fopEn_enemy_c* actor = &i_this->actor;
    
    damage_check2(i_this);

    s8 boss_room_wait_flag = 1;

    switch (i_this->flowerAction) {
        case FLOWER_ACTION_WAIT:
            e_gf_wait(i_this);
            break;

        case FLOWER_ACTION_CHANCE:
            e_gf_chance(i_this);
            boss_room_wait_flag = 0;
            break;

        case FLOWER_ACTION_EATBOMB:
            e_gf_eatbomb(i_this);
            boss_room_wait_flag = 0;
            break;

        case FLOWER_ACTION_END:
            e_gf_end(i_this);
            boss_room_wait_flag = 0;
    }

    if (boss_room_wait_flag != 0) {
        daPy_getPlayerActorClass()->onBossRoomWait();
    }

    if (i_this->keyStatus != 0) {
        daKey_c* key_p = (daKey_c*)fopAcM_SearchByName(PROC_Obj_SmallKey);
        if (key_p != NULL) {
            key_p->seStartTwinkle(Z2SE_OBJ_KEY_TWINKLE, &i_this->keyPos);
        }
    }
}

static void cam_3d_morf(e_gb_class* i_this, f32 param_2) {
    cLib_addCalc2(&i_this->demoCamCenter.x, i_this->demoCamCenterTarget.x, param_2, 
                  i_this->demoCamCenterTargetDist.x * i_this->demoCamStepScale);
    cLib_addCalc2(&i_this->demoCamCenter.y, i_this->demoCamCenterTarget.y, param_2, 
                  i_this->demoCamCenterTargetDist.y * i_this->demoCamStepScale);
    cLib_addCalc2(&i_this->demoCamCenter.z, i_this->demoCamCenterTarget.z, param_2, 
                  i_this->demoCamCenterTargetDist.z * i_this->demoCamStepScale);

    cLib_addCalc2(&i_this->demoCamEye.x, i_this->demoCamEyeTarget.x, param_2,
                  i_this->demoCamEyeTargetDist.x * i_this->demoCamStepScale);
    cLib_addCalc2(&i_this->demoCamEye.y, i_this->demoCamEyeTarget.y, param_2,
                  i_this->demoCamEyeTargetDist.y * i_this->demoCamStepScale);
    cLib_addCalc2(&i_this->demoCamEye.z, i_this->demoCamEyeTarget.z, param_2,
                  i_this->demoCamEyeTargetDist.z * i_this->demoCamStepScale);
}

static void demo_camera(e_gb_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera2 = dComIfGp_getCamera(0);
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz work, pos, sp34, sp40;
    int swBit;

    switch (i_this->demoMode) {
        case 1:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->demoMode = 2;
            i_this->demoCamFovy = 55.0f;
            i_this->demoCounter = 0;
            camera->mCamera.SetTrimSize(3);
            daPy_getPlayerActorClass()->changeOriginalDemo();
            i_this->keyPos.set(-5750.0f, 4410.0f, 10684.0f);
            i_this->keyYRot = KREG_S(4) + 0x5000;
            i_this->demoCamCenter.set(-5689.0f, 4747.0f, 15000.0f);
            i_this->demoCamEye.set(-5700.0f, 4740.0f, 14644.0f);
            i_this->demoCamCenterTarget.set(-5689.0f, 4740.0f, 14643.0f);
            i_this->demoCamEyeTarget.set(-5723.0f, 4733.0f, 14293.0f);
            i_this->demoCamEyeTargetDist.x = fabsf(i_this->demoCamEyeTarget.x - i_this->demoCamEye.x);
            i_this->demoCamEyeTargetDist.y = fabsf(i_this->demoCamEyeTarget.y - i_this->demoCamEye.y);
            i_this->demoCamEyeTargetDist.z = fabsf(i_this->demoCamEyeTarget.z - i_this->demoCamEye.z);
            i_this->demoCamCenterTargetDist.x = fabsf(i_this->demoCamCenterTarget.x - i_this->demoCamCenter.x);
            i_this->demoCamCenterTargetDist.y = fabsf(i_this->demoCamCenterTarget.y - i_this->demoCamCenter.y);
            i_this->demoCamCenterTargetDist.z = fabsf(i_this->demoCamCenterTarget.z - i_this->demoCamCenter.z);
            i_this->demoCamStepScale = 0.0;

            Z2GetAudioMgr()->setBattleBgmOff(true);
            dComIfGp_getEvent()->startCheckSkipEdge(actor);
            swBit = (fopAcM_GetParam(actor) & 0xFF00) >> 8;
            dComIfGs_onSwitch(swBit, fopAcM_GetRoomNo(actor));
            // fallthrough
        case 2:
            pos.set(-5700.0f, 4350.0f, 9670.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&pos, 0, 0);
            if (i_this->demoCounter > 50) {
                cam_3d_morf(i_this, 0.1f);
                cLib_addCalc2(&i_this->demoCamStepScale, 0.02f, 1.0f, 0.0005f);
            }

            if (i_this->demoCounter != 170) break;
            i_this->demoCamCenter.set(-5690.0f, 4432.0f, 9610.0f);
            i_this->demoCamEye.set(-5709.0f, 4445.0f, 9961.0f);
            i_this->demoCamCenterTarget.set(-5741.0f, 4432.0f, 10530.0f);
            i_this->demoCamEyeTarget.set(-5760.0f, 4496.0f, 10896.0f);
            i_this->demoCamEyeTargetDist.x = fabsf(i_this->demoCamEyeTarget.x - i_this->demoCamEye.x);
            i_this->demoCamEyeTargetDist.y = fabsf(i_this->demoCamEyeTarget.y - i_this->demoCamEye.y);
            i_this->demoCamEyeTargetDist.z = fabsf(i_this->demoCamEyeTarget.z - i_this->demoCamEye.z);
            i_this->demoCamCenterTargetDist.x = fabsf(i_this->demoCamCenterTarget.x - i_this->demoCamCenter.x);
            i_this->demoCamCenterTargetDist.y = fabsf(i_this->demoCamCenterTarget.y - i_this->demoCamCenter.y);
            i_this->demoCamCenterTargetDist.z = fabsf(i_this->demoCamCenterTarget.z - i_this->demoCamCenter.z);
            i_this->demoCamStepScale = 0.0f;
            i_this->demoMode = 3;
            i_this->demoCamFovy = 55.0f;
            i_this->demoCounter = 0;
            // fallthrough
        case 3:
            if (i_this->demoCounter > 30) {
                cam_3d_morf(i_this, 0.1f);
                cLib_addCalc2(&i_this->demoCamStepScale, 0.02f, 1.0f, 0.0005f);
            }

            if (i_this->demoCounter == 130) {
                i_this->mode++;
            }

            if (i_this->demoCounter == 135) {
                i_this->sound.startCreatureSound(Z2SE_EN_GB_SWINGHEAD, 0, -1);
            }

            if (i_this->demoCounter == 143) {
                i_this->field_0x670 = 2;
            }

            if (i_this->demoCounter == 162) {
                daPy_getPlayerActorClass()->changeDemoMode(23, 1, 2, 0);
            }

            if (i_this->demoCounter != 190) break;
            i_this->mode++;
            i_this->demoMode = 4;
            i_this->demoCounter = 0;
            body_anm_init(i_this, dRes_ID_E_GB_BCK_GF_OPEN_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            mDoAud_seStart(Z2SE_EN_GF_OPEN, &actor->home.pos, 0, 0);
            i_this->demoCamCenter.set(-5706.0f, 4526.0f, 11379.0f);
            i_this->demoCamEye.set(-5687.0f, 4505.0f, 11008.0f);
            i_this->demoCamCenterTarget.set(-5624.0f, 4439.0f, 9818.0f);
            i_this->demoCamEyeTarget.set(-5604.0f, 4418.0f, 9447.0f);
            i_this->demoCamEyeTargetDist.x = fabsf(i_this->demoCamEyeTarget.x - i_this->demoCamEye.x);
            i_this->demoCamEyeTargetDist.y = fabsf(i_this->demoCamEyeTarget.y - i_this->demoCamEye.y);
            i_this->demoCamEyeTargetDist.z = fabsf(i_this->demoCamEyeTarget.z - i_this->demoCamEye.z);
            i_this->demoCamCenterTargetDist.x = fabsf(i_this->demoCamCenterTarget.x - i_this->demoCamCenter.x);
            i_this->demoCamCenterTargetDist.y = fabsf(i_this->demoCamCenterTarget.y - i_this->demoCamCenter.y);
            i_this->demoCamCenterTargetDist.z = fabsf(i_this->demoCamCenterTarget.z - i_this->demoCamCenter.z);
            i_this->demoCamStepScale = 0.0f;
            i_this->demoCamCenter.y = 4584.0f;
            // fallthrough
        case 4:
            if (i_this->demoCounter == (s16)(NREG_S(5) + 48)) {
                head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_DEMO_DROPKEY_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->sound.startCreatureSound(Z2SE_EN_GB_DROPKEY, 0, -1);
            }

            if (i_this->demoCounter == (s16)(NREG_S(6) + 54)) {
                head_anm_init(i_this, dRes_ID_E_GB_BCK_GB_DEMO_DROPKEY_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->keyPos = actor->home.pos;
                i_this->keyPos.y += NREG_F(9) + 400.0f;
                i_this->field_0x670 = 1;
            }

            if (i_this->demoCounter == (s16)(NREG_S(7) + 68)) {
                body_anm_init(i_this, dRes_ID_E_GB_BCK_GF_CLOSE_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                mDoAud_seStart(Z2SE_EN_GF_CLOSE, &actor->home.pos, 0, 0);
                i_this->keyStatus = 0;
            }

            if (i_this->demoCounter > 150) {
                cam_3d_morf(i_this, 0.2f);
                cLib_addCalc2(&i_this->demoCamStepScale, 0.04f, 1.0f, 0.002f);
            } else if (i_this->demoCounter >= 80) {
                cLib_addCalc2(&i_this->demoCamCenter.y, 4526.0f, 0.05f, 1.0f);
            }

            if (i_this->bodyAnmNo == dRes_ID_E_GB_BCK_GF_CLOSE_e && i_this->flowerAnmP->isStop()) {
                body_anm_init(i_this, dRes_ID_E_GB_BCK_GF_WAIT_CLOSE_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }

            if (i_this->demoCounter == 220) {
                i_this->demoMode = 100;
                i_this->headAction = HEAD_ACTION_WAIT;
                i_this->mode = 0;
                i_this->flowerAction = FLOWER_ACTION_WAIT;
                i_this->flowerMode = 0;
                Z2GetAudioMgr()->setBattleBgmOff(false);
            }
            break;

        case 10:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->demoMode = 11;
            i_this->demoCamFovy = 55.0;
            i_this->demoCounter = 0;
            camera->mCamera.SetTrimSize(3);
            // fallthrough
        case 11:
            i_this->demoCamCenter = actor->home.pos;
            i_this->demoCamCenter.y += YREG_F(12) + 100.0f;
            i_this->demoCamEye = actor->home.pos;
            i_this->demoCamEye.x += YREG_F(13);
            i_this->demoCamEye.y += YREG_F(14) + 300.0f;
            i_this->demoCamEye.z += YREG_F(15) + -600.0f;

            if (i_this->demoCounter == 100) {
                i_this->demoMode = 100;
            }
            break;

        case 100:
            camera->mCamera.Reset(i_this->demoCamCenter, i_this->demoCamEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            i_this->demoMode = 0;
            break;

        case 0:
            break;
    }

    if (i_this->demoMode != 0) {
        camera->mCamera.Set(i_this->demoCamCenter, i_this->demoCamEye, i_this->demoCamFovy, 0);
        i_this->demoCounter++;

        if (i_this->demoMode < 10) {
            dComIfGp_getEvent()->setSkipProc(i_this, dEv_defaultSkipProc, 0);
            if (dComIfGp_getEvent()->checkSkipEdge()) {
                i_this->demoMode = 100;
                cMtx_YrotS(*calc_mtx, player->shape_angle.y);
                work.x = 0.0;
                work.y = JREG_F(1) + 100.0f;
                work.z = JREG_F(2) + -250.0f;
                MtxPosition(&work, &i_this->demoCamEye);
                i_this->demoCamEye += player->current.pos;
                i_this->demoCamCenter = player->current.pos;
                i_this->demoCamCenter.y += 120.0f;
                i_this->headAction = HEAD_ACTION_WAIT;
                i_this->mode = 0;
                i_this->flowerAction = FLOWER_ACTION_WAIT;
                i_this->flowerMode = 0;
                body_anm_init(i_this, dRes_ID_E_GB_BCK_GF_WAIT_CLOSE_e, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->keyModelP = NULL;
                i_this->keyXRot = 0;
                i_this->keyStatus = 0;
                Z2GetAudioMgr()->setBattleBgmOff(false);
            }
        }
    }
}

static int daE_GB_Execute(e_gb_class* i_this) {
    static u16 key_eno[2] = {
        dPa_RM(ID_ZM_S_KEYLIGHT00),
        dPa_RM(ID_ZM_S_KEYLIGHT01),
    };

    fopEn_enemy_c* actor = &i_this->actor;
    cXyz work, pos;

    i_this->frameCounter++;

    for (int i = 0; i < 5; i = i + 1) {
        if (i_this->timer[i] != 0) {
            i_this->timer[i]--;
        }
    }

    if (i_this->invulnerabilityTimer != 0) {
        i_this->invulnerabilityTimer--;
    }

    if (i_this->flowerInvulnerabilityTimer != 0) {
        i_this->flowerInvulnerabilityTimer--;
    }

    action(i_this);

    if (i_this->field_0x668 == 0) {
        i_this->sound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    }

    action2(i_this);
    mDoMtx_stack_c::transS(actor->current.pos.x,actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->yRot);
    mDoMtx_stack_c::XrotM(i_this->xRot);
    mDoMtx_stack_c::YrotM(-i_this->yRot);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::XrotM(actor->shape_angle.x);
    mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
    f32 scale = l_HIO.face_size;
    mDoMtx_stack_c::scaleM(scale, scale, scale);

    J3DModel* model = i_this->anmP->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->anmP->play(NULL, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)), 0);
    i_this->anmP->modelCalc();
    MTXCopy(model->getAnmMtx((int)BREG_S(0) + GB_JNT_MOUTH_1_e), *calc_mtx);

    if (i_this->status != 0) {
        work.set(BREG_F(3) + 20.0f, BREG_F(4), BREG_F(5) + -50.0f);
        MtxPosition(&work, &actor->eyePos);
        actor->attention_info.position = actor->eyePos;
        actor->attention_info.position.y += BREG_F(6) + 50.0f;

        JPABaseEmitter* baseEmitter;
        if (i_this->headAnmNo == dRes_ID_E_GB_BCK_GB_REVIVE_e) {
            i_this->field_0xe6c = dComIfGp_particle_set(i_this->field_0xe6c, dPa_RM(ID_ZM_S_GB_REVIVESLAVER00), &actor->current.pos, NULL, NULL);

            baseEmitter = dComIfGp_particle_getEmitter(i_this->field_0xe6c);
            if (baseEmitter != NULL) {
                baseEmitter->setGlobalSRTMatrix(model->getAnmMtx(GB_JNT_MOUTH_1_e));
            }

            if (i_this->anmP->checkFrame(8.0f)) {                
                i_this->sound.startCreatureVoice(Z2SE_EN_GB_V_NODAMAGE, -1);
            }
        }

        if (i_this->headAnmNo == dRes_ID_E_GB_BCK_GB_DAMAGE_A_e) {
            i_this->field_0xe70 = dComIfGp_particle_set(i_this->field_0xe70, dPa_RM(ID_ZM_S_GB_DAMAGESLAVER00), &actor->current.pos, NULL, NULL);

            baseEmitter = dComIfGp_particle_getEmitter(i_this->field_0xe70);
            if (baseEmitter != NULL) {
                baseEmitter->setGlobalSRTMatrix(model->getAnmMtx(GB_JNT_MOUTH_1_e));
            }
        }

        if (i_this->headAnmNo == dRes_ID_E_GB_BCK_GB_WAIT_e && i_this->anmP->checkFrame(1.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_GB_V_WAIT, -1);
        }
    }

    work.set(BREG_F(6) + 50.0f, BREG_F(7), BREG_F(8));
    MtxPosition(&work, &pos);

    if (i_this->invulnerabilityTimer != 0) {
        pos.x += 10000.0f;
    }

    i_this->headSph.SetC(pos);
    if (i_this->distToPlayer > 500.0f && daPy_getPlayerActorClass()->checkBoomerangChargeTime() != 0) {
        i_this->headSph.SetR((BREG_F(9) + 160.0f) * l_HIO.face_size);
    } else {
        i_this->headSph.SetR((BREG_F(9) + 80.0f) * l_HIO.face_size);
    }

    dComIfG_Ccsp()->Set(&i_this->headSph);
    if (i_this->atSetBitFlag != 0) {
        i_this->headSph.OnAtSetBit();
        i_this->atSetBitFlag = 0;
    } else {
        i_this->headSph.OffAtSetBit();
    }

    mDoMtx_stack_c::transS(actor->home.pos.x, actor->home.pos.y, actor->home.pos.z);
    mDoMtx_stack_c::YrotM(actor->home.angle.y);
    scale = l_HIO.flower_size;
    mDoMtx_stack_c::scaleM(scale, scale, scale);
    model = i_this->flowerAnmP->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->flowerAnmP->play(NULL, 0, 0);
    i_this->brkAnmP->play();
    i_this->flowerAnmP->modelCalc();

    if ((i_this->bodyAnmNo == dRes_ID_E_GB_BCK_GF_WAIT_e || i_this->bodyAnmNo == dRes_ID_E_GB_BCK_GF_WAIT_CLOSE_e) && i_this->flowerAnmP->checkFrame(1.0f)) {
        mDoAud_seStart(Z2SE_EN_GF_WAIT, &actor->home.pos, 0, 0);
    }

    if (i_this->status == 0) {
        actor->eyePos = actor->home.pos;
        actor->eyePos.y += BREG_F(17) + 150.0f;
        actor->attention_info.position = actor->eyePos;
        actor->attention_info.position.y += BREG_F(18) + 50.0f;
    }

    i_this->bodyCyl.SetC(actor->home.pos);
    i_this->bodyCyl.SetR((JREG_F(9) + 170.0f) * l_HIO.flower_size);
    i_this->bodyCyl.SetH((JREG_F(10) + 100.0f) * l_HIO.flower_size);
    dComIfG_Ccsp()->Set(&i_this->bodyCyl);

    if (i_this->keyModelP != NULL) {
        if (i_this->field_0x670 == 1) {
            i_this->keyPos.y += i_this->field_0x680;
            i_this->field_0x680 -= 3.0f;
            i_this->keyXRot += (s16)-0xC00;

            if (i_this->keyPos.y < actor->home.pos.y) {
                i_this->keyXRot = 0;
                i_this->keyModelP = NULL;
            }
        } else if (i_this->field_0x670 == 2) {
            MTXCopy(model->getAnmMtx(GF_JNT_FLOWER_5_e), *calc_mtx);
            work.set(BREG_F(13) + -30.0f, BREG_F(14), BREG_F(15));
            MtxPosition(&work, &i_this->keyPos);
        }

        if (i_this->keyModelP != NULL) {
            mDoMtx_stack_c::transS(i_this->keyPos.x, i_this->keyPos.y, i_this->keyPos.z);
            mDoMtx_stack_c::YrotM(i_this->keyYRot);
            mDoMtx_stack_c::XrotM(i_this->keyXRot);
            mDoMtx_stack_c::XrotM(0x4000);
            scale = BREG_F(16) + 1.5f;
            mDoMtx_stack_c::scaleM(scale, scale, scale);
            i_this->keyModelP->setBaseTRMtx(mDoMtx_stack_c::get());
            pos = i_this->keyPos;
            pos.y += YREG_F(13);
            for (int i = 0; i < 2; i++) {
                i_this->field_0xe62[i] = dComIfGp_particle_set(i_this->field_0xe62[i], key_eno[i], &pos, NULL, NULL);
            }
        }
    }

    f32 fVar2 = i_this->field_0x964 * (YREG_F(4) + 0.001f);

    for (int i = 0; i < 3; i++) {
        if (i_this->field_0x92c[i] == 0) {
            if (i_this->field_0x938 == 2) {
                i_this->field_0x938 = 0;
                i_this->field_0x92c[i] = 2;
                i_this->field_0x932[i] = 17;
                i_this->field_0x928 = YREG_F(8) + 2.0f;
                i_this->field_0x924 = 4000;
                break;
            }
        } else {
            if (i_this->field_0x92c[i] == 1) {
                if (i_this->field_0x935[i] == 0) {
                    i_this->field_0x935[i] = YREG_S(5) + 1;
                    i_this->field_0x900[i_this->field_0x932[i]] = 1;
                    i_this->field_0x932[i]++;
                    if (i_this->field_0x932[i] >= 18) {
                        i_this->field_0x92c[i] = 0;
                    }
                }
            } else if (i_this->field_0x935[i] == 0) {
                i_this->field_0x935[i] = YREG_S(5) + 1;
                i_this->field_0x900[i_this->field_0x932[i]] = 1;
                i_this->field_0x932[i]--;
                if (i_this->field_0x932[i] < 0) {
                    i_this->field_0x92c[i] = 0;
                }
            }

            if (i_this->field_0x935[i] != 0) {
                i_this->field_0x935[i]--;
            }
        }
    }

    for (int i = 0; i < 18; i = i + 1) {
        if (i_this->stemModelP[i] != NULL) {
            if (i_this->field_0x900[i] != 0) {
                i_this->field_0x900[i] +=i_this->field_0x924;

                if (i_this->field_0x900[i] < 0) {
                    i_this->field_0x900[i] = 0;
                }

                i_this->stemModelSize[i] = i_this->field_0x928 * cM_ssin((s16)i_this->field_0x900[i]) + 1.0f;
            }

            MtxTrans(i_this->field_0x6e4[i].x, i_this->field_0x6e4[i].y, i_this->field_0x6e4[i].z, 0);
            cMtx_XrotM(*calc_mtx, i_this->field_0x7bc[i].x);
            cMtx_YrotM(*calc_mtx, i_this->field_0x7bc[i].y + 0x8000);
            MtxScale(l_HIO.face_size * i_this->stemModelSize[i], l_HIO.face_size * i_this->stemModelSize[i], l_HIO.face_size + fVar2, 1);
            MtxTrans(0.0f, 0.0f, XREG_F(4), 1);
            i_this->stemModelP[i]->setBaseTRMtx(*calc_mtx);
        }
    }

    demo_camera(i_this);
    return 1;
}

static int daE_GB_IsDelete(e_gb_class* i_this) {
    return 1;
}

static int daE_GB_Delete(e_gb_class* i_this) {
    "Delete -> E_GB(id=%d)\n";

    fopEn_enemy_c* actor = &i_this->actor;
    fpc_ProcID id = fopAcM_GetID(i_this);

    dComIfG_resDelete(&i_this->phase, "E_gb");
    if (i_this->hioInit != 0) {
        data_806C7928 = 0;
        mDoHIO_DELETE_CHILD(l_HIO.no);
    }

    if (actor->heap != NULL && i_this->field_0x668 == 0) {
        i_this->sound.stopAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    e_gb_class* i_this = (e_gb_class*)actor;

    i_this->anmP = new mDoExt_McaMorf(static_cast<J3DModelData*>(dComIfG_getObjectRes("E_gb", dRes_INDEX_E_GB_BMD_GB_e)), NULL, NULL,
                                              static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("E_gb", dRes_INDEX_E_GB_BCK_GB_WAIT_e)), 2, 1.0f, 
                                              0, -1, 1, NULL, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
    if (i_this->anmP == NULL || i_this->anmP->getModel() == NULL) {
        return 0;
    }

    i_this->flowerAnmP = new mDoExt_McaMorf(static_cast<J3DModelData*>(dComIfG_getObjectRes("E_gb", dRes_INDEX_E_GB_BMD_GF_e)), NULL, NULL,
                                              static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("E_gb", dRes_INDEX_E_GB_BCK_GF_WAIT_CLOSE_e)), 2, 1.0f,
                                              0, -1, 1, NULL, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
    i_this->bodyAnmNo = dRes_ID_E_GB_BCK_GF_WAIT_CLOSE_e;
    if (i_this->flowerAnmP == NULL || i_this->flowerAnmP->getModel() == NULL) {
        return 0;
    }

    i_this->brkAnmP = new mDoExt_brkAnm();
    if (i_this->brkAnmP == NULL) {
        return 0;
    }

    if (i_this->brkAnmP->init(i_this->flowerAnmP->getModel()->getModelData(),
                              static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("E_gb", dRes_INDEX_E_GB_BRK_GF_DIE_e)), 
                               1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }
    i_this->brkAnmP->setPlaySpeed(0.0f);

    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("E_gb", dRes_INDEX_E_GB_BMD_GS_e));
    JUT_ASSERT(3378, modelData != NULL);

    for (int i = 0; i < 18; i++) {
        i_this->stemModelP[i] = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
        if (i_this->stemModelP[i] == NULL) {
            return 0;
        }

        i_this->stemModelSize[i] = 1.0f;
    }

    if (strcmp(dComIfGp_getStartStageName(), "D_MN05") == 0) {
        modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("E_gb", dRes_INDEX_E_GB_BMD_GB_KEY_e));
        JUT_ASSERT(3395, modelData != NULL);
        i_this->keyModelP = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
        if (i_this->keyModelP == NULL) {
            return 0;
        }
    }

    return 1;
}

static cPhs__Step daE_GB_Create(fopAc_ac_c* actor) {
    static dCcD_SrcSph head_cc_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xd}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
            {dCcD_SE_HARD_BODY, 0x0, 0x1, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };

    static dCcD_SrcCyl body_cyl_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x79}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x3}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {
                {0.0f, 0.0f, 0.0f}, // mCenter
                30.0f, // mRadius
                20.0f // mHeight
            } // mCyl
        }
    };

    e_gb_class* i_this = (e_gb_class*)actor;
    fopAcM_ct(&i_this->actor, e_gb_class);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->phase, "E_gb");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_gb PARAM %x\n", fopAcM_GetParam(actor));
        u8 sw_bit = (fopAcM_GetParam(actor) & 0xFF0000) >> 16;
        if (sw_bit != 0xFF) {
            if (dComIfGs_isSwitch(sw_bit, fopAcM_GetRoomNo(actor))) {
                OS_REPORT("E_gb やられ後なので再セットしません\n");
                return cPhs_ERROR_e;
            }
        }

        i_this->arg0 = fopAcM_GetParam(actor);
        i_this->arg1 = (fopAcM_GetParam(actor) & 0xFF00) >> 8;
        i_this->arg2 = (fopAcM_GetParam(actor) & 0xF0000) >> 16;
        i_this->arg3 = (fopAcM_GetParam(actor) & 0xF00000) >> 20;
        OS_REPORT("E_gb //////////////E_GB SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(actor, useHeapInit, 0x4B000)) {
            OS_REPORT("//////////////E_GB SET NON !!\n");
            return cPhs_ERROR_e;
        }
        OS_REPORT("//////////////E_GB SET 2 !!\n");

        if (data_806C7928 == 0) {
            i_this->hioInit = 1;
            data_806C7928 = 1;
            l_HIO.no = mDoHIO_CREATE_CHILD("デカババ", &l_HIO);
        }

        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        actor->field_0x560 = actor->health = 90;

        i_this->stts.Init(253, 0, actor);
        i_this->headSph.Set(head_cc_sph_src);
        i_this->headSph.SetStts(&i_this->stts);
        i_this->bodyCyl.Set(body_cyl_src);
        i_this->bodyCyl.SetStts(&i_this->stts);
        i_this->bodyCyl.OnTgNoHitMark();

        i_this->objAcch.Set(fopAcM_GetPosition_p(actor), fopAcM_GetOldPosition_p(actor), actor, 1, &i_this->acchcir,
                            fopAcM_GetSpeed_p(actor), NULL, NULL);
        i_this->acchcir.SetWall(150.0f, 150.0f);

        i_this->atInfo.mpSound = &i_this->sound;
        i_this->atInfo.mPowerType = 8;

        i_this->sound.init(&actor->current.pos, &actor->eyePos, 3, 1);
        i_this->sound.setEnemyName("E_gb");
        
        i_this->frameCounter = cM_rndF(65535.0f);

        u32 sw_bit_2 = (fopAcM_GetParam(actor) & 0xFF00) >> 8;
        if (fopAcM_GetRoomNo(actor) == 7 && !dComIfGs_isSwitch(sw_bit_2, fopAcM_GetRoomNo(actor))) {
            i_this->headAction = HEAD_ACTION_START;
        } else {
            i_this->headAction = HEAD_ACTION_WAIT;
        }

        i_this->status = 1;
        i_this->keyStatus = 1;
        daE_GB_Execute(i_this);

        Z2GetAudioMgr()->muteSceneBgm(33, 0.0f);
    }

    return phase;
}

e_gb_class::e_gb_class() {}

static actor_method_class l_daE_GB_Method = {
    (process_method_func)daE_GB_Create,
    (process_method_func)daE_GB_Delete,
    (process_method_func)daE_GB_Execute,
    (process_method_func)daE_GB_IsDelete,
    (process_method_func)daE_GB_Draw,
};

actor_process_profile_definition g_profile_E_GB = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_GB,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_gb_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  139,                    // mPriority
  &l_daE_GB_Method,       // sub_method
  0x00040000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
