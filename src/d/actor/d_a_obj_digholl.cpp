/**
 * d_a_obj_digholl.cpp
 * Object - Dig Hole
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_digholl.h"
#include "d/actor/d_a_player.h"
#include "f_op/f_op_actor_mng.h"

int daObjDigholl_c::create() {
    fopAcM_ct(this, daObjDigholl_c);
    field_0x569 = fopAcM_GetParam(this);
    field_0x56a = fopAcM_GetParam(this) >> 8;
    field_0x56c = fopAcM_GetParam(this) >> 0x10 & 0xff;
    if (field_0x56c == 0xff) {
        field_0x56c = -1;
    }
    if (fopAcM_isSwitch(this, field_0x569)) {
        field_0x568 = 2;
        field_0x56b = 1;
    }
    fopAcM_SetMin(this, -60.0f, 0.0f, -100.0f);
    fopAcM_SetMax(this, 60.0f, 10.0f, 100.0f);
    attention_info.position = current.pos;
    eyePos = attention_info.position;
    attention_info.distances[fopAc_attn_ETC_e] = 0x20;
    return cPhs_COMPLEATE_e;
}

static int daObjDigholl_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjDigholl_c*>(i_this)->create();
}

daObjDigholl_c::~daObjDigholl_c() {}

static int daObjDigholl_Delete(daObjDigholl_c* i_this) {
    i_this->~daObjDigholl_c();
    return 1;
}

int daObjDigholl_c::execute() {
    daPy_py_c* player = daPy_getLinkPlayerActorClass();
    if (player->checkNowWolf() &&
        (field_0x56a == 0xff || fopAcM_isSwitch(this, field_0x56a)) &&
        (f32)fabsf(current.pos.y - player->current.pos.y) < 40.0f)
    {
        attention_info.flags = fopAc_AttnFlag_ETC_e;
    } else {
        attention_info.flags = 0;
    }
    if (field_0x568 == 1) {
        if (cLib_distanceAngleS(player->shape_angle.y, shape_angle.y) < 0x6000) {
            shape_angle.y = player->shape_angle.y + 0x10000 - 0x8000;
            current.angle.y = shape_angle.y;
        }
        fopAcM_onSwitch(this, field_0x569);

        if (dComIfGp_event_runCheck()) {
            if (player->getBaseAnimeFrame() > 35.0f) {
                field_0x56b = 1;
            }
        } else {
            field_0x568 = 2;
            field_0x56b = 1;
        }
    } else {
        if ((player->current.pos.abs2XZ(current.pos) < 250000.0f) &&
            (fabsf(current.pos.y - player->current.pos.y) < 200.0f))
        {
            daPy_py_c::setLookPos(&attention_info.position);
        }
    }

    if (player->current.pos.abs(current.pos) < 1000.0f) {
        dComIfGp_particle_setSimple(0x70f, &current.pos, 0xff, g_whiteColor, g_whiteColor, NULL,
                                    0.0f);
        dComIfGp_particle_setSimple(0x73d, &current.pos, 0xff, g_whiteColor, g_whiteColor, NULL,
                                    0.0f);
    }

    return 1;
}

static int daObjDigholl_Execute(daObjDigholl_c* i_this) {
    return i_this->execute();
}

int daObjDigholl_c::draw() {
    return 1;
}

static int daObjDigholl_Draw(daObjDigholl_c* i_this) {
    return i_this->draw();
}

static actor_method_class l_daObjDigholl_Method = {
    (process_method_func)daObjDigholl_Create,  (process_method_func)daObjDigholl_Delete,
    (process_method_func)daObjDigholl_Execute, (process_method_func)NULL,
    (process_method_func)daObjDigholl_Draw,
};

actor_process_profile_definition g_profile_Obj_Digholl = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Digholl,
    &g_fpcLf_Method.base,
    sizeof(daObjDigholl_c),
    0,
    0,
    &g_fopAc_Method.base,
    249,
    &l_daObjDigholl_Method,
    0x44100,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
