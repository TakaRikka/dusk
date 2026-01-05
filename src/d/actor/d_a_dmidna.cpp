/**
 * d_a_dmidna.cpp
 * Dying Midna Actor
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_dmidna.h"
#include "f_op/f_op_actor_mng.h"

static const char l_arcName[] = "Dmidna";

int daDmidna_c::createHeap() {
    mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes(l_arcName, 8), NULL,
                                       NULL, (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 4),
                                       2, 1.0f, 0, -1, NULL, 0, 0);
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int daDmidna_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daDmidna_c*>(i_this)->createHeap();
}

int daDmidna_c::create() {
    fopAcM_ct(this, daDmidna_c);

    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, daDmidna_createHeap, 0xAEE0)) {
            return cPhs_ERROR_e;
        }

        model = mpModelMorf->getModel();
        setMatrix();
    }

    return phase;
}

static int daDmidna_Create(fopAc_ac_c* i_this) {
    return static_cast<daDmidna_c*>(i_this)->create();
}

daDmidna_c::~daDmidna_c() {
    dComIfG_resDelete(&mPhase, l_arcName);
}

static int daDmidna_Delete(daDmidna_c* i_this) {
    i_this->~daDmidna_c();
    return 1;
}

void daDmidna_c::setMatrix() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModelMorf->modelCalc();
}

int daDmidna_c::execute() {
    mpModelMorf->play(0, 0);
    setMatrix();
    return 1;
}

static int daDmidna_Execute(daDmidna_c* i_this) {
    return i_this->execute();
}

int daDmidna_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    mDoExt_modelEntryDL(model);
    return 1;
}

static int daDmidna_Draw(daDmidna_c* i_this) {
    return i_this->draw();
}

static actor_method_class l_daDmidna_Method = {
    (process_method_func)daDmidna_Create,  (process_method_func)daDmidna_Delete,
    (process_method_func)daDmidna_Execute, (process_method_func)NULL,
    (process_method_func)daDmidna_Draw,
};

actor_process_profile_definition g_profile_DMIDNA = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_DMIDNA,
    &g_fpcLf_Method.base,
    sizeof(daDmidna_c),
    0,
    0,
    &g_fopAc_Method.base,
    93,
    &l_daDmidna_Method,
    0x60000,
    fopAc_ENV_e,
    fopAc_CULLBOX_0_e,
};
