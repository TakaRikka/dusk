/**
 * d_a_set_bgobj.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_set_bgobj.h"
#include <cstdio>
#include "f_op/f_op_actor_mng.h"

static const char l_specName[] = "spec.dat";

int daSetBgObj_c::CreateInit() {
    static s16 l_bg_profName[5] = {
        fpcNm_BG_OBJ_e, fpcNm_BG_OBJ_e, fpcNm_BG_OBJ_e, fpcNm_Obj_Flag2_e, fpcNm_Obj_Flag3_e,
    };

    BE(u16)* spec_data_p = (BE(u16)*)dComIfG_getObjectRes(mArcName, l_specName);
    fopAcM_create(l_bg_profName[*spec_data_p], fopAcM_GetParam(this), &current.pos,
                  fopAcM_GetRoomNo(this), &current.angle, &scale, -1);

    return 1;
}

int daSetBgObj_c::create() {
    fopAcM_ct(this, daSetBgObj_c);

    sprintf(mArcName, "%s", getArcName(this));

    int phase = dComIfG_resLoad(&mPhase, mArcName);
    if (phase == cPhs_COMPLEATE_e) {
        CreateInit();
        phase = cPhs_ERROR_e;
    }

    return phase;
}

int daSetBgObj_c::Delete() {
    dComIfG_resDelete(&mPhase, mArcName);
    return 1;
}

static int daSetBgObj_Delete(daSetBgObj_c* i_this) {
    return i_this->Delete();
}

static int daSetBgObj_Create(fopAc_ac_c* i_this) {
    return static_cast<daSetBgObj_c*>(i_this)->create();
}

static actor_method_class l_daSetBgObj_Method = {
    (process_method_func)daSetBgObj_Create,
    (process_method_func)daSetBgObj_Delete,
};

actor_process_profile_definition g_profile_SET_BG_OBJ = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_SET_BG_OBJ_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daSetBgObj_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_SET_BG_OBJ_e,
    /* Actor SubMtd */ &l_daSetBgObj_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_CULL_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
