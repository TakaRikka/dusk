/**
 * @file d_a_obj_brg.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_brg.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_cc_uty.h"
#include "d/actor/d_a_player.h"
#include "d/d_path.h"
#include "Z2AudioLib/Z2Instances.h"
#include "f_op/f_op_camera_mng.h"

static void ride_call_back(dBgW* i_bgw, fopAc_ac_c* i_bgActor, fopAc_ac_c* i_rideActor) {
    obj_brg_class* a_this = (obj_brg_class*)i_bgActor;

    cXyz sp74 = a_this->mBr[0].field_0x0bc - i_rideActor->current.pos;
    int var_r27 = JMAFastSqrt(sp74.x * sp74.x + sp74.z * sp74.z) / (a_this->field_0xaedc * 51.0f) - -0.5f;

    if (var_r27 > a_this->field_0xb1ea - 1) {
        var_r27 = a_this->field_0xb1ea - 1;
    } else if (var_r27 < 0) {
        var_r27 = 0;
    }

    br_s* br_p = &a_this->mBr[var_r27];
    cXyz sp80;
    cXyz sp8C;
    cXyz sp98;

    f32 var_f30;
    if (a_this->mType & 5) {
        var_f30 = 0.85f;
    } else if (a_this->field_0xb1ed == 2) {
        var_f30 = KREG_F(7) + 0.1f;
    } else {
        var_f30 = 1.0f;
    }

    cMtx_YrotS(*calc_mtx, -br_p->field_0x0d4.y);

    sp80 = i_rideActor->current.pos - br_p->field_0x0bc;
    MtxPosition(&sp80, &sp8C);

    sp80 = i_rideActor->old.pos - br_p->field_0x0bc;
    MtxPosition(&sp80, &sp98);

    i_rideActor->speed.y = -5.0f;

    f32 var_f29;
    if (fopAcM_GetName(i_rideActor) == PROC_ALINK) {
        var_f29 = 100.0f;
        br_p->field_0x0e4 = -31.0f;
        a_this->field_0xb1ef = 5;
    } else if (fopAcM_GetName(i_rideActor) == PROC_NPC_KS) {
        var_f29 = 70.0f;
        br_p->field_0x0e4 = -20.0f;
        i_rideActor->speed.y = -20.0f;
    } else {
        var_f29 = 50.0f;
        br_p->field_0x0e4 = -10.0f;
    }

    var_f29 *= var_f30;

    br_p->field_0x0e4 *= var_f30;
    br_p->field_0x0e4 += var_f29 * ((a_this->field_0xaef0 * cM_ssin(a_this->field_0xaf1e)) * 0.03f);

    br_p->field_0x0f2 = -sp8C.x * var_f29;
    br_p->field_0x706 = 2;

    sp74 = sp8C - sp98;

    f32 var_f31 = var_f30 * (sp74.abs() * 0.3f);
    if (var_f31 > 20.0f) {
        var_f31 = 20.0f;
    }

    if (a_this->field_0xaee0 <= var_f31) {
        a_this->field_0xaee0 = var_f31;
    }

    var_f31 = var_f30 * fabsf(sp74.x);
    if (var_f31 > 50.0f) {
        var_f31 = 50.0f;
    }

    if (a_this->field_0xaee4 <= var_f31) {
        a_this->field_0xaee4 += 0.5f;
    }
}

static int daObj_Brg_Draw(obj_brg_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    g_env_light.settingTevStruct(0x10, &a_this->current.pos, &a_this->tevStr);

    if (i_this->field_0xb1f0) {
        return 1;
    }

    br_s* br_p = i_this->mBr;

    f32 var_f32, var_f31, var_f30, var_f29, var_f28;
    for (int i = 0; i < i_this->field_0xb1ea; i++, br_p++) {
        g_env_light.setLightTevColorType_MAJI(br_p->mpBridgeModel, &a_this->tevStr);
        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(br_p->mpBridgeModel);
        dComIfGd_setList();

        if (i_this->mType & 8) {
            for (int j = 0; j < 2; j++) {
                g_env_light.setLightTevColorType_MAJI(br_p->mpKnotModels[j], &a_this->tevStr);
                mDoExt_modelUpdateDL(br_p->mpKnotModels[j]);
            }
        }

        if (!(i_this->mType & 4) && (br_p->field_0x708 & 4)) {
            cXyz sp74;
            cXyz sp80;
            cXyz sp8C;
            cXyz sp98;

            cMtx_YrotS(*calc_mtx, i_this->field_0xaf1e);

            sp74.x = 0.0f;
            sp74.y = 0.0f;
            sp74.z = 1.0f;
            MtxPosition(&sp74, &sp80);

            sp8C = br_p->field_0x074[0] - br_p->field_0x074[1];
            sp98 = br_p->field_0x050[0] - br_p->field_0x050[1];

            cXyz *spF4, *spF8;
            if (!(i_this->mType & 1)) {
                if (i_this->mType & 8) {
                    var_f30 = 5.0f;
                } else {
                    var_f30 = 3.0f;
                }

                f32* spFC = br_p->field_0x014.getSize(0);
                f32* sp100 = br_p->field_0x014.getSize(2);

                if (!(br_p->field_0x708 & 1)) {
                    for (int j = 0; j < 5; j++) {
                        *sp100 = var_f30;
                        *spFC = var_f30;
                        spFC++;
                        sp100++;
                    }
                } else {
                    spF4 = br_p->field_0x014.getPos(0);
                    spF8 = br_p->field_0x014.getPos(2);
                    sp8C.x *= 0.25f;
                    sp8C.y *= 0.25f;
                    sp8C.z *= 0.25f;

                    var_f29 = br_p->field_0x0fe[0] * cM_ssin(i_this->field_0xaf1e * 5);
                    for (int j = 0; j < 5; j++) {
                        *spFC = var_f30;

                        if (j == 2) {
                            var_f31 = 1.0f;

                            if (br_p->field_0x704 <= 1) {
                                *spFC = 0.0f;
                            } else if (br_p->field_0x704 == 2) {
                                *spFC = 1.0f;
                            }

                            br_p->field_0x098[0] = *spF4;
                        } else if (j == 1 || j == 3) {
                            var_f31 = 0.7f;
                        } else {
                            var_f31 = 0.0f;
                        }

                        sp74.x = (sp8C.x * j) + var_f29 * (var_f31 * sp80.x);
                        sp74.y = (sp8C.y * j);
                        sp74.z = (sp8C.z * j) + var_f29 * (var_f31 * sp80.z);

                        *spF4 = br_p->field_0x074[1] + sp74;
                        *spF8 = br_p->field_0x074[0];
                        *sp100 = 0.0f;

                        spF4++;
                        spF8++;
                        spFC++;
                        sp100++;
                    }
                }

                spFC = br_p->field_0x014.getSize(1);
                sp100 = br_p->field_0x014.getSize(3);

                if (!(br_p->field_0x708 & 2)) {
                    for (int j = 0; j < 5; j++) {
                        *sp100 = var_f30;
                        *spFC = var_f30;
                        spFC++;
                        sp100++;
                    }
                } else {
                    spF4 = br_p->field_0x014.getPos(1);
                    spF8 = br_p->field_0x014.getPos(3);
                    sp98.x *= 0.25f;
                    sp98.y *= 0.25f;
                    sp98.z *= 0.25f;

                    var_f29 = br_p->field_0x0fe[1] * cM_ssin(i_this->field_0xaf1e * 5);
                    for (int j = 0; j < 5; j++) {
                        *spFC = var_f30;

                        if (j == 2) {
                            var_f31 = 1.0f;

                            if (br_p->field_0x705 <= 1) {
                                *spFC = 0.0f;
                            } else if (br_p->field_0x705 == 2) {
                                *spFC = 1.0f;
                            }

                            br_p->field_0x098[1] = *spF4;
                        } else if (j == 1 || j == 3) {
                            var_f31 = 0.7f;
                        } else {
                            var_f31 = 0.0f;
                        }

                        sp74.x = (sp98.x * j) + var_f29 * (var_f31 * sp80.x);
                        sp74.y = (sp98.y * j);
                        sp74.z = (sp98.z * j) + var_f29 * (var_f31 * sp80.z);

                        *spF4 = br_p->field_0x050[1] + sp74;
                        *spF8 = br_p->field_0x050[0];
                        *sp100 = 0.0f;

                        spF4++;
                        spF8++;
                        spFC++;
                        sp100++;
                    }
                }

                static GXColor l_color = {0x96, 0x96, 0x96, 0xFF};
                br_p->field_0x014.update(5, l_color, &a_this->tevStr);
                dComIfGd_set3DlineMat(&br_p->field_0x014);
            }
        }
    }

    if (!(i_this->mType & 4) && !(i_this->mType & 5)) {
        cXyz spA4;
        cXyz spB0;

        if (i_this->field_0xb1ed == 1) {
            spA4.x = WREG_F(9) + -150.0f;
            spA4.y = WREG_F(10) + 430.0f;
            spA4.z = WREG_F(11) + -300.0f + 50.0f;
        } else if (i_this->field_0xb1ed == 2) {
            spA4.x = (WREG_F(9) + -150.0f) - 500.0f;
            spA4.y = WREG_F(10) + 430.0f + 900.0f;
            spA4.z = (WREG_F(11) + -300.0f + 50.0f) - 700.0f;
        } else {
            spA4.x = WREG_F(9) + -120.0f;
            spA4.y = WREG_F(10) + 350.0f;
            spA4.z = WREG_F(11) + -80.0f;
        }

        cMtx_YrotS(*calc_mtx, a_this->home.angle.y);
        MtxPosition(&spA4, &spB0);

        cXyz* sp114 = i_this->field_0xae90.getPos(0);
        sp114->x = a_this->home.pos.x + spB0.x;
        sp114->y = a_this->home.pos.y + spB0.y;
        sp114->z = a_this->home.pos.z + spB0.z;

        if (i_this->field_0xb1ed == 1) {
            spA4.y = WREG_F(10) + 430.0f + 20.0f;
            spA4.z = -(WREG_F(11) + -300.0f - 50.0f);
        } else if (i_this->field_0xb1ed == 2) {
            spA4.y = WREG_F(10) + 430.0f + 20.0f + 900.0f;
            spA4.z = -(WREG_F(11) + -300.0f - 50.0f - 700.0f);
        } else {
            spA4.z = -(WREG_F(19) + -80.0f + 50.0f - 80.0f);
            spA4.y = WREG_F(18) + 350.0f + 20.0f;
        }

        sp114 = &i_this->field_0xae90.getPos(0)[i_this->field_0xaed8] + 1;
        MtxPosition(&spA4, &spB0);

        sp114->x = i_this->mEndPos.x + spB0.x;
        sp114->y = i_this->mEndPos.y + spB0.y;
        sp114->z = i_this->mEndPos.z + spB0.z;

        spA4.x *= -1.0f;

        if (i_this->field_0xb1ed == 1) {
            spA4.y = WREG_F(10) + 430.0f;
            spA4.z = WREG_F(11) + -300.0f + 50.0f;
        } else if (i_this->field_0xb1ed == 2) {
            spA4.y = WREG_F(10) + 430.0f + 900.0f;
            spA4.z = (WREG_F(11) + -300.0f + 50.0f) - 700.0f;
        } else {
            spA4.z = WREG_F(11) + -80.0f;
            spA4.y = WREG_F(10) + 350.0f;
        }

        MtxPosition(&spA4, &spB0);

        sp114 = i_this->field_0xae90.getPos(1);
        sp114->x = a_this->home.pos.x + spB0.x;
        sp114->y = a_this->home.pos.y + spB0.y;
        sp114->z = a_this->home.pos.z + spB0.z;

        if (i_this->field_0xb1ed == 1) {
            spA4.y = WREG_F(10) + 430.0f + 20.0f;
            spA4.z = -(WREG_F(11) + -300.0f - 50.0f);
        } else if (i_this->field_0xb1ed == 2) {
            spA4.y = WREG_F(10) + 430.0f + 20.0f + 900.0f;
            spA4.z = -(WREG_F(11) + -300.0f - 50.0f - 700.0f);
        } else {
            spA4.z = -(WREG_F(19) + -80.0f + 50.0f - 80.0f);
            spA4.y = WREG_F(18) + 350.0f + 20.0f;
        }

        sp114 = &i_this->field_0xae90.getPos(1)[i_this->field_0xaed8] + 1;
        MtxPosition(&spA4, &spB0);

        sp114->x = i_this->mEndPos.x + spB0.x;
        sp114->y = i_this->mEndPos.y + spB0.y;
        sp114->z = i_this->mEndPos.z + spB0.z;

        if (i_this->mType & 8) {
            var_f28 = 10.0f;
        } else if (i_this->field_0xb1ed == 1) {
            var_f28 = WREG_F(4) + 6.5f;
        } else if (i_this->field_0xb1ed == 2) {
            var_f28 = WREG_F(5) + 10.0f;
        } else {
            var_f28 = 5.0f;
        }

        static GXColor l_color = {0x96, 0x96, 0x96, 0xFF};
        i_this->field_0xae90.update((u16)(i_this->field_0xaed8 + VREG_S(2) + 2), var_f28, l_color, 0, &a_this->tevStr);
        dComIfGd_set3DlineMat(&i_this->field_0xae90);
    }

    return 1;
}

static s16 wy;

static void control1(obj_brg_class* i_this, br_s* i_part) {
    cXyz sp84;
    cXyz sp90;
    cXyz sp9C;
    
    i_part++;

    i_this->field_0xaf22 += i_this->field_0xaf26;
    i_this->field_0xaf24 += i_this->field_0xaf28;

    s16 var_r27;
    if (i_this->field_0xb1ea > 10) {
        var_r27 = 4000;
    } else {
        var_r27 = 8000;
    }

    cXyz spA8;
    cXyz spB4;

    sp84.x = i_this->field_0xaeec * cM_scos(i_this->field_0xaf22);
    sp84.y = 0.0f;
    sp84.z = 0.0f;

    cMtx_YrotS(*calc_mtx, i_this->home.angle.y);
    MtxPosition(&sp84, &spA8);

    sp84.x = 1.0f;
    MtxPosition(&sp84, &spB4);

    cXyz spC0;

    sp84.x = 0.0f;
    sp84.z = i_this->field_0xaf30 * 5.0f;

    cMtx_YrotS(*calc_mtx, wy);
    MtxPosition(&sp84, &spC0);

    sp84.x = 0.0f;
    sp84.z = i_this->field_0xaedc * 50.0f;

    for (int i = 1; i < i_this->field_0xb1ea; i++, i_part++) {
        f32 var_f27 = (i_part->field_0x0e8 * 0.5f) + (((i_part->field_0x0ec * i_part->field_0x0e0) * 0.5f) + i_part->field_0x0b0.y);
        f32 var_f29 = (i_this->field_0xaee8 * cM_ssin(i_this->field_0xaf22 + (i * var_r27))) * i_part->field_0x0e0;
        f32 var_f26 = (i_this->field_0xaef0 * cM_ssin(i_this->field_0xaf24 + (i * (var_r27 + 1000)))) * i_part->field_0x0e0;

        f32 var_f31 = spC0.x + ((spA8.x * i_part->field_0x0e0) + ((i_part->field_0x0b0.x - i_part[-1].field_0x0b0.x) + (var_f29 * spB4.x)));
        f32 var_f28 = var_f26 + (var_f27 - i_part[-1].field_0x0b0.y);
        f32 var_f30 = spC0.z + ((spA8.z * i_part->field_0x0e0) + ((i_part->field_0x0b0.z - i_part[-1].field_0x0b0.z) + (var_f29 * spB4.z)));

        s16 spC6, spC8;
        spC8 = (s16)cM_atan2s(var_f31, var_f30);
        spC6 = -cM_atan2s(var_f28, JMAFastSqrt(var_f31 * var_f31 + var_f30 * var_f30));

        cMtx_YrotS(*calc_mtx, spC8);
        cMtx_XrotM(*calc_mtx, spC6);
        MtxPosition(&sp84, &sp9C);

        i_part->field_0x0b0.x = i_part[-1].field_0x0b0.x + sp9C.x;
        i_part->field_0x0b0.y = i_part[-1].field_0x0b0.y + sp9C.y;
        i_part->field_0x0b0.z = i_part[-1].field_0x0b0.z + sp9C.z;
    }
}

static void control2(obj_brg_class* i_this, br_s* i_part) {
    cXyz sp68;
    cXyz sp74;

    i_part += i_this->field_0xb1ea - 2;

    sp68.x = 0.0f;
    sp68.y = 0.0f;
    sp68.z = i_this->field_0xaedc * 50.0f;

    f32 x_var, atan_lhs, y_var, var_f28;
    for (int i = 0; i < i_this->field_0xb1ea - 1; ++i, i_part--) {
        var_f28 = (i_part->field_0x0e8 * 0.5f) + (i_part->field_0x0ec * i_part->field_0x0e0 * 0.5f + i_part->field_0x0b0.y);
        atan_lhs = var_f28 - i_part[1].field_0x0b0.y;
        x_var = i_part->field_0x0b0.x - i_part[1].field_0x0b0.x;
        y_var = i_part->field_0x0b0.z - i_part[1].field_0x0b0.z;

        s16 spC6, spC8;
        spC8 = (s16)cM_atan2s(x_var, y_var);
        spC6 = -cM_atan2s(atan_lhs, JMAFastSqrt(x_var * x_var + y_var * y_var));

        i_part[1].field_0x0d4.y = spC8;
        i_part[1].field_0x0d4.x = spC6;

        cMtx_YrotS(*calc_mtx, spC8);
        cMtx_XrotM(*calc_mtx, spC6);
        MtxPosition(&sp68, &sp74);

        i_part->field_0x0b0.x = i_part[1].field_0x0b0.x + sp74.x;
        i_part->field_0x0b0.y = i_part[1].field_0x0b0.y + sp74.y;
        i_part->field_0x0b0.z = i_part[1].field_0x0b0.z + sp74.z;
    }
}

static void control3(obj_brg_class* i_this, br_s* i_part) {
    f32 var_f31 = i_part->field_0x0b0.x - i_part[1].field_0x0b0.x;
    f32 var_f29 = i_part->field_0x0b0.y - i_part[1].field_0x0b0.y;
    f32 var_f30 = i_part->field_0x0b0.z - i_part[1].field_0x0b0.z;

    i_part->field_0x0d4.y = cM_atan2s(var_f31, var_f30);
    i_part->field_0x0d4.x = -cM_atan2s(var_f29, JMAFastSqrt(var_f31 * var_f31 + var_f30 * var_f30));
}

static void cut_control1(obj_brg_class* i_this, br_s* i_part) {
    fopAc_ac_c* a_this = i_this;
    cXyz sp7C;
    cXyz sp88;

    ++i_part;

    cXyz sp94;

    cMtx_YrotS(*calc_mtx, i_this->home.angle.y);
    sp7C.x = 0.0f;
    sp7C.y = 0.0f;
    sp7C.z = 1.0f;
    MtxPosition(&sp7C, &sp94);

    cXyz spA0(0.0f, 0.0f, 0.0f);

    if (fopAcM_GetRoomNo(a_this) == 4 && !dComIfGp_event_runCheck()) {
        cMtx_YrotS(*calc_mtx, wy);
        sp7C.x = 0.0f;
        sp7C.y = 0.0f;
        sp7C.z = i_this->field_0xaf30 * ((cM_ssin((s16) i_this->field_0xaf1e) * 0.1f) + (TREG_F(7) + 0.05f));
        MtxPosition(&sp7C, &spA0);

        i_this->field_0xaef4 = (3000.0f + TREG_F(15)) * i_this->field_0xaf30;
    }

    sp7C.z = i_this->field_0xaedc * 50.0f;

    f32 var_f31, var_f30, var_f29, var_f28;
    for (int i = 1; i < i_this->field_0xaed0; i++, i_part++) {
        var_f29 = i_part->field_0x0b0.y + i_part->field_0x0ec;
        if (var_f29 < i_part->field_0x0dc + 30.0f) {
            var_f29 = i_part->field_0x0dc + 30.0f;
            i_part->field_0x707++;
        }

        var_f28 = var_f29 - i_part[-1].field_0x0b0.y;
        var_f31 = (sp94.x + (i_part->field_0x0b0.x - i_part[-1].field_0x0b0.x)) + (spA0.x * (f32)(i - 1));
        var_f30 = (sp94.z + (i_part->field_0x0b0.z - i_part[-1].field_0x0b0.z)) + (spA0.z * (f32)(i - 1));

        s16 var_r27, var_r26;
        var_r26 = (s16)cM_atan2s(var_f31, var_f30);
        var_r27 = -cM_atan2s(var_f28, JMAFastSqrt(var_f31 * var_f31 + var_f30 * var_f30));

        i_part[-1].field_0x0d4.y = var_r26 + 0x8000;
        i_part[-1].field_0x0d4.x = -var_r27;

        if (i == i_this->field_0xaed0 - 1) {
            i_part->field_0x0d4.y = var_r26 + 0x8000;
            i_part->field_0x0d4.x = -var_r27;
        }

        cMtx_YrotS(*calc_mtx, var_r26);
        cMtx_XrotM(*calc_mtx, var_r27);
        MtxPosition(&sp7C, &sp88);

        i_part->field_0x0b0.x = i_part[-1].field_0x0b0.x + sp88.x;
        i_part->field_0x0b0.y = i_part[-1].field_0x0b0.y + sp88.y;
        i_part->field_0x0b0.z = i_part[-1].field_0x0b0.z + sp88.z;
    }
}

static void cut_control2(obj_brg_class* i_this, br_s* i_part) {
    cXyz sp7C;
    cXyz sp88;

    i_part += i_this->field_0xb1ea - 2;

    cXyz sp94;

    cMtx_YrotS(*calc_mtx, i_this->home.angle.y);
    
    sp7C.x = 0.0f;
    sp7C.y = 0.0f;
    sp7C.z = -1.0f;
    MtxPosition(&sp7C, &sp94);

    cXyz spA0(0.0f, 0.0f, 0.0f);

    if (fopAcM_GetRoomNo(i_this) == 4 && !dComIfGp_event_runCheck()) {
        cMtx_YrotS(*calc_mtx, wy);
        sp7C.x = 0.0f;
        sp7C.y = 0.0f;
        sp7C.z = i_this->field_0xaf30 * ((cM_ssin(i_this->field_0xaf1e) * 0.1f) + (TREG_F(7) + 0.05f));
        MtxPosition(&sp7C, &spA0);
    }

    sp7C.z = i_this->field_0xaedc * 50.0f;

    for (int i = 0; i < (i_this->field_0xb1ea - 1) - i_this->field_0xaed0; i++, i_part--) {
        f32 var_f29 = i_part->field_0x0b0.y + i_part->field_0x0ec;
        if (var_f29 < i_part->field_0x0dc + 30.0f) {
            var_f29 = i_part->field_0x0dc + 30.0f;
            i_part->field_0x707++;
        }

        f32 var_f28 = var_f29 - i_part[1].field_0x0b0.y;
        f32 var_f31 = (sp94.x + (i_part->field_0x0b0.x - i_part[1].field_0x0b0.x)) + (spA0.x * (f32)i);
        f32 var_f30 = (sp94.z + (i_part->field_0x0b0.z - i_part[1].field_0x0b0.z)) + (spA0.z * (f32)i);

        s16 var_r27, var_r26;
        var_r26 = (s16)cM_atan2s(var_f31, var_f30);
        var_r27 = -cM_atan2s(var_f28, JMAFastSqrt(var_f31 * var_f31 + var_f30 * var_f30));

        i_part[1].field_0x0d4.y = var_r26;
        i_part[1].field_0x0d4.x = var_r27;

        if (i == (i_this->field_0xb1ea - 2) - i_this->field_0xaed0) {
            i_part->field_0x0d4.y = var_r26;
            i_part->field_0x0d4.x = var_r27 + 0x8000;
        }

        cMtx_YrotS(*calc_mtx, var_r26);
        cMtx_XrotM(*calc_mtx, var_r27);
        MtxPosition(&sp7C, &sp88);

        i_part->field_0x0b0.x = i_part[1].field_0x0b0.x + sp88.x;
        i_part->field_0x0b0.y = i_part[1].field_0x0b0.y + sp88.y;
        i_part->field_0x0b0.z = i_part[1].field_0x0b0.z + sp88.z;
    }
}

static void himo_cut_control1(obj_brg_class* i_this, cXyz* param_1, f32 param_2) {
    cXyz sp74;
    cXyz sp80;
    cXyz sp8C;

    sp74.x = 0.0f;
    sp74.y = 0.0f;
    sp74.z = i_this->field_0xaf30 * (3.0f + NREG_F(7));
    cMtx_YrotS(*calc_mtx, wy);
    MtxPosition(&sp74, &sp8C);

    sp74.x = 0.0f;
    sp74.y = 0.0f;
    sp74.z = param_2;

    int i, temp_r27 = (intptr_t) ++param_1;
    f32 var_f31, var_f30, var_f29, var_f28, var_f27;
    for (i = 1; i < 5; i++, param_1++) {
        var_f28 = i_this->field_0xaf30 * cM_ssin( (i_this->field_0xaf20 * 6000) + (NREG_S(3) - 10000) * (temp_r27 + i) )
                                       * (NREG_F(11) + 7.0f);
        var_f27 = i_this->field_0xaf30 * cM_ssin( (i_this->field_0xaf20 * 6000) + (NREG_S(4) - 12000) * (temp_r27 + i) )
                                       * (NREG_F(11) + 7.0f);

        var_f31 = var_f28 + (sp8C.x + (param_1->x - param_1[-1].x));
        var_f29 = (param_1->y - param_1[-1].y) - 10.0f;
        var_f30 = var_f27 + (sp8C.z + (param_1->z - param_1[-1].z));

        s16 sp92, sp94;
        sp94 = (s16)cM_atan2s(var_f31, var_f30);
        sp92 = -cM_atan2s(var_f29, JMAFastSqrt(var_f31 * var_f31 + var_f30 * var_f30));

        cMtx_YrotS(*calc_mtx, sp94);
        cMtx_XrotM(*calc_mtx, sp92);
        MtxPosition(&sp74, &sp80);

        param_1->x = param_1[-1].x + sp80.x;
        param_1->y = param_1[-1].y + sp80.y;
        param_1->z = param_1[-1].z + sp80.z;
    }
}

static f32 ita_z_p[] = {
    0.1f,
    0.3f,
    0.5f,
    0.75f,
    0.89999998f,
    1.0f,
    0.89999998f,
    0.75f,
    0.5f,
    0.3f,
    0.1f,
};

static void obj_brg_move(obj_brg_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    daPy_py_c* ply_p = (daPy_py_c*) player;

    cXyz spD0;
    cXyz spDC;
    cXyz spE8;

    br_s* part = i_this->mBr;
    s16 my_tgt;

    int i, j;
    f32 var_f31, var_f30, var_f29, var_f28, var_f27, var_f26, var_f25;
    switch (i_this->field_0xaf1c) {
    case 0:
        i_this->field_0xb1e9 = 0;
        i_this->field_0xaf1c = 2;
        /* fallthrough */
    case 2:
        for (i = 0; i < i_this->field_0xb1ea; i++) {
            if ((i_this->mType & 1) == 1) {
                i_this->mBr[i].field_0x0e0 = 1.0f;
            } else {
                var_f27 = ( f32(i) / (i_this->field_0xb1ea - 1)) * M_PI;
                i_this->mBr[i].field_0x0e0 = fabsf(sin(var_f27));
            }

            i_this->mBr[i].field_0x704 = i_this->mBr[i].field_0x705 = 3;
        }

        i_this->field_0xaf1c = 3;
        /* fallthrough */
    case 3: {
        i_this->field_0xaf1e += (s16) 3000;
        part->field_0x0b0 = a_this->home.pos;

        if ((i_this->mType & 1) == 1) {
            cMtx_YrotS(*calc_mtx, i_this->home.angle.y);
            spD0.x = (i_this->field_0xaeec * cM_scos((s16) i_this->field_0xaf22)) * -2.0f;
            spD0.y = spD0.z = 0.0f;
            MtxPosition(&spD0, &spDC);

            part->field_0x0b0 += spDC;
        }

        control1(i_this, part);
        (*(part + i_this->field_0xb1ea - 1)).field_0x0b0 = i_this->mEndPos;

        if ((i_this->mType & 1) == 1) {
            (*(part + i_this->field_0xb1ea - 1)).field_0x0b0 -= spDC;
        }

        control2(i_this, part);
        control3(i_this, part);

        spE8 = a_this->home.pos - part->field_0x0b0;
        a_this->current.pos = part->field_0x0b0;
        a_this->current.angle = part->field_0x0d4;

        for (i = 0; i < i_this->field_0xb1ea; i++, part++) {
            part->field_0x0bc = part->field_0x0b0;

            var_f30 = ((f32)(i_this->field_0xb1ea - i) / (f32)i_this->field_0xb1ea) * 0.75f;
            part->field_0x0bc.x += spE8.x * var_f30;
            part->field_0x0bc.y += spE8.y * var_f30;
            part->field_0x0bc.z += spE8.z * var_f30;

            if (part->field_0x706) {
                for (j = -5; j <= 5; j++) {
                    if (i + j >= 0 && i + j < i_this->field_0xb1ea) {
                        my_tgt = (f32)part->field_0x0f2 * ita_z_p[j + 5] * part[j].field_0x0e0;
                        cLib_addCalcAngleS2(&part[j].field_0x0f4, my_tgt, 4, 0x800);
                        cLib_addCalc2(&part[j].field_0x0e8, part->field_0x0e4 * ita_z_p[j + 5], 1.0f, 10.0f);
                    }
                }
            }

            if (part->field_0x708 & 4 && (part->field_0x708 & 3) != 3) {
                var_f31 = 0.0f;
                var_f29 = -80.0f;

                if ((part->field_0x708 & 3) == 1) {
                    var_f31 = 7000.0f;
                    var_f29 = -30.0f;
                } else if ((part->field_0x708 & 3) == 2) {
                    var_f31 = -7000.0f;
                    var_f29 = -30.0f;
                }

                for (j = -5; j <= 5; j++) {
                    if (i + j < 0 || i + j >= i_this->field_0xb1ea) {
                        continue;
                    }

                    my_tgt = var_f31 * ita_z_p[j + 5] * part[j].field_0x0e0;
                    cLib_addCalcAngleS2(&part[j].field_0x0f6, my_tgt, 4, 0x800);
                    cLib_addCalc2(&part[j].field_0x0e8, var_f29 * ita_z_p[j + 5], 1.0f, 15.0f);

                    if ((part->field_0x708 & 3) == 0 && (i_this->mType & 4) == 0 && j >= -2 && j <= 2 && part[j].field_0x706 != 0) {
                        if (dComIfGp_event_runCheck()) {
                            i_this->field_0xaed4 = 0;
                        } else {
                            i_this->field_0xaed4 += 2;
                        }

                        if (i_this->field_0xaed4 > 100) {
                            i_this->field_0xaf1c = 4;
                            i_this->field_0xaed0 = i + j;

                            if (i_this->field_0xb1ef != 0) {
                                dComIfGp_getVibration().StartShock(5, 31, cXyz(0.0f, 1.0f, 0.0f));
                            }
                            break;
                        }
                    }

                    if (part[j].field_0x0e4 < -200.0f) {
                        i_this->field_0xaf1c = 4;
                        i_this->field_0xaed0 = i + j;
                        break;
                    }
                }
            }

            if ((part->field_0x708 & 4) != 0 && (part->field_0x0fe[0] != 0 || part->field_0x0fe[1] != 0)) {
                var_f26 = (f32)(part->field_0x0fe[0] | part->field_0x0fe[1]) * 150.0f;
                var_f31 = var_f26 * cM_ssin(i_this->field_0xaf1e * 4);
                for (j = -5; j <= 5; j++) {
                    if (i + j >= 0 && i + j < i_this->field_0xb1ea) {
                        my_tgt = var_f31 * ita_z_p[j + 5] * part[j].field_0x0e0;
                        part[j].field_0x0f6 += my_tgt;
                    }
                }
            }

            if (part->field_0x706 != 0) {
                part->field_0x706--;
            }

            part->field_0x0f2 = 0;
            part->field_0x0d4.z = part->field_0x0f4 + part->field_0x0f6;

            s16 sp138 = 0;
            if (i_this->field_0xb1ec) {
                if (i == 7) {
                    sp138 = 0x800;
                } else if (i == 5) {
                    sp138 = -0x500;
                } else if (i == 9) {
                    sp138 = -0x300;
                }
            }

            cLib_addCalcAngleS2(&part->field_0x0f4, sp138, 4, 0x400);
            cLib_addCalcAngleS2(&part->field_0x0f6, 0, 4, 0x400);
            cLib_addCalc2(&part->field_0x0ec, -15.0f, 1.0f, 5.0f);
            cLib_addCalc0(&part->field_0x0e8, 1.0f, 5.0f);
        }

        if (i_this->field_0xaed4 != 0) {
            i_this->field_0xaed4--;
        }

        i_this->field_0xaef0 = i_this->field_0xaee0;
        i_this->field_0xaee8 = i_this->field_0xaee0;
        i_this->field_0xaeec = i_this->field_0xaee4;
        i_this->field_0xaf28 = 3000;
        i_this->field_0xaf26 = 1400;

        var_f28 = i_this->field_0xaf30 * 3.0f;
        cLib_addCalc2(&i_this->field_0xaee0, var_f28, 0.1f, 0.1f);
        cLib_addCalc2(&i_this->field_0xaee4, var_f28 * 0.3f, 0.1f, 0.05f);
        break;
    }
    case 4:
        for (i = 0; i < i_this->field_0xb1ea; i++, part++) {
            part->field_0x0ec = 0.0f;
        }

        i_this->field_0xaf1c = 5;
        i_this->field_0xaf2c = 50;
        i_this->field_0xaef4 = 4000.0f;

        mDoAud_seStart(JA_SE_OBJ_BREAK_WOOD_L, NULL, 0, 0);

        if (fopAcM_GetRoomNo(a_this) != 4) {
            int sw = (fopAcM_GetParam(a_this) & 0xFF000000) >> 0x18;
            if (sw != 0xFF) {
                dComIfGs_onSwitch(sw, fopAcM_GetRoomNo(a_this));
            }
        }
        /* fallthrough */
    case 5:
        part = i_this->mBr;

        if (i_this->field_0xaf2c != 0) {
            i_this->field_0xaf2c--;
        }

        i_this->field_0xaf1e += 4000;
        part->field_0x0b0 = a_this->home.pos;

        cut_control1(i_this, part);
        (*(part + i_this->field_0xb1ea - 1)).field_0x0b0 = i_this->mEndPos;
        cut_control2(i_this, part);

        if (i_this->field_0xb1ed) {
            var_f25 = i_this->field_0xaf30;
            u32 dbg_0x24 = var_f25 * 110.0f;
            dbg_0x24 -= 10;
            if (dbg_0x24 > 0x7F) {
                dbg_0x24 = 0x7F;
            }

            Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_BROKENBRIDGE, &a_this->home.pos, dbg_0x24, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_BROKENBRIDGE, &i_this->mEndPos, dbg_0x24, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }

        for (i = 0; i < i_this->field_0xb1ea; i++, part++) {
            part->field_0x0bc = part->field_0x0b0;
            cLib_addCalc2(&part->field_0x0ec, -50.0f, 1.0f, 5.0f);
            part->field_0x0dc = -10000.0f;

            if (i_this->field_0xb1f1 == 0 || (i < i_this->field_0xaed0 && i_this->field_0xb1f1 == 2)
                || (i >= i_this->field_0xaed0 && i_this->field_0xb1f1 == 1)) {
                cLib_addCalcAngleS2(&part->field_0x0d4.z, i_this->field_0xaef4 * cM_ssin(i_this->field_0xaf2e + (i * 10000)), 4, 0x800);
            }
        }

        cLib_addCalc0(&i_this->field_0xaef4, 0.05f, 80.0f);
        i_this->field_0xaf2e += (s16) 4500;

        if (i_this->field_0xb1ec) {
            spDC = i_this->mEndPos;
            spDC.y = 3200.0f;
            i_this->mCyl[0].SetC(spDC);

            spDC = a_this->home.pos;
            spDC.y = 3200.0f;
            i_this->mCyl[1].SetC(spDC);

            for (int idx = 0; idx < 2; idx++) {
                i_this->mCyl[idx].SetR(60.0f);
                dComIfG_Ccsp()->Set(&i_this->mCyl[idx]);

                if (i_this->field_0xb1f2[idx] != 0) {
                    i_this->field_0xb1f2[idx]--;
                }

                if (i_this->field_0xb1f2[idx] == 0 && i_this->mCyl[idx].ChkTgHit()) {
                    i_this->field_0xb1f2[idx] = 10;
                    i_this->field_0xb1f1 = idx + 1;

                    dCcU_AtInfo spC4;
                    spC4.mpCollider = i_this->mCyl[idx].GetTgHitObj();
                    at_power_check(&spC4);

                    if (spC4.mAttackPower >= 3) {
                        i_this->field_0xaef4 = 3000.0f;
                    } else {
                        i_this->field_0xaef4 = 1600.0f;
                    }
                }
            }
        }
    }
}

