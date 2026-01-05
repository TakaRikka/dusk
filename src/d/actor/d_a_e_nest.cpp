/**
 * d_a_e_nest.cpp
 * Enemy - Beehive
 */

 #include "d/dolzel_rel.h" // IWYU pragma: keep
 
#include "d/d_s_play.h"
#include "d/actor/d_a_e_nest.h"
#include "d/actor/d_a_npc_tk.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_bomb.h"
#include "d/d_procname.h"
#include "f_op/f_op_camera_mng.h"

static bool hio_set;

static daE_Nest_HIO_c l_HIO;

daE_Nest_HIO_c::daE_Nest_HIO_c() {
    field_0x4 = -1;
    mScale = 1.0f;
}

static void hahen_draw(e_nest_class* i_this) {
    nest_hahen_s* debris = i_this->mDebris;
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    for (int i = 0; i < 5; i++, debris++) {
        if (debris->mActive) {
            g_env_light.setLightTevColorType_MAJI(debris->mpModel, &i_this->tevStr);
            mDoExt_modelUpdateDL(debris->mpModel);
        }
    }
}

static void* s_bomb_sub(void* i_actor, void* i_debris) {
    if (fopAcM_IsActor(i_actor)) {
        fopAc_ac_c* actor = static_cast<fopAc_ac_c*>(i_actor);
        if (dBomb_c::checkBombActor(actor)) {
            dBomb_c* bomb = static_cast<dBomb_c*>(actor);
            if (bomb->checkStateExplode()) {
                nest_hahen_s* debris = static_cast<nest_hahen_s*>(i_debris);
                cXyz delta = debris->mPos - bomb->current.pos;
                if (delta.abs() < 120.0f) {
                    return i_actor;
                }
            }
        }
    }
    return NULL;
}

static void hahen_normal(e_nest_class* i_this, nest_hahen_s* i_debris) {
    fopAc_ac_c* a_this = static_cast<fopAc_ac_c*>(i_this);
    dBgS_LinChk lin_chk;
    dBgS_ObjGndChk_Spl gnd_chk;
    
    i_debris->mCounter++;
    if (i_debris->mTimer != 0) {
        i_debris->mTimer--;
    }

    i_debris->mLastPos = i_debris->mPos;
    i_debris->mPos += i_debris->mSpeed;

    cXyz vec1 = i_debris->mPos;
    vec1.y += 500.0f;
    gnd_chk.SetPos(&vec1);
    f32 ground_cross = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (i_debris->mPos.y < ground_cross) {
        i_debris->mSpeed.y -= 0.5f;
        if (i_debris->mSpeed.y < -5.0f) {
            i_debris->mSpeed.y = -5.0f;
        }
        i_debris->mSpeed.x *= 0.95f;
        i_debris->mSpeed.z *= 0.95f;
    } else {
        i_debris->mSpeed.y -= 4.0f;
        if (i_debris->mSpeed.y < -50.0f) {
            i_debris->mSpeed.y = -50.0f;
        }
    }

    cXyz vec2;
    vec2.set(i_debris->mPos.x, i_debris->mPos.y + 100.0f, i_debris->mPos.z);
    if (fopAcM_gc_c::gndCheck(&vec2)) {
        if (i_debris->mSpeed.y <= 0.0f && i_debris->mPos.y <= fopAcM_gc_c::getGroundY() + 1.0f) {
            i_debris->mPos.y = fopAcM_gc_c::getGroundY() + 1.0f;
            if (i_debris->mSpeed.y < -7.0f && i_debris->mGroundHitStage == 0) {
                i_debris->mMaxRotationZ = cM_rndF(10.0f) + 15.0f;
                i_debris->mGroundHitStage++;
                vec2 = i_debris->mPos;
                vec2.y += 50.0f;
                vec2.z += 10.0f;
                if (fopAcM_gc_c::gndCheck(&vec2)) {
                    vec2.y = fopAcM_gc_c::getGroundY() + 1.0f;
                    i_debris->mGroundAngleX = -cM_atan2s(vec2.y - i_debris->mPos.y,
                                                      vec2.z - i_debris->mPos.z);
                    if (i_debris->mGroundAngleX > 0x3000 || i_debris->mGroundAngleX < -0x3000) {
                        i_debris->mGroundAngleX = 0;
                    }
                    vec2 = i_debris->mPos;
                    vec2.y += 50.0f;
                    vec2.x += 10.0f;
                    if (fopAcM_gc_c::gndCheck(&vec2)) {
                        vec2.y = fopAcM_gc_c::getGroundY() + 1.0f;
                        i_debris->mGroundAngleZ = cM_atan2s(vec2.y - i_debris->mPos.y,
                                                         vec2.x - i_debris->mPos.x);
                        if (i_debris->mGroundAngleZ > 0x3000 || i_debris->mGroundAngleZ < -0x3000) {
                            i_debris->mGroundAngleZ = 0;
                        }
                    }
                }
            } else {
                i_debris->mGroundHitStage = 3;
                i_debris->mSpeed.y = -1.0f;
                i_debris->mSpeed.z = 0.0f;
                i_debris->mSpeed.x = 0.0f;
                if (fpcM_Search(s_bomb_sub, i_debris) != NULL) {
                    i_debris->mActive = false;
                }
            }
            i_debris->mAngleZ = i_debris->mMaxRotationZ * 400.0f
                                * cM_ssin(i_debris->mCounter * 0x1700);
            cLib_addCalcAngleS2(&i_debris->mMaxRotationZ, 0, 1, 1);
        } else {
            if (i_debris->mPos.y < ground_cross) {
                i_debris->mAngleZ = cM_ssin(i_debris->mCounter * 0x500) * 5000.0f;
            } else {
                cLib_addCalcAngleS2(&i_debris->mAngleZ, 0, 4, 0x400);
            }
        }
    }

    if (i_debris->mTimer == 0) {
        cXyz delta_xz = i_debris->mPos - i_debris->mLastPos;
        delta_xz.y = 0.0f;
        if (delta_xz.abs() > 0.01f) {
            cXyz lin_end = i_debris->mPos + delta_xz * 2.0f;
            lin_chk.Set(&i_debris->mLastPos, &lin_end, a_this);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                i_debris->mPos = i_debris->mLastPos;
                i_debris->mSpeed.x *= -1.0f;
                i_debris->mSpeed.z *= -1.0f;
                i_debris->mSpeed.y = 0.0f;
                i_debris->mPos += i_debris->mSpeed;
                i_debris->mTimer = 5;
            }
        }
    }
}

