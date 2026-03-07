// Translation Unit: d_a_e_db_leaf

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_db_leaf.h"
#include "d/d_procname.h"

static int daE_DB_LEAF_Draw(e_db_leaf_class* i_this) {
    g_env_light.settingTevStruct(0,&i_this->current.pos,&i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpMorf->getModel(),&i_this->tevStr);
    i_this->mpMorf->entryDL();
    return 1;
}

static int daE_DB_LEAF_Execute(e_db_leaf_class* i_this) {
    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    i_this->mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::now);
    i_this->mpMorf->modelCalc();
    return 1;
}

static int daE_DB_LEAF_IsDelete(e_db_leaf_class* i_this) {
    return 1;
}

static int daE_DB_LEAF_Delete(e_db_leaf_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "E_DB");
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_db_leaf_class* leaf = static_cast<e_db_leaf_class*>(i_this);

    leaf->mpMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("E_DB", 0x18), NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes("E_DB", 0x13), 2, 1.0, 0, -1, 1, NULL, 0x80000, 0x11000084);    

    if (leaf->mpMorf == NULL || leaf->mpMorf->getModel() == NULL) {
        return 0;
    } else {
        return 1;
    }
}  

static int daE_DB_LEAF_Create(fopAc_ac_c* i_this) {
    e_db_leaf_class* leaf = (e_db_leaf_class*)i_this;
    fopAcM_ct(leaf, e_db_leaf_class);

    int ret = dComIfG_resLoad(&leaf->mPhase, "E_DB");
    if (ret == cPhs_COMPLEATE_e) {
        if(!fopAcM_entrySolidHeap(leaf,useHeapInit,0xA80)) {
            return cPhs_ERROR_e;
        } else {
            fopAcM_SetMtx(leaf, leaf->mpMorf->getModel()->getBaseTRMtx());
            daE_DB_LEAF_Execute(leaf);
        }
    }
    return ret;
}

static actor_method_class l_daE_DB_LEAF_Method = {
    (process_method_func)daE_DB_LEAF_Create,
    (process_method_func)daE_DB_LEAF_Delete,
    (process_method_func)daE_DB_LEAF_Execute,
    (process_method_func)daE_DB_LEAF_IsDelete,
    (process_method_func)daE_DB_LEAF_Draw,
};

actor_process_profile_definition g_profile_E_DB_LEAF = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_E_DB_LEAF,
    &g_fpcLf_Method.base,
    sizeof(e_db_leaf_class),
    0,
    0,
    &g_fopAc_Method.base,
    137,
    &l_daE_DB_LEAF_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
