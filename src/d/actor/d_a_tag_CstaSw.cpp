/**
 * d_a_tag_CstaSw.cpp
 * Overworld Statue switch trigger
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_CstaSw.h"
#include "d/d_procname.h"

daTagCstaSw_HIO_c::daTagCstaSw_HIO_c() {
    unk_0x4 = 0;
}

void daTagCstaSw_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
}

int daTagCstaSw_c::create() {
    fopAcM_ct(this, daTagCstaSw_c);
    mSwitch = getSw();
    setBaseMtx();
    return cPhs_COMPLEATE_e;
}

fopAc_ac_c* daTagCstaSw_c::searchSekizoAct(void* i_actor, void*) {
    if (i_actor && fopAcM_IsActor(i_actor) && fopAcM_GetProfName(i_actor) == PROC_CSTAF) {
        return (fopAc_ac_c*)i_actor;
    }
    return NULL;
}

bool daTagCstaSw_c::areaCheck() {
    bool in_area = false;

    fopAc_ac_c* sekizo_p = fopAcM_Search((fopAcIt_JudgeFunc)searchSekizoAct, this);
    if (!sekizo_p) {
        return false;
    }

    cXyz vec_to_actor = current.pos - sekizo_p->current.pos;
    if (vec_to_actor.absXZ() <= scale.x * 100.0f) {
        in_area = true;
    }

    return in_area;
}

int daTagCstaSw_c::Execute() {
    if (areaCheck() == 1) {
        if (!fopAcM_isSwitch(this, mSwitch)) {
            fopAcM_onSwitch(this, mSwitch);
        }
    } else {
        if (fopAcM_isSwitch(this, mSwitch)) {
            fopAcM_offSwitch(this, mSwitch);
        }
    }
    return 1;
}

int daTagCstaSw_c::Draw() {
    return 1;
}

int daTagCstaSw_c::Delete() {
    return 1;
}

static int daTagCstaSw_Draw(daTagCstaSw_c* i_this) {
    return i_this->Draw();
}

static int daTagCstaSw_Execute(daTagCstaSw_c* i_this) {
    return i_this->Execute();
}

static int daTagCstaSw_Delete(daTagCstaSw_c* i_this) {
    return i_this->Delete();
}

static int daTagCstaSw_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagCstaSw_c*>(i_this)->create();
}

daTagCstaSw_HIO_c::~daTagCstaSw_HIO_c() {}

static daTagCstaSw_HIO_c l_HIO;

static actor_method_class l_daTagCstaSw_Method = {
    (process_method_func)daTagCstaSw_Create,  (process_method_func)daTagCstaSw_Delete,
    (process_method_func)daTagCstaSw_Execute, (process_method_func)NULL,
    (process_method_func)daTagCstaSw_Draw,
};

actor_process_profile_definition g_profile_Tag_CstaSw = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Tag_CstaSw,        // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daTagCstaSw_c),  // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    657,                    // mPriority
    &l_daTagCstaSw_Method,  // sub_method
    0x44000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
