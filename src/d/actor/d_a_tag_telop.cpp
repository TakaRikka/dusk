/**
 * d_a_tag_telop.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_telop.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_msg_mng.h"

int daTag_Telop_c::create() {
    fopAcM_ct(this, daTag_Telop_c);

    mMessageNo = getMessageNo();

    if (dComIfGs_isTmpBit(0x1301)) {
        mMessageStatus = 99;
    } else if (mMessageNo != 0xFFFF) {
        mMessageStatus = 0;
    } else {
        mMessageStatus = 99;
    }

    return cPhs_COMPLEATE_e;
}

void daTag_Telop_c::execute() {
    switch (mMessageStatus) {
    case 0:
        fopMsgM_messageSetDemo(mMessageNo);
        mMessageStatus = 99;
        break;
    case 99:
        fopAcM_delete(this);
        mMessageStatus = -1;
        break;
    }
}

static int daTag_Telop_Draw(daTag_Telop_c* i_this) {
    return 1;
}

static int daTag_Telop_Execute(daTag_Telop_c* i_this) {
    i_this->execute();
    return 1;
}

static int daTag_Telop_IsDelete(daTag_Telop_c* i_this) {
    return 1;
}

static int daTag_Telop_Delete(daTag_Telop_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTag_Telop_c();
    return 1;
}

static int daTag_Telop_Create(fopAc_ac_c* i_this) {
    daTag_Telop_c* telop = static_cast<daTag_Telop_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return telop->create();
}

static actor_method_class l_daTag_Telop_Method = {
    (process_method_func)daTag_Telop_Create,  (process_method_func)daTag_Telop_Delete,
    (process_method_func)daTag_Telop_Execute, (process_method_func)daTag_Telop_IsDelete,
    (process_method_func)daTag_Telop_Draw,
};

actor_process_profile_definition g_profile_TAG_TELOP = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_TAG_TELOP_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTag_Telop_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_TAG_TELOP_e,
    /* Actor SubMtd */ &l_daTag_Telop_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_6_e,
};
