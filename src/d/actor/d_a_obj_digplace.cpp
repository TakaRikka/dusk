/**
 * d_a_obj_digplace.cpp
 * Generic Wolf Digging Spots
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_digplace.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

int daObjDigpl_c::create() {
    fopAcM_ct(this, daObjDigpl_c);

    mType = (fopAcM_GetParam(this) >> 8) & 0xF;
    mSwitch = fopAcM_GetParam(this) & 0xFF;

    if (mType == 5) {
        fopAcM_OffStatus(this, 0x80);
        mpDigPoints =
            dPath_GetRoomPath((fopAcM_GetParam(this) >> 0x14) & 0xFF, fopAcM_GetRoomNo(this));
    }

    if (mType >= 4) {
        mType = 0;
    }

    if (mSwitch != 0xFF && fopAcM_isSwitch(this, mSwitch)) {
        return cPhs_ERROR_e;
    }

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_copy(mDoMtx_stack_c::get(), field_0x570);
    fopAcM_SetMtx(this, field_0x570);

    attention_info.position = current.pos;
    eyePos = attention_info.position;
    tevStr.room_no = dComIfGp_roomControl_getStayNo();
    fopAcM_SetMin(this, -550.0f, -250.0f, -550.0f);
    fopAcM_SetMax(this, 550.0f, 250.0f, 550.0f);
    attention_info.distances[fopAc_attn_ETC_e] = 27;

    return cPhs_COMPLEATE_e;
}

static int daObjDigpl_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjDigpl_c*>(i_this)->create();
}

daObjDigpl_c::~daObjDigpl_c() {}

static int daObjDigpl_Delete(daObjDigpl_c* i_this) {
    i_this->~daObjDigpl_c();
    return 1;
}

int daObjDigpl_c::execute() {
    daPy_py_c* player_p = daPy_getLinkPlayerActorClass();

    if (mDigFlg == 1) {
        if (mSwitch != 0xFF) {
            fopAcM_onSwitch(this, mSwitch);
        }

        cXyz item_pos(current.pos.x, current.pos.y - 30.0f, current.pos.z);

        u8 item_no;
        if (mpDigPoints != NULL) {
            item_no = mpDigPoints->m_points[mCurrentDigPoint].mArg1;
        } else {
            item_no = getItemNum();
        }

        if (mType == 0) {
            fopAcM_createItem(&item_pos, item_no, -1, fopAcM_GetRoomNo(this),
                              &player_p->shape_angle, NULL, 9);
        } else if (mType == 2) {
            fopAcM_createItemFromTable(&item_pos, item_no, -1, fopAcM_GetRoomNo(this),
                                       &player_p->shape_angle, 9, NULL, NULL, NULL, false);
        }

        if (mType == 1) {
            mDigFlg = 2;
        } else if (mpDigPoints != NULL) {
            mUsedDigFlags[mCurrentDigPoint >> 5] |= 1 << (mCurrentDigPoint % 32);
            mDigFlg = 0;
        } else {
            fopAcM_delete(this);
            return 1;
        }
    }

    attention_info.flags &= ~fopAc_AttnFlag_ETC_e;

    if (mpDigPoints != NULL) {
        current.pos.y = player_p->current.pos.y + 100000.0f;
        attention_info.position.y = current.pos.y;
        cXyz point_pos;

        dPnt* point_p = mpDigPoints->m_points;
        f32 dist_to_current_pnt = FLT_MAX;

        for (int i = 0; i < mpDigPoints->m_num; i++, point_p++) {
            if ((mUsedDigFlags[i >> 5] & (1 << (i % 32))) == 0) {
                point_pos.set(point_p->m_position.x, point_p->m_position.y, point_p->m_position.z);

                if (player_p->current.pos.abs(point_pos) < 1000.0f) {
                    dComIfGp_particle_setSimple(0x70F, &point_pos, 255, g_whiteColor, g_whiteColor,
                                                0, 0.0f);
                    dComIfGp_particle_setSimple(0x73D, &point_pos, 255, g_whiteColor, g_whiteColor,
                                                0, 0.0f);
                }

                f32 point_to_plyr_dist = player_p->current.pos.abs2XZ(point_pos);
                if (dist_to_current_pnt > point_to_plyr_dist &&
                    fabsf(point_pos.y - player_p->current.pos.y) < 200.0f)
                {
                    dist_to_current_pnt = point_to_plyr_dist;

                    mCurrentDigPoint = i;
                    current.pos = point_pos;
                    attention_info.position = point_pos;
                    eyePos = point_pos;

                    if (point_p->mArg0 == 0) {
                        mType = 0;
                    } else {
                        mType = 2;
                    }
                }
            }
        }
    }

    if (daPy_py_c::checkNowWolf() && mDigFlg == 0) {
        if (field_0x56b == 0) {
            int seen_angle = fopAcM_seenActorAngleY(player_p, this);
            f32 dist_to_player = fopAcM_searchPlayerDistanceXZ2(this);

            if (seen_angle <= 0x2800 || dist_to_player < 1600.0f) {
                attention_info.flags |= fopAc_AttnFlag_ETC_e;
            }

            if (dist_to_player < 250000.0f &&
                fabsf(current.pos.y - player_p->current.pos.y) < 200.0f)
            {
                daPy_py_c::setLookPos(&attention_info.position);
            }
        }

        if (mpDigPoints == NULL && player_p->current.pos.abs(current.pos) < 1000.0f) {
            dComIfGp_particle_setSimple(0x70F, &current.pos, 255, g_whiteColor, g_whiteColor, 0,
                                        0.0f);
            dComIfGp_particle_setSimple(0x73D, &current.pos, 255, g_whiteColor, g_whiteColor, 0,
                                        0.0f);
        }
    }

    return 1;
}

static int daObjDigpl_Execute(daObjDigpl_c* i_this) {
    return i_this->execute();
}

static int daObjDigpl_Draw(daObjDigpl_c* i_this) {
    return 1;
}

static actor_method_class l_daObjDigpl_Method = {
    (process_method_func)daObjDigpl_Create,  (process_method_func)daObjDigpl_Delete,
    (process_method_func)daObjDigpl_Execute, (process_method_func)NULL,
    (process_method_func)daObjDigpl_Draw,
};

actor_process_profile_definition g_profile_Obj_Digpl = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Digpl,
    &g_fpcLf_Method.base,
    sizeof(daObjDigpl_c),
    0,
    0,
    &g_fopAc_Method.base,
    250,
    &l_daObjDigpl_Method,
    0x44080,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
