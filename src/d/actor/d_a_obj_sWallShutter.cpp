/**
* @file d_a_obj_sWallShutter.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_sWallShutter.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"

static char* l_resNameIdx[2] = {
    "P_Rgate",
    "SDGate",
};

daSwShutter_HIO_c::daSwShutter_HIO_c() {
    mInitSpeed = 0.0f;
    mMaxSpeed = 100.0f;
    mAcceleration = 0.2f;
    mVibrationStrength = 4;
    mShakeAmpZ = 90.0f;
    mShakeAmpY = 45.0f;
    mShakeStrength = 150.0f;
    mShakeAtten = 0.1f;
    mMaxAtten = 5.0f;
    mMinAtten = 0.1f;
}

void daSwShutter_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::ZXYrotM(mShakeRot.x, mShakeRot.y, mShakeRot.z);

    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static const int l_bmdIdx[2] = {4, 4};

int daSwShutter_c::CreateHeap() {
    J3DModelData* modelData =
        (J3DModelData*)dComIfG_getObjectRes(l_resNameIdx[mModelType], l_bmdIdx[mModelType]);
    JUT_ASSERT(199, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

static const int l_dzbIdx[2] = {7, 7};

int daSwShutter_c::create() {
    fopAcM_ct(this, daSwShutter_c);

    mModelType = getModelType();
    if (mModelType == 0xF) {
        mModelType = TYPE_ROCKWALL_e;
    }

    int phase_state = dComIfG_resLoad(&mPhase, l_resNameIdx[mModelType]);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate(l_resNameIdx[mModelType], l_dzbIdx[mModelType],
                         dBgS_MoveBGProc_TypicalRotY, 0x1000, NULL) == cPhs_ERROR_e)
        {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        mCounter = 0;
        field_0x5b0 = 0.0f;

        if (fopAcM_isSwitch(this, getSwBit())) {
            current.pos.y += -450.0f;
            init_modeMoveDownEnd();
        } else {
            mShakeRot.x = 0;
            mShakeRot.y = 0;
            mShakeRot.z = 0;

            mShakeStrength = 0.0f;
            mShakeAmpY = 0.0f;
            mShakeAmpZ = 0.0f;
            mShakeAtten = 0.0f;
            mMaxAtten = 0.0f;
            mMinAtten = 0.0f;

            init_modeWait();
        }

        setBaseMtx();
    }

    return phase_state;
}

int daSwShutter_c::Execute(Mtx** param_0) {
    moveMain();
    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}

static daSwShutter_HIO_c l_HIO;

void daSwShutter_c::moveMain() {
    typedef void (daSwShutter_c::*modeFunc)();
    static modeFunc mode_proc[] = {
        &daSwShutter_c::modeWait,
        &daSwShutter_c::modeMoveDownInit,
        &daSwShutter_c::modeMoveDown,
        &daSwShutter_c::modeMoveDownEnd,
    };

    (this->*mode_proc[mMode])();

    mShakeRot.z = mShakeStrength * cM_scos(mCounter * (s16)cM_deg2s(mShakeAmpY));
    mShakeRot.y = mShakeStrength * cM_ssin(mCounter * (s16)cM_deg2s(mShakeAmpZ));

    cLib_addCalc(&mShakeStrength, 0.0f, mShakeAtten, mMaxAtten, mMinAtten);
    mCounter++;
}

void daSwShutter_c::init_modeWait() {
    mMode = MODE_WAIT;
}

void daSwShutter_c::modeWait() {
    if (fopAcM_isSwitch(this, getSwBit())) {
        init_modeMoveDownInit();
    }
}

void daSwShutter_c::init_modeMoveDownInit() {
    mShakeStrength = l_HIO.mShakeStrength;
    mShakeAmpY = l_HIO.mShakeAmpY;
    mShakeAmpZ = l_HIO.mShakeAmpZ;
    mShakeAtten = l_HIO.mShakeAtten;
    mMaxAtten = l_HIO.mMaxAtten;
    mMinAtten = l_HIO.mMinAtten;

    if (mModelType == TYPE_SUBDAN_e) {
        dComIfGp_particle_set(0x8C73, &current.pos, &shape_angle, NULL);
        dComIfGp_particle_set(0x8C74, &current.pos, &shape_angle, NULL);
    } else {
        dComIfGp_particle_set(0x8709, &current.pos, &shape_angle, NULL);
        dComIfGp_particle_set(0x870A, &current.pos, &shape_angle, NULL);
    }

    mDoAud_seStart(Z2SE_OBJ_WALLSHUTTER_OPEN, &current.pos, 0,
                   dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    dComIfGp_getVibration().StartQuake(2, 15, cXyz(0.0f, 1.0f, 0.0f));
    mMode = MODE_MOVE_DOWN_INIT;
}

void daSwShutter_c::modeMoveDownInit() {
    if (mShakeStrength == 0.0f) {
        init_modeMoveDown();
    }
}

void daSwShutter_c::init_modeMoveDown() {
    fopAcM_SetSpeedF(this, l_HIO.mInitSpeed);

    if (mModelType == TYPE_SUBDAN_e) {
        dComIfGp_particle_set(0x8C77, &current.pos, &shape_angle, NULL);
    } else {
        dComIfGp_particle_set(0x870D, &current.pos, &shape_angle, NULL);
    }

    mMode = MODE_MOVE_DOWN;
}

void daSwShutter_c::modeMoveDown() {
    cLib_chaseF(&speedF, l_HIO.mMaxSpeed, l_HIO.mAcceleration);
    f32 target_dist =
        cLib_addCalc(&current.pos.y, home.pos.y + -450.0f, 1.0f, fopAcM_GetSpeedF(this), 1.0f);

    if (mModelType == TYPE_SUBDAN_e) {
        mEmitterID0 = dComIfGp_particle_set(mEmitterID0, 0x8C75, &current.pos, &shape_angle, NULL);
        mEmitterID1 = dComIfGp_particle_set(mEmitterID1, 0x8C76, &current.pos, &shape_angle, NULL);
    } else {
        mEmitterID0 = dComIfGp_particle_set(mEmitterID0, 0x870B, &current.pos, &shape_angle, NULL);
        mEmitterID1 = dComIfGp_particle_set(mEmitterID1, 0x870C, &current.pos, &shape_angle, NULL);
    }

    if (target_dist == 0.0f) {
        dComIfGp_getVibration().StopQuake(15);
        dComIfGp_getVibration().StartShock(l_HIO.mVibrationStrength, 15, cXyz(0.0f, 1.0f, 0.0f));
        init_modeMoveDownEnd();
    }
}

void daSwShutter_c::init_modeMoveDownEnd() {
    mMode = MODE_MOVE_DOWN_END;
}

void daSwShutter_c::modeMoveDownEnd() {}

int daSwShutter_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daSwShutter_c::Delete() {
    dComIfG_resDelete(&mPhase, l_resNameIdx[mModelType]);
    return 1;
}

static int daSwShutter_Draw(daSwShutter_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daSwShutter_Execute(daSwShutter_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daSwShutter_Delete(daSwShutter_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daSwShutter_Create(fopAc_ac_c* i_this) {
    return ((daSwShutter_c*)i_this)->create();
}

static actor_method_class l_daSwShutter_Method = {
    (process_method_func)daSwShutter_Create,  (process_method_func)daSwShutter_Delete,
    (process_method_func)daSwShutter_Execute, (process_method_func)NULL,
    (process_method_func)daSwShutter_Draw,
};

extern actor_process_profile_definition g_profile_Obj_SwallShutter = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_SwallShutter,   // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daSwShutter_c),   // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    619,                     // mPriority
    &l_daSwShutter_Method,   // sub_method
    0x00044000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
