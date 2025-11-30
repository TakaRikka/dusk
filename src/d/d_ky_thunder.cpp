/**
 * d_ky_thunder.cpp
 * Environment - Thunder
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_ky_thunder.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "SSystem/SComponent/c_counter.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo_rain.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_kankyo_mng.h"

#define EF_THUNDER01_BMD 0x1E
#define EF_THUNDER02_BMD 0x1F
#define EF_THUNDER_BRK 0x2F

int dThunder_c::createHeap() {
    if (mpHeap == NULL) {
        mpHeap = mDoExt_createSolidHeapFromGameToCurrent(0, 0x20);

        if (mpHeap == NULL) {
            return 0;
        }
    }

    return 1;
}

void dThunder_c::adjustHeap() {
    mDoExt_restoreCurrentHeap();

    if ((s32)mDoExt_adjustSolidHeap(mpHeap) >= 0) {
        DCStoreRangeNoSync(mpHeap->getStartAddr(), mpHeap->getHeapSize());
    }
}

int dThunder_c::draw() {
    static cXyz l_offsetPos(0.0f, 40.0f, -250.0f);
    static cXyz l_scale(1.0f, 1.0f, 1.0f);

    camera_class* camera_p = dComIfGp_getCamera(0);

    mDoMtx_stack_c::transS(mThunderPos.x, mThunderPos.y, mThunderPos.z);
    mDoMtx_stack_c::YrotM(field_0x168);
    mDoMtx_stack_c::ZrotM(field_0x15c);
    mDoMtx_stack_c::XrotM(field_0x15c);

    Mtx m;
    MTXCopy(mDoMtx_stack_c::get(), m);

    mModelInfo.mpModel->setBaseScale(field_0x138);
    mModelInfo.mpModel->setBaseTRMtx(m);
    mModelInfo.mBrk.entry(mModelInfo.mpModel->getModelData());

    dComIfGd_setList();
    mDoExt_modelUpdateDL(mModelInfo.mpModel);
    mModelInfo.mBrk.remove(mModelInfo.mpModel->getModelData());
    return 1;
}

static int dThunder_Draw(dThunder_c* i_this) {
    return i_this->draw();
}

int dThunder_c::execute(dThunder_c* i_thunder) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();

    if (env_light->mThunderEff.mState < 10) {
        cLib_addCalc(&mBrkSpeed, 0.1f, 0.05f, 0.017f, 9.999999747378752e-06f);
    }

    mModelInfo.mBrk.setPlaySpeed(mBrkSpeed);
    if (mModelInfo.mBrk.play()) {
        mDoAud_mEnvSe_startFarThunderSe(&mThunderPos);
        fopKyM_Delete(i_thunder);
    }

    f32 fvar7;
    if (dComIfGd_getView() != NULL) {
        fvar7 = dComIfGd_getView()->fovy / 60.0f;
        if (fvar7 >= 1.0f) {
            fvar7 = 1.0f;
        }

        fvar7 = fvar7 * fvar7 * fvar7;
    } else {
        fvar7 = 1.0f;
    }

    fvar7 *= mBrkSpeed;

    if (env_light->mThunderEff.mState < 10) {
        field_0x138.x += fvar7 * field_0x138.x * 0.01f * cM_ssin(field_0x16a);
        field_0x138.y += fvar7 * field_0x138.y * 0.01f * cM_scos(field_0x16c);
        field_0x16a += -25000;
        field_0x16c += 28900;
    } else {
        field_0x138.x += fvar7 * field_0x138.x * 0.02f * cM_ssin(field_0x16a);
        field_0x138.y += fvar7 * field_0x138.y * 0.02f * cM_scos(field_0x16c);
        field_0x16a += -35000;
        field_0x16c += 38900;
    }

    return 1;
}

static int dThunder_Execute(dThunder_c* i_this) {
    return i_this->execute(i_this);
}

static int dThunder_IsDelete(dThunder_c* i_this) {
    return 1;
}

int dThunder_c::thunder_delete(dThunder_c* i_thunder) {
    i_thunder->~dThunder_c();
    return 1;
}

static int dThunder_Delete(dThunder_c* i_this) {
    return i_this->thunder_delete(i_this);
}

static int dThunder_Create(kankyo_class* i_this) {
    dThunder_c* this_ = (dThunder_c*)i_this;

    if (!this_->createHeap()) {
        return cPhs_ERROR_e;
    }

    int ret = this_->create();
    this_->adjustHeap();

    return ret;
}

int dThunder_c::create() {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    camera_class* camera_p = dComIfGp_getCamera(0);
    cXyz spB8;
    cXyz spC4;
    cXyz spD0;
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f29_2;
    f32 var_r28;
    f32 var_r27;

    new (this) dThunder_c();

    J3DModelData* modelData;
    if (cM_rndF(1.0f) < 0.5f) {
        modelData = (J3DModelData*)dComIfG_getObjectRes("Always", EF_THUNDER01_BMD);
    } else {
        modelData = (J3DModelData*)dComIfG_getObjectRes("Always", EF_THUNDER02_BMD);
    }

    mModelInfo.mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x1000000);
    if (mModelInfo.mpModel == NULL) {
        return cPhs_ERROR_e;
    }

    J3DAnmTevRegKey* canm = (J3DAnmTevRegKey*)dComIfG_getObjectRes("Always", EF_THUNDER_BRK);
    if (!mModelInfo.mBrk.init(modelData, canm, TRUE, 0, 1.0f, 0, -1)) {
        return cPhs_ERROR_e;
    }

    mBrkSpeed = 1.0f;
    mModelInfo.mBrk.setPlaySpeed(mBrkSpeed);

    if (env_light->mThunderEff.mState < 10) {
        var_f29 = 1.0f;
    } else {
        var_f29 = 0.25f;
    }

    field_0x168 = cLib_targetAngleY(&camera_p->lookat.eye, &camera_p->lookat.center);

    if (!(g_Counter.mCounter0 & 1)) {
        field_0x168 += 0x7FFF;
    }

    var_r28 = 14.0f;
    var_r27 = 20.0f;
    var_f31 = var_r28;
    var_f30 = var_r27;
    spC4.set(60000.0f, -5000.0f, 0.0f);
    field_0x15c = 2000.0f;

    if (env_light->mThunderEff.mState >= 10) {
        var_f31 = var_r27;
        var_f30 = 24.0f;
        field_0x15c = 2500.0f;
        spC4.y = 6000.0f;
    }

    if (!strcmp(dComIfGp_getStartStageName(), "R_SP30") && dComIfGp_roomControl_getStayNo() == 4) {
        var_r28 = 0.1f;
        var_r27 = 0.25f;
        var_f31 = 0.15f;
        var_f30 = 0.2f;
    }

    field_0x15c = var_f29 * cM_rndFX(field_0x15c);
    field_0x138.x = var_f29 * (var_r28 + cM_rndF(var_r27 - var_r28));

    if (cM_rndFX(1.0f) >= 0.5) {
        field_0x138.x *= -1.0f;
    }

    field_0x138.y = var_f29 * (var_f31 + cM_rndF(var_f30 - var_f31));
    field_0x138.z = 1.0f;

    dKyr_get_vectle_calc(&camera_p->lookat.eye, &camera_p->lookat.center, &spB8);

    // JMAFastSqrt doesn't match here when it should, probably needs to be fixed
    // var_f29_2 = JMAFastSqrt(spB8.x * spB8.x + spB8.z * spB8.z);

    var_f29_2 = spB8.x * spB8.x + spB8.z * spB8.z;
    if (var_f29_2 > 0.0f) {
        // fakematch?
        #ifdef __MWERKS__
        f32 tmp = __frsqrtes(var_f29_2);
        var_f29_2 = tmp * var_f29_2;
        #endif
    }

    s16 temp_r28 = cM_atan2s(spB8.x, spB8.z);
    s16 temp_r31 = cM_atan2s(spB8.y, var_f29_2);

    if (cM_rndFX(1.0f) >= 0.0f) {
        temp_r28 += 0x4000;
    } else {
        temp_r28 -= 0x4000;
    }

    spD0.x = cM_scos(temp_r31) * cM_ssin(temp_r28);
    spD0.z = cM_scos(temp_r31) * cM_scos(temp_r28);

    f32 temp_f1_3 = cM_rndF(spC4.x);
    mThunderPos.x = camera_p->lookat.eye.x + (spB8.x * 100000.0f) + (spD0.x * temp_f1_3);
    mThunderPos.y = camera_p->lookat.eye.y + 2000.0f + cM_rndF(spC4.y);
    mThunderPos.z = camera_p->lookat.eye.z + (spB8.z * 100000.0f) + (spD0.z * temp_f1_3);

    if (!strcmp(dComIfGp_getStartStageName(), "R_SP30") && dComIfGp_roomControl_getStayNo() == 4) {
        mThunderPos.x = cM_rndFX(260.0f) + 19635.0f;
        mThunderPos.y = 4592.0f;
        mThunderPos.z = cM_rndFX(100.0f) + 1507.0f;
    }

    if (cM_rndF(1.0f) < 0.3f) {
        field_0x150.x = -mThunderPos.x;
        field_0x150.y = -mThunderPos.y;
        field_0x150.z = -mThunderPos.z;
        mDoAud_mEnvSe_startFarThunderSe(&field_0x150);
    }

    return cPhs_COMPLEATE_e;
}

static leafdraw_method_class l_dThunder_Method = {
    (process_method_func)dThunder_Create,  (process_method_func)dThunder_Delete,
    (process_method_func)dThunder_Execute, (process_method_func)dThunder_IsDelete,
    (process_method_func)dThunder_Draw,
};

extern kankyo_process_profile_definition g_profile_KY_THUNDER = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KY_THUNDER,
    &g_fpcLf_Method.base,
    sizeof(dThunder_c),
    0,
    0,
    &g_fopKy_Method,
    6,
    &l_dThunder_Method,
};
