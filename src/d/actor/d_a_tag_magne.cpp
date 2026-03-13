/**
 * d_a_tag_magne.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_magne.h"
#include "f_pc/f_pc_name.h"

int daTagMagne_c::Create() {
    if (mTagMagne != NULL) {
        // "Multiple magnet cord(?) management tags are placed!"
        OS_REPORT_ERROR("マグネコード管理タグが複数置かれています！\n");
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

    if (mTagMagne != NULL && fopAcM_GetRoomNo(mTagMagne) != fopAcM_GetRoomNo(this)) {
        return cPhs_INIT_e;
    }

    if (!Create()) {
        return cPhs_ERROR_e;
    }

    OS_REPORT("TAG MAGNE PARAM=%x\n", fopAcM_GetParam(this));

    return cPhs_COMPLEATE_e;
}

int daTagMagne_c::_delete() {
    mTagMagne = NULL;
    return 1;
}

static void daTagMagne_Delete(daTagMagne_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->_delete();
}

static void daTagMagne_Create(fopAc_ac_c* i_this) {
    daTagMagne_c* magne = static_cast<daTagMagne_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    magne->create();
}

static actor_method_class l_daTagMagne_Method = {
    (process_method_func)daTagMagne_Create,
    (process_method_func)daTagMagne_Delete,
    (process_method_func)NULL,
    (process_method_func)NULL,
    (process_method_func)NULL,
};

actor_process_profile_definition g_profile_Tag_Magne = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Tag_Magne_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagMagne_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Tag_Magne_e,
    /* Actor SubMtd */ &l_daTagMagne_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
