/**
 * @file d_a_e_sh.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_sh.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"

class daE_SH_HIO_c : public JORReflexible {
public:
    daE_SH_HIO_c();
    virtual ~daE_SH_HIO_c() {}
#if DEBUG
    void genMessage(JORMContext*);
#endif

    /* 0x04 */ s8 mChild;
    /* 0x08 */ f32 mBaseSize;
    /* 0x0C */ f32 mWalkSpeed;
    /* 0x10 */ f32 mRunSpeed;
    /* 0x14 */ f32 mCheck1;
    /* 0x18 */ f32 mCheck2;
    /* 0x1C */ f32 mCheck3;
    /* 0x20 */ f32 mCheck4;
    /* 0x24 */ f32 mCheck5;
    /* 0x28 */ u8 mIgnoreTime;
};

daE_SH_HIO_c::daE_SH_HIO_c() {
    mChild = -1;
    mBaseSize = 1.15f;
    mWalkSpeed = 5.0f;
    mRunSpeed = 20.0f;
    mCheck1 = 600.0f;
    mCheck2 = 700.0f;
    mCheck3 = 800.0f;
    mCheck4 = 900.0f;
    mCheck5 = 500.0f;
    mIgnoreTime = 0;
}

#if DEBUG
void daE_SH_HIO_c::genMessage(JORMContext* ctx) {
    // "Stalhound"
    ctx->genLabel("  スタルハウンド", 0x80000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("基本サイズ", &mBaseSize, 0.0f, 3.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("歩き速度", &mWalkSpeed, 0.0f, 60.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("走り速度", &mRunSpeed, 0.0f, 60.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("チェック１", &mCheck1, 0.0f, 2000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("チェック２", &mCheck2, 0.0f, 2000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("チェック３", &mCheck3, 0.0f, 2000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("チェック４", &mCheck4, 0.0f, 2000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("チェック５", &mCheck5, 0.0f, 2000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genCheckBox("時間無視", &mIgnoreTime, 1, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}
#endif

static void anm_init(e_sh_class* i_this, int i_resIndex, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->mAnm_p->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_sh", i_resIndex), i_attr,
                           i_morf, i_rate, 0.0f, -1.0f);
    i_this->mCurAnm = i_resIndex;
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DJoint* joint = i_joint;
        int jointNo = joint->getJntNo();

        J3DModel* model = j3dSys.getModel();

        e_sh_class* sh = (e_sh_class*)model->getUserArea();
        e_sh_class* sh0 = sh;
        if (sh != NULL) {
            if (jointNo == 13 || jointNo == 11) {
                MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
                cMtx_YrotM(*calc_mtx, sh->field_0x6ac);

                if (jointNo == 11) {
                    cMtx_XrotM(*calc_mtx, -sh->field_0x6ac);
                    cMtx_ZrotM(*calc_mtx, sh->field_0x6ae);
                } else {
                    cMtx_ZrotM(*calc_mtx, sh->field_0x6ae / 2);
                }

                model->setAnmMtx(jointNo, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }

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

static int daE_SH_Draw(e_sh_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->enemy;
    J3DModel* model = i_this->mAnm_p->getModel();

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);

    i_this->mAnm_p->entryDL();

    if (i_this->field_0x676 != 0) {
        cXyz shadowPos;
        shadowPos.set(actor->current.pos.x, actor->current.pos.y + 50.0f + BREG_F(18),
                      actor->current.pos.z);
        i_this->field_0x6c4 = dComIfGd_setShadow(
            i_this->field_0x6c4, 1, model, &shadowPos, 1000.0f, 0.0f, actor->current.pos.y,
            i_this->mObjAcch.GetGroundH(), i_this->mObjAcch.m_gnd, &i_this->enemy.tevStr, 0, 1.0f,
            dDlst_shadowControl_c::getSimpleTex());

        for (int i = 0; i < ARRAY_SIZE(i_this->field_0x664); i++) {
            g_env_light.setLightTevColorType_MAJI(i_this->field_0x664[i], &actor->tevStr);

            J3DModelData* modelData = i_this->field_0x664[i]->getModelData();
            for (u16 j = 0; j < modelData->getMaterialNum(); j++) {
                J3DMaterial* material = modelData->getMaterialNodePointer(j);
                material->getTevColor(1)->r = 0x64;
                material->getTevColor(1)->g = 0xff;
                material->getTevColor(1)->b = 0xff;
            }

            mDoExt_modelUpdateDL(i_this->field_0x664[i]);
        }
    }

    return 1;
}

static void damage_check(e_sh_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->enemy;
    fopAc_ac_c* unusedPlayer = dComIfGp_getPlayer(0);

    if (i_this->field_0x6a0 == 0) {
        i_this->mStts.Move();

        for (int i = 0; i < ARRAY_SIZE(i_this->mSphArr); i++) {
            if (i_this->mSphArr[i].ChkTgHit()) {
                i_this->mAtInfo.mpCollider = i_this->mSphArr[i].GetTgHitObj();

                cc_at_check(actor, &i_this->mAtInfo);

                if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                    i_this->field_0x6a0 = 20;
                } else {
                    i_this->field_0x6a0 = 10;
                }

                i_this->field_0x678 = 0;
                i_this->field_0xceb = 10;

                if (actor->health <= 0) {
                    i_this->field_0x676 = 11;
                    i_this->mSound.startCreatureVoice(Z2SE_EN_SH_V_DEAD, -1);
                    i_this->mSound.startCreatureSound(Z2SE_EN_SH_DEAD, 0, -1);
                    i_this->field_0x6a4 = 45.0f;
                } else {
                    i_this->field_0x676 = 10;
                    if (i_this->mAtInfo.mHitType == 16) {
                        i_this->field_0x6a4 = 30.0f;
                        i_this->mSound.startCreatureVoice(Z2SE_EN_SH_V_DAMAGE, -1);
                    } else {
                        i_this->field_0x6a4 = 45.0f;
                        i_this->mSound.startCreatureVoice(Z2SE_EN_SH_V_DAMAGE, -1);
                    }
                }

                i_this->field_0x6a8 = fopAcM_searchPlayerAngleY(actor);

                if (actor->health > 1) {
                    return;
                }

                actor->health = 0;
                i_this->mSphArr[i].SetTgHitMark(CcG_Tg_UNK_MARK_3);

                return;
            }
        }
    }
}

static bool hio_set;

static daE_SH_HIO_c l_HIO;

static void e_sh_stop(e_sh_class* i_this) {
    static u16 ap_name[3] = {
        dPa_RM(ID_ZI_S_SH_APPEAR_A),
        dPa_RM(ID_ZI_S_SH_APPEAR_B),
        dPa_RM(ID_ZI_S_SH_APPEAR_C),
    };

    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->enemy;

    i_this->field_0x6a0 = 6;

    dScnKy_env_light_c* envLight = dKy_getEnvlight();
    int hourOfDay = envLight->daytime / 15.0f;
    if (l_HIO.mIgnoreTime != 0) {
        hourOfDay = 0;
    }

    switch (i_this->field_0x678) {
    case 0:
        anm_init(i_this, 4, 1.0f, 0, 0.0f);
        i_this->field_0x678 = 1;
        break;
    case 1:
        if ((hourOfDay <= 4 || hourOfDay >= 19) && i_this->field_0x690 < i_this->field_0x6b8) {
            i_this->field_0x676 = 1;
            i_this->field_0x678 = 0;

            i_this->mAnm_p->setPlaySpeed(1.0f);

            actor->shape_angle.y = actor->current.angle.y = i_this->field_0x68c;

            cXyz scale(l_HIO.mBaseSize, l_HIO.mBaseSize, l_HIO.mBaseSize);
            for (int i = 0; i <= 2; i++) {
                dComIfGp_particle_set(ap_name[i], &actor->current.pos,
                                      &actor->shape_angle, &scale);
            }

            i_this->mSound.startCreatureSound(Z2SE_EN_SH_APPEAR, 0, -1);
        }
        break;
    }
}

static void e_sh_appear(e_sh_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->enemy;

    i_this->field_0x6a0 = 6;

    switch (i_this->field_0x678) {
    case 0:
        if (i_this->mAnm_p->checkFrame(34.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_SH_V_APPEAR,-1);
        }

        if (i_this->mAnm_p->isStop()) {
            i_this->field_0x676 = 2;
            i_this->field_0x678 = 0;
        }

        break;
    }
}

static void e_sh_move(e_sh_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->enemy;

    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    cXyz vecToHome;
    dScnKy_env_light_c* envLight = dKy_getEnvlight();
    int hourOfDay = envLight->daytime / 15.0f;
    if (l_HIO.mIgnoreTime != 0) {
        hourOfDay = 0;
    }

    s16 unkShort1 = 0;
    s16 yawDiff;

    i_this->field_0x6aa = 1;
    i_this->field_0x688 = i_this->field_0x68c;

    if ((i_this->field_0x674 & 0xf) == 0 && cM_rndF(1.0f) < 0.5f) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_SH_V_BREATH, -1);
    }

    f32 unkFloat2;
    f32 unkFloat3;
    f32 unkFloat1;
    switch (i_this->field_0x678) {
    case 0:
        i_this->field_0x6b4 = 1.0f;
        anm_init(i_this, 18, 5.0f, 2, i_this->field_0x6b4);

        i_this->field_0x678++;
        i_this->field_0x698[0] = (s16)(cM_rndF(10.0f) + 10.0f);

        if (i_this->field_0xcea != 0) {
            i_this->field_0x694 = cM_rndFX(0.2f) + 1.5f + JREG_F(6);
        } else {
            i_this->field_0x694 = cM_rndFX(0.2f) + 1.0f;
        }
    case 1: {
        unkShort1 = 0x400;
        if (i_this->field_0x690 < l_HIO.mCheck1) {
            cLib_addCalc2(&i_this->field_0x6b4, 1.0f, 1.0f, 0.05f);
            cLib_addCalc0(&actor->speedF, 1.0f, TREG_F(10) + 4.0f);

            if (actor->speedF < 1.0f) {
                if (i_this->field_0xcea != 0) {
                    i_this->field_0x676 = 3;
                    i_this->field_0x678 = 0;
                } else {
                    i_this->field_0x6b4 = cM_rndF(0.1f) + 0.5f;

                    if (cM_rndF(1.0f) < 0.333f) {
                        anm_init(i_this, 14, 10.0f, 2, i_this->field_0x6b4);
                    } else if (cM_rndF(1.0f) < 0.5) {
                        anm_init(i_this, 16, 10.0f, 2, i_this->field_0x6b4);
                    } else {
                        anm_init(i_this, 17, 10.0f, 2, i_this->field_0x6b4);
                    }

                    i_this->field_0x698[0] = (s16)(cM_rndF(100.0f) + 200.0f);
                    i_this->field_0x678 = 2;
                }
            }
        } else {
            if (i_this->field_0x698[0] == 0 && i_this->field_0x6b4 > 1.5f &&
                i_this->field_0x690 > l_HIO.mCheck4)
            {
                i_this->field_0x678 = 5;
            } else {
                unkFloat1 = player->speedF - 10.0f + TREG_F(11);
                if (unkFloat1 < 0.0f) {
                    unkFloat1 = 0.0f;
                }

                cLib_addCalc2(&actor->speedF, unkFloat1 + 5.0f, 1.0f, l_HIO.mWalkSpeed * 0.2f);

                unkFloat2 = actor->speedF * (TREG_F(12) + 0.135f);
                if (unkFloat2 > TREG_F(13) + 2.0f) {
                    unkFloat2 = TREG_F(13) + 2.0f;
                } else if (unkFloat2 < TREG_F(14) + 0.5f) {
                    unkFloat2 = TREG_F(14) + 0.5f;
                }
                cLib_addCalc2(&i_this->field_0x6b4, unkFloat2, 1.0f, 0.1f);
            }
        }

        break;
    }
    case 2: {
        yawDiff = actor->current.angle.y - i_this->field_0x688;
        if (yawDiff > 0x3000 || yawDiff < -0x3000) {
            i_this->field_0x678 = 3;
            i_this->field_0x6b4 = TREG_F(17) + 1.0f;

            anm_init(i_this, 13, 5.0f, 2, i_this->field_0x6b4);
        }

        goto lbl_4a8; //TODO: probably fakematch
    }
    case 3: {
        unkShort1 = TREG_S(8) + 0x200;

        yawDiff = actor->current.angle.y - i_this->field_0x688;
        if (yawDiff < 0x800 && yawDiff > -0x800) {
            i_this->field_0x6b4 = cM_rndF(0.1f) + 0.5f;
            if (cM_rndF(1.0f) < 0.333f) {
                anm_init(i_this, 14, 10.0f, 2, i_this->field_0x6b4);
            } else if (cM_rndF(1.0f) < 0.5) {
                anm_init(i_this, 16, 10.0f, 2, i_this->field_0x6b4);
            } else {
                anm_init(i_this, 17, 10.0f, 2, i_this->field_0x6b4);
            }

            i_this->field_0x698[0] = (s16)(cM_rndF(100.0f) + 200.0f);
            i_this->field_0x678 = 2;
        }

lbl_4a8:

        cLib_addCalc0(&actor->speedF, 1.0f, TREG_F(13) + 1.0f);

        if (i_this->field_0x690 > l_HIO.mCheck2 || i_this->field_0xcea != 0) {
            i_this->field_0x678 = 0;
            if (i_this->field_0xcea != 0) {
                i_this->field_0x694 = cM_rndFX(0.2f) + 1.5f + JREG_F(6);
            }
        } else if (i_this->field_0x690 < l_HIO.mCheck5) {
            i_this->field_0x678 = 4;
        }

        break;
    }
    case 4: {
        i_this->field_0x6b4 = 1.0f;

        anm_init(i_this, 18, 5.0f, 2, i_this->field_0x6b4);

        i_this->field_0x678 = 45;

        i_this->field_0x698[0] = (s16)(cM_rndF(10.0f) + 10.0f);
        i_this->field_0x694 = cM_rndFX(0.2f) + 1.0f;
    }
    case 45: {
        unkShort1 = 0x800;
        i_this->field_0x688 = i_this->field_0x68c + 0x8000;

        if (i_this->field_0x690 > l_HIO.mCheck5 + 100.0f) {
            cLib_addCalc2(&i_this->field_0x6b4, 1.0f, 1.0f, 0.05f);
            cLib_addCalc0(&actor->speedF, 1.0f, TREG_F(14) + 1.0f);

            if (actor->speedF < 1.0f) {
                i_this->field_0x6b4 = cM_rndF(0.1f) + 0.5f;

                if (cM_rndF(1.0f) < 0.333f) {
                    anm_init(i_this, 14, 10.0f, 2, i_this->field_0x6b4);
                } else if (cM_rndF(1.0f) < 0.5) {
                    anm_init(i_this, 16, 10.0f, 2, i_this->field_0x6b4);
                } else {
                    anm_init(i_this, 17, 10.0f, 2, i_this->field_0x6b4);
                }

                i_this->field_0x698[0] = (s16)(cM_rndF(100.0f) + 200.0f);
                i_this->field_0x678 = 2;
            } else {
                unkShort1 = 0x400;
                i_this->field_0x688 = (int)i_this->field_0x68c;
            }
        } else {
            f32 targetSpeed = player->speedF + 5.0f + TREG_F(11);
            cLib_addCalc2(&actor->speedF, targetSpeed, 1.0f,
                          l_HIO.mWalkSpeed * 0.2f);

            unkFloat3 = actor->speedF * (TREG_F(12) + 0.135f);
            if (unkFloat3 > TREG_F(13) + 3.0f) {
                unkFloat3 = TREG_F(13) + 3.0f;
            } else if (unkFloat3 < TREG_F(14) + 0.5f) {
                unkFloat3 = TREG_F(14) + 0.5f;
            }

            cLib_addCalc2(&i_this->field_0x6b4, unkFloat3, 1.0f, 0.1f);

            if (i_this->field_0x6b4 >= 2.0f && i_this->field_0x698[0] == 0) {
                i_this->field_0x6b4 = 0.75f;
                anm_init(i_this, 10, 4.0f, 2, i_this->field_0x6b4);
                i_this->field_0x678 = 46;
                i_this->field_0x698[0] = (s16)(cM_rndF(10.0f) + 10.0f);
            }
        }

        break;
    }
    case 46: {
        unkShort1 = JREG_S(8) + 0xa00;

        i_this->field_0x688 = i_this->field_0x68c + 0x8000;

        cLib_addCalc2(&i_this->field_0x6b4, 1.3499999f, 1.0f, 0.1f);
        cLib_addCalc2(&actor->speedF, i_this->field_0x6b4 * l_HIO.mRunSpeed, 1.0f,
                      l_HIO.mRunSpeed * 0.5f);

        if (i_this->field_0x690 > l_HIO.mCheck5 + 100.0f) {
            i_this->field_0x6b4 = 2.0f;

            anm_init(i_this, 18, 3.0f, 2, i_this->field_0x6b4);

            i_this->field_0x678 = 45;
        }

        break;
    }
    case 5: {
        i_this->field_0x6b4 = 0.75f;

        anm_init(i_this, 10, 4.0f, 2, i_this->field_0x6b4);

        i_this->field_0x678++;
    }
    case 6: {
        unkShort1 = JREG_S(8) + 0xa00;

        cLib_addCalc2(&i_this->field_0x6b4, 1.125f, 1.0f, 0.1f);
        cLib_addCalc2(&actor->speedF, i_this->field_0x6b4 * l_HIO.mRunSpeed, 1.0f,
                      l_HIO.mRunSpeed * 0.5f);

        if (i_this->field_0x698[2] == 0 && i_this->field_0x690 < l_HIO.mCheck3) {
            i_this->field_0x6b4 = 2.0f;

            anm_init(i_this, 18, 3.0f, 2, i_this->field_0x6b4);

            i_this->field_0x678 = 1;
        }

        break;
    }
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->field_0x688, 8, unkShort1);

    i_this->mAnm_p->setPlaySpeed(i_this->field_0x6b4);

    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 4, 0x2000);

    vecToHome = actor->home.pos - actor->current.pos;
    if (vecToHome.abs() > i_this->field_0x6bc || hourOfDay > 4 && hourOfDay < 19) {
        i_this->field_0x676 = 5;
        i_this->field_0x678 = 0;
    }
}

static void e_sh_attack(e_sh_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->enemy;

    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz unusedXyz1;

    s16 unkShort1 = 0;
    i_this->field_0x6aa = 1;
    i_this->field_0x688 = i_this->field_0x68c;

    s16 unkShort2;
    switch (i_this->field_0x678) {
    case 0:
        anm_init(i_this, 15, 5.0f, 2, 1.0f);

        i_this->field_0x678++;

        i_this->field_0x698[0] = (s16)(cM_rndF(15.0f) + 20.0f);
    case 1:
        unkShort1 = 0x400;
        cLib_addCalc0(&actor->speedF, 1.0f, TREG_F(9) + 1.0f);

        unkShort2 = actor->current.angle.y - i_this->field_0x688;

        if (unkShort2 < 0x800 && unkShort2 > -0x800 && i_this->field_0x698[0] == 0) {
            anm_init(i_this, 5, 5.0f, 0, 1.0f);

            i_this->field_0x678++;
        }

        if ((i_this->field_0x674 & 0xf) == 0 && cM_rndF(1.0f) < 0.75f) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_SH_V_BREATH, -1);
        }

        if (i_this->field_0xcea == 0 && (i_this->field_0x674 & 0xf) == 0) {
            i_this->field_0x676 = 2;
            i_this->field_0x678 = 0;
        }
        break;
    case 2:
        unkShort1 = 0x200;

        if (i_this->mAnm_p->isStop()) {
            actor->speedF = JREG_F(2) + 40.0f;
            actor->speed.y = JREG_F(3) + 20.0f;

            anm_init(i_this, 6, 5.0f, 2, 1.0f);

            i_this->field_0x678++;

            i_this->mSound.startCreatureVoice(Z2SE_EN_SH_V_ATTACK, -1);
        }
        break;
    case 3:
        i_this->field_0xce8 = 1;

        if (i_this->mSph2.ChkAtShieldHit()) {
            actor->speedF = -5.0f;
        }

        if (i_this->mObjAcch.ChkGroundHit()) {
            anm_init(i_this, 7, 3.0f, 0, 1.0f);
            i_this->field_0x678++;
        }

        break;
    case 4:
        cLib_addCalc0(&actor->speedF, 1.0f, JREG_F(4) + 5.0f);

        if (i_this->mAnm_p->isStop()) {
            i_this->field_0x676 = 2;
            i_this->field_0x678 = 0;
        }
        break;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->field_0x688, 8, unkShort1);
    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 4, 0x2000);

    if (i_this->field_0x690 > l_HIO.mCheck3) {
        i_this->field_0x676 = 2;
        i_this->field_0x678 = 0;
    }
}

static void e_sh_disappear(e_sh_class* i_this) {
    static u16 dp_name[3] = {
        dPa_RM(ID_ZI_S_SH_DISAPPEAR_A),
        dPa_RM(ID_ZI_S_SH_DISAPPEAR_B),
        dPa_RM(ID_ZI_S_SH_DISAPPEAR_C),
    };

    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->enemy;

    i_this->field_0x6a0 = 6;

    int i;
    switch (i_this->field_0x678) {
    case 0: {
        anm_init(i_this, 12, 5.0f, 0, 1.0f);

        i_this->field_0x678 = 1;

        cXyz partScale(l_HIO.mBaseSize, l_HIO.mBaseSize, l_HIO.mBaseSize);
        for (i = 0; i < 2; i++) {
            dComIfGp_particle_set(dp_name[i], &actor->current.pos, &actor->shape_angle, &partScale);
        }

        i_this->mSound.startCreatureSound(Z2SE_EN_SH_DISAPPEAR, 0, -1);

        break;
    }
    case 1:
        if (i_this->mAnm_p->isStop()) {
            i_this->field_0x676 = 0;
            i_this->field_0x678 = 0;
            actor->current.pos = actor->home.pos;
        }
    }

    cLib_addCalc0(&actor->speedF, 1.0f, TREG_F(10) + 4.0f);
}

static void e_sh_damage(e_sh_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->enemy;

    i_this->field_0x6a0 = 6;
    actor->speedF = 0.0f;

    switch (i_this->field_0x678) {
    case 0:
        anm_init(i_this,9,3.0f,0,1.0f);

        i_this->field_0x678 = 1;

        break;
    case 1:
        if (i_this->mAnm_p->isStop()) {
            i_this->field_0x676 = 2;

            if (i_this->field_0xcea == 0) {
                i_this->field_0x678 = 0;
            } else {
                i_this->field_0x678 = 5;
                i_this->field_0x698[2] = (s16)(cM_rndF(10.0f) + 10.0f);
            }
        }

        break;
    }
}

static void e_sh_dead(e_sh_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->enemy;

    i_this->field_0x6a0 = 6;
    actor->speedF = 0.0f;

    switch (i_this->field_0x678) {
    case 0:
        anm_init(i_this, 11, 7.0f, 0, 1.0f);

        i_this->field_0x678 = 1;

        break;
    case 1:
        if (i_this->mAnm_p->isStop()) {
            i_this->field_0x678 = 2;
            i_this->field_0x698[0] = 30;
        }

        break;
    case 2:
        if (i_this->field_0x698[0] == 0) {
            fopAcM_createDisappear(actor, &actor->current.pos, 10, 0, 42);
            fopAcM_delete(actor);
        }

        break;
    }
}

static void action(e_sh_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    cXyz baseVec;
    cXyz finalVec;

    i_this->field_0x68c = fopAcM_searchPlayerAngleY(enemy);
    i_this->field_0x690 = i_this->field_0x694 * fopAcM_searchPlayerDistance(enemy);
    i_this->field_0xcea = 1;
    i_this->field_0xcec = 0;

    s8 unkFlag1 = TRUE;

    switch (i_this->field_0x676) {
    case 0:
        e_sh_stop(i_this);
        unkFlag1 = FALSE;
        break;
    case 1:
        i_this->field_0xcec = 1;
        e_sh_appear(i_this);
        break;
    case 2:
        i_this->field_0xcec = 1;
        e_sh_move(i_this);
        break;
    case 3:
        i_this->field_0xcec = 1;
        e_sh_attack(i_this);
        break;
    case 5:
        i_this->field_0xcec = 1;
        e_sh_disappear(i_this);
        unkFlag1 = FALSE;
        break;
    case 10:
        e_sh_damage(i_this);
        break;
    case 11:
        e_sh_dead(i_this);
        unkFlag1 = FALSE;
        break;
    }

    damage_check(i_this);

    if (unkFlag1) {
        fopAcM_OnStatus(enemy, 0);
        i_this->enemy.attention_info.flags = fopAc_attn_CARRY_e;
    } else {
        fopAcM_OffStatus(enemy, 0);
        i_this->enemy.attention_info.flags = fopAc_attn_LOCK_e;
    }

    cMtx_YrotS(*calc_mtx, i_this->enemy.current.angle.y);
    baseVec.x = 0.0f;
    baseVec.y = 0.0f;
    baseVec.z = i_this->enemy.speedF;
    MtxPosition(&baseVec, &finalVec);
    i_this->enemy.speed.x = finalVec.x;
    i_this->enemy.speed.z = finalVec.z;
    i_this->enemy.current.pos += enemy->speed;
    i_this->enemy.speed.y = i_this->enemy.speed.y - (JREG_F(5) + 5.0f);

    cXyz* ccMoveP = i_this->mStts.GetCCMoveP();
    if (ccMoveP != NULL) {
        enemy->current.pos.x += ccMoveP->x;
        enemy->current.pos.y += ccMoveP->y;
        enemy->current.pos.z += ccMoveP->z;
    }

    if (i_this->field_0x6a4 > 1.0f) {
        baseVec.x = 0.0f;
        baseVec.y = 0.0f;
        baseVec.z = -i_this->field_0x6a4;
        cMtx_YrotS(*calc_mtx, i_this->field_0x6a8);
        MtxPosition(&baseVec, &finalVec);
        i_this->enemy.current.pos += finalVec;
        cLib_addCalc0(&i_this->field_0x6a4, 1.0f, 6.0f);

        i_this->enemy.speedF = 0.0f;
    }

    i_this->mObjAcch.CrrPos(dComIfG_Bgsp());

    i_this->field_0x6aa = 1;

    if (i_this->field_0x6aa != 0) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);

        if ((i_this->field_0x674 & 0x7) == 0) {
            baseVec = player->eyePos - enemy->current.pos;
            baseVec.y += TREG_F(2) + -60.0f;
            i_this->field_0x6b0 = cM_atan2s(baseVec.x, baseVec.z) - enemy->shape_angle.y;
            i_this->field_0x6b2 =
                enemy->shape_angle.x + cM_atan2s(baseVec.y, JMAFastSqrt(baseVec.x * baseVec.x +
                                                                         baseVec.z * baseVec.z));

            if (i_this->field_0x6b0 > 8000) {
                i_this->field_0x6b0 = 8000;
            } else if (i_this->field_0x6b0 < -8000) {
                i_this->field_0x6b0 = 0xe0c0;
            }

            if (i_this->field_0x6b2 > 8000) {
                i_this->field_0x6b2 = 8000;
            } else if (i_this->field_0x6b2 < -8000) {
                i_this->field_0x6b2 = 0xe0c0;
            }
        }

        i_this->field_0x6aa = 0;
    } else {
        i_this->field_0x6b0 = 0;
    }

    cLib_addCalcAngleS2(&i_this->field_0x6ac, i_this->field_0x6b0 / 2, 4, 0x1000);
    cLib_addCalcAngleS2(&i_this->field_0x6ae, (s16)i_this->field_0x6b2, 4, 0x1000);

    if (i_this->mObjAcch.ChkGroundHit()) {
        dBgS_GndChk dStack_74;

        cMtx_YrotS(*calc_mtx, enemy->shape_angle.y);

        baseVec.x = 0.0f;
        baseVec.y = 100.0f;
        baseVec.z = l_HIO.mBaseSize * -25.0f;
        MtxPosition(&baseVec, &finalVec);
        finalVec += enemy->current.pos;
        dStack_74.SetPos(&finalVec);

        finalVec.y = dComIfG_Bgsp().GroundCross(&dStack_74);
        baseVec = finalVec - enemy->current.pos;

        s16 sVar6 = (s16)cM_atan2s(baseVec.y,
                                   JMAFastSqrt(baseVec.x * baseVec.x + baseVec.z * baseVec.z));
        if (sVar6 <= 0x2000 && sVar6 >= -0x2000) {
            enemy->current.angle.x = sVar6;
        }
    }

    cLib_addCalcAngleS2(&enemy->shape_angle.x, enemy->current.angle.x, 4, 0x1000);

    cXyz effSize(0.7f, 0.7f, 0.7f);
    setMidnaBindEffect(enemy, &i_this->mSound, &enemy->eyePos, &effSize);
}

static void eff_set(e_sh_class* i_this) {
    static u16 p_name[2] = {
        dPa_RM(ID_ZI_S_SH_YODAPOTA_A),
        dPa_RM(ID_ZI_S_SH_YODAPOTA_B),
    };
    static u32 p_idx[2] = {
        13,
        14,
    };

    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->enemy;

    J3DModel* model = i_this->mAnm_p->getModel();

    if (i_this->field_0xceb != 0) {
        i_this->field_0xceb--;
        i_this->field_0xcec = 2;
    }

    JPABaseEmitter* emitter;

    if (i_this->field_0xcec != 0) {
        if (i_this->field_0xcec == 1) {
            for (int i = 0; i <= 1; i++) {
                i_this->field_0xcf0[i] = dComIfGp_particle_set(i_this->field_0xcf0[i], p_name[i],
                                                               &actor->current.pos, 0, 0);

                emitter = dComIfGp_particle_getEmitter(i_this->field_0xcf0[i]);
                if (emitter != NULL) {
                    MTXCopy(model->getAnmMtx(p_idx[i]), *calc_mtx);
                    emitter->setGlobalSRTMatrix(*calc_mtx);
                }
            }
        } else if (i_this->field_0xcec == 2) {
            i_this->field_0xcf8 = dComIfGp_particle_set(
                i_this->field_0xcf8, dPa_RM(ID_ZI_S_SH_YODAHIT_A), &actor->current.pos, 0, 0);

            emitter = dComIfGp_particle_getEmitter(i_this->field_0xcf8);
            if (emitter != NULL) {
                MTXCopy(model->getAnmMtx(13), *calc_mtx);
                emitter->setGlobalRTMatrix(*calc_mtx);
            }
        }
    }
}

static void anm_se_set(e_sh_class* i_this) {
    s8 playSound = FALSE;
    if (i_this->mCurAnm == 4) {
        if (i_this->mAnm_p->checkFrame(42.0f) || i_this->mAnm_p->checkFrame(44.0f) ||
            i_this->mAnm_p->checkFrame(50.0f) || i_this->mAnm_p->checkFrame(61.0f))
        {
            playSound = TRUE;
        }
    } else if (i_this->mCurAnm == 5) {
        if (i_this->mAnm_p->checkFrame(17.0f) || i_this->mAnm_p->checkFrame(18.0f)) {
            playSound = TRUE;
        }
    } else if (i_this->mCurAnm == 7) {
        if (i_this->mAnm_p->checkFrame(0.5f) || i_this->mAnm_p->checkFrame(1.5f) ||
            i_this->mAnm_p->checkFrame(4.0f) || i_this->mAnm_p->checkFrame(6.0f))
        {
            playSound = TRUE;
        }
    } else if (i_this->mCurAnm == 8) {
        if (i_this->mAnm_p->checkFrame(3.0f) || i_this->mAnm_p->checkFrame(9.0f) ||
            i_this->mAnm_p->checkFrame(14.0f) || i_this->mAnm_p->checkFrame(19.0f))
        {
            playSound = TRUE;
        }
    } else if (i_this->mCurAnm == 9) {
        if (i_this->mAnm_p->checkFrame(7.0f) || i_this->mAnm_p->checkFrame(8.0f)) {
            playSound = TRUE;
        }
    } else if (i_this->mCurAnm == 10) {
        if (i_this->mAnm_p->checkFrame(2.0f) || i_this->mAnm_p->checkFrame(4.5f) ||
            i_this->mAnm_p->checkFrame(11.5f) || i_this->mAnm_p->checkFrame(13.5f))
        {
            playSound = TRUE;
        }
    } else if (i_this->mCurAnm == 11) {
        if (i_this->mAnm_p->checkFrame(6.0f) || i_this->mAnm_p->checkFrame(9.5f) ||
            i_this->mAnm_p->checkFrame(14.5f) || i_this->mAnm_p->checkFrame(19.5f))
        {
            playSound = TRUE;
        }
    } else if (i_this->mCurAnm == 13) {
        if (i_this->mAnm_p->checkFrame(1.0f) || i_this->mAnm_p->checkFrame(1.5f) ||
            i_this->mAnm_p->checkFrame(12.5f) || i_this->mAnm_p->checkFrame(16.0f))
        {
            playSound = TRUE;
        }
    } else if (i_this->mCurAnm == 15) {
        if (i_this->mAnm_p->checkFrame(1.0f) || i_this->mAnm_p->checkFrame(8.0f) ||
            i_this->mAnm_p->checkFrame(11.5f) || i_this->mAnm_p->checkFrame(18.0f) ||
            i_this->mAnm_p->checkFrame(21.0f) || i_this->mAnm_p->checkFrame(28.0f) ||
            i_this->mAnm_p->checkFrame(33.0f))
        {
            playSound = TRUE;
        }
    } else if (i_this->mCurAnm == 18 && (i_this->mAnm_p->checkFrame(1.0f) ||
               i_this->mAnm_p->checkFrame(5.0f) || i_this->mAnm_p->checkFrame(12.5f) ||
               i_this->mAnm_p->checkFrame(19.0f)))
    {
        playSound = TRUE;
    }

    if (playSound) {
        i_this->mSound.startCreatureSound(Z2SE_STALHOUND_FOOTNOTE, 0, -1);
    }
}

static int daE_SH_Execute(e_sh_class* i_this) {
    static f32 time_scale[25] = {
        1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.6f, 0.4f, 0.2f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f, 1.0f,
        1.0f,
    };

    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->enemy;

    cXyz unkXyz1;
    cXyz unkXyz2;
    i_this->field_0x674++;

    for (int i = 0; i < 4; i++) {
        if (i_this->field_0x698[i] != 0) {
            i_this->field_0x698[i]--;
        }
    }

    if (i_this->field_0x6a0 != 0) {
        i_this->field_0x6a0--;
    }

    action(i_this);

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::XrotM(actor->shape_angle.x);
    mDoMtx_stack_c::scaleM(l_HIO.mBaseSize, l_HIO.mBaseSize, l_HIO.mBaseSize);
    J3DModel* model = i_this->mAnm_p->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mAnm_p->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));

    anm_se_set(i_this);

    i_this->mAnm_p->modelCalc();

    cLib_addCalc2(&i_this->field_0x66c, i_this->field_0x670, 1.0f, 0.02f);
    i_this->field_0x670 = 1.0f;

    MTXCopy(model->getAnmMtx(13), *calc_mtx);

    camera_class* camera = dComIfGp_getCamera(0);

    s16 rotX;
    s16 rotY;
    f32 scale;
    for (int i = 0; i < ARRAY_SIZE(i_this->field_0x664); i++) {
        MtxPush();

        if (i == 0) {
            unkXyz1.set(15.0f + TREG_F(0), 3.5f + TREG_F(1), TREG_F(2));
            MtxPosition(&unkXyz1, &unkXyz2);
            unkXyz1 = camera->lookat.eye - unkXyz2;
            rotY = cM_atan2s(unkXyz1.x, unkXyz1.z);
            rotX = -cM_atan2s(unkXyz1.y, JMAFastSqrt(unkXyz1.x * unkXyz1.x + unkXyz1.z * unkXyz1.z));

            scale = unkXyz1.abs() * (0.001f + JREG_F(8));
            if (scale > 2.0f + JREG_F(17)) {
                scale = 2.0f + JREG_F(17);
            }

            dScnKy_env_light_c* envLight = dKy_getEnvlight();
            int hourOfDay = envLight->daytime / 15.0f;
            scale = time_scale[hourOfDay] * i_this->field_0x66c * scale;
            unkXyz1.set(15.0f + TREG_F(0), 3.5f + TREG_F(1), 8.0f + TREG_F(3));
        } else {
            unkXyz1.set(15.0f + TREG_F(0), 3.5f + TREG_F(1), -(8.0f + TREG_F(3)));
        }

        MtxPosition(&unkXyz1, &unkXyz2);
        MtxTrans(unkXyz2.x, unkXyz2.y, unkXyz2.z, 0);
        cMtx_YrotM(*calc_mtx, rotY);
        cMtx_XrotM(*calc_mtx, rotX);
        MtxScale(scale, scale, scale, 1);
        i_this->field_0x664[i]->setBaseTRMtx(*calc_mtx);

        MtxPull();
    }

    for (int i = 0; i < ARRAY_SIZE(i_this->mSphArr); i++) {
        if (i_this->field_0xce9 != 0) {
            i_this->mSphArr[i].OnCoSetBit();
        } else {
            i_this->mSphArr[i].OffCoSetBit();
        }
    }

    i_this->field_0xce9 = 1;

    cXyz unkXyz3(15000.0f, -14000.0f, 23000.0f);

    MTXCopy(model->getAnmMtx(2), *calc_mtx);

    unkXyz1.set(15.0f, -20.0f, TREG_F(6));
    MtxPosition(&unkXyz1, &unkXyz2);
    i_this->mSphArr[1].SetC(unkXyz2);
    i_this->mSphArr[1].SetR((40.0f + TREG_F(7)) * l_HIO.mBaseSize);

    if (i_this->field_0x6a0 != 0) {
        i_this->mSphArr[1].SetC(unkXyz2 + unkXyz3);
    } else {
        i_this->mSphArr[1].SetC(unkXyz2);
    }

    dComIfG_Ccsp()->Set(&i_this->mSphArr[1]);

    MTXCopy(model->getAnmMtx(13), *calc_mtx);

    unkXyz1.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&unkXyz1, &actor->eyePos);
    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += 40.0f;

    unkXyz1.set(5.0f + BREG_F(4), -10.0f + BREG_F(5), 0.0f);
    MtxPosition(&unkXyz1, &unkXyz2);
    if (i_this->field_0xce8 != 0) {
        i_this->mSph2.SetC(unkXyz2);
        i_this->mSph2.SetR((35.0f + BREG_F(6)) * l_HIO.mBaseSize);
        i_this->field_0xce8 = 0;
    } else {
        i_this->mSph2.SetC(unkXyz3);
    }

    dComIfG_Ccsp()->Set(&i_this->mSph2);

    unkXyz1.set(5.0f, -5.0f, 0.0f);
    MtxPosition(&unkXyz1, &unkXyz2);
    if (i_this->field_0x6a0 != 0) {
        i_this->mSphArr[0].SetC(unkXyz2 + unkXyz3);
    } else {
        i_this->mSphArr[0].SetC(unkXyz2);
    }
    i_this->mSphArr[0].SetR(30.0f * l_HIO.mBaseSize);

    dComIfG_Ccsp()->Set(&i_this->mSphArr[0]);

    eff_set(i_this);

    return 1;
}

