/**
 * d_a_kytag03.cpp
 * Odor generation tag / rail
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag03.h"
#include "SSystem/SComponent/c_counter.h"
#include "SSystem/SComponent/c_math.h"
#include "Z2AudioLib/Z2EnvSeMgr.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo_rain.h"
#include "f_op/f_op_camera_mng.h"

static void dEnvSe_getNearPathPos(cXyz* param_0, cXyz* param_1, dPath* i_path) {
    int var_r31 = 0;
    BOOL sp10[2] = {0, 0};
    f32 sp8;

    f32 var_f31 = FLT_MAX;

    int i;
    dPnt* point_p = i_path->m_points;

    cXyz sp3C[2];
    cM3dGLin sp14;

    for (i = 0; i < i_path->m_num; i++) {
        sp8 = cM3d_LenSq(param_1, &point_p->m_position);
        if (var_f31 > sp8) {
            var_f31 = sp8;
            var_r31 = i;
        }

        point_p++;
    }

    point_p = &i_path->m_points[var_r31];
    if (var_r31 != 0) {
        sp14.set(point_p[-1].m_position, point_p[0].m_position);
        sp10[0] = cM3d_Len3dSqPntAndSegLine(&sp14, param_1, &sp3C[0], &sp8);
    }

    if (var_r31 != i_path->m_num - 1) {
        sp14.set(point_p[0].m_position, point_p[1].m_position);
        sp10[1] = cM3d_Len3dSqPntAndSegLine(&sp14, param_1, &sp3C[1], &sp8);
    }

    if (sp10[0]) {
        if (sp10[1]) {
            if (cM3d_LenSq(&sp3C[1], param_1) > sp8) {
                *param_0 = sp3C[0];
            } else {
                *param_0 = sp3C[1];
            }
        } else {
            *param_0 = sp3C[0];
        }
    } else if (sp10[1]) {
        *param_0 = sp3C[1];
    } else {
        param_0->set(point_p[0].m_position);
    }
}

static cXyz get_rail_ratio_pos(dPath* i_path, int i_pointIdx, f32 param_2, s16* param_3,
                               s16* param_4) {
    cXyz point_a;
    cXyz point_b;
    cXyz ret;
    cXyz calc_vec;

    point_a.x = i_path->m_points[i_pointIdx].m_position.x;
    point_a.y = i_path->m_points[i_pointIdx].m_position.y;
    point_a.z = i_path->m_points[i_pointIdx].m_position.z;

    point_b.x = i_path->m_points[i_pointIdx + 1].m_position.x;
    point_b.y = i_path->m_points[i_pointIdx + 1].m_position.y;
    point_b.z = i_path->m_points[i_pointIdx + 1].m_position.z;

    ret.x = point_a.x + param_2 * (point_b.x - point_a.x);
    ret.y = point_a.y + param_2 * (point_b.y - point_a.y);
    ret.z = point_a.z + param_2 * (point_b.z - point_a.z);

    dKyr_get_vectle_calc(&point_a, &point_b, &calc_vec);

    *param_3 =
        cM_atan2s(JMAFastSqrt(calc_vec.x * calc_vec.x + calc_vec.z * calc_vec.z), calc_vec.y);
    *param_4 = cM_atan2s(calc_vec.x, calc_vec.z);

    return ret;
}

static dPath* set_path_info(fopAc_ac_c* i_actor) {
    dPath* path_p = NULL;
    u8 path_id = fopAcM_GetParam(i_actor) >> 0x10;

    if (path_id != 0xFF) {
        path_p = dPath_GetRoomPath(path_id, fopAcM_GetRoomNo(i_actor));
    }

    return path_p;
}

static void getSound_pos(fopAc_ac_c* i_actor) {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz eye = camera->mCamera.Eye();
    cXyz sp28;

    u8 path_id = fopAcM_GetParam(i_actor) >> 0x10;
    if (path_id != 0xFF) {
        for (dPath* path = dPath_GetRoomPath(path_id, fopAcM_GetRoomNo(i_actor)); path != NULL;
             path = dPath_GetNextRoomPath(path, fopAcM_GetRoomNo(i_actor)))
        {
            dEnvSe_getNearPathPos(&sp28, &eye, path);
            mDoAud_mEnvse_registWolfSmellSePos(&sp28);
        }
    }
}

static dPath* get_Extent_pos_start_get(kytag03_class* i_this, dPath* i_path, cXyz*, f32,
                                       int* param_4) {
    *param_4 = 0;
    return i_path;
}

static dPath* get_Extent_pos_end_get(kytag03_class* i_this, dPath* i_path, cXyz*, f32,
                                     int* param_4) {
    *param_4 = i_path->m_num - 1;
    return i_path;
}

static void odour_move(kytag03_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);

    cXyz sp7C;
    cXyz sp88;

    dScnKy_env_light_c* kankyo = dKy_getEnvlight();

    dPath* var_r27;
    dKankyo_odour_Packet* packet = kankyo->mOdourData.mpOdourPacket;
    dPath* var_r25;

    cXyz sp94;
    cXyz spA0;
    cXyz spAC;

    if (i_this->field_0x585 == dComIfGs_getCollectSmell()) {
        if (i_this->field_0x58e != 0xFF) {
            s32 room_no = dComIfGp_roomControl_getStayNo();
            if (dComIfGs_isSwitch(i_this->field_0x58e, room_no)) {
                return;
            }
        }

        dPath* path = i_this->mpPath;
        if (path != NULL) {
            int sp40;
            int sp38;
            var_r27 = get_Extent_pos_start_get(i_this, path, &camera->lookat.eye, 1000.0f, &sp40);
            var_r25 = get_Extent_pos_end_get(i_this, path, &camera->lookat.eye, 1000.0f, &sp38);

            s16 spA;
            s16 sp8;
            sp7C = get_rail_ratio_pos(var_r27, 0, 0.0f, &spA, &sp8);
            sp88 = get_rail_ratio_pos(var_r25, sp38 - 1, 1.0f, &spA, &sp8);

            if (packet != NULL) {
                packet->field_0x17714 = sp7C;
            }

            for (int i = sp40; i < sp38; i++) {
                if (sp40 != sp38 && i == sp38) {
                    break;
                }

                spA0 = get_rail_ratio_pos(var_r27, i, 0.0f, &spA, &sp8);
                spAC = get_rail_ratio_pos(var_r27, i, 1.0f, &spA, &sp8);

                f32 temp_f26 = 55.0f / spA0.abs(spAC);
                for (f32 var_f25 = 0.0f; var_f25 <= 1.0f - (temp_f26 * 1.5f); var_f25 += temp_f26) {
                    if ((g_Counter.mCounter0 & 15) == ((int)(var_f25 * 16.0f) & 15)) {
                        sp94 = get_rail_ratio_pos(var_r27, i, var_f25, &spA, &sp8);

                        f32 var_f23 = sp7C.abs(sp94);
                        f32 temp_f1_2 = sp7C.abs(sp88);
                        if (temp_f1_2 > 0.0f) {
                            var_f23 = 1.0f - (var_f23 / temp_f1_2);
                        }

                        if (var_f23 < 0.0f) {
                            var_f23 = 0.0f;
                        } else if (var_f23 >= 1.0f) {
                            var_f23 = 1.0f;
                        }

                        if (i_this->field_0x586 == 1 || i_this->field_0x586 == 3) {
                            var_f23 = 1.0f;
                        } else if (i_this->field_0x586 == 2 || i_this->field_0x586 == 4) {
                            var_f23 = sp94.abs(sp88) / 5000.0f;

                            if (var_f23 < 0.0f) {
                                var_f23 = 0.0f;
                            } else if (var_f23 >= 1.0f) {
                                var_f23 = 1.0f;
                            }
                        }

                        f32 temp_f22 = (1.0f - var_f23);
                        temp_f22 *= temp_f22;
                        var_f23 *= i_this->field_0x57c;

                        f32 var_f1_4 = camera->lookat.eye.abs(sp94);
                        if (var_f23 > 0.0f && var_f1_4 < 4000.0f) {
                            csXyz spFC;
                            cXyz spB8;

                            spFC.x = spA + (s16)cM_rndFX(temp_f22 * 3000.0f + 2000.0f);
                            spFC.y = sp8 + (s16)cM_rndFX(temp_f22 * 3000.0f + 2000.0f);
                            spFC.z = 0;

                            f32 temp_f0_16 = (cM_rndF(1.0f) * 0.5f) + 1.5f;
                            spB8.x = temp_f0_16;
                            spB8.y = temp_f0_16;
                            spB8.z = temp_f0_16;

                            if (packet != NULL) {
                                int j;
                                for (j = 0; j < 2000; j++) {
                                    EF_ODOUR_EFF* effect = &packet->mOdourEff[j];

                                    if (effect->mStatus == 0) {
                                        if (i_this->field_0x586 != 3 && i_this->field_0x586 != 4) {
                                            effect->mStatus = 1;
                                        } else {
                                            effect->mStatus = 11;
                                        }

                                        effect->mBasePos = sp94;
                                        effect->mBasePos.x += temp_f22 * cM_rndFX(50.0f);

                                        if (i_this->field_0x586 != 3 && i_this->field_0x586 != 4) {
                                            effect->mBasePos.y +=
                                                temp_f22 * 200.0f + cM_rndF(40.0f);
                                            effect->field_0x20 = cM_rndF(1.0f) + 0.8f +
                                                                 temp_f22 * (cM_rndF(2.0f) + 2.0f);
                                        } else {
                                            effect->field_0x20 = cM_rndF(1.0f) + 2.8f +
                                                                 temp_f22 * (cM_rndF(2.0f) + 2.0f);
                                        }

                                        effect->mBasePos.z += temp_f22 * cM_rndFX(50.0f);
                                        effect->mRotX = spFC.x;
                                        effect->mRotY = spFC.y;

                                        effect->field_0x2c = cM_rndF(20.0f) + 45.0f +
                                                             temp_f22 * (cM_rndF(20.0f) + 100.0f);
                                        effect->field_0x28 = var_f23;
                                        break;
                                    }
                                }

                                if (j == 2000) {
                                    // "\nThere is no free space on the smell rail!"
                                    OS_REPORT("\n臭いレールで臭いの空きがありませんでした！");
                                }
                            }
                        }
                    }
                }
            }
            getSound_pos(a_this);
        }
    }
}

static int daKytag03_Draw(kytag03_class* i_this) {
    return 1;
}

static int daKytag03_Execute(kytag03_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    BOOL var_r28 = false;

    a_this->attention_info.position = a_this->current.pos;

    if (!i_this->field_0x584) {
        odour_move(i_this);
    } else {
        cLib_addCalc(&i_this->field_0x578, 1.0f, 0.1f, 0.025f, 0.000001f);

        if (i_this->field_0x585 == 0xB5) {
            fopAc_ac_c* actor = (fopAc_ac_c*)fpcM_SearchByName(PROC_Obj_Movebox);
            if (actor != NULL && actor->current.pos.x < 7425.0f) {
                i_this->field_0x578 = 0.0f;
                var_r28 = true;
            }
        }

        if (i_this->mpEmitter != NULL) {
            static s16 S_ang_y_work = 0;

            cXyz sp2C;
            i_this->mpEmitter->setGlobalTranslation(
                a_this->current.pos.x, a_this->current.pos.y + 30.0f, a_this->current.pos.z);

            sp2C.x = 3.0f;
            sp2C.y = 4.0f;
            sp2C.z = 3.0f;
            i_this->mpEmitter->setGlobalScale(sp2C);

            JGeometry::TVec3<s16> rot(4000, S_ang_y_work, 1500);
            S_ang_y_work += 100;

            i_this->mpEmitter->setGlobalRotation(rot);
            i_this->mpEmitter->setRate(0.5f);
            i_this->mpEmitter->setDirectionalSpeed(2.5f);
            i_this->mpEmitter->setRandomDirectionSpeed(1.5f);

            sp2C.x = i_this->field_0x578 * 2.2f;
            sp2C.y = i_this->field_0x578 * 0.75f;
            sp2C.z = i_this->field_0x578 * 2.2f;
            i_this->mpEmitter->setGlobalDynamicsScale(sp2C);

            if (i_this->field_0x585 == 0xB0) {
                i_this->mpEmitter->setGlobalPrmColor(0xFF, 0xFF, 0xFF);
                i_this->mpEmitter->setGlobalEnvColor(0xFF, 0x78, 0xA8);
            } else if (i_this->field_0x585 == 0xB3) {
                i_this->mpEmitter->setGlobalPrmColor(0xFF, 0xA1, 0x3B);
                i_this->mpEmitter->setGlobalEnvColor(0xD6, 0x60, 0x32);
            } else if (i_this->field_0x585 == 0xB4) {
                i_this->mpEmitter->setGlobalPrmColor(0xFF, 0xDB, 0xFF);
                i_this->mpEmitter->setGlobalEnvColor(0xE4, 0xAC, 0x52);
            } else if (i_this->field_0x585 == 0xB5) {
                i_this->mpEmitter->setGlobalPrmColor(0xD5, 0xCE, 0xA7);
                i_this->mpEmitter->setGlobalEnvColor(0x55, 0x78, 0x27);

                JGeometry::TVec3<s16> rot(4000, 0, 1500);
                i_this->mpEmitter->setGlobalRotation(rot);
                i_this->mpEmitter->setRandomDirectionSpeed(1.0f);

                sp2C.x = i_this->field_0x578 * 2.2f;
                sp2C.y = i_this->field_0x578 * 1.5f;
                sp2C.z = i_this->field_0x578 * 2.2f;
                i_this->mpEmitter->setGlobalDynamicsScale(sp2C);
            } else if (i_this->field_0x585 == 0xB2) {
                i_this->mpEmitter->setGlobalPrmColor(0xFF, 0xC8, 0xFF);
                i_this->mpEmitter->setGlobalEnvColor(0x44, 0x1A, 0x6B);
            }

            i_this->mpEmitter->setGlobalAlpha(kankyo->senses_effect_strength * 223.0f + 32.0f);
        }

        if (i_this->field_0x585 != dComIfGs_getCollectSmell()) {
            i_this->field_0x589 = i_this->field_0x585;

            if (i_this->field_0x587 == 0) {
                if (player->current.pos.abs(a_this->current.pos) < i_this->field_0x574 &&
                    daPy_py_c::checkNowWolf() && !var_r28)
                {
                    i_this->attention_info.distances[fopAc_attn_ETC_e] = 0x3D;
                    a_this->attention_info.flags |= fopAc_AttnFlag_ETC_e;
                } else {
                    a_this->attention_info.flags &= ~fopAc_AttnFlag_ETC_e;
                }

                if (i_this->field_0x588 == 1) {
                    a_this->attention_info.flags &= ~fopAc_AttnFlag_ETC_e;
                    i_this->field_0x587 = 1;
                    i_this->field_0x58a = 30;
                }
            } else if (i_this->field_0x587 == 1) {
                if (i_this->field_0x58a != 0) {
                    i_this->field_0x58a--;
                } else {
                    i_this->field_0x588 = 2;
                    i_this->field_0x587 = 2;
                }
            } else if (i_this->field_0x588 == 0) {
                i_this->field_0x587 = 0;
            }
        } else {
            a_this->attention_info.flags &= ~fopAc_AttnFlag_ETC_e;
        }
    }

    return 1;
}

static int daKytag03_IsDelete(kytag03_class* i_this) {
    return 1;
}

static int daKytag03_Delete(kytag03_class* i_this) {
    if (i_this->mpEmitter != NULL) {
        i_this->mpEmitter->deleteAllParticle();
        i_this->mpEmitter->becomeInvalidEmitter();
        i_this->mpEmitter->quitImmortalEmitter();
        i_this->mpEmitter->setEmitterCallBackPtr(NULL);
        i_this->mpEmitter = NULL;
    }
    return 1;
}

static int daKytag03_Create(fopAc_ac_c* i_this) {
    kytag03_class* a_this = (kytag03_class*)i_this;
    fopAcM_ct(i_this, kytag03_class);

    a_this->field_0x584 = (fopAcM_GetParam(i_this) >> 0x18) & 3;
    a_this->field_0x585 = fopAcM_GetParam(i_this);
    a_this->field_0x586 = fopAcM_GetParam(i_this) >> 8;
    a_this->mpEmitter = NULL;
    a_this->field_0x588 = 0;
    a_this->field_0x58a = 0;
    a_this->field_0x587 = 0;
    a_this->field_0x578 = 1.0f;
    a_this->field_0x57c = (f32)(u8)((i_this->current.angle.x >> 8) & 0xFF) / 255.0f;

    if (i_this->scale.x > 1.0f) {
        a_this->field_0x574 = i_this->scale.x * 50.0f;
    } else {
        a_this->field_0x574 = 300.0f;
    }

    a_this->field_0x58e = i_this->current.angle.x & 0xFF;

    if (a_this->field_0x584 == 0) {
        a_this->mpPath = set_path_info(i_this);
        a_this->field_0x570 = 0.0f;
        a_this->field_0x580 = 0;
        g_env_light.mOdourData.field_0xf21 = 1;
    } else {
        if (a_this->field_0x585 == 0xFF) {
            return cPhs_UNK3_e;
        }

        a_this->mpEmitter = dComIfGp_particle_set(0x84ED, &i_this->current.pos, NULL, NULL);
        if (a_this->mpEmitter != NULL) {
            a_this->mpEmitter->setGlobalEnvColor(0x64, 0xBD, 0x73);
            a_this->mpEmitter->setGlobalPrmColor(0x83, 0xF3, 0x68);
        }
    }

    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daKytag03_Method = {
    (process_method_func)daKytag03_Create,  (process_method_func)daKytag03_Delete,
    (process_method_func)daKytag03_Execute, (process_method_func)daKytag03_IsDelete,
    (process_method_func)daKytag03_Draw,
};

actor_process_profile_definition g_profile_KYTAG03 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KYTAG03,
    &g_fpcLf_Method.base,
    sizeof(kytag03_class),
    0,
    0,
    &g_fopAc_Method.base,
    97,
    &l_daKytag03_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
