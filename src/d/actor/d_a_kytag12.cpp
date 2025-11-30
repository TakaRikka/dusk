/**
 * @file d_a_kytag12.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag12.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo_rain.h"
#include "d/d_kankyo_wether.h"

static BOOL d_kytag12_cut_turn_check() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    BOOL var_r3 = false;
    if (player != NULL && (player->getCutType() == daPy_py_c::CUT_TYPE_TURN_RIGHT ||
                           player->getCutType() == daPy_py_c::CUT_TYPE_TURN_LEFT ||
                           player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT ||
                           player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT))
    {
        var_r3 = true;
    }

    return var_r3;
}

static BOOL d_kytag12_cut_normal_check() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    BOOL var_r3 = true;
    if (player != NULL &&
        (player->getCutType() == 0x28 || player->getCutType() == daPy_py_c::CUT_TYPE_GUARD_ATTACK))
    {
        var_r3 = false;
    }

    return var_r3;
}

static int daKytag12_Draw(kytag12_class* param_0) {
    return 1;
}

static void daKytag12_light_swprd_proc(kytag12_class* i_this) {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (dComIfGs_getSelectEquipSword() == fpcNm_ITEM_LIGHT_SWORD) {
        if (player->getCutType() != daPy_py_c::CUT_TYPE_NONE) {
            if (i_this->field_0x576 == 0) {
                cXyz sp28(player->current.pos);
                sp28.y -= 100.0f;

                i_this->field_0x576 = 25;

                if (d_kytag12_cut_turn_check()) {
                    dKyw_plight_collision_set(&sp28, 0, player->current.angle.y, 500.0f, 2000.0f,
                                              1.0f, 0.0f, 0.015f);
                } else if (d_kytag12_cut_normal_check()) {
                    dKyw_plight_collision_set(&sp28, 1800, player->current.angle.y, 500.0f, 300.0f,
                                              1.0f, 150.0f, 0.3f);
                }
            } else if (i_this->field_0x576 != 0) {
                i_this->field_0x576--;
            }
        } else {
            i_this->field_0x576 = 0;
        }
    }
}

static int daKytag12_Execute_standard(kytag12_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    dKankyo_evil_Packet* temp_r25 = dKy_getEnvlight()->mpEvilPacket;
    cXyz spB8;
    cXyz spC4;

    f32 var_f30;
    f32 var_f25;
    f32 var_f29;
    f32 var_f28;

    int var_r24 = 0;
    int var_r23 = 0;
    int var_r22 = 0;
    int var_r21 = 0;
    int var_r20 = 0;
    int var_r19 = 0;
    int spEC = 0;
    int spE8 = 0;

    if (temp_r25 == NULL) {
        return 1;
    }

    cXyz spD0;
    cXyz spDC;
    cXyz spA8;

    for (int i = 0; i < g_env_light.field_0x1054; i++) {
        EF_EVIL_EFF* effect_p = &temp_r25->mEffect[i];
        var_f29 = 1.0f;

        switch (effect_p->mStatus) {
        case 0:
            if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 && a_this->home.roomNo == 1 &&
                i < 400)
            {
                effect_p->mBasePos.x = cM_rndFX(30.0f) + -5200.0f;
                effect_p->mBasePos.y = cM_rndF(2100.0f) + -700.0f;
                effect_p->mBasePos.z = cM_rndFX(200.0f) + 5400.0f;

                effect_p->field_0x24 = 1.0f;
                effect_p->field_0x38 = fabsf(var_f30) * 50.0f + 100.0f;
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 &&
                       a_this->home.roomNo == 1 && i >= 400 && i < 800)
            {
                effect_p->mBasePos.x = cM_rndFX(30.0f) + -2700.0f;
                effect_p->mBasePos.y = cM_rndF(2100.0f) + -700.0f;
                effect_p->mBasePos.z = cM_rndFX(200.0f) + 5400.0f;

                effect_p->field_0x24 = 1.0f;
                effect_p->field_0x38 = fabsf(var_f30) * 50.0f + 100.0f;
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 &&
                       a_this->home.roomNo == 1 && i >= 800 && i < 1200)
            {
                effect_p->mBasePos.x = cM_rndFX(30.0f) + -2700.0f;
                effect_p->mBasePos.y = cM_rndF(2100.0f) + -1100.0f;
                effect_p->mBasePos.z = cM_rndFX(200.0f) + 3200.0f;

                effect_p->field_0x24 = 1.0f;
                effect_p->field_0x38 = fabsf(var_f30) * 50.0f + 100.0f;
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 &&
                       a_this->home.roomNo == 1 && i >= 1200 && i < 1600)
            {
                effect_p->mBasePos.x = cM_rndFX(30.0f) + -5200.0f;
                effect_p->mBasePos.y = cM_rndF(2100.0f) + -1100.0f;
                effect_p->mBasePos.z = cM_rndFX(200.0f) + 3200.0f;

                effect_p->field_0x24 = 1.0f;
                effect_p->field_0x38 = fabsf(var_f30) * 50.0f + 100.0f;
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 &&
                       a_this->home.roomNo == 11)
            {
                effect_p->mBasePos.x = cM_rndFX(1000.0f);
                effect_p->mBasePos.y = cM_rndF(6700.0f) + -500.0f;
                effect_p->mBasePos.z = cM_rndFX(10.0f) + -2828.0f;

                effect_p->field_0x24 = 1.0f;
                effect_p->field_0x38 = fabsf(var_f30) * 50.0f + 100.0f;
            } else {
                f32 temp_f2;
                switch (a_this->home.roomNo) {
                case 2:
                    temp_f2 = -2.3f;
                    var_f28 = -0.5f;
                    var_f25 = 90.0f;
                    break;
                case 4:
                    temp_f2 = -0.7f;
                    var_f28 = -2.0f;
                    var_f25 = 90.0f;
                    break;
                case 5:
                    temp_f2 = -1.0f;
                    var_f28 = -1.0f;
                    var_f25 = 90.0f;
                    break;
                case 7:
                    temp_f2 = -1.1f;
                    var_f28 = 0.0f;
                    var_f25 = 90.0f;
                    break;
                case 8:
                    temp_f2 = 0.0f;
                    var_f28 = -2.3f;
                    var_f25 = 90.0f;
                    break;
                default:
                    temp_f2 = 0.0f;
                    var_f28 = 0.0f;
                    var_f25 = 90.0f;
                    break;
                }

                effect_p->mBasePos.x =
                    a_this->current.pos.x + cM_rndFX((a_this->scale.x + temp_f2) * 100.0f);
                effect_p->mBasePos.y =
                    a_this->current.pos.y + cM_rndF(a_this->scale.y * 100.0f) + var_f25;
                effect_p->mBasePos.z =
                    a_this->current.pos.z + cM_rndFX((a_this->scale.z + var_f28) * 100.0f);

                effect_p->field_0x24 = 1.0f;
                a_this->current.pos.abs(effect_p->mBasePos);
                effect_p->field_0x38 = fabsf(var_f30) * 50.0f + 100.0f;
            }

            effect_p->field_0x2c = 0.0f;
            effect_p->field_0x40 = 0;
            effect_p->field_0x42 = 0;
            effect_p->mStatus++;
            /* fallthrough */
        case 1: {
            var_f30 = cM_ssin(effect_p->field_0x3c);
            effect_p->field_0x3c += (((f32)(i & 31) * 6.8f) + 630.0f);

            f32 temp_f7 = cM_ssin(effect_p->field_0x3e);
            effect_p->field_0x3e += (((f32)(i & 51) * 4.8) + 562.0);

            effect_p->mPosition.x = temp_f7 * 10.0f;
            effect_p->mPosition.y = (var_f30 + 1.0f) * 13.0f;
            effect_p->mPosition.z = temp_f7 * 9.0f;

            if (effect_p->field_0x38 < 9000.0f) {
                if (g_env_light.field_0x1054 <= 500) {
                    effect_p->field_0x38 = temp_f7 * 10.0f + 85.0f;
                } else if (a_this->home.roomNo == 11) {
                    effect_p->field_0x38 = (temp_f7 * 10.0f + 85.0f) * 1.5f;
                } else {
                    effect_p->field_0x38 = (temp_f7 * 10.0f + 85.0f) * 0.6f;
                }
            }
            break;
        }
        case 2:
            spB8 = effect_p->mBasePos + effect_p->mPosition;
            var_f29 = 0.0f;

            spC4.x = cM_rndFX(0.5f);
            spC4.z = cM_rndFX(0.5f);
            spC4.y = 1.0f;

            effect_p->mPosition.x += spC4.x * 25.0f;
            effect_p->mPosition.y += spC4.y * 25.0f;
            effect_p->mPosition.z += spC4.z * 25.0f;

            if (effect_p->field_0x38 > 4.0f) {
                effect_p->field_0x38 -= 4.0f;
            }

            if (effect_p->field_0x24 <= 0.0f) {
                effect_p->mStatus = 1;
            }
            break;
        }

        if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 && a_this->home.roomNo == 1 &&
            i < 1600)
        {
            effect_p->mBasePos.y += -8.0f;

            if (i >= 800) {
                if (effect_p->mBasePos.y < -1200.0f) {
                    effect_p->mBasePos.y = 900.0f;
                    effect_p->mStatus = 1;
                    effect_p->field_0x24 = 1.0f;
                }
            } else if (effect_p->mBasePos.y < -780.0f) {
                effect_p->mBasePos.y = 1400.0f;
                effect_p->mStatus = 1;
                effect_p->field_0x24 = 1.0f;
            }
        } else if (a_this->home.roomNo == 11) {
            effect_p->mBasePos.y += cM_rndF(-5.0f) + -10.0f;

            if (effect_p->mBasePos.y < -500.0f) {
                effect_p->mBasePos.y = 6200.0f;
                effect_p->mStatus = 1;
                effect_p->field_0x24 = 1.0f;
            }
        }

        for (int j = 0; j < 10; j++) {
            if (g_env_light.dalkmist_influence[j] != NULL &&
                g_env_light.dalkmist_influence[j]->mPos.abs(effect_p->mBasePos) <
                    g_env_light.dalkmist_influence[j]->field_0xc + 100.0f)
            {
                if (effect_p->mStatus != 2 && effect_p->field_0x40 == 0 && var_r23 < 1) {
                    cXyz spF4;
                    spF4 = effect_p->mBasePos;

                    mDoAud_seStart(Z2SE_OBJ_L8_B_FOG_FLY, &spF4, 0, 0);
                    var_r23++;
                }

                effect_p->mStatus = 2;
                effect_p->field_0x40 = 180;
                break;
            }
        }

        spB8 = effect_p->mBasePos + effect_p->mPosition;

        if (player->current.pos.y - 100.0f < spB8.y && player->current.pos.y + 250.0f > spB8.y) {
            spB8.y = player->current.pos.y;

            if (player->current.pos.abs(spB8) < 190.0f && var_f29 > 0.0f &&
                effect_p->field_0x40 == 0)
            {
                var_r22++;
            }
        }

        if (!daPy_py_c::checkNowWolf()) {
            if (effect_p->field_0x38 >= 9000.0f) {
                var_f29 = 0.0f;
            }

            spB8 = effect_p->mBasePos + effect_p->mPosition;
            spB8.y -= 100.0f;

            f32 sp34;
            dKyw_pntlight_collision_get_info(&spB8, &spDC, &sp34);

            if (sp34 > 0.2f) {
                if (effect_p->mStatus != 2 && effect_p->field_0x40 == 0 && var_r23 < 1) {
                    cXyz sp100;
                    sp100 = effect_p->mBasePos;

                    mDoAud_seStart(Z2SE_OBJ_L8_B_FOG_FLY, &sp100, 0, 0);
                    var_r23++;
                }

                effect_p->mStatus = 2;
                effect_p->field_0x40 = 360;
            }

            spB8 = effect_p->mBasePos + effect_p->mPosition;

            if (player->current.pos.y < spB8.y + 100.0f &&
                player->current.pos.y > spB8.y - 200.0f && effect_p->mStatus != 2)
            {
                spB8.y = player->current.pos.y;

                if (player->current.pos.abs(spB8) < 165.0f && var_f29 > 0.0f &&
                    effect_p->field_0x24 >= 0.95f)
                {
                    var_r24++;
                }
            }
        }

        if (var_f29 <= 0.0f || effect_p->field_0x40 == 0) {
            if (var_f29 <= 0.0f) {
                cLib_addCalc(&effect_p->field_0x24, var_f29, 0.25f, 0.07f, 0.0001f);
            } else if (temp_r25->mEffect[0].field_0x42 != 0) {
                cLib_addCalc(&effect_p->field_0x24, 0.04f, 0.5f, 0.5f, 0.1f);
            } else {
                cLib_addCalc(&effect_p->field_0x24, var_f29, 0.1f, 0.01f, 0.001f);
            }
        }

        if (!dComIfGp_event_runCheck() && effect_p->field_0x40 != 0) {
            effect_p->field_0x40--;
        }

        if (effect_p->field_0x24 > 0.95f) {
            if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 && a_this->home.roomNo == 1 &&
                i < 1600)
            {
                if (i < 400) {
                    var_r20++;
                } else if (i >= 400 && i < 800) {
                    var_r19++;
                } else if (i >= 800 && i < 1200) {
                    spEC++;
                } else if (i >= 1200 && i < 1600) {
                    spE8++;
                }
            } else {
                var_r21++;
            }
        }
    }

    if (var_r24 != 0 && i_this->field_0x578 == 0) {
        if (dComIfGp_roomControl_getStayNo() == 11 || dComIfGp_roomControl_getStayNo() == 1) {
            i_this->field_0x578 = 60;
        } else {
            i_this->field_0x578 = 80;
        }
    }

    if (i_this->field_0x578 != 0 && i_this->field_0x578 <= 60) {
        if (!daPy_py_c::checkNowWolf()) {
            temp_r25->mEffect[0].field_0x42 = 45;
        }

        if (player->checkMetamorphose()) {
            i_this->field_0x578 = 0;
        } else {
            player->onForceWolfChange();
        }
    }

    if (i_this->field_0x578 != 0) {
        i_this->field_0x578--;
    }

    if (temp_r25->mEffect[0].field_0x42 != 0) {
        temp_r25->mEffect[0].field_0x42--;
    }

    daKytag12_light_swprd_proc(i_this);

    g_env_light.mEvilInitialized &= 0x7F;

    if (daPy_py_c::checkNowWolf() && var_r22 > 0) {
        g_env_light.mEvilInitialized |= 0x80;
    }

    if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 && a_this->home.roomNo == 1) {
        f32 temp = (f32)var_r20 / (f32)(g_env_light.field_0x1054 / 5);
        i_this->field_0x57c[0].x = -5200.0f;
        i_this->field_0x57c[0].y = player->current.pos.y;
        i_this->field_0x57c[0].z = 5400.0f;
        mDoAud_seStartLevel(Z2SE_OBJ_L8_B_FALL_S, &i_this->field_0x57c[0], temp * 100.0f, 0);

        f32 temp2 = (f32)var_r19 / (f32)(g_env_light.field_0x1054 / 5);
        i_this->field_0x57c[1].x = -2700.0f;
        i_this->field_0x57c[1].y = player->current.pos.y;
        i_this->field_0x57c[1].z = 5400.0f;
        mDoAud_seStartLevel(Z2SE_OBJ_L8_B_FALL_S, &i_this->field_0x57c[1], temp2 * 100.0f, 0);

        f32 temp3 = ((f32)spEC / (f32)(g_env_light.field_0x1054 / 5));
        i_this->field_0x57c[2].x = -2700.0f;
        i_this->field_0x57c[2].y = player->current.pos.y;
        i_this->field_0x57c[2].z = 3200.0f;
        mDoAud_seStartLevel(Z2SE_OBJ_L8_B_FALL_S, &i_this->field_0x57c[2], temp3 * 100.0f, 0);

        f32 temp4 = ((f32)spE8 / (f32)(g_env_light.field_0x1054 / 5));
        i_this->field_0x57c[3].x = -5200.0f;
        i_this->field_0x57c[3].y = player->current.pos.y;
        i_this->field_0x57c[3].z = 3200.0f;
        mDoAud_seStartLevel(Z2SE_OBJ_L8_B_FALL_S, &i_this->field_0x57c[3], temp4 * 100.0f, 0);
    }

    if (a_this->home.roomNo != 11) {
        f32 var_f1_4;
        if (a_this->home.roomNo == 1) {
            var_f1_4 = ((f32)var_r21 / (f32)(g_env_light.field_0x1054 / 5));
        } else {
            var_f1_4 = ((f32)var_r21 / (f32)g_env_light.field_0x1054);
        }

        mDoAud_seStartLevel(Z2SE_OBJ_L8_B_FOG_STAY, &a_this->current.pos, var_f1_4 * 100.0f, 0);
    } else {
        f32 temp = ((f32)var_r21 / (f32)g_env_light.field_0x1054);
        i_this->field_0x57c[0].x = 0.0f;
        i_this->field_0x57c[0].y = player->current.pos.y;
        i_this->field_0x57c[0].z = -2828.0f;
        mDoAud_seStartLevel(Z2SE_OBJ_L8_B_FALL_S, &i_this->field_0x57c[0], temp * 100.0f, 0);
    }

    return 1;
}

