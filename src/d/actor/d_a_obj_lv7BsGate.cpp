/**
 * @file d_a_obj_lv7BsGate.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv7BsGate.h"

class daLv7BsGate_HIO_c : public mDoHIO_entry_c {
public:
    daLv7BsGate_HIO_c();
    virtual ~daLv7BsGate_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ f32 open_speed;
    /* 0x08 */ f32 close_speed;
    /* 0x0C */ u8 vibration;
};

static daLv7BsGate_HIO_c l_HIO;

static char* l_type[2] = {
    "L7Gate",
    "L7GateL",
};

static int l_bmdIdx[] = {4, 4};

static int l_dzbIdx[] = {7, 7};

daLv7BsGate_HIO_c::daLv7BsGate_HIO_c() {
    open_speed = 2.0f;
    close_speed = 12.0f;
    vibration = 3;
}

void daLv7BsGate_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(field_0x5b8, 0.0f, 0.0f);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daLv7BsGate_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_type[mType], l_bmdIdx[mType]);
    JUT_ASSERT(191, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daLv7BsGate_c::create() {
    fopAcM_ct(this, daLv7BsGate_c);

    mType = getType();

    int phase_state = dComIfG_resLoad(&mPhase, l_type[mType]);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate(l_type[mType], l_dzbIdx[mType], dBgS_MoveBGProc_TypicalRotY, 0x2600, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        mIsSwitch = fopAcM_isSwitch(this, getSw());
        mLR = getLR();
        field_0x5b4 = -450.0f;

        if (mLR == 1) {
            shape_angle.y += 0x8000;
        }

        field_0x5b8 = 0.0f;

        if (mIsSwitch) {
            field_0x5b8 = field_0x5b4;
        }

        init_modeWait();
        setBaseMtx();

        #if DEBUG
        l_HIO.entryHIO("ぶら下がりスイッチゲート(Lv7)");
        #endif
    }

    return phase_state;
}

int daLv7BsGate_c::Execute(Mtx** param_0) {
    moveGate();
    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}

void daLv7BsGate_c::moveGate() {
    static void (daLv7BsGate_c::*mode_proc[])() = {
        &daLv7BsGate_c::modeWait,
        &daLv7BsGate_c::modeOpen,
        &daLv7BsGate_c::modeClose,
    };

    u8 prev_switch = mIsSwitch;
    mIsSwitch = fopAcM_isSwitch(this, getSw());
    if (mIsSwitch != prev_switch) {
        if (mIsSwitch) {
            init_modeOpen();
        } else {
            init_modeClose();
        }
    }

    (this->*mode_proc[mMode])();
}

void daLv7BsGate_c::init_modeWait() {
    mMode = 0;
}

void daLv7BsGate_c::modeWait() {}

void daLv7BsGate_c::init_modeOpen() {
    mMode = 1;
}

void daLv7BsGate_c::modeOpen() {
    if (mLR == 0) {
        mDoAud_seStartLevel(Z2SE_OBJ_SW_W_DR_OP, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    if (0.0f == cLib_addCalc(&field_0x5b8, field_0x5b4, 0.2f, l_HIO.open_speed, 0.5f)) {
        if (mLR == 0) {
            mDoAud_seStart(Z2SE_OBJ_SW_W_DR_OP_STOP, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }

        dComIfGp_getVibration().StartShock(l_HIO.vibration, 0xF, cXyz(0.0f, 1.0f, 0.0f));
        init_modeWait();
    }
}

void daLv7BsGate_c::init_modeClose() {
    mMode = 2;
}

void daLv7BsGate_c::modeClose() {
    if (mLR == 0) {
        mDoAud_seStartLevel(Z2SE_OBJ_SW_W_DR_CL, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    if (0.0f == cLib_addCalc(&field_0x5b8, 0.0f, 0.4f, l_HIO.close_speed, 0.5f)) {
        if (mLR == 0) {
            mDoAud_seStart(Z2SE_OBJ_SW_W_DR_CL_STOP, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }

        dComIfGp_getVibration().StartShock(l_HIO.vibration, 0xF, cXyz(0.0f, 1.0f, 0.0f));
        init_modeWait();
    }
}

int daLv7BsGate_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daLv7BsGate_c::Delete() {
    dComIfG_resDelete(&mPhase, l_type[mType]);
    #if DEBUG
    l_HIO.removeHIO();
    #endif
    return 1;
}

static int daLv7BsGate_Draw(daLv7BsGate_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daLv7BsGate_Execute(daLv7BsGate_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daLv7BsGate_Delete(daLv7BsGate_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daLv7BsGate_Create(fopAc_ac_c* i_this) {
    return ((daLv7BsGate_c*)i_this)->create();
}

static actor_method_class l_daLv7BsGate_Method = {
    (process_method_func)daLv7BsGate_Create,
    (process_method_func)daLv7BsGate_Delete,
    (process_method_func)daLv7BsGate_Execute,
    0,
    (process_method_func)daLv7BsGate_Draw,
};

actor_process_profile_definition g_profile_Obj_Lv7BsGate = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Lv7BsGate,     // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daLv7BsGate_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  642,                    // mPriority
  &l_daLv7BsGate_Method,  // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