static void hahen_move(e_nest_class* i_this) {
    nest_hahen_s* debris = i_this->mDebris;
    dBgS_LinChk lin_chk;

    for (int i = 0; i < 5; i++, debris++) {
        if (debris->mActive) {
            if (i_this->mBreakStage == 3 && debris->mActive == true) {
                hahen_normal(i_this, debris);
            }
            MtxTrans(debris->mPos.x, debris->mPos.y, debris->mPos.z, 0);
            mDoMtx_XrotM(*calc_mtx, debris->mGroundAngleX);
            mDoMtx_ZrotM(*calc_mtx, debris->mGroundAngleZ);
            mDoMtx_YrotM(*calc_mtx, debris->mAngleY + 0x4000);
            mDoMtx_ZrotM(*calc_mtx, debris->mAngleZ);
            MtxScale(debris->mScale.x, debris->mScale.y, debris->mScale.z, 1);
            debris->mpModel->setBaseTRMtx(*calc_mtx);
        }
    }

    if (i_this->mBreakStage == 2) {
        i_this->mBreakStage = 3;
    }
}

static int daE_Nest_Draw(e_nest_class* i_this) {
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    if (i_this->mBreakStage != 0) {
        hahen_draw(i_this);
        return 1;
    } else {
        g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &i_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mpModel);
        if (i_this->mAction == e_nest_class::ACT_DROP) {
            cXyz vec;
            f32 scale = l_HIO.mScale * i_this->scale.x;
            vec.set(i_this->current.pos.x, i_this->current.pos.y + 50.0f, i_this->current.pos.z);
            i_this->mShadowKey = 
                dComIfGd_setShadow(i_this->mShadowKey, 1, i_this->mpModel, &vec, scale * 500.0f,
                                   0.0f, i_this->current.pos.y, i_this->mAcch.GetGroundH(),
                                   i_this->mAcch.m_gnd, &i_this->tevStr, 0, 1.0f,
                                   dDlst_shadowControl_c::getSimpleTex());
        }
        return 1;
    }
}

static void mtx_cc_set(e_nest_class* i_this) {
    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->mRotation.y);
    mDoMtx_stack_c::XrotM(i_this->mRotation.x);
    mDoMtx_stack_c::ZrotM(i_this->mRotation.z);
    f32 scale = l_HIO.mScale * i_this->scale.x;
    mDoMtx_stack_c::scaleM(scale, scale, scale);
    mDoMtx_stack_c::transM(0.0f, i_this->mCarryOffset + i_this->mDropOffset, 0.0f);
    i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    cXyz vec;
    vec.x = 0.0f;
    vec.y = -90.0f;
    vec.z = 0.0f;
    mDoMtx_stack_c::multVec(&vec, &i_this->mCenterPos);

    i_this->mCcSph.SetC(i_this->mCenterPos);
    dComIfG_Ccsp()->Set(&i_this->mCcSph);
    i_this->mCcSph.SetR(scale * 40.0f);

    if (i_this->mAction == e_nest_class::ACT_NORMAL) {
        vec = i_this->current.pos;
        vec.y += -30.0f;
        i_this->mCcCyl.SetC(vec);
        i_this->mCcCyl.SetR(5.0f);
        i_this->mCcCyl.SetH(30.0f);
        dComIfG_Ccsp()->Set(&i_this->mCcCyl);
    }
}

