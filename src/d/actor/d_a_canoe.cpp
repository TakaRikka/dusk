/**
 * @file d_a_canoe.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_canoe.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_tag_waterfall.h"
#include "d/actor/d_a_obj_gomikabe.h"
#include "f_op/f_op_kankyo_mng.h"
#include "JSystem/JAudio2/JAUSectionHeap.h"
#include "Z2AudioLib/Z2Instances.h"

#if DEBUG
static BOOL l_debugMode; 
#endif

static BOOL daCanoe_searchTagWaterFall(fopAc_ac_c* i_actor, void* i_data) {
    if (fopAcM_GetName(i_actor) == PROC_Tag_WaterFall) {
        if (((daTagWaterFall_c*)i_actor)->checkHitWaterFall(*(cXyz*)i_data)) {
            daAlink_getAlinkActorClass()->onWaterFallFrontHit();
        }
    }

    return TRUE;
}

int daCanoe_c::createHeap() {
    void* modelData = dComIfG_getObjectRes(mArcName, 4);
    void* paddleModelData = dComIfG_getObjectRes(mArcName, 3);
    void* maskModelData = dComIfG_getObjectRes(mArcName, 5);
    JUT_ASSERT(161, modelData);
    JUT_ASSERT(162, paddleModelData);
    JUT_ASSERT(163, maskModelData);

    mpModel = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    mpPaddleModel = mDoExt_J3DModel__create((J3DModelData*)paddleModelData, 0x80000, 0x11000084);
    if (mpPaddleModel == NULL) {
        return 0;
    }

    mpMaskModel = mDoExt_J3DModel__create((J3DModelData*)maskModelData, 0x80000, 0x11000084);
    if (mpMaskModel == NULL) {
        return 0;
    }

    mSound.init(&current.pos, 1);
    return 1;
}

static int daCanoe_createHeap(fopAc_ac_c* i_this) {
    return ((daCanoe_c*)i_this)->createHeap();
}

static u8 const lit_3766[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const char l_arcName[] = "Canoe";

static const char l_arcName2[] = "CanoeB";

static const char l_arcName3[] = "CanoeE3";

static const f32 l_cylOffsetZ[5] = {200.0f, 110.0f, 20.0f, -70.0f, -160.0f};

static dCcD_SrcCyl l_cylSrc = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x10}, 0x79}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            55.0f, // mRadius
            80.0f // mHeight
        } // mCyl
    }
};

int daCanoe_c::create() {
    fopAcM_ct(this, daCanoe_c);

    if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0) {
             /* dSv_event_flag_c::F_0463 - Fishing Pond - Reserved for fishing */
        if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[463])) {
            return cPhs_ERROR_e;
        }

            /* dSv_event_flag_c::F_0464 - Fishing Pond - Reserved for fishing */
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[464])) {
            fopAcM_SetParam(this, 2);
        } else {
            fopAcM_SetParam(this, 1);
        }
    }

    u32 heap_size = 0x1810;
    if (fopAcM_GetParam(this) == 2) {
        mArcName = l_arcName3;
    } else if (fopAcM_GetParam(this) == 1) {
        mArcName = l_arcName;
        heap_size = 0x1EB0;
    } else {
        mArcName = l_arcName2;
    }

    int phase_state = dComIfG_resLoad(&mPhaseReq, mArcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, daCanoe_createHeap, heap_size)) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_SetMin(this, -80.0f, -40.0f, -230.0f);
        fopAcM_SetMax(this, 80.0f, 90.0f, 280.0f);

        mCcStts.Init(200, 0, this);
        mCcStts.SetRoomId(fopAcM_GetRoomNo(this));
    
        setFrontBackPos();
        mOldFrontPos = mFrontPos;
        mOldBackPos = mBackPos;

        for (int i = 0; i < 3; i++) {
            mAcchCir[i][0].SetWall(10.0f, 60.0f);
            mAcchCir[i][1].SetWall(40.0f, 60.0f);
            mAcchCir[i][2].SetWall(80.0f, 60.0f);
        }

        mAcch[0].Set(this, 3, mAcchCir[0]);
        mAcch[1].Set(&mFrontPos, &mOldFrontPos, this, 3, mAcchCir[1], fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch[2].Set(&mBackPos, &mOldBackPos, this, 3, mAcchCir[2], fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));

        mAcch[1].ClrWaterNone();
        mAcch[1].SetWtrChkMode(2);
        mAcch[2].ClrWaterNone();
        mAcch[2].SetWtrChkMode(2);
        mAcch[0].ClrWaterNone();
        mAcch[0].SetWtrChkMode(2);

        dCcD_Cyl* cyl = mCcCyl;
        for (int i = 0; i < 5; i++, cyl++) {
            cyl->Set(l_cylSrc);
            cyl->SetStts(&mCcStts);
        }

        tevStr.room_no = fopAcM_GetRoomNo(this);
        mReverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
        mAimAngle = shape_angle.y;
        attention_info.distances[fopAc_attn_ETC_e] = 32;
        mRodID = fpcM_ERROR_PROCESS_ID_e;
        setMatrix();
    }

    return phase_state;
}

