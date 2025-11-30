/**
 * d_a_obj_tgake.cpp
 * Howling Cliff
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_tgake.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

void daObjGake_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjGake_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjGake_c::Create() {
    mEventBit1 = getEventBit1();
    mEventBit2 = getEventBit2();
    eventInfo.setIdx(getType());

    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());

    MoveBGExecute();
    return 1;
}

static char* l_arcName = "A_TGake";

int daObjGake_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daObjGake_c::create1st() {
    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        phase = MoveBGCreate(l_arcName, 7, dBgS_MoveBGProc_TypicalRotY, 0x4000, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }

    return phase;
}

int daObjGake_c::Execute(Mtx** param_0) {
    if ((mEventBit2 != 0x3FF &&
         dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[mEventBit2])) ||
        (mEventBit1 != 0x3FF &&
         !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[mEventBit1])))
    {
        if (mpBgW != NULL && mpBgW->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgW);
        }

        mHide = true;
        return 1;
    }

    if (mEventBit1 != 0x3FF && dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[mEventBit1])) {
        if (mpBgW != NULL && !mpBgW->ChkUsed()) {
            dComIfG_Bgsp().Regist(mpBgW, this);
        }

        mHide = false;
        return 1;
    }

    *param_0 = &mBgMtx;
    return 1;
}

int daObjGake_c::Draw() {
    if (mHide) {
        return 1;
    }

    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    return 1;
}

int daObjGake_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjGake_create1st(daObjGake_c* i_this) {
    fopAcM_ct(i_this, daObjGake_c);

    return i_this->create1st();
}

static int daObjGake_MoveBGDelete(daObjGake_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjGake_MoveBGExecute(daObjGake_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjGake_MoveBGDraw(daObjGake_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjGake_METHODS = {
    (process_method_func)daObjGake_create1st,     (process_method_func)daObjGake_MoveBGDelete,
    (process_method_func)daObjGake_MoveBGExecute, (process_method_func)NULL,
    (process_method_func)daObjGake_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_Gake = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Gake,
    &g_fpcLf_Method.base,
    sizeof(daObjGake_c),
    0,
    0,
    &g_fopAc_Method.base,
    568,
    &daObjGake_METHODS,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
