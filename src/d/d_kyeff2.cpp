/**
 * d_kyeff2.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "SSystem/SComponent/c_phase.h"
#include "d/d_kyeff2.h"
#include "d/d_kankyo_wether.h"

static int dKyeff2_Draw(dKyeff2_c* i_this) {
    dKyw_wether_draw2();
    return 1;
}

int dKyeff2_c::execute() {
    dKyw_wether_move_draw2();
    return 1;
}

static int dKyeff2_Execute(dKyeff2_c* i_this) {
    return i_this->execute();
}

static int dKyeff2_IsDelete(dKyeff2_c* i_this) {
    return 1;
}

static int dKyeff2_Delete(dKyeff2_c* i_this) {
    dKyw_wether_delete2();
    return 1;
}

static int dKyeff2_Create(kankyo_class* i_this) {
    dKyw_wether_init2();
    return cPhs_COMPLEATE_e;
}

static leafdraw_method_class l_dKyeff2_Method = {
    (process_method_func)dKyeff2_Create,  (process_method_func)dKyeff2_Delete,
    (process_method_func)dKyeff2_Execute, (process_method_func)dKyeff2_IsDelete,
    (process_method_func)dKyeff2_Draw,
};

extern kankyo_process_profile_definition g_profile_KYEFF2 = {
    fpcLy_CURRENT_e,
    12,
    fpcPi_CURRENT_e,
    PROC_KYEFF2,
    &g_fpcLf_Method.base,
    sizeof(dKyeff2_c),
    0,
    0,
    &g_fopKy_Method,
    3,
    &l_dKyeff2_Method,
};
