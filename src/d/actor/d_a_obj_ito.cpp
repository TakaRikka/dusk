/**
 * @file d_a_obj_ito.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_ito.h"
#include "d/actor/d_a_obj_gm.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"

daObj_Ito_HIO_c::daObj_Ito_HIO_c() {
    field_0x4 = -1;
    field_0x8 = 4.5f;
    field_0xc = 15.0;
    field_0x10 = 1;
}

static u8 l_initHio;

static daObj_Ito_HIO_c l_HIO;

static int daObj_Ito_Draw(obj_ito_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    if (!i_this->field_0x639) {
        return 1;
    }

    g_env_light.settingTevStruct(16, &a_this->current.pos, &a_this->tevStr);

    if (l_HIO.field_0x10 == 0 || (!i_this->field_0x1038 && l_HIO.field_0x10 == 1)) {
        g_env_light.setLightTevColorType_MAJI(i_this->field_0x574[0], &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->field_0x574[0]);
        g_env_light.setLightTevColorType_MAJI(i_this->field_0x574[1], &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->field_0x574[1]);
    }

    if (l_HIO.field_0x10 != 0 && (i_this->field_0x1038 || l_HIO.field_0x10 == 2)) {
        g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
        u8 uVar1 = JREG_S(5) + 0xB4;
        GXColor color = {uVar1, uVar1, uVar1, 0xFF};
        i_this->field_0x103c.mLineMat.update(20, color, &a_this->tevStr);
        dComIfGd_set3DlineMat(&i_this->field_0x103c.mLineMat);

        if (i_this->field_0x1038 && i_this->field_0x1298.field_0x250 > 0.01f) {
            GXColor color2 = {uVar1, uVar1, uVar1, 0xFF};
            i_this->field_0x1298.mLineMat.update(20, color2, &a_this->tevStr);
            dComIfGd_set3DlineMat(&i_this->field_0x1298.mLineMat);
        }
    }

    return 1;
}

static void cut_line_calc(obj_ito_class* i_this, ito_s* param_2, int param_3) {
    int i;
    fopAc_ac_c* a_this = &i_this->actor;
    obj_gm_class* actor_p = (obj_gm_class*)fopAcM_SearchByID(a_this->parentActorID);
    dBgS_GndChk gnd_chk;
    Vec i_pos;
    cXyz sp104, sp110;
    cXyz* pos = param_2->mLineMat.getPos(param_3);
    f32* pfVar1 = &param_2->field_0x1c[param_3][0] + 1;
    f32 fVar1 = param_2->field_0x254 - 30.0f;
    cXyz sp11c(0.0f, 0.0f, 0.0f);
    cXyz sp128;

    if (param_2 == &i_this->field_0x103c && actor_p != NULL && actor_p->mAction == 2) {
        sp11c = *pos - actor_p->actor.current.pos;
        sp11c *= (XREG_F(2) + 0.0018f) * actor_p->actor.speedF;
    }

    cXyz sp134;

    param_2->field_0x244[param_3]++;
    cLib_addCalc0(&param_2->field_0x258, 0.1f, XREG_F(1) + 0.5f);

    pos++;
    f32 fVar2 = 0.0f;
    f32 fVar3 = 0.0f;
    BOOL bVar1 = false;
    cXyz sp140;
    sp104.y = sp104.z = 0.0f;

    if (actor_p != NULL && actor_p->mAction == 1) {
        bVar1 = true;
        cMtx_YrotS(*calc_mtx, actor_p->field_0x64c[0] + param_3 * (hREG_S(3) + 18000));
        sp104.x = hREG_F(8) + 2.0f;
        MtxPosition(&sp104, &sp140);
    }

    sp104.x = 0.0f;
    sp104.z = param_2->field_0x250 * (1.0f - param_3 * 0.03f);

    for (i = 1; i < 20; i++, pos++, pfVar1++) {
        sp134.x = param_2->field_0x258 * cM_ssin((XREG_S(0) + 2500) * param_2->field_0x244[param_3] + i * (XREG_S(1) + 8000));
        sp134.z = param_2->field_0x258 * cM_ssin((XREG_S(2) + 2000) * param_2->field_0x244[param_3] + i * (XREG_S(3) + 7000));

        if (bVar1) {
            fVar3 = sp140.x * cM_ssin(i * (hREG_S(1) + 3000));
            fVar2 = sp140.z * cM_ssin(i * (hREG_S(2) + 4000));
        }

        sp128 = sp11c * (21 - i);
        f32 var_f26;
        f32 var_f31 = pos[0].x - pos[-1].x + sp134.x + sp128.x + fVar3;
        f32 var_f30 = pos[0].z - pos[-1].z + sp134.z + sp128.z + fVar2;
        f32 var_f29 = pos[0].y + fVar1 + sp128.y;

        if (var_f29 < *pfVar1) {
            var_f29 = *pfVar1;
        }

        var_f26 = var_f29 - pos[-1].y;

        cMtx_YrotS(*calc_mtx, cM_atan2s(var_f31, var_f30));
        cMtx_XrotM(*calc_mtx, -cM_atan2s(var_f26, JMAFastSqrt(var_f31 * var_f31 + var_f30 * var_f30)));
        MtxPosition(&sp104, &sp110);
        pos[0].x = pos[-1].x + sp110.x;
        pos[0].y = pos[-1].y + sp110.y;
        pos[0].z = pos[-1].z + sp110.z;

        if ((i_this->field_0x60c + i & 15) == 0) {
            i_pos = pos[0];
            i_pos.y += 200.0f;
            gnd_chk.SetPos(&i_pos);
            *pfVar1 = dComIfG_Bgsp().GroundCross(&gnd_chk) + 5.0f;

            if (pfVar1[0] - pos[0].y > 200.0f) {
                pfVar1[0] = pos[0].y;
            }
        }
    }
}

static void cut_line(obj_ito_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    obj_gm_class* actor_p = (obj_gm_class*)fopAcM_SearchByID(a_this->parentActorID);
    for (int i = 0; i < 6; i++) {
        cXyz* pos = i_this->field_0x103c.mLineMat.getPos(i);

        if (actor_p != NULL) {
            if (i_this->field_0x570 == 0) {
                *pos = actor_p->field_0x684[i];
            } else {
                *pos = actor_p->field_0x6cc[i];
            }
        }

        cut_line_calc(i_this, &i_this->field_0x103c, i);

        if (i_this->field_0x1038 != 0 && i_this->field_0x1298.field_0x250 > 0.01f) {
            cut_line_calc(i_this, &i_this->field_0x1298, i);
            cLib_addCalc0(&i_this->field_0x1298.field_0x250, 1.0f, 1.0f);
        }
    }
}

static void cut_set(obj_ito_class* i_this, int param_2) {
    fopAc_ac_c* a_this = &i_this->actor;
    i_this->field_0x1038 = 1;
    cXyz sp44;

    if (param_2 == 100) {
        sp44 = a_this->current.pos + ((i_this->field_0x618 - a_this->current.pos) * 0.05f);
    } else {
        sp44 = i_this->field_0x678[param_2 - 1].GetC();
    }

    if (i_this->field_0x1509 != 0) {
        i_this->field_0x1298.field_0x250 = 0.0f;
        i_this->field_0x103c.field_0x250 = 0.0f;
    } else {
        i_this->field_0x103c.field_0x250 = (TREG_F(16) + 0.055f) * (a_this->current.pos - sp44).abs();
        i_this->field_0x1298.field_0x250 = (TREG_F(16) + 0.055f) * (i_this->field_0x618 - sp44).abs();
    }

    cXyz sp50((sp44 - a_this->current.pos) * 0.0526f);
    cXyz sp5c((sp44 - i_this->field_0x618) * 0.0526f);

    cXyz* pos;
    for (int i = 0; i < 6; i++) {
        pos = i_this->field_0x103c.mLineMat.getPos(i);
        pos++;
        for (int j = 1; j < 20; j++, pos++) {
            *pos = a_this->current.pos + (sp50 * j);
        }

        pos = i_this->field_0x1298.mLineMat.getPos(i);
        pos++;
        for (int k = 1; k < 20; k++, pos++) {
            *pos = i_this->field_0x618 + (sp5c * k);
        }
    }

    i_this->field_0x103c.field_0x258 = XREG_F(0) + 20.0f;
    i_this->field_0x1298.field_0x258 = XREG_F(0) + 20.0f;
    fopAcM_OffStatus(a_this, 0);
    a_this->attention_info.flags = 0;

    if (l_HIO.field_0x10 == 0) {
        i_this->field_0x638 = 1;
    }
}

static void ito_end(ito_s* i_this) {
    cLib_addCalc0(&i_this->field_0x250, 1.0f, 1.0f);

    for (int i = 0; i < 6; i++) {
        cXyz* pcVar1 = i_this->mLineMat.getPos(i);
        f32* pfVar1 = &i_this->field_0x1c[i][0];

        for (int j = 0; j < 20; j++, pcVar1++, pfVar1++) {
            pcVar1->y -= 10.0f;

            if (pcVar1->y < *pfVar1) {
                pcVar1->y = *pfVar1;
            }
        }
    }
}

static void action(obj_ito_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->actor;
    obj_gm_class* actor_p = (obj_gm_class*)fopAcM_SearchByID(a_this->parentActorID);
    if (actor_p == NULL) {
        i_this->field_0x638 = 1;
    }

    cXyz spe8, spf4;
    i_this->field_0x634 = a_this->scale.x;

    if (actor_p != NULL) {
        actor_p->field_0x614[i_this->field_0x570] = i_this->field_0x618;
        a_this->current.pos = actor_p->field_0x654[i_this->field_0x570];
        spe8 = i_this->field_0x618 - a_this->current.pos;
        i_this->field_0x624 = a_this->current.pos + (spe8 * 0.5f);

        if (actor_p->mAction == 0) {
            spe8 = actor_p->field_0x654[i_this->field_0x570] - actor_p->field_0x66c[i_this->field_0x570];
            i_this->field_0x624 += spe8 * (TREG_F(5) + 0.25f);
        }
    }

    if (l_HIO.field_0x10 == 0 || (i_this->field_0x1038 == 0 && l_HIO.field_0x10 == 1)) {
        spe8 = i_this->field_0x624 - a_this->current.pos;

        if (i_this->field_0x638 == 0) {
            i_this->field_0x630 = l_HIO.field_0x8 * spe8.abs() * 0.001f;
        } else {
            cLib_addCalc0(&i_this->field_0x630, 1.0f, 0.5f);

            if (i_this->field_0x630 <= 0.01f) {
                fopAcM_delete(a_this);
                return;
            }
        }

        s16 sVar1 = -cM_atan2s(spe8.y, spe8.z);
        s32 sVar2 = cM_atan2s(spe8.x, JMAFastSqrt(spe8.y * spe8.y + spe8.z * spe8.z));
        mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
        mDoMtx_stack_c::XrotM(sVar1);
        mDoMtx_stack_c::YrotM(sVar2);
        mDoMtx_stack_c::XrotM(TREG_S(6) - 0x4000);

        if (actor_p != NULL) {
            mDoMtx_stack_c::YrotM(actor_p->field_0x64c[0]);
        }

        mDoMtx_stack_c::scaleM(i_this->field_0x634, i_this->field_0x630, i_this->field_0x634);
        i_this->field_0x574[0]->setBaseTRMtx(mDoMtx_stack_c::get());
        spe8 = i_this->field_0x624 - i_this->field_0x618;

        sVar1 = -cM_atan2s(spe8.y , spe8.z);
        sVar2 = cM_atan2s(spe8.x, JMAFastSqrt(spe8.y * spe8.y + spe8.z * spe8.z));
        mDoMtx_stack_c::transS(i_this->field_0x618.x, i_this->field_0x618.y, i_this->field_0x618.z);
        mDoMtx_stack_c::XrotM(sVar1);
        mDoMtx_stack_c::YrotM(sVar2);
        mDoMtx_stack_c::XrotM(TREG_S(6) - 0x4000);

        if (actor_p != NULL) {
            mDoMtx_stack_c::YrotM(actor_p->field_0x64c[0] / 2);
        }

        mDoMtx_stack_c::scaleM(i_this->field_0x634, i_this->field_0x630, i_this->field_0x634);
        i_this->field_0x574[1]->setBaseTRMtx(mDoMtx_stack_c::get());

        if (i_this->field_0x1509 != 0) {
            cXyz sp17c = i_this->field_0x14f8;
            i_this->field_0x14f8 = a_this->current.pos + ((i_this->field_0x618 - a_this->current.pos) * i_this->field_0x1504);

            if (i_this->field_0x1508 == 0) {
                i_this->field_0x1504 += KREG_F(8) + 0.11f;

                if (i_this->field_0x1504 >= 1.0f) {
                    i_this->field_0x1504 = 1.0f;
                    i_this->field_0x1508 = 1;
                }
            } else {
                i_this->field_0x1504 -= KREG_F(8) + 0.11f;

                if (i_this->field_0x1504 <= 0.0f) {
                    i_this->field_0x1504 = 0.0f;
                    i_this->field_0x1508 = 0;
                }
            }

            i_this->field_0x14f4 = dComIfGp_particle_set(i_this->field_0x14f4, 0x840D, &i_this->field_0x14f8, NULL, NULL);

            mDoAud_seStartLevel(Z2SE_OBJ_WEB_BURN, &i_this->field_0x14f8, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
            i_this->field_0x1509++;
        }
    }

    a_this->eyePos = i_this->field_0x624;
    a_this->eyePos.y += 20.0f;
    a_this->attention_info.position = a_this->eyePos;

    for (int i = 0; i < 8; i++) {
        spf4 = a_this->current.pos - i_this->field_0x618;
        spf4 = (i_this->field_0x618 + (((spf4 * (i + 1))) * (TREG_F(8) + 0.11f)));

        if (actor_p == NULL || i_this->field_0x63a != 0) {
            spf4.y -= 12300.0f;
        }

        i_this->field_0x678[i].SetC(spf4);
        i_this->field_0x678[i].SetR(JREG_F(2) + 30.0f);
        dComIfG_Ccsp()->Set(&i_this->field_0x678[i]);

        if (i_this->field_0x678[i].ChkCoHit()) {
            fopAc_ac_c* var_r25 = i_this->field_0x678[i].GetCoHitObj()->GetAc();
            if (actor_p != NULL && var_r25 != &actor_p->actor) {
                actor_p->field_0xae4 |= 0x4;
            }
        }
    }

    if (actor_p != NULL && actor_p->actor.field_0x567 != 0) {
        actor_p->actor.field_0x567 = 0;
        actor_p->field_0xae4 |= 2;
        cut_set(i_this, 100);
    }

    if (i_this->field_0x63a == 0 && i_this->field_0x638 == 0) {
        int iVar1 = 0;
        dCcU_AtInfo atInfo;
        for (int i = 0; i < 8; i++) {
            if (i_this->field_0x678[i].ChkTgHit()) {
                atInfo.mpCollider = i_this->field_0x678[i].GetTgHitObj();
                at_power_check(&atInfo);

                if (atInfo.mAttackPower != 0 ||
                    atInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG) ||
                    atInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT) ||
                    i_this->field_0x678[i].GetTgHitGObj()->GetAtMtrl() == 1) {
                    if (i_this->field_0x678[i].GetTgHitGObj()->GetAtMtrl() == 1) {
                        i_this->field_0x1509 = 1;
                        i_this->field_0x150a = i + 1;
                    } else {
                        iVar1 = i + 1;
                    }
                } else {
                    if (actor_p != NULL) {
                        actor_p->field_0xae4 |= 4;
                    }
                }
                break;
            }
        }

        if (i_this->field_0x1509 == 15) {
            iVar1 = i_this->field_0x150a;
        }

        if (iVar1 != 0) {
            i_this->field_0x63a = 100;

            if (i_this->field_0x1509 == 0) {
                def_se_set(&i_this->mSound, i_this->field_0x678[iVar1 - 1].GetTgHitObj(), 33, NULL);
            }

            if (i_this->field_0x570 == 1) {
                actor_p->field_0xae4 |= 1;
                cut_set(i_this, iVar1);
            } else if (i_this->field_0x570 == 0) {
                actor_p->field_0xae4 |= 2;
                cut_set(i_this, iVar1);
                
                OS_REPORT("//////////////OBJ_CUT SET !!%d\n", iVar1);
            }
        }

        if (i_this->field_0x570 == 0 && actor_p->mAction == 2 && i_this->field_0x1038 == 0) {
            cut_set(i_this, 100);
        }
    }

    if (l_HIO.field_0x10 != 0 && (i_this->field_0x1038 != 0 || l_HIO.field_0x10 == 2)) {
        if ((l_HIO.field_0x10 == 1 && i_this->field_0x1038 == 2) || (l_HIO.field_0x10 == 2 && i_this->field_0x1038 != 0)) {
            i_this->field_0x63a = 100;

            if (i_this->field_0x638 != 0) {
                ito_end(&i_this->field_0x103c);
                ito_end(&i_this->field_0x1298);

                if (i_this->field_0x103c.field_0x250 <= 0.1f && i_this->field_0x1298.field_0x250 <= 0.1f) {
                    fopAcM_delete(a_this);
                }
            }

            cut_line(i_this);
        } else {
            if (i_this->field_0x638 != 0) {
                fopAcM_delete(a_this);
            } else {
                if (l_HIO.field_0x10 == 1 && i_this->field_0x1038 == 1) {
                    i_this->field_0x1038 = 2;
                }

                a_this->current.angle = actor_p->actor.current.angle;
                cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
                cMtx_XrotM(*calc_mtx, a_this->current.angle.x);

                cXyz* pcVar1;
                s16 sVar5;
                s16 sVar3 = 0;
                s16 sVar4 = 0;
                cXyz* pcVar2;
                for (int i = 0; i < 6; i++) {
                    pcVar1 = i_this->field_0x103c.mLineMat.getPos(i);

                    if (i_this->field_0x570 == 0) {
                        pcVar2 = &actor_p->field_0x684[i];
                    } else {
                        pcVar2 = &actor_p->field_0x6cc[i];
                    }

                    cXyz sp10c;
                    if (i_this->field_0x610 == 0) {
                        cXyz sp118, sp124;

                        sp118.x = 0.0f;
                        if (i_this->field_0x570 == 0) {
                            sp118.y = 5000.0f;
                        } else {
                            sp118.y = -5000.0f;
                        }
                        sp118.z = 0.0f;
                        MtxPosition(&sp118, &sp124);

                        dBgS_LinChk lin_chk;
                        cXyz end(*pcVar2 + sp124);
                        lin_chk.Set(pcVar2, &end, a_this);

                        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                            sp118.y *= (KREG_F(7) + 0.01f);
                            MtxPosition(&sp118, &sp124);
                            i_this->field_0x103c.field_0x1fc[i] = lin_chk.GetCross() + sp124;
                        }
                    }

                    sp10c = (i_this->field_0x103c.field_0x1fc[i] - *pcVar2) * 0.05263158f;
                    f32 fVar1 = (TREG_F(9) + 20.0f) * a_this->scale.x;
                    f32 fVar2 = fVar1 * ((TREG_F(14) + 1.0f + 1.0f) / 19.0f);
                    sVar5 = TREG_S(7) + i * 0x2AAA + 22000;

                    if (actor_p != NULL) {
                        sVar3 = actor_p->field_0x64c[0];
                        sVar4 = sVar3 / 10;
                    }
                    spe8.x = 0.0f;
                    spe8.y = 0.0f;

                    for (int j = 0; j < 20; j++, pcVar1++) {
                        spe8.z = fVar1;
                        MtxPush();
                        cMtx_YrotM(*calc_mtx, sVar5 + sVar3);
                        MtxPosition(&spe8, &spf4);
                        MtxPull();

                        *pcVar1 = (*pcVar2 + (sp10c * j)) + spf4;
                        sVar5 += (s16)((TREG_F(13) + 2200.0f) + (TREG_F(12) + 1500.0f) * cM_ssin(j * (BREG_S(0) + 5000) + i * (BREG_S(1) + 3000)));

                        if (j >= 10) {
                            fVar1 -= fVar2;
                            sVar3 -= sVar4;

                            if (j == 19) {
                                *i_this->field_0x1298.mLineMat.getPos(i) = *pcVar1;
                            }
                        }
                    }
                }

                if (i_this->field_0x610 == 0) {
                    i_this->field_0x610 = 1;
                    i_this->field_0x618 = i_this->field_0x103c.field_0x1fc[0] + ((i_this->field_0x103c.field_0x1fc[3] - i_this->field_0x103c.field_0x1fc[0]) * 0.5f);
                }
            }
        }
    }
}

static int daObj_Ito_Execute(obj_ito_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    i_this->field_0x60c++;

    if (i_this->field_0x63a != 0) {
        i_this->field_0x63a--;
    }

    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x612[i] != 0) {
            i_this->field_0x612[i]--;
        }
    }

    action(i_this);
    fopAcM_OffStatus(a_this, 0);
    a_this->attention_info.flags = 0;
    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    i_this->field_0x639 = 1;

    return 1;
}

static int daObj_Ito_IsDelete(obj_ito_class* i_this) {
    return 1;
}

static int daObj_Ito_Delete(obj_ito_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    dComIfG_resDelete(&i_this->mPhase, "OBJ_ITO");

    if (i_this->field_0x150b != 0) {
        l_initHio = 0;
    }

    if (a_this->heap != NULL) {
        i_this->mSound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    obj_ito_class* i_this = (obj_ito_class*)a_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("OBJ_ITO", 3);

    JUT_ASSERT(1072, modelData != NULL);

    for (int i = 0; i < 2; i++) {
        i_this->field_0x574[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->field_0x574[i] == NULL) {
            return 0;
        }
    }

    if (i_this->field_0x103c.mLineMat.init(6, 20, 1) == 0) {
        return 0;
    }

    if (i_this->field_0x1298.mLineMat.init(6, 20, 1) == 0) {
        return 0;
    }

    f32* pfVar1;
    for (int i = 0; i < 6; i++) {
        pfVar1 = i_this->field_0x103c.mLineMat.getSize(i);
        f32 fVar1 = cM_rndF(0.3f) + 0.4f;
        for (int j = 0; j < 20; j++, pfVar1++) {
            *pfVar1 = fVar1;
        }

        pfVar1 = i_this->field_0x1298.mLineMat.getSize(i);
        fVar1 = cM_rndF(0.3f) + 0.4f;
        for (int j = 0; j < 20; j++, pfVar1++) {
            *pfVar1 = fVar1;
        }

        i_this->field_0x103c.field_0x244[i] = cM_rndF(65535.0f);
        i_this->field_0x1298.field_0x244[i] = cM_rndF(65535.0f);
    }

    return 1;
}

static cPhs__Step daObj_Ito_Create(fopAc_ac_c* a_this) {
    fopAcM_ct(a_this, obj_ito_class);
    obj_ito_class* i_this = (obj_ito_class*)a_this;
    a_this = &i_this->actor;

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->mPhase, "OBJ_ITO");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_ITO PARAM %x\n", fopAcM_GetParam(a_this));

        i_this->field_0x570 = fopAcM_GetParam(a_this) & 1;
        i_this->field_0x571 = (fopAcM_GetParam(a_this) & 30) >> 1;

        OS_REPORT("OBJ_ITO//////////////OBJ_ITO SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x5B20)) {
            OS_REPORT("//////////////OBJ_ITO SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_ITO SET 2 !!\n");

        if (l_initHio == 0) {
            i_this->field_0x150b = 1;
            l_initHio = 1;
            l_HIO.field_0x4 = -1;
        }

        fopAcM_SetMtx(a_this, i_this->field_0x574[0]->getBaseTRMtx());
        fopAcM_SetMin(a_this, -3000.0f, -3000.0f, -3000.0f);
        fopAcM_SetMax(a_this, 3000.0f, 3000.0f, 3000.0f);
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        a_this->attention_info.distances[fopAc_attn_BATTLE_e] = 22;
        i_this->field_0x60c = cM_rndF(65535.0f);
        
        i_this->mStts.Init(0xFF, 0, a_this);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbffff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        for (int i = 0; i < 8; i++) {
            i_this->field_0x678[i].Set(cc_sph_src);
            i_this->field_0x678[i].SetStts(&i_this->mStts);
        }

        i_this->mSound.init(&a_this->current.pos, &a_this->current.pos, 3, 1);

        static f32 obj_size[7] = {
            0.666f, 0.8f, 0.69999999f, 1.2f, 1.0f, 0.69999999f, 1.2f
        };
        a_this->scale.x = obj_size[i_this->field_0x571];

        if (fopAcM_GetRoomNo(a_this) == 4) {
            i_this->field_0x618.set(-5374.0f, hREG_F(1) + 4520.0f + 400.0f, -2342.0f);
        } else {
            cXyz sp9c, spa8;
            cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
            cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
            sp9c.x = 0.0f;
            if (i_this->field_0x570 == 0) {
                sp9c.y = 5000.0f;
            } else {
                sp9c.y = -5000.0f;
            }
            sp9c.z = 0.0f;

            MtxPosition(&sp9c, &spa8);
            dBgS_LinChk lin_chk;
            cXyz end(a_this->current.pos + spa8);
            lin_chk.Set(&a_this->current.pos, &end, a_this);

            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                sp9c.y *= KREG_F(7) + 0.01f;
                MtxPosition(&sp9c, &spa8);
                i_this->field_0x618 = lin_chk.GetCross() + spa8;
            }
        }
    }

    return phase;
}

static actor_method_class l_daObj_Ito_Method = {
    (process_method_func)daObj_Ito_Create,
    (process_method_func)daObj_Ito_Delete,
    (process_method_func)daObj_Ito_Execute,
    (process_method_func)daObj_Ito_IsDelete,
    (process_method_func)daObj_Ito_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_ITO = {
  fpcLy_CURRENT_e,        // mLayerID
  4,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_ITO,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(obj_ito_class),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  61,                     // mPriority
  &l_daObj_Ito_Method,    // sub_method
  0x00044100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