static int daObj_Brg_Execute(obj_brg_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* ply = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*) ply;

    cXyz spC4;
    cXyz spD0;
    cXyz spDC;
    cXyz spE8;

    camera_class* camera = (camera_class*) dComIfGp_getCamera(0);
    spC4 = a_this->current.pos - camera->lookat.eye;

    if (i_this->field_0xb1ef != 0) {
        i_this->field_0xb1ef--;
    }

    f32 sp1BC;
    if (i_this->field_0xb1ed == 2) {
        sp1BC = 5500.0f;
    } else if (i_this->field_0xb1ed == 1) {
        sp1BC = 4000.0f;
    } else {
        sp1BC = 2000.0f;
    }

    if (spC4.abs() > sp1BC) {
        spD0 = camera->lookat.center - camera->lookat.eye;

        s16 atan_res = cM_atan2s(spD0.x, spD0.z);
        cMtx_YrotS(*calc_mtx, -atan_res);
        MtxPosition(&spC4, &spD0);

        if (spD0.z < 0.0f) {
            i_this->field_0xb1f0 = 1;
            return 1;
        }
    }

    i_this->field_0xb1f0 = 0;
    cXyz spF4;
    cXyz sp100 = a_this->current.pos + ((i_this->mEndPos - a_this->current.pos) * 0.5f);

    f32 sp1C0;
    dKyw_get_AllWind_vec(&sp100, &spF4, &sp1C0);

    if (sp1C0 > 0.6f) {
        sp1C0 = 0.6f;
    }
    sp1C0 *= sp1C0;

    cLib_addCalc2(&i_this->field_0xaf30, sp1C0, 0.1f, 0.03f);
    wy = cM_atan2s(spF4.x, spF4.z);
    i_this->field_0xaf20++;

    obj_brg_move(i_this);

    i_this->field_0xaed8 = 0;

    br_s* part = i_this->mBr;
    cXyz sp10C;
    bool var_r25 = 0;
    if (player->getBokoFlamePos(&sp10C)) {
        var_r25 = 1;
    }

    f32 var_f31, var_f30, var_f29, var_f28, var_f27, var_f26;
    var_f26 = 0.0f;
    if (i_this->mType & 1) {
        var_f29 = 1000.0f;
    } else if (i_this->field_0xb1ed == 1) {
        var_f29 = 250.0f + WREG_F(12);
    } else if (i_this->field_0xb1ed == 2) {
        var_f29 = 600.0f + WREG_F(14);
    } else {
        var_f29 = 200.0f;
    }

    var_f31 = 2.0f + TREG_F(17);

    f32 dbg_0x5c;
    for (int i = 0; i < i_this->field_0xb1ea; i++, part++) {
        if (i_this->field_0xb1ed == 2) {
            dbg_0x5c = f32(i) / f32(i_this->field_0xb1ea - 1) * M_PI;
            var_f26 = (-300.0f + WREG_F(15)) * f32(sin(dbg_0x5c));
        } else if (i_this->field_0xb1ed == 1) {
            dbg_0x5c = f32(i) / f32(i_this->field_0xb1ea - 1) * M_PI;
            var_f26 = (-100.0f + WREG_F(13)) * f32(sin(dbg_0x5c));
        }

        MtxTrans(part->field_0x0bc.x, part->field_0x0bc.y, part->field_0x0bc.z, 0);
        cMtx_YrotM(*calc_mtx, part->field_0x0d4.y);
        cMtx_XrotM(*calc_mtx, part->field_0x0d4.x);
        cMtx_ZrotM(*calc_mtx, part->field_0x0d4.z);

        if (i_this->field_0xaed0 != 0) {
            f32 sp1CC;
            if (i > i_this->field_0xaed0) {
                sp1CC = 30.0f;
            } else {
                sp1CC = -30.0f;
            }

            MtxTrans(0.0f, 0.0f, sp1CC, 1);
        }

        spC4.x = part->field_0x0c8.x * 99.0f;
        spC4.y = 5.0f + WREG_F(2);
        spC4.z = 0.0f;
        MtxPosition(&spC4, &part->field_0x074[1]);

        spC4.x *= -1.0f;
        MtxPosition(&spC4, &part->field_0x050[1]);

        spC4.y = -30.0f + TREG_F(4);
        MtxPosition(&spC4, &part->field_0x050[2]);

        spC4.x *= -1.0f;
        MtxPosition(&spC4, &part->field_0x074[2]);

        if (part->field_0x708 & 4) {
            if (part->field_0x0f8 != 0) {
                if (part->field_0x0f8 > 0) {
                    part->field_0x0f8--;
                }

                spC4.x = i_this->field_0xaedc * 66.0f;
                spC4.y = 0.0f;
                spC4.z = 0.0f;
                MtxPosition(&spC4, &part->field_0x074[0]);

                spC4.x *= -1.0f;
                MtxPosition(&spC4, &part->field_0x050[0]);

                part->field_0x074[0].y += var_f29 + var_f26;
                part->field_0x050[0].y += var_f29 + var_f26;

                if (i_this->mType & 8) {
                    MtxPush();

                    MtxTrans(part->field_0x074[0].x, part->field_0x074[0].y, part->field_0x074[0].z, 0);
                    cMtx_YrotM(*calc_mtx, a_this->current.angle.y);
                    MtxScale(var_f31, var_f31, var_f31, 1);
                    part->mpKnotModels[0]->setBaseTRMtx(*calc_mtx);

                    MtxTrans(part->field_0x050[0].x, part->field_0x050[0].y, part->field_0x050[0].z, 0);
                    cMtx_YrotM(*calc_mtx, a_this->current.angle.y);
                    MtxScale(var_f31, var_f31, var_f31, 1);
                    part->mpKnotModels[1]->setBaseTRMtx(*calc_mtx);

                    MtxPull();
                }

                if (i >= 1 && i <= i_this->field_0xb1ea - 2) {
                    var_f30 = (part[-1].field_0x074[0].y - part[1].field_0x074[0].y) * 0.5f + part[1].field_0x074[0].y;
                    if (part->field_0x074[0].y > var_f30) {
                        part->field_0x074[0].y = var_f30;
                    }

                    var_f30 = (part[-1].field_0x050[0].y - part[1].field_0x050[0].y) * 0.5f + part[1].field_0x050[0].y;
                    if (part->field_0x050[0].y > var_f30) {
                        part->field_0x050[0].y = var_f30;
                    }
                }

                if ((i_this->mType & 2) && i == i_this->field_0xb1eb - 1) {
                    i_this->field_0xaf04 = part->field_0x074[0];
                    i_this->field_0xaf10 = part->field_0x050[0];
                }
            }

            if (!(i_this->mType & 9)) {
                dCcU_AtInfo spB8;

                if (i_this->field_0xaed0 != 0) {
                    part->field_0x708 &= (u8) 0x1C;
                }

                if (part->mCyl[0].ChkTgHit() && part->field_0x0fe[0] < 10) {
                    part->mCyl[0].OnTgNoConHit();
                    part->field_0x0fe[0] = 20;

                    spB8.mpCollider = part->mCyl[0].GetTgHitObj();
                    at_power_check(&spB8);

                    if (!spB8.mpCollider->ChkAtType(0x10)) {
                        spB8.mAttackPower = 4;
                    }

                    part->field_0x704 -= spB8.mAttackPower;
                    if (part->field_0x704 <= 0) {
                        part->field_0x708 &= (u8) 0x1E;

                        Z2GetAudioMgr()->seStart(Z2SE_HIT_ROPE_CUT, &part->field_0x074[0], 0,
                                                 dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)), 1.0f, 1.0f, -1.0f, -1.0f, 0);
                        if (fopAcM_GetRoomNo(a_this) == 4) {
                            i_this->field_0xaf1c = 4;
                            i_this->field_0xaed0 = 10;
                        }
                    }
                }

                if (part->mCyl[1].ChkTgHit() && part->field_0x0fe[1] < 10) {
                    part->mCyl[1].OnTgNoConHit();
                    part->field_0x0fe[1] = 20;

                    spB8.mpCollider = part->mCyl[1].GetTgHitObj();
                    at_power_check(&spB8);

                    if (!spB8.mpCollider->ChkAtType(0x10)) {
                        spB8.mAttackPower = 4;
                    }

                    part->field_0x705 -= spB8.mAttackPower;
                    if (part->field_0x705 <= 0) {
                        part->field_0x708 &= (u8) 0x1D;

                        Z2GetAudioMgr()->seStart(Z2SE_HIT_ROPE_CUT, &part->field_0x050[0], 0,
                                                 dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)), 1.0f, 1.0f, -1.0f, -1.0f, 0);
                    }
                }
            } else {
                cXyz sp118;
                sp118.x = sp118.y = sp118.z = 2.0f;

                if (part->mCyl[0].ChkTgHit() && part->field_0x0fe[0] < 10) {
                    part->mCyl[0].OnTgNoConHit();

                    if (i_this->field_0xb1ed == 2) {
                        part->field_0x0fe[0] = 7;
                    } else {
                        part->field_0x0fe[0] = 15;
                    }

                    Z2GetAudioMgr()->seStart(JA_SE_CM_MAGBALL_BOUND, &part->field_0x074[0], 0,
                                             dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)), 1.0f, 1.0f, -1.0f, -1.0f, 0);
                }

                if (part->mCyl[1].ChkTgHit() && part->field_0x0fe[1] < 10) {
                    part->mCyl[1].OnTgNoConHit();

                    if (i_this->field_0xb1ed == 2) {
                        part->field_0x0fe[1] = 7;
                    } else {
                        part->field_0x0fe[1] = 15;
                    }

                    Z2GetAudioMgr()->seStart(JA_SE_CM_MAGBALL_BOUND, &part->field_0x050[0], 0,
                                             dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)), 1.0f, 1.0f, -1.0f, -1.0f, 0);
                }
            }

            cXyz sp124(part->field_0x074[1]);
            if (!(part->field_0x708 & 1)) {
                sp124.y -= 10000.0f;
            }

            part->mCyl[0].SetC(sp124);

            if (part->field_0x0fa != 0) {
                part->field_0x0fa--;

                if (part->field_0xc != NULL) {
                    mDoMtx_stack_c::transS(part->field_0x074[1].x, part->field_0x074[1].y + 100.0f, part->field_0x074[1].z);
                    part->field_0xc->setGlobalRTMatrix(mDoMtx_stack_c::get());

                    if (part->field_0x0fa == 0) {
                        part->field_0xc->becomeInvalidEmitter();
                        part->field_0xc = NULL;
                    }
                }

                if (part->field_0x0fa == 0) {
                    part->field_0x708 &= (u8) 0x1E;
                }
            } else if (var_r25) {
                cXyz sp130;

                if (!(i_this->mType & 9)) {
                    sp124.y += 100.0f;

                    sp130 = sp124 - sp10C;
                    sp130.y *= 0.4f;

                    if (sp130.abs() < 50.0f) {
                        part->field_0x0fa = 30;
                    }
                }
            }

            sp124 = part->field_0x050[1];
            if (!(part->field_0x708 & 2)) {
                sp124.y -= 10000.0f;
            }

            part->mCyl[1].SetC(sp124);

            if (part->field_0x0fc != 0) {
                part->field_0x0fc--;

                if (part->field_0x10 != NULL) {
                    mDoMtx_stack_c::transS(part->field_0x050[1].x, part->field_0x050[1].y + 100.0f, part->field_0x050[1].z);
                    part->field_0x10->setGlobalRTMatrix(mDoMtx_stack_c::get());

                    if (part->field_0x0fc == 0) {
                        part->field_0x10->becomeInvalidEmitter();
                        part->field_0x10 = NULL;
                    }
                }

                if (part->field_0x0fc == 0) {
                    part->field_0x708 &= (u8) 0x1D;
                }
            } else if (var_r25) {
                cXyz sp13C;

                if (!(i_this->mType & 9)) {
                    sp124.y += 100.0f;

                    sp13C = sp124 - sp10C;
                    sp13C.y *= 0.4f;

                    if (sp13C.abs() < 50.0f) {
                        part->field_0x0fc = 30;
                    }
                }
            }

            dComIfG_Ccsp()->Set(&part->mCyl[0]);
            dComIfG_Ccsp()->Set(&part->mCyl[1]);
        }

        for (int j = 0; j < 2; j++) {
            if (part->field_0x0fe[j] != 0) {
                part->field_0x0fe[j]--;
            }
        }

        cMtx_YrotM(*calc_mtx, part->field_0x0f0);

        if (i >= i_this->field_0xb1eb) {
            part->field_0x0c8.x = part->field_0x0c8.y = part->field_0x0c8.z = 0.0f;
            part->mpBridgeModel->setBaseScale(part->field_0x0c8);
            part->field_0x708 = 0;
        }

        MtxTrans(0.0f, (-3.0f + oREG_F(9)) * part->field_0x0c8.y, 0.0f, 1);
        part->mpBridgeModel->setBaseTRMtx(*calc_mtx);

        if (!(i_this->mType & 4) && !(i_this->mType & 1) && (part->field_0x708 & 4)) {
            cXyz* sp1D4 = i_this->field_0xae90.getPos(0);
            cXyz* sp1D8 = i_this->field_0xae90.getPos(1);
            cXyz *sp1DC, *sp1E0;
            int sp1E4 = i_this->field_0xaed8 + 1;

            cXyz sp148;
            spC4.x = 0.0f;
            spC4.y = 0.0f;
            s16 dbg_0xa = (i_this->field_0xb1e9 * 1500);
            spC4.z = i_this->field_0xaf30;
            spC4.z = spC4.z * (cM_ssin(dbg_0xa + (i_this->field_0xaf20 * 1400)) * 2.0f + 5.0f) * 0.15f;

            cMtx_YrotS(*calc_mtx, wy);
            MtxPosition(&spC4, &sp148);

            f32 dbg_0x38;
            if (part->field_0x708 & 1) {
                sp1D4[sp1E4] = part->field_0x074[0];
            } else {
                cXyz sp154 = *(sp1D4 + sp1E4 - 1) - *(sp1D4 + sp1E4 + 1);
                dbg_0x38 = (f32)part->field_0x0fe[0] * cM_ssin(i_this->field_0xaf1e * 6);

                sp1D4[sp1E4].x = sp154.x * 0.5f + (*(sp1D4 + sp1E4 + 1)).x + sp148.x;
                sp1D4[sp1E4].y = sp154.y * 0.5f + (*(sp1D4 + sp1E4 + 1)).y + dbg_0x38 - 1.0f + AREG_F(0);
                sp1D4[sp1E4].z = sp154.z * 0.5f + (*(sp1D4 + sp1E4 + 1)).z + sp148.z;

                sp1DC = part->field_0x014.getPos(0);
                sp1E0 = part->field_0x014.getPos(2);

                if (part->field_0x708 & 0x10) {
                    var_f28 = 0.0f;
                    var_f27 = i_this->field_0xaedc * 25.0f;
                } else {
                    var_f27 = i_this->field_0xaedc * 16.0f;
                    var_f28 = var_f27;
                }

                *sp1DC = sp1D4[sp1E4];
                himo_cut_control1(i_this, sp1DC, var_f28);

                *sp1E0 = part->field_0x074[1];
                himo_cut_control1(i_this, sp1E0, var_f27);
            }

            if (part->field_0x708 & 2) {
                sp1D8[sp1E4] = part->field_0x050[0];
            } else {
                cXyz sp160 = *(sp1D8 + sp1E4 - 1) - *(sp1D8 + sp1E4 + 1);
                dbg_0x38 = (f32)part->field_0x0fe[1] * cM_ssin(i_this->field_0xaf1e * 6);

                sp1D8[sp1E4].x = sp160.x * 0.5f + (*(sp1D8 + sp1E4 + 1)).x + sp148.x;
                sp1D8[sp1E4].y = sp160.y * 0.5f + (*(sp1D8 + sp1E4 + 1)).y + dbg_0x38 - 1.0f + AREG_F(0);
                sp1D8[sp1E4].z = sp160.z * 0.5f + (*(sp1D8 + sp1E4 + 1)).z + sp148.z;

                sp1DC = part->field_0x014.getPos(1);
                sp1E0 = part->field_0x014.getPos(3);

                if (part->field_0x708 & 0x10) {
                    var_f28 = 0.0f;
                    var_f27 = i_this->field_0xaedc * 25.0f;
                } else {
                    var_f27 = i_this->field_0xaedc * 16.0f;
                    var_f28 = var_f27;
                }

                *sp1DC = *(sp1D8 + sp1E4);
                himo_cut_control1(i_this, sp1DC, var_f28);

                *sp1E0 = part->field_0x050[1];
                himo_cut_control1(i_this, sp1E0, var_f27);
            }

            ++i_this->field_0xaed8;
        }
    }

    i_this->mpBgW->CopyBackVtx();

    Vec* vtx_tbl = (Vec*)i_this->mpBgW->GetVtxTbl();

    int dbg_0x30;
    int temp;
    int sp1F8 = 0;
    for (int i = 0; i < i_this->mpBgW->GetVtxNum(); i++) {
        dbg_0x30 = i & 3;
        temp = i >> 2;

        if (temp < i_this->field_0xb1eb) {
            part = &i_this->mBr[temp];
            switch (dbg_0x30) {
            case 0:
                vtx_tbl[i] = part->field_0x074[2];
                break;
            case 1:
                vtx_tbl[i] = part->field_0x050[2];
                break;
            case 2:
                vtx_tbl[i] = part->field_0x074[1];
                break;
            case 3:
                vtx_tbl[i] = part->field_0x050[1];
                break;
            }

            if (temp == 0 || temp == i_this->field_0xb1eb - 1) {
                cMtx_YrotS(*calc_mtx, part->field_0x0d4.y);
                cMtx_XrotM(*calc_mtx, part->field_0x0d4.x);

                spC4.x = spC4.y = 0.0f;

                if (temp == 0) {
                    spC4.z = 50.0f;
                } else if (i_this->field_0xb1eb == i_this->field_0xb1ea) {
                    spC4.z = -50.0f;
                } else {
                    spC4.z = -40.0f;
                }

                MtxPosition(&spC4, &spD0);
                vtx_tbl[i].x += spD0.x;
                vtx_tbl[i].y += spD0.y;
                vtx_tbl[i].z += spD0.z;
            }
            sp1F8 = i;
        } else {
            vtx_tbl[i] = vtx_tbl[sp1F8];
        }

        if (i_this->field_0xaf1c >= 4) {
            vtx_tbl[i].y = 10000.0f;
        }
    }

    i_this->mpBgW->Move();
    return 1;
}

