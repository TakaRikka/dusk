//
// Translation Unit: d_a_ep
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_ep.h"
#include "d/actor/d_a_player.h"
#include "d/d_a_obj.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "m_Do/m_Do_lib.h"

static void hahen_draw(ep_class* i_this) {
    fopAc_ac_c* a_this = i_this;
    ep_hahen_s* epHahenS = i_this->mHahen;
    g_env_light.settingTevStruct(16, &a_this->current.pos, &a_this->tevStr);

    for (int i = 0; i < 6; i++, epHahenS++) {
        if (epHahenS->field_0x97 != 0) {
            g_env_light.setLightTevColorType_MAJI(epHahenS->mpModel, &a_this->tevStr);
            mDoExt_modelUpdateDL(epHahenS->mpModel);
            
            if (epHahenS->field_0x96 < 2) {
                cXyz local_28;
                local_28.set(epHahenS->field_0x4.x, epHahenS->field_0x4.y + 50.0f + BREG_F(18), epHahenS->field_0x4.z);
                epHahenS->mShadow = dComIfGd_setShadow(epHahenS->mShadow, 1, epHahenS->mpModel,
                    &local_28, 200.0f + BREG_F(19), 0.0f, epHahenS->field_0x4.y, epHahenS->mGroundY, 
                    epHahenS->mObjGndCheck, &a_this->tevStr, 0, 1.0f, 
                    dDlst_shadowControl_c::getSimpleTex());
            }
        }
    }
}

