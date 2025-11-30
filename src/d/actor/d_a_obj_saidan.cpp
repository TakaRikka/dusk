//
// d_a_obj_saidan.cpp
// Object - Altar
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_saidan.h"

#include "d/d_com_inf_game.h"

static daSaidan_HIO_c l_HIO;

daSaidan_HIO_c::daSaidan_HIO_c() {
    mMaxStep = 2.0f;
    mTargetPosX = 250.0f;
}

void daSaidan_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daSaidan_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("H_Saidan", 4);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);

    if (mpModel == NULL) {
        return FALSE;
    }

    return TRUE;
}

cPhs__Step daSaidan_c::create() {
    fopAcM_ct(this, daSaidan_c);

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, "H_Saidan");
    if (step == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("H_Saidan", 7, dBgS_MoveBGProc_TypicalRotY, 0x21b0, NULL) == cPhs_ERROR_e)
        {
            return cPhs_ERROR_e;
        } else {
            fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
            fopAcM_setCullSizeBox2(this, mpModel->getModelData());
            mIsSwitch = fopAcM_isSwitch(this, fopAcM_GetParam(this) & 0xff);
            mOriginalPosX = current.pos.x;

            if (mIsSwitch != 0) {
                current.pos.x = mOriginalPosX - l_HIO.mTargetPosX;
                init_modeMoveEnd();
            } else {
                init_modeWait();
            }
            setBaseMtx();
        }
    }

    return step;
}

int daSaidan_c::Execute(Mtx** i_mtx) {
    moveProc();
    *i_mtx = &mpModel->getBaseTRMtx();
    setBaseMtx();

    return TRUE;
}

void daSaidan_c::moveProc() {
    typedef void (daSaidan_c::*proc_func)();
    static proc_func mode_proc[] = {
        &daSaidan_c::modeWait,
        &daSaidan_c::modeMove,
        &daSaidan_c::modeMoveEnd,
    };

    (this->*mode_proc[mMode])();
}

void daSaidan_c::init_modeWait() {
    mMode = MODE_WAIT_e;
}

void daSaidan_c::modeWait() {
    mIsSwitch = fopAcM_isSwitch(this, (u8)fopAcM_GetParam(this));
    if (mIsSwitch != 0) {
        init_modeMove();
    }
}

void daSaidan_c::init_modeMove() {
    mMode = MODE_MOVE_e;
}

void daSaidan_c::modeMove() {
    f32 currX =
        cLib_addCalc(&current.pos.x, mOriginalPosX - l_HIO.mTargetPosX, 0.2f, l_HIO.mMaxStep, 0.5f);

    fopAcM_seStartCurrentLevel(this, Z2SE_OBJ_CHURCH_ALTER_MOVE, 0);

    if (currX == 0.0f) {
        init_modeMoveEnd();
    }
}

void daSaidan_c::init_modeMoveEnd() {
    fopAcM_seStartCurrent(this, Z2SE_OBJ_CHURCH_ALTER_STOP, 0);

    mMode = MODE_MOVE_END_e;
}

void daSaidan_c::modeMoveEnd() {
    /* empty function */
}

int daSaidan_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    return TRUE;
}

int daSaidan_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, "H_Saidan");
    return TRUE;
}

static int daSaidan_Draw(daSaidan_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daSaidan_Execute(daSaidan_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daSaidan_Delete(daSaidan_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daSaidan_Create(fopAc_ac_c* i_this) {
    return static_cast<daSaidan_c*>(i_this)->create();
}

static actor_method_class l_daSaidan_Method = {
    (process_method_func)daSaidan_Create,  (process_method_func)daSaidan_Delete,
    (process_method_func)daSaidan_Execute, NULL,
    (process_method_func)daSaidan_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Saidan = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Saidan,         // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daSaidan_c),      // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    508,                     // mPriority
    &l_daSaidan_Method,      // sub_method
    0x40000,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