static int daObj_Brg_IsDelete(obj_brg_class* i_this) {
    br_s* part = i_this->mBr;

    for (int i = 0; i < i_this->field_0xb1ea; i++, part++) {
        mDoAud_seDeleteObject(&part->field_0x0b0);
    }

    return 1;
}

static int daObj_Brg_Delete(obj_brg_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "Obj_brg");

    if (i_this->mpBgW != NULL) {
        dComIfG_Bgsp().Release(i_this->mpBgW);
    }

    return 1;
}

static void CreateInit(fopAc_ac_c* i_this) {
    obj_brg_class* a_this = (obj_brg_class*)i_this;

    a_this->mCcStts.Init(0xFF, 0xFF, i_this);

    static dCcD_SrcCyl himo_cyl_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0xff, 0x0, 0x0, 0x1}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {
                {0.0f, 0.0f, 0.0f}, // mCenter
                5.0f, // mRadius
                1000.0f // mHeight
            } // mCyl
        } // mCylAttr
    };

    static dCcD_SrcCyl wire_cyl_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_METAL, 0x2, 0x0, 0x0, 0x3}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {
                {0.0f, 0.0f, 0.0f}, // mCenter
                5.0f, // mRadius
                1000.0f // mHeight
            } // mCyl
        } // mCylAttr
    };

    br_s* part = a_this->mBr;
    for (int i = 0; i < a_this->field_0xb1ea; i++, part++) {
        for (int j = 0; j < 2; j++) {
            if (a_this->mType & 8) {
                part->mCyl[j].Set(wire_cyl_src);
                part->mCyl[j].SetStts(&a_this->mCcStts);
                part->mCyl[j].SetH(400.0f);
            } else {
                part->mCyl[j].Set(himo_cyl_src);
                part->mCyl[j].SetStts(&a_this->mCcStts);

                if (!(a_this->mType & 1)) {
                    part->mCyl[j].SetH(200.0f);
                    part->mCyl[j].OffTgShield();
                }
            }
        }
    }

    if (a_this->field_0xb1ec != 0) {
        a_this->mCyl[0].Set(himo_cyl_src);
        a_this->mCyl[0].SetStts(&a_this->mCcStts);

        a_this->mCyl[1].Set(himo_cyl_src);
        a_this->mCyl[1].SetStts(&a_this->mCcStts);

        a_this->mCyl[0].SetH(500.0f);
        a_this->mCyl[1].SetH(500.0f);

        a_this->mCyl[0].OffCoSetBit();
        a_this->mCyl[1].OffCoSetBit();
    }
}

