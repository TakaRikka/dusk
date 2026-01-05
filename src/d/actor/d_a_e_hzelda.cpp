/**
 * @file d_a_e_hzelda.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_hzelda.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "SSystem/SComponent/c_math.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_msg_mng.h"
#include "m_Do/m_Do_graphic.h"
#include "d/d_s_play.h"

#define ANM_ATTACKC         6
#define ANM_ATTACKC_A       7
#define ANM_ATTACKC_B       8
#define ANM_ATTACK_C_WAIT   9
#define ANM_ATTACK_A        10
#define ANM_ATTACK_A_BEFORE 11
#define ANM_ATTACK_B        12
#define ANM_ATTACK_C        13
#define ANM_ATTACK_WAIT     14
#define ANM_BEFORE_ATTACK   15
#define ANM_DAMAGE          16
#define ANM_DAMAGE_LOOP     17
#define ANM_DAMAGE_UP       18
#define ANM_FMOVE_L         19
#define ANM_FMOVE_R         20
#define ANM_FWAIT           21
#define ANM_LAST_DAMAGE     22

enum helda_joint {
    JNT_CENTER,
    JNT_BACKBONE1,
    JNT_BACKBONE2,
    JNT_NECK,
    JNT_HEAD,
    JNT_CHIN,
    JNT_MAYU_L,
    JNT_MAYU_R,
    JNT_MOMI_L1,
    JNT_MOMI_L2,
    JNT_MOMI_L3,
    JNT_MOMI_R1,
    JNT_MOMI_R2,
    JNT_MOMI_R3,
    JNT_MOUTH,
    JNT_OSAGE1,
    JNT_OSAGE2,
    JNT_OSAGE3,
    JNT_SHOULDER_L,
    JNT_ARM_L1,
    JNT_ARM_L2,
    JNT_HAND_L,
    JNT_FINGER_L1,
    JNT_FINGER_L2,
    JNT_THAMB_L,
    JNT_SHOULDER_R,
    JNT_ARM_R1,
    JNT_ARM_R2,
    JNT_HAND_R,
    JNT_FINGER_R1,
    JNT_FINGER_R2,
    JNT_THAMB_R,
    JNT_WAIST,
    JNT_CLOTH1,
    JNT_CLOTH2,
    JNT_CLOTH3,
    JNT_LEG_L1,
    JNT_LEG_L2,
    JNT_FOOT_L,
    JNT_LEG_R1,
    JNT_LEG_R2,
    JNT_FOOT_R,
    JNT_SKIRT_BL1,
    JNT_SKIRT_BL2,
    JNT_SKIRT_BR1,
    JNT_SKIRT_BR2,
    JNT_SKIRT_FL1,
    JNT_SKIRT_FL2,
    JNT_SKIRT_FR1,
    JNT_SKIRT_FR2,
};

enum hzelda_action {
    ACTION_WAIT,
    ACTION_ATTACK_A,  // Sword Attack
    ACTION_ATTACK_B,  // Triangle Attack
    ACTION_ATTACK_C,  // Ball Throw Attack

    ACTION_DAMAGE = 5
};

enum hzelda_timer {
    TIMER_ATTACK_WAIT = 0,
};

daE_HZELDA_HIO_c::daE_HZELDA_HIO_c() {
    field_0x4 = -1;
    mModelSize = 1.0f;
}

static void anm_init(e_hzelda_class* i_this, int i_resID, f32 i_morf, u8 i_attr,
                     f32 i_speed) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Hzelda", i_resID), i_attr, i_morf, i_speed, 0.0f, -1.0f);
    i_this->mAnm = i_resID;
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_hzelda_class* a_this = (e_hzelda_class*)model->getUserArea();

        if (a_this != NULL) {
            MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);

            if (jnt_no == JNT_NECK) {
                cMtx_XrotM(*calc_mtx, a_this->mHeadRotX / 2);
                cMtx_ZrotM(*calc_mtx, -a_this->mHeadRotZ / 2);
            } else if (jnt_no == JNT_HEAD) {
                cMtx_XrotM(*calc_mtx, a_this->mHeadRotX / 2);
                cMtx_ZrotM(*calc_mtx, a_this->mHeadRotZ / 2);
            } else if (jnt_no == JNT_ARM_L1) {
                cMtx_YrotM(*calc_mtx, a_this->mArmLRotY);
            } else if (jnt_no == JNT_ARM_R1) {
                cMtx_YrotM(*calc_mtx, -a_this->mArmRRotY);
            } else if (jnt_no == JNT_WAIST) {
                cMtx_YrotM(*calc_mtx, a_this->mBodyRotY);
                cMtx_ZrotM(*calc_mtx, a_this->mBodyRotZ);
            }

            model->setAnmMtx(jnt_no, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int daE_HZELDA_Draw(e_hzelda_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    J3DModel* model = i_this->mpModelMorf->getModel();

    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);
    i_this->mpModelMorf->entryDL();

    if (i_this->mpSwordModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpSwordModel, &a_this->tevStr);
        
        J3DMaterial* materialp = i_this->mpSwordModel->getModelData()->getMaterialNodePointer(0);
        materialp->getTevColor(0)->r = i_this->mSwordColorIntensity * 255.0f;
        materialp->getTevColor(0)->g = i_this->mSwordColorIntensity * 150.0f;
        materialp->getTevColor(0)->b = 0;

        mDoExt_modelUpdateDL(i_this->mpSwordModel);
    }

    cXyz shadow_pos;
    shadow_pos.set(a_this->current.pos.x, a_this->current.pos.y + 100.0f, a_this->current.pos.z);

    i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &shadow_pos, 1000.0f, 0.0f, a_this->current.pos.y, i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd, &a_this->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    if (i_this->mpSwordModel != NULL) {
        dComIfGd_addRealShadow(i_this->mShadowKey, i_this->mpSwordModel);
    }

    if (i_this->mDrawTriangleAt) {
        i_this->mpTriangleAtBrk->entry(i_this->mpTriangleAtModel->getModelData());
        i_this->mpTriangleAtBtk->entry(i_this->mpTriangleAtModel->getModelData());

        g_env_light.setLightTevColorType_MAJI(i_this->mpTriangleAtModel, &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mpTriangleAtModel);
    }

    return 1;
}

static void* shot_s_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_ARROW && (fopAcM_GetParam(i_actor) == 1 || fopAcM_GetParam(i_actor) == 2)) {
        cXyz vec = ((fopAc_ac_c*)i_data)->current.pos - ((fopAc_ac_c*)i_actor)->current.pos;
        if (vec.abs() < 1000.0f) {
            return i_actor;
        }
    }

    return NULL;
}

static void e_hzelda_wait(e_hzelda_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp24;
    cXyz sp30;

    i_this->field_0x6b4 = 5;
    i_this->mSound.startCreatureSoundLevel(Z2SE_EN_HZE_MOVE, 0, -1);

    switch (i_this->mMode) {
    case -1:
        Z2GetAudioMgr()->bgmStart(Z2BGM_VS_GANON_01, 0, 0);
    case 0:
        anm_init(i_this, ANM_FWAIT, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        i_this->mMode = 1;
        i_this->mTimers[TIMER_ATTACK_WAIT] = cM_rndF(150.0f) + 100.0f;
        i_this->mMoveStep = 0.0f;
        i_this->field_0x5d0 = i_this->mAngleToPlayer + 0x8000;
        break;
    case 1:
        break;
    }

    cLib_addCalc2(&a_this->speedF, 0.0f, 1.0f, 3.0f);
    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mAngleToPlayer, 2, 0x300);

    cMtx_YrotS(*calc_mtx, i_this->field_0x5d0);
    sp24.x = cM_ssin(i_this->mCounter * 300) * 300.0f;
    sp24.y = cM_ssin(i_this->mCounter * 700) * 30.0f + 300.0f;
    sp24.z = cM_ssin(i_this->mCounter * 400) * 150.0f + 600.0f;
    MtxPosition(&sp24, &i_this->mTargetMovePos);
    i_this->mTargetMovePos += player->current.pos;

    cLib_addCalc2(&a_this->current.pos.x, i_this->mTargetMovePos.x, 0.1f, i_this->mMoveStep);
    cLib_addCalc2(&a_this->current.pos.y, i_this->mTargetMovePos.y, 0.1f, i_this->mMoveStep);
    cLib_addCalc2(&a_this->current.pos.z, i_this->mTargetMovePos.z, 0.1f, i_this->mMoveStep);
    cLib_addCalc2(&i_this->mMoveStep, 30.0f, 1.0f, 0.5f);

    i_this->mIsLookatPlayer = true;

    if (i_this->mDodgeMove < 0.1f) {
        if (i_this->mTimers[TIMER_ATTACK_WAIT] == 0) {
            if (i_this->mNoBallCount != 0) {
                i_this->mNoBallCount--;

                // 50-50% chance of sword or triangle attack
                if (cM_rndF(1.0f) < 0.5f) {
                    i_this->mAction = ACTION_ATTACK_A;
                } else {
                    i_this->mAction = ACTION_ATTACK_B;
                }
            } else {
                // 70% chance of ball attack. if failed, 50-50% chance of sword or triangle attack
                if (cM_rndF(1.0f) < 0.7f) {
                    i_this->mAction = ACTION_ATTACK_C;
                } else if (cM_rndF(1.0f) < 0.5f) {
                    i_this->mAction = ACTION_ATTACK_A;
                } else {
                    i_this->mAction = ACTION_ATTACK_B;
                }
            }

            i_this->mMode = 0;
            return;
        }

        // dodge out of the way if player shoots arrow or hookshot
        if (fpcM_Search(shot_s_sub, i_this) != NULL || (daPy_getPlayerActorClass()->checkHookshotShootReturnMode() && !daPy_getPlayerActorClass()->checkHookshotReturnMode())) {
            i_this->mDodgeMove = 40.0f;

            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0x6dc = i_this->mAngleToPlayer + 0x4000;
            } else {
                i_this->field_0x6dc = i_this->mAngleToPlayer - 0x4000;
            }
        }
    }

    cMtx_YrotS(*calc_mtx, i_this->field_0x6dc);
    sp24.x = 0.0f;
    sp24.y = 0.0f;
    sp24.z = i_this->mDodgeMove;
    MtxPosition(&sp24, &sp30);
    a_this->current.pos += sp30;

    cLib_addCalc0(&i_this->mDodgeMove, 1.0f, 2.0f);
}

static int e_hzelda_attack_a(e_hzelda_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz target_dist;
    cXyz sp38;
    f32 target_speed = 0.0f;
    f32 speed_step = 10.0f;
    BOOL move_to_target = FALSE;
    int ret = 1;

    i_this->field_0x6b4 = 5;
    int anm_frame = i_this->mpModelMorf->getFrame();
    s16 sp8 = 0;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_BEFORE_ATTACK, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 1;
        i_this->field_0x5c4 = i_this->mTargetMovePos;
        /* fallthrough */
    case 1:
        move_to_target = TRUE;
        i_this->field_0x68c = 1;
        i_this->mSound.startCreatureSoundLevel(Z2SE_EN_HZE_MOVE, 0, -1);

        if (i_this->mpModelMorf->isStop()) {
            i_this->mMode = 2;
            anm_init(i_this, ANM_ATTACK_A_BEFORE, 5.0f, 0, 1.0f);
            i_this->field_0x5c4 = i_this->mTargetMovePos;
        }
        break;
    case 2:
        i_this->field_0x68c = 1;
        if (anm_frame > 32) {
            target_speed = 30.0f;
            speed_step = 10.0f;
            i_this->mTargetMovePos = player->current.pos;
            i_this->mTargetMovePos.y += 100.0f;
            i_this->field_0x6b8 = 1;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mMode = 3;
            anm_init(i_this, ANM_ATTACK_A, 0.0f, 2, 1.0f);
            i_this->mTimers[TIMER_ATTACK_WAIT] = 90;
        }
        break;
    case 3:
        i_this->field_0x68c = 1;
        target_speed = 50.0f;
        ret = 2;
        i_this->field_0x6b8 = 1;
        i_this->mTargetMovePos = player->current.pos;
        i_this->mTargetMovePos.y += 100.0f;

        target_dist = i_this->mTargetMovePos - a_this->current.pos;
        if (target_dist.abs() < 200.0f || i_this->mTimers[TIMER_ATTACK_WAIT] == 0) {
            i_this->mMode = 4;
        }
        break;
    case 4:
        i_this->field_0x68c = 1;
        speed_step = 1.0f;
        sp8 = -0x1000;
        ret = 2;
        i_this->field_0x6b8 = 1;
        if (a_this->speedF < 20.0f) {
            i_this->mAction = ACTION_WAIT;
            i_this->mMode = 0;
        }
        break;
    }

    if (move_to_target) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mAngleToPlayer, 2, 0x300);
        cLib_addCalc2(&a_this->current.pos.x, i_this->mTargetMovePos.x, 0.05f, i_this->mMoveStep);
        cLib_addCalc2(&a_this->current.pos.y, i_this->mTargetMovePos.y + (30.0f * cM_ssin((i_this->mCounter * 1000))), 0.1f, 30.0f);
        cLib_addCalc2(&a_this->current.pos.z, i_this->mTargetMovePos.z, 0.05f, i_this->mMoveStep);
    }

    if (target_speed > 0.0f) {
        target_dist = i_this->mTargetMovePos - a_this->current.pos;
        cLib_addCalcAngleS2(&a_this->current.angle.y, (s16)cM_atan2s(target_dist.x, target_dist.z), 8, 0x800);
        cLib_addCalcAngleS2(&a_this->current.angle.x, -cM_atan2s(target_dist.y, JMAFastSqrt((target_dist.x * target_dist.x) + (target_dist.z * target_dist.z))), 2, 0x1000);
        cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 2, 0x2000);
    } else {
        cLib_addCalcAngleS2(&a_this->current.angle.x, sp8, 4, 0x400);
        cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 8, 0x1000);
    }

    cLib_addCalc2(&a_this->speedF, target_speed, 0.5f, speed_step);
    return ret;
}

