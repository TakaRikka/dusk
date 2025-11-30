/**
 * d_a_tag_hinit.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_hinit.h"
#include "f_op/f_op_actor_mng.h"

int daTagHinit_c::create() {
    fopAcM_ct(this, daTagHinit_c);

    field_0x569 = shape_angle.x;

    if (field_0x569 != 0xFF && fopAcM_isSwitch(this, field_0x569)) {
        return cPhs_ERROR_e;
    }

    field_0x568 = (shape_angle.x >> 8) & 0xFF;
    field_0x56c = fopAcM_GetParam(this);
    field_0x56e = (fopAcM_GetParam(this) >> 0x10);

    return cPhs_COMPLEATE_e;
}

static int daTagHinit_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagHinit_c*>(i_this)->create();
}

daTagHinit_c::~daTagHinit_c() {}

static int daTagHinit_Delete(daTagHinit_c* i_this) {
    i_this->~daTagHinit_c();
    return 1;
}

int daTagHinit_c::execute() {
    if ((field_0x56c == 0xFFFF ||
         dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[field_0x56c])) &&
        (field_0x56e == 0xFFFF ||
         !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[field_0x56e])) &&
        (field_0x568 == 0xFF || fopAcM_isSwitch(this, field_0x568)))
    {
        if (field_0x569 != 0xFF) {
            fopAcM_onSwitch(this, field_0x569);
        }

        daHorse_c* horse = dComIfGp_getHorseActor();
        horse->setHorsePosAndAngle(&current.pos, shape_angle.y);
        horse->offNoDrawWait();
        fopAcM_delete(this);
    }

    return 1;
}

static int daTagHinit_Execute(daTagHinit_c* i_this) {
    return i_this->execute();
}

static int daTagHinit_Draw(daTagHinit_c*) {
    return 1;
}

static actor_method_class l_daTagHinit_Method = {
    (process_method_func)daTagHinit_Create,  (process_method_func)daTagHinit_Delete,
    (process_method_func)daTagHinit_Execute, (process_method_func)NULL,
    (process_method_func)daTagHinit_Draw,

};

extern actor_process_profile_definition g_profile_Tag_Hinit = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Tag_Hinit,
    &g_fpcLf_Method.base,
    sizeof(daTagHinit_c),
    0,
    0,
    &g_fopAc_Method.base,
    433,
    &l_daTagHinit_Method,
    0x60000,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
