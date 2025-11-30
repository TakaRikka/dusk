/**
 * @file d_a_obj_crvsteel.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_crvsteel.h"
#include "d/d_com_inf_game.h"

static int daObjCRVSTEEL_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjCRVSTEEL_c*>(i_this)->create();
}

static int daObjCRVSTEEL_Delete(daObjCRVSTEEL_c* i_this) {
    i_this->MoveBGDelete();
    return 1;
}

void daObjCRVSTEEL_c::OpenSet(f32 param_1, f32 param_2) {
    speed.y = param_1;
    mMaxHeight = current.pos.y + param_2;
    mAction = 1;
}

void daObjCRVSTEEL_c::CloseSet(f32 param_1) {
    speed.y = param_1;
    mAction = 2;
}

void daObjCRVSTEEL_c::CloseExecute() {
    current.pos.y += speed.y;
    if (current.pos.y < mMinHeight) {
        current.pos.y = mMinHeight;
        fopAcM_effSmokeSet1(&mSmokeParticle1, &mSmokeParticle2, &current.pos, 0, 5.0f, &tevStr, 1);
        mAction = 0;
    }
}

void daObjCRVSTEEL_c::OpenExecute() {
    current.pos.y += speed.y;
    if (current.pos.y > mMaxHeight) {
        current.pos.y = mMaxHeight;
    }
}

void daObjCRVSTEEL_c::Action() {
    switch(mAction) {
    case 0:
        break;
    case 1:
        OpenExecute();
        break;
    case 2:
        CloseExecute();
        break;
    }
}

void daObjCRVSTEEL_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
}

static int daObjCRVSTEEL_Draw(daObjCRVSTEEL_c* i_this) {
   return i_this->Draw();
}

static int daObjCRVSTEEL_Execute(daObjCRVSTEEL_c* i_this) {
    return i_this->MoveBGExecute();
}

static char* l_arcName = "CrvSteel";

int daObjCRVSTEEL_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "U_CrvSteelGate.bmd");
    JUT_ASSERT(86, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mModel != NULL ? TRUE : FALSE;
}

int daObjCRVSTEEL_c::create() {
    fopAcM_ct(this, daObjCRVSTEEL_c);
    int rv = dComIfG_resLoad(&mPhase, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        gravity = -9.0f;
        int dzb_id = dComIfG_getObjctResName2Index(l_arcName, "U_CrvSteelGate.dzb");
        if (dzb_id == -1) {
            // "dzb data not found!<%s>"
            OS_REPORT("dzbデータが見つかりませんでした!<%s>\n\n", l_arcName);
            JUT_ASSERT(443, dzb_id != -1);
        }
        rv = MoveBGCreate(l_arcName, dzb_id, dBgS_MoveBGProc_TypicalRotY, 0x4000, NULL);
        if (rv == cPhs_ERROR_e) {
            return rv;
        }
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                            &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
        fopAcM_SetMtx(this, mModel->getBaseTRMtx());
        cXyz cStack_30(current.pos.x, current.pos.y + 100.0f,
                        current.pos.z);
        if (fopAcM_gc_c::gndCheck(&cStack_30)) {
            current.pos.y = fopAcM_gc_c::getGroundY();
            mMinHeight = current.pos.y;
        }
        fopAcM_setCullSizeBox(this, -400.0f, 0.0f, -50.0f, 400.0f, 900.0f, 50.0f);
        daObjCRVSTEEL_Execute(this);
    }
    return rv;
}

static int daObjCRVSTEEL_IsDelete(daObjCRVSTEEL_c* param_0) {
    return 1;
}

int daObjCRVSTEEL_c::Create() {
    fopAcM_setCullSizeBox(this, -1000.0f, -500.0f, -1000.0f, 1000.0f, 500.0, 1000.0f);
    return cPhs_COMPLEATE_e;
}

int daObjCRVSTEEL_c::Execute(Mtx** param_1) {
    Action();
    *param_1 = &mBgMtx;
    setBaseMtx();
    return 1;
}

int daObjCRVSTEEL_c::Draw() {
    g_env_light.settingTevStruct(8, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    return 1;
}

int daObjCRVSTEEL_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static actor_method_class l_daObjCRVSTEEL_Method = {
    (process_method_func)daObjCRVSTEEL_Create,
    (process_method_func)daObjCRVSTEEL_Delete,
    (process_method_func)daObjCRVSTEEL_Execute,
    (process_method_func)daObjCRVSTEEL_IsDelete,
    (process_method_func)daObjCRVSTEEL_Draw,
};

extern actor_process_profile_definition g_profile_Obj_CRVSTEEL = {
  fpcLy_CURRENT_e,         // mLayerID
  3,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_Obj_CRVSTEEL,       // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daObjCRVSTEEL_c), // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  469,                     // mPriority
  &l_daObjCRVSTEEL_Method, // sub_method
  0x00040180,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
