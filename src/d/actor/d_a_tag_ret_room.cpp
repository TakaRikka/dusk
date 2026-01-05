/**
 * d_a_tag_ret_room.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_ret_room.h"
#include "d/actor/d_a_player.h"

daTagRetRm_c::daTagRetRm_c() {}

daTagRetRm_c::~daTagRetRm_c() {}

int daTagRetRm_c::create() {
    fopAcM_ct(this, daTagRetRm_c);
    init();
    return cPhs_COMPLEATE_e;
}

int daTagRetRm_c::Delete() {
    return 1;
}

int daTagRetRm_c::draw() {
    return 1;
}

int daTagRetRm_c::execute() {
    if (field_0x578 != 0) {
        field_0x578--;

        if (field_0x578 == 0) {
            daPy_py_c::forceRestartRoom(1, 5, 0xc9);
        }
    } else {
        if (chkPlyrInTag()) {
            field_0x578 = 30;
        }
    }
    return 1;
}

void daTagRetRm_c::init() {
    field_0x56c = scale.x * 50;
    field_0x570 = scale.y * 100;
    field_0x574 = scale.z * 50;
    field_0x578 = 0;
}

u8 daTagRetRm_c::chkPlyrInTag() {
    cXyz pos;
    if (dComIfGp_getPlayer(0) == NULL) {
        return 0;
    } else {
        pos = daPy_getPlayerActorClass()->current.pos - current.pos;
        mDoMtx_stack_c::YrotS(-current.angle.y);
        mDoMtx_stack_c::multVec(&pos, &pos);
        if ((0 < pos.y) && (field_0x570 > pos.y) && (field_0x56c > pos.x) &&
            (field_0x574 > pos.z) && (-field_0x56c < pos.x) && (-field_0x574 < pos.z))
        {
            return 1;
        } else {
            return 0;
        }
    }
}

static int daTagRetRm_create(daTagRetRm_c* i_this) {
    fopAcM_ct(i_this, daTagRetRm_c);
    return i_this->create();
}

static int daTagRetRm_Delete(daTagRetRm_c* i_this) {
    return i_this->Delete();
}

static int daTagRetRm_execute(daTagRetRm_c* i_this) {
    return i_this->execute();
}

static int daTagRetRm_draw(daTagRetRm_c* i_this) {
    return i_this->draw();
}

static actor_method_class daTagRetRm_METHODS = {
    (process_method_func)daTagRetRm_create,  (process_method_func)daTagRetRm_Delete,
    (process_method_func)daTagRetRm_execute, (process_method_func)NULL,
    (process_method_func)daTagRetRm_draw,

};

actor_process_profile_definition g_profile_Tag_RetRoom = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Tag_RetRoom,       // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daTagRetRm_c),   // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    731,                    // mPriority
    &daTagRetRm_METHODS,    // sub_method
    0x40000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
