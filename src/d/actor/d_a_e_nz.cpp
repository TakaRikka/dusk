/**
 * @file d_a_e_nz.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_nz.h"
#include "c/c_damagereaction.h"
#include "d/d_cc_d.h"
#include "Z2AudioLib/Z2Instances.h"
#include "f_op/f_op_actor_enemy.h"

class daE_NZ_HIO_c : public JORReflexible {
public:
    daE_NZ_HIO_c();
    virtual ~daE_NZ_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 mId;
    /* 0x08 */ f32 mBasicSize;
    /* 0x0C */ f32 mSpeed;
    /* 0x10 */ f32 mAttackSpeed;
    /* 0x14 */ s16 mWaitTime;
    /* 0x18 */ f32 mCurrentAlphaSpeed;
    /* 0x1C */ f32 mVanishingAlphaSpeed;
};

enum Action {
    ACTION_NORMAL,
    ACTION_ATTACK,
    ACTION_UNKNOWN,
    ACTION_STICK,
    ACTION_DAMANGE,
};

static u8 stick_bit[8] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
};

struct StickDef {
    s8 field_0x0;
    s16 field_0x2;
    s16 field_0x4;
    s16 field_0x6;
    s16 field_0x8;
};

static StickDef stick_d[8] = {
    0x03, 1500, 15000, 0, 0x14,
    0x00, 0, 32767,0, 0,
    0x01, 7000, -30000, 0x00, 0,
    0x02, 1000, 6000, 1500, 0x19,
    0x03, -25000, -10000, 22000, 0x14,
    0x02, 8000, -22000, -3000, 0x0F,
     0x01, 30000, -4100, 1500, 0x19,
     0x02,  5500, -20000, 24000, 0x0F,
};

daE_NZ_HIO_c::daE_NZ_HIO_c() {
    mId = -1;
    mBasicSize = 1.2f;
    mSpeed = 25.0f;
    mAttackSpeed = 45.0f;
    mWaitTime = 5;
    mCurrentAlphaSpeed = 60.0f;
    mVanishingAlphaSpeed = 30.0f;
}

