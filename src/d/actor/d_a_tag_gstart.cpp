/**
 * d_a_tag_gstart.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_gstart.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

int daTagGstart_c::create() {
    fopAcM_ct(this, daTagGstart_c);

    mSwNo = fopAcM_GetParam(this);
    mSwNo2 = fopAcM_GetParam(this) >> 8;
    field_0x56a = fopAcM_GetParam(this) >> 0x10;
    mType = (fopAcM_GetParam(this) >> 0x18) & 0xF;

    return cPhs_COMPLEATE_e;
}

static int daTagGstart_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagGstart_c*>(i_this)->create();
}

daTagGstart_c::~daTagGstart_c() {}

static int daTagGstart_Delete(daTagGstart_c* i_this) {
    i_this->~daTagGstart_c();
    return 1;
}

int daTagGstart_c::execute() {
    if ((mSwNo == 0xFF || dComIfGs_isSwitch(mSwNo, fopAcM_GetHomeRoomNo(this))) &&
        (mSwNo2 == 0xFF || !dComIfGs_isSwitch(mSwNo2, fopAcM_GetHomeRoomNo(this))))
    {
        if (mType != 0 || dComIfGp_getLinkPlayer()->checkWolf()) {
            dComIfGp_getLinkPlayer()->onSceneChangeDead(field_0x56a, fopAcM_GetRoomNo(this));
        }
    }

    return 1;
}

static int daTagGstart_Execute(daTagGstart_c* i_this) {
    return i_this->execute();
}

static int daTagGstart_Draw(daTagGstart_c*) {
    return 1;
}

static actor_method_class l_daTagGstart_Method = {
    (process_method_func)daTagGstart_Create,  (process_method_func)daTagGstart_Delete,
    (process_method_func)daTagGstart_Execute, (process_method_func)NULL,
    (process_method_func)daTagGstart_Draw,
};

extern actor_process_profile_definition g_profile_Tag_Gstart = {
    fpcLy_CURRENT_e,
    2,
    fpcPi_CURRENT_e,
    PROC_Tag_Gstart,
    &g_fpcLf_Method.base,
    sizeof(daTagGstart_c),
    0,
    0,
    &g_fopAc_Method.base,
    437,
    &l_daTagGstart_Method,
    0x60000,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
