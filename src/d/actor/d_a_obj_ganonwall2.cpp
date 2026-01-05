/**
 * @file d_a_obj_ganonwall2.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_ganonwall2.h"
#include "d/d_com_inf_game.h"

static u8 const l_idx[24][4] = {
    0x17, 0x05, 0x06, 0x01, 0x17, 0x05, 0x06, 0x02, 0x17, 0x05, 0x06, 0x03, 0x17, 0x05, 0x06, 0x04,
    0x17, 0x05, 0x06, 0x05, 0x05, 0x06, 0x01, 0x00, 0x06, 0x0B, 0x05, 0x00, 0x06, 0x0B, 0x05, 0x01,
    0x06, 0x0B, 0x05, 0x02, 0x06, 0x0B, 0x05, 0x03, 0x06, 0x0B, 0x05, 0x04, 0x0B, 0x11, 0x06, 0x00,
    0x0B, 0x11, 0x06, 0x01, 0x0B, 0x11, 0x06, 0x02, 0x0B, 0x11, 0x06, 0x03, 0x0B, 0x11, 0x06, 0x04,
    0x0B, 0x11, 0x06, 0x05, 0x11, 0x13, 0x02, 0x00, 0x11, 0x13, 0x02, 0x01, 0x13, 0x17, 0x04, 0x00,
    0x13, 0x17, 0x04, 0x01, 0x13, 0x17, 0x04, 0x02, 0x13, 0x17, 0x04, 0x03, 0x17, 0x05, 0x06, 0x00,
};

static const GXColor l_color[24] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x96, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x64, 0x64, 0x32, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x64, 0x64, 0x64, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x78, 0x78, 0x78, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xB4, 0xB4, 0xB4, 0xFF,
};

static char* l_arcName = "V_CTGWall";

static char* l_matName = "mat00";

void daObjGWall2_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjGWall2_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(this->shape_angle.y);
    MTXCopy(mDoMtx_stack_c::get(), mpModel->mBaseTransformMtx);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjGWall2_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    // cullSizeFar = 1000000.f;
    mMatIdx = 0xffff;
    JUTNameTab* name_table = mpModel->getModelData()->getMaterialTable().getMaterialName();
    for (u16 i = 0; i < mpModel->getModelData()->getMaterialTable().getMaterialNum(); i++) {
        if (strcmp(name_table->getName(i), l_matName) == 0) {
            mMatIdx = i;
        }
    }
    mSePos.set(0.0f, 800.0f, -10372.5f);
    return 1;
}

int daObjGWall2_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    mpModel = mDoExt_J3DModel__create(model_data, 0, 0x11000284);
    if (mpModel == NULL) {
        return 0;
    }
    J3DAnmTextureSRTKey* btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 8);
    mpBtkAnm = new mDoExt_btkAnm();
    if (mpBtkAnm == NULL ||
        mpBtkAnm->init(model_data, btk, 1, 2, 1.0f, 0, -1) == 0)
    {
        return 0;
    }
    return 1;
}

int daObjGWall2_c::create1st() {
    if (getEventBit1() != 0x3FF &&
        dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[(u16)getEventBit1()]) != 0)
    {
        return cPhs_ERROR_e;
    }
    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        phase = (cPhs__Step)MoveBGCreate(l_arcName, 11, NULL, 0x6100, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }
    return phase;
}

int daObjGWall2_c::Execute(Mtx** i_bgMtx) {
    mpBtkAnm->play();
    *i_bgMtx = &mBgMtx;
    setBaseMtx();
    mDoAud_seStartLevel(Z2SE_OBJ_HYRULE_BARRIER, &mSePos, 0, 0);
    return 1;
}

int daObjGWall2_c::Draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    for (u16 i = 0; i < mpModel->getModelData()->getMaterialTable().getMaterialNum(); i++) {
        J3DMaterial* mat = mpModel->getModelData()->getMaterialTable().getMaterialNodePointer(i);
        J3DFog* fog = mat->getPEBlock()->getFog();
        if (fog != NULL) {
            fog = mat->getPEBlock()->getFog();
            J3DFogInfo* fog_info = fog->getFogInfo();
            fog_info->mColor.r = 0;
            fog_info->mColor.g = 0;
            fog_info->mColor.b = 0;
            fog_info->mStartZ = 1000.0f;
            fog_info->mEndZ = 250000.0f;
        }
    }

    int hour = dKy_getdaytime_hour();
    f32 minute = dKy_getdaytime_minute();
    J3DGXColor* mat_tev_k_color =
        mpModel->getModelData()->getMaterialTable().getMaterialNodePointer(mMatIdx)->getTevKColor(1);
    int idx1 = l_idx[hour][0];
    int idx2 = l_idx[hour][1];
    mat_tev_k_color->r = (l_color[idx1].r +
                          (l_color[idx2].r - l_color[idx1].r) *
                              ((minute + l_idx[hour][3] * 60.0f) / ((l_idx[hour][2] + 1) * 60.0f)));
    mat_tev_k_color->g = (l_color[idx1].g +
                          (l_color[idx2].g - l_color[idx1].g) *
                              ((minute + l_idx[hour][3] * 60.0f) / ((l_idx[hour][2] + 1) * 60.0f)));
    mat_tev_k_color->b = (l_color[idx1].b +
                          (l_color[idx2].b - l_color[idx1].b) *
                              ((minute + l_idx[hour][3] * 60.0f) / ((l_idx[hour][2] + 1) * 60.0f)));
    mat_tev_k_color->a = (l_color[idx1].a +
                          (l_color[idx2].r - l_color[idx1].a) *
                              ((minute + l_idx[hour][3] * 60.0f) / ((l_idx[hour][2] + 1) * 60.0f)));

    dComIfGd_setListBG();
    mpBtkAnm->entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    mDoExt_btkAnmRemove(mpModel->getModelData());
    dComIfGd_setList();
    return 1;
}

int daObjGWall2_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static int daObjGWall2_create1st(daObjGWall2_c* i_this) {
    fopAcM_ct(i_this, daObjGWall2_c);
    return i_this->create1st();
}

static int daObjGWall2_MoveBGDelete(daObjGWall2_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjGWall2_MoveBGExecute(daObjGWall2_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjGWall2_MoveBGDraw(daObjGWall2_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjGWall2_METHODS = {
    (process_method_func)daObjGWall2_create1st,
    (process_method_func)daObjGWall2_MoveBGDelete,
    (process_method_func)daObjGWall2_MoveBGExecute, 
    0,
    (process_method_func)daObjGWall2_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_GanonWall2 = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_GanonWall2,     // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjGWall2_c),   // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    580,                     // mPriority
    &daObjGWall2_METHODS,    // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
