/**
 * d_a_obj_rcircle.cpp
 * Object - River Circle
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_rcircle.h"
#include "d/d_procname.h"

static int daObjRCircle_c_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjRCircle_c*>(i_this)->createHeap();
}

static char* l_arcName = "RCircle";

daObjRCircle_c::daObjRCircle_c() {}

daObjRCircle_c::~daObjRCircle_c() {
    dComIfG_resDelete(this, l_arcName);
}

int daObjRCircle_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);

    if (!mpModel) {
        return 0;
    } else {
        J3DAnmTextureSRTKey* srtKey = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 11);
        mBtk.init(modelData, srtKey, 1, 2, 1.0f, 0, -1);

        J3DAnmTevRegKey* regKey = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 8);
        mBrk.init(modelData, regKey, 1, 0, 0.0f, 0, -1);
    }
    return 1;
}

int daObjRCircle_c::create() {
    fopAcM_ct(this, daObjRCircle_c);
    if (fopAcM_isSwitch(this, getSwBit())) {
        return cPhs_ERROR_e;
    }
    
    int phase = dComIfG_resLoad(this, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, (heapCallbackFunc)daObjRCircle_c_createHeap, 0x1000)) {
            return cPhs_ERROR_e;

        } else {
            init();
            setModelMtx();
            fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
            fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        }
    }
    return phase;
}

int daObjRCircle_c::Delete() {
    this->~daObjRCircle_c();
    return 1;
}

int daObjRCircle_c::draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    J3DModelData* modelData = mpModel->getModelData();
    mBtk.entry(modelData);
    mBrk.entry(modelData);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjRCircle_c::execute() {
    if (mSwitch) {
        if (mBrk.isStop()) {
            fopAcM_delete(this);
        }
    } else {
        if (fopAcM_isSwitch(this, getSwBit())) {
            mSwitch = 1;
            mBrk.setPlaySpeed(1.0f);
        }
    }
    mBtk.play();
    mBrk.play();
    return 1;
}

void daObjRCircle_c::init() {
    mSwitch = 0;
}

void daObjRCircle_c::setModelMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    MTXCopy(mDoMtx_stack_c::now, mpModel->mBaseTransformMtx);
}

static int daObjRCircle_create(daObjRCircle_c* i_this) {
    fopAcM_ct(i_this, daObjRCircle_c);
    return i_this->create();
}

static int daObjRCircle_Delete(daObjRCircle_c* i_this) {
    return i_this->Delete();
}

static int daObjRCircle_execute(daObjRCircle_c* i_this) {
    return i_this->execute();
}

static int daObjRCircle_draw(daObjRCircle_c* i_this) {
    return i_this->draw();
}

static actor_method_class daObjRCircle_METHODS = {
    (process_method_func)daObjRCircle_create,  (process_method_func)daObjRCircle_Delete,
    (process_method_func)daObjRCircle_execute, (process_method_func)NULL,
    (process_method_func)daObjRCircle_draw,
};

actor_process_profile_definition g_profile_Obj_RCircle = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Obj_RCircle,
    &g_fpcLf_Method.base,
    sizeof(daObjRCircle_c),
    0,
    0,
    &g_fopAc_Method.base,
    740,
    &daObjRCircle_METHODS,
    0x44180,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
