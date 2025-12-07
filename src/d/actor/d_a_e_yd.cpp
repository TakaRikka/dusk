/**
 * @file d_a_e_yd.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_yd.h"
#include "d/actor/d_a_e_yd_leaf.h"
#include "d/d_bg_w.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_enemy.h"

class daE_YD_HIO_c {
public:
    daE_YD_HIO_c();
    virtual ~daE_YD_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 field_0x8;
    /* 0xC */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ s16 field_0x14;
    /* 0x16 */ s16 field_0x16;
    /* 0x18 */ u8 field_0x18;
};

STATIC_ASSERT(sizeof(daE_YD_HIO_c) == 0x1c);

daE_YD_HIO_c::daE_YD_HIO_c() {
    field_0x4 = -1;
    field_0x8 = 1.0f;
    field_0xc = 0.5f;
    field_0x10 = 1.0f;
    field_0x14 = 0x46;
    field_0x16 = 0x1e;
    field_0x18 = 1;
}

static void yd_disappear(e_yd_class* i_this) {
    cXyz particleScale(1.5f, 1.5f, 1.5f);
    dComIfGp_particle_set(0x826c, &i_this->actor.current.pos, NULL, &particleScale);
    dComIfGp_particle_set(0x826d, &i_this->actor.current.pos, NULL, &particleScale);
    fopAcM_seStart(&i_this->actor, Z2SE_DARK_VANISH, 0);
    fopAcM_createItemFromEnemyID(0x2b, &i_this->actor.current.pos, -1, -1, NULL, NULL, NULL, NULL);
    fopAcM_delete(&i_this->actor);
    i_this->actor.field_0x567 = 1;

    for (s32 i = 0; i <= 2; i++) {
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x1238[i]);
        if (emitter != NULL) {
            emitter->deleteAllParticle();
            dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0x1238[i]);
        }
    }
}

static void anm_init(e_yd_class* i_this, int param_1, f32 param_2, u8 param_3, f32 param_4) {
    J3DAnmTransform* anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("E_yd", param_1);
    i_this->mpMorf->setAnm(anmTransform, param_3, param_2, param_4, 0.0f, -1.0f);
    i_this->field_0x664 = param_1;
}

static s32 leaf_anm_init(e_yd_class* i_this, int param_1, f32 param_2, u8 param_3, f32 param_4) {
    e_yd_leaf_class* leaf = (e_yd_leaf_class*)fopAcM_SearchByID(i_this->mLeafId);
    if (leaf != NULL) {
        J3DAnmTransform* anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("E_yd", param_1);
        leaf->mpMorf->setAnm(anmTransform, param_3, param_2, param_4, 0.0f, -1.0f, NULL);
        return true;
    }

    return false;
}

static s32 daE_YD_Draw(e_yd_class* i_this) {
    static GXColor l_color = { 0x14, 0x0F, 0x00, 0xFF };

    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;

    g_env_light.settingTevStruct(2, &i_this->actor.current.pos, &i_this->actor.tevStr);
    dComIfGd_setListDark();
    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.setLightTevColorType_MAJI(model, &i_this->actor.tevStr);
    i_this->mBrkAnm->entry(model->getModelData());
    i_this->mpMorf->entryDL();
    i_this->mLineMat.update(12, l_color, &i_this->actor.tevStr);
    dComIfGd_set3DlineMat(&i_this->mLineMat);
    for (s32 i = 1; i < 11; i++) {
        if (i_this->field_0x77c[i] != 0) {
            g_env_light.setLightTevColorType_MAJI(i_this->field_0x77c[i], &i_this->actor.tevStr);
            mDoExt_modelUpdateDL(i_this->field_0x77c[i]);
        }
    }

    dComIfGd_setList();

    cXyz unkXyz1(actor->current.pos.x, actor->current.pos.y + 50.0f + BREG_F(18),
        actor->current.pos.z);
    i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &unkXyz1,
        600.0f + MREG_F(19), 0.0f, i_this->actor.current.pos.y, fopAcM_gc_c::getGroundY(),
        *fopAcM_gc_c::getGroundCheck(), &i_this->actor.tevStr, 0, 1.0f,
        dDlst_shadowControl_c::getSimpleTex());

    return 1;
}

static s32 pl_check(e_yd_class* i_this, f32 param_2) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (i_this->mHomeDistFromPlayer < param_2 &&
        (i_this->mLeafRoomNo != 0 ||
        fopAcM_otherBgCheck(&i_this->actor, player) == 0)) {
        return true;
    }
    return false;
}

