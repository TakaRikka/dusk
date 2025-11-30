/**
 * d_a_obj_pleaf.cpp
 * Object - Pumpkin Leaf
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_pleaf.h"

static char* l_resName = "J_Hatake";

daObj_Pleaf_c::~daObj_Pleaf_c() {
    dComIfG_resDelete(&mPhaseReq, getResName());
}

daObj_Pleaf_Param_c::params const daObj_Pleaf_Param_c::m = {0, -3.0f, 1.0f, 900.0f};

int daObj_Pleaf_c::create() {
    fopAcM_ct(this, daObj_Pleaf_c);
    mType = getType();
    int phase = dComIfG_resLoad(&mPhaseReq, getResName());
    if (phase == cPhs_COMPLEATE_e) {
        if (isDelete()) {
            return cPhs_ERROR_e;
        }
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x810)) {
            return cPhs_ERROR_e;
        }
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 50.0f, 300.0f);
        mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                     fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this),
                     fopAcM_GetShapeAngle_p(this));
        mStts.Init(0xFF, 0, this);
        cXyz new_pos = current.pos;
        new_pos.y += 50.0f;
        mGndChk.SetPos(&new_pos);
        mGroundDist = dComIfG_Bgsp().GroundCross(&mGndChk);
        if (mGroundDist != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }
        Execute();
    }
    return phase;
}

int daObj_Pleaf_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(getResName(), 3);
    if (modelData == NULL) {
        return 0;
    }
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel != NULL) {
        return 1;
    }
    return 0;
}

int daObj_Pleaf_c::Delete() {
    this->~daObj_Pleaf_c();
    return 1;
}

int daObj_Pleaf_c::Execute() {
    cXyz new_pos = current.pos;
    new_pos.y += 50.0f;
    mGndChk.SetPos(&new_pos);
    mGroundDist = dComIfG_Bgsp().GroundCross(&mGndChk);
    if (mGroundDist != -G_CM3D_F_INF) {
        setEnvTevColor();
        setRoomNo();
    }
    setMtx();
    attention_info.position = current.pos;
    eyePos = attention_info.position;
    attention_info.flags = 0;
    return 1;
}

int daObj_Pleaf_c::Draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);
    if (mGroundDist != -G_CM3D_F_INF) {
        mShadowKey =
            dComIfGd_setShadow(mShadowKey, 1, mpModel, &current.pos,
                               daObj_Pleaf_Param_c::m.field_0xc, 20.0f, current.pos.y, mGroundDist,
                               mGndChk, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }
    return 1;
}

int daObj_Pleaf_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daObj_Pleaf_c*>(i_this)->CreateHeap();
}

char* daObj_Pleaf_c::getResName() {
    return l_resName;
}

int daObj_Pleaf_c::isDelete() {
    switch (mType) {
    case 0:
        return 0;
    default:
        return 0;
    }
}

void daObj_Pleaf_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daObj_Pleaf_c::setRoomNo() {
    fopAcM_SetRoomNo(this, dComIfG_Bgsp().GetRoomId(mGndChk));
}

void daObj_Pleaf_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static int daObj_Pleaf_Create(void* i_this) {
    return static_cast<daObj_Pleaf_c*>(i_this)->create();
}

static int daObj_Pleaf_Delete(void* i_this) {
    return static_cast<daObj_Pleaf_c*>(i_this)->Delete();
}

static int daObj_Pleaf_Execute(void* i_this) {
    return static_cast<daObj_Pleaf_c*>(i_this)->Execute();
}

static int daObj_Pleaf_Draw(void* i_this) {
    return static_cast<daObj_Pleaf_c*>(i_this)->Draw();
}

static int daObj_Pleaf_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daObj_Pleaf_MethodTable = {
    (process_method_func)daObj_Pleaf_Create,  (process_method_func)daObj_Pleaf_Delete,
    (process_method_func)daObj_Pleaf_Execute, (process_method_func)daObj_Pleaf_IsDelete,
    (process_method_func)daObj_Pleaf_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_PLEAF = {
    fpcLy_CURRENT_e,           // mLayerID
    8,                         // mListID
    fpcPi_CURRENT_e,           // mListPrio
    PROC_OBJ_PLEAF,            // mProcName
    &g_fpcLf_Method.base,     // sub_method
    sizeof(daObj_Pleaf_c),     // mSize
    0,                         // mSizeOther
    0,                         // mParameters
    &g_fopAc_Method.base,      // sub_method
    713,                       // mPriority
    &daObj_Pleaf_MethodTable,  // sub_method
    0x44100,                   // mStatus
    fopAc_ACTOR_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,    // cullType
};

static daObj_Pleaf_Param_c l_HIO;
