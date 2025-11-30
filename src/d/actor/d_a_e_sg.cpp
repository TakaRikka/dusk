/**
* @file d_a_e_sg.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_sg.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_mg_rod.h"
#include "d/actor/d_a_obj_kbox.h"
#include "d/actor/d_a_player.h"

enum Action {
    /* 0x0 */ ACT_MOVE,
    /* 0x1 */ ACT_SEARCH,
    /* 0x2 */ ACT_KAMU,
    /* 0x3 */ ACT_DROP,
    /* 0x5 */ ACT_B_SEARCH = 0x5,
    /* 0x7 */ ACT_ESA_SEARCH = 0x7,
    /* 0xA */ ACT_DAMAGE = 0xA,
};

enum Mode {
    /* 0x0 */ MODE_IDLE,
    /* 0x1 */ MODE_ACTIVE,
    /* 0x5 */ MODE_SLOW = 0x5,
    /* 0xA */ MODE_FOLLOW = 0xA,
    /* 0xB */ MODE_TARGET = 0xB,
};

static int stick_bit[32] = {1 << 0,  1 << 1,  1 << 2,  1 << 3,  1 << 4,  1 << 5,  1 << 6,  1 << 7,
                            1 << 8,  1 << 9,  1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14, 1 << 15,
                            1 << 16, 1 << 17, 1 << 18, 1 << 19, 1 << 20, 1 << 21, 1 << 22, 1 << 23,
                            1 << 24, 1 << 25, 1 << 26, 1 << 27, 1 << 28, 1 << 29, 1 << 30, 1 << 31};

typedef struct {
    s8 joint_no;
    s8 y;
    s8 z;
} stick_info;
static stick_info stick_d[19] = {
    {0x22, 0, 20},  {0x01, 20, 0}, {0x12, 0, 10}, {0x07, 0, -10}, {0x1C, 0, 0},
    {0x09, 0, -10}, {0x0C, 0, 10}, {0x1D, 0, 10}, {0x18, 0, 10},  {0x08, 0, -10},
    {0x13, 0, 10},  {0x0D, 0, 10}, {0x02, 20, 0}, {0x17, 0, 10},  {0x03, 20, 0},
    {0x19, 0, 10},  {0x0E, 0, 10}, {0x20, 0, 0},  {0x14, 0, 10},
};

daE_SG_HIO_c::daE_SG_HIO_c() {
    mUnk0 = -1;
    mScaleFactor = 1.2f;
    mMovementSpeed = 10.0f;
    mSearchSpeed = 13.0f;
    mPlayerHeightThreshold = 500.0f;
}

