/**
 * @file d_a_npc_worm.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_worm.h"

static int nodeCallBack(J3DJoint* joint, int param_2) {
    if (param_2 == 0) {
        int jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        npc_worm_class* i_this = (npc_worm_class*)model->getUserArea();
        if (i_this != NULL && jntNo >= 1 && jntNo <= 9) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            cMtx_YrotM(*calc_mtx, i_this->field_0x5b4[jntNo - 1]);
            model->setAnmMtx(jntNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return 1;
}

static int daNPC_WORM_Draw(npc_worm_class* i_this) {
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    if (i_this->field_0x5d4 > 0.01f) {
        g_env_light.setLightTevColorType_MAJI(i_this->mModel2, &i_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mModel2);
    }

    if (i_this->field_0x5e6 == 0) {
        return 1;
    }
    g_env_light.setLightTevColorType_MAJI(i_this->mModel1, &i_this->tevStr);
    i_this->mModel1->setUserArea((uintptr_t)i_this);
    for (u16 i = 0; i < i_this->mModel1->getModelData()->getJointNum(); i++) {
        if (i >= 1) {
            i_this->mModel1->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
    }
    mDoExt_modelUpdateDL(i_this->mModel1);
    return 1;
}

static void npc_worm_ground(npc_worm_class* i_this) {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    BOOL bVar2 = FALSE;
    switch(i_this->field_0x5a8) {
    case 0:
        i_this->field_0x5e6 = 0;
        i_this->field_0x5a8 = 1;
        i_this->current.pos = i_this->field_0x5d8;
        break;
    case 1:
        if (player->checkWolfDig()) {
            cXyz diff = player->getLeftHandPos() - i_this->current.pos;
            if (diff.abs() < 35.0f) {
                i_this->field_0x5e8[0] = cM_rndF(5.0f) + 10.0f;
                i_this->field_0x5a8 = 2;
                i_this->current.pos.x = player->getLeftHandPos().x + cM_rndFX(20.0f);
                i_this->current.pos.z = player->getLeftHandPos().z + cM_rndFX(20.0f);
            }
        }
        break;
    case 2:
        if (i_this->field_0x5e8[0] == 0) {
            bVar2 = TRUE;
        }
        break;
    }

    if (bVar2) {
        i_this->field_0x5a6 = 1;
        i_this->field_0x5a8 = 0;
        i_this->field_0x5d4 = 0.0f;
    }
}

static void npc_worm_normal(npc_worm_class* i_this) {
    fopAc_ac_c* a_this = i_this;
    switch(i_this->field_0x5a8) {
    case -1:
        a_this->current.angle.y = cM_rndF(65536.0f);
        a_this->speed.y = 0.0f;
        a_this->speedF = 0.0f;
        a_this->shape_angle.x = -cM_rndF(16000.0f);
        i_this->field_0x5d0 = 10.0f;
        i_this->field_0x5e8[1] = cM_rndF(60.0f) + 130.0f;
        i_this->field_0x5a8 = 1;
        i_this->field_0x5e6 = 1;
        break;
    case 0:
        a_this->current.pos.y += 3.0f;
        a_this->current.angle.y = cM_rndF(65536.0f);
        a_this->speed.y = cM_rndF(5.0f) + 15.0f;
        a_this->speedF = cM_rndF(2.0f) + 4.0f;
        a_this->shape_angle.x = -cM_rndF(16000.0f);
        i_this->field_0x5d0 = 10.0f;
        i_this->field_0x5e8[1] = cM_rndF(60.0f) + 130.0f;
        i_this->field_0x5a8 = 1;
        i_this->field_0x5e6 = 1;
    case 1:
        if (a_this->current.pos.y <= i_this->field_0x5cc &&
            i_this->field_0x5e8[0] == 0)
        {
            i_this->field_0x5ca = a_this->current.angle.y + (s16)cM_rndFX(20000.0f);
            i_this->field_0x5e8[0] = cM_rndF(10.0f) + 3.0f;
            a_this->speedF = cM_rndF(0.75f) + 0.5f;
        }

        if (a_this->eventInfo.checkCommandCatch()) {
            fopAcM_delete(a_this);
        } else {
            dComIfGp_att_CatchRequest(a_this, 0x74, 200.0f, 100.0f, -150.0f, 0x5000, 1);
            a_this->eventInfo.onCondition(dEvtCnd_40_e);
        }
        
        #if !DEBUG
        if (i_this->field_0x5e8[1] == 0) {
            i_this->field_0x5a6 = 2;
            i_this->field_0x5a8 = 0;
        }
        #endif

        break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5ca, 4, 0x400);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, 0, 2, 0x800);
}

static void npc_worm_dive(npc_worm_class* i_this) {
    switch(i_this->field_0x5a8) {
    case 0:
        i_this->field_0x5a8 = 1;
        i_this->field_0x5d8.x = i_this->current.pos.x;
        i_this->field_0x5d8.y = i_this->field_0x5cc;
        i_this->field_0x5d8.z = i_this->current.pos.z;
        break;
    case 1:
        if (i_this->field_0x5cc - i_this->current.pos.y > 60.0f) {
            i_this->field_0x5a6 = 0;
            i_this->field_0x5a8 = 0;
        }
        break;
    }

    i_this->shape_angle.y += 1000;
    cLib_addCalcAngleS2(&i_this->shape_angle.x, 0x3ff0, 8, 0x400);
    cXyz local_1c;
    cXyz cStack_28;
    mDoMtx_stack_c::XrotS(i_this->field_0x5c6);
    mDoMtx_stack_c::ZrotM(i_this->field_0x5c8);
    local_1c.x = 0.0f;
    local_1c.y = -1.0f;
    local_1c.z = 0.0f;
    mDoMtx_stack_c::multVec(&local_1c, &cStack_28);
    i_this->current.pos += cStack_28;
    i_this->field_0x5e4 += 0xa00;
    cLib_addCalc2(&i_this->field_0x5d4, 1.0f, 0.1f, 0.02f);
}

static void npc_worm_binwait(npc_worm_class* i_this) {
    dComIfGp_getPlayer(0);
    switch(i_this->field_0x5a8) {
    case 0:
        i_this->field_0x5e6 = 0;
        i_this->field_0x5a8 = 1;
        break;
    case 1:
        if (i_this->check_release()) {
            i_this->field_0x5a6 = 1;
            i_this->field_0x5a8 = -1;
        }
        break;
    }
}

static void action(npc_worm_class* i_this) {
    fopAc_ac_c* a_this = i_this;
    cXyz local_124;
    cXyz local_130;
    int local_140 = 0;
    BOOL bVar6 = false;
    BOOL bVar5 = FALSE;
    BOOL bVar7 = true;

    switch (i_this->field_0x5a6) {
    case 0:
        npc_worm_ground(i_this);
        bVar7 = FALSE;
        break;
    case 1:
        npc_worm_normal(i_this);
        local_140 = 1;
        bVar6 = TRUE;
        bVar5 = TRUE;
        break;
    case 2:
        npc_worm_dive(i_this);
        local_140 = 2;
        break;
    case 3:
        npc_worm_binwait(i_this);
        bVar7 = FALSE;
        break;
    }

    if (local_140 == 1) {
        i_this->field_0x5ac = 2000;
        i_this->field_0x5aa += i_this->field_0x5ac;
        i_this->field_0x5b0 = 8000.0f;
        i_this->field_0x5ae = cM_ssin(i_this->field_0x5a4 * 600) * 2000.0f + 4000.0f;

        for (int i = 0; i < 9; i++) {
            i_this->field_0x5b4[i] = i_this->field_0x5b0 * cM_ssin(i_this->field_0x5aa - i * i_this->field_0x5ae);
        }
        i_this->shape_angle.y = (int)(-(i_this->field_0x5b4[3]  * 2)) + a_this->current.angle.y;
        cLib_addCalc2(&i_this->field_0x5d0, 10.0f, 1.0f, 1.0f);
    } else if (local_140 == 2) {
        i_this->field_0x5aa += i_this->field_0x5ac;
        i_this->field_0x5b0 = 8000.0f;
        i_this->field_0x5ae = 3000;
        int local_154 = 0.17f * (i_this->field_0x5cc - a_this->current.pos.y);
        if (local_154 > 8) {
            local_154 = 8;
        }
        i_this->field_0x5ac = local_154 * 200 + 3500;
        for (int i = 0; i < 9; i++) {
            if (i >= local_154) {
                i_this->field_0x5b4[i] = (i_this->field_0x5b0 * cM_ssin(i_this->field_0x5aa - i * i_this->field_0x5ae));
            } else {
                cLib_addCalcAngleS2(&i_this->field_0x5b4[i], 0, 1, 1000);
            }
        }
        cLib_addCalc0(&i_this->field_0x5d0, 1.0f, 1.0f);
    }
    if (bVar5) {
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        local_124.x = 0.0f;
        local_124.y = 0.0f;
        local_124.z = a_this->speedF;
        MtxPosition(&local_124, &local_130);
        a_this->current.pos.x += local_130.x;
        a_this->current.pos.y += a_this->speed.y;
        a_this->current.pos.z += local_130.z;
        a_this->speed.y -= 5.0f;
        if (a_this->speed.y < -50.0f) {
            a_this->speed.y = -50.0f;
        }
        dBgS_LinChk linkChk;
        linkChk.Set(&a_this->old.pos, &a_this->current.pos, a_this);
        if (dComIfG_Bgsp().LineCross(&linkChk)) {
            a_this->current.pos.x = a_this->old.pos.x;
            a_this->current.pos.z = a_this->old.pos.z;
        }
    }
    if (bVar6) {
        dBgS_GndChk gndchk;
        cXyz local_13c(a_this->current.pos);
        local_13c.y += 50.0f;
        gndchk.SetPos(&local_13c);
        i_this->field_0x5cc = dComIfG_Bgsp().GroundCross(&gndchk) + 3.0f;
        if (a_this->current.pos.y <= i_this->field_0x5cc) {
            a_this->current.pos.y = i_this->field_0x5cc;
            a_this->speed.y = -10.0f;
            local_13c = a_this->current.pos;
            local_13c.y += 50.0f;
            local_13c.z += 10.0f;
            gndchk.SetPos(&local_13c);
            local_13c.y = dComIfG_Bgsp().GroundCross(&gndchk) + 3.0f;
            i_this->field_0x5c6 = -cM_atan2s(local_13c.y - a_this->current.pos.y,
                                             local_13c.z - a_this->current.pos.z);
            if (i_this->field_0x5c6 > 0x3000 || i_this->field_0x5c6 < -0x3000) {
                i_this->field_0x5c6 = 0;
            }
            local_13c = a_this->current.pos;
            local_13c.y += 50.0f;
            local_13c.x += 10.0f;
            gndchk.SetPos(&local_13c);
            local_13c.y = dComIfG_Bgsp().GroundCross(&gndchk) + 3.0f;
            i_this->field_0x5c8 =
                cM_atan2s(local_13c.y - a_this->current.pos.y, local_13c.x - a_this->current.pos.x);
            if (i_this->field_0x5c8 > 0x3000 || i_this->field_0x5c8 < -0x3000) {
                i_this->field_0x5c8 = 0;
            }
        }
    }
    if (bVar7 && fopAcM_searchPlayerDistance(a_this) < 200.0f) {
        daPy_py_c::setLookPos(&a_this->current.pos);
    }
}

static int daNPC_WORM_Execute(npc_worm_class* i_this) {
    i_this->field_0x5a4++;
    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x5e8[i] != 0) {
            i_this->field_0x5e8[i]--;
        }
    }
    action(i_this);
    if (i_this->field_0x5e6 != 0) {
        mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y,
                               i_this->current.pos.z - i_this->field_0x5d0);
        mDoMtx_stack_c::XrotM(i_this->field_0x5c6);
        mDoMtx_stack_c::ZrotM(i_this->field_0x5c8);
        mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
        mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
        mDoMtx_stack_c::transM(0.0f, 0.0f, i_this->field_0x5d0);
        mDoMtx_stack_c::scaleM(0.1f, 0.1f, 0.1f);
        i_this->mModel1->setBaseTRMtx(mDoMtx_stack_c::get());
        fopAcM_SetMtx(i_this, i_this->mModel1->getBaseTRMtx());
    }
    if (i_this->field_0x5d4 > 0.01f) {
        mDoMtx_stack_c::transS(i_this->field_0x5d8.x, i_this->field_0x5d8.y - 2.5f,
                               i_this->field_0x5d8.z);
        mDoMtx_stack_c::XrotM(i_this->field_0x5c6);
        mDoMtx_stack_c::ZrotM(i_this->field_0x5c8);
        mDoMtx_stack_c::YrotM(i_this->field_0x5e4);
        f32 scale = i_this->field_0x5d4 * 0.08f;
        mDoMtx_stack_c::scaleM(scale, scale, scale * 1.1f);
        mDoMtx_stack_c::YrotM(-i_this->field_0x5e4);
        i_this->mModel2->setBaseTRMtx(mDoMtx_stack_c::get());
        fopAcM_SetMtx(i_this, i_this->mModel2->getBaseTRMtx());
    }
    return 1;
}

static int daNPC_WORM_IsDelete(npc_worm_class* i_this) {
    return 1;
}

static int daNPC_WORM_Delete(npc_worm_class* i_this) {
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, "Alink");
    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    npc_worm_class* i_this = (npc_worm_class*)a_this;
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Alink", 0x30);
    JUT_ASSERT(776, modelData != NULL);
    i_this->mModel1 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->mModel1 == NULL) {
        return 0;
    }
    modelData = (J3DModelData*)dComIfG_getObjectRes("Alink", 0x31);
    JUT_ASSERT(794, modelData != NULL);
    i_this->mModel2 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->mModel2 == NULL) {
        return 0;
    }
    return 1;
}

static int daNPC_WORM_Create(fopAc_ac_c* a_this) {
    fopAcM_ct(a_this, npc_worm_class);
    npc_worm_class* i_this = (npc_worm_class*)a_this;
    int rv =
        dComIfG_resLoad(&i_this->mPhase, "Alink");
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("NPC_WORM PARAM %x\n", fopAcM_GetParam(i_this));
        i_this->field_0x598 = fopAcM_GetParam(i_this);
        OS_REPORT("NPC_WORM//////////////NPC_WORM SET 1 !!\n");
        if (fopAcM_entrySolidHeap(i_this, useHeapInit, 0x1380) == 0) {
            OS_REPORT("//////////////NPC_WORM SET NON !!\n");
            return cPhs_ERROR_e;
        }
        OS_REPORT("//////////////NPC_WORM SET 2 !!\n");
        i_this->field_0x5a4 = cM_rndF(65535.0f);
        if (i_this->field_0x598 == 1) {
            i_this->field_0x5a6 = 1;
        } else if (i_this->field_0x598 == 2) {
            i_this->field_0x5a6 = 3;
        } else {
            i_this->field_0x5a6 = 0;
            dBgS_GndChk gndchk;
            cXyz cStack_84(i_this->current.pos);
            cStack_84.y += 100.0f;
            gndchk.SetPos(&cStack_84);
            f32 fVar1 = dComIfG_Bgsp().GroundCross(&gndchk) + 3.0f;
            i_this->field_0x5cc = fVar1;
            i_this->current.pos.y = fVar1;
            i_this->field_0x5d8.x = i_this->current.pos.x;
            i_this->field_0x5d8.y = i_this->field_0x5cc;
            i_this->field_0x5d8.z = i_this->current.pos.z;
            i_this->field_0x5d4 = 1.0f;
        }
        daNPC_WORM_Execute(i_this);
    }
    return rv;
}

static actor_method_class l_daNPC_WORM_Method = {
    (process_method_func)daNPC_WORM_Create,
    (process_method_func)daNPC_WORM_Delete,
    (process_method_func)daNPC_WORM_Execute,
    (process_method_func)daNPC_WORM_IsDelete,
    (process_method_func)daNPC_WORM_Draw,
};

actor_process_profile_definition g_profile_NPC_WORM = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_WORM,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(npc_worm_class), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  429,                    // mPriority
  &l_daNPC_WORM_Method,   // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_0_e,      // cullType
};
