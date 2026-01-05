/**
 * @file d_a_obj_gm.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_gm.h"
#include "d/d_cc_d.h"
#include "d/actor/d_a_obj_so.h"
#include "d/d_a_obj.h"
#include "d/d_s_play.h"

enum OBJ_GM_RES_File_ID {
    /* BMDR */
    /* 0x3 */ BMDR_K_KUMO_HASI00 = 0x3,
    /* 0x4 */ BMDR_K_KUMO_IWA00,
    /* 0x5 */ BMDR_K_KUMO_IWA01,
    /* 0x6 */ BMDR_K_KUMO_TUBO00,
    /* 0x7 */ BMDR_K_KUMO_TUBO01,
};

class daObj_Gm_HIO_c : public JORReflexible {
public:
    daObj_Gm_HIO_c();
    virtual ~daObj_Gm_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ s8 id;
};

static f32 obj_size[7] = {
    30.0f, 50.0f, 50.0f, 140.0f, 100.0f, 70.0f, 200.0f,
};

static f32 weg[7] = {
    60.0f, 70.0f, 80.0f, 150.0f, 100.0f, 60.0f, 200.0f,
};

static s16 tim[7] = {
    1500,
    1000,
    1200,
    700,
    800,
    1000,
    700,
};

static f32 spin_sp[7] = {
    200.0f, 100.0f, 200.0f, 66.7f, 80.0f, 0.0f, 0.0f,
};

static s16 neg[7] = {
    300,
    200,
    250,
    120,
    150,
    250,
    150,
};

static f32 obj_size_maji[7] = {
    0.666f, 0.8f, 0.7f, 1.2f, 1.0f, 0.7f, 1.2f,
};

static int gm_obj_bmd[7] = {
    BMDR_K_KUMO_TUBO00, BMDR_K_KUMO_TUBO01, BMDR_K_KUMO_IWA01, BMDR_K_KUMO_IWA00, BMDR_K_KUMO_HASI00, BMDR_K_KUMO_TUBO00, BMDR_K_KUMO_TUBO00,
};

daObj_Gm_HIO_c::daObj_Gm_HIO_c() {
    id = -1;
}

static int daObj_Gm_Draw(obj_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->actor;
    
    if (i_this->mType < 5) {
        g_env_light.settingTevStruct(16, &a_this->current.pos, &a_this->tevStr);
        g_env_light.setLightTevColorType_MAJI(i_this->mModel, &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mModel);

        if (i_this->mAction == 2) {
            dComIfGd_setSimpleShadow(&a_this->current.pos, i_this->mBgc.GetGroundH(), obj_size[i_this->mType], i_this->mBgc.m_gnd,
                                     0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
        }
    }

    return 1;
}

static s16 wall_angle_get(obj_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz spac, spb8, spa0[2];

    cMtx_YrotS(*calc_mtx, i_this->field_0x720);
    spac.x = 0.0f;
    spac.y = 0.0f;
    spac.z = -50.0f;
    MtxPosition(&spac, &spb8);
    spb8 += a_this->current.pos;
    spac.x = 5.0f;
    spac.y = 0.0f;
    spac.z = obj_size[i_this->mType] + 100.0f;

    for (int i = 0; i < 2; i++) {
        MtxPosition(&spac, &spa0[i]);
        spac.x *= -1.0f;
        spa0[i] += spb8;
        lin_chk.Set(&spb8, &spa0[i], a_this);

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            spa0[i] = lin_chk.GetCross();
        } else {
            OS_REPORT("OBJ GM WALL CHECK NON ??\n");
            return 35;
        }
    }

    spac = spa0[1] - spa0[0];
    return cM_atan2s(spac.x, spac.z) + 0x4000;
}

