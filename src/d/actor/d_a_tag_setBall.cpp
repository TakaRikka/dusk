/**
 * d_a_tag_setBall.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_setBall.h"
#include "d/d_procname.h"

void daTagSetBall_c::initBaseMtx() {
    setBaseMtx();
}

void daTagSetBall_c::setBaseMtx() {}

int daTagSetBall_c::Create() {
    mType = getType_private();
    initBaseMtx();
    return 1;
}

int daTagSetBall_c::create() {
    fopAcM_ct(this, daTagSetBall_c);

    if (!Create()) {
        return cPhs_ERROR_e;
    } else {
        return cPhs_COMPLEATE_e;
    }
}

int daTagSetBall_c::execute() {
    return 1;
}

int daTagSetBall_c::_delete() {
    return 1;
}

static int daTagSetBall_Execute(daTagSetBall_c* i_this) {
    return i_this->execute();
}

static int daTagSetBall_Delete(daTagSetBall_c* i_this) {
    return i_this->_delete();
}

static int daTagSetBall_Create(daTagSetBall_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daTagSetBall_Method = {
    (process_method_func)daTagSetBall_Create,
    (process_method_func)daTagSetBall_Delete,
    (process_method_func)daTagSetBall_Execute,
    (process_method_func)NULL,
    (process_method_func)NULL,
};

extern actor_process_profile_definition g_profile_Tag_SetBall = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_SetBall,
    &g_fpcLf_Method.base,
    sizeof(daTagSetBall_c),
    0,
    0,
    &g_fopAc_Method.base,
    575,
    &l_daTagSetBall_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
