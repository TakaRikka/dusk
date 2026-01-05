/**
 * @file d_a_e_mm_mt.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_e_mm_mt.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_enemy.h"

static int daE_MM_MT_Draw(e_mm_mt_class* i_this) {
    i_this->enemy.model = i_this->mp_model;
    fopAc_ac_c* actor = (fopAc_ac_c*) i_this;

    g_env_light.settingTevStruct(0, &i_this->enemy.current.pos, &i_this->enemy.tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mp_model, &i_this->enemy.tevStr);

    mDoExt_modelUpdateDL(i_this->mp_model);
    
    cXyz local_18(i_this->enemy.current.pos.x, i_this->enemy.current.pos.y + 100, i_this->enemy.current.pos.z);

    if (!fopAcM_CheckStatus(actor, fopAcM_STATUS_CARRY_NOW)) {
        i_this->m_shadowKey = dComIfGd_setShadow(
            i_this->m_shadowKey, 1, i_this->mp_model, &local_18, 700.0f, 0.0f,
            i_this->enemy.current.pos.y, i_this->m_acch.GetGroundH(), i_this->m_acch.m_gnd,
            &i_this->enemy.tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex()
        );
    }

    return 1;
}

static bool e_mm_hookCheck(e_mm_mt_class* i_this) {

    cCcD_Obj* hitObject;
    cXyz VStack_18;
    u32 uVar3;

    fopAc_ac_c* actor = (fopAc_ac_c*) i_this;

    if (i_this->enemy.argument == 1) {
        return false;
    }

    if (i_this->m_sphere.ChkTgHit() 
        && i_this->m_sphere.GetTgHitObj()->ChkAtType(AT_TYPE_HOOKSHOT)
        || fopAcM_CheckStatus(actor, 0x100000)) {
        i_this->field_0x69C = 0.0f;
        i_this->field_0x68A[0] = true;
        cXyz local_24 = i_this->enemy.current.pos;
        if (i_this->m_action == 1) {
            local_24. y -=20.0f;
        } else {
            local_24.y += 20.0f;
        }

        mDoMtx_stack_c::transS(i_this->enemy.current.pos);
        mDoMtx_stack_c::ZXYrotM(i_this->enemy.shape_angle);
        mDoMtx_stack_c::inverse();
        mDoMtx_stack_c::multVec(&local_24, &VStack_18);
        daPy_py_c* pdVar3 = daPy_getPlayerActorClass();
        pdVar3->setHookshotCarryOffset(fopAcM_GetID(i_this), &VStack_18);
        return 1;
    }
    

    if (i_this->field_0x68A[0]) {
        if (daPy_getPlayerActorClass()->setForceGrab(actor, 0, 1)) {
            i_this->m_action = 2;
            i_this->m_mode = 0;
            i_this->field_0x69C = 0.0f;
            i_this->field_0x6A2 = 0;
            i_this->field_0x6A0 = 0;
            i_this->m_spin = 0;
            i_this->m_carryAngleSpeed = 0;
            if (i_this->m_acch.ChkWaterHit()) {
                mDoAud_seStart(Z2SE_EN_MM_MET_LIFT_WTR, &i_this->enemy.current.pos, 0, 0);
            }
            i_this->m_sphere.OffAtSetBit();
            i_this->field_0x68A[0] = false;
            return 1;
        }
        i_this->field_0x68A[0] = false;
    }
    return 0;
}

static void e_mm_mt_normal(e_mm_mt_class* i_this) {
    fopAc_ac_c* helmasaurActor;
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);
    
    
    if (fopAcM_SearchByID(actor->parentActorID, &helmasaurActor)) {
        actor->scale.x = helmasaurActor->scale.x;
        e_mm_class* helmasaur = (e_mm_class*)helmasaurActor;
        actor->shape_angle.y = helmasaurActor->shape_angle.y - 0x4000;

        PSMTXCopy(helmasaur->modelMorf->getModel()->getAnmMtx(11), *calc_mtx);
        cXyz acStack_28(0.0f, 0.0f, 0.0f);
        MtxPosition(&acStack_28, &actor->current.pos);
        actor->eyePos = helmasaur->enemy.eyePos;
        actor->attention_info.position = i_this->enemy.eyePos;
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
        if (!i_this->m_invulnerabilityTimer && helmasaur->field_0xb98) {
            i_this->m_sphere.OnAtSetBit();
            if (i_this->m_sphere.ChkAtShieldHit()) {
                i_this->m_invulnerabilityTimer = 15;
                helmasaur->field_0xb99 |= 1;
                i_this->m_atInfo.mpCollider = i_this->m_sphere.GetAtHitObj(); 
                i_this->m_sphere.SetAtSe(9);
            }
        } else {
            i_this->m_sphere.OffAtSetBit();
        }
        if (actor->argument != 1 
            && ((i_this->m_sphere.ChkTgHit() 
            && i_this->m_sphere.GetTgHitObj()->ChkAtType(AT_TYPE_HOOKSHOT)) 
            || fopAcM_CheckStatus(actor, 0x100000))) {
                helmasaur->field_0xb99 |= 4;
        }
        if (!helmasaur->field_0x672) {
            i_this->m_action = 1;
            i_this->m_mode = 0;
            i_this->field_0x68A[0] = 0;
            i_this->m_acch.OnLineCheck();
            i_this->m_sphere.OnTgShield();
            i_this->m_sphere.SetTgHitMark(CcG_Tg_UNK_MARK_2);
            i_this->m_sphere.SetTgSe(9);
            i_this->m_sphere.OffTgHookShotNoHitMark();
            i_this->m_sphere.OffTgNoHitMark();
            i_this->m_sphere.OffCoSetBit();
            fopAcM_SetGroup(actor, 3);
            fopAcM_OffStatus(actor, 0);
            actor->attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
            actor->attention_info.distances[2] = 0;
            fopAcM_OnStatus(actor, 0x400);
            s16 actor_angle = player->shape_angle.y + 0x4000;
            s16 angle = actor->shape_angle.y - actor_angle;
            if (angle > 0x4000 || angle < -0x4000) {
                i_this->m_carryAngle = -0x8000;
                return;
            }
            i_this->m_carryAngle = 0;
        }
    }
}

static void e_mm_mt_hagare(e_mm_mt_class* i_this) {
    fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);
    fopAc_ac_c* actor = (fopAc_ac_c*) i_this;

    cXyz local_30 = player->current.pos - i_this->enemy.current.pos;
    local_30.y += 150.0f;
    i_this->enemy.current.angle.y = local_30.atan2sX_Z();
    cMtx_YrotS(*calc_mtx, i_this->enemy.current.angle.y);
    cMtx_XrotM(*calc_mtx, (int)local_30.atan2sY_XZ());
    local_30.x = 0.0f;
    local_30.y = 0.0f;
    local_30.z = i_this->enemy.speedF;
    MtxPosition(&local_30, &i_this->enemy.speed);
    if (!e_mm_hookCheck(i_this)) {
        int arg = i_this->m_mode;
        switch (arg) {
            case -10:
                cLib_addCalc2(&i_this->enemy.speedF, 3.0f, 1.0f, 0.15f); 
                cLib_addCalc2(&i_this->field_0x69C, 2000.0f, 0.1f, 200.0f);
                if (i_this->enemy.speedF >= 2.95f) {
                    i_this->m_mode = 0;
                } else {
                    break;
                }
                // fallthrough
            case 0:
                cLib_addCalc2(&i_this->enemy.speedF, 100.0f, 1.0f, 5.0f);
                break;
        }
        i_this->enemy.current.pos += i_this->enemy.speed;
        i_this->enemy.eyePos = i_this->enemy.current.pos;
        i_this->enemy.attention_info.position = i_this->enemy.current.pos;
        s16 spA = i_this->enemy.speedF * 30.0f;
        s16 sVar6;
        if (i_this->m_carryAngle == 0) {
            sVar6 = -0x4000;
        } else {
            sVar6 = 0x4000;
        }

        cLib_addCalcAngleS2(&i_this->enemy.shape_angle.y, 
                        player->shape_angle.y + 0x4000 +  i_this->m_carryAngle,
                        4,
                        spA + 0x100);
        cLib_addCalcAngleS2(&i_this->enemy.shape_angle.z, sVar6, 4, spA);
        local_30 = player->current.pos - i_this->enemy.current.pos;
        local_30.y = 0.0f;
        i_this->m_action = 3;
        i_this->m_mode = 0;
        i_this->m_sphere.OnCoSetBit();
        i_this->field_0x68A[0] = 0;
        i_this->enemy.speedF *= 0.5f;
    }
}

static void e_mm_mt_carry(e_mm_mt_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*) &i_this->enemy;
    fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);

    switch (i_this->m_mode) {
        case 0: {
            cLib_offBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_CARRY_e);
            i_this->m_mode = 1;
            s16 angle_delta_y1 = 0x4000 + player->shape_angle.y;
            s16 angle_delta_y = i_this->m_rotation.y - angle_delta_y1;
            if (angle_delta_y > 0x4000 || angle_delta_y < -0x4000) {
                i_this->m_carryAngle = -0x8000;
            } else {
                i_this->m_carryAngle = 0;
            }
            void(0);
            break;
        }
        case 1:
            if (!fopAcM_checkCarryNow(actor)) {
             i_this->m_action = 3;
             i_this->m_sphere.OnCoSetBit();
             i_this->field_0x68A[0] = false;
             if (fopAcM_GetSpeedF(actor) > 1.0f || fopAcM_GetSpeedF(player) > 10.0f) {
                 actor->speedF = TREG_F(9) + 30.0f + fopAcM_GetSpeedF(player) * 0.25f;
                actor->speed.y = TREG_F(10) + 30.0f;
                actor->current.angle.y = player->shape_angle.y;
                i_this->m_mode = 0;
             } else {
                actor->speedF = 0.0f;
                actor->speed.y = 0.0f;
                i_this->m_mode  = 5;
                i_this->field_0x69C = NREG_F(2) + 1000.0f;
                if (i_this->m_acch.ChkWaterHit()) {
                    mDoAud_seStart(Z2SE_EN_MM_MET_PUT_WTR, &actor->current.pos, 0, 0);
                } else {
                    mDoAud_seStart(Z2SE_EN_MM_MET_PUT_LND, &actor->current.pos, 0, 0);
                }
             }
            }
            // fallthrough
    }
    if (i_this->m_acch.GetGroundH() == -G_CM3D_F_INF) {
        i_this->field_0x698 += 1;
        if (i_this->field_0x698 > 500) {
            fopAcM_delete(actor);
            return;
        }
    } else {
        i_this->field_0x698 = 0;
    }
    actor->eyePos = actor->current.pos;
    actor->attention_info.position = actor->current.pos;
    cLib_addCalcAngleS2(&i_this->m_rotation.y, player->shape_angle.y +  0x4000 + i_this->m_carryAngle,
        1, i_this->m_carryAngleSpeed);
    cLib_addCalcAngleS2(&i_this->m_rotation.z, (s16)((TREG_S(5) + 0x10000) - 0x8000), 1, i_this->m_carryAngleSpeed);
    cLib_addCalcAngleS2(&i_this->m_carryAngleSpeed, 0x2000, 1, 0x100);
}

static s16 wall_angle_get(e_mm_mt_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz vec1, vec2, vec3[2];
    dBgS_LinChk linchk;
    cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec1.z = -50.0f; 
    MtxPosition(&vec1, &vec2);
    vec2 += a_this->current.pos;
    vec1.x = 5.0f;
    vec1.y = 0.0f;
    vec1.z = 200.0f;

    for (int i = 0; i < 2; i++) {
        MtxPosition(&vec1, &vec3[i]);
        vec1.x *= -1.0f;
        vec3[i] += vec2;
        linchk.Set(&vec2, &vec3[i], a_this);
        if (dComIfG_Bgsp().LineCross(&linchk)) {
            vec3[i] = linchk.GetCross();
        } else {
            return 0x23;
        }
    }

    vec1 = vec3[1] - vec3[0]; 
    return  vec1.atan2sX_Z() + 0x4000;
}

static void e_mm_mt_drop(e_mm_mt_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*) i_this;

    cXyz local_28;
    cXyz local_34;

    if (!e_mm_hookCheck(i_this)) {
        switch (i_this->m_mode) {
            case 0:
                // fallthrough
            case 1:
                // fallthrough
            case 2:
                // fallthrough
            case 3:
                // fallthrough
            case 4:
                if (!i_this->m_timer[0] && i_this->m_acch.ChkWallHit()) {
                    s16 wall_angle = wall_angle_get(i_this);
                    if (wall_angle != 0x23) {
                        wall_angle = i_this->enemy.current.angle.y - wall_angle; 
                        i_this->m_spin = wall_angle * (TREG_F(6) + -0.3f);
                        i_this->enemy.current.angle.y += (s16)(0x8000 - (wall_angle << 1)); 
                        if (i_this->m_acch.ChkWaterHit()) {
                            mDoAud_seStart(Z2SE_EN_MM_MET_BOUND_WTR, &i_this->enemy.current.pos, (u32)(i_this->enemy.speed.y), 0);
                        } else {
                            mDoAud_seStart(Z2SE_EN_MM_MET_BOUND_LND, &i_this->enemy.current.pos, (u32)(i_this->enemy.speed.y), 0);
                        }
                        i_this->enemy.speedF *= (AREG_F(4) + 0.5f);
                        i_this->m_timer[0] = 10;
                    }
                }
                if (i_this->m_acch.ChkGroundHit()) {
                    if (i_this->m_mode < 4) {
                        i_this->m_mode += 1;
                    }
                    if (i_this->field_0x6A8 < -30.0f) {
                        i_this->enemy.speed.y = i_this->field_0x6A8 * -0.4f;
                        i_this->enemy.speedF *= (TREG_F(0xE) + 0.75f);
                        if (i_this->m_acch.ChkWaterHit()) {
                            mDoAud_seStart(Z2SE_EN_MM_MET_BOUND_WTR, &i_this->enemy.current.pos, (u32)(i_this->enemy.speed.y), 0);
                        } else {
                            mDoAud_seStart(Z2SE_EN_MM_MET_BOUND_LND, &i_this->enemy.current.pos, (u32)(i_this->enemy.speed.y), 0);
                        }
                        i_this->field_0x69C = NREG_F(0) + 3500.0f;
                        cXyz local_40 = i_this->enemy.current.pos; 
                        cXyz local_4c(1.5f, 1.5f, 1.5f); 
                        if (!i_this->m_spin ) {
                            i_this->m_spin = cM_rndFX(1300.0f);
                        }
                    } else {
                        i_this->m_mode = 5;
                        if (i_this->m_acch.ChkWaterHit()) {
                            mDoAud_seStart(Z2SE_EN_MM_MET_BOUND_WTR, &i_this->enemy.current.pos, 0, 0);
                        } else {
                            mDoAud_seStart(Z2SE_EN_MM_MET_BOUND_LND, &i_this->enemy.current.pos, 0, 0);
                        }    
                    }
                }
                break;
        case 5:
            cLib_addCalc0(&i_this->enemy.speedF, 1.0f, TREG_F(0xB) + 2.0f);
            break;
        }
        cMtx_YrotS(*calc_mtx, i_this->enemy.current.angle.y);
    
        local_28.x = 0.0f;
        local_28.y = 0.0f; 
        local_28.z = i_this->enemy.speedF;
        
        MtxPosition(&local_28, &local_34);
        i_this->enemy.speed.x = local_34.x;
        i_this->enemy.speed.z = local_34.z;
        i_this->enemy.current.pos.x += i_this->enemy.speed.x;
        i_this->enemy.current.pos.z += i_this->enemy.speed.z;
        i_this->enemy.current.pos.y += i_this->enemy.speed.y;
        i_this->enemy.speed.y += -5.0f;
        i_this->enemy.eyePos = i_this->enemy.current.pos;
        i_this->enemy.attention_info.position = i_this->enemy.current.pos;
        cLib_addCalcAngleS2(&i_this->m_rotation.z, (TREG_S(6)+0x10000)-0x8000, 1, 0x1000);
        cLib_onBit<u32>(i_this->enemy.attention_info.flags, fopAc_AttnFlag_CARRY_e);
        i_this->enemy.attention_info.distances[4] = 7;
        i_this->m_rotation.y += i_this->m_spin;
        if (i_this->enemy.speedF >= 20.0f) {
            i_this->m_sphere.OnAtSetBit();
            i_this->m_sphere.OffAtVsBitSet(12);
            i_this->m_sphere.OnAtVsBitSet(2);
            i_this->m_sphere.SetAtHitMark(1);
            if (!i_this->m_invulnerabilityTimer && i_this->m_sphere.ChkAtHit()) {
                i_this->m_invulnerabilityTimer = 4;
                i_this->enemy.speedF *= 0.5f;
            }
        } else {
             i_this->m_sphere.OffAtSetBit();
        }
        if (fopAcM_checkCarryNow(actor)) { 
            i_this->m_action = 2;
            i_this->m_mode = 0;
            i_this->m_carryAngleSpeed = \
            i_this->m_spin = \
            i_this->field_0x6A0 = \
            i_this->field_0x6A2 = 0;
            if (i_this->m_acch.ChkWaterHit()) {
                mDoAud_seStart(Z2SE_EN_MM_MET_LIFT_WTR, &i_this->enemy.current.pos, 0, 0);
            }
            i_this->m_sphere.OffAtSetBit();
            return;
        } 
        if (fopAcM_CheckStatus(actor, 0x200) && !i_this->m_invulnerabilityTimer) {
            i_this->m_action = 1;
            i_this->m_mode = -10;
            i_this->field_0x68A[0] = false;
            i_this->m_sphere.OffCoSetBit();
            i_this->enemy.shape_angle = i_this->m_rotation;

            fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);
            s16 sVar1 = i_this->enemy.shape_angle.y - (s16)(player->shape_angle.y + 0x4000);

            if (sVar1 > 0x4000 || sVar1 < -0x4000) {
                i_this-> m_carryAngle = -0x8000;
                return;
            } 
            i_this->m_carryAngle = 0;
        }
    }
}

static void action(e_mm_mt_class* i_this) {
    fopAc_ac_c* unusedPlayer = dComIfGp_getPlayer(0);
    fopAc_ac_c* actor = static_cast<fopAc_ac_c*>(&i_this->enemy);
    cXyz tgHitPosDist;
    cXyz tgHitPosAbove;

    fopAc_ac_c* helamsaurActor;

    s16 distAngle = 0;
    e_mm_class* helmasaur = NULL;
    if (fopAcM_SearchByID(actor->parentActorID, &helamsaurActor) && helamsaurActor != NULL) {
        helmasaur = (e_mm_class*)helamsaurActor;
    }

    cXyz* ccMove_p = i_this->m_stts.GetCCMoveP();
    if (helmasaur != NULL) {
        helmasaur->field_0x674.x = ccMove_p->x;
        helmasaur->field_0x674.z = ccMove_p->z;
    }

    switch (i_this->m_action) {
    case 0:
        e_mm_mt_normal(i_this);
        i_this->m_acch.CrrPos(dComIfG_Bgsp());
        if (helmasaur != NULL) {
            distAngle = cLib_distanceAngleS(helmasaur->enemy.shape_angle.y - (s16)0x8000,
                                            fopAcM_searchPlayerAngleY(actor));
            if (distAngle > 0x3000 || distAngle < -0x3000 || i_this->m_action) {
                i_this->m_sphere.OnTgShield();
                i_this->m_sphere.SetTgHitMark(CcG_Tg_UNK_MARK_2);
                i_this->m_sphere.SetTgSe(9);
                i_this->m_sphere.OnTgNoHitMark();
            } else {
                i_this->m_sphere.OffTgShield();
                i_this->m_sphere.SetTgHitMark(CcG_Tg_UNK_MARK_0);
                i_this->m_sphere.SetTgSe(0);
                i_this->m_sphere.OffTgNoHitMark();
            }
        }

        break;
    case 1:
        e_mm_mt_hagare(i_this);
        i_this->m_acch.CrrPos(dComIfG_Bgsp());
        i_this->m_rotation = actor->shape_angle;

        break;
    case 2:
        e_mm_mt_carry(i_this);
        i_this->m_invulnerabilityTimer = 3;
        i_this->m_acchCir.SetWall(20.0f, 20.0f);
        i_this->m_acch.CrrPos(dComIfG_Bgsp());

        break;
    case 3:
        e_mm_mt_drop(i_this);
        i_this->field_0x6A8 = actor->speed.y;

        if (i_this->m_sphere.ChkCoHit()) {
            cXyz* ccMove_p = i_this->m_stts.GetCCMoveP();

            if (ccMove_p != NULL) {
                actor->current.pos.x += ccMove_p->x * 0.5f;
                actor->current.pos.z += ccMove_p->z * 0.5f;

                if (fabsf(ccMove_p->x) >= 2.0f || fabsf(ccMove_p->z) >= 2.0f) {
                    cLib_addCalc2(&i_this->field_0x69C, NREG_F(5) + 1000.0f, 0.1f,
                                  NREG_F(6) + 200.0f);
                    s16 playerAngleY = fopAcM_searchPlayerAngleY(actor);
                    s16 playerAngleDelta = i_this->m_rotation.y - playerAngleY;
                    if (playerAngleDelta > 0x4000 || playerAngleDelta < -0x4000) {
                        playerAngleY += (s16)0x8000;
                    }
                    cLib_addCalcAngleS2(&i_this->m_rotation.y, playerAngleY, 4, 0x100);
                }
            }
        }
        i_this->m_acchCir.SetWall(50.0f, 50.0f);
        i_this->m_acch.CrrPos(dComIfG_Bgsp());
        break;
    }

    fopEn_enemy_c* enemy = (fopEn_enemy_c*)actor;
    setMidnaBindEffect(enemy, &i_this->m_sound, &actor->current.pos,
                       &actor->scale);

    if (!i_this->m_invulnerabilityTimer) {
        i_this->m_stts.Move();
        if (i_this->m_sphere.ChkTgHit()) {
            i_this->m_invulnerabilityTimer = 10;
            i_this->m_atInfo.mpCollider = i_this->m_sphere.GetTgHitObj();
            if (i_this->m_sphere.GetTgHitObj()->ChkAtType(0x80)) {
                i_this->m_sound.startCollisionSE(0x40018, 45);
            }
            if (helmasaur != NULL && helmasaur->field_0x672) {
                if (i_this->m_sphere.GetTgSe() == 9 ||
                   (i_this->m_sphere.GetTgHitObj()->ChkAtType(0x8000000) &&
                    helmasaur->action == 2)) {
                    tgHitPosDist = actor->current.pos - *i_this->m_sphere.GetTgHitPosP();
                    csXyz hitMarkAngle;
                    tgHitPosAbove.set(*i_this->m_sphere.GetTgHitPosP());
                    if (i_this->m_sphere.GetTgHitObj()->ChkAtType(16)) {
                        tgHitPosAbove.y += 100.0f;
                    }
                    hitMarkAngle.x = 0;
                    hitMarkAngle.y = (s16)tgHitPosDist.atan2sX_Z();
                    hitMarkAngle.z = 0;
                    def_se_set(&i_this->m_sound, i_this->m_atInfo.mpCollider, 40, actor);

                    cCcD_Obj* collider = i_this->m_atInfo.mpCollider;
                    dComIfGp_setHitMark(2, actor, &tgHitPosAbove, &hitMarkAngle, NULL, 0);
                    helmasaur->field_0xb99 |= (s8)0x2;
                    i_this->m_sphere.ClrTgHit();
                } else {
                    if (!(helmasaur->field_0xb99 & 0x8) &&
                        !(helmasaur->field_0xb99 & 0x10))
                    {
                        s16 health = actor->health;
                        cc_at_check(actor, &i_this->m_atInfo);
                        helmasaur->atInfo.mpCollider = i_this->m_atInfo.mpCollider;
                        helmasaur->field_0x6ac = i_this->m_atInfo.mHitDirection.y;
                        i_this->m_invulnerabilityTimer = 8;

                        if ((int)actor->health <= 2000 - helmasaur->enemy.field_0x560) {
                            helmasaur->field_0xb99 |= (s8)0x10;
                            if (i_this->m_atInfo.mHitType == 1 ||
                                i_this->m_atInfo.mpCollider->ChkAtType(8))
                            {
                                if (!daPy_getPlayerActorClass()->checkCutJumpCancelTurn()) {
                                    dScnPly_c::setPauseTimer(5);
                                }
                            }

                            i_this->m_sphere.OnTgShield();
                            i_this->m_sphere.SetTgHitMark(CcG_Tg_UNK_MARK_2);
                            i_this->m_sphere.SetTgSe(9);
                            i_this->m_sphere.OffTgNoHitMark();
                        } else {
                            if (health != actor->health) {
                                i_this->m_sound.startCreatureVoice(Z2SE_EN_MM_V_DAMAGE, -1);
                            } else {
                                i_this->m_sound.startCreatureVoice(Z2SE_EN_MM_V_NO_DAMAGE, -1);
                            }

                            if (i_this->m_atInfo.mHitType == 1 ||
                                i_this->m_atInfo.mpCollider->ChkAtType(8))
                            {
                                if (!daPy_getPlayerActorClass()->checkCutJumpCancelTurn()) {
                                    dScnPly_c::setPauseTimer(2);
                                }
                            }

                            helmasaur->field_0xb99 |= (s8)0x8;
                        }

                        i_this->m_sphere.ClrTgHit();
                    }
                }
            } else {
                tgHitPosDist = actor->current.pos - *i_this->m_sphere.GetTgHitPosP();
                csXyz local_84;
                tgHitPosAbove.set(*i_this->m_sphere.GetTgHitPosP());

                if (i_this->m_sphere.GetTgHitObj()->ChkAtType(16)) {
                    tgHitPosAbove.y += 100.0f;
                }

                local_84.x = 0;
                local_84.y = (s16)tgHitPosDist.atan2sX_Z();
                local_84.z = 0;
                cCcD_Obj* collider = i_this->m_atInfo.mpCollider;
                def_se_set(&i_this->m_sound, i_this->m_atInfo.mpCollider, 40, actor);
                dComIfGp_setHitMark(2, actor, &tgHitPosAbove, &local_84, NULL, 0);

                fopAc_ac_c* local_a4 = at_power_check(&i_this->m_atInfo);

                if (i_this->m_atInfo.mHitType == 1) {
                    if (daPy_getPlayerActorClass()->getCutType() == 8 ||
                        daPy_getPlayerActorClass()->getCutType() == 9 ||
                        daPy_getPlayerActorClass()->getCutType() == 23 ||
                        daPy_getPlayerActorClass()->getCutType() == 24) {
                        actor->speedF = cM_rndF(10.0f) + 40.0f +  TREG_F(16);
                        i_this->m_spin = (s16)(cM_rndF(2000.0f) + 5500.0f);
                        i_this->field_0x69C = (s16)(cM_rndF(1000.0f) + 5000.0f);
                    } else {
                        actor->speedF = TREG_F(17) + 30.0f;
                        i_this->m_spin = (s16)cM_rndFX(3000.0f);
                        i_this->field_0x69C = NREG_F(10) + 4000.0f;
                    }
                } else {
                    if (local_a4) {
                        actor->speedF = local_a4->speedF * 0.7f;
                        actor->current.angle.y = local_a4->current.angle.y + (s16)cM_rndFX(5000.0f);
                    }
                    i_this->m_spin = (s16)cM_rndFX(3000.0f);
                    i_this->field_0x69C = NREG_F(10) + 4000.0f;
                }

                i_this->m_sphere.ClrTgHit();
            }
        }
    }

    if (helmasaur != NULL && helmasaur->field_0x672 && helmasaur->field_0xb99 != 4) {
        i_this->mp_model->setBaseTRMtx(helmasaur->modelMorf->getModel()->getAnmMtx(11));
    } else {
        i_this->field_0x6A0 = i_this->field_0x69C * cM_ssin(i_this->m_lifetime * (NREG_S(3) + 3000));
        i_this->field_0x6A2 = i_this->field_0x69C * cM_ssin(i_this->m_lifetime * (NREG_S(4) + 4000));

        cLib_addCalc0(&i_this->field_0x69C, 0.1f, NREG_F(1) + 50.0f);
        cLib_addCalcAngleS2(&i_this->m_spin, 0, 1, TREG_S(5) + 50);
        mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y,
                               actor->current.pos.z);
        cMtx_YrotS(*calc_mtx, i_this->m_rotation.y);

        if (fopAcM_checkCarryNow(actor)) {
            tgHitPosDist.x = 30.0f;
        } else {
            tgHitPosDist.x = 0.0f;
        }

        tgHitPosDist.y = 0.0f;
        tgHitPosDist.z = 0.0f;
        MtxPosition(&tgHitPosDist, &tgHitPosAbove);
        mDoMtx_stack_c::transM(tgHitPosAbove.x, tgHitPosAbove.y, tgHitPosAbove.z);

        mDoMtx_stack_c::YrotM((s16)i_this->m_rotation.y);
        mDoMtx_stack_c::XrotM((s16)i_this->field_0x6A0);
        mDoMtx_stack_c::ZrotM(i_this->m_rotation.z + i_this->field_0x6A2);
        mDoMtx_stack_c::scaleM(actor->scale.x, actor->scale.x, actor->scale.x);
        i_this->mp_model->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    cXyz curPos = actor->current.pos;
    if (helmasaur != NULL && helmasaur->field_0x672) {
        fopAc_ac_c* parentActor;
        if (fopAcM_SearchByID(actor->parentActorID, &parentActor) && parentActor) {
            cMtx_YrotS(*calc_mtx, parentActor->shape_angle.y);
            if (actor->argument != 1) {
                tgHitPosDist.x = JREG_F(8);
                tgHitPosDist.y = JREG_F(9) + -60.0f;
                tgHitPosDist.z = JREG_F(10) + 20.0f;
            } else {
                tgHitPosDist.x = JREG_F(11);
                tgHitPosDist.y = JREG_F(12) + -90.0f;
                tgHitPosDist.z = JREG_F(13) + 50.0f;
            }
            MtxPosition(&tgHitPosDist, &curPos);
            curPos += actor->current.pos;
        } else {
            helmasaur->field_0x672 = 0;
        }
    } else {
        cMtx_XrotS(*calc_mtx, i_this->m_rotation.z);
        tgHitPosDist.x = 0.0f;
        tgHitPosDist.y = -(actor->scale.x * 30.0f);
        tgHitPosDist.z = 0.0f;
        MtxPosition(&tgHitPosDist, &curPos);
        curPos += actor->current.pos;
    }
    i_this->m_sph.SetC(curPos);

    if (i_this->m_action == 1) {
        i_this->m_sph.SetR((JREG_F(15) + 60.0f) * actor->scale.x);
    } else if (i_this->m_action == 2) {
        i_this->m_sph.SetR((JREG_F(16) + 100.0f) * actor->scale.x);
    } else {
        i_this->m_sph.SetR((JREG_F(17) + 50.0f) * actor->scale.x);
    }

    dComIfG_Ccsp()->Set(&i_this->m_sph);

    if (i_this->m_invulnerabilityTimer) {
        curPos.y += 20000.0f;
    }
    i_this->m_sphere.SetC(curPos);

    i_this->m_sphere.SetR((JREG_F(14) + 40.0f) * actor->scale.x);

    dComIfG_Ccsp()->Set(&i_this->m_sphere);
}

static int daE_MM_MT_Execute(e_mm_mt_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*) i_this;
    i_this->m_lifetime++;

    for (int i = 0; i < 2; i++) {
        if (i_this->m_timer[i] != 0) {
            i_this->m_timer[i]--;
        }
    }
    if (i_this->m_invulnerabilityTimer != 0) {
        i_this->m_invulnerabilityTimer--;
    }
    action(i_this);
    fopAcM_OffStatus(actor, fopAcM_STATUS_UNK_0x200);
    i_this->m_sound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    return true;
}

static int daE_MM_MT_IsDelete(e_mm_mt_class* i_this) {
    return 1;
}

static int daE_MM_MT_Delete(e_mm_mt_class* i_this) {
    dComIfG_resDelete(&i_this->m_phase, "E_MM_MT");
    if (i_this->enemy.heap != NULL) {
        i_this->m_sound.deleteObject();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    J3DModelData* i_modelData;
    int resource_index = 4;

    if (i_this->argument == 1) {
        resource_index = 3;
    }
    i_modelData = (J3DModelData*) dComIfG_getObjectRes("E_MM_MT", resource_index);
    JUT_ASSERT(1197, i_modelData != NULL);

    ((e_mm_mt_class*)i_this)->mp_model = mDoExt_J3DModel__create(i_modelData, 0x80000, 0x11000084);

    if (((e_mm_mt_class*)i_this)->mp_model == NULL) {
        return 0;
    }

    return 1;
}

static int daE_MM_MT_Create(fopAc_ac_c* i_this) {
    cPhs__Step cVar1;
    bool bVar2;
    f32 fVar3;
    
    e_mm_mt_class* helmasaurShell = (e_mm_mt_class*) i_this;
    fopAc_ac_c* actor = (fopAc_ac_c*) i_this;

    fopAcM_ct(i_this, e_mm_mt_class);

    int phase = dComIfG_resLoad(&helmasaurShell->m_phase, "E_MM_MT");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_MM_MT PARAM %x\n", fopAcM_GetParam(i_this));
        helmasaurShell->parameters = fopAcM_GetParam(i_this);
        OS_REPORT("E_MM_MT//////////////E_MM_MT SET 1 !!\n");
        

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x850)) {
            OS_REPORT("//////////////E_MM_MT SET NON !!\n");
            return cPhs_ERROR_e;
        }
        OS_REPORT("//////////////E_MM_MT SET 2 !!\n");
        fopAcM_SetMtx(i_this, helmasaurShell->mp_model->getBaseTRMtx());
        fopAcM_SetMin(i_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(i_this, 200.0f, 200.0f, 200.0f);
        helmasaurShell->m_stts.Init(100, 0, i_this);
        if (i_this->argument == 1) {
            helmasaurShell->m_stts.SetWeight(0xFE);

        } else {
            helmasaurShell->m_stts.SetWeight(200);
        }

        i_this->health = 2000;
        i_this->field_0x560 = 2000;
        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0xd8fbfdff, 0x3}, 0x75}},  // mObj  
                {dCcD_SE_METAL, 0x0, 0x1, 0x0, 0x0},                // mGObjAt
                {dCcD_SE_NONE, 0x2, 0x0, 0x0, 0x407},                // mGObjTg
                {0x0},                                             // mGObjCo
            },                                                     // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f}  // m_sphere
            }  // m_sphAttr
        };


        helmasaurShell->m_sphere.Set(cc_sph_src);
        helmasaurShell->m_sphere.SetStts(&helmasaurShell->m_stts);
        if (i_this->argument == 1) {
            fopAcM_OffStatus(i_this, 0x80000);
            helmasaurShell->m_sphere.SetAtType(
                #if VERSION == VERSION_GCN_JPN
                AT_TYPE_CSTATUE_SWING
                #else
                0xD8FBBDFF
                #endif
            );
            helmasaurShell->m_stts.SetWeight(0xFE);
            helmasaurShell->m_sphere.SetAtAtp(2);
        } else {
            static dCcD_SrcSph hk_sph_src = {
                {
                    {0x0, {{0x0, 0x0, 0x0}, {0x4080, 0x3}, 0x0}},  // mObj  
                    {dCcD_SE_13, 0x0, 0x0, 0x0, 0x0},                // mGObjAt
                    {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x406},                // mGObjTg
                    {0x0},                                             // mGObjCo
                },                                                     // mObjInf
                {
                    {{0.0f, 0.0f, 0.0f}, 40.0f}  // m_sphere
                }  
            };
            helmasaurShell->m_stts.SetWeight(200);
            helmasaurShell->m_sph.Set(hk_sph_src);
            helmasaurShell->m_sph.SetStts(&helmasaurShell->m_stts);
        }
        helmasaurShell->m_acch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1,
                        &helmasaurShell->m_acchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);

        helmasaurShell->m_acchCir.SetWall(50.0f, 50.0f);
        helmasaurShell->m_sound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        helmasaurShell->m_atInfo.mpSound = &helmasaurShell->m_sound;
        helmasaurShell->m_atInfo.mPowerType = 9;
        i_this->attention_info.distances[fopAc_attn_CARRY_e] = 7;
        helmasaurShell->m_lifetime = cM_rndF(65535.0f);
        daE_MM_MT_Execute(helmasaurShell);
    } 
    return phase;
}

static actor_method_class l_daE_MM_MT_Method = {
    (process_method_func)daE_MM_MT_Create,
    (process_method_func)daE_MM_MT_Delete,
    (process_method_func)daE_MM_MT_Execute,
    (process_method_func)daE_MM_MT_IsDelete,
    (process_method_func)daE_MM_MT_Draw,
};

actor_process_profile_definition g_profile_E_MM_MT = {
  fpcLy_CURRENT_e,        // mLayerID
  8,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_MM_MT,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_mm_mt_class),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  168,                    // mPriority
  &l_daE_MM_MT_Method,    // sub_method
  0x000C0100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