static void e_nest_normal(e_nest_class* i_this) {
    fopAc_ac_c* a_this = static_cast<fopAc_ac_c*>(i_this);
    if (i_this->mHitTimer != 0) {
        i_this->mHitTimer--;
        i_this->mRotation.x = i_this->mHitTimer * cM_ssin(i_this->mHitTimer * 12000) * 50.0f;
        i_this->mRotation.z = i_this->mHitTimer * cM_ssin(i_this->mHitTimer * 14000) * 50.0f;
        if (i_this->mHitTimer == 0) {
            if (i_this->mKnockDown != 0) {
                i_this->mAction = e_nest_class::ACT_DROP;
                i_this->mMode = 0;
                i_this->mTimers[0] = 8;
                i_this->mTimers[2] = 120;
                i_this->mTimers[1] = 100;
                i_this->mDropOffset = 135.0f;
                a_this->current.pos.y -= i_this->mDropOffset * l_HIO.mScale * a_this->scale.x;
                if (i_this->mHawkHit) {
                    daNPC_TK_c* hawk =
                        static_cast<daNPC_TK_c*>(fopAcM_SearchByID(i_this->mHitActorID));
                    hawk->setHawkCamera(a_this);
                }
            } else {
                a_this->health = 1;
            }
        }
    } else {
        a_this->health = 0;
        if (i_this->mIframes == 0) {
            if (i_this->mCcCyl.ChkTgHit()) {
                i_this->mAtInfo.mpCollider = i_this->mCcCyl.GetTgHitObj();
                if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB)) {
                    a_this->field_0x567 = 1;
                    fopAcM_delete(a_this);
                } else {
                    i_this->mIframes = 10;
                    i_this->mHitTimer = 3;
                    i_this->mKnockDown = 1;
                    i_this->mHitActorID = fopAcM_GetID(daPy_getPlayerActorClass());
                }
            } else {
                int knock_down = false;

                if (i_this->mCcSph.ChkTgHit()) {
                    i_this->mIframes = 10;
                    i_this->mAtInfo.mpCollider = i_this->mCcSph.GetTgHitObj();
                    cc_at_check(a_this, &i_this->mAtInfo);
                    if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                        i_this->mAction = e_nest_class::ACT_HOOK;
                        i_this->mMode = 0;
                        i_this->mKnockDown = 0;
                        i_this->mHitActorID = fopAcM_GetID(daPy_getPlayerActorClass());
                    } else if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT) ||
                               i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_IRON_BALL)) {
                        i_this->mHitTimer = 10;
                        i_this->mAtInfo.mpCollider = i_this->mCcSph.GetTgHitObj();
                        knock_down = true;
                    } else {
                        i_this->mHitTimer = 10;
                        i_this->mAtInfo.mpCollider = i_this->mCcSph.GetTgHitObj();
                    }
                } else if (i_this->mCcSph.ChkCoHit()) {
                    i_this->mHitTimer = 5;
                    i_this->mAtInfo.mpCollider = i_this->mCcSph.GetCoHitObj();
                }

                if (i_this->mHitTimer != 0) {
                    fopAc_ac_c* actor = at_power_check(&i_this->mAtInfo);
                    if (knock_down) {
                        i_this->mIframes = 10;
                        if (!dComIfGp_event_runCheck()) {
                            i_this->mHitTimer = 20;
                            i_this->mKnockDown = 1;
                            i_this->mHitActorID = fopAcM_GetID(daPy_getPlayerActorClass());
                            if (!strcmp(dComIfGp_getStartStageName(), "F_SP103")) {
                                i_this->mDemoStage = 1;
                                /* dSv_event_flag_c::F_0084 - Ordon Village - Opening days 2&3: knocked down a beehive with slingshot */
                                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[183]);
                            }
                        }
                    } else if (i_this->mAtInfo.mHitType == HIT_TYPE_LINK_NORMAL_ATTACK
                            || i_this->mAtInfo.mHitType == HIT_TYPE_BOMB
                            || i_this->mAtInfo.mHitType == HIT_TYPE_ARROW
                            || i_this->mAtInfo.mHitType == HIT_TYPE_BOOMERANG
                            || i_this->mAtInfo.mHitType == HIT_TYPE_STUN) {
                        i_this->mHitActorID = fopAcM_GetID(daPy_getPlayerActorClass());
                        /* dSv_event_flag_c::F_0073 - Ordon Village - Attacked after charging at large beehive */
                        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[172]);
                    } else if (fopAcM_GetName(actor) == PROC_NPC_TK) {
                        daNPC_TK_c* hawk = static_cast<daNPC_TK_c*>(actor);
                        hawk->setBump();
                        i_this->mHitActorID = fopAcM_GetID(hawk);
                        i_this->mKnockDown = 1;
                        /* dSv_event_flag_c::F_0072 - Ordon Village - Knocked down large beehive with hawk */
                        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[171]);
                        i_this->mHitTimer = 20;
                        i_this->mHawkHit = true;
                        i_this->mSound.startSound(Z2SE_OBJ_HACHINOSU_HIT, 0, -1);
                    } else if (!strcmp(dComIfGp_getStartStageName(), "F_SP103")) {
                        i_this->mHitActorID = fopAcM_GetID(actor);
                    } else {
                        i_this->mHitTimer = 0;
                    }
                }
            }
        }
    }
}

static s16 wall_angle_get(e_nest_class* i_this) {
    fopAc_ac_c* a_this = static_cast<fopAc_ac_c*>(i_this);
    dBgS_LinChk lin_chk;
    cXyz vec1, vec2, vec3[2];
    mDoMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec1.z = a_this->scale.x * -10.0f;
    MtxPosition(&vec1, &vec2);
    vec2 += a_this->current.pos;
    vec1.x = 5.0f;
    vec1.y = 0.0f;
    vec1.z = a_this->scale.x * 200.0f;
    for (int i = 0; i < 2; i++) {
        MtxPosition(&vec1, &vec3[i]);
        vec1.x *= -1.0f;
        vec3[i] += vec2;
        lin_chk.Set(&vec2, &vec3[i], a_this);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            vec3[i] = lin_chk.GetCross();
        } else {
            return 0x23;
        }
    }
    vec1 = vec3[1] - vec3[0];
    return cM_atan2s(vec1.x, vec1.z) + 0x4000;
}