static void ito_pos_set(obj_gm_class* i_this) {
    cXyz pos;
    pos.x = 0.0f;
    pos.z = (JREG_F(9) + 15.0f) * obj_size_maji[i_this->mType];
    MtxPush();

    for (int i = 0; i < 6; i++) {
        cMtx_YrotM(*calc_mtx, 0x2AAA);
        pos.y = obj_size[i_this->mType] * 0.7f;
        MtxPosition(&pos, &i_this->field_0x684[i]);
        pos.y = -obj_size[i_this->mType] * 0.7f;
        MtxPosition(&pos, &i_this->field_0x6cc[i]);
    }

    MtxPull();
}

static void bg_fix(obj_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->actor;
    cXyz sp28, sp34;

    if (i_this->field_0x630 > 0.1f) {
        i_this->field_0x634 = i_this->field_0x630 * cM_ssin(i_this->field_0x640 * tim[i_this->mType]);
        i_this->field_0x638 = i_this->field_0x630 * cM_ssin(i_this->field_0x640 * (tim[i_this->mType] + 100));
        cLib_addCalc0(&i_this->field_0x630, 0.05f, 0.1f);
        i_this->field_0x640++;
    } else {
        i_this->field_0x640 = 0;
    }

    sp28.set(0.0f, 0.0f, weg[i_this->mType]);
    cMtx_XrotS(*calc_mtx, a_this->current.angle.x);
    MtxPosition(&sp28, &sp34);
    MtxTrans(a_this->current.pos.x, a_this->current.pos.y + sp34.y, a_this->current.pos.z, 0);
    cMtx_YrotM(*calc_mtx, a_this->current.angle.y);
    cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
    MtxTrans(i_this->field_0x634, 0.0f, i_this->field_0x638, 1);
    cMtx_YrotM(*calc_mtx, i_this->field_0x64c[0]);
    MtxPush();
    cMtx_YrotM(*calc_mtx, a_this->shape_angle.y);
    cMtx_XrotM(*calc_mtx, a_this->shape_angle.x);
    i_this->mModel->setBaseTRMtx(*calc_mtx);

    if (i_this->mType == 5) {
        if (i_this->field_0x650 != NULL) {
            ((daTbox_c*)(i_this->field_0x650))->setDrawMtx(*calc_mtx);
        }
    } else if (i_this->mType == 6 && i_this->field_0x650 != NULL) {
        obj_so_class* so_p = (obj_so_class*)(i_this->field_0x650);
        so_p->field_0x1b60 = 1;
        MTXCopy(*calc_mtx, so_p->mMtx);
    }
    MtxPull();

    sp28.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp28, &i_this->field_0x714);
    sp34 = i_this->field_0x714;

    if (i_this->field_0x948 != 0) {
        sp34.y -= 12300.0f;
    }

    if (i_this->mType < 6) {
        i_this->mSph.SetC(sp34);
        i_this->mSph.SetR((JREG_F(3) + 1.0f) * obj_size[i_this->mType]);
        dComIfG_Ccsp()->Set(&i_this->mSph);
    }

    a_this->eyePos = i_this->field_0x714;
    a_this->eyePos.y += 40.0f;
    a_this->attention_info.position = a_this->eyePos;

    sp28.set(0.0f, obj_size[i_this->mType] * 0.7f, 0.0f);
    MtxPosition(&sp28, &i_this->field_0x654[0]);
    sp28.set(0.0f, -obj_size[i_this->mType] * 0.7f, 0.0f);
    MtxPosition(&sp28, &i_this->field_0x654[1]);

    ito_pos_set(i_this);

    if (i_this->mSph.ChkTgHit()) {
        i_this->mAtInfo.mpCollider = i_this->mSph.GetTgHitObj();
        at_power_check(&i_this->mAtInfo);

        if (i_this->mType == 5) {
            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                a_this->field_0x567 = 1;
                return;
            }
        } else {
            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT) || i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                i_this->mAtInfo.mAttackPower = 1;
            }

            if (i_this->mAtInfo.mAttackPower != 0 && i_this->mType <= 1) {
                i_this->field_0x72c = 1;
            } else {
                i_this->field_0x948 = 15;
                i_this->field_0x630 = TREG_F(4) + 20.0f;
                i_this->field_0x648 = TREG_F(6) + 10000.0f;
            }
        }
    }

    if ((i_this->field_0xae4 & 4) != 0) {
        i_this->field_0xae4 &= (s8) -5;
        i_this->field_0x630 = TREG_F(14) + 10.0f;
        i_this->field_0x648 = TREG_F(16) + 5000.0f;
    }

    i_this->mSph.OffAtSetBit();

    if (i_this->mActionMode == 0) {
        i_this->mActionMode = 1;
        i_this->field_0x66c[0] = i_this->field_0x654[0];
        i_this->field_0x66c[1] = i_this->field_0x654[1];
    }

    if ((i_this->field_0xae4 & 1) != 0) {
        i_this->field_0xae4 &= (s8) ~1;
        i_this->mAction = 1;
        i_this->mActionMode = 0;
    }

    if ((i_this->field_0xae4 & 2) != 0) {
        if (i_this->mType >= 5) {
            fopAcM_delete(a_this);
        } else {
            i_this->field_0xae4 &= (s8) ~2;
            i_this->mAction = 2;
            i_this->mActionMode = 0;

            if (i_this->mType != 0) {
                i_this->mSph.OnAtSetBit();
            }
            
            i_this->field_0x728 = 1.0f;
        }
    }
}