static void e_hzelda_attack_b(e_hzelda_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp18;
    cXyz spC;

    i_this->field_0x6b4 = 5;
    i_this->mSound.startCreatureSoundLevel(Z2SE_EN_HZE_MOVE, 0, -1);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_BEFORE_ATTACK, 10.0f, 0, 1.0f);
        i_this->mMode = 1;
        /* fallthrough */
    case 1:
        i_this->field_0x68c = 1;
        dComIfGs_onOneZoneSwitch(12, -1);

        if (i_this->mpModelMorf->isStop()) {
            i_this->mMode = 2;
            anm_init(i_this, ANM_ATTACK_WAIT, 10.0f, 2, 1.0f);
            i_this->mTimers[TIMER_ATTACK_WAIT] = 60;
            i_this->mDrawTriangleAt = TRUE;
            i_this->mTrianglePos = player->current.pos;
            i_this->mTrianglePos.y = 5.0f;
            i_this->mTriangleAnmFrame = 0;

            if (i_this->field_0x718 == 0) {
                i_this->mTriangleSize = 8.0f;
            } else if (i_this->field_0x718 == 1) {
                i_this->mTriangleSize = 10.0f;
            } else {
                i_this->mTriangleSize = 12.0f;
            }

            i_this->field_0x718++;
            i_this->mTriangleRotY = cM_rndF((f32)0x10000);
        }
        break;
    case 2:
        i_this->field_0x68c = 1;
        dComIfGs_onOneZoneSwitch(12, -1);

        if (i_this->mTriangleAnmFrame >= 80.0f) {
            anm_init(i_this, ANM_ATTACK_B, 5.0f, 0, 1.0f);
            i_this->mMode = 3;
        }
        break;
    case 3:
        dComIfGs_onOneZoneSwitch(12, -1);
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, ANM_FWAIT, 10.0f, 2, 1.0f);
        }

        if (i_this->mTriangleAnmFrame >= 210.0f) {
            i_this->mAction = ACTION_WAIT;
            i_this->mMode = 0;
            i_this->mDrawTriangleAt = FALSE;
        }
        break;
    }

    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mAngleToPlayer, 2, 0x400);
    cLib_addCalc2(&i_this->current.pos.x, i_this->mTargetMovePos.x, 0.05f, i_this->mMoveStep);
    cLib_addCalc2(&i_this->current.pos.y, i_this->mTargetMovePos.y, 0.1f, 30.0f);
    cLib_addCalc2(&i_this->current.pos.z, i_this->mTargetMovePos.z, 0.05f, i_this->mMoveStep);
}

