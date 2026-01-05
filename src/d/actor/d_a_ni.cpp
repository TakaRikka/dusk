
#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/d_msg_flow.h"
#include "d/d_s_play.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_ni.h"
#include "d/actor/d_a_npc_moir.h"
#include "d/actor/d_a_player.h"
#include "SSystem/SComponent/c_math.h"
#include "c/c_damagereaction.h"
#include "Z2AudioLib/Z2Instances.h"
#include "f_op/f_op_camera_mng.h"

enum Joint {
    /* 0x0 */ JNT_WAIST,
    /* 0x1 */ JNT_LEGL,
    /* 0x2 */ JNT_FOOTL,
    /* 0x3 */ JNT_LEGR,
    /* 0x4 */ JNT_FOOTR,
    /* 0x5 */ JNT_NECK,
    /* 0x6 */ JNT_HEAD,
    /* 0x7 */ JNT_WINGL,
    /* 0x8 */ JNT_WINGR,
};

daNi_HIO_c::daNi_HIO_c() {
    field_0x04 = -1;
    mBaseSize = 1.2f;
    mMoveSpeed = 4.0f;
    mMotionSpeed = 1.5f;
    mRecognizeLink = 1;
    mRecognizeRange = 200.0f;
    mFleeWalkSpeed = 15.0f;
    mFleeFlySpeed = 20.0f;
    mFleeJumpPower = 40.0f;
    mFleeJumpGravity = -1.0f;
    mThrowSpeed = 20.0f;
    mThrowJumpPower = 15.0f;
    mMogakuModelShake = 0;
    field_0x39 = 1;
}

static void anm_init(ni_class* i_this, int i_resID, f32 i_morf, u8 i_mode, f32 i_speed) {
    if (!(i_this->field_0x610 > 1.0f)) {
        J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes("Ni", i_resID);
        i_this->mpMorf->setAnm(pbck, i_mode, i_morf, i_speed, 0.0f, -1.0f, NULL);
        i_this->mAnmID = i_resID;
    }
}

static void hane_set(ni_class* i_this, f32 i_rate, f32 i_speed) {
    static u16 hane_id[] = {0x82C2, 0x82C0, 0x82C1, 0x82C3};

    i_this->mHaneEmitterID = dComIfGp_particle_set(i_this->mHaneEmitterID, hane_id[i_this->mColor],
                                                   &i_this->current.pos, NULL, NULL);
    dComIfGp_particle_levelEmitterOnEventMove(i_this->mHaneEmitterID);

    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mHaneEmitterID);
    if (emitter != NULL) {
        emitter->setRate(i_rate);
        emitter->setDirectionalSpeed(i_speed);
    }
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int joint_no = i_joint->getJntNo();

        J3DModel* model = j3dSys.getModel();
        ni_class* a_this = (ni_class*)model->getUserArea();
        if (a_this != NULL) {
            MTXCopy(model->getAnmMtx(joint_no), *calc_mtx);
            if (joint_no == JNT_HEAD) {
                cMtx_YrotM(*calc_mtx, a_this->field_0x61c);
                cMtx_XrotM(*calc_mtx, a_this->field_0x61a);
            } else if (joint_no == JNT_NECK) {
                cMtx_XrotM(*calc_mtx, a_this->field_0x61a / 2);
                cMtx_ZrotM(*calc_mtx, a_this->field_0x618);
            } else if (joint_no == JNT_WINGL) {
                cMtx_YrotM(*calc_mtx, a_this->field_0x626);
            } else if (joint_no == JNT_WINGR) {
                cMtx_YrotM(*calc_mtx, a_this->field_0x624);
            } else if (joint_no >= JNT_LEGL && joint_no <= JNT_FOOTR) {
                cMtx_ZrotM(*calc_mtx, a_this->field_0x9ee[joint_no - 1]);
            }

            model->setAnmMtx(joint_no, *calc_mtx);
            MTXCopy(*calc_mtx, j3dSys.mCurrentMtx);

            if (joint_no == TREG_S(0)) {
                MTXCopy(model->getAnmMtx(joint_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, TREG_S(1));
                cMtx_XrotM(*calc_mtx, TREG_S(2));
                cMtx_ZrotM(*calc_mtx, TREG_S(3));

                model->setAnmMtx(joint_no, *calc_mtx);
                MTXCopy(*calc_mtx, j3dSys.mCurrentMtx);
            }

            if (joint_no == TREG_S(4)) {
                MTXCopy(model->getAnmMtx(joint_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, TREG_S(5));
                cMtx_XrotM(*calc_mtx, TREG_S(6));
                cMtx_ZrotM(*calc_mtx, TREG_S(7));

                model->setAnmMtx(joint_no, *calc_mtx);
                MTXCopy(*calc_mtx, j3dSys.mCurrentMtx);
            }
        }
    }

    return 1;
}

static int daNi_Draw(ni_class* i_this) {
    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);

    i_this->mpBtk->entry(model->getModelData());
    i_this->mpMorf->entryDL();

    if (!fopAcM_checkCarryNow(i_this)) {
        cXyz pos;
        pos.set(i_this->current.pos.x, i_this->current.pos.y + 100.0f, i_this->current.pos.z);

        i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &pos, 400.0f, 40.0f,
                                                i_this->current.pos.y, i_this->mAcch.GetGroundH(),
                                                i_this->mAcch.m_gnd, &i_this->tevStr, 0, 1.0f,
                                                dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static void* s_play_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NI) {
        ni_class* a_actor = (ni_class*)i_actor;

        if (a_actor->mAction == ACTION_PLAY_e) {
            return i_actor;
        }
    }

    return NULL;
}

static void damage_check(ni_class* i_this) {
    dComIfGp_getPlayer(0);

    if (i_this->field_0x60c == 0) {
        i_this->mCcStts.Move();

        if (i_this->mCcSph.ChkTgHit()) {
            i_this->field_0x60c = 6;
            i_this->mAtInfo.mpCollider = i_this->mCcSph.GetTgHitObj();

            anm_init(i_this, BCK_MOGAKU, 1.5f, J3DFrameCtrl::EMode_LOOP, 1.5f);

            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                i_this->mAction = ACTION_WINDSPIN_e;
                i_this->mMode = 0;
                i_this->mSound.startSound(Z2SE_CHICKEN_CRY, 0, -1);
            } else {
                at_power_check(&i_this->mAtInfo);
                i_this->mSound.startSound(Z2SE_CHICKEN_CRY, 0, -1);

                i_this->field_0x614 = (f32)i_this->mAngleToPlayer + cM_rndFX(2000);

                if (i_this->mAction == ACTION_PLAY_e) {
                    i_this->field_0x560 = 1;
                    i_this->field_0x60c = 0;
                } else {
                    if (i_this->mAtInfo.mHitType == 1) {
                        i_this->field_0x610 = 40.0f;
                        hane_set(i_this, 15.0f, 30.0f);
                    } else {
                        i_this->field_0x610 = 30.0f;
                        hane_set(i_this, 10.0f, 30.0f);
                    }

                    i_this->mAction = ACTION_DAMAGE_e;
                    i_this->mMode = 0;
                    i_this->health--;

                    if (i_this->mAtInfo.mHitType == 1 && i_this->health <= 0) {
                        i_this->health = 0;

                        if (!dComIfGp_event_runCheck() &&
                            fpcM_Search(s_play_sub, i_this) == NULL)
                        {
                            i_this->mAction = ACTION_PLAY_e;
                            i_this->mMode = 0;
                        }
                    }
                }
            }
        }
    }
}

static u8 data_8095158d;

static daNi_HIO_c l_HIO;

static fopAc_ac_c* target_info[10];

static int target_info_count;

static void* s_t_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_NE && target_info_count < 10)
    {
        target_info[target_info_count] = (fopAc_ac_c*)i_actor;
        target_info_count++;
    }

    return NULL;
}

static fopAc_ac_c* search_test(ni_class* i_this) {
    target_info_count = 0;

    for (int i = 0; i < 10; i++) {
        target_info[i] = NULL;
    }

    fpcM_Search(s_t_sub, i_this);

    f32 search_range = 100.0f;
    if (target_info_count != 0) {
        cXyz dist;

        fopAc_ac_c* target;
        int i = 0;
        while (i < target_info_count) {
            target = target_info[i];

            dist = target->current.pos - i_this->current.pos;
            if (JMAFastSqrt(dist.x * dist.x + dist.z * dist.z) < search_range) {
                return target;
            }

            i++;

            if (i == target_info_count) {
                i = 0;
                search_range += 100.0f;

                if (search_range > 1000.0f) {
                    return NULL;
                }
            }
        }
    } else {
        return NULL;
    }

    return NULL;
}