#if DEBUG
void daE_NZ_HIO_c::genMessage(JORMContext* ctx) {
    // Ghost Rat
    ctx->genLabel("  幽霊ネズミ", 0x80000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Basic Size
    ctx->genSlider("基本サイズ", &mBasicSize, 0.0f, 5.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Basic Speed
    ctx->genSlider("移動速度", &mSpeed, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack Speed
    ctx->genSlider("攻撃速度", &mAttackSpeed, 0.0f, 60.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack Wait Time
    ctx->genSlider("出現タイムラグ", &mWaitTime, 0, 0x23, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Current alpha speed
    ctx->genSlider("現α速度", &mCurrentAlphaSpeed, 0.0f, 255.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Vanishing alpha speed
    ctx->genSlider("消α速度", &mVanishingAlphaSpeed, 0.0f, 255.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}
#endif

static void anm_init(e_nz_class* i_this, int param_2, f32 param_3, u8 param_4, f32 param_5) {
    i_this->mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_NZ", param_2), param_4,
                             param_3, param_5, 0.0f, -1.0f);
    i_this->field_0x5e4 = param_2;
}

static BOOL pl_check(e_nz_class* i_this, f32 param_1) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    dComIfGp_getPlayer(0);
    if (i_this->mPlayerDistance < param_1) {
        s16 angleDiff = a_this->shape_angle.y - i_this->mPlayerAngleY;
        if (angleDiff < 0x5000 && angleDiff > -0x5000) {
            return TRUE;
        }
    }
    return FALSE;
}

static int daE_NZ_Draw(e_nz_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    J3DModel* model = i_this->mpMorf->getModel();
    if (i_this->field_0x5b8 != 0) {
        return 1;
    }

    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    if (i_this->mMaterialAlpha < 1.0f) {
        return 1;
    }

    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);
    J3DModelData* modelData = model->getModelData();
    u8 alpha = i_this->mMaterialAlpha;
    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        J3DGXColorS10* tevColor = modelData->getMaterialNodePointer(i)->getTevColor(2);
        tevColor->a = alpha;
    }
    i_this->mInvModel.entryDL(NULL);

    return 1;
}

bool l_HIOInit;

static daE_NZ_HIO_c l_HIO;

static u8 data_8072C454[4];

static void e_nz_normal(e_nz_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    f32 dVar9 = 0.0f;
    cXyz local_44;
    if ((i_this->field_0x698 & 0x1f) == 0 && cM_rndF(1.0f) < 0.5f) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_NZ_V_NAKU, -1);
    }

    switch(i_this->mSubAction) {
    case 0:
        anm_init(i_this, 9, 3.0f, 2, 1.0f);
        i_this->field_0x6a2[0] = cM_rndF(60.0f) + 30.0f;
        i_this->mSubAction = 1;
        i_this->field_0x698 = cM_rndF(65535.0f);
        break;
    case 1:
        if (i_this->mpMorf->getFrame() >= 1.0f && i_this->mpMorf->getFrame() <= 7.0f) {
            dVar9 = l_HIO.mSpeed;
            a_this->speedF = dVar9;
        }

        if (i_this->mpMorf->checkFrame(1.0f)) {
            i_this->field_0x5d4 += (int)cM_rndFX(3000.0f);
            a_this->speed.y = 20.0f;
        }

        if (i_this->mpMorf->checkFrame(9.0f)) {
            i_this->field_0xa78 = 1;
        }

        if (i_this->field_0x6a2[0] == 0 && i_this->mpMorf->checkFrame(10.0f)) {
            i_this->field_0x6a2[0] = cM_rndF(60.0f) + 30.0f;
            i_this->mSubAction = 2;
            if (cM_rndF(1.0f) < 0.5f) {
                anm_init(i_this, 10, 3.0f, 2, 1.0f);
            } else {
                anm_init(i_this, 8, 3.0f, 2, 1.0f);
            }
        }

        if (i_this->field_0x6a2[2] == 0 && fopAcM_wayBgCheck(a_this, 200.0f, 50.0f)) {
            i_this->field_0x6a2[2] = 20;
            i_this->field_0x6a2[1] = cM_rndF(10.0f) + 20.0f;
            i_this->field_0x5d4 = a_this->current.angle.y + 0x8000;
        }
        break;
    case 2:
        if (i_this->field_0x6a2[0] == 0) {
            i_this->mSubAction = 3;
            i_this->field_0x6a2[0] = 5;
            local_44.x = a_this->home.pos.x + cM_rndFX(500.0f) - a_this->current.pos.x;
            local_44.z = a_this->home.pos.z + cM_rndFX(500.0f) - a_this->current.pos.z;
            i_this->field_0x5d4 = cM_atan2s(local_44.x, local_44.z);
        }
        break;
    case 3:
        if (i_this->field_0x6a2[0] == 0) {
            i_this->mSubAction = 0;
        }
        break;
    }

    if (data_8072C454[0] != 0xff && pl_check(i_this, 700.0f)) {
        i_this->field_0x5d4 = i_this->mPlayerAngleY;
        if (i_this->mPlayerDistance < 400.0f &&
            (i_this->field_0x5e4 == 8 ||
             ((i_this->field_0x5e4 == 9 && i_this->mpMorf->checkFrame(10.0f)))))
        {
            for (int i = 0; i < 8; i++) {
                if ((data_8072C454[0] & stick_bit[i]) == 0) {
                    data_8072C454[0] |= stick_bit[i];
                    i_this->field_0x6ac = i + 1;
                    break;
                }
            }
            i_this->mAction = ACTION_ATTACK;
            i_this->mSubAction = 0;
        }
    }
    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5d4, 2, 0x2000);
    cLib_addCalc2(&a_this->speedF, dVar9, 1.0f, l_HIO.mSpeed * 0.25f);
}

