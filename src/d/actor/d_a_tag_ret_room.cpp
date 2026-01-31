/**
 * d_a_tag_ret_room.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_ret_room.h"

#include "d/actor/d_a_player.h"
#include "d/d_debug_viewer.h"

class daTagRetRm_HIO_c : public mDoHIO_entry_c {
public:
    daTagRetRm_HIO_c();

    void genMessage(JORMContext*);

    void setScale(const cXyz& i_scale) { scale = i_scale; };

    cXyz scale;
    u8 showTag;
};

#if DEBUG
daTagRetRm_HIO_c l_TAG_RET_ROOM_HIO;
#endif

daTagRetRm_HIO_c::daTagRetRm_HIO_c() {
    // empty function
}

#if DEBUG
void daTagRetRm_HIO_c::genMessage(JORMContext* ctx) {
    // "Return to room tag"
    ctx->genLabel("____________部屋戻しタグ____________", 0);
    // "Scale value X"
    ctx->genSlider("スケール値X", &scale.x, 0.0f, 25.0f);
    // "Scale value Y"
    ctx->genSlider("スケール値Y", &scale.y, 0.0f, 25.0f);
    // "Scale value Z"
    ctx->genSlider("スケール値Z", &scale.z, 0.0f, 25.0f);
    // "Display tag ON/OFF"
    ctx->genCheckBox("Tag表示ON/OFF", &showTag, TRUE);
}
#endif

daTagRetRm_c::daTagRetRm_c() {}

daTagRetRm_c::~daTagRetRm_c() {}

int daTagRetRm_c::create() {
    fopAcM_ct(this, daTagRetRm_c);
    init();

#if DEBUG
    // "Room return tag Lv7D"
    l_TAG_RET_ROOM_HIO.entryHIO("部屋戻しタグLv7D");
    l_TAG_RET_ROOM_HIO.setScale(scale);
    l_TAG_RET_ROOM_HIO.showTag = FALSE;
#endif

    return cPhs_COMPLEATE_e;
}

int daTagRetRm_c::Delete() {
#if DEBUG
    l_TAG_RET_ROOM_HIO.removeHIO();
#endif

    return 1;
}

int daTagRetRm_c::draw() {
#if DEBUG
    if (l_TAG_RET_ROOM_HIO.showTag == TRUE) {
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

bool daTagRetRm_c::chkPlyrInTag() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (player == NULL) {
        return false;
    }

    cXyz pos = player->current.pos - current.pos;
    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&pos, &pos);
    if (0 < pos.y && field_0x570 > pos.y && field_0x56c > pos.x && field_0x574 > pos.z &&
        -field_0x56c < pos.x && -field_0x574 < pos.z)
    {
        return true;
    } else {
        return false;
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
