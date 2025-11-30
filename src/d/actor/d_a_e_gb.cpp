/**
 * @file d_a_e_gb.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_gb.h"
#include "d/d_cc_d.h"
#include "d/d_camera.h"
#include "d/d_bomb.h"
#include "d/actor/d_a_obj_smallkey.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"

class daE_GB_HIO_c : public JORReflexible{
public:
    daE_GB_HIO_c();
    virtual ~daE_GB_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ s16 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ s16 field_0x1c;
    /* 0x1E */ s16 field_0x1e;
};


daE_GB_HIO_c::daE_GB_HIO_c() {
    field_0x4 = -1;
    field_0x8 = 1.0f;
    field_0xc = 1.0f;
    field_0x10 = 1.0f;
    field_0x14 = 15;
    field_0x18 = 65.0f;
    field_0x1c = 12;
    field_0x1e = 10;
}

static void head_anm_init(e_gb_class* i_this, int param_2, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->mpModelMorf1->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gb", param_2), i_mode,
                                 i_morf, i_speed, 0.0f, -1.0f, NULL);
    i_this->field_0x688 = param_2;
}

static void body_anm_init(e_gb_class* i_this, int param_2, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->mpModelMorf2->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gb", param_2), i_mode,
                                 i_morf, i_speed, 0.0f, -1.0f, NULL);
    i_this->field_0x68c = param_2;
}

static int daE_GB_Draw(e_gb_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;

    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);

    J3DModel* model = i_this->mpModelMorf2->getModel();
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);

    J3DModelData* modelData = model->getModelData();
    i_this->mBrkAnm->entry(modelData);
    i_this->mpModelMorf2->entryDL();

    if (i_this->field_0x66c != NULL) {
        g_env_light.setLightTevColorType_MAJI(i_this->field_0x66c, &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->field_0x66c);
    }

    if (i_this->field_0x968 != 0) {
        if (i_this->field_0x828[17] != NULL) {
            model = i_this->mpModelMorf1->getModel();
            g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);
            i_this->mpModelMorf1->entryDL();

            for (int i = 0; i < 17; i++) {
                if (i_this->field_0x828[i] != NULL) {
                    g_env_light.setLightTevColorType_MAJI(i_this->field_0x828[i], &a_this->tevStr);
                    mDoExt_modelUpdateDL(i_this->field_0x828[i]);
                }
            }
        }

        cXyz sp38;
        sp38.set(a_this->current.pos.x, a_this->current.pos.y + 100.0f, a_this->current.pos.z);
        i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &sp38, 1400.0f, 0.0f, a_this->current.pos.y,
                                                 i_this->mObjAcch.GetGroundH(), i_this->mObjAcch.m_gnd,
                                                 &a_this->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

        for (int i = 0; i < 17; i++) {
            if (i_this->field_0x828[i] != NULL) {
                dComIfGd_addRealShadow(i_this->mShadowKey, i_this->field_0x828[i]);
            }
        }
    }

    return 1;
}

static u8 data_806C7928;

static daE_GB_HIO_c l_HIO;

static void e_gb_wait(e_gb_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp5c, sp68;
    f32 fVar1 = (a_this->home.pos - dComIfGp_getPlayer(0)->current.pos).abs();
    s8 sVar1 = 0;

    switch (i_this->field_0x69c) {
        case -2:
            head_anm_init(i_this, 16, 3.0f, 0, 1.0f);
            i_this->field_0x6c4[1] = l_HIO.field_0x14 + cM_rndF(l_HIO.field_0x14);
            i_this->field_0x69c = 1;
            i_this->field_0x6c0 = 1.0f;
            break;

        case 0:
            head_anm_init(i_this, 17, 10.0f, 2, 1.0f);
            i_this->field_0x69c = 1;
            i_this->field_0x6c4[1] = l_HIO.field_0x14 + cM_rndF(l_HIO.field_0x14);
            // fallthrough
        case 1:
            if (i_this->field_0x688 != 17 && i_this->mpModelMorf1->isStop()) {
                head_anm_init(i_this, 17, 10.0f, 2, 1.0f);
            }

            if (i_this->field_0x6c4[1] == 0 && fVar1 < 1000.0f) {
                sVar1 = 1;
                OS_REPORT("E_gb NEAR ATTACK d\n");
            }

            if (i_this->field_0x6c4[0] == 0) {
                i_this->field_0x6c4[0] = cM_rndF(30.0f) + 10.0f;
                cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
                sp5c.x = cM_rndFX(100.0f);
                sp5c.y = l_HIO.field_0x8 * 450.0f + cM_rndFX(100.0f);
                sp5c.z = cM_rndFX(100.0f) + 200.0f;
                MtxPosition(&sp5c, &i_this->field_0x6a8);
                i_this->field_0x6a8 += i_this->field_0x6d4;
                i_this->field_0x6c0 = 0.0f;

                if (i_this->field_0x6c4[1] == 0 && fVar1 < 1500.0f) {
                    sVar1 = 1;
                }
            }

            sp68.x = cM_ssin(i_this->field_0x698 * 0x76C) * 30.0f;
            sp68.y = cM_ssin(i_this->field_0x698 * 0x898) * 30.0f;
            sp68.z = cM_scos(i_this->field_0x698 * 2000) * 30.0f;
            cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x6a8.x + sp68.x, 0.1f, i_this->field_0x6c0 * 30.0f);
            cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x6a8.z + sp68.z, 0.1f, i_this->field_0x6c0 * 30.0f);
            cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x6a8.y + sp68.y, 0.1f, i_this->field_0x6c0 * 30.0f);
            cLib_addCalc2(&i_this->field_0x6c0, 1.0f, 1.0f, 0.05f);
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x6b8, 8, 0x100);
    cLib_addCalcAngleS2(&a_this->shape_angle.y, i_this->field_0x6b8, 8, 0x800);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, i_this->field_0x6ba, 8, 0x400);

    if (sVar1 != 0) {
        a_this->speedF = 0.0f;
        i_this->field_0x69a = 1;
        i_this->field_0x69c = 0;
    }
}