static int move_calc(ep_class* i_this, ep_hahen_s* hahen_s) {
    fopAc_ac_c* a_this = i_this;
    static u16 w_eff_id[4] = {
        0x01B8, 0x01B9, 0x01BA, 0x01BB,
    };

    dBgS_LinChk dStack_bc;
    int rv = 0;
    cXyz local_11c;

    hahen_s->field_0x10 = hahen_s->field_0x4;
    hahen_s->field_0x4 += hahen_s->field_0x1c;

    hahen_s->field_0x1c.y -= 5.0f;
    if (hahen_s->field_0x1c.y < -90.0f) {
        hahen_s->field_0x1c.y = -90.0f;
    }

    local_11c.set(hahen_s->field_0x4.x, hahen_s->field_0x4.y + 100.0f, hahen_s->field_0x4.z);
    f32 fVar11 = 0.0f;
    fVar11 = fabsf((35.0f+AREG_F(4)) * cM_ssin(hahen_s->field_0x28.x)) + 6.0f;
    s16 sVar9 = 1000 + TREG_S(6);
    if (fopAcM_gc_c::gndCheck(&local_11c)) {
        hahen_s->mGroundY = fopAcM_gc_c::getGroundY();
        hahen_s->mObjGndCheck = *fopAcM_gc_c::getGroundCheck();

        if (hahen_s->field_0x1c.y <= 0.0f && hahen_s->field_0x4.y <= hahen_s->mGroundY + fVar11) {
            hahen_s->field_0x4.y = hahen_s->mGroundY + fVar11;
            rv = 1;

            if (hahen_s->field_0x1c.y < -20.0f && hahen_s->field_0x96 < 3) {
                hahen_s->field_0x1c.y *= -0.3f;
                if (hahen_s->field_0x96 == 0) {
                    hahen_s->field_0x28.x = cM_rndFX(17000.0f);
                } else {
                    hahen_s->field_0x28.x = cM_rndFX(10000.0f);
                }

                hahen_s->field_0x90 = 0;
                s16* mid_val = &hahen_s->field_0x92;
                s16 unused_s16 = *mid_val /= 2;
                (void) unused_s16;
                ++hahen_s->field_0x96;
                fopAcM_effSmokeSet2(&i_this->field_0xa60, &i_this->field_0xa64, &hahen_s->field_0x4,
                                    NULL, 0.5f, &a_this->tevStr);
                
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_WOODSTICK_BOUND, &hahen_s->field_0x4, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            } else {
                hahen_s->field_0x96 = 3;
                hahen_s->field_0x1c.y = -10.0f;
                sVar9 = 5000 + TREG_S(7);
                hahen_s->field_0x90 = 0;
                hahen_s->field_0x92 = 0;
            }

            hahen_s->field_0x1c.x *= 0.5f;
            hahen_s->field_0x1c.z *= 0.5f;
        }

        if (hahen_s->field_0x97 == 1) {
            hahen_s->field_0x28.y += hahen_s->field_0x92;
            if (hahen_s->field_0x90 != 0) {
                hahen_s->field_0x28.z += hahen_s->field_0x90;
            } else {
                cLib_addCalcAngleS2(&hahen_s->field_0x28.z, 0, 1, sVar9);
            }
            cLib_addCalcAngleS2(&hahen_s->field_0x28.x, 0, 1, 0x800);
        }
    }

    if (hahen_s->field_0x98 == 0) {
        cXyz local_158 = hahen_s->field_0x4 - hahen_s->field_0x10;
        local_158.y = 0.0f;
        if (local_158.abs() > 0.01f) {
            cXyz local_170 = hahen_s->field_0x4 + (local_158 * 1.1f);
            dStack_bc.Set(&hahen_s->field_0x10, &local_170, a_this);
            if (dComIfG_Bgsp().LineCross(&dStack_bc)) {
                hahen_s->field_0x4 = hahen_s->field_0x10;
                hahen_s->field_0x1c.x *= -0.5f;
                hahen_s->field_0x1c.z *= -0.5f;
                hahen_s->field_0x1c.y = 0.0f;
                hahen_s->field_0x4 += hahen_s->field_0x1c;
                hahen_s->field_0x98 = 5;
                rv = 2;

                Z2GetAudioMgr()->seStart(Z2SE_OBJ_WOODSTICK_BOUND, &hahen_s->field_0x4, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
        }
    }

    dBgS_ObjGndChk_Spl cStack_110;
    cXyz local_140(hahen_s->field_0x4);
    local_140.y += 100.0f;
    cStack_110.SetPos(&local_140);
    hahen_s->field_0x34 = dComIfG_Bgsp().GroundCross(&cStack_110);

    if (hahen_s->field_0x4.y <= hahen_s->field_0x34) {
        hahen_s->field_0x97 = 2;
        hahen_s->field_0x4.y = hahen_s->field_0x34;
        hahen_s->field_0x1c.x *= 0.5f;
        hahen_s->field_0x1c.z *= 0.5f;
        hahen_s->field_0x94 = cM_rndF(65536.0f);
        rv = 3;

        f32 my_float = 0.7f;
        cXyz local_14c(my_float, my_float, my_float);
        for (int i = 0; i < 4; i++) {
            i_this->field_0xa68[i] = dComIfGp_particle_set(i_this->field_0xa68[i], w_eff_id[i], &hahen_s->field_0x4,
                                                           &a_this->tevStr, NULL, &local_14c, 0xFF, NULL, -1, 0, 0,
                                                           NULL);
        }

        mDoAud_seStart(Z2SE_CM_BODYFALL_WATER_S, &hahen_s->field_0x4, 0, 0);
    }

    return rv;
}

static void hahen_normal(ep_class* i_this, ep_hahen_s* hahen_s) {
    move_calc(i_this, hahen_s);
}

static void hahen_water(ep_class* i_this, ep_hahen_s* hahen_s) {
    fopAc_ac_c* a_this = i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz local_98, unused_xyz_1;
    dBgS_LinChk dStack_8c;
    hahen_s->field_0x94++;
    if (hahen_s->field_0x98) {
        hahen_s->field_0x98--;
    }

    f32 x = hahen_s->field_0x4.x - player->current.pos.x;
    f32 z = hahen_s->field_0x4.z - player->current.pos.z;
    hahen_s->field_0x10 = hahen_s->field_0x4;
    if (JMAFastSqrt(x * x + z * z) < 60.0f+AREG_F(16)) {
        s16 sVar1 = cM_atan2s(x, z);
        cLib_addCalcAngleS2(&hahen_s->field_0x28.y, sVar1, 0x10, 200);
        cMtx_YrotS(*calc_mtx, sVar1);

        local_98.x = 0.0f;
        local_98.y = 0.0f;
        local_98.z = 3.0f + AREG_F(17);
        MtxPosition(&local_98, &hahen_s->field_0x1c);
    }

    hahen_s->field_0x4 += hahen_s->field_0x1c;
    hahen_s->field_0x1c.y = 0.0f;
    hahen_s->field_0x4.y = hahen_s->field_0x34;
    hahen_s->field_0x1c.x *= 0.95f;
    hahen_s->field_0x1c.z *= 0.95f;
    hahen_s->field_0x28.y += hahen_s->field_0x92;
    cLib_addCalcAngleS2(&hahen_s->field_0x92, 0, 1, 0x28);

    cLib_addCalcAngleS2(&hahen_s->field_0x28.z, (200.0f * cM_ssin(hahen_s->field_0x94 * 0x708)), 1, 0x800);
    cLib_addCalcAngleS2(&hahen_s->field_0x28.x, 0, 4, 0x1000);

    if (hahen_s->field_0x98 == 0) {
        cXyz local_bc = hahen_s->field_0x4 - hahen_s->field_0x10;
        local_bc.y = 0.0f;

        if (local_bc.abs() > 0.01f) {
            cXyz local_d4 = hahen_s->field_0x4 + local_bc * 2.0f;
            dStack_8c.Set(&hahen_s->field_0x10, &local_d4, a_this);

            if (dComIfG_Bgsp().LineCross(&dStack_8c)) {
                hahen_s->field_0x4 = hahen_s->field_0x10;
                hahen_s->field_0x1c.x *= -1.0f;
                hahen_s->field_0x1c.z *= -1.0f;
                hahen_s->field_0x98 = 5;
            }
        }
    }

    fopAcM_effHamonSet(&hahen_s->field_0xa0, &hahen_s->field_0x4, 1.0f, 0.05f);
}

static void hahen_carry(ep_class* i_this, ep_hahen_s* hahen_s) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (!fopAcM_checkCarryNow(i_this)) {
        if (fopAcM_GetSpeedF(i_this) > 0.0f) {
            hahen_s->field_0x97 = 4;
            hahen_s->field_0x96 = 0;
            hahen_s->field_0x28.x = 0;
            hahen_s->field_0x28.z = 0x4000;
            hahen_s->field_0x28.y = player->shape_angle.y;
            if (dComIfGp_getAttention()->Lockon()) {
                hahen_s->field_0x30 = 80.0f;

                fopAc_ac_c* lockOnTarget = dComIfGp_getAttention()->LockonTarget(0);
                if (lockOnTarget) {
                    cXyz local_34, cStack_28;
                    local_34 = lockOnTarget->current.pos - hahen_s->field_0x4;
                    hahen_s->field_0x28.y = cM_atan2s(local_34.x, local_34.z);

                    f32 fVar1 = JMAFastSqrt(local_34.x * local_34.x + local_34.z * local_34.z);
                    s16 sVar1 = -cM_atan2s(local_34.y, fVar1);
                    cMtx_XrotS(*calc_mtx, -0x800 + sVar1);

                    local_34.y = 0.0f;
                    local_34.x = 0.0f;
                    local_34.z = hahen_s->field_0x30;
                    MtxPosition(&local_34, &cStack_28);
                    hahen_s->field_0x1c.y = cStack_28.y;
                } else {
                    hahen_s->field_0x1c.y = 10.0f;
                }
            } else {
                hahen_s->field_0x30 = 70.0f;
                hahen_s->field_0x1c.y = 30.0f;
            }
        } else {
            hahen_s->field_0x97 = 1;
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_WOODSTICK_PUT, &hahen_s->field_0x4, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    }
}

static void hahen_cast(ep_class* i_this, ep_hahen_s* hahen_s) {
    cXyz local_1c, local_28;

    hahen_s->field_0x28.x += 0x1450;

    cMtx_YrotS(*calc_mtx, hahen_s->field_0x28.y);
    local_1c.x = 0.0f;
    local_1c.y = 0.0f;
    local_1c.z = hahen_s->field_0x30;
    MtxPosition(&local_1c, &local_28);
    hahen_s->field_0x1c.x = local_28.x;
    hahen_s->field_0x1c.z = local_28.z;

    if (move_calc(i_this, hahen_s)) {
        hahen_s->field_0x97 = 1;
    }

    i_this->mSph2.SetC(hahen_s->field_0x4);
    dComIfG_Ccsp()->Set(&i_this->mSph2);
}

static void hahen_move(ep_class* i_this) {
    fopAc_ac_c* a_this = i_this;
    ep_hahen_s* epHahenS;
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));
    dBgS_LinChk dStack_cc;

    if (!fopAcM_checkCarryNow(a_this)) {
        f32 fVar1 = 50.0f;
        s8 bVar2 = false;
        cXyz local_d8;
        for (int iters = 0; iters < 10; iters++) {
            epHahenS = static_cast<ep_hahen_s*>(i_this->mHahen);
            for (int j = 0; j < 6; j++, epHahenS++) {
                local_d8 = player->current.pos - epHahenS->field_0x4;
                if (local_d8.abs() < fVar1) {
                    cLib_onBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
                    a_this->current.pos = epHahenS->field_0x4;
                    a_this->attention_info.position = a_this->eyePos = a_this->current.pos;
                    a_this->current.angle = a_this->shape_angle = epHahenS->field_0x28;
                    i_this->field_0xa78 = j;
                    bVar2 = true;
                    break;
                }
            }

            if (bVar2) {
                break;
            }

            // if (bVar2) break;
            fVar1 += 20.0f;
        }
    } else {
        cLib_offBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
        i_this->mHahen[i_this->field_0xa78].field_0x97 =  3;
        i_this->mHahen[i_this->field_0xa78].field_0x4 = a_this->current.pos;
        i_this->mHahen[i_this->field_0xa78].field_0x28 = a_this->shape_angle;
    }

    epHahenS = i_this->mHahen;
    for (int i = 0; i < 6; i++, epHahenS++) {
        if (epHahenS->field_0x97) {
            if (epHahenS->field_0x98) {
                --epHahenS->field_0x98;
            }

            if (i_this->field_0xa79 == 3) {
                if (epHahenS->field_0x97 == 1) {
                    hahen_normal(i_this, epHahenS);
                } else if (epHahenS->field_0x97 == 2) {
                    hahen_water(i_this, epHahenS);
                } else if (epHahenS->field_0x97 == 3) {
                    hahen_carry(i_this, epHahenS);
                } else if (epHahenS->field_0x97 == 4) {
                    hahen_cast(i_this, epHahenS);
                }
            }

            MtxTrans(epHahenS->field_0x4.x, epHahenS->field_0x4.y, epHahenS->field_0x4.z, 0);
            cMtx_YrotM(*calc_mtx, epHahenS->field_0x28.y);
            cMtx_XrotM(*calc_mtx, epHahenS->field_0x28.x);
            cMtx_ZrotM(*calc_mtx, epHahenS->field_0x28.z);
            cMtx_YrotM(*calc_mtx, 0x4000);
            MtxTrans(3.0f + KREG_F(0), KREG_F(1), KREG_F(2), 1);
            cMtx_XrotM(*calc_mtx, 0x4000);
            MtxScale(1.0f, 0.5, 1.0f, 1);
            epHahenS->mpModel->setBaseTRMtx(*calc_mtx);
        }
    }

    if (i_this->field_0xa79 == 2) {
        i_this->field_0xa79 = 3;
    }
}

