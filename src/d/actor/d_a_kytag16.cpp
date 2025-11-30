/**
 * d_a_kytag16.cpp
 * Pikari Tag
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag16.h"
#include "SSystem/SComponent/c_counter.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static int daKytag16_Draw(kytag16_class* i_this) {
    return 1;
}

static int daKytag16_Execute(kytag16_class* i_this) {
    bool var_r29 = false;
    f32 var_f31 = 0.0f;
    f32 var_f30 = 0.0f;

    if (i_this->field_0x571 < 31) {
        if (i_this->field_0x571 > i_this->field_0x572) {
            if (dKy_getdaytime_hour() >= i_this->field_0x571 ||
                dKy_getdaytime_hour() <= i_this->field_0x572)
            {
                var_r29 = true;
            }
        } else if (dKy_getdaytime_hour() >= i_this->field_0x571 &&
                   dKy_getdaytime_hour() <= i_this->field_0x572)
        {
            var_r29 = true;
        }
    } else {
        var_r29 = true;
    }

    if (var_r29) {
        var_r29 = false;

        if (i_this->mSwitch != 0xFF) {
            if (dComIfGs_isSwitch(i_this->mSwitch, dComIfGp_roomControl_getStayNo())) {
                var_r29 = true;
            }
        } else {
            var_r29 = true;
        }
    }

    if (var_r29) {
        if (strcmp(dComIfGp_getStartStageName(), "F_SP117") != 0) {
            cLib_addCalc(&i_this->field_0x568, 1.0f, 0.5f, 0.1f, 0.0001f);
        } else {
            var_f31 = cM_ssin(i_this->field_0x588);
            var_f30 = cM_scos(i_this->field_0x58a);

            i_this->field_0x588 += 0x500;
            i_this->field_0x58a += 800;

            cLib_addCalc(&i_this->field_0x568, 0.9f + (0.1f * cM_ssin(g_Counter.mCounter0 * 0x500)),
                         0.5f, 0.1f, 0.0001f);
            i_this->field_0x580 = 40.0f;
        }
    } else {
        cLib_addCalc(&i_this->field_0x568, 0.0f, 0.5f, 0.05f, 0.0001f);
    }

    if (i_this->field_0x568 > 0.001f) {
        dKy_BossSpotLight_set(&i_this->current.pos, i_this->field_0x578 + (12.0f * var_f31),
                              i_this->field_0x57c + (50.0f * var_f30), i_this->field_0x580,
                              &i_this->mColor, i_this->field_0x574 * i_this->field_0x568,
                              i_this->field_0x58c, i_this->field_0x58d);
    }

    return 1;
}

static int daKytag16_IsDelete(kytag16_class* i_this) {
    return 1;
}

static int daKytag16_Delete(kytag16_class* i_this) {
    return 1;
}

static int daKytag16_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, kytag16_class);
    kytag16_class* a_this = (kytag16_class*)i_this;

    a_this->mSwitch = fopAcM_GetParam(a_this) & 0xFF;
    a_this->field_0x56c = (a_this->current.angle.z >> 10) & 0x3F;

    a_this->mColor.r = fopAcM_GetParam(a_this) >> 0x8;
    a_this->mColor.g = fopAcM_GetParam(a_this) >> 0x10;
    a_this->mColor.b = fopAcM_GetParam(a_this) >> 0x18;

    a_this->field_0x571 = a_this->current.angle.z & 0x1F;
    a_this->field_0x572 = (a_this->current.angle.z >> 5) & 0x1F;
    a_this->field_0x578 = (a_this->shape_angle.x / 32767.0f) * 180.0f;
    a_this->field_0x57c = (a_this->shape_angle.y / 32767.0f) * 180.0f;

    a_this->field_0x588 = cM_rndFX(65535.0f);
    a_this->field_0x58a = cM_rndFX(65535.0f);
    a_this->field_0x584 = 1.0f;
    a_this->field_0x568 = 0.0f;

    switch (a_this->field_0x56c) {
    case 0:
        a_this->field_0x580 = 45.0f;
        a_this->field_0x574 = 1.0f;
        a_this->field_0x58c = 0;
        a_this->field_0x58d = 3;
        break;
    case 1:
        a_this->field_0x580 = 45.0f;
        a_this->field_0x574 = 5.0f;
        a_this->field_0x58c = 0;
        a_this->field_0x58d = 3;
        break;
    case 2:
        a_this->field_0x580 = 45.0f;
        a_this->field_0x574 = 10.0f;
        a_this->field_0x58c = 0;
        a_this->field_0x58d = 3;
        break;
    case 3:
        a_this->field_0x580 = 25.0f;
        a_this->field_0x574 = 1.0f;
        a_this->field_0x58c = 2;
        a_this->field_0x58d = 3;
        break;
    case 4:
        a_this->field_0x580 = 45.0f;
        a_this->field_0x574 = 5.0f;
        a_this->field_0x58c = 2;
        a_this->field_0x58d = 3;
        break;
    case 5:
        a_this->field_0x580 = 70.0f;
        a_this->field_0x574 = 10.0f;
        a_this->field_0x58c = 2;
        a_this->field_0x58d = 3;
        break;
    case 6:
        a_this->field_0x580 = 60.0f;
        a_this->field_0x574 = 0.01f;
        a_this->field_0x58c = 2;
        a_this->field_0x58d = 1;
        break;
    case 7:
        a_this->field_0x580 = 50.0f;
        a_this->field_0x574 = 0.01f;
        a_this->field_0x58c = 2;
        a_this->field_0x58d = 1;
        break;
    }

    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daKytag16_Method = {
    (process_method_func)daKytag16_Create,  (process_method_func)daKytag16_Delete,
    (process_method_func)daKytag16_Execute, (process_method_func)daKytag16_IsDelete,
    (process_method_func)daKytag16_Draw,
};

extern actor_process_profile_definition g_profile_KYTAG16 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KYTAG16,
    &g_fpcLf_Method.base,
    sizeof(kytag16_class),
    0,
    0,
    &g_fopAc_Method.base,
    110,
    &l_daKytag16_Method,
    0x60000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
