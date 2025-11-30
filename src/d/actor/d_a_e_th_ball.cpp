/**
 * @file d_a_e_th_ball.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_th_ball.h"
#include "d/actor/d_a_e_th.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"

enum daE_TH_ACTION {
    ACTION_STOP,
    ACTION_SPIN,
    ACTION_SHOT,
    ACTION_RETURN,
    ACTION_END = 10,
};

static e_th_class* master;

static void chain_draw(e_th_ball_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    f32 size = 1.0f + XREG_F(0);

    J3DModel* model_p;
    s16 rot_z;
    int var_r28 = 0;
    for (int i = i_this->field_0xdd0; i < 49; i++) {
        MtxTrans(i_this->field_0x65c.m_pos[i].x, i_this->field_0x65c.m_pos[i].y, i_this->field_0x65c.m_pos[i].z, 0);
        cMtx_YrotM(*calc_mtx, i_this->field_0x65c.m_rot[i].y);
        cMtx_XrotM(*calc_mtx, i_this->field_0x65c.m_rot[i].x);

        rot_z = var_r28 * 3000;
        if (var_r28 & 1) {
            rot_z += (s16)0x4000;
        }
        cMtx_ZrotM(*calc_mtx, rot_z);
        MtxScale(size, size, size, 1);
        MtxTrans(0.0f, 0.0f, 12.0f + XREG_F(1), 1);

        model_p = i_this->field_0x65c.mp_model[i];
        model_p->setBaseTRMtx(*calc_mtx);
        g_env_light.setLightTevColorType_MAJI(model_p, &a_this->tevStr);
        mDoExt_modelUpdate(model_p);
        var_r28++;
    }

    if (master == NULL || !master->mNoDraw) {
        var_r28 = 0;
        for (int i = i_this->field_0xde8; i < 19; i++) {
            MtxTrans(i_this->field_0xdec.m_pos[i].x, i_this->field_0xdec.m_pos[i].y, i_this->field_0xdec.m_pos[i].z, 0);
            cMtx_YrotM(*calc_mtx, i_this->field_0xdec.m_rot[i].y);
            cMtx_XrotM(*calc_mtx, i_this->field_0xdec.m_rot[i].x);

            rot_z = var_r28 * 3000;
            if (var_r28 & 1) {
                rot_z += (s16)0x4000;
            }
            cMtx_ZrotM(*calc_mtx, rot_z);
            MtxScale(size, size, size, 1);
            MtxTrans(0.0f, 0.0f, 12.0f + XREG_F(1), 1);

            model_p = i_this->field_0xdec.mp_model[i];
            model_p->setBaseTRMtx(*calc_mtx);
            g_env_light.setLightTevColorType_MAJI(model_p, &a_this->tevStr);
            mDoExt_modelUpdate(model_p);
            var_r28++;
        }

        var_r28 = 0;
        for (int i = i_this->field_0x11d4; i < 19; i++) {
            MtxTrans(i_this->field_0x11d8.m_pos[i].x, i_this->field_0x11d8.m_pos[i].y, i_this->field_0x11d8.m_pos[i].z, 0);
            cMtx_YrotM(*calc_mtx, i_this->field_0x11d8.m_rot[i].y);
            cMtx_XrotM(*calc_mtx, i_this->field_0x11d8.m_rot[i].x);

            rot_z = var_r28 * 3000;
            if (var_r28 & 1) {
                rot_z += (s16)0x4000;
            }
            cMtx_ZrotM(*calc_mtx, rot_z);
            MtxScale(size, size, size, 1);
            MtxTrans(0.0f, 0.0f, 12.0f + XREG_F(1), 1);

            model_p = i_this->field_0x11d8.mp_model[i];
            model_p->setBaseTRMtx(*calc_mtx);
            g_env_light.setLightTevColorType_MAJI(model_p, &a_this->tevStr);
            mDoExt_modelUpdate(model_p);
            var_r28++;
        }
    }
}

static int daE_TH_BALL_Draw(e_th_ball_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    if (i_this->mPlayerGet) {
        return 1;
    }

    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpBallModel, &a_this->tevStr);
    mDoExt_modelUpdateDL(i_this->mpBallModel);
    dComIfGd_setSimpleShadow(&a_this->current.pos, i_this->mAcch.GetGroundH(), 55.0f + AREG_F(11), i_this->mAcch.m_gnd, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    chain_draw(i_this);
    return 1;
}

static void chain_control_01(e_th_ball_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz mae;
    cXyz ato;
    dBgS_GndChk gndchk;

    th_chain_s* chain_s = &i_this->field_0x65c;
    if (master != NULL && fopAcM_GetName(master) == PROC_E_TH && master->mNoDraw == 0) {
        chain_s->m_pos[0] = master->mHandR_Pos1;
    }

    cXyz* pos_p = &chain_s->m_pos[1];
    csXyz* rot_p = &chain_s->m_rot[1];
    f32* var_r28 = &chain_s->field_0x6a4[1];
    f32 temp_f27 = i_this->field_0xde4 - (20.0f - (20.0f * i_this->field_0xdd4));

    Vec sp3C;
    cXyz sp30;
    mae = a_this->current.pos - chain_s->m_pos[0];
    cMtx_YrotS(*calc_mtx, cM_atan2s(mae.x, mae.z));
    cMtx_XrotM(*calc_mtx, -cM_atan2s(mae.y, JMAFastSqrt((mae.x * mae.x) + (mae.z * mae.z))));
    mae.x = 0.0f;
    mae.y = 0.0f;
    mae.z = 1000.0f * i_this->field_0xdd4;
    MtxPosition(&mae, &sp30);
    mae.z = 0.0f;

    for (int i = 1; i < 50; i++, pos_p++, rot_p++, var_r28++) {
        f32 temp_f31 = sp30.x + (pos_p[0].x - pos_p[-1].x);
        f32 temp_f28;
        f32 temp_f30 = sp30.z + (pos_p[0].z - pos_p[-1].z);

        f32 var_f29 = sp30.y + (pos_p[0].y + temp_f27);
        if (var_f29 <= *var_r28) {
            var_f29 = *var_r28;
        }
        temp_f28 = var_f29 - pos_p[-1].y;

        s16 rot_x, rot_y;
        rot_y = (s16)cM_atan2s(temp_f31, temp_f30);
        rot_x = -cM_atan2s(temp_f28, JMAFastSqrt((temp_f31 * temp_f31) + (temp_f30 * temp_f30)));
        cMtx_YrotS(*calc_mtx, rot_y);
        cMtx_XrotM(*calc_mtx, rot_x);
        if (i_this->field_0xdd0 == i - 1) {
            mae.z = 25.0f + XREG_F(2);
        }
        MtxPosition(&mae, &ato);
        pos_p[0].x = pos_p[-1].x + ato.x;
        pos_p[0].y = pos_p[-1].y + ato.y;
        pos_p[0].z = pos_p[-1].z + ato.z;

        if (((i_this->mCounter + i) & 15) == 0) {
            sp3C = *pos_p;
            sp3C.y += 200.0f;
            gndchk.SetPos(&sp3C);

            *var_r28 = 7.0f + dComIfG_Bgsp().GroundCross(&gndchk);
            if (*var_r28 - pos_p->y > 200.0f) {
                *var_r28 = pos_p->y;
            }
        }
    }

    for (int i = 0; i < 50; i++) {
        chain_s->field_0x320[i] = chain_s->m_pos[i];
    }
}

static void chain_control_02(e_th_ball_class* i_this) {
    cXyz sp6C;
    cXyz sp60;

    int i;
    s16 spC, spA;
    th_chain_s* chain_s = &i_this->field_0x65c;
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    chain_s->m_pos[49] = i_this->field_0xdd8;
    cXyz* pos_p = &chain_s->m_pos[48];
    csXyz* rot_p = &chain_s->m_rot[48];

    cXyz sp54;
    sp6C.x = 0.0f;
    sp6C.y = 0.0f;
    sp6C.z = 25.0f + XREG_F(2);

    cXyz sp48(0.0f, 0.0f, 0.0f);
    cXyz sp3C;
    
    s8 sp8 = 0;
    if (master != NULL && master->mNoDraw != 0) {
        sp8 = 1;
    }

    for (i = 48; i >= 0; i--, pos_p--, rot_p--) {
        if (sp8) {
            cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
            sp3C.x = (90.0f + VREG_F(7)) * cM_ssin(i * (VREG_S(7) - 3600));
            sp3C.y = 0.0f;
            sp3C.z = -100.0f + VREG_F(8);
            MtxPosition(&sp3C, &sp48);
        }
    
        f32 temp_f30 = sp48.x + (pos_p[0].x - pos_p[1].x);
        f32 temp_f28 = pos_p[0].y - pos_p[1].y;
        f32 temp_f29 = sp48.z + (pos_p[0].z - pos_p[1].z);
        
        spA = (s16)cM_atan2s(temp_f30, temp_f29);
        spC = -cM_atan2s(temp_f28, JMAFastSqrt((temp_f30 * temp_f30) + (temp_f29 * temp_f29)));
        cMtx_YrotS(*calc_mtx, spA);
        cMtx_XrotM(*calc_mtx, spC);
        if (i_this->field_0xdd0 == i + 1) {
            sp6C.z = 0.0f;
        }
        MtxPosition(&sp6C, &sp60);

        sp54 = *pos_p;
        pos_p[0].x = pos_p[1].x + sp60.x;
        pos_p[0].y = pos_p[1].y + sp60.y;
        pos_p[0].z = pos_p[1].z + sp60.z;
        rot_p->x = -spC;
    
        sp54 = (*pos_p - sp54) * (1.0f + TREG_F(6));
        sp54.y = 0.0f;

        f32 var_f31 = 1024.0f * sp54.abs();
        if (var_f31 > 16384.0f || i_this->mAction != ACTION_STOP) {
            rot_p->y = (spA + 0x8000);
        } else {
            if (var_f31 < 2048.0f) {
                var_f31 = 2048.0f;
            }
            cLib_addCalcAngleS2(&rot_p->y, spA + 0x8000, 2, var_f31);
        }
    }
}

static void chain_control_03(e_th_ball_class* i_this) {
    th_chain_s* chain_s = &i_this->field_0x65c;
    cXyz sp2C;
    cXyz sp20;
    sp2C = chain_s->field_0x320[i_this->field_0xdd0] - chain_s->m_pos[i_this->field_0xdd0];

    i_this->field_0xdcc = sp2C.abs();
    cXyz* pos_p = &chain_s->m_pos[i_this->field_0xdd0];
    
    int temp_r26 = 50 - i_this->field_0xdd0;
    for (int i = 0; i < temp_r26; i++, pos_p++) {
        *pos_p += sp2C * ((f32)(temp_r26 - i) / (f32)temp_r26);
    }
}

static void chain_control_11(e_th_ball_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp48;
    cXyz sp3C;
    Vec sp30;
    dBgS_GndChk gndchk;

    th_chain_2s* chain_s = &i_this->field_0xdec;
    chain_s->m_pos[0] = master->mHandR_Pos2;

    cXyz* pos_p = &chain_s->m_pos[1];
    csXyz* rot_p = &chain_s->m_rot[1];
    f32* var_r28 = &chain_s->field_0x398[1];
    f32 temp_f27 = -20.0f;

    cXyz sp24;
    cMtx_YrotS(*calc_mtx, master->shape_angle.y);
    sp48.x = 0.0f;
    sp48.y = 0.0f;
    sp48.z = 10.0f + TREG_F(10);
    MtxPosition(&sp48, &sp24);

    sp48.x = 0.0f;
    sp48.y = 0.0f;
    sp48.z = 0.0f;
    for (int i = 1; i < 20; i++, pos_p++, rot_p++, var_r28++) {
        f32 temp_f31 = sp24.x + (pos_p[0].x - pos_p[-1].x);
        f32 temp_f28;
        f32 temp_f30 = sp24.z + (pos_p[0].z - pos_p[-1].z);
        f32 var_f29 = pos_p[0].y + temp_f27;
        if (var_f29 <= *var_r28) {
            var_f29 = *var_r28;
        }
        temp_f28 = var_f29 - pos_p[-1].y;

        s16 spA, sp8;
        sp8 = (s16)cM_atan2s(temp_f31, temp_f30);
        spA = -cM_atan2s(temp_f28, JMAFastSqrt((temp_f31 * temp_f31) + (temp_f30 * temp_f30)));
        cMtx_YrotS(*calc_mtx, sp8);
        cMtx_XrotM(*calc_mtx, spA);

        if (i_this->field_0xde8 == i - 1) {
            sp48.z = 25.0f + XREG_F(2);
        }
        MtxPosition(&sp48, &sp3C);
        pos_p[0].x = pos_p[-1].x + sp3C.x;
        pos_p[0].y = pos_p[-1].y + sp3C.y;
        pos_p[0].z = pos_p[-1].z + sp3C.z;

        if (((i_this->mCounter + i) & 15) == 0) {
            sp30 = *pos_p;
            sp30.y += 200.0f;
            gndchk.SetPos(&sp30);

            *var_r28 = 7.0f + dComIfG_Bgsp().GroundCross(&gndchk);
            if (*var_r28 - pos_p->y > 200.0f) {
                *var_r28 = pos_p->y;
            }
        }
    }

    for (int i = 0; i < 20; i++) {
        chain_s->field_0x140[i] = chain_s->m_pos[i];
    }
}

static void chain_control_12(e_th_ball_class* i_this) {
    cXyz sp20;
    cXyz sp14;

    int i;
    s16 spA, sp8;
    th_chain_2s* chain_s = &i_this->field_0xdec;
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    chain_s->m_pos[19] = master->mHandL_Pos1;
    cXyz* pos_p = &chain_s->m_pos[18];
    csXyz* rot_p = &chain_s->m_rot[18];

    sp20.x = 0.0f;
    sp20.y = 0.0f;
    sp20.z = 25.0f + XREG_F(2);

    for (i = 18; i >= 0; i--, pos_p--, rot_p--) {
        f32 temp_f31 = pos_p[0].x - pos_p[1].x;
        f32 temp_f29 = pos_p[0].y - pos_p[1].y;
        f32 temp_f30 = pos_p[0].z - pos_p[1].z;

        sp8 = (s16)cM_atan2s(temp_f31, temp_f30);
        spA = -cM_atan2s(temp_f29, JMAFastSqrt((temp_f31 * temp_f31) + (temp_f30 * temp_f30)));
        cMtx_YrotS(*calc_mtx, sp8);
        cMtx_XrotM(*calc_mtx, spA);
        if (i_this->field_0xde8 == i + 1) {
            sp20.z = 0.0f;
        }
        MtxPosition(&sp20, &sp14);
        pos_p[0].x = pos_p[1].x + sp14.x;
        pos_p[0].y = pos_p[1].y + sp14.y;
        pos_p[0].z = pos_p[1].z + sp14.z;

        rot_p->x = -spA;
        rot_p->y = sp8 + 0x8000;
    }
}

static void chain_control_13(e_th_ball_class* i_this) {
    th_chain_2s* chain_s = &i_this->field_0xdec;
    cXyz sp2C;
    cXyz sp20;
    sp2C = chain_s->field_0x140[i_this->field_0xde8] - chain_s->m_pos[i_this->field_0xde8];

    cXyz* pos_p = &chain_s->m_pos[i_this->field_0xde8];
    
    int temp_r26 = 20 - i_this->field_0xde8;
    for (int i = 0; i < temp_r26; i++, pos_p++) {
        *pos_p += sp2C * ((f32)(temp_r26 - i) / (f32)temp_r26);
    }
}

static void chain_control_21(e_th_ball_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    int i;
    cXyz sp44;
    cXyz sp38;
    Vec sp2C;
    dBgS_GndChk gndchk;

    th_chain_2s* chain_s = &i_this->field_0x11d8;
    chain_s->m_pos[0] = master->mHandL_Pos2;

    cXyz* pos_p = &chain_s->m_pos[1];
    cXyz* var_r29 = &chain_s->field_0x230[1];
    csXyz* rot_p = &chain_s->m_rot[1];
    f32* var_r28 = &chain_s->field_0x398[1];
    f32 temp_f26 = -20.0f;

    cXyz sp20;
    sp44.x = 0.0f;
    sp44.y = 0.0f;
    sp44.z = 0.0f;

    f32 temp_f28 = 0.7f + XREG_F(5);
    for (i = 1; i < 20; i++, pos_p++, rot_p++, var_r29++, var_r28++) {
        f32 temp_f31 = var_r29[0].x + (pos_p[0].x - pos_p[-1].x);
        f32 temp_f27;
        f32 temp_f30 = var_r29[0].z + (pos_p[0].z - pos_p[-1].z);
        f32 var_f29 = var_r29[0].y + (pos_p[0].y + temp_f26);
        if (var_f29 <= *var_r28) {
            var_f29 = *var_r28;
        }
        temp_f27 = var_f29 - pos_p[-1].y;

        s16 spA, sp8;
        sp8 = (s16)cM_atan2s(temp_f31, temp_f30);
        spA = -cM_atan2s(temp_f27, JMAFastSqrt((temp_f31 * temp_f31) + (temp_f30 * temp_f30)));
        cMtx_YrotS(*calc_mtx, sp8);
        cMtx_XrotM(*calc_mtx, spA);

        if (i_this->field_0x11d4 == i - 1) {
            sp44.z = 25.0f + XREG_F(2);
        }
        MtxPosition(&sp44, &sp38);
        *var_r29 = *pos_p;

        pos_p[0].x = pos_p[-1].x + sp38.x;
        pos_p[0].y = pos_p[-1].y + sp38.y;
        pos_p[0].z = pos_p[-1].z + sp38.z;

        var_r29[0].x = temp_f28 * (pos_p[0].x - var_r29[0].x);
        var_r29[0].y = temp_f28 * (pos_p[0].y - var_r29[0].y);
        var_r29[0].z = temp_f28 * (pos_p[0].z - var_r29[0].z);

        rot_p[-1].x = spA;
        rot_p[-1].y = sp8;

        if (((i_this->mCounter + i) & 15) == 0) {
            sp2C = *pos_p;
            sp2C.y += 200.0f;
            gndchk.SetPos(&sp2C);

            *var_r28 = 7.0f + dComIfG_Bgsp().GroundCross(&gndchk);
            if (*var_r28 - pos_p->y > 200.0f) {
                *var_r28 = pos_p->y;
            }
        }
    }
}

static void normal_move(e_th_ball_class* i_this, s8 param_1) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz sp30;
    cXyz sp24;
    cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    sp30.x = 0.0f;
    sp30.y = 0.0f;
    sp30.z = a_this->speedF;
    MtxPosition(&sp30, &sp24);
    sp24.y = a_this->speed.y;
    a_this->current.pos += sp24;

    a_this->speed.y -= 5.0f;
    if (param_1 != 0) {
        a_this->current.angle.x += (s16)(200.0f * a_this->speedF);
    }

    f32 y_speed = a_this->speed.y;
    f32 temp_f31 = 47.0f + AREG_F(1);
    a_this->current.pos.y -= temp_f31;
    a_this->old.pos.y -= temp_f31;

    i_this->mAcch.CrrPos(dComIfG_Bgsp());

    a_this->current.pos.y += temp_f31;
    a_this->old.pos.y += temp_f31;

    if (i_this->mAcch.ChkGroundHit()) {
        if (y_speed < -30.0f + TREG_F(19)) {
            a_this->speed.y = 20.0f + TREG_F(18);
            i_this->mSound.startCollisionSE(Z2SE_HIT_HAMMER, dKy_pol_sound_get(&i_this->mAcch.m_gnd));
            
            cXyz sp18(a_this->current.pos);
            sp18.y -= 20.0f;
            cXyz spC(2.0f, 2.0f, 2.0f);
        }

        cLib_addCalc0(&a_this->speedF, 1.0f, 2.0f + TREG_F(17));
    }
}

static void e_th_ball_stop(e_th_ball_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp14;
    cXyz sp8;

    switch (i_this->mMode) {
    case 0:
        if (master->field_0x68a & 1) {
            master->field_0x68a &= ~1;
            i_this->mAction = ACTION_SPIN;
            i_this->mMode = 0;
        }
        break;
    case 1:
        break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 1.0f + TREG_F(17));
    normal_move(i_this, 1);
}

static void e_th_ball_spin(e_th_ball_class* i_this) {
    cXyz sp34;
    cXyz sp28;

    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        i_this->mTimers[0] = 30;
        i_this->field_0x15c0 = 13.0f + YREG_F(16);
        i_this->speedF = 0.0f;
    case 1:
    case 2:
    case 3:
        sp34 = i_this->current.pos - master->mHandR_Pos1;

        if (i_this->mTimers[0] != 0) {
            cLib_addCalcAngleS2(&i_this->shape_angle.y, cM_atan2s(sp34.x, sp34.z), 1, 0xC00);
            cLib_addCalcAngleS2(&i_this->shape_angle.x, -cM_atan2s(sp34.y, JMAFastSqrt((sp34.x * sp34.x) + (sp34.z * sp34.z))), 1, 0xC00);

            if (i_this->mTimers[0] == JREG_S(6) + 15) {
                i_this->field_0x15c4 = JREG_S(7) + 2500;
            }
        } else {
            i_this->shape_angle.y = cM_atan2s(sp34.x, sp34.z);
            i_this->shape_angle.x = -cM_atan2s(sp34.y, JMAFastSqrt((sp34.x * sp34.x) + (sp34.z * sp34.z)));
            i_this->field_0x1a8c = 1;
        }

        i_this->shape_angle.z += i_this->field_0x15c4;

        cLib_addCalcAngleS2(&i_this->field_0x15c4, 0, 1, JREG_S(8) + 25);
        cLib_addCalcAngleS2(&i_this->current.angle.y, 0, 1, 0x800);
        cLib_addCalcAngleS2(&i_this->current.angle.x, 0, 1, 0x800);

        if (master->field_0x68a & 1) {
            master->field_0x68a &= ~1;
            i_this->mMode++;
        }

        if (i_this->mMode == 2) {
            i_this->field_0x15c0 = master->mpModelMorf->getFrame();
        } else {
            i_this->field_0x15c0 += master->mSpinAnmSpeed;
            if (i_this->field_0x15c0 >= 29.0f) {
                i_this->field_0x15c0 -= 29.0f;
            }
        }

        s16 spA = 65536.0f * (i_this->field_0x15c0 / (29.0f + AREG_F(9)));
        cMtx_YrotS(*calc_mtx, spA + master->shape_angle.y + AREG_S(0) + 15000);
        sp34.x = 0.0f;
        sp34.y = -130.0f + AREG_F(8);
        sp34.z = 300.0f + AREG_F(10);
        MtxPosition(&sp34, &sp28);

        cLib_addCalc2(&i_this->speedF, 1000.0f, 1.0f, 1.0f + YREG_F(15));
        cLib_addCalc2(&i_this->current.pos.x, master->mHandR_Pos1.x + sp28.x, 0.5f, i_this->speedF);
        cLib_addCalc2(&i_this->current.pos.y, master->mHandR_Pos1.y + sp28.y, 0.1f, 0.02f * i_this->speedF * master->mSpinAnmSpeed);
        cLib_addCalc2(&i_this->current.pos.z, master->mHandR_Pos1.z + sp28.z, 0.5f, i_this->speedF);

        if (master->field_0x68a & 2) {
            master->field_0x68a &= ~2;

            fopAc_ac_c* player = dComIfGp_getPlayer(0);
            i_this->speedF = 80.0f + TREG_F(15);
            sp34 = player->current.pos - i_this->current.pos;
            i_this->current.angle.y = cM_atan2s(sp34.x, sp34.z);

            s16 sp8 = i_this->current.angle.y - master->shape_angle.y;
            if (sp8 > 0x3000) {
                i_this->current.angle.y = master->shape_angle.y + 0x3000;
            } else if (sp8 < -0x3000) {
                i_this->current.angle.y = master->shape_angle.y - 0x3000;
            }

            if (player->current.pos.y - master->current.pos.y > 50.0f) {
                i_this->speed.y = 40.0f + TREG_F(12);
            } else {
                i_this->speed.y = 10.0f + TREG_F(16);
            }

            i_this->mAction = ACTION_SHOT;
            i_this->mMode = 0;
        }
    }
}

static s16 wall_angle_get(e_th_ball_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    dBgS_LinChk linchk;
    cXyz sp28;
    cXyz sp1C;
    cXyz sp38[2];

    cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    sp28.x = 0.0f;
    sp28.y = 0.0f;
    sp28.z = -50.0f;
    MtxPosition(&sp28, &sp1C);
    sp1C += a_this->current.pos;
    sp28.x = 5.0f;
    sp28.y = 0.0f;
    sp28.z = 200.0f;

    for (int i = 0; i < 2; i++) {
        MtxPosition(&sp28, &sp38[i]);
        sp28.x *= -1.0f;
        sp38[i] += sp1C;
        linchk.Set(&sp1C, &sp38[i], a_this);

        if (dComIfG_Bgsp().LineCross(&linchk)) {
            sp38[i] = linchk.GetCross();
        } else {
            OS_REPORT("E_TH_BALL WALL CHECK NON ??\n");
            return 35;
        }
    }    

    sp28 = sp38[1] - sp38[0];
    return cM_atan2s(sp28.x, sp28.z) + 0x4000;
}

static void e_th_ball_shot(e_th_ball_class* i_this) {
    cXyz sp28;
    cXyz sp1C;
    s8 sp8 = 1;
    f32 temp_f31 = i_this->speedF;

    switch (i_this->mMode) {
    case 1:
        break;
    case 0:
        i_this->mMode = 1;
        break;
    case 2:
        sp8 = 0;
        cLib_addCalc2(&i_this->speedF, -25.0f, 1.0f, 15.0f + AREG_F(7));
        if (i_this->speedF > 0.0f) {
            cLib_addCalc2(&i_this->field_0xde4, 100.0f + JREG_F(0), 1.0f, 30.0f + JREG_F(1));

            sp28 = i_this->current.pos - master->mHandR_Pos1;
            s16 spE = cM_atan2s(sp28.x, sp28.z);
            cLib_addCalcAngleS2(&i_this->shape_angle.y, spE, 1, 0x4000);
            cLib_addCalcAngleS2(&i_this->shape_angle.x, 0, 1, 0x4000);
        } else if (temp_f31 > 0.0f) {
            i_this->current.angle.y += (s16)cM_rndFX(4000.0f);
            i_this->speed.y = 20.0f + AREG_F(5);
        }
    
        if (i_this->speedF < -20.0f + AREG_F(6)) {
            i_this->mAction = ACTION_RETURN;
            i_this->mMode = 0;
        }
        break;
    }

    normal_move(i_this, sp8);

    if (i_this->speedF > 0.0f && i_this->mAcch.ChkWallHit()) {
        s16 wall_angle = wall_angle_get(i_this);
        i_this->mSound.startCollisionSE(Z2SE_HIT_HAMMER, dKy_pol_sound_get(&i_this->mAcchCir));

        if (wall_angle != 35) {
            s16 spA = i_this->current.angle.y - wall_angle;
            i_this->current.angle.y += (s16)(0x8000 - (spA * 2));
            i_this->speedF *= 0.3f + AREG_F(14);
        } else {
            i_this->current.angle.y -= 0x8000;
            i_this->speedF *= 0.3f + AREG_F(14);
        }

        i_this->mAction = ACTION_RETURN;
        i_this->mMode = 0;
        i_this->mTimers[0] = 30;
    }

    if (i_this->mMode == 1 && i_this->field_0xdcc > 160.0f + AREG_F(3)) {
        i_this->mMode = 2;
    }
}

static void e_th_ball_return(e_th_ball_class* i_this) {
    cXyz sp18;
    cXyz spC;

    MTXCopy(master->mpModelMorf->getModel()->getAnmMtx(0x14), *calc_mtx);
    spC.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&spC, &sp18);
    sp18 -= i_this->current.pos;
    s16 temp_r28 = cM_atan2s(-sp18.x, -sp18.z);
    s8 sp8 = 1;

    switch (i_this->mMode) {
    case 0:
        if (master->field_0x68a & 1) {
            master->field_0x68a &= ~1;
            i_this->mMode = 1;
        }

        if (i_this->mTimers[0] == 0) {
            i_this->field_0x15c6 = 1;
        }
        break;
    case 1:
        cLib_addCalc2(&i_this->speedF, -40.0f, 1.0f, 8.0f);
        cLib_addCalcAngleS2(&i_this->shape_angle.y, temp_r28, 1, 0x1000);
        cLib_addCalcAngleS2(&i_this->shape_angle.x, 0, 1, 0x1000);

        sp8 = 0;
        i_this->current.angle.y = temp_r28;
        cLib_addCalcAngleS2(&i_this->current.angle.x, 0, 1, 0x1000);

        if (i_this->speedF <= -40.0f) {
            i_this->speedF = -40.0f + BREG_F(15);
            i_this->speed.y = 30.0f + BREG_F(16);
            i_this->shape_angle.y = cM_rndFX(3000.0f);
            i_this->shape_angle.x = cM_rndFX(3000.0f);
            i_this->field_0xde4 = 100.0f + JREG_F(2);
            i_this->mMode = 2;
        }
        /* fallthrough */
    case 2:
        i_this->current.angle.y = temp_r28;

        if (sp18.abs() < 80.0f + AREG_F(18)) {
            i_this->speed.y = 0.0f;
            i_this->speedF = 10.0f;
            i_this->mMode = 3;
            i_this->current.angle.y += (s16)cM_rndFX(6000.0f);
        } else if (fabsf(i_this->speedF) < 0.1f) {
            i_this->mMode = 3;
        }
        break;
    case 3:
        if (fabsf(i_this->speedF) < 0.1f) {
            i_this->mAction = ACTION_STOP;
            i_this->mMode = 0;
        }
        break;
    }

    normal_move(i_this, sp8);
}

