/**
 * d_a_obj_window.cpp
 * Object - Twilight Hyrule Castle Window Shutter
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_window.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static char* l_arcName = "J_KazeD";

static int const l_bck[3] = {5, 6, -1};

static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x11}, 0x79}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                  // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},                  // mGObjTg
        {0x0},                                               // mGObjCo
    },                                                       // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        40.0f,               // mRadius
        400.0f               // mHeight
    }                        // mCyl
};

void daObjWindow_c::initBaseMtx() {
    mpModel->calc();
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjWindow_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    if (mpBckAnm != NULL) {
        MTXCopy(mpModel->getAnmMtx(0), mBgMtx);
    } else {
        MTXCopy(mpModel->getBaseTRMtx(), mBgMtx);
    }
}

int daObjWindow_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -250.0f, 600.0f, -250.0f, 250.0f, 0.0f, 250.0f);
    if (mType == 1) {
        mStts.Init(0xff, 0xff, this);
        mCyl.Set(l_cyl_src);
        mCyl.SetStts(&mStts);
    }
    tevStr.mLightInf.r = getLightInf();
    return 1;
}

int daObjWindow_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 9);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }
    if (l_bck[mType] > 0) {
        J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, l_bck[mType]);
        mpBckAnm = new mDoExt_bckAnm();
        if (mpBckAnm == NULL || !mpBckAnm->init(anm, 1, 2, 1.0f, 0, -1, false)) {
            return 0;
        }
    }
    return 1;
}

cPhs__Step daObjWindow_c::create1st() {
    mType = getType();
    if (mType == 0xff) {
        mType = 0;
    }
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (step == cPhs_COMPLEATE_e) {
        step = (cPhs__Step)MoveBGCreate(l_arcName, 12, NULL, 0xee0, NULL);
        if (step == cPhs_ERROR_e) {
            return step;
        }
    }
    return step;
}

int daObjWindow_c::Execute(Mtx** i_mtx) {
    if (mpBckAnm != NULL) {
        mpBckAnm->play();
        f32 frame = mpBckAnm->getFrame();
        if (frame == 164.0f || frame == 757.0f) {
            fopAcM_seStart(this, Z2SE_OBJ_SPIRALSTEP_DOOR_BOUND, 0);
        }
        if (frame < 125.0f) {
            fopAcM_seStartLevel(this, Z2SE_OBJ_SPIRALSTEP_DOOR_1, 0);
        } else if (frame < 264.0f) {
            fopAcM_seStartLevel(this, Z2SE_OBJ_SPIRALSTEP_DOOR_2, 0);
        } else if (frame < 364.0f) {
            fopAcM_seStartLevel(this, Z2SE_OBJ_SPIRALSTEP_DOOR_3, 0);
        } else if (frame < 464.0f) {
            fopAcM_seStartLevel(this, Z2SE_OBJ_SPIRALSTEP_DOOR_4, 0);
        } else if (frame < 544.0f) {
            fopAcM_seStartLevel(this, Z2SE_OBJ_SPIRALSTEP_DOOR_5, 0);
        } else if (frame < 714.0f) {
            fopAcM_seStartLevel(this, Z2SE_OBJ_SPIRALSTEP_DOOR_6, 0);
        } else if (frame < 829.0f) {
            fopAcM_seStartLevel(this, Z2SE_OBJ_SPIRALSTEP_DOOR_7, 0);
        } else {
            fopAcM_seStartLevel(this, Z2SE_OBJ_SPIRALSTEP_DOOR_8, 0);
        }
    }
    *i_mtx = &mBgMtx;
    setBaseMtx();
    if (mType == 1) {
        cXyz center(-50.0f, 0.0f, -50.0f);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::multVec(&center, &center);
        mCyl.SetC(center);
        mCyl.SetR(80.0f);
        dComIfG_Ccsp()->Set(&mCyl);
    }
    return 1;
}

int daObjWindow_c::Draw() {
    g_env_light.settingTevStruct(0x14, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    if (mpBckAnm != NULL) {
        mpBckAnm->entry(mpModel->getModelData());
    }
    mDoExt_modelUpdateDL(mpModel);
    mDoExt_bckAnmRemove(mpModel->getModelData());
    dComIfGd_setList();
    return 1;
}

int daObjWindow_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static cPhs__Step daObjWindow_create1st(daObjWindow_c* i_this) {
    fopAcM_ct(i_this, daObjWindow_c);
    return i_this->create1st();
}

static int daObjWindow_MoveBGDelete(daObjWindow_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjWindow_MoveBGExecute(daObjWindow_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjWindow_MoveBGDraw(daObjWindow_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjWindow_METHODS = {
    (process_method_func)daObjWindow_create1st,
    (process_method_func)daObjWindow_MoveBGDelete,
    (process_method_func)daObjWindow_MoveBGExecute,
    (process_method_func)NULL,
    (process_method_func)daObjWindow_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_Window = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Window,
    &g_fpcLf_Method.base,
    sizeof(daObjWindow_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x1EB,
    &daObjWindow_METHODS,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
