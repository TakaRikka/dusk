/**
 * @file d_a_obj_msima.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_msima.h"
#include "Z2AudioLib/Z2Instances.h"
#include "c/c_damagereaction.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_midna.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"

daObj_Msima_HIO_c::daObj_Msima_HIO_c() {
    mId = -1;
    mSize = 1.0f;
    mMaxTiltAngle = 0xc00;
    mMaxSlopeAngle = 0;
    mBasicPosition = 800.0f;
}

#if DEBUG
void daObj_Msima_HIO_c::genMessage(JORMContext* context) {
    // Magma Island
    context->genLabel("　マグネ島　", 0x80000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Size
    context->genSlider("サイズ", (float*)(this + 8), 0.0, 3.0, 0, NULL, 0xffff, 0xffff, 0x200,
                       0x18);
    // Maximum Tilt Angle 
    context->genSlider("最大傾斜角（浮）", &mMaxTiltAngle, 0, 0x2000, 0, NULL, 0xffff, 0xffff,
                       0x200, 0x18);
    // Maximum Slope Angle (Up)
    context->genSlider("最大傾斜角（上）", &mMaxSlopeAngle, 0, 0x2000, 0, NULL, 0xffff, 0xffff,
                       0x200, 0x18);
    // Basic Position
    context->genSlider("基本位置", &mBasicPosition, -5000.0, 5000.0, 0, NULL, 0xffff, 0xffff, 0x200,
                       0x18);
}
#endif

static void ride_call_back(dBgW* i_bgw, fopAc_ac_c* a_this, fopAc_ac_c* param_3) {
    if (fopAcM_GetName(param_3) == PROC_ALINK || fopAcM_GetName(param_3) == PROC_E_GOB ||
    fopAcM_GetName(param_3) == PROC_B_ZANT)
    {
        int idx = 0;
        if (fopAcM_GetName(param_3) != PROC_ALINK) {
            idx = 1;
        }
        
        obj_msima_class* i_this = (obj_msima_class*)a_this;
        i_this->field_0x5bc[idx] = fopAcM_GetID(param_3);
        i_this->field_0x5c4[idx] = 3;
    }
}

static int daObj_Msima_Draw(obj_msima_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    J3DModel* pModel;
    int i;
    int j;
    g_env_light.settingTevStruct(0x10, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &a_this->tevStr);
    dComIfGd_setListBG();
    i_this->mpBtk->entry(i_this->mpModel->getModelData());
    i_this->mpBrk->entry(i_this->mpModel->getModelData());
    mDoExt_modelUpdateDL(i_this->mpModel);
    dComIfGd_setList();
    if (i_this->field_0x570 != 1) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 35; j++) {
                pModel = i_this->mChains[i].model[j];
                g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
                g_env_light.setLightTevColorType_MAJI(pModel, &a_this->tevStr);
                mDoExt_modelUpdateDL(pModel);
            }
        }
    }
    return 1;
}

static void msima_start(obj_msima_class* i_this) {
    i_this->field_0x598 = 0x802;
    i_this->field_0x59a = 0x708;
}

/* 80C9CEA4 0001+00 data_80C9CEA4 @1009 */
/* 80C9CEA5 0003+00 data_80C9CEA5 None */
static bool l_HIOInit;

static daObj_Msima_HIO_c l_HIO;

static cXyz c_pos(0.0f,0.0f,0.0f);

