//
// Tag: Event Area
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_evtarea.h"
#include "d/actor/d_a_npc.h"
#include "d/d_procname.h"

cPhs__Step daTag_EvtArea_c::create() {
    fopAcM_ct(this, daTag_EvtArea_c);

    switch (getType()) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        scale.x *= 100.0f;
        scale.y *= 100.0f;
        scale.z *= 100.0f;
        break;
    case 13:
        scale.x *= 200.0f;
        scale.y *= 200.0f;
        scale.z *= 200.0f;
        break;
    case 14:
        scale.x *= 100.0f;
        scale.y *= 100.0f;
        scale.z *= 100.0f;
        break;
    case 15:
        scale.x *= 0.5f;
        scale.z *= 0.5f;
        scale.x *= 1000.0f;
        scale.y *= 1000.0f;
        scale.z *= 1000.0f;
        break;
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        scale.x *= 100.0f;
        scale.y *= 100.0f;
        scale.z *= 100.0f;
        break;
    case 21:
        scale.x *= 1000.0f;
        scale.z *= 1000.0f;
        break;
    case 22:
    case 23:
        scale.x *= 100.0f;
        scale.y *= 100.0f;
        scale.z *= 100.0f;
        break;
    default:
        scale.x *= 100.0f;
        scale.y *= 100.0f;
        scale.z *= 100.0f;
    }

    // clang-format off
    OS_REPORT("\t(%s:%d) no:%d, onEvt:%d, offEvt:%d, BitSW:%02x, BitSW2:%02x, scale:%f,%f,%f<%08x>\n",
              fopAcM_getProcNameString(this),
              getType(),
              getNo(),
              getOnEvtBit(),
              getOffEvtBit(),
              getBitSW(),
              getBitSW2(),
              scale.x, scale.y, scale.z,
              fopAcM_GetParam(this)
             );
    // clang-format on

    if (isDelete()) {
        return cPhs_ERROR_e;
    }

    field_0x56c = 0;
    return cPhs_COMPLEATE_e;
}

BOOL daTag_EvtArea_c::Delete() {
    return true;
}

BOOL daTag_EvtArea_c::Execute() {
    if (home.roomNo == dComIfGp_roomControl_getStayNo()) {
        if (isDelete()) {
            fopAcM_delete(this);
            return true;
        }

        attention_info.position = current.pos;
        if (getType() == 8) {
            attention_info.position.y += 100.0f;
        }

        fopAcM_SetGroup(this, 0);
        eyePos = attention_info.position;

        if (getType() == 15) {
            if (chkPointInArea(daPy_getPlayerActorClass()->current.pos)) {
                eyePos = daPy_getPlayerActorClass()->eyePos;
                fopAcM_SetGroup(this, 4);
            }
        }

        attention_info.flags = 0;
        return true;
    }

    return false;
}

BOOL daTag_EvtArea_c::Draw() {
    return true;
}

BOOL daTag_EvtArea_c::isDelete() {
    bool ret;
    if (getOffEvtBit() != -1) {
        if (daNpcT_chkEvtBit(getOffEvtBit())) {
            return true;
        }
    }

    ret = false;
    if (getBitSW2() != 0xFF) {
        if (dComIfGs_isSwitch(getBitSW2(), fopAcM_GetRoomNo(this))) {
            ret = true;
        }
    }

    return ret;
}

BOOL daTag_EvtArea_c::chkPointInArea(cXyz i_point, cXyz i_addSize) {
    cXyz area_vtx[4];
    cXyz size, center, player_to_home, point_to_current;

    if (field_0x56c == 0) {
        if ((getOnEvtBit() != -1 && daNpcT_chkEvtBit(getOnEvtBit())) ||
            (getBitSW() != 0xff && dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) ||
            (getOnEvtBit() == -1 && getBitSW() == 0xFF))
        {
            size = scale + i_addSize;

            if (getType() == 15 || getType() == 16) {
                player_to_home = daPy_getPlayerActorClass()->current.pos;
                player_to_home -= home.pos;
                mDoMtx_YrotS(mDoMtx_stack_c::now, -current.angle.y);
                mDoMtx_stack_c::multVec(&player_to_home, &player_to_home);

                // bottom left
                area_vtx[0].x = -size.x;
                area_vtx[0].z = -size.z;

                // bottom right
                area_vtx[1].x = size.x;
                area_vtx[1].z = -size.z;

                // top right
                area_vtx[2].x = size.x;
                area_vtx[2].z = size.z;

                // top left
                area_vtx[3].x = -size.x;
                area_vtx[3].z = size.z;

                if (area_vtx[0].x < player_to_home.x && area_vtx[0].z < player_to_home.z &&
                    area_vtx[2].x > player_to_home.x && area_vtx[2].z > player_to_home.z)
                {
                    point_to_current = i_point - current.pos;
                    if (0 <= (int)point_to_current.y && (int)point_to_current.y < (int)size.y) {
                        return true;
                    }
                }
            } else {
                center = current.pos;
                center.y -= 10.0f;
                if (getType() == 21) {
                    return daNpcT_chkPointInArea(i_point, center, size, shape_angle.y, 0);
                }

                return daNpcT_chkPointInArea(i_point, center, size, shape_angle.y, 1);
            }
        }
    }

    return false;
}

static cPhs__Step daTag_EvtArea_Create(void* i_this) {
    return static_cast<daTag_EvtArea_c*>(i_this)->create();
}

static BOOL daTag_EvtArea_Delete(void* i_this) {
    return static_cast<daTag_EvtArea_c*>(i_this)->Delete();
}

static BOOL daTag_EvtArea_Execute(void* i_this) {
    return static_cast<daTag_EvtArea_c*>(i_this)->Execute();
}

static BOOL daTag_EvtArea_Draw(void* i_this) {
    return static_cast<daTag_EvtArea_c*>(i_this)->Draw();
}

static BOOL daTag_EvtArea_IsDelete(void* i_this) {
    return true;
}

static actor_method_class daTag_EvtArea_MethodTable = {
    (process_method_func)daTag_EvtArea_Create,  (process_method_func)daTag_EvtArea_Delete,
    (process_method_func)daTag_EvtArea_Execute, (process_method_func)daTag_EvtArea_IsDelete,
    (process_method_func)daTag_EvtArea_Draw,
};

actor_process_profile_definition g_profile_TAG_EVTAREA = {
    fpcLy_CURRENT_e,
    8,
    fpcPi_CURRENT_e,
    PROC_TAG_EVTAREA,
    &g_fpcLf_Method.base,
    sizeof(daTag_EvtArea_c),
    0,
    0,
    &g_fopAc_Method.base,
    287,
    &daTag_EvtArea_MethodTable,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