static void e_th_ball_end(e_th_ball_class* i_this) {
    cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f + TREG_F(17));
    normal_move(i_this, 1);
    daPy_py_c::setLookPos(&i_this->current.pos);
    i_this->mCcSph.SetR(45.0f);

    if (i_this->mDemoMode == 0 && fopAcM_searchPlayerDistance(i_this) < 130.0f) {
        dComIfGp_setDoStatusForce(0x35, 0);
        if (mDoCPd_c::getTrigA(PAD_1)) {
            i_this->mDemoMode = 1;
        }
    }
}

static void action(e_th_ball_class* i_this) {
    f32 var_f31 = 0.0f;
    f32 var_f30 = 1.0f;
    f32 var_f28 = 0.02f;
    f32 var_f29 = 0.04f;

    if (master->field_0x68a & 4) {
        master->field_0x68a &= ~4;
        i_this->mAction = ACTION_END;
        i_this->mMode = 0;
    }

    switch (i_this->mAction) {
    case ACTION_STOP:
        e_th_ball_stop(i_this);
        var_f31 = 0.0f;
        i_this->field_0x15c6 = 1;
        var_f30 = TREG_F(13);
        break;
    case ACTION_SPIN:
        e_th_ball_spin(i_this);
        var_f30 = 1.0f + KREG_F(6);
        var_f31 = 0.5f + KREG_F(7);
        var_f28 = 0.002f + YREG_F(13);
        var_f29 = 0.05f + YREG_F(14);
        break;
    case ACTION_SHOT:
        e_th_ball_shot(i_this);
        var_f30 = 0.85f + TREG_F(14);
        var_f31 = TREG_F(3);
        var_f29 = 0.05f + BREG_F(1);
        i_this->field_0x1a8c = 1;
        break;
    case ACTION_RETURN:
        if (i_this->mMode == 1) {
            var_f31 = 1.0f + AREG_F(8);
            var_f28 = 0.3f + AREG_F(9);
            var_f30 = 1.0f + AREG_F(10);
            var_f29 = 0.5f + AREG_F(11);
        } else if (i_this->mMode == 0) {
            var_f30 = 0.85f + TREG_F(14);
            var_f31 = TREG_F(3);
            var_f29 = 0.05f + BREG_F(1);
        } else {
            var_f31 = 0.0f;
            var_f28 = 0.5f + AREG_F(9);
            i_this->field_0x15c6 = 1;
            var_f30 = TREG_F(13);
        }
        e_th_ball_return(i_this);
        break;
    case ACTION_END:
        e_th_ball_end(i_this);
        var_f31 = 0.0f;
        i_this->field_0x15c6 = 1;
        var_f30 = TREG_F(13);
        i_this->field_0xdd0 = VREG_S(3) + 25;
        break;
    }

    i_this->mCcSph.SetC(i_this->current.pos);

    if (master->field_0x6ea != 0) {
        i_this->mCcSph.OnTgShield();
        i_this->mCcSph.OffTgNoHitMark();
    } else {
        i_this->mCcSph.OffTgShield();
        i_this->mCcSph.OnTgNoHitMark();
    }

    dComIfG_Ccsp()->Set(&i_this->mCcSph);

    cXyz at_sph_center(i_this->current.pos);
    if (i_this->field_0x1a8c == 0) {
        at_sph_center.x += 12000.0f;
    } else {
        i_this->field_0x1a8c = 0;
    }

    i_this->mAtSph.SetC(at_sph_center);
    dComIfG_Ccsp()->Set(&i_this->mAtSph);

    cLib_addCalc2(&i_this->field_0xdc8, var_f30, 1.0f, var_f29);
    cLib_addCalc2(&i_this->field_0xdd4, var_f31, 1.0f, var_f28);
    cLib_addCalc0(&i_this->field_0xde4, 1.0f, 10.0f + JREG_F(1));

    if (i_this->field_0x15c6 == 0) {
        cXyz sp2C = i_this->current.pos - master->mHandR_Pos1;
        
        s16 var_r27 = i_this->field_0xdc8 * (50.0f - ((0.035f + XREG_F(3)) * sp2C.abs()));
        if (var_r27 < 0) {
            var_r27 = 0;
        } else if (var_r27 > 45) {
            var_r27 = 45;
        }

        s16 sp8;
        if (fabsf(i_this->speedF) > 10.0f) {
            sp8 = 10;
        } else {
            sp8 = 2;
        }
        cLib_addCalcAngleS2(&i_this->field_0xdd0, var_r27, 1, sp8);
    }

    i_this->field_0x15c6 = 0;
    i_this->field_0xde8 = (s16)((39 - i_this->field_0xdd0) / 4);
    if (i_this->field_0xde8 < 0) {
        i_this->field_0xde8 = 0;
    }

    i_this->field_0x11d4 = (s16)((39 - i_this->field_0xdd0) / 2);
    if (i_this->field_0x11d4 < 0) {
        i_this->field_0x11d4 = 0;
    }

    if (i_this->field_0x658 == 0) {
        if (i_this->mCcSph.ChkTgHit()) {
            i_this->field_0x658 = 10;
            def_se_set(&i_this->mSound, i_this->mCcSph.GetTgHitObj(), 0x28, NULL);
        }

        if (i_this->mAtSph.ChkAtHit()) {
            cCcD_Obj* at_hit_obj = i_this->mAtSph.GetAtHitObj();
            fopAc_ac_c* at_hit_actor = dCc_GetAc(at_hit_obj->GetAc());

            if (i_this->mAction == ACTION_SPIN) {
                if (fopAcM_GetName(at_hit_actor) == PROC_Obj_THASHI) {
                    i_this->speedF = 30.0f + TREG_F(18);
                    
                    cXyz sp20 = i_this->current.pos - master->current.pos;
                    i_this->current.angle.y = cM_atan2s(sp20.x, sp20.z);
                    i_this->speed.y = TREG_F(16);
                    i_this->mAction = ACTION_SHOT;
                    i_this->mMode = 0;

                    master->mAction = ACTION_RETURN;
                    master->mMode = 2;
                    master->mTimers[0] = JREG_S(4) + 30;
                    master->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_th", 0x1B), 2, 10.0f, 1.0f, 0.0f, -1.0f);
                    master->mAnm = 0x1B;
                }
            } else if (i_this->mAction == ACTION_SHOT && i_this->speedF > 0.0f && fopAcM_GetName(at_hit_actor) == PROC_E_MD) {
                i_this->current.angle.y -= 0x8000;
                i_this->speedF *= 0.3f + AREG_F(14);
                i_this->mAction = ACTION_RETURN;
                i_this->mMode = 0;
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER6, 0x4F, cXyz(0.0f, 1.0f, 0.0f));
            }
        }
    }
}

