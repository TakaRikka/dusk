/**
 * @file d_a_obj_lv4SlideWall.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4SlideWall.h"

daSldWall_HIO_c::daSldWall_HIO_c() {
    move_amount = 900.0f;
    speed = 4.0f;
    vibration = 3;
}

void daSldWall_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(field_0x5b0, 0.0f, 0.0f);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daSldWall_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("L4SldWall", 4);
    JUT_ASSERT(160, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, BMD_DEFAULT_DIFF_FLAGS);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

static daSldWall_HIO_c l_HIO;

int daSldWall_c::create() {
    fopAcM_ct(this, daSldWall_c);

    int phase_state = dComIfG_resLoad(&mPhase, "L4SldWall");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("L4SldWall", 7, dBgS_MoveBGProc_TypicalRotY, 0x1300, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        mSwbit = getSw();
        mIsSwitch = fopAcM_isSwitch(this, mSwbit);
        field_0x5b0 = 0.0f;

        if (mIsSwitch) {
            field_0x5b0 = l_HIO.move_amount;
        }

        init_modeWait();
        setBaseMtx();

        #if DEBUG
        l_HIO.entryHIO("スライド壁(Lv4)");
        #endif
    }

    return phase_state;
}

int daSldWall_c::Execute(Mtx** param_0) {
    moveWall();
    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}

void daSldWall_c::moveWall() {
    static void (daSldWall_c::*mode_proc[])() = {
        &daSldWall_c::modeWait,
        &daSldWall_c::modeMoveOpen,
        &daSldWall_c::modeMoveClose,
    };

    u8 var_r30 = mIsSwitch;
    mIsSwitch = fopAcM_isSwitch(this, mSwbit);

    if (mIsSwitch != var_r30) {
        if (mIsSwitch) {
            init_modeMoveOpen();
        } else {
            init_modeMoveClose();
        }
    }

    (this->*mode_proc[mMode])();
}

void daSldWall_c::init_modeWait() {
    mMode = 0;
}

void daSldWall_c::modeWait() {}

void daSldWall_c::init_modeMoveOpen() {
    mMode = 1;
}

void daSldWall_c::modeMoveOpen() {
    mDoAud_seStartLevel(0x801b1, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    f32 var_f31 = cLib_addCalc(&field_0x5b0, l_HIO.move_amount, 0.2f, l_HIO.speed, 0.5f);
    if (var_f31 == 0.0f) {
        setSe();
        init_modeWait();
    }
}

void daSldWall_c::init_modeMoveClose() {
    mMode = 2;
}

void daSldWall_c::modeMoveClose() {
    mDoAud_seStartLevel(0x801b1, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    f32 var_f31 = cLib_addCalc(&field_0x5b0, 0.0f, 0.2f, l_HIO.speed, 0.5f);
    if (var_f31 == 0.0f) {
        setSe();
        init_modeWait();
    }
}

void daSldWall_c::setSe() {
    mDoAud_seStart(0x801b2, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    dComIfGp_getVibration().StartShock(l_HIO.vibration, 0xF, cXyz(0.0f, 1.0f, 0.0f));
}

int daSldWall_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daSldWall_c::Delete() {
    dComIfG_resDelete(&mPhase, "L4SldWall");
    
    #if DEBUG
    l_HIO.removeHIO();
    #endif
    return 1;
}

static int daSldWall_Draw(daSldWall_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daSldWall_Execute(daSldWall_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daSldWall_Delete(daSldWall_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daSldWall_Create(fopAc_ac_c* i_this) {
    return ((daSldWall_c*)i_this)->create();
}

static actor_method_class l_daSldWall_Method = {
    (process_method_func)daSldWall_Create,
    (process_method_func)daSldWall_Delete,
    (process_method_func)daSldWall_Execute,
    0,
    (process_method_func)daSldWall_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Lv4SlideWall = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Lv4SlideWall,  // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daSldWall_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  639,                    // mPriority
  &l_daSldWall_Method,    // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