static void ni_carry_check(ni_class* i_this) {
    dComIfGp_getPlayer(0);

    if (fopAcM_checkCarryNow(i_this)) {
        cLib_offBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
        i_this->mAction = ACTION_CARRY_e;
        i_this->mMode = 0;
        i_this->speed.y = 0.0f;
        i_this->speedF = 0.0f;

        OS_REPORT("//////////////NI CARRY ON! \n");
    }
}

static BOOL move_gake_check(ni_class* i_this, f32 param_1) {
    dBgS_GndChk gnd_chk;
    cXyz sp74;
    cXyz sp80;

    cMtx_YrotS(*calc_mtx, i_this->shape_angle.y);

    sp74.x = 0.0f;
    sp74.y = 100.0f;
    sp74.z = param_1;
    MtxPosition(&sp74, &sp80);
    sp80 += i_this->current.pos;

    gnd_chk.SetPos(&sp80);

    if (i_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 300.0f) {
        return true;
    }

    return false;
}

static void ni_normal(ni_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz sp50;
    cXyz sp5C;

    f32 move_speed = 0.0f;
    switch (i_this->mMode) {
    case 0:
        i_this->mTimers[0] = cM_rndF(70) + 50.0f;
        anm_init(i_this, BCK_WAIT1, 3.0f, J3DFrameCtrl::EMode_LOOP, move_speed);
        i_this->mMode++;
        break;
    case 1:
        if (i_this->mTimers[0] == 0) {
            i_this->mMode = 10;

            if (fopAcM_wayBgCheck(a_this, 100.0f, 30.0f) || move_gake_check(i_this, 100.0f)) {
                i_this->mMoveAngle =
                    (a_this->current.angle.y + 0x10000 + (s16)cM_rndFX(8000)) - 0x8000;
                i_this->mTimers[1] = 30;
            } else {
                sp50 = a_this->home.pos - a_this->current.pos;
                sp50.x += cM_rndFX(500);
                sp50.z += cM_rndFX(500);

                i_this->mMoveAngle = cM_atan2s(sp50.x, sp50.z);
            }

            i_this->mTimers[0] = cM_rndF(60) + 30.0f;
            anm_init(i_this, BCK_WALK_A, 5.0f, J3DFrameCtrl::EMode_LOOP, l_HIO.mMotionSpeed);
        }
        break;
    case 10:
        move_speed = l_HIO.mMoveSpeed;
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mMoveAngle, 4, 0x400);

        if (i_this->mTimers[0] == 0 ||
            ((fopAcM_wayBgCheck(a_this, 100.0f, 30.0f) || move_gake_check(i_this, 100.0f)) &&
             i_this->mTimers[1] == 0))
        {
            i_this->mMode = 0;
        }

        if (i_this->mpMorf->checkFrame(1.0f) || i_this->mpMorf->checkFrame(16.0f)) {
            i_this->mSound.startSound(Z2SE_CHICKEN_WALK, 0, -1);
        }
        break;
    }

    cLib_addCalc2(&a_this->speedF, move_speed, 1.0f, 5.0f);

    if (fabsf(player->speedF) < 20.0f && i_this->field_0x60e == 0) {
        i_this->field_0x60e = 1;
    }

    if (dComIfGp_checkPlayerStatus0(0, 0x8000000)) {
        i_this->field_0x60e = 30;
    }

    s16 sp0A = a_this->shape_angle.y - i_this->mAngleToPlayer;
    if (sp0A < 0x2000 && sp0A > 0x2000) {
        i_this->field_0x60e = 0;
    }

    s8 var_r29 = false;
    i_this->mTargetActorID = -1;

    if (!(i_this->mCounter & 31)) {
        fopAc_ac_c* target = search_test(i_this);
        if (target != NULL) {
            sp50 = target->current.pos - a_this->current.pos;

            if (sp50.abs() < l_HIO.mRecognizeRange) {
                i_this->mTargetActorID = fopAcM_GetID(target);
                var_r29 = true;
            }
        }
    }

    fopAc_ac_c* cow = fopAcM_SearchByName(PROC_COW);
    if (cow != NULL && cow->speedF > 1.0f) {
        sp50 = cow->current.pos - a_this->current.pos;
        f32 var_f30 = sp50.abs();

        if (i_this->field_0xa48 == 0 && i_this->mDistToPlayer < 3000.0f && var_f30 < 2000.0f) {
            i_this->field_0xa48 = 1;

            a_this->current.pos.x = cM_rndFX(200) + -914.0f;
            a_this->current.pos.y = 160.0f;
            a_this->current.pos.z = cM_rndFX(200) + 3931.0f;
        }

        if (var_f30 < 400.0f) {
            var_r29 = true;
            i_this->mSound.startSound(Z2SE_CHICKEN_CRY, 0, -1);
        }
    }

    if (!i_this->checkGold() && (var_r29 || (l_HIO.mRecognizeLink && i_this->field_0x60e == 0 &&
                                             i_this->mDistToPlayer < l_HIO.mRecognizeRange)))
    {
        i_this->mAction = ACTION_AWAY_e;
        i_this->mMode = 0;
    }
}

static void ni_away(ni_class* i_this) {
    ni_class* a_this = (ni_class*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp4C;
    cXyz sp58;

    f32 move_speed = 0.0f;
    f32 speed_max_step = 2.0f;
    s16 angle_max_step = 0x1800;

    fopAc_ac_c* target = fopAcM_SearchByID(a_this->mTargetActorID);
    if (target != NULL) {
        sp4C = i_this->current.pos - target->current.pos;
        a_this->mDistToPlayer = sp4C.abs();
    } else {
        sp4C = i_this->current.pos - player->current.pos;
    }

    switch (a_this->mMode) {
    case 0:
        anm_init(a_this, BCK_WALK_B, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.5f);
        a_this->mMode++;

        if (target == NULL) {
            a_this->mTimers[0] = 20;
        } else {
            a_this->mTimers[0] = 20;
        }
    case 1:
        move_speed = l_HIO.mFleeWalkSpeed;

        if (a_this->mpMorf->checkFrame(1.0f) || a_this->mpMorf->checkFrame(8.0f)) {
            a_this->mSound.startSound(Z2SE_CHICKEN_WALK, 0, -1);
        }

        if (a_this->mTimers[0] == 0) {
            if (a_this->mDistToPlayer > l_HIO.mRecognizeRange + 50.0f) {
                a_this->mAction = ACTION_NORMAL_e;
                a_this->mMode = 0;
            } else if (a_this->mAcch.ChkGroundHit() &&
                       (target != NULL || a_this->mDistToPlayer < l_HIO.mRecognizeRange - 100.0f))
            {
                anm_init(a_this, BCK_FLY, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                a_this->speed.y = l_HIO.mFleeJumpPower;
                a_this->mMode++;
                hane_set(a_this, 5.0f, 25.0f);
            }
        }

        if (a_this->mTimers[1] == 0) {
            a_this->mMoveAngle = (s16)cM_atan2s(sp4C.x, sp4C.z);

            if (fopAcM_wayBgCheck(a_this, 100.0f, 30.0f)) {
                a_this->mTimers[1] = 30;
                a_this->mMoveAngle =
                    (i_this->current.angle.y + 0x10000 + (s16)cM_rndFX(10000)) - 0x8000;
            }
        }
        break;
    case 2:
        move_speed = l_HIO.mFleeFlySpeed;
        angle_max_step = 0x400;
        speed_max_step = 10.0f;

        if (i_this->speed.y <= 0.0f) {
            i_this->gravity = l_HIO.mFleeJumpGravity;

            if (a_this->mAcch.ChkGroundHit() || a_this->field_0x848 != 0) {
                a_this->mMode = 0;
            }
        }
        break;
    }

    cLib_addCalc2(&i_this->speedF, move_speed, 1.0f, speed_max_step);
    cLib_addCalcAngleS2(&i_this->current.angle.y, a_this->mMoveAngle, 2, angle_max_step);
}

static void ni_swim(ni_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp48;
    cXyz sp54;

    f32 move_speed = 0.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_WAIT1, 3.0f, J3DFrameCtrl::EMode_LOOP, move_speed);
        i_this->mMode++;
        i_this->field_0x9e8 = move_speed;
    case 1:
        if (fopAcM_wayBgCheck(i_this, 100.0f, 30.0f)) {
            i_this->mMoveAngle = (i_this->current.angle.y + 0x10000 + (s16)cM_rndFX(8000)) - 0x8000;
            i_this->mTimers[1] = 128;
            i_this->mTimers[0] = 128;
        } else {
            sp48 = i_this->home.pos - i_this->current.pos;
            sp48.x += cM_rndFX(500);
            sp48.z += cM_rndFX(500);

            i_this->mMoveAngle = cM_atan2s(sp48.x, sp48.z);
            i_this->mTimers[0] = cM_rndF(60) + 30.0f;
        }

        i_this->mMode++;
        break;
    case 2:
        move_speed = 2.0f;
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mMoveAngle, 4, 0x100);

        if (i_this->mTimers[0] == 0 ||
            (fopAcM_wayBgCheck(i_this, 100.0f, 30.0f) && i_this->mTimers[1] == 0))
        {
            i_this->mMode = 1;
        }

        if (i_this->field_0x560 != 0) {
            i_this->field_0x560 = 0;
            i_this->mMode++;

            anm_init(i_this, BCK_MOGAKU, 1.0f, 2, 1.0f);
            i_this->field_0x60c = 10;
            i_this->mTimers[2] = cM_rndF(10) + 5.0f;

            i_this->mSound.startSound(Z2SE_CHICKEN_CRY, 0, -1);
            hane_set(i_this, 5.0f, 25.0f);
            i_this->mMoveAngle = cM_rndF(0x10000);
        }
        break;
    case 3:
        move_speed = 2.0f;
        i_this->current.angle.z = cM_ssin(i_this->mCounter * 9000) * 3000.0f;

        if (i_this->mTimers[2] == 0) {
            i_this->mMode = 1;
            anm_init(i_this, BCK_WAIT1, 3.0f, J3DFrameCtrl::EMode_LOOP, 0.0f);
        }

        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mMoveAngle, 4, 0x400);
        break;
    }

    cLib_addCalc2(&i_this->speedF, move_speed, 1.0f, 1.0f);

    if (i_this->field_0x9e4 == 0 && i_this->field_0x848 == 0) {
        i_this->mAction = ACTION_AWAY_e;
        i_this->mMode = 2;

        anm_init(i_this, BCK_FLY, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->speed.y = 20.0f;
        hane_set(i_this, 5.0f, 25.0f);
        i_this->field_0x9e4 = 30;
    }
}