static void e_gb_attack_1(e_gb_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp58, sp64;
    f32 fVar2 = 0.0f;
    f32 fVar3 = 20.0f;
    f32 fVar1 = (a_this->home.pos - dComIfGp_getPlayer(0)->current.pos).abs();

    switch (i_this->field_0x69c) {
        case 0:
            if (fVar1 < 1200.0f && cM_rndF(1.0f) < 0.5f) {
                i_this->field_0x69a = 2;
                i_this->field_0x69c = 0;
                i_this->field_0x6c4[0] = 0;
                return;
            }

            head_anm_init(i_this, 5, 5.0f, 0, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_GB_V_ATTACK, -1);
            i_this->field_0x69c = 1;
            i_this->field_0x6c0 = 0.0f;
            break;

        case 1:
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x6b8, 2, 0x800);
            cLib_addCalcAngleS2(&a_this->current.angle.x, i_this->field_0x6ba, 2, 0x800);
            cMtx_YrotS(*calc_mtx, a_this->current.angle.y);

            sp58.x = 0.0f;
            sp58.y = l_HIO.field_0x8 * 450.0f;
            sp58.z = 0.0f;
            MtxPosition(&sp58, &sp64);
            i_this->field_0x6a8 = i_this->field_0x6d4 + sp64;

            cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x6a8.x, 0.1f, i_this->field_0x6c0 * 30.0f);
            cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x6a8.z, 0.1f, i_this->field_0x6c0 * 30.0f);
            cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x6a8.y, 0.1f, i_this->field_0x6c0 * 30.0f);
            cLib_addCalc2(&i_this->field_0x6c0, 1.0f, 1.0f, 0.1f);

            if (i_this->mpModelMorf1->isStop()) {
                i_this->field_0x69c = 3;
                i_this->field_0x6c4[0] = l_HIO.field_0x1c;
                head_anm_init(i_this, 8, 1.0f, 0, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_GB_V_ATTACK_DASH, -1);
            }
            break;

        case 3:
            fVar2 = l_HIO.field_0x18;
            fVar3 = l_HIO.field_0x18 * 0.5f;
            if (i_this->field_0x6c4[0] == 0) {
                i_this->field_0x6c4[0] = l_HIO.field_0x1e;
                i_this->field_0x69c = 4;
            }
            break;

        case 4:
            if (i_this->field_0x6c4[0] == 0) {
                i_this->field_0x69a = 0;
                i_this->field_0x69c = 0;
                i_this->field_0x6c4[0] = cM_rndF(10.0f) + 10.0f;
                i_this->field_0x6c0 = 0.0f;
                return;
            }
            break;

        case 5:
            fVar3 = 5.0f;
            if (i_this->field_0x6c4[0] == 0 && fabsf(a_this->speedF) < 0.1f) {
                i_this->field_0x69a = 0;
                i_this->field_0x69c = 0;
                i_this->field_0x6c4[0] = cM_rndF(10.0f) + 10.0f;
                i_this->field_0x6c0 = 0.0f;
            }
    }

    cLib_addCalc2(&a_this->speedF, fVar2, 1.0f, fVar3);
    if (a_this->speedF >= 10.0f) {
        i_this->field_0xc20 = 1;
        if (i_this->field_0x69c != 5 && (i_this->mObjAcch.ChkWallHit() != 0 || i_this->mHeadSph.ChkAtShieldHit())) {
            i_this->field_0x69c = 5;
            i_this->field_0x948 = 5000;
            i_this->field_0x952 = 3000;
            i_this->field_0x94c = 3000.0f;

            head_anm_init(i_this, 17, 2.0f, 2, 1.0f);

            i_this->field_0x6c4[0] = 0;
            a_this->speedF = -30.0f;
        }
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 8, 0x800);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 8, 0x400);
}

static void e_gb_attack_2(e_gb_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp60, sp6c;
    f32 fVar1 = 0.0f;
    f32 frame = i_this->mpModelMorf1->getFrame();

    switch (i_this->field_0x69c) {
        case 0:
            head_anm_init(i_this, 7, 5.0f, 2, 1.0f);
            i_this->field_0x6c4[0] = cM_rndF(10.0f) + 49.0f;
            i_this->field_0x69c = 1;
            i_this->mSound.startCreatureSound(Z2SE_EN_GB_V_DOSUN, 0, -1);
            break;

        case 1:
            a_this->speedF = 0.0f;
            a_this->speed.y = 0.0f;

            cLib_addCalc2(&a_this->current.pos.y, a_this->home.pos.y + 200.0f, 0.05f, 3.0f);
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x6b8, 2, 0x800);
            cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 2, 0x800);
            cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 8, 0x800);
            cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 8, 0x400);

            if (i_this->field_0x6c4[0] == 0) {
                head_anm_init(i_this, 6, 2.0f, 0, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_GB_V_ATTACK, -1);
                i_this->mSound.startCreatureSound(Z2SE_EN_GB_DOSUN_DASH, 0, -1);
                i_this->field_0x69c = 2;
            }
            break;

        case 2:
            sp60.x = player->current.pos.x - a_this->current.pos.x;
            sp60.z = player->current.pos.z - a_this->current.pos.z;
            if (frame >= 16.0f) {
                a_this->speedF = 0.0f;
                fVar1 = 0.0f;
            } else if (frame >= 6.0f) {
                fVar1 = 50.0f;
                if (frame == 6.0f) {
                    a_this->speed.y = 50.0f;
                }

                if (JMAFastSqrt(sp60.x * sp60.x + sp60.z * sp60.z) < 100.0f) {
                    a_this->speedF = 0.0f;
                    fVar1 = 0.0f;
                }
            } else {
                a_this->speed.y = 0.0f;
            }

            if (frame >= 10.0f && frame <= 20.0f) {
                i_this->field_0xc20 = 1;
            }

            a_this->speedF = fVar1;
            if (a_this->speed.y <= 0.0f && a_this->current.pos.y <= (a_this->home.pos.y + 150.0f)) {
                a_this->current.pos.y = a_this->home.pos.y + 150.0f;
                if (a_this->speed.y < -30.0f) {
                    fopAcM_effSmokeSet1(&i_this->field_0xe74, &i_this->field_0xe78, &a_this->eyePos, NULL, 1.7f, &a_this->tevStr, 1);
                    dComIfGp_getVibration().StartShock(5, 15, cXyz(0.0f, 1.0f, 0.0f));
                    i_this->mSound.startCreatureSound(Z2SE_EN_GB_DOSUN, 0, -1);
                }

                a_this->speed.y = 0.0f;
                if (i_this->mpModelMorf1->isStop()) {
                    sp60 = a_this->current.pos - a_this->home.pos;
                    if (cM_rndF(1.0f) < 0.5f && sp60.abs() < 1000.0f) {
                        i_this->field_0x69c = 0;
                    } else {
                        i_this->field_0x69a = 0;
                        i_this->field_0x69c = 0;
                        i_this->field_0x6c4[0] = cM_rndF(10.0f) + 10.0f;
                        i_this->field_0x6c0 = 0.0f;
                    }
                }
            }
    }
}

static void e_gb_damage(e_gb_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    cXyz sp24;
    s16 sVar1;

    i_this->field_0x6ce = 35;
    switch (i_this->field_0x69c) {
        case 0:
            i_this->field_0x69c = 1;
            sVar1 = a_this->current.angle.y;
            if ((s16)(i_this->field_0x6b8 - sVar1) < 0) {
                a_this->current.angle.y = sVar1 + 0x2000;
            } else {
                a_this->current.angle.y = sVar1 - 0x2000;
            }

            cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
            sp24.x = 0.0f;
            sp24.y = 50.0f;
            sp24.z = 800.0f;
            MtxPosition(&sp24, &i_this->field_0x6a8);
            i_this->field_0x6a8 += a_this->home.pos;
            a_this->speed.x = fabsf(a_this->current.pos.x - i_this->field_0x6a8.x) * 0.2f;
            a_this->speed.y = fabsf(a_this->current.pos.y - i_this->field_0x6a8.y) * 0.2f;
            a_this->speed.z = fabsf(a_this->current.pos.z - i_this->field_0x6a8.z) * 0.2f;
            i_this->field_0x6c4[0] = 15;
            // fallthrough
        case 1:
            if (i_this->field_0x6c4[0] == 0) {
                if (a_this->health <= 0) {
                    i_this->field_0x69a = 5;
                    i_this->field_0x69c = 0;
                } else {
                    i_this->field_0x69a = 0;
                    i_this->field_0x69c = -2;
                }
            }
    }

    cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x6a8.x, 0.1f, a_this->speed.x);
    cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x6a8.y, 0.1f, a_this->speed.y);
    cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x6a8.z, 0.1f, a_this->speed.z);
    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, 0, 4, 0x800);
}