static int daEp_Draw(ep_class* i_this) {
    if (i_this->mpModel) {
        if (i_this->field_0x602 != 1) {
            g_env_light.settingTevStruct(16, &i_this->current.pos, &i_this->tevStr);
            g_env_light.setLightTevColorType_MAJI(i_this->mpModel->mModelData, &i_this->tevStr);
            mDoExt_modelUpdateDL(i_this->mpModel);
        }
    }

    if (i_this->field_0xa79 != 0) {
        hahen_draw(i_this);
    }

    return 1;
}

static BOOL ep_switch_event_end(ep_class* i_this) {
    int rv = FALSE;
    if (dComIfGp_evmng_endCheck("SHOKUDAI_SWITCH") != 0) {
        dComIfGp_event_reset();
        rv = TRUE;
    }
    return rv;
}

static BOOL ep_switch_event_begin(ep_class* i_this) {
    BOOL rv = 0;

    if (!i_this->eventInfo.checkCommandDemoAccrpt()) {
        fopAcM_orderOtherEvent(i_this, "SHOKUDAI_SWITCH", 0xffff, 1, 0);
    } else {
        i_this->eventInfo.onCondition(2);
        rv = dComIfGp_evmng_getMyStaffId("SHOKUDAI", 0, 0);
        if (rv == -1) {
           ep_switch_event_end(i_this);
        }
    }

    return rv;
}