static s8 e_nz_attack(e_nz_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    cXyz local_38;
    s8 rv = 0;
    switch(i_this->mSubAction) {
    case 0:
        anm_init(i_this, 7, 3.0f, 0, 1.0f);
        i_this->mSubAction = 1;
        a_this->speedF = 0.0f;
        break;
    case 1:
        i_this->field_0x5c8 = i_this->field_0x5bc;
        local_38 = i_this->field_0x5bc - a_this->current.pos;
        a_this->current.angle.y = cM_atan2s(local_38.x, local_38.z);
        a_this->current.angle.x = -cM_atan2s(local_38.y, JMAFastSqrt(local_38.x * local_38.x + local_38.z * local_38.z));
        if (!i_this->mpMorf->isStop()) {
            break;
        }
        anm_init(i_this, 6, 5.0f, 0, 1.0f);
        i_this->mSubAction = 2;
        a_this->speedF = l_HIO.mAttackSpeed;
        i_this->field_0x5dc = 25.0f;
        i_this->field_0x6a2[0] = 20;
        // fallthrough
    case 2:
        rv = 1;
        local_38 = i_this->field_0x5bc - a_this->current.pos;
        if (local_38.abs() < a_this->speedF * 2.0f) {
            i_this->mSubAction = 3;
        } else {
            local_38 = i_this->field_0x5c8 - i_this->field_0x5bc;
            if (local_38.abs() > 50.0f || i_this->field_0x6a2[0] == 0) {
                i_this->mSubAction = 5;
            }
        }
        break;
    case 3:
        i_this->field_0x5c8 = i_this->field_0x5bc;
        rv = 2;
        a_this->speedF = 0.0f;
        cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x5bc.x, 1.0f, 100.0f);
        cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x5bc.y, 1.0f, 100.0f);
        cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x5bc.z, 1.0f, 100.0f);
        local_38 = i_this->field_0x5bc - a_this->current.pos;
        if (local_38.abs() < 5.0f) {
            i_this->mAction = ACTION_STICK;
            i_this->mSubAction = 0;
        }
        break;
    case 5:
        cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x1000);
        if (i_this->mAcch.ChkGroundHit() || i_this->field_0x8c8) {
            i_this->mAction = ACTION_NORMAL;
            i_this->field_0x6a2[0] =  cM_rndF(30.0f) + 30.0f;
            i_this->mSubAction = 2;
            anm_init(i_this, 8, 3.0f, 2, 1.0f);
            a_this->current.angle.x = 0;
            data_8072C454[0] &= ~stick_bit[i_this->field_0x6ac - 1];
            i_this->field_0x6ac = 0;
        }
        break;
    }
    
    return rv;
}

static void e_nz_stick(e_nz_class* i_this) {
    s8 cVar4 = 0;

    switch(i_this->mSubAction) {
    case 0:
        anm_init(i_this, 5, 3.0f, 2, 1.0f);
        i_this->mSubAction = 1;
        dComIfGp_getVibration().StartShock(1, 1, cXyz(0.0f, 1.0f, 0.0f));
        break;
    case 1:
        if (i_this->mpMorf->checkFrame(2.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_NZ_BITE, 0, -1);
        }
        for (int i = 0; i < 8; i++) {
            if ((data_8072C454[0] & stick_bit[i]) != 0) {
                cVar4++;
            }
        }
        if (cVar4 >= 3) {
            daPy_getLinkPlayerActorClass()->onHeavyState();
        }
        break;
    }
}

