/**
 * @file d_a_spinner.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_spinner.h"
#include "d/actor/d_a_tag_sppath.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_mirror.h"
#include "Z2AudioLib/Z2Instances.h"
#include "SSystem/SComponent/c_math.h"
#include "m_Do/m_Do_controller_pad.h"

static u8 const lit_3768[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

int daSpinner_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(daAlink_c::getAlinkArcName(), 0x21);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    mpSpoutBck = (J3DAnmTransform*)dComIfG_getObjectRes(daAlink_c::getAlinkArcName(), 0x16);
    if (!mBck.init(mpSpoutBck, 1, 2, 1.0f, 0, -1, false)) {
        return 0;
    }

    return 1;
}

static int daSpinner_createHeap(fopAc_ac_c* i_this) {
    return ((daSpinner_c*)i_this)->createHeap();
}

static dCcD_SrcCyl l_cylSrc = {
    {
        {0, {{AT_TYPE_SPINNER, 1, 0x12}, {0, 0}, 0x69}},
        {dCcD_SE_SPINNER, 1, 0, 0, {0}},
        {dCcD_SE_NONE, 0, 0, 0, {0}},
        {0},
    },
    {
        {0.0f, 0.0f, 0.0f},
        40.0f,
        85.0f,
    }
};

int daSpinner_c::create() {
    fopAcM_ct(this, daSpinner_c);

    if (!fopAcM_entrySolidHeap(this, daSpinner_createHeap, 0x900)) {
        return cPhs_ERROR_e;
    }

    daAlink_c* player = daAlink_getAlinkActorClass();
    
    mAcch.Set(this, 3, mAcchCir);
    mAcch.SetWaterCheckOffset(10000.0f);
    mAcch.OnLineCheck();
    mAcch.SetWtrChkMode(2);

    mAcchCir[0].SetWall(50.01f, 58.0f);
    mAcchCir[1].SetWall(129.99f, 35.0f);
    mAcchCir[2].SetWall(player->field_0x598, 35.0f);

    mPaPo.init(&mAcch, 85.0f, 85.0f);

    mSound.init(&current.pos, 1);

    mStts.Init(200, 0, this);
    mCyl.Set(l_cylSrc);
    mCyl.SetStts(&mStts);
    mCyl.StartCAt(current.pos);

    this->model = mpModel;

    gravity = player->getSpinnerGravity();
    maxFallSpeed = player->getSpinnerMaxFallSpeed();
    field_0xa76 = 1;

    mAcch.CrrPos(dComIfG_Bgsp());

    fopAcM_setStageLayer(this);

    mBck.setFrame(mBck.getEndFrame());
    mBck.setPlaySpeed(0.0f);

    setMatrix();
    setRoomInfo();
    field_0xa82 = 0x82F;

    return cPhs_COMPLEATE_e;
}

static int daSpinner_Create(fopAc_ac_c* i_this) {
    return ((daSpinner_c*)i_this)->create();
}

daSpinner_c::~daSpinner_c() {
    mSound.deleteObject();
    clearSpreadEffect();
    clearPathMove();
}

static int daSpinner_Delete(daSpinner_c* i_this) {
    i_this->~daSpinner_c();
    return 1;
}

void daSpinner_c::setRoomInfo() {
    int roomNo;
    if (mAcch.GetGroundH() != -G_CM3D_F_INF) {
        roomNo = dComIfG_Bgsp().GetRoomId(mAcch.m_gnd);
        tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mAcch.m_gnd);
    } else {
        roomNo = dComIfGp_roomControl_getStayNo();
    }

    tevStr.room_no = roomNo;
    mReverb = dComIfGp_getReverb(roomNo);
    mStts.SetRoomId(roomNo);
    fopAcM_SetRoomNo(this, roomNo);
}

void daSpinner_c::setMatrix() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + field_0xa84 + 90.0f, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::YrotM(mRotY);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mBck.entry(mpModel->getModelData());
    mpModel->calc();
}

void daSpinner_c::setEffect() {
    static JGeometry::TVec3<f32> effDirection(0.0f, 1.0f, 0.0f);
    static cXyz particleScale(1.5f, 1.5f, 1.5f);

    cXyz effCenter(current.pos);
    cXyz* sp24 = NULL;

    if (mAcch.ChkWaterHit() && dComIfG_Bgsp().GetPolyAtt0(mAcch.m_wtr) != 6) {
        if (mAcch.m_wtr.GetHeight() > mAcch.GetGroundH() && mAcch.m_wtr.GetHeight() > effCenter.y) {
            effCenter.y = mAcch.m_wtr.GetHeight();
            sp24 = &effCenter;
        }
    }

    if (sp24 == NULL && mAcch.ChkGroundHit()) {
        effCenter.y = mAcch.GetGroundH();

        int sp20 = dComIfG_Bgsp().GetGroundCode(mAcch.m_gnd);
        if (sp20 != 4 && sp20 != 10) {
            sp24 = &effCenter;
        }
    }

    mPaPo.setEffectCenter(&tevStr, &effCenter, 4, 0x60000, sp24, &current.angle, NULL, fopAcM_GetRoomNo(this), 0.0f, speedF);

    for (int i = 0; i < 2; i++) {
        u8 eff_type = mPaPo.getCenterEffType(i);
        for (int j = 0; j < 4; j++) {
            JPABaseEmitter* emitter = mPaPo.getCenterEmitter(i, j);
            
            if (emitter != NULL) {
                emitter->setDirection(effDirection);

                if (eff_type == 0) {
                    emitter->setDirectionalSpeed(1.0f);
                    emitter->setGlobalParticleScale(particleScale);
                } else if (eff_type == 1) {
                    emitter->setAwayFromCenterSpeed(3.0f);
                    emitter->setAwayFromAxisSpeed(3.0f);
                    emitter->setDirectionalSpeed(3.0f);
                } else if (eff_type == 3) {
                    emitter->setRate(1.5f);
                    emitter->setAwayFromAxisSpeed(1.0f);
                    emitter->setGlobalParticleScale(particleScale);
                } else if (eff_type == 4) {
                    emitter->setRate(1.5f);
                    emitter->setAwayFromCenterSpeed(2.0f);
                    emitter->setGlobalParticleScale(particleScale);
                }
            }
        }
    }
}

int daSpinner_c::posMove() {
    daAlink_c* player = daAlink_getAlinkActorClass();
    
    f32 move_speed;
    if (!player->checkDemoSpinnerKeep()) {
        if (player->checkDemoSpinnerKeepSpeed()) {
            speedF = player->getSpinnerRideSpeedF();
        }
        move_speed = speedF * cM_scos(current.angle.x);
    } else {
        move_speed = 0.0f;
    }

    if (mpPathMove == NULL) {
        speed.y += gravity;
        if (speed.y < maxFallSpeed) {
            speed.y = maxFallSpeed;
        }
    }

    current.pos.x += move_speed * cM_ssin(current.angle.y);
    current.pos.y += speed.y - (speedF * cM_ssin(current.angle.x));
    current.pos.z += move_speed * cM_scos(current.angle.y);

    if (mpPathMove != NULL) {
        f32 var_f31 = speedF;
        cM3dGLin sp60;
        cXyz sp54;

        while (var_f31 > 0.0f) {
            Vec* curr_path_pos = &mpPathMove->m_points[mPathNo].m_position;
            Vec* next_path_pos = getPathNextPos();

            cXyz sp48(current.pos.x, current.pos.y - -20.0f, current.pos.z);
            cXyz sp3C(curr_path_pos->x - next_path_pos->x, curr_path_pos->y - next_path_pos->y, curr_path_pos->z - next_path_pos->z);

            int spC;
            s16 spA = sp3C.atan2sX_Z();
            sp3C.normalizeZP();
            sp3C *= 58.0f;

            sp3C.x += curr_path_pos->x;
            sp3C.y += curr_path_pos->y;
            sp3C.z += curr_path_pos->z;

            sp60.set(sp3C, *next_path_pos);

            f32 sp10;
            if (!cM3d_Len3dSqPntAndSegLine(&sp60, &sp48, &sp54, &sp10)) {
                if (setNextPathNum() && (mPathNo == 0 || mPathNo == mpPathMove->m_num - 1) && !dPath_ChkClose(mpPathMove)) {
                    var_f31 = 0.0f;
                    if (mpPathMove->m_points[mPathNo].mArg0 == 0) {
                        u8 sp8 = mPathNo;
                        setReflectAngle();
                        mPathNo = sp8;

                        if ((s16)(cM_atan2s(current.pos.x - next_path_pos->x, current.pos.z - next_path_pos->z) - spA) > 0) {
                            spA += 0x4000;
                        } else {
                            spA -= 0x4000;
                        }

                        field_0xab0.set(next_path_pos->x, next_path_pos->y, next_path_pos->z);
                        current.pos.set(next_path_pos->x + (52.199997f * cM_ssin(spA)), -20.0f + next_path_pos->y, next_path_pos->z + (52.199997f * cM_scos(spA)));
                    } else {
                        clearPathMove();
                        field_0xa74 = 1;
                    }
                } else {
                    var_f31 -= field_0xab0.abs(*next_path_pos);
                    field_0xab0.set(next_path_pos->x, next_path_pos->y, next_path_pos->z);

                    sp3C = sp48 - field_0xab0;
                    if ((s16)(sp3C.atan2sX_Z() - current.angle.y) > 0) {
                        spC = 1;
                    } else {
                        spC = -1;
                    }
                }

                if (mpPathMove != NULL) {
                    curr_path_pos = &mpPathMove->m_points[mPathNo].m_position;
                    next_path_pos = getPathNextPos();
                    sp3C.set(next_path_pos->x - curr_path_pos->x, next_path_pos->y - curr_path_pos->y, next_path_pos->z - curr_path_pos->z);

                    spA = current.angle.y;
                    current.angle.y = sp3C.atan2sX_Z();
                    current.angle.x = sp3C.atan2sY_XZ();

                    if (var_f31 > 0.0f) {
                        spA = current.angle.y - spA;
                        var_f31 += 0.0055606803f * spA * spC;

                        if (var_f31 > 0.0f) {
                            sp3C.normalizeZP();
                            spA = current.angle.y + (spC << 0xE);
                            current.pos.set(curr_path_pos->x + (sp3C.x * var_f31), -20.0f + (curr_path_pos->y + (sp3C.y * var_f31)), curr_path_pos->z + (sp3C.z * var_f31));
                        } else {
                            spA = (spC << 0xE) + (current.angle.y + (spC * (179.8341f * fabsf(var_f31))));
                            current.pos.set(curr_path_pos->x, -20.0f + curr_path_pos->y, curr_path_pos->z);
                        }

                        current.pos.x += 29.0f * cM_ssin(spA);
                        current.pos.z += 29.0f * cM_scos(spA);
                        old.pos = current.pos;
                    }
                }
            } else {
                var_f31 = 0.0f;
                field_0xab0 = sp54;
            }
        }
    } else if (mStts.GetCCMoveP() != NULL) {
        current.pos += *mStts.GetCCMoveP();
    }

    mAcch.CrrPos(dComIfG_Bgsp());

    if (mpPathMove == NULL) {
        if (!mJumpFlg) {
            if (!mAcch.ChkGroundHit()) {
                if (current.angle.x < 0) {
                    mJumpFlg = true;
                    speed.y = speedF;
                    if (speed.y > player->getSpinnerRideSpeedF()) {
                        speed.y = player->getSpinnerRideSpeedF();
                    }
                    speed.y *= -player->getSpinnerJumpRate() * cM_ssin(current.angle.x);
                } else {
                    f32 temp_f28 = mAcch.GetGroundH() - current.pos.y;
                    f32 var_f29 = current.pos.absXZ(old.pos) * cM_ssin(fopAcM_getPolygonAngle(mAcch.m_gnd, current.angle.y));
                    if (var_f29 < 0.0f) {
                        var_f29 = 0.0f;
                    }

                    if (temp_f28 < 0.0f && temp_f28 >= -mAcchCir[0].GetWallH() - var_f29) {
                        current.pos.y = mAcch.GetGroundH();
                        mAcch.SetGroundHit();
                        speed.y = 0.0f;
                    } else {
                        mJumpFlg = true;
                    }
                }
            }
        } else if (mAcch.ChkGroundHit()) {
            mJumpFlg = false;
        } else {
            mJumpFlg = true;
        }
    }

    if (mAcch.ChkWaterHit()) {
        f32 temp_f27 = mAcch.m_wtr.GetHeight() - current.pos.y;
        if ((temp_f27 > 50.0f || (dComIfG_Bgsp().GetPolyAtt0(mAcch.m_wtr) == 6 && temp_f27 >= 0.0f))) {
            mDeleteFlg = true;
            return 0;
        }
    }

    return 1;
}

void daSpinner_c::setReflectAngle() {
    if (mpPathMove != NULL) {
        current.angle.y += 0x8000;
        current.angle.x *= -1;

        setNextPathNum();

        mPathDirection *= -1;
        field_0xa76 *= -1;
    } else {
        s16 angle = current.angle.y - field_0xa80;
        if (abs(angle) > 0x4800) {
            current.angle.y = (field_0xa80 << 1) - (current.angle.y + 0x8000);
        } else if (angle > 0x3800) {
            current.angle.y = field_0xa80 + 0x3800;
        } else if (angle < -0x3800) {
            current.angle.y = field_0xa80 - 0x3800;
        }
    } 
}

void daSpinner_c::setWallHit(s16 param_0, u32 param_1) {
    field_0xa80 = param_0;
    setReflectAngle();

    cXyz line_start(current.pos.x, current.pos.y + mAcchCir[0].GetWallH(), current.pos.z);
    cXyz line_end(current.pos.x - (mAcchCir[0].GetWallR() * cM_ssin(param_0)), line_start.y, current.pos.z - (mAcchCir[0].GetWallR() * cM_scos(param_0)));
    mLinChk.Set(&line_start, &line_end, this);
    
    csXyz spC;
    cXyz* var_r29;
    if (dComIfG_Bgsp().LineCross(&mLinChk)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(mLinChk, &plane);
        var_r29 = mLinChk.GetCrossP();

        spC.set(cM_atan2s(plane.mNormal.y, plane.mNormal.absXZ()), cM_atan2s(-plane.mNormal.x, -plane.mNormal.z), 0);
    } else {
        var_r29 = &line_end;
        spC.set(0, param_0 + 0x8000, 0);
    }

    if (field_0xa78 == 0) {
        daAlink_getAlinkActorClass()->itemHitSE(Z2SE_HIT_SPINNER, param_1, &mSound);
        field_0xa78 = 10;

        if (field_0xa79 == 0) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER1, 1, cXyz(0.0f, 1.0f, 0.0f));
            dComIfGp_setHitMark(9, NULL, var_r29, &spC, NULL, 0);
        }
    }
}

void daSpinner_c::setAnm() {
    if (mBck.isStop() && mTrigJump) {
        mBck.init(mpSpoutBck, 1, -1, 1.0f, 0, -1, true);
        daAlink_getAlinkActorClass()->seStartOnlyReverb(Z2SE_AL_SPINNER_EXTEND);
        dComIfGp_getVibration().StartShock(2, 1, cXyz(0.0f, 1.0f, 0.0f));

        if (!mJumpFlg) {
            speed.y = 20.0f;
            mJumpFlg = true;
            mButtonJump = true;
        }

        if (mpPathMove != NULL) { 
            s16 targetAngle = cLib_targetAngleY(&mpPathMove->m_points[mPathNo].m_position, &current.pos);
            if (s16(targetAngle - current.angle.y) > 0) {
                current.angle.y += 0x3000;
            } else {
                current.angle.y -= 0x3000;
            }

            clearPathMove();
            mAcch.ClrWallHit();
        }
    }
}

int daSpinner_c::setNextPathNum() {
    if (mPathNo == 0 && mPathDirection == -1) {
        mPathNo = mpPathMove->m_num - 1;
    } else if (mPathNo == mpPathMove->m_num - 1 && mPathDirection == 1) {
        mPathNo = 0;
    } else {
        mPathNo += mPathDirection;
        return 1;
    }

    return 0;
}

Vec* daSpinner_c::getPathNextPos() {
    int next_point_no = mPathNo + mPathDirection;
    if (next_point_no < 0) {
        return &(mpPathMove->m_points + mpPathMove->m_num - 1)->m_position;
    }

    if (next_point_no >= mpPathMove->m_num) {
        return &mpPathMove->m_points[0].m_position;
    }

    return &mpPathMove->m_points[next_point_no].m_position;
}

int daSpinner_c::checkLineWallHit(cXyz* i_start, cXyz* i_end) {
    mLinChk.Set(i_start, i_end, this);
    if (dComIfG_Bgsp().LineCross(&mLinChk)) {
        if (dBgS_CheckBWallPoly(mLinChk) || daAlink_c::checkStageName("D_MN10A")) {
            return 1;
        }
    }

    return 0;
}

int daSpinner_c::checkPathMove() {
    if (mpPathMove != NULL) {
        return 1;
    }

    daTagSppath_c* sppath_tag = (daTagSppath_c*)mCyl.GetAtHitAc();

    if (mCyl.ChkAtHit() && mSpinnerTag == TAG_NONE && sppath_tag != NULL && fopAcM_GetName(sppath_tag) == PROC_Tag_Sppath) {
        if (sppath_tag->getKeepPath() == field_0xa68) {
            return 0;
        }

        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER2, 1, cXyz(0.0f, 1.0f, 0.0f));
        mpPathMove = sppath_tag->getKeepPath();
        field_0xa68 = mpPathMove;
        mJumpFlg = false;
        speed.y = 0.0f;

        int var_r27 = sppath_tag->getKeepPointNum();
        int sp10;
        if (var_r27 != mpPathMove->m_num - 1) {
            sp10 = var_r27 + 1;
        } else if (dPath_ChkClose(mpPathMove)) {
            sp10 = 0;
        } else {
            sp10 = var_r27;
            var_r27--;
        }

        dPnt* temp_r26 = &mpPathMove->m_points[var_r27];
        dPnt* temp_r25 = &mpPathMove->m_points[sp10];
        cXyz sp50(temp_r25->m_position.x - temp_r26->m_position.x,
                  temp_r25->m_position.y - temp_r26->m_position.y,
                  temp_r25->m_position.z - temp_r26->m_position.z);

        s16 sp8 = sp50.atan2sX_Z();
        cXyz sp44;
        cXyz sp38;
        cXyz sp2C;
        cXyz sp20;

        static const Vec sideCheckVec0 = {30.0f, 10.0f, 5.0f};
        static const Vec sideCheckVec1 = {-30.0f, 10.0f, 5.0f};
        static const Vec initSide0 = {53.0f, 0.0f, 0.0f};
        static const Vec initSide1 = {-53.0f, 0.0f, 0.0f};
        static const Vec initSideOld0 = {78.0f, 0.0f, 0.0f};
        static const Vec initSideOld1 = {-78.0f, 0.0f, 0.0f};

        mDoMtx_stack_c::transS(sppath_tag->current.pos);
        mDoMtx_stack_c::YrotM(sp8);
        mDoMtx_stack_c::multVec(&sideCheckVec0, &sp44);
        mDoMtx_stack_c::multVec(&sideCheckVec1, &sp38);

        int var_r24 = 0;
        int spC = 1;
        cM3dGPla sp5C;

        if (checkLineWallHit(&sp44, &sp38)) {
            var_r24 = 1;
            mDoMtx_stack_c::multVec(&initSide0, &sp2C);
            mDoMtx_stack_c::multVec(&initSideOld0, &sp20);
        } else if (checkLineWallHit(&sp38, &sp44)) {
            var_r24 = 1;
            mDoMtx_stack_c::multVec(&initSide1, &sp2C);
            mDoMtx_stack_c::multVec(&initSideOld1, &sp20);
            spC = 0;
        } else {
            mDoMtx_stack_c::YrotM(-0x8000);
            mDoMtx_stack_c::multVec(&sideCheckVec1, &sp44);
            mDoMtx_stack_c::multVec(&sideCheckVec0, &sp38);

            if (checkLineWallHit(&sp44, &sp38)) {
                var_r24 = 1;
                mDoMtx_stack_c::multVec(&initSide1, &sp2C);
                mDoMtx_stack_c::multVec(&initSideOld1, &sp20);
            } else if (checkLineWallHit(&sp38, &sp44)) {
                var_r24 = 1;
                mDoMtx_stack_c::multVec(&initSide0, &sp2C);
                mDoMtx_stack_c::multVec(&initSideOld0, &sp20);
                spC = 0;
            }
        }

        if (sppath_tag->checkDirectionFront() || (sppath_tag->checkDirectionNone() && cLib_distanceAngleS(sp8, current.angle.y) < 0x4000)) {
            current.angle.y = sp8;
            current.angle.x = sp50.atan2sY_XZ();
            mPathNo = var_r27;
            mPathDirection = 1;

            if (spC != 0) {
                field_0xa76 = -1;
            } else {
                field_0xa76 = 1;
            }
        } else {
            current.angle.y = sp8 + 0x8000;
            current.angle.x = cM_atan2s(sp50.y, sp50.absXZ());
            mPathNo = sp10;
            mPathDirection = -1;

            if (spC != 0) {
                field_0xa76 = 1;
            } else {
                field_0xa76 = -1;
            }
        }

        if (var_r24 != 0) {
            current.pos = sp2C;
            old.pos = sp20;
            old.pos.y += -20.0f;
        } else {
            current.pos = sppath_tag->current.pos;
        }

        field_0xab0 = current.pos;
        current.pos.y += -20.0f;

        mAcchCir[0].SetWallR(58.0f);
        mCyl.SetR(58.0f);

        mRideMoveTime = daAlink_getAlinkActorClass()->getSpinnerRideMoveTime();

        if (mpPathMove->field_0x7 == 0xFF) {
            speedF = daAlink_getAlinkActorClass()->getSpinnerRideSpeedF();
        } else {
            speedF = mpPathMove->field_0x7;
        }

        return 1;
    }

    field_0xa68 = 0;
    return 0;
}

void daSpinner_c::setSpreadEffect() {
    u32* emitter_id = mSpreadEmitterIDs;

    mDoMtx_stack_c::copy(mpModel->getBaseTRMtx());
    if (field_0xa76 < 0) {
        mDoMtx_stack_c::XrotM(0x7FFF);
    }

    mDoMtx_stack_c::get()[1][3] -= 10.0f;

    for (int i = 0; i < 2; i++, emitter_id++) {
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(*emitter_id);

        if (reflectAccept() || (emitter != NULL && !emitter->isEnableDeleteEmitter())) {
            static const u16 effName[2] = {0x8C5, 0x8C6};
            *emitter_id = dComIfGp_particle_set(*emitter_id, effName[i], &current.pos, &shape_angle, NULL);
        }

        emitter = dComIfGp_particle_getEmitter(*emitter_id);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(mDoMtx_stack_c::get());
        }
    }
}

void daSpinner_c::clearSpreadEffect() {
    for (int i = 0; i < 2; i++) {
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mSpreadEmitterIDs[i]);
        if (emitter != NULL) {
            emitter->stopDrawParticle();
        }
    }
}

int daSpinner_c::execute() {
    if (mDeleteFlg) {
        clearSpreadEffect();
        fopAcM_delete(this);
        return 1;
    }

    int sp24 = 0;
    mButtonJump = false;

    daAlink_c* player = daAlink_getAlinkActorClass();
    if (!player->checkGameOverWindow() && field_0xa78 != 0) {
        field_0xa78--;
    }

    f32 pad_stick_value;
    s16 move_angle;
    if (dComIfGp_event_runCheck()) {
        pad_stick_value = 0.0f;
        move_angle = 0;
        mTrigJump = false;
    } else {
        if (mSpinnerTag != TAG_NONE || mpPathMove != NULL || (mJumpFlg && daAlink_c::checkStageName("D_MN10A") && dComIfGs_isZoneSwitch(6, dComIfGp_roomControl_getStayNo()) && dComIfGs_isZoneSwitch(7, dComIfGp_roomControl_getStayNo()))) {
            pad_stick_value = 0.0f;
        } else {
            pad_stick_value = mDoCPd_c::getStickValue(PAD_1);
        }

        move_angle = (mDoCPd_c::getStickAngle3D(PAD_1) + 0x10000 + dCam_getControledAngleY(dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0)))) - 0x8000;

#if VERSION == VERSION_SHIELD_DEBUG
        if (dComIfG_getTrigB(PAD_1) && dComIfGp_getSelectItem(3) == fpcNm_ITEM_SPINNER) {
#else
        if (dComIfG_getTrigA(PAD_1)) {
#endif
            mTrigJump = true;
        } else {
            mTrigJump = false;
        }

        if (fopAcM_GetParam(this) != 0 && mSpinnerTag == TAG_NONE) {
            setAnm();
        }
    }

    if (mPathForceRemove) {
        if (mpPathMove != NULL) {
            clearPathMove();
            mAcch.ClrWallHit();
        }
        mPathForceRemove = false;
    }

    bool sp10 = player->checkSpinnerRideOwn(this);
    mRotY += field_0xa82 * field_0xa76;
    if (reflectAccept()) {
        mRotY += field_0xa76 * 2000;
    }

    mBck.play();

    mSound.framework(0, mReverb);

    if (fopAcM_GetParam(this) == 0) {
        field_0xa7c += (int)(2330.0f * (0.85f + cM_rndF(0.3f)));
        field_0xa84 = 5.0f * cM_ssin(field_0xa7c);
        field_0xa98 = current.pos;

        if (sp10 != 0) {
            current.angle.y = player->current.angle.y;
        } else if (!player->checkSpinnerReady()) {
            clearSpreadEffect();
            fopAcM_delete(this);
            return 1;
        }
    } else if (mSpinnerTag != TAG_NONE) {
        mRideMoveTime = player->getSpinnerRideMoveTime();
        speedF = player->getSpinnerRideSpeedF();
        mJumpFlg = false;
        field_0xa76 = 1;

        if (mSpinnerTag == TAG_START) {
            clearPathMove();

            f32 temp_f30 = current.pos.y;
            if (cLib_chasePosXZ(&current.pos, field_0xaa4, speedF)) {
                mSpinnerTag = 2;
                speed.y = 0.0f;
                field_0xa84 = 0.0f;
                current.pos.y = temp_f30;
            }
        } else if (mSpinnerTag == 2) {
            speed.y += gravity;
            if (speed.y < maxFallSpeed) {
                speed.y = maxFallSpeed;
            }

            if (cLib_chaseF(&current.pos.y, field_0xaa4.y, fabsf(speed.y))) {
                mSpinnerTag = TAG_INTO;
                field_0xa82 = 0x800;

                dComIfGp_particle_setPolyColor(0xE7, mAcch.m_gnd, &player->current.pos, &tevStr, NULL, NULL, 0, NULL, -1, NULL);
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER4, 1, cXyz(0.0f, 1.0f, 0.0f));
            }
        } else {
            current.pos = field_0xaa4;

            if (mSpinnerTag != TAG_END) {
                if (!dComIfGp_event_runCheck() && mTrigJump) {
                    field_0xa82 += 0x200;
                    if (field_0xa82 > 5000) {
                        field_0xa82 = 5000;
                    }
                    mSpinnerTag = TAG_INTO_INC_ROT;
                } else {
                    field_0xa82 -= 0x40;
                    if (field_0xa82 < 0) {
                        field_0xa82 = 0;
                    }
                    mSpinnerTag = TAG_INTO;
                }
            }
        }
    } else {
        field_0xa82 = 0x82F;
        cLib_chaseF(&field_0xa84, 0.0f, 0.5f);

        if (mRideMoveTime == 0) {
            if (cLib_addCalc(&speedF, 0.0f, player->getSpinnerRideDecSpeedRate(), player->getSpinnerRideDecSpeedMax(), player->getSpinnerRideDecSpeedMin()) < 0.1f) {
                mDeleteFlg = true;
                return 1;
            }
        } else if (!player->checkDemoSpinnerKeep() && mpPathMove == NULL) {
            mRideMoveTime--;
        }

        cM3dGPla tri;
        s16 sp12;
        if (mAcch.ChkGroundHit()) {
            dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &tri);
            sp12 = tri.mNormal.atan2sX_Z();
        }

        if (!checkPathMove()) {
            if (mAcch.ChkWallHit() && field_0xa74 == 0) {
                dBgS_AcchCir* acch_cir = mAcchCir;
                for (int i = 0; i < 3; i++, acch_cir++) {
                    if (acch_cir->ChkWallHit()) {
                        setWallHit(acch_cir->GetWallAngleY(), dKy_pol_sound_get(acch_cir));
                        break;
                    }
                }
            } else if (mAcch.ChkGroundHit() && tri.mNormal.y < player->getSlideLimit() && dComIfG_Bgsp().GetSpecialCode(mAcch.m_gnd) != 2 && abs((s16)(sp12 - current.angle.y)) > 0x4000) {
                setWallHit(sp12, dKy_pol_sound_get(&mAcch.m_gnd));
            } else if (mCyl.ChkAtHit() && (mCyl.ChkAtShieldHit() || (mCyl.GetAtHitGObj() != NULL && ((dCcD_GObjInf*)mCyl.GetAtHitGObj())->ChkTgSpinnerReflect()))) {
                cXyz sp58;
                if (mCyl.GetAtHitAc() != NULL) {
                    sp58 = current.pos - mCyl.GetAtHitAc()->current.pos;
                } else {
                    sp58 = current.pos - *mCyl.GetAtHitPosP();
                }

                field_0xa80 = sp58.atan2sX_Z();
                setReflectAngle();
            } else if (sp10 != 0 && pad_stick_value > 0.3f) {
                cLib_addCalcAngleS(&current.angle.y, move_angle, 4, player->getSpinnerRideRotAngleMax(), player->getSpinnerRideRotAngleMin());
            }
        }

        if (sp10 != 0) {
            mCyl.OffCoSPrmBit(0x10);
        } else {
            mCyl.OnCoSPrmBit(0x10);
        }

        field_0xa74 = 0;
    }

    s16 var_r27;
    if (mSpinnerTag != TAG_NONE || mpPathMove != NULL || sp24 != 0) {
        var_r27 = 0;
    } else {
        var_r27 = ((shape_angle.y - current.angle.y) / player->getSpinnerRideRotAngleMax()) * 1500;
    }

    cLib_addCalcAngleS(&shape_angle.z, var_r27, 3, 300, 50);

    if (mSpinnerTag != TAG_NONE || mpPathMove != NULL) {
        var_r27 = 0;
    } else {
        var_r27 = 2000.0f * (speedF / player->getSpinnerRideSpeedF());
        if (var_r27 > 2000) {
            var_r27 = 2000;
        }
    }

    cLib_addCalcAngleS(&shape_angle.x, var_r27, 3, 300, 50);
    shape_angle.y = current.angle.y;

    if (mSpinnerTag == TAG_NONE) {
        if (!player->checkGameOverWindow() && !posMove()) {
            return 1;
        }

        if (!dComIfGp_event_runCheck() && speedF > 3.0f && field_0xa98.abs2XZ(current.pos) < 9.0f) {
            field_0xa79++;
            if (field_0xa79 > 30) {
                mDeleteFlg = true;
            }
        } else {
            field_0xa79 = 0;
            field_0xa98 = current.pos;
        }
    } else {
        field_0xa79 = 0;
        field_0xa98 = current.pos;
        cXyz sp4C(speed);

        mAcch.CrrPos(dComIfG_Bgsp());

        current.pos = field_0xa98;
        speed = sp4C;
    }

    setRoomInfo();
    setMatrix();
    setEffect();

    if (mpPathMove == NULL) {
        if (mAcch.ChkGroundHit()) {
            current.angle.x = fopAcM_getPolygonAngle(mAcch.m_gnd, current.angle.y);
        } else {
            current.angle.x = 0;
        }
    }

    mCyl.MoveCAt(current.pos);
    dComIfG_Ccsp()->Set(&mCyl);
    dComIfG_Ccsp()->SetMass(&mCyl, 1);

    if (reflectAccept()) {
        if (mpPathMove != NULL) {
            mAcchCir[0].SetWallR(58.0f);
            mCyl.SetR(58.0f);
        } else {
            mAcchCir[0].SetWallR(100.0f);
            mCyl.SetR(100.0f);
        }

        mCyl.SetAtAtp(2);
    } else {
        mAcchCir[0].SetWallR(58.0f);
        mCyl.SetR(58.0f);
        mCyl.SetAtAtp(1);
    }

    setSpreadEffect();
    return 1;
}

static int daSpinner_Execute(daSpinner_c* i_this) {
    return i_this->execute();
}

int daSpinner_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    if ((daAlink_getAlinkActorClass()->checkSpinnerReady() && daAlink_getAlinkActorClass()->gravity >= 0.0f) || mDeleteFlg) {
        return 1;
    }

    mDoExt_modelEntryDL(mpModel);
    daMirror_c::entry(mpModel);

    if (!daAlink_getAlinkActorClass()->checkSpinnerRideOwn(this)) {
        cXyz sp8(current.pos.x, 70.0f + current.pos.y, current.pos.z);
        field_0xa94 = dComIfGd_setShadow(field_0xa94, 1, mpModel, &sp8, 300.0f, 0.0f, sp8.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static int daSpinner_Draw(daSpinner_c* i_this) {
    return i_this->draw();
}

static actor_method_class l_daSpinner_Method = {
    (process_method_func)daSpinner_Create,
    (process_method_func)daSpinner_Delete,
    (process_method_func)daSpinner_Execute,
    (process_method_func)NULL,
    (process_method_func)daSpinner_Draw,
};

extern actor_process_profile_definition g_profile_SPINNER = {
  fpcLy_CURRENT_e,       // mLayerID
  4,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_SPINNER,          // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(daSpinner_c),            // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  687,                   // mPriority
  &l_daSpinner_Method,   // sub_method
  0x00060000,            // mStatus
  fopAc_UNK_GROUP_5_e,   // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};

AUDIO_INSTANCES
struct JAUSectionHeap;
template<>
JAUSectionHeap* JASGlobalInstance<JAUSectionHeap>::sInstance; 