static void ball_crash_eff_set(e_hzelda_class* i_this, f32 i_size) {
    cXyz size(i_size, i_size, i_size);
    for (int i = 0; i < 3; i++) {
        static u16 b_id[] = {0x8915, 0x8916, 0x8917};
        dComIfGp_particle_set(b_id[i], &i_this->mBallPos, NULL, &size);
    }
}

static void e_hzelda_attack_c(e_hzelda_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp60;
    cXyz sp54;
    int anm_frame = i_this->mpModelMorf->getFrame();
    
    i_this->field_0x6b4 = 5;
    i_this->mSound.startCreatureSoundLevel(Z2SE_EN_HZE_MOVE, 0, -1);
    
    sp60 = a_this->current.pos - i_this->mBallPos;
    sp60.y += 150.0f;

    f32 dist_to_ball = sp60.abs();

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_BEFORE_ATTACK, 10.0f, 0, 1.0f);
        i_this->mMode = 1;

        // Set Midna tutorial demo switch if player doesn't hit a ball back within 5 attempts
        if (i_this->mMissedBallCount != 0) {
            i_this->mMissedBallCount++;
            if (i_this->mMissedBallCount >= 6) {
                dComIfGs_onOneZoneSwitch(3, -1);
            }
        }
        /* fallthrough */
    case 1:
        i_this->field_0x68c = 1;
        if (i_this->mpModelMorf->isStop()) {
            i_this->mMode = 2;
            anm_init(i_this, ANM_ATTACK_WAIT, 10.0f, 2, 1.0f);
            i_this->mTimers[TIMER_ATTACK_WAIT] = 60;
            i_this->field_0x6e4 = 1;
            i_this->field_0x6e8 = 0.0f;
        }
        break;
    case 2:
        i_this->field_0x68c = 1;
        if (i_this->mTimers[TIMER_ATTACK_WAIT] == 0) {
            i_this->mMode = 3;
            anm_init(i_this, ANM_ATTACK_C, 10.0f, 0, 1.0f);
        }
        break;
    case 3:
        i_this->field_0x68c = 1;
        if (i_this->mpModelMorf->checkFrame(41)) {
            i_this->field_0x6e4 = 2;

            sp60 = player->eyePos - i_this->mBallPos;
            sp60.y -= 40.0f;

            i_this->mBallMoveStep = 35.0f;
            cMtx_YrotS(*calc_mtx, cM_atan2s(sp60.x, sp60.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(sp60.y, JMAFastSqrt((sp60.x * sp60.x) + (sp60.z * sp60.z))));
            sp60.x = 0.0f;
            sp60.y = 0.0f;
            sp60.z = i_this->mBallMoveStep;
            MtxPosition(&sp60, &i_this->mBallMove);

            i_this->mBallSphAt.StartCAt(i_this->mBallPos);
        }

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, ANM_ATTACK_C_WAIT, 3.0f, 2, 1.0f);
            i_this->mMode = 4;
        }
        break;
    case 4:
        if (i_this->field_0x6e4 <= 0) {
            i_this->mAction = ACTION_WAIT;
            i_this->mMode = 0;
        } else if (i_this->field_0x6e4 == 3 && dist_to_ball < 400.0f) {
            if (cM_rndF(1.0f) < 0.5f) {
                anm_init(i_this, ANM_ATTACKC_A, 3.0f, 0, 2.0f);
            } else {
                anm_init(i_this, ANM_ATTACKC_B, 3.0f, 0, 2.0f);
            }

            i_this->mpModelMorf->setFrame(15.0f);
            i_this->mMode = 5;
        }
        break;
    case 5:
        if (i_this->mpModelMorf->checkFrame(30.0f)) {
            i_this->field_0x6e4 = 2;

            sp60 = player->eyePos - i_this->mBallPos;
            sp60.y -= 40.0f;

            if (i_this->mBallMoveStep < 50.0f) {
                i_this->mBallMoveStep += 2.0f;
            }

            cMtx_YrotS(*calc_mtx, cM_atan2s(sp60.x, sp60.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(sp60.y, JMAFastSqrt((sp60.x * sp60.x) + (sp60.z * sp60.z))));
            sp60.x = 0.0f;
            sp60.y = 0.0f;
            sp60.z = i_this->mBallMoveStep;
            MtxPosition(&sp60, &i_this->mBallMove);

            ball_crash_eff_set(i_this, 0.4f);
            i_this->mSound.startCreatureSound(Z2SE_EN_HZE_ATK_C_BALL_RETURN, 0, -1);
        }

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, ANM_ATTACK_C_WAIT, 3.0f, 2, 1.0f);
            i_this->mMode = 4;
        }
        break;
    }

    if (i_this->field_0x6e4 == 3) {
        sp60 = a_this->current.pos - i_this->mBallPos;
        sp60.y += 150.0f;

        cMtx_YrotS(*calc_mtx, cM_atan2s(sp60.x, sp60.z));
        cMtx_XrotM(*calc_mtx, -cM_atan2s(sp60.y, JMAFastSqrt((sp60.x * sp60.x) + (sp60.z * sp60.z))));
        sp60.x = 0.0f;
        sp60.y = 0.0f;
        sp60.z = i_this->mBallMoveStep;
        MtxPosition(&sp60, &i_this->mBallMove);

        if (dist_to_ball < 80.0f) {
            i_this->field_0x6e4 = -1;
            i_this->mAction = ACTION_DAMAGE;

            i_this->mSound.startCreatureVoice(Z2SE_EN_HZE_V_DAMAGE, -1);
            i_this->mSound.startCreatureSound(Z2SE_EN_HZE_ATK_C_BALL_HIT, 0, -1);

            a_this->health--;
            if (a_this->health == 1) {
                i_this->mNoBallCount = 3;
            } else {
                i_this->mNoBallCount = 1;
            }

            if (a_this->health <= 0) {
                i_this->mDemoMode = 1;
                Z2GetAudioMgr()->bgmStop(0x1E, 0);
                i_this->mMode = 10;
                
                i_this->mSound.startCreatureVoice(Z2SE_EN_HZE_V_DAMAGE_LAST, -1);
            } else {
                i_this->mMode = 0;
            }
        }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mAngleToPlayer, 2, 0x400);
    cLib_addCalc2(&a_this->current.pos.x, i_this->mTargetMovePos.x, 0.05f, i_this->mMoveStep);
    cLib_addCalc2(&a_this->current.pos.y, i_this->mTargetMovePos.y + (30.0f * cM_ssin((i_this->mCounter * 1000))), 0.1f, 30.0f);
    cLib_addCalc2(&a_this->current.pos.z, i_this->mTargetMovePos.z, 0.05f, i_this->mMoveStep);
}