static void e_gb_end(e_gb_class* i_this) {
    static u16 eno[2] = {
        0x832F,
        0x8330,
    };

    fopEn_enemy_c* a_this = &i_this->actor;

    i_this->field_0x6ce = 20;
    switch (i_this->field_0x69c) {
        case 0:
            head_anm_init(i_this, 15, 30.0f, 0, 1.0f);
            i_this->field_0x69c = 1;
            break;

        case 1:
            i_this->field_0x69c = 2;
            i_this->mSound.startCreatureSound(Z2SE_EN_GB_BOMBROOT, 0, -1);
            break;

        case 2:
            if ((i_this->field_0x698 & 1) == 0) {
                i_this->field_0x828[i_this->field_0x939] = NULL;
                if (i_this->field_0x939 == 17) {
                    i_this->field_0x69c = 10;
                    fopAcM_createDisappear(a_this, &a_this->eyePos, 12, 0, a_this->field_0x564);
                    i_this->field_0x968 = 0;
                    i_this->field_0x69e = 3;
                    i_this->field_0x6a0 = 0;
                    i_this->field_0x6c4[0] = 80;
                    i_this->field_0x668 = 1;
                    i_this->mSound.stopAnime();
                    Z2GetAudioMgr()->unMuteSceneBgm(33);
                } else {
                    for (int i = 0; i < 2; i++) {
                        dComIfGp_particle_set(eno[i], &i_this->field_0x6e4[i_this->field_0x939], NULL, NULL);
                    }
                }

                i_this->field_0x939++;
            }
            break;

        case 10:
            break;
    }

    a_this->current.pos.y -= 5.0f;
}

static void e_gb_start(e_gb_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    cXyz sp28;

    i_this->field_0x6ce = 20;
    switch (i_this->field_0x69c) {
        case 0:
            i_this->field_0x69c = 1;
            head_anm_init(i_this, 17, 10.0f, 2, 1.0f);
            i_this->mDemoMode = 1;
            break;

        case 2:
            a_this->current.pos.set(-6112.0f, 4520.0f, 10800.0f);
            i_this->field_0x69c = 3;
            break;

        case 3:
            cLib_addCalc2(&a_this->current.pos.x, -5450.0f, 0.5f, 30.0f);
            break;

        case 4:
            cMtx_YrotS(*calc_mtx, a_this->home.angle.y);
            sp28.x = 20.0f;
            sp28.y = 500.0f;
            sp28.z = 0.0f;
            MtxPosition(&sp28, &i_this->field_0x6a8);
            i_this->field_0x6a8 += a_this->home.pos;

            cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x6a8.x, 0.1f, 80.0f);
            cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x6a8.y, 0.1f, 80.0f);
            cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x6a8.z, 0.1f, 80.0f);

            cLib_addCalcAngleS2(&a_this->current.angle.y, -0x4000, 16, 0x200);
            cLib_addCalcAngleS2(&a_this->shape_angle.y, 0x4000, 16, 0x200);
            cLib_addCalcAngleS2(&a_this->shape_angle.x, 0x2000, 16, 0x100);
    }
}