static int ep_switch_event_move(ep_class* i_this) {
    static char* actions[2] = {
        "WAIT",
        "FIRE",
    };

    fopAc_ac_c* a_this = i_this;
    (void) a_this;
    int rv = 0;

    if (dComIfGp_evmng_getIsAddvance(i_this->field_0xa5c)) {
        rv = dComIfGp_evmng_getMyActIdx(i_this->field_0xa5c, actions, 2, 0, 0);
        switch (rv) {
            case 1: {
                if (i_this->field_0xa5b == 0) {
                    mDoAud_seStart(Z2SE_OBJ_FIRE_IGNITION, &i_this->field_0x634, 0, 0);
                }
                break;
            }

            case 0:
            default:
                rv = 0;
                dComIfGp_evmng_cutEnd(i_this->field_0xa5c);
        }
    }

    return rv;
}

static void ep_move(ep_class* i_this) {
    static u16 l_particle_fire_A[2] = {
        0x0100, 0x8110,
    };
    static u16 l_particle_fire_B[2] = {
        0x0101, 0x8111
    };
    static u16 l_particle_kagerou[2] = {
        0x0103, 0x8112
    };

    fopAc_ac_c* a_this = i_this;
    fopAc_ac_c* a_player = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*) a_player;
    cXyz sp1C(i_this->field_0x634.x,
              i_this->field_0x634.y + -240.0f + 235.0f + 15.0f + YREG_F(0),
              i_this->field_0x634.z
    );

    switch (i_this->field_0x5a4) {
        case 0: {
            if (i_this->field_0x60d != 0) {
                cLib_addCalc0(&i_this->field_0x5b0, 0.5f, 0.05f);
                if (i_this->mSph1.ChkTgHit()) {
                    cCcD_Obj* tg_hit_obj = i_this->mSph1.GetTgHitObj();
                    if (tg_hit_obj == NULL || !tg_hit_obj->ChkAtType(0)) {
                        if (i_this->mSph1.GetTgHitGObj() && i_this->mSph1.GetTgHitGObj()->GetAtMtrl() == 1) {
                            if (i_this->field_0x609 != 0xff) {
                                i_this->field_0x5ce = i_this->field_0x609 * 10 + 1;
                            }

                            dComIfGs_onSwitch(i_this->field_0x60d - 1, fopAcM_GetRoomNo(a_this));
                        }
                    }
                }

                if (dComIfGs_isSwitch(i_this->field_0x60d - 1, fopAcM_GetRoomNo(a_this))) {
                    if (i_this->field_0x60b != 0xff && dComIfGp_evmng_existence("SHOKUDAI_SWITCH")) {
                        if ( (i_this->field_0xa5c = ep_switch_event_begin(i_this)) ) {
                            i_this->field_0x60d = 0;
                            if (i_this->field_0xa5c != -1) {
                                ep_switch_event_move(i_this);
                            }
                        }
                    } else {
                        if (i_this->field_0xa5b == 0) {
                            mDoAud_seStart(Z2SE_OBJ_FIRE_IGNITION, &i_this->field_0x634,
                                0, 0);
                        }
                        i_this->field_0x60d = 0;
                    }
                }
            } else {
                if (i_this->field_0x60b != 0xFF && i_this->field_0xa5c != -1) {
                    if (ep_switch_event_move(i_this)) {
                        i_this->field_0x5a4 = 3;
                        i_this->field_0x5b4 = a_this->scale.x;
                    }
                } else {
                    i_this->field_0x5a4 = 3;
                    i_this->field_0x5b4 = a_this->scale.x;
                }
            }

            break;
        }

        case 3:
            i_this->field_0x5a4++;
        case 4:
            cLib_addCalc2(&i_this->field_0x5b0, i_this->field_0x5b4, 0.5f, 0.2f);
            if (i_this->field_0xa79 == 0) {
                if ( i_this->field_0xa50 < (s16)(7 + TREG_S(3)) ) {
                    dComIfGp_particle_setSimple(l_particle_fire_A[i_this->field_0x60c],
                        &sp1C, 0xff, g_whiteColor, g_whiteColor, 
                        0, 0.0f);
                    dComIfGp_particle_setSimple(l_particle_fire_B[i_this->field_0x60c],
                        &sp1C, 0xff, g_whiteColor, g_whiteColor, 
                        0, 0.0f);

                    if (i_this->field_0xa50 == 0 && i_this->mSph1.ChkTgHit()) {
                        cCcD_Obj* tg_hit_obj = i_this->mSph1.GetTgHitObj();
                        fopAc_ac_c* pfVar4 = dCc_GetAc(tg_hit_obj->GetAc());
                        f32 x_diff = a_this->current.pos.x - pfVar4->current.pos.x;
                        f32 z_diff = a_this->current.pos.z - pfVar4->current.pos.z;
                        i_this->field_0xa58 = cM_atan2s(x_diff, z_diff);
                        i_this->field_0xa50 = 0x28;
                    }
                }

                dComIfGp_particle_setSimple(l_particle_kagerou[i_this->field_0x60c],
                    &sp1C, 0xff, g_whiteColor, g_whiteColor,
                    0, 0.0f);
            }

            if (i_this->field_0x5ce == 1 && i_this->field_0x60a != 0xff) {
                dComIfGs_offSwitch(i_this->field_0x60a, fopAcM_GetRoomNo(a_this));
            }

            if (i_this->field_0x60a != 0xff && !dComIfGs_isSwitch(i_this->field_0x60a, fopAcM_GetRoomNo(a_this))) {
                i_this->field_0x5a4 = 0;
                if (i_this->field_0x60a != 0xff) {
                    i_this->field_0x60d = i_this->field_0x60a + 1;
                }

                if (i_this->field_0xa5b == 0) {
                    mDoAud_seStart(Z2SE_OBJ_FIRE_OFF, &i_this->field_0x634, 0, 0);
                }
            } else {
                mDoAud_seStart(Z2SE_OBJ_FIRE_BURNING, &i_this->field_0x634, 0, 0);
            }
            // fallthrough intentional.
        default:
            if (i_this->field_0xa5c != -1) {
                if (ep_switch_event_end(i_this)) {
                    i_this->field_0xa5c = -1;
                } else {
                    dComIfGp_evmng_cutEnd(i_this->field_0xa5c);
                }
            }
            break;
        case 10:
            cLib_addCalc0(&i_this->field_0x5b0, 1.0f, 0.1f);
            if (i_this->field_0x5b0 < 0.05f) {
                if (i_this->mpModel) {
                    i_this->field_0x5a4 = 0;
                    if (i_this->field_0x60a != 0xff) {
                        dComIfGs_offSwitch(i_this->field_0x60a, fopAcM_GetRoomNo(a_this));
                        i_this->field_0x60d = i_this->field_0x60a + 1;
                    }
                } else {
                    fopAcM_delete(a_this);
                }
            }
            break;
    }

    i_this->mLightInf.mPosition = i_this->field_0x634;
    i_this->mLightInf.mColor.r = 0xaf;
    i_this->mLightInf.mColor.g = 0x5d;
    i_this->mLightInf.mColor.b = 0;
    i_this->mLightInf.mPow = (500.0f + SREG_F(11)) * i_this->field_0x5b0;
    i_this->mLightInf.mFluctuation = 1.0f;
    a_this->scale.y = i_this->field_0x5b0;

    if (a_this->scale.y > 0.5f) {
        i_this->mSph1.OnAtSetBit();
    } else {
        i_this->mSph1.OffAtSetBit();
    }

    cXyz local_28(i_this->field_0x634.x, i_this->field_0x634.y + 30.0f, i_this->field_0x634.z);
    i_this->mSph1.SetC(local_28);
    dComIfG_Ccsp()->Set(&i_this->mSph1);

    if (i_this->field_0xa50 != 0) {
        if (i_this->mBaseEmitter != NULL) {
            cXyz local_34, cStack_40;
            f32 fVar1;
            if ( i_this->field_0xa50 > (s16) (10+TREG_S(3)) ) {
                fVar1 = 4.0f + TREG_F(5);
            } else {
                fVar1 = 0.0f;
            }

            cLib_addCalc2(&i_this->field_0xa54, fVar1, 1.0f, 0.5f + TREG_F(8));
            cMtx_YrotS(*calc_mtx, i_this->field_0xa58);

            local_34.set(0.0f, 1.0f, i_this->field_0xa54);
            MtxPosition(&local_34, &cStack_40);

            i_this->mBaseEmitter->setDirection(cStack_40);

            if (i_this->field_0xa50 == 1) {
                i_this->mBaseEmitter->becomeInvalidEmitter();
                i_this->mBaseEmitter = NULL;
            }
        }

        i_this->field_0xa50--;
    }

    return;
}