static void e_hzelda_damage(e_hzelda_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz unused;
    cXyz unused2;

    i_this->field_0x6b4 = 5;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_DAMAGE, 2.0f, 0, 1.0f);
        i_this->mMode = 1;
        /* fallthrough */
    case 1:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, ANM_DAMAGE_LOOP, 5.0f, 2, 1.0f);
            i_this->mTimers[TIMER_ATTACK_WAIT] = 60;
            i_this->mMode = 2;
        }
        break;
    case 2:
        if (i_this->mTimers[TIMER_ATTACK_WAIT] == 0) {
            anm_init(i_this, ANM_DAMAGE_UP, 2.0f, 0, 1.0f);
            i_this->mMode = 3;
        }
        break;
    case 3:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = ACTION_WAIT;
            i_this->mMode = 0;
        }
        break;
    case 10:
        anm_init(i_this, ANM_LAST_DAMAGE, 5.0f, 2, 1.0f);
        i_this->mMode = 11;
    case 11:
        a_this->current.pos.y -= 1.0f;
        break;
    }
}

static void action(e_hzelda_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp50;
    cXyz sp44;

    i_this->mIsLookatPlayer = false;
    i_this->mDistToPlayer = fopAcM_searchPlayerDistance(i_this);
    i_this->mAngleToPlayer = fopAcM_searchPlayerAngleY(i_this);

    int sp10;
    BOOL on_link_search = TRUE;
    BOOL on_attention = TRUE;
    sp10 = 1;

    dComIfGs_offOneZoneSwitch(12, -1);

    switch (i_this->mAction) {
    case ACTION_WAIT:
        e_hzelda_wait(i_this);
        break;
    case ACTION_ATTACK_A:
        sp10 = e_hzelda_attack_a(i_this);
        break;
    case ACTION_ATTACK_B:
        e_hzelda_attack_b(i_this);
        break;
    case ACTION_ATTACK_C:
        e_hzelda_attack_c(i_this);
        break;
    case ACTION_DAMAGE:
        e_hzelda_damage(i_this);
        on_link_search = FALSE;
        break;
    }

    if (on_link_search) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    if (on_attention) {
        fopAcM_OnStatus(a_this, 0);
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x2000);
    cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
    sp50.x = 0.0f;
    sp50.y = 0.0f;
    sp50.z = a_this->speedF;
    MtxPosition(&sp50, &a_this->speed);
    a_this->current.pos += a_this->speed;

    s16 head_target_rotx = 0;
    s16 head_target_rotz = 0;
    s16 rot_max = 7000;

    cXyz player_pos;
    s16 rot_step = 0x1000;
    player_pos = player->eyePos;

    if (i_this->mIsLookatPlayer) {
        sp50 = player_pos - a_this->current.pos;
        sp50.y += -150.0f;

        head_target_rotx = (cM_atan2s(sp50.x, sp50.z) - a_this->shape_angle.y);
        head_target_rotz = (a_this->shape_angle.x + cM_atan2s(sp50.y, JMAFastSqrt((sp50.x * sp50.x) + (sp50.z * sp50.z))));

        if (head_target_rotx > rot_max) {
            head_target_rotx = rot_max;
        } else if (head_target_rotx < -rot_max) {
            head_target_rotx = -rot_max;
        }

        if (head_target_rotz > rot_max) {
            head_target_rotz = rot_max;
        } else if (head_target_rotz < -rot_max) {
            head_target_rotz = -rot_max;
        }
    }

    cLib_addCalcAngleS2(&i_this->mHeadRotX, head_target_rotx, 2, rot_step);
    cLib_addCalcAngleS2(&i_this->mHeadRotZ, head_target_rotz, 2, rot_step);

    if (sp10 != 0) {
        if (sp10 == 1) {
            cMtx_YrotS(*calc_mtx, -a_this->shape_angle.y);
            sp50 = a_this->current.pos - a_this->old.pos;
            MtxPosition(&sp50, &sp44);
            sp44.x *= -400.0f;
            sp44.z *= 400.0f;

            f32 var_f29 = sp44.x;
            f32 var_f30 = -sp44.x;

            if (sp44.x < -1000.0f) {
                sp44.x = -1000.0f;
            } else if (sp44.x > 3500.0f) {
                sp44.x = 3500.0f;
            }

            cLib_addCalc2(&i_this->mArmLRotY, sp44.x, 0.2f, 200.0f);

            if (var_f29 > 1000.0f) {
                var_f29 = 1000.0f;
            } else if (var_f29 < -3500.0f) {
                var_f29 = -3500.0f;
            }

            cLib_addCalc2(&i_this->mArmRRotY, var_f29, 0.2f, 200.0f);

            if (var_f30 > 2000.0f) {
                var_f30 = 2000.0f;
            } else if (var_f30 < -2000.0f) {
                var_f30 = -2000.0f;
            }

            if (sp44.z > 2000.0f) {
                sp44.z = 2000.0f;
            } else if (sp44.z < -2000.0f) {
                sp44.z = -2000.0f;
            }

            cLib_addCalc2(&i_this->mBodyRotY, var_f30, 0.2f, 200.0f);
            cLib_addCalc2(&i_this->mBodyRotZ, sp44.z, 0.2f, 200.0f);
            cLib_addCalcAngleS2(&a_this->shape_angle.z, -0.5f * var_f30, 8, 0x400);
            cLib_addCalcAngleS2(&a_this->shape_angle.x, 0.5f * sp44.z, 8, 0x400);
        } else {
            f32 target_body_rot = a_this->current.angle.y - a_this->old.angle.y;
            target_body_rot *= -10.0f;

            if (target_body_rot > 9000.0f) {
                target_body_rot = 9000.0f;
            } else if (target_body_rot < -9000.0f) {
                target_body_rot = -9000.0f;
            }

            cLib_addCalc2(&i_this->mBodyRotY, target_body_rot, 0.5f, 1000.0f);
            cLib_addCalcAngleS2(&a_this->shape_angle.z, 0, 8, 0x800);
        }
    } else {
        cLib_addCalc0(&i_this->mArmLRotY, 0.1f, 100.0f);
        cLib_addCalc0(&i_this->mArmRRotY, 0.1f, 100.0f);
        cLib_addCalc0(&i_this->field_0x6cc, 0.1f, 100.0f);
        cLib_addCalc0(&i_this->field_0x6c4, 0.1f, 100.0f);
        cLib_addCalc0(&i_this->mBodyRotY, 0.1f, 100.0f);
        cLib_addCalc0(&i_this->mBodyRotZ, 0.1f, 100.0f);
        cLib_addCalcAngleS2(&a_this->shape_angle.z, 0, 8, 0x800);
    }
}

