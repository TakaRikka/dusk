/**
 * @file d_a_e_kg.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_kg.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_enemy.h"

class daE_KG_HIO_c {
public:
    daE_KG_HIO_c();
    virtual ~daE_KG_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mSize;
    /* 0x0c */ f32 field_0xc;
    /* 0x10 */ f32 mCheckRange;
};

daE_KG_HIO_c::daE_KG_HIO_c() {
    field_0x4 = -1;
    mSize = 1.3f;
    field_0xc = 10.0f;
    mCheckRange = 600.0f;
}

static void anm_init(e_kg_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    J3DAnmTransform* anm = (J3DAnmTransform*) dComIfG_getObjectRes("E_kg", i_index);
    i_this->mpMorf->setAnm(anm, i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->mResIndex = i_index;
}

static int daE_KG_Draw(e_kg_class* i_this) {
    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);
    J3DModelData* model_data = model->getModelData();
    i_this->mpBtp->entry(model_data);
    i_this->mpMorf->entryDL();
    if (i_this->mShadowKey != 0x564FF) {
        cXyz my_vec_0;
        my_vec_0.set(i_this->current.pos.x, i_this->current.pos.y + 50.0f, i_this->current.pos.z);
        i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &my_vec_0, 700.0f,
                                                0.0f, i_this->current.pos.y, i_this->mAcch.GetGroundH(),
                                                i_this->mAcch.m_gnd, &i_this->tevStr, 0, 1.0f,
                                                dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static int other_bg_check(e_kg_class* i_this, fopAc_ac_c* i_player) {
    fopAc_ac_c* _this = (fopAc_ac_c*)i_this;
    dBgS_LinChk lin_chk;
    cXyz my_vec_0;
    cXyz kg_pos;
    cXyz o_pos;
    o_pos = i_player->current.pos;
    o_pos.y += 100.0f;
    kg_pos = _this->current.pos;
    kg_pos.y = _this->eyePos.y;
    lin_chk.Set(&kg_pos, &o_pos, _this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return 1;
    } else {
        return 0;
    }
}

static int pl_check(e_kg_class* i_this, f32 i_check_range, s16 i_s16) {
    fopAc_ac_c* i_player = dComIfGp_getPlayer(0);
    if (i_this->mPlayerDist < i_check_range) {
        s16 diff = i_this->shape_angle.y - i_this->mPlayerAngle;
        if (diff < i_s16 && diff > (s16)(-i_s16)) {
            if (other_bg_check(i_this, i_player) == 0) {
                return 1;
            }
        }
    }

    return 0;
}

static void damage_check(e_kg_class* i_this) {
    dComIfGp_getPlayer(0);
    if (i_this->field_0x69c == 0) {
        i_this->mStts.Move();
        if (i_this->mSph.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mSph.GetTgHitObj();
            cc_at_check(i_this, &i_this->mAtInfo);
            if (i_this->mAtInfo.mpCollider->ChkAtType(0xd8000000)) {
                i_this->field_0x69c = 20;
            } else {
                i_this->field_0x69c = 10;
            }

            if (i_this->mAtInfo.mpCollider->ChkAtType(0x2000000)) {
                i_this->health = 0;
            }

            if (i_this->mAtInfo.mHitType == 0x10) {
                i_this->mAction = 3;
            } else {
                if (i_this->health <= 0) {
                    i_this->mAction = 10;
                } else {
                    i_this->mAction = 9;
                }

                i_this->field_0xa54 = 15;
            }

            i_this->current.angle.y = i_this->mAtInfo.mHitDirection.y;
            i_this->field_0x678 = 0;
        }

        if (i_this->health <= 1) {
            i_this->mSph.SetTgHitMark(CcG_Tg_UNK_MARK_3);
        }
    }
}

static int way_bg_check(e_kg_class* i_this, f32 i_f32) {
    fopAc_ac_c* _this = (fopAc_ac_c*)i_this;
    dBgS_LinChk lin_chk;
    cXyz my_vec_0;
    cXyz this_pos;
    cXyz my_vec_2;
    this_pos = _this->current.pos;
    this_pos.y += 50.0f;
    cMtx_YrotS(*calc_mtx, _this->shape_angle.y);
    my_vec_0.x = 0.0f;
    my_vec_0.y = 50.0f;
    my_vec_0.z = i_f32;
    MtxPosition(&my_vec_0, &my_vec_2);
    my_vec_2 += _this->current.pos;
    lin_chk.Set(&this_pos, &my_vec_2, _this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return 1;
    } else {
        return 0;
    }
}

static u8 lbl_204_bss_8;

static daE_KG_HIO_c l_HIO;

static void e_kg_move(e_kg_class* i_this) {
    int frame = i_this->mpMorf->getFrame();
    f32 tgt_val = 0.0f;
    cXyz my_vec_0;
    cXyz my_vec_1;
    if ((i_this->field_0x674 & 0xF) == 0 && cM_rndF(1.0f) < 0.5f) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_KG_V_WAIT, -1);
    }

    switch (i_this->field_0x678) {
        case 0: {
            s16 my_s_val;
            if (i_this->field_0x694[0] == 0) {
                if (way_bg_check(i_this, 200.0f)) {
                    my_s_val = cM_rndFX(10000.0f) + 32768.0f;
                    i_this->field_0x694[1] = 40;
                } else {
                    my_vec_1.x = cM_rndFX(400.0f) + i_this->home.pos.x;
                    my_vec_1.y = i_this->home.pos.y;
                    my_vec_1.z = cM_rndFX(400.0f) + i_this->home.pos.z;
                    my_vec_0 = my_vec_1 - i_this->current.pos;
                    my_s_val = cM_atan2s(my_vec_0.x, my_vec_0.z) - i_this->current.angle.y;
                    if (my_s_val > 0x3000) {
                        my_s_val = 0x3000;
                    } else if (my_s_val < -0x3000) {
                        my_s_val = -0x3000;
                    }
                }

                i_this->field_0x688 = i_this->current.angle.y + my_s_val;
                anm_init(i_this, 0xF, 5.0f, 2, 1.0f);
                i_this->field_0x694[0] = cM_rndF(100.0f) + 50.0f;
                i_this->field_0x678 = 1;
            }

            break;
        }

        case 1: {
            if (frame <= 3 || ((frame >= 11 && frame <= 18) || (frame >= 26))) {
                tgt_val = l_HIO.field_0xc;
            }

            cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->field_0x688, 8, 0x400);

            if (i_this->field_0x694[0] == 0 || (i_this->field_0x694[1] == 0 && way_bg_check(i_this, 200.0f))) {
                i_this->field_0x678 = 0;
                i_this->field_0x694[0] = cM_rndF(100.0f) + 50.0f;
                anm_init(i_this, 0xE, 5.0f, 2, 1.0f);
            }

            break;
        }
    }

    cLib_addCalc2(&i_this->speedF, tgt_val, 1.0f, l_HIO.field_0xc * 0.5f);
    if (pl_check(i_this, l_HIO.mCheckRange, 0x6000)) {
        i_this->mAction = 1;
        i_this->field_0x678 = 0;
    }
}