static int daCanoe_Create(fopAc_ac_c* i_this) {
    daCanoe_c* a_this = (daCanoe_c*)i_this;
    fpc_ProcID id = fopAcM_GetID(i_this);
    return a_this->create();
}

daCanoe_c::~daCanoe_c() {
    dComIfG_resDelete(&mPhaseReq, mArcName);
    mSound.deleteObject();
}

static int daCanoe_Delete(daCanoe_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    i_this->~daCanoe_c();
    return 1;
}

void daCanoe_c::setRoomInfo() {
    int room_no;
    if (mAcch[0].GetGroundH() != -G_CM3D_F_INF) {
        room_no = dComIfG_Bgsp().GetRoomId(mAcch[0].m_gnd);
        tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mAcch[0].m_gnd);
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
    }

    if (room_no != fopAcM_GetRoomNo(this)) {
        tevStr.room_no = room_no;
        mReverb = dComIfGp_getReverb(room_no);
        mCcStts.SetRoomId(room_no);
        fopAcM_SetRoomNo(this, room_no);
    }
}

void daCanoe_c::setMatrix() {
    f32 var_f29;
    if (mShapeOffsetY > 5.0f) {
        var_f29 = 5.0f;
    } else if (mShapeOffsetY < -5.0f) {
        var_f29 = -5.0f;
    } else {
        var_f29 = mShapeOffsetY;
    }

    f32 var_f31 = 0.1f;
    if (fabsf(speedF) < 1.0f) {
        var_f31 *= 0.65f;
    }

    if (!daAlink_getAlinkActorClass()->checkCanoeRideOwn(this) || daAlink_getAlinkActorClass()->checkFishingRodEquip()) {
        var_f31 *= 0.5f;
    }

    field_0x1470 -= var_f31 * cM_fsin(0.31415927f * var_f29);

    if (fabsf(mShapeOffsetY) < 0.05f && fabsf(field_0x1470) < 0.5f) {
        field_0x1470 *= 1.4f;

        f32 var_f28;
        if (field_0x1470 >= 0.0f) {
            var_f28 = -1.0f;
        } else {
            var_f28 = 1.0f;
        }

        if (fabsf(field_0x1470) < 0.0001f) {
            field_0x1470 = 0.5f * var_f28;
        }
    } else if (fabsf(field_0x1470) < 0.0001f && fabsf(mShapeOffsetY) >= 0.05f) {
        if (mShapeOffsetY > 0.0f) {
            field_0x1470 = -0.5f;
        } else {
            field_0x1470 = 0.5f;
        }
    }

    mShapeOffsetY += field_0x1470;

    f32 temp_f30 = 5.0f;
    if (mShapeOffsetY > temp_f30) {
        mShapeOffsetY = temp_f30;
        field_0x1470 = 0.0f;
    } else if (mShapeOffsetY < -temp_f30) {
        mShapeOffsetY = -temp_f30;
        field_0x1470 = 0.0f;
    } else if (fabsf(mShapeOffsetY) > 3.0f) {
        cLib_chaseF(&mShapeOffsetY, 0.0f, 0.015f);
        field_0x1470 *= 0.97f;
    }

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + mShapeOffsetY, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMaskModel->setBaseTRMtx(mDoMtx_stack_c::get());

    attention_info.position.set(current.pos.x - (35.0f * cM_ssin(shape_angle.y)), 40.0f + current.pos.y, current.pos.z - (35.0f * cM_scos(shape_angle.y)));
    eyePos = attention_info.position;
}

