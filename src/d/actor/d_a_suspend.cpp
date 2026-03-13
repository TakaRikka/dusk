/**
 * @file d_a_suspend.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_suspend.h"
#include "f_pc/f_pc_name.h"

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

actor_process_profile_definition g_profile_SUSPEND = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 11,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_SUSPEND_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daSus_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_SUSPEND_e,
    /* Actor SubMtd */ &daSus_METHODS,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_NOPAUSE_e,
    /* Group        */ fopAc_UNK_GROUP_5_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