static BOOL ball_bg_check(e_hzelda_class* i_this) {
    cXyz start;
    cXyz end;
    cXyz unused;
    dBgS_LinChk linchk;

    start = i_this->mBallPos;
    end = i_this->mBallPos + ((i_this->mBallPos - i_this->mOldBallPos) * 2.0f);
    linchk.Set(&start, &end, i_this);

    if (dComIfG_Bgsp().LineCross(&linchk)) {
        return TRUE;
    }

    return FALSE;
}

static void demo_camera(e_hzelda_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* unused = dComIfGp_getCamera(0);
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    cXyz offset;
    cXyz unused2;
    cXyz target_eye;
    cXyz target_center;

    switch (i_this->mDemoMode) {
    case 0:
        break;
    case 1:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoMode = 2;
        i_this->mDemoCamFovy = 55.0f;
        i_this->mDemoTimer = 0;
        camera->mCamera.SetTrimSize(3);
        a_this->current.pos.set(0.0f, 500.0f, -2000.0f);
        a_this->shape_angle.y = 0;
        a_this->current.angle.y = 0;
    case 2:
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        target_center = a_this->current.pos;
        target_center.y += 100.0f;

        if (i_this->mDemoTimer == 0) {
            offset.x = 200.0f;
            offset.y = -300.0f;
            offset.z = 600.0f;
            MtxPosition(&offset, &target_eye);
            target_eye += a_this->current.pos;
            
            i_this->mDemoCamEye = target_eye;
            i_this->mDemoCamCenter = target_center;
        } else if (i_this->mDemoTimer == 60) {
            offset.x = -100.0f;
            offset.y = -50.0f;
            offset.z = 300.0f;
            MtxPosition(&offset, &target_eye);
            
            target_eye += a_this->current.pos;
            i_this->mDemoCamEye = target_eye;
        } else if (i_this->mDemoTimer == 160) {
            offset.x = -100.0f;
            offset.y = 250.0f + -50.0f;
            offset.z = 300.0f - 200.0f;
            MtxPosition(&offset, &target_eye);
            target_eye += a_this->current.pos;
            i_this->mDemoCamEye = target_eye;
        }

        cLib_addCalc2(&i_this->mDemoCamCenter.y, target_center.y, 0.1f, 5.0f);

        if (i_this->mDemoTimer == 220) {
            mDoGph_gInf_c::fadeOut(0.0166f, g_blackColor);
        }

        if (i_this->mDemoTimer == 280) {
            dStage_changeScene(1, 0.0f, 0, fopAcM_GetRoomNo(i_this), 0, -1);
        }
    }

    if (i_this->mDemoMode != 0) {
        camera->mCamera.Set(i_this->mDemoCamCenter, i_this->mDemoCamEye, i_this->mDemoCamFovy, 0);
        i_this->mDemoTimer++;
    }
}

static void anm_se_set(e_hzelda_class* i_this) {
    if (i_this->mAnm == ANM_BEFORE_ATTACK && i_this->mpModelMorf->checkFrame(21)) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_HZE_V_ATK_YOKOKU, -1);
    } else if (i_this->mAnm == ANM_ATTACK_A_BEFORE && i_this->mpModelMorf->checkFrame(1)) {
        i_this->mSound.startCreatureSound(Z2SE_EN_HZE_V_ATK_A_START, 0, -1);
    } else if (i_this->mAnm == ANM_ATTACK_A) {
        i_this->mSound.startCreatureSoundLevel(Z2SE_EN_HZE_ATK_A_MOVE, 0, -1);
    } else if (i_this->mAnm == ANM_ATTACK_C && i_this->mpModelMorf->checkFrame(36)) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_HZE_V_ATK_C_THROW, -1);
    } else if (i_this->mAnm == ANM_DAMAGE_UP && i_this->mpModelMorf->checkFrame(1)) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_HZE_V_REBIRTH, -1);
    } else if ((i_this->mAnm == ANM_ATTACKC_A || i_this->mAnm == ANM_ATTACKC_B) && i_this->mpModelMorf->checkFrame(26)) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_HZE_V_ATK_C_RETURN, -1);
    }
}

static u8 hio_set;

static daE_HZELDA_HIO_c l_HIO;

