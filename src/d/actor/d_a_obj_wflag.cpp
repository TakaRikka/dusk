//
// Object wflag
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_wflag.h"
#include "d/d_com_inf_game.h"
#include "SSystem/SComponent/c_math.h"

static int daObj_Wflag_Draw(obj_wflag_class* i_this) {
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &i_this->tevStr);
    mDoExt_modelUpdateDL(i_this->mpModel);
    J3DModel* morfModel = i_this->mMorf->getModel();
    g_env_light.setLightTevColorType_MAJI(morfModel, &i_this->tevStr);
    i_this->field_0x578->entry(morfModel->getModelData());
    i_this->field_0x57c->entry(morfModel->getModelData());
    i_this->mMorf->entryDL();
    for (int i = 0; i < 1; i++) {
        wf_tail_s* tail = &i_this->mTails[i];
        for (int j = 0; j < 19; j++) {
            g_env_light.setLightTevColorType_MAJI(tail->mModels[j], &i_this->tevStr);
            mDoExt_modelUpdateDL(tail->mModels[j]);
        }
    }
    return 1;
}

static f32 pd[19] = {1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f, 0.4f, 0.3f, 0.2f, 0.2f,
                     0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f};

static void tail_control(fopAc_ac_c* param_1, wf_tail_s* param_2) {
    cXyz local_74;
    cXyz cStack_80;
    int i;
    cXyz* ptr;
    f32 dVar11;
    cXyz local_98;
    cXyz local_a4;
    f32 dVar9;
    f32 dVar14;
    f32 dVar13;
    f32 speed;
    csXyz* pAngle;

    ptr = &param_2->field_0x004[1];
    pAngle = &param_2->field_0x0f4[1];
    dVar11 = -20.0f;
    cMtx_YrotS(*calc_mtx, param_1->current.angle.y);
    local_74.x = 0.0f;
    local_74.y = 0.0f;
    local_74.z = -(20.0f + param_1->speedF * 10.0f);
    MtxPosition(&local_74, &local_98);
    speed = param_1->speedF * 2.0f;
    local_74.x = 0.0f;
    local_74.y = 0.0f;
    local_74.z = 20.0f;
    for (i = 1; i < 20; i++, ptr++, pAngle++) {
        local_a4.x =
            local_98.x * pd[i - 1];
        local_a4.z =
            local_98.z * pd[i - 1];
        local_a4.x += (speed * cM_ssin(param_2->field_0x000 * 5400 + i * (-3700))) * 0.3f;
        local_a4.z += (speed * cM_ssin(param_2->field_0x000 * 5700 + i * (-4200))) * 0.3f;
        local_a4.y = speed * cM_ssin(param_2->field_0x000 * 4200 + i * (-2400));
        dVar9 = local_a4.x + (ptr->x - ptr[-1].x);
        dVar14 = local_a4.y + (dVar11 + (ptr[0].y - ptr[-1].y));
        dVar13 = local_a4.z + (ptr->z - ptr[-1].z);
        s16 xRot = -cM_atan2s(dVar14, dVar13);
        s32 yRot = cM_atan2s(dVar9, JMAFastSqrt(dVar14 * dVar14 + dVar13 * dVar13));
        cMtx_XrotS(*calc_mtx, xRot);
        cMtx_YrotM(*calc_mtx, yRot);
        MtxPosition(&local_74, &cStack_80);
        pAngle[-1].x = xRot;
        pAngle[-1].y = yRot;
        *ptr = ptr[-1] + cStack_80;
    }
}

static void tail_mtxset(wf_tail_s* i_tail) {
    for (int i = 0; i < 19; i++) {
        MtxTrans(i_tail->field_0x004[i].x, i_tail->field_0x004[i].y, i_tail->field_0x004[i].z, 0);
        cMtx_XrotM(*calc_mtx, i_tail->field_0x0f4[i].x);
        cMtx_YrotM(*calc_mtx, i_tail->field_0x0f4[i].y);
        cMtx_XrotM(*calc_mtx, -0x8000);
        cMtx_ZrotM(*calc_mtx, i * 0x2734);
        i_tail->mModels[i]->setBaseTRMtx(*calc_mtx);
    }
}

