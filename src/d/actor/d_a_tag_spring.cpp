/**
 * d_a_tag_spring.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_spring.h"
#include "d/actor/d_a_player.h"
#include "d/d_procname.h"

void daTagSpring_c::initBaseMtx() {
    setBaseMtx();
}

void daTagSpring_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
}

int daTagSpring_c::Create() {
    initBaseMtx();
    return 1;
}

int daTagSpring_c::create() {
    fopAcM_ct(this, daTagSpring_c);

    if (!Create()) {
        return cPhs_ERROR_e;
    }

    fopAcM_GetParam(this);
    return cPhs_COMPLEATE_e;
}

int daTagSpring_c::execute() {
    if (getSwbit() != 0xFF) {
        if (!fopAcM_isSwitch(this, getSwbit())) {
            return 1;
        }
    }

    if (dComIfGp_event_runCheck()) {
        return 1;
    }

    fopAc_ac_c* player_p = daPy_getPlayerActorClass();

    if (checkArea() && fopAcM_wt_c::waterCheck(&player_p->current.pos) &&
        (!dComIfGp_checkPlayerStatus0(0, 0x100000) &&
             fopAcM_wt_c::mWaterY > player_p->current.pos.y ||
         dComIfGp_checkPlayerStatus0(0, 0x100000)))
    {
        if (mTimer != 0) {
            mTimer--;
        } else {
            mTimer = 30;
            dComIfGp_setItemLifeCount(1.0f, 1);
        }
    } else {
        mTimer = 30;
    }

    return 1;
}

u8 daTagSpring_c::checkArea() {
    fopAc_ac_c* player_p = daPy_getPlayerActorClass();
    return player_p->current.pos.absXZ(current.pos) < (scale.x * 1000);
}

int daTagSpring_c::_delete() {
    return 1;
}

static int daTagSpring_Execute(daTagSpring_c* i_this) {
    return i_this->execute();
}

static int daTagSpring_Delete(daTagSpring_c* i_this) {
    return i_this->_delete();
}

static int daTagSpring_Create(daTagSpring_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daTagSpring_Method = {
    (process_method_func)daTagSpring_Create,
    (process_method_func)daTagSpring_Delete,
    (process_method_func)daTagSpring_Execute,
    (process_method_func)NULL,
};

extern actor_process_profile_definition g_profile_Tag_Spring = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Tag_Spring,        // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daTagSpring_c),  // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    587,                    // mPriority
    &l_daTagSpring_Method,  // sub_method
    0x40000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
