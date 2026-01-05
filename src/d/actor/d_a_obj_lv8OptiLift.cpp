/**
* @file d_a_obj_lv8OptiLift.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv8OptiLift.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_path.h"

daOptiLift_HIO_c::daOptiLift_HIO_c() {
    mStopDisappearTime = 30;
    mStartMoveTime = 30;
    mColorAnmFrame = 30;

    mColorR = 0x03;
    mColorG = 0x96;
    mColorB = 0xFF;

    mLightColorR = 0x03;
    mLightColorG = 0x96;
    mLightColorB = 0xFF;

    mLightRadius = 600.0f;
}

void daOptiLift_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(current.angle.x, current.angle.y, current.angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

const f32 daOptiLift_c::mSpeed[] = {
    5.0f,       6.6666665f, 8.333333f,  10.0f,      11.666667f, 13.333333f, 15.0f,      16.666666f,
    18.333334f, 20.0f,      21.666666f, 23.333334f, 25.0f,      26.666666f, 28.333334f, 3.3333333f,
};

int daOptiLift_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Lv8Lift", 5);
    JUT_ASSERT(0xC4, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0, 0x11000284);
    if (mpModel == NULL) {
        return 0;
    }

    int res = mBtk.init(modelData, (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Lv8Lift", 8), TRUE,
                        J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1);
    JUT_ASSERT(0xD4, res == 1);
    return 1;
}

static daOptiLift_HIO_c l_HIO;

int daOptiLift_c::create() {
    fopAcM_ct(this, daOptiLift_c);

    int phase_state = dComIfG_resLoad(&mPhase, "Lv8Lift");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("Lv8Lift", 11, dBgS_MoveBGProc_TypicalRotY, 0xD20, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        mPathID = getPthID();

        dPath* pathp = dPath_GetRoomPath(mPathID, fopAcM_GetRoomNo(this));
        if (pathp == NULL) {
            // "[OptiLift]%d Path could not be found\n"
            OS_REPORT("[光燭台リフト]%d番のパスが見つかりませんでした\n", mPathID);
            return cPhs_INIT_e;
        }

        dPnt* pointp = pathp->m_points;
        current.pos.x = pointp->m_position.x;
        current.pos.y = pointp->m_position.y;
        current.pos.z = pointp->m_position.z;

        mPathDirection = 1;
        mCurrentPoint = 0;
        mMoveSpeed = mSpeed[getMoveSpped()];
        fopAcM_SetSpeedF(this, 0.0f);

        mSwBit = getSw();

        if (mpBgW != NULL) {
            dComIfG_Bgsp().Release(mpBgW);
        }

        mNoRideOffSwTimer = 0;
        mIsSwitch = fopAcM_isSwitch(this, mSwBit);

        init_modeWait();
        field_0x616 = 0;

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        setBaseMtx();

        mpBgW->SetRideCallback(rideCallBack);

        lightSet();
        mLightSet = false;

        J3DGXColor* color = mpModel->getModelData()->getMaterialNodePointer(0)->getTevKColor(1);
        color->r = l_HIO.mColorR;
        color->g = l_HIO.mColorG;
        color->b = l_HIO.mColorB;
    }

    return phase_state;
}

void daOptiLift_c::lightSet() {
    mLight.mPosition = current.pos;
    mLight.mColor.r = l_HIO.mLightColorR;
    mLight.mColor.g = l_HIO.mLightColorG;
    mLight.mColor.b = l_HIO.mLightColorB;
    mLight.mPow = l_HIO.mLightRadius;
    mLight.mFluctuation = 1.0f;
}

void daOptiLift_c::rideCallBack(dBgW* i_bgw, fopAc_ac_c* i_this, fopAc_ac_c* i_rideActor) {
    if (fopAcM_GetName(i_rideActor) == PROC_ALINK) {
        ((daOptiLift_c*)i_this)->mPlayerRide = true;
    }
}

int daOptiLift_c::Execute(Mtx** param_0) {
    moveLift();
    lightSet();

    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();

    mPlayerRide = false;
    return 1;
}

void daOptiLift_c::moveLift() {
    if (mPathID != 0xFF) {
        typedef void (daOptiLift_c::*mode_func)();
        static mode_func mode_proc[] = {
            &daOptiLift_c::modeAcc,  &daOptiLift_c::modeMove,     &daOptiLift_c::modeBrk,
            &daOptiLift_c::modeWait, &daOptiLift_c::modeMoveInit, &daOptiLift_c::modeMoveWait,
            &daOptiLift_c::modeStop, &daOptiLift_c::modeInitSet,  &daOptiLift_c::modeInitSet2,
        };

        if (mPlayerRide != mPrevPlayerRide) {
            if (mIsSwitch && !mPlayerRide) {
                mNoRideOffSwTimer++;
            }

            if (mPlayerRide == true) {
                mNoRideOffSwTimer = 0;
            }
        }

        if (mNoRideOffSwTimer != 0 && !mPlayerRide) {
            mNoRideOffSwTimer++;

            if (mNoRideOffSwTimer == 3) {
                fopAcM_offSwitch(this, mSwBit);
                mNoRideOffSwTimer = 0;
            }
        }

        if (field_0x616 == 0) {
            u8 switch_prev = mIsSwitch;
            mIsSwitch = fopAcM_isSwitch(this, mSwBit);

            if (switch_prev != mIsSwitch) {
                if (mIsSwitch) {
                    mNoRideOffSwTimer = 0;

                    if (mMode == MODE_STOP_e) {
                        if (mpBgW != NULL) {
                            dComIfG_Bgsp().Release(mpBgW);
                        }

                        init_modeInitSet2();
                    } else {
                        init_modeMoveInit();
                    }
                } else {
                    init_modeStop();
                }
            }
        }

        mPrevPlayerRide = mPlayerRide;
        (this->*mode_proc[mMode])();
    }
}

void daOptiLift_c::init_modeAcc() {
    mMode = MODE_ACC_e;
}

void daOptiLift_c::modeAcc() {
    if (cLib_chaseF(&speedF, mMoveSpeed, mMoveSpeed / 30.0f) == 1 || current.pos == mTargetPos) {
        init_modeMove();
    }

    cLib_addCalcPos(&current.pos, mTargetPos, 1.0f, fopAcM_GetSpeedF(this), 0.1f);

    s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
    mDoAud_seStartLevel(Z2SE_OBJ_L8_L_LIFT_MV, &current.pos, fopAcM_GetSpeedF(this), reverb);
}

void daOptiLift_c::init_modeMove() {
    mMode = MODE_MOVE_e;
}

void daOptiLift_c::modeMove() {
    cXyz sp54(mPrevTargetPos);
    cXyz sp60(mTargetPos);

    f32 var_f31 = (sp54 - sp60).abs() * 0.3f;
    f32 min = 30.0f;
    f32 max = 300.0f;

    if (var_f31 < min) {
        var_f31 = min;
    }

    if (var_f31 > max) {
        var_f31 = max;
    }

    if (var_f31 > (current.pos - mTargetPos).abs()) {
        init_modeBrk();
    }

    cLib_addCalcPos(&current.pos, mTargetPos, 1.0f, fopAcM_GetSpeedF(this), 0.1f);

    s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
    mDoAud_seStartLevel(Z2SE_OBJ_L8_L_LIFT_MV, &current.pos, fopAcM_GetSpeedF(this), reverb);
}

void daOptiLift_c::init_modeBrk() {
    mMode = MODE_BRK_e;
}

void daOptiLift_c::modeBrk() {
    cXyz prev_pos(current.pos);

    f32 dist_to_target =
        cLib_addCalcPos(&current.pos, mTargetPos, 0.1f, fopAcM_GetSpeedF(this), 0.5f);

    f32 var_f30 = (prev_pos - current.pos).abs();
    s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
    mDoAud_seStartLevel(Z2SE_OBJ_L8_L_LIFT_MV, &current.pos, var_f30, reverb);

    if (dist_to_target == 0.0f) {
        fopAcM_SetSpeedF(this, 0.0f);
        setNextPoint();

        if (mCurrentPoint != -1) {
            init_modeAcc();
        } else {
            init_modeStop();
        }
    }
}

void daOptiLift_c::init_modeMoveInit() {
    if (mpBgW != NULL) {
        dComIfG_Bgsp().Regist(mpBgW, this);
    }

    mBtk.setPlaySpeed(1.0f);

    s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
    mDoAud_seStart(Z2SE_OBJ_L8_L_LIFT_ON, &current.pos, 0, reverb);

    mMode = MODE_MOVE_INIT_e;
}

void daOptiLift_c::modeMoveInit() {
    if (mBtk.play() == TRUE) {
        init_modeMoveWait();
    }
}

void daOptiLift_c::init_modeMoveWait() {
    dKy_plight_set(&mLight);
    mLightSet = true;
    mMode = MODE_MOVE_WAIT_e;
}

void daOptiLift_c::modeMoveWait() {
    if (mPlayerRide) {
        setNextPoint();

        if (mCurrentPoint != -1) {
            init_modeAcc();
        } else {
            init_modeStop();
        }
    }
}

void daOptiLift_c::init_modeWait() {
    mMode = MODE_WAIT_e;
}

void daOptiLift_c::modeWait() {}

void daOptiLift_c::init_modeStop() {
    if (getArg1() != 0xF) {
        field_0x616 = 1;
    }

    if (mLightSet) {
        dKy_plight_cut(&mLight);
        mLightSet = false;
    }

    mStopDisappearTimer = l_HIO.mStopDisappearTime;
    mBtk.setPlaySpeed(-1.0f);
    mMode = MODE_STOP_e;
}

void daOptiLift_c::modeStop() {
    if (mStopDisappearTimer != 0) {
        mStopDisappearTimer--;
    } else {
        BOOL btk_play = mBtk.play();

        f32 var_f31 = (mBtk.getStartFrame() - 1.0f) + 20.0f;
        if (mBtk.getFrame() <= var_f31 && mpBgW != NULL && mpBgW->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgW);
        }

        if (btk_play == TRUE) {
            fopAcM_offSwitch(this, mSwBit);
            mIsSwitch = fopAcM_isSwitch(this, mSwBit);
            init_modeInitSet();
        }
    }
}

void daOptiLift_c::init_modeInitSet() {
    liftReset();
    field_0x616 = 0;
    mMode = MODE_INIT_SET_e;
}

void daOptiLift_c::modeInitSet() {
    init_modeWait();
}

void daOptiLift_c::init_modeInitSet2() {
    liftReset();
    mMode = MODE_INIT_SET2_e;
}

void daOptiLift_c::modeInitSet2() {
    init_modeMoveInit();
}

void daOptiLift_c::liftReset() {
    dPath* pathp = dPath_GetRoomPath(mPathID, fopAcM_GetRoomNo(this));
    dPnt* pointp = pathp->m_points;

    current.pos.x = pointp->m_position.x;
    current.pos.y = pointp->m_position.y;
    current.pos.z = pointp->m_position.z;

    mPathDirection = 1;
    mCurrentPoint = 0;
}

void daOptiLift_c::setNextPoint() {
    s16 next_point = mCurrentPoint + mPathDirection;
    dPath* pathp = dPath_GetRoomPath(mPathID, fopAcM_GetRoomNo(this));

    if (dPath_ChkClose(pathp)) {
        if (next_point > pathp->m_num - 1) {
            next_point = 0;
        } else if (next_point < 0) {
            next_point = (s8)(pathp->m_num - 1);
        }
    } else {
        if (next_point > pathp->m_num - 1) {
            mPathDirection = -1;
            next_point = -1;
        } else if (next_point < 0) {
            mPathDirection = 1;
            next_point = 1;
        } else {
            dPnt* pointp = &pathp->m_points[mCurrentPoint];
            if (pointp->mArg0 == 0) {
                next_point = mCurrentPoint;
            }
        }
    }

    if (next_point != -1) {
        mPrevTargetPos = mTargetPos;

        dPnt* pointp = &pathp->m_points[next_point];
        mTargetPos.x = pointp->m_position.x;
        mTargetPos.y = pointp->m_position.y;
        mTargetPos.z = pointp->m_position.z;
    }

    mCurrentPoint = next_point;
}

int daOptiLift_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    J3DModelData* modelData = mpModel->getModelData();
    J3DMaterial* materialp = modelData->getMaterialNodePointer(0);

    dComIfGd_setListDarkBG();

    if (materialp->getTexGenBlock()->getTexMtx(1) != NULL) {
        J3DTexMtxInfo* mtx_info = &materialp->getTexGenBlock()->getTexMtx(1)->getTexMtxInfo();
        if (mtx_info != NULL) {
            Mtx m;
            C_MTXLightOrtho(m, 100.0f, -100.0f, -100.0f, 100.0f, 1.0f, 1.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::XrotS(0x4000);
            mDoMtx_stack_c::transM(-current.pos.x, -current.pos.y, -current.pos.z);
            cMtx_concat(m, mDoMtx_stack_c::get(), mtx_info->mEffectMtx);
        }
    }

    mBtk.entry(modelData);
    mDoExt_modelUpdateDL(mpModel);
    mBtk.remove(modelData);

    dComIfGd_setList();

    J3DGXColor* color = materialp->getTevKColor(1);
    color->r = l_HIO.mColorR;
    color->g = l_HIO.mColorG;
    color->b = l_HIO.mColorB;
    return 1;
}

int daOptiLift_c::Delete() {
    dComIfG_resDelete(&mPhase, "Lv8Lift");
    if (mLightSet) {
        dKy_plight_cut(&mLight);
    }

    return 1;
}

static int daOptiLift_Draw(daOptiLift_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daOptiLift_Execute(daOptiLift_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daOptiLift_Delete(daOptiLift_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daOptiLift_Create(fopAc_ac_c* i_this) {
    return ((daOptiLift_c*)i_this)->create();
}

static actor_method_class l_daOptiLift_Method = {
    (process_method_func)daOptiLift_Create,  (process_method_func)daOptiLift_Delete,
    (process_method_func)daOptiLift_Execute, (process_method_func)NULL,
    (process_method_func)daOptiLift_Draw,
};

actor_process_profile_definition g_profile_Obj_Lv8OptiLift = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Lv8OptiLift,    // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daOptiLift_c),    // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    643,                     // mPriority
    &l_daOptiLift_Method,    // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