static void e_nest_drop(e_nest_class* i_this) {
    if (i_this->mAcch.ChkGroundHit()) {
        if (i_this->mKnockDown != 0) {
            i_this->field_0x567 = 1;
            i_this->health = 2;
            return;
        }
        if (i_this->mCcSph.ChkTgHit()) {
            i_this->field_0x567 = 2;
        }
        if (i_this->mMode < 3) {
            if (i_this->mMode == 0) {
                i_this->health = 2;
                i_this->mSound.startSound(Z2SE_OBJ_HACHINOSU_BOUND, 0, -1);
                i_this->speed.y = 20.0f;
                fopAcM_effSmokeSet1(&i_this->mSmokeKey1, &i_this->mSmokeKey2,
                                    &i_this->current.pos, &i_this->shape_angle,
                                    i_this->scale.x, &i_this->tevStr, 1);
                i_this->mWobble = 1000.0f;
                if (i_this->mSpin == 0) {
                    i_this->mSpin = cM_rndFX(1300.0f);
                }
            } else if (i_this->mMode == 1) {
                i_this->speed.y = 5.0f;
            } else if (i_this->mMode == 2) {
                i_this->speed.y = 2.0f;
            }
            i_this->mMode++;
            i_this->speedF *= 0.5f;
        } else {
            i_this->speedF = 0.0f;
        }
    }

    cXyz vec1, vec2;
    mDoMtx_YrotS(*calc_mtx, i_this->current.angle.y);
    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec1.z = i_this->speedF;
    MtxPosition(&vec1, &vec2);
    i_this->speed.x = vec2.x;
    i_this->speed.z = vec2.z;
    i_this->speed.y += -5.0f;
    i_this->current.pos += i_this->speed;
    if (i_this->speed.y < -80.0f) {
        i_this->speed.y = -80.0f;
    }

    if (i_this->mTimers[1] == 0 && i_this->mAcch.ChkWallHit()) {
        s16 wall_angle = wall_angle_get(i_this);
        if (wall_angle != 0x23) {
            s16 angle_delta = i_this->current.angle.y - wall_angle;
            i_this->mSpin = angle_delta * -0.3f;
            i_this->current.angle.y += (s16)(0x8000 - (angle_delta << 1));
            i_this->speedF *= 0.5f;
            i_this->mTimers[1] = 10;
            i_this->mSound.startSound(Z2SE_OBJ_HACHINOSU_BOUND, 0, -1);
        }
    }

    i_this->mDropOffset = 135.0f;
    i_this->mRotation.x = i_this->mWobble * cM_ssin(i_this->mFrame * 3000);
    i_this->mRotation.z = i_this->mWobble * cM_ssin(i_this->mFrame * 4000);
    cLib_addCalc0(&i_this->mWobble, 0.1f, 50.0f);
    i_this->mRotation.y += i_this->mSpin;
    cLib_addCalcAngleS2(&i_this->mSpin, 0, 1, 100);
}

static s8 e_nest_carry(e_nest_class* i_this) {
    fopAc_ac_c* a_this = static_cast<fopAc_ac_c*>(i_this);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    a_this->speed.y = 0.0f;
    s8 ret = true;
    
    switch (i_this->mMode) {
    case 0: {
        cLib_offBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
        i_this->mMode = 1;
        s16 angle_delta_y = i_this->mRotation.y - player->shape_angle.y;
        if (angle_delta_y > 0x4000 || angle_delta_y < -0x4000) {
            i_this->mCarryAngle = 0x8000;
        } else {
            i_this->mCarryAngle = 0;
        }
        i_this->mTimers[0] = 20;
        break;
    }

    case 1:
        if (!fopAcM_checkCarryNow(a_this)) {
            i_this->mTimers[0] = 0;
            i_this->mCcSph.OnCoSetBit();
            i_this->mAction = e_nest_class::ACT_DROP;
            if (fopAcM_GetSpeedF(a_this) >= 1.0f) {
                a_this->speedF = 30.0f + NREG_F(7);
                a_this->speed.y = 30.0f + NREG_F(8);
                a_this->current.angle.y = player->shape_angle.y;
                i_this->mMode = 0;
            } else {
                a_this->speedF = 0.0f;
                i_this->mMode = 3;
                i_this->mWobble = 500.0f + NREG_F(0);
            }
        } else {
            ret = false;
        }
        break;
    }

    cLib_addCalcAngleS2(&i_this->mRotation.y, player->shape_angle.y + i_this->mCarryAngle,
                        1, i_this->mCarryAngleSpeed);
    cLib_addCalcAngleS2(&i_this->mCarryAngleSpeed, 0x2000, 1, 0x100);
    return ret;
}

static int water_check(e_nest_class* i_this) {
    dBgS_LinChk lin_chk;
    cXyz vec;
    vec.x = i_this->current.pos.x;
    vec.y = i_this->current.pos.y + 3000.0f;
    vec.z = i_this->current.pos.z;
    lin_chk.Set(&i_this->current.pos, &vec, i_this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        vec.y = lin_chk.GetCross().y - 10.0f;
    }
    dBgS_ObjGndChk_Spl gnd_chk;
    gnd_chk.SetPos(&vec);
    i_this->mWaterHeight = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (i_this->mWaterHeight > i_this->current.pos.y + 50.0f) {
        return true;
    } else {
        return false;
    }
}

