/**
 * d_a_kytag09.cpp
 * Twilight Film Tag
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag09.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static int daKytag09_Draw(kytag09_class* i_this) {
    J3DModelData* modelData = i_this->mpModel->getModelData();
    g_env_light.settingTevStruct(0x10, NULL, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(modelData, &i_this->tevStr);

    if (i_this->mType != 1) {
        modelData = i_this->mpModel2->getModelData();
        g_env_light.setLightTevColorType_MAJI(modelData, &i_this->tevStr);
    }

    mDoMtx_stack_c::transS(0.0f, 0.0f, 0.0f);

    Mtx m;
    cMtx_copy(mDoMtx_stack_c::get(), m);
    dComIfGd_setListBG();

    if (i_this->mType != 1) {
        i_this->mpModel->setBaseTRMtx(m);
        i_this->mpModel2->setBaseTRMtx(m);

        if (!dComIfGs_isDarkClearLV(1)) {
            i_this->mBtk_2.entry(i_this->mpModel->getModelData());
            i_this->mBtk_2.play();
            mDoExt_modelUpdateDL(i_this->mpModel);
        } else if (!dComIfGs_isDarkClearLV(2)) {
            i_this->mBtk.entry(i_this->mpModel2->getModelData());
            i_this->mBtk.play();
            mDoExt_modelUpdateDL(i_this->mpModel2);
        }
    } else {
        i_this->mpModel->setBaseTRMtx(m);

        if (!dComIfGs_isDarkClearLV(0)) {
            i_this->mBtk_2.entry(i_this->mpModel->getModelData());
            i_this->mBtk_2.play();
            mDoExt_modelUpdateDL(i_this->mpModel);
        }
    }

    dComIfGd_setList();
    return 1;
}

static int daKytag09_Execute(kytag09_class* i_this) {
    return 1;
}

static int daKytag09_IsDelete(kytag09_class* i_this) {
    return 1;
}

static int daKytag09_Delete(kytag09_class* i_this) {
    if (i_this->mType != 1) {
        dComIfG_resDelete(&i_this->mPhase, "Kytag09");
    } else {
        dComIfG_resDelete(&i_this->mPhase, "kytag09_2");
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    kytag09_class* a_this = (kytag09_class*)i_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Kytag09", 4);
    J3DModelData* modelData2 = (J3DModelData*)dComIfG_getObjectRes("Kytag09", 5);

    a_this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (a_this->mpModel == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* btkp = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Kytag09", 9);
    if (!a_this->mBtk_2.init(modelData, btkp, TRUE, 2, 1.0f, 0, -1)) {
        return 0;
    }

    a_this->mpModel2 = mDoExt_J3DModel__create(modelData2, 0x80000, 0x11000284);
    if (a_this->mpModel2 == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* btk2p = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Kytag09", 10);
    if (!a_this->mBtk.init(modelData2, btk2p, TRUE, 2, 1.0f, 0, -1)) {
        return 0;
    }

    return 1;
}

static int useHeapInit2(fopAc_ac_c* i_this) {
    kytag09_class* a_this = (kytag09_class*)i_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("kytag09_2", 4);
    a_this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (a_this->mpModel == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* btkp = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("kytag09_2", 7);
    if (!a_this->mBtk_2.init(modelData, btkp, TRUE, 2, 1.0f, 0, -1)) {
        return 0;
    }

    return 1;
}

static int daKytag09_Create(fopAc_ac_c* i_this) {
    kytag09_class* a_this = (kytag09_class*)i_this;
    a_this->mType = fopAcM_GetParam(i_this) & 0xFF;

    fopAcM_ct(a_this, kytag09_class);

    int phase;
    if (a_this->mType != 1) {
        phase = dComIfG_resLoad(&a_this->mPhase, "Kytag09");
        if (phase == cPhs_COMPLEATE_e && !fopAcM_entrySolidHeap(i_this, useHeapInit, 0x5EB0)) {
            return cPhs_ERROR_e;
        }
    } else {
        phase = dComIfG_resLoad(&a_this->mPhase, "kytag09_2");
        if (phase == cPhs_COMPLEATE_e && !fopAcM_entrySolidHeap(i_this, useHeapInit2, 0x1880)) {
            return cPhs_ERROR_e;
        }
    }

    return phase;
}

static actor_method_class l_daKytag09_Method = {
    (process_method_func)daKytag09_Create,  (process_method_func)daKytag09_Delete,
    (process_method_func)daKytag09_Execute, (process_method_func)daKytag09_IsDelete,
    (process_method_func)daKytag09_Draw,
};

extern actor_process_profile_definition g_profile_KYTAG09 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KYTAG09,
    &g_fpcLf_Method.base,
    sizeof(kytag09_class),
    0,
    0,
    &g_fopAc_Method.base,
    103,
    &l_daKytag09_Method,
    0x60000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