static fpc_ProcID demo_id;

static void get_demo(e_th_ball_class* i_this) {
    cXyz sp14;
    cXyz sp8;

    switch (i_this->mDemoMode) {
    case 0:
        break;
    case 1:
        demo_id = fopAcM_createItemForTrBoxDemo(&i_this->current.pos, fpcNm_ITEM_IRONBALL, -1, fopAcM_GetRoomNo(i_this), NULL, NULL);
        JUT_ASSERT(1670, demo_id != fpcM_ERROR_PROCESS_ID_e);
        i_this->mDemoMode = 2;
        break;
    case 2:
        if (i_this->eventInfo.checkCommandItem()) {
            if (demo_id != fpcM_ERROR_PROCESS_ID_e) {
                dComIfGp_event_setItemPartnerId(demo_id);
            }

            i_this->mPlayerGet = TRUE;
            dComIfGs_onSaveSwitch(95);
        } else {
            fopAcM_orderItemEvent(i_this, 0, 0);
            i_this->eventInfo.onCondition(8);
        }
    }
}

static int daE_TH_BALL_Execute(e_th_ball_class* i_this) {
    cXyz sp14;
    cXyz sp8;

    if (i_this->mPlayerGet) {
        return 1;
    }

    master = (e_th_class*)fopAcM_SearchByID(i_this->parentActorID);
    if (master == NULL) {
        return 1;
    }

    i_this->mCounter++;

    for (int i = 0; i < 3; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->field_0x658 != 0) {
        i_this->field_0x658--;
    }

    action(i_this);

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->current.angle.y);
    mDoMtx_stack_c::XrotM(i_this->current.angle.x);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x - 0x4000);
    mDoMtx_stack_c::YrotM(-i_this->current.angle.y + i_this->shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, 55.0f + XREG_F(4), 0.0f);
    i_this->mpBallModel->setBaseTRMtx(mDoMtx_stack_c::get());

    mDoMtx_stack_c::transM(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&i_this->field_0xdd8);

    chain_control_01(i_this);
    chain_control_02(i_this);
    chain_control_03(i_this);
    chain_control_11(i_this);
    chain_control_12(i_this);
    chain_control_13(i_this);
    chain_control_21(i_this);
    get_demo(i_this);

    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));
    return 1;
}

