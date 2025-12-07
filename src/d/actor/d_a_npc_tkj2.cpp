/**
 * @file d_a_npc_tkj2.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_tkj2.h"
#include "d/d_cc_d.h"
#include "d/d_s_play.h"

enum Tkj2_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_TKJ_FLY = 0x6,
    /* 0x07 */ BCK_TKJ_FLY_FALL,
    /* 0x08 */ BCK_TKJ_F_TALK_A,
    /* 0x09 */ BCK_TKJ_MOGAKU,
    /* 0x0A */ BCK_TKJ_WAIT_A,
    /* 0x0B */ BCK_TKJ_WALK_A,

    /* BMDR */
    /* 0x0E */ BMDR_TKJ = 0xE,

    /* BTK */
    /* 0x11 */ BTK_TKJ = 0x11,

    /* BTP */
    /* 0x14 */ BTP_TKJ = 0x14,
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_ARML1,
    /* 0x02 */ JNT_ARML2,
    /* 0x03 */ JNT_HANDL,
    /* 0x04 */ JNT_TUBASAL,
    /* 0x05 */ JNT_ARMR1,
    /* 0x06 */ JNT_ARMR2,
    /* 0x07 */ JNT_HANDR,
    /* 0x08 */ JNT_TUBASAR,
    /* 0x09 */ JNT_LEGL,
    /* 0x0A */ JNT_FOOTL1,
    /* 0x0B */ JNT_FOOTL2,
    /* 0x0C */ JNT_LEGR,
    /* 0x0D */ JNT_FOOTR1,
    /* 0x0E */ JNT_FOOTR2,
    /* 0x0F */ JNT_NECK1,
    /* 0x10 */ JNT_NECK2,
    /* 0x11 */ JNT_HEAD,
    /* 0x12 */ JNT_CHIN,
    /* 0x13 */ JNT_MOUTH,
};

enum Action {
    /* 0x0 */ ACTION_NORMAL,
    /* 0x1 */ ACTION_CARRY,
    /* 0x2 */ ACTION_FLY,
    /* 0x3 */ ACTION_DROP,
    /* 0x4 */ ACTION_RETURN,
    /* 0x6 */ ACTION_DROP_2 = 0x6,
};

daNpc_Tkj2_HIO_c::daNpc_Tkj2_HIO_c() {
    field_0x4 = -1;
    basic_size = 1.0f;
    hook_hit_size = 1.0f;
    boots_struggle_sp = 2.0f;
}

