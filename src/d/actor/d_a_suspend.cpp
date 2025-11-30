/**
 * @file d_a_suspend.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_suspend.h"
#include "d/d_procname.h"

int daSus_c::create() {
    s8 roomNo = fopAcM_GetRoomNo(this);

    if (roomNo < 0) {
        roomNo = getRoom();
    }

    u8 sw = getSw();
    u8 arg0 = getArg0();
    u8 arg1 = getArg1();

    if (getScale() != 0) {
        scale.x *= 1250.0f;
        scale.y *= 2500.0f;
        scale.z *= 1250.0f;
    } else {
        scale.x *= 125.0f;
        scale.y *= 250.0f;
        scale.z *= 125.0f;
    }

    newData(roomNo, current.pos, scale, sw, arg0, arg1);
    return cPhs_COMPLEATE_e;
}

static int daSus_create(daSus_c* i_this) {
    fopAcM_ct(i_this, daSus_c);

    return i_this->create();
}
static actor_method_class daSus_METHODS = {
    (process_method_func)daSus_create, NULL, NULL, NULL, NULL,
};

extern actor_process_profile_definition g_profile_SUSPEND = {
    fpcLy_CURRENT_e,
    11,
    fpcPi_CURRENT_e,
    PROC_SUSPEND,
    &g_fpcLf_Method.base,
    sizeof(daSus_c),
    0,
    0,
    &g_fopAc_Method.base,
    763,
    &daSus_METHODS,
    0x60000,
    5,
    fopAc_CULLBOX_0_e,
};