void daCanoe_c::setCollision() {
    mCcStts.Move();

    f32 temp_f31 = cM_ssin(shape_angle.y);
    f32 temp_f30 = cM_scos(shape_angle.y);

    cXyz cyl_center;
    cyl_center.y = current.pos.y - 30.0f;

    dCcD_Cyl* cyl = mCcCyl;
    const f32* offset_z = l_cylOffsetZ;

    if (daAlink_getAlinkActorClass()->checkCanoeRideOwn(this)) {
        field_0x1441 = 5;
    } else if (field_0x1441 != 0) {
        field_0x1441--;
    }

    for (int i = 0; i < 5; i++, cyl++, offset_z++) {
        cyl_center.x = current.pos.x + (*offset_z * temp_f31);
        cyl_center.z = current.pos.z + (*offset_z * temp_f30);
        cyl->SetC(cyl_center);
        dComIfG_Ccsp()->Set(cyl);

        if (field_0x1441 != 0) {
            cyl->SetCoVsGrp(0x60);
        } else {
            cyl->SetCoVsGrp(0x70);
        }
    }
}

void daCanoe_c::posMove() {
    f32 move_speed = speedF * cM_scos(field_0x1448);
    current.pos.x += move_speed * cM_ssin(current.angle.y);
    current.pos.y -= speedF * cM_ssin(field_0x1448);
    current.pos.z += move_speed * cM_scos(current.angle.y);

    current.pos += *mCcStts.GetCCMoveP();
    mCcStts.ClrCcMove();

    cXyz stream_speed;
    int stream_power;
    f32 speed_step;
    if (fopAcM_getWaterStream(&current.pos, mAcch[0].m_gnd, &stream_speed, &stream_power, TRUE)) {
        stream_speed *= stream_power;
        if (mWaterSpeed.abs2XZ() > stream_speed.abs2XZ()) {
            speed_step = 3.0f;
        } else {
            speed_step = 1.0f;
        }
    } else {
        stream_speed = cXyz::Zero;
        speed_step = 5.0f;
    }

    cLib_addCalcPosXZ(&mWaterSpeed, stream_speed, 0.5f, speed_step, 0.5f);
    current.pos.x += mWaterSpeed.x;
    current.pos.z += mWaterSpeed.z;
}

void daCanoe_c::checkGomikabe(cBgS_PolyInfo& i_polyinfo) {
    if (daAlink_c::getMoveBGActorName(i_polyinfo, 1) == PROC_Obj_GOMIKABE) {
        ((daObjGOMIKABE_c*)dComIfG_Bgsp().GetActorPointer(i_polyinfo))->BreakSet();
    }
}

void daCanoe_c::setFrontBackPos() {
    f32 temp_f31 = cM_ssin(shape_angle.y);
    f32 temp_f1 = cM_scos(shape_angle.y);
    mFrontPos.set(current.pos.x + (l_cylOffsetZ[0] * temp_f31), current.pos.y, current.pos.z + (l_cylOffsetZ[0] * temp_f1));
    mBackPos.set(current.pos.x + (l_cylOffsetZ[4] * temp_f31), current.pos.y, current.pos.z + (l_cylOffsetZ[4] * temp_f1));
}