static void daEp_set_mtx(ep_class* i_this) {
    if (i_this->mpModel) {
        f32 fVar1 = 70.0f;
        if ((i_this->field_0xa5a & 1) != 0) {
            fVar1 = 140.0f;
        }

        MtxTrans(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z, 0);
        cMtx_YrotM(*calc_mtx, i_this->current.angle.y);
        MtxTrans(0.0f, fVar1, 0.0f, 1);
        cMtx_XrotM(*calc_mtx, i_this->shape_angle.x);
        MtxTrans(0.0f, -fVar1, 0.0f, 1);
        i_this->mpModel->setBaseTRMtx(*calc_mtx);

        i_this->field_0x5dc = i_this->field_0x634;
        i_this->field_0x5f4 = i_this->field_0x5e8;
        
        cXyz local_28;
        local_28.z = 0.0f;
        local_28.x = 0.0f;
        if ((i_this->field_0xa5a & 1) != 0) {
            local_28.y = 280.0f;
        } else {
            local_28.y = 140.0f;
        }
        MtxPosition(&local_28, &i_this->field_0x634);

        i_this->eyePos = i_this->field_0x634;
        i_this->eyePos.y += 20.0f;
        i_this->attention_info.position = i_this->eyePos;
        i_this->attention_info.position.y += 30.0f;

        local_28.y = 0.0f;
        MtxPosition(&local_28, &i_this->field_0x5e8);
    } else {
        i_this->field_0x634 = i_this->current.pos;
    }
}

