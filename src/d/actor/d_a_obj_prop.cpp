/**
 * d_a_obj_prop.cpp
 * Object - Propeller
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_prop.h"
#include "d/d_procname.h"

static bool daObjProp_c_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjProp_c*>(i_this)->createHeap();
}

static const char* l_arcName = "L7Prop";

daObjProp_c::daObjProp_c() {}

daObjProp_c::~daObjProp_c() {
    dComIfG_resDelete(this, l_arcName);
}

static const u32 BMD_IDX[] = {3, 4};

bool daObjProp_c::createHeap() {
    u32 nameArg = getNameArg_0();
    u32 bmd = BMD_IDX[nameArg];
    mpModel = mDoExt_J3DModel__create((J3DModelData*)dComIfG_getObjectRes(l_arcName, bmd), 0x80000,
                                      0x11000084);
    return mpModel != NULL;
}

int daObjProp_c::create() {
    fopAcM_ct(this, daObjProp_c);

    int phase = dComIfG_resLoad(this, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, (heapCallbackFunc)daObjProp_c_createHeap, 0x1000)) {
            return cPhs_ERROR_e;

        } else {
            init();
            setModelMtx();
            cullMtx = mpModel->getBaseTRMtx();
            fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        }
    }
    return phase;
}

int daObjProp_c::Delete() {
    this->~daObjProp_c();
    return 1;
}

int daObjProp_c::draw() {
    g_env_light.settingTevStruct(NULL, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mDoExt_modelUpdate(mpModel);
    return 1;
}

static const s16 ADD_ANGLE[] = {0x1800, 0xE800};

int daObjProp_c::execute() {
    shape_angle.y += ADD_ANGLE[mArg];
    setModelMtx();
    return 1;
}

void daObjProp_c::init() {
    mArg = getArg_0() & 1;
}

void daObjProp_c::setModelMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    MTXCopy(mDoMtx_stack_c::now, mpModel->mBaseTransformMtx);
}

static int daObjProp_create(daObjProp_c* i_this) {
    fopAcM_ct(i_this, daObjProp_c);
    return i_this->create();
}

static int daObjProp_Delete(daObjProp_c* i_this) {
    return i_this->Delete();
}

static int daObjProp_execute(daObjProp_c* i_this) {
    return i_this->execute();
}

static int daObjProp_draw(daObjProp_c* i_this) {
    return i_this->draw();
}

static actor_method_class daObjProp_METHODS = {
    (process_method_func)daObjProp_create,  (process_method_func)daObjProp_Delete,
    (process_method_func)daObjProp_execute, (process_method_func)NULL,
    (process_method_func)daObjProp_draw,
};

extern actor_process_profile_definition g_profile_Obj_Prop = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Obj_Prop,
    &g_fpcLf_Method.base,
    sizeof(daObjProp_c),
    0,
    0,
    &g_fopAc_Method.base,
    737,
    &daObjProp_METHODS,
    0x40000,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
