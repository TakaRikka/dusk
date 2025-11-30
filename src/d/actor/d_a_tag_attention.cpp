/**
 * d_a_tag_attention.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_attention.h"
#include "d/actor/d_a_player.h"

static const u8 l_dist_tbl[] = { 0x23, 0x24 };

int daAttp_c::Create() {
    attention_info.distances[fopAc_attn_LOCK_e] = l_dist_tbl[daAttp_prm::getArg4(this)];

    execute();
    return 1;
}

int daAttp_c::create() {
    fopAcM_ct(this, daAttp_c);
    Create();
    return cPhs_COMPLEATE_e;
}

int daAttp_c::execute() {
    bool bVar1 = true;
    fopAc_ac_c& link = *daPy_getPlayerActorClass();

    cXyz pos = link.current.pos - current.pos;
    mDoMtx_stack_c::YrotS(-shape_angle.y);
    mDoMtx_stack_c::multVec(&pos, &pos);

    switch (daAttp_prm::getArg2(this)) {
    case 0:
        if (pos.z < 0) {
            bVar1 = false;
        }
        break;
    case 1:
        if (pos.y > 0) {
            bVar1 = false;
        }
        break;
    case 2:
        if (pos.y < 0) {
            bVar1 = false;
        }
        break;
    case 3:
        if (pos.y > 0 || pos.z < 0) {
            bVar1 = false;
        }
        break;
    case 4:
        if (pos.y < 0 || pos.z < 0) {
            bVar1 = false;
        }
        break;
    default:
        break;
    case 7:
        break;
    }

    if (fopAcM_isSwitch(this, daAttp_prm::getSw1(this))) {
        switch (daAttp_prm::getArg1(this)) {
        case 0:
            break;
        case 1:
            if (daPy_py_c::checkNowWolf()) {
                bVar1 = false;
            }
            break;
        case 2:
            if (!daPy_py_c::checkNowWolf()) {
                bVar1 = false;
            }
            break;
        case 3:
            bVar1 = false;
            break;
        }
    } else {
        switch (daAttp_prm::getArg0(this)) {
        case 0:
            break;
        case 1:
            if (daPy_py_c::checkNowWolf()) {
                bVar1 = false;
            }
            break;
        case 2:
            if (!daPy_py_c::checkNowWolf()) {
                bVar1 = false;
            }
            break;
        case 3:
            bVar1 = false;
        }

        if (daAttp_prm::getSw2(this) != 0xff) {
            if (fopAcM_isSwitch(this, daAttp_prm::getSw2(this))) {
                if (daAttp_prm::getArg3(this) == 0) {
                    bVar1 = false;
                }
            } else if (daAttp_prm::getArg3(this) == 1) {
                bVar1 = false;
            }
        }
    }

    if (bVar1) {
        attention_info.flags |= fopAc_AttnFlag_LOCK_e;
    } else {
        attention_info.flags &= ~fopAc_AttnFlag_LOCK_e;
    }

    return 1;
}

int daAttp_c::draw() {
    return 1;
}

int daAttp_c::_delete() {
    return 1;
}

static int daAttp_Draw(daAttp_c* i_this) {
    return i_this->draw();
}


static int daAttp_Execute(daAttp_c* i_this) {
    return i_this->execute();
}

static int daAttp_Delete(daAttp_c* i_this) {
    return i_this->_delete();
}

static int daAttp_Create(fopAc_ac_c* i_this) {
    return static_cast<daAttp_c*>(i_this)->create();
}

static actor_method_class l_daAttp_Method = {
    (process_method_func)daAttp_Create,  (process_method_func)daAttp_Delete,
    (process_method_func)daAttp_Execute, (process_method_func)NULL,
    (process_method_func)daAttp_Draw,
};

extern actor_process_profile_definition g_profile_Tag_Attp = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Tag_Attp,          // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daAttp_c),       // mSize
    0,                      // mSizeOther
    0,                      // mParameter
    &g_fopAc_Method.base,   // sub_method
    440,                    // mPriority
    &l_daAttp_Method,       // sub_method
    0x40100,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_0_e,      // cullType   
};
