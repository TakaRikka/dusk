/**
 * @file d_a_obj_swpush5.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_swpush5.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"
#include "Z2AudioLib/Z2Instances.h"

static const int l_dzbIdx[] = {7, 8};

static void rideCallBack(dBgW* i_bgw, fopAc_ac_c* i_bgActor, fopAc_ac_c* i_rideActor) {
    daObjSw5_c* i_this = (daObjSw5_c*)i_bgActor;
    daPy_py_c* player_p = daPy_getPlayerActorClass();

    if (i_this->checkPushable() && fopAcM_CheckStatus(i_rideActor, fopAcM_STATUS_UNK_0x400) && fopAcM_GetName(i_rideActor) == PROC_ALINK) {
        static cXyz l_push_check_pos[4] = {
            cXyz(-50.0f, 0.0f, -50.0f),
            cXyz(50.0f, 0.0f, -50.0f),
            cXyz(50.0f, 0.0f, 50.0f),
            cXyz(-50.0f, 0.0f, 50.0f),
        };

        cXyz local_rideActor_pos = i_rideActor->current.pos - i_bgActor->current.pos;
        mDoMtx_stack_c::YrotS(-i_bgActor->current.angle.y);
        mDoMtx_stack_c::multVec(&local_rideActor_pos, &local_rideActor_pos);

        if (l_push_check_pos[0].x <= local_rideActor_pos.x && l_push_check_pos[2].x >= local_rideActor_pos.x &&
            l_push_check_pos[0].z <= local_rideActor_pos.z && l_push_check_pos[2].z >= local_rideActor_pos.z)
        {
            i_this->mUnkRideTimer = 6;
            if (player_p->checkBootsOrArmorHeavy()) {
                i_this->mIsPlayerRideHvy = TRUE;
            } else {
                i_this->mIsPlayerRideHvy = FALSE;
            }

            if (fopAcM_CheckCarryType(i_rideActor, fopAcM_CARRY_TYPE_1)) {
                i_this->field_0x5ae = 2;
            } else {
                i_this->field_0x5ae = 1;
            }
        }
    }
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model_p = j3dSys.getModel();
        daObjSw5_c* i_this = (daObjSw5_c*)model_p->getUserArea();

        cMtx_copy(model_p->getAnmMtx(jnt_no), mDoMtx_stack_c::get());

        if (jnt_no == 1) {
            mDoMtx_stack_c::transM(i_this->mTopPos, 0.0f, 0.0f);
        }

        model_p->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
        mDoMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    }

    return 1;
}

void daObjSw5_c::initBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mMtx);
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjSw5_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + mTopPos, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjSw5_c::Create() {
    if (dComIfG_Bgsp().Regist(mpBgW2, this)) {
        return 0;
    }

    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());

    mpBgW->SetRideCallback(rideCallBack);

    J3DJoint* sw_jnt = mpModel->getModelData()->getJointNodePointer(1);
    JUT_ASSERT(276, sw_jnt != NULL);

    sw_jnt->setCallBack(nodeCallBack);
    mpModel->setUserArea((uintptr_t)this);

    if (fopAcM_isSwitch(this, getSwNo())) {
        mTopPos = -25.0f;
        modeWaitUpperInit();
    }

    mpBgW2->Move();
    mpBgW2->SetLock();
    return 1;
}

static char* l_arcName = "D_Hfsw00";

int daObjSw5_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(308, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, BMD_DEFAULT_DIFF_FLAGS);
    if (mpModel == NULL) {
        return 0;
    }

    mpBgW2 = new dBgW();
    if (mpBgW2 == NULL || mpBgW2->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, l_dzbIdx[0]), 1, &mMtx)) {
        mpBgW2 = NULL;
        return 0;
    }

    return 1;
}

int daObjSw5_c::create() {
    fopAcM_ct(this, daObjSw5_c);

    int phase_state = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = MoveBGCreate(l_arcName, l_dzbIdx[1], NULL, 0x1060, NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }
    }

    return phase_state;
}

int daObjSw5_c::Execute(Mtx** param_0) {
    setting_ride_flag();
    mCounter++;

    event_proc_call();
    mode_proc_call();
    calc_top_pos();

    mpBgW2->Move();

    *param_0 = &mBgMtx;
    setBaseMtx();

    field_0x5af = field_0x5ae;
    field_0x5ad = mUnkRideTimer;
    field_0x5ae = 0;
    mIsPlayerRideHvyPrev = mIsPlayerRideHvy;
    mIsPlayerRideHvy = FALSE;
    return 1;
}

void daObjSw5_c::setting_ride_flag() {
    if (mUnkRideTimer != 0) {
        mUnkRideTimer--;
    }

    if (field_0x5ae != 0) {
        if (field_0x5b1 != 0) {
            field_0x5b0 = 8;
        } else {
            field_0x5b0++;
            if (field_0x5b0 > 8) {
                field_0x5b1 = 1;
            }
        }

        if (mIsPlayerRideHvyPrev != mIsPlayerRideHvy) {
            field_0x5b1 = 0;
            field_0x5b0 = 0;
        }
    } else if (field_0x5b1 != 0) {
        field_0x5b0--;
        if (field_0x5b0 == 0) {
            field_0x5b1 = 0;
        }
    } else {
        field_0x5b0 = 0;
    }
}

// getSwNo2 always returns 0xFF, so this function always returns true
BOOL daObjSw5_c::checkPushable() {
    u8 swbit2 = getSwNo2();
    if (swbit2 == 0xFF) {
        return true;
    }

    if (swbit2 != 0xFF && fopAcM_isSwitch(this, swbit2)) {
        return true;
    }

    return false;
}

void daObjSw5_c::event_proc_call() {
    static void (daObjSw5_c::*l_func[])() = {
        &daObjSw5_c::actionPauseNone,
        &daObjSw5_c::actionPauseOrder,
        &daObjSw5_c::actionPause,
        &daObjSw5_c::actionWait,
        &daObjSw5_c::actionOrder,
        &daObjSw5_c::actionEvent,
    };

    (this->*l_func[mAction])();
}

void daObjSw5_c::actionPauseNoneInit() {
    mAction = 0;
}

void daObjSw5_c::actionPauseNone() {}

void daObjSw5_c::actionPauseOrderInit() {
    if (mAction != 1) {
        mAction = 1;
        fopAcM_orderPotentialEvent(this, 2, 0, 0);
        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
    }
}

void daObjSw5_c::actionPauseOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        actionPauseInit();
    } else {
        actionPauseNoneInit();
    }
}

void daObjSw5_c::actionPauseInit() {
    daPy_getPlayerActorClass()->onWolfEyeKeep();
    mPauseTimer = 10;
    mAction = 2;
}

void daObjSw5_c::actionPause() {
    daPy_getPlayerActorClass()->onWolfEyeKeep();

    mPauseTimer--;
    if (mPauseTimer == 0) {
        dComIfGp_event_reset();
        actionPauseNoneInit();
    }
}

void daObjSw5_c::actionWait() {}

void daObjSw5_c::actionOrder() {}

void daObjSw5_c::actionEvent() {}

void daObjSw5_c::calc_top_pos() {
    speed.y -= (field_0x5f8 - field_0x5fc) * (0.9f + KREG_F(1));
    speed.y -= speed.y * (0.5f + KREG_F(0));
    field_0x5f8 += speed.y;

    mTopPos = -25.0f * (1.0f - field_0x5f8);
    mTopPos = cLib_minMaxLimit<f32>(mTopPos, -25.0f, 0.0f);
}

void daObjSw5_c::mode_proc_call() {
    static void (daObjSw5_c::*l_mode_func[])() = {
        &daObjSw5_c::modeWaitLower,
        &daObjSw5_c::modeLower,
        &daObjSw5_c::modeWaitUpper,
        &daObjSw5_c::modeUpper,
    };

    (this->*l_mode_func[mMode])();
}

void daObjSw5_c::modeWaitLowerInit() {
    speed.y = 0.0f;
    field_0x5f8 = 1.0f;
    field_0x5fc = 1.0f;
    mMode = 0;
}

void daObjSw5_c::modeWaitLower() {
    bool var_r30 = false;
    bool do_pause = false;
    daPy_py_c* player = daPy_getPlayerActorClass();
    
    if (field_0x5b1 != 0) {
        var_r30 = true;
    }

    if (field_0x5ae != 0 && field_0x5af == 0) {
        do_pause = true;
    }

    if (var_r30) {
        if (mIsPlayerRideHvy) {
            mPauseTimer += 30;
            modeLowerInit();
        } else {
            if (field_0x5fc == 1.0f) {
                fopAcM_seStart(this, Z2SE_OBJ_HEAVY_FUMISW_RIDE, 0);
            }
            field_0x5fc = 0.8f;
        }
    } else {
        if (field_0x5fc != 1.0f) {
            fopAcM_seStart(this, Z2SE_OBJ_HEAVY_FUMISW_DROP, 0);
        }
        field_0x5fc = 1.0f;
    }

    if (do_pause) {
        actionPauseOrderInit();
    }
}

void daObjSw5_c::modeLowerInit() {
    fopAcM_seStart(this, Z2SE_OBJ_HEAVY_FUMISW_ON, 0);
    field_0x5fc = 0.0f;
    mMode = 1;
}

void daObjSw5_c::modeLower() {
    daPy_getPlayerActorClass();
    if (-25.0f == mTopPos) {
        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        modeWaitUpperInit();

        fopAcM_onSwitch(this, getSwNo());
        OS_REPORT("ヘビー踏みスイッチ：スイッチＯＮしました<%d>\n", getSwNo());
    }
}

void daObjSw5_c::modeWaitUpperInit() {
    mMode = 2;
}

void daObjSw5_c::modeWaitUpper() {
    if (!fopAcM_isSwitch(this, getSwNo())) {
        modeUpperInit();
    }
}

void daObjSw5_c::modeUpperInit() {
    fopAcM_seStart(this, Z2SE_OBJ_HEAVY_FUMISW_OFF, 0);
    field_0x5fc = 1.0f;
    mMode = 3;
}

void daObjSw5_c::modeUpper() {
    if (mTopPos == 0.0f) {
        modeWaitLowerInit();

        fopAcM_offSwitch(this, getSwNo());
        OS_REPORT("ヘビー踏みスイッチ：スイッチＯＦＦしました<%d>\n", getSwNo());

        field_0x5b1 = 0;
        field_0x5b0 = 0;
    }
}

int daObjSw5_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjSw5_c::Delete() {
    if (mpBgW2 != NULL && mpBgW2->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW2);
    }

    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjSw5_Draw(daObjSw5_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daObjSw5_Execute(daObjSw5_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjSw5_Delete(daObjSw5_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjSw5_Create(fopAc_ac_c* i_this) {
    return ((daObjSw5_c*)i_this)->create();
}

static actor_method_class l_daObjSw5_Method = {
    (process_method_func)daObjSw5_Create,
    (process_method_func)daObjSw5_Delete,
    (process_method_func)daObjSw5_Execute,
    0,
    (process_method_func)daObjSw5_Draw,
};

actor_process_profile_definition g_profile_Obj_Swpush5 = {
  fpcLy_CURRENT_e,        // mLayerID
  2,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Swpush5,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjSw5_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  13,                     // mPriority
  &l_daObjSw5_Method,     // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES
