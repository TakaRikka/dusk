/**
 * @file d_a_obj_poCandle.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_poCandle.h"

class daPoCandle_HIO_c : public mDoHIO_entry_c {
public:
    daPoCandle_HIO_c();
    virtual ~daPoCandle_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ u8 waittimer;
    /* 0x8 */ f32 firescale;
};

daPoCandle_HIO_c::daPoCandle_HIO_c() {
    waittimer = 30;
    firescale = 3.0f;
}

void daPoCandle_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(current.angle.x, current.angle.y, current.angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daPoCandle_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("P_PCNDL", 4);
    JUT_ASSERT(160, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daPoCandle_c::create() {
    fopAcM_ct(this, daPoCandle_c);

    int phase_state = dComIfG_resLoad(&mPhase, "P_PCNDL");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("P_PCNDL", 7, dBgS_MoveBGProc_TypicalRotY, 0x1600, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        setBaseMtx();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        mFirePos.x = current.pos.x;
        mFirePos.y = 25.0f + current.pos.y;
        mFirePos.z = current.pos.z;

        mLightType = getLightType();
        if (mLightType == 0xFF) {
            mLightType = 1;
        }

        lightInit();
        mSwbit = getSwBit();
        mIDNum = getID();
        mFireFlag = 0;
        mOnPlight = 0;

        mIsSwitch = fopAcM_isSwitch(this, mSwbit);
        if (mIsSwitch) {
            mOnPlight = TRUE;
        }

        init_modeWait();
    }

    return phase_state;
}

void daPoCandle_c::lightInit() {
    mLightPos = mFirePos;
    mLightPos.y += 10.0f;

    if (mLightType != 0) {
        mLight.mPosition = mLightPos;
        mLight.mColor.r = 0xBC;
        mLight.mColor.g = 0x66;
        mLight.mColor.b = 0x42;
        mLight.mPow = 500.0f;
        mLight.mFluctuation = 1.0f;
        return;
    }
    
    mPlightRefDist = 0.0f;
}

void daPoCandle_c::setLight() {
    if (mLightType != 0) {
        dKy_plight_set(&mLight);
    }
}

void daPoCandle_c::cutLight() {
    if (mLightType != 0) {
        dKy_plight_cut(&mLight);
    }
}

void daPoCandle_c::pointLightProc() {
    if (mLightType == 0) {
        GXColor light_color = {0xBC, 0x66, 0x42, 0xFF};

        if (mOnPlight) {
            cLib_addCalc(&mPlightRefDist, 1.0f, 0.5f, 0.1f, 0.0001f);
        } else {
            cLib_addCalc(&mPlightRefDist, 0.0f, 0.5f, 0.1f, 0.0001f);
        }

        if (mPlightRefDist >= 0.000001f) {
            dKy_BossLight_set(&mLightPos, &light_color, mPlightRefDist, 0);
        }
    }
}

static daPoCandle_HIO_c l_HIO;

int daPoCandle_c::Execute(Mtx** param_0) {
    static void (daPoCandle_c::*mode_proc[])() = {
        &daPoCandle_c::modeWait,
        &daPoCandle_c::modeOnFire,
        &daPoCandle_c::modeOnSwWait,
        &daPoCandle_c::modeEnd,
    };

    eventUpdate();

    if (mOnPlight) {
        cXyz effscale(l_HIO.firescale, l_HIO.firescale, l_HIO.firescale);
        mGlowEmtID = dComIfGp_particle_set(mGlowEmtID, dPa_RM(ID_ZF_S_POFIRED00_GLOW), &mFirePos, &shape_angle, &effscale);
        mFireEmtID = dComIfGp_particle_set(mFireEmtID, dPa_RM(ID_ZF_S_POFIRED01_FIRE), &mFirePos, &shape_angle, &effscale);
        mDoAud_seStartLevel(Z2SE_OBJ_FIRE_BURNING, &mFirePos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    (this->*mode_proc[mMode])();

    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    pointLightProc();
    return 1;
}

void daPoCandle_c::init_modeWait() {
    mMode = 0;
}

void daPoCandle_c::modeWait() {
    u8 prev_switch = mIsSwitch;
    mIsSwitch = fopAcM_isSwitch(this, mSwbit);

    if (prev_switch != mIsSwitch) {
        if (mIsSwitch) {
            mFireFlag = 1;
            init_modeOnFire();
            return;
        } else {
            mFireFlag = 0;
            mOnPlight = FALSE;
            cutLight();
        }
    }
}

void daPoCandle_c::init_modeOnFire() {
    mDoAud_seStart(Z2SE_OBJ_FIRE_IGNITION, &mFirePos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    cXyz effscale(l_HIO.firescale, l_HIO.firescale, l_HIO.firescale);
    dComIfGp_particle_set(dPa_RM(ID_ZF_S_POFIRED02_FIRE), &mFirePos, &shape_angle, &effscale);
    setLight();
    mOnPlight = TRUE;
    mMode = 1;
}

void daPoCandle_c::modeOnFire() {
    init_modeWait();
}

void daPoCandle_c::modeOnSwWait() {
    if (mSwWaitTimer != 0) {
        mSwWaitTimer--;
    } else {
        fopAcM_onSwitch(this, mSwbit);
        init_modeEnd();
    }
}

void daPoCandle_c::init_modeEnd() {
    mMode = 3;
}

void daPoCandle_c::modeEnd() {}

bool daPoCandle_c::eventStart() {
    init_modeOnFire();
    return true;
}

int daPoCandle_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daPoCandle_c::Delete() {
    dComIfG_resDelete(&mPhase, "P_PCNDL");
    if (mLightType != 0) {
        dKy_plight_cut(&mLight);
    }

    return 1;
}

static int daPoCandle_Draw(daPoCandle_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daPoCandle_Execute(daPoCandle_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daPoCandle_Delete(daPoCandle_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daPoCandle_Create(fopAc_ac_c* i_this) {
    return ((daPoCandle_c*)i_this)->create();
}

static actor_method_class l_daPoCandle_Method = {
    (process_method_func)daPoCandle_Create,
    (process_method_func)daPoCandle_Delete,
    (process_method_func)daPoCandle_Execute,
    0,
    (process_method_func)daPoCandle_Draw,
};

actor_process_profile_definition g_profile_Obj_poCandle = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_poCandle,      // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daPoCandle_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  650,                    // mPriority
  &l_daPoCandle_Method,   // sub_method
  0x00044000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
