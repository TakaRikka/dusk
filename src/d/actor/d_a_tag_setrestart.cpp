/**
 * d_a_tag_setrestart.cpp
 * RMBack0
 * Sets player room restart info
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_setrestart.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

int daTagRestart_c::Create() {
    field_0x56c[0].x = scale.x * -100.0f;
    field_0x56c[0].z = scale.z * -100.0f;

    field_0x56c[1].x = scale.x * 100.0f;
    field_0x56c[1].z = scale.z * -100.0f;

    field_0x56c[2].x = scale.x * 100.0f;
    field_0x56c[2].z = scale.z * 100.0f;

    field_0x56c[3].x = scale.x * -100.0f;
    field_0x56c[3].z = scale.z * 100.0f;

    field_0x56c[3].y = 0.0f;
    field_0x56c[2].y = 0.0f;
    field_0x56c[1].y = 0.0f;
    field_0x56c[0].y = 0.0f;

    mPlayerRoomNo = -1;
    return 1;
}

int daTagRestart_c::create() {
    fopAcM_ct(this, daTagRestart_c);

    if (!Create()) {
        return cPhs_ERROR_e;
    }

    return cPhs_COMPLEATE_e;
}

int daTagRestart_c::execute() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    cXyz player_pos = player->current.pos;
    player_pos -= home.pos;
    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&player_pos, &player_pos);

    if (field_0x56c[0].x < player_pos.x && field_0x56c[0].z < player_pos.z &&
        field_0x56c[2].x > player_pos.x && field_0x56c[2].z > player_pos.z)
    {
        int roomNo = dComIfGp_roomControl_getStayNo();
        if (roomNo != mPlayerRoomNo) {
            cXyz pos_offset;
            s16 restart_angle;
            if (roomNo == daTagRestart_prm::getFRoomNo(this)) {
                restart_angle = home.angle.y;
                pos_offset.set(0.0f, 0.0f, 100.0f);
            } else {
                restart_angle = home.angle.y + 0x8000;
                pos_offset.set(0.0f, 0.0f, -100.0f);
            }

            pos_offset.y += 100.0f;
            mDoMtx_stack_c::YrotS(home.angle.y);
            mDoMtx_stack_c::multVec(&pos_offset, &pos_offset);
            cXyz restart_pos = current.pos + pos_offset;

            if (fopAcM_gc_c::gndCheck(&restart_pos)) {
                restart_pos.y = fopAcM_gc_c::getGroundY();
            }

            dComIfGs_setRestartRoom(restart_pos, restart_angle, fopAcM_GetRoomNo(player));
        }
    }

    mPlayerRoomNo = fopAcM_GetRoomNo(player);
    return 1;
}

int daTagRestart_c::_delete() {
    return 1;
}

static int daTagRestart_Execute(daTagRestart_c* i_this) {
    return i_this->execute();
}

static int daTagRestart_Delete(daTagRestart_c* i_this) {
    return i_this->_delete();
}

static int daTagRestart_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagRestart_c*>(i_this)->create();
}

static actor_method_class l_daTagRestart_Method = {
    (process_method_func)daTagRestart_Create,
    (process_method_func)daTagRestart_Delete,
    (process_method_func)daTagRestart_Execute,
    (process_method_func)NULL,
    (process_method_func)NULL,
};

actor_process_profile_definition g_profile_Tag_Restart = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_Restart,
    &g_fpcLf_Method.base,
    sizeof(daTagRestart_c),
    0,
    0,
    &g_fopAc_Method.base,
    592,
    &l_daTagRestart_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