static int daE_TH_BALL_IsDelete(e_th_ball_class* i_this) {
    return 1;
}

static int daE_TH_BALL_Delete(e_th_ball_class* i_this) {
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, "E_th_ball");

    if (i_this->heap != NULL) {
        i_this->mSound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    e_th_ball_class* i_this = (e_th_ball_class*)a_this;

    void* modelData = dComIfG_getObjectRes("E_th_ball", 4);
    JUT_ASSERT(1839, modelData != NULL);
    i_this->mpBallModel = mDoExt_J3DModel__create((J3DModelData*)modelData, 0, 0x11000084);
    if (i_this->mpBallModel == NULL) {
        return 0;
    }

    modelData = dComIfG_getObjectRes("E_th_ball", 7);
    JUT_ASSERT(1851, modelData != NULL);
    for (int i = 0; i < 50; i++) {
        i_this->field_0x65c.mp_model[i] = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
        if (i_this->field_0x65c.mp_model[i] == NULL) {
            return 0;
        }
    }

    for (int i = 0; i < 20; i++) {
        i_this->field_0xdec.mp_model[i] = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
        if (i_this->field_0xdec.mp_model[i] == NULL) {
            return 0;
        }
    }

    for (int i = 0; i < 20; i++) {
        i_this->field_0x11d8.mp_model[i] = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
        if (i_this->field_0x11d8.mp_model[i] == NULL) {
            return 0;
        }
    }

    return 1;
}