static int daEp_Execute(ep_class* i_this) {
    static u16 eff_name[3] = {
        0x8340, 0x8341, 0x8342,
    };

    fopAc_ac_c* a_this = i_this;
    i_this->field_0x601 = 0;
    i_this->field_0x602 = 0;
    if (i_this->field_0x640) {
        i_this->field_0x640--;
    }

    if (i_this->mpModel) {
        cXyz local_78 = a_this->current.pos;
        if (i_this->field_0x640 != 0) {
            local_78.y -= 8000.0f;
        }

        i_this->mCyl.SetC(a_this->current.pos);
        dComIfG_Ccsp()->Set(&i_this->mCyl);

        if (i_this->field_0x640 == 0 && i_this->mCyl.ChkTgHit()) {
            i_this->field_0x640 = 10;
            daObj::HitSeStart(&a_this->current.pos, fopAcM_GetRoomNo(a_this), &i_this->mCyl, 11);
        }

        fopAcM_rollPlayerCrash(a_this, 35.0f, 0, 200.0f, -100.0f, 0, -0.8999999761581421);
        mDoLib_clipper::changeFar(1000000.0f);
        i_this->field_0x602 = fopAcM_checkCullingBox(i_this->mpModel->getBaseTRMtx(),
                                                     -30.0f, 0.0f, -30.0f, 30.0f, 180.0f, 30.0f);
        mDoLib_clipper::resetFar();
    }

    for (int i = 0; i < 3; i++) {
        if (i_this->field_0x5c8[i]) {
            i_this->field_0x5c8[i]--;
        }
    }

    if (i_this->field_0x5ce && i_this->field_0x5ce < 10000) {
        i_this->field_0x5ce--;
    }

    if (!dComIfGp_event_runCheck() && i_this->field_0xa5b != 0) {
        i_this->field_0xa5b--;
    }

    if (!i_this->field_0x5c8[0]) {
        i_this->field_0x5c8[0] = cM_rndF(5.0f);
        i_this->field_0x5c4 = cM_rndF(4.0f) + 8.0f;
    }

    if (!i_this->field_0x5c8[1]) {
        if (i_this->field_0xa50) {
            i_this->field_0x5c8[1] = cM_rndF(5.0f);
            i_this->field_0x5bc = cM_rndF(0.2f) + 0.55f;
        } else {
            i_this->field_0x5c8[1] = cM_rndF(6.0f) + 3.0f;
            i_this->field_0x5bc = cM_rndF(0.075f) + 0.75f;
        }
    }

    cLib_addCalc2(&i_this->field_0x5c0, i_this->field_0x5c4, 1.0f, 1.0f);
    cLib_addCalc2(&i_this->field_0x5b8, i_this->field_0x5bc, 0.4f, 0.04f);

    mDoLib_clipper::changeFar(1000000.0f);
    i_this->field_0x601 = fopAcM_checkCullingBox(i_this->field_0x570, -160.0f, -160.0f, -160.0f, 160.0f, 160.0f, 160.0f);
    mDoLib_clipper::resetFar();

    if (i_this->field_0xa79 == 0) {
        ep_move(i_this);
        if (i_this->mCyl.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mCyl.GetTgHitObj();
            if (i_this->mAtInfo.mpCollider) {
                at_power_check(&i_this->mAtInfo);
                if (i_this->mAtInfo.mpCollider->ChkAtType(0x10040) || i_this->mAtInfo.mAttackPower >= 10) {
                    i_this->field_0xa79 = 1;
                    if (i_this->mAtInfo.mHitType == 1) {
                        dScnPly_c::setPauseTimer(2);
                    }

                    mDoAud_seStart(JA_SE_OBJ_BREAK_WOOD_L, &i_this->field_0x634, 0, 0);
                    dKy_plight_cut(&i_this->mLightInf);
                }
            }
        }
    }

    fopAcM_OffStatus(a_this, 0);
    a_this->attention_info.flags = 0;
    cLib_offBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_BATTLE_e);

    if (i_this->field_0xa79 == 1) {
        i_this->field_0xa79 = 2;
        i_this->mpModel = NULL;
        ep_hahen_s* epHahenS = i_this->mHahen;

        cXyz local_84, cStack_90;
        for (int i = 0; i < 6; i++, epHahenS++) {
            epHahenS->field_0x97 = 1;
            epHahenS->field_0x4 = a_this->home.pos;
            if (i < 3) {
                epHahenS->field_0x4.y += 40.0f;
                epHahenS->field_0x28.z = 0x363c;
            } else {
                epHahenS->field_0x4.y += 110.0f;
                epHahenS->field_0x28.z = -0x363c;
            }
            epHahenS->field_0x28.y = (i * 0x5555) + 1000;

            cMtx_YrotS(*calc_mtx, epHahenS->field_0x28.y);
            local_84.x = 0.0f;
            local_84.y = 0.0f;
            local_84.z = 10.0f + AREG_F(10);
            MtxPosition(&local_84, &cStack_90);
            epHahenS->field_0x4 += cStack_90;

            epHahenS->field_0x90 = 3000.0f + cM_rndF(1000.0f);
            epHahenS->field_0x92 = cM_rndFX(2000.0f);
            epHahenS->field_0x1c.y = 20.0f + cM_rndF(15.0f);
            epHahenS->field_0x1c.x = cM_rndFX(15.0f);
            epHahenS->field_0x1c.z = cM_rndFX(15.0f);
            epHahenS->field_0x96 = 0;
        }

        cXyz local_9c = i_this->field_0x634;
        local_9c.y += 20.0f;

        for (int i = 0; i < 3; i++) {
            dComIfGp_particle_set(eff_name[i], &local_9c, NULL, NULL);
        }
    }

    if (i_this->field_0xa79) {
        hahen_move(i_this);
    }

    return 1;
}