static void swing(obj_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->actor;
    cXyz sp40, sp4c;

    if (i_this->mSph.ChkTgHit()) {
        if (i_this->mType <= 1) {
            i_this->field_0x72c = 1;
            return;
        }

        if (i_this->field_0x63c < 0x100 && i_this->field_0x63c > -0x100) {
            i_this->field_0x640 = 0;
            i_this->field_0x63c = (KREG_F(15) + 10.0f) * spin_sp[i_this->mType];
            i_this->field_0x644 = fopAcM_searchPlayerAngleY(a_this);
        } else if (i_this->field_0x63e < 0x100 && i_this->field_0x63e > -0x100) {
            s16 angle = i_this->field_0x644 - fopAcM_searchPlayerAngleY(a_this);
            i_this->field_0x642 = 0;
            i_this->field_0x63e = i_this->field_0x63c * cM_ssin(angle);
        }
    }

    if (i_this->field_0x63c > 0x1000 || i_this->field_0x63c < -0x1000) {
        if (i_this->mType != 0) {
            i_this->mSph.OnAtSetBit();
        }
    } else {
        i_this->mSph.OffAtSetBit();
    }

    switch (i_this->mActionMode) {
        case 0:
            sp40 = i_this->field_0x614[0] - i_this->field_0x714;
            i_this->field_0x62c = sp40.abs();
            i_this->mActionMode = 1;
            i_this->field_0x640 = 0x4000;
            i_this->field_0x63c = a_this->current.angle.x;
            i_this->field_0x644 = a_this->current.angle.y;
            a_this->current.angle.y = 0;
            // fallthrough
        case 1:
            i_this->field_0x648 = TREG_F(6) + 10000.0f;
            a_this->current.angle.x = i_this->field_0x63c * cM_ssin(i_this->field_0x640);
            a_this->current.angle.z = i_this->field_0x63e * cM_ssin(i_this->field_0x642);
            sp40.x = i_this->field_0x62c;
            if (sp40.x > 1500.0f) {
                sp40.x = 1500.0f;
            }

            s16 sVar2 = 2000.0f - sp40.x;
            i_this->field_0x640 += sVar2;
            i_this->field_0x642 += sVar2;
            cLib_addCalcAngleS2(&i_this->field_0x63c, 0, 32, YREG_S(3) + 3);
            cLib_addCalcAngleS2(&i_this->field_0x63e, 0, 32, YREG_S(3) + 3);
            MtxTrans(i_this->field_0x614[0].x, i_this->field_0x614[0].y, i_this->field_0x614[0].z, 0);
            cMtx_YrotM(*calc_mtx, i_this->field_0x644);
            cMtx_YrotM(*calc_mtx, a_this->current.angle.y);
            cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
            cMtx_ZrotM(*calc_mtx, a_this->current.angle.z);
            cMtx_YrotM(*calc_mtx, -i_this->field_0x644);
            MtxTrans(0.0f, -i_this->field_0x62c, 0.0f, 1);
            cMtx_YrotM(*calc_mtx, i_this->field_0x64c[0]);
            MtxPush();
            cMtx_YrotM(*calc_mtx, a_this->shape_angle.y);
            cMtx_XrotM(*calc_mtx, a_this->shape_angle.x);
            i_this->mModel->setBaseTRMtx(*calc_mtx);
            MtxPull();

            sp40.set(0.0f, 0.0f, 0.0f);
            sp4c = a_this->current.pos;
            MtxPosition(&sp40, &a_this->current.pos);
            a_this->speed = a_this->current.pos - sp4c;

            if (i_this->mType < 5) {
                i_this->mSph.SetC(a_this->current.pos);
                dComIfG_Ccsp()->Set(&i_this->mSph);
            }

            sp40.set(0.0f, obj_size[i_this->mType], 0.0f);
            MtxPosition(&sp40, &i_this->field_0x654[0]);
            ito_pos_set(i_this);

            a_this->eyePos = a_this->current.pos;
            a_this->eyePos.y += 40.0f;
            a_this->attention_info.position = a_this->eyePos;
            a_this->current.pos.y -= obj_size[i_this->mType];
            a_this->old.pos.y -= obj_size[i_this->mType];
            i_this->mBgc.CrrPos(dComIfG_Bgsp());
            a_this->current.pos.y += obj_size[i_this->mType];
            a_this->old.pos.y += obj_size[i_this->mType];

            if (i_this->mBgc.ChkGroundHit() || i_this->mBgc.ChkWallHit()) {
                if (i_this->mType <= 1) {
                    i_this->field_0x72c = 1;
                } else {
                    i_this->field_0xae4 |= 2;
                    fopAcM_effSmokeSet1(&i_this->field_0xae8, &i_this->field_0xaec, &a_this->current.pos, NULL, (TREG_F(18) + 0.025f) * obj_size[i_this->mType],
                                        &a_this->tevStr, 1);
                }
            }
            break;
    }

    if ((i_this->field_0xae4 & 2) != 0) {
        if (i_this->mType >= 5) {
            fopAcM_delete(a_this);
        } else {
            i_this->field_0xae4 &= ~2;
            i_this->mAction = 2;
            i_this->mActionMode = 0;

            if (i_this->mType != 0) {
                i_this->mSph.OnAtSetBit();
            }

            i_this->field_0x720 = cM_atan2s(a_this->speed.x, a_this->speed.z);
            a_this->speedF = JMAFastSqrt(a_this->speed.x * a_this->speed.x + a_this->speed.z * a_this->speed.z);
            i_this->field_0x728 = 1.0f;
        }
    }
}

