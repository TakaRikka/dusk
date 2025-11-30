/**
 * d_a_obj_lv4PoGate.cpp
 * Arbiter's Grounds Poe Gate
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4PoGate.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

daLv4PoGate_HIO_c::daLv4PoGate_HIO_c() {
    mOpenSpeed = 10.0f;
    mCloseStep1Speed = 10.0f;
    mCloseStep2Speed = 2.0f;
    mCloseStep1Amount = 995.0f;
    mCloseStep2Amount = 980.0f;
    mCloseStep1Wait = 0;
    mCloseStep2Wait = 5;
    mCloseStep3Speed = 0.1f;
    mCloseStep3Max = 45.0f;
    mShockStrength = 3;
}

void daLv4PoGate_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, mMoveValue, 0.0f);

    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daLv4PoGate_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("L4R02Gate", 4);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daLv4PoGate_c::create() {
    fopAcM_ct(this, daLv4PoGate_c);

    int phase = dComIfG_resLoad(&mPhase, "L4R02Gate");
    if (phase == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("L4R02Gate", 7, dBgS_MoveBGProc_TypicalRotY, 0x1F00, NULL) == cPhs_ERROR_e)
        {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        mSw = getSw();
        mInitMove = fopAcM_isSwitch(this, mSw);

        mMoveTarget = 1000.0f;
        mMoveValue = 0.0f;

        if (!mInitMove) {
            mMoveValue = mMoveTarget;
        }

        init_modeWait();
        setBaseMtx();
    }

    return phase;
}

int daLv4PoGate_c::Execute(Mtx** param_0) {
    moveGate();
    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();

    return 1;
}

static daLv4PoGate_HIO_c l_HIO;

void daLv4PoGate_c::moveGate() {
    typedef void (daLv4PoGate_c::*modeFunc)();
    static modeFunc mode_proc[] = {
        &daLv4PoGate_c::modeWait,       &daLv4PoGate_c::modeMoveOpen,
        &daLv4PoGate_c::modeMoveClose,  &daLv4PoGate_c::modeMoveCloseWait,
        &daLv4PoGate_c::modeMoveClose2, &daLv4PoGate_c::modeMoveClose2Wait,
        &daLv4PoGate_c::modeMoveClose3,
    };

    u8 current_state = mInitMove;
    mInitMove = fopAcM_isSwitch(this, mSw);

    if (mInitMove != current_state) {
        if (mInitMove) {
            init_modeMoveClose();
        } else {
            init_modeMoveOpen();
        }
    }

    (this->*mode_proc[mMode])();
}

void daLv4PoGate_c::init_modeWait() {
    mMode = MODE_WAIT_e;
}

void daLv4PoGate_c::modeWait() {}

void daLv4PoGate_c::init_modeMoveOpen() {
    cXyz sound_pos(current.pos);
    sound_pos.y = home.pos.y;

    mDoAud_seStart(Z2SE_OBJ_POU_GATE_OP, &sound_pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    setEffect(0);
    mMode = MODE_MOVE_OPEN_e;
}

void daLv4PoGate_c::modeMoveOpen() {
    if (cLib_addCalc(&mMoveValue, mMoveTarget, 0.5f, l_HIO.mOpenSpeed, 0.5f) == 0.0f) {
        mDoAud_seStart(Z2SE_READ_RIDDLE_A, NULL, 0, 0);
        setSe();
        init_modeWait();
    }
}

void daLv4PoGate_c::init_modeMoveClose() {
    cXyz sound_pos(current.pos);
    sound_pos.y = home.pos.y;

    mDoAud_seStart(Z2SE_OBJ_POU_GATE_CL, &sound_pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    setEffect(1);
    mMode = MODE_MOVE_CLOSE_e;
}

void daLv4PoGate_c::modeMoveClose() {
    if (cLib_addCalc(&mMoveValue, l_HIO.mCloseStep1Amount, 0.5f, l_HIO.mCloseStep1Speed, 0.1f) ==
        0.0f)
    {
        init_modeMoveCloseWait();
    }
}

void daLv4PoGate_c::init_modeMoveCloseWait() {
    mCloseWaitTime = l_HIO.mCloseStep1Wait;
    mMode = MODE_MOVE_CLOSE_WAIT_e;
}

void daLv4PoGate_c::modeMoveCloseWait() {
    if (mCloseWaitTime != 0) {
        mCloseWaitTime--;
    } else {
        init_modeMoveClose2();
    }
}

void daLv4PoGate_c::init_modeMoveClose2() {
    mMode = MODE_MOVE_CLOSE2_e;
}

void daLv4PoGate_c::modeMoveClose2() {
    if (cLib_addCalc(&mMoveValue, l_HIO.mCloseStep2Amount, 0.5f, l_HIO.mCloseStep2Speed, 0.5f) ==
        0.0f)
    {
        init_modeMoveClose2Wait();
    }
}

void daLv4PoGate_c::init_modeMoveClose2Wait() {
    mCloseWaitTime = l_HIO.mCloseStep2Wait;
    mMode = MODE_MOVE_CLOSE2_WAIT_e;
}

void daLv4PoGate_c::modeMoveClose2Wait() {
    if (mCloseWaitTime != 0) {
        mCloseWaitTime--;
    } else {
        init_modeMoveClose3();
    }
}

void daLv4PoGate_c::init_modeMoveClose3() {
    fopAcM_SetSpeedF(this, l_HIO.mCloseStep3Speed);
    mMode = MODE_MOVE_CLOSE3_e;
}

void daLv4PoGate_c::modeMoveClose3() {
    cLib_chaseF(&speedF, l_HIO.mCloseStep3Max, l_HIO.mCloseStep3Max / 30.0f);

    if (cLib_addCalc(&mMoveValue, 0.0f, 1.0f, speedF, 1.0f) == 0.0f) {
        setSe();
        init_modeWait();
    }
}

void daLv4PoGate_c::setSe() {
    dComIfGp_getVibration().StartShock(l_HIO.mShockStrength, 15, cXyz(0.0f, 1.0f, 0.0f));
}

void daLv4PoGate_c::setEffect(int param_0) {
    static u16 const particle_id[] = {0x88C9, 0x88CA, 0x88CB, 0x88CC, 0x88CD,
                                      0x88CE, 0x88CF, 0x88D0, 0x88D1, 0x88D2};

    int start_idx;
    int num;

    if (param_0 != 0) {
        start_idx = 0;
        num = 6;
    } else {
        start_idx = 6;
        num = 10;
    }

    for (int i = start_idx; i < num; i++) {
        dComIfGp_particle_set(particle_id[i], &current.pos, &shape_angle, &scale, 255, NULL, -1,
                              NULL, NULL, NULL);
    }
}

int daLv4PoGate_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daLv4PoGate_c::Delete() {
    dComIfG_resDelete(&mPhase, "L4R02Gate");
    return 1;
}

static int daLv4PoGate_Draw(daLv4PoGate_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daLv4PoGate_Execute(daLv4PoGate_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daLv4PoGate_Delete(daLv4PoGate_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daLv4PoGate_Create(fopAc_ac_c* i_this) {
    return static_cast<daLv4PoGate_c*>(i_this)->create();
}

static actor_method_class l_daLv4PoGate_Method = {
    (process_method_func)daLv4PoGate_Create,  (process_method_func)daLv4PoGate_Delete,
    (process_method_func)daLv4PoGate_Execute, (process_method_func)NULL,
    (process_method_func)daLv4PoGate_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Lv4PoGate = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Lv4PoGate,
    &g_fpcLf_Method.base,
    sizeof(daLv4PoGate_c),
    0,
    0,
    &g_fopAc_Method.base,
    638,
    &l_daLv4PoGate_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
