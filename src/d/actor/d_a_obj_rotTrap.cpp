/**
 * @file d_a_obj_rotTrap.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_rotTrap.h"
#include "d/d_s_play.h"

daRotTrap_HIO_c::daRotTrap_HIO_c() {
    vibration = 3;
    rotate_time = 4.5f;
    field_0x8 = 10;
    AT_adjust_SW = 0;
    index = 0;
    acc = 150;
}

static daRotTrap_HIO_c l_HIO;

dCcD_SrcGObjInf const daRotTrap_c::mCcDObjInfo = {
    {0, {{AT_TYPE_800, 0x1, 0x1F}, {0x0, 0x0}, {0}}},
    {dCcD_SE_SWORD, 0, 1, 0, 0},
    {dCcD_SE_STONE, 0, 0, 0, 0},
    {0}
};

dCcD_SrcCyl daRotTrap_c::mCcDCyl = {
    mCcDObjInfo,
    {{0.0f, 0.0f, 0.0f}, 0.0f, 0.0f}
};

static Vec atPos[] = {
    {-37.0f, 130.0f, -200.0f},
    {-37.0f, 130.0f, -300.0f},
    {-37.0f, 130.0f, -400.0f},
    {-37.0f, 130.0f, -500.0f},
    {-37.0f, 130.0f, -600.0f},
    {-37.0f, 130.0f, -700.0f},
    {-37.0f, 130.0f, -800.0f},
    {37.0f, 130.0f, 200.0f},
    {37.0f, 130.0f, 300.0f},
    {37.0f, 130.0f, 400.0f},
    {37.0f, 130.0f, 500.0f},
    {37.0f, 130.0f, 600.0f},
    {37.0f, 130.0f, 700.0f},
    {37.0f, 130.0f, 800.0f},
};

static f32 atR[] = {
    50.0f,
    50.0f,
    50.0f,
    50.0f,
    50.0f,
    50.0f,
    50.0f,
    50.0f,
    50.0f,
    50.0f,
    50.0f,
    50.0f,
    50.0f,
    50.0f,
};

void daRotTrap_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);

    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daRotTrap_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("P_kama", 4);
    JUT_ASSERT(214, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, BMD_DEFAULT_DIFF_FLAGS);
    if (mpModel == NULL) {
        return 0;
    }

    mpBgW2 = new dBgW();
    if (mpBgW2 == NULL || mpBgW2->Set((cBgD_t*)dComIfG_getObjectRes("P_kama", 8), 1, &mBgMtx)) {
        mpBgW2 = NULL;
        return 0;
    }

    return 1;
}

int daRotTrap_c::create() {
    fopAcM_ct(this, daRotTrap_c);

    int phase_state = dComIfG_resLoad(&mPhase, "P_kama");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("P_kama", 7, dBgS_MoveBGProc_TypicalRotY, 0x7DF0, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        mCcStts.Init(0xFF, 0xFF, this);

        for (int i = 0; i < 14; i++) {
            mCcCyl[i].Set(mCcDCyl);
            mCcCyl[i].SetStts(&mCcStts);
        }

        mSwbit = getSw();

        if (mSwbit == 0xFF) {
            init_modeMove();
        } else if (fopAcM_isSwitch(this, mSwbit)) {
            init_modeMove();
        } else {
            init_modeWait();
        }
    
        setBaseMtx();
    }

    return phase_state;
}

int daRotTrap_c::Execute(Mtx** param_0) {
    procMain();
    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    mCcStts.Move();
    return 1;
}

void daRotTrap_c::procMain() {
    static void (daRotTrap_c::*mode_proc[])() = {
        &daRotTrap_c::modeWait,
        &daRotTrap_c::modeAcc,
        &daRotTrap_c::modeMove,
    };

    (this->*mode_proc[mMode])();

#if DEBUG
    if (l_HIO.AT_adjust_SW == TRUE) {
        shape_angle.y = 0;
    }
#endif

    for (int i = 0; i < 14; i++) {
#if DEBUG
        if (l_HIO.AT_adjust_SW == TRUE && i == l_HIO.index) {
            f32 adjust_value = 1.0f;

            if (mDoCPd_c::getHoldR(PAD_2)) {
                adjust_value = 10.0f;
            }

            if (mDoCPd_c::getTrigUp(PAD_2)) {
                atPos[i].z -= adjust_value;
            } else if (mDoCPd_c::getTrigDown(PAD_2)) {
                atPos[i].z += adjust_value;
            } else if (mDoCPd_c::getTrigLeft(PAD_2)) {
                atPos[i].x -= adjust_value;
            } else if (mDoCPd_c::getTrigRight(PAD_2)) {
                atPos[i].x += adjust_value;
            } else if (mDoCPd_c::getTrigA(PAD_2)) {
                atPos[i].y += adjust_value;
            } else if (mDoCPd_c::getTrigB(PAD_2)) {
                atPos[i].y -= adjust_value;
            } else if (mDoCPd_c::getTrigX(PAD_2)) {
                atR[i] += adjust_value;
            } else if (mDoCPd_c::getTrigY(PAD_2)) {
                atR[i] -= adjust_value;
            }

            OS_REPORT("==%d  x:%f y:%f z:%f s:%f ==\n", i, atPos[i].x, atPos[i].y, atPos[i].z, atR[i]);
        }
#endif

        mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);
        cXyz sp14;
        cXyz sp8(atPos[i]);
        mDoMtx_stack_c::multVec(&sp8, &sp14);
        sp14 += current.pos;

        mCcCyl[i].SetR(atR[i]);
        mCcCyl[i].SetH(45.0f + oREG_F(1));
        mCcCyl[i].SetC(sp14);
        dComIfG_Ccsp()->Set(&mCcCyl[i]);
    }
}

void daRotTrap_c::init_modeWait() {
    if (mpBgW != NULL) {
        dComIfG_Bgsp().Release(mpBgW);
    }

    if (mpBgW2 != NULL) {
        dComIfG_Bgsp().Regist(mpBgW2, this);
        mpBgW2->Move();
    }

    mMode = 0;
}

void daRotTrap_c::modeWait() {
    if (fopAcM_isSwitch(this, mSwbit)) {
        init_modeAcc();
    }

    if (mpBgW2 != NULL) {
        mpBgW2->Move();
    }
}

void daRotTrap_c::init_modeAcc() {
    if (mpBgW2 != NULL) {
        dComIfG_Bgsp().Release(mpBgW2);
        mpBgW2->Move();
    }

    if (mpBgW != NULL) {
        dComIfG_Bgsp().Regist(mpBgW, this);
    }

    dComIfGp_getVibration().StartShock(l_HIO.vibration, 0xF, cXyz(0.0f, 1.0f, 0.0f));
    field_0x5b0 = cM_deg2s(360.0f / (30.0f * l_HIO.rotate_time));
    field_0x5b2 = 0;
    field_0x5ae = 0;
    mMode = 1;
}

void daRotTrap_c::modeAcc() {
    mDoAud_seStartLevel(Z2SE_OBJ_RL_SWD_LV, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    BOOL temp_r30 = cLib_chaseAngleS(&field_0x5b2, field_0x5b0, field_0x5b0 / l_HIO.acc);
    shape_angle.y += field_0x5b2;
    seSet();

    if (temp_r30 == TRUE) {
        init_modeMove();
    }
}

void daRotTrap_c::init_modeMove() {
    field_0x5ae = 0;
    mMode = 2;
}

void daRotTrap_c::modeMove() {
    mDoAud_seStartLevel(Z2SE_OBJ_RL_SWD_LV, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    field_0x5b0 = cM_deg2s(360.0f / (30.0f * l_HIO.rotate_time));

    if (0.0f != l_HIO.rotate_time) {
        shape_angle.y += field_0x5b0;
    }

    seSet();
}

void daRotTrap_c::seSet() {
    if (shape_angle.y & 0x8000) {
        if (field_0x5ae == 0) {
            mDoAud_seStart(Z2SE_OBJ_RL_SWD_TRG, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            field_0x5ae = 1;
        }
    } else if (field_0x5ae != 0) {
        mDoAud_seStart(Z2SE_OBJ_RL_SWD_TRG, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        field_0x5ae = 0;
    }
}

int daRotTrap_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daRotTrap_c::Delete() {
    dComIfG_resDelete(&mPhase, "P_kama");

    if (mpBgW2 != NULL && mpBgW2->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW2);
    }

    return 1;
}

static int daRotTrap_Draw(daRotTrap_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daRotTrap_Execute(daRotTrap_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daRotTrap_Delete(daRotTrap_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daRotTrap_Create(fopAc_ac_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    return ((daRotTrap_c*)i_this)->create();
}

static actor_method_class l_daRotTrap_Method = {
    (process_method_func)daRotTrap_Create,
    (process_method_func)daRotTrap_Delete,
    (process_method_func)daRotTrap_Execute,
    (process_method_func)NULL,
    (process_method_func)daRotTrap_Draw,
};

extern actor_process_profile_definition g_profile_Obj_RotTrap = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_RotTrap,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daRotTrap_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  525,                    // mPriority
  &l_daRotTrap_Method,    // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