static int ni_carry(ni_class* i_this) {
    ni_class* a_this = (ni_class*)i_this;
    cXyz sp40;
    cXyz sp4C;
    cXyz sp58;

    cLib_addCalcAngleS2(&a_this->current.angle.y, dComIfGp_getPlayer(0)->shape_angle.y, 1, 0x2000);
    i_this->mCcSph.OffCoSetBit();
    i_this->speed.y = 0.0f;

    BOOL update_position = false;
    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_MOGAKU, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode++;
        // fallthrough
    case 1:
        hane_set(i_this, 0.1f, 20.0f);
        break;
    }

    if (l_HIO.mMogakuModelShake) {
        f32 var_f1;
        if (daPy_getPlayerActorClass()->checkPlayerFly()) {
            var_f1 = 1000.0f;
        } else {
            var_f1 = 2500.0f;
        }

        a_this->current.angle.z = var_f1 * cM_ssin(i_this->mCounter * 9000);
    }

    if (!fopAcM_checkCarryNow(a_this)) {
        OS_REPORT("//////////////NI CARRY OFF ! \n");

        if (fopAcM_GetSpeedF(a_this) > 0.0f) {
            i_this->mMode = 0;
            a_this->speedF = l_HIO.mThrowSpeed;
            a_this->speed.y = l_HIO.mThrowJumpPower;

            i_this->mAction = ACTION_FLY_e;
            i_this->mMode = 0;
        } else if (i_this->checkGold()) {
            i_this->mAction = ACTION_NORMAL_e;
            i_this->mMode = 0;
        } else {
            i_this->mAction = ACTION_AWAY_e;
            i_this->mMode = 0;
        }

        fopAcM_carryOffRevise(a_this);
        update_position = true;
    }

    return update_position;
}

static void ni_fly(ni_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_FALL, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode++;
        i_this->mTimers[0] = 40;
        i_this->field_0x5c0 = player->current.pos;
        // fallthrough
    case 1:
        hane_set(i_this, 0.1f, 20.0f);
        break;
    }

    i_this->gravity = l_HIO.mFleeJumpGravity;
    if (i_this->speed.y < -20.0f) {
        i_this->speed.y = -20.0f;
    }

    if (i_this->checkGold() && i_this->mTimers[0] == 0) {
        i_this->mAction = ACTION_RETURN_e;
        i_this->mMode = 0;
    } else if (i_this->mAcch.ChkGroundHit() || i_this->mAcch.ChkWallHit() ||
               i_this->field_0x848 != 0)
    {
        i_this->mAction = ACTION_DAMAGE_e;
        i_this->mMode = 0;
    }
}

static void ni_drop(ni_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_FALL, 1.0f, 0, 1.0f);
        i_this->mMode++;
        // fallthrough
    case 1:
        hane_set(i_this, 0.1f, 20.0f);
        break;
    }

    i_this->gravity = l_HIO.mFleeJumpGravity;
    if (i_this->mAcch.ChkGroundHit() || i_this->field_0x848 != 0) {
        i_this->mAction = ACTION_NORMAL_e;
        i_this->mMode = 0;
    }

    if (i_this->checkGold() && i_this->current.pos.y < player->current.pos.y - 100.0f &&
        i_this->mAcch.GetGroundH() < -20000.0f)
    {
        i_this->mAction = ACTION_RETURN_e;
        i_this->mMode = 0;
        i_this->field_0x5c0 = player->current.pos;
    }
}

static void ni_return(ni_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp38;
    sp38.x = i_this->field_0x5c0.x - i_this->current.pos.x;
    sp38.z = i_this->field_0x5c0.z - i_this->current.pos.z;

    i_this->mMoveAngle = cM_atan2s(sp38.x, sp38.z);
    f32 var_f31 = JMAFastSqrt(sp38.x * sp38.x + sp38.z * sp38.z);

    switch (i_this->mMode) {
    case 1:
        break;
    case 0:
        i_this->mMode++;
        i_this->mTimers[0] = 200;
        anm_init(i_this, BCK_FLY, 2.0f, J3DFrameCtrl::EMode_LOOP, 2.0f);
        break;
    }

    cLib_addCalc2(&i_this->speedF, 20.0f, 1.0f, 10.0f);

    if (i_this->current.pos.y < player->current.pos.y + 50.0f) {
        i_this->gravity = 2.0f;
        if (i_this->speed.y > 20.0f) {
            i_this->speed.y = 20.0f;
        }
    } else {
        i_this->gravity = -1.0f;
        if (i_this->speed.y < -20.0f) {
            i_this->speed.y = -20.0f;
        }
    }

    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mMoveAngle, 4, 0x400);

    if (i_this->mTimers[0] == 0 || var_f31 < 100.0f) {
        i_this->mAction = ACTION_DROP_e;
        i_this->mMode = 0;
    }
}

static void ni_damage(ni_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_WALK_B, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode++;

        i_this->mTimers[0] = cM_rndF(20.0f) + 40.0f;
        i_this->mTimers[1] = 10;
        i_this->current.angle.y = i_this->mAngleToPlayer + 0x8000;
        // fallthrough
    case 1:
        if (i_this->mTimers[1] == 0) {
            i_this->mMoveAngle = i_this->current.angle.y + (s16)cM_rndFX(10000.0f);
            i_this->mTimers[1] = cM_rndF(15.0f) + 5.0f;

            if (cM_rndF(1.0f) < 0.35f) {
                i_this->speed.y = cM_rndF(15.0f) + 15.0f;
            }
        }

        hane_set(i_this, 0.1f, 20.0f);

        if (i_this->mTimers[0] == 0) {
            i_this->mAction = 0;
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc2(&i_this->speedF, l_HIO.mFleeWalkSpeed, 1.0f, 2.0f);
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mMoveAngle, 2, 0x2000);
}

static void* s_b_sub(void* i_actor, void* i_data) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_BOOMERANG &&
        daPy_py_c::checkBoomerangCharge() && fopAcM_GetParam(i_actor) == 1)
    {
        return i_actor;
    }

    return NULL;
}

static void ni_windspin(ni_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    fopAc_ac_c* boomerang = (fopAc_ac_c*)fpcM_Search(s_b_sub, i_this);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_FALL, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode++;

        hane_set(i_this, 0.1f, 20.0f);
        i_this->mBoomerangMoveMaxStep = 0.0f;
        // fallthrough
    case 1:
        i_this->current.angle.y -= 0x2200;
        i_this->shape_angle.y = i_this->current.angle.y;
        break;
    }

    if (boomerang == NULL) {
        i_this->mAction = ACTION_DAMAGE_e;
        i_this->mMode = 0;
        return;
    }

    if (i_this->mBoomerangMoveMaxStep >= 100.0f) {
        i_this->current.pos.x = boomerang->current.pos.x;
        i_this->current.pos.z = boomerang->current.pos.z;
    } else {
        cLib_addCalc2(&i_this->current.pos.x, boomerang->current.pos.x, 1.0f,
                      i_this->mBoomerangMoveMaxStep);
        cLib_addCalc2(&i_this->current.pos.z, boomerang->current.pos.z, 1.0f,
                      i_this->mBoomerangMoveMaxStep);
        cLib_addCalc2(&i_this->mBoomerangMoveMaxStep, 200.0f, 1.0f, 20.0f);
    }

    cLib_addCalc2(&i_this->current.pos.y, boomerang->current.pos.y - 60.0f, 0.2f, 10.0f);
    i_this->gravity = 0.0f;
    i_this->speed.y = 0.0f;
}