void daCanoe_c::frontBackBgCheck() {
    setFrontBackPos();
    mAcch[1].CrrPos(dComIfG_Bgsp());
    mAcch[2].CrrPos(dComIfG_Bgsp());

    int sp20;
    int sp1C;
    BOOL front_hit = FALSE;
    BOOL back_hit = FALSE;
    cM3dGPla sp78;

    if ((mFrontPos.y - mAcch[1].GetGroundH()) < 30.0f) {
        mFrontPos = mOldFrontPos;
        sp20 = 1;
    } else {
        sp20 = 0;
    }

    if ((mBackPos.y - mAcch[2].GetGroundH()) < 30.0f) {
        mBackPos = mOldBackPos;
        sp1C = 1;
    } else {
        sp1C = 0;
    }

    cXyz sp6C;
    f32 temp_f28 = 60.0f * cM_ssin(shape_angle.y);
    f32 temp_f27 = 60.0f * cM_scos(shape_angle.y);

    s16 spA, sp8;
    if (sp20 == 0) {
        sp6C.set(mFrontPos.x + temp_f28, mFrontPos.y, mFrontPos.z + temp_f27);
        mLineChk.Set(&current.pos, &sp6C, this);

        if (dComIfG_Bgsp().LineCross(&mLineChk)) {
            sp6C -= mLineChk.GetCross();
            dComIfG_Bgsp().GetTriPla(mLineChk, &sp78);

            f32 temp_f30 = -sp6C.absXZ() * cM_scos(sp6C.atan2sX_Z() - cM_atan2s(-sp78.mNormal.x, -sp78.mNormal.z));
            mFrontPos.x -= temp_f30 * sp78.mNormal.x;
            mFrontPos.y -= sp6C.y;
            mFrontPos.z -= temp_f30 * sp78.mNormal.z;
            front_hit = TRUE;

            spA = sp78.mNormal.atan2sX_Z();
            checkGomikabe(mLineChk);
        } else if (mAcch[1].ChkWallHit()) {
            front_hit = TRUE;
            for (int i = 0; i < 3; i++) {
                if (mAcchCir[1][i].ChkWallHit()) {
                    spA = mAcchCir[1][i].GetWallAngleY();
                    checkGomikabe(mAcchCir[1][i]);
                }
            }
        }
    }

    if (sp1C == 0) {
        sp6C.set(mBackPos.x - temp_f28, mBackPos.y, mBackPos.z - temp_f27);
        mLineChk.Set(&current.pos, &sp6C, this);

        if (dComIfG_Bgsp().LineCross(&mLineChk)) {
            sp6C -= mLineChk.GetCross();
            dComIfG_Bgsp().GetTriPla(mLineChk, &sp78);

            f32 temp_f30_2 = -sp6C.absXZ() * cM_scos(sp6C.atan2sX_Z() - cM_atan2s(-sp78.mNormal.x, -sp78.mNormal.z));
            mBackPos.x -= temp_f30_2 * sp78.mNormal.x;
            mBackPos.y -= sp6C.y;
            mBackPos.z -= temp_f30_2 * sp78.mNormal.z;
            back_hit = TRUE;

            sp8 = sp78.mNormal.atan2sX_Z();
            checkGomikabe(mLineChk);
        } else if (mAcch[2].ChkWallHit()) {
            back_hit = TRUE;
            for (int i = 0; i < 3; i++) {
                if (mAcchCir[2][i].ChkWallHit()) {
                    sp8 = mAcchCir[2][i].GetWallAngleY();
                    checkGomikabe(mAcchCir[2][i]);
                }
            }
        }
    }

    if (mHitInvulnerabilityTimer > 0) {
        mHitInvulnerabilityTimer--;
    }

    if (daAlink_getAlinkActorClass()->checkCanoeSlider()) {
        for (int i = 0; i < 3; i++) {
            mAcchCir[i][1].SetWall(-50.0f, 60.0f);
        }
    
        if (mWaterSpeed.abs2XZ() > 400.0f && (front_hit || back_hit)) {
            if (!mIsSliderHit && mHitInvulnerabilityTimer == 0) {
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER6, 0xF, cXyz(0.0f, 1.0f, 0.0f));
                mIsSliderHit = TRUE;
                mHitInvulnerabilityTimer = 30;
                field_0x1490 = 0;
                mHitCount++;
                dMeter2Info_setMiniGameCount(mHitCount);

                if (field_0x1450 < 0) {
                    field_0x1450 -= 0x100;
                } else {
                    field_0x1450 += 0x100;
                }

                if (front_hit != 0) {
                    field_0x1460 = spA;
                } else {
                    field_0x1460 = sp8;
                }

                mWaterSpeed *= 0.6f;
                daAlink_getAlinkActorClass()->onCanoeItemCancel();

                if (dMeter2Info_getNowCount() != 0) {
                    dMeter2Info_setNowCount(dMeter2Info_getNowCount() - 1);
                    mDoAud_seStart(Z2SE_SY_MG_CANOE_SCORE_DW, NULL, 0, 0);
                }

                mSound.startSound(Z2SE_AL_CANOE_COL, 0, -1);
            }
        } else {
            mIsSliderHit = FALSE;
        }
    }

    cXyz sp60 = mFrontPos - mBackPos;
    f32 var_f29 = sp60.absXZ();
    if (var_f29 > 1.0f) {
        var_f29 = 1.0f / var_f29;
    } else {
        var_f29 = 1.0f;
    }

    shape_angle.y = sp60.atan2sX_Z();
    current.angle.y = shape_angle.y;

    f32 max_speed = daAlink_getAlinkActorClass()->getCanoeMaxSpeed();
    s16 var_r27;
    if (front_hit != 0 || sp20 != 0) {
        current.pos = mFrontPos - (sp60 * (l_cylOffsetZ[0] / (l_cylOffsetZ[0] - l_cylOffsetZ[4])));
        setFrontBackPos();

        if (speedF > 0.0f) {
            if (sp20 != 0) {
                speedF = 0.0f;
            } else {
                var_r27 = (shape_angle.y + 0x8000) - spA;
                if (abs(var_r27) < 0x4000) {
                    max_speed *= 1.0f - (0.6f * cM_scos(var_r27));
                    if (speedF > max_speed) {
                        speedF = max_speed;
                    }
                }
            }
        }
    } else if (sp1C != 0 || back_hit != 0) {
        current.pos = mBackPos - (sp60 * (l_cylOffsetZ[4] / (l_cylOffsetZ[0] - l_cylOffsetZ[4])));
        setFrontBackPos();

        if (speedF < 0.0f) {
            if (sp1C != 0) {
                speedF = 0.0f;
            } else {
                var_r27 = shape_angle.y - sp8;
                if (abs(var_r27) < 0x4000) {
                    max_speed *= -(1.0f - (0.6f * cM_scos(var_r27)));
                    if (speedF < max_speed) {
                        speedF = max_speed;
                    }
                }
            }
        }
    }

    if (mHitInvulnerabilityTimer != 0) {
        if ((s16)(shape_angle.y - field_0x1460) >= 0) {
            var_r27 = field_0x1460 + 0x4000;
        } else {
            var_r27 = field_0x1460 - 0x4000;
        }
        cLib_addCalcAngleS2(&shape_angle.y, var_r27, 4, 0x400);
        current.angle.y = shape_angle.y;

        current.pos.x += mHitInvulnerabilityTimer * cM_ssin(field_0x1460);
        current.pos.z += mHitInvulnerabilityTimer * cM_scos(field_0x1460);
        setFrontBackPos();
    }
}