static int daKytag12_Execute_arrival(kytag12_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    dKankyo_evil_Packet* temp_r25 = dKy_getEnvlight()->mpEvilPacket;

    f32 var_f27;
    f32 var_f22;
    f32 var_f21;

    int var_r22 = 0;
    int var_r21 = 0;
    int var_r20 = 0;
    int var_r19 = 0;

    if (temp_r25 == NULL) {
        return 1;
    }

    cXyz sp80;
    cXyz sp8C;

    for (int i = 0; i < g_env_light.field_0x1054; i++) {
        EF_EVIL_EFF* effect_p = &temp_r25->mEffect[i];
        var_f27 = 1.0f;

        var_f22 = cM_ssin(effect_p->field_0x3c);
        effect_p->field_0x3c += ((f32)(i & 240) * 4.8f);

        var_f21 = cM_ssin(effect_p->field_0x3e);
        effect_p->field_0x3e += ((f32)(i & 240) * 3.8);

        if (effect_p->field_0x38 < 9000.0f) {
            if (a_this->home.roomNo == 2) {
                effect_p->field_0x38 = (var_f21 * 10.0f + 85.0f) * 0.8f;
            } else {
                effect_p->field_0x38 = (var_f21 * 10.0f + 85.0f) * 0.6f;
            }
        }

        switch (effect_p->mStatus) {
        case 0: {
            s16 temp_r3 = cM_rndF(0x10000);

            sp80.x = cM_rndFX(cM_ssin(temp_r3) * 1000.0f);
            sp80.y = cM_rndF(100.0f);
            sp80.z = cM_rndFX(cM_scos(temp_r3) * 1000.0f);

            effect_p->mBasePos.x = a_this->current.pos.x + sp80.x;
            effect_p->mBasePos.y = a_this->current.pos.y + sp80.y + 35.0f;
            effect_p->mBasePos.z = a_this->current.pos.z + sp80.z;

            effect_p->field_0x24 = 1.0f;
            effect_p->field_0x38 = fabsf(var_f22) * 50.0f + 100.0f;

            effect_p->field_0x40 = 0;
            effect_p->field_0x42 = 0;
            effect_p->field_0x2c = 0.0f;
            effect_p->mPosition.x = -sp80.x;
            effect_p->mPosition.y = -sp80.y;
            effect_p->mPosition.z = -sp80.z;
            effect_p->mStatus++;
            break;
        }
        case 1: {
            sp80.x = 0.0f;
            sp80.y = effect_p->mPosition.y;
            sp80.z = 0.0f;

            f32 var_f20 = effect_p->mPosition.abs(sp80);
            if (var_f20 >= 20.0f) {
                var_f20 -= 20.0f;
            } else {
                var_f20 = 0.0f;
            }

            cLib_addCalc(&effect_p->mPosition.x, var_f21 * 20.0f, 0.1f, 20.0f, 0.01f);
            cLib_addCalc(&effect_p->mPosition.y,
                         (var_f22 + 1.0f) * 20.0f + (var_f20 / 1000.0f) * 1000.0f, 0.5f, 32.0f,
                         0.01f);
            cLib_addCalc(&effect_p->mPosition.z, var_f21 * 20.0f, 0.1f, 10.0f, 0.01f);
            break;
        }
        case 2:
            effect_p->mPosition.x = var_f21 * 20.0f;
            effect_p->mPosition.y = (var_f22 + 1.0f) * 20.0f;
            effect_p->mPosition.z = var_f21 * 20.0f;

            if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 && a_this->home.roomNo == 1 &&
                i < 1600)
            {
                effect_p->mBasePos.y += cM_rndF(-4.0f) + -4.0f;

                if (i >= 800) {
                    if (effect_p->mBasePos.y < -1100.0f) {
                        effect_p->mBasePos.y = 900.0f;
                    }
                } else if (effect_p->mBasePos.y < -680.0f) {
                    effect_p->mBasePos.y = 1400.0f;
                }
            }
            break;
        case 3:
            sp80 = effect_p->mBasePos + effect_p->mPosition;
            var_f27 = 0.0f;

            sp8C.x = cM_rndFX(0.5f);
            sp8C.z = cM_rndFX(0.5f);
            sp8C.y = 1.0f;

            effect_p->mPosition.x += sp8C.x * 25.0f;
            effect_p->mPosition.y += sp8C.y * 25.0f;
            effect_p->mPosition.z += sp8C.z * 25.0f;

            if (effect_p->field_0x38 > 4.0f) {
                effect_p->field_0x38 -= 4.0f;
            }

            if (effect_p->field_0x24 <= 0.0f) {
                effect_p->mStatus = 2;
            }
            break;
        }

        for (int j = 0; j < 10; j++) {
            if (g_env_light.dalkmist_influence[j] != NULL &&
                g_env_light.dalkmist_influence[j]->mPos.abs(effect_p->mBasePos) <
                    g_env_light.dalkmist_influence[j]->field_0xc + 100.0f)
            {
                if (effect_p->mStatus != 3 && effect_p->field_0x40 == 0 && var_r21 < 1) {
                    sp80 = effect_p->mBasePos;

                    mDoAud_seStart(Z2SE_OBJ_L8_B_FOG_FLY, &sp80, 0, 0);
                    var_r21++;
                }

                effect_p->mStatus = 3;
                effect_p->field_0x40 = 180;
                break;
            }
        }

        sp80 = effect_p->mBasePos + effect_p->mPosition;

        if (player->current.pos.y - 100.0f < sp80.y && player->current.pos.y + 250.0f > sp80.y) {
            sp80.y = player->current.pos.y;

            if (player->current.pos.abs(sp80) < 190.0f && var_f27 > 0.0f &&
                effect_p->field_0x24 >= 0.95f)
            {
                var_r20++;
            }
        }

        if (!daPy_py_c::checkNowWolf()) {
            if (effect_p->field_0x38 >= 9000.0f) {
                var_f27 = 0.0f;
            }

            sp80 = effect_p->mBasePos + effect_p->mPosition;
            cXyz sp98;
            sp80.y -= 100.0f;

            f32 sp34;
            dKyw_pntlight_collision_get_info(&sp80, &sp98, &sp34);

            if (sp34 > 0.2f) {
                if (effect_p->mStatus != 3 && effect_p->field_0x40 == 0 && var_r21 < 1) {
                    cXyz spA4;
                    spA4 = effect_p->mBasePos;

                    mDoAud_seStart(Z2SE_OBJ_L8_B_FOG_FLY, &spA4, 0, 0);
                    var_r21++;
                }

                effect_p->mStatus = 3;
                effect_p->field_0x40 = 360;
            }

            sp80 = effect_p->mBasePos + effect_p->mPosition;

            if (player->current.pos.y - 100.0f < sp80.y && player->current.pos.y + 250.0f > sp80.y)
            {
                sp80.y = player->current.pos.y;

                if (player->current.pos.abs(sp80) < 40.0f && var_f27 > 0.0f &&
                    effect_p->field_0x24 >= 0.95f && effect_p->mStatus != 3)
                {
                    var_r22++;
                }
            }
        }

        if (var_f27 <= 0.0f || effect_p->field_0x40 == 0) {
            if (var_f27 <= 0.0f) {
                cLib_addCalc(&effect_p->field_0x24, var_f27, 0.25f, 0.07f, 0.0001f);
            } else {
                cLib_addCalc(&effect_p->field_0x24, var_f27, 0.1f, 0.01f, 0.001f);
            }
        }

        if (!dComIfGp_event_runCheck() && effect_p->field_0x40 != 0) {
            effect_p->field_0x40--;
        }

        if (effect_p->field_0x24 > 0.95f) {
            var_r19++;
        }
    }

    if (var_r22 != 0) {
        if (dComIfGp_roomControl_getStayNo() == 11) {
            i_this->field_0x578 = 60;
        } else if (i_this->field_0x578 < 40) {
            i_this->field_0x578++;
        }
    } else {
        if (i_this->field_0x578 != 0) {
            i_this->field_0x578--;
        }
    }

    if (i_this->field_0x578 >= 20 && !player->checkMetamorphose()) {
        player->onForceWolfChange();
    }

    daKytag12_light_swprd_proc(i_this);

    g_env_light.mEvilInitialized &= 0x7F;

    if (daPy_py_c::checkNowWolf() && var_r20 > 0) {
        g_env_light.mEvilInitialized |= 0x80;
    }

    f32 temp = ((f32)var_r19 / (f32)g_env_light.field_0x1054);
    mDoAud_seStartLevel(Z2SE_OBJ_L8_B_FOG_STAY, &i_this->current.pos, temp * 100.0f, 0);
    return 1;
}

