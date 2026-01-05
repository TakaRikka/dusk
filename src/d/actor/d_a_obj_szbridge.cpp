/**
 * @file d_a_obj_szbridge.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_obj_szbridge.h"

static void rideCallback(dBgW* param_0, fopAc_ac_c* param_1, fopAc_ac_c* param_2) {
    static_cast<daObjSZbridge_c*>(param_1)->rideActor(param_2);
}

static const cull_box l_cull_box = {
    {-310.0f, -30.0f, -760.0f},
    {310.0f, 360.0f, 760.0f},
};

void daObjSZbridge_c::rideActor(fopAc_ac_c* i_actor) {
    cXyz pos = i_actor->current.pos;
    pos.y = mWaterY;
    if ((!cM3d_IsZero(i_actor->speed.getSquareMag()) && cM_rndF(1.0f) < 0.3f) || field_0x620 == 0) {
        fopAcM_effHamonSet(&field_0x618[0], &pos, 7.0f, 0.04f);
        speed.y -= 0.25f;
    }
    field_0x620 = 10;
}

static const char* l_arcName = "S_Zbridge";

int daObjSZbridge_c::create1st() {
    int rv = dComIfG_resLoad(this, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        setMtx();
        rv = MoveBGCreate(l_arcName, 7, dBgS_MoveBGProc_Typical, 0x11190, &mBgMtx);
        if (rv == cPhs_ERROR_e) {
            return rv;
        }
    }
    return rv;
}

void daObjSZbridge_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    MTXCopy(mDoMtx_stack_c::get(), field_0x5d8);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjSZbridge_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mpModel != NULL ? 1 : 0;
}

void daObjSZbridge_c::chkBg() {
    cXyz chk_pos;

    dBgS_ObjGndChk(gnd_chk);

    chk_pos = current.pos;
    chk_pos.y += 700.0f;

    gnd_chk.SetPos(&chk_pos);
    gnd_chk.SetActorPid(base.base.id);
    mGroundY = dComIfG_Bgsp().GroundCross(&gnd_chk);
    chk_pos.y = mGroundY;

    f32 posY = current.pos.y + 700.0f;
    dBgS_WtrChk(wtr_chk);
    wtr_chk.Set(chk_pos, posY);

    if (dComIfG_Bgsp().WaterChk(&wtr_chk)) {
        mWaterY = wtr_chk.GetHeight();
    } else {
        mWaterY = -G_CM3D_F_INF;
    }

    if (mWaterY > mGroundY) {
        speed.y -= 0.05f * (current.pos.y - (mWaterY - 70.0f));
        speed.y *= 0.985f;
        current.pos.y += speed.y;

        if (current.pos.y > 5.0f + (mWaterY - 70.0f)) {
            current.pos.y = 5.0f + (mWaterY - 70.0f);
        }

        if (current.pos.y < (mWaterY - 70.0f) - 5.0f) {
            current.pos.y = (mWaterY - 70.0f) - 5.0f;
        }

        speed.y = current.pos.y - old.pos.y;
        field_0x614 = 1;
    } else {
        current.pos.y = mGroundY;
        field_0x614 = 0;
    }
}

int daObjSZbridge_c::Create() {
    mpBgW->SetRideCallback(rideCallback);
    chkBg();
    speed.y = 0;

    mpModel->setBaseTRMtx(field_0x5d8);
    fopAcM_SetMtx(this, field_0x5d8);
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x,
                          l_cull_box.max.y, l_cull_box.max.z);

    for (int i = 0; i < 2; i++) {
        field_0x618[i] = -1;
    }

    field_0x620 = 0;

    return 1;
}

int daObjSZbridge_c::Execute(Mtx** param_0) {
    chkBg();

    if (field_0x620 != 0) {
        field_0x620--;
    }

    setMtx();
    mpModel->setBaseTRMtx(field_0x5d8);

    *param_0 = &mBgMtx;

    return 1;
}

int daObjSZbridge_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    return 1;
}

int daObjSZbridge_c::Delete() {
    dComIfG_resDelete(this, l_arcName);
    return 1;
}

static int daObjSZbridge_create1st(daObjSZbridge_c* i_this) {
    fopAcM_ct(i_this, daObjSZbridge_c);
    return i_this->create1st();
}

static int daObjSZbridge_MoveBGDelete(daObjSZbridge_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjSZbridge_MoveBGExecute(daObjSZbridge_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjSZbridge_MoveBGDraw(daObjSZbridge_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjSZbridge_METHODS = {
    (process_method_func)daObjSZbridge_create1st,
    (process_method_func)daObjSZbridge_MoveBGDelete,
    (process_method_func)daObjSZbridge_MoveBGExecute,
    0,
    (process_method_func)daObjSZbridge_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_SZbridge = {
    fpcLy_CURRENT_e,          // mLayerID
    3,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_Obj_SZbridge,        // mProcName
    &g_fpcLf_Method.base,     // sub_method
    sizeof(daObjSZbridge_c),  // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    676,                      // mPriority
    &daObjSZbridge_METHODS,   // sub_method
    0x00040100,               // mStatus
    fopAc_ACTOR_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
};
