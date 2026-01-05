/**
 * d_a_obj_wsword.cpp
 * Object - Wooden Sword
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_wsword.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

static char* l_arcName = "Obj_brksw";

static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x19}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},          // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},          // mGObjTg
        {0x0},                                       // mGObjCo
    },                                               // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            50.0f,               // mRadius
            150.0f               // mHeight
        }                        // mCyl
    }
};

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjWSword_c*>(i_this)->CreateHeap();
}

void daObjWSword_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjWSword_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daObjWSword_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    mStts.Init(0xff, 0xff, this);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mStts);
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    return 1;
}

int daObjWSword_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 3);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mpModel != NULL ? 1 : 0;
}

int daObjWSword_c::create() {
    fopAcM_ct(this, daObjWSword_c);
    int phase = dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, (heapCallbackFunc)CheckCreateHeap, 0x840)) {
            return cPhs_ERROR_e;
        } else if (!Create()) {
            return cPhs_ERROR_e;
        }
    }
    return phase;
}

int daObjWSword_c::execute() {
    setBaseMtx();
    mCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCyl);
    return 1;
}

int daObjWSword_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);
    return 1;
}

int daObjWSword_c::_delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static int daObjWSword_Draw(daObjWSword_c* i_this) {
    return i_this->draw();
}

static int daObjWSword_Execute(daObjWSword_c* i_this) {
    return i_this->execute();
}

static int daObjWSword_Delete(daObjWSword_c* i_this) {
    return i_this->_delete();
}

static int daObjWSword_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjWSword_c*>(i_this)->create();
}

static actor_method_class l_daObjWSword_Method = {
    (process_method_func)daObjWSword_Create,
    (process_method_func)daObjWSword_Delete,
    (process_method_func)daObjWSword_Execute,
    (process_method_func)NULL,
    (process_method_func)daObjWSword_Draw,
};

actor_process_profile_definition g_profile_Obj_WoodenSword = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Obj_WoodenSword,
    &g_fpcLf_Method.base,
    sizeof(daObjWSword_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x20F,
    &l_daObjWSword_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
