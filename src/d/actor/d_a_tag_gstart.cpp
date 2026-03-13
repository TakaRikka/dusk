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
    mSwNo2 = (fopAcM_GetParam(this) >> 8) & 0xFF;
    field_0x56a = (fopAcM_GetParam(this) >> 0x10) & 0xFF;
    mType = (fopAcM_GetParam(this) >> 0x18) & 0xF;

    return cPhs_COMPLEATE_e;
}

static int daTagGstart_Create(fopAc_ac_c* i_this) {
    daTagGstart_c* gStart = static_cast<daTagGstart_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return gStart->create();
}

daTagGstart_c::~daTagGstart_c() {}

static int daTagGstart_Delete(daTagGstart_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTagGstart_c();
    return 1;
}

int daTagGstart_c::execute() {
    if ((mSwNo == 0xFF || fopAcM_isSwitch(this, mSwNo)) &&
        (mSwNo2 == 0xFF || !fopAcM_isSwitch(this, mSwNo2)))
    {
        if (mType != 0 || daPy_py_c::checkNowWolf()) {
            daPy_getLinkPlayerActorClass()->onSceneChangeDead(field_0x56a, fopAcM_GetRoomNo(this));
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

actor_process_profile_definition g_profile_Tag_Gstart = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 2,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Tag_Gstart_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagGstart_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Tag_Gstart_e,
    /* Actor SubMtd */ &l_daTagGstart_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_NOPAUSE_e,
    /* Group        */ fopAc_ENV_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