static int daE_HZELDA_Execute(e_hzelda_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp114;
    cXyz sp108;
    cXyz spFC;

    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (i_this->mDemoMode == 0 && !player->checkElecDamage() && dComIfGp_event_runCheck()) {
        return 1;
    }

    if (i_this->mMsgSetTimer != 0) {
        i_this->mMsgSetTimer--;
        if (i_this->mMsgSetTimer == 0) {
            fopMsgM_messageSetDemo(0x490);
        }
    }

    i_this->mCounter++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->field_0x6b4 != 0) {
        i_this->field_0x6b4--;
    }

    action(i_this);
    demo_camera(i_this);

    a_this->current.pos.y -= 30.0f;
    a_this->old.pos.y -= 30.0f;
    i_this->mAcch.CrrPos(dComIfG_Bgsp());

    a_this->current.pos.y += 30.0f;
    a_this->old.pos.y += 30.0f;

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(a_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
    
    f32 model_scale = l_HIO.mModelSize * a_this->scale.x;
    mDoMtx_stack_c::scaleM(model_scale, model_scale, model_scale);

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    i_this->mpModelMorf->modelCalc();
    anm_se_set(i_this);

    MTXCopy(model->getAnmMtx(JNT_HEAD), *calc_mtx);
    sp114.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp114, &a_this->eyePos);
    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += 80.0f;

    cXyz spF0(0.0f, 0.0f, 0.0f);
    if (i_this->field_0x6b4 != 0) {
        spF0.set(-200000.0f, 100000.0f, 20000.0f);
    }

    i_this->mSphCc[0].SetC(a_this->eyePos + spF0);
    i_this->mSphCc[0].SetR(25.0f);
    dComIfG_Ccsp()->Set(&i_this->mSphCc[0]);

    MTXCopy(model->getAnmMtx(JNT_WAIST), *calc_mtx);
    sp114.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp114, &sp108);
    i_this->mSphCc[1].SetC(sp108 + spF0);
    i_this->mSphCc[1].SetR(30.0f);
    dComIfG_Ccsp()->Set(&i_this->mSphCc[1]);

    MTXCopy(model->getAnmMtx(JNT_LEG_L2), *calc_mtx);
    sp114.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp114, &sp108);
    i_this->mSphCc[2].SetC(sp108 + spF0);
    i_this->mSphCc[2].SetR(35.0f);
    dComIfG_Ccsp()->Set(&i_this->mSphCc[2]);

    if (i_this->mpSwordModel != NULL) {
        MTXCopy(model->getAnmMtx(JNT_HAND_R), *calc_mtx);
        i_this->mpSwordModel->setBaseTRMtx(*calc_mtx);

        if (i_this->field_0x6b8 != 0) {
            sp114.set(20.0f, 0.0f, -40.0f);
            MtxPosition(&sp114, &sp108);
            i_this->mSphAt.SetC(sp108);
            i_this->mSphAt.SetR(30.0f);
            dComIfG_Ccsp()->Set(&i_this->mSphAt);
            i_this->field_0x6b8 = 0;
        }
    }

    i_this->field_0x958.Move();
    i_this->field_0xd3c.Move();
    i_this->field_0x127c.Move();

    BOOL on_player_at_sph = FALSE;

    if (daPy_py_c::checkMasterSwordEquip()) {
        if (daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_NONE) {
            i_this->mSwordAtTimer++;
            if (i_this->mSwordAtTimer < 6) {
                on_player_at_sph = TRUE;
            }
        } else {
            i_this->mSwordAtTimer = 0;
        }
    }

    if (daPy_getPlayerActorClass()->checkEmptyBottleSwing()) {
        i_this->mBottleSwingAtTimer++;
        if (i_this->mBottleSwingAtTimer < 10) {
            on_player_at_sph = TRUE;
        }
    } else {
        i_this->mBottleSwingAtTimer = 0;
    }

    if (on_player_at_sph) {
        daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        sp114.x = 0.0f;
        sp114.y = 110.0f;
        sp114.z = 40.0f;
        MtxPosition(&sp114, &sp108);
        sp108 += player->current.pos;

        i_this->mPlAtSph.SetC(sp108);
        i_this->mPlAtSph.SetR(80.0f);
        dComIfG_Ccsp()->Set(&i_this->mPlAtSph);
    }

    if (i_this->field_0x6e4 > 0) {
        i_this->mBallSound.startLevelSound(Z2SE_EN_HZE_ATK_C_BALL, 0, -1);
    
        if (i_this->field_0x6e4 == 1) {
            MTXCopy(model->getAnmMtx(JNT_HAND_R), *calc_mtx);
            i_this->mpSwordModel->setBaseTRMtx(*calc_mtx);
            sp114.set(20.0f, 0.0f, -80.0f);
            MtxPosition(&sp114, &i_this->mBallPos);
            cLib_addCalc2(&i_this->field_0x6e8, 1.0f, 1.0f, 0.02f);
        } else {
            i_this->mOldBallPos = i_this->mBallPos;
            i_this->mBallPos += i_this->mBallMove;

            cXyz spE4(i_this->mBallPos);
            BOOL sp64 = 0;

            if (i_this->field_0x6e4 == 2) {
                if (i_this->mBallSphAt.ChkAtShieldHit() || i_this->mBallSphAt.ChkAtHit()) {
                    if (i_this->mBallSphAt.ChkAtHit()) {
                        cCcD_Obj* hitobj = i_this->mBallSphAt.GetAtHitObj();
                        fopAc_ac_c* hit_actor = dCc_GetAc(hitobj->GetAc());

                        if (fopAcM_GetName(hit_actor) == PROC_ALINK) {
                            if (!daPy_getPlayerActorClass()->checkPlayerGuard()) {
                                i_this->mSound.startCreatureVoice(Z2SE_EN_HZE_V_LAUGH, -1);
                                sp64 = 1;
                            } else {
                                sp114 = a_this->current.pos - i_this->mBallPos;
                                sp114.y += 140.0f;

                                s16 spE = 0;
                                s16 spC = 0;

                                i_this->field_0x6e4 = 4;
                                cMtx_YrotS(*calc_mtx, (s16)cM_atan2s(sp114.x, sp114.z));
                                sp114.x = 0.0f;
                                sp114.y = 0.0f;
                                sp114.z = i_this->mBallMoveStep;
                                MtxPosition(&sp114, &i_this->mBallMove);
                                i_this->mBallMove.y = -10.0f;
                                i_this->field_0x6ba = 0;

                                i_this->mSound.startCreatureSound(Z2SE_EN_HZE_ATK_C_BALL_RETURN, 0, -1);
                                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER3, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                                ball_crash_eff_set(i_this, 0.4f);
                                dScnPly_c::setPauseTimer(2);

                                spE4.y += 10000.0f;
                                i_this->mTimers[2] = 80;
                            }
                        }
                    }
                } else if (i_this->mBallTgSph.ChkTgHit()) {
                    sp114 = a_this->current.pos - i_this->mBallPos;
                    sp114.y += 140.0f;

                    s16 spA = 0;
                    s16 sp8 = 0;

                    if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
                        if (cM_rndF(1.0f) < 0.5f) {
                            sp8 = 0xA00;
                        } else {
                            sp8 = -0xA00;
                        }

                        if (cM_rndF(1.0f) < 0.5f) {
                            spA = 0xA00;
                        } else {
                            spA = -0xA00;
                        }

                        i_this->field_0x6e4 = 4;
                    } else {
                        i_this->mMissedBallCount = 0;
                        i_this->field_0x6e4 = 3;
                    }

                    cMtx_YrotS(*calc_mtx, (sp8 + cM_atan2s(sp114.x, sp114.z)));
                    cMtx_XrotM(*calc_mtx, (spA - cM_atan2s(sp114.y, JMAFastSqrt((sp114.x * sp114.x) + (sp114.z * sp114.z)))));
                    sp114.x = 0.0f;
                    sp114.y = 0.0f;
                    sp114.z = i_this->mBallMoveStep;
                    MtxPosition(&sp114, &i_this->mBallMove);
                    i_this->field_0x6ba = 0;
    
                    i_this->mSound.startCreatureSound(Z2SE_EN_HZE_ATK_C_BALL_RETURN, 0, -1);
                    dComIfGp_getVibration().StartShock(3, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                    ball_crash_eff_set(i_this, 0.4f);
                    dScnPly_c::setPauseTimer(2);

                    spE4.y += 10000.0f;
                    i_this->mTimers[2] = 0x50;
                }
            } else if (i_this->field_0x6e4 >= 3) {
                spE4.y += 10000.0f;
                daPy_py_c* sp58 = (daPy_py_c*)dComIfGp_getPlayer(0);

                if (i_this->mTimers[2] == 0 || sp58->checkElecDamage()) {
                    sp64 = 1;
                }
            }

            if (sp64 || ball_bg_check(i_this)) {
                i_this->field_0x6e4 = -1;
                i_this->mSound.startCreatureSound(Z2SE_EN_HZE_ATK_C_BALL_HIT, 0, -1);

                spE4.y += 10000.0f;
            }

            i_this->mBallTgSph.SetC(spE4);
            i_this->mBallSphAt.MoveCAt(spE4);
            dComIfG_Ccsp()->Set(&i_this->mBallTgSph);
            dComIfG_Ccsp()->Set(&i_this->mBallSphAt);
        }

        for (int i = 0; i < 3; i++) {
            static u16 b_id[] = {0x8918, 0x8919, 0x891A};
            i_this->field_0x141c[i] = dComIfGp_particle_set(i_this->field_0x141c[i], b_id[i], &i_this->mBallPos, NULL, NULL);
        }
    } else if (i_this->field_0x6e4 < 0) {
        for (int i = 0; i < 3; i++) {
            JPABaseEmitter* emitterp = dComIfGp_particle_getEmitter(i_this->field_0x141c[i]);
            if (emitterp != NULL) {
                emitterp->deleteAllParticle();
                dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0x141c[i]);
            }
        }

        ball_crash_eff_set(i_this, 1.0f);
        i_this->field_0x6e4 = 0;
    }

    if (i_this->mDrawTriangleAt) {
        mDoMtx_stack_c::transS(i_this->mTrianglePos.x, i_this->mTrianglePos.y, i_this->mTrianglePos.z);
        mDoMtx_stack_c::YrotM(i_this->mTriangleRotY + 0x8000);

        f32 size = i_this->mTriangleSize;
        mDoMtx_stack_c::scaleM(size, size, size);
        i_this->mpTriangleAtModel->setBaseTRMtx(mDoMtx_stack_c::get());

        i_this->mpTriangleAtBrk->setFrame(i_this->mTriangleAnmFrame);
        i_this->mpTriangleAtBtk->setFrame(i_this->mTriangleAnmFrame);

        if (i_this->mTriangleAnmFrame < 210) {
            if (i_this->mTriangleAnmFrame < 100) {
                i_this->mTriangleAnmFrame += 2;
                if (i_this->mTriangleAnmFrame == 2) {
                    Z2GetAudioMgr()->seStart(Z2SE_EN_HZE_ATK_B_LIGHT, &i_this->mTrianglePos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                }

                if (i_this->mTriangleAnmFrame == 100) {
                    Z2GetAudioMgr()->seStart(Z2SE_EN_HZE_ATK_B_LIGHTWALL, &i_this->mTrianglePos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                    i_this->mSound.startCreatureVoice(Z2SE_EN_HZE_V_ATK_B_LIGHTWALL, -1);

                    csXyz particle_rot(0, i_this->mTriangleRotY + 0x8000, 0);
                    cXyz particle_size(size, size, size);

                    for (int i = 0; i < 5; i++) {
                        static u16 b_id[] = {0x8945, 0x8946, 0x8947, 0x8948, 0x8949};
                        dComIfGp_particle_set(b_id[i], &i_this->mTrianglePos, &particle_rot, &particle_size);
                    }
                }
            } else {
                i_this->mTriangleAnmFrame++;
            }
        }

        if (i_this->mTriangleAnmFrame > 105 && i_this->mTriangleAnmFrame < 135) {
            daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

            sp108.x = player->current.pos.x - i_this->mTrianglePos.x;
            sp108.y = 0.0f;
            sp108.z = player->current.pos.z - i_this->mTrianglePos.z;
            cMtx_YrotS(*calc_mtx, -i_this->mTriangleRotY);
            MtxPosition(&sp108, &sp114);

            f32 var_f31 = fabsf(57.295f * cM_atan2f(sp114.x, sp114.z));
            if (var_f31 >= 60.0f && var_f31 <= 120.0f) {
                var_f31 = 120.0f - var_f31;
            } else if (var_f31 >= 120.0f && var_f31 <= 180.0f) {
                var_f31 -= 120.0f;
            }

            f32 temp_f28 = 1.0f / cM_fcos(M_PI * (var_f31 / 180.0f));
            if (JMAFastSqrt((sp114.x * sp114.x) + (sp114.z * sp114.z)) < temp_f28 * (50.0f * i_this->mTriangleSize)) {
                i_this->mTriAtSph.SetC(player->current.pos);
                i_this->mTriAtSph.SetR(5.0f);
                dComIfG_Ccsp()->Set(&i_this->mTriAtSph);

                i_this->mSound.startCreatureVoice(Z2SE_EN_HZE_V_LAUGH, -1);
            }
        }
    }

    if (i_this->field_0x68c != 0) {
        cLib_addCalc2(&i_this->mSwordColorIntensity, 1.0f, 1.0f, 0.025f);
        i_this->field_0x68c = 0;
        i_this->field_0x1418 = dComIfGp_particle_set(i_this->field_0x1418, 0x895F, &a_this->current.pos, NULL, NULL);
        
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x1418);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(i_this->mpSwordModel->getBaseTRMtx());
            dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0x1418);
        }
    } else {
        cLib_addCalc0(&i_this->mSwordColorIntensity, 1.0f, 0.05f);
    }

    if (i_this->mAnm == ANM_DAMAGE || i_this->mAnm == ANM_LAST_DAMAGE || i_this->mAnm == ANM_DAMAGE_LOOP) {
        J3DModel* model = i_this->mpModelMorf->getModel();

        for (int i = 0; i < 6; i++) {
            static u16 b_id[] = {0x891B, 0x891C, 0x891D, 0x891E, 0x891F, 0x8920};
            i_this->field_0x1428[i] = dComIfGp_particle_set(i_this->field_0x1428[i], b_id[i], &a_this->current.pos, NULL, NULL);
        
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x1428[i]);
            if (emitter != NULL) {
                if (i <= 2) {
                    emitter->setGlobalSRTMatrix(model->getAnmMtx(JNT_BACKBONE2));
                } else {
                    emitter->setGlobalSRTMatrix(model->getAnmMtx(JNT_WAIST));
                }
            }
        }

        i_this->mSound.startCreatureSoundLevel(Z2SE_EN_HZE_DAMAGE, 0, -1);
    }

    i_this->mBallSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    return 1;
}

