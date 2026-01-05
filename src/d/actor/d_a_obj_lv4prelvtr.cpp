/**
* @file d_a_obj_lv4prelvtr.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4prelvtr.h"
#include "d/d_com_inf_game.h"

static char* l_arcName = "PRElvtr";

int daObjPRElvtr_c::create1st() {
    int phase = dComIfG_resLoad(this, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        setMtx();
        phase = MoveBGCreate(l_arcName, 7, dBgS_MoveBGProc_Trans, 0x3680, &mMtx1);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }
    return phase;
}

void daObjPRElvtr_c::setMtx() {
    PSMTXTrans(mDoMtx_stack_c::get(), current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    PSMTXCopy(mDoMtx_stack_c::get(), mMtx2);
    PSMTXCopy(mDoMtx_stack_c::get(), mMtx1);
}

int daObjPRElvtr_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    return mpModel != NULL ? 1 : 0;
}

static const cull_box l_cull_box = {
    {-725.0f, -315.0f, -725.0f},
    {725.0f, -15.0f, 725.0f},
};

int daObjPRElvtr_c::Create() {
    mpModel->setBaseTRMtx(mMtx2);
    fopAcM_SetMtx(this, mMtx2);
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z,
                          l_cull_box.max.x, l_cull_box.max.y, l_cull_box.max.z);
    return 1;
}

static fopAc_ac_c* searchObjSwTurn(void* i_actor1, void* i_actor2) {
    if (i_actor1 != NULL && fopAcM_IsActor(i_actor1) &&
        fopAcM_GetProfName(i_actor1) == PROC_Obj_SwTurn)
    {
        if (((daObjSwTurn_c*)i_actor1)->getSwNo() == ((daObjPRElvtr_c*)i_actor2)->getSwNo()) {
            return (fopAc_ac_c*)i_actor1;
        }
    }

    return NULL;
}

int daObjPRElvtr_c::Execute(Mtx** i_mtx) {
    daObjSwTurn_c* sw_turn =
        (daObjSwTurn_c*)fopAcM_Search((fopAcIt_JudgeFunc)searchObjSwTurn, this);

    if (sw_turn != NULL) {
        field_0x60c = sw_turn->getRotateAngle();
        f32 fVar1 = (field_0x60c / 16383.0f) * 900.0f;
        current.pos.y = home.pos.y - fVar1;
        sw_turn->current.pos.y = sw_turn->home.pos.y - fVar1;
    }

    setMtx();
    mpModel->setBaseTRMtx(mMtx2);
    *i_mtx = &mMtx1;

    return 1;
}

int daObjPRElvtr_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjPRElvtr_c::Delete() {
    dComIfG_resDelete(this, l_arcName);
    return 1;
}

static int daObjPRElvtr_create1st(daObjPRElvtr_c* i_this) {
    fopAcM_ct(i_this, daObjPRElvtr_c);
    return i_this->create1st();
}

static int daObjPRElvtr_MoveBGDelete(daObjPRElvtr_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjPRElvtr_MoveBGExecute(daObjPRElvtr_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjPRElvtr_MoveBGDraw(daObjPRElvtr_c* i_this) {
    return i_this->Draw();
}

static actor_method_class daObjPRElvtr_METHODS = {
    (process_method_func)daObjPRElvtr_create1st,     (process_method_func)daObjPRElvtr_MoveBGDelete,
    (process_method_func)daObjPRElvtr_MoveBGExecute, 0,
    (process_method_func)daObjPRElvtr_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_PRElvtr = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_PRElvtr,        // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjPRElvtr_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    665,                     // mPriority
    &daObjPRElvtr_METHODS,   // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