static int nodeCallBack(J3DJoint* i_joint, int i_callbackCondition) {
    if (i_callbackCondition == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_sg_class* _this = (e_sg_class*)model->getUserArea();
        MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);

        if (jnt_no >= 1 && jnt_no <= 3) {
            cMtx_YrotM(*calc_mtx, _this->mJoints[jnt_no] + (s16)_this->mJointYRot);
        }
        if (jnt_no == 5) {
            cMtx_ZrotM(*calc_mtx, _this->mJointAngle);
        }

        model->setAnmMtx(jnt_no, *calc_mtx);

        MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        if (jnt_no == 0) {
            MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
            cMtx_YrotM(*calc_mtx, 0);
            cMtx_XrotM(*calc_mtx, 0);
            cMtx_ZrotM(*calc_mtx, 0);
            model->setAnmMtx(jnt_no, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int daE_SG_Draw(e_sg_class* i_this) {
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &i_this->tevStr);
    mDoExt_modelUpdateDL(i_this->mpModel);

    if (i_this->mAction == ACT_DROP) {
        cXyz shadow_pos(i_this->current.pos.x, i_this->current.pos.y + 100.0f,
                        i_this->current.pos.z);

        _GXTexObj* tex_obj = dDlst_shadowControl_c::getSimpleTex();

        i_this->mShadowKey =
            dComIfGd_setShadow(i_this->mShadowKey, 1, i_this->mpModel, &shadow_pos, 400.0f, 0.0f,
                               i_this->current.pos.y, i_this->mAcch.GetGroundH(),
                               i_this->mAcch.m_gnd, &i_this->tevStr, 0, 1.0f, tex_obj);
    }

    return 1;
}

static bool hio_init;

static daE_SG_HIO_c l_HIO;

static int stick_pt;

static void pl_joint_search(e_sg_class* i_this) {
    if (stick_pt != 0x7ffff) {
        for (int i = 0; i < 19; i++) {
            if ((stick_pt & stick_bit[i]) == 0) {
                stick_pt |= stick_bit[i];

                i_this->mStickIdx = i + 1;
                return;
            }
        }

    } else {
        i_this->mStickIdx = -1;
    }
}

static int otherBgCheck(fopAc_ac_c* i_this, cXyz* i_end_pos, cXyz* o_cross_pos) {
    dBgS_LinChk line_check;
    cXyz start_pos;
    cXyz end_pos;

    end_pos = *i_end_pos;
    end_pos.y += 30.0f;
    start_pos = i_this->current.pos;
    line_check.Set(&start_pos, &end_pos, i_this);

    if (dComIfG_Bgsp().LineCross(&line_check)) {
        if (o_cross_pos != NULL) {
            *o_cross_pos = line_check.GetCross();
        }

        return 1;
    }

    return 0;
}

static obj_kbox_class* target_info[10];

static s8 bg_on[10];

static cXyz bg_cross[10];

static int target_info_count;

static void* s_box_sub(void* i_ac, void*) {
    if (fopAcM_IsActor(i_ac) && fopAcM_GetName(i_ac) == PROC_OBJ_KBOX && target_info_count < 10) {
        target_info[target_info_count] = (obj_kbox_class*)i_ac;
        target_info_count++;
    }

    return NULL;
}

static obj_kbox_class* search_box(e_sg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    target_info_count = 0;

    for (int i = 0; i < 10; i++) {
        target_info[i] = NULL;
        bg_on[i] = 0;
    }

    fpcM_Search(s_box_sub, actor);

    f32 radius = 100.0f;

    if (target_info_count != 0) {
        obj_kbox_class* target_ac;
        int i = 0;

        while (i < target_info_count) {
            target_ac = target_info[i];

            if (bg_on[i] || otherBgCheck(actor, &target_ac->field_0x5b4, &bg_cross[i])) {
                bg_on[i] = 1;
                cXyz diff = bg_cross[i] - target_ac->field_0x5b4;
                if (diff.abs() < 100.0f && target_ac->field_0x5b4.y < i_this->mGroundY) {
                    diff = target_ac->field_0x5b4 - i_this->home.pos;
                    if (JMAFastSqrt(diff.x * diff.x + diff.z * diff.z) < radius) {
                        return target_ac;
                    }
                }
            }

            i++;

            if (i == target_info_count) {
                i = 0;
                radius += 100.0f;
                if (radius > i_this->mSearchBound) {
                    return NULL;
                }
            }
        }
    } else {
        return NULL;
    }

    return NULL;
}

static dmg_rod_class* search_esa(e_sg_class* i_this) {
    dmg_rod_class* rod = (dmg_rod_class*)fopAcM_SearchByName(PROC_MG_ROD);

    if (rod != NULL && rod->kind == 1 && rod->action != 5 && rod->field_0x100d != 0 &&
        rod->actor.current.pos.y < rod->field_0x590 - 20.0f)
    {
        return rod;
    }

    return NULL;
}

static void e_sg_move(e_sg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* target;
    dmg_rod_class* rod;

    cXyz local_54;

    s16 max_angle_step;
    f32 max_step;
    s16 target_angle = 0;

    switch (i_this->mMode) {
    case MODE_IDLE:
        if (i_this->mTimers[0] == 0) {
            for (int i = 0; i < 0x14; i++) {
                i_this->mTargetPos.x = i_this->home.pos.x + cM_rndFX(i_this->mIdleMoveBound);
                i_this->mTargetPos.y = i_this->home.pos.y + cM_rndFX(50.0f);
                i_this->mTargetPos.z = i_this->home.pos.z + cM_rndFX(i_this->mIdleMoveBound);

                local_54 = i_this->mTargetPos - i_this->current.pos;
                local_54.y = 0.0f;
                if (local_54.abs() > i_this->mIdleMoveBound * 0.5f) {
                    i_this->mMode = MODE_ACTIVE;
                    i_this->mTimers[0] = cM_rndF(30.0f) + 30.0f;
                    break;
                }
            }
        }
        i_this->mStepSpeed = 0.1f;
        max_angle_step = 0x0;
        max_step = 0.05f;
        break;

    case MODE_ACTIVE:
        if (i_this->mTimers[0] == 0) {
            i_this->mMode = MODE_IDLE;
            i_this->mTimers[0] = cM_rndF(30.0f) + 30.0f;
        }
        i_this->mStepSpeed = 0.5f;
        max_angle_step = 0x400;
        max_step = 0.3f;
        break;

    case MODE_FOLLOW:
        i_this->mTargetPos = player->current.pos;
        i_this->mTargetPos.y = i_this->mGroundY - 20.0f;
        i_this->mStepSpeed = 0.3f;
        max_angle_step = 0x200;
        max_step = 0.2f;
        target_angle = cM_ssin(i_this->mRandomSeed * 700) * 12000.0f;
        if (i_this->mTimers[0] == 0) {
            i_this->mMode = MODE_IDLE;
        }
        i_this->mCollisionResponse = true;
        break;

    case MODE_TARGET:
        target = fopAcM_SearchByID(i_this->mTargetActorID);
        if (target != NULL) {
            i_this->mTargetPos = target->current.pos;
            i_this->mTargetPos.y = i_this->mGroundY - 20.0f;
        }
        i_this->mStepSpeed = 0.3f;
        max_angle_step = 0x200;
        max_step = 0.2f;
        target_angle = cM_ssin(i_this->mRandomSeed * 700) * 12000.0f;
        if (i_this->mTimers[0] == 0) {
            i_this->mMode = MODE_IDLE;
        }
        i_this->mCollisionResponse = true;
        break;
    }

    if (i_this->mTargetPos.y > i_this->mGroundY - 20.0f) {
        i_this->mTargetPos.y = i_this->mGroundY - 20.0f;
    }

    local_54 = i_this->mTargetPos - i_this->current.pos;
    s16 sVar1 = actor->current.angle.y;
    cLib_addCalcAngleS2(&actor->current.angle.y, target_angle + cM_atan2s(local_54.x, local_54.z),
                        4, max_angle_step);

    cLib_addCalcAngleS2(
        &actor->current.angle.x,
        -cM_atan2s(local_54.y, JMAFastSqrt(local_54.x * local_54.x + local_54.z * local_54.z)), 0x4,
        max_angle_step);

    sVar1 -= actor->current.angle.y;
    f32 fVar6 = 5.0f * sVar1;
    if (fVar6 > 4000.0f) {
        fVar6 = 4000.0f;
    } else if (fVar6 < -4000.0f) {
        fVar6 = -4000.0f;
    }

    cLib_addCalc2(&i_this->mJointYRot, fVar6, 0.5f, 1000.0f);
    cLib_addCalc2(&actor->speedF, i_this->mTargetSpeed * l_HIO.mMovementSpeed, 1.0f, max_step);

    BOOL bg_check = fopAcM_otherBgCheck(actor, player);
    if ((i_this->mRandomSeed & 0x7) == (fopAcM_GetID(actor) & 0x7)) {
        target = (fopAc_ac_c*)search_box(i_this);

        if (target != NULL) {
            i_this->mTargetActorID = fopAcM_GetID(target);
            i_this->mAction = ACT_B_SEARCH;
            i_this->mMode = MODE_IDLE;
            i_this->mRandomSeed = cM_rndF(65536.0f);
            i_this->mTimers[0] = cM_rndF(30.0f) + 30.0f;

        } else if (i_this->mTimers[1] == 0 && bg_check == 0 &&
                   player->current.pos.y - 5.0f < i_this->mGroundY &&
                   player->current.pos.y > i_this->mGroundY - l_HIO.mPlayerHeightThreshold &&
                   i_this->mTargetDist < i_this->mSearchBound)
        {
            i_this->mAction = ACT_SEARCH;
            i_this->mMode = MODE_IDLE;
            i_this->mRandomSeed = cM_rndF(65536.0f);
            pl_joint_search(i_this);

        } else if ((i_this->mRandomSeed & 0x1f) == (fopAcM_GetID(actor) & 0x1f) &&
                   cM_rndF(1.0f) < 0.3f && (rod = search_esa(i_this), rod != NULL))
        {
            i_this->mTargetActorID = fopAcM_GetID(rod);
            i_this->mAction = ACT_ESA_SEARCH;
            i_this->mMode = MODE_IDLE;
            i_this->mRandomSeed = cM_rndF(65536.0f);
        }
    }

    cLib_addCalcAngleS2(&i_this->mJointAngle, 0, 2, 1000);
    i_this->current.angle.z = 0;
    i_this->mRotationTarget = 0;
}

static void e_sg_search(e_sg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);

    s16 max_angle_step;
    f32 max_speed_step;
    switch (i_this->mMode) {
    case MODE_IDLE:
        i_this->mStepSpeed = 0.9f;
        max_angle_step = 0x600;
        max_speed_step = 0.5f;
        break;
    }

    MTXCopy(
        daPy_getLinkPlayerActorClass()->getModelJointMtx(stick_d[i_this->mStickIdx - 1].joint_no),
        *calc_mtx);

    cXyz local_74(0.0f, stick_d[i_this->mStickIdx - 1].y, stick_d[i_this->mStickIdx - 1].z);
    MtxPosition(&local_74, &i_this->mTargetPos);
    local_74 = i_this->mTargetPos - actor->current.pos;

    s16 sVar1 = i_this->current.angle.y;

    f32 fVar9 = i_this->mPlayerDist * 10.0f;
    if (fVar9 > 7000.0f) {
        fVar9 = 7000.0f;
    }

    f32 fVar7 = cM_ssin(i_this->mRandomSeed * 1200);
    f32 fVar8 = cM_ssin(i_this->mRandomSeed * 900);
    int iVar2 = fVar9 * fVar8 * 0.4f;
    cLib_addCalcAngleS2(&actor->current.angle.y,
                        (s16)(fVar9 * fVar7) + cM_atan2s(local_74.x, local_74.z), 4,
                        max_angle_step);
    fVar9 = JMAFastSqrt(local_74.x * local_74.x + local_74.z * local_74.z);
    cLib_addCalcAngleS2(&actor->current.angle.x, -(iVar2 + cM_atan2s(local_74.y, fVar9)), 4,
                        max_angle_step);

    sVar1 -= i_this->current.angle.y;
    fVar9 = sVar1 * 5.0f;
    if (fVar9 > 4000.0f) {
        fVar9 = 4000.0f;
    } else if (fVar9 < -4000.0f) {
        fVar9 = -4000.0f;
    }

    cLib_addCalc2(&i_this->mJointYRot, fVar9, 0.5f, 1000.0f);
    cLib_addCalc2(&i_this->speedF, i_this->mTargetSpeed * l_HIO.mSearchSpeed, 1.0f, max_speed_step);

    if (player->current.pos.y - 5.0f > i_this->mGroundY ||
        player->current.pos.y < i_this->mGroundY - l_HIO.mPlayerHeightThreshold ||
        dComIfGp_checkPlayerStatus0(0, 8) || fopAcM_otherBgCheck(actor, dComIfGp_getPlayer(0)))
    {
        i_this->mAction = ACT_MOVE;
        i_this->mMode = MODE_FOLLOW;
        i_this->mTimers[0] = cM_rndF(80.0f) + 80.0f;
        i_this->mTimers[1] = cM_rndF(80.0f) + 80.0f;
        if (i_this->mStickIdx != 0) {
            stick_pt &= ~stick_bit[i_this->mStickIdx - 1];
            i_this->mStickIdx = 0;
        }

    } else {
        if (local_74.abs() < 50.0f && i_this->mStickIdx > 0) {
            i_this->mAction = ACT_KAMU;
            i_this->mMode = MODE_IDLE;
            i_this->mKamuSpeed = i_this->speedF;
            i_this->mKamuTimer = 0;
        }
    }

    cLib_addCalcAngleS2(&i_this->mJointAngle, 5000, 2, 2000);
    i_this->mAttackCollistion = true;
}

static void e_sg_b_search(e_sg_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    obj_kbox_class* kbox_ac = (obj_kbox_class*)fopAcM_SearchByID(i_this->mTargetActorID);

    if (kbox_ac == NULL) {
        i_this->mAction = ACT_MOVE;
        i_this->mMode = MODE_IDLE;
        i_this->speedF = 0.0f;
        i_this->mTimers[0] = cM_rndF(60.0f);

    } else {
        cXyz local_78;

        if (i_this->mTimers[0] == 0 && (i_this->mRandomSeed & 0x7) == (fopAcM_GetID(actor) & 0x7)) {
            cXyz cross_pos;
            if (otherBgCheck(actor, &kbox_ac->field_0x5b4, &cross_pos)) {
                local_78 = cross_pos - kbox_ac->field_0x5b4;

                if (local_78.abs() > 100.0f) {
                    i_this->mAction = ACT_MOVE;
                    i_this->mMode = MODE_IDLE;
                    return;
                }
            }
        }

        f32 max_speed_step;
        f32 fVar1;
        s16 max_angle_step;

        switch (i_this->mMode) {
        case MODE_IDLE:
            i_this->mStepSpeed = 0.9f;
            max_angle_step = 0x800;
            max_speed_step = 0.5f;
            break;
        }

        i_this->mTargetPos = kbox_ac->field_0x5b4;
        i_this->mTargetPos.y += 30.0f;

        local_78 = i_this->mTargetPos - actor->current.pos;
        s16 sVar1 = actor->current.angle.y;

        fVar1 = i_this->mPlayerDist * 20.0f;
        if (fVar1 > 3000.0f) {
            fVar1 = 3000.0f;
        }

        cLib_addCalcAngleS2(&actor->current.angle.y,
                            (s16)(fVar1 * cM_ssin(i_this->mRandomSeed * 1200)) +
                                cM_atan2s(local_78.x, local_78.z),
                            4, max_angle_step);
        cLib_addCalcAngleS2(
            &actor->current.angle.x,
            -cM_atan2s(local_78.y, JMAFastSqrt(local_78.x * local_78.x + local_78.z * local_78.z)),
            4, max_angle_step);

        sVar1 -= actor->current.angle.y;
        fVar1 = sVar1 * 5.0f;
        if (fVar1 > 4000.0f) {
            fVar1 = 4000.0f;
        } else if (fVar1 < -4000.0f) {
            fVar1 = -4000.0f;
        }

        cLib_addCalc2(&i_this->mJointYRot, fVar1, 0.5f, 1000.0f);
        cLib_addCalc2(&actor->speedF, i_this->mTargetSpeed * l_HIO.mSearchSpeed, 1.0f,
                      max_speed_step);

        s16 target_angle = 5000;
        local_78 = kbox_ac->field_0x5b4 - actor->current.pos;

        if (local_78.abs() < 200.0f) {
            target_angle = cM_ssin(i_this->mRandomSeed * 0x3100) * 2000.0f + 3000.0f;

            if (i_this->mTimers[1] == 0 && i_this->mAcch.ChkWallHit()) {
                i_this->mTimers[1] = cM_rndF(30.0f) + 10.0f;
                kbox_ac->field_0x598 = (s16)(cM_rndF(700.0f) + 300.0f);
            }
            cLib_addCalc2(&kbox_ac->field_0x5ac, -100.0f, 1.0f, 0.05f);
            if ((i_this->mRandomSeed & 0xf) == 0) {
                i_this->mSound.startCreatureSound(Z2SE_EN_SG_BITE, 0, -1);
            }
        }

        cLib_addCalcAngleS2(&i_this->mJointAngle, target_angle, 1, 0x4000);
    }
}

static void e_sg_esa_search(e_sg_class* i_this) {
    fopAc_ac_c* target = fopAcM_SearchByID(i_this->mTargetActorID);

    cXyz local_78;
    cXyz cStack_84;

    if (target == NULL) {
        i_this->mAction = ACT_MOVE;
        i_this->mMode = MODE_IDLE;
        i_this->speedF = 0.0f;
        i_this->mTimers[0] = cM_rndF(20.0f);
    } else {
        if (i_this->mTimers[0] == 0 && (i_this->mRandomSeed & 0x7) == (fopAcM_GetID(i_this) & 0x7))
        {
            cStack_84 = target->current.pos;
            cStack_84.y = cStack_84.y - 40.0f;
            if (target->current.pos.y > i_this->mGroundY || otherBgCheck(i_this, &cStack_84, 0)) {
                i_this->mAction = ACT_MOVE;
                i_this->mMode = MODE_IDLE;
                i_this->speedF = 0.0f;
                i_this->mTimers[0] = cM_rndF(20.0f);
                return;
            }
        }

        f32 max_speed_step;
        s16 max_angle_step;

        switch (i_this->mMode) {
        case MODE_IDLE:
            i_this->mStepSpeed = 0.7f;
            max_angle_step = 0x900;
            max_speed_step = 0.5f;
            break;
        }
        i_this->mTargetPos = target->current.pos;
        local_78 = i_this->mTargetPos - i_this->current.pos;

        s16 sVar1 = i_this->current.angle.y;
        f32 fVar6 = i_this->mPlayerDist * 20.0f;
        if (fVar6 > 3000.0f) {
            fVar6 = 3000.0f;
        }
        cLib_addCalcAngleS2(&i_this->current.angle.y,
                            (s16)(fVar6 * cM_ssin(i_this->mRandomSeed * 0x4b0)) +
                                cM_atan2s(local_78.x, local_78.z),
                            4, max_angle_step);
        cLib_addCalcAngleS2(
            &i_this->current.angle.x,
            -cM_atan2s(local_78.y, JMAFastSqrt(local_78.x * local_78.x + local_78.z * local_78.z)),
            4, max_angle_step);

        sVar1 -= i_this->current.angle.y;
        fVar6 = sVar1 * 5.0f;

        if (fVar6 > 4000.0f) {
            fVar6 = 4000.0f;
        } else if (fVar6 < -4000.0f) {
            fVar6 = -4000.0f;
        }

        cLib_addCalc2(&i_this->mJointYRot, fVar6, 0.5f, 1000.0f);
        cLib_addCalc2(&i_this->speedF, i_this->mTargetSpeed * l_HIO.mSearchSpeed, 1.0f,
                      max_speed_step);

        s16 target_angle = 5000;
        if (local_78.abs() < 200.0f) {
            target_angle = cM_ssin(i_this->mRandomSeed * 0x3100) * 2000.0f + 3000.0f;
        }

        cLib_addCalcAngleS2(&i_this->mJointAngle, target_angle, 1, 0x4000);
    }
}

static void e_sg_kamu(e_sg_class* i_this) {
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);

    switch (i_this->mMode) {
    case MODE_IDLE:
        i_this->mAttackCollistion = true;
        if ((i_this->mRandomSeed & 0xf) == 0) {
            if (i_this->mKamuTimer != 0) {
                i_this->mKamuTimer--;
            }
            i_this->mSound.startCreatureSound(Z2SE_EN_SG_BITE, 0, -1);
        }

        if (mDoCPd_c::getTrigA(0)) {
            i_this->mKamuTimer++;
        }

        if (player->current.pos.y > i_this->mGroundY + 10.0f || player->speedF >= 10.0f) {
            i_this->mMode = MODE_ACTIVE;
            i_this->mTimers[0] = (cM_rndF(30.0f) + 10.0f);
        }

        if (i_this->mKamuTimer >= 10 || dComIfGp_checkPlayerStatus0(0, 8)) {
            i_this->mMode = MODE_ACTIVE;
            i_this->mTimers[0] = 0;
        }
        break;

    case MODE_ACTIVE:
        if (i_this->mTimers[0] == 0) {
            i_this->mAction = ACT_DROP;
            i_this->mMode = MODE_IDLE;
            i_this->speed.setall(0.0f);
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->current.angle.z = 0x4000;
            } else {
                i_this->current.angle.z = -0x4000;
            }
            if (i_this->mStickIdx == 0) {
                return;
            }
            stick_pt &= ~stick_bit[i_this->mStickIdx - 1];
            i_this->mStickIdx = 0;
            return;
        }
        break;
    }

    i_this->mStepSpeed = 0.9f;

    MTXCopy(
        daPy_getLinkPlayerActorClass()->getModelJointMtx(stick_d[i_this->mStickIdx - 1].joint_no),
        *calc_mtx);
    cXyz local_48(0.0f, stick_d[i_this->mStickIdx - 1].y, stick_d[i_this->mStickIdx - 1].z);

    MtxPosition(&local_48, &i_this->mTargetPos);

    cLib_addCalc2((f32*)&i_this->current, i_this->mTargetPos.x, 1.0f, i_this->mKamuSpeed);
    cLib_addCalc2(&i_this->current.pos.y, i_this->mTargetPos.y, 1.0f, i_this->mKamuSpeed);
    cLib_addCalc2(&i_this->current.pos.z, i_this->mTargetPos.z, 1.0f, i_this->mKamuSpeed);
    cLib_addCalc2(&i_this->mKamuSpeed, 1000.0f, 1.0f, 10.0f);

    local_48 = player->current.pos;
    local_48 += player->speed * 20.0f;
    local_48 -= i_this->current.pos;

    cLib_addCalcAngleS2(&i_this->current.angle.y,
                        cM_atan2s(local_48.x, local_48.z) +
                            (s16)(cM_ssin(i_this->mRandomSeed * 0xaf0) * 8000.0f),
                        8, 0x800);
    cLib_addCalcAngleS2(&i_this->current.angle.x, 0, 8, 0x800);
    cLib_addCalcAngleS2(&i_this->mJointAngle,
                        cM_ssin(i_this->mRandomSeed * 0x1500) * 2000.0f + 3000.0f, 1, 0x2000);
}

