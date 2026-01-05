/**
 * @file d_a_obj_rotBridge.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_rotBridge.h"

static char* l_resNameIdx[3] = {
    "P_Mbridge",
    "P_Mbridge",
    "P_Mbrid9",
};

daRotBridge_HIO_c::daRotBridge_HIO_c() {
    field_0x5 = 60;
    field_0x6 = 1;
    shake_power = 3;
    vibration_range[0] = 1500.0f;
    vibration_range[1] = 1500.0f;
    vibration_range[2] = 1500.0f;
    draw_wall_collision = FALSE;
}

void daRotBridge_c::setBaseMtx() {

    if (mpBgW2 != NULL) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);

        if (home.angle.y == 0) {
            mDoMtx_stack_c::YrotM(-0x4000);
        } else {
            mDoMtx_stack_c::YrotM(-0x8000);
        }

        mpModel->setBaseScale(scale);
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        MTXCopy(mDoMtx_stack_c::get(), mMtx);
    }

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static const int l_bmdIdx[] = {
    8,
    8,
    8,
};

static const int l_dzbIdx[] = {
    12,
    12,
    11,
};

static const int l_bckIdx[] = {
    5,
    5,
    5,
};

int daRotBridge_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameIdx[mType], l_bmdIdx[mType]);
    JUT_ASSERT(216, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, BMD_DEFAULT_DIFF_FLAGS);
    if (mpModel == NULL) {
        return 0;
    }

    int res = mBck.init((J3DAnmTransform*)dComIfG_getObjectRes(l_resNameIdx[mType], l_bckIdx[mType]), 1, 0, 1.0f, 0, -1, 0);
    JUT_ASSERT(230, res == 1);

    u8 temp_r25 = (fopAcM_GetParam(this) & 0xF000) >> 0xC;
    if (mType == 1 && temp_r25 != 15) {
        mpBgW2 = new dBgW();
        if (mpBgW2 == NULL || mpBgW2->Set((cBgD_t*)dComIfG_getObjectRes(l_resNameIdx[mType], 0xB), 1, &mMtx)) {
            mpBgW2 = NULL;
            return 0;
        }
    } else {
        mpBgW2 = NULL;
    }

    return 1;
}

int daRotBridge_c::create() {
    fopAcM_ct(this, daRotBridge_c);

    mType = (fopAcM_GetParam(this) & 0xF00) >> 8;

    int phase_state = dComIfG_resLoad(&mPhase, l_resNameIdx[mType]);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate(l_resNameIdx[mType], l_dzbIdx[mType], dBgS_MoveBGProc_TypicalRotY, 0x2C80, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        bitSw = fopAcM_isSwitch(this, fopAcM_GetParam(this) & 0xFF);
        mBck.setEndFrame(mBck.getEndFrame() - 1.0f);
        mBckSpeed = 1.0f;
        mBridgeAngle = 0;

        if (bitSw == 0) {
            shape_angle.y += 0x4000;
            mBridgeAngle = 1;
        }

        mBck.setPlaySpeed(mBckSpeed);
        mpModel->calcAnmMtx();

        mpBgW->SetRideCallback(rideCallBack);
        if (mpBgW2 != NULL && mpBgW2->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgW2);
            mpBgW2->Move();
        }

        mDoMove = FALSE;
        mIsPlayerRide = FALSE;
        mIsPlayerInArea = FALSE;

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        setBaseMtx();
    }

    return phase_state;
}

void daRotBridge_c::rideCallBack(dBgW* i_bgw, fopAc_ac_c* i_bgActor, fopAc_ac_c* i_rideActor) {
    if (fopAcM_GetName(i_rideActor) == PROC_ALINK) {
        ((daRotBridge_c*)i_bgActor)->mIsPlayerRide = TRUE;
    }
}

int daRotBridge_c::Execute(Mtx** param_0) {
    u8 isSw = fopAcM_isSwitch(this, fopAcM_GetParam(this) & 0xFF);
    if (bitSw != isSw) {
        if (isSw) {
            mDoMove = TRUE;
            fopAcM_seStart(this, Z2SE_OBJ_ROLLBRG_MOVESTART, 0);

            if (mpBgW2 != NULL) {
                dComIfG_Bgsp().Regist(mpBgW2, this);
                mpBgW2->Move();
            }
        }

        bitSw = isSw;
    }

    playerAreaCheck();
    moveBridge();
    *param_0 = (Mtx*)mpModel->getAnmMtx(0);
    mIsPlayerRide = FALSE;
    return 1;
}

static daRotBridge_HIO_c l_HIO;

void daRotBridge_c::playerAreaCheck() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    cXyz sp8 = current.pos - player->current.pos;
    f32 pl_dist = sp8.absXZ();

    mIsPlayerInArea = FALSE;
    if (pl_dist <= l_HIO.vibration_range[mType]) {
        mIsPlayerInArea = TRUE;
    }
}

void daRotBridge_c::moveBridge() {
    if (mDoMove) {
        mDoMtx_MtxToRot(mpModel->getAnmMtx(0), &mRotation);
        shape_angle.y = mRotation.y;

        if (mBck.play()) {
            mBck.setPlaySpeed(mBckSpeed);
            mBck.setFrame(mBck.getStartFrame());
            mDoMove = FALSE;

            if (mpBgW2 != NULL && mpBgW2->ChkUsed()) {
                dComIfG_Bgsp().Release(mpBgW2);
                mpBgW2->Move();
            }

            if (mIsPlayerRide || mIsPlayerInArea) {
                dComIfGp_getVibration().StartShock(l_HIO.shake_power, 0xF, cXyz(0.0f, 1.0f, 0.0f));
            }

            shape_angle.y &= 0xF000;
            setBaseMtx();
            mBridgeAngle ^= 1;
        } else {
            if (mBck.getFrame() == mBck.getEndFrame() - 10.0f) {
                fopAcM_seStart(this, Z2SE_OBJ_ROLLBRG_MOVEEND, 0);
            }

            fopAcM_seStartLevel(this, Z2SE_OBJ_ROLLBRG_MOVE, 0);
        }
    }
}

int daRotBridge_c::Draw() {
#if DEBUG
    if (l_HIO.draw_wall_collision && mpBgW2 != NULL) {
        mpBgW2->CalcPlane();
    }
#endif

    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    mBck.entry(mpModel->getModelData());

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daRotBridge_c::Delete() {
    dComIfG_resDelete(&mPhase, l_resNameIdx[mType]);

    if (mpBgW2 != NULL && mpBgW2->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW2);
    }

    return 1;
}

static int daRotBridge_Draw(daRotBridge_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daRotBridge_Execute(daRotBridge_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daRotBridge_Delete(daRotBridge_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daRotBridge_Create(fopAc_ac_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    return ((daRotBridge_c*)i_this)->create();
}

static actor_method_class l_daRotBridge_Method = {
    (process_method_func)daRotBridge_Create,
    (process_method_func)daRotBridge_Delete,
    (process_method_func)daRotBridge_Execute,
    0,
    (process_method_func)daRotBridge_Draw,
};

actor_process_profile_definition g_profile_Obj_RotBridge = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_RotBridge,     // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daRotBridge_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  490,                    // mPriority
  &l_daRotBridge_Method,  // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