static int e_kg_attack(e_kg_class* i_this) {
    int frame = i_this->mpMorf->getFrame();
    f32 next_speed = 0.0f;
    s16 angle_add = 0;
    cXyz my_vec_0;
    cXyz my_vec_1;
    int ret_val = 0;
    switch (i_this->field_0x678) {
        case 0: {
            anm_init(i_this, 0xF, 5.0f, 2, 2.0f);
            i_this->field_0x678 = 1;
            i_this->mSound.startCreatureVoice(Z2SE_EN_KG_V_FIND, -1);
            break;
        }

        case 1: {
            angle_add = 0x700;
            if (frame <= 3 || ((frame >= 11 && frame <= 18) || (frame >= 26))) {
                next_speed = l_HIO.field_0xc * 2.0f;
            }

            i_this->speedF = next_speed;
            if (pl_check(i_this, l_HIO.mCheckRange + 100.0f, 0x7000) == 0) {
                i_this->mAction = 0;
                i_this->field_0x678 = 0;
                i_this->field_0x694[0] = 0;
            } else if (i_this->mPlayerDist < 300.0f) {
                anm_init(i_this, 0x5, 5.0f, 0, 1.0f);
                i_this->field_0x678 = 2;
                i_this->field_0x66c = cM_rndF(50.0f) + 100.0f;
            }

            break;
        }

        case 2: {
            i_this->speedF = 0.0f;
            if (frame == 50) {
                i_this->mSound.startCreatureVoice(Z2SE_EN_KG_V_ATTACK, -1);
            }

            if (frame < 58) {
                angle_add = 0x400;
            }

            if (frame >= 63) {
                i_this->mpMorf->setPlaySpeed(0.0f);
                i_this->field_0x678 = 3;
                i_this->speed.y = 20.0f;
                i_this->speedF = 40.0f;
                i_this->mSound.startCreatureSound(Z2SE_EN_KG_ATTACK, 0, -1);
            }

            break;
        }

        case 3: {
            ret_val = 1;
            if (i_this->mAcch.ChkGroundHit()) {
                i_this->mpMorf->setPlaySpeed(1.0f);
                i_this->field_0x678 = 4;
            }

            if (i_this->mSph.ChkAtShieldHit()) {
                i_this->speedF = -10.0f;
            }

            break;
        }

        case 4: {
            cLib_addCalc0(&i_this->speedF, 1.0f, 15.0f);
            if (i_this->mpMorf->isStop()) {
                i_this->field_0x694[0] = cM_rndF(30.0f) + 30.0f;
                anm_init(i_this, 0xE, 5.0f, 2, 1.0f);
                i_this->field_0x678 = 5;
            }

            break;
        }

        case 5: {
            if (i_this->field_0x694[0] == 0) {
                i_this->mAction = 0;
                i_this->field_0x678 = 0;
            }

            break;
        }
    }

    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mPlayerAngle, 8, angle_add);
    return ret_val;
}

