/**
 * d_a_obj_lv3WaterB.cpp
 * Morpheel Arena Water
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv3WaterB.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "f_op/f_op_actor.h"
#include "f_op/f_op_actor_mng.h"

static int daObj_Lv3waterB_Draw(obj_lv3WaterB_class* i_this) {
    fopAc_ac_c* const actor = i_this;

    g_env_light.settingTevStruct(0x10, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpBWaterModel, &actor->tevStr);

    i_this->mpBWaterBtk->entry(i_this->mpBWaterModel->getModelData());

    dComIfGd_setListInvisisble();
    J3DMaterial* material_p = i_this->mpBWaterModel->getModelData()->getMaterialNodePointer(0);
    if (material_p->getTexGenBlock()->getTexMtx(0) != NULL) {
        J3DTexMtxInfo* tex_mtx_info = &material_p->getTexGenBlock()->getTexMtx(0)->getTexMtxInfo();
        if (tex_mtx_info != NULL) {
            Mtx m;
            C_MTXLightPerspective(m, dComIfGd_getView()->fovy, dComIfGd_getView()->aspect, 1.0f,
                                  1.0f, -0.015f, 0.0f);
            #if WIDESCREEN_SUPPORT
            mDoGph_gInf_c::setWideZoomLightProjection(m);
            #endif

            tex_mtx_info->setEffectMtx(m);
            i_this->mpBWaterModel->getModelData()->simpleCalcMaterial((MtxP)j3dDefaultMtx);
        }
    }

    mDoExt_modelUpdateDL(i_this->mpBWaterModel);

    if (i_this->mpOctHibiModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpOctHibiModel, &actor->tevStr);

        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(i_this->mpOctHibiModel);
        dComIfGd_setList();
    }

    return 1;
}

static void action(obj_lv3WaterB_class* i_this) {
    fopAc_ac_c* const actor = i_this;

    switch (i_this->mAction) {
    case LV3WATERB_ACT_WAIT:
        break;
    case LV3WATERB_ACT_REMOVE:
        dComIfG_Bgsp().Release(i_this->mpOctHibiBgW);
        i_this->mpOctHibiModel = NULL;
        i_this->mAction = LV3WATERB_ACT_WAIT;
        break;
    case LV3WATERB_ACT_END:
        dKy_custom_colset(4, 4, 0.0f);

        if (i_this->field_0x586 == 0) {
            actor->current.pos.y = (actor->home.pos.y - 14450.0f) + AREG_F(17) + 160.0f + 4000.0f + AREG_F(16);
            i_this->field_0x586 = 1;
        }

        cLib_addCalc2(&actor->current.pos.y, (actor->home.pos.y - 14450.0f) + AREG_F(17) + 160.0f, 0.05f,
                      10.0f + AREG_F(19));
        break;
    }

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    i_this->mpBWaterModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), i_this->mpBWaterMtx);

    i_this->mpBWaterBgW->Move();
    i_this->mpBWaterBtk->play();

    if (i_this->mpOctHibiModel != NULL) {
        mDoMtx_stack_c::transS(0.0f, 0.0f, 0.0f);
        i_this->mpOctHibiModel->setBaseTRMtx(mDoMtx_stack_c::get());
        MTXCopy(mDoMtx_stack_c::get(), i_this->mpOctHibiMtx);

        i_this->mpOctHibiBgW->Move();
    }
}

static int daObj_Lv3waterB_Execute(obj_lv3WaterB_class* i_this) {
    fopAc_ac_c* const actor = i_this;

    action(i_this);
    return 1;
}

static int daObj_Lv3waterB_IsDelete(obj_lv3WaterB_class* i_this) {
    return 1;
}

static int daObj_Lv3waterB_Delete(obj_lv3WaterB_class* i_this) {
    fopAc_ac_c* const actor = i_this;
    fopAcM_RegisterDeleteID(i_this, "Obj_Lv3waterB");

    dComIfG_resDelete(&i_this->mBWaterPhase, "L3_bwater");
    dComIfG_resDelete(&i_this->mOcthibiPhase, "S_octhibi");

    dComIfG_Bgsp().Release(i_this->mpBWaterBgW);

    if (i_this->mpOctHibiBgW != NULL) {
        dComIfG_Bgsp().Release(i_this->mpOctHibiBgW);
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    obj_lv3WaterB_class* const actor = static_cast<obj_lv3WaterB_class*>(i_this);

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("L3_bwater", 5);
    JUT_ASSERT(308, modelData != NULL);

    actor->mpBWaterModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (actor->mpBWaterModel == NULL) {
        return 0;
    }

    actor->mpBWaterBtk = new mDoExt_btkAnm();
    if (actor->mpBWaterBtk == NULL) {
        return 0;
    }

    if (!actor->mpBWaterBtk->init(actor->mpBWaterModel->getModelData(),
                                  (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("L3_bwater", 8),
                                  TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1))
    {
        return 0;
    }

    actor->mpBWaterBgW = new dBgW();
    if (actor->mpBWaterBgW == NULL) {
        return 0;
    }

    if (actor->mpBWaterBgW->Set((cBgD_t*)dComIfG_getObjectRes("L3_bwater", 11),
        dBgW::MOVE_BG_e, &actor->mpBWaterMtx) == true) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("S_octhibi", 4);
    JUT_ASSERT(343, modelData != NULL);

    actor->mpOctHibiModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (actor->mpOctHibiModel == NULL) {
        return 0;
    }

    actor->mpOctHibiBgW = new dBgW();
    if (actor->mpOctHibiBgW == NULL) {
        return 0;
    }

    if (actor->mpOctHibiBgW->Set((cBgD_t*) dComIfG_getObjectRes("S_octhibi", 7),
        dBgW::MOVE_BG_e, &actor->mpOctHibiMtx) == true) {
        return 0;
    }

    return 1;
}

static int daObj_Lv3waterB_Create(fopAc_ac_c* i_this) {
    obj_lv3WaterB_class* const actor = static_cast<obj_lv3WaterB_class*>(i_this);
    fopAcM_ct(actor, obj_lv3WaterB_class);

    int phase_state = dComIfG_resLoad(&actor->mBWaterPhase, "L3_bwater");
    int octhibi_phase_state = dComIfG_resLoad(&actor->mOcthibiPhase, "S_octhibi");
    if (phase_state == cPhs_COMPLEATE_e && octhibi_phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_LV3WATERB PARAM %x\n", fopAcM_GetParam(i_this));
        OS_REPORT("OBJ_LV3WATERB//////////////OBJ_LV3WATERB SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x4B000)) {
            OS_REPORT("//////////////OBJ_LV3WATERB SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_LV3WATERB SET 2 !!\n");

        if (dComIfG_Bgsp().Regist(actor->mpBWaterBgW, actor)) {
            return cPhs_ERROR_e;
        }

        if (dComIfG_Bgsp().Regist(actor->mpOctHibiBgW, actor)) {
            return cPhs_ERROR_e;
        }

        if (dComIfGs_isStageBossEnemy()) {
            i_this->current.pos.y = (i_this->home.pos.y - 14450.0f) + AREG_F(17) + 160.0f;
            actor->mAction = LV3WATERB_ACT_END;
            actor->field_0x586 = 1;
            actor->mpOctHibiModel = NULL;
        }

        daObj_Lv3waterB_Execute(actor);
        return phase_state;
    } else if (phase_state == cPhs_COMPLEATE_e) {
        return octhibi_phase_state;
    }

    return phase_state;
}

static actor_method_class l_daObj_Lv3waterB_Method = {
    (process_method_func)daObj_Lv3waterB_Create,  (process_method_func)daObj_Lv3waterB_Delete,
    (process_method_func)daObj_Lv3waterB_Execute, (process_method_func)daObj_Lv3waterB_IsDelete,
    (process_method_func)daObj_Lv3waterB_Draw,
};

actor_process_profile_definition g_profile_OBJ_LV3WATERB = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_OBJ_LV3WATERB,
    &g_fpcLf_Method.base,
    sizeof(obj_lv3WaterB_class),
    0,
    0,
    &g_fopAc_Method.base,
    618,
    &l_daObj_Lv3waterB_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
