/**
 * d_a_tboxSw.cpp
 * Turns on a switch when a treasure chest is opened
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tboxSw.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

int daTboxSw_c::Create() {
    return 1;
}

int daTboxSw_c::create() {
    fopAcM_ct(this, daTboxSw_c);

    if (!Create()) {
        return cPhs_ERROR_e;
    }

    return cPhs_COMPLEATE_e;
}

int daTboxSw_c::execute() {
    if (dComIfGs_isTbox(daTboxSw_prm::getTboxNo(this))) {
        dComIfGs_onSwitch(daTboxSw_prm::getSwNo(this), fopAcM_GetHomeRoomNo(this));
        fopAcM_delete(this);
    }

    return 1;
}

int daTboxSw_c::draw() {
    return 1;
}

int daTboxSw_c::_delete() {
    return 1;
}

static int daTboxSw_Draw(daTboxSw_c* i_this) {
    return i_this->draw();
}

static int daTboxSw_Execute(daTboxSw_c* i_this) {
    return i_this->execute();
}

static int daTboxSw_Delete(daTboxSw_c* i_this) {
    return i_this->_delete();
}

static int daTboxSw_Create(fopAc_ac_c* i_this) {
    return static_cast<daTboxSw_c*>(i_this)->create();
}

static actor_method_class l_daTboxSw_Method = {
    (process_method_func)daTboxSw_Create,  (process_method_func)daTboxSw_Delete,
    (process_method_func)daTboxSw_Execute, (process_method_func)NULL,
    (process_method_func)daTboxSw_Draw,
};

actor_process_profile_definition g_profile_TBOX_SW = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_TBOX_SW,
    &g_fpcLf_Method.base,
    sizeof(daTboxSw_c),
    0,
    0,
    &g_fopAc_Method.base,
    526,
    &l_daTboxSw_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
