/**
 * d_a_tag_mist.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_mist.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

void daTagMist_c::offSw() {
    u8 sw_no_num;
    u8 top_sw_no;

    top_sw_no = daTagMist_prm::getTopSwNo(this);
    sw_no_num = daTagMist_prm::getSwNoNum(this);

    for (int i = 0; i < sw_no_num; i++) {
        fopAcM_offSwitch(this, top_sw_no + i);
    }
}

void daTagMist_c::onMySw() {
    u32 my_sw_no;
    u32 top_sw_no;

    top_sw_no = daTagMist_prm::getTopSwNo(this);
    my_sw_no = daTagMist_prm::getMySwNo(this);

    if (my_sw_no != 0xFF) {
        fopAcM_onSwitch(this, top_sw_no + my_sw_no);
    }
}

BOOL daTagMist_c::isMySw() {
    u32 my_sw_no;
    u32 top_sw_no;

    top_sw_no = daTagMist_prm::getTopSwNo(this);
    my_sw_no = daTagMist_prm::getMySwNo(this);

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

    mVertices[3].y = 0.0f;
    mVertices[2].y = 0.0f;
    mVertices[1].y = 0.0f;
    mVertices[0].y = 0.0f;

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

        home.angle.z = 0;
        home.angle.x = 0;
        current.angle.z = 0;
        current.angle.x = 0;
        shape_angle.z = 0;
        shape_angle.x = 0;
    }

    if (!Create()) {
        return cPhs_ERROR_e;
    }

    return cPhs_COMPLEATE_e;
}

int daTagMist_c::execute() {
    if (fopAcM_isSwitch(this, daTagMist_prm::getSwNo2(this))) {
        return 1;
    }

    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    cXyz player_dist(player_p->current.pos);
    player_dist -= home.pos;

    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&player_dist, &player_dist);

    u8 player_inside = (mVertices[0].x < player_dist.x && mVertices[0].z < player_dist.z) &&
                       (mVertices[2].x > player_dist.x && mVertices[2].z > player_dist.z);

    if (player_inside && !mPlayerTouched) {
        offSw();
        onMySw();
        mPlayerNo = daTagMist_prm::getExitNo(this);
    }

    mPlayerTouched = player_inside;
    return 1;
}

int daTagMist_c::_delete() {
    return 1;
}

static int daTagMist_Execute(daTagMist_c* i_this) {
    return i_this->execute();
}

static int daTagMist_Delete(daTagMist_c* i_this) {
    return i_this->_delete();
}

static int daTagMist_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagMist_c*>(i_this)->create();
}

static actor_method_class l_daTagMist_Method = {
    (process_method_func)daTagMist_Create,
    (process_method_func)daTagMist_Delete,
    (process_method_func)daTagMist_Execute,
    (process_method_func)NULL,
    (process_method_func)NULL,
};

extern actor_process_profile_definition g_profile_Tag_Mist = {
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
