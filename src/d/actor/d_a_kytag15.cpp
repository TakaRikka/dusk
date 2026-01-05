/**
 * d_a_kytag15.cpp
 * Z Shake Tag
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag15.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static int daKytag15_Draw(kytag15_class* i_this) {
    cXyz scale;
    mDoMtx_stack_c::transS(0.0f, 0.0f, 0.0f);

    scale.x = 0.00524f;
    scale.y = 0.00524f;
    scale.z = 0.00524f;

    if (i_this->mpModel != NULL && i_this->field_0x590 > 0.0f) {
        J3DGXColorS10 color;
        J3DMaterial* mat_p = i_this->mpModel->getModelData()->getMaterialNodePointer(0);
        color.b = 0;
        color.g = 0;
        color.r = 0;
        color.a = -180;

        if (i_this->field_0x58c == 1) {
            color.a = -100;
        }

        color.a = (color.a + 255.0f) * i_this->field_0x590 + -255.0f;

        mat_p->setTevColor(0, &color);
        i_this->mpModel->setBaseScale(scale);
        i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

        i_this->mBtk.entry(i_this->mpModel->getModelData());
        i_this->mBtk.play();

        if (i_this->field_0x58c == 1) {
            i_this->mBtk.play();
        }

        dComIfGd_setXluList2DScreen();
        mDoExt_modelUpdateDL(i_this->mpModel);
        dComIfGd_setList();
    }

    return 1;
}

static int daKytag15_Execute(kytag15_class* i_this) {
    if (!i_this->field_0x58c) {
        if (!dKy_daynight_check()) {
            cLib_addCalc(&i_this->field_0x590, 1.0f, 0.05f, 0.01f, 1.0E-8f);
        } else {
            cLib_addCalc(&i_this->field_0x590, 0.0f, 0.05f, 0.01f, 1.0E-8f);
        }
    } else if (g_env_light.wether_pat0 == 11) {
        cLib_addCalc(&i_this->field_0x590, 1.0f, 0.05f, 0.01f, 1.0E-8f);
    }

    return 1;
}

static int daKytag15_IsDelete(kytag15_class* i_this) {
    return 1;
}

static int daKytag15_Delete(kytag15_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "Kytag15");
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    kytag15_class* a_this = (kytag15_class*)i_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Kytag15", 4);
    a_this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (a_this->mpModel == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Kytag15", 7);
    if (!a_this->mBtk.init(modelData, btk, TRUE, 2, 1.0f, 0, -1)) {
        return 0;
    }

    return 1;
}

static int daKytag15_Create(fopAc_ac_c* i_this) {
    kytag15_class* a_this = (kytag15_class*)i_this;

    if (strcmp(dComIfGp_getStartStageName(), "F_SP118") == 0) {
        a_this->field_0x58c = 1;
    } else {
        a_this->field_0x58c = 0;
    }

    fopAcM_ct(a_this, kytag15_class);

    if (a_this->field_0x58c == 0) {
        if (!dKy_daynight_check()) {
            a_this->field_0x590 = 1.0f;
        } else {
            a_this->field_0x590 = 0.0f;
        }
    } else {
        a_this->field_0x590 = 0.0f;
    }

    int phase = dComIfG_resLoad(&a_this->mPhase, "Kytag15");
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0)) {
            return cPhs_ERROR_e;
        }
    }

    return phase;
}

static actor_method_class l_daKytag15_Method = {
    (process_method_func)daKytag15_Create,  (process_method_func)daKytag15_Delete,
    (process_method_func)daKytag15_Execute, (process_method_func)daKytag15_IsDelete,
    (process_method_func)daKytag15_Draw,
};

actor_process_profile_definition g_profile_KYTAG15 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KYTAG15,
    &g_fpcLf_Method.base,
    sizeof(kytag15_class),
    0,
    0,
    &g_fopAc_Method.base,
    109,
    &l_daKytag15_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
