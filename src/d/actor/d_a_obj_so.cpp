/**
 * @file d_a_obj_so.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_so.h"
#include "d/d_cc_d.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_w.h"
#include "d/actor/d_a_obj_pillar.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_uty.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"

daObj_So_HIO_c::daObj_So_HIO_c() {
    field_0x4 = -1;
    mScale = 0.8f;
}

static int daObj_So_Draw(obj_so_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    g_env_light.settingTevStruct(16, &a_this->current.pos, &a_this->tevStr);

    if (a_this->health != 0) {
        g_env_light.setLightTevColorType_MAJI(i_this->field_0x574, &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->field_0x574);
    }

    for (int i = 0; i < 26; i++) {
        if (i_this->field_0x5fc[i] != 0) {
            g_env_light.setLightTevColorType_MAJI(i_this->field_0x594[i], &a_this->tevStr);
            mDoExt_modelUpdateDL(i_this->field_0x594[i]);

            if (i_this->field_0xdae != 1 && i >= 2) {
                if (i < 10 && i_this->field_0x1a98[i - 2] != 0) {
                    g_env_light.setLightTevColorType_MAJI(i_this->field_0x1aa0[i - 2], &a_this->tevStr);
                    mDoExt_modelUpdateDL(i_this->field_0x1aa0[i - 2]);
                }   
            }
        }
    }

    return 1;
}

static void ride_call_back_0(dBgW* i_BgW, fopAc_ac_c* a_this, fopAc_ac_c* param_3) {
    obj_so_class* i_this = (obj_so_class*)a_this;

    i_this->field_0xa28[0].x = 0;
    i_this->field_0xc30[0] = 3;
}

static void ride_call_back_1(dBgW* i_BgW, fopAc_ac_c* a_this, fopAc_ac_c* param_3) {
    obj_so_class* i_this = (obj_so_class*)a_this;

    i_this->field_0xa28[1].x = 0;
    i_this->field_0xc30[1] = 3;
}

static void so_hasira(obj_so_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    daPillar_c* pillar_p = (daPillar_c*)fopAcM_SearchByName(PROC_Obj_Pillar);
    if (pillar_p != NULL) {
        mDoMtx_stack_c::transS(pillar_p->current.pos.x, pillar_p->current.pos.y, pillar_p->current.pos.z);
        mDoMtx_stack_c::YrotM(pillar_p->mRotY);
        mDoMtx_stack_c::XrotM(pillar_p->shape_angle.x);
        mDoMtx_stack_c::YrotM(-pillar_p->mRotY);
        mDoMtx_stack_c::transM(0.0f, 510.0f, 0.0f);

        mDoMtx_multVecZero(mDoMtx_stack_c::get(), &a_this->current.pos);
        a_this->shape_angle.y = -0x5000;
        i_this->field_0xdc2 = -i_this->field_0xdc8 * cM_ssin(i_this->field_0xdac * 0xF00);
        cLib_addCalc0(&i_this->field_0xdc8, 0.1f, 33.0f);

        if (i_this->field_0xdb2[0] == 1 && !dComIfGp_event_runCheck() && i_this->field_0xdc0 == 0) {
            i_this->field_0xdc0++;
        }

        if (daPy_getPlayerActorClass()->checkFrontRollCrash() && fopAcM_searchPlayerDistanceXZ(a_this) < 200.0f) {
            OS_REPORT("OBJ_SO HA HIT \n");
            i_this->field_0xdb2[0] = 20;
            i_this->field_0xdc4 = 1;
            if (i_this->field_0xdc0 != 0 && a_this->shape_angle.x > 0x100) {
                i_this->field_0xdc0++;
                if (i_this->field_0xdc0 >= 2) {
                    i_this->field_0xdae = 1;
                    i_this->field_0xdb0 = 0;
                    i_this->mDemoMode = 1;
                    i_this->field_0xdbe = 0;
                    a_this->shape_angle.x = 0;
                    i_this->field_0xdb2[0] = 18;
                    return;
                }
            }
        }

        i_this->field_0xdbe += 5000;
        s16 target = pillar_p->shape_angle.x << 1;
        if (a_this->shape_angle.x < target) {
            a_this->shape_angle.x = target;
        } else {
            cLib_addCalcAngleS2(&a_this->shape_angle.x, target, 16, 20);
        }
    }
}

static void part_set(obj_so_class* i_this, s8 param_2) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp48, sp54;

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM((s16)a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM((s16)a_this->shape_angle.x);

    f32 scale = a_this->scale.x;
    mDoMtx_stack_c::scaleM(scale, scale, scale);
    
    for (int i = 0; i < 26; i++) {
        mDoMtx_stack_c::push();
        if (i < 2) {
            sp48.x = 0.0f;
            if (i == 0) {
                sp48.y = VREG_F(6) + 210.0f;
            } else {
                sp48.y = VREG_F(7);
            }
            sp48.z = 0.0f;
            mDoMtx_stack_c::multVec(&sp48, &i_this->field_0x618[i]);
            i_this->field_0x7b8[i].set(0.8f, 1.0f, 0.8f);
            i_this->field_0xa28[i].y = a_this->shape_angle.y;
            if (i_this->field_0x570 != 1) {
                i_this->field_0xa28[i].x = a_this->shape_angle.x + (int)cM_rndFX(4000.0f);
            }
        } else if (i < 10) {
            mDoMtx_stack_c::YrotM(8092 * i - 0xF18);
            sp48.x = 0.0f;
            sp48.y = 110.0f;
            sp48.z = 90.0f;
            mDoMtx_stack_c::multVec(&sp48, &i_this->field_0x618[i]);
            i_this->field_0x7b8[i].set(1.0f, 2.0f, 1.0f);
            i_this->field_0xa28[i].y = a_this->shape_angle.y;
            i_this->field_0xa28[i].x = a_this->shape_angle.x;

            if (i_this->field_0x1a98[i - 2] != 0) {
                i_this->field_0x7b8[i].y = 1.0f;
                i_this->field_0x618[i].y += 60.0f;
            }
        } else {
            mDoMtx_stack_c::YrotM(8092 * i + 18200);
            sp48.x = 0.0f;
            if (i <= 17) {
                sp48.y = 205.0f;
            } else {
                sp48.y = -5.0f;
            }
            if ((i & 1) != 0) {
                sp48.y += 8.0f;
            }
            sp48.z = 95.0f;

            mDoMtx_stack_c::multVec(&sp48, &i_this->field_0x618[i]);
            i_this->field_0xa28[i].y = a_this->shape_angle.y;
            i_this->field_0xa28[i].x = a_this->shape_angle.x;
            i_this->field_0xb60[i] = 8092 * i + 0x4CE;
            i_this->field_0x7b8[i].set(1.0f, 0.8f, 1.0f);
        }

        mDoMtx_stack_c::pop();
        i_this->field_0x5fc[i] = param_2;
        if (param_2 == 1) {
            sp48.x = 0.0f;
            i_this->field_0xc30[i] = 0;
            if (i_this->field_0x570 != 1) {
                if (fopAcM_GetRoomNo(a_this) == 4) {
                    if (i < 2) {
                        cMtx_YrotS(*calc_mtx, 0x2000 * i + 0x1EFF);
                    } else {
                        cMtx_YrotS(*calc_mtx, 500 * i + 0x253B);
                        i_this->field_0xbfc[i] = cM_rndFX(6000.0f);
                    }

                    sp48.y = cM_rndF(5.0f) + 30.0f;
                    sp48.z = cM_rndFX(5.0f) + 20.0f;
                } else {
                    if (i < 2) {
                        cMtx_YrotS(*calc_mtx, 0x8000 * i + (u16)-0x2000);
                    } else if (i < 10) {
                        cMtx_YrotS(*calc_mtx, 0x2000 * i + 0xAAA);
                        i_this->field_0xbfc[i] = cM_rndFX(4000.0f);
                    } else {
                        cMtx_YrotS(*calc_mtx, 0x1000 * i + 0x1554);
                        i_this->field_0xbfc[i] = cM_rndFX(6000.0f);
                    }

                    sp48.y = cM_rndF(20.0f) + 20.0f;
                    sp48.z = cM_rndFX(6.0f) + 15.0f;
                }
            } else {
                if (i < 2) {
                    cMtx_YrotS(*calc_mtx, 0x8000 * i + 0x6000);
                } else {
                    cMtx_YrotS(*calc_mtx, cM_rndFX(32768.0f));
                    i_this->field_0xbfc[i] = cM_rndFX(1000.0f);
                }

                if (i == 1) {
                    sp48.z = 0.0f;
                    sp48.y = 0.0f;
                } else {
                    sp48.y = cM_rndF(5.0f) + 10.0f;
                    sp48.z = cM_rndFX(4.0f) + 15.0f;
                }
            }

            MtxPosition(&sp48, &i_this->field_0x8f0[i]);
            i_this->field_0xbc8[i] = cM_rndFX(32768.0f);
        }
    }
}

static void so_drop(obj_so_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp30, sp3c;
    s8 sVar2 = 0;
    s8 sVar1 = 1;

    switch (i_this->field_0xdb0) {
        case 0:
            a_this->speed.y = 30.0f;
            a_this->speedF = -10.0f;
            i_this->field_0xdb0 = 1;
            break;

        case 1:
            if (i_this->field_0xdb2[0] != 0) {
                sVar1 = 0;
            }
            
            cLib_addCalcAngleS2(&a_this->shape_angle.x, -0x4000, 4, 0x300);
            if (sVar1 && i_this->mObjAcch.ChkGroundHit()) {
                dComIfGs_onSwitch(fopAcM_GetParam(a_this) >> 16 & 0xFF, fopAcM_GetRoomNo(a_this));
                a_this->health = 0;
                i_this->field_0xdb0 = 2;
                sVar2 = 1;
                i_this->field_0x1054 = 1;
            }
            break;

        case 2:
            a_this->speed.y = 0.0f;
            a_this->speedF = 0.0f;
    }

    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    sp30.x = 0.0f;
    sp30.y = 0.0f;
    sp30.z = a_this->speedF;
    MtxPosition(&sp30, &sp3c);

    a_this->speed.x = sp3c.x;
    a_this->speed.z = sp3c.z;
    a_this->current.pos += a_this->speed;
    a_this->speed.y -= 5.0f;

    if (sVar1) {
        f32 fVar1 = 90.0f * cM_ssin(a_this->shape_angle.x);
        a_this->current.pos.y += fVar1;
        a_this->old.pos.y += fVar1;
        i_this->mObjAcch.CrrPos(dComIfG_Bgsp());
        a_this->current.pos.y -= fVar1;
        a_this->old.pos.y -= fVar1;
    }

    if (sVar2) {
        part_set(i_this, 1);
        mDoAud_seStart(Z2SE_OBJ_MONKEYJAIL_BREAK, 0, 0, 0);
        csXyz cStack_50(0, 0, 0);
        fopAcM_effSmokeSet1(&i_this->field_0x1b98, &i_this->field_0x1b9c, &a_this->current.pos, &cStack_50,
                            2.5f, &a_this->tevStr, 1);
        dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0x1b98);
        dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0x1b9c);
        dComIfGp_getVibration().StartShock(5, 1, cXyz(0.0f, 1.0f, 0.0f));
        i_this->field_0x1bc0 = 30.0f;
    }
}

static void so_break(obj_so_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp2c, sp38;
    switch (i_this->field_0xdb0) {
        case 0:
            i_this->field_0xdb0 = 1;
            part_set(i_this, 2);
            a_this->health = 0;
            // fallthrough
        case 1:
        default:
            a_this->eyePos = a_this->current.pos;
            a_this->eyePos.y += 90.0f;
            a_this->attention_info.position = a_this->eyePos;
            i_this->field_0xdc2 = -i_this->field_0xdc8 * cM_ssin(i_this->field_0xdac * 0xF00);
            cLib_addCalc0(&i_this->field_0xdc8, 0.1f, 33.0f);
    }
}

static void so_hang(obj_so_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* actor = fopAcM_SearchByID(i_this->field_0x1b94);

    switch (i_this->field_0xdb0) {
        case 0:
            if (actor != NULL) {
                i_this->field_0xdb2[0] = 20;
                i_this->field_0xdb0 = 1;
            }
            break;

        case 1:
            if (i_this->field_0xdb2[0] == 0 && !dComIfGp_event_runCheck()) {
                if (i_this->field_0x1b60 == 0) {
                    i_this->field_0xdb0 = 2;
                }

                i_this->field_0xdc2 = -i_this->field_0xdc8 * cM_ssin(i_this->field_0xdac * 0xF00);
                cLib_addCalc0(&i_this->field_0xdc8, 0.1f, 33.0f);
            }
            break;

        case 2:
            a_this->current.pos.y += a_this->speed.y;
            a_this->speed.y -= 5.0f;
            i_this->mObjAcch.CrrPos(dComIfG_Bgsp());
            if (i_this->mObjAcch.ChkGroundHit()) {
                dComIfGs_onSwitch(fopAcM_GetParam(a_this) >> 16 & 0xFF, fopAcM_GetRoomNo(a_this));
                a_this->health = 0;
                i_this->field_0x1054 = 1;
                i_this->field_0xdb0 = 3;
                part_set(i_this, 1);
                mDoAud_seStart(Z2SE_OBJ_MONKEYJAIL_BREAK, 0, 0, 0);
                csXyz cStack_38(0, 0, 0);
                fopAcM_effSmokeSet1(&i_this->field_0x1b98, &i_this->field_0x1b9c, &a_this->current.pos, &cStack_38,
                                    2.5f, &a_this->tevStr, 1);
                dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0x1b98);
                dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0x1b9c);
                dComIfGp_getVibration().StartShock(5, 1, cXyz(0.0f, 1.0f, 0.0f));
            }
            break;
        case 3:
            break;
    }
}

static void action(obj_so_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp38;

    if (i_this->field_0x1056 != 0) {
        i_this->field_0x1056--;
    }

    switch (i_this->field_0xdae) {
        case 0:
            so_hasira(i_this);
            break;

        case 1:
            so_drop(i_this);
            break;

        case 2:
           so_break(i_this);
           break;

        case 3:
            so_hang(i_this);
    }

    if (a_this->health != 0) {
        if (i_this->field_0x1b60 != 0) {
            MTXCopy(i_this->mMtx, *calc_mtx);
            MtxTrans(0.0f, -60.0f, 0.0f, 1);
            MtxTrans(0.0f, 200.0f, 0.0f, 1);
            cMtx_XrotM(*calc_mtx, i_this->field_0xdc2);
            mDoMtx_MtxToRot(*calc_mtx, &a_this->shape_angle);
            MtxTrans(0.0f, -200.0f, 0.0f, 1);
            i_this->field_0x574->setBaseTRMtx(*calc_mtx);
            sp38.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&sp38, &a_this->current.pos);
            i_this->field_0x1b60 = 0;
        } else {
            mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
            mDoMtx_stack_c::YrotM(i_this->field_0xdbe);
            mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
            mDoMtx_stack_c::XrotM(a_this->shape_angle.x + i_this->field_0xdc2);
            mDoMtx_stack_c::YrotM(-i_this->field_0xdbe);
            f32 scale = a_this->scale.x;
            mDoMtx_stack_c::scaleM(scale, scale, scale);
            i_this->field_0x574->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }
}

static void part_move(obj_so_class* i_this) {
    static u16 e_id[3] = {
        0x82AB,
        0x82AC,
        0x82AD,
    };

    fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->actor;
    cXyz spec, spf8, sp104;
    s16 tempCos;
    s16 iVar16 = 0;
    s16 iVar17 = 0;
    csXyz cStack_13c(0, 0, 0);
    dBgS_ObjGndChk cStack_e0;
    f32 fVar1;

    for (int i = 0; i < 26; i++) {
        if (i_this->field_0x5fc[i] != 0) {
            if (i_this->field_0x5fc[i] == 1) {
                if (i < 2) {
                    if (i_this->field_0xc30[i] != 0) {
                        cLib_addCalcAngleS2(&i_this->field_0xa28[i].x, NREG_S(7), 2, NREG_S(5) + 300);
                        tempCos = cM_scos(i_this->field_0xa28[i].x) * 12000.0f;
                        i_this->field_0xb94[i] += tempCos;
                    }

                    fVar1 = fabsf((BREG_F(0) + 70.0f) * cM_ssin(i_this->field_0xa28[i].x)) + 5.0f;
                } else if (i < 10) {
                    cLib_addCalcAngleS2(&i_this->field_0xa28[i].y, i_this->field_0xbc8[i], 1, 0x800);
                    if (i_this->field_0xc30[i] == 0) {
                        i_this->field_0xa28[i].x += i_this->field_0xbfc[i];
                    } else {
                        cLib_addCalcAngleS2(&i_this->field_0xa28[i].x, -0x4000, 1, 0xC00);
                    }

                    fVar1 = fabsf((BREG_F(2) + 110.0f) * cM_scos(i_this->field_0xa28[i].x)) + 5.0f;
                } else {
                    fVar1 = 5.0f;
                    cLib_addCalcAngleS2(&i_this->field_0xa28[i].y, i_this->field_0xbc8[i], 1, 0x800);
                    if (i_this->field_0xc30[i] == 0) {
                        i_this->field_0xa28[i].x += i_this->field_0xbfc[i];
                    } else {
                        cLib_addCalcAngleS2(&i_this->field_0xa28[i].x, 0, 1, 0xC00);
                    }

                    cLib_addCalcAngleS2(&i_this->field_0xb60[i], 0, 1, 0xC00);
                    fVar1 = fabsf((BREG_F(3) + 40.0f) * cM_ssin(i_this->field_0xa28[i].x)) + 5.0f;
                }

                i_this->field_0x618[i] += i_this->field_0x8f0[i];
                i_this->field_0x8f0[i].y -= 5.0f;
                if (i_this->field_0x8f0[i].y < -100.0f) {
                    i_this->field_0x8f0[i].y = -100.0f;
                }

                if (!dComIfGp_event_runCheck() && fopAcM_GetRoomNo(a_this) == 19 && i_this->field_0x618[i].y < 2570.0f) {
                    i_this->field_0x5fc[i] = 0;
                }

                if (fopAcM_GetRoomNo(a_this) != 2) {
                    if (strcmp(dComIfGp_getStartStageName(), "F_SP108") != 0) {
                        spf8 = i_this->field_0x618[i];
                        spf8.y += 110.0f;
                        cStack_e0.SetPos(&spf8);
                        a_this->home.pos.y = dComIfG_Bgsp().GroundCross(&cStack_e0);
                    }
                }

                if (i_this->field_0x618[i].y <= a_this->home.pos.y + fVar1) {
                    i_this->field_0x618[i].y = a_this->home.pos.y + fVar1;
                    if (i_this->field_0x8f0[i].y < 0.0f && i_this->field_0xc30[i] < 3) {
                        if (i_this->field_0xc30[i] == 0) {
                            fopAcM_effSmokeSet2(&i_this->field_0xc4c[i], &i_this->field_0xcb4[i], &i_this->field_0x618[i],
                                                &cStack_13c, TREG_F(19) + 1.0f, &a_this->tevStr);
                            dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0xc4c[i]);
                            dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0xcb4[i]);

                            if (i <= 1) {
                                if (i_this->field_0x570 != 1) {
                                    i_this->field_0xa28[i].x = a_this->shape_angle.x + (s16)cM_rndFX(4000.0f);
                                } else {
                                    if (i == 0) {
                                        i_this->field_0xa28[i].x = (s16)cM_rndFX(2000.0f) + -0x3700;
                                    } else {
                                        i_this->field_0xa28[i].x = (s16)cM_rndFX(2000.0f) + -0x1000;
                                    }
                                }
                            }
                        }

                        i_this->field_0x8f0[i].y *= -0.3f;
                        i_this->field_0xc30[i]++;
                        i_this->field_0x8f0[i].x *= 0.5f;
                        i_this->field_0x8f0[i].z *= 0.5f;

                        if (i >= 2) {
                            i_this->field_0xa28[i].x += (s16)cM_rndFX(i_this->field_0x8f0[i].y * 400.0f);
                            i_this->field_0xbc8[i] += (s16)cM_rndFX(i_this->field_0x8f0[i].y * 200.0f);
                        }
                    } else {
                        i_this->field_0x8f0[i].y = -10.0f;
                        i_this->field_0x8f0[i].x = i_this->field_0x8f0[i].z = 0.0f;
                        if (daPy_getPlayerActorClass()->checkFrontRollCrash()) {
                            spec.x = player->current.pos.x - i_this->field_0x618[i].x;
                            spec.y = -(player->current.pos.y - i_this->field_0x618[i].y);
                            spec.z = player->current.pos.z - i_this->field_0x618[i].z;
                            if (spec.y > 200.0f && JMAFastSqrt(spec.x * spec.x + spec.z * spec.z) < 300.0f) {
                                if (i >= 2) {
                                    i_this->field_0x8f0[i].x = cM_rndFX(10.0f);
                                    i_this->field_0x8f0[i].z = cM_rndFX(10.0f);
                                }

                                i_this->field_0x8f0[i].y = 5.0f;
                                i_this->field_0xc30[i] = 1;
                                i_this->field_0xa28[i].x = cM_rndFX(500.0f) + -1024.0f;
                            }
                        }
                    }
                }
            }

            if (i >= 2 && i_this->field_0x5fc[i] == 1) {
                for (int j = 0; j < 2; j++) {
                    spec.x = i_this->field_0x618[i].x - i_this->field_0x618[j].x;
                    spec.z = i_this->field_0x618[i].z - i_this->field_0x618[j].z;

                    if (JMAFastSqrt(spec.x * spec.x + spec.z * spec.z) < BREG_F(9) + 180.0f) {
                        cMtx_YrotS(*calc_mtx, cM_atan2s(spec.x, spec.z));
                        spec.x = 0.0f;
                        spec.y = 0.0f;
                        spec.z = BREG_F(9) + 180.0f;
                        MtxPosition(&spec, &spf8);
                        i_this->field_0x618[i].x = i_this->field_0x618[j].x + spf8.x;
                        i_this->field_0x618[i].z = i_this->field_0x618[j].z + spf8.z;
                    }
                }
            }

            mDoMtx_stack_c::transS(i_this->field_0x618[i].x, i_this->field_0x618[i].y + i_this->field_0x750[i], i_this->field_0x618[i].z);
            mDoMtx_stack_c::XrotM((s16)i_this->field_0xdc2);

            if (i == 0 || (i >= 10 && i <= 17)) {
                mDoMtx_stack_c::YrotM((s16)i_this->field_0xdb8);
                mDoMtx_stack_c::XrotM(i_this->field_0xdb6);
                mDoMtx_stack_c::YrotM(-i_this->field_0xdb8);
            }

            if (i == 0) {
                mDoMtx_stack_c::YrotM((s16)i_this->field_0xb94[i]);
                mDoMtx_stack_c::XrotM(i_this->field_0xdb6);
            } else if (i == 1) {
                mDoMtx_stack_c::YrotM(-i_this->field_0xb94[i]);
            }

            mDoMtx_stack_c::YrotM(i_this->field_0xa28[i].y);
            mDoMtx_stack_c::XrotM(i_this->field_0xa28[i].x);
            mDoMtx_stack_c::ZrotM(i_this->field_0xa28[i].z);

            if (i == 0) {
                mDoMtx_stack_c::YrotM(-i_this->field_0xb94[i]);
            } else if (i == 1) {
                mDoMtx_stack_c::YrotM(i_this->field_0xb94[i]);
            }

            if (i >= 10) {
                mDoMtx_stack_c::YrotM(i_this->field_0xb60[i]);
                mDoMtx_stack_c::XrotM(0x4000);
            }

            mDoMtx_stack_c::scaleM(i_this->field_0x7b8[i].x, i_this->field_0x7b8[i].y, i_this->field_0x7b8[i].z);

            if (i >= 2 && i < 10 && i_this->field_0x5fc[i] == 2) {
                sp104 = i_this->field_0x618[i];
                if (i_this->field_0x1a98[i - 2] < 2) {
                    sp104.y += XREG_F(5) + -80.0f;
                    i_this->field_0x10b8[i - 2].SetR(XREG_F(6) + 20.0f);
                    i_this->field_0x10b8[i - 2].SetH(XREG_F(7) + 165.0f);
                    if (i_this->field_0x1056 == 0) {
                        if (i_this->field_0x10b8[i - 2].ChkTgHit() != 0) {
                            i_this->mAtInfo.mpCollider = i_this->field_0x10b8[i - 2].GetTgHitObj();
                            def_se_set(&i_this->mSound, i_this->mAtInfo.mpCollider, 41, NULL);
                            at_power_check(&i_this->mAtInfo);
                            OS_REPORT("OBJ_SO ATHIT %d \n", i_this->mAtInfo.mAttackPower);

                            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_40) != 0) {
                                for (int i = 0; i < 8; i++) {
                                    i_this->field_0x1a98[i] = 2;
                                }

                                OS_REPORT("OBJ_SO BO HIT \n");
                            } else {
                                if (i_this->mAtInfo.mAttackPower >= 20) {
                                    i_this->field_0x1a98[i - 2] = 2;
                                } else {
                                    i_this->field_0x1056 = 2;
                                    i_this->field_0x1a98[i - 2] = 2;
                                }
                            }

                            i_this->field_0x1ac0[i - 2] = i_this->field_0x618[i];
                            if (i_this->field_0x1a98[i - 2] >= 2) {
                                i_this->field_0x1a98[i - 2] = 2;
                                i_this->field_0x1ac0[i - 2].y += AREG_F(9) + -90.0f;
                            } else {
                                i_this->field_0x1ac0[i - 2].y += VREG_F(9) + -87.0f;
                            }

                            if (i_this->field_0x1a98[i - 2] == 2) {
                                for (int j = 0; j < 8; j++) {
                                    i_this->field_0x1b40[j] = cM_rndF(2.0f) + 4.0f;
                                    i_this->field_0x1b50[j] = cM_rndF(2.0f) + 4.0f;
                                }

                                i_this->field_0xa28[i].x = cM_rndFX(2500.0f);
                                i_this->field_0xa28[i].z = cM_rndFX(2500.0f);
                                i_this->field_0x1b20[i - 2] = cM_rndFX(2500.0f);
                                i_this->field_0x1b30[i - 2] = cM_rndFX(2500.0f);
                                i_this->field_0x1b40[i - 2] = cM_rndF(5.0f) + 10.0f;
                                i_this->field_0x1b50[i - 2] = cM_rndF(5.0f) + 10.0f;
                            } else {
                                i_this->field_0xa28[i].x = cM_rndFX(200.0f);
                                i_this->field_0xa28[i].z = cM_rndFX(200.0f);
                                i_this->field_0x1b20[i - 2] = cM_rndFX(200.0f);
                                i_this->field_0x1b30[i - 2] = cM_rndFX(200.0f);
                            }

                            cXyz sp110(AREG_F(19) + 0.3f, AREG_F(19) + 0.3f, AREG_F(19) + 0.3f);
                            for (int j = 0; j < 3; j++) {
                                dComIfGp_particle_set(e_id[j], &i_this->field_0x618[i], &a_this->shape_angle, &sp110);
                            }
                        }
                    }
                } else {
                    sp104.y += 20000.0f;
                }

                if (i_this->field_0x1a98[i - 2] != 0) {
                    if (i_this->field_0x1a98[i - 2] == 2) {
                        spec = i_this->field_0x618[i] - player->current.pos;
                        spec.y = 0.0f;
                        f32 var_f30 = spec.abs();
                        if (var_f30 < 50.0f) {
                            cMtx_YrotS(*calc_mtx, cM_atan2s(spec.x, spec.z));
                            spec.x = 0.0f;
                            spec.y = 0.0f;
                            spec.z = (50.0f - var_f30) * (JREG_F(0) + 100.0f);
                            MtxPosition(&spec, &spf8);
                            cLib_addCalcAngleS2(&i_this->field_0xa28[i].x, spf8.z, 2, 0x400);
                            cLib_addCalcAngleS2(&i_this->field_0xa28[i].z, -spf8.x, 2, 0x400);
                        }

                        if (i_this->field_0x1b40[i - 2] != 0) {
                            i_this->field_0x1b40[i - 2]--;
                        }

                        iVar16 = (i_this->field_0x1b40[i - 2] * 700) * cM_ssin(i_this->field_0xdac * 0x1300);
                        iVar17 = (i_this->field_0x1b40[i - 2] * 700) * cM_ssin(i_this->field_0xdac * 0x1800);
                        mDoMtx_stack_c::YrotM(iVar17);
                        mDoMtx_stack_c::XrotM(iVar16);
                        mDoMtx_stack_c::scaleM(1.0f, AREG_F(6) + 0.45f, 1.0f);
                        mDoMtx_stack_c::transM(0.0f, AREG_F(8) + -20.0f, 0.0f);
                        i_this->field_0x750[i] = AREG_F(7) + 80.0f;
                    } else {
                        mDoMtx_stack_c::scaleM(1.0f, VREG_F(6) + 0.5f, 1.0f);
                        mDoMtx_stack_c::transM(0.0f, VREG_F(8) + -20.0f, 0.0f);
                        i_this->field_0x750[i] = VREG_F(7) + 77.0f;
                    }
                }

                i_this->field_0x10b8[i - 2].SetC(sp104);
                dComIfG_Ccsp()->Set(&i_this->field_0x10b8[i - 2]);
            }

            i_this->field_0x594[i]->setBaseTRMtx(mDoMtx_stack_c::get());

            if (i <= 1 && fopAcM_GetRoomNo(a_this) == 2 && i_this->mBgW[i] != NULL && (i_this->field_0xc30[i] != 0 || i == 1)) {
                mDoMtx_stack_c::transM(0.0f, KREG_F(6) + -30.0f, 0.0f);
                mDoMtx_stack_c::scaleM(1.0f, KREG_F(7) + 8.0f, 1.0f);
                MTXCopy(mDoMtx_stack_c::get(), i_this->mBgMtx[i]);
                i_this->mBgW[i]->Move();
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        if (i_this->field_0x1a98[i] != 0) {
            if (i_this->field_0x1a98[i] == 2) {
                spec = i_this->field_0x618[i + 2] - player->current.pos;
                spec.y = 0.0f;
                f32 var_f29 = spec.abs();
                if (var_f29 < 50.0f) {
                    cMtx_YrotS(*calc_mtx, cM_atan2s(spec.x, spec.z));
                    spec.x = 0.0f;
                    spec.y = 0.0f;
                    spec.z = (50.0f - var_f29) * (JREG_F(1) + 100.0f);
                    MtxPosition(&spec, &spf8);
                    cLib_addCalcAngleS2(&i_this->field_0x1b20[i], spf8.z, 2, 0x400);
                    cLib_addCalcAngleS2(&i_this->field_0x1b30[i], -spf8.x, 2, 0x400);
                }
            }

            mDoMtx_stack_c::transS(i_this->field_0x1ac0[i].x, i_this->field_0x1ac0[i].y, i_this->field_0x1ac0[i].z);
            
            if (i_this->field_0x1b50[i] != 0) {
                i_this->field_0x1b50[i]--;
            }

            iVar16 = (i_this->field_0x1b50[i] * 700) * cM_ssin(i_this->field_0xdac * 0x1500);
            iVar17 = (i_this->field_0x1b50[i] * 700) * cM_ssin(i_this->field_0xdac * 0x1200);
            mDoMtx_stack_c::XrotM(i_this->field_0x1b20[i] + iVar16);
            mDoMtx_stack_c::ZrotM(i_this->field_0x1b30[i] + iVar17 + 0x8000);
            mDoMtx_stack_c::scaleM(i_this->field_0x7b8[i + 2].x, i_this->field_0x7b8[i + 2].y, i_this->field_0x7b8[i + 2].z);

            if (i_this->field_0x1a98[i] == 2) {
                mDoMtx_stack_c::scaleM(1.0f, AREG_F(6) + 0.45f, 1.0f);
                mDoMtx_stack_c::transM(0.0f, AREG_F(10) + -30.0f, 0.0f);
            } else {
                mDoMtx_stack_c::scaleM(1.0f, VREG_F(6) + 0.5f, 1.0f);
                mDoMtx_stack_c::transM(0.0f, VREG_F(10) + -30.0f, 0.0f);
            }

            i_this->field_0x1aa0[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }

    if (i_this->field_0x1054 == 0) {
        s8 sVar2 = 0;
        s8 sVar1 = 0;
        u32 uVar1 = 0;
        u32 uVar2 = 0;
        for (int i = 0; i < 8; i++) {
            if (i_this->field_0x1a98[i] == 2) {
                sVar2 = 0;
                for (int j = 1; j < 8; j++) {
                    if (i_this->field_0x1a98[(i + j) & 7] == 2) {
                        sVar2++;
                        uVar1 = (i + j) & 7;
                    } else {
                        sVar2 = 0;
                    }
                }

                if (sVar2 >= 3 && sVar1 < sVar2) {
                    sVar1 = sVar2;
                    uVar2 = uVar1;
                }
            } else {
                sVar2 = 0;
            }
        }

        if (sVar1 != 0) {
            if (sVar1 == 3) {
                uVar2--;
            } else if (sVar1 == 4) {
                uVar2 -= 2;
            } else if (sVar1 == 5) {
                uVar2 -= 2;
            } else if (sVar1 >= 6) {
                uVar2 -= 3;
            }
            uVar2 &= 7;

            cXyz sp11c(i_this->field_0x618[uVar2 + 2] - a_this->current.pos);
            i_this->field_0xdbc = cM_atan2s(sp11c.x, sp11c.z);
            i_this->field_0xdba = sVar1 * (AREG_S(7) + 500);

            if (sVar1 == 7) {
                part_set(i_this, 1);
                i_this->mSound.startCreatureSound(Z2SE_OBJ_MONKEYJAIL_BREAK, 0, -1);
                i_this->field_0xdae = 1;
                i_this->field_0xdb0 = 2;
                i_this->field_0x1054 = 1;

                for (int i = 0; i < 8; i++) {
                    i_this->field_0x750[i + 2] = 0.0f;
                }

                i_this->field_0xdb8 = i_this->field_0xdbc = i_this->field_0xdb6 = i_this->field_0xdba = 0;

                stage_stag_info_class* info = dComIfGp_getStage()->getStagInfo();
                if (dStage_stagInfo_GetSTType(info) == 0) {
                    int sw = ((fopAcM_GetParam(a_this) & 0xff000000) >> 24);
                    dComIfGs_onSwitch(sw, fopAcM_GetRoomNo(a_this));
                } else {
                    int sw = (fopAcM_GetParam(a_this) & 0xff0000) >> 16;
                    dComIfGs_onSwitch(sw, fopAcM_GetRoomNo(a_this));
                }
            }
        }

        cLib_addCalcAngleS2(&i_this->field_0xdb8, i_this->field_0xdbc, 2, 0x800);
        cLib_addCalcAngleS2(&i_this->field_0xdb6, i_this->field_0xdba, 2, 0x800);
    }
}

static void demo_camera(obj_so_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp30, sp3c;

    switch (i_this->mDemoMode) {
        case 1:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->mDemoMode = 2;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0x1ba2 = 0;
            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
            sp30.x = -550.0f;
            sp30.y = 3300.0f;
            sp30.z = -600.0f;
            MtxPosition(&sp30, &i_this->mDemoCamEye);
            i_this->mDemoCamEye.x += a_this->current.pos.x;
            i_this->mDemoCamEye.z += a_this->current.pos.z;
            i_this->mDemoCamFovy = 40.0f;
            // fallthrough
        case 2:
            sp3c.set(12459.0f, 3152.0f, 4628.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp3c, 0xfffffaee, 0);
            i_this->mDemoCamCenter.x = a_this->current.pos.x;
            i_this->mDemoCamCenter.z = a_this->current.pos.z;
            sp3c.y = (a_this->current.pos.y + 150.0f) - 50.0f;
            if (sp3c.y >= 3602.0f) {
                i_this->mDemoCamCenter.y = 3602.0f;
            } else {
                i_this->mDemoCamCenter.y = sp3c.y;
            }

            cLib_addCalc2(&i_this->mDemoCamFovy, 65.0f, 0.05f, 0.5f);

            if (i_this->field_0x1ba2 == 120) {
                Z2GetAudioMgr()->setBattleBgmOff(false);
                csXyz i_angle(0, 0x7FFF, 0);
                cXyz sp48(-4264.0f, 302.0f, -2330.0f);
                cXyz sp54(16400.0f, 3529.0f, 8385.0f);
                sp54 += sp48;
                fpc_ProcID i_parentID = fopAcM_create(PROC_E_OC, 0xFFFF0103, &sp54, fopAcM_GetRoomNo(a_this), &i_angle, 0, -1);
                sp54.set(16200.0f, 3481.0f, 8418.0f);
                sp54 += sp48;
                fopAcM_createChild(PROC_E_OC, i_parentID, 0xFFFF0104, &sp54, fopAcM_GetRoomNo(a_this), &i_angle, NULL, -1, NULL);
            } else if (i_this->field_0x1ba2 == 125) {
                i_this->mDemoMode = 100;
            }
            break;

        case 100:
            i_this->mDemoMode = 0;
            break;

        case 0:
            break;
    }

    if (i_this->mDemoMode != 0) {
        s16 i_bank = i_this->field_0x1bc0 * cM_scos(i_this->field_0x1ba2 * 0x3500) * 10.0f;
        cXyz i_center(i_this->mDemoCamCenter);
        cXyz i_eye(i_this->mDemoCamEye);
        i_center.y += i_this->field_0x1bc0 * cM_ssin(i_this->field_0x1ba2 * 0x3200) * 2.0f;
        i_eye.y += i_this->field_0x1bc0 * cM_ssin(i_this->field_0x1ba2 * 0x3200) * 2.0f;
        camera->mCamera.Set(i_center, i_eye, i_bank, i_this->mDemoCamFovy);
        cLib_addCalc0(&i_this->field_0x1bc0, 1.0f, 2.0f);
        i_this->field_0x1ba2++;
    }
}

static u8 data_80CE3C90;

static daObj_So_HIO_c l_HIO;

static int daObj_So_Execute(obj_so_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    i_this->field_0xdac++;
    a_this->scale.x = l_HIO.mScale;

    for (int i = 0; i < 2; i++) {
        if (i_this->field_0xdb2[i] != 0) {
            i_this->field_0xdb2[i]--;
        }
    }

    action(i_this);
    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    part_move(i_this);
    demo_camera(i_this);
    return 1;
}

static int daObj_So_IsDelete(obj_so_class* i_this) {
    return 1;
}

static int daObj_So_Delete(obj_so_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    dComIfG_resDelete(&i_this->mPhase, "Obj_so");
    if (i_this->field_0x1bcc != 0) {
        data_80CE3C90 = 0;
        mDoHIO_DELETE_CHILD(data_80CE3C90);
    }

    for (int i = 0; i < 2; i++) {
        if (i_this->mBgW[i] != NULL) {
            dComIfG_Bgsp().Release(i_this->mBgW[i]);
        }
    }

    if (a_this->heap != NULL) {
        i_this->mSound.stopAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    obj_so_class* i_this = (obj_so_class*)a_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_so", 7);
    JUT_ASSERT(1784, modelData != NULL);
    i_this->field_0x574 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->field_0x574 == NULL) {
        return 0;
    }

    for (int i = 0; i < 2; i++) {
        i_this->mBgW[i] = new dBgW();
        if (i_this->mBgW[i] == NULL) {
            return 0;
        }

        if (i_this->mBgW[i]->Set((cBgD_t*)dComIfG_getObjectRes("Obj_so", 10), 1, &i_this->mBgMtx[i]) == 1) {
            return 0;
        }

        i_this->mBgW[i]->SetCrrFunc(dBgS_MoveBGProc_Typical);
        if (i == 0) {
            i_this->mBgW[i]->SetRideCallback(ride_call_back_0);
        } else {
            i_this->mBgW[i]->SetRideCallback(ride_call_back_1);
        }
    }

    for (int i = 0; i < 26; i++) {
        if (i == 0) {
            modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_so", 4);
        } else if (i == 2) {
            modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_so", 5);
        }

        i_this->field_0x594[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->field_0x594[i] == NULL) {
            return 0;
        }
    }

    for (int i = 0; i < 8; i++) {
        i_this->field_0x1aa0[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->field_0x1aa0[i] == NULL) {
            return 0;
        }
    }

    i_this->mSound.init(&a_this->current.pos, &a_this->current.pos, 3, 1);
    return 1;
}

static int daObj_So_Create(fopAc_ac_c* a_this) {
    static dCcD_SrcCyl cc_cyl_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xd8fafd3f, 0x1f}, 0x79}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            10.0f, // mRadius
            120.0f // mHeight
        } // mCyl
    };

    obj_so_class* i_this = (obj_so_class*)a_this;
    fopAcM_ct(a_this, obj_so_class);

    int phase = dComIfG_resLoad(&i_this->mPhase, "Obj_so");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_SO PARAM %x\n", fopAcM_GetParam(a_this));
        i_this->field_0x570 = fopAcM_GetParam(a_this);
        i_this->field_0x571 = fopAcM_GetParam(a_this) >> 8;
        int swBit = fopAcM_GetParam(a_this) >> 24;
        if (swBit != 0xFF) {
            if (dComIfGs_isSwitch(swBit, fopAcM_GetRoomNo(a_this))) {
                return cPhs_ERROR_e;
            }
        }

        if (fopAcM_GetRoomNo(a_this) == 4) {
            i_this->field_0x570 = 2;
        }
        OS_REPORT("OBJ_SO//////////////OBJ_SO SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x12560)) {
            OS_REPORT("//////////////OBJ_SO SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_SO SET 2 !!\n");
        for (int i = 0; i < 2; i++) {
            if (i_this->mBgW[i] != NULL) {
                if (dComIfG_Bgsp().Regist(i_this->mBgW[i], a_this)) {
                    return cPhs_ERROR_e;
                }
            }
        }

        if (data_80CE3C90 == 0) {
            i_this->field_0x1bcc = 1;
            data_80CE3C90 = 1;
            mDoHIO_CREATE_CHILD("猿オリＯＢＪ", &l_HIO); // Monkey jail OBJ
            l_HIO.field_0x4 = -1;
        }

        fopAcM_SetMtx(a_this, i_this->field_0x574->getBaseTRMtx());
        fopAcM_SetMin(a_this, -300.0f, -500.0f, -300.0f);
        fopAcM_SetMax(a_this, 300.0f, 500.0f, 300.0f);
        i_this->mObjAcch.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this,
                             1, &i_this->mAcchCir, fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(50.0f, 50.0f);

        a_this->health = 1;
        if (i_this->field_0x570 == 1) {
            i_this->field_0xdae = 2;
            i_this->mStts.Init(0xFF, 0, a_this);

            for (int i = 0; i < 8; i++) {
                i_this->field_0x10b8[i].Set(cc_cyl_src);
                i_this->field_0x10b8[i].SetStts(&i_this->mStts);
            }

            a_this->current.pos.y += 26.0f;
            if (strcmp(dComIfGp_getStartStageName(), "F_SP108") == 0) {
                a_this->home.pos.y = 1100.0f;
            }
        } else {
            if (i_this->field_0x570 == 2) {
                i_this->field_0xdae = 3;
                if (fopAcM_GetRoomNo(a_this) == 4) {
                    cXyz sp2c(-5374.0f, 4280.0f, -2342.0f);
                    i_this->field_0x1b94 = fopAcM_create(PROC_OBJ_GM, 0x600, &sp2c, fopAcM_GetRoomNo(a_this),
                                                         NULL, NULL, -1);
                }
            }

            if (fopAcM_GetRoomNo(a_this) == 2) {
                a_this->home.pos.y = 3152.0f;
            }
        }

        i_this->mAtInfo.mpSound = &i_this->mSound;
        daObj_So_Execute(i_this);
    }

    return phase;
}

static actor_method_class l_daObj_So_Method = {
    (process_method_func)daObj_So_Create,
    (process_method_func)daObj_So_Delete,
    (process_method_func)daObj_So_Execute,
    (process_method_func)daObj_So_IsDelete,
    (process_method_func)daObj_So_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_SO = {
  fpcLy_CURRENT_e,        // mLayerID
  2,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_SO,            // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(obj_so_class),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  55,                     // mPriority
  &l_daObj_So_Method,     // sub_method
  0x00044000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