static int daE_HZELDA_IsDelete(e_hzelda_class* i_this) {
    return 1;
}

static int daE_HZELDA_Delete(e_hzelda_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "Hzelda");

    if (i_this->mInitHIO) {
        hio_set = 0;
    }

    if (i_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
        i_this->mBallSound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_hzelda_class* a_this = (e_hzelda_class*)i_this;

    a_this->mpModelMorf = new mDoExt_McaMorfSO(static_cast<J3DModelData*>(dComIfG_getObjectRes("Hzelda", 0x1B)), NULL, NULL,
                                               static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Hzelda", 0x15)),
                                               J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, &a_this->mSound, 0x80000, 0x11000284);
    if (a_this->mpModelMorf == NULL || a_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = a_this->mpModelMorf->getModel();
    model->setUserArea((uintptr_t)a_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("Hzelda", 0x1C));
    JUT_ASSERT(2129, modelData != NULL);
    a_this->mpSwordModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (a_this->mpSwordModel == NULL) {
        return 0;
    }

    modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("Hzelda", 0x1A));
    JUT_ASSERT(2149, modelData != NULL);
    a_this->mpTriangleAtModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (a_this->mpTriangleAtModel == NULL) {
        return 0;
    }

    a_this->mpTriangleAtBrk = new mDoExt_brkAnm();
    if (a_this->mpTriangleAtBrk == NULL) {
        return 0;
    }

    if (a_this->mpTriangleAtBrk->init(modelData, static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("Hzelda", 0x1F)),
                                       TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1) == FALSE) {
        return 0;
    }

    a_this->mpTriangleAtBtk = new mDoExt_btkAnm();
    if (a_this->mpTriangleAtBtk == NULL) {
        return 0;
    }

    if (a_this->mpTriangleAtBtk->init(a_this->mpTriangleAtModel->getModelData(),
                                       static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("Hzelda", 0x23)),
                                       TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1) == FALSE) {
        return 0;
    }

    a_this->mpTriangleAtBrk->setPlaySpeed(0.0f);
    a_this->mpTriangleAtBtk->setPlaySpeed(0.0f);
    return 1;
}