static void damage_check(e_nz_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    dComIfGp_getPlayer(0);
    if (i_this->field_0x6aa == 0) {
        i_this->mStts.Move();
        if (i_this->mSph.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mSph.GetTgHitObj();
            cc_at_check(a_this, &i_this->mAtInfo);
            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_MIDNA_LOCK | AT_TYPE_10000000 | AT_TYPE_WOLF_CUT_TURN | AT_TYPE_WOLF_ATTACK)) {
                i_this->field_0x6aa = 20;
            } else {
                i_this->field_0x6aa = 10;
            }

            if (i_this->mAction == ACTION_STICK) {
                a_this->current.angle.y = (i_this->field_0x6ac - 1) * 0x2000;
            } else {
                a_this->current.angle.y = i_this->mAtInfo.mHitDirection.y;
            }
            i_this->mAction = ACTION_DAMANGE;
            i_this->mSubAction = 0;
            i_this->mSound.startCreatureVoice(Z2SE_EN_NZ_V_DEATH, -1);
        }

        if (a_this->health <= 1) {
            a_this->health = 0;
            i_this->mSph.SetTgHitMark(CcG_Tg_UNK_MARK_3);
        }
    }
}

static void e_nz_damage(e_nz_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    i_this->field_0x6aa = 6;
    switch (i_this->mSubAction) {
    case 0:
        anm_init(i_this, 4, 5.0f, 0, 1.0f);
        i_this->mSubAction = 1;
        a_this->speed.y = cM_rndF(10.0f) + 40.0f;
        a_this->speedF = -30.0f;
        i_this->field_0x6a2[0] = cM_rndF(5.0f) + 15.0f;
        // fallthrough
    case 1:
        if (i_this->field_0x6a2[0] == 0) {
            fopAcM_createDisappear(a_this, &a_this->eyePos, 6, 3, 0x27);
            if (i_this->field_0x5b6 == 1) {
                a_this->current = a_this->home;
                a_this->old = a_this->current;
                a_this->health = 10;
                i_this->mAction = ACTION_NORMAL;
                i_this->mSubAction = 0;
                i_this->mMaterialAlpha = 0.0f;
                if (i_this->field_0x6ac != 0) {
                    data_8072C454[0] &= ~stick_bit[i_this->field_0x6ac - 1];
                    i_this->field_0x6ac = 0;
                }
            } else {
                fopAcM_delete(a_this);
            }
        }
        break;
    }
}

static BOOL getPolyColor(cBgS_PolyInfo& param_1, int param_2, _GXColor* param_3,
                             _GXColor* param_4, u8* param_5, f32* param_6) {
    if (!dComIfG_Bgsp().ChkPolySafe(param_1)) {
        return FALSE;
    }

    if (param_2 == 0) {
        dKy_pol_eff_prim_get(&param_1, param_3);
        dKy_pol_eff_env_get(&param_1, param_4);
        *param_5 = dKy_pol_eff_alpha_get(&param_1);
        *param_6 = dKy_pol_eff_ratio_get(&param_1);
    } else {
        dKy_pol_eff2_prim_get(&param_1, param_3);
        dKy_pol_eff2_env_get(&param_1, param_4);
        *param_5 = dKy_pol_eff2_alpha_get(&param_1);
        *param_6 = dKy_pol_eff2_ratio_get(&param_1);
    }

    return TRUE;
}

