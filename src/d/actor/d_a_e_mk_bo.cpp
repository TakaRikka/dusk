/**
 * @file d_a_e_mk_bo.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_mk_bo.h"
#include "d/actor/d_a_e_mk.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"
#include "c/c_damagereaction.h"
#include "Z2AudioLib/Z2Instances.h"

static int daE_MK_BO_Draw(e_mk_bo_class* i_this) {
    if (i_this->field_0x9b4 != 0) {
        return 1;
    }

    fopAc_ac_c* actor = &i_this->enemy;

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->model, &actor->tevStr);

    if (i_this->action >= 2) {
        i_this->model->getModelData()->getMaterialNodePointer(1)->getTevColor(1)->r = 25;
        i_this->model->getModelData()->getMaterialNodePointer(1)->getTevColor(1)->g = 35;
        i_this->model->getModelData()->getMaterialNodePointer(1)->getTevColor(1)->b = 10;
    }

    mDoExt_modelUpdateDL(i_this->model);

    if (i_this->field_0x9b5 != 0) {
        J3DModel* model = i_this->efModelMorf->getModel();
        i_this->pbtk->entry(model->getModelData());
        i_this->efModelMorf->entryDL();
    }

    return 1;
}

static void hit_check(e_mk_bo_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    fopAc_ac_c* a_parent = (fopAc_ac_c*)fopAcM_SearchByID(actor->parentActorID);
    e_mk_class* e_mk = (e_mk_class*)a_parent;
    cXyz sp24;

    if (i_this->ccAtSph.ChkTgHit() || i_this->acch.ChkGroundHit() || i_this->acch.ChkWallHit() || (e_mk != NULL && e_mk->demoMode == e_mk_class::DEMO_MODE_START && i_this->ccAtSph.ChkAtHit()) || i_this->ccAtSph.ChkAtShieldHit()) {
        i_this->field_0x5f8 = 20;
        OS_REPORT("E_MK_BO HIT \n");

        if (i_this->ccAtSph.ChkTgHit()) {
            i_this->sound.startSound(Z2SE_COL_FLIP_BOOM, 0, -1);
        }

        if (i_this->ccAtSph.ChkAtHit()) {
            cCcD_Obj* at_hit_obj = i_this->ccAtSph.GetAtHitObj();
            fopAc_ac_c* at_hit_actor = dCc_GetAc(at_hit_obj->GetAc());
            if (at_hit_actor != NULL && fopAcM_GetName(at_hit_actor) == PROC_ALINK && daPy_getPlayerActorClass()->checkPlayerGuard()) {
                dComIfGp_getVibration().StartShock(4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            }
        }

        if (i_this->field_0x998 == 1) {
            i_this->field_0x998 = 2;
        }

        if (i_this->mode <= 2) {
            i_this->mode = 3;
            i_this->field_0x5e0 = a_parent->current.pos;
            i_this->field_0x5e0.y += 100.0f;

            sp24 = i_this->field_0x5e0 - actor->current.pos;
            actor->current.angle.y = cM_atan2s(sp24.x, sp24.z);
            actor->current.angle.x = -cM_atan2s(sp24.y, JMAFastSqrt(SQUARE(sp24.x) + SQUARE(sp24.z)));

            if (i_this->acch.ChkGroundHit()) {
                fopAcM_effSmokeSet2(&i_this->field_0x990, &i_this->field_0x994, &actor->current.pos, &actor->current.angle, 3.0f + NREG_F(18), &actor->tevStr);
            }
        }
    }
}

static s8 e_mk_bo_shot(e_mk_bo_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* a_parent = (fopAc_ac_c*)fopAcM_SearchByID(actor->parentActorID);

    s8 rt = 1;
    if (a_parent == NULL) {
        return rt;
    }

    e_mk_class* e_mk = (e_mk_class*)a_parent;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz sp3C;

    switch (i_this->mode) {
    case 0:
        if (e_mk->db != NULL) {
            i_this->field_0x5e0 = e_mk->db->enemy.current.pos;
            i_this->field_0x5e0.y += 100.0f;
        } else {
            i_this->field_0x5e0 = player->current.pos;
            i_this->field_0x5e0.y += 100.0f;
        }

        i_this->mode = 1;

        sp3C = i_this->field_0x5e0 - actor->current.pos;
        actor->current.angle.y = cM_atan2s(sp3C.x, sp3C.z);
        i_this->field_0x5fa = actor->current.angle.y;
        actor->current.angle.x = -cM_atan2s(sp3C.y, JMAFastSqrt(SQUARE(sp3C.x) + SQUARE(sp3C.z)));
        i_this->field_0x5ee = 0x400;
        break;
    case 1:
        sp3C = i_this->field_0x5e0 - actor->current.pos;
        if (sp3C.abs() < 300.0f + TREG_F(18)) {
            i_this->mode = 2;
            i_this->timers[0] = 30;
        }
        break;
    case 2:
    case 3:
        if (i_this->timers[0] == 0) {
            rt = 0;
        }

        i_this->field_0x5e0 = a_parent->current.pos;
        i_this->field_0x5e0.y += 160.0f;

        sp3C = i_this->field_0x5e0 - actor->current.pos;
        cLib_addCalcAngleS2(&actor->current.angle.y, cM_atan2s(sp3C.x, sp3C.z), 1, i_this->field_0x5ec);
        cLib_addCalcAngleS2(&actor->current.angle.x, -cM_atan2s(sp3C.y, JMAFastSqrt(SQUARE(sp3C.x) + SQUARE(sp3C.z))), 1, i_this->field_0x5ec);
        cLib_addCalcAngleS2(&i_this->field_0x5ec, i_this->field_0x5ee, 1, 0x40);
        cLib_addCalcAngleS2(&i_this->field_0x5ee, 0x2000, 1, 4);

        f32 temp_f31 = sp3C.abs();
        if (e_mk->demoSubMode != 0 || (e_mk->action == e_mk_class::ACT_SHOOT && e_mk->mode >= 20)) {
            if (temp_f31 < 350.0f + YREG_F(18)) {
                i_this->field_0x600 = 1;
                e_mk->unkFlag4 = 3;
            } else if (temp_f31 < 600.0f + YREG_F(17)) {
                e_mk->unkFlag4 = 2;
            }
        } else if (e_mk->action == e_mk_class::ACT_YORO && temp_f31 < 120.0f + YREG_F(19)) {
            e_mk->unkFlag4 = 4;
            i_this->action = 1;
            i_this->mode = 0;
            dComIfGp_setHitMark(3, actor, &actor->current.pos, &actor->shape_angle, NULL, 0);
            actor->speed.y = 30.0f;
            i_this->timers[0] = 40;
            return rt;
        }
    }

    actor->speedF = 40.0f + NREG_F(0);
    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    cMtx_XrotM(*calc_mtx, actor->current.angle.x);
    sp3C.x = 0.0f;
    sp3C.y = 0.0f;
    sp3C.z = actor->speedF;
    MtxPosition(&sp3C, &actor->speed);
    actor->current.pos += actor->speed;
    actor->shape_angle.z = -(TREG_S(2) + 0x1800);

    if (i_this->ccAtSph.ChkAtHit() && !i_this->ccAtSph.ChkAtShieldHit()) {
        OS_REPORT("E_MK_BO ANY HIT !!!!\n");

        cCcD_Obj* at_hit_obj = i_this->ccAtSph.GetAtHitObj();
        if (fopAcM_GetName(dCc_GetAc(at_hit_obj->GetAc())) == PROC_ALINK) {
            OS_REPORT("E_MK_BO PL HIT !!!!\n");
            e_mk->unkFlag4 = 5;
            i_this->field_0x5f8 = 10;
        }
    }

    cLib_addCalc2(&i_this->field_0x5f0, 50.0f + BREG_F(2), 1.0f, 5.0f);
    return rt;
}

static s8 e_mk_bo_start(e_mk_bo_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* a_parent = fopAcM_SearchByID(actor->parentActorID);

    s8 rt = 2;
    if (a_parent == NULL) {
        return rt;
    }

    e_mk_class* e_mk = (e_mk_class*) a_parent;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp38;

    f32 temp_f31;
    switch (i_this->mode) {
    case 0:
        i_this->field_0x998 = 1;
        i_this->field_0x5e0.set(0.0f, 5000.0f + JREG_F(18), -3900.0f);
        i_this->mode = 1;

        sp38 = i_this->field_0x5e0 - actor->current.pos;
        actor->current.angle.y = cM_atan2s(sp38.x, sp38.z);
        i_this->field_0x5fa = actor->current.angle.y;
        actor->current.angle.x = -cM_atan2s(sp38.y, JMAFastSqrt(SQUARE(sp38.x) + SQUARE(sp38.z)));
        i_this->field_0x5ee = 0x400;
        break;
    case 1:
        sp38 = i_this->field_0x5e0 - actor->current.pos;
        if (sp38.abs() < 200.0f + TREG_F(18)) {
            i_this->mode = 2;
            i_this->timers[0] = 30;
        }
        break;
    case 2:
    case 3:
        if (i_this->timers[0] == 0) {
            rt = 0;
        }

        i_this->field_0x5e0 = a_parent->current.pos;
        i_this->field_0x5e0.y += 160.0f;

        sp38 = i_this->field_0x5e0 - actor->current.pos;
        cLib_addCalcAngleS2(&actor->current.angle.y, cM_atan2s(sp38.x, sp38.z), 1, i_this->field_0x5ec);
        cLib_addCalcAngleS2(&actor->current.angle.x, -cM_atan2s(sp38.y, JMAFastSqrt(SQUARE(sp38.x) + SQUARE(sp38.z))), 1, i_this->field_0x5ec);
        cLib_addCalcAngleS2(&i_this->field_0x5ec, i_this->field_0x5ee, 1, 0x40);
        cLib_addCalcAngleS2(&i_this->field_0x5ee, 0x2000, 1, 4);

        temp_f31 = sp38.abs();
        if (e_mk->demoSubMode != 0 || (e_mk->action == e_mk_class::ACT_SHOOT && e_mk->mode >= 20)) {
            if (temp_f31 < 350.0f + YREG_F(18)) {
                i_this->field_0x600 = 1;
                e_mk->unkFlag4 = 3;
            } else if (temp_f31 < 600.0f + YREG_F(17)) {
                e_mk->unkFlag4 = 2;
            }
        }
        break;
    }

    actor->speedF = 40.0f + NREG_F(0);
    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    cMtx_XrotM(*calc_mtx, actor->current.angle.x);
    sp38.x = 0.0f;
    sp38.y = 0.0f;
    sp38.z = actor->speedF;
    MtxPosition(&sp38, &actor->speed);
    actor->current.pos += actor->speed;
    actor->shape_angle.z = -(TREG_S(2) + 0x1800);

    cLib_addCalc2(&i_this->field_0x5f0, 50.0f + BREG_F(2), 1.0f, 5.0f);
    return rt;
}

static s8 e_mk_bo_r04(e_mk_bo_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* a_parent = fopAcM_SearchByID(actor->parentActorID);

    s8 rt = 2;
    if (a_parent == NULL) {
        return rt;
    }

    e_mk_class* e_mk = (e_mk_class*) a_parent;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp1C;

    f32 temp_f31;
    switch (i_this->mode) {
    case 0:
        i_this->field_0x998 = 1;
        i_this->mode = 1;

        actor->current.angle.y = a_parent->shape_angle.y + (VREG_S(7) - 0x3000);
        i_this->field_0x5fa = actor->current.angle.y;
        i_this->timers[0] = JREG_S(7) + 65;
        i_this->timers[1] = 30;
        break;
    case 1:
        if (i_this->timers[0] != 0) {
            actor->current.angle.y += i_this->field_0x5ee;
            i_this->field_0x5ee = JREG_S(9) + 350;
            actor->current.angle.x = JREG_S(2) + 1700;
        } else {
            i_this->field_0x5e0 = a_parent->current.pos;
            i_this->field_0x5e0.y += 160.0f;

            sp1C = i_this->field_0x5e0 - actor->current.pos;
            cLib_addCalcAngleS2(&actor->current.angle.y, cM_atan2s(sp1C.x, sp1C.z), 1, i_this->field_0x5ec);
            cLib_addCalcAngleS2(&actor->current.angle.x, -cM_atan2s(sp1C.y, JMAFastSqrt(SQUARE(sp1C.x) + SQUARE(sp1C.z))), 1, i_this->field_0x5ec);
            cLib_addCalcAngleS2(&i_this->field_0x5ec, i_this->field_0x5ee, 1, 0x40);
            cLib_addCalcAngleS2(&i_this->field_0x5ee, 0x2000, 1, JREG_S(8) + 4);

            temp_f31 = sp1C.abs();
            if (i_this->timers[1] == 0 && temp_f31 < 350.0f + YREG_F(18)) {
                i_this->field_0x600 = 1;
                e_mk->unkFlag4 = 3;
            } else if (temp_f31 < 600.0f + YREG_F(17)) {
                e_mk->unkFlag4 = 2;
            }
        }
        break;
    }

    actor->speedF = 40.0f + NREG_F(0);
    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    cMtx_XrotM(*calc_mtx, actor->current.angle.x);
    sp1C.x = 0.0f;
    sp1C.y = 0.0f;
    sp1C.z = actor->speedF;
    MtxPosition(&sp1C, &actor->speed);
    actor->current.pos += actor->speed;
    actor->shape_angle.z = -(TREG_S(2) + 0x1800);

    cLib_addCalc2(&i_this->field_0x5f0, 50.0f + BREG_F(2), 1.0f, 5.0f);
    return rt;
}

static void e_mk_bo_hasira(e_mk_bo_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* a_parent = fopAcM_SearchByID(actor->parentActorID);

    if (a_parent != NULL) {
        e_mk_class* e_mk = (e_mk_class*)a_parent;
        if (e_mk->demoMode == e_mk_class::DEMO_MODE_END) {
            i_this->action = e_mk_class::ACT_SHOOT;
            i_this->field_0x998 = 1;
            return;
        }

        cXyz sp24;
        cXyz sp18;
        if (e_mk->hasira != NULL) {
            daPillar_c* pillar = e_mk->hasira;
            cXyz spC;

            mDoMtx_stack_c::transS(e_mk->hasira->current.pos.x, e_mk->hasira->current.pos.y, e_mk->hasira->current.pos.z);
            mDoMtx_stack_c::YrotM(pillar->mRotY);
            mDoMtx_stack_c::XrotM(e_mk->hasira->shape_angle.x);
            mDoMtx_stack_c::YrotM(-pillar->mRotY);
            mDoMtx_stack_c::transM(0.0f, 500.0f, 0.0f);
            mDoMtx_multVecZero(mDoMtx_stack_c::get(), &spC);

            mDoMtx_stack_c::YrotS(actor->current.angle.y + i_this->field_0x5fa);

            sp24.x = 0.0f;
            sp24.y = 0.0f;
            sp24.z = i_this->field_0x5fc;
            mDoMtx_stack_c::multVec(&sp24, &sp18);

            cLib_addCalc2(&actor->current.pos.x, spC.x + sp18.x, 1.0f, 50.0f);
            cLib_addCalc2(&actor->current.pos.z, spC.z + sp18.z, 1.0f, 50.0f);
            actor->current.pos.y += actor->speed.y;
            actor->speed.y -= 5.0f;

            if (actor->current.pos.y <= 7.0f + spC.y) {
                actor->current.pos.y = 7.0f + spC.y;
                if (i_this->mode <= 3) {
                    actor->speed.y *= -0.4f;
                } else {
                    actor->speed.y = 0.0f;
                }

                if (i_this->mode <= 1) {
                    i_this->timers[0] = 20;
                }

                i_this->mode++;
            }

            if (pillar->checkRollAttack()) {
                i_this->timers[0] = 20;
            }
        }

        if (i_this->timers[0] != 0) {
            i_this->field_0x5fc += 0.1f + AREG_F(19);
        }

        actor->shape_angle.x = (200.0f + KREG_F(2)) * (i_this->timers[0] * cM_ssin(i_this->timers[0] * 12000));
        actor->shape_angle.z = 0;
        cLib_addCalc2(&i_this->field_0x5f0, 10.0f + BREG_F(3), 1.0f, 5.0f);
    }
}

static void e_mk_bo_demo_ground(e_mk_bo_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (actor->health != 0) {
        actor->health = 0;
        i_this->action = 3;
        i_this->mode = 0;
        actor->home.pos.y = actor->current.pos.y;
    }

    fopAc_ac_c* a_parent = fopAcM_SearchByID(actor->parentActorID);
    e_mk_class* e_mk = (e_mk_class*)a_parent;
}

static void e_mk_bo_demo_spin(e_mk_bo_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp34;
    cXyz sp28;
    f32 var_f31 = 0.0f;

    switch (i_this->mode) {
    case 4:
        break;
    case 0:
        actor->speed.y = 0.0f;
        i_this->mode = 1;
        actor->home.pos = actor->current.pos;
        i_this->timers[0] = 20;
        actor->speedF = 0.0f;
        /* fallthrough */
    case 1:
        i_this->sound.startLevelSound(Z2SE_BOOM_GET_DEMO_LOOP1, 0, -1);
        if (i_this->timers[0] == 1) {
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_PEEL_OFF_LEAVES, &actor->current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)), 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }

        if (i_this->timers[0] == 0) {
            actor->field_0x560 = 1;
        }

        if (i_this->timers[0] <= 10 && (i_this->counter & 15) == 0) {
            f32 temp_f30 = 1.0f;
            static cXyz sc(temp_f30, temp_f30, temp_f30);
            dComIfGp_particle_set(dPa_RM(ID_ZI_S_OTIBA_A), &actor->home.pos, &actor->tevStr, &actor->shape_angle, &sc);
        }

        actor->shape_angle.y += i_this->field_0x602;
        cLib_addCalcAngleS2(&i_this->field_0x602, 0x2000, 2, (ZREG_S(0) + 0x200));
        cLib_addCalc2(&actor->current.pos.y, 200.0f + (actor->home.pos.y + ZREG_F(1)), 0.1f, actor->speed.y);
        cLib_addCalc2(&actor->speed.y, 20.0f + ZREG_F(2), 1.0f, 0.5f + ZREG_F(3));

        if (actor->health != 0) {
            actor->health = 0;
            actor->speedF = 0.0f;
            actor->current.angle.y = fopAcM_searchPlayerAngleY(actor) + 0x8000;
            i_this->field_0x5ec = 0;
            i_this->timers[0] = 40;
            i_this->mode = 2;
        }
        break;
    case 2: 
        i_this->sound.startLevelSound(Z2SE_BOOM_GET_DEMO_LOOP2, 0, -1);

        var_f31 = 40.0f;
        actor->shape_angle.y += i_this->field_0x602;

        if (i_this->timers[0] == 0) {
            actor->current.angle.y += i_this->field_0x5ec;
            cLib_addCalcAngleS2(&i_this->field_0x5ec, 0x400, 1, 0x100);
            
            s16 sp8 = (actor->current.angle.y - fopAcM_searchPlayerAngleY(actor));
            if (sp8 <= 0x400 && sp8 > -0x400) {
                i_this->mode = 3;
            }

            cLib_addCalcAngleS2(&actor->shape_angle.z, -0x1200, 2, 0x200);
        }
        break;
    case 3:
        i_this->sound.startLevelSound(Z2SE_BOOM_GET_DEMO_LOOP2, 0, -1);

        var_f31 = 40.0f;
        actor->shape_angle.y += i_this->field_0x602;

        cLib_addCalcAngleS2(&actor->current.angle.y, fopAcM_searchPlayerAngleY(actor), 1, 0x400);
        cLib_addCalc2(&actor->current.pos.y, 170.0f + (actor->home.pos.y + ZREG_F(1)), 0.1f,5.0f);

        if (fopAcM_searchPlayerDistanceXZ(actor) < 50.0f) {
            e_mk_class* e_mk = (e_mk_class*)fopAcM_SearchByID(actor->parentActorID);
            e_mk->demoSubMode++;
            i_this->mode = 4;
            i_this->field_0x600 = 1;
        }
        break;
    }

    cLib_addCalc2(&actor->speedF, var_f31, 1.0f, 1.0f);
    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    sp34.x = 0.0f;
    sp34.y = 0.0f;
    sp34.z = actor->speedF;
    MtxPosition(&sp34, &sp28);
    actor->current.pos += sp28;

    if (var_f31 > 0.0f && (i_this->counter & 7) == 0) {
        i_this->sound.startSound(Z2SE_EN_MK_BOOM_FLY, 0, -1);
    }
}