static void anm_init(npc_tkj2_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Tkj2", i_index), i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->field_0x5f4 = i_index;
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        int jntNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        npc_tkj2_class* i_this = (npc_tkj2_class*)model->getUserArea();

        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);

            if (jntNo == JNT_NECK1) {
                cMtx_XrotM(*calc_mtx, i_this->field_0xa8e / 2);
                cMtx_ZrotM(*calc_mtx, i_this->field_0xa90 / 2);
            } else if (jntNo == JNT_NECK2) {
                cMtx_YrotM(*calc_mtx, i_this->field_0xa92);
                cMtx_XrotM(*calc_mtx, i_this->field_0xa8e / 2);
                cMtx_ZrotM(*calc_mtx, i_this->field_0xa90 / 2);
            }

            model->setAnmMtx(jntNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int daNpc_Tkj2_Draw(npc_tkj2_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    J3DModel* model = i_this->mpModelMorf->getModel();

    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);
    i_this->mpBtkAnm->entry(model->getModelData());
    i_this->mpBtpAnm->entry(model->getModelData());
    i_this->mpModelMorf->entryDL();

    if (!fopAcM_checkCarryNow(a_this)) {
        cXyz pos;
        pos.set(a_this->current.pos.x, a_this->current.pos.y + 100.0f, a_this->current.pos.z);
        i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &pos, 400.0f, 40.0f,
                                                a_this->current.pos.y, i_this->mObjAcch.GetGroundH(), i_this->mObjAcch.m_gnd,
                                                &a_this->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static void npc_tkj2_carry_check(npc_tkj2_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    if (fopAcM_checkCarryNow(a_this)) {
        cLib_offBit<u32>(a_this->attention_info.flags, 0x10);
        i_this->mAction = 1;
        i_this->field_0x60a = 0;
        a_this->speed.y = 0.0f;
        a_this->speedF = 0.0f;
        i_this->field_0x630 = 0;
        i_this->mStts.Init(100, 0, a_this);
    }
}

static void mode_init_boomCarry(npc_tkj2_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    i_this->mObjAcch.ClrMoveBGOnly();
    i_this->mObjAcch.ClrGrndNone();
    i_this->mBoomerangMove.initOffset(&a_this->current.pos, i_this->field_0x88c.GetTgHitGObj());
    i_this->field_0x5f2 = 1;
}

static void Wind_effect(npc_tkj2_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    i_this->field_0x5eb = 0;

    if (i_this->field_0x88c.ChkTgHit()) {
        if (!fopAcM_checkCarryNow(a_this) && i_this->field_0x5f2 == 0 && i_this->field_0x5ea == 0) {
            dCcD_GObjInf* tgHitObj = i_this->field_0x88c.GetTgHitGObj();
            if (tgHitObj->GetAtMtrl() == dCcD_MTRL_WIND) {
                if (i_this->field_0x5eb == 0) {
                    i_this->mpModelMorf->setPlaySpeed(VREG_F(1) + 2.0f);

                    if (i_this->mAction == 0) {
                        i_this->field_0x60a = 0;
                        a_this->speedF = 5.0f;
                        a_this->speed.y = 20.0f;
                        i_this->mAction = 2;
                        i_this->field_0x60a = 0;
                    }
                }

                cXyz* atVecP = tgHitObj->GetAtVecP();
                *atVecP *= yREG_F(2) + 2.0f;
                cLib_addCalc(&i_this->field_0x5fc.x, atVecP->x, 0.1f, 100.0f, 0.0f);
                cLib_addCalc(&i_this->field_0x5fc.y, atVecP->y, 0.1f, 100.0f, 0.0f);
                cLib_addCalc(&i_this->field_0x5fc.z, atVecP->z, 0.1f, 100.0f, 0.0f);

                s16 sVar1 = (s16)cM_atan2s(i_this->field_0x5fc.x, i_this->field_0x5fc.z);

                if (i_this->field_0x5fc.x != 0.0f || i_this->field_0x5fc.z != 0.0f) {
                    if ((s16)(sVar1 - i_this->field_0x5c4) < 0) {
                        cLib_addCalcAngleS(&a_this->shape_angle.z, 0x2000, 0x10, 0x1000, 0);
                    } else {
                        cLib_addCalcAngleS(&a_this->shape_angle.z, 0xFFFFE000, 2, 0x1000, 0);
                    }
                }

                i_this->field_0x5c4 = cM_atan2s(i_this->field_0x5fc.x, i_this->field_0x5fc.z);
                i_this->field_0x5eb = 1;
            }
        }
    } else {
        cLib_chaseAngleS(&a_this->shape_angle.z, 0, 0x500);
    }

    if (i_this->field_0x5eb == 0) {
        cXyz sp30(0.0f, 0.0f, 0.0f);
        cLib_addCalc(&i_this->field_0x5fc.x, sp30.x, 0.1f, 100.0f, 0.0f);
        cLib_addCalc(&i_this->field_0x5fc.y, sp30.y, 0.1f, 100.0f, 0.0f);
        cLib_addCalc(&i_this->field_0x5fc.z, sp30.z, 0.1f, 100.0f, 0.0f);
    }

    a_this->current.pos += i_this->field_0x5fc;
}

static void CheckRoof(npc_tkj2_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz end;
    cXyz spc8(0.0f, 50.0f, 1000.0f);
    dBgS_LinChk lin_chk;

    for (int i = 0; i < 4; i++) {
        cLib_offsetPos(&end, &a_this->current.pos, a_this->shape_angle.y + i * 0x4000, &spc8);
        lin_chk.Set(&a_this->current.pos, &end, NULL);
        
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
            cXyz* np = plane.GetNP();
            cXyz spd4(0.0f, 1.0f, 0.0f);
            cXyz spe0;
            cXyz spec(0.0f, 0.0f, 0.0f);
            cXyz spf8(0.0f, np->y, np->z);

            i_this->field_0x5ec.z = -cM_atan2s(np->x, spec.abs(spf8));
            i_this->field_0x5ec.x = cM_atan2s(np->z, np->y);
            i_this->field_0x630 = 1;
            a_this->current.pos = lin_chk.GetCross();
            return;
        }
    }
}

static bool CheckWall(npc_tkj2_class* i_this, s16 param_2) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp1b0(0.0f, -100.0f, a_this->speedF);
    cXyz sp1bc(0.0f, 100.0f, 0.0f);
    cXyz sp1c8(0.0f, 100.0f, 100.0f);
    cXyz sp1d4(0.0f, -100.0f, 100.0f);
    dBgS_LinChk lin_chk_1, lin_chk_2, lin_chk_3;

    mDoMtx_stack_c::transS(a_this->current.pos);
    mDoMtx_stack_c::ZXYrotM(i_this->field_0x5ec);
    mDoMtx_stack_c::YrotM(param_2);
    mDoMtx_stack_c::multVec(&sp1b0, &sp1b0);
    mDoMtx_stack_c::multVec(&sp1bc, &sp1bc);
    mDoMtx_stack_c::multVec(&sp1c8, &sp1c8);
    mDoMtx_stack_c::multVec(&sp1d4, &sp1d4);

    lin_chk_1.Set(&sp1bc, &sp1b0, NULL);
    lin_chk_2.Set(&sp1bc, &sp1c8, NULL);
    lin_chk_3.Set(&sp1c8, &sp1d4, NULL);

    if (dComIfG_Bgsp().LineCross(&lin_chk_2)) {
        return false;
    }

    if (!dComIfG_Bgsp().LineCross(&lin_chk_3)) {
        return false;
    }

    if (dComIfG_Bgsp().LineCross(&lin_chk_1)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(lin_chk_1, &plane);
        cXyz* np = plane.GetNP();
        cXyz sp1e0(0.0f, 0.0f, 0.0f);
        cXyz sp1ec(0.0f, np->y, np->z);
        f32 fVar1 = sp1e0.abs(sp1ec);
        a_this->current.pos = lin_chk_1.GetCross();
        s16 sVar1 = -cM_atan2s(np->x, fVar1);
        s16 sVar2 = cM_atan2s(np->z, np->y);
        cLib_chaseAngleS(&i_this->field_0x5ec.x, sVar2, 0x150);
        cLib_chaseAngleS(&i_this->field_0x5ec.z, sVar1, 0x150);
        return true;
    }

    return false;
}