static void kuki_control1(e_gb_class* i_this) {
    static s16 pow_xa[17] = {
        0xD000, 0xD800, 0xE000, 0xE800, 0xF000,
        0x0000, 0x1000, 0x1800, 
        0x2000, 0x2800, 0x3000, 
        0x3800, 0x3000, 0x2000, 
        0x1000, 0x0000, 0x0000,
    };

    static s16 pow_xa_chance[17] = {
        0xC800, 0xD000, 0xD800, 0xE000, 0xE800,
        0xF000, 0xF800, 0x0000, 
        0x0000, 0x0000, 0x0000,
        0x0000, 0x0800, 0x0000,
        0x0800, 0x0000, 0x0800,
    };

    static f32 wav_d[17] = {
        0.0f, 0.2f, 0.4f, 
        0.5f, 0.6f, 0.8f, 1.0f,
        1.0f, 1.0f, 1.0f, 
        1.0f, 1.0f, 0.8f, 0.6f,
        0.4f, 0.2f, 0.0f,
    };

    fopEn_enemy_c* a_this = &i_this->actor;
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
        if (i_this->field_0x69a != 5) {
            if (i_this->field_0x94c > 1.0f) {
                fVar6 = i_this->field_0x94c * wav_d[i - 1] * 0.035f;
                fVar3 = fVar6 * cM_ssin(i_this->field_0x698 * (7000 + TREG_S(0)) + i * (7000 + TREG_S(1)));
                fVar7 = fVar6 * cM_scos(i_this->field_0x698 * (7700 + TREG_S(2)) + i * (5000 + TREG_S(3)));
            } else {
                fVar6 = (60.0f + TREG_F(8)) * wav_d[i - 1];
                fVar3 = fVar6 * cM_ssin(i_this->field_0x698 * (2000 + TREG_S(0)) + i * (7000 + TREG_S(1)));
                fVar7 = fVar6 * cM_scos(i_this->field_0x698 * (2300 + TREG_S(2)) + i * (5000 + TREG_S(3)));
            }

            cMtx_YrotS(*calc_mtx, (s16)a_this->current.angle.y);
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
            fVar3 = fVar6 * cM_ssin(i_this->field_0x698 * (1100 + TREG_S(0)) + i * (7000 + TREG_S(1)));
            fVar7 = fVar6 * cM_scos(i_this->field_0x698 * (1500 + TREG_S(2)) + i * (5000 + TREG_S(3)));
            fVar4 = fVar3 + (pcVar1->x - (pcVar1 - 1)->x);
            fVar8 = pcVar1->y - 5.0f;
            
            if (fVar8 < (i_this->mObjAcch.GetGroundH() + 5.0f + YREG_F(18))) {
                fVar8 = i_this->mObjAcch.GetGroundH() + 5.0f + YREG_F(18);
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
    fopEn_enemy_c* a_this = &i_this->actor;
    cXyz sp58, sp64;

    int i;
    s16 sVar1;
    s16 sVar2;
    cXyz* pcVar1 = &i_this->field_0x6e4[17];
    csXyz* pcVar2 = &i_this->field_0x7bc[17];
    *pcVar1 = a_this->current.pos;
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
    cXyz sp40;
    sp40 = i_this->field_0x6e4[0] - i_this->field_0x6d4;
    cXyz* pcVar1 = i_this->field_0x6e4;

    for (int i = 0; i < 18; i++) {
        *pcVar1 -= sp40 * ((18 - i) / 17.0f);
        pcVar1++;
    }

    i_this->field_0x964 = sp40.abs();
}

static void* s_b_sub(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && dBomb_c::checkBombActor((fopAc_ac_c*)i_actor) && !((dBomb_c*)i_actor)->checkStateExplode()) {
        cXyz sp28 = ((fopAc_ac_c*)i_actor)->current.pos - ((fopAc_ac_c*)i_data)->home.pos;
        if (fabsf(sp28.y) < 110.0f && JMAFastSqrt(sp28.x * sp28.x + sp28.z * sp28.z) < 150.0f) {
            fopAcM_delete(((fopAc_ac_c*)i_actor));
            return i_actor;
        }
    }
    return NULL;
}

static void damage_check(e_gb_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;

    i_this->mStts.Move();
    if (i_this->field_0x6ce == 0) {
        if (i_this->mHeadSph.ChkAtShieldHit()) {
            i_this->field_0x6ce = 6;
            i_this->mAtInfo.mpCollider = i_this->mHeadSph.GetAtHitObj();
            def_se_set(&i_this->mSound, i_this->mAtInfo.mpCollider, 0x2D, NULL);
            dComIfGp_getVibration().StartShock(6, 31, cXyz(0.0f, 1.0f, 0.0f));
        } else if (i_this->mHeadSph.ChkTgHit() != 0) {
            i_this->field_0x6ce = 6;
            i_this->mAtInfo.mpCollider = i_this->mHeadSph.GetTgHitObj();
            cc_at_check(a_this, &i_this->mAtInfo);
            if (i_this->mAtInfo.mHitType == HIT_TYPE_STUN) {
                i_this->field_0x69a = 1;
                i_this->field_0x69c = 5;
                i_this->field_0x948 = 5000;
                i_this->field_0x952 = 3000;
                i_this->field_0x94c = 3000.0f;

                head_anm_init(i_this, 17, 2.0f, 2, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_GB_V_NODAMAGE, -1);
                i_this->field_0x6c4[0] = 30;
            } else {
                i_this->field_0x69a = 3;
                i_this->field_0x69c = 0;

                head_anm_init(i_this, 11, 2.0f, 0, 1.0f);
                if (a_this->health <= 0) {
                    i_this->mSound.startCreatureVoice(Z2SE_EN_GB_V_DIE, -1);
                } else {
                    i_this->mSound.startCreatureVoice(Z2SE_EN_GB_V_DAMAGE, -1);
                }
            }
        }
    }
}

static void action(e_gb_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    cXyz sp34;
    cXyz sp40;
    sp34 = dComIfGp_getPlayer(0)->current.pos - a_this->current.pos;
    sp34.y += 100.0f;

    i_this->field_0x6b8 = (s16)cM_atan2s(sp34.x, sp34.z);
    i_this->field_0x6ba = -cM_atan2s(sp34.y, JMAFastSqrt(sp34.x * sp34.x + sp34.z * sp34.z));
    i_this->field_0x6bc = fopAcM_searchPlayerDistance(a_this);

    damage_check(i_this);

    s8 bVar1 = 0;
    s8 sVar1 = 0;
    s8 bVar2 = 1;
    sp34 = a_this->current.pos - a_this->home.pos;
    cMtx_YrotS(*calc_mtx, cM_atan2s(sp34.x, sp34.z));
    sp34.x = 0.0f;
    sp34.y = KREG_F(0) + 30.0f;
    sp34.z = KREG_F(1) + 150.0f;
    MtxPosition(&sp34, &i_this->field_0x6d4);
    i_this->field_0x6d4 += a_this->home.pos;
    i_this->field_0x6e0 = 0;
    a_this->field_0x566 = 0;
    s8 bVar3 = 1;

    switch (i_this->field_0x69a) {
        case 0:
            e_gb_wait(i_this);
            bVar1 = 1;
            a_this->field_0x566 = 1;
            break;

        case 1:
            e_gb_attack_1(i_this);
            bVar1 = 1;
            sVar1 = 1;
            a_this->field_0x566 = 1;
            break;

        case 2:
            e_gb_attack_2(i_this);
            bVar1 = 1;
            sVar1 = 2;
            bVar2 = 0;
            a_this->field_0x566 = 1;
            break;

        case 3:
            e_gb_damage(i_this);
            a_this->field_0x566 = 1;
            bVar1 = 1;
            bVar3 = 0;
            break;

        case 5:
            e_gb_end(i_this);
            bVar1 = 1;
            a_this->field_0x566 = 1;
            bVar3 = 0;
            break;

        case 10:
            e_gb_start(i_this);
            bVar2 = 0;
            i_this->field_0x6e0 = 1;
            a_this->field_0x566 = 1;
            bVar3 = 0;
    }

    if (bVar3 != 0) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    if (sVar1 == 1) {
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
        sp34.x = 0.0f;
        sp34.y = 0.0f;
        sp34.z = a_this->speedF;
        MtxPosition(&sp34, &a_this->speed);
        a_this->current.pos += a_this->speed;
    } else if (sVar1 == 2) {
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        sp34.x = 0.0f;
        sp34.y = 0.0f;
        sp34.z = a_this->speedF;

        MtxPosition(&sp34, &sp40);
        a_this->speed.x = sp40.x;
        a_this->speed.z = sp40.z;
        a_this->current.pos += a_this->speed;
        a_this->speed.y -= JREG_F(12) + 10.0f;
    }

    if (bVar2 != 0) {
        f32 fVar1;
        if (i_this->field_0x69a != 5) {
            fVar1 = ZREG_F(3) + 150.0f;
        } else {
            fVar1 = 60.0f;
        }

        i_this->mObjAcch.CrrPos(dComIfG_Bgsp());
        if (a_this->current.pos.y < fVar1 + i_this->mObjAcch.GetGroundH()) {
            a_this->current.pos.y = fVar1 + i_this->mObjAcch.GetGroundH();
        }
    }

    if (bVar1 != 0) {
        fopAcM_OnStatus(a_this, 0);
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    }

    sp34 = a_this->current.pos - i_this->field_0x6d4;
    i_this->field_0x93c = i_this->field_0x940 * (sp34.abs() * (BREG_F(0) + 0.1f));

    if (i_this->field_0x93c > (35.0f + BREG_F(1)) * l_HIO.field_0x8) {
        i_this->field_0x93c = (35.0f + BREG_F(1)) * l_HIO.field_0x8;
    }

    cLib_addCalc2(&i_this->field_0x940, 1.0f, 1.0f, 0.01f);
    kuki_control1(i_this);
    kuki_control2(i_this);
    kuki_control3(i_this);
    cLib_addCalc2(&i_this->field_0x944, 1.0f, 1.0f, 0.05f);

    if (i_this->field_0x94c > 0.1f) {
        if (i_this->field_0x948 != 0) {
            i_this->field_0x950 += i_this->field_0x948;
            cLib_addCalcAngleS2(&i_this->field_0x952, 0, 1, VREG_S(3) + 0x96);
        } else {
            i_this->field_0x952 = i_this->field_0x94c * cM_scos((s16)i_this->field_0x94a);
        }

        i_this->field_0x94a += (s16)(10000 + VREG_S(2));
        cLib_addCalc0(&i_this->field_0x94c, 1.0f, VREG_F(2) + 150.0f);
    } else {
        i_this->field_0x952 = 0;
    }

    cXyz sp4c(2.0f, 2.0f, 2.0f);
    setMidnaBindEffect(a_this, &i_this->mSound, &a_this->eyePos, &sp4c);
}

static void e_gf_wait(e_gb_class* i_this) {
    switch (i_this->field_0x6a0) {
        case 0:
            i_this->field_0x6a0 = 1;
            break;

        case 1:
        default:
            return;
    }
}

static void e_gf_chance(e_gb_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    cXyz sp38;
    BOOL bVar2 = i_this->mpModelMorf2->isStop();
    s8 bVar1 = 0;

    switch (i_this->field_0x6a0) {
        case 0:
            if (i_this->field_0x6c4[0] == 30) {
                i_this->mDemoMode = 10;
            }

            if (i_this->field_0x6c4[0] == 0) {
                body_anm_init(i_this, 24, 5.0f, 0, 1.0f);
                mDoAud_seStart(Z2SE_EN_GF_OPEN, &a_this->home.pos, 0, 0);
                i_this->field_0x6a0 = 1;
            }
            break;

        case 1:
            if (i_this->mpModelMorf2->isStop()) {
                body_anm_init(i_this, 25, 2.0f, 2, 1.0f);
                i_this->field_0x6a0 = 2;
            }
            // fallthrough
        case 2:
            bVar1 = 1;
    }

    if (bVar1 && fpcM_Search(s_b_sub, i_this) != NULL) {
        i_this->field_0x69e = 2;
        i_this->field_0x6a0 = 0;
    }
}

static void e_gf_eatbomb(e_gb_class* i_this) {
    static u16 eno[5] = {
        0x82E8,
        0x82E9,
        0x82EA,
        0x82EB,
        0x82EC,
    };

    fopEn_enemy_c* a_this = &i_this->actor;
    cXyz sp28;

    switch (i_this->field_0x6a0) {
        case 0:
            body_anm_init(i_this, 23, 3.0f, 0, 1.0f);
            mDoAud_seStart(Z2SE_EN_GF_EAT, &a_this->home.pos, 0, 0);
            i_this->field_0x6a0 = 1;
            // fallthrough
        case 1:
            if (i_this->mpModelMorf2->isStop()) {
                body_anm_init(i_this, 20, 2.0f, 0, 1.0f);
                mDoAud_seStart(Z2SE_EN_GF_BURST, &a_this->home.pos, 0, 0);
                i_this->field_0x6a0 = 2;
            }
            break;

        case 2:
            if (i_this->mpModelMorf2->isStop()) {
                i_this->field_0x69e = 10;
                i_this->field_0x6a0 = 0;
                i_this->mBrkAnm->setPlaySpeed(1.0f);
            }
            break;
    }

    if (i_this->field_0x68c == 20) {
        for (int i = 0; i < 5; i++) {
            i_this->field_0xe7c[i] = dComIfGp_particle_set(i_this->field_0xe7c[i], eno[i], 
                                                           &a_this->home.pos, NULL, NULL);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0xe7c[i]);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(i_this->mpModelMorf2->getModel()->getAnmMtx(5));
            }
        }
    }
}