static void damage_check(e_yd_class* i_this) {
    fopAc_ac_c* pfVar1 = dComIfGp_getPlayer(0);
    if (i_this->field_0x69c[3] == 0) {
        i_this->mStts.Move();
        if (i_this->field_0x9d8.ChkAtShieldHit()) {
            i_this->field_0x69c[3] = 6;
            i_this->field_0x66e = 7;
            i_this->field_0x670 = 0;
            i_this->field_0x69c[0] = 10;
            i_this->field_0x69c[1] = 100;
            i_this->field_0x684 = pfVar1->shape_angle.y;
            i_this->field_0x1234 = 10;
            dComIfGp_getVibration().StartShock(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        } else {
            cCcD_Obj* uVar4;
            s8 cVar6 = 0;
            for (s32 i = 0; i < 4; i++) {
                if (i_this->field_0xb14[i].ChkTgHit() != 0) {
                    i_this->field_0x69c[3] = 10;
                    uVar4 = i_this->field_0xb14[i].GetTgHitObj();
                    i_this->field_0xff4.mpCollider = uVar4;
                    s32 j = i_this->field_0xff4.mpCollider->ChkAtType(AT_TYPE_BOOMERANG);
                    if (j != 0) {
                        cVar6 = 1;
                    } else {
                        j = i_this->field_0xff4.mpCollider->ChkAtType(AT_TYPE_BOMB | AT_TYPE_40);
                        if (j != 0 || i_this->field_0x66e == 6) {
                            at_power_check(&i_this->field_0xff4);
                            if (daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_NONE) {
                                cVar6 = 1;
                            } else {
                                cVar6 = 2;
                            }
                        } else {
                            i_this->field_0x66e = 7;
                            i_this->field_0x670 = 0;
                            i_this->field_0x69c[0] = 10;
                            i_this->field_0x69c[1] = 100;
                            i_this->field_0x684 = pfVar1->shape_angle.y;
                            def_se_set(&i_this->mCreatureSound, i_this->field_0xff4.mpCollider, 0x2d, 0);
                            i_this->field_0x1234 = 10;
                        }
                    }
                    break;
                }
            }
            if (cVar6 == 0 && i_this->field_0x8a0.ChkTgHit() != 0) {
                uVar4 = i_this->field_0x8a0.GetTgHitObj();
                i_this->field_0xff4.mpCollider = uVar4;
                dCc_GetAc(i_this->field_0xff4.mpCollider->GetAc());
                if (i_this->field_0xff4.mpCollider->ChkAtType(
                        AT_TYPE_BOMB | AT_TYPE_40 | AT_TYPE_BOOMERANG | AT_TYPE_MIDNA_LOCK) != 0) {
                    cVar6 = 2;
                } else {
                    i_this->field_0x69c[3] = 6;
                    i_this->field_0x66e = 7;
                    i_this->field_0x670 = 0;
                    i_this->field_0x69c[0] = 10;
                    i_this->field_0x69c[1] = 100;
                    i_this->field_0x684 = pfVar1->shape_angle.y;
                    i_this->field_0x1234 = 10;
                    def_se_set(&i_this->mCreatureSound, i_this->field_0xff4.mpCollider, 0x2d, 0);
                }
            }
            if (cVar6 != 0) {
                yd_disappear(i_this);
                if (cVar6 == 1) {
                    dScnPly_c::setPauseTimer(4);
                }
            }
        }
    }
}

static u8 data_807F7D68;

static daE_YD_HIO_c l_HIO;

static void e_yd_stay(e_yd_class* i_this) {
    i_this->field_0x69c[3] = 0xf;

    switch (i_this->field_0x670) {
    case 0:
        i_this->field_0x678.y = 0.0f;
    case 1:
        anm_init(i_this, 14, 10.0f, 0, 1.0f);
        if (leaf_anm_init(i_this, 21, 10.0f, 2, 0.0f) != 0) {
            i_this->field_0x670 = 5;
            i_this->field_0x694 = 0;
            i_this->field_0x69c[0] = 0x32;
        }
        break;
    case 2:
        if (i_this->mLeafRoomNo != 0) {
            i_this->field_0x678.y = -170.0f;
        } else {
            i_this->field_0x678.y = 170.0f;
        }
        i_this->field_0x670 = 1;
        break;
    case 5:
        cLib_addCalc2(&i_this->actor.current.pos.x, i_this->actor.home.pos.x, 0.2f,
                      i_this->field_0x694 * 10.0f);
        cLib_addCalc2(&i_this->actor.current.pos.y,
                      i_this->actor.home.pos.y + i_this->field_0x678.y, 0.2f,
                      i_this->field_0x694 * 10.0f);
        cLib_addCalc2(&i_this->actor.current.pos.z, i_this->actor.home.pos.z, 0.2f,
                      i_this->field_0x694 * 10.0f);
        cLib_addCalc2(&i_this->field_0x694, 1.0f, 1.0f, 0.1f);
        cXyz cStack_28 = i_this->actor.current.pos - i_this->actor.home.pos;
        cStack_28.y = 0.0f;
        if (cStack_28.abs() < 20.0f) {
            cLib_addCalc0(&i_this->field_0x678.y, 1.0f, 4.0f);
            if (i_this->mLeafRoomNo != 0) {
                cLib_addCalcAngleS2(&i_this->actor.shape_angle.x, 0x4000, 4, 0x200);
            } else {
                cLib_addCalcAngleS2(&i_this->actor.shape_angle.x, -0x4000, 4, 0x200);
            }
            cLib_addCalc2(&i_this->field_0x84c, 0.4f, 1.0f, 0.02f);
            cLib_addCalc2(&i_this->field_0x698, l_HIO.field_0xc, 1.0f, 0.05f);
        }
        if (i_this->field_0x69c[0] == 0 &&
            (i_this->field_0x5b7 == 1 ||
                pl_check(i_this, i_this->field_0x674) != 0)) {
            if (i_this->mLeafRoomNo != 0) {
                i_this->field_0x66e = 2;
            } else {
                i_this->field_0x66e = 1;
            }
            i_this->field_0x670 = 0;
        }
        break;
    }
}

static void e_yd_appear(e_yd_class* i_this) {
    s32 frame = i_this->mpMorf->getFrame();
    switch (i_this->field_0x670) {
    case 0:
        anm_init(i_this, 6, 10.0f, 0, 1.0f);
        leaf_anm_init(i_this, 20, 10.0f, 2, 1.0f);
        i_this->field_0x670 = 1;
        i_this->field_0x678.x = i_this->actor.home.pos.x + cM_rndFX(100.0f);
        i_this->field_0x678.z = i_this->actor.home.pos.z + cM_rndFX(100.0f);
        if (i_this->mLeafRoomNo != 0) {
            i_this->field_0x678.y = i_this->actor.home.pos.y - (170.0f + cM_rndFX(50.0f));
        } else {
            i_this->field_0x678.y = i_this->actor.home.pos.y + 170.0f + cM_rndFX(50.0f);
        }
        i_this->field_0x694 = 0.2f;
        i_this->mCreatureSound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);
        i_this->mCreatureSound.startCreatureSound(Z2SE_EN_DB_APPEAR, 0, -1);
        break;
    case 1:
        if (frame >= 15 && frame <= 35) {
            i_this->field_0x1235 = 2;
        }
        i_this->field_0xb10 = 1;
        cLib_addCalc2(&i_this->actor.current.pos.x, i_this->field_0x678.x, 0.1f, i_this->field_0x694 * 30.0f);
        cLib_addCalc2(&i_this->actor.current.pos.z, i_this->field_0x678.z, 0.1f, i_this->field_0x694 * 30.0f);
        cLib_addCalc2(&i_this->actor.current.pos.y, i_this->field_0x678.y, 0.1f, i_this->field_0x694 * 30.0f);
        cLib_addCalc2(&i_this->field_0x694, 1.0f, 1.0f, 0.05f);
        if (i_this->mpMorf->isStop()) {
            i_this->field_0x66e = 3;
            i_this->field_0x670 = 0;
        }
        break;
    }

    if (i_this->mLeafRoomNo != 0) {
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.y, i_this->mAngleToPlayer + 0x8000, 8, 0x800);
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.x, 0x8000, 8, 0x400);
    } else {
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.y, i_this->mAngleToPlayer, 8, 0x800);
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.x, 0, 8, 0x400);
    }
    cLib_addCalc2(&i_this->field_0x698, l_HIO.field_0x10, 1.0f, 0.1f);
}

static void e_yd_appear_v(e_yd_class* i_this) {
    cXyz local_34;

    dComIfGp_getPlayer(0);
    f32 fVar2 = TREG_F(17) + 60.0f;
    s16 sVar1 = i_this->field_0x670;
    switch (sVar1) {
    case 0:
        anm_init(i_this, 6, 5.0f, 0, 1.0f);
        leaf_anm_init(i_this, 20, 5.0f, 2, 1.0f);
        i_this->field_0x670 = 1;
        i_this->field_0x69c[0] = JREG_S(6) + 30;
        i_this->field_0x694 = 0;
        i_this->mpMorf->setFrame(15.0f);
    case 1:
        local_34.x = fVar2 * cM_ssin(i_this->field_0x66c * (TREG_S(0) + 1200));
        local_34.z = fVar2 * cM_scos(i_this->field_0x66c * (TREG_S(2) + 1500));
        cLib_addCalc2(&i_this->actor.current.pos.x, i_this->actor.home.pos.x + local_34.x,
            0.1f, i_this->field_0x694 * 40.0f);
        cLib_addCalc2(&i_this->actor.current.pos.z, i_this->actor.home.pos.z + local_34.z,
            0.1f, i_this->field_0x694 * 40.0f);
        cLib_addCalc2(&i_this->actor.current.pos.y, i_this->actor.home.pos.y - (JREG_F(18) + 400.0f),
            0.5f, i_this->field_0x694 * 40.0f);
        cLib_addCalc2(&i_this->field_0x694, 1.0f, 1.0f, JREG_F(5) + 0.2f);
        if (i_this->field_0x69c[0] == 0) {
            i_this->field_0x66e = 3;
            i_this->field_0x670 = 0;
            i_this->field_0x850 = 0.0f;
        }
    default:
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.x, 0x4000, 4, 0x400);
        cLib_addCalc2(&i_this->field_0x84c,
                      JREG_F(4) + 0.5f, 1.0f, 0.05f);
        cLib_addCalc2(&i_this->field_0x698, l_HIO.field_0x10, 1.0f, 0.15f);
        i_this->field_0xb10 = 1;
        break;
    }
}

