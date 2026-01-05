/**
 * d_a_obj_onsen.cpp
 * Object - Hot Spring
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_onsen.h"
#include "d/d_procname.h"

void daObjOnsen_c::initBaseMtx() {
    mpModel[0]->setBaseScale(scale);
    mpModel[1]->setBaseScale(scale);
    setBaseMtx();
}

void daObjOnsen_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    MTXCopy(mDoMtx_stack_c::now, mpModel[0]->mBaseTransformMtx);
    MTXCopy(mDoMtx_stack_c::now, mpModel[1]->mBaseTransformMtx);
    MTXCopy(mDoMtx_stack_c::now, mBgMtx);
}

int daObjOnsen_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel[0]->mBaseTransformMtx);
    fopAcM_setCullSizeBox2(this, mpModel[0]->getModelData());
    return 1;
}

static const u32 l_bmd[2] = {5, 5};

static const u32 l_btk[2] = {9, 9};

static const u32 l_bmd2[2] = {6, 6};

static const u32 l_dzb[2] = {12, 12};

static const u32 l_heap_size[2] = {10240, 6880};

static char* l_arcName[2] = {
    "H_Onsen",
    "H_KaOnsen",
};

int daObjOnsen_c::CreateHeap() {
    J3DModelData* modelData2 = (J3DModelData*)dComIfG_getObjectRes(l_arcName[mType], l_bmd[mType]);
    mpModel[0] = mDoExt_J3DModel__create(modelData2, 0x80000, 0x11000084);
    if (!mpModel[0]) {
        return 0;
    }

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName[mType], l_bmd2[mType]);
    mpModel[1] = mDoExt_J3DModel__create(modelData, 0x80000, 0x19000284);
    if (!mpModel[1]) {
        return 0;
    }

    J3DAnmTextureSRTKey* srtKey =
        (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName[mType], l_btk[mType]);
    mpBtk = new mDoExt_btkAnm();
    if (!mpBtk || !mpBtk->init(modelData, srtKey, 1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    return 1;
}

int daObjOnsen_c::create1st() {
    mType = getType();
    int phase = dComIfG_resLoad(&mPhaseReq, l_arcName[mType]);
    if (phase == cPhs_COMPLEATE_e) {
        phase = MoveBGCreate(l_arcName[mType], l_dzb[mType], NULL, l_heap_size[mType], NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }
    return phase;
}

int daObjOnsen_c::Execute(Mtx** i_mtx) {
    *i_mtx = &mBgMtx;
    mpBtk->play();
    return 1;
}

int daObjOnsen_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel[0], &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel[1], &tevStr);
    dKy_bg_MAxx_proc(mpModel[0]);
    mDoExt_modelUpdateDL(mpModel[0]);
    mpBtk->entry(mpModel[1]->getModelData());
    dComIfGd_setListInvisisble();
    dKy_bg_MAxx_proc(mpModel[1]);
    mDoExt_modelUpdateDL(mpModel[1]);
    dComIfGd_setList();
    return 1;
}

int daObjOnsen_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName[mType]);
    return 1;
}

static int daObjOnsen_create1st(daObjOnsen_c* i_this) {
    fopAcM_ct(i_this, daObjOnsen_c);
    return i_this->create1st();
}

static int daObjOnsen_MoveBGDelete(daObjOnsen_c* i_this) {
    return static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGDelete();
}

static int daObjOnsen_MoveBGExecute(daObjOnsen_c* i_this) {
    return static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGExecute();
}

static int daObjOnsen_MoveBGDraw(daObjOnsen_c* i_this) {
    return static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGDraw();
}

static actor_method_class daObjOnsen_METHODS = {
    (process_method_func)daObjOnsen_create1st,     (process_method_func)daObjOnsen_MoveBGDelete,
    (process_method_func)daObjOnsen_MoveBGExecute, (process_method_func)NULL,
    (process_method_func)daObjOnsen_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_Onsen = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Onsen,
    &g_fpcLf_Method.base,
    sizeof(daObjOnsen_c),
    0,
    0,
    &g_fopAc_Method.base,
    602,
    &daObjOnsen_METHODS,
    0x44100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
