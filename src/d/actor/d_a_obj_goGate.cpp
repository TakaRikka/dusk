/**
* @file d_a_obj_goGate.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_goGate.h"
#include "d/d_com_inf_game.h"

daGoGate_HIO_c::daGoGate_HIO_c() {
    mSpeed = 2.0f;
    mShockStrength = 3;
}

void daGoGate_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(field_0x5b4, 0.0f, 0.0f);

    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daGoGate_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("P_Ggate", 4);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mpModel ? 1 : 0;
}

cPhs__Step daGoGate_c::create() {
    fopAcM_ct(this, daGoGate_c);

    cPhs__Step phase = static_cast<cPhs__Step>(dComIfG_resLoad(&mPhase, "P_Ggate"));
    if (phase == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("P_Ggate", 7, dBgS_MoveBGProc_TypicalRotY, 0x1c00, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        mSw = getSw();
        mOpen = fopAcM_isSwitch(this, mSw);
        mLR = getLR();
        float tmp;
        if (mLR == 0) {
            tmp = -250.0f;
        } else {
            tmp = 250.0f;
        }
        field_0x5b0 = tmp;
        field_0x5b4 = 0.0f;
        if (mOpen != 0) {
            field_0x5b4 = field_0x5b0;
        }
        MoveBGExecute();
        init_modeWait();
        setBaseMtx();
    }
    return phase;
}

int daGoGate_c::Execute(Mtx** param_0) {
    moveGate();
    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();

    return 1;
}

static daGoGate_HIO_c l_HIO;

void daGoGate_c::moveGate() {
    typedef void (daGoGate_c::*daGoGate_modeFunc)();
    static daGoGate_modeFunc mode_proc[] = {
        &daGoGate_c::modeWait,
        &daGoGate_c::modeMoveOpen,
        &daGoGate_c::modeMoveClose,
    };

    u8 was_open = mOpen;
    mOpen = fopAcM_isSwitch(this, mSw);
    if (mOpen != was_open) {
        if (mOpen) {
            init_modeMoveOpen();
        } else {
            init_modeMoveClose();
        }
    }

    (this->*mode_proc[mMode])();
}

void daGoGate_c::init_modeWait() {
    mMode = MODE_WAIT;
}

void daGoGate_c::modeWait() {}

void daGoGate_c::init_modeMoveOpen() {
    mMode = MODE_OPEN;
}

void daGoGate_c::modeMoveOpen() {
    mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);
    cXyz local(field_0x5b0, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&local, &local);
    local += current.pos;
    mDoAud_seStartLevel(Z2SE_OBJ_GORONGATE_MOVE, &current.pos, 0,
                        dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    if (cLib_addCalc(&field_0x5b4, field_0x5b0, 0.2f, l_HIO.mSpeed, 0.5f) == 0.0f) {
        setSe();
        init_modeWait();
    }
}

void daGoGate_c::init_modeMoveClose() {
    mMode = MODE_CLOSE;
}

void daGoGate_c::modeMoveClose() {
    mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);

    cXyz local(field_0x5b0, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&local, &local);
    local += current.pos;

    mDoAud_seStartLevel(Z2SE_OBJ_GORONGATE_MOVE, &current.pos, 0,
                        dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (cLib_addCalc(&field_0x5b4, 0.0f, 0.2f, l_HIO.mSpeed, 0.5f) == 0.0f) {
        setSe();
        init_modeWait();
    }
}

void daGoGate_c::setSe() {
    mDoAud_seStart(Z2SE_OBJ_GORONGATE_STOP, &current.pos, 0,
                   dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    dComIfGp_getVibration().StartShock(l_HIO.mShockStrength, 0xf, cXyz(0.0f, 1.0f, 0.0f));
}

int daGoGate_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daGoGate_c::Delete() {
    dComIfG_resDelete(&mPhase, "P_Ggate");
    return 1;
}

static int daGoGate_Draw(daGoGate_c* i_this) {
    return i_this->Draw();
}

static int daGoGate_Execute(daGoGate_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daGoGate_Delete(daGoGate_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daGoGate_Create(fopAc_ac_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<daGoGate_c*>(i_this)->create();
}

static actor_method_class l_daGoGate_Method = {
    (process_method_func)daGoGate_Create,  (process_method_func)daGoGate_Delete,
    (process_method_func)daGoGate_Execute, (process_method_func)NULL,
    (process_method_func)daGoGate_Draw,
};

extern actor_process_profile_definition g_profile_Obj_GoGate = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_GoGate,         // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daGoGate_c),      // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    504,                     // mPriority
    &l_daGoGate_Method,      // sub_method
    0x00040000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
