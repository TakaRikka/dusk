/**
 * d_a_kytag02.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag02.h"
#include "f_op/f_op_actor_mng.h"

/* -80855AC0 000078 0008+00 1/0 0/0 0/0 .text            daKytag02_Draw__FP13kytag02_class
 */
static int daKytag02_Draw(kytag02_class* i_this) {
    return 1;
}

static int daKytag02_Execute(kytag02_class* i_this) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    cXyz* wind_vec = dKyw_get_wind_vec();

    if (dComIfGs_isSwitch(i_this->field_0x577, i_this->home.roomNo)) {
        dKyw_evt_wind_set_go();
        dKyw_evt_wind_set(0, i_this->current.angle.y);

        if (i_this->field_0x578 != 99) {
            env_light->TeachWind_existence = 1;
            env_light->mMoyaMode = 4;
            env_light->mMoyaCount = 50;

            switch (i_this->field_0x576) {
            case 0:
                if (i_this->field_0x574 <= 0) {
                    env_light->custom_windpower = 0.27f;
                    i_this->field_0x574 = 60;
                    mDoAud_mEnvse_setWindType(2);
                    i_this->field_0x576++;
                }
                break;
            case 1:
                if (i_this->field_0x574 <= 0) {
                    env_light->custom_windpower = 0.40499997f;
                    i_this->field_0x574 = 90;
                    mDoAud_mEnvse_setWindType(1);
                    i_this->field_0x576++;
                }
                break;
            case 2:
                if (i_this->field_0x574 <= 0) {
                    env_light->custom_windpower = 0.0f;
                    i_this->field_0x574 = 90;
                    i_this->field_0x576 = 0;
                    mDoAud_mEnvse_setWindType(3);
                }
                break;
            }

            if (i_this->field_0x574 != 0) {
                i_this->field_0x574--;
            }
        } else {
            env_light->TeachWind_existence = 0xFF;
            env_light->custom_windpower = 0.3f;
        }
    } else {
        env_light = dKy_getEnvlight();
        env_light->evt_wind_go = 0;

        if (i_this->field_0x578 != 99) {
            env_light->mMoyaCount = 0;
        }

        env_light->TeachWind_existence = 0;
    }

    if (i_this->field_0x578 != 99) {
        mDoAud_mEnvse_setWindDirection(wind_vec);

        s8 reverb = dComIfGp_getReverb(dComIfGp_roomControl_getStayNo());
        mDoAud_mEnvse_startStrongWindSe(reverb);
    }

    return 1;
}

static int daKytag02_IsDelete(kytag02_class* i_this) {
    return 1;
}

static int daKytag02_Delete(kytag02_class* i_this) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();

    env_light->evt_wind_go = 0;
    env_light->mMoyaCount = 0;
    env_light->TeachWind_existence = 0;
    return 1;
}

static int daKytag02_Create(fopAc_ac_c* i_this) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    dKyw_get_wind_vec();

    fopAcM_ct(i_this, kytag02_class);
    kytag02_class* a_this = (kytag02_class*)i_this;

    a_this->field_0x574 = 0;
    a_this->field_0x576 = 0;
    a_this->field_0x577 = a_this->current.angle.x & 0xFF;

    env_light->custom_windpower = 0.0f;

    a_this->field_0x578 = fopAcM_GetParam(a_this) & 0xFF;

    if (dComIfGs_isSwitch(a_this->field_0x577, a_this->home.roomNo)) {
        dKyw_evt_wind_set_go();
        dKyw_evt_wind_set(0, a_this->current.angle.y);

        if (a_this->field_0x578 == 99) {
            env_light->TeachWind_existence = 0xFF;
        } else {
            env_light->TeachWind_existence = 1;
            env_light->mMoyaMode = 4;
            env_light->mMoyaCount = 50;
        }
    }

    if (a_this->field_0x578 != 99) {
        mDoAud_mEnvse_initStrongWind();
    }

    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daKytag02_Method = {
    (process_method_func)daKytag02_Create,  (process_method_func)daKytag02_Delete,
    (process_method_func)daKytag02_Execute, (process_method_func)daKytag02_IsDelete,
    (process_method_func)daKytag02_Draw,
};

actor_process_profile_definition g_profile_KYTAG02 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KYTAG02,
    &g_fpcLf_Method.base,
    sizeof(kytag02_class),
    0,
    0,
    &g_fopAc_Method.base,
    96,
    &l_daKytag02_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