static void msima_drop(obj_msima_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    a_this->shape_angle.x = -i_this->field_0x59c * cM_ssin(i_this->field_0x594);
    a_this->shape_angle.z = -i_this->field_0x59c * cM_ssin(i_this->field_0x596);
    cLib_addCalc2(&i_this->field_0x59c, 300.0f, 0.1f, 30.0f);

    switch(i_this->field_0x584) {
    case 0:
         if (i_this->field_0x586[0] == 0) {
            a_this->current.pos.y += a_this->speed.y;
            a_this->speed.y -= 5.0f;
            if (a_this->speed.y <= -80.0f) {
                a_this->speed.y = -80.0f;
            }
        }

        if (a_this->current.pos.y < i_this->field_0x58c + l_HIO.mBasicPosition) {
            i_this->field_0x584 = 1;
            i_this->field_0x59c = 2000.0f;
            cXyz cStack_38(a_this->current.pos);
            cStack_38.y = 0.0f;
            dComIfGp_particle_set(0x8456, &cStack_38, 0, 0);
            dComIfGp_particle_set(0x8457, &cStack_38, 0, 0);
            dComIfGp_particle_set(0x8458, &cStack_38, 0, 0);
            mDoAud_seStart(Z2SE_OBJ_GOBFLOOR_INTO_MAGMA, &c_pos, 0,
                           dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
        }
        break;
    case 1:
        break;
    }
}

static void msima_float(obj_msima_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    f32 dVar7 = i_this->field_0x59c * 0.004f;
    if (i_this->field_0x570 != 1) {
        cLib_addCalc2(&a_this->current.pos.y,
                      (i_this->field_0x58c + l_HIO.mBasicPosition + dVar7 * cM_ssin(i_this->mFloatingOscillator * 400)), 0.1f,
                      30.0f);
    }
    a_this->shape_angle.x = -i_this->field_0x59c * cM_ssin(i_this->field_0x594);
    a_this->shape_angle.z = -i_this->field_0x59c * cM_ssin(i_this->field_0x596);
    cLib_addCalcAngleS2(&i_this->field_0x598, 350, 1, 10);
    cLib_addCalcAngleS2(&i_this->field_0x59a, 300, 1, 10);
    cLib_addCalc2(&i_this->field_0x59c, 300.0f, 0.1f, 30.0f);
    cLib_addCalc2(&i_this->field_0x5a4, i_this->field_0x5a8, 0.2f, 20.0f);
    
    f32 fVar1;
    if (i_this->field_0x5a0 != 0) {
        fVar1 = -20.0f;
    } else {
        fVar1 = 0.0f;
    }

    cLib_addCalc2(&i_this->field_0x5a8, fVar1, 1.0f, 5.0f);
}

static void action(obj_msima_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz cStack_70;
    cXyz cStack_7c;
    i_this->field_0x594 += i_this->field_0x598;
    i_this->field_0x596 += i_this->field_0x59a;

    switch (i_this->mAction) {
    case obj_msima_class::ACTION_START:
        msima_start(i_this);
        break;
    case obj_msima_class::ACTION_DROP:
        msima_drop(i_this);
        break;
    case obj_msima_class::ACTION_FLOAT_1:
    case obj_msima_class::ACTION_FLOAT_2:
        msima_float(i_this);
        break;
    }

    mDoMtx_stack_c::transS(a_this->current.pos.x,
                           a_this->current.pos.y + i_this->field_0x5a4 + 30.0f,
                           a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->field_0x5ac);
    mDoMtx_stack_c::XrotM(i_this->field_0x5ae);
    mDoMtx_stack_c::YrotM(-i_this->field_0x5ac);
    mDoMtx_stack_c::YrotM((s16)a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM((s16)a_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM((s16)a_this->shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, -30.0f, 0.0f);
    mDoMtx_stack_c::scaleM(l_HIO.mSize, l_HIO.mSize, l_HIO.mSize);
    i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpBrk->play();
    i_this->mpBtk->play();
    cStack_70.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&cStack_70, &i_this->field_0x5b0);
    cXyz cStack_88(i_this->field_0x5b0);
    cStack_88.y += TREG_F(0) + 15.0f;
    a_this->eyePos = cStack_88;
    a_this->attention_info.position = cStack_88;
    MTXCopy(mDoMtx_stack_c::get(), i_this->mBgMtx);
    i_this->mpBgW->Move();
    cXyz cStack_94;
    s8 local_c8 = 0;
    if (daPy_getPlayerActorClass()->checkMagneBootsOn()) {
        i_this->field_0x5c4[0] = 3;
        i_this->field_0x5bc[0] = fopAcM_GetID(player);
    }
    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x5c4[i] != 0) {
            i_this->field_0x5c4[i]--;
            fopAc_ac_c* iVar11 = fopAcM_SearchByID(i_this->field_0x5bc[i]);
            if (iVar11 != 0) {
                if (i_this->field_0x5a0 == 0) {
                    i_this->field_0x5a8 = AREG_F(0) + -50.0f;
                }

                i_this->field_0x5a0 = 3;
                if (local_c8 != 0) {
                    cStack_94 = cStack_94 + (iVar11->current.pos - cStack_94) * 0.5f;
                } else {
                    cStack_94 = iVar11->current.pos;
                }
                local_c8++;
            }
        }
    }

    if (local_c8 != 0) {
        f32 fVar1 = 1.0f;
        s16 local_c4 = 0x400;
        if (i_this->field_0x59c > 400.0f) {
            local_c4 = 0x4000;
            fVar1 = 1.5f;
        }
        f32 dVar16 = (cStack_94.x - a_this->current.pos.x);
        f32 dVar15 = (cStack_94.z - a_this->current.pos.z);
        cLib_addCalcAngleS2(&i_this->field_0x5ac, cM_atan2s(dVar16, dVar15), 8, local_c4);
        s16 local_c6 = l_HIO.mSize * ((TREG_F(3) + 2.5f) * JMAFastSqrt(dVar16 * dVar16 + dVar15 * dVar15));
        s16 local_50 = l_HIO.mMaxTiltAngle * fVar1;

        if (i_this->mAction == obj_msima_class::ACTION_START) {
            local_50 = l_HIO.mMaxSlopeAngle;
        } else {
            if (i_this->mAction == obj_msima_class::ACTION_FLOAT_2) {
                local_50 = 0;
            }
        }

        if (local_c6 > (s16)local_50) {
            local_c6 = local_50;
        } else if (local_c6 < (s16)-local_50) {
            local_c6 = -local_50;
        }
        cLib_addCalcAngleS2(&i_this->field_0x5ae, local_c6, 8, TREG_S(7) + 0x400);
    } else {
        cLib_addCalcAngleS2(&i_this->field_0x5ae, 0, 4, 0x800);
    }
}

