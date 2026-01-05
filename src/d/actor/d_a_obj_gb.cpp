/**
 * @file d_a_obj_gb.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_gb.h"
#include "SSystem/SComponent/c_lib.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"

static int daObj_Gb_Draw(obj_gb_class* i_this) {
    g_env_light.settingTevStruct(0x10, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mModel, &i_this->tevStr);
    J3DModelData* modelData = (J3DModelData*)i_this->mModel->getModelData();
    for (u16 i = 0; i <= 1; i++) {
        J3DMaterial* material = (J3DMaterial*)modelData->getMaterialNodePointer(i);
        material->getTevKColor(1)->a = i_this->mColorAlpha;
    }

    dKy_bg_MAxx_proc(i_this->mModel);
    i_this->mBtk->entry(modelData);
    i_this->mBrk->entry(modelData);
    mDoExt_modelUpdateDL(i_this->mModel);
    return 1;
}

static int daObj_Gb_Execute(obj_gb_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz acStack_30;
    cXyz cStack_3c;
    i_this->field_0x58e++;
    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x58a[i] != 0) {
            i_this->field_0x58a[i]--;
        }
    }
    switch(i_this->mIsFinalBattle) {
    case 0:
        if (i_this->field_0x57c == 0) {
            for (int i = 0; i < 20; i++) {
                if (i_this->mBrkFrame < 0.5f) {
                    Z2GetAudioMgr()->seStart(Z2SE_OBJ_GANON_BARRIER_APPR, &i_this->field_0x594[i],
                                             0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                }
                Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_GANON_BARRIER, &i_this->field_0x594[i], 0, 0,
                                              1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
        } else {
            if (i_this->mBrkFrame < 0.5f) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_GANON_BARRIER_APPR, &i_this->current.pos, 0, 0,
                                         1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
            Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_GANON_BARRIER, &i_this->current.pos, 0, 0, 1.0f,
                                          1.0f, -1.0f, -1.0f, 0);
        }
        cLib_addCalc2(&i_this->mBrkFrame, 29.0f, 1.0f, 1.0f);
        if (strcmp(dComIfGp_getStartStageName(), "D_MN09B") != 0) {
            if (dComIfGs_isSwitch(i_this->mSw2, fopAcM_GetRoomNo(i_this)) ||
                !dComIfGs_isSwitch(i_this->mSw1, fopAcM_GetRoomNo(i_this)))
            {
                i_this->mIsFinalBattle = 1;
            }
        }
        break;
    case 1:
        cLib_addCalc2(&i_this->mBrkFrame, 0, 1.0f, 1.0f);
        if (!dComIfGs_isSwitch(i_this->mSw2, fopAcM_GetRoomNo(i_this)) &&
            dComIfGs_isSwitch(i_this->mSw1, fopAcM_GetRoomNo(i_this)))
        {
            i_this->mIsFinalBattle = 0;
        }
        break;
    }
    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->current.angle.y);
    if (i_this->field_0x57c == 0) {
        mDoMtx_stack_c::scaleM(i_this->scale.x, i_this->scale.y, i_this->scale.x);
    } else {
        mDoMtx_stack_c::scaleM(i_this->scale.x, i_this->scale.y, 1.0f);
    }
    i_this->mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mBtk->play();
    i_this->mBrk->setFrame(i_this->mBrkFrame);
    if (i_this->mBrkFrame < 1.0f) {
        mDoMtx_stack_c::transM(0.0f, -200.0f, 0.0f);
        mDoMtx_stack_c::scaleM(1.0f, 0.0f, 1.0f);
    }
    MTXCopy(mDoMtx_stack_c::get(), i_this->mBgMtx);
    i_this->mpBgW->Move();
    return 1;
}

static int daObj_Gb_IsDelete(obj_gb_class* param_0) {
    return 1;
}

static int daObj_Gb_Delete(obj_gb_class* i_this) {
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, "Obj_gb");
    if (i_this->mpBgW != NULL) {
        dComIfG_Bgsp().Release(i_this->mpBgW);
    }
    return 1;
}

static int bmd[2] = {
    6, 7,
};

static int brk[2] = {
    10, 11,
};

static int btk[2] = {
    14, 15,
};

static int dzb[2] = {
    18, 19,
};

static int useHeapInit(fopAc_ac_c* actor) {
    obj_gb_class* i_this = (obj_gb_class*)actor;
    J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes("Obj_gb", bmd[i_this->field_0x57c]);
    JUT_ASSERT(324, modelData != NULL);
    i_this->mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (i_this->mModel == NULL) {
        return 0;
    }
    i_this->mBtk = new mDoExt_btkAnm();
    if (i_this->mBtk == NULL) {
        return 0;
    }
    J3DAnmTextureSRTKey* anmTexture = (J3DAnmTextureSRTKey*) dComIfG_getObjectRes(
        "Obj_gb", btk[i_this->field_0x57c]);
    if (i_this->mBtk->init(i_this->mModel->getModelData(), anmTexture, 1, 2, 1.0f, 0, -1) == 0) {
        return 0;
    }
    i_this->mBrk = new mDoExt_brkAnm();
    if (i_this->mBrk== NULL) {
        return 0;
    }
    J3DAnmTevRegKey* anmTevKey = (J3DAnmTevRegKey*)dComIfG_getObjectRes(
        "Obj_gb", brk[i_this->field_0x57c]);
    if (i_this->mBrk->init(i_this->mModel->getModelData(), anmTevKey, 1, 2, 0.0f, 0, -1) == 0) {
        return 0;
    }
    i_this->mpBgW = new dBgW();
    if (i_this->mpBgW == NULL) {
        return 0;
    }
    cBgD_t* pbGd = (cBgD_t*)dComIfG_getObjectRes(
        "Obj_gb", dzb[i_this->field_0x57c]);
    return i_this->mpBgW->Set(pbGd, 1, &i_this->mBgMtx) == 1 ? 0 : 1;
}

static int daObj_Gb_Create(fopAc_ac_c* actor) {
    fopAcM_ct(actor, obj_gb_class);
    obj_gb_class* i_this = (obj_gb_class*)actor;
    int rv = dComIfG_resLoad(&i_this->mPhase, "Obj_gb");
    
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_GB PARAM %x\n", fopAcM_GetParam(i_this));
        i_this->field_0x57c = fopAcM_GetParam(i_this);
        if (i_this->field_0x57c == 0xff) {
            i_this->field_0x57c = 0;
        }
        u8 local_47 = (fopAcM_GetParam(i_this) >> 8) & 0xff;
        if (local_47 == 0xff) {
            if (i_this->field_0x57c == 0) {
                local_47 = 100;
            } else {
                local_47 = 10;
            }
        }
        if (i_this->field_0x57c == 0) {
            i_this->scale.x = local_47 * 0.01f;
        } else {
            i_this->scale.x = local_47 * 0.5f;
        }
        i_this->scale.y = ((fopAcM_GetParam(i_this) >> 16) & 0xff) * 0.333333f * 0.5f;
        i_this->mColorAlpha = fopAcM_GetParam(i_this) >> 24;
        i_this->mSw1 = i_this->current.angle.x & 0xff;
        i_this->mSw2 = (i_this->current.angle.x & 0xff00) >> 8;
        i_this->shape_angle.x = 0;
        i_this->current.angle.x = 0;
        OS_REPORT("OBJ_GB//////////////OBJ_GB SET 1 !!\n");
        int heapSize;
        if (i_this->field_0x57c == 0) {
            heapSize = 0x1c40;
        } else {
            heapSize = 0x1320;
        }
        if (fopAcM_entrySolidHeap(i_this, useHeapInit, heapSize) == 0) {
            OS_REPORT("//////////////OBJ_GB SET NON !!\n");
            return cPhs_ERROR_e;
        }
        OS_REPORT("//////////////OBJ_GB SET 2 !!\n");
        if (i_this->mpBgW != NULL) {
            if (dComIfG_Bgsp().Regist(i_this->mpBgW, i_this) != 0) {
                return cPhs_ERROR_e;
            }
        }
        i_this->field_0x58e = cM_rndF(65536.0f);
        if (strcmp(dComIfGp_getStartStageName(), "D_MN09B") != 0) {
            i_this->mIsFinalBattle = 1;
        }
        s16 local_44 = 0;
        cXyz cStack_30(0.0f, 0.0f, i_this->scale.x * 1000.0f);
        for (int i = 0; i < 20; i++, local_44 += 0xccc) {
            cMtx_YrotS(*calc_mtx, local_44);
            MtxPosition(&cStack_30, &i_this->field_0x594[i]);
            i_this->field_0x594[i] += i_this->current.pos;
        }
        daObj_Gb_Execute(i_this);
    }
    return rv;
}

static actor_method_class l_daObj_Gb_Method = {
    (process_method_func)daObj_Gb_Create,
    (process_method_func)daObj_Gb_Delete,
    (process_method_func)daObj_Gb_Execute,
    (process_method_func)daObj_Gb_IsDelete,
    (process_method_func)daObj_Gb_Draw,
};

actor_process_profile_definition g_profile_OBJ_GB = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_GB,            // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(obj_gb_class),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  56,                     // mPriority
  &l_daObj_Gb_Method,     // sub_method
  0x00044000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