static int ni_demo0(ni_class* i_this) {
    cXyz sp28;
    cXyz sp34;
    daNpcMoiR_c* moi = (daNpcMoiR_c*)fopAcM_SearchByName(PROC_NPC_MOIR);

    if (i_this->mTimers[0] == 0 && moi != NULL) {
        mDoMtx_stack_c::copy(moi->getHandRMtx());
        mDoMtx_stack_c::transM(-5.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&sp34);
    } else {
        return 0;
    }

    int var_r30 = 0;

    switch (i_this->mMode) {
    case 0:
        i_this->current.pos.y = i_this->home.pos.y + 20000.0f;
        i_this->speed.y = 0.0f;

        if (i_this->field_0x5fe == 0) {
            break;
        }

        i_this->mMode = 1;
        i_this->field_0x5fe = 0;
        i_this->mDemoInitPos.set(-37864.0f, 1012.0f, -22106.0f);
        i_this->current.pos = i_this->mDemoInitPos;
        anm_init(i_this, BCK_FLY, 1.0f, 2, 1.0f);
        // fallthrough
    case 1:
        i_this->field_0x5c0 = sp34;
        var_r30 = 2;
        i_this->speedF = 20.0f;

        sp28 = i_this->field_0x5c0 - i_this->current.pos;
        sp28.y += 50.0f;

        i_this->current.angle.y = cM_atan2s(sp28.x, sp28.z);
        i_this->current.angle.x =
            -cM_atan2s(sp28.y, JMAFastSqrt(sp28.x * sp28.x + sp28.z * sp28.z));

        if (sp28.abs() < 30.0f) {
            i_this->mMode = 2;
            i_this->mTimers[1] = 20;
        }
        break;
    case 2:
        i_this->field_0x5c0 = sp34;
        if (i_this->mTimers[1] != 0) {
            cLib_addCalc2(&i_this->current.pos.x, i_this->field_0x5c0.x, 0.1f, 5.0f);
            cLib_addCalc2(&i_this->current.pos.y, i_this->field_0x5c0.y, 0.1f, 5.0f);
            cLib_addCalc2(&i_this->current.pos.z, i_this->field_0x5c0.z, 0.1f, 5.0f);

            if (i_this->mTimers[1] == 1) {
                anm_init(i_this, BCK_WAIT1, 3.0f, J3DFrameCtrl::EMode_LOOP, 0.0f);
            }
        } else {
            i_this->current.pos = i_this->field_0x5c0;
        }

        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mAngleToPlayer, 2, 0x1000);
        if (i_this->field_0x5fe != 0) {
            i_this->field_0x5fe = 0;
            i_this->mAction = ACTION_DROP_e;
            i_this->mMode = 0;
            i_this->speedF = 0.0f;
        }
        break;
    }

    return var_r30;
}

static void ni_message(ni_class* i_this) {
    switch (i_this->mMode) {
    case 1:
        break;
    case 0:
        anm_init(i_this, BCK_WAIT1, 3.0f, J3DFrameCtrl::EMode_LOOP, 0.0f);
        i_this->mMode++;
        break;
    }

    cLib_addCalc0(&i_this->speedF, 1.0f, 2.0f);
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mAngleToPlayer, 2, 0x1000);

    if (!dComIfGp_event_runCheck() && i_this->mDistToPlayer > 400.0f) {
        i_this->mAction = ACTION_NORMAL_e;
        i_this->mMode = 0;
    }
}

static BOOL playwallcheck(ni_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    dBgS_LinChk lin_chk;
    f32 x_diff = a_this->current.pos.x - a_this->old.pos.x;
    f32 z_diff = a_this->current.pos.z - a_this->old.pos.z;

    if (x_diff * x_diff + z_diff * z_diff > 0.0f) {
        cXyz offset;
        cXyz line_start;
        cXyz line_end;
        line_start = a_this->current.pos;
        line_start.y += 100.0f;

        cMtx_YrotS(*calc_mtx, cM_atan2s(x_diff, z_diff));
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 50.0f;
        MtxPosition(&offset, &line_end);
        line_end += line_start;

        for (int i = 0; i < 2; i++) {
            lin_chk.Set(&line_start, &line_end, a_this);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                a_this->current.pos.x = a_this->old.pos.x;
                a_this->current.pos.z = a_this->old.pos.z;
                a_this->speedF = 0.0f;
                return true;
            }

            line_start.y += 50.0f;
            line_end.y += 50.0f;
        }
    }

    return false;
}

