/**
 * d_a_kytag07.cpp
 * Environment Tag 7
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag07.h"
#include "d/d_stage.h"
#include "f_op/f_op_overlap_mng.h"

static int daKytag07_Draw(kytag07_class* i_this) {
    return 1;
}

static int daKytag07_Execute(kytag07_class* i_this) {
    if (i_this->field_0x58c != 99) {
        cLib_addCalc(&i_this->mLightInfluence.mPow, i_this->field_0x588, 0.1f, 1000.0f, 0.001f);
    } else {
        if (i_this->home.roomNo != dStage_roomControl_c::getStayNo()) {
            cLib_addCalc(&i_this->mLightInfluence.mPow, 0.0f, 0.25f, 10000.0f, 1.0f);
        }

        if (i_this->mLightInfluence.mPow <= 0.01f) {
            fopAcM_delete(i_this);
        }
    }

    return 1;
}

static int daKytag07_IsDelete(kytag07_class* i_this) {
    i_this->field_0x58c = 99;

    if (i_this->mLightInfluence.mPow <= 0.01f) {
        return 1;
    } else {
        return fopOvlpM_IsDoingReq() == 1 ? 1 : 0;
    }
}

static int daKytag07_Delete(kytag07_class* i_this) {
    dKy_plight_cut(&i_this->mLightInfluence);
    return 1;
}

static int daKytag07_Create(fopAc_ac_c* i_this) {
    kytag07_class* a_this = (kytag07_class*)i_this;

    fopAcM_ct(a_this, kytag07_class);

    a_this->mLightInfluence.mPosition = i_this->current.pos;
    a_this->mLightInfluence.mColor.r = fopAcM_GetParam(i_this) & 0xFF;
    a_this->mLightInfluence.mColor.g = fopAcM_GetParam(i_this) >> 8 & 0xFF;
    a_this->mLightInfluence.mColor.b = fopAcM_GetParam(i_this) >> 16 & 0xFF;
    a_this->mLightInfluence.mPow = 1e-10;
    a_this->mLightInfluence.mFluctuation = 1.0f;
    a_this->field_0x588 = 1000.0f * i_this->scale.x;
    a_this->field_0x58c = 0;

    dKy_plight_priority_set(&a_this->mLightInfluence);
    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daKytag07_Method = {
    (process_method_func)daKytag07_Create, (process_method_func)daKytag07_Delete,
    (process_method_func)daKytag07_Execute, (process_method_func)daKytag07_IsDelete,
    (process_method_func)daKytag07_Draw};

actor_process_profile_definition g_profile_KYTAG07 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_KYTAG07_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(kytag07_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_KYTAG07_e,
    /* Actor SubMtd */ &l_daKytag07_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