static void drop(obj_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->actor;
    cXyz sp38, sp44;

    cMtx_YrotS(*calc_mtx, i_this->field_0x720);
    sp38.x = 0.0f;
    sp38.y = 0.0f;
    sp38.z = a_this->speedF * i_this->field_0x728;
    MtxPosition(&sp38, &sp44);
    a_this->current.pos.x += sp44.x;
    a_this->current.pos.z += sp44.z;
    a_this->current.pos.y += a_this->speed.y;
    a_this->speed.y -= 5.0f;
    a_this->current.pos.y -= obj_size[i_this->mType];
    a_this->old.pos.y -= obj_size[i_this->mType];
    f32 speedY = a_this->speed.y;
    i_this->mBgc.CrrPos(dComIfG_Bgsp());
    a_this->current.pos.y += obj_size[i_this->mType];
    a_this->old.pos.y += obj_size[i_this->mType];

    if (i_this->mBgc.ChkGroundHit()) {
        if (i_this->mType <= 1) {
            i_this->field_0x72c = 1;
        } else {
            if (speedY < -50.0f) {
                a_this->speed.y = speedY * -0.3f;
            }

            if (i_this->mType != 4) {
                cLib_addCalc0(&a_this->speedF, 1.0f, TREG_F(17) + 1.0f);
            } else {
                i_this->field_0x72c = 1;
            }

            if (i_this->field_0x726 == 0) {
                i_this->field_0x726 = 1;
                fopAcM_effSmokeSet1(&i_this->field_0xae8, &i_this->field_0xaec, &a_this->current.pos, NULL, (TREG_F(18) + 0.025f) * obj_size[i_this->mType], &a_this->tevStr, 1);

                if (i_this->mType >= 3) {
                    i_this->mSound.startCreatureSound(Z2SE_EN_AI_HAMMER_HIT, 0, -1);
                    dComIfGp_getVibration().StartShock(YREG_S(2) + 5, 15, cXyz(0.0f, 1.0f, 0.0f));
                }
            }
        }
    } else {
        i_this->mSph.OffAtSetBit();
    }

    if (i_this->mTimers[0] == 0 && i_this->mBgc.ChkWallHit()) {
        i_this->mTimers[0] = 10;
        s16 wallAngle = wall_angle_get(i_this);
        if (a_this->speedF > 5.0f && wallAngle != 35) {
            i_this->field_0x720 += (s16)(0x8000 - ((s16)(i_this->field_0x720 - wallAngle) << 1));
            i_this->field_0x722 *= -1;
            i_this->mTimers[0] = 10;
            a_this->speedF *= AREG_F(4) + 0.35f;
        } else {
            a_this->speedF *= AREG_F(5) + 0.9f;
        }
    }

    if (i_this->field_0x726 != 0) {
        i_this->field_0x722 += (int)(a_this->speedF * i_this->field_0x728 * spin_sp[i_this->mType]);
    }

    MtxTrans(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z, 0);
    cMtx_YrotM(*calc_mtx, i_this->field_0x720);
    cMtx_XrotM(*calc_mtx, i_this->field_0x722);
    cMtx_YrotM(*calc_mtx, -i_this->field_0x720);
    cMtx_YrotM(*calc_mtx, a_this->current.angle.y);
    cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
    cMtx_YrotM(*calc_mtx, i_this->field_0x64c[0]);

    sp38.set(0.0f, obj_size[i_this->mType], 0.0f);
    MtxPosition(&sp38, &i_this->field_0x654[0]);
    sp38.set(0.0f, -obj_size[i_this->mType], 0.0f);
    MtxPosition(&sp38, &i_this->field_0x654[1]);

    ito_pos_set(i_this);

    cMtx_YrotM(*calc_mtx, a_this->shape_angle.y);
    cMtx_XrotM(*calc_mtx, a_this->shape_angle.x);
    i_this->mModel->setBaseTRMtx(*calc_mtx);
    i_this->mSph.SetC(a_this->current.pos);
    dComIfG_Ccsp()->Set(&i_this->mSph);
    a_this->eyePos = a_this->current.pos;
    a_this->eyePos.y += 40.0f;
    a_this->attention_info.position = a_this->eyePos;
}

