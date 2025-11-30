//
// d_a_obj_lv3saka00.cpp
// Object: lv3saka00 (Lakebed Temple)
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv3saka00.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

void daObjLv3saka_c::initBaseMtx() {
    setBaseMtx();
}

void daObjLv3saka_c::setBaseMtx() {
    MTXTrans(mDoMtx_stack_c::now, current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_YrotM(mDoMtx_stack_c::get(), shape_angle.y);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjLv3saka_c::Create() {
    if (fopAcM_isSwitch(this, getSwbit())) {
        if (mpBgW->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgW);
        }
        dComIfG_Bgsp().Regist(mpBgW2, this);
        mpBgW2->Move();
    }

    initBaseMtx();

    if (fopAcM_isSwitch(this, getSwbit())) {
        mpBgW2->Move();
    }

    return 1;
}

static int const l_dzbIdx[] = {3, 3};

static int const l_dzbIdx2[] = {4, 4};

static char* l_arcName[] = {"Obj_saka", "Obj_saka2"};

int daObjLv3saka_c::CreateHeap() {
    mpBgW2 = new dBgW();
    if (mpBgW2 == NULL ||
        mpBgW2->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName[getType()], l_dzbIdx2[getType()]), 1,
                    &mBgMtx))
    {
        mpBgW2 = NULL;
        return 0;
    }

    return 1;
}

static int const l_heap_size[] = {0x00004E00, 0x00002900};

int daObjLv3saka_c::create1st() {
    int phase = dComIfG_resLoad(&mPhase, l_arcName[getType()]);
    if (phase == cPhs_COMPLEATE_e) {
        phase = MoveBGCreate(l_arcName[getType()], l_dzbIdx[getType()], dBgS_MoveBGProc_TypicalRotY,
                             l_heap_size[getType()], NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }
    return phase;
}

int daObjLv3saka_c::Execute(Mtx** param_0) {
    if (fopAcM_isSwitch(this, getSwbit())) {
        setWtrDzb();
    } else {
        setNrmDzb();
    }

    *param_0 = &mBgMtx;
    setBaseMtx();

    return 1;
}

void daObjLv3saka_c::setNrmDzb() {
    if (!mpBgW->ChkUsed()) {
        if (mpBgW2->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgW2);
        }
        dComIfG_Bgsp().Regist(mpBgW, this);
        mpBgW->Move();
    }
}

void daObjLv3saka_c::setWtrDzb() {
    if (!mpBgW2->ChkUsed()) {
        if (mpBgW->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgW);
        }
        dComIfG_Bgsp().Regist(mpBgW2, this);
        mpBgW2->Move();
    }
}

int daObjLv3saka_c::Draw() {
    return 1;
}

int daObjLv3saka_c::Delete() {
    if (mpBgW2 != NULL && mpBgW2->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW2);
    }
    dComIfG_resDelete(&mPhase, l_arcName[getType()]);
    return 1;
}

static int daObjLv3saka_create1st(daObjLv3saka_c* i_this) {
    fopAcM_ct(i_this, daObjLv3saka_c);
    return i_this->create1st();
}

static int daObjLv3saka_MoveBGDelete(daObjLv3saka_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjLv3saka_MoveBGExecute(daObjLv3saka_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjLv3saka_MoveBGDraw(daObjLv3saka_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjLv3saka_METHODS = {
    (process_method_func)daObjLv3saka_create1st, (process_method_func)daObjLv3saka_MoveBGDelete,
    (process_method_func)daObjLv3saka_MoveBGExecute, (process_method_func)NULL,
    (process_method_func)daObjLv3saka_MoveBGDraw};

extern actor_process_profile_definition g_profile_Obj_Lv3R10Saka = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Lv3R10Saka,     // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daObjLv3saka_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    528,                     // mPriority
    &daObjLv3saka_METHODS,   // sub_method
    0x40100,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
