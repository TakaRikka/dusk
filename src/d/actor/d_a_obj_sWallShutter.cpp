/**
* @file d_a_obj_sWallShutter.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_sWallShutter.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"

static DUSK_CONST char* l_resNameIdx[DUSK_IF_ELSE(10, 2)] = {
    "P_Rgate",
    "SDGate",
#if TARGET_PC
    "losGate1",
    "SDGate",
    "los_Maze",
    "losGate5",
    "losGate1",
    "losGate2",
    "losGate3",
    "losGate4",
#endif
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

static const int l_bmdIdx[DUSK_IF_ELSE(10, 2)] = {
    4,
    4,
#if TARGET_PC
    4,
    4,
    1,
    4,
    4,
    4,
    4,
    4,
#endif
};

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

static const int l_dzbIdx[DUSK_IF_ELSE(10, 2)] = {
    7,
    7,
#if TARGET_PC
    7,
    7,
    2,
    7,
    7,
    7,
    7,
    7,
#endif
};

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
            #if TARGET_PC
            if (dusk::tphd_active()) {
                switch (getModelType()) {
                case 4:
                    current.pos.y -= -370.0f;
                    break;
                case 5:
                    current.pos.y -= -450.0f;
                    break;
                case 9:
                    current.pos.y -= -740.0f;
                    break;
                default:
                    current.pos.y += -450.0f;
                    break;
                }

                init_modeMoveDownEnd();
            } else {
                current.pos.y += -450.0f;
                init_modeMoveDownEnd();
            }
            #else
            current.pos.y += -450.0f;
            init_modeMoveDownEnd();
            #endif
        } else {
            #if TARGET_PC
            if (dusk::tphd_active() && getModelType() == 2) {
                current.pos.y += -450.0f;
            }
            #endif

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

#if TARGET_PC
    if (dusk::tphd_active()) {
        // TPHD: opening a Cave-of-Shadows shutter gate reveals the floor below
        dStage_showLOSNextFloor(fopAcM_GetRoomNo(this));
    }
#endif

    if (mModelType == TYPE_SUBDAN_e) {
        dComIfGp_particle_set(0x8C73, &current.pos, &shape_angle, NULL);
        dComIfGp_particle_set(0x8C74, &current.pos, &shape_angle, NULL);
    }
#if TARGET_PC
    else if (dusk::tphd_active() && (mModelType == 0 || mModelType == 3 || mModelType == 4 || mModelType == 5 || mModelType == 9)) {
        mShakeStrength = 0.0f;
    }
#endif
    else {
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
    #if TARGET_PC
    if (dusk::tphd_active() && mModelType == 2) {
        fopAcM_SetSpeedF(this, 13.6f);
    } else
    #endif
    {
        fopAcM_SetSpeedF(this, l_HIO.mInitSpeed);
    }

    if (mModelType == TYPE_SUBDAN_e IF_DUSK(|| (dusk::tphd_active() && (mModelType != 4 && mModelType != 5 && mModelType != 9)))) {
        dComIfGp_particle_set(0x8C77, &current.pos, &shape_angle, NULL);
    } else {
        dComIfGp_particle_set(0x870D, &current.pos, &shape_angle, NULL);
    }

    mMode = MODE_MOVE_DOWN;
}

void daSwShutter_c::modeMoveDown() {
#if TARGET_PC
    if (dusk::tphd_active() && mModelType == 2) {
        cLib_chaseF(&speedF, l_HIO.mInitSpeed, l_HIO.mAcceleration);
    } else
#endif
    {
        cLib_chaseF(&speedF, l_HIO.mMaxSpeed, l_HIO.mAcceleration);
    }

#if TARGET_PC
    f32 target_dist;
    if (dusk::tphd_active()) {
        switch (getModelType()) {
        case 4:
            target_dist = cLib_addCalc(&current.pos.y, home.pos.y - -370.0f, 1.0f, fopAcM_GetSpeedF(this), 1.0f);
            break;
        case 5:
            target_dist = cLib_addCalc(&current.pos.y, home.pos.y - -450.0f, 1.0f, fopAcM_GetSpeedF(this), 1.0f);
            break;
        case 9:
            target_dist = cLib_addCalc(&current.pos.y, home.pos.y - -740.0f, 1.0f, fopAcM_GetSpeedF(this), 1.0f);
            break;
        default:
            target_dist = cLib_addCalc(&current.pos.y, home.pos.y + -450.0f, 1.0f, fopAcM_GetSpeedF(this), 1.0f);
            break;
        }
    } else {
        target_dist = cLib_addCalc(&current.pos.y, home.pos.y + -450.0f, 1.0f, fopAcM_GetSpeedF(this), 1.0f);
    }
#else
    f32 target_dist =
        cLib_addCalc(&current.pos.y, home.pos.y + -450.0f, 1.0f, fopAcM_GetSpeedF(this), 1.0f);
#endif

    if (mModelType == TYPE_SUBDAN_e IF_DUSK(|| (dusk::tphd_active() && (mModelType != 4 && mModelType != 5 && mModelType != 9)))) {
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

static DUSK_CONST actor_method_class l_daSwShutter_Method = {
    (process_method_func)daSwShutter_Create,  (process_method_func)daSwShutter_Delete,
    (process_method_func)daSwShutter_Execute, (process_method_func)NULL,
    (process_method_func)daSwShutter_Draw,
};

DUSK_PROFILE actor_process_profile_definition DUSK_CONST g_profile_Obj_SwallShutter = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 3,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Obj_SwallShutter_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daSwShutter_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Obj_SwallShutter_e,
    /* Actor SubMtd */ &l_daSwShutter_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
