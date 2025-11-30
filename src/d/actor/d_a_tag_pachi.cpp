#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_pachi.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

daTagPati_c::~daTagPati_c() {}

static dCcD_SrcCyl const l_sph_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xD8FBFDFF, 0x0}, 0x19}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x8},                 // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                 // mGObjTg
        {0x0},                                              // mGObjCo
    },                                                      // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        0.0f,                // mRadius
        0.0f                 // mHeight
    }                        // mCyl
};

int daTagPati_c::create() {
    fopAcM_ct(this, daTagPati_c);
    field_0x6E0 = get_prm();
    mStts.Init(0xFF, 0xFF, this);
    mCyl.Set(l_sph_src);
    mCyl.SetStts(&mStts);
    field_0x6E4 = 0;
    return cPhs_COMPLEATE_e;
}

int daTagPati_c::Execute() {
    col_set();
    field_0x6E4 = chk_CoHit();
    return 1;
}

void daTagPati_c::col_set() {
    mCyl.SetR(1350.f);
    mCyl.SetH(300000.0f);
    mCyl.SetC(home.pos);
    dComIfG_Ccsp()->Set(&mCyl);
}

u32 daTagPati_c::chk_CoHit() {
    return mCyl.ChkCoHit() ? 1 : 0;
}

static int daTagPati_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagPati_c*>(i_this)->create();
}

static int daTagPati_Execute(fopAc_ac_c* i_this) {
    return static_cast<daTagPati_c*>(i_this)->Execute();
}

static int daTagPati_Delete(daTagPati_c* i_this) {
    i_this->~daTagPati_c();
    return 1;
}

static actor_method_class l_daTagPati_Method = {
    (process_method_func)daTagPati_Create,
    (process_method_func)daTagPati_Delete,
    (process_method_func)daTagPati_Execute,
};

extern actor_process_profile_definition g_profile_TAG_PATI = {
    fpcLy_CURRENT_e,
    7,
    fpcLy_CURRENT_e,
    PROC_TAG_PATI,
    &g_fpcLf_Method.base,
    sizeof(daTagPati_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x173,
    &l_daTagPati_Method,
    0x40000,
    0,
    fopAc_CULLBOX_CUSTOM_e,
};
