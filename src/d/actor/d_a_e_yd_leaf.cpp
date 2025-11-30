/**
 * d_a_e_yd_leaf.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_yd_leaf.h"
#include "d/d_com_inf_game.h"

static int daE_YD_LEAF_Draw(e_yd_leaf_class* i_this) {
    g_env_light.settingTevStruct(2, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpMorf->getModel(), &i_this->tevStr);

    dComIfGd_setListDark();
    i_this->mpMorf->entryDL();
    dComIfGd_setList();
    return 1;
}

static int daE_YD_LEAF_Execute(e_yd_leaf_class* i_this) {
    if (i_this->field_0x5ba == 0) {
        stage_stag_info_class* pstag = dComIfGp_getStage()->getStagInfo();
        if (dStage_stagInfo_GetSTType(pstag) != ST_FIELD &&
            fopAcM_SearchByID(i_this->parentActorID) == NULL)
        {
            i_this->field_0x5ba = 1;
        }
    } else {
        cLib_addCalc0(&i_this->scale.y, 1.0f, 0.08f);

        if (i_this->scale.y <= 0.01f) {
            fopAcM_delete(i_this);
        }
    }

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::scaleM(i_this->scale.y, i_this->scale.y, i_this->scale.y);
    i_this->mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpMorf->modelCalc();
    return 1;
}

static int daE_YD_LEAF_IsDelete(e_yd_leaf_class* i_this) {
    return 1;
}

static int daE_YD_LEAF_Delete(e_yd_leaf_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "E_YD");
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_yd_leaf_class* a_this = static_cast<e_yd_leaf_class*>(i_this);

    a_this->mpMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("E_YD", 0x19), NULL,
                                        NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_YD", 0x14),
                                        2, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11000084);
    if (a_this->mpMorf == NULL || a_this->mpMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int daE_YD_LEAF_Create(fopAc_ac_c* i_this) {
    e_yd_leaf_class* a_this = static_cast<e_yd_leaf_class*>(i_this);
    fopAcM_ct(a_this, e_yd_leaf_class);

    int phase_state = dComIfG_resLoad(&a_this->mPhase, "E_YD");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_YD_LEAF//////////////E_YD_LEAF SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0xA80)) {
            OS_REPORT("//////////////E_YD_LEAF SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_YD_LEAF SET 2 !!\n");

        fopAcM_SetMtx(a_this, a_this->mpMorf->getModel()->getBaseTRMtx());
        a_this->scale.y = 1.0f;
        daE_YD_LEAF_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_YD_LEAF_Method = {
    (process_method_func)daE_YD_LEAF_Create,  (process_method_func)daE_YD_LEAF_Delete,
    (process_method_func)daE_YD_LEAF_Execute, (process_method_func)daE_YD_LEAF_IsDelete,
    (process_method_func)daE_YD_LEAF_Draw,
};

extern actor_process_profile_definition g_profile_E_YD_LEAF = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_E_YD_LEAF,
    &g_fpcLf_Method.base,
    sizeof(e_yd_leaf_class),
    0,
    0,
    &g_fopAc_Method.base,
    145,
    &l_daE_YD_LEAF_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