static void e_yd_wait(e_yd_class* i_this) {
    f32 fVar6 = TREG_F(12) + 30.0f;
    if ((i_this->field_0x66c & 0xf) == 0 && cM_rndF(1.0f) < 0.75f) {
        i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_DB_V_WAIT, -1);
    }
    cXyz commonXyz;
    switch (i_this->field_0x670) {
    case -1:
        if (i_this->mpMorf->isStop() != 0) {
            i_this->field_0x670 = 0;
        } else {
            cXyz cStack_54(0.0f, 90.0f, -100.0f);
            cMtx_YrotS(*calc_mtx, i_this->actor.shape_angle.y);
            cXyz homeOffset;
            MtxPosition(&cStack_54, &homeOffset);
            i_this->field_0x678 = i_this->actor.home.pos + homeOffset;
            cLib_addCalc2(&i_this->actor.current.pos.x, i_this->field_0x678.x, 0.3f, 30.0f);
            cLib_addCalc2(&i_this->actor.current.pos.y, i_this->field_0x678.y, 0.3f, 30.0f);
            cLib_addCalc2(&i_this->actor.current.pos.z, i_this->field_0x678.z, 0.3f, 30.0f);
            break;
        }
    case 0:
        anm_init(i_this, 19, 10.0f, 2, 1.0f);
        leaf_anm_init(i_this, 21, 10.0f, 2, 1.0f);
        i_this->field_0x670 = 1;
        if (i_this->field_0x5b6 == 1) {
            i_this->field_0x69c[1] = cM_rndF(20.0f) + 30.0f;
        } else {
            i_this->field_0x69c[1] = cM_rndF(l_HIO.field_0x16) + l_HIO.field_0x16;
        }
        i_this->field_0x69c[0] = 0;
        break;
    case 1:
        if (i_this->field_0x69c[0] == 0) {
            i_this->field_0x69c[0] = cM_rndF(30.0f) + 10.0f;
            i_this->field_0x678.x = i_this->actor.home.pos.x + cM_rndFX(100.0f);
            i_this->field_0x678.z = i_this->actor.home.pos.z + cM_rndFX(100.0f);
            if (i_this->mLeafRoomNo != 0) {
                i_this->field_0x678.y = i_this->actor.home.pos.y - (cM_rndFX(50.0f) + 170.0f);
            } else {
                i_this->field_0x678.y = i_this->actor.home.pos.y + 170.0f + cM_rndFX(50.0f);
            }
            i_this->field_0x694 = 0;
        }
        commonXyz.x = cM_ssin(i_this->field_0x66c * (TREG_S(0) + 1900)) * fVar6;
        commonXyz.y = cM_ssin(i_this->field_0x66c * (TREG_S(1) + 2200)) * fVar6;
        commonXyz.z = cM_scos(i_this->field_0x66c * (TREG_S(2) + 2000)) * fVar6;
        cLib_addCalc2(&i_this->actor.current.pos.x, i_this->field_0x678.x + commonXyz.x,
            0.1f, i_this->field_0x694 * 20.0f);
        cLib_addCalc2(&i_this->actor.current.pos.z, i_this->field_0x678.z + commonXyz.z,
            0.1f, i_this->field_0x694 * 20.0f);
        cLib_addCalc2(&i_this->actor.current.pos.y, i_this->field_0x678.y + commonXyz.y,
            0.1f, i_this->field_0x694 * 20.0f);
        cLib_addCalc2(&i_this->field_0x694, 1.0f, 1.0f, 0.05f);
        if (i_this->field_0x69c[2] == 0) {
            if (i_this->field_0x5b7 != 1 && pl_check(i_this, i_this->field_0x674 + 100.0f) == 0)
            {
                i_this->field_0x66e = 0;
                i_this->field_0x670 = 2;
                i_this->mCreatureSound.startCreatureSound(Z2SE_EN_DB_HIKKOMU, 0, -1);
            } else {
                if (i_this->field_0x69c[1] == 0 && pl_check(i_this, 700.0f) != 0)
                {
                    i_this->field_0x66e = 4;
                    i_this->field_0x670 = 0;
                }
            }
        }
    default:
        break;
    }

    if (i_this->mLeafRoomNo != 0) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.y, i_this->mAngleToPlayer + 0x8000, 8, 0x800);
        cXyz playerOffset = player->eyePos - i_this->actor.current.pos;
        fVar6 = JMAFastSqrt(playerOffset.x * playerOffset.x + playerOffset.z * playerOffset.z);
        s32 sVar4 = cM_atan2s(playerOffset.y, fVar6);
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.x, sVar4 + 0x8000, 8, 0x400);
        i_this->field_0xb10 = 1;
    } else {
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.y, i_this->mAngleToPlayer, 8, 0x800);
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.x, 0, 8, 0x400);
    }
}

