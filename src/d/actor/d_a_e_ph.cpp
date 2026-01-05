/**
 * d_a_e_ph.cpp
 * Peahat Enemy
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_ph.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"

#define PH_BMD 20

#define PH_JNT_HEAD 2

#define ANM_APPEAR 4
#define ANM_DAMAGE 5
#define ANM_DAMAGE_ARROW 6
#define ANM_DEAD 7
#define ANM_FALL 8
#define ANM_FALL_WAIT 9
#define ANM_HANG_END 10
#define ANM_HANG_START 11
#define ANM_HANG_WAIT 12
#define ANM_LEAVE 13
#define ANM_WAIT 14
#define ANM_WAKE 15
#define ANM_WIND 16
#define ANM_WIND_FALL 17


daE_PH_HIO_c::daE_PH_HIO_c() {
    field_0x4 = -1;
    mHangPos = 0.0f;
    mStopTime = 0.0f;
    mSpeed = 0.0f;
    mRotationInterpolation = 1.0f;
    mRotationYInterpolation = 0.1f;
}

void daE_PH_c::initCcSph() {
    const static dCcD_SrcSph ccSphSrc = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x43}, 0x75}},  // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                  // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x1},                  // mGObjTg
            {0x0},                                               // mGObjCo
        },                                                       // mObjInf
        {
            {{0.0f, -100.0f, 0.0f}, 60.0f}  // mCcSph
        }                                   // mSphAttr
    };

    mCcStts.Init(0xFF, 0xFF, this);
    mCcSph.Set(ccSphSrc);
    mCcSph.SetStts(&mCcStts);
    mCcSph.SetC(current.pos);
}

static BOOL other_bg_check(daE_PH_c* i_this, fopAc_ac_c* i_other) {
    daE_PH_c* start_actor = (daE_PH_c*)i_this;  // necessary to fix reg alloc

    dBgS_LinChk line_chk;
    cXyz sp28;
    cXyz start;
    cXyz end;

    end = i_other->current.pos;
    end.y += 100.0f;

    start = start_actor->current.pos;
    start.y += 100.0f;

    line_chk.Set(&start, &end, start_actor);
    if (dComIfG_Bgsp().LineCross(&line_chk)) {
        return true;
    }

    return false;
}

void daE_PH_c::SetStopingCam() {
    camera_class* camera_p = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    camera_p->mCamera.Set(mCamCenter, mCamEye, mCamFovY, 0);
}

void daE_PH_c::SetStopCam(cXyz i_center, f32 i_offsetZ, f32 i_offsetY, s16 i_offsetAngle) {
    camera_class* camera_p = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz offset(0.0f, i_offsetY, i_offsetZ);
    cXyz eye_offset(0.0f, 0.0f, 0.0f);

    mCamCenter.set(i_center.x, i_center.y, i_center.z);
    mCamCenterTarget = mCamCenter;

    cLib_offsetPos(&eye_offset, &i_center, i_offsetAngle, &offset);
    mCamEye = eye_offset;
    mCamEyeTarget = eye_offset;

    camera_p->mCamera.Set(mCamCenter, mCamEye, mCamFovY, 0);
}

void daE_PH_c::SetCMoveCam(cXyz i_target, f32 i_scale, f32 i_maxStep) {
    mCamCenterTarget = i_target;
    cLib_addCalcPos2(&mCamCenter, mCamCenterTarget, i_scale, i_maxStep);
}

void daE_PH_c::SetReleaseCam() {
    camera_class* camera_p = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    camera_p->mCamera.Reset(mCamCenter, mCamEye);
    camera_p->mCamera.Start();
    camera_p->mCamera.SetTrimSize(0);
    dComIfGp_event_reset();
}

int daE_PH_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jnt_no = i_joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));

    if (jnt_no == PH_JNT_HEAD) {
        mDoMtx_stack_c::YrotM(mHeadRotY);
        mDoMtx_stack_c::XrotM(-mHeadRotX);
    }

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

static int JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daE_PH_c* a_this = (daE_PH_c*)model->getUserArea();
        if (a_this != NULL) {
            a_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daE_PH_c::setCcSph() {
    cXyz center(0.0f, AREG_F(0) + -10.0f, 0.0f);

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::multVec(&center, &center);

    mCcSph.SetC(center);
    mCcSph.SetR(AREG_F(1) + 80.0f);
    dComIfG_Ccsp()->Set(&mCcSph);
}

int daE_PH_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_PH", PH_BMD);
    JUT_ASSERT(0, modelData != NULL);

    mpMorf = new mDoExt_McaMorfSO(
        modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_PH", ANM_WAIT),
        J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, &mSound, 0x80000, 0x11000084);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daE_PH_c*>(i_this)->CreateHeap();
}

static int daE_PH_Create(fopAc_ac_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<daE_PH_c*>(i_this)->create();
}

static int daE_PH_Delete(daE_PH_c* i_this) {
    fopAcM_GetID(i_this);
    i_this->Delete();
    return 1;
}

void daE_PH_c::SetAnm(int i_anmID, int i_attr, f32 i_morf, f32 i_speed) {
    J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectRes("E_PH", i_anmID);
    mpMorf->setAnm(anm, i_attr, i_morf, i_speed, 0.0f, -1.0f);
    mAnmID = i_anmID;
}

void daE_PH_c::SearchNearP() {
    f32 nearest_dist = G_CM3D_F_INF;

    for (int i = 0; i < mpPath->m_num; i++) {
        f32 pnt_dist = current.pos.absXZ(dPath_GetPnt(mpPath, i)->m_position);
        if (pnt_dist < nearest_dist) {
            nearest_dist = pnt_dist;

            mCurrentPntNo = i;
            mCurrentPntPos.x = current.pos.x;
            mCurrentPntPos.y = current.pos.y;
            mCurrentPntPos.z = current.pos.z;

            if (mCurrentPntNo == mpPath->m_num - 1) {
                if (dPath_ChkClose(mpPath)) {
                    mCurrentPntNo = 0;
                } else {
                    mCurrentPntNo--;
                }
            } else {
                mCurrentPntNo++;
            }

            if (mAction == 1) {
                SetCheckAngle();
            } else {
                S_SetAngle();
            }
        }
    }
}

void daE_PH_c::FlyAnm() {
    if (mAnmID == ANM_DAMAGE_ARROW || mAnmID == ANM_HANG_START || mAnmID == ANM_HANG_WAIT) {
        if (mpMorf->isStop()) {
            SetAnm(ANM_WAIT, J3DFrameCtrl::EMode_LOOP, 5.0f, mAnmSpeed);
        }

        mFlyRockEMKey = dComIfGp_particle_set(mFlyRockEMKey, 0x878C, &current.pos, &tevStr,
                                              &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
        mFlyTsubuEMKey =
            dComIfGp_particle_set(mFlyTsubuEMKey, 0x878D, &current.pos, &tevStr, &shape_angle,
                                  NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
    }

    if (mAnmID == ANM_HANG_START || mAnmID == ANM_WAIT) {
        if (fopAcM_checkHookCarryNow(this) && dComIfGp_checkPlayerStatus1(0, 0x10000)) {
            field_0x630 = -10.0f - BREG_F(0);
            field_0x5ae = 15;
            field_0x616 = 0x1000;
            field_0x618 = 0x1000;
            SetAnm(ANM_HANG_WAIT, J3DFrameCtrl::EMode_LOOP, 5.0f, mAnmSpeed);
        } else if (mAnmID != ANM_WAIT) {
            SetAnm(ANM_WAIT, J3DFrameCtrl::EMode_LOOP, 5.0f, mAnmSpeed);
        }
    } else if (mAnmID == ANM_HANG_WAIT) {
        if (!fopAcM_checkHookCarryNow(this) || !dComIfGp_checkPlayerStatus1(0, 0x10000)) {
            SetAnm(ANM_HANG_END, J3DFrameCtrl::EMode_NONE, 5.0f, mAnmSpeed);
        }
    } else if (mAnmID == ANM_HANG_END) {
        if (mpMorf->isStop()) {
            SetAnm(ANM_WAIT, J3DFrameCtrl::EMode_LOOP, 5.0f, mAnmSpeed);
        }
    }
}

static u8 initialized;

static daE_PH_HIO_c l_HIO;

void daE_PH_c::SetShapeAngle() {
    mHeadRotX += field_0x612 * mAnmSpeed;
    mSound.startCreatureSoundLevel(Z2SE_EN_PH_PROPELLER, field_0x612 * mAnmSpeed, -1);

    cXyz target_pos(mCurrentPntPos.x, mCurrentPntPos.y, mCurrentPntPos.z);
    field_0x61c = cLib_targetAngleY(&current.pos, &target_pos);

    s16 prev_angle = current.angle.y;
    cLib_chaseAngleS(&current.angle.y, field_0x61c,
                     l_HIO.mRotationInterpolation * (field_0x5b4 * 0x100));

    s16 temp_r4 = prev_angle - current.angle.y;
    if (field_0x5b4 != 0x46) {
        cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 0x20, 0x1000);
    }

    cLib_chaseAngleS(&shape_angle.x, -temp_r4 * (AREG_F(5) + 50.0f), 0x30);
}

void daE_PH_c::GoTarget() {
    cXyz target_pos(mCurrentPntPos.x, mCurrentPntPos.y, mCurrentPntPos.z);
    cLib_addCalc2(&speed.y, mTargetSpeedY, l_HIO.mRotationYInterpolation, 10.0f);
    cLib_addCalc2(&speedF, mTargetSpeedF, 0.1f, 20.0f);

    SetShapeAngle();

    int factor = current.pos.absXZ(target_pos) / speedF;
    if (factor == 0) {
        factor = 1;
    }

    mTargetSpeedY = (target_pos.y - current.pos.y) / (f32)factor;

    cLib_chaseF(&mAnmSpeed, field_0x620, 0.06f);
    cLib_chaseAngleS(&field_0x612, field_0x610 * mAnmSpeed, 0x10);
    mpMorf->setPlaySpeed(mAnmSpeed);

    if (mAcch.GetGroundH() != -G_CM3D_F_INF && current.pos.y < mAcch.GetGroundH() + 100.0f) {
        current.pos.y = mAcch.GetGroundH() + 100.0f;
    }
}

void daE_PH_c::SetCheckAngle() {
    cXyz sp34(mCurrentPntPos.x, mCurrentPntPos.y, mCurrentPntPos.z);
    mCurrentPntPos = dPath_GetPnt(mpPath, mCurrentPntNo)->m_position;

    cXyz sp40(mCurrentPntPos.x, mCurrentPntPos.y, mCurrentPntPos.z);
    cXyz sp4C(sp34.x - sp40.x, sp34.y - sp40.y, sp34.z - sp40.z);
    field_0x61e = cLib_targetAngleY(&sp34, &sp40);

    mTargetSpeedF =
        (l_HIO.mSpeed + (f32)mMoveSpeed) * (1.5f - fabsf(cM_ssin(cM_atan2s(sp4C.y, sp4C.absXZ()))));

    cXyz sp58(mCurrentPntPos.x, mCurrentPntPos.y, mCurrentPntPos.z);
    field_0x61c = cLib_targetAngleY(&current.pos, &sp58);

    field_0x620 = -cM_ssin(cM_atan2s(sp4C.y, sp4C.absXZ())) * 0.5f + 1.0f;
    if (field_0x620 < 0.8f) {
        field_0x620 = 0.8f;
    }

    field_0x5b8 = field_0x620;
    mStopTimer = l_HIO.mStopTime + (f32)mStopTimePrm;

    if (sp34.x == sp40.x && sp34.z == sp40.z) {
        mAction = 0;
    }
}

bool daE_PH_c::C_CheckPoint() {
    cXyz target_pos(mCurrentPntPos.x, mCurrentPntPos.y, mCurrentPntPos.z);
    s16 angle = field_0x61e - cLib_targetAngleY(&target_pos, &current.pos);

    if (angle < 0x4000 && angle > -0x4000) {
        return true;
    }

    return false;
}

void daE_PH_c::DownBoots() {
    s16 temp_r0 = (f32)(field_0x612 * mAnmSpeed);
    mHeadRotX += temp_r0;

    mSound.startCreatureSoundLevel(Z2SE_EN_PH_PROPELLER, (f32)field_0x612 * mAnmSpeed, -1);
    speed.y = 0.0f;

    daPy_py_c* player_p = daPy_getPlayerActorClass();
    speedF = 0.0f;
    current.pos.y += -10.0f;

    if (!player_p->checkEquipHeavyBoots() || mAnmID != ANM_HANG_WAIT) {
        mCAction++;
        mAnmSpeed = nREG_F(0) + 3.0f;
        field_0x620 = nREG_F(0) + 3.0f;
    }

    mpMorf->setPlaySpeed(mAnmSpeed);
}

void daE_PH_c::UpBoots() {
    daPy_py_c* player_p = daPy_getPlayerActorClass();
    mHeadRotX += 0x1000;

    mSound.startCreatureSoundLevel(Z2SE_EN_PH_PROPELLER, field_0x612 * mAnmSpeed, -1);

    mTargetSpeedY = 0.0f;
    current.pos.y += 10.0f;

    if (player_p->checkEquipHeavyBoots() && mAnmID == ANM_HANG_WAIT) {
        mCAction--;
        mAnmSpeed = nREG_F(0) + 3.0f;
        field_0x620 = nREG_F(0) + 3.0f;
    }

    mpMorf->setPlaySpeed(mAnmSpeed);

    if (current.pos.y > field_0x640) {
        mCAction = 0;
    }
}

void daE_PH_c::C_Action() {
    daPy_py_c* player_p = daPy_getPlayerActorClass();

    switch (mCAction) {
    case 0:
        if (player_p->checkEquipHeavyBoots() && mAnmID == ANM_HANG_WAIT) {
            mCAction++;
            field_0x640 = current.pos.y;
            mAnmSpeed = nREG_F(0) + 6.0f;
            field_0x620 = nREG_F(0) + 6.0f;
        }

        if (!player_p->checkEquipHeavyBoots() || mAnmID != ANM_HANG_WAIT) {
            field_0x620 = field_0x5b8;
        }

        if (C_CheckPoint()) {
            if (field_0x662) {
                SearchBackPos();
            } else {
                SearchNextPos();
            }

            SetCheckAngle();
        }

        GoTarget();
        break;
    case 1:
        DownBoots();
        if (!player_p->checkEquipHeavyBoots() || mAnmID != ANM_HANG_WAIT) {
            field_0x620 = field_0x5b8;
        }
        break;
    case 2:
        GoTarget();

        if (C_CheckPoint()) {
            if (field_0x662) {
                SearchBackPos();
            } else {
                SearchNextPos();
            }

            SetCheckAngle();
        }

        UpBoots();
        if (!player_p->checkEquipHeavyBoots() || mAnmID != ANM_HANG_WAIT) {
            field_0x620 = field_0x5b8;
        }
        break;
    }

    cLib_chaseF(&mAnmSpeed, field_0x620, 0.06f);
    FlyAnm();
}

void daE_PH_c::SearchNextPos() {
    cXyz target_pos(mCurrentPntPos.x, mCurrentPntPos.y, mCurrentPntPos.z);

    if (mCurrentPntNo + 1 >= mpPath->m_num) {
        if (dPath_ChkClose(mpPath)) {
            mCurrentPntNo = 0;
        } else {
            mCurrentPntNo--;
            field_0x662 = 1;
        }
    } else {
        mCurrentPntNo++;
    }
}

void daE_PH_c::SearchBackPos() {
    cXyz target_pos(mCurrentPntPos.x, mCurrentPntPos.y, mCurrentPntPos.z);

    if (mCurrentPntNo <= 0) {
        if (dPath_ChkClose(mpPath)) {
            mCurrentPntNo = mpPath->m_num;
        } else {
            mCurrentPntNo++;
            field_0x662 = 0;
        }
    } else {
        mCurrentPntNo--;
    }
}

void daE_PH_c::S_SetPlaySpeed() {
    mHeadRotX += field_0x612;

    mSound.startCreatureSoundLevel(Z2SE_EN_PH_PROPELLER, field_0x612, -1);

    cLib_chaseF(&mAnmSpeed, field_0x620, yREG_F(8) + 0.3f);
    cLib_chaseAngleS(&field_0x612, field_0x610 * field_0x620, 0x10);
    mpMorf->setPlaySpeed(mAnmSpeed);
}

void daE_PH_c::S_GoTarget() {
    cXyz unused(mCurrentPntPos.x, mCurrentPntPos.y, mCurrentPntPos.z);

    if (mAcch.GetGroundH() != -G_CM3D_F_INF && current.pos.y < mAcch.GetGroundH() + 100.0f) {
        current.pos.y = mAcch.GetGroundH() + 100.0f;
    }

    cXyz target_pos(mCurrentPntPos.x, mCurrentPntPos.y, mCurrentPntPos.z);
    cLib_addCalc2(&speedF, mTargetSpeedF, 0.01f, 20.0f);

    int factor = current.pos.absXZ(target_pos) / speedF;
    if (factor == 0) {
        factor = 1;
    }

    mTargetSpeedY = (target_pos.y - current.pos.y) / (f32)factor;

    if (field_0x654.x == target_pos.x && field_0x654.z == target_pos.z) {
        int factor = (current.pos.abs(target_pos) / speedF);
        f32 unused = factor;
        mTargetSpeedY = (target_pos.y - current.pos.y) / factor;
    }

    f32 prev_y = current.pos.y;
    cLib_chasePos(&current.pos, target_pos, speedF);
    current.pos.y = prev_y;

    cLib_addCalc2(&speed.y, mTargetSpeedY, 0.05f, 10.0f);
    cLib_chaseAngleS(&field_0x612, field_0x610 * mAnmSpeed, 0x10);
    S_SetPlaySpeed();
}

bool daE_PH_c::S_CheckPoint() {
    cXyz target_pos(mCurrentPntPos.x, mCurrentPntPos.y, mCurrentPntPos.z);

    if (current.pos.abs(target_pos) < 200.0f && mStopTimer == 0) {
        if (field_0x662) {
            SearchBackPos();
        } else {
            SearchNextPos();
        }

        return true;
    }

    return false;
}

void daE_PH_c::S_SetAngle() {
    cXyz sp30(mCurrentPntPos.x, mCurrentPntPos.y, mCurrentPntPos.z);

    field_0x654 = mCurrentPntPos;
    mCurrentPntPos = dPath_GetPnt(mpPath, mCurrentPntNo)->m_position;

    cXyz sp3C(mCurrentPntPos.x, mCurrentPntPos.y, mCurrentPntPos.z);
    cXyz sp48(sp3C.x - sp30.x, sp3C.y - sp30.y, sp3C.z - sp30.z);

    field_0x61e = cM_atan2s(sp48.x, sp48.z);

    mTargetSpeedF =
        (l_HIO.mSpeed + (f32)mMoveSpeed) * (1.5f - fabsf(cM_ssin(cM_atan2s(sp48.y, sp48.absXZ()))));

    if (sp30.x == sp3C.x && sp30.z == sp3C.z) {
        mTargetSpeedF = l_HIO.mSpeed + (f32)mMoveSpeed;
    }

    field_0x620 = cM_ssin(cM_atan2s(sp48.y, sp48.absXZ())) * 1.5f + 1.0f;
    if (field_0x620 < 0.8f) {
        field_0x620 = 0.8f;
    }

    field_0x63c = sp30.abs(sp3C);
    mStopTimer = l_HIO.mStopTime + (f32)mStopTimePrm;
}

void daE_PH_c::FlyAnm2() {
    if (mAnmID == ANM_DAMAGE_ARROW || mAnmID == ANM_HANG_START || mAnmID == ANM_HANG_WAIT) {
        if (mpMorf->isStop()) {
            SetAnm(ANM_WAIT, J3DFrameCtrl::EMode_LOOP, 5.0f, mAnmSpeed);
        }

        mFlyRockEMKey = dComIfGp_particle_set(mFlyRockEMKey, 0x878C, &current.pos, &tevStr,
                                              &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
        mFlyTsubuEMKey =
            dComIfGp_particle_set(mFlyTsubuEMKey, 0x878D, &current.pos, &tevStr, &shape_angle,
                                  NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
    }

    if (mAnmID == ANM_WAIT) {
        if (fopAcM_searchPlayerDistance(this) < 1000.0f &&
            dComIfGp_checkPlayerStatus1(0, 0x10000))
        {
            attention_info.flags = 0;
            field_0x630 = -10.0f - BREG_F(0);
            field_0x5ae = 15;
            field_0x616 = 0x1000;
            field_0x618 = 0x1000;
            SetAnm(ANM_HANG_WAIT, J3DFrameCtrl::EMode_LOOP, 5.0f, mAnmSpeed);
        } else if (fopAcM_checkHookCarryNow(this)) {
            attention_info.flags = 0;
        } else if (mAnmID != ANM_WAIT) {
            SetAnm(ANM_WAIT, J3DFrameCtrl::EMode_LOOP, 5.0f, mAnmSpeed);
        }
    } else if (mAnmID == ANM_HANG_WAIT) {
        attention_info.flags = 0;

        if (!fopAcM_checkHookCarryNow(this) || !dComIfGp_checkPlayerStatus1(0, 0x10000)) {
            SetAnm(ANM_HANG_END, J3DFrameCtrl::EMode_NONE, 5.0f, mAnmSpeed);
        }
    } else if (mAnmID == ANM_HANG_END) {
        if (mpMorf->isStop()) {
            SetAnm(ANM_WAIT, J3DFrameCtrl::EMode_LOOP, 5.0f, mAnmSpeed);
        }
    }
}

void daE_PH_c::S_Action() {
    if (S_CheckPoint()) {
        S_SetAngle();
    }

    FlyAnm();

    if (mStopTimer > 0) {
        speed.y = 0.0f;
        speedF = 0.0f;
    } else {
        S_GoTarget();
    }
}

void daE_PH_c::StopAction() {
    daPy_py_c* player_p = daPy_getPlayerActorClass();

    switch (mCAction) {
    case 0:
        if (mAnmID == ANM_HANG_WAIT || mAnmID == ANM_HANG_START) {
            mHeadRotX += (s16)(field_0x612 + 0x1000);
        } else {
            mHeadRotX += (s16)(field_0x612 + 0x500);
        }

        mSound.startCreatureSoundLevel(Z2SE_EN_PH_PROPELLER, field_0x612 + 0x500, -1);

        if (field_0x5ae == 0) {
            cLib_addCalc2(&current.pos.y, home.pos.y, 0.1f, nREG_F(1) + 30.0f);
        }

        if (player_p->checkEquipHeavyBoots() && mAnmID == ANM_HANG_WAIT) {
            mCAction++;
            field_0x640 = current.pos.y;
            mAnmSpeed = nREG_F(0) + 6.0f;
            field_0x620 = nREG_F(0) + 6.0f;
        }
        break;
    case 1:
        DownBoots();
        if (player_p->checkEquipHeavyBoots() && mAnmID == ANM_HANG_WAIT) {
            mAnmSpeed = nREG_F(0) + 6.0f;
            field_0x620 = nREG_F(0) + 6.0f;
        }
        break;
    case 2:
        UpBoots();

        if (player_p->checkEquipHeavyBoots() && mAnmID == ANM_HANG_WAIT) {
            mAnmSpeed = nREG_F(0) + 6.0f;
            field_0x620 = nREG_F(0) + 6.0f;
        }
        break;
    }
}

void daE_PH_c::Action() {
    daPy_py_c* player_p = daPy_getPlayerActorClass();

    switch (mAction) {
    case 1:
        C_Action();
        fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
        AttentionSet();

        if (fopAcM_searchPlayerDistance(this) > 2000.0f || other_bg_check(this, player_p)) {
            attention_info.flags = 0;
        }
        break;
    case 2:
        if (field_0x5b2) {
            StopAction();

            if (fopAcM_searchPlayerDistance(this) > 3000.0f || other_bg_check(this, player_p)) {
                attention_info.flags = 0;
            }

            cXyz* cc_move_p = mCcStts.GetCCMoveP();
            if (cc_move_p != NULL) {
                current.pos.x += cc_move_p->x;
                current.pos.y += cc_move_p->y;
                current.pos.z += cc_move_p->z;
            }

            FlyAnm2();
            AttentionSet();

            if (strcmp(dComIfGp_getStartStageName(), "D_MN07A") == 0) {
                if (mAnmID == ANM_HANG_START) {
                    dComIfGs_onZoneSwitch(0x18, fopAcM_GetRoomNo(this));
                }

                if (!dComIfGs_isSwitch(0x3F, fopAcM_GetRoomNo(this))) {
                    fopAcM_delete(this);
                }
            }
        } else {
            attention_info.flags = 0;

            if (dComIfGs_isSwitch(0x3F, fopAcM_GetRoomNo(this))) {
                field_0x5b2 = 1;
            }
        }

        mCcSph.OnCoSetBit();
        break;
    default:
        if (fopAcM_searchPlayerDistance(this) > 2000.0f || other_bg_check(this, player_p)) {
            attention_info.flags = 0;
        }

        S_Action();

        cXyz* cc_move_p = mCcStts.GetCCMoveP();
        if (cc_move_p != NULL) {
            current.pos.x += cc_move_p->x;
            current.pos.y += cc_move_p->y;
            current.pos.z += cc_move_p->z;
        }

        current.pos.y += speed.y;
        AttentionSet();
        break;
    }

    ObjHit();
    BaseSet();

    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mAcch.CrrPos(dComIfG_Bgsp());
}

void daE_PH_c::SetHeadAngle(s16 i_targetAngle) {
    cLib_addCalcAngleS2(&field_0x612, i_targetAngle, 0x10, 0x1000);
    mHeadRotX += field_0x612;

    mSound.startCreatureSoundLevel(Z2SE_EN_PH_PROPELLER, field_0x612, -1);
}

void daE_PH_c::CamAction() {
    cXyz sp20(0.0f, 0.0f, 0.0f);

    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(2));
    mDoMtx_stack_c::multVec(&sp20, &sp20);
    sp20.y += yREG_F(3);

    cXyz sp2C;
    cXyz sp38;

    switch (mCamAction) {
    case 0:
        sp2C.set(-2638.0f, 0.0f, 2566.0f);

        if (dComIfGs_isSwitch(0x10, fopAcM_GetRoomNo(this))) {
            SetStopCam(sp20, 500.0f, 100.0f, cLib_targetAngleY(&current.pos, &sp2C));
            mCamAction++;
        }
        break;
    case 1:
        SetCMoveCam(sp20, 0.1f, 40.0f);
        SetStopingCam();
        break;
    case 2:
        sp2C.set(0.0f, 0.0f, 0.0f);
        SetStopCam(sp20, 2000.0f, 1000.0f, cLib_targetAngleY(&sp2C, &current.pos));
        mCamAction++;
        break;
    case 3:
        SetCMoveCam(sp20, 0.1f, 40.0f);
        SetStopingCam();

        if (mTimers[0] == 0) {
            mCamAction++;
        }
        break;
    case 4:
        mCamEye.set(0.0f, 200.0f, -500.0f);
        mCamCenter.set(0.0f, 100.0f, 0.0f);
        SetReleaseCam();

        Z2GetAudioMgr()->bgmStart(Z2BGM_DRAGON_BTL02, 0, 0);
        fopAcM_onSwitch(this, 0x3F);
        break;
    }
}

void daE_PH_c::DemoAction() {
    switch (mCAction) {
    case 0:
        if (dComIfGs_isSwitch(0x10, fopAcM_GetRoomNo(this))) {
            mCAction++;
            mAnmID = 100;
            mTimers[0] = 50;

            if (mAction == 4) {
                mTimers[0] = cM_rndF(100.0f) + 150.0f;
            }
        }

        speed.y = 0.0f;
        break;
    case 1:
        if (mTimers[0] == 0 && mAnmID != ANM_APPEAR) {
            mpMorf->setPlaySpeed(1.0f);
            mAnmID = ANM_APPEAR;
        }

        if (mpMorf->checkFrame(154) && mAnmID == ANM_APPEAR) {
            speed.y = yREG_F(0) + 70.0f;
            mSound.startCreatureSound(Z2SE_EN_PH_APPEAR, 0, -1);
            mCAction++;
        }

        if (mpMorf->getFrame() > 126.0f && mAnmID == ANM_APPEAR) {
            SetHeadAngle(0x2000);
        }
        break;
    case 2:
        speed.y += yREG_F(1) + -1.8f;

        if (mpMorf->isStop()) {
            mCAction++;
            mTimers[0] = 50;
            SetAnm(ANM_WAIT, J3DFrameCtrl::EMode_LOOP, 5.0f, 1.0f);
        }

        if (speed.y <= 0.0f) {
            SetHeadAngle(0x1000);
            cLib_addCalcAngleS2(&field_0x612, 0x1000, 0x10, 0x1000);
            cLib_addCalcAngleS2(&field_0x612, 0x1000, 0x10, 0x1000);
        } else {
            SetHeadAngle(0x2000);
        }
        break;
    case 3:
        cLib_addCalc2(&speed.y, 15.0f, yREG_F(6) + 0.3f, yREG_F(7) + 3.5f);

        if (mAction == 5) {
            if (speed.y >= 0.0f) {
                speed.y = 0.0f;
                SetHeadAngle(0x1000);
                cLib_addCalcAngleS2(&field_0x612, 0x1000, 0x10, 0x1000);
            } else {
                SetHeadAngle(0x1000);
                cLib_addCalcAngleS2(&field_0x612, 0x1000, 0x10, 0x1000);
            }
        } else {
            SetHeadAngle(0x1500);
        }

        if (mTimers[0] == 0) {
            mCAction++;
            mCamAction++;
            mTimers[0] = 300;
            mTimers[1] = 360;
        }

        mpMorf->setPlaySpeed(speed.y / 20.0f + 1.0f);
        break;
    case 4:
        cLib_addCalc2(&speed.y, 15.0f, 0.1f, 0.5f);

        if (mAction == 5) {
            if (mTimers[0] > 220) {
                speed.y = 0.0f;
                SetHeadAngle(0x1000);
                cLib_addCalcAngleS2(&field_0x612, 0x1000, 0x10, 0x1000);
            } else {
                SetHeadAngle(0x1500);
            }
        } else {
            SetHeadAngle(0x1500);
        }

        mpMorf->setPlaySpeed(speed.y / 20.0f + 1.0f);

        if (mTimers[1] == 0 || !dComIfGp_event_runCheck()) {
            fopAcM_delete(this);
        }
        break;
    }

    cXyz* cc_move_p = mCcStts.GetCCMoveP();
    if (cc_move_p != NULL) {
        current.pos.x += cc_move_p->x;
        current.pos.y += cc_move_p->y;
        current.pos.z += cc_move_p->z;
    }
    current.pos.y += speed.y;

    if (mAnmID == ANM_APPEAR) {
        mDemoRockEMKey =
            dComIfGp_particle_set(mDemoRockEMKey, 0x8C6F, &current.pos, &tevStr, &shape_angle,
                                  NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
        mDemoTsubuEMKey =
            dComIfGp_particle_set(mDemoTsubuEMKey, 0x8C70, &current.pos, &tevStr, &shape_angle,
                                  NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
        mDemoRock2EMKey =
            dComIfGp_particle_set(mDemoRock2EMKey, 0x8C71, &current.pos, &tevStr, &shape_angle,
                                  NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
        mDemoRock3EMKey =
            dComIfGp_particle_set(mDemoRock3EMKey, 0x8C72, &current.pos, &tevStr, &shape_angle,
                                  NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
    }
}

void daE_PH_c::ToumeiAction() {
    cXyz hs_offset;

    if (fopAcM_searchPlayerDistance(this) > XREG_F(1) + 2300.0f ||
        dComIfGp_checkPlayerStatus1(0, 0x10000))
    {
        attention_info.flags = 0;
    }

    if (mCcSph.ChkTgHit() && field_0x5ae == 0) {
        mCcSph.ClrTgHit();
        field_0x5ae = 20;
        mStopTimer = 300;

        dComIfGp_particle_set(0x878C, &current.pos, &tevStr, &shape_angle, NULL);
        dComIfGp_particle_set(0x878D, &current.pos, &tevStr, &shape_angle, NULL);
        dComIfGp_particle_set(0x878E, &current.pos, &tevStr, &shape_angle, NULL);

        mSound.startCreatureSound(Z2SE_EN_PH_HIT, 0, -1);
    }

    field_0x5ae--;
    if (field_0x5ae <= 0) {
        field_0x5ae = 0;
    } else {
        hs_offset.set(0.0f, l_HIO.mHangPos - 80.0f, 0.0f);
        daPy_getPlayerActorClass()->setHookshotCarryOffset(fopAcM_GetID(this), &hs_offset);
    }

    mStopTimer--;
    if (mStopTimer <= 0) {
        mStopTimer = 0;
    }

    mCcSph.SetC(current.pos);
    mCcSph.SetR(AREG_F(1) + 80.0f);
    dComIfG_Ccsp()->Set(&mCcSph);
}

int daE_PH_c::Execute() {
    daPy_getPlayerActorClass();
    cXyz sp28;

    EyeSet();

    switch (mAction) {
    case 3:
        ToumeiAction();
        break;
    case 4:
    case 5:
        for (int i = 0; i < 3; i++) {
            mTimers[i]--;
            if (mTimers[i] <= 0) {
                mTimers[i] = 0;
            }
        }

        De_Timer();
        DemoAction();

        if (mAction == 5) {
            CamAction();
        }

        mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        break;
    default:
        De_Timer();
        Action();
        break;
    }

    setBaseMtx();

    if (mAction != 3) {
        setCcSph();
    }

    return 1;
}

void daE_PH_c::AttentionSet() {
    daPy_py_c* player_p = daPy_getPlayerActorClass();
    cXyz sp38;

    if (mAnmID != ANM_HANG_START && mAnmID != ANM_HANG_WAIT && mAnmID != ANM_HANG_END) {
        if (strcmp(dComIfGp_getStartStageName(), "D_MN07A") == 0) {
            attention_info.distances[fopAc_attn_BATTLE_e] = 0x52;
        } else {
            attention_info.distances[fopAc_attn_BATTLE_e] = 0x53;
        }

        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else if (current.pos.absXZ(fopAcM_GetPosition(player_p)) > 1000.0f) {
        if (strcmp(dComIfGp_getStartStageName(), "D_MN07A") == 0) {
            attention_info.distances[fopAc_attn_BATTLE_e] = 0x52;
        } else {
            attention_info.distances[fopAc_attn_BATTLE_e] = 0x53;
        }
    } else {
        attention_info.distances[fopAc_attn_BATTLE_e] = 0;
        attention_info.flags = 0;
    }

    if (player_p->checkDragonHangRide()) {
        attention_info.distances[fopAc_attn_BATTLE_e] = 0;
        attention_info.flags = 0;
    }
}

void daE_PH_c::ObjHit() {
    f32 y = l_HIO.mHangPos;
    y -= 80.0f;

    cM_ssin(shape_angle.x);

    cXyz hs_offset(0.0f, y + KREG_F(8), 0.0f);

    if (dComIfGp_checkPlayerStatus0(0, 0x4000)) {
        mCcSph.OffTgShield();
    } else {
        mCcSph.OnTgShield();
    }

    if (field_0x5ae > 0) {
        daPy_getPlayerActorClass()->setHookshotCarryOffset(fopAcM_GetID(this), &hs_offset);
        cLib_chaseAngleS(&shape_angle.x, 0, 0x100);
        cLib_chaseF(&field_0x630, BREG_F(0) + 0.0f, 1.0f);
        cLib_chaseF(&mAnmSpeed, yREG_F(10) + 2.0f, yREG_F(11) + 0.5f);

        current.pos.y += field_0x630;
        mCcSph.ClrTgHit();
    } else {
        mCcSph.OnTgSetBit();

        if (mCcSph.ChkTgHit()) {
            mAtInfo.mpCollider = mCcSph.GetTgHitObj();
            cCcD_Obj* hit_obj = mCcSph.GetTgHitObj();

            field_0x630 = -15.0f - BREG_F(0);
            field_0x5ae = 15;
            field_0x616 = 0x1000;
            field_0x618 = 0x1000;

            if (hit_obj->ChkAtType(AT_TYPE_HOOKSHOT)) {
                field_0x616 = 0x1000;
                field_0x618 = 0x1000;
                SetAnm(ANM_HANG_START, J3DFrameCtrl::EMode_NONE, 1.0f, 1.0f);
            } else {
                SetAnm(ANM_DAMAGE_ARROW, J3DFrameCtrl::EMode_NONE, 1.0f, 1.0f);
            }

            dComIfGp_particle_set(0x878E, &current.pos, &tevStr, &shape_angle, NULL);
            mSound.startCreatureSound(Z2SE_EN_PH_HIT, 0, -1);
            mCcSph.OffTgSetBit();
        }
    }
}

void daE_PH_c::BaseSet() {
    cLib_chaseAngleS(&field_0x616, 0, 0x40);
    field_0x61a += field_0x618;

    shape_angle.x = field_0x616 * cM_ssin(field_0x61a);
    mHeadRotY = field_0x616 * cM_ssin(field_0x61a);
}

void daE_PH_c::De_Timer() {
    field_0x5ae--;
    if (field_0x5ae <= 0) {
        field_0x5ae = 0;
    }

    mStopTimer--;
    if (mStopTimer < 0) {
        mStopTimer = 0;
    }
}

void daE_PH_c::EyeSet() {
    eyePos = current.pos;
    eyePos.y += TREG_F(2);

    attention_info.position.x = current.pos.x;
    attention_info.position.z = current.pos.z;
    attention_info.position.y = current.pos.y + 170.0f + TREG_F(1);
}

int daE_PH_c::Delete() {
    dComIfG_resDelete(&mPhase, "E_PH");
    if (mInitializedHIO) {
        initialized = 0;
    }

    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }

    return 1;
}

void daE_PH_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::XrotM(shape_angle.x);
    mDoMtx_stack_c::ZrotM(shape_angle.z);

    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
}

int daE_PH_c::Draw() {
    J3DModel* model = mpMorf->getModel();

    if (mAction != 3) {
        if (field_0x5b2) {
            cXyz sp28;
            sp28.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);

            g_env_light.settingTevStruct(0, &current.pos, &tevStr);
            g_env_light.setLightTevColorType_MAJI(model, &tevStr);
            mpMorf->entryDL();

            mShadowKey = dComIfGd_setShadow(mShadowKey, 0, model, &sp28, BREG_F(16) + 1000.0f,
                                            BREG_F(17) + 100.0f, current.pos.y, mAcch.GetGroundH(),
                                            mAcch.m_gnd, &tevStr, 0, 1.0f,
                                            dDlst_shadowControl_c::getSimpleTex());
        }
    } else {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    }

    return 1;
}

static int daE_PH_Draw(daE_PH_c* i_this) {
    return i_this->Draw();
}

static int daE_PH_Execute(daE_PH_c* i_this) {
    return i_this->Execute();
}

int daE_PH_c::create() {
    fopAcM_ct(this, daE_PH_c);

    int phase_state = dComIfG_resLoad(&mPhase, "E_PH");
    if (phase_state == cPhs_COMPLEATE_e) {
        mAction = fopAcM_GetParam(this) & 0xF;

        if (dComIfGs_isZoneSwitch(2, fopAcM_GetRoomNo(this)) && mAction == 4) {
            return cPhs_ERROR_e;
        }

        if (dComIfGs_isZoneSwitch(2, fopAcM_GetRoomNo(this)) && mAction == 5) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("E_PH PARAM %x\n", fopAcM_GetParam(this));

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x11E0)) {
            return cPhs_ERROR_e;
        }

        gravity = 0.0f;

        field_0x5b4 = (fopAcM_GetParam(this) >> 4) & 0xF;
        if (field_0x5b4 == 0 || field_0x5b4 == 15) {
            field_0x5b4 = 70;
        }

        if (mAction != 3 && mAction != 2 && mAction != 4) {
            mPathNo = fopAcM_GetParam(this) >> 8;
            mpPath = dPath_GetRoomPath(mPathNo, fopAcM_GetRoomNo(this));
        }

        mMoveSpeed = fopAcM_GetParam(this) >> 0x10;
        mStopTimePrm = fopAcM_GetParam(this) >> 0x18;

        if (mMoveSpeed == 0xFF) {
            mMoveSpeed = 0;
        }

        if (mStopTimePrm == 0xFF) {
            mStopTimePrm = 0;
        }

        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        mAcchCir.SetWall(100.0f, 100.0f);

        if (!initialized) {
            initialized = 1;
            mInitializedHIO = 1;
            l_HIO.field_0x4 = -1;
        }

        fopAcM_setCullSizeFar(this, 3.0f);
        fopAcM_setCullSizeBox(this, -500.0f, -500.0f, -500.0f, 500.0f, 500.0f, 500.0f);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);

        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        attention_info.distances[fopAc_attn_BATTLE_e] = 0x22;

        cXyz sp3C(current.pos.x, current.pos.y + 200.0f, current.pos.z);
        mSound.init(&current.pos, &sp3C, 3, 1);
        mAtInfo.mpSound = &mSound;

        J3DModel* model = mpMorf->getModel();
        model->setUserArea((uintptr_t)this);

        for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
            if (i != 0) {
                model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
            }
        }

        field_0x610 = 0x1000;
        field_0x612 = 0x500;
        mAnmSpeed = 1.0f;

        initCcSph();

        mSound.setEnemyName("E_PH");

        speedF = 10.0f;
        field_0x5b2 = 1;

        if (mAction == 3) {
            mCcSph.OffTgShield();
            mCcSph.OnTgNoHitMark();

            attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            attention_info.flags = 0;

            cXyz sp48(current.pos.x, current.pos.y, current.pos.z);
            attention_info.position.x = current.pos.x;
            attention_info.position.z = current.pos.z;
            attention_info.position.y = current.pos.y + 150.0f + TREG_F(1);
        } else if (mAction == 2 || mAction == 4 || mAction == 5) {
            speedF = 0.0f;
            attention_info.flags = 0;
        } else {
            SearchNearP();
        }

        if (mAction == 5) {
            dCamera_c* camera_p = dCam_getBody();
            mCamFovY = camera_p->Fovy();

            mCamCenter = camera_p->Center();
            mCamCenterTarget = mCamCenter;

            mCamEye = camera_p->Eye();
            mCamEyeTarget = mCamEye;
        }

        if (mAction == 4 || mAction == 5) {
            mpMorf->setPlaySpeed(0.0f);
            SetAnm(ANM_APPEAR, J3DFrameCtrl::EMode_NONE, 5.0f, 0.0f);
            mAnmID = ANM_WAIT;
        }

        if (strcmp(dComIfGp_getStartStageName(), "D_MN07A") == 0 && mAction == 2) {
            if (!dComIfGs_isZoneSwitch(2, fopAcM_GetRoomNo(this))) {
                field_0x5b2 = 0;
                fopAcM_offSwitch(this, 0x10);
                fopAcM_offSwitch(this, 0x3F);
            }
        }

        l_HIO.mSpeed = mMoveSpeed;
        mCamAction = 0;
        fopAcM_OnStatus(this, 0x4000);
        daE_PH_Execute(this);
    }

    return phase_state;
}

static int daE_PH_IsDelete(daE_PH_c* i_this) {
    return 1;
}

static actor_method_class l_daE_PH_Method = {
    (process_method_func)daE_PH_Create,  (process_method_func)daE_PH_Delete,
    (process_method_func)daE_PH_Execute, (process_method_func)daE_PH_IsDelete,
    (process_method_func)daE_PH_Draw,
};

actor_process_profile_definition g_profile_E_PH = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_E_PH,
    &g_fpcLf_Method.base,
    sizeof(daE_PH_c),
    0,
    0,
    &g_fopAc_Method.base,
    131,
    &l_daE_PH_Method,
    0x244100,
    fopAc_ENEMY_e,
    fopAc_CULLBOX_CUSTOM_e,
};
