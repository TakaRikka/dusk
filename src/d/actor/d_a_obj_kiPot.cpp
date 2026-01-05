/**
* @file d_a_obj_kiPot.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kiPot.h"
#include "d/d_com_inf_game.h"

static daKiPot_HIO_c l_HIO;

static cull_box l_cull_box = {
    {-50.0f, 0.0f, -50.0f},
    {50.0f, 100.0f, 50.0f},
};

daKiPot_HIO_c::daKiPot_HIO_c() {
    field_0x06 = 0x14;
}

void daKiPot_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
}

int daKiPot_c::create() {
    fopAcM_ct(this, daKiPot_c);

    setBaseMtx();

    f32 min_x = l_cull_box.min.x * scale.x;
    f32 min_y = l_cull_box.min.y * scale.y;
    f32 min_z = l_cull_box.min.z * scale.x;
    f32 max_x = l_cull_box.max.x * scale.x;
    f32 max_y = l_cull_box.max.y * scale.y;
    fopAcM_setCullSizeBox(this, min_x, min_y, min_z, max_x, max_y, max_y);

    field_0x575 = 0;
    init_modeWait();

    return cPhs_COMPLEATE_e;
}

int daKiPot_c::Execute() {
    procMain();
    attention_info.position = current.pos;
    attention_info.position.y += 150.0f;
    eyePos = current.pos;
    setBaseMtx();
    return 1;
}

void daKiPot_c::procMain() {
    typedef void (daKiPot_c::*modeProcessFunc)();
    static modeProcessFunc mode_proc[] = {
        &daKiPot_c::modeWait,
    };

    (this->*mode_proc[mMode])();
}

void daKiPot_c::init_modeWait() {
    mMode = 0;
}

void daKiPot_c::modeWait() {
    if (chkEvent() && eventInfo.checkCommandCatch() == 0) {
        dComIfGp_att_CatchRequest(this, fpcNm_ITEM_UGLY_SOUP, 100.0f, 50.0f, -50.0f, 0x2000, 1);
        eventInfo.onCondition(dEvtCnd_40_e);
    }
}

s32 daKiPot_c::chkEvent() {
    s32 result = 1;

    if (!dComIfGp_getEvent()->isOrderOK()) {
        result = 0;

        if (eventInfo.checkCommandCatch()) {
            return result;
        }
    }
    return result;
}

int daKiPot_c::Draw() {
    return 1;
}

int daKiPot_c::Delete() {
    return 1;
}

static int daKiPot_Draw(daKiPot_c* i_this) {
    return i_this->Draw();
}

static int daKiPot_Execute(daKiPot_c* i_this) {
    return i_this->Execute();
}

static int daKiPot_Delete(daKiPot_c* i_this) {
    return i_this->Delete();
}

static int daKiPot_Create(fopAc_ac_c* i_this) {
    return static_cast<daKiPot_c*>(i_this)->create();
}

static actor_method_class l_daKiPot_Method = {
    (process_method_func)daKiPot_Create,  (process_method_func)daKiPot_Delete,
    (process_method_func)daKiPot_Execute, 0,
    (process_method_func)daKiPot_Draw,
};

actor_process_profile_definition g_profile_Obj_KiPot = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_KiPot,          // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daKiPot_c),       // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    517,                     // mPriority
    &l_daKiPot_Method,       // sub_method
    0x00044000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
