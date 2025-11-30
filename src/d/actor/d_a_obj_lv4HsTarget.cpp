/**
 * d_a_obj_lv4HsTarget.cpp
 * Arbiter's Grounds Clawshot Target
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4HsTarget.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

daLv4HsTarget_HIO_c::daLv4HsTarget_HIO_c() {}

void daLv4HsTarget_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);

    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daLv4HsTarget_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("L4HsMato", 4);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daLv4HsTarget_c::create() {
    fopAcM_ct(this, daLv4HsTarget_c);

    int phase = dComIfG_resLoad(&mPhase, "L4HsMato");
    if (phase == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("L4HsMato", 7, dBgS_MoveBGProc_TypicalRotY, 0x1200, NULL) == cPhs_ERROR_e)
        {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        setBaseMtx();
    }

    return phase;
}

int daLv4HsTarget_c::Execute(Mtx** param_0) {
    *param_0 = &mpModel->getBaseTRMtx();
    return 1;
}

int daLv4HsTarget_c::Draw() {
    g_env_light.settingTevStruct(0x40, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    mDoExt_modelUpdateDL(mpModel);
    return 1;
}

int daLv4HsTarget_c::Delete() {
    dComIfG_resDelete(&mPhase, "L4HsMato");
    return 1;
}

static int daLv4HsTarget_Draw(daLv4HsTarget_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daLv4HsTarget_Execute(daLv4HsTarget_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daLv4HsTarget_Delete(daLv4HsTarget_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daLv4HsTarget_Create(fopAc_ac_c* i_this) {
    return static_cast<daLv4HsTarget_c*>(i_this)->create();
}

static daLv4HsTarget_HIO_c l_HIO;

static actor_method_class l_daLv4HsTarget_Method = {
    (process_method_func)daLv4HsTarget_Create,  (process_method_func)daLv4HsTarget_Delete,
    (process_method_func)daLv4HsTarget_Execute, (process_method_func)NULL,
    (process_method_func)daLv4HsTarget_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Lv4HsTarget = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Lv4HsTarget,
    &g_fpcLf_Method.base,
    sizeof(daLv4HsTarget_c),
    0,
    0,
    &g_fopAc_Method.base,
    640,
    &l_daLv4HsTarget_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
