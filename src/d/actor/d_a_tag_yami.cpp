/**
 * d_a_tag_yami.cpp
 * Tag - Yami (Twili)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_yami.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"

const static dCcD_SrcCyl l_sph_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x0}, 0x19}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x8},                 // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                 // mGObjTg
        {0x0},                                              // mGObjCo
    },                                                      // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            0.0f,                // mRadius
            0.0f                 // mHeight
        }  // mCyl
    }
};

int daTagYami_c::create() {
    fopAcM_ct(this, daTagYami_c);

        /* dSv_event_flag_c::F_0570 - Palace of Twilight - Cleared Palace of Twilight */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[570])) {
        return cPhs_ERROR_e;
    }

    OS_REPORT("-------------- TAG YAMI CREATE!!\n");
    mStts.Init(0xff, 0xff, this);
    mCyl.Set(l_sph_src);
    mCyl.SetStts(&mStts);

    mIsCoHit = 0;
    mMTagSw = get_MTagSw();
    mMTagSw2 = get_MTagSw2();

    switch (get_prm()) {
    case 2: {
        cXyz coC;
        calcCoC(coC);
        setCoC(coC);
        setCoR(700.0f);
        setCoH(300.0f);

        if (fopAcM_isSwitch(this, 0x3d) && fopAcM_isSwitch(this, 0x3e)) {
            off_CoHit();
        } else {
            on_CoHit();
        }
        break;
    }
    default:
        return cPhs_ERROR_e;
    }

    return cPhs_COMPLEATE_e;
}

int daTagYami_c::Execute() {
    col_set();
    mIsCoHit = chk_CoHit();
    return 1;
}

void daTagYami_c::calcCoC(cXyz& i_dst) {
    cXyz base(0.0f, 0.0f, 430.0f);
    mDoMtx_stack_c::ZXYrotS(csXyz(0, -0x7f96, 0));
    mDoMtx_stack_c::multVec(&base, &base);
    i_dst = home.pos + base;
}

void daTagYami_c::col_set() {
    /* empty function */
}

u32 daTagYami_c::chk_CoHit() {
    if (!mCoHitValid) {
        return 0;
    }

    f32 coR = getCoR();
    f32 plDist = getPlDist();
    return plDist < coR;
}

f32 daTagYami_c::getPlDist() {
    cXyz coC(mCoC);
    daPy_py_c* py = daPy_getPlayerActorClass();
    cXyz pyPos = fopAcM_GetPosition((fopAc_ac_c*)py);
    cXyz diff = pyPos - coC;
    return diff.absXZ();
}

void daTagYami_c::onMidnaTagSw() {
    fopAcM_onSwitch(this, mMTagSw);
}

void daTagYami_c::offMidnaTagSw2() {
    OS_REPORT("-----------off mtag2 sw!!\n");
    fopAcM_offSwitch(this, mMTagSw2);
}

int daTagYami_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagYami_c*>(i_this)->create();
}

int daTagYami_Execute(fopAc_ac_c* i_this) {
    return static_cast<daTagYami_c*>(i_this)->Execute();
}

int daTagYami_Delete(daTagYami_c* i_this) {
    i_this->~daTagYami_c();
    return 1;
}

static actor_method_class l_daTagYami_Method = {
    (process_method_func)daTagYami_Create,
    (process_method_func)daTagYami_Delete,
    (process_method_func)daTagYami_Execute,
};

actor_process_profile_definition g_profile_TAG_YAMI = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_TAG_YAMI,           // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daTagYami_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    21,                      // mPriority
    &l_daTagYami_Method,     // sub_method
    0x40000,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
