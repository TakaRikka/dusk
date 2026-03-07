/**
 * d_a_obj_toaru_maki.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_toaru_maki.h"
#include "d/d_bg_w.h"

static int daObjToaruMaki_c_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjToaruMaki_c*>(i_this)->createHeap();
}

static char* l_arcName[2] = {"T_Maki1", "T_Maki2"};

daObjToaruMaki_c::daObjToaruMaki_c() {
    mpBgW = NULL;
    mpModel = NULL;
}

daObjToaruMaki_c::~daObjToaruMaki_c() {
    dComIfG_resDelete(this, l_arcName[getNameArg_0()]);
}

static const u32 l_bmdIdx[2] = {4, 3};

int daObjToaruMaki_c::createHeap() {
    u32 nameArg = getNameArg_0();
    J3DModelData* a_model_data_p =
        (J3DModelData*)dComIfG_getObjectRes(l_arcName[nameArg], l_bmdIdx[nameArg]);
    JUT_ASSERT(101, a_model_data_p != NULL);
    mpModel = mDoExt_J3DModel__create(a_model_data_p, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }
    if (nameArg == 0) {
        mpBgW = new dBgW();
        if (mpBgW == NULL) {
            return 0;
        }
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        MTXCopy(mDoMtx_stack_c::get(), mMtx);
        cBgD_t* cBgD = (cBgD_t*)dComIfG_getObjectRes(l_arcName[nameArg], 7);
        if (mpBgW->Set(cBgD, 1, &mMtx) == 1) {
            return 0;
        }
    }
    return 1;
}

static const u32 l_heapSize[2] = {
    0x2000,
    0x0EB0,
};

int daObjToaruMaki_c::create() {
    fopAcM_ct(this, daObjToaruMaki_c);
    u32 nameArg = getNameArg_0();
    int rv = dComIfG_resLoad(this, l_arcName[nameArg]);
    if (rv == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, daObjToaruMaki_c_createHeap, l_heapSize[nameArg]) == 0) {
            return cPhs_ERROR_e;
        }
        mDoExt_setupStageTexture(mpModel->getModelData());
        if (nameArg == 0) {
            if (dComIfG_Bgsp().Regist(mpBgW, this) != 0) {
                return cPhs_ERROR_e;
            }
        }
        setModelMtx();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    }
    return rv;
}

int daObjToaruMaki_c::Delete() {
    if (getNameArg_0() == 0 && mpBgW != NULL && mpBgW->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW);
    }
    this->~daObjToaruMaki_c();
    return 1;
}

void daObjToaruMaki_c::setModelMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daObjToaruMaki_c::draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setList();
    mDoExt_modelUpdateDL(mpModel);
    return 1;
}

int daObjToaruMaki_c::execute() {
    setModelMtx();
    return 1;
}

static int daObjToaruMaki_create(daObjToaruMaki_c* i_this) {
    // !@bug this setup is called twice. No adverse effects though.
    fopAcM_ct(i_this, daObjToaruMaki_c);
    return i_this->create();
}

static int daObjToaruMaki_Delete(daObjToaruMaki_c* i_this) {
    return i_this->Delete();
}

static int daObjToaruMaki_execute(daObjToaruMaki_c* i_this) {
    return i_this->execute();;
}

static int daObjToaruMaki_draw(daObjToaruMaki_c* i_this) {
    return i_this->draw();
}

static actor_method_class daObjToaruMaki_METHODS = {
    (process_method_func)daObjToaruMaki_create,
    (process_method_func)daObjToaruMaki_Delete,
    (process_method_func)daObjToaruMaki_execute,
    NULL,
    (process_method_func)daObjToaruMaki_draw,
};

actor_process_profile_definition g_profile_Obj_ToaruMaki = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_Obj_ToaruMaki,      // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daObjToaruMaki_c),// mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  749,                     // mPriority
  &daObjToaruMaki_METHODS, // sub_method
  0x00040000,              // mStatus
  fopAc_ENV_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