static void e_kg_back(e_kg_class* i_this) {
    switch (i_this->field_0x678) {
        case 0: {
            anm_init(i_this, 0x9, 3.0f, 2, 0.0f);
            i_this->field_0x678 = 1;
            i_this->speedF = cM_rndFX(5.0f) + -50.0f;
            break;
        }

        case 1: {
            if ((i_this->field_0x674 & 0x3) == 0) {
                fopAcM_effSmokeSet2(&i_this->mSmokeKey1, &i_this->mSmokeKey2, &i_this->current.pos, NULL,
                                    0.8f, &i_this->tevStr);
            }

            cLib_addCalc0(&i_this->speedF, 1.0f, 4.0f);
            if (i_this->mAcch.ChkWallHit()) {
                i_this->speedF *= 0.5f;
            }

            if (fabsf(i_this->speedF) < 0.1f) {
                i_this->mAction = 1;
                i_this->field_0x678 = 0;
                i_this->field_0x66c = 11;
            }

            break;
        }
    }
}

static void e_kg_s_damage(e_kg_class* i_this) {
    switch (i_this->field_0x678) {
        case 0: {
            if (cM_rndF(1.0f) < 0.5f) {
                anm_init(i_this, 0x7, 3.0f, 0, 1.0f);
            } else {
                anm_init(i_this, 0x8, 3.0f, 0, 1.0f);
            }

            i_this->field_0x678 = 1;
            i_this->speedF = cM_rndFX(5.0f) + -40.0f;
            i_this->mSound.startCreatureVoice(Z2SE_EN_KG_V_DAMAGE, -1);
            break;
        }

        case 1: {
            cLib_addCalc0(&i_this->speedF, 1.0f, 4.0f);
            if (i_this->mAcch.ChkWallHit()) {
                i_this->speedF *= 0.5f;
            }

            if (fabsf(i_this->speedF) < 0.1f) {
                i_this->mAction = 1;
                i_this->field_0x678 = 0;
                i_this->field_0x66c = 11;
            }

            break;
        }
    }
}

