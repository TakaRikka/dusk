/**
 * d_a_e_arrow.cpp
 * Enemy Arrow
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_arrow.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_alink.h"
#include "d/d_bomb.h"
#include "d/d_item.h"
#include "d/d_meter2.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "d/d_s_play.h"
#include "Z2AudioLib/Z2Instances.h"
#include "JSystem/JAudio2/JAUSectionHeap.h"

static int daE_ARROW_Draw(e_arrow_class* i_this) {
    if (i_this->field_0xa14) {
        return 1;
    }

    if (fopAcM_GetParam(i_this) & 0x80000000) {
        g_env_light.settingTevStruct(2, &i_this->current.pos, &i_this->tevStr);
    } else {
        g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    }

    if (i_this->mpModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &i_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mpModel);
    }

    return 1;
}

static u8 const lit_3983[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static void fire_eff_set(e_arrow_class* i_this) {
    cXyz sp38;
    cXyz sp44;

    cMtx_YrotS(*calc_mtx, i_this->shape_angle.y);
    cMtx_XrotM(*calc_mtx, i_this->shape_angle.x);

    sp38.x = 0.0f;
    sp38.y = 0.0f;

    if (i_this->mAction == ACTION_ARROW_BOUND) {
        sp38.z = TREG_F(14) + 28.0f;
    } else {
        sp38.z = TREG_F(14) + 20.0f;
    }

    MtxPosition(&sp38, &sp44);
    sp44 += i_this->current.pos;

    int emitter_num;
    u16 particle_names[2];
    if (i_this->mArrowType == ARROW_TYPE_BOMB) {
        emitter_num = 2;
        particle_names[0] = 0x86A5;
        particle_names[1] = 0x86A6;
    } else {
        emitter_num = 1;
        particle_names[0] = 0x8113;
    }

    i_this->field_0x5d8.x = i_this->speed.x + i_this->field_0x5d0;
    i_this->field_0x5d8.y = i_this->speed.y;
    i_this->field_0x5d8.z = i_this->speed.z + i_this->field_0x5d4;

    if (i_this->mArrowType == ARROW_TYPE_FIRE) {
        i_this->field_0x5d8 *= 0.9f;
    } else {
        i_this->field_0x5d8 *= 0.8f;
    }

    for (int i = 0; i < emitter_num; i++) {
        i_this->mFireEMKeys[i] = dComIfGp_particle_set(i_this->mFireEMKeys[i], particle_names[i],
                                                       &sp44, &i_this->shape_angle, NULL);

        if (i == 0) {
            JPABaseEmitter* emitter_p = dComIfGp_particle_getEmitter(i_this->mFireEMKeys[i]);
            if (emitter_p != NULL) {
                if (i_this->field_0x5d8.abs() > 1.0f) {
                    emitter_p->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
                    emitter_p->setUserWork((uintptr_t)&i_this->field_0x5d8);
                } else {
                    emitter_p->setParticleCallBackPtr(NULL);
                }
            }
        }
    }
}

static void hit_check(e_arrow_class* i_this) {
    dBgS_ObjGndChk_Spl ground_chk;

    cXyz gnd_chk_pos(i_this->current.pos);
    gnd_chk_pos.y += 200.0f;
    ground_chk.SetPos(&gnd_chk_pos);

    if (i_this->current.pos.y <= dComIfG_Bgsp().GroundCross(&ground_chk)) {
        i_this->speedF = 0.0f;
        i_this->speed.set(0.0f, 0.0f, 0.0f);
        fopAcM_delete(i_this);
        return;
    }

    dBgS_ArrowLinChk line_chk;
    cXyz line_chk_start(i_this->current.pos);
    for (int i = 0; i < 2; i++) {
        line_chk_start -= i_this->speed;
    }

    line_chk.Set(&line_chk_start, &i_this->current.pos, i_this);

    if (dComIfG_Bgsp().LineCross(&line_chk)) {
        i_this->current.pos = line_chk.GetCross();
        dComIfG_Bgsp().ArrowStickCallBack(line_chk, i_this, i_this->current.pos);

        if (i_this->mFlags & 0x10) {
            i_this->mAction = ACTION_ARROW_FIRE;
            i_this->current.pos.x = -93652.0f;
            i_this->current.pos.y = -5975.0f;
        } else {
            i_this->mAction = ACTION_ARROW_BG;
            i_this->current.pos -= i_this->speed * 0.35f;
        }

        i_this->mMode = 0;
        i_this->speedF = 0.0f;
        i_this->speed.set(0.0f, 0.0f, 0.0f);
        i_this->field_0x5d4 = 0.0f;
        i_this->field_0x5d0 = 0.0f;

        dKy_Sound_set(i_this->current.pos, 3, fopAcM_GetID(i_this), 10);

        int poly_att0 = dComIfG_Bgsp().GetPolyAtt0(line_chk);
        dKy_pol_sound_get(&line_chk);

        i_this->field_0x5ce = 5;
        if (poly_att0 == 5) {
            i_this->field_0x5ce = 20;
        }

        i_this->mSound.startCollisionSE(Z2SE_HIT_ARROW_STICK, dKy_pol_sound_get(&line_chk), NULL);

        cXyz sp114(0.3f, 0.3f, 0.3f);
        i_this->mStickSmokeEMKey = dComIfGp_particle_set(
            i_this->mStickSmokeEMKey, ID_ZI_J_DASHSMOKE_A, &i_this->current.pos,
            &i_this->tevStr, &i_this->shape_angle, &sp114, 0xFF, NULL, -1, NULL, NULL, NULL);
    } else {
        cXyz sp120 = i_this->current.pos + (i_this->speed * 0.5f);
        i_this->mCcTgSph.SetC(sp120);
        dComIfG_Ccsp()->Set(&i_this->mCcTgSph);

        if (i_this->mCcAtSph.ChkAtShieldHit() && i_this->mCcAtSph.GetAtHitAc() != NULL &&
            fopAcM_GetName(i_this->mCcAtSph.GetAtHitAc()) == PROC_ALINK &&
            daAlink_getAlinkActorClass()->checkWoodShieldEquipNotIronBall())
        {
            i_this->mAction = ACTION_ARROW_SHIELD;
            i_this->mMode = 0;
            i_this->speedF = 0.0f;

            BOOL var_r26;
            if (i_this->mArrowType == ARROW_TYPE_FIRE) {
                var_r26 = 1;
                JPABaseEmitter* emitter_p = dComIfGp_particle_getEmitter(i_this->mFireEMKeys[0]);
                if (emitter_p != NULL) {
                    emitter_p->deleteAllParticle();
                    dComIfGp_particle_levelEmitterOnEventMove(i_this->mFireEMKeys[0]);
                }
            } else {
                var_r26 = 0;
            }

            i_this->mArrowType = ARROW_TYPE_NORMAL;
            i_this->field_0x5ce = 0;

            daAlink_getAlinkActorClass()->setArrowShieldActor(i_this, var_r26);
            daAlink_getAlinkActorClass()->setArrowShieldPos(
                &i_this->current.pos, &i_this->shape_angle, &i_this->field_0x9d4,
                &i_this->field_0x9c8);

            static cXyz localOffset(0.0f, 0.0f, -30.0f);
            mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y,
                                   i_this->current.pos.z);
            mDoMtx_stack_c::ZXYrotM(i_this->shape_angle.x, i_this->shape_angle.y, 0);
            mDoMtx_stack_c::multVec(&localOffset, &i_this->current.pos);
        } else if (i_this->mCcTgSph.ChkTgHit() || i_this->mCcAtSph.ChkAtShieldHit()) {
            i_this->mAction = ACTION_ARROW_SPIN;
            i_this->mMode = 0;
            i_this->current.angle.x = cM_rndFX(2000.0f) - 10000.0f;
            i_this->speedF *= 0.3f;

            if (i_this->mCcTgSph.ChkTgHit()) {
                i_this->current.angle.y += (s16)(cM_rndFX(8000.0f) + 32768.0f);
                i_this->mSound.startSound(Z2SE_COL_FLIP_ARROW, 0, -1);
            } else {
                i_this->current.angle.y += (s16)(cM_rndFX(4000.0f) + 32768.0f);
            }

            dKy_Sound_set(i_this->current.pos, 3, fopAcM_GetID(i_this), 10);
        } else {
            i_this->mCcAtSph.MoveCAt(sp120);
            dComIfG_Ccsp()->Set(&i_this->mCcAtSph);

            if (i_this->mCcAtSph.ChkAtHit()) {
                cXyz sp12C(0.4f, 0.4f, 0.4f);
                dComIfGp_setHitMark(1, i_this, &i_this->current.pos, &i_this->shape_angle, &sp12C,
                                      0);

                fopAcM_delete(i_this);

                if (i_this->mArrowType == ARROW_TYPE_FIRE) {
                    i_this->mArrowType = ARROW_TYPE_NORMAL;

                    JPABaseEmitter* emitter_p = dComIfGp_particle_getEmitter(i_this->field_0x9ec);
                    if (emitter_p != NULL) {
                        emitter_p->stopDrawParticle();
                    }
                }
            }
        }
    }
}

static void e_arrow_shot(e_arrow_class* i_this) {
    switch (i_this->mMode) {
    case 0: {
        i_this->speedF = 100.0f;
        i_this->mMode = 1;

        if (i_this->mFlags & 0x10) {
            i_this->mTimers[0] = 10000;
        } else {
            i_this->mTimers[0] = 100;
        }

        i_this->shape_angle.z = cM_rndF(0xFFFF);

        fopAc_ac_c* parent_p = fopAcM_SearchByID(i_this->parentActorID);
        if (parent_p != NULL) {
            i_this->field_0x5d0 = parent_p->speed.x;
            i_this->field_0x5d4 = parent_p->speed.z;
        }
    }
    case 1:
        if (i_this->mFlags & 0x20) {
            fopAc_ac_c* coach_p = fopAcM_SearchByName(PROC_NPC_COACH);

            if (coach_p != NULL) {
                cXyz sp30;
                sp30 = coach_p->eyePos - i_this->current.pos;

                s16 temp_r4 = cM_atan2s(sp30.x, sp30.z);
                s16 temp_r0 = temp_r4 - i_this->current.angle.y;
                if (temp_r0 < 0x1000 && temp_r0 > -0x1000) {
                    cLib_addCalcAngleS2(&i_this->current.angle.y, temp_r4, 2, 25);
                    f32 sq = JMAFastSqrt(sp30.x * sp30.x + sp30.z * sp30.z);

                    cLib_addCalcAngleS2(&i_this->current.angle.x, -cM_atan2s(sp30.y, sq), 2, 25);
                } else {
                    i_this->current.angle.x += 65;
                }
            } else {
                i_this->current.angle.x += 65;
            }
        } else {
            i_this->current.angle.x += 65;
        }

        i_this->shape_angle.y = i_this->current.angle.y;
        i_this->shape_angle.x = i_this->current.angle.x;

        if (i_this->mTimers[0] == 0) {
            fopAcM_delete(i_this);
        }
    }
}

static void e_arrow_bg(e_arrow_class* i_this) {
    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        i_this->mTimers[0] = 100;
    case 1:
        if (checkItemGet(fpcNm_ITEM_BOW, 1) && fopAcM_searchPlayerDistance(i_this) < 30.0f) {
            dComIfGp_setItemArrowNumCount(1);
            fopAcM_createItemForSimpleDemo(&i_this->current.pos, fpcNm_ITEM_ARROW_1, -1, NULL, NULL, 0.0f,
                                           0.0f);
            mDoAud_seStart(Z2SE_CONSUMP_ITEM_GET, NULL, 0, 0);

            i_this->mTimers[0] = 0;
        }

        if (i_this->mTimers[0] == 0) {
            fopAcM_delete(i_this);
        }
    }
}

static void e_arrow_spin(e_arrow_class* i_this) {
    switch (i_this->mMode) {
    case 0:
        i_this->mTimers[0] = 25;

        if (cM_rndF(1.0f) > 0.5f) {
            i_this->mMode = 1;
        } else {
            i_this->mMode = 2;
        }
    case 1:
        i_this->shape_angle.y -= 0x400;
    case 2:
        i_this->shape_angle.y += 0x200;
        i_this->shape_angle.x -= 0x1200;
        i_this->current.angle.x += 700;

        if (i_this->mTimers[0] == 0) {
            fopAcM_delete(i_this);
        }
    }
}

static void* s_limit_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) != NULL && i_actor != i_data &&
        fopAcM_GetName(i_actor) == PROC_E_ARROW &&
        static_cast<e_arrow_class*>(i_actor)->mAction == ACTION_ARROW_FIRE &&
        fabsf(static_cast<e_arrow_class*>(i_actor)->field_0xa00.z -
              static_cast<e_arrow_class*>(i_data)->field_0xa00.z) < 300.0f)
    {
        return i_actor;
    }

    return NULL;
}

static void e_arrow_demo_fire(e_arrow_class* i_this) {
    cXyz sp9C;
    csXyz spB0;

    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        i_this->field_0xa00 = i_this->current.pos;

        if (i_this->field_0xa00.z > 39000.0f) {
            i_this->field_0xa0c = -0x8000;
        } else {
            i_this->field_0xa0c = 0;
        }

        i_this->mTimers[0] = 20;
    case 1:
        spB0.set(0, i_this->field_0xa0c, 0);

        static u16 fire_name[] = {0x84EE, 0x84EF, 0x84F0, 0x84F1};

        for (int i = 0; i < 4; i++) {
            i_this->field_0x9f0[i] = dComIfGp_particle_set(i_this->field_0x9f0[i], fire_name[i],
                                                           &i_this->field_0xa00, &spB0, NULL);

            dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0x9f0[i]);
        }

        if (i_this->mTimers[0] == 0) {
            daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);

            f32 var_f31;
            if (i_this->field_0xa0c == 0) {
                var_f31 = 41200.0f;
                sp9C.z = -700.0f;
            } else {
                var_f31 = 36800.0f;
                sp9C.z = 700.0f;
            }

            if (!player_p->checkWolfDig() &&
                (!dComIfGp_event_runCheck() ||
                 fopAcM_getTalkEventPartner(daPy_getLinkPlayerActorClass()) !=
                     (fopAc_ac_c*)daPy_py_c::getMidnaActor()))
            {
                if (!dMsgObject_isTalkNowCheck()) {
                    cLib_addCalc2(&i_this->field_0xa00.z, var_f31, 0.1f, i_this->field_0xa10);
                }
            }

            if (i_this->field_0xa0c == 0 && fpcM_Search(s_limit_sub, i_this) != NULL) {
                dBgS_ObjGndChk gnd_chk;
                sp9C = player_p->current.pos;
                sp9C.y += 200.0f;
                gnd_chk.SetPos(&sp9C);

                if (dComIfG_Bgsp().GroundCross(&gnd_chk) > -10000.0f) {
                    i_this->mMode = 2;
                    return;
                }
            }

            cLib_addCalc2(&i_this->field_0xa10, 10.0f, 1.0f, 1.0f);

            if (!dComIfGp_event_runCheck()) {
                sp9C.y = 0.0f;
                sp9C.x = 0.0f;

                i_this->mCcFireEffSph.SetC(i_this->field_0xa00 + sp9C);
                i_this->mCcFireEffSph.SetR(800.0f);
                dComIfG_Ccsp()->Set(&i_this->mCcFireEffSph);
            }

            i_this->mpModel = NULL;
        }

        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_STRAWFENCE_BURNING, &i_this->field_0xa00, 0, 0, 1.0f,
                                      1.0f, -1.0f, -1.0f, 0);
        break;
    case 2:
        dMeter2Info_getMeterClass()->setLifeZero();
        break;
    }
}

static void atHit_CB(fopAc_ac_c* i_atActor, dCcD_GObjInf* i_atObjInf, fopAc_ac_c* i_tgActor,
                     dCcD_GObjInf* i_tgObjInf) {
    if (i_tgActor != NULL && fopAcM_GetName(i_tgActor) == PROC_ALINK) {
        if (daAlink_getAlinkActorClass()->checkWoodShieldEquipNotIronBall()) {
            daAlink_getAlinkActorClass()->getArrowShieldOffset(
                &i_atActor->current.pos, &i_atActor->shape_angle,
                &static_cast<e_arrow_class*>(i_atActor)->field_0x9d4,
                &static_cast<e_arrow_class*>(i_atActor)->field_0x9c8);
        }
    }
}

static void e_arrow_shield(e_arrow_class* i_this) {
    e_arrow_class* a_this = (e_arrow_class*)i_this;
    daAlink_getAlinkActorClass()->setArrowShieldPos(&a_this->current.pos, &a_this->shape_angle,
                                                    &i_this->field_0x9d4, &i_this->field_0x9c8);

    static cXyz localOffset(0.0f, 0.0f, -30.0f);
    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::ZXYrotM(a_this->shape_angle.x, a_this->shape_angle.y, 0);
    mDoMtx_stack_c::multVec(&localOffset, &a_this->current.pos);

    if (daAlink_getAlinkActorClass()->checkStickArrowReset()) {
        fopAcM_delete(a_this);
    }
}

static void e_arrow_demo_bound(e_arrow_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* const player_p = dComIfGp_getPlayer(0);

    if (i_this->mMode == 0) {
        i_this->field_0xa0c = 12000;
        i_this->field_0xa10 = 2500.0f;
        i_this->mMode = 1;
        i_this->mTimers[0] = 80;
    }

    if (i_this->mTimers[0] == 0) {
        fopAcM_delete(a_this);
    }

    if (i_this->field_0xa0c > 0) {
        a_this->shape_angle.x += (s16)i_this->field_0xa10;

        if (a_this->shape_angle.x > i_this->field_0xa0c ||
            a_this->shape_angle.x < (s16)-i_this->field_0xa0c)
        {
            if (i_this->field_0xa0c > 1000) {
                fopAcM_seStart(a_this, Z2SE_OBJ_KEY_BOUND, i_this->field_0xa0c * 0.01f);
            }

            i_this->field_0xa0c /= 2;

            if (a_this->shape_angle.x > 0) {
                a_this->shape_angle.x = i_this->field_0xa0c;
            } else {
                a_this->shape_angle.x = -i_this->field_0xa0c;
            }

            i_this->field_0xa10 *= -1.0f;
        }
    }

    a_this->shape_angle.y = player_p->shape_angle.y - 0x2000;
    a_this->current.pos.x = player_p->current.pos.x - 60.0f;
    a_this->current.pos.y =
        player_p->current.pos.y + 5.0f + fabsf(cM_ssin(a_this->shape_angle.x) * 50.0f);
    a_this->current.pos.z = player_p->current.pos.z + 10.0f + 25.0f;
}

static void action(e_arrow_class* i_this) {
    s8 try_create_explosion = false;

    switch (i_this->mAction) {
    case ACTION_ARROW_SHOT:
        e_arrow_shot(i_this);
        dComIfGp_att_LookRequest(i_this, 1000.0f, 300.0f, -300.0f, 0x6000, 1);
        hit_check(i_this);

        if (i_this->mArrowType == ARROW_TYPE_FIRE) {
            i_this->mSound.startLevelSound(Z2SE_OBJ_ARROW_FLY_FIRE, 0, -1);
        } else {
            i_this->mSound.startLevelSound(Z2SE_OBJ_ARROW_FLY_NORMAL, 0, -1);
        }
        break;
    case ACTION_ARROW_BG:
        e_arrow_bg(i_this);

        if (i_this->mArrowType == ARROW_TYPE_FIRE) {
            i_this->mSound.startLevelSound(Z2SE_OBJ_ARROW_FIRE_READY, 0, -1);
        }

        try_create_explosion = true;
        break;
    case ACTION_ARROW_SPIN:
        e_arrow_spin(i_this);
        try_create_explosion = true;
        break;
    case ACTION_ARROW_SHIELD:
        e_arrow_shield(i_this);
        try_create_explosion = true;
        break;
    case ACTION_ARROW_FIRE:
        e_arrow_demo_fire(i_this);
        break;
    case ACTION_ARROW_BOUND:
        e_arrow_demo_bound(i_this);
        return;
    }

    if (try_create_explosion && i_this->mArrowType == ARROW_TYPE_BOMB) {
        dBomb_c::createNormalBombExplode(&i_this->current.pos);
        fopAcM_delete(i_this);
    }

    cMtx_YrotS(*calc_mtx, i_this->current.angle.y);
    cMtx_XrotM(*calc_mtx, i_this->current.angle.x);

    cXyz sp20;
    sp20.x = 0.0f;
    sp20.y = 0.0f;
    sp20.z = i_this->speedF;
    MtxPosition(&sp20, &i_this->speed);

    i_this->current.pos.x += i_this->speed.x + i_this->field_0x5d0;
    i_this->current.pos.y += i_this->speed.y;
    i_this->current.pos.z += i_this->speed.z + i_this->field_0x5d4;

    i_this->eyePos = i_this->current.pos;
}

static int daE_ARROW_Execute(e_arrow_class* i_this) {
    const fopAc_ac_c* a_this = static_cast<fopAc_ac_c*>(i_this);

    if (i_this->field_0xa14 != 0) {
        return 1;
    }

    cXyz sp60;
    cXyz sp6C;
    cXyz sp78;

    i_this->field_0x5c0++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    action(i_this);

    if (i_this->mpModel == NULL) {
        return 1;
    }

    if (i_this->mArrowType >= ARROW_TYPE_FIRE) {
        fire_eff_set(i_this);
    }

    s16 var_r29;
    s16 var_r28;

    if (i_this->field_0x5ce != 0) {
        i_this->field_0x5ce--;

        f32 temp_f1 = i_this->field_0x5ce * 50.0f;
        var_r29 = temp_f1 * cM_ssin(i_this->field_0x5c0 * 0x7000);
        var_r28 = temp_f1 * cM_ssin(i_this->field_0x5c0 * 0x7A00);
    } else {
        var_r28 = 0;
        var_r29 = 0;
    }

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(a_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, 0.0f, 40.0f);
    mDoMtx_stack_c::YrotM(var_r29);
    mDoMtx_stack_c::XrotM(var_r28);
    mDoMtx_stack_c::transM(0.0f, 0.0f, -40.0f);
    mDoMtx_stack_c::scaleM(1.0f, 1.0f, 0.95f);
    mDoMtx_stack_c::YrotM(-23500);
    mDoMtx_stack_c::XrotM(-2000);
    mDoMtx_stack_c::transM(-60.0f, -10.0f, 40.0f);

    i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    return 1;
}

static int daE_ARROW_IsDelete(e_arrow_class* i_this) {
    return 1;
}

static int daE_ARROW_Delete(e_arrow_class* i_this) {
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, i_this->mResName);
    i_this->mSound.deleteObject();

    JPABaseEmitter* emitter_p = dComIfGp_particle_getEmitter(i_this->field_0x9ec);
    if (emitter_p != NULL) {
        emitter_p->setParticleCallBackPtr(NULL);
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_arrow_class* a_this = static_cast<e_arrow_class*>(i_this);

    int res_id;
    if (fopAcM_GetParam(a_this) & 0x80000000) {
        res_id = 0x52;
    } else {
        res_id = 0x45;
    }

    if (a_this->mArrowType == ARROW_TYPE_BOMB) {
        res_id++;
    }

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(a_this->mResName, res_id);
    JUT_ASSERT(0, modelData != NULL);

    a_this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (a_this->mpModel == NULL) {
        return 0;
    }

    a_this->mSound.init(&a_this->current.pos, 1);
    return 1;
}

static int daE_ARROW_Create(fopAc_ac_c* i_this) {
    e_arrow_class* const a_this = static_cast<e_arrow_class*>(i_this);
    fopAcM_ct(i_this, e_arrow_class);

    if (fopAcM_GetParam(a_this) & 0x80000000) {
        a_this->mResName = "E_rdy";
    } else {
        a_this->mResName = "E_rd";
    }

    int phase_state = dComIfG_resLoad(&a_this->mPhase, a_this->mResName);
    if (phase_state == cPhs_COMPLEATE_e) {
        a_this->mArrowType = fopAcM_GetParam(a_this) & 0xF;
        a_this->mFlags = fopAcM_GetParam(a_this) & 0xF0;

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x820)) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(a_this, a_this->mpModel->getBaseTRMtx());

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x1d}, {0x0, 0x0}, 0x0}},  // mObj
                {dCcD_SE_ARROW_STICK, 0x0, 0x8, 0x0, 0x0},                     // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                            // mGObjTg
                {0x0},                                                         // mGObjCo
            },                                                                 // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 20.0f}  // mSph
            }                                // mSphAttr
        };

        static dCcD_SrcSph tg_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fafdb7, 0x3}, 0x0}},  // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                // mGObjAt
                {dCcD_SE_METAL, 0x5, 0x0, 0x0, 0x22},              // mGObjTg
                {0x0},                                             // mGObjCo
            },                                                     // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f}  // mSph
            }                                // mSphAttr
        };

        static dCcD_SrcSph fire_eff_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x4, 0x1d}, {0x0, 0x0}, 0x75}},  // mObj
                {dCcD_SE_ARROW_STICK, 0x0, 0x0, 0x1, 0x0},                      // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                             // mGObjTg
                {0x0},                                                          // mGObjCo
            },                                                                  // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 20.0f}  // mSph
            }                                // mSphAttr
        };

        a_this->mCcStts.Init(0xFF, 0, a_this);
        a_this->mCcAtSph.Set(at_sph_src);
        a_this->mCcAtSph.SetStts(&a_this->mCcStts);
        a_this->mCcAtSph.SetAtHitCallback(atHit_CB);

        if (a_this->mArrowType == ARROW_TYPE_FIRE) {
            a_this->mCcAtSph.SetAtMtrl(dCcD_MTRL_FIRE);
        }

        a_this->mCcAtSph.StartCAt(a_this->current.pos);

        a_this->mCcTgSph.Set(tg_sph_src);
        a_this->mCcTgSph.SetStts(&a_this->mCcStts);

        a_this->field_0x5c0 = cM_rndF(0xFFFF);
        if (a_this->mFlags & 0x10) {
            a_this->mCcFireEffSph.Set(fire_eff_sph_src);
            a_this->mCcFireEffSph.SetStts(&a_this->mCcStts);
        }

        mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
        a_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    return phase_state;
}

static actor_method_class l_daE_ARROW_Method = {
    (process_method_func)daE_ARROW_Create,  (process_method_func)daE_ARROW_Delete,
    (process_method_func)daE_ARROW_Execute, (process_method_func)daE_ARROW_IsDelete,
    (process_method_func)daE_ARROW_Draw,
};

actor_process_profile_definition g_profile_E_ARROW = {
    fpcLy_CURRENT_e,
    8,
    fpcPi_CURRENT_e,
    PROC_E_ARROW,
    &g_fpcLf_Method.base,
    sizeof(e_arrow_class),
    0,
    0,
    &g_fopAc_Method.base,
    163,
    &l_daE_ARROW_Method,
    0x44100,
    fopAc_ENEMY_e,
    fopAc_CULLBOX_0_e,
};

AUDIO_INSTANCES;
template<>
JAUSectionHeap* JASGlobalInstance<JAUSectionHeap>::sInstance;
