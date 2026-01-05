/**
* @file d_a_obj_hakai_ftr.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_hakai_ftr.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"

static int daObjHFtr_c_createHeap(fopAc_ac_c* param_0) {
    return ((daObjHFtr_c*)param_0)->createHeap();
}

static const char* l_arcName[3] = {"HChair", "HSofa", "HTable"};

daObjHFtr_c::daObjHFtr_c() {}

daObjHFtr_c::~daObjHFtr_c() {
    u32 index = getNameId();
    dComIfG_resDelete(this, l_arcName[index]);
}

static u32 const l_bmdIdx[3] = {4, 4, 4};

static u32 const l_dzbIdx[3] = {7, 7, 7};

int daObjHFtr_c::createHeap() {
    uint nameId = getNameId();

    J3DModelData* a_model_data_p =
        (J3DModelData*)dComIfG_getObjectRes(l_arcName[nameId], l_bmdIdx[nameId]);
    JUT_ASSERT(0xb2, a_model_data_p != NULL);

    mpModel = mDoExt_J3DModel__create(a_model_data_p, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    field_0x5a4 = new dBgW();
    if (field_0x5a4 == NULL) {
        return 0;
    }

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    MTXCopy(mDoMtx_stack_c::get(), mMtx);
    if (field_0x5a4->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName[nameId], l_dzbIdx[nameId]), 1,
                         &mMtx) == 1)
    {
        return 0;
    }

    return 1;
}

static u32 const l_heapSize[3] = {0x2000, 0x2000, 0x2000};

int daObjHFtr_c::create() {
    fopAcM_ct(this, daObjHFtr_c);

    u32 nameIndex = getNameId();
    s32 loadResult = dComIfG_resLoad(this, l_arcName[nameIndex]);
    if (loadResult == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, daObjHFtr_c_createHeap, l_heapSize[nameIndex])) {
            return cPhs_ERROR_e;
        }

        if (dComIfG_Bgsp().Regist(field_0x5a4, this) != 0) {
            return cPhs_ERROR_e;
        }

        init();
        setModelMtx();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    }

    return loadResult;
}

int daObjHFtr_c::Delete() {
    if (field_0x5a4 != NULL && field_0x5a4->ChkUsed()) {
        dComIfG_Bgsp().Release(field_0x5a4);
    }
    this->~daObjHFtr_c();
    return 1;
}

int daObjHFtr_c::draw() {
    g_env_light.settingTevStruct(8, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(this->mpModel);
    dComIfGd_setList();

    return 1;
}

dCcD_SrcCyl const daObjHFtr_c::s_CcDCyl = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x400020, 0x11}, 0x0}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},               // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},               // mGObjTg
        {0x0},                                            // mGObjCo
    },   
    {                                                 // mObjInf
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            0.0f,                // mRadius
            0.0f                 // mHeight
        }  // mCyl
    }
};

int daObjHFtr_c::execute() {
    if (chkHit()) {
        callEmt();

        s32 nameId = getNameId();
        Z2SoundID soundId = nameId == 1 ? Z2SE_OBJ_BREAK_WOOD_FURNITURE : Z2SE_OBJ_BREAK_BARREL;
        fopAcM_seStartCurrent(this, soundId, 0);

        fopAcM_delete(this);
        return 1;
    }

    setModelMtx();
    for (s32 i = 0; i < 2; i++) {
        dComIfG_Ccsp()->Set(&mCyls[i]);
    }
    return 1;
}

void daObjHFtr_c::init() {
    static s32 const COL_OFFSET_X[3][2] = {{0, 0}, {50, -50}, {0, 0}};
    static s32 const COL_HEIGHT[3] = {170, 160, 110};
    static s32 const COL_RADIUS[3] = {60, 70, 90};

    u32 nameId = getNameId();
    mStts.Init(0xff, 0, this);
    for (s32 i = 0; i < 2; i++) {
        mCyls[i].Set(s_CcDCyl);
        mCyls[i].SetStts(&mStts);
        cXyz cStack_48(current.pos.x + COL_OFFSET_X[nameId][i], current.pos.y, current.pos.z);
        mCyls[i].SetC(cStack_48);
        mCyls[i].SetH(COL_HEIGHT[nameId]);
        mCyls[i].SetR(COL_RADIUS[nameId]);
    }
    // return iVar2;
}

void daObjHFtr_c::setModelMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

bool daObjHFtr_c::chkHit() {
    for (s32 i = 0; i < 2; i++) {
        if (mCyls[i].ChkTgHit()) {
            return true;
        }
    }
    return false;
}


void daObjHFtr_c::callEmt() {
    static u16 const PARTICLE_NAME[4] = {
        0x8AE7,
        0x8AE8,
        0x8AE9,
        0x8AEA,
    };

    cXyz acStack_24(current.pos.x, current.pos.y, current.pos.z);
    csXyz cStack_38(current.angle);
    cXyz cStack_30(1.0f, 1.0f, 1.0f);
    for (s32 i = 0; i < 4; i++) {
        dComIfGp_particle_set(PARTICLE_NAME[i], &acStack_24, &cStack_38, &cStack_30);
    }
}

static int daObjHFtr_create(daObjHFtr_c* param_0) {
    fopAcM_ct(param_0, daObjHFtr_c);
    return param_0->create();
}

static int daObjHFtr_Delete(daObjHFtr_c* param_0) {
    return param_0->Delete();
}

static int daObjHFtr_execute(daObjHFtr_c* param_0) {
    return param_0->execute();
}

static int daObjHFtr_draw(daObjHFtr_c* param_0) {
    return param_0->draw();
}

static actor_method_class daObjHFtr_METHODS = {
    (process_method_func)daObjHFtr_create,  (process_method_func)daObjHFtr_Delete,
    (process_method_func)daObjHFtr_execute, 0,
    (process_method_func)daObjHFtr_draw,
};

actor_process_profile_definition g_profile_Obj_HFtr = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_HFtr,           // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjHFtr_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    744,                     // mPriority
    &daObjHFtr_METHODS,      // sub_method
    0x00040180,              // mStatus
    fopAc_ENV_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