static void action(obj_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->actor;
    s8 bVar1 = 1;

    switch (i_this->mAction) {
        case 0:
            bg_fix(i_this);
            break;
        
        case 1:
            swing(i_this);
            break;

        case 2:
            drop(i_this);
            bVar1 = 0;
            break;
    }

    if (bVar1) {
        s16 target = i_this->field_0x648 * cM_ssin(i_this->field_0x608 * neg[i_this->mType]);
        cLib_addCalc2(&i_this->field_0x648, NREG_F(8) + 2000.0f, 0.05f, TREG_F(5) + 10.0f);
        cLib_addCalcAngleS2(&i_this->field_0x64c[0], target, 16, 200);
    }

    if (i_this->mSph.ChkTgHit()) {
        if (i_this->mType >= 2) {
            i_this->field_0x948 = 15;
            def_se_set(&i_this->mSound, i_this->mSph.GetTgHitObj(), 35, NULL);
        }

        fopAc_ac_c* actor_p = dCc_GetAc(i_this->mSph.GetTgHitObj()->GetAc());
        if (actor_p != NULL && fopAcM_GetName(actor_p) == PROC_OBJ_GM) {
            i_this->field_0xae4 |= 2;
        }
    }

    fopAcM_OffStatus(a_this, 0);
    a_this->attention_info.flags = 0;
}

