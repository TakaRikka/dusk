/**
 * d_a_tag_magne.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_magne.h"
#include "d/d_procname.h"

int daTagMagne_c::Create() {
    if (mTagMagne != NULL) {
        return 0;
    }

    mTagMagne = this;
    mSwNo1 = getSwNo1();
    mSwNo2 = getSwNo2();
    mSwNo3 = getSwNo3();

    return 1;
}

int daTagMagne_c::create() {
    fopAcM_ct(this, daTagMagne_c);

    if (mTagMagne != NULL && mTagMagne->current.roomNo != current.roomNo) {
        return cPhs_INIT_e;
    }

    if (!Create()) {
        return cPhs_ERROR_e;
    }

    return cPhs_COMPLEATE_e;
}

int daTagMagne_c::_delete() {
    mTagMagne = NULL;
    return 1;
}

static void daTagMagne_Delete(daTagMagne_c* i_this) {
    i_this->_delete();
}

static void daTagMagne_Create(fopAc_ac_c* i_this) {
    ((daTagMagne_c*)i_this)->create();
}

static actor_method_class l_daTagMagne_Method = {
    (process_method_func)daTagMagne_Create,
    (process_method_func)daTagMagne_Delete,
    (process_method_func)NULL,
    (process_method_func)NULL,
    (process_method_func)NULL,
};

extern actor_process_profile_definition g_profile_Tag_Magne = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_Magne,
    &g_fpcLf_Method.base,
    sizeof(daTagMagne_c),
    0,
    0,
    &g_fopAc_Method.base,
    590,
    &l_daTagMagne_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