static int daKytag12_Execute_R00(kytag12_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    dKankyo_evil_Packet* temp_r25 = dKy_getEnvlight()->mpEvilPacket;

    f32 var_f26;
    f32 var_f25;

    int var_r24 = 0;

    if (temp_r25 == NULL) {
        return 1;
    }

    cXyz sp80;
    cXyz sp8C;
    cXyz sp98;

    for (int i = 0; i < g_env_light.field_0x1054; i++) {
        EF_EVIL_EFF* effect_p = &temp_r25->mEffect[i];
        var_f25 = 1.0f;

        f32 f31;
        switch (effect_p->mStatus) {
        case 0:
            if (i < 500) {
                effect_p->mBasePos.x = a_this->current.pos.x + cM_rndFX(550.0f);
                effect_p->mBasePos.y = cM_rndF(5900.0f) + -3000.0f;
                effect_p->mBasePos.z = a_this->current.pos.z + cM_rndFX(80.0f);

                effect_p->field_0x24 = 1.0f;
                effect_p->field_0x38 = fabsf(var_f26) * 50.0f + 100.0f;
            } else if (i >= 500 && i < 1000) {
                effect_p->mBasePos.x = cM_rndFX(250.0f) + 244.0f;
                effect_p->mBasePos.y = cM_rndF(4100.0f) + 2951.0f;
                effect_p->mBasePos.z = cM_rndFX(50.0f) + 2206.0f;

                effect_p->field_0x24 = 1.0f;
                effect_p->field_0x38 = fabsf(var_f26) * 50.0f + 100.0f;
            } else if (i >= 1000 && i < 1500) {
                effect_p->mBasePos.y = cM_rndF(6800.0f) + -3200.0f;
                effect_p->mBasePos.z = cM_rndFX(480.0f) + 5350.0f;

                f31 = (effect_p->mBasePos.y / 1000.0f);
                effect_p->mBasePos.x = f31 * -200.0f + -3000.0f;

                effect_p->field_0x24 = 1.0f;
                effect_p->field_0x38 = fabsf(var_f26) * 50.0f + 150.0f;
            } else if (i >= 1500 && i < 2000) {
                effect_p->mBasePos.y = cM_rndF(6800.0f) + -3200.0f;
                effect_p->mBasePos.z = cM_rndFX(510.0f) + 5300.0f;

                f31 = (effect_p->mBasePos.y / 1000.0f);
                effect_p->mBasePos.x = f31 * -200.0f + 3400.0f;

                effect_p->field_0x24 = 1.0f;
                effect_p->field_0x38 = fabsf(var_f26) * 50.0f + 150.0f;
            } else {
                effect_p->mBasePos.x = cM_rndFX(650.0f);
                effect_p->mBasePos.y = cM_rndF(3000.0f);
                effect_p->mBasePos.z = cM_rndFX(100.0f);

                effect_p->field_0x24 = 1.0f;
                effect_p->field_0x38 = fabsf(var_f26) * 50.0f + 100.0f;
            }

            effect_p->field_0x40 = 0;
            effect_p->field_0x2c = 0.0f;
            effect_p->mStatus++;
            /* fallthrough */
        case 1: {
            var_f26 = cM_ssin(effect_p->field_0x3c);
            effect_p->field_0x3c += ((f32)(i & 240) * 4.8f);

            f32 temp_f7 = cM_ssin(effect_p->field_0x3e);
            effect_p->field_0x3e += ((f32)(i & 240) * 3.8);

            effect_p->mPosition.x = temp_f7 * 20.0f;
            effect_p->mPosition.y = (var_f26 + 1.0f) * 20.0f;
            effect_p->mPosition.z = temp_f7 * 20.0f;

            if (effect_p->field_0x38 < 9000.0f) {
                effect_p->field_0x38 = (temp_f7 * 10.0f + 85.0f) * 1.5f;
            }
            break;
        }
        case 2:
            sp80 = effect_p->mBasePos + effect_p->mPosition;
            var_f25 = 0.0f;

            sp8C.x = cM_rndFX(0.5f);
            sp8C.z = cM_rndFX(0.5f);
            sp8C.y = 1.0f;

            effect_p->mPosition.x += sp8C.x * 40.0f;
            effect_p->mPosition.y += sp8C.y * 40.0f;
            effect_p->mPosition.z += sp8C.z * 40.0f;

            if (effect_p->field_0x38 > 4.0f) {
                effect_p->field_0x38 -= 4.0f;
            }

            if (effect_p->field_0x24 <= 0.0f && effect_p->field_0x40 == 0) {
                effect_p->mStatus = 1;
            }
            break;
        }

        effect_p->mBasePos.y -= ((f32)i / (f32)g_env_light.field_0x1054) * 10.0f + 15.0f;

        if (i < 500) {
            if (effect_p->mBasePos.y < -3000.0f) {
                effect_p->mBasePos.y = 2900.0f;
                effect_p->field_0x24 = 0.0f;
                effect_p->mStatus = 1;
            }
        } else if (i >= 500 && i < 1000) {
            if (effect_p->mBasePos.y < 2951.0f) {
                effect_p->mBasePos.y = 7000.0f;
                effect_p->field_0x24 = 0.0f;
                effect_p->mStatus = 1;
            }
        } else if (i >= 1000 && i < 1500) {
            if (effect_p->mBasePos.y < -3800.0f) {
                effect_p->mBasePos.y = 2800.0f;
                effect_p->field_0x24 = 0.0f;
                effect_p->mStatus = 1;
            }

            f31 = (effect_p->mBasePos.y / 1000.0f);
            effect_p->mBasePos.x = f31 * -200.0f + -3000.0f;

            if (effect_p->mBasePos.y < -680.0f) {
                f31 = (effect_p->mBasePos.y / 1000.0f);
                effect_p->mBasePos.x = f31 * -20.0f + -2250.0f;
            } else if (effect_p->mBasePos.y < 0.0f) {
                f31 = (effect_p->mBasePos.y / 1000.0f);
                effect_p->mBasePos.x = f31 * -1160.0f + -3000.0f;
                effect_p->mBasePos.y += 10.0f;
            }
        } else if (i >= 1500 && i < 2000) {
            if (effect_p->mBasePos.y < -3800.0f) {
                effect_p->mBasePos.y = 2800.0f;
                effect_p->field_0x24 = 0.0f;
                effect_p->mStatus = 1;
            }

            f31 = (effect_p->mBasePos.y / 1000.0f);
            effect_p->mBasePos.x = f31 * 200.0f + 3400.0f;

            if (effect_p->mBasePos.y < -380.0f) {
                f31 = (effect_p->mBasePos.y / 1000.0f);
                effect_p->mBasePos.x = f31 * -20.0f + 2700.0f;
            } else if (effect_p->mBasePos.y < 200.0f) {
                f31 = (effect_p->mBasePos.y / -800.0f);
                effect_p->mBasePos.x = f31 * -1060.0f + 3200.0f;
                effect_p->mBasePos.y += 10.0f;
            }
        }

        if (i_this->field_0x575 == 0 && player->current.pos.z < 2700.0f &&
            player->current.pos.z > 2600.0f)
        {
            player->current.pos.z = 2650.0f;
        }

        sp80 = effect_p->mBasePos + effect_p->mPosition;
        sp80.y = player->current.pos.y;

        if (player->current.pos.abs(sp80) < 600.0f &&
            dComIfGs_getSelectEquipSword() == fpcNm_ITEM_LIGHT_SWORD && player->getCutType() != daPy_py_c::CUT_TYPE_NONE &&
            d_kytag12_cut_turn_check())
        {
            i_this->field_0x575 = 1;
        }

        if (i_this->field_0x575 == 1) {
            mDoAud_seStart(Z2SE_OBJ_L8_B_FOG_FLY, &sp80, 0, 0);
            i_this->field_0x575 = 2;
            effect_p->mStatus = 2;
            effect_p->field_0x40 = 360;
        }

        for (int j = 0; j < 10; j++) {
            if (g_env_light.dalkmist_influence[j] != NULL &&
                g_env_light.dalkmist_influence[j]->mPos.abs(effect_p->mBasePos) <
                    g_env_light.dalkmist_influence[j]->field_0xc + 100.0f)
            {
                if (effect_p->mStatus != 2 && effect_p->field_0x40 == 0 && var_r24 < 1) {
                    cXyz spA4;
                    spA4 = effect_p->mBasePos;

                    var_r24++;
                }

                effect_p->mStatus = 2;
                effect_p->field_0x40 = 180;
                break;
            }
        }

        if (!daPy_py_c::checkNowWolf()) {
            if (effect_p->field_0x38 >= 9000.0f) {
                var_f25 = 0.0f;
            }

            sp80 = effect_p->mBasePos + effect_p->mPosition;
            cXyz spB0;
            sp80.y -= 100.0f;

            f32 sp34;
            dKyw_pntlight_collision_get_info(&sp80, &spB0, &sp34);

            if (sp34 > 0.2f) {
                if (effect_p->mStatus != 2 && effect_p->field_0x40 == 0 && var_r24 < 1) {
                    cXyz spBC;
                    spBC = effect_p->mBasePos;

                    mDoAud_seStart(Z2SE_OBJ_L8_B_FOG_FLY, &spBC, 0, 0);
                    var_r24++;
                }

                effect_p->mStatus = 2;
                effect_p->field_0x40 = 360;
            }
        }

        if (var_f25 <= 0.0f) {
            cLib_addCalc(&effect_p->field_0x24, var_f25, 0.25f, 0.07f, 0.0001f);
        } else {
            cLib_addCalc(&effect_p->field_0x24, var_f25, 0.25f, 0.1f, 0.0001f);
        }

        if (effect_p->field_0x40 != 0) {
            effect_p->field_0x40--;

            if (effect_p->field_0x40 == 0) {
                i_this->field_0x575 = 0;
            }
        }
    }

    i_this->field_0x578 = 0;

    daKytag12_light_swprd_proc(i_this);

    i_this->field_0x57c[0].y = player->current.pos.y;
    i_this->field_0x57c[1].y = player->current.pos.y;
    i_this->field_0x57c[2].y = player->current.pos.y;
    i_this->field_0x57c[0].x = a_this->current.pos.x;
    i_this->field_0x57c[0].z = a_this->current.pos.z;

    mDoAud_seStartLevel(Z2SE_OBJ_L8_B_FALL_L, &i_this->field_0x57c[0], 100, 0);

    i_this->field_0x57c[1].x = -3000.0f;
    i_this->field_0x57c[1].z = 5350.0f;
    mDoAud_seStartLevel(Z2SE_OBJ_L8_B_FALL_L, &i_this->field_0x57c[1], 100, 0);

    i_this->field_0x57c[2].x = 3400.0f;
    i_this->field_0x57c[2].z = 5300.0f;
    mDoAud_seStartLevel(Z2SE_OBJ_L8_B_FALL_L, &i_this->field_0x57c[2], 100, 0);
    return 1;
}