static void e_sg_drop(e_sg_class* i_this) {
    cXyz local_3c;
    cXyz local_48;

    if (fopAcM_CheckStatus(i_this, 0x100000) == 0) {
        i_this->mStepSpeed = 1.2f;
        if (i_this->mAcch.ChkGroundHit()) {
            local_3c = i_this->home.pos - i_this->current.pos;

            cMtx_YrotS(*calc_mtx, cM_atan2s(local_3c.x, local_3c.z));
            local_3c.x = cM_rndFX(5.0f);
            local_3c.y = cM_rndF(10.0f) + 10.0f;
            local_3c.z = cM_rndF(5.0f) + 1.0f;
            MtxPosition(&local_3c, &i_this->speed);

            if (cM_rndF(1.0f) < 0.5f) {
                i_this->current.angle.z = 0x4000;
            } else {
                i_this->current.angle.z = -0x4000;
            }
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->mRotationTarget = 0;
            } else {
                i_this->mRotationTarget = -0x8000;
            }

            i_this->current.angle.y += (s16)cM_rndFX(15000.0f);
            i_this->mSound.startCreatureSound(Z2SE_EN_SG_BOUND, 0, -1);
        }

        i_this->current.pos += i_this->speed;
        i_this->speed.y -= 3.0f;

        f32 offset = l_HIO.mScaleFactor * 20.0f * i_this->scale.x;

        i_this->current.pos.y -= offset;
        i_this->old.pos.y -= offset;

        i_this->mAcch.CrrPos(dComIfG_Bgsp());
        i_this->current.pos.y += offset;
        i_this->old.pos.y += offset;

        if (i_this->current.pos.y < i_this->mGroundY) {
            i_this->speedF = 0.0f;
            i_this->mAction = ACT_MOVE;
            i_this->mMode = MODE_FOLLOW;
            i_this->mTimers[0] = cM_rndF(60.0f) + 50.0f;
            i_this->mTimers[1] = cM_rndF(80.0f) + 80.0f;
            i_this->mWaterSplash = true;

            if (i_this->field_0x567) {
                i_this->field_0x567 = false;

                cMtx_YrotS(*calc_mtx, i_this->mPlayerAngle);
                local_3c.x = 0.0f;
                local_3c.y = 0.0f;
                local_3c.z = 100.0f;
                MtxPosition(&local_3c, &local_48);

                local_48.x += i_this->current.pos.x;
                local_48.z += i_this->current.pos.z;
                local_48.y = i_this->mGroundY + 10.0f;
                fopAcM_createItem(&local_48, fpcNm_ITEM_HEART, -1, -1, NULL, NULL, 0);
            }
        }
    }
}

