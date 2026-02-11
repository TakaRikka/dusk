/**
 * d_a_tag_mist.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_mist.h"
#include "d/d_com_inf_game.h"
#include "d/d_debug_viewer.h"
#include "d/d_procname.h"

class daTagMist_HIO_c : public mDoHIO_entry_c {
public:
    daTagMist_HIO_c();

    void genMessage(JORMContext*);

    u8 show_range;
};

#if DEBUG
daTagMist_HIO_c l_HIO;

daTagMist_HIO_c::daTagMist_HIO_c() {
    show_range = 0;
}

void daTagMist_HIO_c::genMessage(JORMContext* ctx) {
    // "Fog range tag"
    ctx->genLabel("霧の範囲タグ", 0);
    // "Show range"
    ctx->genCheckBox("範囲表示", &show_range, 1);
}
#endif

void daTagMist_c::offSw() {
    u8 top_sw_no = daTagMist_prm::getTopSwNo(this);
    u8 my_sw_no = daTagMist_prm::getMySwNo(this);
    u8 sw_no_num = daTagMist_prm::getSwNoNum(this);

    for (int i = 0; i < sw_no_num; i++) {
        fopAcM_offSwitch(this, top_sw_no + i);
    }
}

void daTagMist_c::onMySw() {
    u8 top_sw_no = daTagMist_prm::getTopSwNo(this);
    u8 my_sw_no = daTagMist_prm::getMySwNo(this);

    if (my_sw_no != 0xFF) {
        fopAcM_onSwitch(this, top_sw_no + my_sw_no);
    }
}

BOOL daTagMist_c::isMySw() {
    u8 top_sw_no = daTagMist_prm::getTopSwNo(this);
    u8 my_sw_no = daTagMist_prm::getMySwNo(this);

    if (my_sw_no != 0xFF && fopAcM_isSwitch(this, top_sw_no + my_sw_no)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int daTagMist_c::Create() {
    mVertices[0].x = scale.x * -50.0f;
    mVertices[0].z = scale.z * -50.0f;

    mVertices[1].x = scale.x * 50.0f;
    mVertices[1].z = scale.z * -50.0f;

    mVertices[2].x = scale.x * 50.0f;
    mVertices[2].z = scale.z * 50.0f;

    mVertices[3].x = scale.x * -50.0f;
    mVertices[3].z = scale.z * 50.0f;

    mVertices[0].y = mVertices[1].y = mVertices[2].y = mVertices[3].y = 0.0f;

    if (isMySw()) {
        setPlayerNo(daTagMist_prm::getExitNo(this));
    }

    mPlayerTouched = false;
    return 1;
}

int daTagMist_c::create() {
    fopAcM_ct(this, daTagMist_c);

    if (!mInitParams) {
        mInitParams = true;

        field_0x5a2 = home.angle.x;
        field_0x5a4 = home.angle.z;

        home.angle.x = home.angle.z = 0;
        current.angle.x = current.angle.z = 0;
        shape_angle.x = shape_angle.z = 0;
    }

    if (!Create()) {
        return cPhs_ERROR_e;
    }

#if DEBUG
    // "Fog range tag"
    l_HIO.entryHIO("霧の範囲タグ");
#endif

    return cPhs_COMPLEATE_e;
}

int daTagMist_c::execute() {
    u8 sw_no_2 = daTagMist_prm::getSwNo2(this);
    if (fopAcM_isSwitch(this, sw_no_2)) {
        return 1;
    }

    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    cXyz player_dist(player_p->current.pos);
    player_dist -= home.pos;

    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&player_dist, &player_dist);

    u8 player_inside = mVertices[0].x < player_dist.x && mVertices[0].z < player_dist.z &&
                       mVertices[2].x > player_dist.x && mVertices[2].z > player_dist.z;

    if (player_inside && !mPlayerTouched) {
        offSw();
        onMySw();
        setPlayerNo(daTagMist_prm::getExitNo(this));
    }

    mPlayerTouched = player_inside;
    return 1;
}

int daTagMist_c::draw() {
#if DEBUG
    if (l_HIO.show_range) {
        GXColor color = {0, 0, 0x80, 0x80};
        cXyz sp10[8];

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(current.angle.y);

        sp10[0] = mVertices[0];
        sp10[1] = mVertices[1];
        sp10[2] = mVertices[3];
        sp10[3] = mVertices[2];
        sp10[4] = mVertices[0];
        sp10[5] = mVertices[1];
        sp10[6] = mVertices[3];
        sp10[7] = mVertices[2];

        for (int i = 0; i < 8; i++) {
            mDoMtx_stack_c::multVec(&sp10[i], &sp10[i]);
        }

        for (int i = 0; i < 4; i++) {
            sp10[i].y += 1000.0f;
        }

        dDbVw_drawCube8pXlu(sp10, color);
    }
#endif

    return 1;
}

int daTagMist_c::_delete() {
#if DEBUG
    l_HIO.removeHIO();
#endif
    return 1;
}

static int daTagMist_Draw(daTagMist_c* i_this) {
    return i_this->draw();
}

static int daTagMist_Execute(daTagMist_c* i_this) {
    return i_this->execute();
}

static int daTagMist_Delete(daTagMist_c* i_this) {
    int id = fopAcM_GetID(i_this);
    return i_this->_delete();
}

static int daTagMist_Create(fopAc_ac_c* i_this) {
    daTagMist_c* mist = static_cast<daTagMist_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return mist->create();
}

static actor_method_class l_daTagMist_Method = {
    (process_method_func)daTagMist_Create,
    (process_method_func)daTagMist_Delete,
    (process_method_func)daTagMist_Execute,
    (process_method_func)NULL,
#if DEBUG
    (process_method_func)daTagMist_Draw,
#else
    (process_method_func)NULL,
#endif
};

actor_process_profile_definition g_profile_Tag_Mist = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_Mist,
    &g_fpcLf_Method.base,
    sizeof(daTagMist_c),
    0,
    0,
    &g_fopAc_Method.base,
    292,
    &l_daTagMist_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