void daCanoe_c::setPaddleEffect() {
    static cXyz paddleRippleScale(0.7f, 0.7f, 0.7f);

    daAlink_c* player = daAlink_getAlinkActorClass();
    if (player->checkCanoePaddleGrab(this)) {
        if (mPaddleWaterType == 4) {
            mPaddleWaterType = 1;
            return;
        }

        if (mPaddleWaterType == 0) {
            mPaddleWaterType = 2;
            return;
        }

        cXyz paddle_top;
        cXyz sp20;
        cXyz* local_paddle_top = daAlink_c::getCanoeLocalPaddleTop();
        mDoMtx_multVec(mpPaddleModel->getBaseTRMtx(), local_paddle_top, &paddle_top);

        if (fopAcM_wt_c::waterCheck(&paddle_top) && paddle_top.y < fopAcM_wt_c::getWaterY()) {
            cXyz sp14;
            mDoMtx_multVecZero(mpPaddleModel->getBaseTRMtx(), &sp14);

            cXyz sp8 = paddle_top - sp14;
            if (sp8.y > -1.0f) {
                sp8.y = -1.0f;
            }

            sp8 *= (fopAcM_wt_c::getWaterY() - sp14.y) / sp8.y;
            sp14 += sp8;
            sp14.y = fopAcM_wt_c::getWaterY();

            dComIfGp_particle_setWaterRipple(&field_0x1480, *fopAcM_wt_c::getWaterCheck(), &sp14, speedF, &tevStr, &paddleRippleScale, fopAcM_GetRoomNo(this));

            if (mPaddleWaterType == 2) {
                if (!player->checkCanoeWait() && !player->checkCanoeSubjectivity()) {
                    fopKyM_createWpillar(&sp14, 0.3f, 2);
                }
                mPaddleWaterType = 3;
            } else {
                mPaddleWaterType = 1;
            }
        } else {
            mPaddleWaterType = 2;
        }
    } else {
        mPaddleWaterType = 0;
    }
}

