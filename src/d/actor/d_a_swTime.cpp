/**
 * d_a_swTime.cpp
 * Switch Time
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_swTime.h"
#include "d/d_procname.h"


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
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_SwTime,            // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daSwTime_c),     // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    576,                    // mPriority
    &l_daSwTime_Method,     // sub_method
    0x40100,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
