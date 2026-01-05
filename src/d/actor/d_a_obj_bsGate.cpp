/**
 * d_a_obj_bsGate.cpp
 * Object - Lakebed Temple Gate
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_bsGate.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

daBsGate_HIO_c::daBsGate_HIO_c() {
    mOpenSpeed = 2.0f;
    mCloseSpeed = 12.0f;
    mShockStrength = 3;
}

void daBsGate_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(mOpenDist, 0.0f, 0.0f);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daBsGate_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("S_Zgate", 4);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }
    return 1;
}

cPhs__Step daBsGate_c::create() {
    fopAcM_ct(this, daBsGate_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, "S_Zgate");
    if (step == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("S_Zgate", 7, dBgS_MoveBGProc_TypicalRotY, 0xf00, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        mOpen = fopAcM_isSwitch(this, fopAcM_GetParam(this) & 0xff);
        mSide = (fopAcM_GetParam(this) >> 8) & 0xff;
        mTargetDist = -250.0f;
        if (mSide == 1) {
            shape_angle.y += 0x8000;
        }
        mOpenDist = 0.0f;
        if (mOpen) {
            mOpenDist = mTargetDist;
        }
        init_modeWait();
        setBaseMtx();
    }
    return step;
}

int daBsGate_c::Execute(Mtx** i_mtxP) {
    moveGate();
    *i_mtxP = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}

static daBsGate_HIO_c l_HIO;

void daBsGate_c::moveGate() {
    typedef void (daBsGate_c::*daBsGate_modeFunc)();
    static daBsGate_modeFunc mode_proc[3] = {
        &daBsGate_c::modeWait,
        &daBsGate_c::modeOpen,
        &daBsGate_c::modeClose,
    };
    u8 was_open = mOpen;
    mOpen = fopAcM_isSwitch(this, fopAcM_GetParam(this) & 0xff);
    if (mOpen != was_open) {
        if (mOpen) {
            init_modeOpen();
        } else {
            init_modeClose();
        }
    }
    (this->*mode_proc[mMode])();
}

void daBsGate_c::init_modeWait() {
    mMode = MODE_WAIT;
}

void daBsGate_c::modeWait() {
    /* empty function */
}

void daBsGate_c::init_modeOpen() {
    mMode = MODE_OPEN;
}

void daBsGate_c::modeOpen() {
    if (mSide == 0) {
        mDoAud_seStartLevel(Z2SE_OBJ_SW_W_DR_OP, &current.pos, 0,
                            dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
    if (cLib_addCalc(&mOpenDist, mTargetDist, 0.2f, l_HIO.mOpenSpeed, 0.5f) == 0.0f) {
        if (mSide == 0) {
            mDoAud_seStart(Z2SE_OBJ_SW_W_DR_OP_STOP, &current.pos, 0,
                           dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }
        dComIfGp_getVibration().StartShock(l_HIO.mShockStrength, 0xf, cXyz(0.0f, 1.0f, 0.0f));
        init_modeWait();
    }
}

void daBsGate_c::init_modeClose() {
    mMode = MODE_CLOSE;
}

void daBsGate_c::modeClose() {
    if (mSide == 0) {
        mDoAud_seStartLevel(Z2SE_OBJ_SW_W_DR_CL, &current.pos, 0,
                            dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
    if (cLib_addCalc(&mOpenDist, 0.0f, 0.4f, l_HIO.mCloseSpeed, 0.5f) == 0.0f) {
        if (mSide == 0) {
            mDoAud_seStart(Z2SE_OBJ_SW_W_DR_CL_STOP, &current.pos, 0,
                           dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }
        dComIfGp_getVibration().StartShock(l_HIO.mShockStrength, 0xf, cXyz(0.0f, 1.0f, 0.0f));
        init_modeWait();
    }
}

int daBsGate_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daBsGate_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, "S_Zgate");
    return 1;
}

static int daBsGate_Draw(daBsGate_c* i_this) {
    return i_this->MoveBGDraw();
}


static int daBsGate_Execute(daBsGate_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daBsGate_Delete(daBsGate_c* i_this) {
    return i_this->MoveBGDelete();
}

static cPhs__Step daBsGate_Create(fopAc_ac_c* i_this) {
    return static_cast<daBsGate_c*>(i_this)->create();
}

static actor_method_class l_daBsGate_Method = {
    (process_method_func)daBsGate_Create,
    (process_method_func)daBsGate_Delete,
    (process_method_func)daBsGate_Execute,
    (process_method_func)NULL,
    (process_method_func)daBsGate_Draw,
};

actor_process_profile_definition g_profile_Obj_BsGate = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_BsGate,
    &g_fpcLf_Method.base,
    sizeof(daBsGate_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x206,
    &l_daBsGate_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