static void e_yd_attack(e_yd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->actor;

    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);

    cXyz commonXyz1;
    cXyz commonXyz2;

    s16 pitch = 0;

    commonXyz1 = player->eyePos - enemy->current.pos;

    pitch = (s16)cM_atan2s(commonXyz1.y,
        JMAFastSqrt(commonXyz1.x * commonXyz1.x + commonXyz1.z * commonXyz1.z));

    switch (i_this->field_0x670) {
    case 0: {
        i_this->field_0x670 = 1;
        i_this->field_0x69c[0] = 17;
        i_this->field_0x694 = 0;
        i_this->field_0xb11 = 0;
        i_this->mCreatureSound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);
        i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_DB_V_ATTACK, -1);
        leaf_anm_init(i_this, 20, 5.0f, 2, 1.0f);
        anm_init(i_this, 7, 5.0f, 0, 1.0f);
    }
    case 1: {
        if (i_this->mLeafRoomNo != 0) {
            cLib_addCalcAngleS2(&enemy->shape_angle.y, i_this->mAngleToPlayer + 0x8000, 8, 0x800);
            commonXyz1.x = 0.0f;
            commonXyz1.y = TREG_F(3) + -170.0f;
            commonXyz1.z = TREG_F(4) + 100.0f;
        } else {
            cLib_addCalcAngleS2(&enemy->shape_angle.y, i_this->mAngleToPlayer, 8, 0x800);
            commonXyz1.x = 0.0f;
            commonXyz1.y = TREG_F(3) + 170.0f;
            commonXyz1.z = TREG_F(4) + -100.0f;
        }
        cMtx_YrotS(*calc_mtx, enemy->shape_angle.y);
        MtxPosition(&commonXyz1, &commonXyz2);
        i_this->field_0x678 = enemy->home.pos + commonXyz2;
        cLib_addCalc2(&enemy->current.pos.x, i_this->field_0x678.x, 0.1f,
                      i_this->field_0x694 * 20.0f);
        cLib_addCalc2(&enemy->current.pos.z, i_this->field_0x678.z, 0.1f,
                      i_this->field_0x694 * 20.0f);
        cLib_addCalc2(&enemy->current.pos.y, i_this->field_0x678.y, 0.1f,
                      i_this->field_0x694 * 20.0f);
        cLib_addCalc2(&i_this->field_0x694, 1.0f, 1.0f, 0.1f);
        if (i_this->field_0x69c[0] == 0) {
            i_this->field_0x670 = 2;
            i_this->field_0x69c[1] = 10;
            if (i_this->mLeafRoomNo != 0) {
                cMtx_YrotS(*calc_mtx, enemy->shape_angle.y + 0x8000);
                cMtx_XrotM(*calc_mtx, -pitch);
                commonXyz1.x = 0.0f;
                commonXyz1.y = TREG_F(5);
                commonXyz1.z = TREG_F(6) + 350.0f;
                MtxPosition(&commonXyz1, &commonXyz2);
                i_this->field_0x678 = enemy->current.pos + commonXyz2;
            } else {
                cMtx_YrotS(*calc_mtx, enemy->shape_angle.y);
                cMtx_XrotM(*calc_mtx, (s16)(-pitch + 0x800));
                commonXyz1.x = 0.0f;
                commonXyz1.y = 0.0f;
                commonXyz1.z = TREG_F(6) + 350.0f;
                MtxPosition(&commonXyz1, &commonXyz2);
                i_this->field_0x678 = enemy->current.pos + commonXyz2;
            }
            enemy->speed.x = fabsf(i_this->field_0x678.x - enemy->current.pos.x);
            enemy->speed.y = fabsf(i_this->field_0x678.y - enemy->current.pos.y);
            enemy->speed.z = fabsf(i_this->field_0x678.z - enemy->current.pos.z);
            i_this->field_0x694 = 0;
            i_this->field_0x9d8.StartCAt(enemy->current.pos);
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0x690 = 0x4000;
            } else {
                i_this->field_0x690 = -0x4000;
            }
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_DB_V_BITE, -1);
        }
        break;
    }
    case 2: {
        if (i_this->field_0x69c[1] != 0) {
            i_this->field_0xb10 = 1;
        }
        cLib_addCalc2(&enemy->current.pos.x, i_this->field_0x678.x, 0.8f,
                      enemy->speed.x * i_this->field_0x694);
        cLib_addCalc2(&enemy->current.pos.z, i_this->field_0x678.z, 0.8f,
                      enemy->speed.z * i_this->field_0x694);
        if (l_HIO.field_0x18 == 1) {
            cLib_addCalcAngleS2(&enemy->shape_angle.z, i_this->field_0x690, 2, 0x2000);
            cLib_addCalc2(&enemy->current.pos.y, i_this->field_0x678.y - 30.0f, 0.8f,
                          enemy->speed.y * i_this->field_0x694);
        } else {
            cLib_addCalc2(&enemy->current.pos.y, i_this->field_0x678.y, 0.8f,
                          enemy->speed.y * i_this->field_0x694);
        }
        cLib_addCalc2(&i_this->field_0x694, 0.2f, 1.0f, 0.05f);
        if (!daPy_getPlayerActorClass()->getDkCaught()) {
            if (!daPy_getPlayerActorClass()->getStCaught() && i_this->field_0x9d8.ChkAtHit()) {
                OS_REPORT("E_YD//////////////AT  SET 1 !!\n");

                i_this->field_0x670 = 5;
                i_this->field_0x694 = 20.0f;
                anm_init(i_this, 8, 2.0f, 2, 1.0f);
                i_this->field_0x69c[0] = 0x78;
                daPy_getPlayerActorClass()->setDkCaught(enemy);
                dComIfGp_getVibration().StartShock(6, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                break;
            }
        }
        if (i_this->mpMorf->isStop()) {
            i_this->field_0x670 = 3;
            i_this->field_0x69c[0] = 0;
        }
        break;
    }
    case 3: {
        if (i_this->field_0x69c[0] == 0) {
            i_this->field_0x66e = 3;
            i_this->field_0x670 = 0;
        }
        break;
    }
    case 5: {
        i_this->field_0x856 = 1;
        i_this->mCreatureSound.startCreatureVoiceLevel(Z2SE_EN_DB_V_BITING, -1);
        i_this->field_0x8a0.OffCoSetBit();
        pitch = 0;
        if (daPy_getPlayerActorClass()->getDkCaught()) {
            if (i_this->field_0x69c[0] == 30) {
                dComIfGp_setItemLifeCount(-1.0f, 0);
            }
            commonXyz1 = player->current.pos - enemy->home.pos;
            s16 yaw = cM_atan2s(commonXyz1.x, commonXyz1.z);
            commonXyz1.y = 0.0f;
            f32 fVar8 = commonXyz1.abs();
            if (fVar8 > 100.0f + YREG_F(2)) {
                f32 fVar7 = (0.1f + YREG_F(0)) * (fVar8 - (100.0f + YREG_F(2)));
                if (fVar7 > 100.0f + YREG_F(1)) {
                    fVar7 = 100.0f + YREG_F(1);
                }
                daPy_getPlayerActorClass()->setOutPower(fVar7, yaw + 0x8000, 0);
            }
            if (i_this->mLeafRoomNo == 0) {
                cMtx_YrotS(*calc_mtx, yaw);
                cLib_addCalcAngleS2(&enemy->shape_angle.z,
                    (i_this->field_0x690 + (2000.0f + KREG_F(17)) *
                        cM_ssin(i_this->field_0x66c * (12000 + KREG_S(0)))),
                    2, 0x2000);
                commonXyz1.x = 0.0f;
                commonXyz1.y = (100.0f + XREG_F(2)) - 30.0f;
                commonXyz1.z = XREG_F(3) - 50.0f;
                MtxPosition(&commonXyz1, &i_this->field_0x678);
                i_this->field_0x678 += player->current.pos;
                cLib_addCalcAngleS2(&enemy->shape_angle.y, yaw, 2, 0x1000);
            } else {
                pitch = -0x4000;
                i_this->field_0x678 = player->eyePos;
                i_this->field_0x678.y += 70.0f + JREG_F(8);
            }
            cLib_addCalc2(&enemy->current.pos.x, i_this->field_0x678.x, 1.0f,
                          i_this->field_0x694);
            cLib_addCalc2(&enemy->current.pos.z, i_this->field_0x678.z, 1.0f,
                          i_this->field_0x694);
            cLib_addCalc2(&enemy->current.pos.y, i_this->field_0x678.y, 1.0f,
                          i_this->field_0x694);
            cLib_addCalc2(&i_this->field_0x694, 200.0f, 1.0f, 10.0f);
        } else {
            i_this->field_0x66e = 3;
            i_this->field_0x670 = -1;
            anm_init(i_this, 0x11, 2.0f, 0, 1.0f);
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_DB_V_RELEASE, -1);
        }
        break;
    }
    }

    if (i_this->field_0xb10 != 0) {
        f32 fVar8 = 50.0f + JREG_F(11);
        enemy->current.pos.y -= fVar8;
        enemy->old.pos.y -= fVar8;
        i_this->field_0x1058.CrrPos(dComIfG_Bgsp());
        enemy->current.pos.y += fVar8;
        enemy->old.pos.y += fVar8;
    }
    if (i_this->mLeafRoomNo != 0) {
        cLib_addCalcAngleS2(&enemy->shape_angle.x, pitch + 0x8000, 8, 0x400);
        i_this->field_0xb10 = 1;
    } else {
        cLib_addCalcAngleS2(&enemy->shape_angle.x, -pitch, 8, 0x400);
    }
}

static void e_yd_chance(e_yd_class* i_this) {
    switch (i_this->field_0x670) {
    case 0:
        if (i_this->mLeafRoomNo != 0) {
            anm_init(i_this, 11, 5.0f, 2, 1.0f);
        } else {
            anm_init(i_this, 10, 5.0f, 2, 1.0f);
        }
        leaf_anm_init(i_this, 21, 5.0f, 2, 1.0f);
        i_this->field_0x670 = 1;
        i_this->field_0x69c[0] = 0x3c;
        i_this->field_0x694 = 0;
    case 1: {
        cXyz unkXyz1;
        unkXyz1.x = cM_ssin(i_this->field_0x66c * (900 + TREG_S(0))) * (60.0f + TREG_F(17));
        unkXyz1.z = cM_scos(i_this->field_0x66c * (1000 + TREG_S(2))) * (60.0f + TREG_F(17));
        cLib_addCalc2(&i_this->actor.current.pos.x,
            i_this->actor.home.pos.x + unkXyz1.x,
            0.1f, i_this->field_0x694 * 20.0f);
        cLib_addCalc2(&i_this->actor.current.pos.z,
            i_this->actor.home.pos.z + unkXyz1.z,
            0.1f, i_this->field_0x694 * 20.0f);
        if (i_this->mLeafRoomNo != 0) {
            cLib_addCalc2(&i_this->actor.current.pos.y,
                i_this->actor.home.pos.y - (230.0f + TREG_F(18)),
                0.1f, i_this->field_0x694 * 20.0f);
        } else {
            cLib_addCalc2(&i_this->actor.current.pos.y,
                i_this->actor.home.pos.y + 230.0f + TREG_F(18),
                0.1f, i_this->field_0x694 * 20.0f);
        }
        cLib_addCalc2(&i_this->field_0x694, 1.0f, 1.0f, 0.1f);
        if (i_this->field_0x69c[0] <= 50 && i_this->field_0x69c[0] >= 10) {
            i_this->mCreatureSound.startCreatureSoundLevel(Z2SE_EN_DB_V_FAINT, 0, -1);
        }
        if (i_this->field_0x69c[0] == 0) {
            i_this->field_0x670++;
            anm_init(i_this, 18, 3.0f, 0, 1.0f);
            leaf_anm_init(i_this, 20, 3.0f, 2, 1.0f);
            i_this->mCreatureSound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);
        }
        break;
    }
    case 2:
        if (i_this->mpMorf->isStop()) {
            i_this->field_0x66e = 3;
            i_this->field_0x670 = 0;
            i_this->field_0x850 = 0;
        }
    }

    if (i_this->mLeafRoomNo != 0) {
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.x, 0x4000, 4, 0x400);
    } else {
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.x, -0x4000, 4, 0x400);
    }
    cLib_addCalc2(&i_this->field_0x84c, 0.5f + JREG_F(4), 1.0f, 0.05f);
}