static int simple_bg_check(e_nest_class* i_this) {
    fopAc_ac_c* a_this = static_cast<fopAc_ac_c*>(i_this);
    cXyz vec2, vec3, vec1;
    vec1 = a_this->current.pos - a_this->old.pos;
    vec1.y = 0.0f;
    if (vec1.abs() > 0.0f) {
        s16 angle_y = cM_atan2s(vec1.x, vec1.z);
        vec2 = a_this->current.pos;
        vec2.y = i_this->mWaterHeight + 3.0f;
        vec1.y = 0.0f;
        mDoMtx_YrotS(*calc_mtx, angle_y);
        static f32 c_x[3] = {50.0f, -50.0f, 0.0f};
        static f32 c_z[3] = {0.0f, 0.0f, 50.0f};
        for (int i = 0; i < 3; i++) {
            vec1.x = c_x[i] * l_HIO.mScale * a_this->scale.x;
            vec1.z = c_z[i] * l_HIO.mScale * a_this->scale.x;
            MtxPosition(&vec1, &vec3);
            vec3 += vec2;
            dBgS_LinChk lin_chk;
            lin_chk.Set(&vec2, &vec3, a_this);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                return true;
            }
        }
    }
    return false;
}

static void e_nest_float(e_nest_class* i_this) {
    cLib_addCalc0(&i_this->speed.y, 1.0f, 0.25f);
    i_this->current.pos += i_this->speed;
    i_this->speed.x *= 0.95f;
    i_this->speed.z *= 0.95f;

    cXyz vec = i_this->current.pos;
    vec.y = i_this->mWaterHeight;
    if (i_this->current.pos.y + 60.0f > vec.y) {
        fopAcM_effHamonSet(&i_this->mRippleKey, &vec, i_this->scale.x, 0.05f);
    }
    
    cLib_addCalc2(&i_this->current.pos.y, i_this->mWaterHeight - 40.0f +
                  i_this->mBob * 0.004f * cM_ssin(i_this->mFrame * 900), 0.1f, 10.0f);
    i_this->mRotation.x = i_this->mBob * cM_ssin(i_this->mFrame * 1000);
    i_this->mRotation.z = i_this->mBob * cM_ssin(i_this->mFrame * 1100);
    cLib_addCalc2(&i_this->mBob, 500.0f, 0.1f, 30.0f);
    i_this->mRotation.y += i_this->mSpin;

    if (i_this->mCcSph.ChkCoHit()) {
        cXyz* move_p = i_this->mCcStts.GetCCMoveP();
        if (move_p != NULL) {
            cLib_addCalc2(&i_this->speed.x, move_p->x * 0.5f, 0.1f, 1.0f);
            cLib_addCalc2(&i_this->speed.z, move_p->z * 0.5f, 0.1f, 1.0f);
            cLib_addCalc2(&i_this->mBob, 1000.0f, 0.1f, 100.0f);
        }
    }

    cLib_addCalcAngleS2(&i_this->mSpin, 0, 1, 150);

    if (simple_bg_check(i_this)) {
        i_this->current.pos = i_this->old.pos;
    }

    if (i_this->mAcch.ChkGroundHit()) {
        i_this->mAction = e_nest_class::ACT_DROP;
        i_this->speedF = 0.0f;
        i_this->mMode = 3;
    }

    if (i_this->mCcSph.ChkTgHit()) {
        i_this->field_0x567 = 2;
    }
}

static void e_nest_hahen(e_nest_class* i_this) {
    fopAc_ac_c* a_this = static_cast<fopAc_ac_c*>(i_this);
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));

    switch (i_this->mMode) {
    case 0:
        i_this->mTimers[0] = 10;
        i_this->mMode = 1;
        // no break

    case 1:
        if (i_this->mTimers[0] == 1 && i_this->mHawkHit) {
            daNPC_TK_c* hawk = static_cast<daNPC_TK_c*>(fopAcM_SearchByID(i_this->mHitActorID));
            hawk->setHawkSideCamera(a_this->current.pos);
        }

        for (int i = 0; i < 5; i++) {
            if (i_this->mDebris[i].mActive
                                && i_this->mDebris[i].mPos.y > i_this->mWaterHeight - 10.0f) {
                cXyz player_to_debris = i_this->mDebris[i].mPos - player->current.pos;
                if (player_to_debris.abs() < 100.0f) {
                    a_this->current.pos = i_this->mDebris[i].mPos;
                    a_this->attention_info.position = a_this->current.pos;
                    a_this->eyePos = a_this->attention_info.position;
                    if (a_this->eventInfo.checkCommandCatch()) {
                        i_this->mMode = 2;
                        i_this->mTimers[0] = 60;
                    } else {
                        dComIfGp_att_CatchRequest(a_this, 0x76, 100.0f, 50.0f, -150.0f, 0x5000, 1);
                        a_this->eventInfo.onCondition(dEvtCnd_40_e);
                    }
                    break;
                }
            }
        }
        break;

    case 2:
        if (i_this->mTimers[0] == 0) {
            i_this->mMode = 1;
        }
        break;
    }

    if (fopAcM_searchPlayerDistance(a_this) < 230.0f) {
        player->setLookPos(&a_this->current.pos);
    }

    if (!strcmp(dComIfGp_getStartStageName(), "F_SP103") && i_this->mTimers[2] == 70) {
        mDoAud_seStart(Z2SE_READ_RIDDLE_B, 0, 0, 0);
    }

    if (i_this->mTimers[2] == 1 && i_this->mHawkHit) {
        daNPC_TK_c* hawk = static_cast<daNPC_TK_c*>(fopAcM_SearchByID(i_this->mHitActorID));
        if (hawk != NULL) {
            hawk->endHawkCamera();
        }
    }

    cXyz gnd_chk_pos = a_this->current.pos;
    gnd_chk_pos.y += 200.0f;
    if (fopAcM_gc_c::gndCheck(&gnd_chk_pos)) {
        a_this->current.pos.y = fopAcM_gc_c::getGroundY();
    }
}