static void e_sg_damage(e_sg_class* i_this) {
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);

    cXyz local_2c;
    cXyz cStack_38;

    i_this->mStepSpeed = 0.0f;

    switch (i_this->mMode) {
    case MODE_IDLE:
        local_2c = i_this->current.pos - player->eyePos;

        i_this->current.angle.y = cM_atan2s(local_2c.x, local_2c.z);
        if (i_this->current.pos.y < i_this->mGroundY) {
            i_this->current.angle.x = -cM_atan2s(
                local_2c.y, JMAFastSqrt(local_2c.x * local_2c.x + local_2c.z * local_2c.z));
            i_this->mMode = MODE_SLOW;
            i_this->speedF = cM_rndF(3.0f) + 10.0f;

        } else {
            i_this->current.angle.x = 0;
            i_this->mMode = MODE_ACTIVE;
            i_this->speedF = cM_rndF(10.0f) + 40.0f;
        }
        i_this->mRotationTarget = cM_rndF(65536.0f);
        i_this->speed.y = 0.0f;
        break;

    case MODE_ACTIVE:
        i_this->speed.y -= 3.0f;
        if (i_this->mAcch.ChkWallHit()) {
            i_this->speedF = 0.0f;
        }
        if (i_this->mAcch.ChkGroundHit()) {
            cLib_addCalc0(&i_this->speedF, 1.0f, 2.0f);
        }
        if (i_this->current.pos.y < i_this->mGroundY) {
            i_this->mMode = MODE_SLOW;
            i_this->mWaterSplash = true;
            i_this->speedF *= 0.2f;
            i_this->speed.y = 0.0f;
        }
        break;

    case MODE_SLOW:
        cLib_addCalc0(&i_this->speedF, 1.0f, 0.5f);
        break;
    }

    cMtx_YrotS(*calc_mtx, i_this->current.angle.y);
    cMtx_XrotM(*calc_mtx, i_this->current.angle.x);
    local_2c.x = 0.0f;
    local_2c.y = 0.0f;
    local_2c.z = i_this->speedF * l_HIO.mScaleFactor * i_this->scale.x;
    MtxPosition(&local_2c, &cStack_38);
    i_this->current.pos += cStack_38;
    i_this->current.pos.y += i_this->speed.y;

    i_this->mAcch.CrrPos(dComIfG_Bgsp());
    if (i_this->speedF == 0.0f) {
        fopAcM_createDisappear(i_this, &i_this->eyePos, 5, 0, 0x10);
        fopAcM_delete(i_this);
    }
}

