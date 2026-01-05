/**
 * @file d_a_obj_lv8Lift.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv8Lift.h"
#include "d/d_com_inf_game.h"
#include "d/d_path.h"
#include "d/d_bg_w.h"

daL8Lift_HIO_c::daL8Lift_HIO_c() {
    mStopDisappearTime = 30;
    mStartMoveTime = 60;
    mColorAnmFrame = 1;

    mColorR = 3;
    mColorG = 150;
    mColorB = 255;

    mLightColorR = 3;
    mLightColorG = 150;
    mLightColorB = 255;

    mLightRadius = 600.0f;
}

void daL8Lift_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(current.angle.x, current.angle.y, current.angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::now);
}

f32 const daL8Lift_c::mSpeed[16] = {
    5.0f, 6.6666665f, 8.333333f, 10.0f, 11.666667f, 13.333333f,
    15.0f, 16.666666f, 18.333334f, 20.0f, 21.666666, 23.333334,
    25.0f, 26.666666f, 28.333334f, 3.3333333f
};

int daL8Lift_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("L8Lift", 5);
    JUT_ASSERT(190, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0, 0x11000284);
    if (!mpModel) {
        return 0;
    }

    int res = mBtk.init(modelData, (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("L8Lift", 8),
        1, 0, 1.0f, 0, -1);
    JUT_ASSERT(207, res == 1);

    return 1;
}

static daL8Lift_HIO_c l_HIO;

int daL8Lift_c::create() {
    fopAcM_ct(this, daL8Lift_c);

    int phase_state = dComIfG_resLoad(&mPhase, "L8Lift");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("L8Lift", 11, dBgS_MoveBGProc_TypicalRotY, 0xd40, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        mAcchCir.SetWall(100.0f, 150.0f);
        mAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, NULL, NULL);

        mPathID = getPthID();
        dPath* mPath = dPath_GetRoomPath(mPathID, fopAcM_GetRoomNo(this));
        if (!mPath) {
            OS_REPORT("[マグネ岩空中リフト]%d番のパスが見つかりませんでした\n", mPathID);
            return cPhs_INIT_e;
        }

        dPnt* dPnt = mPath->m_points;
        current.pos = dPnt->m_position;
        mPathDirection = 1;
        mCurrentPoint = 0;
        mMoveSpeed = mSpeed[getMoveSpeed()];
        speedF = 0.0f;
        mSwbit = getSw();
        lightSet();
        mLightSet = 0;
        field_0x808 = 1;
        
        if (mSwbit == 0xff) {
            mBtk.setFrame(mBtk.getEndFrame() - 1.0f);
            mLightSet = 1;
            dKy_plight_set(&mLight);
            init_modeWaitInit();
        } else {
            mNoRideOffSwTimer = 0;
            mIsSwitch = fopAcM_isSwitch(this, mSwbit);
            if (mIsSwitch) {
                init_modeOnAnm();
            } else {
                if (mpBgW) {
                    dComIfG_Bgsp().Release(mpBgW);
                }
                field_0x808 = 0;
                init_modeMoveWait();
            }
        }

        setBaseMtx();
        mpBgW->SetRideCallback(rideCallBack);
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        
        J3DGXColor* color = mpModel->getModelData()->getMaterialNodePointer(0)->getTevKColor(1);
        color->r = l_HIO.mColorR;
        color->g = l_HIO.mColorG;
        color->b = l_HIO.mColorB;
    }

    return phase_state;
}

void daL8Lift_c::lightSet() {
    mLight.mPosition = current.pos;
    mLight.mColor.r = l_HIO.mLightColorR;
    mLight.mColor.g = l_HIO.mLightColorG;
    mLight.mColor.b = l_HIO.mLightColorB;
    mLight.mPow = l_HIO.mLightRadius;
    mLight.mFluctuation = 1.0f;
}

void daL8Lift_c::rideCallBack(dBgW* i_bgw, fopAc_ac_c* i_this, fopAc_ac_c* i_rideActor) {
    if (fopAcM_GetName(i_rideActor) == PROC_ALINK) {
        ((daL8Lift_c*)i_this)->mPlayerRide = true;
    }
}

int daL8Lift_c::Execute(f32 (**param_1)[3][4]) {
    moveLift();
    lightSet();
    *param_1 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    mPlayerRide = 0;
    return 1;
}

void daL8Lift_c::moveLift() {
    if (mPathID != 0xff) {
        typedef void (daL8Lift_c::*mode_func)();
        static mode_func mode_proc[] = {
            &daL8Lift_c::modeAcc,  &daL8Lift_c::modeMove,     &daL8Lift_c::modeBrk,
            &daL8Lift_c::modeWait, &daL8Lift_c::modeWaitInit, &daL8Lift_c::modeOnAnm, 
            &daL8Lift_c::modeMoveWait,  &daL8Lift_c::modeStop, &daL8Lift_c::modeInitSet,
            &daL8Lift_c::modeInitSet2
        };

        if (mSwbit != 0xff) {
            if (mPlayerRide != mPrevPlayerRide) {
                if (mIsSwitch && !mPlayerRide) {
                    mNoRideOffSwTimer++;
                }

                if (mPlayerRide == 1) {
                    mNoRideOffSwTimer = 0;
                }
            }

            if (mNoRideOffSwTimer && !mPlayerRide) {
                mNoRideOffSwTimer++;
                if (mNoRideOffSwTimer == 3) {
                    init_modeStop();
                    mNoRideOffSwTimer = 0;
                }
            }

            u8 switch_prev = mIsSwitch;
            mIsSwitch = fopAcM_isSwitch(this, getSw());
            if (switch_prev != mIsSwitch) {
                if (mIsSwitch) {
                    if (mMode == MODE_STOP_e) {
                        if (mpBgW) {
                            dComIfG_Bgsp().Release(mpBgW);
                        }
                    
                        init_modeInitSet();
                    } else {
                        init_modeOnAnm();
                    }
                } else {
                    init_modeStop();
                }
            }

            mPrevPlayerRide = mPlayerRide;
        }
        
        (this->*mode_proc[mMode])();
    }
}

void daL8Lift_c::modeAcc() {
    int iVar1 = cLib_chaseF(&speedF, mMoveSpeed, mMoveSpeed / 30.0f);
    if (iVar1 == 1 || current.pos == mTargetPos) {
        init_modeMove();
    }
    cLib_addCalcPos(&current.pos, mTargetPos, 1.0f, speedF, 0.1f);
    mDoAud_seStartLevel(Z2SE_OBJ_L8_L_LIFT_MV, &current.pos, fopAcM_GetSpeedF(this), dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daL8Lift_c::init_modeMove() {
    mMode = MODE_MOVE_e;
}

void daL8Lift_c::modeMove() {
    cXyz sp38 = mPrevTargetPos;
    cXyz sp44 = mTargetPos;

    f32 fVar9 = (sp38 - sp44).abs() * 0.3f;
    if (fVar9 < 30.0f) {
        fVar9 = 30.0f;
    }
    if (fVar9 > 300.0f) {
        fVar9 = 300.0f;
    }

    if (fVar9 > (current.pos - mTargetPos).abs()) {
        init_modeBrk();
    }

    cLib_addCalcPos(&current.pos, mTargetPos, 1.0f, fopAcM_GetSpeedF(this), 0.1f);
    mDoAud_seStartLevel(Z2SE_OBJ_L8_L_LIFT_MV, &current.pos, fopAcM_GetSpeedF(this), dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daL8Lift_c::init_modeBrk() {
    mMode = MODE_BRK_e;
}

void daL8Lift_c::modeBrk() {
    cXyz sp44 = current.pos;
    f32 fVar6 = cLib_addCalcPos(&current.pos, mTargetPos, 0.1f, speedF, 0.5f);
    f32 fVar7 = (sp44 - current.pos).abs();
    mDoAud_seStartLevel(Z2SE_OBJ_L8_L_LIFT_MV, &current.pos, fVar7, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (fVar6 == 0.0f) {
        fopAcM_SetSpeedF(this, 0.0f);
        init_modeWaitInit();
    }
}

void daL8Lift_c::init_modeWaitInit() {
    mMode = MODE_WAIT_INIT_e;
}

void daL8Lift_c::modeWaitInit() {
    setNextPoint();
    init_modeWait();
}

void daL8Lift_c::init_modeWait() {
    mMode = MODE_WAIT_e;
}

void daL8Lift_c::modeWait() {
    mMode = MODE_ACC_e;
}

void daL8Lift_c::init_modeMoveWait() {
    mMode = MODE_MOVE_WAIT_e;
}

void daL8Lift_c::modeMoveWait() {
    if ((mSwbit != 0xff && mIsSwitch) && mPlayerRide) {
        init_modeWaitInit();
    }
}

void daL8Lift_c::init_modeOnAnm() {
    if (mpBgW) {
        dComIfG_Bgsp().Regist(mpBgW, this);
    }

    mBtk.setPlaySpeed(1.0f);
    mDoAud_seStart(Z2SE_OBJ_L8_L_LIFT_ON, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mMode = MODE_ON_ANM_e;
}

void daL8Lift_c::modeOnAnm() {
    if (mBtk.play() == 1) {
        field_0x808 = 1;
        mLightSet = 1;
        dKy_plight_set(&mLight);
        init_modeMoveWait();
    }
}

void daL8Lift_c::init_modeStop() {
    if (mLightSet) {
        mLightSet = 0;
        dKy_plight_cut(&mLight);
    }

    mStopDisappearTimer = l_HIO.mStopDisappearTime;
    mBtk.setPlaySpeed(-1.0f);
    mMode = MODE_STOP_e;
}

void daL8Lift_c::modeStop() {
    if (mStopDisappearTimer) {
        mStopDisappearTimer--;
    } else {
        int iVar1 = mBtk.play();
        if ((mBtk.getFrame() <= mBtk.getStartFrame() - 1.0f + 20.0f) &&
            mpBgW && mpBgW->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgW);
        }

        if (iVar1 == 1) {
            if (mIsSwitch) {
                init_modeInitSet();
            } else {
                init_modeInitSet2();
            }
        }
    }
}

void daL8Lift_c::init_modeInitSet() {
    liftReset();
    mMode = MODE_INIT_SET_e;
}

void daL8Lift_c::modeInitSet() {
    init_modeOnAnm();
}

void daL8Lift_c::init_modeInitSet2() {
    liftReset();
    mMode = MODE_INIT_SET2_e;
}

void daL8Lift_c::modeInitSet2() {
    init_modeMoveWait();
}

void daL8Lift_c::liftReset() {
    dPath* mPath = dPath_GetRoomPath(mPathID, fopAcM_GetRoomNo(this));
    dPnt* mPnt = mPath->m_points;
    current.pos = mPnt->m_position;
    mPathDirection = 1;
    mCurrentPoint = 0;
}

void daL8Lift_c::setNextPoint() {
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
            mPathDirection = 0xff;
            next_point = (s8)(pathp->m_num - 2);
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

    mPrevTargetPos = mTargetPos;
    dPnt* mPnt = pathp->m_points;
    mTargetPos = mPnt[next_point].m_position;
    mCurrentPoint = next_point;
}

int daL8Lift_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    J3DModelData* modelData = mpModel->getModelData();
    J3DMaterial* mpMatNode = modelData->getMaterialNodePointer(0);
    dComIfGd_setListDarkBG();

    if (mpMatNode->getTexGenBlock()->getTexMtx(1)) {
        J3DTexMtxInfo* texMtxInfo = &mpMatNode->getTexGenBlock()->getTexMtx(1)->getTexMtxInfo();
        if (texMtxInfo) {
            Mtx mMtx58;
            C_MTXLightOrtho(mMtx58, 100.0f, -100.0f, -100.0f, 100.0f, 1.0f, 1.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::XrotS(0x4000);
            mDoMtx_stack_c::transM(-current.pos.x, -current.pos.y, -current.pos.z);
            cMtx_concat(mMtx58, mDoMtx_stack_c::get(), texMtxInfo->mEffectMtx);
        }
    }

    mBtk.entry(modelData);
    mDoExt_modelUpdateDL(mpModel);
    mBtk.remove(modelData);
    dComIfGd_setList();

    J3DGXColor* mColor = mpMatNode->getTevKColor(1);
    mColor->r = l_HIO.mColorR;
    mColor->g = l_HIO.mColorG;
    mColor->b = l_HIO.mColorB;

    return 1;
}

int daL8Lift_c::Delete() {
    dComIfG_resDelete(&mPhase, "L8Lift");
    if (mLightSet) {
        dKy_plight_cut(&mLight);
    }
    return 1;
}

static int daL8Lift_Draw(daL8Lift_c* i_this) {
    return i_this->Draw();
}

static int daL8Lift_Execute(daL8Lift_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daL8Lift_Delete(daL8Lift_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daL8Lift_Create(fopAc_ac_c* a_this) {
    daL8Lift_c* i_this = (daL8Lift_c*)a_this;
    return i_this->create();
}

static actor_method_class l_daL8Lift_Method = {
    (process_method_func)daL8Lift_Create,
    (process_method_func)daL8Lift_Delete,
    (process_method_func)daL8Lift_Execute,
    0,
    (process_method_func)daL8Lift_Draw,
};

actor_process_profile_definition g_profile_Obj_Lv8Lift = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Lv8Lift,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daL8Lift_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  645,                    // mPriority
  &l_daL8Lift_Method,     // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