static void e_nest_hook(e_nest_class* i_this) {
    i_this->mIframes = 10;

    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        i_this->speedF = 0.0f;
        i_this->mTimers[0] = 3;
        // no break

    case 1:
    default:
        if (i_this->mTimers[0] == 0 && !fopAcM_checkHookCarryNow(i_this)) {
            i_this->mAction = e_nest_class::ACT_DROP;
            i_this->mMode = 0;
            i_this->mDropOffset = 135.0f;
            i_this->current.pos.y -= i_this->mDropOffset * l_HIO.mScale * i_this->scale.x;
            fopAcM_OffStatus(i_this, 0x80000);
        }
        break;
    }
}

static void bee_nest_action(e_nest_class* i_this) {
    i_this->attention_info.position = i_this->eyePos = i_this->current.pos;
    i_this->eyePos.y += i_this->scale.x * -80.0f;

    s8 bvar7 = false;
    s8 float_check = false;
    f32 carry_offset = 0.0f;
    s8 carry_check = false;

    if (i_this->mIframes != 0) {
        i_this->mIframes--;
    }

    i_this->mCcStts.Move();

    switch (i_this->mAction) {
    case e_nest_class::ACT_NORMAL:
        e_nest_normal(i_this);
        break;
    case e_nest_class::ACT_DROP:
        e_nest_drop(i_this);
        carry_check = true;
        if (i_this->mTimers[0] == 0) {
            bvar7 = true;
        }
        float_check = true;
        break;
    case e_nest_class::ACT_HOOK:
        e_nest_hook(i_this);
        break;
    case e_nest_class::ACT_CARRY:
        bvar7 = e_nest_carry(i_this);
        carry_offset = 15.0f;
        break;
    case e_nest_class::ACT_FLOAT:
        e_nest_float(i_this);
        bvar7 = true;
        carry_check = true;
        break;
    case e_nest_class::ACT_DEBRIS:
        e_nest_hahen(i_this);
        cLib_offBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
        return;
    }

    cLib_addCalc2(&i_this->mCarryOffset, carry_offset, 1.0f, 5.0f);

    if (bvar7) {
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
    }

    if (carry_check) {
        cLib_onBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
        i_this->attention_info.distances[fopAc_attn_CARRY_e] = 0x2c;
        if (fopAcM_checkCarryNow(i_this)) {
            i_this->mAction = e_nest_class::ACT_CARRY;
            i_this->mMode = 0;
            i_this->mCcSph.OffCoSetBit();
        }
    } else {
        cLib_offBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }

    if (float_check && water_check(i_this)) {
        i_this->mAction = e_nest_class::ACT_FLOAT;
        i_this->mMode = 0;
        i_this->health = 2;
        if (i_this->speedF < 5.0f) {
            mDoMtx_YrotS(*calc_mtx, i_this->current.angle.y);
            cXyz vec;
            vec.x = 0.0f;
            vec.y = -2.0f;
            vec.z = 2.5f;
            MtxPosition(&vec, &i_this->speed);
        } else {
            i_this->current.pos.y = i_this->mWaterHeight;
            i_this->speed.y = -8.0f;
            i_this->speed.x *= 0.5f;
            i_this->speed.z *= 0.5f;
            i_this->mBob = 2500.0f;
        }
        i_this->speedF = 0.0f;
        cXyz vec1(i_this->scale.x, i_this->scale.x, i_this->scale.x);
        cXyz vec2 = i_this->current.pos;
        vec2.y = i_this->mWaterHeight;
        static u16 w_eff_id[] = {0x01B8, 0x01B9, 0x01BA, 0x01BB};
        for (int i = 0; i < 4; i++) {
            i_this->mParticleKey[i] =
                dComIfGp_particle_set(i_this->mParticleKey[i], w_eff_id[i], &vec2, &i_this->tevStr,
                                      NULL, &vec1, -1, NULL, -1, NULL, NULL, NULL);
        }
        i_this->mSound.startSound(Z2SE_AL_INTO_WATER, 0, -1);
    }

    if (i_this->field_0x567 != 0) {
        i_this->mSound.startSound(Z2SE_OBJ_HACHINOSU_BREAK, 0, -1);
        cXyz vec1 = i_this->current.pos;
        f32 scale = i_this->scale.x * 0.7f;
        cXyz vec2(scale, scale, scale);
        static u16 b_eff_id[] = {0x832C, 0x832D};
        for (int i = 0; i < 2; i++) {
            dComIfGp_particle_set(b_eff_id[i], &vec1, &i_this->tevStr, &i_this->shape_angle, &vec2);
        }
        i_this->mBreakStage = 1;
        if (i_this->field_0x567 == 1) {
            dComIfGs_onSwitch(i_this->mBrokenSwitch, fopAcM_GetRoomNo(i_this));
        }
        i_this->field_0x567 = 0;
    }

    mtx_cc_set(i_this);
}

static void* shot_b_sub(void* i_actor, void* i_data) {
    daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_BOOMERANG
                    && !dComIfGp_checkPlayerStatus0(0, 0x80000) && player->checkBoomerangCharge()
                    && fopAcM_GetParam(i_actor) == 1) {
        return i_actor;
    }
    return NULL;
}

