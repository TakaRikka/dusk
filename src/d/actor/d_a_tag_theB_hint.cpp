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
        daNpcTheB_c* the_b = (daNpcTheB_c*)fpcM_SearchByName(PROC_NPC_THEB);
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
    fpcLy_CURRENT_e,          // mLayerID
    7,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_Tag_TheBHint,        // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daTagTheBHint_c),  // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    265,                      // mPriority
    &l_daTagTheBHint_Method,  // sub_method
    0x40000,                  // mStatus
    fopAc_ACTOR_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
};
