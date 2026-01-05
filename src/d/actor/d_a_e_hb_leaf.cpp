/**
 * d_a_e_hb_leaf.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_hb_leaf.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"

static int daE_HB_LEAF_Draw(e_hb_leaf_class* i_this) {
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpMorf->getModel(), &i_this->tevStr);
    i_this->mpMorf->entryDL();
    return 1;
}

static int daE_HB_LEAF_Execute(e_hb_leaf_class* i_this) {
    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    i_this->mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpMorf->modelCalc();

    fopAc_ac_c* actor = fopAcM_SearchByID(i_this->field_0x5bc);
    if (i_this->field_0x5ba == 0) {
        if (actor != NULL) {
            i_this->field_0x5ba++;
        }
    } else if (i_this->field_0x5ba == 1 && actor == NULL) {
        i_this->field_0x5ba++;
        i_this->field_0x5b8 = TREG_S(8) + 100;
    }

    return 1;
}

static int daE_HB_LEAF_IsDelete(e_hb_leaf_class* i_this) {
    return 1;
}

static int daE_HB_LEAF_Delete(e_hb_leaf_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "E_HB");
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_hb_leaf_class* a_this = static_cast<e_hb_leaf_class*>(i_this);

    a_this->mpMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("E_HB", 0x18), NULL,
                                        NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_HB", 0x13),
                                        2, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11000084);
    if (a_this->mpMorf == NULL || a_this->mpMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int daE_HB_LEAF_Create(fopAc_ac_c* i_this) {
    e_hb_leaf_class* a_this = static_cast<e_hb_leaf_class*>(i_this);
    fopAcM_ct(a_this, e_hb_leaf_class);

    int phase_state = dComIfG_resLoad(&a_this->mPhase, "E_HB");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_HB_LEAF//////////////E_HB_LEAF SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0xB00)) {
            OS_REPORT("//////////////E_HB_LEAF SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_HB_LEAF SET 2 !!\n");

        fopAcM_SetMtx(a_this, a_this->mpMorf->getModel()->getBaseTRMtx());
        daE_HB_LEAF_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_HB_LEAF_Method = {
    (process_method_func)daE_HB_LEAF_Create,  (process_method_func)daE_HB_LEAF_Delete,
    (process_method_func)daE_HB_LEAF_Execute, (process_method_func)daE_HB_LEAF_IsDelete,
    (process_method_func)daE_HB_LEAF_Draw,
};

actor_process_profile_definition g_profile_E_HB_LEAF = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_E_HB_LEAF,
    &g_fpcLf_Method.base,
    sizeof(e_hb_leaf_class),
    0,
    0,
    &g_fopAc_Method.base,
    141,
    &l_daE_HB_LEAF_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