static void action(e_mk_bo_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp24;
    cXyz sp18;

    fopAc_ac_c* a_parent = fopAcM_SearchByID(actor->parentActorID);
    e_mk_class* e_mk = (e_mk_class*)a_parent;
    s8 var_r28;
    u32 sound = Z2SE_MK_DARK_BOOMERANG;

    switch (i_this->action) {
    case 0:
        if (e_mk->demoMode == e_mk_class::DEMO_MODE_START) {
            var_r28 = e_mk_bo_start(i_this);
        } else if (e_mk->demoMode == e_mk_class::DEMO_MODE_R04 || fopAcM_GetRoomNo(actor) == 4) {
            var_r28 = e_mk_bo_r04(i_this);
        } else {
            var_r28 = e_mk_bo_shot(i_this);
            sound = Z2SE_EN_MK_BOOM_FLY;
        }

        actor->shape_angle.y += (s16) 0x2000;

        if ((i_this->counter & 7) == 0) {
            i_this->sound.startSound(sound, 0, -1);
        }

        if (var_r28 == 1) {
            i_this->acch.CrrPos(dComIfG_Bgsp());
            hit_check(i_this);
        } else if (var_r28 == 0) {
            i_this->field_0x5f8 = 10;
        }
        break;
    case 1:
        e_mk_bo_hasira(i_this);
        i_this->field_0x5f8 = 10;
        break;
    case 2:
        e_mk_bo_demo_ground(i_this);
        i_this->field_0x5f8 = 10;
        break;
    case 3:
        e_mk_bo_demo_spin(i_this);
        i_this->field_0x5f8 = 10;
        i_this->field_0x9b5 = 1;
        break;
    }

    cXyz spC(actor->current.pos);
    if (i_this->field_0x5f8 != 0) {
        spC.y += 10000.0f;
    }

    i_this->ccAtSph.SetC(spC);
    dComIfG_Ccsp()->Set(&i_this->ccAtSph);
}