static int daEp_IsDelete(ep_class* i_this) {
    return 1;
}

static int daEp_Delete(ep_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "Ep");
    dKy_plight_cut(&i_this->mLightInf);
    return 1;
}

static int model_d[8] = {
    3,4, 5, 6, 6, 6, 6, 6,
};

static int daEp_CreateHeap(fopAc_ac_c* a_this) {
    ep_class* i_this = (ep_class*)a_this;

    void* modelData = dComIfG_getObjectRes("Ep", model_d[i_this->field_0xa5a]);
    JUT_ASSERT(0x855, modelData != NULL);

    i_this->mpModel = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
    if (i_this->mpModel == NULL) {
        return 0;
    }
    
    MtxScale(0.0f, 0.0f, 0.0f, 0);
    i_this->mpModel->setBaseTRMtx(*calc_mtx);

    modelData = dComIfG_getObjectRes("Ep", 7);
    JUT_ASSERT(0x884, modelData != NULL);

    for (int i = 0; i < 6; i++) {
        i_this->mHahen[i].mpModel = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
        if (i_this->mHahen[i].mpModel == NULL) {
            return 0;
        }
    }
    
    return 1;
    
}

static void daEp_CreateInit(fopAc_ac_c* a_this) {
    ep_class* i_this = (ep_class*)a_this;

    i_this->field_0x5a4 = 0;
    i_this->field_0x60d = 0;
    fopAcM_SetMtx(i_this, i_this->field_0x570);
    fopAcM_SetMin(i_this, -160.0f, -160.0f, -160.0f);
    fopAcM_SetMax(i_this, 160.0f, 160.0f, 160.0f);
    daEp_set_mtx(i_this);
    i_this->field_0x601 = 0;
    i_this->field_0x602 = 0;
}