static void damage_check(e_sg_class* i_this) {
    if (i_this->mInvincibilityTimer == 0) {
        i_this->mStts.Move();

        if (i_this->mSph.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mSph.GetTgHitObj();

            cc_at_check(i_this, &i_this->mAtInfo);

            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                i_this->mAction = ACT_DROP;
                i_this->mMode = MODE_IDLE;
                i_this->speed.setall(0.0f);

            } else {
                i_this->mAction = ACT_DAMAGE;
                i_this->mMode = MODE_IDLE;
                i_this->mInvincibilityTimer = 1000;

                i_this->mSound.startCreatureVoice(Z2SE_EN_SG_V_DEATH, -1);
            }
        }
        if (i_this->health <= 1) {
            i_this->health = 0;
            i_this->mSph.SetTgHitMark(CcG_Tg_UNK_MARK_3);
        }
    }
}

static void action(e_sg_class* i_this) {
    s8 move_flag = false;
    s8 att_flag = true;
    s8 link_search_flag = false;

    switch (i_this->mAction) {
    case ACT_MOVE:
        e_sg_move(i_this);
        break;

    case ACT_SEARCH:
        e_sg_search(i_this);
        link_search_flag = true;
        break;

    case ACT_KAMU:
        e_sg_kamu(i_this);
        move_flag = true;
        att_flag = false;
        link_search_flag = true;
        break;

    case ACT_DROP:
        e_sg_drop(i_this);
        move_flag = true;
        break;

    case ACT_B_SEARCH:
        e_sg_b_search(i_this);
        break;

    case ACT_ESA_SEARCH:
        e_sg_esa_search(i_this);
        break;

    case ACT_DAMAGE:
        e_sg_damage(i_this);
        move_flag = true;
        att_flag = false;
        break;
    }

    if (att_flag) {
        fopAcM_OnStatus(i_this, 0);
        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(i_this, 0);
        i_this->attention_info.flags = 0;
    }

    if (link_search_flag) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    cLib_addCalc2(&i_this->mTargetSpeed, i_this->mStepSpeed, 1.0f, 0.2f);
    cLib_addCalc2(&i_this->mJointSpeed, 2000.0f + i_this->mTargetSpeed * 2000.0f, 0.5f, 200.0f);
    i_this->mSwimAngleSpeed = i_this->mTargetSpeed * 13000.0f + 2000.0f;

    s16 tmp = i_this->mSwimAngle;
    i_this->mSwimAngle += i_this->mSwimAngleSpeed;

    if (i_this->current.pos.y < i_this->mGroundY && tmp < 0 && i_this->mSwimAngle >= 0) {
        i_this->mSound.startCreatureSound(Z2SE_EN_SG_SWIM, 0, -1);
    }

    cXyz local_28;
    f32 local_34[3] = {
        1.0f,
        2.0f,
        3.5f,
    };
    cXyz cStack_40;

    for (int i = 0; i < 3; i++) {
        i_this->mJoints[i + 1] =
            i_this->mJointSpeed * cM_ssin(i_this->mSwimAngle + i * -15000) * local_34[i];
    }
    i_this->mJoints[0] = i_this->mJointSpeed * cM_ssin(i_this->mSwimAngle - 7000) * -0.3f;

    cXyz* cc_move;
    if (i_this->mCollisionResponse && (cc_move = i_this->mStts.GetCCMoveP())) {
        i_this->current.pos += *cc_move * 0.75f;
    }

    if (!move_flag) {
        cMtx_YrotS(*calc_mtx, i_this->current.angle.y);
        cMtx_XrotM(*calc_mtx, i_this->current.angle.x);

        local_28.x = 0.0f;
        local_28.y = 0.0f;
        local_28.z = i_this->speedF * l_HIO.mScaleFactor * i_this->scale.x;

        MtxPosition(&local_28, &i_this->speed);
        i_this->current.pos += i_this->speed;

        if (i_this->mAction == ACT_B_SEARCH) {
            obj_kbox_class* kbox = (obj_kbox_class*)fopAcM_SearchByID(i_this->mTargetActorID);
            cMtx_YrotS(*calc_mtx, i_this->current.angle.y);

            local_28.x = 0.0f;
            local_28.y = 0.0f;
            local_28.z = i_this->speedF * -2.5f;

            MtxPosition(&local_28, &cStack_40);
            i_this->current.pos += cStack_40;
            i_this->old.pos += cStack_40;

            i_this->mAcch.CrrPos(dComIfG_Bgsp());
            i_this->current.pos -= cStack_40;
            i_this->old.pos -= cStack_40;

            if (i_this->current.pos.y > 15.0f + kbox->field_0x5b4.y) {
                i_this->current.pos.y = 15.0f + kbox->field_0x5b4.y;
            }
        } else {
            i_this->mAcch.CrrPos(dComIfG_Bgsp());

            if (i_this->current.pos.y > i_this->mGroundY - 10.0f) {
                i_this->current.pos.y = i_this->mGroundY - 10.0f;
            }
        }
    }
}

