/**
 * d_a_obj_kjgjs.cpp
 * Object - Kjgjs
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kjgjs.h"
#include "d/d_bg_w.h"
#include "d/d_procname.h"

static const u32 l_dzbidx[2] = {8, 9};
static const char* l_arcName = "K_jgjs";

int daObjKJgjs_c::create1st() {
    int phase = dComIfG_resLoad(this, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        setMtx();
        field_0x60c = getType();
        if (field_0x60c < 0) {
            field_0x60c = 0;
        } else if (field_0x60c > 2) {
            field_0x60c = 1;
        }
        phase = MoveBGCreate(l_arcName, l_dzbidx[field_0x60c], dBgS_MoveBGProc_TypicalRotY, 0x47f0,
                             &mMtx[0]);

        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }
    return phase;
}

void daObjKJgjs_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::XYZrotM(current.angle.x, current.angle.y, current.angle.z);
    MTXCopy(mDoMtx_stack_c::now, mMtx[1]);
    MTXCopy(mDoMtx_stack_c::now, mMtx[0]);
}

static const u32 l_bmdidx[2] = {4, 5};

int daObjKJgjs_c::CreateHeap() {
    mpModel = mDoExt_J3DModel__create(
        (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_bmdidx[field_0x60c]), 0x80000, 0x11000084);
    return mpModel != NULL ? 1 : 0;
}

static const cull_box l_cull_box[2] = {
    {{-1260.0f, -3050.0f, -1260.0f},
    {1260.0f, 2800.0f, 1260.0f}},
    {{-76.37f, -10.83f, -78.51},
    {75.52f, 10.95f, 78.06f}},
};

int daObjKJgjs_c::Create() {
    fopAcM_SetMtx(this, mMtx[1]);
    MTXCopy(mMtx[1], mpModel->mBaseTransformMtx);
    mpBgW->OffRoofRegist();
    if (!field_0x60c) {
        fopAcM_setCullSizeBox(this, l_cull_box[0].min.x, l_cull_box[0].min.y, l_cull_box[0].min.z,
                              l_cull_box[0].max.x, l_cull_box[0].max.y, l_cull_box[0].max.z);
    } else {
        fopAcM_setCullSizeBox(this, l_cull_box[1].min.x, l_cull_box[1].min.y, l_cull_box[1].min.z,
                              l_cull_box[1].max.x, l_cull_box[1].max.y, l_cull_box[1].max.z);
    }
    return 1;
}

int daObjKJgjs_c::Execute(f32 (**i_mtx)[3][4]) {
    *i_mtx = mMtx;
    return 1;
}

int daObjKJgjs_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjKJgjs_c::Delete() {
    dComIfG_resDelete(this, l_arcName);
    return 1;
}

static int daObjKJgjs_create1st(daObjKJgjs_c* i_this) {
    fopAcM_ct(i_this, daObjKJgjs_c);
    return i_this->create1st();
}

static int daObjKJgjs_MoveBGDelete(daObjKJgjs_c* i_this) {
    return static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGDelete();
}

static int daObjKJgjs_MoveBGExecute(daObjKJgjs_c* i_this) {
    return static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGExecute();
}

static int daObjKJgjs_MoveBGDraw(daObjKJgjs_c* i_this) {
    return static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGDraw();
}

daObjKJgjs_c::~daObjKJgjs_c() {}

static actor_method_class daObjKJgjs_METHODS = {
    (process_method_func)daObjKJgjs_create1st,     (process_method_func)daObjKJgjs_MoveBGDelete,
    (process_method_func)daObjKJgjs_MoveBGExecute, (process_method_func)NULL,
    (process_method_func)daObjKJgjs_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_KJgjs = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_KJgjs,
    &g_fpcLf_Method.base,
    sizeof(daObjKJgjs_c),
    0,
    0,
    &g_fopAc_Method.base,
    677,
    &daObjKJgjs_METHODS,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