void daCanoe_c::setCanoeSliderEffect() {
    if (daAlink_getAlinkActorClass()->checkCanoeSlider()) {
        if (mWaterSpeed.abs2XZ() > 25.0f || fabsf(speedF) > 5.0f) {
            u32* emitter_id = field_0x1488;
            for (int i = 0; i < 2; i++, emitter_id++) {
                static u16 const effName[] = {0x8A5A, 0x8A5B};
                *emitter_id = dComIfGp_particle_set(*emitter_id, effName[i], &current.pos, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
                
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(*emitter_id);
                if (emitter != NULL) {
                    emitter->setGlobalRTMatrix(mpModel->getBaseTRMtx());
                }
            }
        }

        JPABaseEmitter* temp_r3_3 = dComIfGp_particle_getEmitter(field_0x1490);
        if ((temp_r3_3 != NULL && !temp_r3_3->isEnableDeleteEmitter()) || mHitInvulnerabilityTimer == 30) {
            field_0x1490 = dComIfGp_particle_set(field_0x1490, 0x8AA6, &current.pos, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
            
            JPABaseEmitter* temp_r3_4 = dComIfGp_particle_getEmitter(field_0x1490);
            if (temp_r3_4 != NULL) {
                temp_r3_4->setGlobalRTMatrix(mpModel->getBaseTRMtx());
            }
        }

        cXyz sp28 = daAlink_getAlinkActorClass()->current.pos + (current.pos - old.pos);
        fopAcIt_Executor((fopAcIt_ExecutorFunc)daCanoe_searchTagWaterFall, &sp28);
    }
}

int daCanoe_c::execute() {
    daAlink_c* player = daAlink_getAlinkActorClass();
    mOldFrontPos = mFrontPos;
    mOldBackPos = mBackPos;

#if DEBUG
    if (player->checkCanoeRideOwn(this) && daAlink_c::checkDebugMoveInput()) {
        if (l_debugMode) {
            l_debugMode = FALSE;
        } else {
            l_debugMode = TRUE;
        }
    }

    if (l_debugMode && player->checkCanoeRideOwn(this)) {
        f32 move_speed = 50.0f;
        if (mDoCPd_c::getHoldLockR(PAD_1)) {
            move_speed = 100.0f;
        }

        if (mDoCPd_c::getHoldY(PAD_1)) {
            current.pos.y += move_speed;
            mFrontPos.y += move_speed;
            mBackPos.y += move_speed;
        } else if (mDoCPd_c::getHoldX(PAD_1)) {
            current.pos.y -= move_speed;
            mFrontPos.y -= move_speed;
            mBackPos.y -= move_speed;
        }

        s16 temp_r26 = mDoCPd_c::getStickAngle3D(PAD_1);
        s16 move_angle = (dCam_getControledAngleY(dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0))) + temp_r26) - 0x8000;

        current.pos.x += move_speed * mDoCPd_c::getStickValue(PAD_1) * cM_ssin(move_angle);
        mFrontPos.x += move_speed * mDoCPd_c::getStickValue(PAD_1) * cM_ssin(move_angle);
        mBackPos.x += move_speed * mDoCPd_c::getStickValue(PAD_1) * cM_ssin(move_angle);
        current.pos.z += move_speed * mDoCPd_c::getStickValue(PAD_1) * cM_scos(move_angle);
        mFrontPos.z += move_speed * mDoCPd_c::getStickValue(PAD_1) * cM_scos(move_angle);
        mBackPos.z += move_speed * mDoCPd_c::getStickValue(PAD_1) * cM_scos(move_angle);

        old.pos = current.pos;
        mOldFrontPos = mFrontPos;
        mOldBackPos = mBackPos;
    } else
#endif
    {
        s16 sp12 = shape_angle.y;
        s16 var_r27;
        if (mIsForceAimAngle) {
            var_r27 = mAimAngle - shape_angle.y;
            s16 sp10 = player->getCanoeMaxRotSpeed();
            if (var_r27 > sp10) {
                var_r27 = sp10;
            } else if (var_r27 < -sp10) {
                var_r27 = -sp10;
            }
            mIsForceAimAngle = FALSE;
        } else {
            var_r27 = 0;
            if (player->checkCanoeSlider() && mWaterSpeed.abs2XZ() > 400.0f) {
                s16 spE = mWaterSpeed.atan2sX_Z();
                if (cLib_distanceAngleS(shape_angle.y, spE) > 0x4000) {
                    spE += 0x8000;
                }
                cLib_addCalcAngleS2(&shape_angle.y, spE, 4, 75);
            }
        }

        cLib_addCalcAngleS(&field_0x144e, var_r27, 5, 15, 5);
        shape_angle.y += field_0x144e;
        current.angle.y = shape_angle.y;
    
        if (field_0x1456 != 0) {
            field_0x1456--;
            shape_angle.z += field_0x1454 * cM_fsin(0.20943952f * field_0x1456);
        }

        if (speedF >= 0.0f) {
            var_r27 = 6.0f * (s16)(shape_angle.y - sp12);
        } else {
            var_r27 = 6.0f * (s16)(sp12 - shape_angle.y);
        }
    
        var_r27 -= shape_angle.z;
        if (var_r27 > 0x2000) {
            var_r27 = 0x2000;
        } else if (var_r27 < -0x2000) {
            var_r27 = -0x2000;
        }
    
        var_r27 *= 2;

        s16 spC, spA;
        if (!player->checkCanoeRideOwn(this) && player->checkFishingRodEquip()) {
            spC = 25;
            spA = 30;
        } else {
            spC = 50;
            spA = 65;
        }

        field_0x1450 += spA * cM_ssin(var_r27);

        if (abs(var_r27) < 0x100 && abs(field_0x1450) < spC) {
            field_0x1450 = field_0x1450 * (1.4f + cM_rndF(1.0f));

            f32 var_f30;
            if (field_0x1450 >= 0.0f) {
                var_f30 = -1.0f;
            } else {
                var_f30 = 1.0f;
            }

            if (fabsf(speedF) < 1.0f) {
                field_0x1450 *= 0.65f;
            }

            if (!player->checkCanoeRideOwn(this) && player->checkFishingRodEquip()) {
                field_0x1450 *= 0.5f;
            }

            if (field_0x1450 == 0) {
                field_0x1450 = spC * var_f30;
            }
        }

        s16 sp8;
        if (player->checkCanoeRideOwn(this) && player->checkFishingRodEquip()) {
            sp8 = 0x40;
        } else {
            sp8 = 0x100;
        }

        if (field_0x1450 > sp8) {
            field_0x1450 = sp8;
        } else if (field_0x1450 < -sp8) {
            field_0x1450 = -sp8;
        }

        shape_angle.z += field_0x1450;
        field_0x1452 = field_0x1452 - (40.0f * cM_ssin(field_0x145a));

        if (abs(field_0x145a) < 0x164 && abs(field_0x1452) < 5) {
            field_0x1452 = field_0x1452 * (1.4f + cM_rndF(1.0f));
            if (field_0x1452 == 0) {
                field_0x1452 = 5;
            }
        } else if (field_0x1452 == 0 && abs(field_0x145a) >= 0x164) {
            if (field_0x145a > 0) {
                field_0x1452 = -5;
            } else {
                field_0x1452 = 5;
            }
        }

        field_0x145a += field_0x1452;
        if (!player->checkCanoeRideOwn(this) || player->checkCanoeWait()) {
            mAimSpeed = 0.0f;
        }

        cLib_addCalc(&speedF, mAimSpeed, 0.5f, player->getCanoeCres(), 0.01f);

        if (speedF > player->getCanoeMaxSpeed()) {
            speedF = player->getCanoeMaxSpeed();
        } else if (speedF < -player->getCanoeBackMaxSpeed()) {
            speedF = -player->getCanoeBackMaxSpeed();
        }

        posMove();
        frontBackBgCheck();
        mAcch[0].CrrPos(dComIfG_Bgsp());

        static cXyz bodyRippleScale(2.0f, 2.0f, 2.0f);

        if (mAcch[0].ChkWaterHit()) {
            current.pos.y = mAcch[0].m_wtr.GetHeight();

            if (field_0x1444 != 0 && (!player->checkCanoeSlider() || (dComIfGp_particle_getEmitter(field_0x1488[0]) == NULL && dComIfGp_particle_getEmitter(field_0x1488[1]) == NULL))) {
                dComIfGp_particle_setWaterRipple(&field_0x1478, mAcch[0].m_wtr, &current.pos, fabsf(speedF), &tevStr, &bodyRippleScale, fopAcM_GetRoomNo(this));
            }

            for (int i = 0; i < 3; i++) {
                if (mAcchCir[0][i].ChkWallHit()) {
                    checkGomikabe(mAcchCir[0][i]);
                }
            }
        } else {
            current.pos = old.pos;
            setFrontBackPos();
        }
    }

    cXyz sp34(mFrontPos);
    cXyz sp28(mBackPos);

    if (fopAcM_wt_c::waterCheck(&sp34)) {
        sp34.y = fopAcM_wt_c::getWaterY();
    }

    if (fopAcM_wt_c::waterCheck(&sp28)) {
        sp28.y = fopAcM_wt_c::getWaterY();
    }

    sp34 -= sp28;
    field_0x1448 = sp34.atan2sY_XZ();
    shape_angle.x = field_0x1448 + field_0x145a;

    if (mIsForcePosAngle) {
        current.pos = mForcePos;
        shape_angle.y = mForceAngle;
        current.angle.y = mForceAngle;
        setFrontBackPos();
        mIsForcePosAngle = FALSE;
    }

    dCcD_Cyl* cyl;
    if (checkTandem()) {
        cyl = &mCcCyl[1];
    } else {
        cyl = &mCcCyl[3];
    }

    if ((mCcCyl[2].ChkCoHit() && mCcCyl[2].GetCoHitAc() == (fopAc_ac_c*)player) || (cyl->ChkCoHit() && cyl->GetCoHitAc() == (fopAc_ac_c*)player)) {
        mJumpRideFlg = TRUE;
    } else {
        mJumpRideFlg = FALSE;
    }

    setPaddleEffect();
    setMatrix();
    setCollision();
    setRoomInfo();
    setCanoeSliderEffect();

    if (!player->checkCanoeRideOwn(this) && speedF < 0.1f && mWaterSpeed.abs2XZ() < 0.1f && !player->checkFishingRodEquip()) {
        attention_info.flags |= fopAc_AttnFlag_ETC_e;
    } else {
        attention_info.flags &= ~fopAc_AttnFlag_ETC_e;
    }

    u32 sp1C = 127.0f * (fabsf(speedF) / player->getCanoeMaxSpeed());
    if (sp1C > 127) {
        sp1C = 127;
    }

    mSound.startLevelSound(Z2SE_OBJ_CANOE_WAVE, sp1C, -1);
    mSound.framework(0, mReverb);
    return 1;
}