static s8 action(e_nz_class* i_this) {
    static u16 eff_id[4] = {
        0x01B8,
        0x01B9,
        0x01BA,
        0x01BB,
    };

    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    cXyz local_74;
    cXyz local_80;
    i_this->mPlayerDistance = fopAcM_searchPlayerDistance(a_this);
    i_this->mPlayerAngleY = fopAcM_searchPlayerAngleY(a_this);
    damage_check(i_this);
    s8 action_result = 0;
    s8 is_active = 0;

    switch (i_this->mAction) {
    case ACTION_NORMAL:
        e_nz_normal(i_this);
        break;
    case ACTION_DAMANGE:
        e_nz_damage(i_this);
        break;
    case ACTION_ATTACK:
        action_result = e_nz_attack(i_this);
        is_active = 1;
        break;

    case ACTION_STICK:
        if (daPy_py_c::getMidnaActor()) {
            daPy_py_c::getMidnaActor()->onRatBody(0);
        }
        e_nz_stick(i_this);
        action_result = 3;
        is_active = 1;
        break;
    }

    if (is_active) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 2, 0x2000);
    cLib_addCalcAngleS2(&a_this->shape_angle.z, a_this->current.angle.z, 2, 0x2000);

    if (action_result == 0) {
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        local_74.x = 0.0f;
        local_74.y = 0.0f;
        local_74.z = a_this->speedF;
        MtxPosition(&local_74, &local_80);
        a_this->speed.x = local_80.x;
        a_this->speed.z = local_80.z;
        a_this->current.pos += a_this->speed * l_HIO.mBasicSize;
        a_this->speed.y += a_this->gravity;
        a_this->gravity = -8.0f;
        if (a_this->speed.y < -120.0f) {
            a_this->speed.y = -120.0f;
        }
    } else if (action_result == 1) {
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
        local_74.x = 0.0f;
        local_74.y = 0.0f;
        local_74.z = a_this->speedF;
        MtxPosition(&local_74, &a_this->speed);
        a_this->current.pos += a_this->speed * l_HIO.mBasicSize;
    }

    i_this->mAcch.CrrPos(dComIfG_Bgsp());

    dBgS_ObjGndChk_Spl gnd_chk;
    i_this->field_0x8c8 = 0;
    local_74 = a_this->current.pos;
    local_74.y += 200.0f;
    gnd_chk.SetPos(&local_74);
    if (a_this->current.pos.y <= dComIfG_Bgsp().GroundCross(&gnd_chk)) {
        a_this->current.pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
        a_this->speed.y = 0.0f;
        i_this->field_0x8c8 = 1;
    }

    if (i_this->field_0xa78) {
        if (i_this->mPlayerDistance < 5000.0f) {
            static cXyz sc(0.35f, 0.35f, 0.35f);
            u8 alpha;
            f32 ratio;
            GXColor prim_color;
            GXColor env_color;
            if (getPolyColor(i_this->mAcch.m_gnd, 0, &prim_color, &env_color, &alpha, &ratio)) {
                if (i_this->field_0x8c8 != 0) {
                    for (int i = 0; i < 4; i++) {
                        i_this->mParticleIds[i] = dComIfGp_particle_setColor(
                            i_this->mParticleIds[i], eff_id[i], &a_this->current.pos,
                            &a_this->tevStr, &prim_color, &env_color, ratio, alpha,
                            &a_this->shape_angle, &sc, NULL, -1, NULL);
                    }
                    i_this->mSound.startCreatureSound(Z2SE_EN_NZ_FN_WATER, 0, -1);
                } else {
                    i_this->mParticle = dComIfGp_particle_setColor(
                        i_this->mParticle, 0xe6, &a_this->current.pos, &a_this->tevStr,
                        &prim_color, &env_color, ratio, alpha, &a_this->shape_angle, &sc, NULL, -1,
                        NULL);
                    JPABaseEmitter* pEmitter =
                        dComIfGp_particle_getEmitter(i_this->mParticle);
                    if (pEmitter != NULL) {
                        pEmitter->setRate(1.0f);
                        i_this->mSound.startCreatureSound(Z2SE_EN_NZ_FOOTNOTE, 0, -1);
                    }
                }
            }
        }
        i_this->field_0xa78 = 0;
    }

    cXyz bind_scale(0.5f, 0.5f, 0.5f);
    setMidnaBindEffect(a_this, &i_this->mSound, &a_this->eyePos, &bind_scale);

    return action_result;
}