static void e_yd_s_damage(e_yd_class* i_this) {
    cXyz unkXyz1;
    switch (i_this->field_0x670) {
        case 0:
            anm_init(i_this, 12, 3.0f, 0, 1.0f);
            leaf_anm_init(i_this, 20, 3.0f, 2, 0.0f);
            i_this->mCreatureSound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);
            if (i_this->mLeafRoomNo != 0) {
                cMtx_YrotS(*calc_mtx, i_this->actor.shape_angle.y + 0x8000);
            } else {
                cMtx_YrotS(*calc_mtx, i_this->actor.shape_angle.y);
            }
            unkXyz1.x = 0.0f;
            unkXyz1.y = 15.0f;
            unkXyz1.z = -50.0f + TREG_F(16);
            MtxPosition(&unkXyz1, &i_this->actor.speed);
            i_this->field_0x670 = 1;
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_DB_V_DAMAGE_S, -1);
        break;
    case 1:
        i_this->actor.current.pos += i_this->actor.speed;
        unkXyz1.x = i_this->actor.current.pos.x - i_this->actor.home.pos.x;
        unkXyz1.z = i_this->actor.current.pos.z - i_this->actor.home.pos.z;
        if (JMAFastSqrt(unkXyz1.x * unkXyz1.x + unkXyz1.z * unkXyz1.z) > 250.0f + nREG_F(8)) {
            i_this->actor.speed.zero();
        } else {
            f32 mult = 0.92f;
            i_this->actor.speed.x = i_this->actor.speed.x * mult;
            i_this->actor.speed.y = i_this->actor.speed.y * mult;
            i_this->actor.speed.z = i_this->actor.speed.z * mult;
        }
        if (i_this->field_0x69c[0] == 0) {
            i_this->field_0x66e = 6;
            i_this->field_0x670 = 0;
        }
        if (i_this->field_0x69c[1] == 0) {
            i_this->field_0x66e = 3;
            i_this->field_0x670 = 0;
            i_this->field_0x850 = 0;
        }
        break;
    }
    if (i_this->mLeafRoomNo != 0) {
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.x, 0x4000, 4, 0x400);
    } else {
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.x, -0x4000, 4, 0x400);
    }
}

static void e_yd_damage(e_yd_class* i_this) {
    cXyz unkXyz1;
    switch (i_this->field_0x670) {
    case 0:
        anm_init(i_this, 12, 3.0f, 0, 1.0f);
        leaf_anm_init(i_this, 20, 3.0f, 2, 0.0f);
        i_this->mCreatureSound.startCreatureSound(Z2SE_EN_DB_GRASS, 0, -1);
        cMtx_YrotS(*calc_mtx, i_this->field_0x684);
        unkXyz1.x = 0.0f;
        if (i_this->field_0x5b4 != 1) {
            if (i_this->field_0x5b4 == 2) {
                unkXyz1.y = 20.0f + AREG_F(1);
            } else {
                unkXyz1.y = 30.0f;
            }
            unkXyz1.z = -50.0f;
        } else {
            unkXyz1.y = 0.0f;
            unkXyz1.z = -20.0f;
        }
        MtxPosition(&unkXyz1, &i_this->actor.speed);
        i_this->field_0x670 = 1;
        i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_DB_V_DAMAGE, -1);
        break;
    case 1:
        unkXyz1 = i_this->actor.current.pos - i_this->actor.home.pos;
        if (unkXyz1.abs() > 450.0f + TREG_F(15)) {
            i_this->actor.current.pos = i_this->actor.old.pos;
            i_this->actor.speed.x *= -0.1f;
            i_this->actor.speed.z *= -0.1f;
            i_this->field_0x670++;
        }
    case 2:
        s16 sVar4;
        if (i_this->field_0x5b4 == 2) {
            sVar4 = 0;
        } else {
            sVar4 = -0x8000;
        }
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.y, (i_this->field_0x684 + sVar4), 2, 0x2000);
        i_this->actor.current.pos += i_this->actor.speed;
        if (i_this->field_0x5b4 != 1) {
            if (i_this->field_0x5b4 == 2) {
                i_this->actor.speed.y -= (1.0f + AREG_F(0));
            } else {
                i_this->actor.speed.y -= 5.0f;
            }
        }
        f32 fVar6 = i_this->field_0x698 * (40.0f + TREG_F(15)) * l_HIO.field_0x8;
        i_this->actor.current.pos.y -= fVar6;
        i_this->actor.old.pos.y -= fVar6;
        i_this->field_0x1058.CrrPos(dComIfG_Bgsp());
        i_this->actor.current.pos.y += fVar6;
        i_this->actor.old.pos.y += fVar6;
        if (i_this->mpMorf->isStop()) {
            i_this->field_0x66e = 3;
            i_this->field_0x670 = 0;
        }
        break;
    }
}

static void kuki_control1(e_yd_class* i_this) {
    fopEn_enemy_c* enemy = &i_this->actor;
    cXyz stack_38;
    cXyz stack_2c;
    cXyz stack_20;
    f32 x, y, z;
    f32 fVar9;
    f32 fVar11;
    f32 fVar1;
    f32 fVar2;
    s32 i;
    s16 yaw;
    s16 pitch;

    static s16 pow_xa[11] = {
        0x0000, 0x0000, 0x1000, 0x2000,
        0x3000, 0x4000, 0x3000, 0x2000,
        0x1000, 0x0000, 0x0000
    };

    cXyz* curXyz = &i_this->mJointPositions[0];
    curXyz[0] = enemy->home.pos;

    cXyz stack_14;
    stack_38.x = 0.0f;
    stack_38.y = 0.0f;
    stack_38.z = i_this->field_0x848;

    curXyz++;

    fVar1 = 50.0f + TREG_F(2);

    stack_20.x = 0.0f;
    stack_20.z = 0.0f;

    if (i_this->field_0x66e == 6) {
        stack_20.y = 0.0f;

        fVar1 = 70.0f + JREG_F(1);

        stack_14.set(0.0f, -30.0f + JREG_F(3), 0.0f);
    } else {
        stack_20.y = -(200.0f + TREG_F(0));
    }

    fVar2 = i_this->field_0x850;

    for (i = 1; i < 12; i++, curXyz++) {
        if (i_this->field_0x66e == 6) {
            fVar9 = fVar1 * cM_ssin(i_this->field_0x66c * (1000 + TREG_S(0)) +
                i * (12000 + TREG_S(1)));
            fVar11 = fVar1 * cM_scos(i_this->field_0x66c * (1300 + TREG_S(2)) +
                i * (10000 + TREG_S(3)));
        } else {
            fVar9 = fVar1 * cM_ssin(i_this->field_0x66c * (2000 + TREG_S(0)) +
                i * (12000 + TREG_S(1)));
            fVar11 = fVar1 * cM_scos(i_this->field_0x66c * (2300 + TREG_S(2)) +
                i * (10000 + TREG_S(3)));
            if (i_this->mLeafRoomNo != 0) {
                cMtx_YrotS(*calc_mtx, (s16)enemy->shape_angle.y);
                cMtx_XrotM(*calc_mtx, pow_xa[i - 1] + 0x8000);
            } else {
                cMtx_YrotS(*calc_mtx, (s16)enemy->shape_angle.y);
                cMtx_XrotM(*calc_mtx, (s16)pow_xa[i - 1]);
            }
            MtxScale(fVar2, fVar2, fVar2, 1);
            MtxPosition(&stack_20, &stack_14);
        }

        x = curXyz[0].x - curXyz[-1].x + stack_14.x + fVar9;
        y = curXyz[0].y - curXyz[-1].y - stack_14.y;
        z = curXyz[0].z - curXyz[-1].z + stack_14.z + fVar11;

        pitch = (s16)cM_atan2s(x, z);
        yaw = -cM_atan2s(y, JMAFastSqrt(x * x + z * z));
        cMtx_YrotS(*calc_mtx, pitch);
        cMtx_XrotM(*calc_mtx, yaw);

        MtxPosition(&stack_38, &stack_2c);

        curXyz[0] = curXyz[-1] + stack_2c;
    }
}