static void demo_camera(e_nest_class* i_this) {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    s8 end_demo = false;
    
    switch (i_this->mDemoStage) {
    case 0:
        break;

    case 1:
        if (!i_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(i_this, 2, -1, 0);
            i_this->eventInfo.onCondition(2);
            return;
        }
        camera->mCamera.Stop();
        i_this->mDemoStage = 2;
        i_this->mDemoTimer = 0;
        i_this->mDemoCameraFovY = 55.0f;
        camera->mCamera.SetTrimSize(3);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
        i_this->mDemoCameraCenter.x = i_this->mCenterPos.x;
        i_this->mDemoCameraCenter.z = i_this->mCenterPos.z;
        i_this->mDemoCameraCenter.y = i_this->mCenterPos.y - 60.0f;
        i_this->mDemoCameraEye.set(-1290.0f, 1342.0f, 1077.0f);
        // no break

    case 2:
        cLib_addCalc2(&i_this->mDemoCameraCenter.x, i_this->mCenterPos.x, 0.2f, 100.0f);
        cLib_addCalc2(&i_this->mDemoCameraCenter.y, i_this->mCenterPos.y - 60.0f, 0.2f, 100.0f);
        cLib_addCalc2(&i_this->mDemoCameraCenter.z, i_this->mCenterPos.z, 0.2f, 100.0f);
        if (i_this->mDemoTimer > 60) {
            cLib_addCalc2(&i_this->mDemoCameraFovY, 40.0f, 0.15f, 0.2f);
        }
        if (i_this->mDemoTimer == 120) {
            end_demo = true;
        }
        break;
    }

    if (end_demo) {
        camera->mCamera.Reset(i_this->mDemoCameraCenter, i_this->mDemoCameraEye);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->mDemoStage = 0;
    }

    if (i_this->mDemoStage != 0) {
        camera->mCamera.Set(i_this->mDemoCameraCenter, i_this->mDemoCameraEye,
                            i_this->mDemoCameraFovY, 0);
        i_this->mDemoTimer++;
    }
}

static int daE_Nest_Execute(e_nest_class* i_this) {
    cXyz vec1, vec2;

    i_this->mFrame++;
    for (int i = 0; i < 3; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));

    if (i_this->mBreakStage == 3) {
        fopAc_ac_c* boomerang = (fopAc_ac_c*)fpcM_Search(shot_b_sub, i_this);
        if (boomerang != NULL) {
            cXyz delta = boomerang->current.pos - i_this->current.pos;
            if (delta.abs() < 200.0f) {
                nest_hahen_s* debris = i_this->mDebris;
                for (int i = 0; i < 5; i++, debris++) {
                    if (debris->mSpeed.y <= 0.0f) {
                        debris->mActive = true;
                        debris->mAngleZ = cM_rndFX(10000.0f);
                        mDoMtx_YrotS(*calc_mtx, debris->mAngleY);
                        vec1.x = 0.0f;
                        vec1.y = i_this->scale.x * 20.0f;
                        vec1.z = 0.0f;
                        MtxPosition(&vec1, &vec2);
                        debris->mPos += vec2;
                        vec1.x = 0.0f;
                        vec1.y = i_this->scale.x * (cM_rndF(5.0f) + 20.0f);
                        vec1.z = 0.0f;
                        MtxPosition(&vec1, &debris->mSpeed);
                        debris->mGroundHitStage = 0;
                    }
                }
            }
        }
    }

    if (i_this->mBreakStage == 1) {
        i_this->mBreakStage = 2;
        i_this->mAction = e_nest_class::ACT_DEBRIS;
        i_this->mMode = 0;
        fopAcM_OffStatus(i_this, fopAcStts_CULL_e);
        nest_hahen_s* debris = i_this->mDebris;
        for (int i = 0; i < 5; i++, debris++) {
            debris->mActive = true;
            float scale = i_this->scale.x * (cM_rndF(0.2f) + 0.7f);
            debris->mScale.x = scale;
            debris->mScale.z = scale;
            debris->mScale.y = i_this->scale.x * 0.7f;
            debris->mPos = i_this->current.pos;
            debris->mAngleY = i * 0x3333;
            debris->mAngleZ = -0x4000;
            mDoMtx_YrotS(*calc_mtx, debris->mAngleY);
            vec1.x = 0.0f;
            vec1.y = i_this->scale.x * 20.0f;
            vec1.z = i_this->scale.x * 25.0f;
            MtxPosition(&vec1, &vec2);
            debris->mPos += vec2;
            vec1.x = 0.0f;
            vec1.y = i_this->scale.x * (cM_rndF(3.0f) + 10.0f);
            vec1.z = i_this->scale.x * (cM_rndF(1.0f) + 2.0f);
            MtxPosition(&vec1, &debris->mSpeed);
            debris->mGroundHitStage = 0;
            debris->mCounter = cM_rndF(0x10000);
        }
    }

    hahen_move(i_this);
    if (i_this->mParamType == e_nest_class::TYPE_BEE) {
        bee_nest_action(i_this);
    }
    demo_camera(i_this);
    return 1;
}

static int daE_Nest_IsDelete(e_nest_class* param_0) {
    return 1;
}