static int ni_play(ni_class* i_this) {
    cXyz sp78;
    cXyz sp84;

    int var_r30 = 0;
    int var_r29 = 1;
    s16 var_r28 = 0x4000;

    i_this->mPadMainStickX = mDoCPd_c::getStickX3D(PAD_1);
    i_this->mPadMainStickY = mDoCPd_c::getStickY(PAD_1);
    i_this->mPadSubStickY = mDoCPd_c::getSubStickY(PAD_1);
    i_this->mPadSubStickX = mDoCPd_c::getSubStickX(PAD_1);
    i_this->mPadAnalogL = mDoCPd_c::getAnalogL(PAD_1);
    i_this->mPadHoldL = mDoCPd_c::getHoldL(PAD_1);

    if (fabsf(i_this->mPadMainStickX) < 0.05f) {
        i_this->mPadMainStickX = 0.0f;
    }

    if (fabsf(i_this->mPadMainStickY) < 0.05f) {
        i_this->mPadMainStickY = 0.0f;
    }

    if (fabsf(i_this->mPadSubStickY) < 0.05f) {
        i_this->mPadSubStickY = 0.0f;
    }

    if (fabsf(i_this->mPadSubStickX) < 0.05f) {
        i_this->mPadSubStickX = 0.0f;
    }

    switch (i_this->mMode) {
    case 0:
        i_this->mPlayAnmSpeed = 0.0f;
        anm_init(i_this, BCK_WALK_A, 5.0f, J3DFrameCtrl::EMode_LOOP, i_this->mPlayAnmSpeed);
        i_this->mMode++;
        // fallthrough
    case 1:
        if (i_this->mpMorf->checkFrame(1.0f) || i_this->mpMorf->checkFrame(16.0f)) {
            i_this->mSound.startSound(Z2SE_CHICKEN_WALK, 0, -1);
        }

        if (i_this->speedF > 10.0f) {
            i_this->mPlayAnmSpeed = 1.0f;
            anm_init(i_this, BCK_WALK_B, 3.0f, J3DFrameCtrl::EMode_LOOP, i_this->mPlayAnmSpeed);
            i_this->mMode++;
        }
        break;
    case 2:
        if (i_this->mpMorf->checkFrame(1.0f) || i_this->mpMorf->checkFrame(8.0f)) {
            i_this->mSound.startSound(Z2SE_CHICKEN_WALK, 0, -1);
        }

        if (i_this->speedF < 10.0f) {
            i_this->mPlayAnmSpeed = 1.5f;
            anm_init(i_this, BCK_WALK_A, 5.0f, J3DFrameCtrl::EMode_LOOP, i_this->mPlayAnmSpeed);
            i_this->mMode = 1;
        }
        break;
    case 5:
        if (i_this->mpMorf->isStop()) {
            i_this->mMode = 0;
        }
        break;
    case 10:
        var_r28 = 0x800;

        if (mDoCPd_c::getHoldA(PAD_1)) {
            i_this->gravity = -1.0f;
            if (i_this->speed.y < -10.0f) {
                i_this->speed.y = -10.0f;
            }
        } else {
            i_this->gravity = -2.0f;
        }

        if (i_this->mAcch.ChkGroundHit() || i_this->field_0x848) {
            i_this->mMode = 0;
            i_this->field_0xaf0 = 0.0f;
        }

        var_r29 = 0;
        break;
    case 20:
        var_r30 = 1;
        var_r28 = 0x400;

        if (i_this->field_0x848 == 0) {
            i_this->mMode = 0;
        }
        break;
    case 30:
        var_r30 = 1;
        i_this->current.angle.z = cM_ssin(i_this->mCounter * 9000) * 3000.0f;

        if (i_this->field_0x60c == 0) {
            i_this->mMode = 1;
            anm_init(i_this, BCK_WAIT1, 3.0f, J3DFrameCtrl::EMode_LOOP, 0.0f);
        }
        break;
    }

    sp78.x = i_this->mPadMainStickX;
    sp78.y = 0.0f;
    sp78.z = -i_this->mPadMainStickY;

    camera_class* camera = dComIfGp_getCamera(0);
    sp84 = camera->lookat.center - camera->lookat.eye;

    cMtx_YrotS(*calc_mtx, cM_atan2s(-sp84.x, -sp84.z));
    MtxPosition(&sp78, &sp84);
    f32 var_f31 = sp84.x;
    f32 var_f30 = sp84.z;

    if (fabsf(var_f31) >= 0.1f || fabsf(var_f30) >= 0.1f) {
        cLib_addCalcAngleS2(&i_this->current.angle.y, cM_atan2s(var_f31, var_f30), 2, var_r28);
    }

    f32 temp_f1 = JMAFastSqrt(var_f31 * var_f31 + var_f30 * var_f30);
    if (temp_f1 > 0.3f) {
        cLib_addCalc2(&i_this->field_0xabc, temp_f1, 1.0f, 0.2f);
    } else {
        cLib_addCalc0(&i_this->field_0xabc, 1.0f, 0.2f);
    }

    f32 var_f30_2 = i_this->field_0xabc * (i_this->field_0xabc * i_this->field_0xabc);
    if (var_f30_2 > 1.0f) {
        var_f30_2 = 1.0f;
    }

    if (var_r30 != 0) {
        cLib_addCalc2(&i_this->speedF, var_f30_2 * 2.0f, 1.0f, 0.05f);
    } else {
        f32 var_f1_2 = var_f30_2;
        if (i_this->field_0xa04 <= -0x2000) {
            var_f1_2 = 0.0f;
        } else if (i_this->field_0xa04 <= -0x1800) {
            var_f1_2 = var_f30_2 / 4;
        } else if (i_this->field_0xa04 <= -0x1000) {
            var_f1_2 = var_f30_2 / 2;
        } else if (i_this->field_0xa04 <= -0x800) {
            f32 temp = 0.75f;  // temp needed for reg alloc
            var_f1_2 = var_f30_2 * temp;
        }

        i_this->speedF = var_f1_2 * 15.0f;
    }

    if (i_this->mMode < 5) {
        if (i_this->mAnmID == BCK_WALK_A) {
            i_this->mPlayAnmSpeed = var_f30_2 * 5.0f;
        } else {
            i_this->mPlayAnmSpeed = var_f30_2 * 2.0f;
        }

        i_this->mpMorf->setPlaySpeed(i_this->mPlayAnmSpeed);

        if (i_this->speed.y <= -15.0f) {
            i_this->speed.y = 0.0f;
            i_this->mMode = 10;
            anm_init(i_this, BCK_MOGAKU, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            hane_set(i_this, 5.0f, 25.0f);
        }
    }

    if (i_this->mMode < 20 && i_this->field_0x848 != 0) {
        i_this->mMode = 20;
        anm_init(i_this, BCK_WAIT1, 3.0f, J3DFrameCtrl::EMode_LOOP, 0.0f);
        i_this->speedF *= 0.5f;
    }

    if ((var_r29 != 0 || var_r30 != 0) && mDoCPd_c::getTrigA(PAD_1)) {
        i_this->speed.y = 20.0f;
        i_this->mMode = 10;
        anm_init(i_this, BCK_MOGAKU, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        hane_set(i_this, 5.0f, 25.0f);
    }

    if (i_this->field_0x560 != 0) {
        i_this->field_0x560 = 0;

        if (i_this->field_0x60c == 0) {
            i_this->field_0x60c = cM_rndF(10.0f) + 5.0f;
            i_this->mMode = 30;
            anm_init(i_this, BCK_MOGAKU, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mSound.startSound(Z2SE_CHICKEN_CRY, 0, -1);
            hane_set(i_this, 5.0f, 25.0f);
        }
    }

    if (fabsf(i_this->current.pos.y - i_this->mAcch.GetGroundH()) < 5.0f) {
        i_this->field_0xa00 = i_this->mAcch.GetGroundH();
    } else if (fabsf(i_this->current.pos.y - i_this->mSplGroundH) < 40.0f) {
        i_this->field_0xa00 = i_this->mSplGroundH;
    }

    if (i_this->field_0xa00 >= i_this->current.pos.y) {
        i_this->field_0xa00 = i_this->current.pos.y;
    }

    cMtx_YrotS(*calc_mtx, i_this->shape_angle.y);
    sp78.x = 0.0f;
    sp78.y = 50.0f;
    sp78.z = 10.0f;
    MtxPosition(&sp78, &sp84);
    sp84 += i_this->current.pos;

    if (fopAcM_gc_c::gndCheck(&sp84) != 0) {
        sp84.y = fopAcM_gc_c::getGroundY();
        sp78 = sp84 - i_this->current.pos;

        i_this->field_0xa04 = -cM_atan2s(sp78.y, JMAFastSqrt(sp78.x * sp78.x + sp78.z * sp78.z));
        if (i_this->field_0xa04 > 0) {
            i_this->field_0xa04 = 0;
        }
    } else {
        i_this->field_0xa04 = 0;
    }

    return var_r30;
}

static void play_camera(ni_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = (camera_class*)dComIfGp_getCamera(0);
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp1D8;
    cXyz sp1E4;
    cXyz sp1F0;
    cXyz sp1FC;

    int var_r27 = 0;

    f32 var_f31;
    f32 var_f29;
    f32 var_f28;
    f32 var_f27;
    f32 temp;
    f32 var_f3;
    f32 temp_f30_2;
    f32 temp_f1_4;
    f32 analog_l = i_this->mPadAnalogL;

    BOOL hold_l = i_this->mPadHoldL;
    cXyz sp208(0.0f, 0.0f, 0.0f);

    int var_r25 = 0;
    switch (i_this->mPlayCamMode) {
    case 0:
        i_this->mPlayCamMode = 1;
        i_this->field_0xa00 = a_this->current.pos.y;
        // fallthrough
    case 1:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        i_this->mPlayCamMode = 2;
        i_this->mPlayCamModeTimer = 0;
        camera->mCamera.Stop();

        i_this->field_0xafc = 500.0f;
        i_this->field_0xac8 = camera0->lookat.eye;
        i_this->field_0xad4 = camera0->lookat.center;
        i_this->field_0xaec = dComIfGd_getView()->fovy;

        player->changeOriginalDemo();
        player->changeDemoMode(1, 1, 0, 0);

        mDoGph_gInf_c::fadeOut(0.1f, g_blackColor);
        // fallthrough
    case 2:
        if (i_this->mPlayCamModeTimer != 30) {
            break;
        }

        mDoGph_gInf_c::fadeOut(-0.05f, g_blackColor);
        i_this->mPlayCamMode = 3;
        i_this->mPlayCamModeTimer = 0;
        // fallthrough
    case 3: {
        var_r25 = 1;
        i_this->field_0xaec = 55.0f;
        sp1D8 = i_this->field_0xac8 - a_this->current.pos;

        s16 var_r4_2 = cM_atan2s(-sp1D8.x, -sp1D8.z);
        var_f31 = TREG_F(12) + 0.3f;
        var_f29 = i_this->field_0xafc;
        var_f28 = 180.0f;
        var_f27 = 100.0f;

        if (analog_l > TREG_F(12) + 0.5f) {
            var_r4_2 = a_this->shape_angle.y;
            var_f31 = NREG_F(5) + 0.5f;

            if (hold_l) {
                var_r27 = 1;
                var_f29 = 300.0f;
                var_f28 = 230.0f;
                var_f27 = 150.0f;
                i_this->field_0xafc = 500.0f;
            }
        } else {
            temp = i_this->mPadSubStickX;

            temp *= TREG_F(3) + 5000.0f;
            i_this->field_0xaf4 += (s16)temp;
            i_this->field_0xafc += i_this->mPadSubStickY * (TREG_F(7) + -25.0f);

            if (i_this->field_0xafc > (TREG_F(8) + 800.0f)) {
                i_this->field_0xafc = (TREG_F(8) + 800.0f);
            } else if (i_this->field_0xafc < (TREG_F(9) + 300.0f)) {
                i_this->field_0xafc = (TREG_F(9) + 300.0f);
            }
        }

        cLib_addCalcAngleS2(&i_this->field_0xaf4, var_r4_2, 2, 0x2000);
        cLib_addCalc2(&i_this->field_0xaf8, var_f29, 0.5f, 30.0f);
        cLib_addCalc2(&i_this->field_0xb00, var_f28, 0.5f, 5.0f);
        cLib_addCalc2(&i_this->field_0xb04, var_f27, 0.5f, 5.0f);

        cMtx_YrotS(*calc_mtx, i_this->field_0xaf4);
        sp1D8.x = 0.0f;
        sp1D8.y = 0.0f;
        sp1D8.z = -i_this->field_0xaf8;
        MtxPosition(&sp1D8, &sp1F0);
        sp1F0 += a_this->current.pos;

        cLib_addCalc2(&i_this->field_0xac8.x, sp1F0.x, var_f31, 1000.0f);
        cLib_addCalc2(&i_this->field_0xac8.z, sp1F0.z, var_f31, 1000.0f);

        if (i_this->field_0xac8.y < sp1F0.y) {
            var_f3 = 1000.0f;
        } else {
            var_f31 = 0.1f;
            var_f3 = 10.0f;
        }
        cLib_addCalc2(&i_this->field_0xac8.y, sp1F0.y, var_f31, var_f3);

        sp1FC = a_this->current.pos;
        sp1FC.y = i_this->field_0xb04 +
                  (i_this->field_0xa00 +
                   ((a_this->current.pos.y - i_this->field_0xa00) * (AREG_F(11) + 0.3f)));
        i_this->field_0xad4.y = sp1FC.y;

        cLib_addCalc2(&i_this->field_0xaf0, 1.0f, 1.0f, NREG_F(8) + 0.01f);
        cLib_addCalc2(&i_this->field_0xad4.x, sp1FC.x, NREG_F(4) + 0.3f, 200.0f);
        cLib_addCalc2(&i_this->field_0xad4.y, sp1FC.y, NREG_F(5) + 0.2f,
                      200.0f * i_this->field_0xaf0);
        cLib_addCalc2(&i_this->field_0xad4.z, sp1FC.z, NREG_F(4) + 0.3f, 200.0f);

        if (i_this->mPlayCamModeTimer < 2) {
            i_this->field_0xac8 = sp1F0;
            i_this->field_0xad4 = sp1FC;
        }

        if (i_this->mPlayCamModeTimer == 350 || (a_this->home.pos.y - a_this->current.pos.y) > 2000.0f) {
            i_this->mPlayCamMode = 4;
            i_this->mPlayCamModeTimer = 0;
            mDoGph_gInf_c::fadeOut(0.05f, g_blackColor);
        }
        break;
    }
    case 4:
        var_r25 = 1;
        if (i_this->mPlayCamModeTimer == 30) {
            daPy_py_c* upc_pl = (daPy_py_c*)dComIfGp_getPlayer(0);
            mDoGph_gInf_c::fadeOut(-0.05f, g_blackColor);

            cMtx_YrotS(*calc_mtx, upc_pl->shape_angle.y);
            sp1D8.x = 0.0f;
            sp1D8.y = JREG_F(1) + 200.0f;
            sp1D8.z = JREG_F(2) + -250.0f;
            MtxPosition(&sp1D8, &i_this->field_0xac8);
            i_this->field_0xac8 += upc_pl->current.pos;

            i_this->field_0xad4 = upc_pl->current.pos;
            i_this->field_0xad4.y += 120.0f;

            camera->mCamera.Reset(i_this->field_0xad4, i_this->field_0xac8, i_this->field_0xaec, 0);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();

            upc_pl->cancelOriginalDemo();

            i_this->mPlayCamMode = 0;
            i_this->mAction = ACTION_NORMAL_e;
            i_this->mMode = 0;
            a_this->health = 10;
        }
        break;
    }

    if (i_this->mPlayCamMode != 0) {
        i_this->mPlayCamModeTimer++;

        cXyz sp214(i_this->field_0xac8);
        cXyz sp220(i_this->field_0xad4);

        if (var_r25 != 0) {
            dBgS_CamLinChk cam_line_chk;
            cam_line_chk.Set(&i_this->field_0xad4, &i_this->field_0xac8, a_this);

            if (dComIfG_Bgsp().LineCross(&cam_line_chk)) {
                sp1D8.x = sp214.x - sp220.x;
                sp1D8.z = sp214.z - sp220.z;

                s16 sp8 = (s16)cM_atan2s(sp1D8.x, sp1D8.z);
                cMtx_YrotS(*calc_mtx, sp8);
                sp1D8.x = 0.0f;
                sp1D8.y = 0.0f;
                sp1D8.z = hREG_F(10) + 50.0f;
                MtxPosition(&sp1D8, &sp1E4);
                sp214 = cam_line_chk.GetCross() - sp1E4;
            }

            if (var_r27 == 0) {
                dBgS_GndChk gnd_chk;
                dBgS_ObjGndChk_Spl gnd_chk_spl;

                sp1E4.x = sp214.x;
                sp1E4.y = sp214.y + 200.0f + nREG_F(8);
                sp1E4.z = sp214.z;

                gnd_chk.SetPos(&sp1E4);
                temp_f30_2 = dComIfG_Bgsp().GroundCross(&gnd_chk);

                gnd_chk_spl.SetPos(&sp1E4);
                temp_f1_4 = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);

                if (temp_f30_2 < temp_f1_4) {
                    sp214.y = temp_f1_4;
                } else {
                    sp214.y = temp_f30_2;
                }

                sp214.y += i_this->field_0xb00;
                if (sp214.y < a_this->current.pos.y + nREG_F(9)) {
                    sp214.y = a_this->current.pos.y + nREG_F(9);
                }
            } else {
                sp214.y += NREG_F(18) + 250.0f;
            }
        }

        camera->mCamera.Set(sp220, sp214, i_this->field_0xaec, 0);
    }
}

static void action(ni_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp148;
    cXyz sp154;

    a_this->gravity = -5.0f;
    i_this->mCcSph.OnCoSetBit();

    i_this->mDistToPlayer = fopAcM_searchPlayerDistance(a_this);
    i_this->mAngleToPlayer = fopAcM_searchPlayerAngleY(a_this);
    a_this->current.angle.z = 0;

    damage_check(i_this);

    BOOL var_r27 = false;
    BOOL var_r26 = false;
    BOOL var_r25 = false;
    BOOL var_r24 = false;
    BOOL update_position = true;
    int demo_state = 1;

    if (i_this->field_0x610 < 1.0f) {
        i_this->field_0xa50 = 0;

        switch (i_this->mAction) {
        case ACTION_NORMAL_e:
            ni_normal(i_this);
            var_r27 = true;
            var_r26 = true;
            var_r25 = true;
            i_this->field_0xa50 = 1;
            break;
        case ACTION_AWAY_e:
            ni_away(i_this);
            var_r27 = true;
            var_r26 = true;
            var_r25 = true;
            break;
        case ACTION_SWIM_e:
            ni_swim(i_this);
            i_this->field_0xa50 = 1;
            var_r25 = true;
            var_r24 = true;
            break;
        case ACTION_CARRY_e:
            update_position = ni_carry(i_this);
            break;
        case ACTION_FLY_e:
            ni_fly(i_this);
            var_r25 = true;
            break;
        case ACTION_DROP_e:
            ni_drop(i_this);
            var_r25 = true;
            break;
        case ACTION_RETURN_e:
            ni_return(i_this);
            break;
        case ACTION_DAMAGE_e:
            ni_damage(i_this);
            var_r27 = true;
            var_r26 = true;
            break;
        case ACTION_WINDSPIN_e:
            ni_windspin(i_this);
            var_r25 = true;
            break;
        case ACTION_DEMO0_e:
            demo_state = ni_demo0(i_this);
            break;
        case ACTION_MESSAGE_e:
            ni_message(i_this);
            var_r27 = true;
            var_r25 = true;
            var_r26 = true;
            i_this->field_0xa50 = 2;
            break;
        case ACTION_PLAY_e:
            var_r24 = ni_play(i_this);
            break;
        }

        if (var_r27 && a_this->speed.y <= -15.0f) {
            i_this->mAction = ACTION_DROP_e;
            i_this->mMode = 0;
            a_this->speed.y = 0.0f;
            var_r25 = true;
        }

        if (i_this->field_0x9e4 == 0 && var_r26 && i_this->field_0x848 != 0) {
            i_this->mAction = ACTION_SWIM_e;
            i_this->mMode = 0;
            i_this->field_0x9e4 = 30;
            a_this->speedF *= 0.5f;
            var_r25 = true;
        }

        if (i_this->field_0xa50 == 1 && daPy_py_c::checkNowWolf() &&
            i_this->mDistToPlayer < 300.0f)
        {
            i_this->mAction = ACTION_MESSAGE_e;
            i_this->mMode = 0;
            var_r25 = true;
        }

        if (var_r25) {
            cLib_onBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
            ni_carry_check(i_this);
        } else {
            cLib_offBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
        }
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x2000);
    a_this->shape_angle.z = a_this->current.angle.z;

    if (i_this->mAcch.ChkWallHit()) {
        a_this->speedF *= 0.5f;
        i_this->field_0x610 = 0.0f;
    }

    if (demo_state == 1) {
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        sp148.x = 0.0f;
        sp148.y = 0.0f;
        sp148.z = a_this->speedF;

        MtxPosition(&sp148, &sp154);

        if (i_this->field_0x610 > 1.0f) {
            cXyz sp160;
            sp148.x = 0.0f;
            sp148.y = 0.0f;
            sp148.z = -i_this->field_0x610;

            cMtx_YrotS(*calc_mtx, i_this->field_0x614);
            MtxPosition(&sp148, &sp160);
            sp154 += sp160;
        }

        i_this->speed.x = sp154.x;
        i_this->speed.z = sp154.z;
        a_this->current.pos += a_this->speed;

        a_this->speed.y += a_this->gravity;
        if (a_this->speed.y < -80.0f) {
            a_this->speed.y = -80.0f;
        }

        cLib_addCalc0(&i_this->field_0x5d8, 1.0f, 5.0f);
    } else if (demo_state == 2) {
        cMtx_YrotS(*calc_mtx, i_this->current.angle.y);
        cMtx_XrotM(*calc_mtx, i_this->current.angle.x);

        sp148.x = 0.0f;
        sp148.y = 0.0f;
        sp148.z = a_this->speedF;

        MtxPosition(&sp148, &sp154);
        a_this->current.pos += sp154;

        sp148 = i_this->field_0x5c0 - i_this->mDemoInitPos;
        f32 var_f31 = sp148.abs();

        sp148 = i_this->field_0x5c0 - a_this->current.pos;
        f32 var_f1 = sp148.abs();

        f32 var_f3 = var_f31 * 0.2f;
        if (var_f3 > 100.0f) {
            var_f3 = 100.0f;
        }

        i_this->field_0x5d8 = var_f3 * cM_ssin((var_f1 / var_f31) * (f32)0x8000);
    } else {
        cLib_addCalc0(&i_this->field_0x5d8, 1.0f, 5.0f);
    }

    if (i_this->field_0x610 > 1.0f) {
        cLib_addCalc0(&i_this->field_0x610, 1.0f, 5.0f);
        a_this->speedF = 0.0f;
        hane_set(i_this, 1.0f, 25.0f);
    }

    if (!fopAcM_checkCarryNow(a_this)) {
        cXyz* cc_move = i_this->mCcStts.GetCCMoveP();

        if (cc_move != NULL) {
            a_this->current.pos.x += cc_move->x * 0.5f;
            a_this->current.pos.z += cc_move->z * 0.5f;
        }
    }

    if (update_position) {
        if (i_this->mAction == ACTION_PLAY_e) {
            playwallcheck(i_this);
        } else if (strcmp(dComIfGp_getStartStageName(), "F_SP103") == 0) {
            sp148.x = 1076.0f - a_this->current.pos.x;
            sp148.z = 4128.0f - a_this->current.pos.z;

            if (sp148.x * sp148.x + sp148.z * sp148.z < 50000.0f) {
                playwallcheck(i_this);
            }
        }

        i_this->mAcch.CrrPos(dComIfG_Bgsp());
        if (i_this->mAcch.ChkGroundHit()) {
            a_this->home.pos.y = a_this->current.pos.y;
        }
    }

    if (!(i_this->mCounter & 3)) {
        if (cM_rndF(1.0f) < 0.3f) {
            i_this->field_0x620 = cM_rndFX(5000.0f);
        }

        if (cM_rndF(1.0f) < 0.3f) {
            i_this->field_0x61e = cM_rndFX(1000.0f);
        }

        if (cM_rndF(1.0f) < 0.3f) {
            i_this->field_0x622 = cM_rndFX(1000.0f);
        }

        if (cM_rndF(1.0f) < 0.3f) {
            i_this->field_0x628 = cM_rndFX(1000.0f);
        }

        if (cM_rndF(1.0f) < 0.3f) {
            i_this->field_0x62a = cM_rndFX(1000.0f);
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0x61a, i_this->field_0x620, 2, 0x1000);
    cLib_addCalcAngleS2(&i_this->field_0x618, i_this->field_0x61e, 2, 0x1000);
    cLib_addCalcAngleS2(&i_this->field_0x61c, i_this->field_0x622, 2, 0x1000);
    cLib_addCalcAngleS2(&i_this->field_0x626, i_this->field_0x62a, 4, 500);
    cLib_addCalcAngleS2(&i_this->field_0x624, i_this->field_0x628, 4, 500);

    for (int i = 0; i < 4; i++) {
        static f32 j_w[] = {6000.0f, 10000.0f, 6000.0f, 10000.0f};

        static f32 j_a[] = {-1000.0f, 10000.0f, -1000.0f, 10000.0f};

        if (var_r24 != 0) {
            i_this->field_0x9ee[i] =
                j_a[i] + (j_w[i] * (0.5f * a_this->speedF *
                                    cM_ssin(i_this->mCounter * 0x1000 + (s32)-(i * 0x4000))));
        } else {
            i_this->field_0x9ee[i] = 0;
        }
    }

    if (var_r24 != 0) {
        i_this->field_0x9ec =
            (a_this->speedF * (cM_ssin(i_this->mCounter * 0x1000) * 800.0f)) * 0.5f;

        cXyz hamon_pos(a_this->current.pos);
        hamon_pos.y = i_this->mSplGroundH;

        f32 emit_rate;
        if (a_this->speedF >= 10.0f) {
            emit_rate = 0.2f;
        } else {
            emit_rate = 0.05f;
        }

        fopAcM_effHamonSet(&i_this->mHamonEmtrID, &hamon_pos, 1.0f, emit_rate);
        fopAcM_riverStream(&a_this->current.pos, &i_this->field_0x9e6, &i_this->field_0x9e8, 0.5f);
    } else {
        i_this->field_0x9ec = 0;
    }

    dBgS_ObjGndChk_Spl gnd_chk_spl;
    i_this->field_0x848 = 0;

    dBgS_LinChk line_chk;
    sp148.x = a_this->current.pos.x;
    sp148.y = a_this->current.pos.y + 3000.0f;
    sp148.z = a_this->current.pos.z;
    line_chk.Set(&a_this->current.pos, &sp148, a_this);

    if (dComIfG_Bgsp().LineCross(&line_chk)) {
        sp148.y = line_chk.GetCross().y - 10.0f;
    }

    gnd_chk_spl.SetPos(&sp148);
    i_this->mSplGroundH = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);

    if (a_this->current.pos.y <= i_this->mSplGroundH - 30.0f) {
        cLib_addCalc2(&a_this->current.pos.y, i_this->mSplGroundH - 30.0f, 0.5f, 5.0f);

        if (a_this->speed.y <= -10.0f) {
            i_this->field_0xa06 = 1;
        }

        a_this->speed.y = 0.0f;
        i_this->field_0x848 = 1;
    }

    if (i_this->field_0xa06) {
        cXyz sp178(a_this->current.pos);
        sp178.y = i_this->mSplGroundH;

        static cXyz sc(0.5f, 0.5f, 0.5f);

        static u16 w_eff_id[] = {0x01B8, 0x01B9, 0x01BA, 0x01BB};

        for (int i = 0; i < 4; i++) {
            i_this->field_0xa08[i] =
                dComIfGp_particle_set(i_this->field_0xa08[i], w_eff_id[i], &sp178, &a_this->tevStr,
                                      0, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
        }

        i_this->mSound.startSound(Z2SE_CM_BODYFALL_WATER_S, 0, -1);
        i_this->field_0xa06 = 0;
    }
}

static int message(ni_class* i_this) {
    if (i_this->field_0xa52) {
        i_this->field_0x60c = 10;

        if (i_this->mMsgFlow.doFlow(i_this, NULL, 0)) {
            dComIfGp_event_reset();
            i_this->field_0xa52 = 0;
        }
        return 1;
    }

    if (dComIfGp_event_runCheck() && i_this->eventInfo.checkCommandTalk()) {
        i_this->mMsgFlow.init(i_this, i_this->mFlowNo, 0, NULL);
        i_this->field_0xa52 = 1;
        OS_REPORT("////////NI MSG FNO %d\n", i_this->mFlowNo);
    }

    if (i_this->field_0xa50 == 2 && i_this->mFlowNo != -1 && daPy_py_c::checkNowWolf()) {
        fopAcM_OnStatus(i_this, 0);
        cLib_onBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
        i_this->eventInfo.onCondition(1);
    } else {
        fopAcM_OffStatus(i_this, 0);
        cLib_offBit<u32>(i_this->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    }

    return 0;
}

static int daNi_Execute(ni_class* i_this) {
    if (cDmrNowMidnaTalk() && !fopAcM_checkCarryNow(i_this)) {
        return 1;
    }

    cXyz sp30(0.0f, 0.0f, 0.0f);
    cXyz eno_pos;

    i_this->mCounter++;
    i_this->field_0xa50 = 0;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->field_0x60c != 0) {
        i_this->field_0x60c--;
    }

    if (i_this->field_0x60e != 0) {
        i_this->field_0x60e--;
    }

    if (i_this->field_0x9e4 != 0) {
        i_this->field_0x9e4--;
    }

    action(i_this);

    J3DModel* model = i_this->mpMorf->getModel();
    if (i_this->field_0xb08) {
        fopAcM_OnCarryType(i_this, fopAcM_CARRY_CHICKEN);
        i_this->field_0xb08 = 0;
    } else {
        mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y + i_this->field_0x5d8,
                               i_this->current.pos.z);
        mDoMtx_stack_c::YrotM(i_this->shape_angle.y + i_this->field_0x9ec);
        mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
        mDoMtx_stack_c::scaleM(l_HIO.mBaseSize, l_HIO.mBaseSize, l_HIO.mBaseSize);
        model->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    i_this->mpMorf->play(&i_this->eyePos, 0, 0);
    i_this->mpBtk->setFrame(i_this->mColor);
    i_this->mpMorf->modelCalc();

    MTXCopy(model->getAnmMtx(5), *calc_mtx);
    MtxPosition(&sp30, &i_this->eyePos);
    i_this->attention_info.position = i_this->eyePos;
    i_this->attention_info.position.y += 30.0f;

    if (i_this->mAction != ACTION_DEMO0_e) {
        i_this->mCcSph.SetC(i_this->eyePos);
        i_this->mCcSph.SetR(l_HIO.mBaseSize * 23.0f);
        dComIfG_Ccsp()->Set(&i_this->mCcSph);
    }

    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));

    if (i_this->mAnmID == BCK_MOGAKU || i_this->mAnmID == BCK_FLY || i_this->mAnmID == BCK_FALL) {
        if (i_this->mpMorf->checkFrame(1) || i_this->mpMorf->checkFrame(5) ||
            i_this->mpMorf->checkFrame(9) || i_this->mpMorf->checkFrame(13) ||
            i_this->mpMorf->checkFrame(17) || i_this->mpMorf->checkFrame(21) ||
            i_this->mpMorf->checkFrame(25))
        {
            i_this->mSound.startSound(Z2SE_CHICKEN_WING, 0, -1);
        }

        if (i_this->mpMorf->checkFrame(1)) {
            i_this->mSound.startSound(Z2SE_CHICKEN_CRY, 0, -1);
        }
    }

    message(i_this);

    if (i_this->mAction == ACTION_PLAY_e) {
        play_camera(i_this);
    }

    if (i_this->mColor == COLOR_GOLD) {
        eno_pos = i_this->current.pos;
        eno_pos.y += 40.0f;

        static u16 key_eno[] = {0x827A, 0x827B};

        for (int i = 0; i < 2; i++) {
            i_this->mEnoEmtrIDs[i] =
                dComIfGp_particle_set(i_this->mEnoEmtrIDs[i], key_eno[i], &eno_pos, NULL, NULL);
        }
    }

    return 1;
}

static int daNi_IsDelete(ni_class* i_this) {
    return 1;
}

static int daNi_Delete(ni_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "Ni");

    if (i_this->mInitHIO) {
        data_8095158d = 0;
    }

    if (i_this->heap != NULL) {
        i_this->mSound.deleteObject();
    }

    return 1;
}

