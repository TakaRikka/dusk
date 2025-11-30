/**
 * @file d_a_obj_lv6Lblock.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv6Lblock.h"

static f32 l_liftUpMax[] = {
    300.0f,
    150.0f,
};

daLv6Lblock_HIO_c::daLv6Lblock_HIO_c() {
    speed = 15.0f;
    unk_0xc = 3;
}

void daLv6Lblock_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daLv6Lblock_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("L6Lblock", 4);
    JUT_ASSERT(177, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daLv6Lblock_c::create() {
    fopAcM_ct(this, daLv6Lblock_c);

    int phase_state = dComIfG_resLoad(&mPhase, "L6Lblock");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("L6Lblock", 7, dBgS_MoveBGProc_TypicalRotY, 0xD80, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        mSwState = getSwState();
        if (mSwState == 0xF) {
            mSwState = 0;
        }

        mSwbit = getSw();
        mIsSwitch = isSwitch();

        mMaxHeight = l_liftUpMax[getUpMax()];
        current.pos.y -= 300.0f;
        mBaseHeight = current.pos.y;

        if (mIsSwitch) {
            current.pos.y = mBaseHeight + mMaxHeight;
        } else {
            init_modeWait();
        }

        setBaseMtx();
    }

    return phase_state;
}

u8 daLv6Lblock_c::isSwitch() {
    return mSwState ^ fopAcM_isSwitch(this, mSwbit);
}

int daLv6Lblock_c::Execute(Mtx** param_0) {
    moveBlock();
    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}

static daLv6Lblock_HIO_c l_HIO;

void daLv6Lblock_c::moveBlock() {
    static void (daLv6Lblock_c::*mode_proc[])() = {
        &daLv6Lblock_c::modeWait,
        &daLv6Lblock_c::modeLiftUp,
        &daLv6Lblock_c::modeLiftDown,
    };

    u8 prev_sw = mIsSwitch;
    mIsSwitch = isSwitch();

    if (mIsSwitch != prev_sw) {
        if (mIsSwitch) {
            init_modeLiftUp();
        } else {
            init_modeLiftDown();
        }
    }

    (this->*mode_proc[mMode])();
}

void daLv6Lblock_c::init_modeWait() {
    mMode = 0;
}

void daLv6Lblock_c::modeWait() {}

void daLv6Lblock_c::init_modeLiftUp() {
    mMode = 1;
}

void daLv6Lblock_c::modeLiftUp() {
    f32 dist_to_target = cLib_addCalc(&current.pos.y, mBaseHeight + mMaxHeight, 0.1f, l_HIO.speed, 5.0f);
    mDoAud_seStartLevel(Z2SE_OBJ_L6_UD_BLOCK_UP, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (0.0f == dist_to_target) {
        mDoAud_seStart(Z2SE_OBJ_L6_UD_BLOCK_UP_ST, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        init_modeWait();
    }
}

void daLv6Lblock_c::init_modeLiftDown() {
    mMode = 2;
}

void daLv6Lblock_c::modeLiftDown() {
    f32 dist_to_target = cLib_addCalc(&current.pos.y, mBaseHeight, 0.1f, l_HIO.speed, 5.0f);
    mDoAud_seStartLevel(Z2SE_OBJ_L6_UD_BLOCK_DW, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (0.0f == dist_to_target) {
        mDoAud_seStart(Z2SE_OBJ_L6_UD_BLOCK_DW_ST, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        init_modeWait();
    }
}

int daLv6Lblock_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daLv6Lblock_c::Delete() {
    dComIfG_resDelete(&mPhase, "L6Lblock");
    return 1;
}

static int daLv6Lblock_Draw(daLv6Lblock_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daLv6Lblock_Execute(daLv6Lblock_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daLv6Lblock_Delete(daLv6Lblock_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daLv6Lblock_Create(fopAc_ac_c* i_this) {
    return ((daLv6Lblock_c*)i_this)->create();
}

static actor_method_class l_daLv6Lblock_Method = {
    (process_method_func)daLv6Lblock_Create,
    (process_method_func)daLv6Lblock_Delete,
    (process_method_func)daLv6Lblock_Execute,
    (process_method_func)NULL,
    (process_method_func)daLv6Lblock_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Lv6Lblock = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Lv6Lblock,     // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daLv6Lblock_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  625,                    // mPriority
  &l_daLv6Lblock_Method,  // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
