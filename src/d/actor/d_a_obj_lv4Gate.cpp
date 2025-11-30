/**
 * d_a_obj_lv4Gate.cpp
 * Arbiter's Grounds Generic Gate
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4Gate.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static daLv4Gate_HIO_c l_HIO;

daLv4Gate_HIO_c::daLv4Gate_HIO_c() {
    mMoveSpeed = 2.0f;
    mShockStrength = 3;
}

void daLv4Gate_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(mMoveValue, 0.0f, 0.0f);

    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daLv4Gate_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("L4Gate", 4);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daLv4Gate_c::create() {
    fopAcM_ct(this, daLv4Gate_c);

    int phase = dComIfG_resLoad(&mPhase, "L4Gate");
    if (phase == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("L4Gate", 7, dBgS_MoveBGProc_TypicalRotY, 0x2300, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        mInitMove = fopAcM_isSwitch(this, fopAcM_GetParam(this) & 0xFF);
        mMoveType = fopAcM_GetParam(this) >> 8;

        f32 target;
        // type 0 is a right side gate, otherwise a left side gate
        if (mMoveType == 0) {
            target = -250.0f;
        } else {
            target = 250.0f;
        }

        mMoveTarget = target;
        mMoveValue = 0.0f;
        mGateOpened = false;

        if (mInitMove) {
            mMoveValue = mMoveTarget;
            mGateOpened = true;
            init_modeMoveEnd();
        } else {
            init_modeWait();
        }

        setBaseMtx();
    }

    return phase;
}

int daLv4Gate_c::Execute(Mtx** i_mtxP) {
    moveGate();
    *i_mtxP = &mpModel->getBaseTRMtx();
    setBaseMtx();

    return 1;
}

void daLv4Gate_c::moveGate() {
    typedef void (daLv4Gate_c::*modeFunc)();
    static modeFunc mode_proc[] = {&daLv4Gate_c::modeWait, &daLv4Gate_c::modeMove,
                                   &daLv4Gate_c::modeMoveEnd};

    (this->*mode_proc[mMode])();
}

void daLv4Gate_c::init_modeWait() {
    mMode = MODE_WAIT_e;
}

void daLv4Gate_c::modeWait() {
    mInitMove = fopAcM_isSwitch(this, fopAcM_GetParam(this) & 0xFF);
    if (mInitMove) {
        init_modeMove();
    }
}

void daLv4Gate_c::init_modeMove() {
    mMode = MODE_MOVE_e;
}

void daLv4Gate_c::modeMove() {
    mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);

    cXyz sound_pos(mMoveTarget, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&sound_pos, &sound_pos);
    sound_pos += current.pos;

    mDoAud_seStartLevel(Z2SE_OBJ_GORONGATE_MOVE, &sound_pos, 0,
                          dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (cLib_addCalc(&mMoveValue, mMoveTarget, 0.2f, l_HIO.mMoveSpeed, 0.5f) == 0.0f) {
        init_modeMoveEnd();
    }
}

void daLv4Gate_c::init_modeMoveEnd() {
    if (!mGateOpened) {
        mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);

        cXyz sound_pos(mMoveTarget, 0.0f, 0.0f);
        mDoMtx_stack_c::multVec(&sound_pos, &sound_pos);
        sound_pos += current.pos;

        mDoAud_seStart(Z2SE_OBJ_GORONGATE_STOP, &sound_pos, 0,
                       dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

        dComIfGp_getVibration().StartShock(l_HIO.mShockStrength, 15, cXyz(0.0f, 1.0f, 0.0f));
    }

    mMode = MODE_MOVE_END_e;
}

void daLv4Gate_c::modeMoveEnd() {}

int daLv4Gate_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daLv4Gate_c::Delete() {
    dComIfG_resDelete(&mPhase, "L4Gate");
    return 1;
}

static int daLv4Gate_Draw(daLv4Gate_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daLv4Gate_Execute(daLv4Gate_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daLv4Gate_Delete(daLv4Gate_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daLv4Gate_Create(fopAc_ac_c* i_this) {
    return static_cast<daLv4Gate_c*>(i_this)->create();
}

static actor_method_class l_daLv4Gate_Method = {
    (process_method_func)daLv4Gate_Create,  (process_method_func)daLv4Gate_Delete,
    (process_method_func)daLv4Gate_Execute, (process_method_func)NULL,
    (process_method_func)daLv4Gate_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Lv4Gate = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Lv4Gate,
    &g_fpcLf_Method.base,
    sizeof(daLv4Gate_c),
    0,
    0,
    &g_fopAc_Method.base,
    637,
    &l_daLv4Gate_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
