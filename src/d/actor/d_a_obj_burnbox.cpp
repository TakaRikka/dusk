/**
* @file d_a_obj_burnbox.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_burnbox.h"
#include "d/d_com_inf_game.h"

void daObjBurnBox_c::initBaseMtx() {
    mpBoxModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjBurnBox_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpBoxModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

const u32 l_bmd[3] = {
    0x04,
    0x04,
    0x03,
};

const u32 l_heap_size[3] = {
    0x0820,
    0x0C00,
    0x0820,
};

const dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x1e}, {0x0, 0x1e}, 0x79}}, // mObj
        {dCcD_SE_NONE, 0x1, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            40.0f, // mRadius
            100.0f // mHeight
        } // mCyl
    }
};

int daObjBurnBox_c::Create() {
    static const u16 particle_id[5] = {
        0x8526, 0x8527, 0x8528, 0x8529, 0x8746,
    };

    initBaseMtx();
    fopAcM_SetMtx(this, mpBoxModel->getBaseTRMtx());
    mStts.Init(0xff, 0xff, this);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mStts);
    fopAcM_setCullSizeBox2(this, mpBoxModel->getModelData());
    cXyz particlePos = current.pos;
    cXyz particleScale = scale;
    int type = getType();

    switch (type) {
    case 0:
    case 2:
        particlePos.y += 37.5f;
        particleScale.setall(0.5f);
        break;
    case 1:
        particlePos.y += 75.0f;
        break;
    }

    for (int index = 0; index < 5; index++) {
        mpEmitters[index] = dComIfGp_particle_set(particle_id[index], &particlePos, NULL, &particleScale, 0xff,
            NULL, -1, NULL, NULL, NULL);
    }
    return 1;
}

const char* l_arcName[3] = {
    "Kkiba_00", // crate
    "Kkiba_00",
    "J_taru00", // barrel
};

int daObjBurnBox_c::CreateHeap() {
    u8 type = getType();
    J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes(l_arcName[type], l_bmd[type]);
    JUT_ASSERT(0x106, modelData != NULL);

    mpBoxModel = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
    if (mpBoxModel == NULL) {
        return 0;
    }
    return 1;
}

cPhs__Step daObjBurnBox_c::create1st() {
    cPhs__Step phaseStep = (cPhs__Step) dComIfG_resLoad(&mPhaseReq, l_arcName[getType()]);
    if (phaseStep == cPhs_COMPLEATE_e) {
        int dzb_id = -1;
        u8 type = getType();

        switch (type) {
        case 1:
            dzb_id = 7;
        }

        phaseStep = (cPhs__Step) MoveBGCreate(l_arcName[type], dzb_id, dBgS_MoveBGProc_TypicalRotY,
                                             l_heap_size[type], NULL);
        if (phaseStep == cPhs_ERROR_e) {
            return phaseStep;
        }
    }
    return phaseStep;
}

int daObjBurnBox_c::Execute(Mtx** mtx) {
    field_0x724++;
    if (field_0x724 % 3 == 0) {
        field_0x728--;
        if (field_0x728 == -20) {
            fopAcM_delete(this);
        }
    }

    *mtx = &mBgMtx;
    setBaseMtx();

    switch (getType()) {
    case 0:
    case 2:
        mCyl.SetC(current.pos);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    return 1;
}

int daObjBurnBox_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpBoxModel, &tevStr);
    dComIfGd_setListBG();

    J3DModelData* boxModelData = mpBoxModel->getModelData();
    for (u16 index = 0; index < boxModelData->getMaterialNum(); index++) {
        J3DGXColorS10* tevColor = boxModelData->getMaterialNodePointer(index)->getTevColor(0);
        tevColor->r = field_0x728;
        tevColor->g = field_0x728;
        tevColor->b = field_0x728;
    }
    mDoExt_modelUpdateDL(mpBoxModel);
    for (u16 index = 0; index < boxModelData->getMaterialNum(); index++) {
        J3DGXColorS10* tevColor = boxModelData->getMaterialNodePointer(index)->getTevColor(0);
        tevColor->r = 0;
        tevColor->g = 0;
        tevColor->b = 0;
    }
    dComIfGd_setList();
    return 1;
}

int daObjBurnBox_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName[getType()]);
    return 1;
}

int daObjBurnBox_create1st(daObjBurnBox_c* i_this) {
    fopAcM_ct(i_this, daObjBurnBox_c);
    return i_this->create1st();
}

int daObjBurnBox_MoveBGDelete(daObjBurnBox_c* i_this) {
    return i_this->MoveBGDelete();
}

int daObjBurnBox_MoveBGExecute(daObjBurnBox_c* i_this) {
    return i_this->MoveBGExecute();
}

int daObjBurnBox_MoveBGDraw(daObjBurnBox_c* i_this) {
    return i_this->MoveBGDraw();
}

actor_method_class daObjBurnBox_METHODS = {
    (process_method_func) daObjBurnBox_create1st,     (process_method_func)daObjBurnBox_MoveBGDelete,
    (process_method_func) daObjBurnBox_MoveBGExecute, NULL,
    (process_method_func) daObjBurnBox_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_BurnBox = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_BurnBox,        // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjBurnBox_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    532,                     // mPriority
    &daObjBurnBox_METHODS,   // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
