/**
 * d_a_tag_theB_hint.cpp
 * Tag - Telma B Hint
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_theB_hint.h"
#include "d/actor/d_a_npc_theB.h"

int daTagTheBHint_c::create() {
    fopAcM_ct(this, daTagTheBHint_c);
    field_0x568 = std::pow(scale.x * 100.0f,2.0f);
    return cPhs_COMPLEATE_e;
}

int daTagTheBHint_c::execute() {
    if (fopAcM_isSwitch(this, getSwitchNo())) {
        fopAcM_delete(this);
        return 1;
    }

    if (fopAcM_searchPlayerDistanceXZ2(this) < field_0x568) {
        daNpcTheB_c* the_b = (daNpcTheB_c*)fpcM_SearchByName(fpcNm_NPC_THEB_e);
        if (the_b) {
            int roomNo = getRoomNo() == 0xff ? fopAcM_GetRoomNo(this) : getRoomNo();

            the_b->setHintEvent(getMessageNo(), getLinkID(), roomNo);
        }
    }

    return 1;
}

static int daTagTheBHint_Create(fopAc_ac_c* i_this) {
    daTagTheBHint_c* theBHint = static_cast<daTagTheBHint_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return theBHint->create();
}

static int daTagTheBHint_Delete(daTagTheBHint_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTagTheBHint_c();
    return 1;
}

static int daTagTheBHint_Execute(daTagTheBHint_c* i_this) {
    return i_this->execute();
}

static actor_method_class l_daTagTheBHint_Method = {
    (process_method_func)daTagTheBHint_Create,
    (process_method_func)daTagTheBHint_Delete,
    (process_method_func)daTagTheBHint_Execute
};

actor_process_profile_definition g_profile_Tag_TheBHint = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Tag_TheBHint_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagTheBHint_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Tag_TheBHint_e,
    /* Actor SubMtd */ &l_daTagTheBHint_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