static cXyz chain_p[4] = {
    cXyz(0.0f,200.0f,1100.0f),
    cXyz(-1100.0f,200.0f,0.0f),
    cXyz(0.0f,200.0f,-1100.0f),
    cXyz(1100.0f,200.0f,0.0f),
};

static void chain_move(obj_msima_class* i_this, ms_chain_s* param_2, int param_3) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    cXyz cStack_4c;
    f32 scale = 2.95f;
    MTXCopy(i_this->mpModel->getBaseTRMtx(), mDoMtx_stack_c::get());
    cXyz cStack_58(chain_p[param_3]);
    mDoMtx_stack_c::multVec(&cStack_58, &param_2->mPos);
    mDoMtx_stack_c::transS(param_2->mPos.x, param_2->mPos.y, param_2->mPos.z);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y + (param_3 << 14));
    s16 zRot = 0;
    for (int i = 0; i < 35; i++) {
        zRot += (s16)((s16)cM_rndFX2(3000.0f) + 0x4000);
        mDoMtx_stack_c::push();
        mDoMtx_stack_c::scaleM(scale, scale, scale);
        mDoMtx_stack_c::XrotM(0x4000);
        mDoMtx_stack_c::ZrotM(zRot);
        param_2->model[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoMtx_stack_c::pop();
        mDoMtx_stack_c::transM(0.0f, 55.0f, 0.0f);
    }
}

static cXyz chain_p2[4] = {
    cXyz(0.0f,4000.0f,1100.0f),
    cXyz(-1100.0f,4000.0f,0.0f),
    cXyz(0.0f,4000.0f,-1100.0f),
    cXyz(1100.0f,4000.0f,0.0f),
};

