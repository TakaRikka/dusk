/**
 * d_a_obj_smoke.cpp
 * Object - Smoke
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_smoke.h"
#include "f_pc/f_pc_name.h"

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
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Obj_Smoke_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjSmoke_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Obj_Smoke_e,
    /* Actor SubMtd */ &l_daObjSmoke_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_CULL_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