static int daE_NZ_Execute(e_nz_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    
    f32 alphaTarget = 0.0f;
    f32 alphaStep = l_HIO.mVanishingAlphaSpeed;
    if (daPy_py_c::checkNowWolfPowerUp()) {
        if (i_this->field_0x6a2[3] == 0) {
            alphaTarget = 255.0f;
            alphaStep = l_HIO.mCurrentAlphaSpeed;
        }
    } else {
        i_this->field_0x6a2[3] = l_HIO.mWaitTime;
    }

    cLib_addCalc2(&i_this->mMaterialAlpha, alphaTarget, 1.0f, alphaStep);
    
    if (cDmrNowMidnaTalk() || dComIfGp_event_runCheck()) {
        return 1;
    }

    cXyz local_58;
    cXyz local_64;
    cXyz local_70;
    
    if (i_this->field_0x5b8 != 0) {
        if (dComIfGs_isSwitch(i_this->field_0x5b8, fopAcM_GetRoomNo(a_this))) {
            i_this->field_0x5b8 = 0;
        } else {
            return 1;
        }
    }
    
    i_this->field_0x698++;
    
    for (int i = 0; i < 4; i++) {
        if (i_this->field_0x6a2[i] != 0) {
            i_this->field_0x6a2[i]--;
        }
    }
    
    if (i_this->field_0x6aa != 0) {
        i_this->field_0x6aa--;
    }
    
    J3DModel* model = i_this->mpMorf->getModel();
    
    if (i_this->mAction == ACTION_STICK || i_this->mAction == ACTION_ATTACK) {
        daPy_py_c* player = daPy_getLinkPlayerActorClass();
        MtxP joint_mtx = player->getModelJointMtx(stick_d[i_this->field_0x6ac - 1].field_0x0);
        MTXCopy(joint_mtx, *calc_mtx);
        cMtx_YrotM(*calc_mtx, stick_d[i_this->field_0x6ac - 1].field_0x2);
        cMtx_XrotM(*calc_mtx, stick_d[i_this->field_0x6ac - 1].field_0x4);
        cMtx_ZrotM(*calc_mtx, stick_d[i_this->field_0x6ac - 1].field_0x6);
        MtxTrans(0.0f, stick_d[i_this->field_0x6ac - 1].field_0x8, 0.0f, 1);
        local_58.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&local_58, &i_this->field_0x5bc);
        
        if (i_this->mAction == ACTION_STICK) {
            model->setBaseTRMtx(*calc_mtx);
            a_this->current.pos = i_this->field_0x5bc;
        }
    }
    
    if (action(i_this) != 3) {
        i_this->field_0x5d8 += i_this->field_0x5dc;
        i_this->field_0x5dc -= 8.0f;
        if (i_this->field_0x5d8 < 0.0f) {
            i_this->field_0x5dc = 0.0f;
            i_this->field_0x5d8 = 0.0f;
        }
        
        mDoMtx_stack_c::transS(a_this->current.pos.x,
                                a_this->current.pos.y + i_this->field_0x5d8,
                                a_this->current.pos.z);
        mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
        mDoMtx_stack_c::XrotM(a_this->shape_angle.x);
        mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
        mDoMtx_stack_c::scaleM(l_HIO.mBasicSize, l_HIO.mBasicSize, l_HIO.mBasicSize);
        model->setBaseTRMtx(mDoMtx_stack_c::get());
    }
    
    i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    i_this->mpMorf->modelCalc();
    
    MtxP joint_mtx = model->getAnmMtx(6);
    cMtx_copy(joint_mtx, *calc_mtx);
    local_58.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&local_58, &a_this->eyePos);
    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += 40.0f;
    local_58.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&local_58, &local_64);
    
    if (i_this->mMaterialAlpha < 1.0f) {
        local_64.z += 10000.0f;
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    } else {
        if (i_this->mAction == ACTION_STICK) {
            i_this->mSph.SetTgType(0x40000002);
            if ((i_this->field_0x698 & 0x20) != 0) {
                dComIfGp_att_LookRequest(a_this, 400.0f,300.0f, -300.0f, 0x6000, 1);
            }
            fopAcM_OffStatus(a_this, 0);
            a_this->attention_info.flags = 0;
        } else {
            i_this->mSph.SetTgType(0xd8fbfdff);
            fopAcM_OnStatus(a_this, 0);
            a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        }
    }
    
    i_this->mSph.SetC(local_64);
    i_this->mSph.SetR(25.0f * l_HIO.mBasicSize);
    dComIfG_Ccsp()->Set(&i_this->mSph);
    return 1;
}

static int daE_NZ_IsDelete(e_nz_class* i_this) {
    return 1;
}