static int daE_TH_BALL_Create(fopAc_ac_c* a_this) {
    e_th_ball_class* i_this = (e_th_ball_class*)a_this;
    fopAcM_ct(i_this, e_th_ball_class);

    int phase_state = dComIfG_resLoad(&i_this->mPhase, "E_th_ball");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_TH_BALL PARAM %x\n", fopAcM_GetParam(a_this));
        OS_REPORT("E_TH_BALL//////////////E_TH_BALL SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x4B000)) {
            OS_REPORT("//////////////E_TH_BALL SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_TH_BALL SET 2 !!\n");

        fopAcM_SetMtx(i_this, i_this->mpBallModel->getBaseTRMtx());
        fopAcM_SetMin(i_this, -2000.0f, -1000.0f, -2000.0f);
        fopAcM_SetMax(i_this, 2000.0f, 1000.0f, 2000.0f);
        
        i_this->mCcStts.Init(0xF0, 0, i_this);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8ebfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x1, 0x2, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_METAL, 0x2, 0x0, 0x0, 0x3}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 55.0f} // mSph
            } // mSphAttr
        };

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_BOSS_SWING, 0x4, 0x1d}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_NONE, 0x1, 0xe, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 55.0f} // mSph
            } // mSphAttr
        };

        i_this->mCcSph.Set(cc_sph_src);
        i_this->mCcSph.SetStts(&i_this->mCcStts);
        i_this->mAtSph.Set(at_sph_src);
        i_this->mAtSph.SetStts(&i_this->mCcStts);

        i_this->mSound.init(&i_this->current.pos, NULL, 3, 1);

        i_this->mAcch.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1, &i_this->mAcchCir, fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(50.0f, 50.0f);

        daE_TH_BALL_Execute(i_this);
    }

    return phase_state;
}

static actor_method_class l_daE_TH_BALL_Method = {
    (process_method_func)daE_TH_BALL_Create,
    (process_method_func)daE_TH_BALL_Delete,
    (process_method_func)daE_TH_BALL_Execute,
    (process_method_func)daE_TH_BALL_IsDelete,
    (process_method_func)daE_TH_BALL_Draw,
};

extern actor_process_profile_definition g_profile_E_TH_BALL = {
  fpcLy_CURRENT_e,         // mLayerID
  8,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_E_TH_BALL,          // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(e_th_ball_class), // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  135,                     // mPriority
  &l_daE_TH_BALL_Method,   // sub_method
  0x00040100,              // mStatus
  fopAc_ENEMY_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