static int daCanoe_Execute(daCanoe_c* i_this) {
    return i_this->execute();
}

int daCanoe_c::draw() {
    field_0x1444 = 1;

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);

    dComIfGd_setXluListBG();
    mDoExt_modelUpdateDL(mpMaskModel);
    dComIfGd_setList();

    daAlink_c* player = daAlink_getAlinkActorClass();
    if (!player->checkPlayerNoDraw() || !player->checkCanoePaddleGrab(this)) {
        if (player->checkCanoePaddleGrab(this)) {
            mpPaddleModel->setBaseTRMtx(player->getCanoePaddleMatrix());
        } else {
            mDoMtx_stack_c::copy(mpModel->getBaseTRMtx());

            f32 trans_z = 23.0f;
            if (checkTandem()) {
                trans_z += 175.0f;
            }

            mDoMtx_stack_c::transM(20.0f, 29.0f, trans_z);
            mDoMtx_stack_c::XYZrotM(cM_deg2s(-2.0f), cM_deg2s(12.0f), cM_deg2s(177.0f));
            mpPaddleModel->setBaseTRMtx(mDoMtx_stack_c::get());
        }

        g_env_light.setLightTevColorType_MAJI(mpPaddleModel, &tevStr);
        mDoExt_modelUpdateDL(mpPaddleModel);
    }

    mShadowId = dComIfGd_setShadow(mShadowId, 1, mpModel, &current.pos, 800.0f, 0.0f, current.pos.y, mAcch[0].GetGroundH(), mAcch[0].m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    dComIfGd_addRealShadow(mShadowId, mpPaddleModel);
    return 1;
}

static int daCanoe_Draw(daCanoe_c* i_this) {
    return i_this->draw();
}

static actor_method_class l_daCanoe_Method = {
    (process_method_func)daCanoe_Create,
    (process_method_func)daCanoe_Delete,
    (process_method_func)daCanoe_Execute,
    0,
    (process_method_func)daCanoe_Draw,
};

actor_process_profile_definition g_profile_CANOE = {
  fpcLy_CURRENT_e,         // mLayerID
  4,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_CANOE,              // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daCanoe_c),       // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  724,                     // mPriority
  &l_daCanoe_Method,       // sub_method
  0x00044100,              // mStatus
  fopAc_ENV_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES
template<>
JAUSectionHeap* JASGlobalInstance<JAUSectionHeap>::sInstance;
