/**
 * d_a_tag_wara_howl.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_wara_howl.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

daTagWrHowl_c::daTagWrHowl_c() {}

daTagWrHowl_c::~daTagWrHowl_c() {}

int daTagWrHowl_c::create() {
    fopAcM_ct(this, daTagWrHowl_c);
    init();
    return cPhs_COMPLEATE_e;
}

int daTagWrHowl_c::Delete() {
    return 1;
}

int daTagWrHowl_c::draw() {
    return 1;
}

int daTagWrHowl_c::execute() {
    attention_info.flags = 0;

    if (chkWlfInTag()) {
        attention_info.flags |= fopAc_AttnFlag_ETC_e;
        attention_info.distances[fopAc_attn_ETC_e] = 65;
    }

    return 1;
}

void daTagWrHowl_c::init() {
    field_0x56c = 50.0f * scale.x;
    field_0x570 = field_0x56c * field_0x56c;
}

bool daTagWrHowl_c::chkWlfInTag() {
    if (dComIfGp_getPlayer(0) == NULL) {
        return false;
    }

    if (daPy_py_c::checkNowWolf()) {
        cXyz player_pos = dComIfGp_getPlayer(0)->current.pos;
        if (player_pos.abs2(current.pos) < field_0x570) {
            return true;
        }
    }

    return false;
}

static int daTagWrHowl_create(daTagWrHowl_c* i_this) {
    fopAcM_ct(i_this, daTagWrHowl_c);
    return i_this->create();
}

static int daTagWrHowl_Delete(daTagWrHowl_c* i_this) {
    return i_this->Delete();
}

static int daTagWrHowl_execute(daTagWrHowl_c* i_this) {
    return i_this->execute();
}

static int daTagWrHowl_draw(daTagWrHowl_c* i_this) {
    return i_this->draw();
}

static actor_method_class daTagWrHowl_METHODS = {
    (process_method_func)daTagWrHowl_create,  (process_method_func)daTagWrHowl_Delete,
    (process_method_func)daTagWrHowl_execute, (process_method_func)NULL,
    (process_method_func)daTagWrHowl_draw,
};

actor_process_profile_definition g_profile_Tag_WaraHowl = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_WaraHowl,
    &g_fpcLf_Method.base,
    sizeof(daTagWrHowl_c),
    0,
    0,
    &g_fopAc_Method.base,
    733,
    &daTagWrHowl_METHODS,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