static void kuki_control2(e_yd_class* i_this) {
    i_this->mJointPositions[11] = i_this->actor.current.pos;

    cXyz posMult;
    posMult.x = 0.0f;
    posMult.y = 0.0f;
    posMult.z = i_this->field_0x848;

    s32 i = 10;
    s16 pitch;
    s32 yaw;
    cXyz* xyzArr = &i_this->mJointPositions[10];
    csXyz* sxyzArr = &i_this->mJointRotations[10];
    while (i >= 0) {
        f32 xDiff = xyzArr[0].x - xyzArr[1].x;
        f32 yDiff = xyzArr[0].y - xyzArr[1].y;
        f32 zDiff = xyzArr[0].z - xyzArr[1].z;
        yaw = cM_atan2s(xDiff, zDiff);
        pitch = -cM_atan2s(yDiff, JMAFastSqrt(xDiff * xDiff + zDiff * zDiff));
        cMtx_YrotS(*calc_mtx, yaw);
        cMtx_XrotM(*calc_mtx, pitch);
        cXyz pos;
        MtxPosition(&posMult, &pos);
        xyzArr[0] = xyzArr[1] + pos;
        sxyzArr->x = pitch;
        sxyzArr->y = yaw;
        xyzArr -= 1;
        sxyzArr -= 1;
        i--;
    }
}

static f32 dummy_4709() {
    return -30.0f;
}

static f32 dummy_4710() {
    return -200.0f;
}

static void kuki_control3(e_yd_class* i_this) {
    cXyz base = i_this->mJointPositions[0] - i_this->actor.home.pos;
    cXyz* vec = &i_this->mJointPositions[0];
    for (s32 i = 0; i < 12; i++) {
        *vec -= base * ((f32)(12 - i) / 11.0f);
        vec++;
    }
}

static f32 dummy_4861() {
    return -10.0f;
}

static f32 dummy_4862() {
    return 10000.0f;
}

static f32 dummy_4863() {
    return 65536.0f;
}

static f32 dummy_4864() {
    return -5.0f;
}

