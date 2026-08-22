#pragma once
#include "f_op/f_op_actor_mng.h"
#include "mods/svc/log.hpp"

// This function calls the constructor for a given actor class. This is needed because the actor
// system only allocates memory before the create method is called
template <class T>
bool mod_fopAcM_ct(T* ptr) {
    if (ptr->layer_tag.layer == NULL) {
        mods::log::error("Actor Layer Tag is Null!!!");
        return false;
    }
    if (!fopAcM_CheckCondition(ptr, fopAcCnd_INIT_e)) {
        fopAcM_ct_placement(ptr, T);
        fopAcM_OnCondition(ptr, fopAcCnd_INIT_e);
    }
    if (ptr->layer_tag.layer == NULL) {
        mods::log::error("Actor Layer Tag is Null!!!");
        return false;
    }
    return true;
}