static int CallbackCreateHeap(fopAc_ac_c* i_this) {
    obj_brg_class* a_this = (obj_brg_class*)i_this;

    int sp30 = a_this->mType & 1;
    if (a_this->mType == 4) {
        sp30 = 1;
    } else if (a_this->mType == 8) {
        sp30 = 2;
    }

    static const int obj_brg_bmd[] = {5, 6, 7};
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_brg", obj_brg_bmd[sp30]);
    JUT_ASSERT(0, modelData != NULL);

    J3DModelData* modelData2;
    if (a_this->mType == 8) {
        modelData2 = (J3DModelData*)dComIfG_getObjectRes("Obj_brg", 8);
        JUT_ASSERT(0, modelData2 != NULL);
    }
    
    br_s* part = a_this->mBr;

    for (int i = 0; i < a_this->field_0xb1ea; i++, part++) {
        part->mpBridgeModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (part->mpBridgeModel == NULL) {
            return 0;
        }

        if (a_this->mType == 8) {
            for (int j = 0; j < 2; j++) {
                part->mpKnotModels[j] = mDoExt_J3DModel__create(modelData2, 0x80000, 0x11000084);
                if (part->mpKnotModels[j] == NULL) {
                    return 0;
                }
            }
        }

        if (!(a_this->mType & 4)) {
            part->field_0x708 = 7;

            if (a_this->field_0xb1ed == 1) {
                part->field_0x708 |= 0x10;

                if (i == 3 || i == 4 || i == 7 || i == 11 || i == 13 || i == 15 || i == 18 || i == 19) {
                    part->field_0x708 = 0x16;
                } else if (i == 1 || i == 2 || i == 5 || i == 6 || i == 9 || i == 12 || i == 16 || i == 17) {
                    part->field_0x708 = 0x15;
                }
            }

            if ((a_this->mType & 1) != 1) {
                part->field_0x0f8 = -1;

                BOOL sp40;
                if (a_this->mType & 8) {
                    sp40 = part->field_0x014.init(4, 5, (ResTIMG*)dComIfG_getObjectRes("Obj_brg", 16), 1);
                } else {
                    sp40 = part->field_0x014.init(4, 5, (ResTIMG*)dComIfG_getObjectRes("Obj_brg", 15), 1);
                }

                if (!sp40) {
                    return 0;
                }
            }

            if (i == 0) {
                BOOL sp44;
                if (a_this->mType & 8) {
                    sp44 = a_this->field_0xae90.init(2, a_this->field_0xb1ea + 2, (ResTIMG*)dComIfG_getObjectRes("Obj_brg", 16), 0);
                } else {
                    sp44 = a_this->field_0xae90.init(2, a_this->field_0xb1ea + 2, (ResTIMG*)dComIfG_getObjectRes("Obj_brg", 15), 0);
                }

                if (!sp44) {
                    return 0;
                }

                cXyz* sp48 = a_this->field_0xae90.getPos(0);
                for (int j = 0; j < a_this->field_0xb1ea + 2; j++, sp48++) {
                    *sp48 = i_this->current.pos;
                }

                sp48 = a_this->field_0xae90.getPos(1);
                for (int j = 0; j < a_this->field_0xb1ea + 2; j++, sp48++) {
                    *sp48 = i_this->current.pos;
                }
            }
        }

        if (!(a_this->mType & 1)) {
            part->field_0x0c8.y = 1.0f;
            part->field_0x0c8.x = cM_rndF(0.05f) + 1.0f;
        } else {
            part->field_0x0c8.y = 1.0f;
            part->field_0x0c8.x = 1.0f;
        }

        if (a_this->field_0xb1ed == 1) {
            part->field_0x0c8.x *= 1.5f;
            part->field_0x0c8.y *= 1.5f; 
        } else if (a_this->field_0xb1ed == 2) {
            part->field_0x0c8.x *= 2.5f;
            part->field_0x0c8.y *= 2.5f;
        }

        part->field_0x0c8.z = a_this->field_0xaedc;
        part->mpBridgeModel->setBaseScale(part->field_0x0c8);

        if (cM_rndF(1.0f) < 0.5f) {
            part->field_0x0f0 = 0x8000;
        }
    }

    OS_REPORT("BGSV SET 1! \n");

    a_this->mpBgW = new dBgWSv();
    if (a_this->mpBgW == NULL) {
        return 0;
    }

    if ((a_this->mType & 1) == 1) {
        if (a_this->mpBgW->Set((cBgD_t*)dComIfG_getObjectRes("Obj_brg", 12), 0)) {
            return 0;
        }
    } else {
        if (a_this->mpBgW->Set((cBgD_t*)dComIfG_getObjectRes("Obj_brg", 11), 0)) {
            return 0;
        }
    }

    a_this->mpBgW->SetRideCallback(ride_call_back);
    a_this->mpBgW->CopyBackVtx();

    Vec* vtx_tbl = (Vec*)a_this->mpBgW->GetVtxTbl();
    for (int i = 0; i < a_this->mpBgW->GetVtxNum(); i++) {
        vtx_tbl[i] = i_this->current.pos;
    }

    a_this->mpBgW->Move();
    OS_REPORT("BGSV SET 2! \n");
    return 1;
}