static int daEp_Create(fopAc_ac_c* a_this) {
    static dCcD_SrcSph sph_src = {
        {
            {0x0, {{0x0, 0x0, 0x10}, {0x20, 0x11}, 0x0}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x1, 0x0}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };

    static dCcD_SrcCyl co_cyl_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdbf, 0x1f}, 0x79}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            35.0f, // mRadius
            120.0f // mHeight
        } // mCyl
    };

    static dCcD_SrcSph at_sph_src = {
        {
            {0x0, {{AT_TYPE_THROW_OBJ, 0x0, 0x1f}, {0x0, 0x0}, 0x0}}, // mObj
            {dCcD_SE_THROW_OBJ, 0x1, 0x0, 0x0, 0x1}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };

    int rv;
    ep_class* i_this = (ep_class*)a_this;
    fopAcM_ct(a_this, ep_class);

    rv = dComIfG_resLoad(&i_this->mPhase, "Ep");
    if (rv == cPhs_COMPLEATE_e) {
        if (fopAcM_GetParam(i_this) & 8) {
            i_this->field_0x60c = fopAcM_GetParam(i_this) & 7;
            i_this->field_0x60c += 1;
            i_this->field_0x60c &= 1;
            i_this->field_0xa5a = -1;
        } else {
            i_this->field_0xa5a = fopAcM_GetParam(i_this) & 7;
            i_this->field_0xa5a += 1;
            i_this->field_0xa5a &= 7;
        }
    
        i_this->field_0x609 = fopAcM_GetParam(i_this) >> 8;
        i_this->field_0x60b = fopAcM_GetParam(i_this) >> 16;
        i_this->field_0x60a = fopAcM_GetParam(i_this) >> 24;

        i_this->mStts.Init(0xff, 0xff, i_this);
        i_this->mSph1.Set(sph_src);
        i_this->mSph1.SetStts(&i_this->mStts);

        if (i_this->field_0x60c == 1) {
            i_this->mSph1.SetR(70.0f);
        }

        if (i_this->field_0xa5a < 0) {
            OS_REPORT("   /////   EP PARAM %x\n");
        } else {
            if (!fopAcM_entrySolidHeap(i_this, daEp_CreateHeap, 0x4b000)) {
                return cPhs_ERROR_e;
            }

            i_this->mCyl.Set(co_cyl_src);
            i_this->mCyl.SetStts(&i_this->mStts);
            i_this->mCyl.OnTgNoHitMark();

            if ((i_this->field_0xa5a & 1) != 0) {
                i_this->mCyl.SetH(240.0f);
            }
        }

        daEp_CreateInit(i_this);
        if (i_this->field_0x60a != 0xff) {
            if (!dComIfGs_isSwitch(i_this->field_0x60a, fopAcM_GetRoomNo(i_this))) {
                i_this->field_0x60d = i_this->field_0x60a + 1;
            }
        }

        i_this->field_0x5ce = 20000;
        dKy_plight_set(&i_this->mLightInf);
        i_this->field_0xe6c = cM_rndF(255.0f);
    }

    i_this->field_0xa5c = -1;
    i_this->attention_info.distances[4] = 7;
    fopAcM_OnCarryType(i_this, fopAcM_CARRY_UNK_30);

    i_this->mSph2.Set(at_sph_src);
    i_this->mSph2.SetStts(&i_this->mStts);
    i_this->mSph2.OffAtVsPlayerBit();
    i_this->field_0xa5b = 60;

    return rv;
}

static actor_method_class l_daEp_Method = {
    (process_method_func)daEp_Create,
    (process_method_func)daEp_Delete,
    (process_method_func)daEp_Execute,
    (process_method_func)daEp_IsDelete,
    (process_method_func)daEp_Draw,
};

extern actor_process_profile_definition g_profile_EP = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_EP,
    &g_fpcLf_Method.base,
    sizeof(ep_class),
    0,
    0,
    &g_fopAc_Method.base,
    691,
    &l_daEp_Method,
    0x10244000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e
};
