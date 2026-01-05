/**
 * d_a_obj_hsTarget.cpp
 * Object - Hookshot Target
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_hsTarget.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static const char* l_resNameIdx[2] = {"hsMato", "L7HsMato"};

daHsTarget_HIO_c::daHsTarget_HIO_c() {}

void daHsTarget_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static const u32 l_bmdIdx[2] = {4, 4};

int daHsTarget_c::CreateHeap() {
    mpModel = mDoExt_J3DModel__create(
        (J3DModelData*)dComIfG_getObjectRes(l_resNameIdx[mIndex], l_bmdIdx[mIndex]),
        0x80000, 0x11000084);
    return mpModel != NULL ? 1 : 0;
}

static const u32 l_dzbIdx[2] = {7, 7};

int daHsTarget_c::create() {
    fopAcM_ct(this, daHsTarget_c);

    mIndex = getModelType();
    if (mIndex == 0x0f) {
        mIndex = 0;
    }
    int phase = dComIfG_resLoad(&mPhaseReq, l_resNameIdx[mIndex]);
    if (phase == cPhs_COMPLEATE_e) {
        if (MoveBGCreate(l_resNameIdx[mIndex], l_dzbIdx[mIndex],
                         dBgS_MoveBGProc_TypicalRotY, 0xac0, NULL) == cPhs_ERROR_e)
        {
            return cPhs_ERROR_e;
        } else {
            fopAcM_SetMtx(this, mpModel->mBaseTransformMtx);
            fopAcM_setCullSizeBox2(this, mpModel->getModelData());
            setBaseMtx();
        }
    }
    return phase;
}

int daHsTarget_c::Execute(Mtx** param_0) {
    *param_0 = &mpModel->getBaseTRMtx();
    return 1;
}

int daHsTarget_c::Draw() {
    g_env_light.settingTevStruct(0x40, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);
    return 1;
}

int daHsTarget_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_resNameIdx[mIndex]);
    return 1;
}

static int daHsTarget_Draw(daHsTarget_c* i_this) {
    return static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGDraw();
}

static int daHsTarget_Execute(daHsTarget_c* i_this) {
    return static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGExecute();
}

static int daHsTarget_Delete(daHsTarget_c* i_this) {
    return static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGDelete();
}

static int daHsTarget_Create(fopAc_ac_c* i_this) {
    return static_cast<daHsTarget_c*>(i_this)->create();
}

static daHsTarget_HIO_c l_HIO;

static actor_method_class l_daHsTarget_Method = {
    (process_method_func)daHsTarget_Create,  (process_method_func)daHsTarget_Delete,
    (process_method_func)daHsTarget_Execute, (process_method_func)NULL,
    (process_method_func)daHsTarget_Draw,
};

actor_process_profile_definition g_profile_Obj_HsTarget = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_HsTarget,
    &g_fpcLf_Method.base,
    sizeof(daHsTarget_c),
    0,
    0,
    &g_fopAc_Method.base,
    501,
    &l_daHsTarget_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
