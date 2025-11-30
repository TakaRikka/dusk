/**
 * d_a_vrbox.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_vrbox.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "f_op/f_op_actor_mng.h"

static int daVrbox_color_set(vrbox_class* i_this);

static int daVrbox_Draw(vrbox_class* i_this) {
    J3DModel* soraModel_p = i_this->mpSoraModel;
    f32 fvar = 0.0f;
    dStage_FileList_dt_c* filelist_p = NULL;

    daVrbox_color_set(i_this);

    if (g_env_light.hide_vrbox) {
        return 1;
    }

    if (dComIfGp_roomControl_getStayNo() >= 0) {
        s32 stayNo = dComIfGp_roomControl_getStayNo();
        filelist_p = dComIfGp_roomControl_getStatusRoomDt(stayNo)->getFileListInfo();
    }

    if (filelist_p != NULL) {
        fvar = dStage_FileList_dt_SeaLevel(filelist_p);
    }

    if (dComIfGd_getView() != NULL) {
        fvar = (dComIfGd_getInvViewMtx()[1][3] - fvar) * 0.09f;
    } else {
        fvar = 0.0f;
    }

    mDoMtx_stack_c::transS(dComIfGd_getInvViewMtx()[0][3], dComIfGd_getInvViewMtx()[1][3] - fvar,
                           dComIfGd_getInvViewMtx()[2][3]);

    soraModel_p->setBaseTRMtx(mDoMtx_stack_c::get());
    dKy_GxFog_set();

    // these casts look like fake matches, but this ptr is used as both J3DModel and J3DModelData?
    for (int i = ((J3DModelData*)soraModel_p)->getMaterialNum() - 1; i >= 0; i--) {
        J3DMaterial* material_p = ((J3DModelData*)soraModel_p)->getMaterialNodePointer(i);

        J3DFogInfo* fogInfo_p;
        if (material_p != NULL) {
            fogInfo_p = material_p->getFog()->getFogInfo();
        }

        fogInfo_p->mType = 2;
    }

    dComIfGd_setListSky();
    mDoExt_modelUpdateDL(soraModel_p);
    dComIfGd_setList();

    return 1;
}

static int daVrbox_color_set(vrbox_class* i_this) {
    if ((g_env_light.vrbox_kasumi_outer_col.r + g_env_light.vrbox_kasumi_outer_col.g +
         g_env_light.vrbox_kasumi_outer_col.b + g_env_light.vrbox_sky_col.r + g_env_light.vrbox_sky_col.g +
         g_env_light.vrbox_sky_col.b + g_env_light.vrbox_kumo_top_col.r + g_env_light.vrbox_kumo_top_col.g +
         g_env_light.vrbox_kumo_top_col.b) == 0)
    {
        g_env_light.hide_vrbox = true;
        return 1;
    }

    g_env_light.hide_vrbox = false;

    J3DModelData* modelData = i_this->mpSoraModel->getModelData();
    J3DGXColorS10 color;

    J3DMaterial* material_0 = modelData->getMaterialNodePointer(0);
    if (material_0 != NULL) {
        material_0->setCullMode(0);
        material_0->change();

        color.r = g_env_light.vrbox_sky_col.r;
        color.g = g_env_light.vrbox_sky_col.g;
        color.b = g_env_light.vrbox_sky_col.b;
        color.a = 255;
        material_0->setTevColor(0, &color);
    }

    J3DMaterial* material_1 = modelData->getMaterialNodePointer(1);
    if (material_1 != NULL) {
        material_1->setCullMode(0);
        material_1->change();

        color.r = g_env_light.vrbox_kasumi_inner_col.r;
        color.g = g_env_light.vrbox_kasumi_inner_col.g;
        color.b = g_env_light.vrbox_kasumi_inner_col.b;
        color.a = g_env_light.vrbox_kasumi_inner_col.a;
        material_1->setTevColor(0, &color);
    }

    return 1;
}

static int daVrbox_Execute(vrbox_class* i_this) {
    return 1;
}

static int daVrbox_IsDelete(vrbox_class* i_this) {
    return 1;
}

static int daVrbox_Delete(vrbox_class* i_this) {
    return 1;
}

static int daVrbox_solidHeapCB(fopAc_ac_c* i_this) {
    vrbox_class* this_ = (vrbox_class*)i_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getStageRes("vrbox_sora.bmd");
    this_->mpSoraModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11020202);

    return modelData != NULL && this_->mpSoraModel != NULL;
}

static int daVrbox_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, vrbox_class);
    vrbox_class* this_ = (vrbox_class*)i_this;

    this_->field_0x574 = 0;
    int phase = cPhs_COMPLEATE_e;

    if (fopAcM_entrySolidHeap(this_, daVrbox_solidHeapCB, 0xC60)) {
        dComIfGp_onStatus(1);
        g_env_light.hide_vrbox = false;
    } else {
        phase = cPhs_ERROR_e;
    }

    return phase;
}

static actor_method_class l_daVrbox_Method = {
    (process_method_func)daVrbox_Create,  (process_method_func)daVrbox_Delete,
    (process_method_func)daVrbox_Execute, (process_method_func)daVrbox_IsDelete,
    (process_method_func)daVrbox_Draw,
};

extern actor_process_profile_definition g_profile_VRBOX = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_VRBOX,
    &g_fpcLf_Method.base,
    sizeof(vrbox_class),
    0,
    0,
    &g_fopAc_Method.base,
    7,
    &l_daVrbox_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
