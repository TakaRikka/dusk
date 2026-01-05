/**
 * d_a_obj_smoke.cpp
 * Object - Smoke
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_smoke.h"
#include "d/d_procname.h"

void daObjSmoke_c::initBaseMtx() {
    setBaseMtx();
}

void daObjSmoke_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
}

int daObjSmoke_c::Create() {
    initBaseMtx();

    if (fopAcM_isSwitch(this, getSwbit())) {
        mParticle = dComIfGp_particle_set(0x85A6, &current.pos, &current.angle, &scale, 255, NULL,
                                          -1, NULL, NULL, NULL);
    }

    return 1;
}

int daObjSmoke_c::create() {
    fopAcM_ct(this, daObjSmoke_c);
    if (Create() == 0) {
        return cPhs_ERROR_e;
    }
    return cPhs_COMPLEATE_e;
}

int daObjSmoke_c::execute() {
    if (fopAcM_isSwitch(this, getSwbit()) && !mParticle) {
        mParticle = dComIfGp_particle_set(0x85A6, &current.pos, &current.angle, &scale, 255, NULL,
                                          -1, NULL, NULL, NULL);
    }

    return 1;
}

int daObjSmoke_c::_delete() {
    if (mParticle) {
        mParticle->quitImmortalEmitter();
        mParticle->becomeInvalidEmitter();
        mParticle = 0;
    }
    return 1;
}

static int daObjSmoke_Execute(daObjSmoke_c* i_this) {
    return i_this->execute();
}

static int daObjSmoke_Delete(daObjSmoke_c* i_this) {
    return i_this->_delete();
}

static int daObjSmoke_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjSmoke_c*>(i_this)->create();
}

static actor_method_class l_daObjSmoke_Method = {
    (process_method_func)daObjSmoke_Create,
    (process_method_func)daObjSmoke_Delete,
    (process_method_func)daObjSmoke_Execute,
};

actor_process_profile_definition g_profile_Obj_Smoke = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Smoke,          // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daObjSmoke_c),    // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    632,                     // mPriority
    &l_daObjSmoke_Method,    // sub_method
    0x40100,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