static BOOL useHeapInit(fopAc_ac_c* i_this) {
    ni_class* a_this = (ni_class*)i_this;

    a_this->mpMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("Ni", 16), NULL, NULL,
                                        (J3DAnmTransform*)dComIfG_getObjectRes("Ni", 11), 0, 1.0f,
                                        0, -1, 1, NULL, 0x80000, 0x11000284);

    if (a_this->mpMorf == NULL || a_this->mpMorf->getModel() == NULL) {
        return false;
    }

    J3DModel* model = a_this->mpMorf->getModel();
    a_this->model = model;
    model->setUserArea((uintptr_t)a_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    a_this->mpBtk = new mDoExt_btkAnm();
    if (a_this->mpBtk == NULL) {
        return false;
    }

    if (!a_this->mpBtk->init(a_this->mpMorf->getModel()->getModelData(),
                             (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Ni", 0x13), TRUE,
                             J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1))
    {
        return false;
    }

    a_this->mSound.init(&a_this->current.pos, 2);
    return true;
}

static int daNi_Create(fopAc_ac_c* i_this) {
    ni_class* a_this = (ni_class*)i_this;
    fopAcM_ct(a_this, ni_class);

    int phase_state = dComIfG_resLoad(&a_this->mPhase, "Ni");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("NI PARAM %x\n", fopAcM_GetParam(a_this));

        a_this->field_0x5b4 = fopAcM_GetParam(a_this);
        a_this->mType = fopAcM_GetParam(a_this) >> 8;

        if (a_this->mType == COLOR_RANDOM) {
            a_this->mColor = cM_rndF(2.999f);
        } else {
            a_this->mColor = a_this->mType;
            if (a_this->mColor > COLOR_GOLD) {
                a_this->mColor = COLOR_GOLD;
            }
        }

        u32 params = fopAcM_GetParam(i_this);
        params &= 0xFFFF00FF;
        params |= (a_this->mColor << 8);
        fopAcM_SetParam(i_this, params);

        a_this->mFlowNo = i_this->current.angle.x;
        i_this->shape_angle.x = 0;
        i_this->current.angle.x = 0;

        OS_REPORT("NI//////////////NI SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0xDE0)) {
            OS_REPORT("//////////////NI SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("NI//////////////NI SET 2 !!\n");

        if (data_8095158d == 0) {
            a_this->mInitHIO = true;
            data_8095158d = 1;
            l_HIO.field_0x04 = -1;
        }

        i_this->attention_info.flags = 0;
        a_this->mAction = ACTION_NORMAL_e;
        fopAcM_SetMtx(i_this, a_this->mpMorf->getModel()->getBaseTRMtx());

        a_this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1,
                          &a_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->mAcchCir.SetWall(55.0f, 45.0f);

        a_this->field_0x560 = 0;
        i_this->health = 10;

        a_this->mCcStts.Init(10, 0, i_this);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}},  // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                 // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                 // mGObjTg
                {0x0},                                              // mGObjCo
            },                                                      // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 20.0f}  // mSph
            }                                // mSphAttr
        };

        a_this->mCcSph.Set(cc_sph_src);
        a_this->mCcSph.SetStts(&a_this->mCcStts);

        if (a_this->checkGold()) {
            a_this->mCcSph.OffTgSetBit();
        }

        i_this->attention_info.distances[fopAc_attn_CARRY_e] = 7;
        a_this->mCounter = cM_rndF(0x10000);

        cXyz sp34(i_this->current.pos);
        sp34.y += 50.0f;

        if (fopAcM_gc_c::gndCheck(&sp34)) {
            i_this->current.pos.y = fopAcM_gc_c::getGroundY();
            i_this->old = i_this->current;
        }

        if (strcmp(dComIfGp_getStartStageName(), "F_SP108") == 0 &&
             /* dSv_event_flag_c::F_0528 - Faron Woods - Resistance Rusl summoned golden cucco */
            !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[528]))
        {
            a_this->mAction = ACTION_DEMO0_e;
            a_this->mTimers[0] = 10;
        }

        a_this->mSplGroundH = -G_CM3D_F_INF;

        daNi_Execute(a_this);
    }

    return phase_state;
}

AUDIO_INSTANCES;

static actor_method_class l_daNi_Method = {
    (process_method_func)daNi_Create,
    (process_method_func)daNi_Delete,
    (process_method_func)daNi_Execute,
    (process_method_func)daNi_IsDelete,
    (process_method_func)daNi_Draw,
};

actor_process_profile_definition g_profile_NI = {
  fpcLy_CURRENT_e,          // mLayerID
  7,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_NI,                  // mProcName
  &g_fpcLf_Method.base,     // sub_method
  sizeof(ni_class),         // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  695,                      // mPriority
  &l_daNi_Method,           // sub_method
  0x80c4000,                // mStatus
  fopAc_NPC_e,              // mActorType
  fopAc_ACTOR_e,            // cullType
};