static void e_gf_end(e_gb_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    cXyz sp30;

    switch (i_this->field_0x6a0) {
        case 0:
            body_anm_init(i_this, 22, 5.0f, 0, 1.0f);
            mDoAud_seStart(Z2SE_EN_GF_DIE, &a_this->home.pos, 0, 0);
            i_this->field_0x6a0 = 1;
            // fallthrough
        case 1:
            if (i_this->mpModelMorf2->isStop()) {
                i_this->field_0x6a0 = 3;
                i_this->field_0x6c4[0] = 40;
            }
            break;

        case 3:
            if (i_this->field_0x6c4[0] == 20) {
                dComIfGs_onSwitch((fopAcM_GetParam(a_this) & 0xFF0000) >> 16, fopAcM_GetRoomNo(a_this));
            }

            if (i_this->field_0x6c4[0] == 0) {
                daKey_c* key_p = (daKey_c*)fopAcM_SearchByName(PROC_Obj_SmallKey);
                cXyz sp3c(a_this->home.pos);
                if (key_p != NULL) {
                    key_p->setPos(sp3c);
                    key_p->startMove(60.0f, 4.0f);
                }

                fopAcM_createDisappear(a_this, &sp3c, 20, 0, a_this->field_0x564);
                fopAcM_delete(a_this);
            }
    }
}

static void damage_check2(e_gb_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;

    if (i_this->field_0x6d0 == 0) {
        if (i_this->mBodyCyl.ChkTgHit() != 0) {
            i_this->field_0x6d0 = 6;
            i_this->mAtInfo.mpCollider = i_this->mBodyCyl.GetTgHitObj();
            def_se_set(&i_this->mSound, i_this->mAtInfo.mpCollider, 0x2D, NULL);

            if (i_this->field_0x68c == 26 || i_this->field_0x68c == 19) {
                body_anm_init(i_this, 19, 2.0f, 0, 1.0f);
                mDoAud_seStart(Z2SE_EN_GF_BOUND_CLOSE, &a_this->home.pos, 0, 0);
            } else if (i_this->field_0x68c == 24 || i_this->field_0x68c == 25 || i_this->field_0x68c == 18) {
                body_anm_init(i_this, 18, 2.0f, 0, 1.0f);
                mDoAud_seStart(Z2SE_EN_GF_BOUND, &a_this->home.pos, 0, 0);
            }
        }

        if (i_this->field_0x68c == 18 && i_this->mpModelMorf2->isStop()) {
            body_anm_init(i_this, 25, 10.0f, 2, 1.0f);
            i_this->mpModelMorf2->setFrame(47.0f);
        } else if (i_this->field_0x68c == 19 && i_this->mpModelMorf2->isStop()) {
            body_anm_init(i_this, 26, 10.0f, 2, 1.0f);
        }
    }
}

static void action2(e_gb_class* i_this) {
    damage_check2(i_this);

    s8 bVar1 = 1;

    switch (i_this->field_0x69e) {
        case 0:
            e_gf_wait(i_this);
            break;

        case 3:
            e_gf_chance(i_this);
            bVar1 = 0;
            break;

        case 2:
            e_gf_eatbomb(i_this);
            bVar1 = 0;
            break;

        case 10:
            e_gf_end(i_this);
            bVar1 = 0;
    }

    if (bVar1 != 0) {
        daPy_getPlayerActorClass()->onBossRoomWait();
    }

    if (i_this->field_0x955 != 0) {
        daKey_c* key_p = (daKey_c*)fopAcM_SearchByName(PROC_Obj_SmallKey);
        if (key_p != NULL) {
            key_p->field_0x978 = i_this->field_0x674;
            mDoAud_seStartLevel(Z2SE_OBJ_KEY_TWINKLE, &key_p->field_0x978, 0, 
                                dComIfGp_getReverb(fopAcM_GetRoomNo(key_p)));
            key_p->field_0x984 = 1;
        }
    }
}

static void cam_3d_morf(e_gb_class* i_this, f32 param_2) {
    cLib_addCalc2(&i_this->mDemoCamCenter.x, i_this->field_0xeb8.x, param_2, 
                  i_this->field_0xed0.x * i_this->field_0xee4);
    cLib_addCalc2(&i_this->mDemoCamCenter.y, i_this->field_0xeb8.y, param_2, 
                  i_this->field_0xed0.y * i_this->field_0xee4);
    cLib_addCalc2(&i_this->mDemoCamCenter.z, i_this->field_0xeb8.z, param_2, 
                  i_this->field_0xed0.z * i_this->field_0xee4);

    cLib_addCalc2(&i_this->mDemoCamEye.x, i_this->field_0xeac.x, param_2,
                  i_this->field_0xec4.x * i_this->field_0xee4);
    cLib_addCalc2(&i_this->mDemoCamEye.y, i_this->field_0xeac.y, param_2,
                  i_this->field_0xec4.y * i_this->field_0xee4);
    cLib_addCalc2(&i_this->mDemoCamEye.z, i_this->field_0xeac.z, param_2,
                  i_this->field_0xec4.z * i_this->field_0xee4);
}

