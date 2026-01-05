/**
 * d_a_tag_poFire.cpp
 * Tag - Poe Fire
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_poFire.h"
#include "d/d_procname.h"

daTagPoFire_HIO_c::daTagPoFire_HIO_c() {
    unk_0x4 = 0x14;
}

void daTagPoFire_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
}

int daTagPoFire_c::create() {
    fopAcM_ct(this, daTagPoFire_c);
    field_0x569 = 0;
    setBaseMtx();
    return cPhs_COMPLEATE_e;
}

int daTagPoFire_c::Execute() {
    if (field_0x569) {
        fopAcM_delete(this);
    }
    return 1;
}

int daTagPoFire_c::Draw() {
    return 1;
}

int daTagPoFire_c::Delete() {
    return 1;
}

static int daTagPoFire_Draw(daTagPoFire_c* i_this) {
    return i_this->Draw();
}

static int daTagPoFire_Execute(daTagPoFire_c* i_this) {
    return i_this->Execute();
}

static int daTagPoFire_Delete(daTagPoFire_c* i_this) {
    return i_this->Delete();
}

static int daTagPoFire_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagPoFire_c*>(i_this)->create();
}

daTagPoFire_HIO_c::~daTagPoFire_HIO_c() {}

static u8 lit_3617[12];

static daTagPoFire_HIO_c l_HIO;

static actor_method_class l_daTagPoFire_Method = {
    (process_method_func)daTagPoFire_Create,  (process_method_func)daTagPoFire_Delete,
    (process_method_func)daTagPoFire_Execute, (process_method_func)NULL,
    (process_method_func)daTagPoFire_Draw,
};

actor_process_profile_definition g_profile_Tag_poFire = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Tag_poFire,        // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daTagPoFire_c),  // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    649,                    // mPriority
    &l_daTagPoFire_Method,  // sub_method
    0x44000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
