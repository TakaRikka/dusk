/**
 * d_a_tboxSw.cpp
 * Turns on a switch when a treasure chest is opened
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tboxSw.h"
#include "d/d_com_inf_game.h"
#include "f_pc/f_pc_name.h"

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
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_TBOX_SW_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTboxSw_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_TBOX_SW_e,
    /* Actor SubMtd */ &l_daTboxSw_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_CULL_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