static void chain_move2(obj_msima_class* i_this, ms_chain_s* param_2, int param_3) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    cXyz local_5c;
    s16 zRot = 0;
    param_2->field_0x90++;
    f32 scale = 2.95f;
    cXyz cStack_58(chain_p2[param_3]);
    mDoMtx_stack_c::YrotS(a_this->shape_angle.y);
    mDoMtx_stack_c::multVec(&cStack_58, &local_5c);
    if (i_this->mAction == obj_msima_class::ACTION_FLOAT_2) {
        local_5c.y += 250.0f;
    }
    mDoMtx_stack_c::transS(local_5c.x, local_5c.y, local_5c.z);
    f32 dVar10 = 1.0f;
    for (int i = 0; i < 35; i++) {
        zRot += (s16)((s16)cM_rndFX2(3000.0f) + 0x4000);
        if (i > 5) {
            dVar10 = 0.8f * (i - 5) + 1.0f;
        }
        s16 iVar1 = (dVar10 * (param_2->field_0x8c * cM_ssin(param_2->field_0x90 * 2000 + i * -1200))) * 0.3f;
        s16 iVar2 = dVar10 * (param_2->field_0x8c * cM_ssin(param_2->field_0x90 * 2200 + i * -1200));
        mDoMtx_stack_c::XrotM(iVar1);
        mDoMtx_stack_c::ZrotM(iVar2);
        mDoMtx_stack_c::push();
        mDoMtx_stack_c::scaleM(scale, scale, scale);
        mDoMtx_stack_c::XrotM(0x4000);
        mDoMtx_stack_c::ZrotM(zRot);
        param_2->model[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoMtx_stack_c::pop();
        mDoMtx_stack_c::transM(0.0f, -55.0f, 0.0f);
    }
    cLib_addCalc0(&param_2->field_0x8c, 0.03f, 20.0f);
}

static int daObj_Msima_Execute(obj_msima_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    if (dComIfGp_event_runCheck()) {
        if (fopAcM_getTalkEventPartner(daPy_getLinkPlayerActorClass()) == daPy_py_c::getMidnaActor()) {
            return 1;
        }
    }
    i_this->mFloatingOscillator++;
    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x586[i] != 0)
        {
            i_this->field_0x586[i]--;
        }
    }

    if (i_this->field_0x5a0 != 0) {
        i_this->field_0x5a0--;
    }

    action(i_this);
    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    cM_initRnd2(0xc, 0x7b, 0x32);
    if (i_this->field_0x570 != 1) {
        for (int i = 0; i < 4; i++) {
            if (i_this->mChains[i].field_0x92 != 0) {
                chain_move2(i_this, &i_this->mChains[i], i);
            } else {
                chain_move(i_this, &i_this->mChains[i], i);
            }
        }
    }
    return 1;
}

static int daObj_Msima_IsDelete(obj_msima_class* i_this) {
    return 1;
}

