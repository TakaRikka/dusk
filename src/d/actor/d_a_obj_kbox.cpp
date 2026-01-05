/**
 * @file d_a_obj_kbox.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/d_s_play.h"
#include "d/actor/d_a_obj_kbox.h"
#include "d/d_cc_d.h"
#include "Z2AudioLib/Z2Instances.h"

daObj_Kbox_HIO_c::daObj_Kbox_HIO_c() {
    mId = -1;
    mWidth = 2.0f;
}

#if DEBUG
void daObj_Kbox_HIO_c::genMessage(JORMContext* ctx) {
    // Skull fish wooden box
    ctx->genLabel("　スカル魚木箱　", 0x80000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Width
    ctx->genSlider("横幅", &mWidth, 0.0f, 3.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}
#endif

static bool hio_set;

static daObj_Kbox_HIO_c l_HIO;

static void ride_call_back(dBgW* param_1, fopAc_ac_c* param_2, fopAc_ac_c* param_3) {
    if (fopAcM_GetName(param_3) == PROC_ALINK) {
        obj_kbox_class* i_this = (obj_kbox_class*) param_2;
        if (i_this->field_0x5a0 == 0) {
            i_this->field_0x5a8 = -50.0f;
        }
        i_this->field_0x5a0 = 3;
        f32 xDiff = param_3->current.pos.x - param_2->current.pos.x;
        f32 zDiff = param_3->current.pos.z - param_2->current.pos.z;
        i_this->field_0x5b0 = cM_atan2s(xDiff, zDiff);
        s16 sVar4 = (20.0f ) * JMAFastSqrt(xDiff * xDiff + zDiff * zDiff)* l_HIO.mWidth;
        if (sVar4 > 0x1000) {
            sVar4 = 0x1000;
        } else if (sVar4 < -0x1000) {
            sVar4 = -0x1000;
        }
        cLib_addCalcAngleS2(&i_this->field_0x5b2, sVar4, 4, 0x2000);
    }
}

static void break_eff(obj_kbox_class* i_this) {
    static u16 const particle_id[3] = {
        0x82AB,
        0x82AC,
        0x82AD,
    };
    cXyz emitterPos(i_this->mActor.current.pos);
    cXyz particleScale(1.0f, 1.0f, 1.0f);
    J3DModelData* kibako_bmd = (J3DModelData*)dComIfG_getObjectRes("Always", "BreakWoodBox.bmd");
    JUT_ASSERT(146, kibako_bmd != NULL);
    JPABaseEmitter* pEmitter = dComIfGp_particle_set(0x82af, &emitterPos, NULL, NULL, 0xff,  &dPa_modelEcallBack::getEcallback(),
                                                             fopAcM_GetRoomNo(&i_this->mActor), NULL, NULL, &particleScale);
    dPa_modelEcallBack::setModel(pEmitter, kibako_bmd, i_this->mActor.tevStr, 3, NULL, 0, 0);
    for (int i = 0; i < 3; i++) {
        dComIfGp_particle_set(particle_id[i], &emitterPos, 0, &particleScale, 0xff, 0, 0xffffffff,
                              0, 0, 0);
    }
}

static int daObj_Kbox_Draw(obj_kbox_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    g_env_light.settingTevStruct(8, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mModel, &a_this->tevStr);
    J3DMaterial* material = i_this->mModel->getModelData()->getMaterialNodePointer(0);
    material->getTevKColor(1)->a = 125.0f - i_this->field_0x59c;
    mDoExt_modelUpdateDL(i_this->mModel);
    if (!fopAcM_checkCarryNow(a_this)) {
        cXyz cStack_38;
        cStack_38.set(a_this->current.pos.x,
                    a_this->current.pos.y + 50.0f, a_this->current.pos.z);
        i_this->mShadowId = dComIfGd_setShadow(
            i_this->mShadowId, 1, i_this->mModel, &cStack_38,
            500.0f, 0.0f, a_this->current.pos.y, i_this->mAcch.GetGroundH(),
            i_this->mAcch.m_gnd, &a_this->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }
    return 1;
}

static void kbox_ground(obj_kbox_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    i_this->mSph.OnCoSetBit();
    cLib_addCalc0(&a_this->speedF, 1.0f, 2.0f);
    cLib_onBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
    if (fopAcM_checkCarryNow(a_this)) {
        cLib_offBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
        i_this->field_0x57a = 1;
        i_this->field_0x57c = 0;
        a_this->speedF = 0.0f;
        i_this->mSph.OffCoSetBit();
    }
}

static void kbox_carry(obj_kbox_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    cXyz local_20;
    cXyz cStack_2c;
    if (!fopAcM_checkCarryNow(a_this)) {
        a_this->current.angle.y = player->shape_angle.y;
        if (fopAcM_GetSpeedF(a_this) > 1.0f ||
            fopAcM_GetSpeedF(player) > 10.0f)
        {
            a_this->speedF = 17.0f + (fopAcM_GetSpeedF(player) * 0.1f);
            a_this->speed.y = 20.0f;
            cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
            local_20.x = 0.0f;
            local_20.y = a_this->speed.y;
            local_20.z = a_this->speedF;
            MtxPosition(&local_20, &cStack_2c);
            a_this->current.pos += cStack_2c * 2.0f;
            i_this->field_0x57a = 2;
            i_this->field_0x57c = 0;
            i_this->field_0x57e[1] = 10;
            fopAcM_carryOffRevise(a_this);
        } else {
            a_this->speedF = 0.0f;
            a_this->speed.y = -10.0f;
            i_this->field_0x57a = 0;
            i_this->field_0x57c = 0;
        }
    }
}

static BOOL water_check(obj_kbox_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    dBgS_LinChk mLinChk;
    cXyz local_15c;
    local_15c.x = a_this->current.pos.x;
    local_15c.y = a_this->current.pos.y + 3000.0f;
    local_15c.z = a_this->current.pos.z;
    mLinChk.Set(&a_this->current.pos, &local_15c, a_this);
    if (dComIfG_Bgsp().LineCross(&mLinChk)) {
        local_15c.y =  mLinChk.GetCross().y - 10.0f;
    }
    dBgS_ObjGndChk_Spl mGndChkSpl;
    dBgS_ObjGndChk mGndChk;
    Vec VStack_168;
    VStack_168 = local_15c;
    mGndChkSpl.SetPos(&VStack_168);
    f32 dVar8 = dComIfG_Bgsp().GroundCross(&mGndChkSpl);
    mGndChk.SetPos(&VStack_168);
    f32 dVar9 = dComIfG_Bgsp().GroundCross(&mGndChk);
    if (dVar8 > dVar9) {
        i_this->field_0x588  = dVar8;
        if (i_this->field_0x588 > a_this->current.pos.y) {
            return TRUE;
        }
    }
    return FALSE;
}

static int kbox_drop(obj_kbox_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    BOOL rv = TRUE;
    if (a_this->speedF > 10.0f && i_this->field_0x57e[0] == 0 && i_this->mAcch.ChkWallHit()) {
        i_this->field_0x57e[0] = 0x28;
        i_this->field_0x594 = 2000.0f;
        a_this->speedF *= -0.5f;
    }

    if (i_this->mAcch.ChkGroundHit()) {
        if (i_this->field_0x57c == 0) {
            i_this->field_0x57c = 1;
            a_this->speed.y = 15.0f;
            i_this->field_0x594 = 1000.0f;
            a_this->speedF *= 0.4f;
        } else {
            cLib_addCalc0(&a_this->speedF, 1.0f, 2.0f);
            if (i_this->field_0x594 == 0.0f) {
                i_this->field_0x57a = 0;
                i_this->field_0x57c = 0;
            }
        }
    }
    a_this->shape_angle.x = i_this->field_0x594 * cM_ssin(i_this->field_0x578 * 9000);
    a_this->shape_angle.z = i_this->field_0x594 * cM_ssin(i_this->field_0x578 * 8000);
    cLib_addCalc0(&i_this->field_0x594, 1.0f, 100.0f);
    if (i_this->field_0x57e[1] != 0) {
        rv = FALSE;
    }

    return rv;
}

static void kbox_float(obj_kbox_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    cLib_addCalc0(&a_this->speed.y, 1.0f, 0.25f);
    a_this->current.pos += a_this->speed;
    a_this->speed.x *= 0.95f;
    a_this->speed.z *= 0.95f;
    cXyz cStack_70(a_this->current.pos);
    cStack_70.y = i_this->field_0x588;
    f32 dVar9 = 0.05f;
    if (i_this->field_0x598) {
        dVar9 = 0.1f;
        i_this->field_0x59c = fabsf(i_this->field_0x5ac) * 2.5f;
        if (i_this->field_0x59c > 125.0f) {
            i_this->field_0x59c = 125.0f;
        }
        cXyz cStack_7c(0.5f, 0.5f, 0.5f);
        cXyz cStack_88(a_this->current.pos);
        cStack_88.y +=
            i_this->field_0x5a4 + i_this->field_0x590 + i_this->field_0x58c + i_this->field_0x5ac;
        i_this->field_0x9e8 = dComIfGp_particle_set(i_this->field_0x9e8, 0x86c3,
                                                  &cStack_88, &a_this->tevStr, &a_this->shape_angle,
                                                  &cStack_7c, 0xff, 0, 0xffffffff, 0, 0, 0);
        if (i_this->field_0x5ac <= -50.0f) {
            fopAcM_delete(a_this);
            dComIfGp_particle_set(0x86c4, &cStack_88, &a_this->tevStr, &a_this->shape_angle,
                                  &cStack_7c);
        }
    }

    if (i_this->field_0x5b4.y + 60.0f > i_this->field_0x588) {
        fopAcM_effHamonSet(&i_this->field_0x9e0, &cStack_70, 2.0f, dVar9);
    }

    f32 dVar8 = i_this->field_0x594 * 0.004f;
    cLib_addCalc2(&a_this->current.pos.y,
                  (i_this->field_0x588 - 25.0f) + dVar8 * cM_ssin(i_this->field_0x578 * 900), 0.1f,
                  10.0f);
    a_this->shape_angle.x = i_this->field_0x594 * cM_ssin((i_this->field_0x578 * 1000));
    a_this->shape_angle.z = i_this->field_0x594 * cM_ssin((i_this->field_0x578 * 1100));
    cLib_addCalc2(&i_this->field_0x594, 500.0f, 0.1f, 30.0f);
    a_this->shape_angle.x += (s16)(i_this->field_0x598 * cM_ssin((i_this->field_0x578 * 4000)));
    a_this->shape_angle.z += (s16)(i_this->field_0x598 * cM_ssin((i_this->field_0x578 * 4200)));
    cLib_addCalc2(&i_this->field_0x598, 0, 0.1f, 30.0f);
    i_this->field_0x58c = 30.0f;
    cLib_addCalc2(&i_this->field_0x5a4, i_this->field_0x5a8, 0.2f, 20.0f);

    f32 fVar10;
    if (i_this->field_0x5a0 != 0) {
        fVar10 = -20.0f;
    } else {
        fVar10 = 0.0f;
    }
    cLib_addCalc2(&i_this->field_0x5a8, fVar10, 1.0f, 5.0f);
}

static void action(obj_kbox_class* i_this) {
    fopAc_ac_c* player;
    s8 bVar2;
    s8 bVar3;
    s8 bVar4;
    s8 bVar5;
    fopAc_ac_c* a_this;
    cXyz* local_64;
    int local_77;
    s8 local_78;
    BOOL local_68;
    int i;
    a_this = (fopAc_ac_c*)&i_this->mActor;
    player = static_cast<fopAc_ac_c*>(dComIfGp_getPlayer(0));
    cXyz local_38;
    cXyz local_44;
    bVar2 = 0;
    bVar3 = 0;
    bVar4 = 0;
    bVar5 = 0;
    local_77 = 1;
    local_78 = 0;
    f32 dVar10 = 0.0f;
    switch (i_this->field_0x57a) {
        case 0:
            kbox_ground(i_this);
            local_78 = 1;
            break;
        case 1:
            kbox_carry(i_this);
            bVar4 = 1;
            if (player->current.pos.y + 100.0f < a_this->current.pos.y) {
                dVar10 = TREG_F(2) + 10.0f;
            }
            local_77 = 0;
            break;
        case 2:
            local_77 = kbox_drop(i_this);
            local_78 = 2;
            break;
        case 3:
            kbox_float(i_this);
            bVar4 = 1;
            bVar5 = true;
            local_77 = 0;
            break;
    }
    cLib_addCalc2(&i_this->field_0x590, dVar10, 1.0f, 5.0f);
    if (!bVar4) {
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        local_38.x = 0.0f;
        local_38.y = 0.0f;
        local_38.z = a_this->speedF;
        MtxPosition(&local_38, &local_44);
        a_this->speed.x = local_44.x;
        a_this->speed.z = local_44.z;
        a_this->current.pos += a_this->speed;
        a_this->speed.y -= 4.0f;
        if (a_this->speed.y < -100.0f) {
            a_this->speed.y = -100.0f;
        }
    }
    if ((s8)local_77 != 0) {
        local_64 = i_this->mStts.GetCCMoveP();
        if (local_64 != NULL) {
            a_this->current.pos.x += local_64->x * 0.5f;
            a_this->current.pos.z += local_64->z * 0.5f;
        }
    }

    i_this->mAcch.CrrPos(dComIfG_Bgsp());
    local_68 = water_check(i_this);
    if (local_78 != 0 && local_68 != 0) {
        i_this->field_0x57a = 3;
        i_this->field_0x57c = 0;
        if (local_78 == 1) {
            i_this->field_0x594 = 1000.0f;
            cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
            local_38.x = 0.0f;
            local_38.y = -2.0f;
            local_38.z = 2.5f;
            MtxPosition(&local_38, &a_this->speed);
        } else {
            a_this->current.pos.y = i_this->field_0x588 - TREG_F(11);
            a_this->speed.y = -5.0f + TREG_F(12);
            a_this->speed.x *= 0.5f;
            a_this->speed.z *= 0.5f;
            i_this->field_0x594 = 3000.0f;
        }

        static cXyz sc(1.25f, 1.25f, 1.25f);
        cXyz cStack_50(a_this->current.pos);
        cStack_50.y = i_this->field_0x588;
        for (i = 0; i < 4; i++) {
            static u16 w_eff_id[4] = {
                0x01B8, 0x01B9, 0x01BA, 0x01BB,
            };

            i_this->field_0x9d0[i] = dComIfGp_particle_set(
                i_this->field_0x9d0[i],
                w_eff_id[i],
                &cStack_50, &a_this->tevStr, &a_this->shape_angle,
                &sc, 0xff, 0, -1, 0, 0, 0);
        }

        i_this->mSound.startSound(Z2SE_CM_BODYFALL_WATER_S, 0, -1);
    }
    mDoMtx_stack_c::transS(a_this->current.pos.x,
                           a_this->current.pos.y + i_this->field_0x5a4 + +i_this->field_0x590 +
                               i_this->field_0x58c + i_this->field_0x5ac,
                           a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->field_0x5b0);
    mDoMtx_stack_c::XrotM(i_this->field_0x5b2);
    mDoMtx_stack_c::YrotM((s16)-i_this->field_0x5b0);
    mDoMtx_stack_c::YrotM((s16)a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM((s16)a_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM((s16)a_this->shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, -i_this->field_0x58c, 0.0f);
    mDoMtx_stack_c::scaleM(0.5f, 0.5f, 0.5f);
    i_this->mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    local_38.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&local_38, &i_this->field_0x5b4);
    cXyz cStack_5c(i_this->field_0x5b4);
    cStack_5c.y += 15.0f + TREG_F(0);
    a_this->eyePos = cStack_5c;
    a_this->attention_info.position = cStack_5c;
    i_this->mSph.SetC(cStack_5c);
    i_this->mSph.SetR((40.0f + TREG_F(1)) * l_HIO.mWidth);
    if ((s8)local_77 != 0) {
        i_this->mSph.OnCoSetBit();
    } else {
        i_this->mSph.OffCoSetBit();
    }

    dComIfG_Ccsp()->Set(&i_this->mSph);
    if (!bVar5) {
        mDoMtx_stack_c::transM(0.0f, 20000.0f, 20000.0f);
    } else {
        mDoMtx_stack_c::scaleM(2.0f, 2.0f, 2.0f);
        mDoMtx_stack_c::transM(0.0f, 15.0f + JREG_F(17), 0.0f);
    }

    MTXCopy(mDoMtx_stack_c::get(), i_this->mBgMtx);
    i_this->mBgw->Move();
    if (i_this->field_0x5a0 == 0) {
        cLib_addCalcAngleS2(&i_this->field_0x5b2, 0, 4, 0x800);
    }
}

static int daObj_Kbox_Execute(obj_kbox_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    i_this->field_0x578++;
    for (int i = 0; i < 2; i = i + 1) {
        if (i_this->field_0x57e[i] != 0) {
            i_this->field_0x57e[i]--;
        }
    }
    if (i_this->field_0x5a0 != 0) {
        i_this->field_0x5a0--;
    }
    action(i_this);
    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    if (i_this->mSph.ChkTgHit()) {
        i_this->mAtInfo.mpCollider = i_this->mSph.GetTgHitObj();
        at_power_check(&i_this->mAtInfo);
        if (i_this->mAtInfo.mAttackPower >= 10) {
            fopAcM_delete(a_this);
            break_eff(i_this);
            dScnPly_c::setPauseTimer(3);
            mDoAud_seStart(Z2SE_OBJ_BREAK_WOOD_S, &a_this->current.pos, 0, 0);
        }
    }
    return 1;
}

static int daObj_Kbox_IsDelete(obj_kbox_class* i_this) {
    return 1;
}

static int daObj_Kbox_Delete(obj_kbox_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    fopAcM_GetID(a_this);
    dComIfG_resDelete(&i_this->mPhase, "Obj_kbox");
    if (i_this->mIsHIOOwner) {
        hio_set = false;
        mDoHIO_DELETE_CHILD(l_HIO.mId);
    }
    dComIfG_Bgsp().Release(i_this->mBgw);
    if (a_this->heap != NULL) {
        i_this->mSound.deleteObject();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    obj_kbox_class* i_this = (obj_kbox_class*)a_this;
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_kbox", 4);
    JUT_ASSERT(911, modelData != NULL);
    i_this->mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->mModel == NULL) {
        return 0;
    }

    a_this->model = i_this->mModel;
    i_this->mBgw = new dBgW();
    if (i_this->mBgw == NULL) {
        return 0;
    }

    if (i_this->mBgw->Set((cBgD_t*)dComIfG_getObjectRes("Obj_kbox", 7), 1, &i_this->mBgMtx) == 1) {
       return 0;
    }

    i_this->mBgw->SetCrrFunc(dBgS_MoveBGProc_Typical);
    i_this->mBgw->SetRideCallback(ride_call_back);
    return 1;
}

static int daObj_Kbox_Create(fopAc_ac_c* a_this) {
    obj_kbox_class* i_this = (obj_kbox_class*)a_this;
    fopAcM_ct(a_this, obj_kbox_class);
    int rv = dComIfG_resLoad(&i_this->mPhase, "Obj_kbox");
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_KBOX PARAM %x\n", fopAcM_GetParam(a_this));
        i_this->field_0x570 = fopAcM_GetParam(a_this) & 0xff;
        if (i_this->field_0x570 == 0xff) {
            i_this->field_0x570 = 0;
        }
        OS_REPORT("OBJ_KBOX//////////////OBJ_KBOX SET 1 !!\n");
        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0xc10)) {
            OS_REPORT("//////////////OBJ_KBOX SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_KBOX SET 2 !!\n");
        if (dComIfG_Bgsp().Regist(i_this->mBgw, a_this) != 0) {
            return cPhs_ERROR_e;
        }

        if (!hio_set) {
            i_this->mIsHIOOwner = true;
            hio_set = true;
            l_HIO.mId = mDoHIO_CREATE_CHILD("スカル魚専用木箱", &l_HIO);
        }
        fopAcM_SetMtx(a_this, i_this->mModel->getBaseTRMtx());
        fopAcM_SetMin(a_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(a_this, 200.0f, 200.0f, 200.0f);
        i_this->mStts.Init(200, 0, a_this);
        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };
        i_this->mSph.Set(cc_sph_src);
        i_this->mSph.SetStts(&i_this->mStts);
        i_this->mSph.OnTgNoHitMark();
        i_this->mAcch.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1,
                          &i_this->mAcchCir, fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(60.0f, 60.0f);
        a_this->attention_info.distances[fopAc_attn_CARRY_e] = 0x2c;
        i_this->mSound.init(&a_this->current.pos, 2);
        i_this->field_0x578 = cM_rndF(65536.0f);
        daObj_Kbox_Execute(i_this);
    }
    return rv;
}

static actor_method_class l_daObj_Kbox_Method = {
    (process_method_func)daObj_Kbox_Create,
    (process_method_func)daObj_Kbox_Delete,
    (process_method_func)daObj_Kbox_Execute,
    (process_method_func)daObj_Kbox_IsDelete,
    (process_method_func)daObj_Kbox_Draw,
};

actor_process_profile_definition g_profile_OBJ_KBOX = {
  fpcLy_CURRENT_e,        // mLayerID
  8,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_KBOX,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(obj_kbox_class), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  43,                     // mPriority
  &l_daObj_Kbox_Method,   // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
