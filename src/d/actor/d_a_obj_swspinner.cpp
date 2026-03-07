/**
 * d_a_obj_swspinner.cpp
 * Object - Spinner Switch
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_swspinner.h"
#include "d/d_procname.h"

static void* search_spinner_sub(void* i_actor, void* i_data) {
    daSpinner_c* spinner = (daSpinner_c*)i_actor;
    daObjSwSpinner_c* sw = (daObjSwSpinner_c*)i_data;

    if (spinner != NULL && fopAc_IsActor(spinner) && fpcM_GetProfName(spinner) == PROC_SPINNER) {
        if (spinner->current.pos.absXZ(sw->current.pos) < sw->GetR()) {
            if (!sw->mSpinnerIn) {
                sw->mSpinnerIn = true;
            }
            spinner->setSpinnerTag(sw->attention_info.position);
            return spinner;
        }

        if (sw->mSpinnerIn) {
            spinner->offSpinnerTag();
            sw->mSpinnerIn = false;
        }
    }

    return NULL;
}

void daObjSwSpinner_c::initBaseMtx() {
    mpModelA->setBaseScale(scale);
    mpModelB->setBaseScale(scale);
    setBaseMtx();
}

void daObjSwSpinner_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    mpModelA->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mMtx);

    MTXTrans(mDoMtx_stack_c::now, current.pos.x, current.pos.y + mPartBHeight, current.pos.z);
    mDoMtx_stack_c::YrotM(home.angle.y);
    mpModelB->setBaseTRMtx(mDoMtx_stack_c::get());

    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjSwSpinner_c::Create() {
    if (fopAcM_isSwitch(this, getSwbit2())) {
        mPartBHeight = 50.0f;
        mCanUse = false;
    } else {
        mPartBHeight = 30.0f;
        mCanUse = true;
    }

    attention_info.position.y -= 100.0f;

    initBaseMtx();
    fopAcM_SetMtx(this, mpModelA->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModelA->getModelData());

    dComIfG_Bgsp().Regist(mpBgW2, this);
    mpBgW2->Move();
    return 1;
}

static char* l_arcName = "P_Sswitch";

int daObjSwSpinner_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(0xED, modelData != NULL);
    mpModelA = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModelA == NULL) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    JUT_ASSERT(0xF8, modelData != NULL);
    mpModelB = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModelB == NULL) {
        return 0;
    }

    mpBgW2 = new dBgW();
    if (mpBgW2 == NULL || mpBgW2->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, 8), 1, &mMtx)) {
        mpBgW2 = NULL;
        return 0;
    }

    return 1;
}

int daObjSwSpinner_c::create1st() {
    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        phase = MoveBGCreate(l_arcName, 9, NULL, 0x2200, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }

    return phase;
}

int daObjSwSpinner_c::Execute(Mtx** param_0) {
    daSpinner_c* spinner = NULL;
    mPrevAngle = shape_angle.y;

    if (mCanUse) {
        spinner = (daSpinner_c*)fopAcM_Search((fopAcIt_JudgeFunc)search_spinner_sub, this);
        if (spinner != NULL) {
            if ((mSpinnerIn && spinner->checkSpinnerTagInto()) ||
                spinner->checkSpinnerTagIntoIncRot())
            {
                shape_angle.y = spinner->getAngleY();
            }
        } else {
            mSpinnerIn = false;
        }
    }

    mRotSpeedY = cLib_distanceAngleS(shape_angle.y, mPrevAngle);
    if (mRotSpeedY == 0) {
        mCount = 0;
    } else {
        mCount++;
    }

    if (fopAcM_isSwitch(this, getSwbit2())) {
        if (cLib_addCalc(&mPartBHeight, 50.0f, 0.1f, 10.0f, 2.0f) == 0.0f && mCanUse) {
            if (mSpinnerIn) {
                spinner->offSpinnerTag();
                mSpinnerIn = false;
            }

            shape_angle.y = home.angle.y;
            mCanUse = false;
        }

        if (!mPrevIsSwbit2) {
            fopAcM_seStart(this, Z2SE_OBJ_SPNR_SW_CL, 0);
        }
    } else if (mSpinnerIn) {
        if (mCount > 200) {
            if (mRotSpeedY > 3000) {
                fopAcM_onSwitch(this, getSwbit());
            } else {
                fopAcM_offSwitch(this, getSwbit());
            }
        }
    } else {
        dComIfGs_offSwitch(getSwbit() & 0xFF, fopAcM_GetHomeRoomNo(this));
    }

    if (mSpinnerIn && !mPrevSpinnerIn) {
        fopAcM_seStart(this, Z2SE_OBJ_SPNR_SW_FIT, 0);
    }

    if (mRotSpeedY != 0) {
        fopAcM_seStartLevel(this, Z2SE_OBJ_SPNR_SW_RL, mRotSpeedY);
    }

    *param_0 = &mBgMtx;
    setBaseMtx();
    mpBgW2->Move();

    attention_info.position = current.pos;
    attention_info.position.y -= 100.0f;

    mPrevIsSwbit2 = fopAcM_isSwitch(this, getSwbit2());
    mPrevSpinnerIn = mSpinnerIn;
    return 1;
}

int daObjSwSpinner_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModelA, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModelB, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModelA);
    mDoExt_modelUpdateDL(mpModelB);
    dComIfGd_setList();
    return 1;
}

int daObjSwSpinner_c::Delete() {
    if (mpBgW2 != NULL && mpBgW2->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW2);
    }

    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjSwSpinner_create1st(daObjSwSpinner_c* i_this) {
    fopAcM_ct(i_this, daObjSwSpinner_c);
    return i_this->create1st();
}

static int daObjSwSpinner_MoveBGDelete(daObjSwSpinner_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjSwSpinner_MoveBGExecute(daObjSwSpinner_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjSwSpinner_MoveBGDraw(daObjSwSpinner_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjSwSpinner_METHODS = {
    (process_method_func)daObjSwSpinner_create1st,
    (process_method_func)daObjSwSpinner_MoveBGDelete,
    (process_method_func)daObjSwSpinner_MoveBGExecute,
    (process_method_func)NULL,
    (process_method_func)daObjSwSpinner_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_SwSpinner = {
    fpcLy_CURRENT_e,           // mLayerID
    3,                         // mListID
    fpcPi_CURRENT_e,           // mListPrio
    PROC_Obj_SwSpinner,        // mProcName
    &g_fpcLf_Method.base,     // sub_method
    sizeof(daObjSwSpinner_c),  // mSize
    0,                         // mSizeOther
    0,                         // mParameters
    &g_fopAc_Method.base,      // sub_method
    559,                       // mPriority
    &daObjSwSpinner_METHODS,   // sub_method
    0x40100,                   // mStatus
    fopAc_ACTOR_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,    // cullType
};