static int daE_MK_BO_Execute(e_mk_bo_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz sp64;
    cXyz sp58;
    cXyz sp4C;

    if (cDmrNowMidnaTalk()) {
        return 1;
    }

    if (i_this->field_0x9b4 != 0) {
        return 1;
    }

    if (i_this->field_0x600 != 0) {
        fopAcM_delete(actor);
#if VERSION == VERSION_GCN_JPN
        return 1;
#endif
    }

    i_this->counter++;

    for (int i = 0; i < 2; i++) {
        if (i_this->timers[i] != 0) {
            i_this->timers[i]--;
        }
    }

    if (i_this->field_0x5f8 != 0) {
        i_this->field_0x5f8--;
    }

    action(i_this);
    cLib_addCalcAngleS2(&i_this->field_0x5fa, actor->current.angle.y, 4, 0x1000);

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(s16(i_this->field_0x5fa));
    mDoMtx_stack_c::ZrotM(s16(actor->shape_angle.z));
    mDoMtx_stack_c::YrotM(s16(actor->shape_angle.y));
    mDoMtx_stack_c::XrotM(s16(actor->shape_angle.x));

    fopAc_ac_c* a_parent = fopAcM_SearchByID(actor->parentActorID);
    if (a_parent == NULL) {
        return 1;
    }

    f32 size = a_parent->scale.x;
    mDoMtx_stack_c::scaleM(size, size, size);
    mDoMtx_stack_c::transM(0.0f, 0.0f, i_this->field_0x5f0);
    i_this->model->setBaseTRMtx(mDoMtx_stack_c::get());

    e_mk_class* e_mk = (e_mk_class*)a_parent;
    if (e_mk->unkFlag4 == 6) {
        e_mk->unkFlag4 = 0;
        fopAcM_delete(actor);
    }

    actor->eyePos = actor->current.pos;
    i_this->sound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));

    if (i_this->field_0x998 != 0) {
        JPABaseEmitter* emt;
        int eff_num = 4;
        if (i_this->action == 2) {
            eff_num = 2;
        }

        static u16 bo_eno_1[] = {
            dPa_RM(ID_ZI_S_MKDEMO_BOOM_WAIT_A),
            dPa_RM(ID_ZI_S_MKDEMO_BOOM_WAIT_B),
            dPa_RM(ID_ZI_S_MKDEMO_BOOM_KISEKI_A),
            dPa_RM(ID_ZI_S_MKDEMO_BOOM_KISEKI_B),
        };

        static u16 bo_eno_2[] = {
            dPa_RM(ID_ZI_S_MKDEMO_BOOM2_WAIT_A),
            dPa_RM(ID_ZI_S_MKDEMO_BOOM2_WAIT_B),
            dPa_RM(ID_ZI_S_MKDEMO_BOOM2_KISEKI_A),
            dPa_RM(ID_ZI_S_MKDEMO_BOOM2_KISEKI_B),
        };

        static u16 bo_eno_12[] = {
            dPa_RM(ID_ZI_S_MKDEMO_BOOM_HIT_A),
            dPa_RM(ID_ZI_S_MKDEMO_BOOM_HIT_B),
        };

        for (int i = 0; i < eff_num; i++) {
            if (i_this->action >= 2) {
                if (i < 2) {
                    e_mk->boEno1Prtcls[i] = dComIfGp_particle_set(e_mk->boEno1Prtcls[i], bo_eno_2[i], &actor->current.pos, NULL, NULL);
                    emt = dComIfGp_particle_getEmitter(e_mk->boEno1Prtcls[i]);
                } else {
                    i_this->field_0x99c[i - 2] = dComIfGp_particle_set(i_this->field_0x99c[i - 2], bo_eno_2[i], &actor->current.pos, NULL, NULL);
                    emt = dComIfGp_particle_getEmitter(i_this->field_0x99c[i - 2]);
                }
            } else if (i < 2) {
                e_mk->boEno1Prtcls[i] = dComIfGp_particle_set(e_mk->boEno1Prtcls[i], bo_eno_1[i], &actor->current.pos, NULL, NULL);
                emt = dComIfGp_particle_getEmitter(e_mk->boEno1Prtcls[i]);
            } else {
                i_this->field_0x99c[i - 2] = dComIfGp_particle_set(i_this->field_0x99c[i - 2], bo_eno_1[i], &actor->current.pos, NULL, NULL);
                emt = dComIfGp_particle_getEmitter(i_this->field_0x99c[i - 2]);
            }

            if (emt != NULL) {
                MTXCopy(i_this->model->getBaseTRMtx(), mDoMtx_stack_c::get());

                if (i_this->field_0x998 == 2) {
                    mDoMtx_stack_c::transM(10000.0f, 10000.0f, 100000.0f);
                }

                emt->setGlobalSRTMatrix(mDoMtx_stack_c::get());
            }
        }

        if (i_this->field_0x998 == 2) {
            i_this->field_0x998 = 0;
            
            for (int i = 0; i < 2; i++) {
                dComIfGp_particle_set(bo_eno_12[i], &actor->current.pos, NULL, NULL);
            }
        }
    }

    if (i_this->field_0x9b5 != 0) {
        mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
        mDoMtx_stack_c::scaleM(size, size, size);

        J3DModel* model = i_this->efModelMorf->getModel();
        model->setBaseTRMtx(mDoMtx_stack_c::get());

        i_this->efModelMorf->play(NULL, 0, 0);
        i_this->pbtk->play();
        i_this->efModelMorf->modelCalc();

        cXyz sp40(size, size, size);
        for (int i = 0; i < 4; i++) {
            cXyz sp34(actor->current.pos);
            if (i >= 1) {
                sp34.y = actor->home.pos.y;
            }

            static u16 bo_eno_end[] = {
                ID_ZI_J_SPBOOM_LEAF_A,
                ID_ZI_J_SPBOOM_ROCK_A,
                ID_ZI_J_SPBOOM_SYOUGEKI_A,
                ID_ZI_J_SPBOOM_SYOUGEKI_B,
            };

            i_this->field_0x9a4[i] = dComIfGp_particle_set(i_this->field_0x9a4[i], bo_eno_end[i], &sp34, &actor->tevStr, NULL, &sp40, 0xFF, NULL, -1, NULL, NULL, NULL);
        }
    }

    return 1;
}

