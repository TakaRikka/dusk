/**
 * d_a_tag_rmbit_sw.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_rmbit_sw.h"

#include "d/d_debug_viewer.h"
#include "d/d_procname.h"

class daTagRmbitSw_HIO_c : public mDoHIO_entry_c {
public:
    daTagRmbitSw_HIO_c();

    void setScale(const cXyz& i_scale);

    void genMessage(JORMContext*);

    cXyz scale;
    u8 show_tag;
};

#if DEBUG
daTagRmbitSw_HIO_c l_RMBIT_HIO;

daTagRmbitSw_HIO_c::daTagRmbitSw_HIO_c() {
    // empty function
}

void daTagRmbitSw_HIO_c::setScale(const cXyz& i_scale) {
    scale = i_scale;
}

void daTagRmbitSw_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("____________部屋戻しタグ____________", 0);
    ctx->genSlider("スケール値X", &scale.x, 0.0f, 25.0f);
    ctx->genSlider("スケール値Y", &scale.y, 0.0f, 25.0f);
    ctx->genSlider("スケール値Z", &scale.z, 0.0f, 25.0f);
    ctx->genCheckBox("Tag表示ON/OFF", &show_tag, 0x1);
}
#endif

daTagRmbitSw_c::daTagRmbitSw_c() {}

daTagRmbitSw_c::~daTagRmbitSw_c() {}

int daTagRmbitSw_c::create() {
    fopAcM_ct(this, daTagRmbitSw_c);

    init();

#if DEBUG
    // "Destination room bit change switch"
    l_RMBIT_HIO.entryHIO("到達部屋ビット変更スイッチ");
    l_RMBIT_HIO.setScale(scale);
    l_RMBIT_HIO.show_tag = FALSE;
#endif

    if (getArg1_RoomNo() == -1) {
        JUT_WARN(165, "%s", " VisitedRoomNo == -1 !!!!!\n");
    }

    return cPhs_COMPLEATE_e;
}

int daTagRmbitSw_c::Delete() {
#if DEBUG
    l_RMBIT_HIO.removeHIO();
#endif

    return 1;
}

int daTagRmbitSw_c::draw() {
#if DEBUG
    if (l_RMBIT_HIO.show_tag == TRUE) {
        GXColor color = {0xff, 0, 0, 0x80};
        cXyz sp10[8];

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(current.angle.y);

        sp10[0].set(-field_0x56c, 0.0f, -field_0x574);
        sp10[1].set(-field_0x56c, 0.0f, field_0x574);
        sp10[2].set(field_0x56c, 0.0f, -field_0x574);
        sp10[3].set(field_0x56c, 0.0f, field_0x574);
        sp10[4].set(-field_0x56c, field_0x570, -field_0x574);
        sp10[5].set(-field_0x56c, field_0x570, field_0x574);
        sp10[6].set(field_0x56c, field_0x570, -field_0x574);
        sp10[7].set(field_0x56c, field_0x570, field_0x574);

        for (int i = 0; i < ARRAY_SIZE(sp10); i++) {
            mDoMtx_stack_c::multVec(&sp10[i], &sp10[i]);
        }

        dDbVw_drawCube8pXlu(sp10, color);
    }
#endif

    return 1;
}

int daTagRmbitSw_c::execute() {
#if DEBUG
    field_0x56c = l_RMBIT_HIO.scale.x * 50.0f;
    field_0x570 = l_RMBIT_HIO.scale.y * 100.0f;
    field_0x574 = l_RMBIT_HIO.scale.z * 50.0f;
#endif

    if (chkPlyrInTag()) {
        if (getArg3_CngBitFlg() == 1) {
            if (getSwBit() == 0xff || fopAcM_isSwitch(this, getSwBit())) {
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

bool daTagRmbitSw_c::chkPlyrInTag() {
    fopAc_ac_c* fopPos = dComIfGp_getPlayer(0);

    if (!fopPos) {
        return false;
    }

    cXyz cPos = fopPos->current.pos - current.pos;
    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&cPos, &cPos);
    if (0.0f < cPos.y && field_0x570 > cPos.y && field_0x56c > cPos.x &&
        field_0x574 > cPos.z && -field_0x56c < cPos.x && -field_0x574 < cPos.z)
    {
        return true;
    }
    return false;
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
