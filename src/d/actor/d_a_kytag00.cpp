/**
 * d_a_kytag00.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag00.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_camera_mng.h"

static cXyz get_check_pos(kytag00_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    camera_class* camera_p = (camera_class*)dComIfGp_getCamera(0);
    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    cXyz pos;

    if (player_p != NULL) {
        f32 dist_to_cameye = a_this->current.pos.abs(camera_p->lookat.eye);
        f32 dist_to_player = a_this->current.pos.abs(player_p->current.pos);

        pos = player_p->current.pos;
        if (dComIfGp_event_runCheck() && dist_to_cameye < dist_to_player) {
            pos = camera_p->lookat.eye;
        }
    } else {
        pos.set(0.0f, 0.0f, 0.0f);
    }

    return pos;
}

static void wether_tag_reset(kytag00_class* i_this) {
    i_this->field_0x56d = 0;

    g_env_light.mColpatPrevGather = g_env_light.mColpatWeather;
    g_env_light.mColpatCurrGather = g_env_light.mColpatWeather;
    g_env_light.mColPatBlendGather = 0.0f;
    g_env_light.mColPatModeGather = 1;
}

static void wether_tag_move(kytag00_class* i_this) {
    cXyz spC;
    cXyz check_pos = get_check_pos(i_this);

    f32 temp_f31 = i_this->field_0x57c * 100.0f;

    spC.x = check_pos.x;
    spC.y = i_this->current.pos.y;
    spC.z = check_pos.z;
    f32 var_f1 = i_this->current.pos.abs(spC);

    if (var_f1 < i_this->field_0x584 && check_pos.y >= i_this->current.pos.y - temp_f31 &&
        check_pos.y < temp_f31 + (i_this->current.pos.y + (5000.0f * i_this->scale.y)) &&
        i_this->field_0x588 > 0.0f)
    {
        f32 var_f8;
        f32 var_f9;
        var_f9 = 1.0f;
        var_f8 = 1.0f;

        f32 temp_f10 = i_this->field_0x584 - i_this->field_0x580;
        if (0.0f != temp_f10) {
            var_f9 = (i_this->field_0x584 - var_f1) / temp_f10;
            if (var_f9 > var_f8) {
                var_f9 = var_f8;
            }
        }

        if (check_pos.y <= i_this->current.pos.y) {
            if (0.0f != temp_f31) {
                f32 var_f2 = (i_this->current.pos.y - check_pos.y) / temp_f31;
                if (var_f2 < 0.0f) {
                    var_f2 = 0.0f;
                }
                var_f8 = 1.0f - var_f2;
            }
        } else if (0.0f != temp_f31) {
            f32 var_f2_2 =
                (check_pos.y - (i_this->current.pos.y + (5000.0f * i_this->scale.y))) / temp_f31;
            if (var_f2_2 < 0.0f) {
                var_f2_2 = 0.0f;
            }
            var_f8 = 1.0f - var_f2_2;
        }

        var_f9 *= (var_f8 * i_this->field_0x588);
        if (g_env_light.PrevCol == g_env_light.UseCol) {
            i_this->field_0x56d = 1;

            switch (i_this->field_0x56f) {
            case 0:
                g_env_light.mColpatPrevGather = g_env_light.mColpatWeather;
                g_env_light.mColpatCurrGather = 0;
                g_env_light.mColPatBlendGather = var_f9;
                g_env_light.mColPatModeGather = 1;
                break;
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
            case 13:
            case 14:
            case 15:
                if (var_f9 > 0.5f) {
                    g_env_light.mColpatPrevGather = g_env_light.mColpatWeather;
                    g_env_light.mColpatCurrGather = i_this->field_0x56f;
                    g_env_light.mColPatBlendGather = var_f9;
                    if (var_f9 >= 1.0f) {
                        g_env_light.mColpatPrevGather = i_this->field_0x56f;
                    }
                } else {
                    g_env_light.mColpatPrevGather = i_this->field_0x56f;
                    g_env_light.mColpatCurrGather = g_env_light.mColpatWeather;
                    g_env_light.mColPatBlendGather = 1.0f - var_f9;
                }

                g_env_light.mColPatModeGather = 1;
                break;
            default:
                i_this->field_0x56d = 0;
                break;
            }
        }
    } else if (i_this->field_0x56d != 0) {
        wether_tag_reset(i_this);
    }
}

static void raincnt_set(f32 param_0) {
    int cnt = param_0 * param_0 * param_0 * 250.0f;
    if (cnt > g_env_light.base_raincnt) {
        g_env_light.raincnt = cnt;
    }
}

static void raincnt_cut() {
    g_env_light.raincnt = g_env_light.base_raincnt;
}

static void wether_tag_efect_reset(kytag00_class* i_this) {
    i_this->field_0x56c = 0;

    switch (i_this->field_0x570) {
    case 1:
        raincnt_cut();
        break;
    case 2:
    case 16:
        g_env_light.mSnowCount = 0;
        g_env_light.mMoyaCount = 0;
        break;
    case 3:
    case 14:
        g_env_light.mHousiCount = 0;
        g_env_light.field_0xea9 = 0;
        break;
    case 4:
    case 5:
    case 6:
        if (g_env_light.mThunderEff.mMode == 2) {
            g_env_light.mThunderEff.mMode = 0;
        }

        if (i_this->field_0x570 == 8 || i_this->field_0x570 == 9) {
            raincnt_cut();
        }

        if (i_this->field_0x570 == 9) {
            g_env_light.mMoyaCount = 0;
        }
        break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 17:
    case 18:
        g_env_light.mMoyaCount = 0;
        break;
    case 15:
        g_env_light.field_0x1048 = 0;
        break;
    }
}

static void wether_tag_efect_move(kytag00_class* i_this) {
    cXyz spC;
    cXyz check_pos = get_check_pos(i_this);

    f32 temp_f31 = i_this->field_0x57c * 100.0f;

    spC.x = check_pos.x;
    spC.y = i_this->current.pos.y;
    spC.z = check_pos.z;
    f32 var_f1 = i_this->current.pos.abs(spC);

    if (var_f1 < i_this->field_0x584 && check_pos.y >= i_this->current.pos.y - temp_f31 &&
        check_pos.y < temp_f31 + (i_this->current.pos.y + (5000.0f * i_this->scale.y)) &&
        i_this->field_0x588 > 0.0f)
    {
        f32 var_f9 = 1.0f;
        f32 var_f8 = 1.0f;

        i_this->field_0x56c = 1;

        f32 temp_f10 = i_this->field_0x584 - i_this->field_0x580;
        if (0.0f != temp_f10) {
            var_f9 = (i_this->field_0x584 - var_f1) / temp_f10;
            if (var_f9 > var_f8) {
                var_f9 = var_f8;
            }
        }

        if (check_pos.y <= i_this->current.pos.y) {
            if (0.0f != temp_f31) {
                f32 var_f2 = (i_this->current.pos.y - check_pos.y) / temp_f31;
                if (var_f2 < 0.0f) {
                    var_f2 = 0.0f;
                }
                var_f8 = 1.0f - var_f2;
            }
        } else if (0.0f != temp_f31) {
            f32 var_f2_2 =
                (check_pos.y - (i_this->current.pos.y + (5000.0f * i_this->scale.y))) / temp_f31;
            if (var_f2_2 < 0.0f) {
                var_f2_2 = 0.0f;
            }
            var_f8 = 1.0f - var_f2_2;
        }

        var_f9 *= (var_f8 * i_this->field_0x588);

        switch (i_this->field_0x570) {
        case 1:
            raincnt_set(var_f9);
            break;
        case 2:
            if (i_this->home.roomNo == dComIfGp_roomControl_getStayNo()) {
                g_env_light.field_0xe92 = false;

                if (g_env_light.mSnowCount < (int)(var_f9 * 500.0f)) {
                    g_env_light.mSnowCount += (int)(var_f9 * 500.0f * 0.05f);

                    if (g_env_light.mSnowCount > (int)(var_f9 * 500.0f)) {
                        g_env_light.mSnowCount = (int)(var_f9 * 500.0f);
                    }
                }

                g_env_light.mMoyaMode = 0;
                if (g_env_light.mMoyaCount < (int)(var_f9 * 50.0f)) {
                    g_env_light.mMoyaCount += (int)(var_f9 * 50.0f * 0.05f);

                    if (g_env_light.mMoyaCount > (int)(var_f9 * 50.0f)) {
                        g_env_light.mMoyaCount = (int)(var_f9 * 50.0f);
                    }
                }
            } else {
                g_env_light.mSnowCount = 0;
                g_env_light.mMoyaCount = 0;
            }

            mDoAud_setSnowPower(var_f9 * 63.0f);
            break;
        case 3:
        case 14:
            g_env_light.field_0xea9 = 0;

            if (i_this->home.roomNo == dComIfGp_roomControl_getStayNo()) {
                g_env_light.mHousiCount = var_f9 * 300.0f;
            } else {
                g_env_light.mHousiCount = 0;
            }

            if (i_this->field_0x570 == 14) {
                g_env_light.field_0xea9 = 1;
            }
            break;
        case 4:
            if (g_env_light.mThunderEff.mMode == 0) {
                g_env_light.mThunderEff.mMode = 2;
            }
            break;
        case 5:
            if (g_env_light.mThunderEff.mMode == 0) {
                g_env_light.mThunderEff.mMode = 2;
            }

            raincnt_set(var_f9);
            break;
        case 6:
            g_env_light.mMoyaMode = 1;
            g_env_light.mMoyaCount = var_f9 * 50.0f;
            raincnt_set(var_f9);

            if (g_env_light.mThunderEff.mMode == 0) {
                g_env_light.mThunderEff.mMode = 2;
            }
            break;
        case 7:
            g_env_light.mMoyaMode = 0;
            g_env_light.mMoyaCount = var_f9 * 50.0f;
            break;
        case 8:
            g_env_light.mMoyaMode = 1;
            g_env_light.mMoyaCount = var_f9 * 50.0f;
            break;
        case 9:
            g_env_light.mMoyaMode = 2;
            g_env_light.mMoyaCount = var_f9 * 50.0f;
            break;
        case 10:
            g_env_light.mMoyaMode = 3;
            g_env_light.mMoyaCount = var_f9 * 50.0f;
            break;
        case 11:
            g_env_light.mMoyaMode = 50;
            g_env_light.mMoyaCount = var_f9 * 50.0f * g_env_light.senses_effect_strength;
            break;
        case 12:
            g_env_light.mMoyaMode = 5;
            g_env_light.mMoyaCount = var_f9 * 50.0f;
            break;
        case 13:
            g_env_light.mMoyaMode = 6;
            g_env_light.mMoyaCount = var_f9 * 50.0f;
            break;
        case 15:
            g_env_light.field_0x1048 = var_f9 * 100.0f;
            break;
        case 16:
            if (i_this->home.roomNo == dComIfGp_roomControl_getStayNo()) {
                g_env_light.field_0xe92 = true;

                if (g_env_light.mSnowCount < (int)(var_f9 * 500.0f)) {
                    g_env_light.mSnowCount += (int)(var_f9 * 500.0f * 0.05f);

                    if (g_env_light.mSnowCount > (int)(var_f9 * 500.0f)) {
                        g_env_light.mSnowCount = (int)(var_f9 * 500.0f);
                    }
                }

                g_env_light.mMoyaMode = 0;
                if (g_env_light.mMoyaCount < (int)(var_f9 * 50.0f)) {
                    g_env_light.mMoyaCount += (int)(var_f9 * 50.0f * 0.05f);

                    if (g_env_light.mMoyaCount > (int)(var_f9 * 50.0f)) {
                        g_env_light.mMoyaCount = (int)(var_f9 * 50.0f);
                    }
                }
            } else {
                g_env_light.mSnowCount = 0;
                g_env_light.mMoyaCount = 0;
            }

            mDoAud_setSnowPower(var_f9 * 127.0f);
            break;
        case 17:
            g_env_light.mMoyaMode = 8;
            g_env_light.mMoyaCount = var_f9 * 50.0f;
            break;
        case 18:
            g_env_light.mMoyaMode = 9;
            g_env_light.mMoyaCount = var_f9 * 50.0f;
            break;
        }
    } else if (i_this->field_0x56c != 0) {
        wether_tag_efect_reset(i_this);
    }
}

static int daKytag00_Draw(kytag00_class* i_this) {
    wether_tag_efect_move(i_this);
    return 1;
}

static int daKytag00_Execute(kytag00_class* i_this) {
    BOOL var_r30 = TRUE;

    if (i_this->field_0x571 != 0xFF && dComIfGs_isSwitch(i_this->field_0x571, dComIfGp_roomControl_getStayNo())) {
        var_r30 = FALSE;
    } else if (i_this->field_0x572 != 0xFF && !dComIfGs_isSwitch(i_this->field_0x572, dComIfGp_roomControl_getStayNo())) {
        var_r30 = FALSE;
    }

    if (var_r30 == TRUE) {
        cLib_addCalc(&i_this->field_0x588, 1.0f, 0.1f, 0.01f, 0.0001f);
    } else {
        cLib_addCalc(&i_this->field_0x588, 0.0f, 0.1f, 0.01f, 0.0001f);
    }

    wether_tag_move(i_this);
    return 1;
}

static int daKytag00_IsDelete(kytag00_class* i_this) {
    return 1;
}

static int daKytag00_Delete(kytag00_class* i_this) {
    wether_tag_reset(i_this);
    wether_tag_efect_reset(i_this);
    g_env_light.mMoyaCount = 0;
    return 1;
}

static int daKytag00_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, kytag00_class);
    kytag00_class* a_this = (kytag00_class*)i_this;

    a_this->field_0x56e = 0;
    a_this->field_0x56f = fopAcM_GetParam(a_this) & 0xFF;
    a_this->field_0x570 = (fopAcM_GetParam(a_this) >> 8) & 0xFF;
    a_this->field_0x578 = (fopAcM_GetParam(a_this) >> 0x10) & 0xFF;
    a_this->field_0x57c = (fopAcM_GetParam(a_this) >> 0x18) & 0xFF;
    a_this->field_0x571 = a_this->current.angle.x & 0xFF;
    a_this->field_0x572 = (a_this->current.angle.x >> 8) & 0xFF;

    BOOL var_r30 = TRUE;

    if (a_this->field_0x571 != 0xFF && dComIfGs_isSwitch(a_this->field_0x571, dComIfGp_roomControl_getStayNo())) {
        var_r30 = FALSE;
    } else if (a_this->field_0x572 != 0xFF && !dComIfGs_isSwitch(a_this->field_0x572, dComIfGp_roomControl_getStayNo())) {
        var_r30 = FALSE;
    }

    if (var_r30 == TRUE) {
        a_this->field_0x588 = 1.0f;
    } else {
        a_this->field_0x588 = 0.0f;
    }

    if (a_this->field_0x578 == 0xFF) {
        a_this->field_0x578 = 10;
    }

    if (a_this->field_0x57c == 0xFF) {
        a_this->field_0x57c = 10;
    }

    a_this->field_0x580 = a_this->scale.x * 5000.0f;
    a_this->field_0x584 = a_this->scale.x * 5000.0f + a_this->field_0x578 * 100.0f;
    a_this->field_0x56c = 0;
    a_this->field_0x56d = 0;
    g_env_light.mMoyaCount = 0;

    wether_tag_efect_move(a_this);
    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daKytag00_Method = {
    (process_method_func)daKytag00_Create,  (process_method_func)daKytag00_Delete,
    (process_method_func)daKytag00_Execute, (process_method_func)daKytag00_IsDelete,
    (process_method_func)daKytag00_Draw,
};

extern actor_process_profile_definition g_profile_KYTAG00 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KYTAG00,
    &g_fpcLf_Method.base,
    sizeof(kytag00_class),
    0,
    0,
    &g_fopAc_Method.base,
    94,
    &l_daKytag00_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
