/**
 * @file d_a_obj_ihasi.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_obj_ihasi.h"
#include "d/d_com_inf_game.h"

static int daObj_Ihasi_Draw(obj_ihasi_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    if (i_this->mFlag1 != 1) {
        g_env_light.settingTevStruct(0x20, &a_this->current.pos, &a_this->tevStr);
        g_env_light.setLightTevColorType_MAJI(i_this->mpModel1, &a_this->tevStr);

        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(i_this->mpModel1);
        dComIfGd_setList();
    } else {
        g_env_light.settingTevStruct(0x10, &a_this->current.pos, &a_this->tevStr);
        g_env_light.setLightTevColorType_MAJI(i_this->mpModel2, &a_this->tevStr);

        i_this->mpBtkAnm->entry(i_this->mpModel2->getModelData());
        mDoExt_modelUpdateDL(i_this->mpModel2);
    }

    return 1;
}

static void ih_normal(obj_ihasi_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
}

static void ih_disappear(obj_ihasi_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
}

static void action(obj_ihasi_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp14;
    cXyz sp8;

    switch (i_this->mFlag1) {
    case 0:
        ih_normal(i_this);
        break;
    case 1:
        ih_disappear(i_this);
        break;
    }

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);

    if (i_this->mFlag1 == 1) {
        mDoMtx_stack_c::transM(34800.0f, -300.0f, -26735.0f);
        i_this->mpBtkAnm->play();
        i_this->mpModel2->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        i_this->mpModel1->setBaseTRMtx(mDoMtx_stack_c::get());
        MTXCopy(mDoMtx_stack_c::get(), i_this->mMtx);
        i_this->mpBgW->Move();
    }
}

static int daObj_Ihasi_Execute(obj_ihasi_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    i_this->field_0x57c += (s8)1;

    for (int i = 0; i < 2; i++) {
        if (i_this->mArr[i] != 0) {
            i_this->mArr[i] -= (s8)1;
        }
    }

    if (i_this->mFlag2 != 0) {
        i_this->mFlag1 = 1;
        i_this->mFlag2 = 0;

        dComIfG_Bgsp().Release(i_this->mpBgW);
        i_this->mpBgW = NULL;
        /* dSv_event_flag_c::M_050 - Main Event - Eldin Bridge disappears */
        dComIfGs_onEventBit((u16)dSv_event_flag_c::saveBitLabels[0x53]);
    }

    action(i_this);
    return 1;
}

static int daObj_Ihasi_IsDelete(obj_ihasi_class* param_0) {
    return 1;
}