static int daE_SH_IsDelete(e_sh_class* i_this) {
    return TRUE;
}

static int daE_SH_Delete(e_sh_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->enemy;
    fpc_ProcID unusedId = fopAcM_GetID(i_this);

    dComIfG_resDelete(&i_this->mPhase, "E_sh");

    if (i_this->mInitState != 0) {
        hio_set = false;
        mDoHIO_DELETE_CHILD(l_HIO.mChild);
    }

    if (actor->heap != NULL) {
        i_this->mAnm_p->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    u16 i1, i2;

    e_sh_class* sh = (e_sh_class*)i_this;

    sh->mAnm_p = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_sh", 22), NULL, NULL,
                                      (J3DAnmTransform*)dComIfG_getObjectRes("E_sh", 14), 2, 1.0f,
                                      0, -1, &sh->mSound, 0x80000, 0x11000084);

    if (sh->mAnm_p == NULL || sh->mAnm_p->getModel() == NULL) {
        return 0;
    } else {
        J3DModel* model = sh->mAnm_p->getModel();
        model->setUserArea((uintptr_t)sh);
        for (i1 = 0; i1 < model->getModelData()->getJointNum(); i1++) {
            model->getModelData()->getJointNodePointer(i1)->setCallBack(nodeCallBack);
        }

        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_sh", 21);
        JUT_ASSERT(0x711, modelData != NULL);

        for (i2 = 0; i2 < ARRAY_SIZE(sh->field_0x664); i2++) {
            sh->field_0x664[i2] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            if (sh->field_0x664[i2] == NULL) {
                return 0;
            }
        }
    }

    return 1;
}

