/**
 * @file d_a_obj_kita.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kita.h"
#include "SSystem/SComponent/c_lib.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"

daObj_Kita_HIO_c::daObj_Kita_HIO_c() {
    mId = -1;
    mModelSizeX = 1.2f;
    mModelSizeZ = 0.9f;
    mGap = 200.0f;
    mSwingRange = 0.6f;
}

#if DEBUG
void daObj_Kita_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("　 風で動く連続板", 0x80000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Model Size X
    ctx->genSlider("モデルサイズＸ", &mModelSizeX, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Model Size Z
    ctx->genSlider("モデルサイズＺ", &mModelSizeZ, 0.0f, 10.0f, 0,
                             NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Gap
    ctx->genSlider("隙間", &mGap, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Swing Range
    ctx->genSlider("揺れ幅", &mSwingRange, 0.0f, 2.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}
#endif

static int daObj_Kita_Draw(obj_kita_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    g_env_light.settingTevStruct(0x10, &a_this->current.pos, &a_this->tevStr);
    for (int i = 0; i < i_this->mNumKitas - 1; i++) {
        g_env_light.setLightTevColorType_MAJI(i_this->mKitas[i].mModel, &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mKitas[i].mModel);
    }
    return 1;
}

static void ride_call_back(dBgW* param_0, fopAc_ac_c* param_1, fopAc_ac_c* param_2) {
    ((obj_kita_class*)param_1)->field_0x590 = 20;
}

static bool hio_set;

static daObj_Kita_HIO_c l_HIO;

static void ita_control(obj_kita_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    cXyz local_84;
    cXyz cStack_90;
    if (i_this->field_0x590 != 0) {
        i_this->field_0x590--;
    }
    f32 dVar13 = -40.0f;
    i_this->mKitas[0].mPos = a_this->current.pos;
    cLib_addCalc2(&i_this->field_0x580, 5.0f + 500.0f * dKyw_get_wind_pow(), 1.0f, 10.0f);
    f32 dVar10 = i_this->field_0x580;
    int local_b0 = dVar10 / 3.5f;
    if (local_b0 > 127) {
        local_b0 = 127;
    }
    Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_BRIDGE_TRAILING,
                                  &i_this->mKitas[i_this->mNumKitas / 2].mPos, local_b0, 0, 1.0f,
                                  1.0f, -1.0f, -1.0f, 0);
    cXyz local_9c;
    cXyz* local_b4 = dKyw_get_wind_vec();
    if (dVar10 > 0.0f) {
        a_this->current.angle.y = cM_atan2s(local_b4->x, local_b4->z);
    }
    cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    local_84.x = 0.0f;
    local_84.y = 0.0f;
    local_84.z = dVar10;
    MtxPosition(&local_84, &local_9c);
    local_84.x = 0.0f;
    local_84.y = 0.0f;
    local_84.z = l_HIO.mGap;
    if (i_this->field_0x590 != 0) {
        i_this->field_0x588 = 5000;
        i_this->field_0x58a = 5200;
        i_this->field_0x58c = 10000;
        i_this->field_0x58e = 9000;
    } else {
        i_this->field_0x58c = (cM_ssin(i_this->field_0x572 << 10) * 3000.0f) + 9000.0f;
        i_this->field_0x58e = (cM_ssin(i_this->field_0x572 * 0x430) * 3000.0f) + 9000.0f;
        if ((i_this->field_0x572 & 7) == 0) {
            i_this->field_0x588 = cM_rndFX(1000.0f) + 7000.0f;
            i_this->field_0x58a = cM_rndFX(1000.0f) + 7000.0f;
        }
    }
    i_this->field_0x584 += i_this->field_0x588;
    i_this->field_0x586 += i_this->field_0x58a;
    for (int i = 1; i < i_this->mNumKitas; i++) {
        local_9c.y = 0.075f * (dVar10 * cM_ssin(i_this->field_0x584 + i * -i_this->field_0x58c));
        if (i_this->field_0x590 == 0) {
            local_9c.y *= i * l_HIO.mSwingRange + 1.2f;
        }
        f32 dVar15 = local_9c.x + (i_this->mKitas[i].mPos.x - i_this->mKitas[i - 1].mPos.x);
        f32 dVar11 =
            (local_9c.y + (dVar13 + (i_this->mKitas[i].mPos.y - i_this->mKitas[i - 1].mPos.y)));
        f32 dVar14 = (local_9c.z + (i_this->mKitas[i].mPos.z - i_this->mKitas[i - 1].mPos.z));
        i_this->mKitas[i - 1].mAngle.y = cM_atan2s(dVar15, dVar14);
        i_this->mKitas[i - 1].mAngle.x =
            -cM_atan2s(dVar11, JMAFastSqrt(dVar15 * dVar15 + dVar14 * dVar14));
        cLib_addCalcAngleS2(
            &i_this->mKitas[i - 1].mAngle.z,
            3.0f * (dVar10 * cM_ssin(i_this->field_0x586 + i * -i_this->field_0x58e)), 2, 0x600);
        cMtx_YrotS(*calc_mtx, i_this->mKitas[i - 1].mAngle.y);
        cMtx_XrotM(*calc_mtx, i_this->mKitas[i - 1].mAngle.x);
        MtxPosition(&local_84, &cStack_90);
        i_this->mKitas[i].mPos = i_this->mKitas[i - 1].mPos + cStack_90;
    }
}

static void action(obj_kita_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    cXyz cStack_24;
    cXyz cStack_30;

    switch(i_this->field_0x574) {
    case 0:
        break;
    }

    ita_control(i_this);
    for (int i = 0; i < i_this->mNumKitas - 1; i++) {
        mDoMtx_stack_c::transS(i_this->mKitas[i].mPos.x,
                               i_this->mKitas[i].mPos.y,
                               i_this->mKitas[i].mPos.z);
        mDoMtx_stack_c::YrotM(i_this->mKitas[i].mAngle.y);
        mDoMtx_stack_c::XrotM(i_this->mKitas[i].mAngle.x);
        mDoMtx_stack_c::ZrotM(i_this->mKitas[i].mAngle.z);
        mDoMtx_stack_c::scaleM(l_HIO.mModelSizeX * i_this->mKitas[i].mScaleX, l_HIO.mModelSizeX,
                               l_HIO.mModelSizeZ);
        if (i_this->mKitas[i].field_0x28 != 0) {
            mDoMtx_stack_c::YrotM(i_this->mKitas[i].field_0x28);
            mDoMtx_stack_c::transM(0.0f, 0.0f, -200.0f);
        }
        i_this->mKitas[i].mModel->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoMtx_stack_c::scaleM(1.0f, 1.0f, 1.15f);
        MTXCopy(mDoMtx_stack_c::get(), i_this->mKitas[i].mMtx);
        i_this->mKitas[i].mBgW->Move();
    }
}

static int daObj_Kita_Execute(obj_kita_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    i_this->field_0x572++;
    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x578[i] != 0) {
            i_this->field_0x578[i]--;
        }
    }
    action(i_this);
    return 1;
}

static int daObj_Kita_IsDelete(obj_kita_class * i_this) {
    return 1;
}

static int daObj_Kita_Delete(obj_kita_class* i_this) {
    fopAcM_RegisterDeleteID(i_this, "Obj_Kita");
    dComIfG_resDelete(&i_this->mPhase, "Obj_kita");
    if (i_this->mIsHIOOwner) {
        hio_set = 0;
        mDoHIO_DELETE_CHILD(l_HIO.mId);
    }

    for (int i = 0; i < i_this->mNumKitas - 1; i++) {
        dComIfG_Bgsp().Release(i_this->mKitas[i].mBgW);
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    obj_kita_class* i_this = (obj_kita_class*)a_this;
    for (int i = 0; i < i_this->mNumKitas - 1; i++) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_kita", 4);
        JUT_ASSERT(475, modelData != NULL);
        i_this->mKitas[i].mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->mKitas[i].mModel == NULL) {
            return 0;
        }

        i_this->mKitas[i].mBgW = new dBgW();
        if (i_this->mKitas[i].mBgW == NULL) {
            return 0;
        }

        cBgD_t* bgd = (cBgD_t*)dComIfG_getObjectRes("Obj_kita", 7);
        if (i_this->mKitas[i].mBgW->Set(bgd, 1, &i_this->mKitas[i].mMtx) == 1) {
            return 0;
        }
        i_this->mKitas[i].mBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);
        i_this->mKitas[i].mBgW->SetRideCallback(ride_call_back);
    }
    return 1;
}

static int daObj_Kita_Create(fopAc_ac_c* a_this) {
    obj_kita_class* i_this = (obj_kita_class*)a_this;
    fopAcM_ct(a_this, obj_kita_class);

    int rv = dComIfG_resLoad(&i_this->mPhase, "Obj_kita");
    if (rv == 4) {
        OS_REPORT("OBJ_KITA PARAM %x\n", fopAcM_GetParam(a_this));
        i_this->field_0x570 = fopAcM_GetParam(a_this) & 0xff;
        if (i_this->field_0x570 == 0xff || i_this->field_0x570 == 0)
        {
            i_this->mNumKitas = 11;
        } else {
            i_this->mNumKitas = i_this->field_0x570 + 1;
            if (i_this->mNumKitas > 21) {
                i_this->mNumKitas = 21;
            }
        }

        OS_REPORT("OBJ_KITA//////////////OBJ_KITA SET 1 !!\n");
        if (fopAcM_entrySolidHeap(a_this, useHeapInit, 0x4760) == 0) {
            OS_REPORT("//////////////OBJ_KITA SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_KITA SET 2 !!\n");

        for (int i = 0; i < i_this->mNumKitas - 1; i++) {
            if (dComIfG_Bgsp().Regist(i_this->mKitas[i].mBgW, a_this) != 0) {
                return cPhs_ERROR_e;
            }

            if (cM_rndF(1.0f) < 0.5f) {
                i_this->mKitas[i].field_0x28 = 0x8000;
            }

            i_this->mKitas[i].mScaleX = cM_rndFX(0.05f) + 1.0f;
        }
        
        if (!hio_set) {
            i_this->mIsHIOOwner = true;
            hio_set = 1;
            l_HIO.mId = mDoHIO_CREATE_CHILD("風で動く連続板", &l_HIO);
        }

        i_this->field_0x572 = cM_rndF(65535.0f);
        daObj_Kita_Execute(i_this);
    }
    return rv;
}

static actor_method_class l_daObj_Kita_Method = {
    (process_method_func)daObj_Kita_Create,
    (process_method_func)daObj_Kita_Delete,
    (process_method_func)daObj_Kita_Execute,
    (process_method_func)daObj_Kita_IsDelete,
    (process_method_func)daObj_Kita_Draw,
};

actor_process_profile_definition g_profile_OBJ_KITA = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_KITA,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(obj_kita_class), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  707,                    // mPriority
  &l_daObj_Kita_Method,   // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
