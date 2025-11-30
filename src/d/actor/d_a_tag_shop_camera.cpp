/**
 * d_a_tag_shop_camera.cpp
 * Tag - Shop Camera
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_shop_camera.h"

int daTag_ShopCamera_c::create() {
    fopAcM_ct(this, daTag_ShopCamera_c);
    initialize();
    return cPhs_COMPLEATE_e;
}

int daTag_ShopCamera_c::Delete() {
    return 1;
}

int daTag_ShopCamera_c::Execute() {
    return 1;
}

int daTag_ShopCamera_c::Draw() {
    return 1;
}
 
void daTag_ShopCamera_c::initialize() {
    fopAcM_setCullSizeBox(this,-30.0f,-15.0f,-30.0f,30.0f,45.0f,30.0f);
    attention_info.flags = 0;
    current.angle.set(0,home.angle.y,0);
    shape_angle = current.angle;
}

static int daTag_ShopCamera_Create(void* i_this) {
    return static_cast<daTag_ShopCamera_c*>(i_this)->create();
}

static int daTag_ShopCamera_Delete(void* i_this) {
    return static_cast<daTag_ShopCamera_c*>(i_this)->Delete();
}

static int daTag_ShopCamera_Execute(void* i_this) {
    return static_cast<daTag_ShopCamera_c*>(i_this)->Execute();
}

static int daTag_ShopCamera_Draw(void* i_this) {
    return static_cast<daTag_ShopCamera_c*>(i_this)->Draw();
}

static int daTag_ShopCamera_IsDelete(void* param_0) {
    return 1;
}

static actor_method_class daTag_ShopCamera_MethodTable = {
    (process_method_func)daTag_ShopCamera_Create,
    (process_method_func)daTag_ShopCamera_Delete,
    (process_method_func)daTag_ShopCamera_Execute,
    (process_method_func)daTag_ShopCamera_IsDelete,
    (process_method_func)daTag_ShopCamera_Draw,
};

extern actor_process_profile_definition g_profile_TAG_SHOPCAM = {
    fpcLy_CURRENT_e,              // mLayerID
    7,                            // mListID
    fpcPi_CURRENT_e,              // mListPrio
    PROC_TAG_SHOPCAM,             // mProcName
    &g_fpcLf_Method.base,        // sub_method
    sizeof(daTag_ShopCamera_c),     // mSize
    0,                            // mSizeOther
    0,                            // mParameters
    &g_fopAc_Method.base,         // sub_method
    74,                           // mPriority
    &daTag_ShopCamera_MethodTable,  // sub_method
    0x64100,                      // mStatus
    5,                            // mActorType
    fopAc_CULLBOX_CUSTOM_e,       // cullType
};