static void kuki_ha_set(e_yd_class* i_this) {
    cXyz* pos = &i_this->mJointPositions[0];
    csXyz* rot = &i_this->mJointRotations[0];
    for (s32 i = 0; i < 12; i++, pos++, rot++) {
        if (i > 0 && i < 11 && i_this->field_0x77c[i] != NULL) {
            mDoMtx_stack_c::transS(pos->x, pos->y, pos->z);
            mDoMtx_stack_c::YrotM(rot->y);
            mDoMtx_stack_c::XrotM(rot->x);
            mDoMtx_stack_c::ZrotM(i << 13);
#if DEBUG
            if (i_this->actor.current.pos.x == 1.0f &&
                i_this->actor.current.pos.y == 1.0f &&
                i_this->actor.current.pos.z == 1.0f) {
                mDoMtx_stack_c::scaleM(i_this->field_0x7dc[i], i_this->field_0x7dc[i],
                    i_this->field_0x7dc[i]);
            } else {
                mDoMtx_stack_c::scaleM(i_this->actor.current.pos.x, i_this->actor.current.pos.y,
                    i_this->actor.current.pos.z);
            }
#else
            mDoMtx_stack_c::scaleM(i_this->field_0x7dc[i], i_this->field_0x7dc[i],
                i_this->field_0x7dc[i]);
#endif
            i_this->field_0x77c[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }
}

static void action(e_yd_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->actor;

    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);

    cXyz offsetFromHome;
    cXyz unkXyz1;

    i_this->mAngleToPlayer = fopAcM_searchPlayerAngleY(enemy);

    i_this->mHomeDistFromPlayer = (enemy->home.pos - player->current.pos).abs();

    enemy->field_0x566 = 0;

    i_this->field_0x1235 = 1;

    damage_check(i_this);

    if (i_this->field_0x69c[3] != 0 && i_this->field_0x856 != 0) {
        daPy_getPlayerActorClass()->offDkCaught();
        daPy_getPlayerActorClass()->offDkCaught2();
    }

    s8 unkFlag1 = false;
    s8 unusedFlag = false;
    s8 unkFlag2 = false;
    s8 unkFlag3 = false;

    i_this->field_0x8a0.OnCoSetBit();

    i_this->field_0x856 = 0;

    switch (i_this->field_0x66e) {
    case 0:
        e_yd_stay(i_this);
        i_this->field_0x1235 = 0;
        break;
    case 1:
        e_yd_appear(i_this);
        unkFlag1 = true;
        unkFlag2 = true;
        enemy->field_0x566 = 1;
        unkFlag3 = true;
        break;
    case 2:
        e_yd_appear_v(i_this);
        unkFlag1 = true;
        unkFlag2 = true;
        enemy->field_0x566 = 1;
        unkFlag3 = true;
        break;
    case 3:
        e_yd_wait(i_this);
        unkFlag1 = true;
        unkFlag2 = true;
        enemy->field_0x566 = 1;
        unkFlag3 = true;
        break;
    case 4:
        e_yd_attack(i_this);
        unkFlag1 = true;
        unkFlag2 = true;
        enemy->field_0x566 = 1;
        unkFlag3 = true;
        break;
    case 6:
        e_yd_chance(i_this);
        unkFlag1 = true;
        unkFlag2 = true;
        enemy->field_0x566 = 1;
        unkFlag3 = true;
        break;
    case 7:
        e_yd_s_damage(i_this);
        unkFlag1 = true;
        unkFlag2 = true;
        enemy->field_0x566 = 1;
        unkFlag3 = true;
        break;
    case 8:
        e_yd_damage(i_this);
        unkFlag1 = true;
    }

    if (unkFlag3) {
        i_this->mCreatureSound.setLinkSearch(true);
    } else {
        i_this->mCreatureSound.setLinkSearch(false);
    }

    if (unkFlag1) {
        fopAcM_OnStatus(enemy, 0);
        enemy->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(enemy, 0);
        enemy->attention_info.flags = 0;
    }

    if (!unusedFlag) {
        offsetFromHome = enemy->current.pos - enemy->home.pos;

        i_this->field_0x848 = offsetFromHome.abs() * (0.2f + TREG_F(1)) * i_this->field_0x84c;
        if (i_this->field_0x848 > 25.0f + TREG_F(7)) {
            i_this->field_0x848 = 25.0f + TREG_F(7);
        }

        cLib_addCalc2(&i_this->field_0x84c, 1.0f, 1.0f, 0.01f);

        kuki_control1(i_this);
        kuki_control2(i_this);
        kuki_control3(i_this);
        kuki_ha_set(i_this);

        player = fopAcM_SearchByID(i_this->mLeafId);
        if (player != NULL) {
            ((e_yd_leaf_class*)player)->mpMorf->play(NULL, 0, 0);
        }

        cLib_addCalc2(&i_this->field_0x850, 1.0f, 1.0f, 0.05f);
    } else {
        //kuki_control1_e(i_this); // method is stripped in release build
        kuki_ha_set(i_this);
    }

    cXyz* jointPos = &i_this->mJointPositions[0];
    cXyz* lineMatPos = i_this->mLineMat.getPos(0);
    cXyz unkXyz2(-30000.0f, 300000.0f, 30000.0f);
    s32 sphIdx = 0;
    for (s32 i = 0; i < 12;) {
        *lineMatPos = *jointPos;
        if (i == 3 || i == 5 || i == 7 || i == 9) {
            if (unkFlag2) {
                i_this->field_0xb14[sphIdx].SetC(*jointPos);
                i_this->field_0xb14[sphIdx].SetR(30.0f);
            } else {
                i_this->field_0xb14[sphIdx].SetC(*jointPos + unkXyz2);
            }
            dComIfG_Ccsp()->Set(&i_this->field_0xb14[sphIdx]);
            sphIdx++;
        }
        i++;
        lineMatPos++;
        cXyz* _ = jointPos++; // debug match
    }
    cLib_addCalc0(&i_this->field_0x858, 1.0f, 5.0f + TREG_F(12));
    cXyz xyz(1.0f, 1.0f, 1.0f);
    setMidnaBindEffect(enemy, &i_this->mCreatureSound, &enemy->eyePos, &xyz);
}

static void eff_set(e_yd_class* i_this) {
    static u16 p_name[3] = {
        0x8811,
        0x8812,
        0x8813,
    };
    static s32 p_idx[3] = { 2, 2, 6 };

    f32 fVar1;
    J3DModel* model;
    bool bVar5;
    u32 uVar2;
    JPABaseEmitter* pJVar3;
    Mtx* pMVar4;
    s32 iVar6;
    cXyz acStack_48;

    if (i_this->actor.field_0x567 == 0) {
        model = i_this->mpMorf->getModel();
        if (i_this->field_0x1234 != 0) {
            i_this->field_0x1234 -= 1;
            i_this->field_0x1235 = 3;
        }
        if (i_this->field_0x1235 != 1) {
            fVar1 = 0.0f;
        } else {
            fVar1 = 1.0f;
        }
        cLib_addCalc2(&i_this->field_0x1250, fVar1, 1.0f, 0.05f);
        for (iVar6 = 0; iVar6 <= 2; iVar6++) {
            uVar2 = dComIfGp_particle_set(i_this->field_0x1238[iVar6],
                p_name[iVar6], &i_this->actor.current.pos, 0, 0);
            i_this->field_0x1238[iVar6] = uVar2;
            pJVar3 = dComIfGp_particle_getEmitter(i_this->field_0x1238[iVar6]);
            if (pJVar3 != NULL) {
                pMVar4 = (Mtx*)model->getAnmMtx(p_idx[iVar6]);
                MTXCopy(*pMVar4, *calc_mtx);
                MtxScale(i_this->field_0x1250, i_this->field_0x1250, i_this->field_0x1250, 1);
                pJVar3->setGlobalSRTMatrix(*calc_mtx);
            }
        }
        if (i_this->field_0x1235 != 0) {
            if (i_this->field_0x1235 == 1) {
                acStack_48 = i_this->actor.current.pos;
                acStack_48.y += 100.0f;
                bVar5 = fopAcM_gc_c::gndCheck(&acStack_48);
                if (bVar5) {
                    acStack_48.y = fopAcM_gc_c::getGroundY();
                }
                uVar2 = dComIfGp_particle_set(i_this->field_0x1248, 0x8810, &acStack_48, 0, 0);
                i_this->field_0x1248 = uVar2;
            } else if (i_this->field_0x1235 == 2) {
                uVar2 = dComIfGp_particle_set(i_this->field_0x1244,
                    0x880e, &i_this->actor.current.pos, 0, 0);
                i_this->field_0x1244 = uVar2;
                pJVar3 = dComIfGp_particle_getEmitter(i_this->field_0x1244);
                if (pJVar3 != NULL) {
                    pMVar4 = (Mtx*)model->getAnmMtx(1);
                    MTXCopy(*pMVar4, *calc_mtx);
                    pJVar3->setGlobalRTMatrix(*calc_mtx);
                }
            } else if (i_this->field_0x1235 == 3) {
                uVar2 = dComIfGp_particle_set(i_this->field_0x124c,
                    0x880f, &i_this->actor.current.pos, 0, 0);
                i_this->field_0x124c = uVar2;
                pJVar3 = dComIfGp_particle_getEmitter(i_this->field_0x124c);
                if (pJVar3 != NULL) {
                    pMVar4 = (Mtx*)model->getAnmMtx(1);
                    MTXCopy(*pMVar4, *calc_mtx);
                    pJVar3->setGlobalRTMatrix(*calc_mtx);
                }
            }
            i_this->field_0x1235 = 0;
        }
    }
}

static void anm_se_set(e_yd_class* i_this) {
    if (i_this->field_0x664 == 6) {
        if (i_this->mpMorf->checkFrame(22.0f) != 0) {
            i_this->mCreatureSound.startCreatureSound(Z2SE_EN_DB_BERON, 0, -1);
        }
    } else if (i_this->field_0x664 == 7) {
        if (i_this->mpMorf->checkFrame(21.0f) != 0) {
            i_this->mCreatureSound.startCreatureSound(Z2SE_EN_DB_ATTACK, 0, -1);
        }
    } else if (i_this->field_0x664 == 18) {
        if (i_this->mpMorf->checkFrame(15.0f) != 0) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_DB_V_AWAKE, -1);
        }
    } else if (i_this->field_0x664 == 16) {
        if (i_this->mpMorf->checkFrame(3.0f) != 0) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_DB_V_CREEP, -1);
        }
    }
}

static int daE_YD_Execute(e_yd_class* i_this) {
    i_this->field_0x66c++;

    for (s32 i = 0; i < 3; i++) {
        if (i_this->field_0x69c[i] != 0) {
            i_this->field_0x69c[i]--;
        }
    }

    if (i_this->field_0x69c[3] != 0) {
        i_this->field_0x69c[3]--;
    }

    action(i_this);

    anm_se_set(i_this);

    mDoMtx_stack_c::transS(i_this->actor.current.pos.x, i_this->actor.current.pos.y,
                           i_this->actor.current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->actor.shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->actor.shape_angle.x);
    mDoMtx_stack_c::ZrotM(i_this->actor.shape_angle.z);
    cLib_addCalcAngleS2(&i_this->actor.shape_angle.z, 0, 8, 0x400);

    f32 scale = l_HIO.field_0x8 * i_this->field_0x698;
    mDoMtx_stack_c::scaleM(scale, scale, scale);

    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(&i_this->actor)));
    i_this->mBrkAnm->play();

    i_this->mpMorf->modelCalc();

    cXyz cStack_2c;
    cXyz cStack_20;
    cXyz cStack_14(-30000.0f, 10000.0f, -20000.0f);

    MTXCopy(model->getAnmMtx(1), *calc_mtx);

    cStack_2c.set(JREG_F(0) + 30.0f, KREG_F(1), KREG_F(2));

    if (i_this->mLeafRoomNo != 0) {
        i_this->actor.eyePos = i_this->mJointPositions[6 + YREG_S(3)];

        i_this->actor.attention_info.position = i_this->actor.eyePos;

        i_this->field_0x8a0.SetTgType(0xd8fbfdbf);
    } else {
        MtxPosition(&cStack_2c, &i_this->actor.eyePos);

        i_this->actor.attention_info.position = i_this->actor.eyePos;
        i_this->actor.attention_info.position.y += 60.0f;

        i_this->field_0x8a0.SetTgType(0xd8fbfdff);
    }

    MtxPosition(&cStack_2c, &cStack_20);

    if (i_this->field_0x69c[3] != 0) {
        cStack_20 += cStack_14;
    }

    i_this->field_0x8a0.SetC(cStack_20);
    i_this->field_0x8a0.SetR(l_HIO.field_0x8 * (JREG_F(2) + 40.0f) * i_this->field_0x698);

    dComIfG_Ccsp()->Set(&i_this->field_0x8a0);

    if (i_this->field_0xb10 != 0) {
        i_this->field_0x9d8.MoveCAt(cStack_20);

        i_this->field_0xb10 = 0;
    } else {
        i_this->field_0x9d8.SetC(cStack_20 + cStack_14);
    }

    i_this->field_0x9d8.SetR(l_HIO.field_0x8 * (JREG_F(3) + 30.0f) * i_this->field_0x698);

    dComIfG_Ccsp()->Set(&i_this->field_0x9d8);

    eff_set(i_this);

    return 1;
}