static void e_kg_damage(e_kg_class* i_this) {
    i_this->field_0x69c = 6;
    switch (i_this->field_0x678) {
        case 0: {
            anm_init(i_this, 0x6, 5.0f, 2, 1.0f);
            i_this->field_0x678 = 1;
            i_this->speed.y = cM_rndFX(5.0f) + 45.0f;
            i_this->speedF = cM_rndFX(5.0f) + -35.0f;
            i_this->field_0x69e = cM_rndFX(1700.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_KG_V_DEATH, -1);
            fopAcM_OffStatus(i_this, 0);
            i_this->attention_info.flags = 0;
            break;
        }

        case 1:
        case 2: {
            if (i_this->mAcch.ChkGroundHit()) {
                if (i_this->field_0x678 == 1) {
                    i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
                }

                ++i_this->field_0x678;
                if (i_this->field_0x678 == 3) {
                    i_this->field_0x664 = 1.0f;
                    i_this->field_0x694[0] = 30;
                } else {
                    i_this->speed.y = 15.0f;
                    i_this->speedF *= 0.75f;
                    fopAcM_effSmokeSet1(&i_this->mSmokeKey1, &i_this->mSmokeKey2, &i_this->current.pos, NULL,
                                        1.1f, &i_this->tevStr, 1);
                }
            }

            break;
        }

        case 3: {
            i_this->mpMorf->setPlaySpeed(i_this->field_0x664);
            cLib_addCalc0(&i_this->field_0x664, 1.0f, 0.015f);
            cLib_addCalc0(&i_this->speedF, 1.0f, 3.0f);
            if (i_this->field_0x694[0] == 0) {
                fopAcM_createDisappear(i_this, &i_this->current.pos, 8, 0, 0x2D);
                int param_val = fopAcM_GetParam(i_this) >> 0x18;
                if (param_val != 0xff) {
                    dComIfGs_onSwitch(param_val, fopAcM_GetRoomNo(i_this));
                }

                fopAcM_delete(i_this);
            }
        }
    }

    i_this->shape_angle.y += i_this->field_0x69e;
    cLib_addCalcAngleS2(&i_this->field_0x69e, 0, 1, 0x20);
    cLib_addCalcAngleS2(&i_this->shape_angle.x, 0x7FFF, 1, 0x800);
    if (i_this->mAcch.ChkWallHit()) {
        i_this->speedF *= 0.7f;
    }
}

static void e_kg_roof(e_kg_class* i_this) {
    if ((i_this->field_0x674 & 0xF) == 0 && cM_rndF(1.0f) < 0.5f) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_KG_V_WAIT, -1);
    }

    switch (i_this->field_0x678) {
        case 0: {
            anm_init(i_this, 0xE, 5.0f, 2, 1.0f);
            i_this->field_0x678 = 1;
            i_this->shape_angle.x = -0x8000;
            i_this->mShadowKey = 0x564FF;
        }
        // fallthrough intentional
        case 1: {
            if (daPy_getPlayerActorClass()->checkFrontRollCrash()) {
                i_this->field_0x678 = 2;
            }

            i_this->field_0x694[0] = cM_rndFX(25.0f) + 50.0f;
        }
        // fallthrough intentional
        case 2: {
            i_this->current.pos = i_this->home.pos;
            i_this->speed.y = 0.0f;
            cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mPlayerAngle + 0x8000, 8, 0x400);
            if (i_this->field_0x694[0] == 0) {
                i_this->field_0x678 = 3;
                i_this->mShadowKey = 0;
                i_this->mSound.startCreatureSound(Z2SE_EN_KG_FALL, 0, -1);
            }

            break;
        }

        case 3: {
            cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mPlayerAngle, 1, 0x800);
            cLib_addCalcAngleS2(&i_this->shape_angle.x, 0, 1, 0x800);
            if (i_this->mAcch.ChkGroundHit()) {
                i_this->shape_angle.x = 0;
                i_this->mAction = 1;
                i_this->field_0x678 = 0;
            }
        }
    }
}