static int daE_HZELDA_Create(fopAc_ac_c* i_this) {
    e_hzelda_class* a_this = (e_hzelda_class*)i_this;
    fopAcM_ct(a_this, e_hzelda_class);

    int phase_state = dComIfG_resLoad(&a_this->mPhase, "Hzelda");
    a_this->mPrm0 = fopAcM_GetParam(i_this);

    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_HZELDA PARAM %x\n", fopAcM_GetParam(i_this));

        int swbit = fopAcM_GetParam(i_this) >> 0x18;
        if (swbit != 0xFF) {
            if (dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(i_this))) {
                return cPhs_ERROR_e;
            }
        }

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x4B000)) {
            OS_REPORT("//////////////E_HZELDA SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (hio_set == 0) {
            a_this->mInitHIO = 1;
            hio_set = 1;
            l_HIO.field_0x4 = -1;
        }

        fopAcM_SetMtx(i_this, a_this->mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(i_this, 200.0f, 200.0f, 200.0f);

        a_this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1,
                          &a_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->mAcchCir.SetWall(50.0f, 70.0f);

        i_this->health = 3;
        i_this->field_0x560 = 3;

        a_this->field_0x958.Init(200, 0, i_this);
        for (int i = 0; i <= 2; i++) {
            static dCcD_SrcSph cc_sph_src = {
                {
                    {0x0, {{0x0, 0x0, 0x0}, {0xd8fafdbf, 0x3}, 0x0}}, // mObj
                    {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                    {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                    {0x0}, // mGObjCo
                }, // mObjInf
                {
                    {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
                } // mSphAttr
            };
    
            a_this->mSphCc[i].Set(cc_sph_src);
            a_this->mSphCc[i].SetStts(&a_this->field_0x958);
        }

        a_this->field_0xd3c.Init(255, 0, i_this);

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xd}, {0xd8fafdbf, 0x3}, 0x0}}, // mObj
                {dCcD_SE_METAL, 0x0, 0xb, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
            } // mSphAttr
        };

        a_this->mSphAt.Set(at_sph_src);
        a_this->mSphAt.SetStts(&a_this->field_0xd3c);
        a_this->mSphAt.OnTgShield();
        a_this->mSphAt.SetTgHitMark(CcG_Tg_UNK_MARK_2);

        static dCcD_SrcSph ball_at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x4, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_13, 0x1, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 15.0f} // mSph
            } // mSphAttr
        };

        a_this->mBallSphAt.Set(ball_at_sph_src);
        a_this->mBallSphAt.SetStts(&a_this->field_0xd3c);
        a_this->mBallSphAt.SetAtMtrl(dCcD_MTRL_ELECTRIC);

        static dCcD_SrcSph ball_tg_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0x4000000, 0x3}, 0x0}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
            } // mSphAttr
        };

        a_this->mBallTgSph.Set(ball_tg_sph_src);
        a_this->mBallTgSph.SetStts(&a_this->field_0x958);
        a_this->mBallTgSph.OnTgNoHitMark();

        static dCcD_SrcSph tri_at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x4, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_13, 0x0, 0xe, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 10.0f} // mSph
            } // mSphAttr
        };

        a_this->mTriAtSph.Set(tri_at_sph_src);
        a_this->mTriAtSph.SetStts(&a_this->field_0x958);

        a_this->field_0x127c.Init(0xFF, 0, dComIfGp_getPlayer(0));

        static dCcD_SrcSph pl_at_sph_src = {
            {
                {0x0, {{AT_TYPE_MASTER_SWORD, 0x2, 0x3}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
            } // mSphAttr
        };

        a_this->mPlAtSph.Set(pl_at_sph_src);
        a_this->mPlAtSph.SetStts(&a_this->field_0x127c);

        a_this->mSound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        a_this->mAtInfo.mpSound = &a_this->mSound;

        a_this->mAction = ACTION_WAIT;
        a_this->mMode = -1;
        i_this->attention_info.distances[fopAc_attn_BATTLE_e] = 22;

        a_this->mBallSound.init(&a_this->mBallPos, 1);

        if (!dComIfGs_isSaveDunSwitch(2)) {
            a_this->mMsgSetTimer = 30;
            dComIfGs_onSaveDunSwitch(2);
        }

        a_this->mMissedBallCount = 1;
        daE_HZELDA_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_HZELDA_Method = {
    (process_method_func)daE_HZELDA_Create,
    (process_method_func)daE_HZELDA_Delete,
    (process_method_func)daE_HZELDA_Execute,
    (process_method_func)daE_HZELDA_IsDelete,
    (process_method_func)daE_HZELDA_Draw,
};

actor_process_profile_definition g_profile_E_HZELDA = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_HZELDA,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_hzelda_class), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  142,                    // mPriority
  &l_daE_HZELDA_Method,   // sub_method
  0x00044000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
