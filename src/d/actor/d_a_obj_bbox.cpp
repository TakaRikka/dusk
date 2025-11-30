/**
 * d_a_obj_bbox.cpp
 * Object - Breakable Wooden Box
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_bbox.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

const static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x1f}, {0xd8000000, 0x11}, 0x78}},  // mObj
        {dCcD_SE_NONE, 0x1, 0x0, 0x0, 0x0},                   // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},                   // mGObjTg
        {0x0},                                                // mGObjCo
    },                                                        // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        90.0f,               // mRadius
        100.0f               // mHeight
    }                        // mCyl
};

static char* l_arcName = "M_BBox";

void daObjBBox_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjBBox_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjBBox_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->mBaseTransformMtx);
    mStts.Init(0xff, 0xff, this);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mStts);
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    return 1;
}

int daObjBBox_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mpModel != NULL ? 1 : 0;
}

int daObjBBox_c::create1st() {
    fopAcM_ct(this, daObjBBox_c);

    if (fopAcM_isSwitch(this, getSwNo())) {
        return cPhs_ERROR_e;
    }
    int phase = dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        phase = MoveBGCreate(l_arcName, 7, NULL, 0xC00, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }
    return phase;
}

int daObjBBox_c::Execute(Mtx** i_mtx) {
    static const u16 particle_id[5] = {
        0x83B0, 0x83B1, 0x83B2, 0x83B3, 0x83B4,
    };
    if (mCyl.ChkTgHit() != 0) {
        cCcD_Obj* hitObj = mCyl.GetTgHitObj();
        if (hitObj != NULL) {
            if (hitObj->ChkAtType(0xd8000000)) {
                for (int i = 0; i < 5; i++) {
                    dComIfGp_particle_set(particle_id[i], &current.pos, NULL, &scale, 0xff, NULL,
                                          -1, NULL, NULL, NULL);
                }
                fopAcM_seStart(this, Z2SE_OBJ_WOODBOX_BREAK, 0);
                fopAcM_onSwitch(this, getSwNo());
                fopAcM_delete(this);
            }
        }
    }
    *i_mtx = &mBgMtx;
    setBaseMtx();
    mCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCyl);
    return 1;
}

int daObjBBox_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjBBox_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static int daObjBBox_create1st(daObjBBox_c* i_this) {
    fopAcM_ct(i_this, daObjBBox_c);
    return i_this->create1st();
}

static int daObjBBox_MoveBGDelete(daObjBBox_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjBBox_MoveBGExecute(daObjBBox_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjBBox_MoveBGDraw(daObjBBox_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjBBox_METHODS = {
    (process_method_func)daObjBBox_create1st,
    (process_method_func)daObjBBox_MoveBGDelete,
    (process_method_func)daObjBBox_MoveBGExecute,
    (process_method_func)NULL,
    (process_method_func)daObjBBox_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_BBox = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_BBox,
    &g_fpcLf_Method.base,
    sizeof(daObjBBox_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x1F3,
    &daObjBBox_METHODS,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
