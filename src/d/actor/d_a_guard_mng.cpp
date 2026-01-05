/**
 * d_a_guard_mng.cpp
 * Guard
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_guard_mng.h"
#include "f_pc/f_pc_executor.h"

static void* s_sub(void* i_actorP, void* i_this) {
    if (fopAcM_IsActor(i_actorP)) {
        if (fopAcM_GetName(i_actorP) == PROC_NPC_CD3 || fopAcM_GetName(i_actorP) == PROC_NPC_PASSER || fopAcM_GetName(i_actorP) == PROC_NPC_FGUARD) {
            static_cast<daGuardMng_c*>(i_this)->countMerchantNum();
        } else {
            if (fopAcM_GetName(i_actorP) == PROC_TAG_GUARD) {
                static_cast<daGuardMng_c*>(i_this)->checkAppearTag((daTagGuard_c *)i_actorP);
            }
        }
    }
    
    return 0;
}

u8 daGuardMng_c::checkMerchantNum() {
    field_0x56c = G_CM3D_F_INF;
    mMerchantNum = 0;
    fpcM_Search(s_sub,this);
    return mMerchantNum;
}

void daGuardMng_c::checkAppearTag(daTagGuard_c* i_this) {
    cXyz pos;
    if ((u8)i_this->getAppearPoint(&pos) != 0) {
        pos -= *fopAcM_GetPosition_p(dComIfGp_getPlayer(0));
        f32 pos_abs = pos.abs2();

        if (field_0x56c > pos_abs) {
            mpTagGuard = i_this;
            field_0x56c = pos_abs;
        }
    }
}

static int daGuardMng_Execute(daGuardMng_c* i_this) {
    return i_this->execute();
}

static int daGuardMng_IsDelete(daGuardMng_c* param_0) {
    return 1;
}

static int daGuardMng_Delete(daGuardMng_c* i_this) {
    i_this->~daGuardMng_c();
    return 1;
}

static int daGuardMng_Create(fopAc_ac_c* i_this) {
    return static_cast<daGuardMng_c*>(i_this)->create();
}

static actor_method_class l_daGuardMng_Method = {
    (process_method_func)daGuardMng_Create,
    (process_method_func)daGuardMng_Delete,
    (process_method_func)daGuardMng_Execute,
    (process_method_func)daGuardMng_IsDelete
};

actor_process_profile_definition g_profile_GUARD_MNG = {
    fpcLy_CURRENT_e,        // mLayerID      
    7,                      // mListID    
    fpcPi_CURRENT_e,        // mListPrio     
    PROC_GUARD_MNG,         // mProcName                 
    &g_fpcLf_Method.base,  // sub_method                        
    sizeof(daGuardMng_c),   // mSize                        
    0,                      // mSizeOther    
    0,                      // mParameters           
    &g_fopAc_Method.base,   // sub_method                       
    407,                    // mPriority          
    &l_daGuardMng_Method,   // sub_method                       
    0x40100,                // mStatus              
    fopAc_ACTOR_e,          // mActorType    
    fopAc_CULLBOX_0_e,      // cullType   
};
