/**
* @file d_a_b_dre.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_dre.h"
#include "SSystem/SComponent/c_math.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_obj_ystone.h"
#include "d/actor/d_a_player.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_graphic.h"

int daB_DRE_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("B_DR", 0x49);
    JUT_ASSERT(0xB4, modelData != NULL);

    if (mAction == 0) {
        mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                           (J3DAnmTransform*)dComIfG_getObjectRes("B_DR", 0x1D), 0,
                                           1.0f, 0, -1, &mSound, 0, 0x11000084);
    } else {
        mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                           (J3DAnmTransform*)dComIfG_getObjectRes("B_DR", 0x1F), 2,
                                           1.0f, 0, -1, &mSound, 0, 0x11000084);
        if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
            return 0;
        }

        mpCoreBrk = new mDoExt_brkAnm();
        if (mpCoreBrk == NULL) {
            return 0;
        }

        J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("B_DR", 0x51);
        if (!mpCoreBrk->init(mpModelMorf->getModel()->getModelData(), pbrk, TRUE, 2, 1.0f, 0, -1)) {
            return 0;
        }
    }

    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return ((daB_DRE_c*)i_this)->CreateHeap();
}

static int daB_DRE_Create(fopAc_ac_c* i_this) {
    return ((daB_DRE_c*)i_this)->create();
}

void daB_DRE_c::SetAnm(mDoExt_McaMorfSO* i_modelMorf, int i_anm, int i_attr, f32 i_morf,
                       f32 i_speed) {
    i_modelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_DR", i_anm), i_attr, i_morf,
                        i_speed, 0.0f, -1.0f);
    mAnm = i_anm;
}

static int daB_DRE_Delete(daB_DRE_c* i_this) {
    i_this->Delete();
    return 1;
}

bool daB_DRE_c::CameraSet() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    dCamera_c* cam_body = dCam_getBody();

    if (!eventInfo.checkCommandDemoAccrpt()) {
        fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
        eventInfo.onCondition(0);
        return 0;
    }

    camera->mCamera.Stop();

    mCamFovy = cam_body->Fovy();
    mCamCenter = cam_body->Center();
    mCamCenterTarget = mCamCenter;
    mCamEye = cam_body->Eye();
    mCamEyeTarget = mCamEye;

    camera->mCamera.SetTrimSize(3);
    return 1;
}

void daB_DRE_c::SetStopingCam() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovy, 0);
}

void daB_DRE_c::SetStopCam(cXyz i_center, f32 param_1, f32 i_eyeY, s16 i_offsetAngle) {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    cXyz eye_pos(0.0f, i_eyeY, param_1);
    mCamCenter.set(i_center.x, i_center.y, i_center.z);
    mCamCenterTarget = mCamCenter;

    cLib_offsetPos(&eye_pos, &i_center, i_offsetAngle, &eye_pos);
    mCamEye = eye_pos;
    mCamEyeTarget = eye_pos;

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovy, 0);
    field_0x608 = param_1;
}

bool daB_DRE_c::SetCMoveCam(cXyz i_target, f32 i_step) {
    mCamCenterTarget = i_target;
    if (cLib_chasePos(&mCamCenter, mCamCenterTarget, i_step)) {
        return 1;
    }

    return 0;
}

void daB_DRE_c::SetCMoveCam(cXyz i_target, f32 i_scale, f32 i_step) {
    mCamCenterTarget = i_target;
    cLib_addCalcPos2(&mCamCenter, mCamCenterTarget, i_scale, i_step);
}

void daB_DRE_c::SetEyeMoveCam(cXyz param_0, f32 param_1, f32 param_2, s16 i_offsetAngle,
                              f32 i_step) {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    cXyz offset(0.0f, param_2, param_1);
    cXyz target(0.0f, 0.0f, 0.0f);
    cLib_offsetPos(&target, &param_0, i_offsetAngle, &offset);
    mCamEyeTarget = target;
    cLib_chasePos(&mCamEye, mCamEyeTarget, i_step);

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovy, 0);
}

void daB_DRE_c::SetEyeMoveCam(cXyz param_0, f32 param_1, f32 param_2, s16 i_offsetAngle,
                              f32 i_scale, f32 i_step) {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    cXyz target(0.0f, param_2, param_1);
    cLib_offsetPos(&target, &param_0, i_offsetAngle, &target);
    mCamEyeTarget = target;
    cLib_addCalcPos(&mCamEye, mCamEyeTarget, i_scale, i_step, 0.0f);

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovy, 0);
}

void daB_DRE_c::SetMoveCam(f32 i_scale, f32 i_step) {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    cLib_addCalcPos(&mCamCenter, mCamCenterTarget, i_scale, i_step, 0.0f);
    cLib_addCalcPos(&mCamEye, mCamEyeTarget, i_scale, i_step, 0.0f);

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovy, 0);
}

void daB_DRE_c::SetReleaseCam() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    camera->mCamera.Reset(mCamCenter, mCamEye);
    camera->mCamera.Start();
    camera->mCamera.SetTrimSize(0);
    dComIfGp_event_reset();
}

namespace {
static f32 s_dis;

static cXyz* s_LinkPos;

static s16 s_TargetAngle;

static cXyz s_HeadPos;

static cXyz s_BodyPos;

static cXyz s_TalePos;
}  // namespace

void daB_DRE_c::CamAction2() {
    cXyz pos;
    pos = current.pos;

    cXyz spD0;
    cXyz spC4;
    csXyz sp14;

    daPy_py_c* player = daPy_getPlayerActorClass();
    dCamera_c* camera = dCam_getBody();
    cXyz spB8;

    eyePos = s_BodyPos;
    attention_info.position = s_BodyPos;

    cXyz spAC;

    switch (mCameraMode) {
    case 0:
        pos.set(-1800.0f, 0.0f, 0.0f);
        player->setPlayerPosAndAngle(&pos, shape_angle.y + 0x5000, 0);
        if (mTimers[0] == 0) {
            mCameraMode++;
            mTimers[0] = 170;
            current.pos.y += 1000.0f;

            spAC = s_BodyPos;
            spAC.y += 1000.0f;
            SetStopCam(spAC, 600.0f, 200.0f, field_0x5fc);
        }
        break;
    case 1:
        if (mTimers[0] < 140) {
            SetCMoveCam(s_BodyPos, 9.0f);
            SetEyeMoveCam(s_BodyPos, 300.0f, 200.0f, field_0x5fc, 0.1f, 30.0f);
        }

        if (mTimers[0] == 0) {
            mCameraMode++;
            current.pos.set(0.0f, 120.0f, -700.0f);
            old.pos = current.pos;
            field_0x5fc = shape_angle.y + 0x500;
            pos.y = 0.0f;

            SetStopCam(current.pos, 1000.0f, -40.0f, field_0x5fc);
            mTimers[0] = 100;
            dComIfGp_event_setTalkPartner(this);
            player->changeDemoMode(0x17, 3, 0, 0);
            pos.set(0.0f, 0.0f, 800.0f + TREG_F(8));
            player->setPlayerPosAndAngle(&pos, shape_angle.y + 0x5000, 0);
            return;
        }
        break;
    case 2:
        field_0x5fc -= 5;
        if (mTimers[0] == 0) {
            SetAnm(mpModelMorf, 0x38, 0, 5.0f, 1.0f);
            dScnKy_env_light_c* kankyo = dKy_getEnvlight();
            kankyo->wether = 2;
            mDrMode++;
            mCameraMode++;
        }

        pos.y = 0.0f;

        SetEyeMoveCam(pos, 1000.0f, 80.0f, field_0x5fc, 5.0f);
        break;
    case 3:
        if (mTimers[0] == 0) {
            mBlurRate = 0.0f;
        }

        if (mpModelMorf->getFrame() > 250.0f && mAnm == 0x17) {
            SetCMoveCam(s_HeadPos, 0.1f, 100.0f);
        } else {
            spD0 = s_HeadPos;

            if (!(mpModelMorf->getFrame() <= 130.0f) || mAnm != 0x38) {
                spD0.y -= 1000.0f + TREG_F(12);
            }

            if (spD0.y < s_TalePos.y) {
                spD0.y = s_TalePos.y;
            }

            SetCMoveCam(spD0, 0.1f, 100.0f);
        }

        pos.y = 0.0f;

        if (mpModelMorf->getFrame() > 130.0f && mAnm == 0x38) {
            SetEyeMoveCam(pos, 1650.0f + TREG_F(5), 1.0f + TREG_F(13), field_0x5fc, 0.2f,
                          100.0f + TREG_F(9));
        } else if (mAnm == 0x17) {
            SetEyeMoveCam(pos, 1650.0f + TREG_F(5), 1.0f + TREG_F(13), field_0x5fc, 0.2f,
                          100.0f + TREG_F(9));
        } else {
            SetEyeMoveCam(pos, 1000.0f, 80.0f, field_0x5fc, 5.0f + TREG_F(10));
            field_0x5fc -= 2.0f + TREG_F(11);
        }
        break;
    case 4:
        if (mDrMode != 4) {
            pos.y = 0.0f;
            SetEyeMoveCam(pos, 2500.0f, 200.0f, field_0x5fc, 5.0f + XREG_F(2));
            SetCMoveCam(s_BodyPos, 0.1f, 100.0f);
            SetStopingCam();
        }
        break;
    }
}

void daB_DRE_c::DrAction2() {
    cXyz eye_pos;
    cXyz sp8;
    switch (mDrMode) {
    case 1:
        if (mpModelMorf->checkFrame(130.0f)) {
            mTargetSpeedY = 0.0f;
            speed.y = 150.0f + TREG_F(6);
        }

        if (mpModelMorf->isStop()) {
            mDrMode++;
            SetAnm(mpModelMorf, 0x17, 0, 5.0f, 1.0f);
        }
        break;
    case 0:
        break;
    case 2:
        if (mpModelMorf->checkFrame(110.0f)) {
            argument = 1;
            mBlurRate = 150.0f;
            mTimers[0] = 100;
            mHideParts = true;
        } else if (mpModelMorf->checkFrame(270.0f)) {
            mBlurRate = 150.0f;
            mTimers[0] = 100;
            mHideParts = true;
        }

        if (mpModelMorf->getFrame() < 270.0f) {
            cLib_addCalc2(&mBlurRate, 0.0f, 0.2f, 1.0f);
        } else {
            cLib_addCalc2(&mBlurRate, 0.0f, 0.2f, 1.0f);
        }

        if (mpModelMorf->isStop()) {
            SetAnm(mpModelMorf, 0x35, 2, 5.0f, 1.0f);
            mTargetSpeedY = 30.0f;
            mCameraMode++;
            mDrMode++;
            dComIfGp_getVibration().StopQuake(0x1F);
            return;
        }
        break;
    case 3:
        if (current.pos.y > 5000.0f) {
            mDrMode++;

            sp8.set(0.0f, 0.0f, 0.0f);
            eye_pos.set(0.0f, 200.0f, 1500.0f);
            cLib_offsetPos(&sp8, &current.pos, shape_angle.y, &eye_pos);
            mCamEye = eye_pos;
            mCamCenter = *s_LinkPos;
            mCamCenter.y += 100.0f;

            fopAcM_onSwitch(this, 16);
            mTimers[0] = 50;
            return;
        }
        break;
    case 4:
        if (mTimers[0] == 0) {
            fopAcM_delete(this);
        }
        break;
    }
}

void daB_DRE_c::CamAction() {
    cXyz pos;
    pos = current.pos;
    pos.y += 200.0f;

    cXyz sp94;
    cXyz sp88;
    csXyz sp8;

    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (mCameraMode) {
    case 0:
        if (CameraSet()) {
            dComIfGs_offSwitch(16, fopAcM_GetRoomNo(this));
            dComIfGs_offSwitch(63, fopAcM_GetRoomNo(this));

            dComIfGp_getVibration().StartQuake(VIBMODE_Q_POWER5, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            mCameraMode++;
            field_0x5fc = shape_angle.y + 0x7000;
            SetStopCam(pos, 1000.0f + yREG_F(1), 100.0f + yREG_F(2), field_0x5fc);
            mTimers[0] = 0;
            mDrMode++;

            player->cancelOctaIealHang();
            pos.set(100.0f + HREG_F(0), 0.0f, 800.0f + HREG_F(1));
            player->setPlayerPosAndAngle(&pos, shape_angle.y + 0x5000, 0);
        }
        break;
    case 1:
        field_0x5fc -= 16;
        SetCMoveCam(pos, 0.1f, 100.0f);
        SetEyeMoveCam(pos, field_0x608, 500.0f, field_0x5fc, 50.0f);
        break;
    case 2:
        field_0x5fc -= 32;
        SetCMoveCam(s_HeadPos, 0.2f + aREG_F(11), 200.0f + aREG_F(12));
        SetEyeMoveCam(pos, 1000.0f + yREG_F(3), -500.0f + yREG_F(4), field_0x5fc, 10.0f);

        pos.set(100.0f + HREG_F(0), 0.0f, 800.0f + HREG_F(1));
        player->setPlayerPosAndAngle(&pos, shape_angle.y + 0x5000, 0);
        break;
    case 3:
        pos.y += 600.0f;
        SetCMoveCam(pos, 0.2f, 20.0f);
        SetEyeMoveCam(pos, 1000.0f + yREG_F(5), yREG_F(6), field_0x5fc, 2.0f);
        break;
    case 4:
        pos = current.pos;
        pos.y += 800.0f + yREG_F(14);
        SetCMoveCam(pos, 0.4f, 300.0f + yREG_F(16));

        if (mTimers[3] == 0 && mAnm == 0x19) {
            SetAnm(mpModelMorf, 0x24, 0, 5.0f, 1.0f);
            mDrMode++;
        }
        SetStopingCam();
        break;
    case 5:
        SetCMoveCam(current.pos, 0.4f, 100.0f);
        SetStopingCam();
        break;
    case 6:
        SetStopingCam();
        break;
    case 7:
        SetMoveCam(0.05f + hREG_F(0), 5.0f + hREG_F(1));
        break;
    case 8:
        if (mCamCenter.y < 280.0f + XREG_F(8)) {
            SetReleaseCam();
            mCameraMode++;
            mDrMode++;
            mTimers[0] = 50;
        }

        SetMoveCam(0.05f + hREG_F(0), 5.0f + hREG_F(1));
        break;
    case 9:
        if (mTimers[0] == 0) {
            fopAcM_delete(this);
        }
        break;
    }
}

void daB_DRE_c::Action() {
    switch (mAction) {
    case 0:
        CamAction();
        DrAction();
        break;
    case 1:
        CamAction2();
        DrAction2();
        break;
    }
}

void daB_DRE_c::DrAction() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp90;
    csXyz sp1C;
    cXyz sp84;
    cXyz sp78;

    switch (mDrMode) {
    case 0:
        BreathSet();
        break;
    case 1:
        if (mTimers[0] == 0) {
            mCameraMode++;
            SetStopCam(s_HeadPos, 800.0f, -800.0f, shape_angle.y + 0x1500);
            field_0x5fc = shape_angle.y + 0x1000;
            mDrMode++;
        }
        BreathSet();
        break;
    case 2:
        if (mpModelMorf->checkFrame(80.0f)) {
            dComIfGp_getVibration().StopQuake(0x1F);
        }

        if (mpModelMorf->isStop()) {
            SetAnm(mpModelMorf, 0x19, 2, 10.0f, 1.0f);
            mDrMode++;
            mCameraMode++;
            mTimers[3] = 140;

            player->changeOriginalDemo();
            player->changeDemoMode(4, 2, 0, 0);
            mTimers[2] = 30;
        }
        BreathSet();
        break;
    case 3:
        if (mTimers[0] == 0) {
            mTargetSpeedY = 30.0f;
            mTimers[0] = 50.0f + cM_rndF(50.0f);
        }

        if (mTimers[3] == 0 && mpModelMorf->checkFrame(1.0f)) {
            mTimers[3] = 60;
            mCameraMode++;
            field_0x5fc = shape_angle.y;
            sp90 = current.pos;
            sp90.y += 800.0f + yREG_F(14);
            SetStopCam(sp90, 100.0f + yREG_F(12), 4100.0f + yREG_F(13), shape_angle.y);
            mTargetSpeedF = 0.0f;
        }

        if (mTimers[1] != 0) {
            BreathSet();
        }

        if (mTimers[1] == 1) {
            mTimers[2] = 30.0f + cM_rndF(30.0f);
        }

        if (mTimers[2] == 1) {
            mTimers[1] = 30.0f + cM_rndF(30.0f);
        }
        break;
    case 4:
        if (mpModelMorf->getFrame() > 40.0f) {
            mTargetSpeedY -= 1.2f;
        }

        if (mpModelMorf->isStop()) {
            SetAnm(mpModelMorf, 0x21, 2, 5.0f, 1.0f);
        }

        if (current.pos.y < 12000.0f + TREG_F(0)) {
            current.pos.y = 7000.0f;
            old.pos.y = 7000.0f;
            sp90 = current.pos;
            mCameraMode++;

            SetStopCam(sp90, 6500.0f + yREG_F(17), -7000.0f + yREG_F(18), shape_angle.y);
            field_0x5fc = shape_angle.y;
            mDrMode++;
        }
    case 5:
        if (current.pos.y < 3000.0f) {
            sp84 = current.pos;
            sp84.y += 300.0f;

            fopAcM_createDisappear(this, &sp84, 49.0f + XREG_F(0), 0, XREG_F(1));
            mAnm = 0x1F;

            dScnKy_env_light_c* kankyo = dKy_getEnvlight();
            kankyo->wether = 0;

            mHideModel = true;
            mDrMode++;
            mTargetSpeedY = 0.0f;
            speed.y = 0.0f;
            speedF = 0.0f;

            sp90 = current.pos;
            sp1C.set(0, shape_angle.y + 0x1000, 0);
            fopAcM_createItemForBoss(&sp90, fpcNm_ITEM_UTAWA_HEART, fopAcM_GetRoomNo(this), &sp1C,
                                     NULL, 6.0f + TREG_F(9), 20.0f + TREG_F(10), -1);
            mTimers[0] = 30;
        }
        break;
    case 6:
        fopAc_ac_c* lifecon;
        fopAcM_SearchByName(PROC_Obj_LifeContainer, &lifecon);
        if (lifecon != NULL) {
            dComIfGp_event_setTalkPartner(lifecon);
            player->changeDemoMode(0x2F, 3, 0, 0);
        }

        if (mTimers[0] == 0) {
            sp90.set(150.0f + aREG_F(0), 100.0f, 800.0f + aREG_F(1));
            SetStopCam(sp90, 300.0f + aREG_F(3), 0.0f, shape_angle.y - 0x1000);
            mCameraMode++;
            mTimers[0] = 150;
            mDrMode++;
        }
        break;
    case 7:
        if (mTimers[0] == 0) {
            mDrMode++;
            mCameraMode++;
            mTimers[0] = 80;
        }
        break;
    case 8:
        if (mTimers[0] == 0) {
            cXyz sp6C(0.0f, 0.0f, 0.0f);
            sp90.set(150.0f + aREG_F(0), 400.0f + XREG_F(4), 800.0f);
            cLib_offsetPos(&sp90, &sp90, shape_angle.y + 0x5000, &sp6C);
            fopAcM_create(PROC_OBJ_YSTONE, 0, &sp90, fopAcM_GetRoomNo(this), NULL, NULL, -1);

            sp90.set(-325.0f, 0.0f, 215.0f);
            sp1C.set(0, 0, 0);
            fopAcM_createWarpHole(&sp90, &sp1C, fopAcM_GetRoomNo(this), 2, 1, 0xFF);

            mDrMode++;
            mCameraMode++;
            mTimers[0] = 155;
            field_0x638 = 0;
        }

        cLib_addCalc2(&mCamCenterTarget.y, 350.0f, 0.1f + hREG_F(5), 10.0f + hREG_F(6));
        break;
    case 9:
        if (mTimers[0] == 145) {
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_KAGERI_MIRROR_GET, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
        }

        fopAc_ac_c* ystone;
        fopAcM_SearchByName(PROC_OBJ_YSTONE, &ystone);

        if (ystone != NULL && field_0x638 == 0) {
            field_0x638 = 1;
            dComIfGp_event_setTalkPartner(ystone);
        }

        if (ystone != NULL && mTimers[0] == 0) {
            player->changeDemoMode(0x2F, 3, 0, 0);
            cXyz pos(ystone->current.pos);
            pos.y -= 1.0f;
            ((obj_ystone_class*)ystone)->setCurrentPos(pos);

            if (mCamCenterTarget.y >= (pos.y - 70.0f) + XREG_F(6)) {
                cLib_addCalc2(&mCamCenterTarget.y, (pos.y - 70.0f) + XREG_F(6), 0.1f + hREG_F(7),
                              2.0f + hREG_F(8));
            }
        }
        break;
    }
}

void daB_DRE_c::SoundChk() {
    switch (mAnm) {
    case 0x21:
        mSound.startCreatureSoundLevel(Z2SE_EN_DR_FALL, 0, -1);
        break;
    case 0x1D:
        if (mpModelMorf->checkFrame(0)) {
            mSound.startCreatureSound(Z2SE_EN_DR_V_DEATH, 0, -1);
        }

        if (mpModelMorf->checkFrame(5) || mpModelMorf->checkFrame(43) ||
            mpModelMorf->checkFrame(70) || mpModelMorf->checkFrame(91) ||
            mpModelMorf->checkFrame(113) || mpModelMorf->checkFrame(133) ||
            mpModelMorf->checkFrame(160) || mpModelMorf->checkFrame(186) ||
            mpModelMorf->checkFrame(213) || mpModelMorf->checkFrame(238) ||
            mpModelMorf->checkFrame(261))
        {
            mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
        }
        break;
    case 0x35:
        if (mpModelMorf->checkFrame(7)) {
            mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
        }
        break;
    case 0x17:
        if (mpModelMorf->checkFrame(7) || mpModelMorf->checkFrame(40) ||
            mpModelMorf->checkFrame(75) || mpModelMorf->checkFrame(143) ||
            mpModelMorf->checkFrame(177) || mpModelMorf->checkFrame(210) ||
            mpModelMorf->checkFrame(243) || mpModelMorf->checkFrame(270) ||
            mpModelMorf->checkFrame(309))
        {
            mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
        }

        if (mpModelMorf->checkFrame(0)) {
            mSound.startCreatureSound(Z2SE_EN_DR_BREAK_ARMOR, 0, -1);
        }

        if (mpModelMorf->checkFrame(268)) {
            mSound.startCreatureVoice(Z2SE_EN_DR_V_BURU, -1);
        }

        if (mpModelMorf->checkFrame(110)) {
            dComIfGp_getVibration().StartQuake(VIBMODE_Q_POWER8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        } else if (mpModelMorf->checkFrame(130)) {
            dComIfGp_getVibration().StopQuake(0x1F);
        } else if (mpModelMorf->checkFrame(270)) {
            dComIfGp_getVibration().StartQuake(VIBMODE_Q_POWER5, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        }
        break;
    case 0x19:
        if (mpModelMorf->checkFrame(5) || mpModelMorf->checkFrame(35) ||
            mpModelMorf->checkFrame(65) || mpModelMorf->checkFrame(95) ||
            mpModelMorf->checkFrame(125))
        {
            mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
        }
        break;
    case 0x38:
        if (mpModelMorf->checkFrame(102) || mpModelMorf->checkFrame(130) ||
            mpModelMorf->checkFrame(157) || mpModelMorf->checkFrame(180))
        {
            mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
        }

        if (mpModelMorf->checkFrame(0)) {
            mSound.startCreatureVoice(Z2SE_EN_DR_V_WAKEUP, -1);
        }
        break;
    }
}

int daB_DRE_c::Execute() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    s_LinkPos = &fopAcM_GetPosition(player);
    s_TargetAngle = cLib_targetAngleY(&current.pos, s_LinkPos);
    s_dis = current.pos.abs(*s_LinkPos);

    cXyz pos(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0x13));
    mDoMtx_stack_c::multVec(&pos, &pos);
    s_HeadPos = pos;

    pos.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0xF));
    mDoMtx_stack_c::multVec(&pos, &pos);
    s_BodyPos = pos;

    pos.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0x2D));
    mDoMtx_stack_c::multVec(&pos, &pos);
    s_TalePos = pos;

    for (int i = 0; i < 5; i++) {
        mTimers[i]--;
        if (mTimers[i] <= 0) {
            mTimers[i] = 0;
        }
    }

    Action();
    SoundChk();
    SpeedSet();

    current.pos.x += speed.x;
    current.pos.y += speed.y;
    current.pos.z += speed.z;

    if (mAction == 1) {
        mpCoreBrk->play();
    }

    mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mAcch.CrrPos(dComIfG_Bgsp());
    setBaseMtx();
    return 1;
}

void daB_DRE_c::SpeedSet() {
    cLib_addCalc2(&speed.y, mTargetSpeedY, 0.2f, 10.0f);
    cLib_addCalc2(&speedF, mTargetSpeedF, 0.2f, 10.0f);
}

static u8 data_805CB0D4;

void daB_DRE_c::BreathSet() {
    cXyz sp20;
    cXyz sp14;
    cLib_addCalc2(&field_0x618, 50.0f, 0.7f, 4.0f);
    data_805CB0D4 = field_0x618;

    csXyz angle(shape_angle);
    for (int i = 0; i < 4; i++) {
        static u16 effId[] = {0x899B, 0x899C, 0x899D, 0x899E};
        mBreathEmtrIDs[i] =
            dComIfGp_particle_set(mBreathEmtrIDs[i], effId[i], &s_HeadPos, &angle, NULL);

        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mBreathEmtrIDs[i]);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(mpModelMorf->getModel()->getAnmMtx(0x13));
        }

        mSound.startCreatureVoiceLevel(Z2SE_EN_DR_V_FIRE, -1);
    }
}

int daB_DRE_c::Delete() {
    dComIfG_resDelete(&mPhase, "B_DR");

    if (heap != NULL) {
        mpModelMorf->stopZelAnime();
    }

    return 1;
}

void daB_DRE_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(0.8f, 0.8f, 0.8f);
    mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());

    if (mAction == 0) {
        for (u16 i = 0; i < mpModelMorf->getModel()->getModelData()->getJointNum(); i++) {
            if (i != 0) {
                mpModelMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(NULL);
            }
        }
    }

    mpModelMorf->modelCalc();
}

int daB_DRE_c::Draw() {
    J3DModel* model = mpModelMorf->getModel();
    if (!mHideModel) {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);

        if (mAction == 1) {
            mpCoreBrk->entry(model->getModelData());
            if (mHideParts) {
                J3DShape* shape =
                    mpModelMorf->getModel()->getModelData()->getMaterialNodePointer(2)->getShape();
                if (shape != NULL) {
                    shape->hide();
                }
            }
        }

        mpModelMorf->entryDL();
        if (mAction == 1) {
            mpCoreBrk->remove(model->getModelData());
        }
    }

    cXyz shadow_pos;
    shadow_pos.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);

    if (mAction != 0) {
        tevStr.field_0x344 = 1.0f;
        mShadowKey = dComIfGd_setShadow(mShadowKey, 0, model, &shadow_pos, BREG_F(19) + 6000.0f,
                                        BREG_F(18), current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd,
                                        &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    if (mBlurRate != 0.0f) {
        mDoGph_gInf_c::setBlureRate(mBlurRate);
        mDoGph_gInf_c::onBlure();
    } else {
        mDoGph_gInf_c::offBlure();
    }

    return 1;
}

static int daB_DRE_Draw(daB_DRE_c* i_this) {
    return i_this->Draw();
}

static int daB_DRE_Execute(daB_DRE_c* i_this) {
    return i_this->Execute();
}

int daB_DRE_c::create() {
    fopAcM_ct(this, daB_DRE_c);

    int phase_state = dComIfG_resLoad(&mPhase, "B_DR");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("B_DRE PARAM %x\n", fopAcM_GetParam(this));
        mAction = fopAcM_GetParam(this) & 0xFF;

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x4B000)) {
            return cPhs_ERROR_e;
        }

        daPy_py_c* player = daPy_getPlayerActorClass();
        fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());

        mAcchCir.SetWall(50.0f, 80.0f);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);

        mSound.init(&current.pos, &eyePos, 3, 1);

        fopAcM_SetMin(this, -40000.0f, -40000.0f, -40000.0f);
        fopAcM_SetMax(this, 40000.0f, 40000.0f, 40000.0f);

        mAtInfo.mpSound = &mSound;
        mHideModel = false;
        mDrMode = 0;
        field_0x5b5 = 0;
        mCameraMode = 0;
        field_0x5b9 = 0;

        setBaseMtx();

        dCamera_c* camera = dCam_getBody();

        mSound.setEnemyName("B_DR");

        if (mAction == 0) {
            current.pos.x = 0.0f;
            current.pos.z = 0.0f;
            current.pos.y = 9600.0f;

            daPy_py_c* player = daPy_getPlayerActorClass();
            mAnm = 29;
        } else {
            mCamFovy = camera->Fovy();
            mCamCenter = camera->Center();
            mCamCenterTarget = mCamCenter;
            mCamEye = camera->Eye();
            mCamEyeTarget = mCamEye;

            field_0x5fc = shape_angle.y - 0x4000;
            current.pos.x += 1800.0f;
            current.pos.y -= 1000.0f;

            mTimers[0] = 80;
            mSound.startCreatureSound(Z2SE_EN_DR_CORE_GLOW, 0, -1);
        }

        old.pos = current.pos;
        shape_angle.y = 0;

        daB_DRE_Execute(this);
    }

    return phase_state;
}

static int daB_DRE_IsDelete(daB_DRE_c* i_this) {
    return 1;
}

static actor_method_class l_daB_DRE_Method = {
    (process_method_func)daB_DRE_Create,  (process_method_func)daB_DRE_Delete,
    (process_method_func)daB_DRE_Execute, (process_method_func)daB_DRE_IsDelete,
    (process_method_func)daB_DRE_Draw,
};

extern actor_process_profile_definition g_profile_B_DRE = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_B_DRE,              // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daB_DRE_c),       // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    231,                     // mPriority
    &l_daB_DRE_Method,       // sub_method
    0x00044100,              // mStatus
    fopAc_ENEMY_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