static int daE_Nest_Delete(e_nest_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "E_nest");
    
    if (i_this->mHIOInit) {
        hio_set = false;
    }

    if (i_this->heap != NULL) {
        i_this->mSound.deleteObject();
        i_this->mBeeSound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_nest_class* _this = static_cast<e_nest_class*>(i_this);

    J3DModelData* model_data = static_cast<J3DModelData*>(dComIfG_getObjectRes("E_nest", 3));
    _this->mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (_this->mpModel == NULL) {
        return 0;
    }
    _this->model = _this->mpModel;

    J3DModelData* debris_model_data = static_cast<J3DModelData*>(dComIfG_getObjectRes("E_nest", 4));
    for (int i = 0; i < 5; i++) {
        _this->mDebris[i].mpModel = mDoExt_J3DModel__create(debris_model_data, 0x80000, 0x11000084);
        if (_this->mDebris[i].mpModel == NULL) {
            return 0;
        }
    }

    _this->mSound.init(&_this->current.pos, 1);
    _this->mBeeSound.init(&_this->current.pos, 2);
    
    return 1;
}

static cPhs__Step daE_Nest_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, e_nest_class);
    e_nest_class* _this = static_cast<e_nest_class*>(i_this);

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&_this->mPhase, "E_nest");
    if (step == cPhs_COMPLEATE_e) {
        _this->mBrokenSwitch = (u8)((u16)_this->current.angle.z >> 8);
        _this->shape_angle.z = 0;
        _this->current.angle.z = 0;
        _this->mParamType = (u8)fopAcM_GetParam(_this);
        _this->mParamType = e_nest_class::TYPE_BEE;
        _this->mParam1 = (u8)(fopAcM_GetParam(_this) >> 8);
        _this->mParamScale = (u8)(fopAcM_GetParam(_this) >> 16);
        if (_this->mParamScale == 0xff) {
            _this->mParamScale = 0;
        }

        if (!fopAcM_entrySolidHeap(_this, useHeapInit, 0x3000)) {
            return cPhs_ERROR_e;
        }

        if (!hio_set) {
            _this->mHIOInit = true;
            hio_set = true;
            l_HIO.field_0x4 = -1;
        }

        fopAcM_SetMtx(_this, _this->mpModel->getBaseTRMtx());
        fopAcM_SetMin(_this, -300.0f, -300.0f, -300.0f);
        fopAcM_SetMax(_this, 300.0f, 300.0f, 300.0f);

        if (_this->mParamScale == 0xff || _this->mParamScale == 0) {
            _this->mParamScale = 10;
        }
        _this->scale.x = _this->mParamScale * 0.1f;
        
        _this->mRotation.y = _this->current.angle.y;

        if (_this->mBrokenSwitch != 0xff
            && dComIfGs_isSwitch(_this->mBrokenSwitch, fopAcM_GetRoomNo(_this)))
        {
            cXyz pos = _this->current.pos;
            pos.y -= 100.0f;
            if (fopAcM_gc_c::gndCheck(&pos)) {
                pos.y = fopAcM_gc_c::mGroundY;
            }
            _this->mBreakStage = 1;
            _this->current.pos = pos;
            _this->mAction = e_nest_class::ACT_DEBRIS;
            _this->mMode = 1;
            fopAcM_SetMtx(_this, _this->mDebris[0].mpModel->getBaseTRMtx());
            return step;
        }

        _this->mCcStts.Init(100, 0, _this);
        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x1}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 30.0f} // mCcSph
            } // mCcSphAttr
        };
        _this->mCcSph.Set(cc_sph_src);
        _this->mCcSph.SetStts(&_this->mCcStts);
        static dCcD_SrcCyl cc_cyl_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0x2020, 0x3}, 0x79}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {
                    {0.0f, 0.0f, 0.0f}, // mCenter
                    5.0f, // mRadius
                    10.0f // mHeight
                } // mCcCyl
            }
        };
        _this->mCcCyl.Set(cc_cyl_src);
        _this->mCcCyl.SetStts(&_this->mCcStts);

        if (_this->mParamType == e_nest_class::TYPE_BEE) {
            fopAcM_createChild(PROC_E_BEE, fopAcM_GetID(_this),
                               _this->mParamScale << 16 | 0x100 | _this->mParam1,
                               &_this->current.pos, fopAcM_GetRoomNo(_this),
                               NULL, NULL, -1, NULL);
            _this->mAcch.Set(&_this->current.pos, &_this->old.pos, _this, 1,
                             &_this->mAcchCir, &_this->speed, NULL, NULL);
            _this->mAcchCir.SetWall(_this->scale.x * 50.0f, _this->scale.x * 50.0f);
        } else if (_this->mParamType == e_nest_class::TYPE_BUG) {
            fopAcM_createChild(PROC_E_BUG, fopAcM_GetID(_this),
                               _this->mParam1 | 0xff000000, &_this->current.pos,
                               fopAcM_GetRoomNo(_this), NULL, NULL, -1, NULL);
        }
        
        mtx_cc_set(_this);
        _this->mHitActorID = -1;
    }
    return step;
}

static actor_method_class l_daE_Nest_Method = {
    (process_method_func)daE_Nest_Create,
    (process_method_func)daE_Nest_Delete,
    (process_method_func)daE_Nest_Execute,
    (process_method_func)daE_Nest_IsDelete,
    (process_method_func)daE_Nest_Draw,
};

actor_process_profile_definition g_profile_E_NEST = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_E_NEST,
    &g_fpcLf_Method.base,
    sizeof(e_nest_class),
    0,
    0,
    &g_fopAc_Method.base,
    0xB2,
    &l_daE_Nest_Method,
    0xD4100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
