/**
 * d_a_izumi_gate.cpp
 * Spring - Gate
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_izumi_gate.h"
#include "d/d_procname.h"

void daIzumiGate_c::initBaseMtx() {
    setBaseMtx();
}

void daIzumiGate_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    MTXCopy(mDoMtx_stack_c::now, mBgMtx);
    MTXCopy(mDoMtx_stack_c::now, mpModel->mBaseTransformMtx);
}

static int daIzumiGate_Draw(daIzumiGate_c* i_this) {
    return static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGDraw();
}

static int daIzumiGate_Execute(daIzumiGate_c* i_this) {
    return static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGExecute();
}

static int daIzumiGate_IsDelete(daIzumiGate_c* i_this) {
    return 1;
}

static int daIzumiGate_Delete(daIzumiGate_c* i_this) {
    static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGDelete();
    return 1;
}

static char* l_arcName = "M_IzmGate";

int daIzumiGate_c::create() {
    fopAcM_ct(this, daIzumiGate_c);
    int phase = dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        int objectName = dComIfG_getObjctResName2Index(l_arcName, "M_IzumiGate_b.dzb");
        phase = MoveBGCreate(l_arcName, objectName, dBgS_MoveBGProc_TypicalRotY, 0x4000, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
        fopAcM_SetMtx(this, mBgMtx);
    }
    return phase;
}

static int daIzumiGate_Create(fopAc_ac_c* i_this) {
    return static_cast<daIzumiGate_c*>(i_this)->create();
}

int daIzumiGate_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "M_IzumiGate_b.bmd");
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mpModel != NULL ? 1 : 0;
}

int daIzumiGate_c::Create() {
    initBaseMtx();
    return cPhs_COMPLEATE_e;
}

int daIzumiGate_c::Execute(Mtx** i_mtx) {
    *i_mtx = &mBgMtx;
    setBaseMtx();
    return 1;
}

int daIzumiGate_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daIzumiGate_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static actor_method_class l_daIzumiGate_Method = {
    (process_method_func)daIzumiGate_Create,  (process_method_func)daIzumiGate_Delete,
    (process_method_func)daIzumiGate_Execute, (process_method_func)daIzumiGate_IsDelete,
    (process_method_func)daIzumiGate_Draw,
};

actor_process_profile_definition g_profile_Izumi_Gate = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Izumi_Gate,
    &g_fpcLf_Method.base,
    sizeof(daIzumiGate_c),
    0,
    0,
    &g_fopAc_Method.base,
    38,
    &l_daIzumiGate_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
