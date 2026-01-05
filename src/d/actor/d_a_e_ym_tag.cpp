/**
 * d_a_e_ym_tag.cpp
 * Enemy - Shadow Insect / 闇虫 (Yami Mushi) - Tag
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_ym_tag.h"
#include "f_op/f_op_actor_mng.h"

static int daE_YM_TAG_Draw(daE_YM_TAG_c* i_this) {
    return 1;
}

static void* s_e_ym(void* i_actorP1, void* i_actorP2) {
    if (fopAcM_IsActor(i_actorP1) && fopAcM_GetName(i_actorP1) == PROC_E_YM) {
        if (fpcM_IsCreating(fopAcM_GetID(i_actorP1)) == 0) {
            if (static_cast<daE_YM_c*>(i_actorP1)->getTagNo() == (u8)fopAcM_GetParam(i_actorP2)) {
                return i_actorP1;
            }
        }
    }

    return 0;
}

int daE_YM_TAG_c::execute() {
    daE_YM_c* shadow_insectP = (daE_YM_c*)fpcM_Search(s_e_ym,this);

    if (shadow_insectP) {
        shadow_insectP->setTagPos(current.pos);
        shadow_insectP->setTagPosP();
        fopAcM_delete(this);
    }

    return 1;
}

static int daE_YM_TAG_Execute(daE_YM_TAG_c* i_this) {
    return i_this->execute();
}

static int daE_YM_TAG_IsDelete(daE_YM_TAG_c* i_this) {
    return 1;
}

static int daE_YM_TAG_Delete(daE_YM_TAG_c* i_this) {
    return 1;
}

int daE_YM_TAG_c::create() {
    fopAcM_ct(this, daE_YM_TAG_c);
    if ((u8)fopAcM_GetParam(this) == 0xFF) {
        return cPhs_ERROR_e;
    }
    return cPhs_COMPLEATE_e;
}

static int daE_YM_TAG_Create(daE_YM_TAG_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daE_YM_TAG_Method = {
    (process_method_func)daE_YM_TAG_Create,
    (process_method_func)daE_YM_TAG_Delete,
    (process_method_func)daE_YM_TAG_Execute,
    (process_method_func)daE_YM_TAG_IsDelete,
    (process_method_func)daE_YM_TAG_Draw
};

actor_process_profile_definition g_profile_E_YM_TAG = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_E_YM_TAG,          // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daE_YM_TAG_c),   // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    195,                    // mPriority
    &l_daE_YM_TAG_Method,   // sub_method
    0x44000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_0_e,      // cullType   
};