static int daE_MK_BO_IsDelete(e_mk_bo_class* i_this) {
    return 1;
}

static int daE_MK_BO_Delete(e_mk_bo_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAcM_RegisterDeleteID(i_this, "E_MK_BO");
    dComIfG_resDelete(&i_this->phase, "E_mk");

    if (actor->heap != NULL) {
        i_this->sound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_mk_bo_class* a_this = (e_mk_bo_class*)i_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_mk", 0x2E);
    JUT_ASSERT(1265, modelData != NULL);

    a_this->model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (a_this->model == NULL) {
        return 0;
    }

    a_this->efModelMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("E_mk", 0x2F), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_mk", 6), 2, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11000284);
    if (a_this->efModelMorf == NULL || a_this->efModelMorf->getModel() == NULL) {
        return 0;
    }

    a_this->pbtk = new mDoExt_btkAnm();
    if (a_this->pbtk == NULL) {
        return 0;
    }

    if (!a_this->pbtk->init(a_this->efModelMorf->getModel()->getModelData(), (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("E_mk", 0x34), 1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    a_this->sound.init(&i_this->current.pos, 1);
    return 1;
}

static int daE_MK_BO_Create(fopAc_ac_c* i_this) {
    e_mk_bo_class* a_this = (e_mk_bo_class*)i_this;
    fopAcM_ct(&a_this->enemy, e_mk_bo_class);

    int phase_state = dComIfG_resLoad(&a_this->phase, "E_mk");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x3120)) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(i_this, a_this->model->getBaseTRMtx());

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_40, 0x2, 0x1f}, {0xd8fbfdff, 0x3}, 0x0}}, // mObj
                {dCcD_SE_WOOD, 0x1, 0x1, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_METAL, 0x5, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 30.0f} // mSph
            } // mSphAttr
        };

        a_this->ccStts.Init(0xFF, 0, i_this);
        a_this->ccAtSph.Set(at_sph_src);
        a_this->ccAtSph.SetStts(&a_this->ccStts);

        if (fopAcM_GetRoomNo(i_this) == 4) {
            a_this->ccAtSph.SetR(100.0f + BREG_F(15));
        }

        a_this->acch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->acchcir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->acchcir.SetWall(80.0f, 100.0f);
        a_this->counter = cM_rndF(65535.0f);

        if ((fopAcM_GetParam(i_this) & 0xFF) == 2) {
            a_this->action = 1;
        } else {
            a_this->field_0x9b4 = 1;
        }

        mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
        a_this->model->setBaseTRMtx(mDoMtx_stack_c::get());
        daE_MK_BO_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_MK_BO_Method = {
    (process_method_func)daE_MK_BO_Create,
    (process_method_func)daE_MK_BO_Delete,
    (process_method_func)daE_MK_BO_Execute,
    (process_method_func)daE_MK_BO_IsDelete,
    (process_method_func)daE_MK_BO_Draw,
};

actor_process_profile_definition g_profile_E_MK_BO = {
  fpcLy_CURRENT_e,       // mLayerID
  8,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_E_MK_BO,          // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(e_mk_bo_class), // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  166,                   // mPriority
  &l_daE_MK_BO_Method,   // sub_method
  0x00044100,            // mStatus
  fopAc_ENEMY_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};

AUDIO_INSTANCES
