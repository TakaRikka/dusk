/**
 * d_a_tag_ajnot.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_ajnot.h"
#include "d/actor/d_a_player.h"
#include "d/d_procname.h"

int daTagAJnot_c::create() {
    fopAcM_ct(this, daTagAJnot_c);
    return cPhs_COMPLEATE_e;
}

static int daTagAJnot_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagAJnot_c*>(i_this)->create();
}

daTagAJnot_c::~daTagAJnot_c() {}

static int daTagAJnot_Delete(daTagAJnot_c* i_this) {
    i_this->~daTagAJnot_c();
    return 1;
}

int daTagAJnot_c::execute() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 y_dist_to_player = player->current.pos.y - current.pos.y;

    if (y_dist_to_player >= 0.0f && y_dist_to_player <= scale.y * 100.0f &&
        player->current.pos.abs2XZ(current.pos) <= scale.x * (10000.0f * scale.x))
    {
        int prm = (fopAcM_GetParam(this) & 0xF);

        if (prm == 1) {
            player->onForceAutoJump();
        } else {
            player->onNotAutoJump();

            if (prm == 15) {
                player->onNotHang();
            }
        }
    }

    return 1;
}

static int daTagAJnot_Execute(daTagAJnot_c* i_this) {
    return i_this->execute();
}

static int daTagAJnot_Draw(daTagAJnot_c* i_this) {
    return 1;
}

static actor_method_class l_daTagAJnot_Method = {
    (process_method_func)daTagAJnot_Create,  (process_method_func)daTagAJnot_Delete,
    (process_method_func)daTagAJnot_Execute, (process_method_func)NULL,
    (process_method_func)daTagAJnot_Draw,
};

extern actor_process_profile_definition g_profile_Tag_AJnot = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Tag_AJnot,
    &g_fpcLf_Method.base,
    sizeof(daTagAJnot_c),
    0,
    0,
    &g_fopAc_Method.base,
    436,
    &l_daTagAJnot_Method,
    0x44000,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
