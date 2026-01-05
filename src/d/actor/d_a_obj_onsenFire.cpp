/**
 * d_a_obj_onsenFire.cpp
 * Object - Hotspring Fire
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_onsenFire.h"
#include "d/d_procname.h"

int daObjOnsenFire_c::create() {
    fopAcM_ct(this, daObjOnsenFire_c);
         /* dSv_event_flag_c::F_0377 - Castle Town - Goron spring water shop open! */
    if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[377])) {
        return cPhs_ERROR_e;
    }
    return cPhs_COMPLEATE_e;
}

int daObjOnsenFire_c::execute() {
    dComIfGp_particle_setSimple(0x100,&current.pos,0xff,g_whiteColor,g_whiteColor,0,0.0f);
    dComIfGp_particle_setSimple(0x101,&current.pos,0xff,g_whiteColor,g_whiteColor,0,0.0f);
    dComIfGp_particle_setSimple(0x103,&current.pos,0xff,g_whiteColor,g_whiteColor,0,0.0f);
    Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_ONSEN_WARM_FIRE,&current.pos,0,0,1.0f,1.0f,-1.0f,-1.0f,0);
    return 1;
}

static int daObjOnsenFire_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjOnsenFire_c*>(i_this)->create();
}

static int daObjOnsenFire_Execute(daObjOnsenFire_c* i_this) {
    return i_this->execute();
}

static int daObjOnsenFire_Delete(daObjOnsenFire_c* i_this) {
    i_this->~daObjOnsenFire_c();
    return 1;
}

static actor_method_class l_daObjOnsenFire_Method = {
    (process_method_func)daObjOnsenFire_Create,
    (process_method_func)daObjOnsenFire_Delete,
    (process_method_func)daObjOnsenFire_Execute
};

actor_process_profile_definition g_profile_OBJ_ONSEN_FIRE = {
    fpcLy_CURRENT_e,           // mLayerID         
    3,                         // mListID       
    fpcPi_CURRENT_e,           // mListPrio        
    PROC_OBJ_ONSEN_FIRE,       // mProcName                           
    &g_fpcLf_Method.base,     // sub_method                           
    sizeof(daObjOnsenFire_c),  // mSize                               
    0,                         // mSizeOther       
    0,                         // mParameters              
    &g_fopAc_Method.base,      // sub_method                          
    612,                       // mPriority                 
    &l_daObjOnsenFire_Method,  // sub_method                              
    0x44000,                   // mStatus                 
    fopAc_ACTOR_e,             // mActorType       
    fopAc_CULLBOX_CUSTOM_e,    // cullType        
};