static bool GndCheck2(npc_tkj2_class* i_this) {
    dBgS_ObjGndChk obj_gnd_chk;
    cXyz sp78(i_this->field_0x5b8.x, i_this->field_0x5b8.y + 100.0f, i_this->field_0x5b8.z);
    obj_gnd_chk.SetPos(&sp78);
    sp78.y = dComIfG_Bgsp().GroundCross(&obj_gnd_chk);

    if (i_this->field_0x5b8.y - sp78.y > 500.0f) {
        return true;
    }

    return false;
}

static bool GndCheck(npc_tkj2_class* i_this, cXyz param_2) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp14;

    cMtx_YrotS(*calc_mtx, i_this->field_0x5c4);
    MtxPosition(&param_2, &sp14);
    sp14 += a_this->current.pos;

    if (a_this->current.pos.y - a_this->home.pos.y < -500.0f) {
        return true;
    }

    return false;
}

static void npc_tkj2_normal(npc_tkj2_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp38, sp44;
    f32 fVar1 = 0.0f;
    cXyz sp50(0.0f, 100.0f, 0.0f);

    switch (i_this->field_0x60a) {
        case 0:
            i_this->field_0x610[0] = cM_rndF(70.0f) + 50.0f;
            anm_init(i_this, BCK_TKJ_WAIT_A, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->field_0x60a++;

            if (i_this->field_0x630 != 0) {
                i_this->field_0x60a++;
            }
            break;

        case 1:
            if (i_this->field_0x610[0] == 0) {
                i_this->field_0x60a = 10;
                sp50.set(0.0f, 100.0f, 200.0f);
                sp38 = a_this->home.pos - a_this->current.pos;
                sp38.x += cM_rndFX(300.0f);
                sp38.z += cM_rndFX(300.0f);
                i_this->field_0x5c4 = cM_atan2s(sp38.x, sp38.z);

                if (GndCheck(i_this, sp50)) {
                    i_this->field_0x5c4 = a_this->current.angle.y + 0x8000 + (s16)cM_rndFX(4000.0f);
                } else if (fopAcM_wayBgCheck(a_this, 100.0f, 30.0f)) {
                    i_this->field_0x5c4 = a_this->current.angle.y + 0x8000 + (s16)cM_rndFX(4000.0f);
                }

                i_this->field_0x610[0] = cM_rndF(60.0f) + 30.0f;
                anm_init(i_this, BCK_TKJ_WALK_A, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;

        case 2:
            if (i_this->field_0x610[0] == 0) {
                i_this->field_0x60a = 11;
                sp50.set(0.0f, 100.0f, 200.0f);
                sp38 = a_this->home.pos - a_this->current.pos;
                sp44.set(0.0f, 0.0f, 0.0f);
                mDoMtx_stack_c::ZXYrotS(i_this->field_0x5ec);
                mDoMtx_stack_c::multVec(&sp38, &sp44);
                sp44.x += cM_rndFX(300.0f);
                sp44.z += cM_rndFX(300.0f);
                i_this->field_0x5c4 = cM_atan2s(sp44.x, sp44.z);

                if (!CheckWall(i_this, i_this->field_0x5c4)) {
                    i_this->field_0x5c4 += 0x8000;
                }

                i_this->field_0x610[0] = cM_rndF(60.0f) + 30.0f;
                anm_init(i_this, BCK_TKJ_WALK_A, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;

        case 10:
            fVar1 = 5.0f;
            sp50.set(0.0f, 100.0f, 100.0f);
            
            if (GndCheck(i_this, sp50)) {
                i_this->field_0x60a = 1;
                i_this->field_0x610[0] = 0;
            }

            if (i_this->field_0x610[0] == 0) {
                i_this->field_0x60a = 0;
                a_this->speedF = 0.0f;
                fVar1 = 3.0f;
            }
            break;

        case 11:
            fVar1 = 5.0f;

            if (!CheckWall(i_this, a_this->current.angle.y)) {
                fVar1 = 0.0f;
            }

            if (i_this->field_0x610[0] == 0) {
                i_this->field_0x60a = 0;
            }
            break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5c4, 8, 0x400);
    cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, 5.0f);
}

static cXyz return_pos_get(npc_tkj2_class* i_this) {
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    dBgS_GndChk gnd_chk;
    cXyz sp84, sp90;

    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        } else if (i == 1) {
            cMtx_YrotS(*calc_mtx, player->shape_angle.y - 0x4000);
        } else {
            cMtx_YrotS(*calc_mtx, player->shape_angle.y + 0x4000);
        }

        sp84.x = 0.0f;
        sp84.y = 100.0f;
        sp84.z = -100.0f;
        MtxPosition(&sp84, &sp90);
        sp90 += player->current.pos;
        gnd_chk.SetPos(&sp90);
        sp90.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

        if (fabsf(player->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk)) < 50.0f) {
            return sp90;
        }
    }

    return player->current.pos;
}

static u8 l_initHIO;

static daNpc_Tkj2_HIO_c l_HIO;

static s8 npc_tkj2_carry(npc_tkj2_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp30, sp3c, sp48;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    cLib_addCalcAngleS2(&a_this->current.angle.y, player->shape_angle.y, 1, 0x2000);
    i_this->field_0x88c.OffCoSetBit();
    a_this->speed.y = 0.0f;
    s8 rv = 0;
    cXyz& speed = fopAcM_GetSpeed(player);
    a_this->home.pos = a_this->current.pos;
    i_this->mStts.ClrCcMove();

    switch (i_this->field_0x60a) {
        case 0:
            anm_init(i_this, BCK_TKJ_MOGAKU, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->field_0x60a++;
            // fallthrough
        case 1:
            if (i_this->mpModelMorf->checkFrame(0.0f)) {
                i_this->mSound.startCreatureVoice(Z2SE_TKJ_V_CATCHED_CRY, -1);
            }

            if (player->checkEquipHeavyBoots() && speed.y != 0.0f) {
                i_this->mpModelMorf->setPlaySpeed(l_HIO.boots_struggle_sp);
            } else if (i_this->field_0x88c.ChkTgHit()) {
                if (i_this->field_0x88c.GetTgHitGObj()->GetAtMtrl() == dCcD_MTRL_WIND) {
                    i_this->mpModelMorf->setPlaySpeed(2.0f);
                    i_this->field_0x88c.ClrTgHit();
                }
            } else if (speed.y != 0.0f) {
                i_this->mpModelMorf->setPlaySpeed(1.0f);
            } else {
                i_this->mpModelMorf->setPlaySpeed(1.0f);
            }
            break;
    }

    if (!fopAcM_checkCarryNow(a_this)) {
        if (player->checkGrabThrow()) {
            i_this->mSound.startCreatureVoice(Z2SE_TKJ_V_THROWN_CRY, -1);
            i_this->field_0x60a = 0;
            a_this->speedF = 20.0f;
            a_this->speed.y = 15.0f;
            i_this->mAction = 2;
            i_this->field_0x60a = 0;
        } else if (i_this->mObjAcch.ChkGroundHit()) {
            i_this->mAction = 0;
            i_this->field_0x60a = 0;
            i_this->field_0x610[0] = 15;
        } else if (speed.y < 0.0f) {
            i_this->mAction = 6;
            i_this->field_0x60a = 0;
        } else {
            i_this->mAction = 3;
            i_this->field_0x60a = 0;
        }

        fopAcM_carryOffRevise(a_this);
        rv = 1;
        a_this->home.pos = return_pos_get(i_this);
    }

    
    if (fopAcM_checkCarryNow(a_this)) {
        if (daPy_getLinkPlayerActorClass()->checkAutoJump()) {
            if (strcmp(dComIfGp_getStartStageName(), "D_MN07") == 0 && fopAcM_GetRoomNo(a_this) == 1) {
                fopAcM_onSwitch(a_this, 0x44);
            }
        }
    }
    
    return rv;
}

static void npc_tkj2_fly(npc_tkj2_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    f32 fVar1;

    switch (i_this->field_0x60a) {
        case 0:
            anm_init(i_this, BCK_TKJ_FLY, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->field_0x60a++;
            i_this->field_0x610[0] = 50;
            fVar1 = 1.0f;
            i_this->field_0x5b8 = a_this->home.pos;
            break;
        
        case 1:
            fVar1 = 0.3f;
            break;
    }

    cLib_addCalc2(&a_this->speedF, 20.0f, fVar1, 10.0f);
    i_this->field_0x5d0 = -1.0f;

    if (i_this->field_0x5eb != 0) {
        i_this->field_0x5d0 = 0.0f;
        a_this->speed.y = 0.0f;
        a_this->speedF *= 0.5f;
    }

    if (a_this->speed.y < -20.0f) {
        a_this->speed.y = -20.0f;
    }

    if (i_this->field_0x610[0] == 0) {
        i_this->mAction = 4;
        i_this->field_0x60a = 0;
        i_this->field_0x5f8 = 0x500;

        if (GndCheck2(i_this)) {
            i_this->field_0x5b8 = fopAcM_GetPosition(player);
        }
    } else if (i_this->mObjAcch.ChkGroundHit()) {
        i_this->mAction = 0;
        i_this->field_0x60a = 0;
        a_this->home.pos = a_this->current.pos;
    }
}

static void npc_tkj2_drop(npc_tkj2_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    switch (i_this->field_0x60a) {
        case 0:
            anm_init(i_this, BCK_TKJ_FLY_FALL, 2.0f, J3DFrameCtrl::EMode_NONE, 0.5f);
            i_this->field_0x60a++;
            break;

        case 1:
            break;
    }

    i_this->field_0x5d0 = -2.0f;

    if (i_this->mObjAcch.ChkGroundHit()) {
        i_this->mAction = 0;
        i_this->field_0x60a = 0;
        a_this->home.pos = a_this->current.pos;
    }
    
    cLib_addCalc2(&a_this->speedF, 20.0f, 0.5f, 10.0f);
}
static void npc_tkj2_return(npc_tkj2_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp4c;

    sp4c.x = i_this->field_0x5b8.x - a_this->current.pos.x;
    sp4c.z = i_this->field_0x5b8.z - a_this->current.pos.z;
    i_this->field_0x5c4 = cM_atan2s(sp4c.x, sp4c.z);
    JMAFastSqrt(sp4c.x * sp4c.x + sp4c.z * sp4c.z);
    f32 fVar1;

    switch (i_this->field_0x60a) {
        case 0:
            anm_init(i_this, BCK_TKJ_FLY, 1.0f, J3DFrameCtrl::EMode_LOOP, 0.5f);
            i_this->field_0x60a++;
            i_this->field_0x610[0] = 200;
            i_this->field_0xa98 = 0.5f;
            // fallthrough
        case 1:
            fVar1 = 0.2f;
            cLib_addCalc2(&i_this->field_0xa98, 2.0f, 0.2f, 0.1f);
            break;
    }

    i_this->mpModelMorf->setPlaySpeed(i_this->field_0xa98);
    cLib_addCalc2(&a_this->speedF, 20.0f, fVar1, 2.0f);
    i_this->field_0x5d0 = VREG_F(0);
    cLib_addCalc2(&a_this->speed.y, (i_this->field_0x5b8.y + 100.0f + XREG_F(0) - a_this->current.pos.y) / (i_this->field_0x5b8.absXZ(a_this->current.pos) / a_this->speedF),
                  0.2f, XREG_F(2) + 1.5f);

    if (i_this->field_0x5eb != 0) {
        a_this->speed.y = 0.0f;
        i_this->field_0x5d0 = 0.0f;
        a_this->speedF *= 0.5f;

        if (a_this->speedF < 10.0f) {
            a_this->speedF = 10.0f;
        }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5c4, 10, i_this->field_0x5f8);
    cXyz sp58(a_this->current.pos.x, a_this->current.pos.y + 200.0f, a_this->current.pos.z);
    f32 fVar2 = a_this->current.pos.absXZ(i_this->field_0x5b8);
    cXyz sp64(0.0f, 100.0f, 0.0f);

    if (i_this->field_0x610[0] == 0) {
        if (i_this->field_0x5eb != 0) {
            i_this->field_0x610[0] = 50;
        } else {
            a_this->home.pos = fopAcM_GetPosition(player);
            i_this->field_0x5b8 = a_this->home.pos;
        }
    }

    if (fVar2 < 100.0f) {
        if (!GndCheck2(i_this)) {
            anm_init(i_this, BCK_TKJ_FLY_FALL, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mAction = 6;
            i_this->field_0x60a = 0;
        } else {
            a_this->home.pos = fopAcM_GetPosition(player);
            i_this->field_0x5b8 = a_this->home.pos;
        }
    }

    if (i_this->mObjAcch.ChkGroundHit()) {
        anm_init(i_this, BCK_TKJ_FLY_FALL, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mAction = 6;
        i_this->field_0x60a = 0;
    }
}

static void ObjHit(npc_tkj2_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    
    i_this->field_0x620--;
    if (i_this->field_0x620 <= 0) {
        i_this->field_0x620 = 0;
    }

    if (i_this->field_0x620 > 0) {
        i_this->field_0x88c.ClrTgHit();

        if (i_this->field_0x630 == 0 && a_this->speedF != 0.0f) {
            cLib_addCalc2(&a_this->speedF, 10.0f, 1.0f, 5.0f);
        }
    } else if (i_this->field_0x88c.ChkTgHit()) {
        if (a_this->speedF == 0.0f) {
            i_this->field_0x88c.ClrTgHit();
            i_this->field_0x60a = 0;
            a_this->speedF = 5.0f;
            a_this->speed.y = 20.0f;
            i_this->mAction = 2;
            i_this->field_0x60a = 0;
            anm_init(i_this, BCK_TKJ_FLY, 1.0f, J3DFrameCtrl::EMode_LOOP, 2.0f);
            i_this->field_0x60a++;
            i_this->field_0x610[0] = 50;
            i_this->field_0x5b8 = a_this->home.pos;
        } else {
            i_this->field_0x620 = 20;
            i_this->field_0x610[0] = 40;
            i_this->mpModelMorf->setPlaySpeed(2.0f);
        }
    }
}

static void action(npc_tkj2_class* i_this) {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->actor;
    cXyz sp38, sp44;

    i_this->field_0x5d0 = -5.0f;
    a_this->current.angle.z = 0;

    if (i_this->field_0x630 != 0) {
        a_this->gravity = 0.0f;
    }

    i_this->field_0x88c.OnCoSetBit();
    i_this->field_0x5c8 = fopAcM_searchPlayerDistance(a_this);
    i_this->field_0x5cc = fopAcM_searchPlayerAngleY(a_this);

    s8 sVar1 = 0;
    BOOL bVar1 = FALSE;
    s8 sVar2 = 1;
    cXyz sp50(0.0f, 100.0f, 0.0f);
    i_this->field_0xa8c = 0;

    switch (i_this->mAction) {
        case ACTION_NORMAL:
            npc_tkj2_normal(i_this);
            sVar1 = 1;
            bVar1 = TRUE;

            if (i_this->field_0x630 != 0) {
                sVar2 = 0;
            }

            if (i_this->field_0x88c.ChkTgHit()) {
                cCcD_ObjHitInf* tgHitObj = i_this->field_0x88c.GetTgHitObj();

                if (tgHitObj != NULL) {
                    if (tgHitObj->ChkAtType(AT_TYPE_BOOMERANG)) {
                        mode_init_boomCarry(i_this);
                    } else {
                        tgHitObj->ChkAtType(AT_TYPE_HOOKSHOT);
                    }
                }
            }

            ObjHit(i_this);
            break;

        case ACTION_CARRY:
            sVar2 = npc_tkj2_carry(i_this);
            break;

        case ACTION_FLY:
            npc_tkj2_fly(i_this);
            break;

        case ACTION_DROP:
            npc_tkj2_drop(i_this);
            cLib_addCalc2(&a_this->speedF, 5.0f, 0.3f, 1.0f);

            if (GndCheck(i_this, sp50)) {
                i_this->field_0x5b8 = a_this->home.pos;
                i_this->field_0x60a = 0;
                a_this->speedF = 5.0f;
                a_this->speed.y = 10.0f;
                i_this->mAction = 4;

                if (GndCheck2(i_this)) {
                    i_this->field_0x5b8 = fopAcM_GetPosition(player);
                }

                i_this->field_0x5c4 = fopAcM_searchPlayerAngleY(a_this) + 0x8000;
                i_this->field_0x5f8 = 0x500;
            }
            break;

        case ACTION_DROP_2:
            npc_tkj2_drop(i_this);

            if (GndCheck2(i_this)) {
                i_this->field_0x5b8 = fopAcM_GetPosition(player);
                i_this->field_0x60a = 0;
                a_this->speedF = 5.0f;
                a_this->speed.y = 10.0f;
                i_this->mAction = 4;
                i_this->field_0x5f8 = 0x500;
                i_this->field_0x5c4 = fopAcM_searchPlayerAngleY(a_this) + 0x8000;
            }
            break;

        case ACTION_RETURN:
            npc_tkj2_return(i_this);
            break;
    }

    Wind_effect(i_this);
    cLib_addCalc(&a_this->gravity, i_this->field_0x5d0, 0.5f, 10.0f, 0.0f);

    if (i_this->field_0x5c8 < 200.0f) {
        i_this->field_0xa8c = 1;
    }

    if (fopAcM_CheckStatus(a_this, fopAcM_STATUS_HOOK_CARRY_NOW)) {
        i_this->field_0x5ea = 1;
    } else if (i_this->field_0x5ea != 0) {
        i_this->field_0x5ea = 0;

        if (dComIfGp_checkPlayerStatus1(0, 0x2000000) || dComIfGp_checkPlayerStatus1(0, 0x10000)) {
            i_this->mAction = 3;
            i_this->field_0x630 = 0;
            i_this->field_0x60a = 0;
            a_this->speedF = 0.0f;
            a_this->speed.y = 0.0f;
        } else if (player->setForceGrab(a_this, 0, 1)) {
            cLib_offBit<u32>(a_this->attention_info.flags, 0x10);
            i_this->mAction = 1;
            i_this->field_0x630 = 0;
            i_this->field_0x60a = 0;
            a_this->speedF = 0.0f;
            a_this->speed.y = 0.0f;
        }
    }

    if (sVar1 != 0 && i_this->field_0x630 == 0) {
        if (GndCheck(i_this, sp50)) {
            i_this->field_0x60a = 0;
            a_this->speedF = 20.0f;
            a_this->speed.y = 15.0f;
            i_this->mAction = 2;
            i_this->field_0x60a = 0;
        } else if (a_this->speed.y <= -15.0f) {
            i_this->mAction = 3;
            i_this->field_0x60a = 0;
            a_this->speed.y = 0.0f;
            bVar1 = TRUE;
            i_this->field_0x610[0] = 15;
        }
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x2000);

    if (i_this->field_0x630 == 0) {
        if (i_this->field_0x630 != 0) {
            mDoMtx_stack_c::ZXYrotS(i_this->field_0x5ec);
            mDoMtx_stack_c::YrotM(a_this->current.angle.y);
        } else {
            mDoMtx_stack_c::YrotS(a_this->current.angle.y);
        }

        sp38.x = 0.0f;
        sp38.y = 0.0f;
        sp38.z = a_this->speedF;
        mDoMtx_stack_c::multVec(&sp38, &sp44);
        a_this->speed.x = sp44.x;
        a_this->speed.z = sp44.z;
        a_this->current.pos += a_this->speed;
        a_this->speed.y += a_this->gravity;

        if (a_this->speed.y < -80.0f) {
            a_this->speed.y = -80.0f;
        }
    }

    if (!fopAcM_checkCarryNow(a_this)) {
        cXyz* ccMoveP = i_this->mStts.GetCCMoveP();
        
        if (ccMoveP != NULL) {
            a_this->current.pos.x += ccMoveP->x;
            a_this->current.pos.y += ccMoveP->y;
            a_this->current.pos.z += ccMoveP->z;
        }
    }

    if (sVar2) {
        i_this->mObjAcch.CrrPos(dComIfG_Bgsp());
    }

    if (i_this->field_0x610[2] == 0) {
        i_this->field_0x610[2] = cM_rndF(100.0f) + 10.0f;
        i_this->field_0x5e8 = 8;
    }

    if (i_this->field_0x5e8 != 0) {
        i_this->field_0x5e8--;
        i_this->field_0x5e4 = i_this->field_0x5e8;

        if (i_this->field_0x5e4 > 5.0f) {
            i_this->field_0x5e4 = 0.0f;
        }
    } else {
        i_this->field_0x5e4 = 0.0f;
    }

    s16 sVar3 = 0;
    s16 sVar4 = 0;
    s16 sVar5 = BREG_S(0) + 6000;
    s16 sVar6 = BREG_S(1) + 9000;

    if (i_this->field_0xa8c != 0) {
        sp38 = player->eyePos - a_this->current.pos;
        sp38.y += TREG_F(1) + -110.0f;
        s16 sVar7 = a_this->shape_angle.y - i_this->field_0x5cc;

        if (sVar7 < 0x4000 && sVar7 > -0x4000) {
            sVar3 = cM_atan2s(sp38.x, sp38.z) - a_this->shape_angle.y;
            sVar4 = (s16)cM_atan2s(sp38.y, JMAFastSqrt(sp38.x * sp38.x + sp38.z * sp38.z));
            
            if (sVar3 > sVar6) {
                sVar3 = sVar6;
            } else if (sVar3 < -sVar6) {
                sVar3 = -sVar6;
            }

            if (sVar4 > sVar5) {
                sVar4 = sVar5;
            } else if (sVar4 < -sVar5) {
                sVar4 = -sVar5;
            }
        }

        if (i_this->field_0x610[3] == 0) {
            i_this->field_0x610[3] = cM_rndF(20.0f) + 10.0f;
            i_this->field_0xa94 = cM_rndFX(2000.0f);
        }
    } else {
        i_this->field_0xa94 = 0;
    }

    cLib_addCalcAngleS2(&i_this->field_0xa8e, sVar3, 2, 0x600);
    cLib_addCalcAngleS2(&i_this->field_0xa90, sVar4, 2, 0x600);
    cLib_addCalcAngleS2(&i_this->field_0xa92, i_this->field_0xa94, 4, 300);

    if (i_this->field_0x5f2 != 0) {
        cLib_chaseAngleS(&i_this->field_0x5ec.x, 0, 0x100);
        cLib_chaseAngleS(&i_this->field_0x5ec.z, 0, 0x100);

        if (i_this->field_0x5f3 != 0) {
            if (player->getGrabUpEnd() || player->getGrabUpStart()) {
                i_this->mAction = 0;
                i_this->field_0x630 = 0;
                i_this->field_0x60a = 0;
                a_this->speedF = 0.0f;
                a_this->speed.y = 0.0f;
                i_this->field_0x5f2 = 0;
                i_this->field_0x5f3 = 0;
            } else if (player->setForceGrab(a_this, 0, 1)) {
                cLib_offBit<u32>(a_this->attention_info.flags, 0x10);
                i_this->mAction = 1;
                i_this->field_0x630 = 0;
                i_this->field_0x60a = 0;
                a_this->speedF = 0.0f;
                a_this->speed.y = 0.0f;
                i_this->field_0x5f2 = 0;
                i_this->field_0x5f3 = 0;
            } else {
                i_this->mAction = 0;
                i_this->field_0x630 = 0;
                i_this->field_0x60a = 0;
                a_this->speedF = 0.0f;
                a_this->speed.y = 0.0f;
                i_this->field_0x5f2 = 0;
                i_this->field_0x5f3 = 0;
            }
        } else if (i_this->field_0x5c8 < 250.0f) {
            i_this->field_0x5f3 = 1;
        } else if (i_this->mBoomerangMove.posMove(&a_this->current.pos, &a_this->shape_angle.y, NULL, 0x1C00) != 0) {
            i_this->mBoomerangMove.bgCheckAfterOffset(&a_this->current.pos);
            a_this->current.pos.y += 0.5f;
        }
    }

    if (i_this->mAction != 1) {
        npc_tkj2_carry_check(i_this);
    }

    if (bVar1) {
        cLib_onBit<u32>(a_this->attention_info.flags, 0x10);
    } else {
        cLib_offBit<u32>(a_this->attention_info.flags, 0x10);
    }
}

static int daNpc_Tkj2_Execute(npc_tkj2_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp28(0.0f, 0.0f, 0.0f);
    cXyz sp34;

    for (int i = 0; i < 4; i++) {
        if (i_this->field_0x610[i] != 0) {
            i_this->field_0x610[i]--;
        }
    }

    if (i_this->field_0x618 != 0) {
        i_this->field_0x618--;
    }

    if (i_this->field_0x61a != 0) {
        i_this->field_0x61a--;
    }

    action(i_this);

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);

    if (i_this->field_0x630 != 0) {
        mDoMtx_stack_c::ZXYrotM(i_this->field_0x5ec);
    }

    mDoMtx_stack_c::ZXYrotM(a_this->shape_angle);
    mDoMtx_stack_c::scaleM(l_HIO.basic_size, l_HIO.basic_size, l_HIO.basic_size);
    
    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));

    if ((i_this->field_0x5f4 == 6 && i_this->mpModelMorf->checkFrame(1.0f)) ||
        (i_this->field_0x5f4 == 7 &&  (i_this->mpModelMorf->checkFrame(1.0f) || i_this->mpModelMorf->checkFrame(7.0f) || i_this->mpModelMorf->checkFrame(13.0f) || i_this->mpModelMorf->checkFrame(20.0f))) ||
        (i_this->field_0x5f4 == 9 && (i_this->mpModelMorf->checkFrame(1.0f) || i_this->mpModelMorf->checkFrame(6.0f) || i_this->mpModelMorf->checkFrame(11.0f) || i_this->mpModelMorf->checkFrame(16.0f)))) {
        i_this->mSound.startCreatureSound(Z2SE_TKJ_WING, 0, -1);
    }

    if ((i_this->field_0x5f4 == 7 && (i_this->mpModelMorf->checkFrame(17.0f) || i_this->mpModelMorf->checkFrame(19.0f))) ||
        (i_this->field_0x5f4 == 0xB && (i_this->mpModelMorf->checkFrame(7.0f) || i_this->mpModelMorf->checkFrame(17.0f)))) {
        i_this->mSound.startCreatureSound(Z2SE_TKJ_FOOTNOTE, 0, -1);
    }

    i_this->mpBtkAnm->setFrame(i_this->field_0x5e0);
    i_this->mpBtpAnm->setFrame(i_this->field_0x5e4);
    i_this->mpModelMorf->modelCalc();

    MTXCopy(model->getAnmMtx(JNT_HEAD), *calc_mtx);
    MtxPosition(&sp28, &a_this->eyePos);
    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += 30.0f;

    cXyz sp40(a_this->current.pos);
    sp40.y += l_HIO.hook_hit_size * 20.0f;
    i_this->field_0x88c.SetC(sp40);
    i_this->field_0x88c.SetR(l_HIO.hook_hit_size * 40.0f);
    dComIfG_Ccsp()->Set(&i_this->field_0x88c);

    return 1;
}

static int daNpc_Tkj2_IsDelete(npc_tkj2_class* i_this) {
    return 1;
}

static int daNpc_Tkj2_Delete(npc_tkj2_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    dComIfG_resDelete(&i_this->mPhase, "Tkj2");

    if (i_this->field_0xa9c != 0) {
        l_initHIO = 0;
    }

    if (a_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    npc_tkj2_class* i_this = (npc_tkj2_class*)a_this;

    i_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("Tkj2", BMDR_TKJ), NULL, NULL,
                                               (J3DAnmTransform*)dComIfG_getObjectRes("Tkj2", BCK_TKJ_WAIT_A), 0, 1.0f, 0, -1, &i_this->mSound, 0x80000, 0x11020284);
    if (i_this->mpModelMorf == NULL || i_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setUserArea((uintptr_t)i_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    i_this->mpBtkAnm = new mDoExt_btkAnm();
    if (i_this->mpBtkAnm == NULL) {
        return 0;
    }
    if (i_this->mpBtkAnm->init(i_this->mpModelMorf->getModel()->getModelData(), (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Tkj2", BTK_TKJ),
                               1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }

    i_this->mpBtpAnm = new mDoExt_btpAnm();
    if (i_this->mpBtpAnm == NULL) {
        return 0;
    }
    if (i_this->mpBtpAnm->init(i_this->mpModelMorf->getModel()->getModelData(), (J3DAnmTexPattern*)dComIfG_getObjectRes("Tkj2", BTP_TKJ),
                               1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }

    return 1;
}

static cPhs__Step daNpc_Tkj2_Create(fopAc_ac_c* a_this) {
    fopAcM_ct(a_this, npc_tkj2_class);
    npc_tkj2_class* i_this = (npc_tkj2_class*)a_this;

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->mPhase, "Tkj2");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("NPC_TKJ2 PARAM %x\n", fopAcM_GetParam(a_this));

        i_this->field_0x5b4 = fopAcM_GetParam(a_this);
        a_this->shape_angle.x = 0;
        a_this->current.angle.x = 0;

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x2F00)) {
            OS_REPORT("//////////////NPC_TKJ2 SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (l_initHIO == 0) {
            i_this->field_0xa9c = 1;
            l_initHIO = 1;
            l_HIO.field_0x4 = -1;
        }

        #if DEBUG
        fopAcM_OnStatus(a_this, 0);
        #endif

        a_this->attention_info.flags = 0;
        i_this->mAction = 0;

        fopAcM_SetMtx(a_this, i_this->mpModelMorf->getModel()->getBaseTRMtx());
        i_this->mObjAcch.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1, &i_this->mAcchCir,
                             fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(70.0f, 80.0f);
        a_this->health = 100;
        a_this->field_0x560 = 100;

        if (i_this->field_0x5b4 == 1) {
            CheckRoof(i_this);
        }

        if (i_this->field_0x630 != 0) {
            i_this->mStts.Init(0xFF, 0, a_this);
        } else {
            i_this->mStts.Init(100, 0, a_this);
        }

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0x14202, 0x11}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 20.0f} // mSph
            } // mSphAttr
        };

        i_this->field_0x88c.Set(cc_sph_src);
        i_this->field_0x88c.SetStts(&i_this->mStts);
        a_this->attention_info.distances[fopAc_attn_CARRY_e] = 7;

        i_this->mSound.init(&a_this->current.pos, &a_this->eyePos, 3, 1);
        i_this->field_0x88c.OnTgNoHitMark();
        i_this->field_0x5fc.set(0.0f, 0.0f, 0.0f);
        daNpc_Tkj2_Execute(i_this);
    }

    return phase;
}

static actor_method_class l_daNpc_Tkj2_Method = {
    (process_method_func)daNpc_Tkj2_Create,
    (process_method_func)daNpc_Tkj2_Delete,
    (process_method_func)daNpc_Tkj2_Execute,
    (process_method_func)daNpc_Tkj2_IsDelete,
    (process_method_func)daNpc_Tkj2_Draw,
};

extern actor_process_profile_definition g_profile_NPC_TKJ2 = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_TKJ2,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(npc_tkj2_class), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  696,                    // mPriority
  &l_daNpc_Tkj2_Method,   // sub_method
  0x000C4000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_0_e,      // cullType
};
