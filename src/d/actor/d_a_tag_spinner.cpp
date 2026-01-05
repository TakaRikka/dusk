/**
 * d_a_tag_spinner.cpp
 * Tag - Spinner
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_spinner.h"
#include "d/d_procname.h"

static int CheckCreateHeap(fopAc_ac_c* param_0) {
    daTagSpinner_c* tag = (daTagSpinner_c*)param_0;
    return tag->CreateHeap();
}

static void* search_spinner_sub(void* tag_0, void* tag_1) {
    fopAc_ac_c* actor = (fopAc_ac_c*)tag_0;
    daTagSpinner_c* actor2 = (daTagSpinner_c*)tag_1;

    if ((actor != NULL) && (fopAcM_IsActor(actor) != NULL) && (fopAcM_GetProfName(actor) == 0xF2)) {
        if ((actor->current.pos.absXZ(actor2->current.pos)) < (actor2->GetR())) {
            return actor;
        }
    }
    return 0;
}

void daTagSpinner_c::initBaseMtx() {
    setBaseMtx();
}

void daTagSpinner_c::setBaseMtx() {
    MTXTrans(mDoMtx_stack_c::now, current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_ZXYrotM(mDoMtx_stack_c::now, shape_angle.x, shape_angle.y, shape_angle.z);
}

int daTagSpinner_c::Create() {
    initBaseMtx();
    mRadius = scale.x * 100.0f;
    field_0x56c = scale.y * 100.0f;
    return 1;
}

int daTagSpinner_c::CreateHeap() {
    return 1;
}

int daTagSpinner_c::create() {
    int iVar1;
    int iVar2;
    fopAcM_ct(this, daTagSpinner_c);

    if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, 0)) {
        iVar2 = 5;
    } else {
        iVar1 = Create();
        if (iVar1 == NULL) {
            iVar2 = 5;
        } else {
            iVar2 = 4;
        }
    }
    return iVar2;
}

int daTagSpinner_c::execute() {
    daSpinner_c* mpSpinner =
        (daSpinner_c*)fopAcM_Search((fopAcIt_JudgeFunc)search_spinner_sub, this);

    if (mpSpinner) {
        mpSpinner->setSpinnerTag(current.pos);
        fopAcM_onSwitch(this, daTagSpinner_prm::getSwNo(this));
    } else {
        fopAcM_offSwitch(this, daTagSpinner_prm::getSwNo(this));
    }
    return 1;
}

int daTagSpinner_c::_delete() {
    return 1;
}

static int daTagSpinner_Execute(daTagSpinner_c* tag) {
    return tag->execute();
}

static int daTagSpinner_Delete(daTagSpinner_c* i_this) {
    return i_this->_delete();
}

static int daTagSpinner_Create(fopAc_ac_c* tag) {
    return static_cast<daTagSpinner_c*>(tag)->create();
}

static actor_method_class l_daTagSpinner_Method = {
    (process_method_func)daTagSpinner_Create,
    (process_method_func)daTagSpinner_Delete,
    (process_method_func)daTagSpinner_Execute,

};

actor_process_profile_definition g_profile_Tag_Spinner = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Tag_Spinner,        // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daTagSpinner_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    605,                     // mPriority
    &l_daTagSpinner_Method,  // sub_method
    0x40100,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
