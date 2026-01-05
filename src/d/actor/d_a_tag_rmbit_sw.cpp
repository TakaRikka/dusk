/**
 * d_a_tag_rmbit_sw.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_rmbit_sw.h"
#include "d/d_procname.h"

daTagRmbitSw_c::daTagRmbitSw_c() {}

daTagRmbitSw_c::~daTagRmbitSw_c() {}

int daTagRmbitSw_c::create() {
    fopAcM_ct(this, daTagRmbitSw_c);
    init();
    return cPhs_COMPLEATE_e;
}

int daTagRmbitSw_c::Delete() {
    return 1;
}

int daTagRmbitSw_c::draw() {
    return 1;
}

int daTagRmbitSw_c::execute() {
    if (chkPlyrInTag()) {
        if (getArg3_CngBitFlg() == 1) {
            if ((s32)getSwBit() == 0xff || fopAcM_isSwitch(this, getSwBit())) {
                if (getArg2_CngBitVal() == 1) {
                    if (getArg0_RoomSaveTableNo() == -1) {
                        dComIfGs_onVisitedRoom(getArg1_RoomNo());
                    } else {
                        dComIfGs_onSaveVisitedRoom(getArg0_RoomSaveTableNo(), getArg1_RoomNo());
                    }
                } else {
                    if (!getArg2_CngBitVal()) {
                        if (getArg0_RoomSaveTableNo() == -1) {
                            dComIfGs_offVisitedRoom(getArg1_RoomNo());
                        } else {
                            dComIfGs_offSaveVisitedRoom(getArg0_RoomSaveTableNo(),
                                                        getArg1_RoomNo());
                        }
                    }
                }
            }
        } else {
            if (!getArg3_CngBitFlg()) {
                if ((s32)getSwBit() == 0xff || !fopAcM_isSwitch(this, getSwBit())) {
                    if (getArg2_CngBitVal() == 1) {
                        if (getArg0_RoomSaveTableNo() == -1) {
                            dComIfGs_onVisitedRoom(getArg1_RoomNo());
                        } else {
                            dComIfGs_onSaveVisitedRoom(getArg0_RoomSaveTableNo(), getArg1_RoomNo());
                        }
                    } else {
                        if (!getArg2_CngBitVal()) {
                            if (getArg0_RoomSaveTableNo() == -1) {
                                dComIfGs_offVisitedRoom(getArg1_RoomNo());
                            } else {
                                dComIfGs_offSaveVisitedRoom(getArg0_RoomSaveTableNo(),
                                                            getArg1_RoomNo());
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}

void daTagRmbitSw_c::init() {
    field_0x56c = scale.x * 50.0f;
    field_0x570 = scale.y * 100.0f;
    field_0x574 = scale.z * 50.0f;
}

u8 daTagRmbitSw_c::chkPlyrInTag() {
    fopAc_ac_c* fopPos = dComIfGp_getPlayer(0);

    if (!fopPos) {
        return 0;
    } else {
        cXyz cPos = fopPos->current.pos - current.pos;
        mDoMtx_stack_c::YrotS(-current.angle.y);
        mDoMtx_stack_c::multVec(&cPos, &cPos);
        if ((0.0f < cPos.y) && (field_0x570 > cPos.y) && (field_0x56c > cPos.x) &&
            (field_0x574 > cPos.z) && (-field_0x56c < cPos.x) && (-field_0x574 < cPos.z))
        {
            return 1;
        }
    }
    return 0;
}

int daTagRmbitSw_create(daTagRmbitSw_c* i_this) {
    fopAcM_ct(i_this, daTagRmbitSw_c);
    return i_this->create();
}

static int daTagRmbitSw_Delete(daTagRmbitSw_c* i_this) {
    return i_this->Delete();
}

static int daTagRmbitSw_execute(daTagRmbitSw_c* i_this) {
    return i_this->execute();
}

static int daTagRmbitSw_draw(daTagRmbitSw_c* i_this) {
    return i_this->draw();
}

static actor_method_class daTagRmbitSw_METHODS = {
    (process_method_func)daTagRmbitSw_create,  (process_method_func)daTagRmbitSw_Delete,
    (process_method_func)daTagRmbitSw_execute, (process_method_func)NULL,
    (process_method_func)daTagRmbitSw_draw,

};

actor_process_profile_definition g_profile_Tag_RmbitSw = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_RmbitSw,
    &g_fpcLf_Method.base,
    sizeof(daTagRmbitSw_c),
    0,
    0,
    &g_fopAc_Method.base,
    748,
    &daTagRmbitSw_METHODS,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