static void action(e_kg_class* i_this) {
    fopAc_ac_c* actor_this = i_this;
    cXyz my_vec_0;
    cXyz my_vec_1;
    i_this->mPlayerAngle = fopAcM_searchPlayerAngleY(actor_this);
    i_this->mPlayerDist = fopAcM_searchPlayerDistance(actor_this);
    damage_check(i_this);
    i_this->mSph.OffAtVsPlayerBit();
    s16 max_step = 0;
    s8 new_0x670 = 0;
    i_this->mSph.OnCoSetBit();
    switch (i_this->mAction) {
        case 0: {
            e_kg_move(i_this);
            max_step = 0x2000;
            break;
        }

        case 1: {
            if (e_kg_attack(i_this)) {
                i_this->mSph.OnAtVsPlayerBit();
            }
            max_step = 0x2000;
            break;
        }

        case 5: {
            e_kg_roof(i_this);
            max_step = 0x2000;
            break;
        }

        case 3: {
            e_kg_back(i_this);
            new_0x670 = 5;
            break;
        }

        case 9: {
            e_kg_s_damage(i_this);
            new_0x670 = 5;
            break;
        }

        case 10: {
            e_kg_damage(i_this);
            new_0x670 = 5;
        }

        case 4:
        case 6:
        case 7:
        case 8:
        case 2: {
            break;
        }
    }

    cMtx_YrotS(*calc_mtx, actor_this->current.angle.y);
    my_vec_0.x = 0.0;
    my_vec_0.y = 0.0;
    my_vec_0.z = actor_this->speedF;
    MtxPosition(&my_vec_0, &my_vec_1);
    actor_this->speed.x = my_vec_1.x;
    actor_this->speed.z = my_vec_1.z;
    actor_this->current.pos += actor_this->speed;
    actor_this->speed.y -= 5.0f;
    cXyz* cc_move_p = i_this->mStts.GetCCMoveP();
    if (cc_move_p != NULL) {
        actor_this->current.pos.x += cc_move_p->x;
        actor_this->current.pos.y += cc_move_p->y;
        actor_this->current.pos.z += cc_move_p->z;
    }

    f32 pos_adjust = 0.0f;
    if (actor_this->shape_angle.x) {
        pos_adjust = cM_ssin(actor_this->shape_angle.x / 2) * 30.0f;
    }

    actor_this->current.pos.y -= pos_adjust;
    actor_this->old.pos.y -= pos_adjust;
    i_this->mAcch.CrrPos(dComIfG_Bgsp());
    actor_this->current.pos.y += pos_adjust;
    actor_this->old.pos.y += pos_adjust;
    if (new_0x670) {
        i_this->field_0x670 = new_0x670;
    } else if (i_this->field_0x66c == 0) {
        i_this->field_0x66c = cM_rndF(100.0f) + 30.0f;
    } else {
        --i_this->field_0x66c;
        if (i_this->field_0x66c <= 11) {
            i_this->field_0x670 = 11 - i_this->field_0x66c;
        }
    }

    cLib_addCalcAngleS2(&i_this->shape_angle.y, i_this->current.angle.y, 4, max_step);
    cXyz eff_size(0.7f, 0.7f, 0.7f);
    setMidnaBindEffect(i_this, &i_this->mSound, &i_this->eyePos, &eff_size);
}

static void anm_se_set(e_kg_class* i_this) {
    if (i_this->mResIndex == 0xF && (i_this->mpMorf->checkFrame(3.0f) || i_this->mpMorf->checkFrame(9.0f)
                                  || i_this->mpMorf->checkFrame(16.0f) || i_this->mpMorf->checkFrame(23.0f))) {
        i_this->mSound.startCreatureSound(Z2SE_EN_KG_FOOTNOTE, 0, -1);
    }
}

static int daE_KG_Execute(e_kg_class* i_this) {
    cXyz my_vec_0;
    cXyz my_vec_1;
    ++i_this->field_0x674;
    for (int idx = 0; idx < 4; ++idx) {
        if (i_this->field_0x694[idx] != 0) {
            --i_this->field_0x694[idx];
        }
    }

    if (i_this->field_0x69c != 0) {
        --i_this->field_0x69c;
    }

    action(i_this);
    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::scaleM(l_HIO.mSize, l_HIO.mSize, l_HIO.mSize);
    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));
    anm_se_set(i_this);
    i_this->mpBtp->setFrame(i_this->field_0x670);
    i_this->mpMorf->modelCalc();
    MTXCopy(model->getAnmMtx(1), *calc_mtx);
    my_vec_0.set(20.0f, 0.0f, 0.0f);
    MtxPosition(&my_vec_0, &i_this->eyePos);
    i_this->attention_info.position = i_this->eyePos;
    i_this->attention_info.position.y += 30.0f;
    i_this->setDownPos(&i_this->eyePos);
    my_vec_0.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&my_vec_0, &my_vec_1);
    if (i_this->field_0x69c != 0) {
        my_vec_1.x += 10000.0f;
    }

    i_this->mSph.SetC(my_vec_1);
    i_this->mSph.SetR(l_HIO.mSize * 50.0f);
    dComIfG_Ccsp()->Set(&i_this->mSph);
    if (i_this->field_0xa54 != 0) {
        --i_this->field_0xa54;
        MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(0x10), *calc_mtx);
        cXyz my_vec_2;
        cXyz my_vec_3;
        my_vec_2.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&my_vec_2, &my_vec_3);
        cXyz my_vec_4(l_HIO.mSize, l_HIO.mSize, l_HIO.mSize);
        i_this->field_0xa58 = dComIfGp_particle_set(i_this->field_0xa58, 0x81C7, &my_vec_3, &i_this->shape_angle, &my_vec_4);
    }

    return 1;
}

