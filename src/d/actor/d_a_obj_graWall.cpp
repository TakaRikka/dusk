/**
 * d_a_obj_graWall.cpp
 * Object - Goron Adult Wall (Deleted)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_graWall.h"
#include "d/d_procname.h"

int daObjGraWall_c::Create() {
    fopAcM_ct(this, daObjGraWall_c);
    field_0x6e0 = fopAcM_GetParam(this);

    if (field_0x6e0 != 0xff) {
        if (dComIfGs_isSwitch(field_0x6e0, fopAcM_GetRoomNo(this)) != 0) {
            return cPhs_ERROR_e;
        }
    } else {
            /* dSv_event_flag_c::M_029 - Death Mountain (room) - Win wrestle match against Gor Coron */
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[62])) {
            return cPhs_ERROR_e;
        }
    }
    col_init();

    return cPhs_COMPLEATE_e;
}

int daObjGraWall_c::Execute() {
    if (field_0x6e0 != 0xff) {
        if (dComIfGs_isSwitch(field_0x6e0, fopAcM_GetRoomNo(this)) != 0) {
            fopAcM_delete(this);
            return 1;
        }
    } else {
            /* dSv_event_flag_c::M_029 - Death Mountain (room) - Win wrestle match against Gor Coron */
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[62])) {
            fopAcM_delete(this);
            return 1;
        }
    }
    col_set();
    return 1;
}

int daObjGraWall_c::Delete() {
    return 1;
}

static const f32 l_DATA[2] = {180.0f, 280.0f};

const static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x19}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x8},          // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},          // mGObjTg
        {0x0},                                       // mGObjCo
    },                                               // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        0.0f,                // mRadius
        0.0f                 // mHeight
    }                        // mCyl
};

void daObjGraWall_c::col_init() {
    mStts.Init(0xff, 0xff, this);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mStts);
}

void daObjGraWall_c::col_set() {
    mCyl.SetC(current.pos);
    mCyl.SetH(l_DATA[1]);
    mCyl.SetR(l_DATA[0]);
    dComIfG_Ccsp()->Set(&mCyl);
}

static int daObjGraWall_Execute(daObjGraWall_c* i_this) {
    return i_this->Execute();
}

static int daObjGraWall_IsDelete(daObjGraWall_c* i_this) {
    return 1;
}

static int daObjGraWall_Delete(daObjGraWall_c* i_this) {
    return i_this->Delete();
}

static int daObjGraWall_create(fopAc_ac_c* i_this) {
    return static_cast<daObjGraWall_c*>(i_this)->Create();
}

static actor_method_class l_daObjGraWall_Method = {
    (process_method_func)daObjGraWall_create,
    (process_method_func)daObjGraWall_Delete,
    (process_method_func)daObjGraWall_Execute,
    (process_method_func)daObjGraWall_IsDelete,
    (process_method_func)NULL,
};

extern actor_process_profile_definition g_profile_GRA_WALL = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_GRA_WALL,
    &g_fpcLf_Method.base,
    sizeof(daObjGraWall_c),
    0,
    0,
    &g_fopAc_Method.base,
    611,
    &l_daObjGraWall_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