static int daObj_Ihasi_Delete(obj_ihasi_class* _this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)_this;
    fopAcM_RegisterDeleteID(_this, "Obj_Ihasi");
    dComIfG_resDelete(&_this->mPhaseReq, "Obj_ihasi");

    if (_this->mpBgW != NULL) {
        dComIfG_Bgsp().Release(_this->mpBgW);
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_actor) {
    obj_ihasi_class* i_this = (obj_ihasi_class*)i_actor;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_ihasi", 5);
    JUT_ASSERT(0x115, modelData != NULL);

    i_this->mpModel1 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (i_this->mpModel1 == NULL) {
        return 0;
    }

    i_this->mpBgW = new dBgW();
    if (i_this->mpBgW == NULL) {
        return 0;
    }

    if (i_this->mpBgW->Set((cBgD_t*)dComIfG_getObjectRes("Obj_ihasi", 0xc), 1, &i_this->mMtx) == 1) {
        return 0;
    }

    i_this->mpBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);

    modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_ihasi", 6);
    JUT_ASSERT(0x12B, modelData != NULL);

    i_this->mpModel2 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (i_this->mpModel2 == NULL) {
        return 0;
    }

    i_this->mpBtkAnm = new mDoExt_btkAnm();
    if (i_this->mpBtkAnm == NULL) {
        return 0;
    }

    if (i_this->mpBtkAnm->init(i_this->mpModel2->getModelData(), (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Obj_ihasi", 9), 1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }

    return 1;
}

static int daObj_Ihasi_Create(fopAc_ac_c* i_actor) {
    obj_ihasi_class* i_this = static_cast<obj_ihasi_class*>(i_actor);
    fopAcM_ct(i_this, obj_ihasi_class);

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&i_this->mPhaseReq, "Obj_ihasi");

    if (step == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_IHASI PARAM %x\n", fopAcM_GetParam(i_actor));
        u32 param1;
        u32 param2;
        cXyz pos;
        csXyz angle;

        /* dSv_event_flag_c::M_050 - Main Event - Eldin Bridge disappears */
        if (dComIfGs_isEventBit((u16)dSv_event_flag_c::saveBitLabels[0x53])) {
            angle.set(0x0, -0x8000, 0x0);
            pos.set(34750.0f, -300.0f, -44000.0f);

            param1 = fopAcM_GetParam(i_actor) & 0xff000000 | 0xfffe00;
            fopAcM_create(PROC_E_S1, param1, &pos, fopAcM_GetRoomNo(i_actor), &angle, NULL, -1);

            pos.set(35250.0f, -300.0f, -43500.0f);
            angle.y += (s16)0x5555;
            fopAcM_create(PROC_E_S1, param1, &pos, fopAcM_GetRoomNo(i_actor), &angle, NULL, -1);

            pos.set(34250.0f, -300.0f, -43500.0f);
            angle.y += (s16)0x5555;
            fopAcM_create(PROC_E_S1, param1, &pos, fopAcM_GetRoomNo(i_actor), &angle, NULL, -1);

            return cPhs_ERROR_e;
        }
        OS_REPORT("OBJ_IHASI//////////////OBJ_IHASI SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_actor, useHeapInit, 0x43b0)) {
            OS_REPORT("//////////////OBJ_IHASI SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_IHASI SET 2 !!\n");
        if (dComIfG_Bgsp().Regist(i_this->mpBgW, i_this)) {
            return cPhs_ERROR_e;
        }

        i_this->field_0x57c = cM_rndF(65536.0f);

        if (!strcmp(dComIfGp_getStartStageName(), "F_SP121") && fopAcM_GetRoomNo(i_actor) == 0) {
            pos.set(34800.0f, 5700.0f, -26735.0f);
            fopAcM_create(PROC_E_WAP, 0xffffff35, &pos, fopAcM_GetRoomNo(i_actor), NULL, NULL, -1);

            angle.set(0x0, -0x8000, 0x0);
            pos.set(34800.0f, -300.0f, -30340.0f);
            param2 = fopAcM_GetParam(i_actor) & 0xff000000 | 0xfffe00;
            fopAcM_create(PROC_E_S1, param2, &pos, fopAcM_GetRoomNo(i_actor), &angle, NULL, -1);

            pos.set(35087.0f, -300.0f, -29879.0f);
            fopAcM_create(PROC_E_S1, param2, &pos, fopAcM_GetRoomNo(i_actor), &angle, NULL, -1);

            pos.set(34500.0f, -300.0f, -29879.0f);
            fopAcM_create(PROC_E_S1, param2, &pos, fopAcM_GetRoomNo(i_actor), &angle, NULL, -1);
        }

        daObj_Ihasi_Execute(i_this);
    }

    return step;
}

static actor_method_class l_daObj_Ihasi_Method = {
    (process_method_func)daObj_Ihasi_Create,  (process_method_func)daObj_Ihasi_Delete,
    (process_method_func)daObj_Ihasi_Execute, (process_method_func)daObj_Ihasi_IsDelete,
    (process_method_func)daObj_Ihasi_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_IHASI = {
    fpcLy_CURRENT_e,          // mLayerID
    3,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_OBJ_IHASI,           // mProcName
    &g_fpcLf_Method.base,     // sub_method
    sizeof(obj_ihasi_class),  // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    533,                      // mPriority
    &l_daObj_Ihasi_Method,    // sub_method
    0x00044000,               // mStatus
    fopAc_ACTOR_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
};