static int daObj_Msima_Delete(obj_msima_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    fopAcM_GetID(a_this);
    dComIfG_resDelete(&i_this->mPhase, "MAGNESIMA");
    if (i_this->mIsHIOOwner) {
        l_HIOInit = false;
        mDoHIO_DELETE_CHILD(l_HIO.mId);
    }
    dComIfG_Bgsp().Release(i_this->mpBgW);
    if (a_this->heap != NULL) {
        i_this->mSound.deleteObject();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    obj_msima_class* i_this = (obj_msima_class*)a_this;
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("MAGNESIMA", obj_msima_class::BMDR_S_MAGNE_SIMA);
    JUT_ASSERT(847, modelData != NULL);
    i_this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (i_this->mpModel == NULL) {
        return 0;
    }

    i_this->mpBgW = new dBgW();
    if (i_this->mpBgW == NULL) {
        return 0;
    }
    
    cBgD_t* bgd = (cBgD_t*)dComIfG_getObjectRes("MAGNESIMA", obj_msima_class::DZB_S_MAGNE_SIMA);
    if (i_this->mpBgW->Set(bgd, 1, &i_this->mBgMtx) == 1) {
        return 0;
    }

    i_this->mpBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);
    i_this->mpBgW->SetRideCallback(ride_call_back);

    i_this->mpBtk = new mDoExt_btkAnm();
    if (i_this->mpBtk == NULL) {
        return 0;
    }

    if (!i_this->mpBtk->init(i_this->mpModel->getModelData(), (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("MAGNESIMA", obj_msima_class::BTK_S_MAGNE_SIMA), 1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    i_this->mpBrk = new mDoExt_brkAnm();
    if (i_this->mpBrk == NULL) {
        return 0;
    }

    if (!i_this->mpBrk->init(i_this->mpModel->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes("MAGNESIMA", obj_msima_class::BRK_S_MAGNE_SIMA), 1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    if (i_this->field_0x570 != 1) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 35; j++) {
                modelData = (J3DModelData*)dComIfG_getObjectRes("MAGNESIMA", obj_msima_class::BMDR_KUSARI);
                JUT_ASSERT(898, modelData != NULL);
                i_this->mChains[i].model[j] =
                    mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
                if (i_this->mChains[i].model[j] == NULL) {
                    return 0;
                }
            }
            i_this->mChains[i].field_0x90 = cM_rndF(65536.0f);
        }
    }
    return 1;
}

static int daObj_Msima_Create(fopAc_ac_c* a_this) {
    obj_msima_class* i_this = (obj_msima_class*)a_this;
    fopAcM_ct(a_this, obj_msima_class);

    int rv = dComIfG_resLoad(&i_this->mPhase, "MAGNESIMA");
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_MSIMA PARAM %x\n", fopAcM_GetParam(a_this));
        i_this->field_0x570 = fopAcM_GetParam(a_this) & 0xff;
        if (i_this->field_0x570 == 0xff) {
            i_this->field_0x570 = 0;
        }

        OS_REPORT("OBJ_MSIMA//////////////OBJ_MSIMA SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x4b000)) {
            OS_REPORT("//////////////OBJ_MSIMA SET NON !!\n");
            return cPhs_ERROR_e;
        } 

        OS_REPORT("//////////////OBJ_MSIMA SET 2 !!\n");

        if (dComIfG_Bgsp().Regist(i_this->mpBgW, a_this) != 0) {
            return cPhs_ERROR_e;
        }

        if (!l_HIOInit) {
            i_this->mIsHIOOwner = true;
            l_HIOInit = 1;
            // Magma Island
            l_HIO.mId = mDoHIO_CREATE_CHILD("マグネ島", &l_HIO);
        }

        i_this->mSound.init(&a_this->current.pos, 2);
        i_this->mFloatingOscillator = cM_rndF(65536.0f);
        if (i_this->field_0x570 != 1) {
            if (dComIfGs_isStageMiddleBoss()) {
                i_this->mAction = obj_msima_class::ACTION_FLOAT_2;
                i_this->field_0x58c = 1150.0f;
                for (int i = 0; i < 4; i++) {
                    i_this->mChains[i].field_0x92 = 1;
                }
                a_this->current.pos.y = i_this->field_0x58c + l_HIO.mBasicPosition;
            } else {
                i_this->field_0x58c = a_this->home.pos.y;
                if (cDmr_SkipInfo != 0 || dComIfGs_isZoneSwitch(5, 0xffffffff)) {
                    a_this->current.pos.x = 0.0f;
                    a_this->current.pos.y = 801.0f;
                    a_this->current.pos.z = 0.0f;
                    for (int i = 0; i < 4; i++) {
                        i_this->mChains[i].field_0x92 = 1;
                    }
                    i_this->mAction = obj_msima_class::ACTION_FLOAT_1;
                } else {
                    i_this->mAction = obj_msima_class::ACTION_START;
                    a_this->current.pos.y = i_this->field_0x58c + 1900.0f;
                }
            }
            a_this->shape_angle.y = 0x2000;
            daObj_Msima_Execute(i_this);
            cXyz cStack_2c(0.0f, 0.0f, 0.0f);
            fopAcM_createChild(PROC_OBJ_MYOGAN, fopAcM_GetID(a_this), 0, &cStack_2c,
                               fopAcM_GetRoomNo(a_this), 0, 0, -1, 0);
        } else {
            for (int i = 0; i < 4; i++) {
                i_this->mChains[i].field_0x92 = 1;
            }
            i_this->mAction = obj_msima_class::ACTION_FLOAT_1;
        }
        OS_REPORT("//////////////OBJ_MSIMA SET 3 !!\n");
    }
    return rv;
}

static actor_method_class l_daObj_Msima_Method = {
    (process_method_func)daObj_Msima_Create,
    (process_method_func)daObj_Msima_Delete,
    (process_method_func)daObj_Msima_Execute,
    (process_method_func)daObj_Msima_IsDelete,
    (process_method_func)daObj_Msima_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_MSIMA = {
  fpcLy_CURRENT_e,         // mLayerID
  3,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_OBJ_MSIMA,          // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(obj_msima_class), // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  51,                      // mPriority
  &l_daObj_Msima_Method,   // sub_method
  0x00044000,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
