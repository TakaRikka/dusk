#include "d/dolzel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_ss_base.h"
#include "f_op/f_op_actor_mng.h"

daObj_SSBase_c::daObj_SSBase_c() {
    mpParentPtr = NULL;
    field_0x570 = 0;
    mFlowNodeNum = 0xFFFF;
    mValueNum = 0;
}

daObj_SSBase_c::~daObj_SSBase_c() {}

u32 daObj_SSBase_c::getProcessID() {
    return fopAcM_GetID(this);
}

void daObj_SSBase_c::setSoldOut() {
    /* empty function */
}
