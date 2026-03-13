/**
 * d_a_tag_setBall.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_setBall.h"
#include "f_pc/f_pc_name.h"

void daTagSetBall_c::initBaseMtx() {
    setBaseMtx();
}

void daTagSetBall_c::setBaseMtx() {}

int daTagSetBall_c::Create() {
    mType = getType_private();
    initBaseMtx();
    return 1;
}

int daTagSetBall_c::create() {
    fopAcM_ct(this, daTagSetBall_c);

    if (!Create()) {
        return cPhs_ERROR_e;
    } else {
        return cPhs_COMPLEATE_e;
    }
}

int daTagSetBall_c::execute() {
    return 1;
}

int daTagSetBall_c::_delete() {
    return 1;
}

static int daTagSetBall_Execute(daTagSetBall_c* i_this) {
    return i_this->execute();
}

static int daTagSetBall_Delete(daTagSetBall_c* i_this) {
    int id = fopAcM_GetID(i_this);
    return i_this->_delete();
}

static int daTagSetBall_Create(daTagSetBall_c* i_this) {
    int id = fopAcM_GetID(i_this);
    return i_this->create();
}

static actor_method_class l_daTagSetBall_Method = {
    (process_method_func)daTagSetBall_Create,
    (process_method_func)daTagSetBall_Delete,
    (process_method_func)daTagSetBall_Execute,
    (process_method_func)NULL,
    (process_method_func)NULL,
};

actor_process_profile_definition g_profile_Tag_SetBall = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Tag_SetBall_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagSetBall_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Tag_SetBall_e,
    /* Actor SubMtd */ &l_daTagSetBall_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_CULL_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