static void* s_tbox_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_TBOX) {
        f32 fVar1 = ((fopAc_ac_c*)i_actor)->current.pos.x - ((fopAc_ac_c*)i_data)->current.pos.x;
        f32 fVar2 = ((fopAc_ac_c*)i_actor)->current.pos.z - ((fopAc_ac_c*)i_data)->current.pos.z;
        if (fVar1 * fVar1 + fVar2 * fVar2 < 40000.0f) {
            return i_actor;
        }
    }

    return NULL;
}

static void* s_so_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_SO) {
        return i_actor;
    }

    return NULL;
}


static int daObj_Gm_Execute(obj_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->actor;

    if (i_this->field_0x650 == NULL) {
        if (i_this->mType == 5) {
            i_this->field_0x650 = fpcM_Search(s_tbox_sub, i_this);
        } else if (i_this->mType == 6) {
            i_this->field_0x650 = fpcM_Search(s_so_sub, i_this);
        }
    }

    i_this->field_0x608++;

    if (i_this->field_0x948 != 0) {
        i_this->field_0x948--;
    }

    for (int i = 0; i < 2; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    action(i_this);

    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));

    if (i_this->field_0x72c != 0) {
        if (i_this->mSwBit != 0) {
            dComIfGs_onSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this));
        }

        if (i_this->mType == 0) {
            daObj::make_eff_break_gm_kotubo(a_this);
            i_this->mSound.startCreatureSound(JA_SE_OBJ_BREAK_CHINA_S, 0, -1);

            if ((fopAcM_GetParam(a_this) & 0xFF) == 32) {
                fopAcM_createItemForMidBoss(&a_this->current.pos, fpcNm_ITEM_SMALL_KEY, fopAcM_GetRoomNo(a_this), NULL, NULL, 0, fopAcM_GetParam(a_this) >> 24);
            }
        } else if (i_this->mType == 1) {
            daObj::make_eff_break_gm_ootubo(a_this);
            i_this->mSound.startCreatureSound(JA_SE_OBJ_BREAK_CHINA_L, 0, -1);
        } else {
            fopAcM_createDisappear(a_this, &a_this->current.pos, obj_size[i_this->mType] * 0.2f, 0, a_this->field_0x564);
        }

        fopAcM_createItemFromTable(&a_this->current.pos, a_this->field_0x565, -1, fopAcM_GetRoomNo(a_this), &a_this->current.angle, 0, NULL, NULL, NULL, false);
        fopAcM_delete(a_this);
    }

    return 1;
}

static int daObj_Gm_IsDelete(obj_gm_class* i_this) {
    return 1;
}

static bool hio_set;

static int daObj_Gm_Delete(obj_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->actor;

    fopAcM_GetID(a_this);
    dComIfG_resDelete(&i_this->mPhase, "OBJ_GM");

    if (i_this->mIsFirstSpawn) {
        hio_set = false;
    }

    if (a_this->heap != NULL) {
        i_this->mSound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    obj_gm_class* i_this = (obj_gm_class*)a_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("OBJ_GM", gm_obj_bmd[i_this->mType]);
    JUT_ASSERT(1141, modelData != NULL);

    i_this->mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->mModel == NULL) {
        return 0;
    }

    return 1;
}

