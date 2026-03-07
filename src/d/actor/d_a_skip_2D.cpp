/**
 * d_a_skip2D.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_skip_2D.h"
#include "d/d_meter_button.h"
#include "f_op/f_op_actor_mng.h"

static int daSkip2D_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daSkip2D_c*>(i_this)->createHeap();
}

int daSkip2D_c::createHeap() {
    mpButton = new dMeterButton_c();

    if (mpButton == NULL) {
        return 0;
    }

    return 1;
}

int daSkip2D_c::create() {
    fopAcM_ct(this, daSkip2D_c);

    if (!fopAcM_entrySolidHeap(this, daSkip2D_createHeap, 0)) {
        return cPhs_ERROR_e;
    }

    return cPhs_COMPLEATE_e;
}

int daSkip2D_c::destroy() {
    return 1;
}

int daSkip2D_c::draw() {
    dComIfGd_set2DOpaTop((dDlst_base_c*)&mpButton);  // ???
    return 1;
}

int daSkip2D_c::execute() {
    return 1;
}

void daSkip2D_c::initiate() {
    field_0x56c = 0;
    mpButton = NULL;
    field_0x574 = 0;
}

static int daSkip2D_Create(void* i_this) {
    return static_cast<daSkip2D_c*>(i_this)->create();
}

static int daSkip2D_Delete(void* i_this) {
    return static_cast<daSkip2D_c*>(i_this)->destroy();
}

static int daSkip2D_Execute(void* i_this) {
    return static_cast<daSkip2D_c*>(i_this)->execute();
}

static int daSkip2D_Draw(void* i_this) {
    return static_cast<daSkip2D_c*>(i_this)->draw();
}

static int daSkip2D_IsDelete(void*) {
    return 1;
}

static actor_method_class daSkip2D_MethodTable = {
    (process_method_func)daSkip2D_Create,  (process_method_func)daSkip2D_Delete,
    (process_method_func)daSkip2D_Execute, (process_method_func)daSkip2D_IsDelete,
    (process_method_func)daSkip2D_Draw,
};

actor_process_profile_definition g_profile_SKIP2D = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_SKIP2D,
    &g_fpcLf_Method.base,
    sizeof(daSkip2D_c),
    0,
    0,
    &g_fopAc_Method.base,
    772,
    &daSkip2D_MethodTable,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
