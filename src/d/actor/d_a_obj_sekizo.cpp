/**
 * d_a_obj_sekizo.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_sekizo.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"

static struct {
    u32 bmdIdx;
    u32 resIdx;
} l_bmdData[1] = {4, 1};

static struct {
    u32 dzbIdx;
    u32 resIdx;
} l_dzbData[1] = {7, 1};

static char* l_resNameList[2] = {"", "Sekizo"};

cPhs__Step daObj_Sekizo_c::create() {
    fopAcM_ct(this, daObj_Sekizo_c);

    field_0x5b0 = 0;
    cPhs__Step step =
        (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_resNameList[l_bmdData[field_0x5b0].resIdx]);
    if (step == cPhs_COMPLEATE_e) {
        step = (cPhs__Step)MoveBGCreate(l_resNameList[l_dzbData[field_0x5b0].resIdx],
                                        l_dzbData[field_0x5b0].dzbIdx, dBgS_MoveBGProc_TypicalRotY,
                                        0x4000, NULL);
        if (step == cPhs_ERROR_e) {
            return step;
        }
    }
    return step;
}

int daObj_Sekizo_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(
        l_resNameList[l_bmdData[field_0x5b0].resIdx], l_bmdData[field_0x5b0].bmdIdx);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return false;
    }

    return true;
}

int daObj_Sekizo_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    field_0x5b2 = false;
    field_0x5b1 = true;
    return 1;
}

int daObj_Sekizo_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_resNameList[l_bmdData[field_0x5b0].resIdx]);
    return 1;
}

int daObj_Sekizo_c::Execute(Mtx** i_mtx) {
    if (home.roomNo == dComIfGp_roomControl_getStayNo()) {
        *i_mtx = &mBgMtx;
        setBaseMtx();
        if (field_0x5b2) {
            if (field_0x5b1) {
                dComIfG_Bgsp().Release(mpBgW);
                field_0x5b1 = false;
            }
        } else if (!field_0x5b1) {
            dComIfG_Bgsp().Regist(mpBgW, this);
            field_0x5b1 = true;
        }
    }
    return 1;
}

int daObj_Sekizo_c::Draw() {
    if (!field_0x5b2) {
        g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_setList();
    }
    return 1;
}

void daObj_Sekizo_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObj_Sekizo_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
}

static int daObj_Sekizo_Create(void* i_this) {
    return static_cast<daObj_Sekizo_c*>(i_this)->create();
}

static int daObj_Sekizo_Delete(void* i_this) {
    return static_cast<daObj_Sekizo_c*>(i_this)->MoveBGDelete();
}

static int daObj_Sekizo_Execute(void* i_this) {
    return static_cast<daObj_Sekizo_c*>(i_this)->MoveBGExecute();
}

static int daObj_Sekizo_Draw(void* i_this) {
    return static_cast<daObj_Sekizo_c*>(i_this)->MoveBGDraw();
}

static int daObj_Sekizo_IsDelete(void* i_this) {
    return 1;
}

static u8 lit_3800[12];

static daObj_Sekizo_Param_c l_HIO;

static actor_method_class daObj_Sekizo_MethodTable = {
    (process_method_func)daObj_Sekizo_Create, (process_method_func)daObj_Sekizo_Delete,
    (process_method_func)daObj_Sekizo_Execute, (process_method_func)daObj_Sekizo_IsDelete,
    (process_method_func)daObj_Sekizo_Draw};

actor_process_profile_definition g_profile_OBJ_SEKIZO = {
    fpcLy_CURRENT_e,            // mLayerID
    7,                          // mListID
    fpcPi_CURRENT_e,            // mListPrio
    PROC_OBJ_SEKIZO,            // mProcName
    &g_fpcLf_Method.base,      // sub_method
    sizeof(daObj_Sekizo_c),     // mSize
    0,                          // mSizeOther
    0,                          // mParameters
    &g_fopAc_Method.base,       // mSubMtd
    88,                         // mPriority
    &daObj_Sekizo_MethodTable,  // mSubMtd
    0x44000,                    // status
    fopAc_ACTOR_e,              // mActorType
    fopAc_CULLBOX_CUSTOM_e,     // cullType
};