static int daKytag12_Execute(kytag12_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    if (g_env_light.field_0x1051 != a_this->home.roomNo) {
        return 0;
    }

    switch (a_this->home.roomNo) {
    case 0:
    case 1:
        g_env_light.field_0x1054 = 2000;
        break;
    case 2:
        g_env_light.field_0x1054 = 500;
        break;
    case 11:
    case 0x33:
    case 0x34:
        g_env_light.field_0x1054 = 1000;
        break;
    default:
        g_env_light.field_0x1054 = 500;
        break;
    }

    if (i_this->field_0x574 == 1) {
        daKytag12_Execute_arrival(i_this);
    } else if (i_this->field_0x574 == 2) {
        daKytag12_Execute_R00(i_this);
    } else {
        daKytag12_Execute_standard(i_this);
    }

    return 1;
}

static int daKytag12_IsDelete(kytag12_class* i_this) {
    return 1;
}

static int daKytag12_Delete(kytag12_class* i_this) {
    g_env_light.field_0x1054 = 0;
    return 1;
}

static int daKytag12_Create(fopAc_ac_c* i_this) {
    kytag12_class* a_this = (kytag12_class*)i_this;
    fopAcM_ct(a_this, kytag12_class);

    a_this->field_0x574 = fopAcM_GetParam(i_this);
    a_this->field_0x576 = 0;
    a_this->field_0x578 = 0;
    a_this->field_0x575 = 0;

    g_env_light.field_0x1054 = 0;
    g_env_light.field_0x1051 = i_this->home.roomNo;
    dKyr_evil_init();
    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daKytag12_Method = {
    (process_method_func)daKytag12_Create,
    (process_method_func)daKytag12_Delete,
    (process_method_func)daKytag12_Execute,
    (process_method_func)daKytag12_IsDelete,
    (process_method_func)daKytag12_Draw,
};

extern actor_process_profile_definition g_profile_KYTAG12 = {
  fpcLy_CURRENT_e,       // mLayerID
  7,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_KYTAG12,          // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(kytag12_class), // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  106,                   // mPriority
  &l_daKytag12_Method,   // sub_method
  0x00044000,            // mStatus
  fopAc_ACTOR_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