static daObj_Gm_HIO_c l_HIO;

static cPhs__Step daObj_Gm_Create(fopAc_ac_c* a_this) {
    fopAcM_ct(a_this, obj_gm_class);
    obj_gm_class* i_this = (obj_gm_class*)a_this;

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->mPhase, "OBJ_GM");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_GM PARAM %x\n", fopAcM_GetParam(a_this));
        a_this->field_0x565 = fopAcM_GetParam(a_this);
        i_this->mType = (fopAcM_GetParam(a_this) >> 8) & 15;
        if (i_this->mType == 15) {
            i_this->mType = 0;
        }

        i_this->field_0x571 = (fopAcM_GetParam(a_this) & 0xF000) >> 12;
        i_this->mSwBit = a_this->current.angle.z;
        a_this->shape_angle.z = 0;
        a_this->current.angle.z = 0;

        if (i_this->mSwBit != 0) {
            if (dComIfGs_isSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this))) {
                OS_REPORT("OBJ_GM 破壊後なので再セットしません\n");
                return cPhs_ERROR_e;
            }
        }

        OS_REPORT("OBJ_GM//////////////OBJ_GM SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x1000)) {
            OS_REPORT("//////////////OBJ_GM SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_GM SET 2 !!\n");

        if (!hio_set) {
            i_this->mIsFirstSpawn = true;
            hio_set = true;
            l_HIO.id = -1;
        }

        fopAcM_SetMtx(a_this, i_this->mModel->getBaseTRMtx());
        fopAcM_SetMin(a_this, -300.0f, -300.0f, -300.0f);
        fopAcM_SetMax(a_this, 300.0f, 300.0f, 300.0f);
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        i_this->mStts.Init(0xFF, 0, a_this);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0x1f}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x2, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_METAL, 0x2, 0x0, 0x0, 0x3}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        i_this->mSph.Set(cc_sph_src);
        i_this->mSph.SetStts(&i_this->mStts);
        i_this->mSound.init(&a_this->current.pos, &a_this->current.pos, 3, 1);

        if (i_this->mType < 5) {
            a_this->shape_angle.y = cM_rndF(65536.0f);
            a_this->shape_angle.x = cM_rndFX(10000.0f);
        }

        i_this->mBgc.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1, &i_this->mAcchCir, fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(50.0f, obj_size[i_this->mType]);
        
        i_this->field_0x608 = cM_rndF(65536.0f);

        daObj_Gm_Execute(i_this);

        fopAcM_createChild(PROC_OBJ_ITO, fopAcM_GetID(a_this), i_this->mType << 1, &a_this->current.pos,
                           fopAcM_GetRoomNo(a_this), &a_this->current.angle, NULL, -1, NULL);

        if (i_this->field_0x571 == 1) {
            fopAcM_createChild(PROC_OBJ_ITO, fopAcM_GetID(a_this), (i_this->mType << 1) | 1, &a_this->current.pos,
                            fopAcM_GetRoomNo(a_this), &a_this->current.angle, NULL, -1, NULL);
        } else {
            a_this->current.angle.x = 0;
        }
    }

    return phase;
}

static actor_method_class l_daObj_Gm_Method = {
    (process_method_func)daObj_Gm_Create,
    (process_method_func)daObj_Gm_Delete,
    (process_method_func)daObj_Gm_Execute,
    (process_method_func)daObj_Gm_IsDelete,
    (process_method_func)daObj_Gm_Draw,
};

actor_process_profile_definition g_profile_OBJ_GM = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_GM,            // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(obj_gm_class),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  57,                     // mPriority
  &l_daObj_Gm_Method,     // sub_method
  0x00044100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