static int daE_YD_IsDelete(e_yd_class* i_this) {
    return 1;
}

static int daE_YD_Delete(e_yd_class* i_this) {
    dComIfG_resDelete(&i_this->field_0x5ac, "E_yd");
    if (i_this->field_0x1268 != 0) {
        data_807F7D68 = 0;
    }
    if (i_this->actor.heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    f32 fVar10;

    e_yd_class* a_this = (e_yd_class*)i_this;

    a_this->mpMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_yd", 24),
        NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_yd", 19), 2, 1.0f, 0, -1,
        &a_this->mCreatureSound, 0x80000, 0x11000084);
    if (a_this->mpMorf == NULL || a_this->mpMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = a_this->mpMorf->getModel();

    a_this->mBrkAnm = new mDoExt_brkAnm();
    if (a_this->mBrkAnm == NULL) {
        return 0;
    }

    s32 brkAnmRes = a_this->mBrkAnm->init(model->getModelData(),
        (J3DAnmTevRegKey*)dComIfG_getObjectRes("E_yd", 29), 1, 2, 1.0f, 0, -1);
    if (brkAnmRes == 0) {
        return 0;
    }

    if (!a_this->mLineMat.init(1, 12, (ResTIMG*)dComIfG_getObjectRes("E_yd", 32), 1)) {
        return 0;
    }

    f32* lineMatSize = a_this->mLineMat.getSize(0);
    for (s32 i = 0; i < 0xc; i++, lineMatSize++) {
        *lineMatSize  = (f32)XREG_F(0) * 0.1f + 5.0f;
    }
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_yd", 26);
    JUT_ASSERT(2943, modelData != NULL);

    for (s32 uVar9 = 1; uVar9 < 11; uVar9++) {
        if ((uVar9 & 1) != 0) {
            model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            a_this->field_0x77c[uVar9] = model;
            if (a_this->field_0x77c[uVar9] == 0) {
                return 0;
            }
            fVar10 = cM_rndF(1.0f);
            a_this->field_0x7dc[uVar9] = fVar10 + 1.0f;
        }
    }
    return 1;
}

static int daE_YD_Create(fopAc_ac_c* i_this) {
    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{0x0, 0x0, 0xd}, {(s32)0xd8fbfdff, 0x3}, 0x25}}, // mObj
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
            {0x0, {{0x0, 0x0, 0xd}, {(s32)0xd8fbfdff, 0x3}, 0x0}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 20.0f} // mSph
        } // mSphAttr
    };

    fopAcM_ct(i_this, e_yd_class);
    e_yd_class* a_this = (e_yd_class*)i_this;

    s32 loadRv = dComIfG_resLoad(&a_this->field_0x5ac, "E_yd");
    if (loadRv == 4) {
        OS_REPORT("E_YD PARAM %x\n", fopAcM_GetParam(i_this));

        a_this->field_0x5b4 = fopAcM_GetParam(i_this) & 0xf;
        a_this->field_0x5b6 = fopAcM_GetParam(i_this) >> 4 & 0x3;
        a_this->field_0x5b7 = fopAcM_GetParam(i_this) >> 6 & 0x3;
        a_this->field_0x5b5 = fopAcM_GetParam(i_this) >> 8;
        if (a_this->field_0x5b4 == 0xf) {
            a_this->field_0x5b4 = 0;
        }
        if (((e_yd_class*)i_this)->field_0x5b5 == 0xff || ((e_yd_class*)i_this)->field_0x5b5 == 0) {
            ((e_yd_class*)i_this)->field_0x674 = 500.0f;
        } else {
            ((e_yd_class*)i_this)->field_0x674 = ((e_yd_class*)i_this)->field_0x5b5 * 100.0f;
        }

        OS_REPORT("E_YD//////////////E_YD SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x4700)) {
            OS_REPORT("//////////////E_YD SET NON !!\n");

            return 5;
        }

        OS_REPORT("//////////////E_YD SET 2 !!\n");

        if (data_807F7D68 == 0) {
            ((e_yd_class*)i_this)->field_0x1268 = 1;
            data_807F7D68 = 1;
            l_HIO.field_0x4 = -1;
        }

        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(i_this, ((e_yd_class*)i_this)->mpMorf->getModel()->getBaseTRMtx());

        fopAcM_SetMin(i_this, -300.0f, -400.0f, -300.0f);
        fopAcM_SetMax(i_this, 300.0f, 400.0f, 300.0f);

        i_this->health = 30;
        i_this->field_0x560 = 30;

        a_this->mStts.Init(100, 0, i_this);

        a_this->field_0x8a0.Set(cc_sph_src);
        a_this->field_0x8a0.SetStts(&a_this->mStts);

        a_this->field_0x9d8.Set(at_sph_src);
        a_this->field_0x9d8.SetStts(&a_this->mStts);

        for (s32 i = 0; i < 4; i++) {
            a_this->field_0xb14[i].Set(kuki_sph_src);
            a_this->field_0xb14[i].SetStts(&a_this->mStts);
        }

        a_this->field_0x1058.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1,
            &a_this->field_0x1018, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->field_0x1018.SetWall(50.0f, 50.0f);

        a_this->mCreatureSound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        a_this->mCreatureSound.setEnemyName("E_yd");
        a_this->field_0xff4.mpSound = &a_this->mCreatureSound;
        a_this->field_0xff4.mPowerType = 1;
        a_this->field_0xff4.field_0x18 = 45;

        s8 leafRoomNo = 1;
        a_this->field_0x66c = cM_rndF(65535.0f);
        a_this->field_0x698 = l_HIO.field_0xc;
        if (a_this->field_0x5b4 == 2) {
            i_this->current.angle.x = -0x8000;
            i_this->shape_angle.x = -0x8000;
            a_this->mLeafRoomNo = leafRoomNo;
        }

        leafRoomNo = fopAcM_GetRoomNo(i_this);

        a_this->mLeafId = fopAcM_createChild(PROC_E_YD_LEAF, fopAcM_GetID(i_this), 0,
            &i_this->current.pos, leafRoomNo, &i_this->shape_angle, NULL, -1, NULL);

        i_this->field_0x565 = fopAcM_GetParam(i_this) >> 16;
        i_this->field_0x564 = fopAcM_GetParam(i_this) >> 24;

        daE_YD_Execute((e_yd_class*)i_this);
    }

    return loadRv;
}

static actor_method_class l_daE_YD_Method = {
    (process_method_func)daE_YD_Create,
    (process_method_func)daE_YD_Delete,
    (process_method_func)daE_YD_Execute,
    (process_method_func)daE_YD_IsDelete,
    (process_method_func)daE_YD_Draw,
};

extern actor_process_profile_definition g_profile_E_YD = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_E_YD,              // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(e_yd_class),     // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    143,                    // mPriority
    &l_daE_YD_Method,       // sub_method
    0x10050100,             // mStatus
    fopAc_ENEMY_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