static int daE_SH_Create(fopAc_ac_c* i_this) {
    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {(s32)0xd8fbfdff, 0x3}, 0x75}}, // mObj
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
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };

    e_sh_class* sh = (e_sh_class*)i_this;

    fopAcM_ct((fopAc_ac_c*)sh, e_sh_class);

    int resLoadResult = dComIfG_resLoad(&sh->mPhase, "E_sh");
    if (resLoadResult == cPhs_COMPLEATE_e) {
        OS_REPORT("", fopAcM_GetParam(i_this));

        sh->field_0x5b4 = (u8)fopAcM_GetParam(i_this);
        u8 bVar1 = (fopAcM_GetParam(i_this) & 0xff00) >> 8;
        u8 bVar2 = (fopAcM_GetParam(i_this) & 0xff0000) >> 16;

        if (bVar1 == 0xff) {
            sh->field_0x6bc = 100000.0f;
        } else {
            sh->field_0x6bc = (f32)bVar1 * 100.0f;
        }

        if (bVar2 == 0xff) {
            sh->field_0x6b8 = 500.0f;
        } else {
            sh->field_0x6b8 = (f32)bVar2 * 100.0f;
        }

        OS_REPORT("");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x2960)) {
            OS_REPORT("");
            return cPhs_ERROR_e;
        }

        OS_REPORT("");
        if (!hio_set) {
            sh->mInitState = 1;
            hio_set = true;
            l_HIO.mChild = mDoHIO_CREATE_CHILD("E_sh", &l_HIO);
        }

        i_this->attention_info.flags = fopAc_attn_CARRY_e;

        fopAcM_SetMtx(i_this, sh->mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(i_this, 200.0f, 200.0f, 200.0f);
        i_this->field_0x560 = i_this->health = 40;
        sh->mStts.Init(200, 0, i_this);
        sh->mSphArr[0].Set(cc_sph_src);
        sh->mSphArr[0].SetStts(&sh->mStts);
        sh->mSphArr[1].Set(cc_sph_src);
        sh->mSphArr[1].SetStts(&sh->mStts);
        sh->mSph2.Set(at_sph_src);
        sh->mSph2.SetStts(&sh->mStts);

        sh->mObjAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1,
                         &sh->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);

        sh->mAcchCir.SetWall(50.0f, 60.0f);

        sh->mSound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);

        sh->mAtInfo.mpSound = &sh->mSound;
        sh->mAtInfo.field_0x18 = 0x22;
        sh->mAtInfo.mPowerType = 4;

        sh->mSound.setEnemyName("E_sh");

        sh->field_0x694 = cM_rndFX(0.2f) + 1.0f;

        daE_SH_Execute(sh);
    }

    return resLoadResult;
}

static actor_method_class l_daE_SH_Method = {
    (process_method_func)daE_SH_Create,
    (process_method_func)daE_SH_Delete,
    (process_method_func)daE_SH_Execute,
    (process_method_func)daE_SH_IsDelete,
    (process_method_func)daE_SH_Draw,
};

actor_process_profile_definition g_profile_E_SH = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_E_SH,              // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(e_sh_class),     // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    126,                    // mPriority
    &l_daE_SH_Method,       // sub_method
    0x00040100,             // mStatus
    fopAc_ENEMY_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