static int daE_KG_IsDelete(e_kg_class* i_this) {
    return 1;
}

static int daE_KG_Delete(e_kg_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "E_kg");
    if (i_this->field_0xa70) {
        lbl_204_bss_8 = 0;
    }

    if (i_this->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_kg_class* a_this = (e_kg_class*) i_this;
    a_this->mpMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_kg", 0x12), NULL, NULL,
                                          (J3DAnmTransform*)dComIfG_getObjectRes("E_kg", 0xE), 2, 1.0f, 0,
                                          -1, &a_this->mSound, 0x80000, 0x11020084);
    if (a_this->mpMorf == NULL || a_this->mpMorf->getModel() == NULL) {
        return 0;
    }

    a_this->mpBtp = new mDoExt_btpAnm();
    if (a_this->mpBtp == NULL) {
        return cPhs_ERROR_e;
    }

    J3DAnmTexPattern* btk = (J3DAnmTexPattern*) dComIfG_getObjectRes("E_kg", 0x15);
    return (!a_this->mpBtp->init(a_this->mpMorf->getModel()->getModelData(), btk, TRUE, 0, 1.0f, 0, -1)) ? cPhs_ERROR_e : 1;
}

static int daE_KG_Create(fopAc_ac_c* i_this) {
    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xd}, {0xdafbfdff, 0x3}, 0x75}}, // mObj
            {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };

    fopAcM_ct(i_this, e_kg_class);
    e_kg_class* a_this = (e_kg_class*) i_this;
    int phase = dComIfG_resLoad(&a_this->mPhase, "E_kg");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_kg PARAM %x\n", fopAcM_GetParam(i_this));
        a_this->field_0x5b4 = fopAcM_GetParam(i_this);
        a_this->field_0x5b5 = (fopAcM_GetParam(i_this) >> 8) & 0xf;
        int param_val = fopAcM_GetParam(i_this) >> 0x18;
        if (param_val != 0xff) {
            if (dComIfGs_isSwitch(param_val, fopAcM_GetRoomNo(i_this))) {
                return cPhs_ERROR_e;
            }
        }

        OS_REPORT("E_kg//////////////E_KG SET 1 !!\n");
        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x1fe0)) {
            OS_REPORT("//////////////E_KG SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (lbl_204_bss_8 == 0) {
            a_this->field_0xa70 = 1;
            lbl_204_bss_8 = 1;
            l_HIO.field_0x4 = -1;
        }

        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(i_this, a_this->mpMorf->getModel()->getBaseTRMtx());
        i_this->health = 0x50;
        i_this->field_0x560 = 0x50;
        a_this->mStts.Init(100, 0, i_this);
        a_this->mSph.Set(cc_sph_src);
        a_this->mSph.SetStts(&a_this->mStts);
        a_this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this,
                          1, &a_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->mAcchCir.SetWall(50.0f, 50.0f);
        a_this->mSound.init(&a_this->current.pos, &a_this->eyePos, 3, 1);
        a_this->mAtInfo.mPowerType = 1;
        a_this->mAtInfo.mpSound = &a_this->mSound;
        a_this->mSound.setEnemyName("E_kg");
        if (a_this->field_0x5b4 == 1) {
            a_this->mAction = 5;
        }

        a_this->field_0x674 = cM_rndF(65535.0f);
        daE_KG_Execute(a_this);
    }
    return phase;
}

static actor_method_class l_daE_KG_Method = {
    (process_method_func)daE_KG_Create,
    (process_method_func)daE_KG_Delete,
    (process_method_func)daE_KG_Execute,
    (process_method_func)daE_KG_IsDelete,
    (process_method_func)daE_KG_Draw,
};

extern actor_process_profile_definition g_profile_E_KG = {
  fpcLy_CURRENT_e,       // mLayerID
  7,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_E_KG,             // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(e_kg_class),    // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  185,                   // mPriority
  &l_daE_KG_Method,      // sub_method
  0x100C0120,            // mStatus
  fopAc_ENEMY_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