static int daObj_Wflag_Execute(obj_wflag_class* i_this) {
    cXyz cStack_3c;
    i_this->field_0x836++;
    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x830[i] != 0) {
            i_this->field_0x830[i]--;
        }
    }
    if (i_this->field_0x834 != 0) {
        i_this->field_0x834--;
    }
    fopAc_ac_c* actor = fopAcM_SearchByID(i_this->parentActorID);
    if (actor != NULL) {
        i_this->current.angle.y = actor->shape_angle.y;
        i_this->speedF = actor->speedF;
        // TODO: Figure out what the actor class is
        MTXCopy((*(mDoExt_McaMorfSO**)((char*)actor + 0x5e0))->getModel()->getAnmMtx(16), *calc_mtx);
        cMtx_XrotM(*calc_mtx, (i_this->speedF * cM_ssin(i_this->field_0x836 * 5400)) * 5.0f);
        i_this->mpModel->setBaseTRMtx(*calc_mtx);
    } else {
         mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
        mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
        i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
    MTXCopy(i_this->mpModel->getBaseTRMtx(), mDoMtx_stack_c::get());
    mDoMtx_stack_c::transM(133.0f, 248.0f, 0.0f);
    mDoMtx_stack_c::YrotM(0x4000);
    mDoMtx_stack_c::XrotM(2100);
    mDoMtx_stack_c::ZrotM(0);
    i_this->mMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->field_0x578->play();
    i_this->field_0x57c->play();
    i_this->mMorf->play(NULL, 0, 0);
    i_this->mMorf->modelCalc();
    MTXCopy(i_this->mpModel->getBaseTRMtx(), mDoMtx_stack_c::get());
    for (int i = 0; i < 1; i++) {
        wf_tail_s* pTail = &i_this->mTails[i];
        mDoMtx_stack_c::transM(143.0f, 418.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(pTail->field_0x004);
        pTail->field_0x000++;
        tail_control(i_this, pTail);
        tail_mtxset(pTail);
    }
    return 1;
}

static int daObj_Wflag_IsDelete(obj_wflag_class* i_this) {
    return 1;
}

static int daObj_Wflag_Delete(obj_wflag_class* i_this) {
    dComIfG_resDelete(&i_this->mPhaseReq, "Obj_wflag");
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_actor) {
    obj_wflag_class* i_this = (obj_wflag_class*)i_actor;
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_wflag", 10);
    JUT_ASSERT(409, modelData != NULL);
    i_this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->mpModel == NULL) {
        return 0;
    } 
    i_this->mMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("Obj_wflag", 11), NULL, NULL, ( J3DAnmTransform*)dComIfG_getObjectRes("Obj_wflag", 6), 2, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11020284);
    if (i_this->mMorf == NULL || i_this->mMorf->getModel() == NULL) {
        return 0;
    } 
    i_this->field_0x578 = new mDoExt_btkAnm();
    if (i_this->field_0x578 == NULL) {
        return 0;
    }

    if (i_this->field_0x578->init(i_this->mMorf->getModel()->getModelData(), (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Obj_wflag", 14), 1, 2, 1.0f, 0, -1) == 0) {
        return 0;
    }

    i_this->field_0x57c = new mDoExt_btpAnm();
    if (i_this->field_0x57c == NULL) {
        return cPhs_ERROR_e;
    } 

    if (i_this->field_0x57c->init(i_this->mMorf->getModel()->getModelData(), (J3DAnmTexPattern*)dComIfG_getObjectRes("Obj_wflag", 18), 1, 2, 1.0f, 0, -1) == 0) {
        return cPhs_ERROR_e;
    }

    modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("Obj_wflag", 9));
    JUT_ASSERT(479, modelData != NULL);
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 19; j++) {
            i_this->mTails[i].mModels[j] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            if (i_this->mTails[i].mModels[j] == NULL) {
                return 0;
            }
        }
    }
    return 1;
}

static int daObj_Wflag_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, obj_wflag_class);
    int rv = dComIfG_resLoad(&((obj_wflag_class*)i_this)->mPhaseReq, "Obj_wflag");
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_WFLAG PARAM %x\n", fopAcM_GetParam(i_this));
        ((obj_wflag_class*)i_this)->field_0x82c = fopAcM_GetParam(i_this);
        OS_REPORT("OBJ_WFLAG//////////////OBJ_WFLAG SET 1 !!\n");
        if (fopAcM_entrySolidHeap(i_this, useHeapInit, 0xd700) == 0) {
            OS_REPORT("//////////////OBJ_WFLAG SET NON !!\n");
            return cPhs_ERROR_e;
        } else {
            OS_REPORT("//////////////OBJ_WFLAG SET 2 !!\n");
            ((obj_wflag_class*)i_this)->field_0x836 = cM_rndF(65536.0f);
            daObj_Wflag_Execute(((obj_wflag_class*)i_this));
        }
    }
    return rv;
}

static actor_method_class l_daObj_Wflag_Method = {
    (process_method_func)daObj_Wflag_Create,
    (process_method_func)daObj_Wflag_Delete,
    (process_method_func)daObj_Wflag_Execute,
    (process_method_func)daObj_Wflag_IsDelete,
    (process_method_func)daObj_Wflag_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_WFLAG = {
    fpcLy_CURRENT_e,          // mLayerID
    7,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_OBJ_WFLAG,           // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(obj_wflag_class),  // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    53,                       // mPriority
    &l_daObj_Wflag_Method,    // sub_method
    0x40000,                  // mStatus
    fopAc_ACTOR_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
};