static int daE_SG_Execute(e_sg_class* i_this) {
    if (i_this->mInactive) {
        return 1;
    }

    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);

    i_this->mRandomSeed++;
    if (i_this->mInitTimer != 0) {
        i_this->mInitTimer--;
        mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
        mDoMtx_stack_c::YrotM(i_this->shape_angle.y + i_this->mJoints[0]);
        i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        return 1;
    }

    dBgS_LinChk lin_chk;

    cXyz local_138;
    local_138.x = i_this->current.pos.x;
    local_138.y = i_this->current.pos.y + 3000.0f;
    local_138.z = i_this->current.pos.z;

    lin_chk.Set(&i_this->current.pos, &local_138, (fopAc_ac_c*)i_this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        local_138.y = lin_chk.GetCross().y - 10.0f;
    }

    dBgS_ObjGndChk_Spl gnd_chk;
    gnd_chk.SetPos(&local_138);

    f32 fVar71 = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (fVar71 != -G_CM3D_F_INF && i_this->mGroundY != fVar71) {
        i_this->mGroundY = fVar71;
        i_this->home.pos.y = i_this->mGroundY - 250.0f + cM_rndFX(50.0f);
    }
    i_this->mPlayerAngle = fopAcM_searchPlayerAngleY(i_this);
    i_this->mPlayerDist = fopAcM_searchPlayerDistance(i_this);
    i_this->mTargetDist = (i_this->home.pos - player->current.pos).abs();

    for (int i = 0; i < 3; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }
    if (i_this->mInvincibilityTimer != 0) {
        i_this->mInvincibilityTimer--;
    }

    damage_check(i_this);
    action(i_this);

    if (i_this->speed.y < -100.0f) {
        i_this->speed.y = -100.0f;
    }

    cLib_addCalcAngleS2(&i_this->shape_angle.y, i_this->current.angle.y, 2, 0x1000);
    cLib_addCalcAngleS2(&i_this->shape_angle.x, i_this->current.angle.x, 2, 0x1000);

    s16 target_angle = 0x0;
    if (i_this->field_0x6C6 != 0) {
        i_this->field_0x6C6--;
        target_angle = 0x4000;
    }

    cLib_addCalcAngleS2(&i_this->shape_angle.z, i_this->current.angle.z + target_angle, 2, 0x1000);
    cLib_addCalcAngleS2(&i_this->mRotation, i_this->mRotationTarget, 2, 0x1800);

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y + i_this->mJoints[0]);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
    mDoMtx_stack_c::YrotM(i_this->mRotation);

    f32 scale = l_HIO.mScaleFactor * i_this->scale.x;
    mDoMtx_stack_c::scaleM(scale, scale, scale);
    mDoMtx_stack_c::transM(0.0f, 0.0f, -15.0f);

    i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->eyePos = i_this->current.pos;
    i_this->attention_info.position = i_this->eyePos;
    i_this->attention_info.position.y += 30.0f;

    local_138 = i_this->eyePos;
    if (i_this->mInvincibilityTimer != 0) {
        local_138.x -= 20000.0f;
    }
    i_this->mSph.SetC(local_138);

    i_this->mSph.SetR(l_HIO.mScaleFactor * 20.0f * i_this->scale.x);

    if (i_this->mAttackCollistion) {
        i_this->mSph.OnAtSetBit();
        i_this->mAttackCollistion = false;
    } else {
        i_this->mSph.OffAtSetBit();
    }
    if (i_this->mCollisionResponse) {
        i_this->mSph.OnCoSetBit();
        i_this->mCollisionResponse = false;
    } else {
        i_this->mSph.OffCoSetBit();
    }

    dComIfG_Ccsp()->Set(&i_this->mSph);
    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo((fopAc_ac_c*)i_this)));

    if (i_this->mWaterSplash) {
        cXyz cStack_144 = i_this->current.pos;
        cStack_144.y = i_this->mGroundY;

        scale = l_HIO.mScaleFactor * 0.5f * i_this->scale.x;
        static cXyz sc(scale, scale, scale);

        static u16 w_eff_id[] = {
            0x01B8,
            0x01B9,
            0x01BA,
            0x01BB,
        };

        for (int k = 0; k < 4; k++) {
            i_this->mParticleKey[k] = dComIfGp_particle_set(
                i_this->mParticleKey[k], w_eff_id[k], &cStack_144, &i_this->tevStr,
                &i_this->shape_angle, &sc, 0xff, 0, -1, 0, 0, 0);
        }

        fopAcM_effHamonSet(&i_this->mHamon, &cStack_144, 0.5f, 1.0f);
        i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_WATER_S, 0, -1);
        i_this->mWaterSplash = false;
    }

    return 1;
}

