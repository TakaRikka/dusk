/**
 * d_a_swTime.cpp
 * Switch Time
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_swTime.h"
#include "f_pc/f_pc_name.h"


int daSwTime_c::Create() {
    mTime = getTime();
    mSwbit = getSwbit();
    mSwbit2 = getSwbit2();
    return 1;
}

int daSwTime_c::create() {
    fopAcM_ct(this, daSwTime_c);
    if (!Create()) {
        return cPhs_ERROR_e;
    }
    return cPhs_COMPLEATE_e;
}

int daSwTime_c::execute() {
    if (mSwbit2 != 0xFF && !fopAcM_isSwitch(this,mSwbit2)) {
        return 1;
    }

    if (dKy_getdaytime_hour() == mTime && dKy_getdaytime_minute() == 0) {
        fopAcM_onSwitch(this,mSwbit);
    }

    return 1;
}

int daSwTime_c::_delete() {
    return 1;
}

static int daSwTime_Execute(daSwTime_c* i_this) {
    return i_this->execute();
}

static int daSwTime_Delete(daSwTime_c* i_this) {
    return i_this->_delete();
}

static int daSwTime_Create(daSwTime_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daSwTime_Method = {
    (process_method_func)daSwTime_Create,
    (process_method_func)daSwTime_Delete,
    (process_method_func)daSwTime_Execute
};

actor_process_profile_definition g_profile_SwTime = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_SwTime_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daSwTime_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_SwTime_e,
    /* Actor SubMtd */ &l_daSwTime_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_CULL_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
