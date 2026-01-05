/**
 * d_a_kytag06.cpp
 * Weather System Manager
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag06.h"
#include "SSystem/SComponent/c_counter.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "f_op/f_op_camera_mng.h"
#include "Z2AudioLib/Z2Instances.h"

static void dice_wether_init(u8 i_weatherMode, f32 i_weatherTime, f32 i_currentTime) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();

    env_light->dice_wether_mode = i_weatherMode;
    env_light->dice_wether_time =
        i_currentTime + i_weatherTime + cM_rndF(i_weatherTime) + cM_rndF(i_weatherTime);

    if (env_light->dice_wether_time >= 360.0f) {
        env_light->dice_wether_time -= 360.0f;
    }
}

static void dice_wether_execute(u8 i_weatherMode, f32 i_weatherTime, f32 i_currentTime) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    env_light->dice_wether_mode = i_weatherMode;

    if (env_light->dice_wether_mode != DICE_MODE_DONE_e) {
        env_light->dice_wether_time =
            i_currentTime + i_weatherTime + cM_rndF(i_weatherTime) + cM_rndF(i_weatherTime);

        if (env_light->dice_wether_time >= 360.0f) {
            env_light->dice_wether_time -= 360.0f;
        }

        env_light->dice_wether_counter++;
    } else {
        env_light->dice_wether_mode = DICE_MODE_SUNNY_e;
        env_light->dice_wether_state++;
    }
}

static void dice_rain_minus() {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();

    if ((g_Counter.mCounter0 & 3) == 0) {
        // run block every 4 frames
        if (env_light->raincnt > 40) {
            env_light->raincnt -= 3;
        } else if (env_light->raincnt != 0) {
            env_light->raincnt--;
        }

        dKyw_rain_set(env_light->raincnt);
    }
}

static void dKy_event_proc() {
    static f32 S_time_table[8] = {45.0f, 75.0f, 120.0f, 150.0f, 180.0f, 240.0f, 270.0f, 360.0f};
    static u8 S_wether_table[8] = {0, 1, 2, 3, 4, 0, 1, 2};

    static u8 S_wether_mode_pat01[4] = {1, 2, 1, 0xFF};
    static f32 S_wether_time_pat01[3] = {7.5f, 7.5f, 7.5f};

    static u8 S_wether_mode_pat02[4] = {1, 4, 1, 0xFF};
    static f32 S_wether_time_pat02[3] = {5.0f, 15.0f, 5.0f};

    static u8 S_wether_mode_pat03[4] = {1, 3, 1, 0xFF};
    static f32 S_wether_time_pat03[3] = {7.5f, 15.0f, 7.5f};

    static u8 S_wether_mode_pat04[2] = {1, 0xFF};
    static f32 S_wether_time_pat04[1] = {30.0f};

    static u8 S_wether_mode_pat05[4] = {1, 5, 1, 0xFF};
    static f32 S_wether_time_pat05[3] = {3.75f, 7.5f, 3.75f};

    dScnKy_env_light_c* env_light = dKy_getEnvlight();

    if (!g_env_light.camera_water_in_status) {
        if (g_env_light.light_init_timer == 0) {
            f32 current_time = dComIfGs_getTime();

            switch (env_light->dice_wether_state) {
            case DICE_STATE_RESET_e:
                if (current_time > env_light->dice_wether_change_time &&
                    current_time - env_light->dice_wether_change_time < 15.0f)
                {
                    env_light->dice_wether_state = DICE_STATE_INIT_e;
                }
                break;
            case DICE_STATE_INIT_e: {
                u8 table_no = cM_rndF(12.99f);
                if (table_no >= 8) {
                    env_light->dice_wether_state = DICE_STATE_NEXT_e;
                } else {
                    env_light->dice_wether_pat = S_wether_table[table_no];
                    env_light->dice_wether_counter = 0;

                    switch (env_light->dice_wether_pat) {
                    case 0:
                        dice_wether_init(S_wether_mode_pat01[env_light->dice_wether_counter],
                                         S_wether_time_pat01[env_light->dice_wether_counter],
                                         current_time);
                        break;
                    case 1:
                        dice_wether_init(S_wether_mode_pat02[env_light->dice_wether_counter],
                                         S_wether_time_pat02[env_light->dice_wether_counter],
                                         current_time);
                        break;
                    case 2:
                        dice_wether_init(S_wether_mode_pat03[env_light->dice_wether_counter],
                                         S_wether_time_pat03[env_light->dice_wether_counter],
                                         current_time);
                        break;
                    case 3:
                        dice_wether_init(S_wether_mode_pat04[env_light->dice_wether_counter],
                                         S_wether_time_pat04[env_light->dice_wether_counter],
                                         current_time);
                        break;
                    case 4:
                        dice_wether_init(S_wether_mode_pat05[env_light->dice_wether_counter],
                                         S_wether_time_pat05[env_light->dice_wether_counter],
                                         current_time);
                        break;
                    }

                    env_light->dice_wether_counter++;
                    env_light->dice_wether_state++;
                }
                break;
            }
            case DICE_STATE_EXEC_e:
                if (current_time > env_light->dice_wether_time &&
                    current_time - env_light->dice_wether_time < 180.0f)
                {
                    switch (env_light->dice_wether_pat) {
                    case 0:
                        dice_wether_execute(S_wether_mode_pat01[env_light->dice_wether_counter],
                                            S_wether_time_pat01[env_light->dice_wether_counter],
                                            current_time);
                        break;
                    case 1:
                        dice_wether_execute(S_wether_mode_pat02[env_light->dice_wether_counter],
                                            S_wether_time_pat02[env_light->dice_wether_counter],
                                            current_time);
                        break;
                    case 2:
                        dice_wether_execute(S_wether_mode_pat03[env_light->dice_wether_counter],
                                            S_wether_time_pat03[env_light->dice_wether_counter],
                                            current_time);
                        break;
                    case 3:
                        dice_wether_execute(S_wether_mode_pat04[env_light->dice_wether_counter],
                                            S_wether_time_pat04[env_light->dice_wether_counter],
                                            current_time);
                        break;
                    case 4:
                        dice_wether_execute(S_wether_mode_pat05[env_light->dice_wether_counter],
                                            S_wether_time_pat05[env_light->dice_wether_counter],
                                            current_time);
                        break;
                    }
                }
                break;
            case DICE_STATE_NEXT_e:
                u8 time_table_no = cM_rndF(7.99f);
                env_light->dice_wether_change_time = current_time + S_time_table[time_table_no];

                if (env_light->dice_wether_change_time >= 360.0f) {
                    env_light->dice_wether_change_time -= 360.0f;
                }

                env_light->dice_wether_state = DICE_STATE_RESET_e;
                break;
            }

            if (g_env_light.field_0x130b == 1) {
                env_light->dice_wether_mode = DICE_MODE_UNK6_e;
            }

            if (g_env_light.mColPatMode == 0 && g_env_light.mColPatModeGather == 0) {
                u8 weather_colpat;

                switch (env_light->dice_wether_mode) {
                case DICE_MODE_SUNNY_e:
                    weather_colpat = 0;
                    if (g_env_light.mThunderEff.mMode == 1) {
                        g_env_light.mThunderEff.mMode = 0;
                    }

                    dice_rain_minus();
                    break;
                case DICE_MODE_CLOUDY_e:
                    g_env_light.mThunderEff.mMode = 0;
                    weather_colpat = 1;
                    dice_rain_minus();
                    break;
                case DICE_MODE_RAIN_LIGHT_e:
                    weather_colpat = 1;
                    if (env_light->raincnt < 40) {
                        env_light->raincnt++;
                        dKyw_rain_set(env_light->raincnt);
                    } else {
                        env_light->raincnt--;
                        dKyw_rain_set(env_light->raincnt);
                    }
                    break;
                case DICE_MODE_THUNDER_HEAVY_e:
                    g_env_light.mThunderEff.mMode = 1;
                case DICE_MODE_RAIN_HEAVY_e:
                    weather_colpat = 2;
                    if (env_light->raincnt < 250) {
                        env_light->raincnt++;
                        dKyw_rain_set(env_light->raincnt);
                    }
                    break;
                case DICE_MODE_THUNDER_LIGHT_e:
                    weather_colpat = 1;
                    g_env_light.mThunderEff.mMode = 1;
                    dice_rain_minus();
                    break;
                case DICE_MODE_UNK6_e:
                    weather_colpat = 0;
                    if (g_env_light.mThunderEff.mMode == 1) {
                        g_env_light.mThunderEff.mMode = 0;
                    }

                    if (env_light->raincnt > 2) {
                        env_light->raincnt -= 2;
                    } else {
                        env_light->raincnt = 0;
                    }

                    dKyw_rain_set(env_light->raincnt);
                }

                if (env_light->mColpatWeather != weather_colpat) {
                    g_env_light.mColpatWeather = weather_colpat;
                    g_env_light.mColpatCurrGather = weather_colpat;
                }
            }

            if (g_env_light.mColPatMode == 0 && g_env_light.mColPatModeGather == 0 &&
                g_env_light.mColpatCurrGather != 0xFF &&
                g_env_light.wether_pat1 != g_env_light.mColpatCurrGather)
            {
                g_env_light.mColPatBlendGather = 0.0f;
            }
        }
    }
}

static void daKytag06_type_04_Execute(kytag06_class*) {
    dKy_event_proc();
}

static dPath* set_path_info(fopAc_ac_c* i_this) {
    kytag06_class* a_this = (kytag06_class*)i_this;
    dPath* room_path = NULL;

    if (a_this->mPathNo != 0xFF) {
        room_path = dPath_GetRoomPath(a_this->mPathNo, fopAcM_GetRoomNo(a_this));
    }

    return room_path;
}

static dPnt* near_rail_get(kytag06_class* i_this, cXyz* i_pos) {
    f32 nearest_dist = 100000000.0f;
    dPath* path = i_this->mpPath;
    dPnt* pnt;

    for (int i = 0; i < path->m_num; i++) {
        f32 pnt_dist = i_pos->abs(path->m_points[i].m_position);

        if (pnt_dist < nearest_dist) {
            nearest_dist = pnt_dist;
            pnt = &path->m_points[i];
        }
    }

    return pnt;
}

static int daKytag06_Draw(kytag06_class* i_this) {
    if (i_this->mType == 4) {
        daKytag06_type_04_Execute(i_this);
    }

    return 1;
}

static void daKytag06_lv7_boss_wether_proc(kytag06_class* i_this, u8 param_1) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    g_env_light.mThunderEff.mMode = 0;

    u8 weather_colpat = g_env_light.mColpatWeather;
    switch (param_1) {
    case 0:
        weather_colpat = 0;
        if (env_light->raincnt > 20) {
            env_light->raincnt -= 4;
        } else if (env_light->raincnt != 0) {
            env_light->raincnt--;
        }
        dKyw_rain_set(env_light->raincnt);
        break;
    case 1:
        weather_colpat = 1;
        dice_rain_minus();
        break;
    case 2:
        weather_colpat = 2;
        if (env_light->raincnt < 250) {
            env_light->raincnt++;
            dKyw_rain_set(env_light->raincnt);
        }

        g_env_light.mThunderEff.mMode = 1;
        break;
    }

    if (env_light->mColpatWeather != weather_colpat && !g_env_light.camera_water_in_status) {
        g_env_light.mColpatWeather = weather_colpat;
        dKy_change_colpat(weather_colpat);
    }
}

static void daKytag06_Ganon_wether_proc(kytag06_class* i_this, u8 param_1) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();

    g_env_light.mThunderEff.mMode = 0;
    dKyw_evt_wind_set(0, -0x4000);

    switch (param_1) {
    case 0:
        g_env_light.mMoyaMode = 10;
        g_env_light.mMoyaCount = 50;

        cLib_addCalc(&i_this->mWindPower, 0.3f, 0.25f, 0.01f, 1e-05f);
        dKyw_custom_windpower(i_this->mWindPower);
        break;
    case 1:
        g_env_light.mThunderEff.mMode = 1;
        g_env_light.mMoyaMode = 10;
        g_env_light.mMoyaCount = 50;

        cLib_addCalc(&i_this->mWindPower, 0.7f, 0.25f, 0.01f, 1e-05f);
        dKyw_custom_windpower(i_this->mWindPower);
        break;
    case 2:
        g_env_light.mThunderEff.mMode = 0;
        g_env_light.mMoyaMode = 10;
        g_env_light.mMoyaCount = 50;

        cLib_addCalc(&i_this->mWindPower, 0.7f, 0.25f, 0.01f, 1e-05f);
        dKyw_custom_windpower(i_this->mWindPower);
        break;
    case 3:
        g_env_light.mMoyaCount = 50;

        switch (i_this->mMode) {
        case 0:
            g_env_light.mMoyaMode = 10;
            cLib_addCalc(&i_this->mWindPower, 0.0f, 0.5f, 0.5f, 0.01f);

            if (i_this->mWindPower < 0.01f) {
                i_this->mMode++;
            }
            break;
        case 1:
            g_env_light.mMoyaMode = 11;
            cLib_addCalc(&i_this->mWindPower, 1.0f, 0.5f, 0.5f, 1e-05f);

            if (i_this->mWindPower > 0.99f) {
                i_this->mMode++;
            }
            break;
        case 2:
            cLib_addCalc(&i_this->mWindPower, 0.0f, 0.1f, 0.015f, 1e-05f);

            if (i_this->mWindPower < 0.01f) {
                i_this->mMode++;
            }
            break;
        case 3:
            i_this->mMode = 0;
            break;
        }

        dKyw_custom_windpower(i_this->mWindPower);
        break;
    }

    if (dComIfGp_event_runCheck()) {
        g_env_light.mThunderEff.mMode = 1;
    }
}

static void daKytag06_wether_proc(kytag06_class* i_this, u8 param_1) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    g_env_light.mThunderEff.mMode = 0;

    u8 weather_colpat = env_light->mColpatWeather;
    switch (param_1) {
    case 0:
        weather_colpat = 0;
        dice_rain_minus();

        if (env_light->mSnowCount != 0) {
            env_light->mSnowCount--;
        }
        break;
    case 1:
        weather_colpat = 1;
        dice_rain_minus();

        if (env_light->mSnowCount != 0) {
            env_light->mSnowCount--;
        }
        break;
    case 2:
        weather_colpat = 1;

        if ((g_Counter.mCounter0 & 3) == 0) {
            // run block every 4 frames
            if (env_light->raincnt < 40) {
                env_light->raincnt++;
                dKyw_rain_set(env_light->raincnt);
            } else {
                env_light->raincnt--;
                dKyw_rain_set(env_light->raincnt);
            }
        }

        if (env_light->mSnowCount != 0) {
            env_light->mSnowCount--;
        }
        break;
    case 3:
        weather_colpat = 1;

        if (env_light->raincnt < 250) {
            env_light->raincnt++;
            dKyw_rain_set(env_light->raincnt);
        }

        if (env_light->mSnowCount != 0) {
            env_light->mSnowCount--;
        }
        break;
    case 4:
        g_env_light.mThunderEff.mMode = 1;
        weather_colpat = 1;

        if (env_light->raincnt < 250) {
            env_light->raincnt++;
            dKyw_rain_set(env_light->raincnt);
        }

        if (env_light->mSnowCount != 0) {
            env_light->mSnowCount--;
        }
        break;
    case 5:
        weather_colpat = 1;
        g_env_light.mThunderEff.mMode = 1;

        dice_rain_minus();

        if (env_light->mSnowCount != 0) {
            env_light->mSnowCount--;
        }
        break;
    case 6:
        weather_colpat = 1;

        if (env_light->mSnowCount < 125) {
            if ((g_Counter.mCounter0 & 3) == 0) {
                // run block every 4 frames
                env_light->mSnowCount++;
            }
        } else {
            env_light->mSnowCount--;
        }
        break;
    case 7:
        weather_colpat = 2;

        if (env_light->mSnowCount < 500) {
            env_light->mSnowCount++;
        }
        break;
    }

    if (env_light->mColpatWeather != weather_colpat && !g_env_light.camera_water_in_status) {
        g_env_light.mColpatWeather = weather_colpat;
        dKy_change_colpat(weather_colpat);
    }
}

static void daKytag06_type_07_wether_Execute(kytag06_class* i_this) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    daKytag06_wether_proc(i_this, env_light->wether);
}

static void daKytag06_type_07_Execute(kytag06_class* i_this) {
    cXyz spX;
    f32 current_time = dComIfGs_getTime();
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    camera_class* camera = dComIfGp_getCamera(0);

    if (i_this->field_0x574 == 0) {
        if (current_time > 285.0f || current_time < 82.5f) {
            i_this->field_0x591 = 1;
        } else {
            i_this->field_0x591 = 0;
        }
    }

    switch (i_this->mMode) {
    case 0:
        if (i_this->field_0x591 != 0) {
            cLib_addCalc(&i_this->mWindPower, 0.8f, 0.5f, 1.0f, 0.0001f);

            if (i_this->mWindPower > 0.79f) {
                i_this->mMode++;
            }
        }
        break;
    case 1:
        cLib_addCalc(&i_this->mWindPower, 0.6f, 0.5f, 1.5f, 0.0001f);

        if (i_this->mWindPower < 0.61f) {
            i_this->mMode++;
        }
        break;
    case 2:
        cLib_addCalc(&i_this->mWindPower, 1.0f, 0.5f, 1.5f, 0.0001f);

        if (i_this->mWindPower > 0.99f) {
            i_this->mWindPower = 1.0f;
            i_this->mMode++;
        }
        break;
    case 3:
        if (i_this->field_0x591 == 0) {
            cLib_addCalc(&i_this->mWindPower, 0.0f, 0.5f, 1.0f, 0.0001f);

            if (i_this->mWindPower < 0.01f) {
                i_this->mWindPower = 0.0f;
                i_this->mMode = 0;
            }
        }
        break;
    }

    spX.x = -273.0f;
    spX.y = 233.0f;
    spX.z = 160.0f;

    GXColor color;
    color.r = 88;
    color.g = 76;
    color.b = 50;
    color.a = 255;

    daKytag06_type_07_wether_Execute(i_this);

    if (camera != NULL) {
        if (camera->lookat.eye.y > 0.0f) {
            dKy_BossLight_set(&spX, &color, i_this->mWindPower * 2.0f, 0);
            g_env_light.bg_amb_col[3].r = (u8)(i_this->mWindPower * 230.0f + 25.0f);
            g_env_light.bg_amb_col[3].g = (u8)(i_this->mWindPower * 215.0f + 30.0f);
            g_env_light.bg_amb_col[3].b = (u8)(i_this->mWindPower * 155.0f + 25.0f);
        } else {
            g_env_light.bg_amb_col[3].r = 0;
            g_env_light.bg_amb_col[3].g = 0;
            g_env_light.bg_amb_col[3].b = 0;
            g_env_light.mThunderEff.mMode = 0;
        }
    }
}

static void daKytag06_type_06_Execute(kytag06_class* i_this) {
    static u16 w_eff_name[4] = {
        0x8753,
        0x8754,
        0x8755,
        0x8756,
    };

    dComIfGs_getTime();
    dScnKy_env_light_c* env_light = dKy_getEnvlight();

    daKytag06_wether_proc(i_this, env_light->wether);
    cXyz spX;

    f32 current_time = dComIfGs_getTime();
    if (i_this->field_0x574 == 0) {
        if (current_time > 285.0f || current_time < 82.5f) {
            i_this->field_0x591 = 1;
        } else {
            i_this->field_0x591 = 0;
        }
    }

    switch (i_this->mMode) {
    case 0:
        if (i_this->field_0x591 != 0) {
            cLib_addCalc(&i_this->mWindPower, 0.8f, 0.5f, 1.0f, 0.0001f);

            if (i_this->mWindPower > 0.79f) {
                i_this->mMode++;
            }
        }
        break;
    case 1:
        cLib_addCalc(&i_this->mWindPower, 0.6f, 0.5f, 1.5f, 0.0001f);

        if (i_this->mWindPower < 0.61f) {
            i_this->mMode++;
        }
        break;
    case 2:
        cLib_addCalc(&i_this->mWindPower, 1.0f, 0.5f, 1.5f, 0.0001f);

        if (i_this->mWindPower > 0.99f) {
            i_this->mWindPower = 1.0f;
            i_this->mMode++;
        }
        break;
    case 3:
        if (i_this->field_0x591 == 0) {
            cLib_addCalc(&i_this->mWindPower, 0.0f, 0.5f, 1.0f, 0.0001f);

            if (i_this->mWindPower < 0.01f) {
                i_this->mWindPower = 0.0f;
                i_this->mMode = 0;
            }
        }
        break;
    }

    g_env_light.bg_amb_col[3].r = (u8)(i_this->mWindPower * 245.0f + 10.0f);
    g_env_light.bg_amb_col[3].g = (u8)(i_this->mWindPower * 185.0f + 15.0f);
    g_env_light.bg_amb_col[3].b = (u8)(i_this->mWindPower * 130.0f + 20.0f);

    static cXyz c_pos(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; i++) {
        i_this->field_0x580[i] =
            dComIfGp_particle_set(i_this->field_0x580[i], w_eff_name[i], &c_pos, NULL, NULL);
    }
}

static void daKytag06_type_08_Execute(kytag06_class* i_this) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    daKytag06_lv7_boss_wether_proc(i_this, env_light->wether);
}

static void daKytag06_type_09_Execute(kytag06_class* i_this) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    daKytag06_Ganon_wether_proc(i_this, env_light->wether);
}

static void daKytag06_type_10_Execute(kytag06_class* i_this) {
    u8 tmp = 0;
    if (dComIfGs_isEventBit(dSv_event_flag_c::F_0266)) {
        // 2008: Snowpeak Ruins completed
        tmp = 0;
    } else if (dComIfGs_isEventBit(dSv_event_flag_c::F_0065)) {
        // 1420: Talked to Yeta after giving cheese
        tmp = 2;
    } else if (dComIfGs_isEventBit(dSv_event_flag_c::F_0064)) {
        // 1440: Talked to Yeta after giving pumpkin
        tmp = 1;
    }

    switch (tmp) {
    case 0:
        dKy_change_colpat(0);
        break;
    case 1:
        dKy_change_colpat(1);
        break;
    case 2:
        dKy_change_colpat(2);
        break;
    default:
        dKy_change_colpat(0);
        break;
    }

    if (dComIfGp_roomControl_getStayNo() == 4) {
        g_env_light.field_0xe90 = 0;

        if (g_env_light.mSnowCount == 0 && g_env_light.mSnowInitialized &&
            g_env_light.mpSnowPacket != NULL)
        {
            g_env_light.mpSnowPacket->field_0x6d88 = 0;

            for (int i = 0; i < 500; i++) {
                g_env_light.mpSnowPacket->mSnowEff[i].mStatus = 0;
            }
        }

        dKyw_evt_wind_set_go();
        camera_class* camera = dComIfGp_getCamera(0);

        cXyz eye(camera->lookat.eye);

        if (camera != NULL) {
            if (((eye.x > 2079.0f && eye.x < 3013.0f && eye.y < 864.0f && eye.z > -6000.0f &&
                  eye.z < -4145.0f) ||
                 eye.x < -2960.0f) ||
                ((eye.z > -880.0f || eye.z < -6000.0f) ||
                 (eye.z < -4920.0f && eye.y < 864.0f && eye.x < -2000.0f)))
            {
                cLib_addCalc(&i_this->field_0x570, 0.0f, 0.6f, 0.5f, 0.01f);
            } else {
                cLib_addCalc(&i_this->field_0x570, i_this->mWindPower, 0.5f, 0.5f, 0.001f);
            }

            switch (tmp) {
            case 0:
                g_env_light.field_0xe92 = 0;
                cLib_addCalc(&i_this->mWindPower, 0.0f, 0.5f, 0.1f, 0.001f);
                dKyw_custom_windpower(0.2f);
                break;
            case 1:
                g_env_light.field_0xe92 = 0;
                cLib_addCalc(&i_this->mWindPower, 1.0f, 0.5f, 0.1f, 0.001f);
                dKyw_custom_windpower(0.35f);
                break;
            case 2:
                g_env_light.field_0xe92 = 1;
                cLib_addCalc(&i_this->mWindPower, 1.0f, 0.5f, 0.1f, 0.001f);
                dKyw_custom_windpower(0.5f);
                break;
            }
        }
    } else {
        g_env_light.field_0xe92 = 0;
        i_this->mWindPower = 0.0f;
        i_this->field_0x570 = 0.0f;
        dKyw_custom_windpower(0.3f);
    }

    g_env_light.mSnowCount = i_this->mWindPower * 500.0f;
    g_env_light.mMoyaCount = i_this->field_0x570 * 50.0f;

    if (!g_env_light.field_0xe92) {
        g_mEnvSeMgr.setSnowPower(i_this->mWindPower * 63.0f);
    } else {
        g_mEnvSeMgr.setSnowPower(i_this->mWindPower * 127.0f);
    }

    if (dComIfGp_roomControl_getStayNo() != 0 && dComIfGp_roomControl_getStayNo() != 3 &&
        (dComIfGp_roomControl_getStayNo() < 5 || dComIfGp_roomControl_getStayNo() > 9) &&
        dComIfGp_roomControl_getStayNo() != 11 && dComIfGp_roomControl_getStayNo() != 13)
    {
        g_env_light.field_0xe90 = 0;
    }
}

static void daKytag06_type_11_Execute(kytag06_class* i_this) {
    if (dKy_darkworld_check() && dComIfG_play_c::getLayerNo(0) == 14) {
        switch (dComIfGp_roomControl_getStayNo()) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            dKy_change_colpat(dComIfGp_roomControl_getStayNo());
            break;
        case 8:
            dKy_change_colpat(5);
            break;
        }
    }
}

static void daKytag06_type_05_Execute(kytag06_class* i_this) {
    cXyz spX;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (player != NULL) {
        spX.x = player->current.pos.x;
        spX.y = i_this->current.pos.y;
        spX.z = player->current.pos.z;

        if (i_this->current.pos.abs(spX) < i_this->scale.x * 5000.0f) {
            g_env_light.field_0x130b = 1;
        }
    }
}

static void daKytag06_type_03_Execute(kytag06_class* i_this) {
    if (i_this->field_0x597 == 0) {
        switch (i_this->mMode) {
        case 0:
            // 0C01: Midna's Desperate Hour started    1E08: Midna's Desperate Hour Completed
            if (dComIfGs_isEventBit(dSv_event_flag_c::M_071) && !dComIfGs_isEventBit(dSv_event_flag_c::F_0250)) {
                i_this->mMode = 1;
                i_this->field_0x578 = 450;
                dKy_change_colpat(i_this->field_0x591);

                dComIfGs_onSwitch(i_this->mSwNo, dComIfGp_roomControl_getStayNo());
            }
            break;
        case 1: {
            int old = i_this->field_0x578;
            i_this->field_0x578--;

            if (old <= 0) {
                i_this->mMode++;
            }
            break;
        }
        case 2:
            if ((g_Counter.mCounter0 & 7) == 0) {
                // run block every 8 frames
                if (g_env_light.raincnt < 250) {
                    g_env_light.raincnt++;
                } else {
                    i_this->mMode++;
                }
            }
            break;
        case 3:
            break;
        }
    } else if (i_this->field_0x597 == 2 && dComIfGp_roomControl_getStayNo() != 0) {
        switch (i_this->mMode) {
        case 0:
            if (i_this->mSwNo != 0xFF && dComIfGs_isSwitch(i_this->mSwNo, i_this->current.roomNo)) {
                cLib_addCalc(&i_this->mWindPower, 1.0f, 0.1f, 0.01f, 0.0001f);

                if (i_this->mWindPower >= 1.0f) {
                    i_this->mWindPower = 1.0f;
                    g_env_light.mColpatWeather = 0;
                    i_this->mMode = 1;
                }

                dKy_custom_colset(i_this->field_0x591, 0, i_this->mWindPower);
            } else {
                dKy_custom_colset(0, i_this->field_0x591, 1.0f);
                g_env_light.mColpatWeather = i_this->field_0x591;
            }
            break;
        case 1:
            break;
        }
    }
}

static int daKytag06_Execute(kytag06_class* i_this) {
    dComIfGs_getTime();

    switch (i_this->mType) {
    case 1: {
        camera_class* camera = dComIfGp_getCamera(0);

        if (i_this->mpPath != NULL) {
            int target1 = 0;
            int target2 = 0;
            dPnt* pnt = near_rail_get(i_this, &camera->lookat.eye);

            if (pnt != NULL && pnt->mArg0 != 0xFF) {
                dKy_change_colpat(pnt->mArg0);

                if (pnt->mArg0 == 0) {
                    target1 = 250;
                    target2 = 25;
                } else if (pnt->mArg0 == 1) {
                    target1 = 500;
                    target2 = 50;
                } else if (pnt->mArg0 == 2) {
                    target1 = 125;
                    target2 = 12;
                }

                if ((int)i_this->mWindPower > 250) {
                    g_env_light.field_0xe92 = 1;
                } else {
                    g_env_light.field_0xe92 = 0;
                }

                cLib_addCalc(&i_this->mWindPower, (s16)target1, 0.5f, 1.0f, 0.01f);
                cLib_addCalc(&i_this->field_0x570, (s16)target2, 0.5f, 1.0f, 0.01f);

                g_env_light.mSnowCount = i_this->mWindPower;
                g_env_light.mMoyaCount = i_this->field_0x570;
                g_mEnvSeMgr.setSnowPower((i_this->mWindPower / 500.0f) * 127.0f);
                break;
            }
        }
        break;
    }
    case 2:
        if (i_this->mSwNo != 0xFF) {
            if (dComIfGs_isSwitch(i_this->mSwNo, dComIfGp_roomControl_getStayNo()) && i_this->mMode == 0) {
                i_this->mMode = 1;
                dKy_change_colpat(i_this->field_0x591);
            }
        }
        break;
    case 3:
        daKytag06_type_03_Execute(i_this);
        break;
    case 5:
        daKytag06_type_05_Execute(i_this);
        break;
    case 6:
        daKytag06_type_06_Execute(i_this);
        break;
    case 7:
        daKytag06_type_07_Execute(i_this);
        break;
    case 8:
        daKytag06_type_08_Execute(i_this);
        break;
    case 9:
        daKytag06_type_09_Execute(i_this);
        break;
    case 10:
        if (strcmp(dComIfGp_getStartStageName(), "F_SP114") != 0) {
            daKytag06_type_10_Execute(i_this);
        } else {
            daKytag06_type_03_Execute(i_this);
        }
        break;
    case 11:
        daKytag06_type_11_Execute(i_this);
        break;
    default:
        if (i_this->mMode == 0) {
            switch (i_this->field_0x591) {
            case 0:
                if (dComIfGs_BossLife_public_Get() != -1 && dComIfGs_BossLife_public_Get() <= 6) {
                    dKy_change_colpat(1);
                    i_this->field_0x591++;
                }
                break;
            case 1:
                if (i_this->field_0x578 > 600) {
                    g_env_light.mMoyaMode = 0;

                    if (g_env_light.mMoyaCount < 50) {
                        g_env_light.mMoyaCount++;
                    } else {
                        i_this->field_0x591++;
                    }
                }
                break;
            case 2:
                if (i_this->field_0x578 > 900) {
                    if (g_env_light.raincnt < 250) {
                        g_env_light.raincnt++;
                    } else {
                        i_this->field_0x591++;
                    }
                }
                break;
            case 3:
                if (i_this->field_0x578 > 1100) {
                    g_env_light.mThunderEff.mMode = 2;
                    i_this->field_0x591++;
                }
                break;
            }

            if (dComIfGs_BossLife_public_Get() != -1 && dComIfGs_BossLife_public_Get() <= 3) {
                i_this->field_0x591 = 0;
                i_this->field_0x578 = 0;
                i_this->mMode = 1;
            }
        } else {
            switch (i_this->field_0x591) {
            case 0:
                if (i_this->field_0x578 > 0) {
                    g_env_light.mThunderEff.mMode = 0;

                    if (g_env_light.raincnt > 0) {
                        g_env_light.raincnt--;
                    } else {
                        i_this->field_0x591++;
                    }
                }
                break;
            case 1:
                if (i_this->field_0x578 > 150) {
                    g_env_light.mMoyaMode = 0;

                    if (g_env_light.mMoyaCount >= 0) {
                        g_env_light.mMoyaCount--;
                    } else {
                        i_this->field_0x591++;
                    }
                }
                break;
            case 2:
                if (i_this->field_0x578 > 300) {
                    dKy_change_colpat(6);
                    i_this->field_0x591++;
                }
                break;
            case 3:
                if (i_this->field_0x578 > 420) {
                    dKy_change_colpat(0);
                    i_this->field_0x591++;
                }
                break;
            case 4:
                if (dComIfGs_BossLife_public_Get() == 0) {
                    f32 time = dComIfGs_getTime();

                    if (time < 255.0f) {
                        time += 0.03f;

                        if (dComIfGp_event_runCheck()) {
                            time += 0.1f;
                        }

                        dComIfGs_setTime(time);
                    }
                }
                break;
            }
        }

        if ((i_this->mMode != 0 || i_this->field_0x591 != 0) && i_this->field_0x578 < 101849) {
            i_this->field_0x578++;
        }

        if (i_this->field_0x578 < 99999) {
            i_this->field_0x57c++;
        }
        break;
    case 4:
        void(0);
        break;
    }

    return 1;
}

static int daKytag06_IsDelete(kytag06_class* i_this) {
    return 1;
}

static int daKytag06_Delete(kytag06_class* i_this) {
    if (i_this->mType == 0 && i_this->mMode != 0 && i_this->field_0x591 >= 4) {
        dComIfGs_setTime(270.0f);
    }

    return 1;
}

static void daKytag06_type04_init(fopAc_ac_c* i_this) {
    kytag06_class* a_this = (kytag06_class*)i_this;

    a_this->mSwNo = fopAcM_GetParam(a_this) >> 8;
    a_this->field_0x591 = fopAcM_GetParam(a_this) & 0xFF;
    a_this->mMode = 0;
}

static void daKytag06_type06_init(fopAc_ac_c* i_this) {
    kytag06_class* a_this = (kytag06_class*)i_this;

    a_this->mSwNo = fopAcM_GetParam(a_this) >> 8;
    a_this->field_0x591 = fopAcM_GetParam(a_this) & 0xFF;
    a_this->mMode = 0;
    a_this->field_0x598 = 0;
}

static void daKytag06_type02_init(fopAc_ac_c* i_this) {
    kytag06_class* a_this = (kytag06_class*)i_this;

    a_this->mSwNo = fopAcM_GetParam(a_this) >> 8;
    a_this->field_0x591 = fopAcM_GetParam(a_this) & 0xFF;
    a_this->mMode = 0;
}

static void daKytag06_type03_init(fopAc_ac_c* i_this) {
    kytag06_class* a_this = (kytag06_class*)i_this;

    a_this->mSwNo = fopAcM_GetParam(a_this) >> 8;
    a_this->field_0x591 = fopAcM_GetParam(a_this) & 0xFF;
    a_this->mMode = 0;

    if (a_this->field_0x597 == 0) {
        // 0C01: Midna's Desperate Hour started    1E08: Midna's Desperate Hour Completed
        if (dComIfGs_isEventBit(0x0C01) && !dComIfGs_isEventBit(0x1E08) && a_this->mSwNo != 0xFF)
        {
            if (dComIfGs_isSwitch(a_this->mSwNo, dComIfGp_roomControl_getStayNo())) {
                a_this->mMode = 9;
                g_env_light.raincnt = 250;
                g_env_light.mColpatWeather = a_this->field_0x591;
                g_env_light.wether_pat0 = a_this->field_0x591;
                g_env_light.wether_pat1 = a_this->field_0x591;
            }
        }
    } else if (a_this->field_0x597 == 2 && dComIfGp_roomControl_getStayNo() != 0) {
        if (a_this->mSwNo != 0xFF && !dComIfGs_isSwitch(a_this->mSwNo, a_this->current.roomNo)) {
            dKy_custom_colset(0, a_this->field_0x591, 1.0f);
            g_env_light.mColpatWeather = a_this->field_0x591;
        } else {
            a_this->mMode = 1;
        }
    }
}

static int daKytag06_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, kytag06_class);
    kytag06_class* a_this = static_cast<kytag06_class*>(i_this);

    a_this->mType = (fopAcM_GetParam(a_this) >> 0x18) & 0xF;
    a_this->field_0x597 = (fopAcM_GetParam(a_this) >> 0x1C);
    a_this->mWindPower = 0.0f;
    a_this->field_0x570 = 0.0f;

    switch (a_this->mType) {
    case 1:
        a_this->mPathNo = fopAcM_GetParam(a_this) >> 0x10;
        a_this->mpPath = set_path_info(a_this);
        break;
    case 2:
        daKytag06_type02_init(a_this);
        break;
    case 3:
        daKytag06_type03_init(a_this);
        break;
    case 4:
        daKytag06_type04_init(a_this);
        break;
    case 6:
        daKytag06_type06_init(a_this);
        break;
    case 7:
        a_this->mMode = 0;
        a_this->field_0x591 = 0;
        a_this->field_0x574 = 0;
        break;
    case 8:
        daKytag06_type06_init(a_this);
        g_env_light.mMoyaMode = 9;
        g_env_light.mMoyaCount = 50;
        break;
    case 9:
        daKytag06_type06_init(a_this);
        break;
    case 10:
        if (strcmp(dComIfGp_getStartStageName(), "F_SP114") != 0) {
            daKytag06_type06_init(a_this);
        } else {
            daKytag06_type03_init(a_this);
        }
        break;
    case 11:
        daKytag06_type06_init(a_this);
        break;
    default:
        if (strcmp(dComIfGp_getStartStageName(), "T_MUKAO") != 0) {
            g_env_light.mColpatWeather = 3;
            g_env_light.wether_pat0 = 3;
            g_env_light.wether_pat1 = 3;
        } else {
            dComIfGs_setTime(210.0f);
        }

        a_this->field_0x578 = 0;
        a_this->field_0x57c = 0;
        a_this->mMode = 0;
        a_this->field_0x591 = 0;
        a_this->field_0x574 = 640;
        break;
    case 5:
        break;
    }

    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daKytag06_Method = {
    (process_method_func)daKytag06_Create,  (process_method_func)daKytag06_Delete,
    (process_method_func)daKytag06_Execute, (process_method_func)daKytag06_IsDelete,
    (process_method_func)daKytag06_Draw,
};

actor_process_profile_definition g_profile_KYTAG06 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KYTAG06,
    &g_fpcLf_Method.base,
    sizeof(kytag06_class),
    0,
    0,
    &g_fopAc_Method.base,
    100,
    &l_daKytag06_Method,
    0x60000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};

AUDIO_INSTANCES;