static int daE_SG_IsDelete(e_sg_class* i_this) {
    return 1;
}

static int daE_SG_Delete(e_sg_class* i_this) {
    dComIfG_resDelete(&i_this->mPhaseReq, "E_sg");

    if (i_this->mHioInit) {
        hio_init = false;
    }

    if (i_this->heap != NULL) {
        i_this->mSound.deleteObject();
    }

    if (i_this->mStickIdx != 0) {
        stick_pt &= ~stick_bit[i_this->mStickIdx - 1];
        i_this->mStickIdx = 0;
    }

    return 1;
}

static Vec jv_offset = {0.0f, 0.0f, 0.0f};

static dJntColData_c jc_data = {
    0x00, 0x01, 0x0001, 10.0f, &jv_offset,
};

static int useHeapInit(fopAc_ac_c* i_this) {
    e_sg_class* a_this = static_cast<e_sg_class*>(i_this);

    J3DModelData* model = (J3DModelData*)dComIfG_getObjectRes("E_sg", 0x3);
    for (u16 i = 0; i < model->getJointNum(); i++) {
        model->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    a_this->mpModel = (J3DModel*)mDoExt_J3DModel__create(model, 0x80000, 0x11000084);
    if (a_this->mpModel == NULL) {
        return 0;
    }

    a_this->mpModel->setUserArea((uintptr_t)i_this);
    if (a_this->mJoint.init(a_this, &jc_data, a_this->mpModel, 1) == 0) {
        return 0;
    }

    return 1;
}

static int daE_SG_Create(fopAc_ac_c* i_this) {
    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0xd8fbfdff, 0x3}, 0x75}},  // mObj
            {dCcD_SE_HARD_BODY, 0x0, 0xc, 0x0, 0x0},                              // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                                   // mGObjTg
            {0x0},                                                                // mGObjCo
        },                                                                        // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
        }  // mSphAttr
    };

    e_sg_class* a_this = static_cast<e_sg_class*>(i_this);
    fopAcM_ct(i_this, e_sg_class);

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&a_this->mPhaseReq, "E_sg");

    if (step == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 4000)) {
            return cPhs_ERROR_e;
        }

        if (hio_init == false) {
            a_this->mHioInit = true;
            hio_init = true;
            l_HIO.mUnk0 = -1;
        }

        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        fopAcM_SetMtx(a_this, a_this->mpModel->getBaseTRMtx());

        a_this->health = 0x14;
        a_this->field_0x560 = 0x14;
        a_this->mStts.Init(100, 0, i_this);
        a_this->mSph.Set(cc_sph_src);
        a_this->mSph.SetStts(&a_this->mStts);
        a_this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(a_this), i_this, 1,
                          &a_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->mAcchCir.SetWall(15.0f, 30.0f);
        a_this->mSound.init(&i_this->current.pos, &a_this->eyePos, 0x3, 0x1);
        a_this->mSound.setEnemyName("E_sg");

        a_this->mAtInfo.mpSound = &a_this->mSound;
        a_this->mRandomSeed = cM_rndF(65536.0f);

        int child_count = fopAcM_GetParamBit(i_this, 0, 8);
        a_this->mIdleMoveBound = fopAcM_GetParamBit(i_this, 8, 8) * 100.0f;
        a_this->mSearchBound = fopAcM_GetParamBit(i_this, 16, 8) * 100.0f;

        if (child_count == 0xff) {
            child_count = 10;
        }

        if (child_count < 32) {
            csXyz child_angle(0, 0, 0);
            u32 parameters;

            for (int i = 0; i < child_count; i++) {
                parameters = 0x64 + i | fopAcM_GetParam(i_this) >> 8 << 8;

                cXyz child_pos;
                f32 res_x = i_this->current.pos.x + cM_rndFX(a_this->mIdleMoveBound);
                i_this->current.pos.x = res_x;
                child_pos.x = res_x;

                f32 res_y = i_this->current.pos.y + cM_rndFX(100.0f);
                i_this->current.pos.y = res_y;
                child_pos.y = res_y;

                f32 res_z = i_this->current.pos.z + cM_rndFX(a_this->mIdleMoveBound);
                i_this->current.pos.z = res_z;
                child_pos.z = res_z;

                child_angle.y = cM_rndF(65536.0f);

                fopAcM_createChild(PROC_E_SG, fopAcM_GetID(i_this), parameters, &child_pos,
                                   fopAcM_GetRoomNo(i_this), &child_angle, 0, -1, 0);
            }
            a_this->mArg0 = false;

        } else {
            a_this->mArg0 = (child_count & 0x1f) + 1;
            a_this->scale.x = cM_rndFX(0.1f) + 1.0f;
        }

        a_this->field_0x6b4 = cM_rndFX(50.0f) + -150.0f;
        a_this->mGroundY = -100000.0f;
        a_this->mInitTimer = 40;

        daE_SG_Execute(a_this);
    }

    return step;
}

AUDIO_INSTANCES;

static actor_method_class l_daE_SG_Method = {
    (process_method_func)daE_SG_Create,  (process_method_func)daE_SG_Delete,
    (process_method_func)daE_SG_Execute, (process_method_func)daE_SG_IsDelete,
    (process_method_func)daE_SG_Draw,
};

extern actor_process_profile_definition g_profile_E_SG = {
    fpcLy_CURRENT_e,       // mLayerID
    7,                     // mListID
    fpcPi_CURRENT_e,       // mListPrio
    PROC_E_SG,             // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(e_sg_class),    // mSize
    0,                     // mSizeOther
    0,                     // mParameters
    &g_fopAc_Method.base,  // sub_method
    122,                   // mPriority
    &l_daE_SG_Method,      // sub_method
    0x000D0100,            // mStatus
    fopAc_ENEMY_e,         // mActorType
    fopAc_CULLBOX_0_e,     // cullType
};
