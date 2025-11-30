/**
 * d_a_e_bi_leaf.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_bi_leaf.h"
#include "d/d_com_inf_game.h"

static int daE_BI_LEAF_Draw(e_bi_leaf_class* i_this) {
    if (i_this->field_0x5b4 == 1) {
        return 1;
    }

    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpModel, &i_this->tevStr);
    mDoExt_modelUpdateDL(i_this->mpModel);
    return 1;
}

static void action(e_bi_leaf_class* i_this) {
    switch (i_this->mAction) {
    case 0:
        if (i_this->mTimer == 0) {
            fopAc_ac_c* parent_p = fopAcM_SearchByID(i_this->parentActorID);
            if (parent_p == NULL) {
                OS_REPORT("//////////////LEAF ID 2  %d\n", i_this->parentActorID);

                if (fopAcM_GetRoomNo(i_this) == 50) {
                    i_this->mTimer = 90;
                } else {
                    i_this->mTimer = 60;
                }

                i_this->mAction = 1;
            }
        }
        break;
    case 1:
        if (i_this->mTimer == 0) {
            i_this->mAction = 0;

            i_this->parentActorID =
                fopAcM_createChild(PROC_E_BI, fopAcM_GetID(i_this), (i_this->field_0x5b4 << 8) | 1,
                                   &i_this->current.pos, fopAcM_GetRoomNo(i_this),
                                   &i_this->current.angle, NULL, -1, NULL);
            i_this->mTimer = 20;
        }
        break;
    }
}

static int daE_BI_LEAF_Execute(e_bi_leaf_class* i_this) {
    if (i_this->mTimer != 0) {
        i_this->mTimer--;
    }

    action(i_this);

    if (i_this->field_0x5b4 == 1) {
        return 1;
    }

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    return 1;
}

static int daE_BI_LEAF_IsDelete(e_bi_leaf_class* i_this) {
    return 1;
}

static int daE_BI_LEAF_Delete(e_bi_leaf_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "E_BI");
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_bi_leaf_class* a_this = static_cast<e_bi_leaf_class*>(i_this);

    if (a_this->field_0x5b4 != 1) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_BI", 15);
        JUT_ASSERT(0, modelData != NULL);

        a_this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (a_this->mpModel == NULL) {
            return 0;
        }
    }

    return 1;
}

static int daE_BI_LEAF_Create(fopAc_ac_c* i_this) {
    e_bi_leaf_class* a_this = static_cast<e_bi_leaf_class*>(i_this);
    fopAcM_ct(a_this, e_bi_leaf_class);

    int phase_state = dComIfG_resLoad(&a_this->mPhase, "E_BI");
    if (phase_state == cPhs_COMPLEATE_e) {
        a_this->field_0x5b4 = fopAcM_GetParam(a_this) & 0xFF;
        OS_REPORT("E_BI_LEAF//////////////E_BI_LEAF SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0xA00)) {
            OS_REPORT("//////////////E_BI_LEAF SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_BI_LEAF SET 2 !!\n");

        fopAcM_SetMtx(a_this, a_this->mpModel->getBaseTRMtx());
        daE_BI_LEAF_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_BI_LEAF_Method = {
    (process_method_func)daE_BI_LEAF_Create,  (process_method_func)daE_BI_LEAF_Delete,
    (process_method_func)daE_BI_LEAF_Execute, (process_method_func)daE_BI_LEAF_IsDelete,
    (process_method_func)daE_BI_LEAF_Draw,
};

extern actor_process_profile_definition g_profile_E_BI_LEAF = {
    fpcLy_CURRENT_e,
    9,
    fpcPi_CURRENT_e,
    PROC_E_BI_LEAF,
    &g_fpcLf_Method.base,
    sizeof(e_bi_leaf_class),
    0,
    0,
    &g_fopAc_Method.base,
    148,
    &l_daE_BI_LEAF_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