static int daObj_Brg_Create(fopAc_ac_c* i_this) {
    obj_brg_class* a_this = (obj_brg_class*)i_this;
    int phase_state;
    dPath* path;
    dPnt* pnt;
    int sp54;
    br_s* part;
    int loop, sp60, brno, iter;
    int fop_id = fopAcM_GetID(i_this);
    fopAcM_ct(i_this, obj_brg_class);

    phase_state = dComIfG_resLoad(&a_this->mPhase, "Obj_brg");
    if (phase_state == cPhs_COMPLEATE_e) {
        a_this->mType = fopAcM_GetParam(i_this);
        a_this->mType = 0;
        a_this->field_0xb1e9 = (fopAcM_GetParam(i_this) & 0xFF00) >> 8;

        OS_REPORT("OBJ_BRG SET %x\n", fopAcM_GetParam(i_this));

        u8 path_id = (fopAcM_GetParam(i_this) & 0xFF0000) >> 0x10;
        a_this->field_0xaedc = 1.5f;

        if (strcmp(dComIfGp_getStartStageName(), "F_SP104") == 0) {
            a_this->mType = 8;
            a_this->field_0xb1ed = 2;
            a_this->field_0xaedc *= (3.0f + NREG_F(12));
        }

        if (path_id == 0xFF) {
            if (strcmp(dComIfGp_getStartStageName(), "D_MN05") == 0) {
                if (fopAcM_GetRoomNo(i_this) == 2) {
                    a_this->mType = 4;

                    i_this->home.pos.set(NREG_F(0) + 10155.0f + 1586.0f,
                                         NREG_F(1) + 3740.0f + 2.0f,
                                         NREG_F(2) + 5015.0f + -1280.0f);
                    a_this->mEndPos.set(NREG_F(3) + 11070.0f + 1586.0f,
                                        NREG_F(4) + 3730.0f + 2.0f,
                                        NREG_F(5) + 4705.0f + -1280.0f);

                    a_this->field_0xb1ec = 1;
                } else if (fopAcM_GetRoomNo(i_this) == 4) {
                    i_this->home.pos.set(0.0f, 3285.0f, 3425.0f);
                    a_this->mEndPos.set(0.0f, 3270.0f, 1095.0f);

                    a_this->field_0xaedc *= (1.5f + YREG_F(5));
                    a_this->field_0xb1ed = 1;
                } else {
                    return cPhs_ERROR_e;
                }
            }
        } else {
            path = dPath_GetRoomPath(path_id, fopAcM_GetRoomNo(i_this));
            if (path != NULL) {
                pnt = &path->m_points[0];
                i_this->home.pos.x = pnt->m_position.x;
                i_this->home.pos.y = pnt->m_position.y;
                i_this->home.pos.z = pnt->m_position.z;

                a_this->mEndPos.x = (pnt+1)->m_position.x;
                a_this->mEndPos.y = (pnt+1)->m_position.y;
                a_this->mEndPos.z = (pnt+1)->m_position.z;
            } else {
                OS_REPORT("NON PATH !!!!!!!!! %d!\n", path_id);
                return cPhs_ERROR_e;
            }
        }

        cXyz sp40 = a_this->mEndPos - i_this->home.pos;
        i_this->home.angle.y = (s16) cM_atan2s(sp40.x, sp40.z);
        i_this->home.angle.x = -cM_atan2s(sp40.y, JMAFastSqrt(sp40.x * sp40.x + sp40.z * sp40.z));

        f32 var_f31 = TREG_F(14) + 3.0f;
        if (a_this->field_0xb1ed == 1) {
            var_f31 = TREG_F(14) + 2.0f;
        } else if (a_this->field_0xb1ed == 2) {
            var_f31 = TREG_F(15);
        } else if (a_this->field_0xb1ec != 0) {
            var_f31 = 0.0f;
        } else if (sp40.abs() > 1300.0f) {
            var_f31 += TREG_F(15) + 3.0f;
        } else if (fopAcM_GetRoomNo(i_this) == 1) {
            var_f31 = 0.0f;
        }

        a_this->field_0xb1ea = sp40.abs() / ((var_f31 + 47.0f) * a_this->field_0xaedc);

        if (a_this->field_0xb1ea >= 24) {
            OS_REPORT("TOO MANY BIRIDGE !!!! %d!\n", a_this->field_0xb1ea);
            return cPhs_ERROR_e;
        }

        OS_REPORT("BIRIDGE JOINT !!!! %d!\n", a_this->field_0xb1ea);

        if (!fopAcM_entrySolidHeap(i_this, CallbackCreateHeap, 0x375E0)) {
            return cPhs_ERROR_e;
        }

        CreateInit(i_this);

        if (a_this->mpBgW != NULL) {
            if (dComIfG_Bgsp().Regist(a_this->mpBgW, a_this)) {
                return cPhs_ERROR_e;
            }
        }

        if (a_this->mType & 2) {
            if (a_this->field_0xb1ea >= 16) {
                a_this->field_0xb1eb = 15;
            } else if (a_this->field_0xb1ea >= 12) {
                a_this->field_0xb1eb = 11;
            } else {
                a_this->field_0xb1eb = 7;
            }
        } else {
            a_this->field_0xb1eb = a_this->field_0xb1ea;
        }

        OS_REPORT("OBJ_BRG SET END \n");

        sp54 = (fopAcM_GetParam(i_this) & 0xFF000000) >> 0x18;
        s8 sp78 = 0;
        if (sp54 != 0xFF && dComIfGs_isSwitch(sp54, fopAcM_GetRoomNo(i_this))) {
            sp78 = 1;
        }

        part = a_this->mBr;
        for (loop = 0; loop < a_this->field_0xb1ea; loop++, part++) {
            part->field_0x074[0].y = i_this->home.pos.y + 350.0f;
            part->field_0x050[0].y = i_this->home.pos.y + 350.0f;
        }

        sp60 = 10;
        if (a_this->field_0xb1ed == 2) {
            sp60 = 2;

            static cXyz brg_init_pos[] = {
                cXyz(334.0f, 249.0f, -11868.0f),
                cXyz(334.0f, 180.0f, -12082.0f),
                cXyz(334.0f, 114.0f, -12297.0f),
                cXyz(334.0f, 50.0f, -12512.0f),
                cXyz(334.0f, -9.0f, -12729.0f),
                cXyz(334.0f, -63.0f, -12948.0f),
                cXyz(334.0f, -111.0f, -13168.0f),
                cXyz(334.0f, -150.0f, -13389.0f),
                cXyz(334.0f, -181.0f, -13612.0f),
                cXyz(334.0f, -203.0f, -13836.0f),
                cXyz(334.0f, -215.0f, -14061.0f),
                cXyz(334.0f, -217.0f, -14286.0f),
                cXyz(334.0f, -208.0f, -14511.0f),
                cXyz(334.0f, -190.0f, -14735.0f),
                cXyz(334.0f, -162.0f, -14958.0f),
                cXyz(334.0f, -126.0f, -15180.0f),
                cXyz(334.0f, -82.0f, -15401.0f),
                cXyz(334.0f, -31.0f, -15620.0f),
                cXyz(334.0f, 24.0f, -15838.0f),
                cXyz(334.0f, 85.0f, -16054.0f),
                cXyz(334.0f, 150.0f, -16270.0f),
                cXyz(334.0f, 216.0f, -16485.0f),
            };

            for (brno = 0; brno < a_this->field_0xb1ea; brno++) {
                a_this->mBr[brno].field_0x098[2] = brg_init_pos[brno];
            }
        }

        for (iter = 0; iter < sp60; iter++) {
            if (iter == 5 && sp78) {
                if (a_this->field_0xb1ec != 0) {
                    a_this->field_0xaf1c = 5;
                    a_this->field_0xaed0 = 7;
                } else if (a_this->field_0xb1ed == 1) {
                    a_this->field_0xaf1c = 5;
                    a_this->field_0xaed0 = 10;
                    a_this->mType = 4;
                } else {
                    a_this->field_0xaf1c = 5;
                    a_this->field_0xaed0 = a_this->field_0xb1ea / 2;
                }
            }

            daObj_Brg_Execute(a_this);
        }
    }

    return phase_state;
}

AUDIO_INSTANCES;

static actor_method_class l_daObj_Brg_Method = {
    (process_method_func)daObj_Brg_Create,
    (process_method_func)daObj_Brg_Delete,
    (process_method_func)daObj_Brg_Execute,
    (process_method_func)daObj_Brg_IsDelete,
    (process_method_func)daObj_Brg_Draw,
};

actor_process_profile_definition g_profile_OBJ_BRG = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_BRG,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(obj_brg_class),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  54,                     // mPriority
  &l_daObj_Brg_Method,    // sub_method
  0x00044000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