static int daE_NZ_Delete(e_nz_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, "E_NZ");
    if (i_this->mIsHIOOwner) {
        l_HIOInit = 0;
        mDoHIO_DELETE_CHILD(l_HIO.mId);
    }

    if (a_this->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }

    if (i_this->field_0x6ac != 0) {
        data_8072C454[0] &= ~stick_bit[i_this->field_0x6ac - 1];
        i_this->field_0x6ac = 0;
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    e_nz_class* i_this = (e_nz_class*)a_this;
    i_this->mpMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_NZ", 13), NULL,
                                          NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_NZ", 10),
                                          2, 1.0f, 0, -1, &i_this->mSound, 0x80000, 0x11000084);
    if (i_this->mpMorf == NULL || i_this->mpMorf->getModel() == NULL) {
        return 0;
    }

    MtxScale(0.0f, 0.0f, 0.0f, 0);
    i_this->mpMorf->getModel()->setBaseTRMtx(*calc_mtx);
    if (i_this->mInvModel.create(i_this->mpMorf->getModel(), 1) == 0) {
        return 0;
    }
    return 1;
}

static int daE_NZ_Create(fopAc_ac_c* a_this) {
    e_nz_class* i_this = (e_nz_class*)a_this;
    fopAcM_ct(a_this, e_nz_class);
    int phase = dComIfG_resLoad(&i_this->mPhase, "E_NZ");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_NZ PARAM %x\n", fopAcM_GetParam(a_this));
        i_this->field_0x5b6 = fopAcM_GetParam(a_this) & 0xff;
        i_this->field_0x5b7 = (fopAcM_GetParam(a_this) & 0xff00) >> 8;
        i_this->field_0x5b8 = (fopAcM_GetParam(a_this) & 0xff000000) >> 24;
        OS_REPORT("E_NZ//////////////E_NZ SET 1 !!\n");

        if (fopAcM_entrySolidHeap(a_this, useHeapInit, 0x17e0) == 0) {
            OS_REPORT("//////////////E_NZ SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (!l_HIOInit) {
            i_this->mIsHIOOwner = 1;
            l_HIOInit = true;
            // Ghost Rat
            l_HIO.mId = mDoHIO_CREATE_CHILD("幽霊ネズミ", &l_HIO);
        }
        fopAcM_SetMtx(a_this, i_this->mpMorf->getModel()->getBaseTRMtx());
        a_this->health = 10;
        a_this->field_0x560 = 10;
        i_this->mStts.Init(100, 0, a_this);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x0}},  // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                // mGObjTg
                {0x0},                                             // mGObjCo
            },                                                     // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
            }  // mSphAttr
        };

        i_this->mSph.Set(cc_sph_src);
        i_this->mSph.SetStts(&i_this->mStts);
        i_this->mAcch.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1,
                            &i_this->mAcchCir, fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(30.0f, 30.0f);
        i_this->mSound.init(&a_this->current.pos, &a_this->eyePos, 3, 1);
        i_this->mSound.setEnemyName("E_nz");
        i_this->mAtInfo.mpSound = &i_this->mSound;
        i_this->mAtInfo.mPowerType = 1;
        i_this->field_0x698 = cM_rndF(65535.0f);
        daE_NZ_Execute(i_this);
        g_env_light.settingTevStruct(0, &(a_this->current).pos, &a_this->tevStr);
    }
    return phase;
}

AUDIO_INSTANCES

static actor_method_class l_daE_NZ_Method = {
    (process_method_func)daE_NZ_Create,
    (process_method_func)daE_NZ_Delete,
    (process_method_func)daE_NZ_Execute,
    (process_method_func)daE_NZ_IsDelete,
    (process_method_func)daE_NZ_Draw,
};

extern actor_process_profile_definition g_profile_E_NZ = {
  fpcLy_CURRENT_e,       // mLayerID
  7,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_E_NZ,             // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(e_nz_class),    // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  179,                   // mPriority
  &l_daE_NZ_Method,      // sub_method
  0x10044120,            // mStatus
  fopAc_ENEMY_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
