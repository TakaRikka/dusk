/**
 * d_a_kytag01.cpp
 * Sacred Grove Mist Tag
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag01.h"
#include "SSystem/SComponent/c_math.h"
#include "Z2AudioLib/Z2EnvSeMgr.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo_rain.h"
#include "f_op/f_op_camera_mng.h"

static cXyz get_check_pos(kytag01_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    cXyz pos;
    f32 dist_to_cameye = a_this->current.pos.abs(camera->lookat.eye);
    f32 dist_to_player = a_this->current.pos.abs(player->current.pos);

    if (dComIfGp_event_runCheck()) {
        if (dist_to_cameye < dist_to_player) {
            pos = camera->lookat.eye;
        } else {
            pos = player->current.pos;
        }
    } else {
        pos = player->current.pos;
    }

    return pos;
}

static void mist_tag_move(kytag01_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    cXyz sp80;
    cXyz check_pos;
    cXyz sp98;
    cXyz spA4;
    cXyz spB0;
    cXyz spBC;

    check_pos = get_check_pos(i_this);
    sp80 = a_this->current.pos;
    sp80.y = check_pos.y;

    f32 var_f31 = sp80.abs(check_pos);
    if (var_f31 > i_this->mNamiInnerRange) {
        if (var_f31 > i_this->mNamiOuterRange) {
            if (var_f31 > i_this->mNamiOuterRange + 100.0f) {
                var_f31 = 1.0f;
            } else {
                var_f31 = 1.0f;
            }
        } else {
            var_f31 = (var_f31 - i_this->mNamiInnerRange) /
                      (i_this->mNamiOuterRange - i_this->mNamiInnerRange);
            i_this->field_0x59c = 1;
        }
    } else {
        var_f31 = 0.0f;
        i_this->field_0x59c = 1;
    }

    var_f31 *= i_this->field_0x594;

    spBC = camera->lookat.center;
    spBC.y = camera->lookat.eye.y;

    dKyr_get_vectle_calc(&camera->lookat.eye, &spBC, &sp98);
    sp80.y = camera->lookat.eye.y;

    dKyr_get_vectle_calc(&camera->lookat.eye, &sp80, &spA4);

    s16 var_r29_2 = cM_atan2s(sp98.x, sp98.z);
    s16 var_r26 = cM_atan2s(spA4.x, spA4.z);

    f32 temp_f2_2 = 1.0f - ((f32)(s16)abs((s16)(var_r29_2 - var_r26)) / 32768.0f);
    temp_f2_2 *= temp_f2_2 * temp_f2_2 * temp_f2_2;

    f32 var_f3 = 1.0f - temp_f2_2;
    var_f3 += 0.2f;

    if (var_f3 > 1.0f) {
        var_f3 = 1.0f;
    }

    dKy_fog_startendz_set(-2000.0f, 200.0f, var_f31 * var_f3);
    f32 temp_f1 = i_this->field_0x594;
    g_env_light.mColpatPrevGather = 0;
    g_env_light.mColpatCurrGather = 1;
    g_env_light.mColPatBlendGather = temp_f1;
    g_env_light.mColPatModeGather = 1;
    g_env_light.mMoyaMode = 3;
    g_env_light.mMoyaCount = g_env_light.mColPatBlendGather * 50.0f;

    if (g_env_light.mColPatBlendGather > 0.5f) {
        mDoAud_startFogSe();
    }
}

static int daKytag01_Draw(kytag01_class* i_this) {
    return 1;
}

static int daKytag01_Execute(kytag01_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    camera_class* camera = dComIfGp_getCamera(0);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if ((a_this->argument & 0xFF) == 2) {
        if (i_this->mStartPoint == dComIfGs_getStartPoint()) {
            i_this->mStartPoint = 0xFF;
            dKy_change_colpat(i_this->field_0x59e);
        }
    } else if ((a_this->argument & 0xFF) == 0) {
        if (a_this->home.roomNo == dComIfGp_roomControl_getStayNo()) {
            if (i_this->mSwNo1 != 0xFF) {
                if (dComIfGs_isSwitch(i_this->mSwNo1, a_this->home.roomNo)) {
                    if (i_this->mSwNo2 != 0xFF &&
                        dComIfGs_isSwitch(i_this->mSwNo2, a_this->home.roomNo))
                    {
                        cLib_addCalc(&i_this->field_0x594, 0.0f, 0.05f, 0.02f, 0.0001f);

                        if (i_this->field_0x594 <= 0.0f) {
                            g_env_light.mColpatPrevGather = 0;
                            g_env_light.mColpatCurrGather = 0;
                            g_env_light.mColPatBlendGather = 0.0f;
                            g_env_light.mColPatModeGather = 1;
                            g_env_light.mMoyaCount = 0;
                            fopAcM_delete(i_this);
                        }
                    } else {
                        cLib_addCalc(&i_this->field_0x594, 1.0f, 0.05f, 0.02f, 0.0001f);
                    }
                } else {
                    cLib_addCalc(&i_this->field_0x594, 0.0f, 0.05f, 0.02f, 0.0001f);
                }
            }

            if (i_this->field_0x59c == 2) {
                i_this->field_0x594 = 1.0f;
            }

            if (i_this->field_0x594 > 0.0f) {
                mist_tag_move(i_this);
            }
        }
    }

    return 1;
}

static int daKytag01_IsDelete(kytag01_class* i_this) {
    return 1;
}

static int daKytag01_Delete(kytag01_class* i_this) {
    return 1;
}

static int daKytag01_Create(fopAc_ac_c* i_this) {
    kytag01_class* a_this = (kytag01_class*)i_this;
    fopAcM_ct(i_this, kytag01_class);

    if ((a_this->argument & 0xFF) == 2) {
        a_this->mNamiInnerRange = i_this->scale.x * 100.0f;
        a_this->field_0x59d = fopAcM_GetParam(i_this);
        a_this->mStartPoint = fopAcM_GetParam(i_this) >> 8;

        if (a_this->mStartPoint == dComIfGs_getStartPoint() &&
            i_this->home.roomNo == dComIfGp_roomControl_getStayNo())
        {
            a_this->field_0x59e = g_env_light.wether_pat0;
            g_env_light.mColpatWeather = a_this->field_0x59d;
            g_env_light.wether_pat0 = a_this->field_0x59d;
            g_env_light.wether_pat1 = g_env_light.wether_pat0;
        }
    } else {
        a_this->mNamiSize = (int)((fopAcM_GetParam(i_this) >> 0x10) & 0xFF);
        a_this->mSwNo1 = i_this->current.angle.x & 0xFF;
        a_this->mSwNo2 = (i_this->current.angle.x >> 8) & 0xFF;
        a_this->field_0x594 = 0.0f;

        if ((a_this->argument & 0xFF) == 0) {
            if (a_this->mNamiSize == 255.0f) {
                a_this->mNamiSize = 10.0f;
            }

            a_this->mNamiInnerRange = i_this->scale.x * 100.0f;
            a_this->mNamiOuterRange = a_this->mNamiInnerRange + a_this->mNamiSize * 100.0f;

            if (a_this->mNamiInnerRange + 100.0f >= a_this->mNamiOuterRange) {
                OS_REPORT("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
                // "\nLost Woods  Fog Tag scale range is too narrow so force fix!"
                OS_REPORT("\n迷いの森　霧タグのスケールでの範囲指定が狭すぎなので強制修正!");
                OS_REPORT("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

                OS_REPORT("\nnami utigawa=[%f]", a_this->mNamiInnerRange);
                OS_REPORT("\nnami sotogawa=[%f]", a_this->mNamiOuterRange);

                a_this->mNamiOuterRange = a_this->mNamiInnerRange + 100.0f;
            }

            a_this->field_0x59c = 0;
        }
    }

    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daKytag01_Method = {
    (process_method_func)daKytag01_Create,  (process_method_func)daKytag01_Delete,
    (process_method_func)daKytag01_Execute, (process_method_func)daKytag01_IsDelete,
    (process_method_func)daKytag01_Draw,
};

extern actor_process_profile_definition g_profile_KYTAG01 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KYTAG01,
    &g_fpcLf_Method.base,
    sizeof(kytag01_class),
    0,
    0,
    &g_fopAc_Method.base,
    95,
    &l_daKytag01_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
