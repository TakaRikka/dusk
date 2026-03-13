/**
 * d_a_obj_onsenFire.cpp
 * Object - Hotspring Fire
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_onsenFire.h"
#include "f_pc/f_pc_name.h"

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
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 3,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_OBJ_ONSEN_FIRE_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjOnsenFire_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_OBJ_ONSEN_FIRE_e,
    /* Actor SubMtd */ &l_daObjOnsenFire_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
