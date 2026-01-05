/**
 * d_a_hitobj.cpp
 * 
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_hitobj.h"
#include "f_op/f_op_kankyo_mng.h"

static int daHitobj_Draw(hitobj_class* i_this) {
    return 1;
}

static int daHitobj_Execute(hitobj_class* i_this) {
    if (i_this->field_0x572 != 0) {
        i_this->field_0x572--;
        i_this->field_0x5b0.SetC(i_this->current.pos);
        dComIfG_Ccsp()->Set(&i_this->field_0x5b0);
    } else {
        fopKyM_Delete(i_this);
    }

    return 1;
}

static int daHitobj_IsDelete(hitobj_class* i_this) {
    return 1;
}

static int daHitobj_Delete(hitobj_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase,"Hitobj");
    return 1;
}

static int daHitobj_Create(fopAc_ac_c* i_this) {
    static dCcD_SrcSph cc_sph_src = {
        {
            {0, {{0, 0, 0x13}, {0, 3}, 0}},
            {dCcD_SE_NONE, 0, 0, 0, {0}},
            {dCcD_SE_NONE, 0, 0, 0, {0}},
            {0},
        },
        {
            {
                {0.0f, 0.0f, 0.0f},
                100.0f,
            },
        }
    };

    hitobj_class* hitobj = (hitobj_class*)i_this;
    fopAcM_ct(i_this, hitobj_class);

    int ret = dComIfG_resLoad(&hitobj->mPhase,"Hitobj");

    if (ret == cPhs_COMPLEATE_e) {
        hitobj->field_0x570 = fopAcM_GetParam(hitobj);
        hitobj->field_0x574.Init(0xff,0xff,hitobj);
        hitobj->field_0x5b0.Set(cc_sph_src);
        hitobj->field_0x5b0.SetStts(&hitobj->field_0x574);
        hitobj->field_0x572 = 3;
    }

    return ret;
}

static actor_method_class l_daHitobj_Method = {
    (process_method_func)daHitobj_Create,
    (process_method_func)daHitobj_Delete,
    (process_method_func)daHitobj_Execute,
    (process_method_func)daHitobj_IsDelete,
    (process_method_func)daHitobj_Draw
};

actor_process_profile_definition g_profile_HITOBJ = {
    fpcLy_CURRENT_e,        // mLayerID   
    7,                      // mListID  
    fpcPi_CURRENT_e,        // mListPrio   
    PROC_HITOBJ,            // mProcName            
    &g_fpcLf_Method.base,  // sub_method                      
    sizeof(hitobj_class),   // mSize                      
    0,                      // mSizeOther  
    0,                      // mParameters         
    &g_fopAc_Method.base,   // sub_method                     
    690,                    // mPriority       
    &l_daHitobj_Method,     // sub_method                   
    0x40000,                // mStatus            
    fopAc_ACTOR_e,          // mActorType  
    fopAc_CULLBOX_0_e,      // cullType     
};
