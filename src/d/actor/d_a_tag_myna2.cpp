/**
 * @file d_a_tag_myna2.cpp
 * @brief Tag - Creates Plumm's actor.
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_player.h"
#include "d/actor/d_a_tag_myna2.h"
#include "d/d_procname.h"

s32 daTagMyna2_c::create() {
    fopAcM_ct(this, daTagMyna2_c);
    mSwitchNo = getSwBit();
    mMode = getMode();
    mTimer = 5;
    return cPhs_COMPLEATE_e;
}

s32 daTagMyna2_c::execute() {
    if (!dComIfGp_event_runCheck() &&
        !dComIfGs_isSwitch(mSwitchNo,fopAcM_GetRoomNo(this)) &&
        (daPy_getPlayerActorClass()->current.pos - current.pos).absXZ() < getExtent()) {
        csXyz actor_angle(30, (s16)fopAcM_searchPlayerAngleY(this), 0);
        cXyz actor_pos = current.pos;
        actor_pos.y += 1000.0f;

        if (fopAcM_gc_c::gndCheck(&actor_pos) != 0) {
            actor_pos.y = fopAcM_gc_c::getGroundY();
        } else {
            actor_pos = current.pos;
        }

        u32 var_r29 = 0xffff0001;
        if (mTimer == 0) {
            if (fopAcM_create(PROC_MYNA2, var_r29 | mSwitchNo << 8, &actor_pos,
                              fopAcM_GetRoomNo(this), &actor_angle, 0, -1)
                != fpcM_ERROR_PROCESS_ID_e) {
                dComIfGs_onSwitch(mSwitchNo,fopAcM_GetRoomNo(this));
            }
        }
    }
  
    if (dComIfGp_event_runCheck()) {
        mTimer = 5;
    } else {
        cLib_calcTimer(&mTimer);
    }

    return 1;
}

static s32 daTagMyna2_Create(fopAc_ac_c* i_this) {
    daTagMyna2_c* myna2 = static_cast<daTagMyna2_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return myna2->create();
}

static s32 daTagMyna2_Execute(daTagMyna2_c* i_this) {
    return i_this->execute();
}

static s32 daTagMyna2_Delete(daTagMyna2_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTagMyna2_c();
    return 1;
}

static actor_method_class l_daTagMyna2_Method = {
    (process_method_func)daTagMyna2_Create,
    (process_method_func)daTagMyna2_Delete,
    (process_method_func)daTagMyna2_Execute,
};

actor_process_profile_definition g_profile_TAG_MYNA2 = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_TAG_MYNA2,         // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daTagMyna2_c),   // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    394,                    // mPriority
    &l_daTagMyna2_Method,   // sub_method
    0x44000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
