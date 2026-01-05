/**
 * d_kyeff.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_kyeff.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo_wether.h"

static int dKyeff_Draw(dKyeff_c* i_this) {
    dKyw_wether_draw();
    return 1;
}

int dKyeff_c::execute() {
    if (strcmp(dComIfGp_getStartStageName(), "Name")) {
        dKyw_wether_move();
    }

    dKyw_wether_move_draw();
    dKy_FiveSenses_fullthrottle_dark();
    mDoAud_mEnvse_framework();
    return 1;
}

static int dKyeff_Execute(dKyeff_c* i_this) {
    return i_this->execute();
}

static int dKyeff_IsDelete(dKyeff_c* i_this) {
    return 1;
}

static int dKyeff_Delete(dKyeff_c* i_this) {
    dKyw_wether_delete();
    return 1;
}

// Helper functions to set float literal order
static f32 dummyLiteralOrder() {
    return 0.0f;
}

static f32 dummyLiteralOrder2() {
    return 1.0f;
}

static f32 dummyLiteralOrder3(s32 hours) {
    return hours;
}

static int dKyeff_Create(kankyo_class* i_this) {
    OSCalendarTime time;

    dKyw_wether_init();
    if (!strcmp(dComIfGp_getStartStageName(), "Name")) {
        OSTicksToCalendarTime(OSGetTime(), &time);
        g_env_light.global_wind_influence.vec.set(1.0f, 0.0f, 0.0f);
        g_env_light.global_wind_influence.pow = 0.7f;
        g_env_light.daytime = time.hour * 15.0f;
    }

    return cPhs_COMPLEATE_e;
}

static leafdraw_method_class l_dKyeff_Method = {
    (process_method_func)dKyeff_Create,  (process_method_func)dKyeff_Delete,
    (process_method_func)dKyeff_Execute, (process_method_func)dKyeff_IsDelete,
    (process_method_func)dKyeff_Draw,
};

kankyo_process_profile_definition g_profile_KYEFF = {
    fpcLy_CURRENT_e,
    12,
    fpcPi_CURRENT_e,
    PROC_KYEFF,
    &g_fpcLf_Method.base,
    sizeof(dKyeff_c),
    0,
    0,
    &g_fopKy_Method,
    5,
    &l_dKyeff_Method,
};