static void demo_camera(e_gb_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz sp1c, sp28, sp34, sp40;
    int swBit;

    switch (i_this->mDemoMode) {
        case 1:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->mDemoMode = 2;
            i_this->mDemoCamFovy = 55.0f;
            i_this->field_0xe92 = 0;
            camera->mCamera.SetTrimSize(3);
            daPy_getPlayerActorClass()->changeOriginalDemo();
            i_this->field_0x674.set(-5750.0f, 4410.0f, 10684.0f);
            i_this->field_0x684 = 0x5000;
            i_this->mDemoCamCenter.set(-5689.0f, 4747.0f, 15000.0f);
            i_this->mDemoCamEye.set(-5700.0f, 4740.0f, 14644.0f);
            i_this->field_0xeb8.set(-5689.0f, 4740.0f, 14643.0f);
            i_this->field_0xeac.set(-5723.0f, 4733.0f, 14293.0f);
            i_this->field_0xec4.x = fabsf(i_this->field_0xeac.x - i_this->mDemoCamEye.x);
            i_this->field_0xec4.y = fabsf(i_this->field_0xeac.y - i_this->mDemoCamEye.y);
            i_this->field_0xec4.z = fabsf(i_this->field_0xeac.z - i_this->mDemoCamEye.z);
            i_this->field_0xed0.x = fabsf(i_this->field_0xeb8.x - i_this->mDemoCamCenter.x);
            i_this->field_0xed0.y = fabsf(i_this->field_0xeb8.y - i_this->mDemoCamCenter.y);
            i_this->field_0xed0.z = fabsf(i_this->field_0xeb8.z - i_this->mDemoCamCenter.z);
            i_this->field_0xee4 = 0.0;

            Z2GetAudioMgr()->setBattleBgmOff(true);
            dComIfGp_getEvent().startCheckSkipEdge(a_this);
            swBit = (fopAcM_GetParam(a_this) & 0xFF00) >> 8;
            dComIfGs_onSwitch(swBit, fopAcM_GetRoomNo(a_this));
            // fallthrough
        case 2:
            sp28.set(-5700.0f, 4350.0f, 9670.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp28, 0, 0);
            if (i_this->field_0xe92 > 50) {
                cam_3d_morf(i_this, 0.1f);
                cLib_addCalc2(&i_this->field_0xee4, 0.02f, 1.0f, 0.0005f);
            }

            if (i_this->field_0xe92 != 170) break;
            i_this->mDemoCamCenter.set(-5690.0f, 4432.0f, 9610.0f);
            i_this->mDemoCamEye.set(-5709.0f, 4445.0f, 9961.0f);
            i_this->field_0xeb8.set(-5741.0f, 4432.0f, 10530.0f);
            i_this->field_0xeac.set(-5760.0f, 4496.0f, 10896.0f);
            i_this->field_0xec4.x = fabsf(i_this->field_0xeac.x - i_this->mDemoCamEye.x);
            i_this->field_0xec4.y = fabsf(i_this->field_0xeac.y - i_this->mDemoCamEye.y);
            i_this->field_0xec4.z = fabsf(i_this->field_0xeac.z - i_this->mDemoCamEye.z);
            i_this->field_0xed0.x = fabsf(i_this->field_0xeb8.x - i_this->mDemoCamCenter.x);
            i_this->field_0xed0.y = fabsf(i_this->field_0xeb8.y - i_this->mDemoCamCenter.y);
            i_this->field_0xed0.z = fabsf(i_this->field_0xeb8.z - i_this->mDemoCamCenter.z);
            i_this->field_0xee4 = 0.0f;
            i_this->mDemoMode = 3;
            i_this->mDemoCamFovy = 55.0f;
            i_this->field_0xe92 = 0;
            // fallthrough
        case 3:
            if (i_this->field_0xe92 > 30) {
                cam_3d_morf(i_this, 0.1f);
                cLib_addCalc2(&i_this->field_0xee4, 0.02f, 1.0f, 0.0005f);
            }

            if (i_this->field_0xe92 == 130) {
                i_this->field_0x69c++;
            }

            if (i_this->field_0xe92 == 135) {
                i_this->mSound.startCreatureSound(Z2SE_EN_GB_SWINGHEAD, 0, -1);
            }

            if (i_this->field_0xe92 == 143) {
                i_this->field_0x670 = 2;
            }

            if (i_this->field_0xe92 == 162) {
                daPy_getPlayerActorClass()->changeDemoMode(23, 1, 2, 0);
            }

            if (i_this->field_0xe92 != 190) break;
            i_this->field_0x69c++;
            i_this->mDemoMode = 4;
            i_this->field_0xe92 = 0;
            body_anm_init(i_this, 24, 5.0f, 0, 1.0f);
            mDoAud_seStart(Z2SE_EN_GF_OPEN, &a_this->home.pos, 0, 0);
            i_this->mDemoCamCenter.set(-5706.0f, 4526.0f, 11379.0f);
            i_this->mDemoCamEye.set(-5687.0f, 4505.0f, 11008.0f);
            i_this->field_0xeb8.set(-5624.0f, 4439.0f, 9818.0f);
            i_this->field_0xeac.set(-5604.0f, 4418.0f, 9447.0f);
            i_this->field_0xec4.x = fabsf(i_this->field_0xeac.x - i_this->mDemoCamEye.x);
            i_this->field_0xec4.y = fabsf(i_this->field_0xeac.y - i_this->mDemoCamEye.y);
            i_this->field_0xec4.z = fabsf(i_this->field_0xeac.z - i_this->mDemoCamEye.z);
            i_this->field_0xed0.x = fabsf(i_this->field_0xeb8.x - i_this->mDemoCamCenter.x);
            i_this->field_0xed0.y = fabsf(i_this->field_0xeb8.y - i_this->mDemoCamCenter.y);
            i_this->field_0xed0.z = fabsf(i_this->field_0xeb8.z - i_this->mDemoCamCenter.z);
            i_this->field_0xee4 = 0.0f;
            i_this->mDemoCamCenter.y = 4584.0f;
            // fallthrough
        case 4:
            if (i_this->field_0xe92 == 48) {
                head_anm_init(i_this, 14, 5.0f, 0, 1.0f);
                i_this->mSound.startCreatureSound(Z2SE_EN_GB_DROPKEY, 0, -1);
            }

            if (i_this->field_0xe92 == 54) {
                head_anm_init(i_this, 14, 5.0f, 0, 1.0f);
                i_this->field_0x674 = a_this->home.pos;
                i_this->field_0x674.y += 400.0f;
                i_this->field_0x670 = 1;
            }

            if (i_this->field_0xe92 == 68) {
                body_anm_init(i_this, 21, 5.0f, 0, 1.0f);
                mDoAud_seStart(Z2SE_EN_GF_CLOSE, &a_this->home.pos, 0, 0);
                i_this->field_0x955 = 0;
            }

            if (i_this->field_0xe92 > 150) {
                cam_3d_morf(i_this, 0.2f);
                cLib_addCalc2(&i_this->field_0xee4, 0.04f, 1.0f, 0.002f);
            } else if (i_this->field_0xe92 >= 80) {
                cLib_addCalc2(&i_this->mDemoCamCenter.y, 4526.0f, 0.05f, 1.0f);
            }

            if (i_this->field_0x68c == 21 && i_this->mpModelMorf2->isStop()) {
                body_anm_init(i_this, 26, 10.0f, 2, 1.0f);
            }

            if (i_this->field_0xe92 == 220) {
                i_this->mDemoMode = 100;
                i_this->field_0x69a = 0;
                i_this->field_0x69c = 0;
                i_this->field_0x69e = 0;
                i_this->field_0x6a0 = 0;
                Z2GetAudioMgr()->setBattleBgmOff(false);
            }
            break;

        case 10:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->mDemoMode = 11;
            i_this->mDemoCamFovy = 55.0;
            i_this->field_0xe92 = 0;
            camera->mCamera.SetTrimSize(3);
            // fallthrough
        case 11:
            i_this->mDemoCamCenter = a_this->home.pos;
            i_this->mDemoCamCenter.y += 100.0f;
            i_this->mDemoCamEye = a_this->home.pos;
            i_this->mDemoCamEye.y += 300.0f;
            i_this->mDemoCamEye.z += -600.0f;

            if (i_this->field_0xe92 == 100) {
                i_this->mDemoMode = 100;
            }
            break;

        case 100:
            camera->mCamera.Reset(i_this->mDemoCamCenter, i_this->mDemoCamEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            i_this->mDemoMode = 0;
            break;

        case 0:
            break;
    }

    if (i_this->mDemoMode != 0) {
        camera->mCamera.Set(i_this->mDemoCamCenter, i_this->mDemoCamEye, i_this->mDemoCamFovy, 0);
        i_this->field_0xe92++;

        if (i_this->mDemoMode < 10) {
            dComIfGp_getEvent().setSkipProc(i_this, dEv_defaultSkipProc, 0);
            if (dComIfGp_getEvent().checkSkipEdge()) {
                i_this->mDemoMode = 100;
                cMtx_YrotS(*calc_mtx, player->shape_angle.y);
                sp1c.x = 0.0;
                sp1c.y = 100.0f;
                sp1c.z = -250.0f;
                MtxPosition(&sp1c, &i_this->mDemoCamEye);
                i_this->mDemoCamEye += player->current.pos;
                i_this->mDemoCamCenter = player->current.pos;
                i_this->mDemoCamCenter.y += 120.0f;
                i_this->field_0x69a = 0;
                i_this->field_0x69c = 0;
                i_this->field_0x69e = 0;
                i_this->field_0x6a0 = 0;
                body_anm_init(i_this, 26, 1.0f, 2, 1.0f);
                i_this->field_0x66c = NULL;
                i_this->field_0x686 = 0;
                i_this->field_0x955 = 0;
                Z2GetAudioMgr()->setBattleBgmOff(false);
            }
        }
    }
}

static int daE_GB_Execute(e_gb_class* i_this) {
    static u16 key_eno[2] = {
        0x827A,
        0x827B,
    };

    fopEn_enemy_c* a_this = &i_this->actor;
    cXyz sp50, sp5c;

    i_this->field_0x698++;

    for (int i = 0; i < 5; i = i + 1) {
        if (i_this->field_0x6c4[i] != 0) {
            i_this->field_0x6c4[i]--;
        }
    }

    if (i_this->field_0x6ce != 0) {
        i_this->field_0x6ce--;
    }

    if (i_this->field_0x6d0 != 0) {
        i_this->field_0x6d0--;
    }

    action(i_this);
    if (i_this->field_0x668 == 0) {
        i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    }

    action2(i_this);
    mDoMtx_stack_c::transS(a_this->current.pos.x,a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->field_0x950);
    mDoMtx_stack_c::XrotM(i_this->field_0x952);
    mDoMtx_stack_c::YrotM(-i_this->field_0x950);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(a_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.field_0x8, l_HIO.field_0x8, l_HIO.field_0x8);

    J3DModel* model = i_this->mpModelMorf1->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpModelMorf1->play(NULL, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)), 0);
    i_this->mpModelMorf1->modelCalc();
    MTXCopy(model->getAnmMtx(3), *calc_mtx);

    if (i_this->field_0x968 != 0) {
        sp50.set(20.0f, 0.0f, -50.0f);
        MtxPosition(&sp50, &a_this->eyePos);
        a_this->attention_info.position = a_this->eyePos;
        a_this->attention_info.position.y += 50.0f;

        JPABaseEmitter* baseEmitter;
        if (i_this->field_0x688 == 16) {
            i_this->field_0xe6c = dComIfGp_particle_set(i_this->field_0xe6c, 0x82E7, &a_this->current.pos, NULL, NULL);

            baseEmitter = dComIfGp_particle_getEmitter(i_this->field_0xe6c);
            if (baseEmitter != NULL) {
                baseEmitter->setGlobalSRTMatrix(model->getAnmMtx(3));
            }

            if (i_this->mpModelMorf1->checkFrame(8.0f)) {                
                i_this->mSound.startCreatureVoice(Z2SE_EN_GB_V_NODAMAGE, -1);
            }
        }

        if (i_this->field_0x688 == 11) {
            i_this->field_0xe70 = dComIfGp_particle_set(i_this->field_0xe70, 0x82E6, &a_this->current.pos, NULL, NULL);

            baseEmitter = dComIfGp_particle_getEmitter(i_this->field_0xe70);
            if (baseEmitter != NULL) {
                baseEmitter->setGlobalSRTMatrix(model->getAnmMtx(3));
            }
        }

        if (i_this->field_0x688 == 17 && i_this->mpModelMorf1->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_GB_V_WAIT, -1);
        }
    }

    sp50.set(50.0f, 0.0f, 0.0f);
    MtxPosition(&sp50, &sp5c);

    if (i_this->field_0x6ce != 0) {
        sp5c.x += 10000.0f;
    }

    i_this->mHeadSph.SetC(sp5c);
    if (i_this->field_0x6bc > 500.0f && daPy_py_c::checkBoomerangChargeTime() != 0) {
        i_this->mHeadSph.SetR(l_HIO.field_0x8 * 160.0f);
    } else {
        i_this->mHeadSph.SetR(l_HIO.field_0x8 * 80.0f);
    }

    dComIfG_Ccsp()->Set(&i_this->mHeadSph);
    if (i_this->field_0xc20 != 0) {
        i_this->mHeadSph.OnAtSetBit();
        i_this->field_0xc20 = 0;
    } else {
        i_this->mHeadSph.OffAtSetBit();
    }

    mDoMtx_stack_c::transS(a_this->home.pos.x, a_this->home.pos.y, a_this->home.pos.z);
    mDoMtx_stack_c::YrotM(a_this->home.angle.y);
    mDoMtx_stack_c::scaleM(l_HIO.field_0xc, l_HIO.field_0xc, l_HIO.field_0xc);
    model = i_this->mpModelMorf2->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpModelMorf2->play(NULL, 0, 0);
    i_this->mBrkAnm->play();
    i_this->mpModelMorf2->modelCalc();

    if ((i_this->field_0x68c == 25 || i_this->field_0x68c == 26) && i_this->mpModelMorf2->checkFrame(1.0f)) {
        mDoAud_seStart(Z2SE_EN_GF_WAIT, &a_this->home.pos, 0, 0);
    }

    if (i_this->field_0x968 == 0) {
        a_this->eyePos = a_this->home.pos;
        a_this->eyePos.y += 150.0f;
        a_this->attention_info.position = a_this->eyePos;
        a_this->attention_info.position.y += 50.0f;
    }

    i_this->mBodyCyl.SetC(a_this->home.pos);
    i_this->mBodyCyl.SetR(l_HIO.field_0xc * 170.0f);
    i_this->mBodyCyl.SetH(l_HIO.field_0xc * 100.0f);
    dComIfG_Ccsp()->Set(&i_this->mBodyCyl);

    if (i_this->field_0x66c != NULL) {
        if (i_this->field_0x670 == 1) {
            i_this->field_0x674.y += i_this->field_0x680;
            i_this->field_0x680 -= 3.0f;
            i_this->field_0x686 += -0xC00;

            if (i_this->field_0x674.y < a_this->home.pos.y) {
                i_this->field_0x686 = 0;
                i_this->field_0x66c = NULL;
            }
        } else if (i_this->field_0x670 == 2) {
            MTXCopy(model->getAnmMtx(5), *calc_mtx);
            sp50.set(-30.0f, 0.0f, 0.0f);
            MtxPosition(&sp50, &i_this->field_0x674);
        }

        if (i_this->field_0x66c != NULL) {
            mDoMtx_stack_c::transS(i_this->field_0x674.x, i_this->field_0x674.y, i_this->field_0x674.z);
            mDoMtx_stack_c::YrotM(i_this->field_0x684);
            mDoMtx_stack_c::XrotM(i_this->field_0x686);
            mDoMtx_stack_c::XrotM(0x4000);
            f32 fVar1 = 1.5f;
            mDoMtx_stack_c::scaleM(fVar1, fVar1, fVar1);
            i_this->field_0x66c->setBaseTRMtx(mDoMtx_stack_c::get());
            sp5c = i_this->field_0x674;
            for (int i = 0; i < 2; i++) {
                i_this->field_0xe62[i] = dComIfGp_particle_set(i_this->field_0xe62[i], key_eno[i], &sp5c, NULL, NULL);
            }
        }
    }

    f32 fVar2 = i_this->field_0x964 * 0.001f;

    for (int i = 0; i < 3; i++) {
        if (i_this->field_0x92c[i] == 0) {
            if (i_this->field_0x938 == 2) {
                i_this->field_0x938 = 0;
                i_this->field_0x92c[i] = 2;
                i_this->field_0x932[i] = 17;
                i_this->field_0x928 = 2.0f;
                i_this->field_0x924 = 4000;
                break;
            }
        } else {
            if (i_this->field_0x92c[i] == 1) {
                if (i_this->field_0x935[i] == 0) {
                    i_this->field_0x935[i] = 1;
                    i_this->field_0x900[i_this->field_0x932[i]] = 1;
                    i_this->field_0x932[i]++;
                    if (i_this->field_0x932[i] >= 18) {
                        i_this->field_0x92c[i] = 0;
                    }
                }
            } else if (i_this->field_0x935[i] == 0) {
                i_this->field_0x935[i] = 1;
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
        if (i_this->field_0x828[i] != NULL) {
            if (i_this->field_0x900[i] != 0) {
                i_this->field_0x900[i] +=i_this->field_0x924;

                if (i_this->field_0x900[i] < 0) {
                    i_this->field_0x900[i] = 0;
                }

                i_this->field_0x8b8[i] = i_this->field_0x928 * cM_ssin(i_this->field_0x900[i]) + 1.0f;
            }

            MtxTrans(i_this->field_0x6e4[i].x, i_this->field_0x6e4[i].y, i_this->field_0x6e4[i].z, 0);
            cMtx_XrotM(*calc_mtx, i_this->field_0x7bc[i].x);
            cMtx_YrotM(*calc_mtx, i_this->field_0x7bc[i].y + 0x8000);
            MtxScale(l_HIO.field_0x8 * i_this->field_0x8b8[i], l_HIO.field_0x8 * i_this->field_0x8b8[i], l_HIO.field_0x8 + fVar2, 1);
            MtxTrans(0.0f, 0.0f, 0.0f, 1);
            i_this->field_0x828[i]->setBaseTRMtx(*calc_mtx);
        }
    }

    demo_camera(i_this);
    return 1;
}

static int daE_GB_IsDelete(e_gb_class* i_this) {
    return 1;
}

static int daE_GB_Delete(e_gb_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    dComIfG_resDelete(&i_this->mPhase, "E_gb");
    if (i_this->field_0xef5 != 0) {
        data_806C7928 = 0;
    }

    if (a_this->heap != NULL && i_this->field_0x668 == 0) {
        i_this->mSound.stopAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    e_gb_class* i_this = (e_gb_class*)a_this;

    i_this->mpModelMorf1 = new mDoExt_McaMorf(static_cast<J3DModelData*>(dComIfG_getObjectRes("E_gb", 29)), NULL, NULL,
                                              static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("E_gb", 17)), 2, 1.0f, 
                                              0, -1, 1, NULL, 0x80000, 0x11000084);
    if (i_this->mpModelMorf1 == NULL || i_this->mpModelMorf1->getModel() == NULL) {
        return 0;
    }

    i_this->mpModelMorf2 = new mDoExt_McaMorf(static_cast<J3DModelData*>(dComIfG_getObjectRes("E_gb", 31)), NULL, NULL,
                                              static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("E_gb", 26)), 2, 1.0f,
                                              0, -1, 1, NULL, 0x80000, 0x11000084);
    i_this->field_0x68c = 26;
    if (i_this->mpModelMorf2 == NULL || i_this->mpModelMorf2->getModel() == NULL) {
        return 0;
    }

    i_this->mBrkAnm = new mDoExt_brkAnm();
    if (i_this->mBrkAnm == NULL) {
        return 0;
    }

    if (i_this->mBrkAnm->init(i_this->mpModelMorf2->getModel()->getModelData(),
                              static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("E_gb", 35)), 
                               1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }
    i_this->mBrkAnm->setPlaySpeed(0.0f);

    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("E_gb", 32));
    JUT_ASSERT(3378, modelData != NULL);

    for (int i = 0; i < 18; i++) {
        i_this->field_0x828[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->field_0x828[i] == NULL) {
            return 0;
        }

        i_this->field_0x8b8[i] = 1.0f;
    }

    if (strcmp(dComIfGp_getStartStageName(), "D_MN05") == 0) {
        modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("E_gb", 30));
        JUT_ASSERT(3395, modelData != NULL);
        i_this->field_0x66c = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->field_0x66c == NULL) {
            return 0;
        }
    }

    return 1;
}

