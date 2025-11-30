/**
 * d_a_obj_kkanban.cpp
 * Object - Kakariko Sign
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kkanban.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_procname.h"

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        s32 jointNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        daObjKKanban_c* userArea = (daObjKKanban_c*)model->getUserArea();
        if (jointNo == userArea->mJointIdx) {
            cMtx_copy(model->getAnmMtx(jointNo), mDoMtx_stack_c::get());
            mDoMtx_stack_c::XrotM(userArea->mRotX);
            mDoMtx_stack_c::ZrotM(userArea->mRotZ);
            model->setAnmMtx(jointNo, mDoMtx_stack_c::get());
            mDoMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
            MTXCopy(mDoMtx_stack_c::get(), userArea->getMoveBgMtx());
        }
    }
    return 1;
}

void daObjKKanban_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjKKanban_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static char* l_arcName = "H_KKanban";

static dCcD_SrcTri l_tri_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x20, 0x11}, 0x0}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},           // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},           // mGObjTg
        {0x0},                                        // mGObjCo
    },                                                // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}  // mTri
    }                                                                 // mTriAttr
};

int daObjKKanban_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    JUTNameTab* jointName = mpModel->getModelData()->getJointTree().getJointName();
    mJointIdx = -1;
    for (u16 i = 0; i < mpModel->getModelData()->getJointNum(); i++) {
        if (!strcmp(jointName->getName(i), "move")) {
            mJointIdx = i;
        }
    }
    mpModel->getModelData()->getJointNodePointer(mJointIdx)->setCallBack(nodeCallBack);
    mpModel->setUserArea((uintptr_t)this);
    fopAcM_setCullSizeBox(this, -950.0f, -2150.0f, -2000.0f, 100.0f, 50.0f, 800.0f);
    mStts.Init(0xff, 0xff, this);
    mTri.Set(l_tri_src);
    mTri.SetStts(&mStts);
    return 1;
}

void daObjKKanban_c::setTG() {
    cXyz tg[3];

    tg[0].set(0.0f, 0.0f, 0.0f);
    tg[1].set(0.0f, -500.0f, -500.0f);
    tg[2].set(0.0f, -1200.0f, 760.0f);
    mDoMtx_stack_c::copy(mBgMtx);
    for (int i = 0; i < 3; i++) {
        mDoMtx_stack_c::multVec(&tg[i], &tg[i]);
    }
    mTri.setPos(&tg[0], &tg[1], &tg[2]);
}

int daObjKKanban_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mpModel != NULL ? 1 : 0;
}

int daObjKKanban_c::create1st() {
    int phase = dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        phase = MoveBGCreate(l_arcName, 7, NULL, 0x1950, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }
    return phase;
}

int daObjKKanban_c::Execute(Mtx** i_mtx) {
    field_0x750++;
    mRotX = (field_0x74c + 50.0f) * cM_scos((field_0x74a + 1000) * field_0x750);
    mRotZ = (field_0x74c + 100.0f) * cM_ssin((field_0x74a + 1000) * field_0x750);
    cLib_addCalc0(&field_0x74c, 0.05f, 10.0f);
    cLib_addCalcAngleS2(&field_0x74a, 0, 0x14, 100);
    if (mTri.ChkTgHit() != 0) {
        field_0x74c = 2000.0f;
        field_0x74a = 0;
    }
    *i_mtx = &mBgMtx;
    setBaseMtx();
    setTG();
    dComIfG_Ccsp()->Set(&mTri);
    return 1;
}

int daObjKKanban_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjKKanban_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static int daObjKKanban_create1st(daObjKKanban_c* i_this) {
    fopAcM_ct(i_this, daObjKKanban_c);
    return i_this->create1st();
}

static int daObjKKanban_MoveBGDelete(daObjKKanban_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjKKanban_MoveBGExecute(daObjKKanban_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjKKanban_MoveBGDraw(daObjKKanban_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjKKanban_METHODS = {
    (process_method_func)daObjKKanban_create1st,     (process_method_func)daObjKKanban_MoveBGDelete,
    (process_method_func)daObjKKanban_MoveBGExecute, (process_method_func)NULL,
    (process_method_func)daObjKKanban_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_KKanban = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_KKanban,        // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daObjKKanban_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    548,                     // mPriority
    &daObjKKanban_METHODS,   // sub_method
    0x40100,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