static cPhs__Step daE_GB_Create(fopAc_ac_c* a_this) {
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
            {0.0f, 0.0f, 0.0f}, // mCenter
            30.0f, // mRadius
            20.0f // mHeight
        } // mCyl
    };

    e_gb_class* i_this = (e_gb_class*)a_this;
    fopAcM_ct(a_this, e_gb_class);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->mPhase, "E_gb");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_gb PARAM %x\n", fopAcM_GetParam(a_this));
        u8 swBit = fopAcM_GetParam(a_this) >> 16;
        if (swBit != 0xFF) {
            if (dComIfGs_isSwitch(swBit, fopAcM_GetRoomNo(a_this))) {
                OS_REPORT("E_gb やられ後なので再セットしません\n");
                return cPhs_ERROR_e;
            }
        }

        i_this->field_0x5b4 = fopAcM_GetParam(a_this);
        i_this->field_0x5b5 = fopAcM_GetParam(a_this) >> 8;
        i_this->field_0x5b6 = fopAcM_GetParam(a_this) >> 16 & 15;
        i_this->field_0x5b7 = fopAcM_GetParam(a_this) >> 20 & 15;
        OS_REPORT("E_gb //////////////E_GB SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x4B000)) {
            OS_REPORT("//////////////E_GB SET NON !!\n");
            return cPhs_ERROR_e;
        }
        OS_REPORT("//////////////E_GB SET 2 !!\n");

        if (data_806C7928 == 0) {
            i_this->field_0xef5 = 1;
            data_806C7928 = 1;
            l_HIO.field_0x4 = -1;
        }

        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        a_this->health = 90;
        a_this->field_0x560 = 90;

        i_this->mStts.Init(253, 0, a_this);
        i_this->mHeadSph.Set(head_cc_sph_src);
        i_this->mHeadSph.SetStts(&i_this->mStts);
        i_this->mBodyCyl.Set(body_cyl_src);
        i_this->mBodyCyl.SetStts(&i_this->mStts);
        i_this->mBodyCyl.OnTgNoHitMark();

        i_this->mObjAcch.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1, &i_this->mAcchCir,
                            fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(150.0f, 150.0f);

        i_this->mAtInfo.mpSound = &i_this->mSound;
        i_this->mAtInfo.mPowerType = 8;

        i_this->mSound.init(&a_this->current.pos, &a_this->eyePos, 3, 1);
        i_this->mSound.setEnemyName("E_gb");
        
        i_this->field_0x698 = cM_rndF(65535.0f);

        u32 swBit2 = (fopAcM_GetParam(a_this) >> 8) & 0xff;
        s16 roomNo = fopAcM_GetRoomNo(a_this);
        if (roomNo == 7 && !dComIfGs_isSwitch(swBit2, roomNo)) {
            i_this->field_0x69a = 10;
        } else {
            i_this->field_0x69a = 0;
        }

        i_this->field_0x968 = 1;
        i_this->field_0x955 = 1;
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

extern actor_process_profile_definition g_profile_E_GB = {
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
